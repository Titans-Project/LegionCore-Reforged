////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
///      Based on TrinityCore code       ///
///   Copyright 2008-2012 TrinityCore    ///
///   <http://www.trinitycore.org/>      ///
///   Copyright 2005-2009 MaNGOS         ///
///   <http://getmangos.com/>            ///
///                                      ///
////////////////////////////////////////////

#include "BattlefieldMgr.h"
#include "BattlegroundDefines.h"
#include "BattlegroundMgr.h"
#include "Chat.h"
#include "DisableMgr.h"
#include "GameTables.h"
#include "NPCPackets.h"
#include "OutdoorPvPMgr.h"
#include "WowTime.hpp"
#include "GameEventMgr.h"
#include "LFGMgr.h"
#include <atomic>

void WorldSession::HandleBattlemasterHello(WorldPackets::NPC::Hello& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Creature* unit = player->GetNPCIfCanInteractWith(packet.Unit, UNIT_NPC_FLAG_BATTLEMASTER);
    if (!unit)
        return;

    if (!unit->isBattleMaster())                             // it's not battlemaster
        return;

    // Stop the npc if moving
    {
        uint32 pause = unit->GetMovementTemplate().GetInteractionPauseTimer();
        unit->PauseMovement(pause);
        unit->SetHomePosition(unit->GetPosition());
    }

    uint16 bgTypeId = sBattlegroundMgr->GetBattleMasterBG(unit->GetEntry());

    if (!player->GetBGAccessByLevel(bgTypeId))
    {
        // temp, must be gossip message...
        SendNotification(LANG_YOUR_BG_LEVEL_REQ_ERROR);
        return;
    }

    sBattlegroundMgr->SendBattlegroundList(player, packet.Unit, bgTypeId);
}

void WorldSession::HandleBattlemasterJoin(WorldPackets::Battleground::Join& packet)
{
    uint16 const queueID = packet.QueueID - MS::Battlegrounds::QueueOffsets::Battleground;
    JoinBattlegroundQueue(queueID, packet.RolesMask, packet.JoinAsGroup, packet.BlacklistMap);
}

// Shared by the battlemaster and by the A'shran button of the PvP frame, which reaches the queue through the dungeon finder opcodes
void WorldSession::JoinBattlegroundQueue(uint16 queueID, uint8 rolesMask, bool joinAsGroup, WorldPackets::Battleground::IgnorMapInfo const& blacklistMap)
{
    Player* player = GetPlayer();

    if (!sBattlemasterListStore.LookupEntry(queueID))
    {
        TC_LOG_ERROR("network", "Battleground: invalid bgtype (%u) received. possible cheater? player guid %u", queueID, _player->GetGUIDLow());
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, queueID))
    {
        ChatHandler(this).PSendSysMessage(LANG_BG_DISABLED);
        return;
    }

    if (player->InBattleground())
        return;

    // Arenas, rated battlegrounds and brawls have their own join opcodes, with the checks this path lacks
    uint8 bgQueueTypeId = MS::Battlegrounds::GetBgQueueTypeIdByBgTypeID(queueID);
    bool const normalQueue = (bgQueueTypeId >= MS::Battlegrounds::BattlegroundQueueTypeId::BattlegroundAlteracValley && bgQueueTypeId <= MS::Battlegrounds::BattlegroundQueueTypeId::BattlegroundRandom)
        || bgQueueTypeId == MS::Battlegrounds::BattlegroundQueueTypeId::BattlegroundAshran;
    if (!normalQueue || queueID == MS::Battlegrounds::BattlegroundTypeId::BattlegroundRatedEyeOfTheStorm)
        return;

    Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(queueID);
    if (!bg || bg->IsArena())
        return;

    PVPDifficultyEntry const* bracketEntry = sDB2Manager.GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
    if (!bracketEntry)
        return;

    if (!joinAsGroup )
    {
        if (player->isUsingLfg())
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_LFG_CANT_USE_BATTLEGROUND);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        if (!player->CanJoinToBattleground(MS::Battlegrounds::IternalPvpTypes::Battleground))
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        if (player->GetBattlegroundQueueIndex(MS::Battlegrounds::GetBgQueueTypeIdByBgTypeID(MS::Battlegrounds::BattlegroundTypeId::BattlegroundRandom)) < PLAYER_MAX_BATTLEGROUND_QUEUES)
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_IN_RANDOM_BG);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        if (player->InBattlegroundQueue() && queueID == MS::Battlegrounds::BattlegroundTypeId::BattlegroundRandom)
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_IN_NON_RANDOM_BG);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        if (player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
            return;

        if (!player->HasFreeBattlegroundQueueId())
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_BATTLEGROUND_TOO_MANY_QUEUES);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        player->SetQueueRoleMask(bracketEntry->RangeIndex, rolesMask);

        BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
        uint32 joinTime = 0;
        uint32 avgWaitTime = 0;
        uint8 joinType = 0;
        {
            // the world thread may free the entry: read it under the queue lock
            std::lock_guard<std::recursive_mutex> queueGuard(bgQueue.GetLock());
            GroupQueueInfo* ginfo = bgQueue.AddGroup(player, nullptr, queueID, bracketEntry, 0, false, false, blacklistMap);
            joinTime = ginfo->JoinTime;
            joinType = ginfo->JoinType;
            avgWaitTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->RangeIndex);
        }

        WorldPackets::Battleground::BattlefieldStatusQueued queued;
        sBattlegroundMgr->BuildBattlegroundStatusQueued(&queued, bg, player, player->AddBattlegroundQueueId(bgQueueTypeId), joinTime, avgWaitTime, joinType, false);
        SendPacket(queued.Write());
    }
    else
    {
        Group* grp = player->GetGroup();
        if (!grp)
            return;

        if (grp->GetLeaderGUID() != player->GetGUID())
            return;

        ObjectGuid errorGuid;
        auto err = grp->CanJoinBattlegroundQueue(bg, bgQueueTypeId, 0, false, 0, errorGuid);

        if (!err)
        {
            sLFGMgr->InitiBattlgroundCheckRoles(grp, player->GetGUID(), queueID, rolesMask, bgQueueTypeId, blacklistMap);
            return;
        }

        // the members stand on other maps: each gets his status in his own thread
        for (auto const& slot : grp->GetMemberSlots())
        {
            ObjectAccessor::PostToPlayer(slot.Guid, [bg, err, errorGuid](Player* member) -> void
            {
                WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
                sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, member, 0, err, &errorGuid);
                member->SendDirectMessage(battlefieldStatus.Write());
            });
        }

        return;
    }

    sBattlegroundMgr->ScheduleQueueUpdate(new QueueSchedulerItem(0, 0, bgQueueTypeId, queueID, bracketEntry->RangeIndex, Roles(rolesMask), bracketEntry->MinLevel));
}

void WorldSession::HandlePVPLogData(WorldPackets::Battleground::NullCmsg& /*packet*/)
{
    Player* player = GetPlayer();
    Battleground* bg = player->GetBattleground();
    if (!bg)
        return;

    if (bg->IsArena())
        return;

    WorldPackets::Battleground::PVPLogData pvpLogData;
    bg->BuildPvPLogDataPacket(pvpLogData);
    SendPacket(pvpLogData.Write());
}

void WorldSession::HandleBattlefieldList(WorldPackets::Battleground::ListClient& packet)
{
    if (sBattlemasterListStore.LookupEntry(packet.ListID))
        sBattlegroundMgr->SendBattlegroundList(GetPlayer(), ObjectGuid::Empty, uint16(packet.ListID));
}

const uint32 DisableSpecs[]
{
    SPEC_PALADIN_HOLY,
    SPEC_PALADIN_PROTECTION,

    SPEC_WARRIOR_PROTECTION,

    SPEC_DRUID_BEAR,
    SPEC_DRUID_RESTORATION,

    SPEC_DK_BLOOD,

    SPEC_PRIEST_DISCIPLINE,
    SPEC_PRIEST_HOLY,

    SPEC_SHAMAN_RESTORATION,

    SPEC_MONK_MISTWEAVER,
    SPEC_MONK_BREWMASTER,

    SPEC_DEMON_HUNER_VENGEANCE
};

static auto Arena1v1CheckTalents = [](Player* player) -> bool // Return false, if player have tank/heal spec
{
    if (!player)
        return false;

/*
    if (player->getLevel() < 110)
        return false;

    for (auto curSpec : DisableSpecs)
    {
        if (player->GetUInt32Value(PLAYER_FIELD_CURRENT_SPEC_ID) == curSpec)
        {
            ChatHandler(player).PSendSysMessage("You can't use tank/heal spec");
            return false;
        }
    }*/

    return true;
};

void WorldSession::HandleBattleFieldPort(WorldPackets::Battleground::Port& packet)
{
    if (packet.Ticket.Id >= PLAYER_MAX_BATTLEGROUND_QUEUES)   // an index: MAX itself read past the array
        return;

    Player* player = GetPlayer();
    if (!player->InBattlegroundQueue())
        return;

    uint8 bgQueueTypeId = player->GetBattlegroundQueueTypeId(packet.Ticket.Id);
    if (bgQueueTypeId == MS::Battlegrounds::BattlegroundQueueTypeId::None)
        return;

    if (bgQueueTypeId > MS::Battlegrounds::BattlegroundQueueTypeId::Max)
        return;

    uint16 bgTypeId = MS::Battlegrounds::GetBgTemplateIdByQueueTypeID(bgQueueTypeId);
    if (bgQueueTypeId == MS::Battlegrounds::BattlegroundQueueTypeId::Brawl)
        bgTypeId = sBattlegroundMgr->GetAtiveBrawlBgTypeId();


    if (bgTypeId == MS::Battlegrounds::BattlegroundTypeId::None || bgTypeId >= MS::Battlegrounds::BattlegroundTypeId::Max)
        return;

    BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
    GroupQueueInfo ginfo;
    if (!bgQueue.GetPlayerGroupInfoData(player->GetGUID(), &ginfo))
        return;

    if (!ginfo.IsInvitedToBGInstanceGUID && packet.AcceptedInvite)
        return;

    uint16 const bgLookupTypeId = MS::Battlegrounds::IsRandomGeneratedBg(bgTypeId) ? MS::Battlegrounds::BattlegroundTypeId::None : ginfo.NativeBgTypeId;
    uint16 const bgTemplateTypeId = bgTypeId;
    Battleground* bg = sBattlegroundMgr->GetBattleground(ginfo.IsInvitedToBGInstanceGUID, bgLookupTypeId);
    if (!bg && !packet.AcceptedInvite)
        bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTemplateTypeId);
    if (!bg)
        return;

    // get real bg type
    bgTypeId = bg->GetTypeID();

    PVPDifficultyEntry const* bracketEntry = sDB2Manager.GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
    if (!bracketEntry)
        return;

    if (ginfo.JoinType == 2 || ginfo.JoinType == 3)
    {
        auto group = player->GetGroup();
        if (group && (!group->GetMaxCountOfRolesForArenaQueue(ROLES_HEALER) || !group->GetMaxCountOfRolesForArenaQueue(ROLES_TANK)))
            return;
    }

    if (packet.AcceptedInvite && (ginfo.JoinType == 0))
    {
        if (!player->CanJoinToBattleground(bg->IsArena() || bg->IsSkirmish() ? MS::Battlegrounds::IternalPvpTypes::Arena : MS::Battlegrounds::IternalPvpTypes::Battleground))
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            SendPacket(battlefieldStatus.Write());

            packet.AcceptedInvite = false;
        }

        if (player->getLevel() > bg->GetMaxLevel())
            packet.AcceptedInvite = false;
    }
    
    if (bg->GetStatus() == STATUS_WAIT_LEAVE) // if bg just end, then leave
        packet.AcceptedInvite = false;

    if (packet.AcceptedInvite)
    {
        if (!player->IsInvitedForBattlegroundQueueType(bgQueueTypeId))
            return;

        if ((bgQueueTypeId == MS::Battlegrounds::BattlegroundQueueTypeId::Arena1v1) && !Arena1v1CheckTalents(player))
        {
            ChatHandler(player).PSendSysMessage("You can't join on this specialization!");
            return;
        }

        if (bgQueueTypeId == MS::Battlegrounds::BattlegroundQueueTypeId::ArenaSoloQ3v3 && ginfo.index && (ginfo.index != (player->GetRoleForSoloQ() + MS::Battlegrounds::QueueGroupTypes::NormalHorde)))
        {
            switch (ginfo.index)
            {
                case 4:
                    ChatHandler(player).PSendSysMessage("You can't join on this specialization! Needed melee spec!");
                    break;
                case 5:
                    ChatHandler(player).PSendSysMessage("You can't join on this specialization! Needed range spec!");
                    break;
                case 6:
                    ChatHandler(player).PSendSysMessage("You can't join on this specialization! Needed heal spec!");
                    break;
                default:
                    break;
            }
            return;
        }

        if (!player->InBattleground())
            player->SetBattlegroundEntryPoint();

        if (!player->IsAlive())
        {
            player->ResurrectPlayer(1.0f);
            player->SpawnCorpseBones();
        }

        if (player->isInFlight())
        {
            player->GetMotionMaster()->MovementExpired();
            player->CleanupAfterTaxiFlight();
        }

        WorldPackets::Battleground::BattlefieldStatusActive battlefieldStatus;
        sBattlegroundMgr->BuildBattlegroundStatusActive(&battlefieldStatus, bg, player, packet.Ticket.Id, player->GetBattlegroundQueueJoinTime(bgQueueTypeId), ginfo.JoinType);
        player->SendDirectMessage(battlefieldStatus.Write());

        bgQueue.RemovePlayer(player->GetGUID(), false);

        if (Battleground* currentBg = player->GetBattleground())
            currentBg->RemovePlayerAtLeave(player->GetGUID(), false, true);

        player->SetBattlegroundId(bg->GetInstanceID(), bgTypeId);
        player->SetBGTeam(ginfo.Team);
        sBattlegroundMgr->SendToBattleground(player, ginfo.IsInvitedToBGInstanceGUID, bgTypeId);
    }
    else // leave queue
    {
        bool const arenaDeserter = bg->IsArena() && bg->IsRated() && bg->GetJoinType() != MS::Battlegrounds::JoinType::Arena1v1
            && (bg->GetStatus() == STATUS_WAIT_JOIN || bg->GetStatus() == STATUS_IN_PROGRESS);

        // Runs in the leaving player's own thread; the battleground is looked up again, the world thread may free it meanwhile
        auto leaveQueue = [instanceGuid = ginfo.IsInvitedToBGInstanceGUID, bgLookupTypeId, bgTemplateTypeId, bgQueueTypeId, ticketId = packet.Ticket.Id, arenaDeserter](Player* member) -> void
        {
            Battleground* leftBg = sBattlegroundMgr->GetBattleground(instanceGuid, bgLookupTypeId);
            if (!leftBg)
                leftBg = sBattlegroundMgr->GetBattlegroundTemplate(bgTemplateTypeId);

            if (leftBg)
            {
                WorldPackets::Battleground::BattlefieldStatusNone none;
                sBattlegroundMgr->BuildBattlegroundStatusNone(&none, member, ticketId, member->GetBattlegroundQueueJoinTime(bgQueueTypeId));
                member->SendDirectMessage(none.Write());

                WorldPackets::Battleground::BattlefieldStatusFailed failed;
                sBattlegroundMgr->BuildBattlegroundStatusFailed(&failed, leftBg, member, ticketId, MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_LEAVE_QUEUE);
                member->SendDirectMessage(failed.Write());
            }

            if (arenaDeserter)
                member->SendOperationsAfterDelay(OAD_ARENA_DESERTER);

            member->RemoveBattlegroundQueueId(bgQueueTypeId);  // must be called this way, because if you move this call to queue->removeplayer, it causes bugs
        };

        // The leader takes out the members queued with him; anyone else leaves alone
        Group* group = player->GetGroup();
        if (group && group->GetLeaderGUID() == player->GetGUID())
        {
            for (auto const& slot : group->GetMemberSlots())
            {
                if (!ginfo.Players.count(slot.Guid))
                    continue;

                // the member stands on another map
                if (ObjectAccessor::PostToPlayer(slot.Guid, leaveQueue))
                    bgQueue.RemovePlayer(slot.Guid, true);
            }
        }
        else
        {
            leaveQueue(player);
            bgQueue.RemovePlayer(player->GetGUID(), true);
        }

        if (!ginfo.JoinType)
            sBattlegroundMgr->ScheduleQueueUpdate(new QueueSchedulerItem(ginfo.MatchmakerRating, ginfo.JoinType, bgQueueTypeId, bgTypeId, bracketEntry->RangeIndex));
    }
}

void WorldSession::HandleLeaveBattlefield(WorldPackets::Battleground::NullCmsg& /*packet*/)
{
    // not allow leave battleground in combat
    if (_player->isInCombat())
        if (Battleground* bg = _player->GetBattleground())
            if (bg->GetStatus() != STATUS_WAIT_LEAVE)
                return;

    _player->LeaveBattleground();
}

void WorldSession::HandleBattlefieldStatus(WorldPackets::Battleground::NullCmsg& /*packet*/)
{
    Player* player = GetPlayer();

    for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
    {
        uint8 bgQueueTypeId = player->GetBattlegroundQueueTypeId(i);
        if (!bgQueueTypeId)
            continue;

        uint8 joinType = MS::Battlegrounds::GetBgJoinTypeByQueueTypeID(bgQueueTypeId);

        auto bg = player->GetBattleground();
        if (bg && bg->GetJoinType() == joinType)
        {
            WorldPackets::Battleground::BattlefieldStatusActive battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusActive(&battlefieldStatus, bg, player, i, player->GetBattlegroundQueueJoinTime(bgQueueTypeId), joinType);
            SendPacket(battlefieldStatus.Write());
            continue;
        }

        BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
        GroupQueueInfo ginfo;
        if (!bgQueue.GetPlayerGroupInfoData(player->GetGUID(), &ginfo))
            continue;

        if (ginfo.IsInvitedToBGInstanceGUID)
        {
            bg = sBattlegroundMgr->GetBattleground(ginfo.IsInvitedToBGInstanceGUID, ginfo.NativeBgTypeId);
            if (!bg)
                continue;

            WorldPackets::Battleground::BattlefieldStatusNeedConfirmation battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusNeedConfirmation(&battlefieldStatus, bg, player, i, player->GetBattlegroundQueueJoinTime(bgQueueTypeId), getMSTimeDiff(getMSTime(), ginfo.RemoveInviteTime), joinType);
            SendPacket(battlefieldStatus.Write());
        }
        else
        {
            bg = sBattlegroundMgr->GetBattlegroundTemplate(ginfo.NativeBgTypeId);
            if (!bg)
                continue;

            PVPDifficultyEntry const* bracketEntry = sDB2Manager.GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
            if (!bracketEntry)
                continue;

            WorldPackets::Battleground::BattlefieldStatusQueued queued;
            sBattlegroundMgr->BuildBattlegroundStatusQueued(&queued, bg, player, i, ginfo.JoinTime, bgQueue.GetAverageQueueWaitTime(&ginfo, bracketEntry->RangeIndex), joinType, ginfo.Players.size() > 1);
            SendPacket(queued.Write());
        }
    }
}

void WorldSession::JoinBracket(uint8 bracketType, uint8 rolesMask /*= ROLES_DEFAULT*/, uint16 extraBgTypeId /*= 0*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (player->InBattleground())
        return;

    uint16 bgTypeId = MS::Battlegrounds::BattlegroundTypeId::None;
    switch (bracketType)
    {
        case MS::Battlegrounds::BracketType::Arena2v2:
        case MS::Battlegrounds::BracketType::Arena3v3:
            bgTypeId = MS::Battlegrounds::BattlegroundTypeId::ArenaAll;
            break;
        case MS::Battlegrounds::BracketType::RatedBattleground:
            bgTypeId = MS::Battlegrounds::BattlegroundTypeId::RatedBattleground;
            break;
        case MS::Battlegrounds::BracketType::Brawl:
            bgTypeId = extraBgTypeId;
            break;
        default:
            break;
    }

    if (bgTypeId == MS::Battlegrounds::BattlegroundTypeId::None)
        return;

    auto bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
    if (!bg)
    {
        TC_LOG_ERROR("network", "Battleground: template bg not found BgTypeID %u", bgTypeId);
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, bgTypeId))
    {
        ChatHandler(this).PSendSysMessage(LANG_ARENA_DISABLED);
        return;
    }

    bgTypeId = bg->GetTypeID();

    auto bracketEntry = sDB2Manager.GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
    if (!bracketEntry)
        return;

    auto jointype = MS::Battlegrounds::GetJoinTypeByBracketSlot(bracketType);
    auto bgQueueTypeId = MS::Battlegrounds::GetBgQueueTypeIdByBgTypeID(bgTypeId, jointype);

    if (player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
        return;

    auto& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

    if (bracketType != MS::Battlegrounds::BracketType::Brawl)
    {
        Group* grp = player->GetGroup();
        if (!grp || grp->GetLeaderGUID() != player->GetGUID())
            return;

        ObjectGuid errorGuid;
        if (uint8 err = grp->CanJoinBattlegroundQueue(bg, bgQueueTypeId, jointype, true, bracketType, errorGuid))
        {
            WorldPackets::Battleground::BattlefieldStatusFailed failed;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&failed, bg, player, 0, err, &errorGuid);
            SendPacket(failed.Write());
            return;
        }

        uint32 matchmakerRating = grp->GetAverageMMR(bracketType);
        uint32 avgTime = 0;
        uint32 joinTime = 0;
        uint8 joinType = 0;
        {
            std::lock_guard<std::recursive_mutex> queueGuard(bgQueue.GetLock());
            GroupQueueInfo* ginfo = bgQueue.AddGroup(player, grp, bgTypeId, bracketEntry, jointype, true, false, WorldPackets::Battleground::IgnorMapInfo(), matchmakerRating);
            avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->RangeIndex);
            joinTime = ginfo->JoinTime;
            joinType = ginfo->JoinType;
        }

        auto takeSlot = [bg, bgQueueTypeId, joinTime, avgTime, joinType](Player* member) -> void
        {
            WorldPackets::Battleground::BattlefieldStatusQueued queued;
            sBattlegroundMgr->BuildBattlegroundStatusQueued(&queued, bg, member, member->AddBattlegroundQueueId(bgQueueTypeId), joinTime, avgTime, joinType, true);
            member->SendDirectMessage(queued.Write());
        };

        // the leader takes his slot at once (a second join packet of his must see it); the members stand on other maps and take theirs in their own thread
        for (auto const& slot : grp->GetMemberSlots())
        {
            if (slot.Guid == player->GetGUID())
                takeSlot(player);
            else
                ObjectAccessor::PostToPlayer(slot.Guid, takeSlot);
        }

        sBattlegroundMgr->ScheduleQueueUpdate(new QueueSchedulerItem(matchmakerRating, jointype, bgQueueTypeId, bgTypeId, bracketEntry->RangeIndex, static_cast<Roles>(rolesMask)));
        return;
    }

    // if brawls
    if (auto grp = player->GetGroup())
    {
        if (grp->GetLeaderGUID() != player->GetGUID())
            return;

        ObjectGuid errorGuid;
        if (auto err = grp->CanJoinBattlegroundQueue(bg, bgQueueTypeId, 0, false, bracketType, errorGuid))
        {
            WorldPackets::Battleground::BattlefieldStatusFailed failed;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&failed, bg, player, 0, err, &errorGuid);
            SendPacket(failed.Write());
            return;
        }

        sLFGMgr->InitiBattlgroundCheckRoles(grp, player->GetGUID(), bgTypeId, rolesMask, bgQueueTypeId, WorldPackets::Battleground::IgnorMapInfo(), false);
        return;
    }
    else
    {
        if (player->isUsingLfg())
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_LFG_CANT_USE_BATTLEGROUND);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        if (!player->CanJoinToBattleground(MS::Battlegrounds::IternalPvpTypes::Brawl))
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        player->SetQueueRoleMask(bracketEntry->RangeIndex, rolesMask);

        uint32 joinTime = 0;
        uint32 avgWaitTime = 0;
        {
            std::lock_guard<std::recursive_mutex> queueGuard(bgQueue.GetLock());
            auto ginfo = bgQueue.AddGroup(player, nullptr, bgTypeId, bracketEntry);
            joinTime = ginfo->JoinTime;
            avgWaitTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->RangeIndex);
        }

        WorldPackets::Battleground::BattlefieldStatusQueued battlefieldStatus;
        sBattlegroundMgr->BuildBattlegroundStatusQueued(&battlefieldStatus, bg, player, player->AddBattlegroundQueueId(bgQueueTypeId), joinTime, avgWaitTime, 0, false);
        SendPacket(battlefieldStatus.Write());
    }

    sBattlegroundMgr->ScheduleQueueUpdate(new QueueSchedulerItem(0, 0, bgQueueTypeId, bgTypeId, bracketEntry->RangeIndex, static_cast<Roles>(rolesMask), bracketEntry->MinLevel));
}

void WorldSession::HandleBattlemasterJoinArena(WorldPackets::Battleground::JoinArena& packet)
{
    JoinBracket(packet.TeamSizeIndex, packet.Roles);
}

void WorldSession::HandleBattlemasterJoinBrawl(WorldPackets::Battleground::BattlemasterJoinBrawl& packet)
{
    uint32 temp = 0;
    if (!sBattlegroundMgr->IsBrawlActive(temp))
        return;

    auto data = sBattlegroundMgr->GetBrawlData();
    JoinBracket(MS::Battlegrounds::BracketType::Brawl, packet.RolesMask, data.BgTypeId);
}

void WorldSession::HandleJoinRatedBattleground(WorldPackets::Battleground::JoinRatedBattleground& /*packet*/)
{
    JoinBracket(MS::Battlegrounds::BracketType::RatedBattleground);
}

void WorldSession::HandleRequestRatedInfo(WorldPackets::Battleground::NullCmsg& /*packet*/)
{
    GetPlayer()->SendPvpRatedStats();
}

void WorldSession::HandleRequestPvpOptions(WorldPackets::Battleground::NullCmsg& /*packet*/)
{
    WorldPackets::Battleground::PVPOptionsEnabled options;
    options.RatedArenas = true;
    options.ArenaSkirmish = true;
    options.PugBattlegrounds = true;
    options.WargameBattlegrounds = true;
    options.WargameArenas = true;
    options.RatedBattlegrounds = true;
    SendPacket(options.Write());
}

void WorldSession::HandleRequestPvpReward(WorldPackets::Battleground::RequestPVPRewards& /*packet*/)
{
    GetPlayer()->SendPvpRewards();
}

//! This is const data used for calc some field for SMSG_RATED_BATTLEFIELD_INFO 
void WorldSession::HandlePersonalRatedInfoRequest(WorldPackets::Battleground::NullCmsg& /*packet*/)
{
    WorldPackets::Battleground::ConquestFormulaConstants constants;
    constants.PvpMinCPPerWeek = 1500;
    constants.PvpMaxCPPerWeek = 3000;
    constants.PvpCPBaseCoefficient = 1511.26f;
    constants.PvpCPExpCoefficient = 1639.28f;
    constants.PvpCPNumerator = 0.00412f;
    SendPacket(constants.Write());
}

void WorldSession::HandleAreaSpiritHealerQuery(WorldPackets::Battleground::AreaSpiritHealerQuery& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Creature* unit = player->GetMap()->GetCreature(packet.HealerGuid);
    if (!unit)
        return;

    if (!unit->isSpiritService())
        return;

    if (Battleground* bg = player->GetBattleground())
        sBattlegroundMgr->SendAreaSpiritHealerQuery(player, bg, packet.HealerGuid);

    if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(player->GetCurrentZoneID()))
        bf->SendAreaSpiritHealerQuery(player, packet.HealerGuid);

    if (auto outdoorPvP = player->GetOutdoorPvP())
        outdoorPvP->SendAreaSpiritHealerQueryOpcode(player, packet.HealerGuid);
}

void WorldSession::HandleAreaSpiritHealerQueue(WorldPackets::Battleground::AreaSpiritHealerQueue& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    // only a ghost near the healer: the mass resurrection gives full health and mana. The range is wide on purpose,
    // ghosts can reappear well above the healer (Seething Shore gunships)
    if (player->IsAlive())
        return;

    Creature* unit = player->GetMap()->GetCreature(packet.HealerGuid);
    if (!unit)
        return;

    if (!unit->isSpiritService() || !player->IsWithinDistInMap(unit, 60.0f))
        return;

    if (Battleground* bg = player->GetBattleground())
        bg->AddPlayerToResurrectQueue(packet.HealerGuid, player->GetGUID());

    if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(player->GetCurrentZoneID()))
    {
        bf->RemovePlayerFromResurrectQueue(player->GetGUID());
        bf->AddPlayerToResurrectQueue(packet.HealerGuid, player->GetGUID());
    }

    if (auto outdoorPvP = player->GetOutdoorPvP())
        outdoorPvP->AddPlayerToResurrectQueue(packet.HealerGuid, player->GetGUID());
}

namespace
{
    // The members of a skirmish group stand on other maps: each is checked in his own thread, one after the other in
    // group order, then the leader's thread goes on
    struct SkirmishJoinCheck
    {
        ObjectGuid Leader;
        ObjectGuid GroupGuid;
        std::vector<ObjectGuid> Members;
        uint16 BgTypeId = 0;
        uint8 BgQueueTypeId = 0;
        uint8 JoinType = 0;
        uint8 RolesMask = 0;
    };

    void ContinueSkirmishJoin(Player* leader, std::shared_ptr<SkirmishJoinCheck> const& check)
    {
        Group* grp = leader->GetGroup();
        Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(check->BgTypeId);
        if (!grp || !bg || grp->GetGUID() != check->GroupGuid || grp->GetLeaderGUID() != leader->GetGUID())
            return;

        ObjectGuid errorGuid;
        uint8 err = grp->CanJoinBattlegroundQueue(bg, check->BgQueueTypeId, check->JoinType, false, 0, errorGuid);

        if (!err)
        {
            sLFGMgr->InitiBattlgroundCheckRoles(grp, leader->GetGUID(), check->BgTypeId, check->RolesMask, check->BgQueueTypeId, WorldPackets::Battleground::IgnorMapInfo(), true);
            return;
        }

        for (auto const& slot : grp->GetMemberSlots())
        {
            ObjectAccessor::PostToPlayer(slot.Guid, [bg, err, errorGuid](Player* member) -> void
            {
                WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
                sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, member, 0, err, &errorGuid);
                member->SendDirectMessage(battlefieldStatus.Write());
            });
        }
    }

    void CheckSkirmishMember(std::shared_ptr<SkirmishJoinCheck> check, size_t index);

    // Held by the action posted to a member. If that action is destroyed without having run (he logged out), the
    // destructor goes on with the next member, so that the leader's join never stalls
    struct SkirmishStep
    {
        SkirmishStep(std::shared_ptr<SkirmishJoinCheck> const& joinCheck, size_t nextIndex) : Check(joinCheck), Next(nextIndex) { }
        ~SkirmishStep()
        {
            if (!Handled.load())
                CheckSkirmishMember(Check, Next);
        }

        std::shared_ptr<SkirmishJoinCheck> Check;
        size_t Next;
        std::atomic<bool> Handled{ false };
    };

    void CheckSkirmishMember(std::shared_ptr<SkirmishJoinCheck> check, size_t index)
    {
        for (; index < check->Members.size(); ++index)
        {
            auto step = std::make_shared<SkirmishStep>(check, index + 1);
            bool const posted = ObjectAccessor::PostToPlayer(check->Members[index], [check, step](Player* member) -> void
            {
                step->Handled = true;

                Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(check->BgTypeId);
                if (!bg)
                    return;

                // he left the group meanwhile: nothing to check for him
                Group* memberGroup = member->GetGroup();
                if (!memberGroup || memberGroup->GetGUID() != check->GroupGuid)
                {
                    CheckSkirmishMember(check, step->Next);
                    return;
                }

                uint8 result = 0;
                if (member->isUsingLfg())
                    result = MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_LFG_CANT_USE_BATTLEGROUND;
                else if (!member->CanJoinToBattleground(MS::Battlegrounds::IternalPvpTypes::Skirmish))
                    result = MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS;
                else if (!member->IsAlive())
                    result = MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_GROUP_JOIN_BATTLEGROUND_DEAD;
                else if (member->GetBattlegroundQueueIndex(check->BgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
                    return;
                else if (!member->HasFreeBattlegroundQueueId())
                    result = MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_BATTLEGROUND_TOO_MANY_QUEUES;
                else
                {
                    CheckSkirmishMember(check, step->Next);
                    return;
                }

                WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
                sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, member, 0, result);
                auto const& message = battlefieldStatus.Write();
                ObjectAccessor::SendToPlayer(check->Leader, message);
                member->SendDirectMessage(message);
            });

            if (posted)
                return;

            step->Handled = true;   // not posted (he is offline): this loop goes on itself
        }

        // every member passed (one who left is skipped)
        ObjectAccessor::PostToPlayer(check->Leader, [check](Player* leader) -> void { ContinueSkirmishJoin(leader, check); });
    }
}

void WorldSession::HandleJoinSkirmish(WorldPackets::Battleground::JoinSkirmish& packet)
{
    Player* player = GetPlayer();
    Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(MS::Battlegrounds::BattlegroundTypeId::ArenaAll);
    if (!bg)
        return;

    if (!MS::Battlegrounds::IsSkirmishJoinTypes(packet.Bracket))
        return;

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, MS::Battlegrounds::BattlegroundTypeId::ArenaAll))
    {
        ChatHandler(this).PSendSysMessage(LANG_ARENA_DISABLED);
        return;
    }

    uint16 bgTypeId = bg->GetTypeID();

    if (player->InBattleground())
        return;

    PVPDifficultyEntry const* bracketEntry = sDB2Manager.GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
    if (!bracketEntry)
        return;

    uint8 jointype = MS::Battlegrounds::JoinType::Arena2v2;
    uint8 bgQueueTypeId = MS::Battlegrounds::GetBgQueueTypeIdByBgTypeID(bgTypeId, jointype);

    if (!packet.JoinAsGroup)
    {
        if (player->isUsingLfg())
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_LFG_CANT_USE_BATTLEGROUND);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        if (!player->CanJoinToBattleground(MS::Battlegrounds::IternalPvpTypes::Skirmish))
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        if (!player->IsAlive())
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_GROUP_JOIN_BATTLEGROUND_DEAD);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        if (player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
            return;

        if (!player->HasFreeBattlegroundQueueId())
        {
            WorldPackets::Battleground::BattlefieldStatusFailed battlefieldStatus;
            sBattlegroundMgr->BuildBattlegroundStatusFailed(&battlefieldStatus, bg, player, 0, MS::Battlegrounds::GroupJoinBattlegroundResult::ERR_BATTLEGROUND_TOO_MANY_QUEUES);
            SendPacket(battlefieldStatus.Write());
            return;
        }

        player->SetQueueRoleMask(bracketEntry->RangeIndex, packet.RolesMask);

        BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
        uint32 joinTime = 0;
        uint32 avgWaitTime = 0;
        {
            std::lock_guard<std::recursive_mutex> queueGuard(bgQueue.GetLock());
            GroupQueueInfo* ginfo = bgQueue.AddGroup(player, nullptr, bgTypeId, bracketEntry, jointype);
            joinTime = ginfo->JoinTime;
            avgWaitTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->RangeIndex);
        }

        WorldPackets::Battleground::BattlefieldStatusQueued battlefieldStatus;
        sBattlegroundMgr->BuildBattlegroundStatusQueued(&battlefieldStatus, bg, player, player->AddBattlegroundQueueId(bgQueueTypeId), joinTime, avgWaitTime, jointype, false);
        SendPacket(battlefieldStatus.Write());
    }
    else
    {
        Group* grp = player->GetGroup();
        if (!grp)
            return;

        if (grp->GetLeaderGUID() != player->GetGUID())
            return;

        auto check = std::make_shared<SkirmishJoinCheck>();
        check->Leader = player->GetGUID();
        check->GroupGuid = grp->GetGUID();
        for (auto const& slot : grp->GetMemberSlots())
            check->Members.push_back(slot.Guid);
        check->BgTypeId = bgTypeId;
        check->BgQueueTypeId = bgQueueTypeId;
        check->JoinType = jointype;
        check->RolesMask = packet.RolesMask;
        CheckSkirmishMember(check, 0);
        return;
    }

    sBattlegroundMgr->ScheduleQueueUpdate(new QueueSchedulerItem(0, jointype, bgQueueTypeId, bgTypeId, bracketEntry->RangeIndex, Roles(packet.RolesMask)));
}

void WorldSession::HandleHearthAndResurrect(WorldPackets::Battleground::HearthAndResurrect& /*packet*/)
{
    if (_player->isInFlight())
        return;

    // Offered on the release spirit dialog: alive, it would be a free heal and hearthstone
    if (_player->IsAlive())
        return;

    AreaTableEntry const* atEntry = sAreaTableStore.LookupEntry(_player->GetCurrentAreaID());
    if (!atEntry || !(atEntry->Flags[0] & AREA_FLAG_CAN_HEARTH_AND_RESURRECT))
        return;

    Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(_player->GetCurrentZoneID());
    if (!bf || !bf->IsWarTime())
        return;

    _player->BuildPlayerRepop();
    _player->ResurrectPlayer(1.0f);
    _player->TeleportTo(_player->m_homebindMapId, _player->m_homebindX, _player->m_homebindY, _player->m_homebindZ, _player->GetOrientation());
}

void WorldSession::HandleUpdatePrestigeLevel(WorldPackets::Battleground::NullCmsg& /*packet*/)
{
    Player* player = GetPlayer();

    HonorInfo* honorInfo = player->GetHonorInfo();
    if (honorInfo->HonorLevel != HonorInfo::MaxHonorLevel || honorInfo->PrestigeLevel >= sWorld->getIntConfig(CONFIG_MAX_PRESTIGE_LEVEL))
        return;

    if (!honorInfo->IncreasePrestigeLevel())
        return;

    honorInfo->HonorLevel = 0;
    honorInfo->NextHonorLevel = 1;

    player->UpdateAchievementCriteria(CRITERIA_TYPE_PRESTIGE_LEVEL_UP, honorInfo->PrestigeLevel);

    if (GtHonorLevelEntry const* data = sHonorLevelGameTable.GetRow(honorInfo->NextHonorLevel))
        honorInfo->NextHonorAtLevel = data->Prestige[honorInfo->PrestigeLevel];

    player->UpdateHonorFields(true); // Update data fo honor level

    player->SetUInt32Value(PLAYER_FIELD_HONOR, honorInfo->CurrentHonorAtLevel);
    player->SetUInt32Value(PLAYER_FIELD_HONOR_LEVEL, honorInfo->HonorLevel);
    player->SetUInt32Value(PLAYER_FIELD_HONOR_NEXT_LEVEL, honorInfo->NextHonorAtLevel);
    player->SetUInt32Value(PLAYER_FIELD_PRESTIGE, honorInfo->PrestigeLevel);

    player->RemoveAllPvPTalent();
    player->SendTalentsInfoData(false);
}

void WorldSession::HandleAcceptWargameInvite(WorldPackets::Battleground::AcceptWargameInvite& packet)
{
    sBattlegroundMgr->InitWargame(_player, packet.OpposingPartyMember, packet.QueueID, packet.Accept);
}

void WorldSession::HandleStartWarGame(WorldPackets::Battleground::StartWargame& packet)
{
    if (!_player->GetGroup())
        return;

    if (_player->GetGroup()->GetLeaderGUID() != _player->GetGUID())
        return;

    // the opposing leader may stand on another map: only his group's identity is read, under the accessor lock
    bool validOpponent = false;
    if (!ObjectAccessor::WithPlayer(packet.OpposingPartyMember, [&](Player* opposingPartyLeader)
    {
        Group* opposingGroup = opposingPartyLeader->GetGroup();
        validOpponent = opposingGroup && opposingGroup->GetLeaderGUID() == opposingPartyLeader->GetGUID() && opposingGroup != _player->GetGroup();
    }) || !validOpponent)
        return;

    if (_player->HasWargameRequest())
        return;

    WargameRequest request;
    request.OpposingPartyMemberGUID = packet.OpposingPartyMember;
    request.TournamentRules = packet.TournamentRules;
    request.CreationDate = GameTime::GetGameTime();
    request.QueueID = packet.QueueID;

    _player->SetWargameRequest(request);

    WorldPackets::Battleground::CheckWargameEntry response;
    response.OpposingPartyBnetAccountID = _player->GetSession()->GetBattlenetAccountGUID();
    response.OpposingPartyMember = _player->GetGUID();
    response.QueueID = packet.QueueID;
    response.TimeoutSeconds = 60;
    response.TournamentRules = packet.TournamentRules;
    ObjectAccessor::SendToPlayer(packet.OpposingPartyMember, response.Write());
}

void WorldSession::HandleRequestPvpBrawlInfo(WorldPackets::Battleground::RequestPvpBrawlInfo& /*packet*/)
{
    uint32 expirationTimer = MS::Utilities::Globals::InSeconds::Week;
    auto isHolidayActive = sBattlegroundMgr->IsBrawlActive(expirationTimer);

    WorldPackets::Battleground::RequestPvpBrawlInfoResponse response;
    response.IsActive = isHolidayActive;

    if (isHolidayActive)
    {
        response.TimeToEnd = expirationTimer;
        auto data = sBattlegroundMgr->GetBrawlData();
        response.BattlegroundTypeId = data.BgTypeId;
        response.Name = data.Name;
        response.Description = "";
        response.Description2 = "";
        response.BrawlType = 0;

        if(auto info = sHolidayNamesStore.LookupEntry(data.HolidayNameId))
            if (strlen(info->Name->Str[GetSessionDbLocaleIndex()]))
                response.Name = info->Name->Str[GetSessionDbLocaleIndex()];

        if (auto info = sAdventureJournalStore.LookupEntry(data.JournalId))
            if (strlen(info->Description->Str[GetSessionDbLocaleIndex()]))
                response.Description2 = info->Description->Str[GetSessionDbLocaleIndex()];
    }

    SendPacket(response.Write());
}

void WorldSession::HandleRequstCrowdControlSpell(WorldPackets::Battleground::RequstCrowdControlSpell& packet)
{
    if (Battleground* bg = _player->GetBattleground())
    {
        bool foundPlayer = false;

        for (auto itr : bg->GetPlayers())
        {
            if (itr.first == _player->GetGUID())
                continue;

            if (itr.first == packet.PlayerGuid)
            {
                foundPlayer = true;
                break;
            }
        }

        if (!foundPlayer)
            return;

        if (Player* opponent = ObjectAccessor::GetPlayer(*_player, packet.PlayerGuid))
        {
            WorldPackets::Battleground::ArenaCrowdControlSpells response;
            response.PlayerGuid = opponent->GetGUID();
            response.CrowdControlSpellID = 195710;

            for (uint32 i = 0; i < MAX_TALENT_COLUMNS; ++i)
            {
                for (auto const& talent : sDB2Manager.GetPvpTalentByPosition(opponent->getClass(), 0, i))
                {
                    if (!opponent->HasPvPTalent(talent->SpellID))
                        continue;

                    response.CrowdControlSpellID = talent->SpellID;
                    break;
                }
            }

            opponent->SetCrowdControlSpellId(response.CrowdControlSpellID);

            SendPacket(response.Write());
        }
    }
}
