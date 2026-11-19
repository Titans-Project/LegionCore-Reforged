////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
///      Based on TrinityCore code       ///
///   Copyright 2008-2012 TrinityCore    ///
///   <http://www.trinitycore.org/>      ///
///                                      ///
////////////////////////////////////////////

#include "PartyPackets.h"
#include "GlobalFunctional.h"
#include "GroupMgr.h"
#include "SocialMgr.h"
#include "BattlegroundMgr.h"
#include "LFG.h"
#include <memory>

class Aura;

namespace
{
// A group started by an invite is pending until its first invitee accepts. If the invitee's thread never runs the
// invite (he logged out first) the guard drops the pending group, so that its leader is not left with a dead invite.
// A pending group has no guid and is not in GroupMgr: it is found again through its leader, whose invite names it as
// long as it is alive (his logout or the drop withdraws that invite, GroupMgr frees the group 10 s later). The stored
// pointer is only an identity token and is not dereferenced unless the leader still names it.
struct PendingGroupGuard
{
    PendingGroupGuard(Group* pending, ObjectGuid leader) : group(pending), leaderGuid(leader) { }
    ~PendingGroupGuard() { Drop(); }

    void Release() { group = nullptr; }

    Group* Find() const
    {
        Group* found = nullptr;
        if (group)
        {
            ObjectAccessor::WithPlayer(leaderGuid, [this, &found](Player* leader)
            {
                if (leader->GetGroupInvite() == group)
                    found = group;
            }, ObjectAccessor::PlayerScope::InOrOutOfWorld);
        }
        return found;
    }

    void Drop()
    {
        Group* pending = Find();
        group = nullptr;
        if (!pending)
            return;

        {
            std::lock_guard<std::mutex> inviteGuard(pending->m_inviteLock);
            // already dropped by the leader's logout, which queued it
            if (pending->IsDisposed() || pending->IsCreated())
                return;

            pending->RemoveAllInvites();
            pending->SetDisposed();
        }
        sGroupMgr->QueueForDelete(pending);
    }

    Group* group;
    ObjectGuid leaderGuid;
};
}

void WorldSession::SendPartyResult(PartyOperation operation, std::string const& member, PartyResult res, uint32 val /* = 0 */)
{
    WorldPackets::Party::PartyCommandResult packet;
    packet.Name = member;
    packet.Command = operation;
    packet.Result = res;
    packet.ResultData = val;
    SendPacket(packet.Write());
}

void WorldSession::HandlePartyInvite(WorldPackets::Party::PartyInviteClient& packet)
{
    Player* c_player = GetPlayer();
    if (!c_player)
        return;

    if (c_player->IsSpectator())
        return;

    // cheating
    if (!normalizePlayerName(packet.TargetName))
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_BAD_PLAYER_NAME_S);
        return;
    }

    ObjectGuid inviteeGuid = ObjectAccessor::FindPlayerGuidByName(packet.TargetName);
    if (inviteeGuid.IsEmpty())
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_BAD_PLAYER_NAME_S);
        return;
    }

    // The invitee may stand on another map: what the checks need is read once, under the accessor lock.
    struct InviteeState
    {
        bool gameMaster = false;
        bool invisible = false;
        uint32 team = 0;
        uint32 mapId = 0;
        uint32 instanceId = 0;
        bool inInstance = false;
        Difficulty dungeonDifficulty = DIFFICULTY_NONE;
        bool hasInvite = false;
        Group* group = nullptr;
        Group* originalGroup = nullptr;
    } invitee;

    ObjectGuid inviterGuid = c_player->GetGUID();
    if (!ObjectAccessor::WithPlayer(inviteeGuid, [&invitee](Player* player)
    {
        invitee.gameMaster = player->isGameMaster();
        invitee.invisible = player->HasPlayerExtraFlag(PLAYER_EXTRA_INVISIBLE_STATUS);
        invitee.team = player->GetTeam();
        invitee.mapId = player->GetMapId();
        invitee.instanceId = player->GetInstanceId();
        invitee.inInstance = player->InInstance();
        invitee.dungeonDifficulty = player->GetDungeonDifficultyID();
        invitee.hasInvite = player->GetGroupInvite() != nullptr;
        invitee.group = player->GetGroup();
        invitee.originalGroup = player->GetOriginalGroup();
    }))
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_BAD_PLAYER_NAME_S);
        return;
    }

    // restrict invite to GMs
    if (!sWorld->getBoolConfig(CONFIG_ALLOW_GM_GROUP) && !c_player->isGameMaster() && invitee.gameMaster)
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_BAD_PLAYER_NAME_S);
        return;
    }

    if (invitee.invisible && !c_player->isGameMaster())
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_BAD_PLAYER_NAME_S);
        return;
    }

    // can't group with
    if (!c_player->isGameMaster() && !sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP) && c_player->GetTeam() != invitee.team
        || invitee.mapId == 1101 || c_player->GetMapId() == 1101) // deathmatch
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_PLAYER_WRONG_FACTION);
        return;
    }
    if (c_player->InInstance() && invitee.inInstance && c_player->GetInstanceId() != invitee.instanceId && c_player->GetMapId() == invitee.mapId)
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_TARGET_NOT_IN_INSTANCE_S);
        return;
    }

    if (invitee.inInstance && invitee.dungeonDifficulty != c_player->GetDungeonDifficultyID())
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_IGNORING_YOU_S);
        return;
    }

    // by guid: the invitee's PlayerSocial is destroyed at his logout, which may come before he leaves the accessor
    if (sSocialMgr->HasContact(inviteeGuid, inviterGuid, SOCIAL_FLAG_IGNORED))
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_IGNORING_YOU_S);
        return;
    }

    Group* group = c_player->GetGroup();
    if (group && group->isBGGroup())
        group = c_player->GetOriginalGroup();

    Group* group2 = invitee.group;
    if (group2 && group2->isBGGroup())
        group2 = invitee.originalGroup;

    if (group2 || invitee.hasInvite)
    {
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_ALREADY_IN_GROUP_S);

        //! tell the player that they were invited but it failed as they were already in a group
        if (group2)
        {
            WorldPackets::Party::PartyInvite partyInvite;
            partyInvite.Initialize(c_player, packet.ProposedRoles, false);
            ObjectAccessor::SendToPlayer(inviteeGuid, partyInvite.Write());
        }
        return;
    }

    if (group)
    {
        // not have permissions for invite
        if (!group->IsLeader(c_player->GetGUID()) && !group->IsAssistant(c_player->GetGUID()) && !(group->GetGroupFlags() & GROUP_FLAG_EVERYONE_ASSISTANT))
        {
            SendPartyResult(PARTY_OP_INVITE, "", ERR_NOT_LEADER);
            return;
        }
        // not have place
        if (group->IsFull())
        {
            SendPartyResult(PARTY_OP_INVITE, "", ERR_GROUP_FULL);
            return;
        }
    }

    // ok, but group not exist, start a new group
    // but don't create and save the group to the DB until
    // at least one person joins
    std::shared_ptr<PendingGroupGuard> pending;
    ObjectGuid groupGuid;
    if (!group)
    {
        group = new Group;
        // new group: if can't add then delete
        if (!group->AddLeaderInvite(c_player))
        {
            delete group;
            return;
        }
        pending = std::make_shared<PendingGroupGuard>(group, inviterGuid);
    }
    else
        groupGuid = group->GetGUID();

    auto partyInvite = std::make_shared<WorldPackets::Party::PartyInvite>();
    partyInvite->Initialize(c_player, packet.ProposedRoles, true);

    // The invite is written on the invitee, so it runs in his own thread (his logout withdraws it there). A failure
    // after this point is silent, as before; the guard drops the pending group if the action never runs.
    std::string targetName = packet.TargetName;
    bool posted = ObjectAccessor::PostToPlayer(inviteeGuid, [inviterGuid, groupGuid, targetName, partyInvite, pending](Player* invited) -> void
    {
        bool added = false;
        if (pending)
        {
            if (Group* newGroup = pending->Find())
            {
                std::lock_guard<std::mutex> inviteGuard(newGroup->m_inviteLock);
                added = !newGroup->IsDisposed() && newGroup->AddInvite(invited);
            }

            if (added)
                pending->Release();
            else
                pending->Drop();
        }
        else if (Group* existing = sGroupMgr->GetGroupByGUID(groupGuid))
            added = existing->AddInvite(invited);

        if (!added)
            return;

        invited->SendDirectMessage(partyInvite->Write());

        WorldPackets::Party::PartyCommandResult result;
        result.Name = targetName;
        result.Command = PARTY_OP_INVITE;
        result.Result = ERR_PARTY_RESULT_OK;
        ObjectAccessor::SendToPlayer(inviterGuid, result.Write());
    }, 0, ObjectAccessor::PlayerScope::InWorld);

    if (!posted)
        SendPartyResult(PARTY_OP_INVITE, packet.TargetName, ERR_BAD_PLAYER_NAME_S);
}

// Runs in the thread of the player who accepts. A group still to be created is made in its leader's thread (Create
// writes on him): when he stands on another map, he makes it there and calls this again from the acceptor's thread.
static void AcceptPartyInvite(Player* player)
{
    Group* group = player->GetGroupInvite();
    if (!group)
        return;

    WorldSession* session = player->GetSession();
    bool dispose = false;
    {
        // Two invitees may accept from two maps at once: the full check, the creation of a new group and the join
        // are one step. m_inviteLock is only taken here and before any other lock, so holding it through
        // Create/AddMember cannot close a cycle.
        std::lock_guard<std::mutex> inviteGuard(group->m_inviteLock);

        // dropped (the pending group is queued for deletion) or withdrawn by another thread since it was read
        if (group->IsDisposed() || player->GetGroupInvite() != group)
            return;

        // The leader stays alive while his invite names this group: his logout withdraws it under m_inviteLock,
        // held here.
        ObjectGuid leaderGuid = group->GetLeaderGUID();
        Player* leader = nullptr;
        bool leaderElsewhere = false;
        if (!group->IsCreated() && !group->IsDisbanding() && leaderGuid != player->GetGUID())
        {
            leader = group->GetInvited(leaderGuid);
            if (leader && (leader->GetGroupInvite() != group || !leader->IsInWorld() || leader->IsDelete()))
                leader = nullptr;

            leaderElsewhere = leader && ObjectAccessor::FindPlayer(player->FindMap(), leaderGuid) != leader;
        }

        if (leaderElsewhere)
        {
            ObjectGuid acceptorGuid = player->GetGUID();
            if (ObjectAccessor::PostToPlayer(leaderGuid, [group, acceptorGuid](Player* groupLeader) -> void
            {
                // The pending group is only dereferenced while the leader's invite names it: it is alive then.
                // Otherwise another acceptance created it, or it was dropped; the acceptor reads his invite again.
                if (groupLeader->GetGroupInvite() == group)
                {
                    std::lock_guard<std::mutex> leaderInviteGuard(group->m_inviteLock);
                    if (group->IsDisposed())
                        return;

                    if (!group->IsCreated())
                    {
                        if (group->IsDisbanding())
                            return;

                        group->RemoveInvite(groupLeader);
                        group->Create(groupLeader);
                        sGroupMgr->AddGroup(group);
                    }
                }

                ObjectAccessor::PostToPlayer(acceptorGuid, [](Player* acceptor) -> void { AcceptPartyInvite(acceptor); });
            }, 0, ObjectAccessor::PlayerScope::InWorld))
                return;

            // the leader is zoning or leaving
            group->RemoveAllInvites();
            group->SetDisposed();
            dispose = true;
        }
        else
        {
            // Remove player from invitees in any case
            group->RemoveInvite(player);

            if (leaderGuid == player->GetGUID())
            {
                TC_LOG_ERROR("network", "HandleGroupAcceptOpcode: player %s(%d) tried to accept an invite to his own group", player->GetName(), player->GetGUIDLow());
                return;
            }

            // Group is full
            if (group->IsFull())
            {
                session->SendPartyResult(PARTY_OP_INVITE, "", ERR_GROUP_FULL);
                return;
            }

            // Forming a new group, create it (a created group being disbanded also has no member left: not reborn)
            if (!group->IsCreated())
            {
                if (group->IsDisbanding())
                    return;

                // This can happen if the leader is zoning. To be removed once delayed actions for zoning are implemented
                if (!leader)
                {
                    group->RemoveAllInvites();
                    group->SetDisposed();
                    dispose = true;
                }
                else
                {
                    group->RemoveInvite(leader);
                    group->Create(leader);
                    sGroupMgr->AddGroup(group);
                }
            }

            // Everything is fine, do it, PLAYER'S GROUP IS SET IN ADDMEMBER!!!
            if (!dispose && !group->AddMember(player))
                return;
        }
    }

    if (dispose)
    {
        sGroupMgr->QueueForDelete(group);
        return;
    }

    group->BroadcastGroupUpdate();
}

void WorldSession::HandlePartyInviteResponse(WorldPackets::Party::PartyInviteResponse& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Group* group = player->GetGroupInvite();
    if (!group)
        return;

    if (packet.Accept)
        AcceptPartyInvite(player);
    else
    {
        // Remember leader (group pointer will be invalid if group gets disbanded)
        ObjectGuid leaderGuid = group->GetLeaderGUID();

        // uninvite, group can be deleted
        player->UninviteFromGroup();

        ObjectAccessor::SendToPlayer(leaderGuid, WorldPackets::Party::GroupDecline(player->GetName()).Write());
    }
}

void WorldSession::HandlePartyUninvite(WorldPackets::Party::PartyUninvite& packet)
{
    if (packet.TargetGUID == GetPlayer()->GetGUID())
        return;

    PartyResult res = GetPlayer()->CanUninviteFromGroup();
    if (res != ERR_PARTY_RESULT_OK)
    {
        SendPartyResult(PARTY_OP_UNINVITE, "", res);
        return;
    }

    Group* grp = GetPlayer()->GetGroup();
    if (!grp)
        return;

    if (auto player = GetPlayer())
        if (!player->CanKickFromChallenge())
            return;

    if (grp->IsLeader(packet.TargetGUID))
    {
        SendPartyResult(PARTY_OP_UNINVITE, "", ERR_NOT_LEADER);
        return;
    }

    if (grp->IsMember(packet.TargetGUID))
    {
        Player::RemoveFromGroup(grp, packet.TargetGUID, GROUP_REMOVEMETHOD_KICK, GetPlayer()->GetGUID(), packet.Reason.c_str());
        return;
    }

    if (grp->GetInvited(packet.TargetGUID))
    {
        // the invitee may stand on another map: he leaves the invite in his own thread, found again by guid
        ObjectGuid groupGuid = grp->GetGUID();
        ObjectAccessor::PostToPlayer(packet.TargetGUID, [groupGuid](Player* invited) -> void
        {
            if (Group* invite = invited->GetGroupInvite())
                if (invite->GetGUID() == groupGuid)
                    invited->UninviteFromGroup();
        });
        return;
    }

    SendPartyResult(PARTY_OP_UNINVITE, "", ERR_TARGET_NOT_IN_GROUP_S);
}

void WorldSession::HandleSetPartyLeader(WorldPackets::Party::SetPartyLeader& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    // the new leader and the members may stand on other maps: short reads under the accessor lock
    bool targetInGroup = false;
    ObjectAccessor::WithPlayer(packet.TargetGUID, [group, &targetInGroup](Player* player) { targetInGroup = player->GetGroup() == group; });

    if (!group->IsLeader(GetPlayer()->GetGUID()) || !targetInGroup)
        return;

    // Prevent exploits with instance saves; a dungeon finder group's instance belongs to the group, its lead may change inside
    if (!group->isLFGGroup())
    {
        for (auto const& member : group->GetMemberSlots())
        {
            bool inInstance = false;
            ObjectAccessor::WithPlayer(member.Guid, [&inInstance](Player* plr) { inInstance = plr->FindMap() && plr->FindMap()->Instanceable(); });
            if (inInstance)
                return;
        }
    }

    group->ChangeLeader(packet.TargetGUID, packet.PartyIndex);
    group->SendUpdate();
}

void WorldSession::HandleSetRole(WorldPackets::Party::SetRole& packet)
{
    if (packet.Role & ~(lfg::PLAYER_ROLE_TANK | lfg::PLAYER_ROLE_HEALER | lfg::PLAYER_ROLE_DAMAGE))
        return;

    Group* group = GetPlayer()->GetGroup();
    ObjectGuid senderGuid = GetPlayer()->GetGUID();
    if (packet.TargetGUID != senderGuid)
    {
        if (!group || !group->IsMember(packet.TargetGUID))
            return;

        if (!group->IsLeader(senderGuid) && !group->IsAssistant(senderGuid) && !(group->GetGroupFlags() & GROUP_FLAG_EVERYONE_ASSISTANT))
            return;
    }

    uint8 oldRole = group ? group->GetLfgRoles(packet.TargetGUID) : 0;
    if (oldRole == packet.Role)
        return;

    WorldPackets::Party::RoleChangedInform roleChangedInform;
    roleChangedInform.PartyIndex = packet.PartyIndex;
    roleChangedInform.From = GetPlayer()->GetGUID();
    roleChangedInform.ChangedUnit = packet.TargetGUID;
    roleChangedInform.OldRole = oldRole;
    roleChangedInform.NewRole = packet.Role;

    if (group)
    {
        group->BroadcastPacket(roleChangedInform.Write(), false);
        group->SetLfgRoles(packet.TargetGUID, packet.Role);
    }
    else
        SendPacket(roleChangedInform.Write());
}

void WorldSession::HandleLeaveGroup(WorldPackets::Party::LeaveGroup& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Group* grp = player->GetGroup();
    if (!grp)
        return;

    if (player->InBattleground())
    {
        SendPartyResult(PARTY_OP_INVITE, "", ERR_INVITE_RESTRICTED);
        return;
    }

    SendPartyResult(PARTY_OP_LEAVE, player->GetName(), ERR_PARTY_RESULT_OK);
    player->RemoveFromGroup(GROUP_REMOVEMETHOD_LEAVE);
}

void WorldSession::HandleSetLootMethod(WorldPackets::Party::SetLootMethod& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group || group->RollIsActive())
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()) || group->isLFGGroup())
        return;

    if (packet.LootThreshold < ITEM_QUALITY_UNCOMMON || packet.LootThreshold > ITEM_QUALITY_ARTIFACT)
        return;

    if (packet.LootMethod > PERSONAL_LOOT)
        return;

    if (packet.LootMethod == MASTER_LOOT && !group->IsMember(packet.LootMasterGUID))
        return;

    group->SetLootMethod(static_cast<LootMethod>(packet.LootMethod));
    // the client GUID only names a master looter; round robin restarts from the leader
    group->SetLooterGuid(packet.LootMethod == MASTER_LOOT ? packet.LootMasterGUID : GetPlayer()->GetGUID());
    group->SetLootThreshold(static_cast<ItemQualities>(packet.LootThreshold));
    group->SendUpdate();
}

void WorldSession::HandleMinimapPing(WorldPackets::Party::MinimapPingClient& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Group* group = player->GetGroup();
    if (!group)
        return;

    // BroadcastPacket reaches each member by guid under the accessor lock, whatever map he stands on
    WorldPackets::Party::MinimapPing minimapPing;
    minimapPing.Sender = player->GetGUID();
    minimapPing.PositionX = packet.PositionX;
    minimapPing.PositionY = packet.PositionY;
    group->BroadcastPacket(minimapPing.Write(), true, -1, player->GetGUID());
}

void WorldSession::HandleUpdateRaidTarget(WorldPackets::Party::UpdateRaidTarget& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Group* group = player->GetGroup();
    if (!group)
        return;

    if (packet.Symbol == -1)
        group->SendTargetIconList(packet.PartyIndex);
    else if (group->IsLeader(player->GetGUID()) || group->IsAssistant(player->GetGUID()))
        group->SetTargetIcon(packet.Symbol, packet.Target, player->GetGUID(), packet.PartyIndex);
}

void WorldSession::HandleConvertRaid(WorldPackets::Party::ConvertRaid& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group || group->InChallenge() || group->isLFGGroup())
        return;

    if (_player->InBattleground())
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()) || group->GetMembersCount() < 2)
        return;

    SendPartyResult(PARTY_OP_INVITE, "", ERR_PARTY_RESULT_OK);

    if (packet.Raid)
    {
        if (!group->isRaidGroup())
            group->ConvertToRaid();
    }
    else if (group->isRaidGroup())
        group->ConvertToGroup();
}

void WorldSession::HandleChangeSubGroup(WorldPackets::Party::ChangeSubGroup& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (packet.NewSubGroup >= MAX_RAID_SUBGROUPS)
        return;

    ObjectGuid senderGuid = GetPlayer()->GetGUID();
    if (!group->IsLeader(senderGuid) && !group->IsAssistant(senderGuid) && !(group->GetGroupFlags() & GROUP_FLAG_EVERYONE_ASSISTANT))
        return;

    if (!group->HasFreeSlotSubGroup(packet.NewSubGroup))
        return;

    group->ChangeMembersGroup(packet.TargetGUID, packet.NewSubGroup);
}

void WorldSession::HandleSwapSubGroups(WorldPackets::Party::SwapSubGroups& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    ObjectGuid senderGuid = GetPlayer()->GetGUID();
    if (!group->IsLeader(senderGuid) && !group->IsAssistant(senderGuid) && !(group->GetGroupFlags() & GROUP_FLAG_EVERYONE_ASSISTANT))
        return;

    group->SwapMembersGroups(packet.FirstTarget, packet.SecondTarget);
}

void WorldSession::HandleSetEveryoneIsAssistant(WorldPackets::Party::SetEveryoneIsAssistant& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()))
        return;

    group->ChangeFlagEveryoneAssistant(packet.EveryoneIsAssistant);
}

void WorldSession::HandleSetAssistantLeader(WorldPackets::Party::SetAssistantLeader& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()))
        return;

    group->SetGroupMemberFlag(packet.Target, packet.Apply, MEMBER_FLAG_ASSISTANT);
    group->SendUpdate();
}

void WorldSession::HandleSetPartyAssignment(WorldPackets::Party::SetPartyAssignment& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    ObjectGuid senderGuid = GetPlayer()->GetGUID();
    if (!group->IsLeader(senderGuid) && !group->IsAssistant(senderGuid) && !(group->GetGroupFlags() & GROUP_FLAG_EVERYONE_ASSISTANT))
        return;

    // SetGroupMemberFlag clears the previous holder only when the flag is given, and sends the update
    switch (packet.Assignment)
    {
        case GROUP_ASSIGN_MAINASSIST:
            group->SetGroupMemberFlag(packet.Target, packet.Set, MEMBER_FLAG_MAINASSIST);
            break;
        case GROUP_ASSIGN_MAINTANK:
            group->SetGroupMemberFlag(packet.Target, packet.Set, MEMBER_FLAG_MAINTANK);
            break;
        default:
            break;
    }
}

void WorldSession::HandleDoReadyCheck(WorldPackets::Party::DoReadyCheck& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()) && !(group->GetGroupFlags() & GROUP_FLAG_EVERYONE_ASSISTANT))
        return;

    if (!group->StartReadyCheck(GetPlayer()->GetGUID(), packet.PartyIndex))
        return;

    WorldPackets::Party::ReadyCheckStarted readyCheckStarted;
    readyCheckStarted.PartyGUID = group->GetGUID();
    readyCheckStarted.PartyIndex = packet.PartyIndex;
    readyCheckStarted.InitiatorGUID = GetPlayer()->GetGUID();
    readyCheckStarted.Duration = READY_CHECK_DURATION;
    group->BroadcastPacket(readyCheckStarted.Write(), false, -1);

    group->OfflineReadyCheck();

    // Group::Update is not ticked: the timeout rides on the initiator, the group is looked up again
    ObjectGuid groupGuid = group->GetGUID();
    uint32 startTime = group->GetReadyCheckStartTime();
    GetPlayer()->AddDelayedEvent(READY_CHECK_DURATION, [groupGuid, startTime]() -> void
    {
        if (Group* readyCheckGroup = sGroupMgr->GetGroupByGUID(groupGuid))
            readyCheckGroup->ReadyCheckTimeout(startTime);
    });

    if (group->IsReadyCheckCompleted())
        group->EndReadyCheck();
}

void WorldSession::HandleReadyCheckResponse(WorldPackets::Party::ReadyCheckResponseClient& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group || !group->SetMemberReadyChecked(GetPlayer()->GetGUID()))
        return;

    WorldPackets::Party::ReadyCheckResponse response;
    response.PartyGUID = group->GetGUID();
    response.Player = GetPlayer()->GetGUID();
    response.IsReady = packet.IsReady;
    group->BroadcastPacket(response.Write(), true);

    if (group->IsReadyCheckCompleted())
        group->EndReadyCheck();
}

void WorldSession::HandleRequestPartyJoinUpdates(WorldPackets::Party::RequestPartyJoinUpdates& packet)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    group->SendUpdate();
    group->SendTargetIconList(packet.PartyIndex);
    group->SendRaidMarkersChanged(this, packet.PartyIndex);
}

void WorldSession::HandleRequestPartyMemberStats(WorldPackets::Party::RequestPartyMemberStats& packet)
{
    WorldPackets::Party::PartyMemberStats partyMemberStats;

    // stats (position, health...) of anyone outside the requester's group are not his to see
    bool allowed = packet.TargetGUID == GetPlayer()->GetGUID();
    if (!allowed)
    {
        Group* group = GetPlayer()->GetGroup();
        Group* originalGroup = GetPlayer()->GetOriginalGroup();
        allowed = (group && group->IsMember(packet.TargetGUID)) || (originalGroup && originalGroup->IsMember(packet.TargetGUID));
    }

    // auras, pet and powers belong to the member's map thread: a member of another map builds his stats there
    bool found = false;
    if (allowed)
    {
        if (Player* member = ObjectAccessor::GetPlayer(*GetPlayer(), packet.TargetGUID))
        {
            partyMemberStats.Initialize(member);
            found = true;
        }
        else
        {
            ObjectGuid const requester = GetPlayer()->GetGUID();
            if (ObjectAccessor::PostToPlayer(packet.TargetGUID, [requester](Player* member)
            {
                WorldPackets::Party::PartyMemberStats stats;
                stats.Initialize(member);
                ObjectAccessor::SendToPlayer(requester, stats.Write());
            }, 0, ObjectAccessor::PlayerScope::InWorld))
                return;
        }
    }

    if (!found)
    {
        if (sBattlegroundMgr->isTesting())
            partyMemberStats.Initialize(packet.TargetGUID);
        else
        {
            partyMemberStats.MemberStats.GUID = packet.TargetGUID;
            partyMemberStats.MemberStats.Status = MEMBER_STATUS_OFFLINE;
        }
    }

    SendPacket(partyMemberStats.Write());
}

void WorldSession::HandleOptOutOfLoot(WorldPackets::Party::OptOutOfLoot& packet)
{
    GetPlayer()->SetPassOnGroupLoot(packet.PassOnLoot);
}

void WorldSession::HandleInitiateRolePoll(WorldPackets::Party::InitiateRolePoll& packet)
{
    if (!GetPlayer()->GetGroup())
        return;

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()) && !(group->GetGroupFlags() & GROUP_FLAG_EVERYONE_ASSISTANT))
    {
        SendPartyResult(PARTY_OP_INVITE, "", ERR_NOT_LEADER);
        return;
    }

    WorldPackets::Party::RolePollInform rolePollInform;
    rolePollInform.From = GetPlayer()->GetGUID();
    rolePollInform.PartyIndex = packet.PartyIndex;
    group->BroadcastPacket(rolePollInform.Write(), true);
}

void WorldSession::HandleClearRaidMarker(WorldPackets::Party::ClearRaidMarker& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Group* group = player->GetGroup();
    if (!group)
        return;

    if (group->isRaidGroup() && !group->IsLeader(player->GetGUID()) && !group->IsAssistant(player->GetGUID()))
        return;

    group->DeleteRaidMarker(packet.MarkerId);
}
