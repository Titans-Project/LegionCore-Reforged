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

#include "Common.h"
#include "GameTime.h"
#include "Log.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "GossipDef.h"
#include "QuestDef.h"
#include "ObjectAccessor.h"
#include "Group.h"
#include "Battleground.h"
#include "BattlegroundAlteracValley.h"
#include "ScriptMgr.h"
#include "GameObjectAI.h"
#include "QuestPackets.h"
#include "GarrisonPackets.h"
#include "MovementPackets.h"
#include "MoveSpline.h"
#include "QuestData.h"
#include "GameEventMgr.h"

// The receiver may be on another map: the packet goes through the accessor, which only needs the guid
static bool SendQuestPushResult(ObjectGuid receiver, ObjectGuid sender, uint8 result)
{
    WorldPackets::Quest::QuestPushResultResponse data;
    data.SenderGUID = sender;
    data.Result = result;
    return ObjectAccessor::SendToPlayer(receiver, data.Write());
}

void WorldSession::HandleQuestGiverStatusQuery(WorldPackets::Quest::QuestGiverStatusQuery& packet)
{
    QuestGiverStatus questStatus = QuestGiverStatus::None;

    Object* questGiver = ObjectAccessor::GetObjectByTypeMask(*_player, packet.QuestGiverGUID, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT);
    if (!questGiver)
    {
        TC_LOG_DEBUG("network", "Error in CMSG_QUESTGIVER_STATUS_QUERY, called for non-existing questgiver (%s)", packet.QuestGiverGUID.ToString().c_str());
        return;
    }

    switch (questGiver->GetTypeId())
    {
        case TYPEID_UNIT:
        {
            TC_LOG_DEBUG("network", "WORLD: Received CMSG_QUESTGIVER_STATUS_QUERY for npc, %s", questGiver->GetGUID().ToString().c_str());
            if (!questGiver->ToCreature()->IsHostileTo(_player))       // do not show quest status to enemies
                questStatus = _player->GetQuestDialogStatus(questGiver);
            break;
        }
        case TYPEID_GAMEOBJECT:
        {
            TC_LOG_DEBUG("network", "WORLD: Received CMSG_QUESTGIVER_STATUS_QUERY for GameObject %s", questGiver->GetGUID().ToString().c_str());
            questStatus = _player->GetQuestDialogStatus(questGiver);
            break;
        }
        default:
            TC_LOG_ERROR("network", "QuestGiver called for unexpected type %u", questGiver->GetTypeId());
            break;
    }

    //inform client about status of quest
    _player->PlayerTalkClass->SendQuestGiverStatus(questStatus, packet.QuestGiverGUID);
}

void WorldSession::HandleQuestGiverHello(WorldPackets::Quest::QuestGiverHello& packet)
{
    Creature* creature = GetPlayer()->GetNPCIfCanInteractWith(packet.QuestGiverGUID,UNIT_NPC_FLAG_QUESTGIVER, UNIT_NPC_FLAG2_NONE);
    if (!creature)
        return;

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // Stop the npc if moving
    {
        uint32 pause = creature->GetMovementTemplate().GetInteractionPauseTimer();
        creature->PauseMovement(pause);
        creature->SetHomePosition(creature->GetPosition());
    }

    _player->PlayerTalkClass->ClearMenus();
    if (sScriptMgr->OnGossipHello(_player, creature))
        return;

    _player->PrepareGossipMenu(creature, creature->GetCreatureTemplate()->GossipMenuId, true);
    _player->SendPreparedGossip(creature);

    creature->AI()->sGossipHello(_player);
}

void WorldSession::HandleQuestGiverAcceptQuest(WorldPackets::Quest::QuestGiverAcceptQuest& packet)
{
    Object* object;
    if (!packet.QuestGiverGUID.IsPlayer())
        object = ObjectAccessor::GetObjectByTypeMask(*_player, packet.QuestGiverGUID, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT | TYPEMASK_ITEM);
    else
        object = ObjectAccessor::GetPlayer(*_player, packet.QuestGiverGUID);

#define CLOSE_GOSSIP_CLEAR_SHARING_INFO() \
    do { \
        _player->PlayerTalkClass->SendCloseGossip(); \
        _player->ClearQuestSharingInfo(); \
    } while (0)

    // no or incorrect quest giver
    if (!object)
    {
        CLOSE_GOSSIP_CLEAR_SHARING_INFO();
        return;
    }

    if (Player* playerQuestObject = object->ToPlayer())
    {
        // Various checks for player quest givers
        if ((!_player->GetPlayerSharingQuest().IsEmpty() && _player->GetPlayerSharingQuest() != packet.QuestGiverGUID) // Player tries to accept a shared quest from a different questiver
            || (_player != playerQuestObject) && !playerQuestObject->CanShareQuest(packet.QuestID))                    // Player tries to accept a quest that cannot be shared
        {
            CLOSE_GOSSIP_CLEAR_SHARING_INFO();
            return;
        }

        // Player tries to accept a quest from a player questgiver that he is not in a party/raid with
        if (!_player->IsInSameRaidWith(playerQuestObject))
        {
            CLOSE_GOSSIP_CLEAR_SHARING_INFO();
            return;
        }

        // Player tries to accept a quest from himself that cannot be taken. Such quests must be provided by a quest with QUEST_FLAGS_AUTOCOMPLETE prior to that
        if (_player->GetPopupQuestId())
        {
            if (_player->GetPopupQuestId() != packet.QuestID || _player->GetGUID() != packet.QuestGiverGUID)
            {
                CLOSE_GOSSIP_CLEAR_SHARING_INFO();
                return;
            }
        }
        else if (_player->GetGUID() == packet.QuestGiverGUID) // Covering all remaining possible cheat scenarios when trying to exploit quest sharing
        {
            CLOSE_GOSSIP_CLEAR_SHARING_INFO();
            return;
        }
    }
    else
    {
        if (!object->hasQuest(packet.QuestID))
        {
            CLOSE_GOSSIP_CLEAR_SHARING_INFO();
            return;
        }
    }

    // some kind of WPE protection
    if (!_player->CanInteractWithQuestGiver(object))
    {
        CLOSE_GOSSIP_CLEAR_SHARING_INFO();
        return;
    }

    if (Quest const* quest = sQuestDataStore->GetQuestTemplate(packet.QuestID))
    {
        // prevent cheating
        if (!GetPlayer()->CanTakeQuest(quest, true))
        {
            CLOSE_GOSSIP_CLEAR_SHARING_INFO();
            return;
        }

        if (!_player->GetPlayerSharingQuest().IsEmpty())
        {
            if (SendQuestPushResult(_player->GetPlayerSharingQuest(), _player->GetGUID(), QUEST_PARTY_MSG_ACCEPT_QUEST))
                _player->ClearQuestSharingInfo();
        }

        if (_player->CanAddQuest(quest, true))
        {
            _player->AddQuestAndCheckCompletion(quest, object);

            if (quest->HasFlag(QUEST_FLAGS_PARTY_ACCEPT))
            {
                if (Group* group = _player->GetGroup())
                {
                    group->ForEachMemberInMap(_player->GetMap(), [&](Player* player)
                    {
                        if (player == _player || !player->CanContact())     // not self
                            return;

                        if (player->CanTakeQuest(quest, true))
                        {
                            player->SetQuestSharingInfo(_player->GetGUID(), quest->GetQuestId());

                            //need confirmation that any gossip window will close
                            player->PlayerTalkClass->SendCloseGossip();

                            _player->SendQuestConfirmAccept(quest, player);
                        }
                    });
                }
            }

            _player->PlayerTalkClass->SendCloseGossip();

            if (quest->HasFlag(QUEST_FLAGS_LAUNCH_GOSSIP_ACCEPT))
            {
                auto launchGossip = [&](WorldObject* worldObject)
                {
                    _player->PlayerTalkClass->ClearMenus();
                    // a scripted quest giver builds its own menu, as when it is opened by hand
                    if (Creature* creature = worldObject->ToCreature())
                        if (sScriptMgr->OnGossipHello(_player, creature))
                            return;

                    _player->PrepareGossipMenu(worldObject, _player->GetDefaultGossipMenuForSource(worldObject), true);

                    // nothing left to offer but the quest just taken: its progress page would keep the quest window open
                    QuestMenu const& questMenu = _player->PlayerTalkClass->GetQuestMenu();
                    if (_player->PlayerTalkClass->GetGossipMenu().Empty() && questMenu.GetMenuItemCount() == 1 && questMenu.GetItem(0).QuestId == quest->GetQuestId())
                        return;

                    _player->SendPreparedGossip(worldObject);
                };

                if (Creature* creature = object->ToCreature())
                    launchGossip(creature);
                else if (GameObject* go = object->ToGameObject())
                    launchGossip(go);
            }

            if (quest->SourceSpellID)
                _player->CastSpell(_player, quest->SourceSpellID, true);

            // A quest has been accepted, reset popup follow quest ID because the quest has either been taken or another took its place
            // so the player has to take the follow quest at its corresponding creature questgiver instead.
            _player->SetPopupQuestId(0);

            return;
        }
    }

    CLOSE_GOSSIP_CLEAR_SHARING_INFO();

#undef CLOSE_GOSSIP_CLEAR_SHARING_INFO
}

void WorldSession::HandleQuestGiverQueryQuest(WorldPackets::Quest::QuestGiverQueryQuest& packet)
{
    // Verify that the guid is valid and is a questgiver or involved in the requested quest
    Object* object = ObjectAccessor::GetObjectByTypeMask(*_player, packet.QuestGiverGUID, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT | TYPEMASK_ITEM);
    if (!object || (!object->hasQuest(packet.QuestID) && !object->hasInvolvedQuest(packet.QuestID)))
    {
        _player->PlayerTalkClass->SendCloseGossip();
        return;
    }

    if (Quest const* quest = sQuestDataStore->GetQuestTemplate(packet.QuestID))
    {
        // not sure here what should happen to quests with QUEST_FLAGS_AUTOCOMPLETE
        // if this breaks them, add && object->IsItem() to this check
        // item-started quests never have that flag
        if (!_player->CanTakeQuest(quest, true))
            return;

        // Auto-accept skips HandleQuestGiverAcceptQuest, so its range check must be done here
        if (quest->IsAutoAccept() && _player->CanInteractWithQuestGiver(object) && _player->CanAddQuest(quest, true))
            _player->AddQuestAndCheckCompletion(quest, object);

        if (quest->IsAutoComplete())
            _player->PlayerTalkClass->SendQuestGiverRequestItems(quest, object->GetGUID(), _player->CanCompleteQuest(quest->GetQuestId()), true);
        else
        {
            _player->SetPopupQuestId(0);
            _player->PlayerTalkClass->SendQuestGiverQuestDetails(quest, object->GetGUID(), true, false);
        }
    }
}

void WorldSession::HandleQueryQuestInfo(WorldPackets::Quest::QueryQuestInfo& packet)
{
    if (!_player)
        return;

    if (Quest const* quest = sQuestDataStore->GetQuestTemplate(packet.QuestID))
        _player->PlayerTalkClass->SendQuestQueryResponse(quest->GetQuestId());
    else
    {
        WorldPackets::Quest::QueryQuestInfoResponse response;
        response.QuestID = packet.QuestID;
        SendPacket(response.Write());
    }
}

void WorldSession::HandleQueryTreasurePicker(WorldPackets::Quest::QueryTreasurePicker& packet)
{
    auto quest = sQuestDataStore->GetQuestTemplate(packet.QuestID);
    if (!quest || !sWorld->getBoolConfig(CONFIG_WORLD_QUEST))
        return;

    WorldPackets::Quest::QueryQuestRewardResponse response;
    response.QuestID = packet.QuestID;
    response.QuestRewardID = packet.QuestRewardID;

    if (quest->QuestInfoID)
    {
        auto wq = sQuestDataStore->GetWorldQuest(quest);
        if (!wq)
            return;

        response.Money = wq->Gold;
        uint32 ItemID = GetPlayer()->GetTeamId() == TEAM_HORDE ? wq->ItemList[CLASS_NONE].ItemIDH : wq->ItemList[CLASS_NONE].ItemIDA;
        uint32 ItemCount = wq->ItemList[CLASS_NONE].ItemCount;
        if (!ItemID)
        {
            ItemID = GetPlayer()->GetTeamId() == TEAM_HORDE ? wq->ItemList[GetPlayer()->getClass()].ItemIDH : wq->ItemList[GetPlayer()->getClass()].ItemIDA;
            ItemCount = wq->ItemList[GetPlayer()->getClass()].ItemCount;
        }
        if (ItemID)
        {
            WorldPackets::Quest::QueryQuestRewardResponse::ItemReward itemRew;
            itemRew.Item.ItemID = ItemID;
            itemRew.ItemCount = ItemCount;

            uint32 modTreeID = 0;
            auto bonusTree = GetPlayer()->GetWorldQuestBonusTreeMod(wq);
            modTreeID = std::get<0>(bonusTree);
            uint32 needLevel = GetPlayer()->GetWorldQuestRewardItemLevel(wq);

            itemRew.Item.ItemBonus.emplace();
            itemRew.Item.ItemBonus->Context = modTreeID;
            itemRew.Item.ItemBonus->BonusListIDs = sObjectMgr->GetItemBonusForLevel(ItemID, modTreeID, GetPlayer()->getLevel(), needLevel);

            response.Items.emplace_back(itemRew);
        }

        if (wq->CurrencyID)
            response.Currencys.emplace_back(WorldPackets::Quest::QuestCurrency(wq->CurrencyID, wq->CurrencyCount));
        if (wq->worldQuest->Currency)
            response.Currencys.emplace_back(WorldPackets::Quest::QuestCurrency(wq->worldQuest->Currency, wq->worldQuest->CurrencyCount));
    }
    else
    {
        auto bracket = 0;
        switch (packet.QuestID)
        {
            case 45536: // 2v2 Weekly Quest UI
                bracket = MS::Battlegrounds::BracketType::Arena2v2;
                break;
            case 45537: // 3v3 Weekly Quest UI
                bracket = MS::Battlegrounds::BracketType::Arena3v3;
                break;
            case 45538: // 10v10 Weekly Quest UI
                bracket = MS::Battlegrounds::BracketType::RatedBattleground;
                break;
            default:
                return;
        }

        auto data = GetPlayer()->GetItemDataForRatedQuest(bracket);
        uint32 modTreeID = std::get<0>(data);
        uint32 needLevel = std::get<1>(data);
        std::vector<uint32> BonusListIDs = sObjectMgr->GetItemBonusForLevel(147421, modTreeID, GetPlayer()->getLevel(), needLevel);

        WorldPackets::Quest::QueryQuestRewardResponse::ItemReward itemRew;
        itemRew.ItemCount = 1;
        itemRew.Item.ItemID = 150369;
        itemRew.Item.ItemBonus.emplace();
        itemRew.Item.ItemBonus->Context = modTreeID;
        itemRew.Item.ItemBonus->BonusListIDs = sObjectMgr->GetItemBonusForLevel(147421, modTreeID, GetPlayer()->getLevel(), needLevel);
        response.Items.emplace_back(itemRew);
    }

    GetPlayer()->SendDirectMessage(response.Write());
}

void WorldSession::HandleQuestGiverChooseReward(WorldPackets::Quest::QuestGiverChooseReward& packet)
{
    Quest const* quest = sQuestDataStore->GetQuestTemplate(packet.QuestID);
    if (!quest)
        return;

    // This is Real Item Entry, not slot id as pre 5.x
    if (packet.ItemChoiceID)
    {
        ItemTemplate const* rewardProto = sObjectMgr->GetItemTemplate(packet.ItemChoiceID);
        if (!rewardProto)
        {
            TC_LOG_ERROR("entities.player.cheat", "Error in CMSG_QUESTGIVER_CHOOSE_REWARD: player %s (%s) tried to get invalid reward item (Item Entry: %u) for quest %u (possible packet-hacking detected)", _player->GetName(), _player->GetGUID().ToString().c_str(), packet.ItemChoiceID, packet.QuestID);
            return;
        }

        bool itemValid = false;
        for (uint32 i = 0; i < quest->m_rewChoiceItemsCount; ++i)
        {
            if (quest->RewardChoiceItemId[i] && quest->RewardChoiceItemId[i] == uint32(packet.ItemChoiceID))
            {
                itemValid = true;
                break;
            }
        }

        if (!itemValid && quest->PackageID)
        {
            if (std::vector<QuestPackageItemEntry const*> const* questPackageItems = sDB2Manager.GetQuestPackageItems(quest->PackageID))
            {
                for (QuestPackageItemEntry const* questPackageItem : *questPackageItems)
                {
                    if (questPackageItem->ItemID != packet.ItemChoiceID)
                        continue;

                    if (_player->CanSelectQuestPackageItem(questPackageItem))
                    {
                        itemValid = true;
                        break;
                    }
                }
            }

            if (!itemValid)
            {
                if (std::vector<QuestPackageItemEntry const*> const* questPackageItems = sDB2Manager.GetQuestPackageItemsFallback(quest->PackageID))
                {
                    for (QuestPackageItemEntry const* questPackageItem : *questPackageItems)
                    {
                        if (questPackageItem->ItemID != packet.ItemChoiceID)
                            continue;

                        itemValid = true;
                        break;
                    }
                }
            }
        }

        if (!itemValid)
        {
            TC_LOG_ERROR("entities.player.cheat", "Error in CMSG_QUESTGIVER_CHOOSE_REWARD: player %s (%s) tried to get reward item (Item Entry: %u) wich is not a reward for quest %u (possible packet-hacking detected)", _player->GetName(), _player->GetGUID().ToString().c_str(), packet.ItemChoiceID, packet.QuestID);
            return;
        }
    }

    Object* object = _player;

    if (!quest->HasFlag(QUEST_FLAGS_AUTOCOMPLETE))
    {
        object = ObjectAccessor::GetObjectByTypeMask(*_player, packet.QuestGiverGUID, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT);
        if (!object || !object->hasInvolvedQuest(packet.QuestID))
            return;

        // some kind of WPE protection
        if (!_player->CanInteractWithQuestGiver(object))
            return;

    }
    // As in TrinityCore: an auto-complete quest skipped the whole first test, so a forged reward packet
    // claimed one meant for another class, race or level, or whose previous quest was not done
    if ((!_player->CanSeeStartQuest(quest) && _player->GetQuestStatus(packet.QuestID) == QUEST_STATUS_NONE) ||
        (_player->GetQuestStatus(packet.QuestID) != QUEST_STATUS_COMPLETE && !quest->IsAutoComplete()))
        return;

    if (_player->CanRewardQuest(quest, packet.ItemChoiceID, true))
    {
        _player->RewardQuest(quest, packet.ItemChoiceID, object, true);

        switch (object->GetTypeId())
        { 
        case TYPEID_UNIT:
        case TYPEID_PLAYER:
            {
                //For AutoSubmition was added plr case there as it almost same exclude AI script cases.
                Creature *creatureQGiver = object->ToCreature();
                if (!creatureQGiver || !(sScriptMgr->OnQuestReward(_player, creatureQGiver, quest, packet.ItemChoiceID)))
                {
                    // Send next quest
                    if (Quest const* nextQuest = _player->GetNextQuest(packet.QuestGiverGUID, quest))
                    {
                        if (nextQuest->IsAutoAccept() && _player->CanAddQuest(nextQuest, true) && _player->CanTakeQuest(nextQuest, true))
                        {
                            if (creatureQGiver)
                            {
                                sScriptMgr->OnQuestAccept(_player, creatureQGiver, nextQuest);
                                creatureQGiver->AI()->sQuestAccept(_player, nextQuest);
                            }

                            _player->AddQuestAndCheckCompletion(nextQuest, object);
                        }

                        // Previous quest has been rewarded via popup. The player may now accept the follow quest from himself.
                        if (object->IsPlayer() && quest->HasFlag(QUEST_FLAGS_AUTOCOMPLETE) && !nextQuest->IsAutoAccept())
                            _player->SetPopupQuestId(nextQuest->GetQuestId());
                        else
                            _player->SetPopupQuestId(0);

                        _player->PlayerTalkClass->SendQuestGiverQuestDetails(nextQuest, packet.QuestGiverGUID, true, false);
                    }

                    if (creatureQGiver)
                        creatureQGiver->AI()->sQuestReward(_player, quest, packet.ItemChoiceID);
                }
                break;
            }
        case TYPEID_GAMEOBJECT:
            if (!sScriptMgr->OnQuestReward(_player, dynamic_cast<GameObject*>(object), quest, packet.ItemChoiceID))
            {
                // Send next quest
                if (Quest const* nextQuest = _player->GetNextQuest(packet.QuestGiverGUID, quest))
                {
                    if (_player->CanTakeQuest(nextQuest, false))
                    {
                        if (nextQuest->IsAutoAccept() && _player->CanAddQuest(nextQuest, true))
                            _player->AddQuestAndCheckCompletion(nextQuest, object);

                        _player->SetPopupQuestId(0);
                        _player->PlayerTalkClass->SendQuestGiverQuestDetails(nextQuest, packet.QuestGiverGUID, true, false);
                    }
                }

                object->ToGameObject()->AI()->QuestReward(_player, quest, packet.ItemChoiceID);
            }
            break;
        default:
            break;
        }

        // AutoTake system
        _player->PrepareAreaQuest(_player->GetCurrentAreaID());
    }
    else 
        _player->PlayerTalkClass->SendQuestGiverOfferReward(quest, packet.QuestGiverGUID, true);
}

void WorldSession::HandleQuestGiverRequestReward(WorldPackets::Quest::QuestGiverRequestReward& packet)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_QUESTGIVER_REQUEST_REWARD npc = %s, quest = %u", packet.QuestGiverGUID.ToString().c_str(), packet.QuestID);

    Quest const* quest = sQuestDataStore->GetQuestTemplate(packet.QuestID);
    if (!quest)
        return;

    if (!quest->HasFlag(QUEST_FLAGS_AUTOCOMPLETE))
    {
        Object* object = ObjectAccessor::GetObjectByTypeMask(*_player, packet.QuestGiverGUID, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT);
        if (!object || !object->hasInvolvedQuest(packet.QuestID))
            return;

        // some kind of WPE protection
        if (!_player->CanInteractWithQuestGiver(object))
            return;
    }

    if (_player->CanCompleteQuest(packet.QuestID))
        _player->CompleteQuest(packet.QuestID);

    if (_player->GetQuestStatus(packet.QuestID) != QUEST_STATUS_COMPLETE)
        return;

    if (quest)
        _player->PlayerTalkClass->SendQuestGiverOfferReward(quest, packet.QuestGiverGUID, true);
}

void WorldSession::HandleQuestLogRemoveQuest(WorldPackets::Quest::QuestLogRemoveQuest& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (packet.Entry >= MAX_QUEST_LOG_SIZE)
        return;

    if (uint32 questId = player->GetQuestSlotQuestId(packet.Entry))
    {
        if (!player->TakeQuestSourceItem(questId, true))
            return;                                     // can't un-equip some items, reject quest cancel

        if (Quest const* quest = sQuestDataStore->GetQuestTemplate(questId))
        {
            if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED))
                player->RemoveTimedQuest(questId);

            for (QuestObjective const& obj : quest->Objectives)
            {
                if (obj.Type == QUEST_OBJECTIVE_COMPLETE_CRITERIA_TREE)
                    if (CriteriaTree const* tree = sAchievementMgr->GetCriteriaTree(obj.ObjectID))
                        for (CriteriaTree const* node : tree->Children)
                            player->GetAchievementMgr()->RemoveCriteriaProgress(node);
            }

            sScriptMgr->OnQuestAbandon(player, quest);
        }

        player->TakeQuestSourceItem(questId, true); // remove quest src item from player
        player->RemoveActiveQuest(questId);
        player->GetAchievementMgr()->RemoveTimedAchievement(CRITERIA_TIMED_TYPE_ITEM, questId);

        TC_LOG_DEBUG("network", "Player %u abandoned quest %u", player->GetGUIDLow(), questId);
    }

    _player->SetQuestSlot(packet.Entry, 0);

    _player->UpdateAchievementCriteria(CRITERIA_TYPE_QUEST_ABANDONED, 1);
}

void WorldSession::HandleQuestConfirmAccept(WorldPackets::Quest::QuestConfirmAccept& packet)
{
    if (_player->GetSharedQuestID() != uint32(packet.QuestID))
        return;

    // Read before the clear below: with an empty guid the sharer was never found
    ObjectGuid const sharerGuid = _player->GetPlayerSharingQuest();
    _player->ClearQuestSharingInfo();
    Quest const* quest = sQuestDataStore->GetQuestTemplate(packet.QuestID);
    if (!quest)
      return;

    // Reads the sharer's quest log, so only a sharer on the same map thread is reachable
    Player* originalPlayer = ObjectAccessor::GetPlayer(*_player, sharerGuid);
    if (!originalPlayer)
        return;

    if (!_player->IsInSameRaidWith(originalPlayer))
        return;

    if (!originalPlayer->IsActiveQuest(packet.QuestID))
        return;

    if (!_player->CanTakeQuest(quest, true))
        return;

    if (!_player->CanAddQuest(quest, true))
        return;

    _player->AddQuestAndCheckCompletion(quest, nullptr); // NULL, this prevent DB script from duplicate running

    if (quest->SourceSpellID)
        _player->CastSpell(_player, quest->SourceSpellID, true);
}

void WorldSession::HandleQuestgiverCompleteQuest(WorldPackets::Quest::QuestGiverCompleteQuest& packet)
{
    bool autoCompleteMode = packet.FromScript; // 0 - standart complete quest mode with npc, 1 - auto-complete mode

    TC_LOG_ERROR("network", "WORLD: Received CMSG_QUESTGIVER_COMPLETE_QUEST npc = %s, questId = %u self-complete: %u", packet.QuestGiverGUID.ToString().c_str(), packet.QuestID, autoCompleteMode ? 1 : 0);

    Quest const* quest = sQuestDataStore->GetQuestTemplate(packet.QuestID);
    if (!quest)
        return;

    if (autoCompleteMode && !quest->HasFlag(QUEST_FLAGS_AUTOCOMPLETE))
        return;

    Object* object = nullptr;
    if (autoCompleteMode)
        object = _player;
    else
        object = ObjectAccessor::GetObjectByTypeMask(*_player, packet.QuestGiverGUID, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT);

    if (!object)
        return;

    if (!autoCompleteMode)
    {
        if (!object->hasInvolvedQuest(packet.QuestID))
            return;

        // some kind of WPE protection
        if (!_player->CanInteractWithQuestGiver(object))
            return;
    }

    else
    {
        // Do not allow completing quests on other players.
        if (packet.QuestGiverGUID != _player->GetGUID())
            return;
    }

    if (!_player->CanSeeStartQuest(quest) && _player->GetQuestStatus(packet.QuestID) == QUEST_STATUS_NONE)
    {
        TC_LOG_ERROR("network", "Possible hacking attempt: Player %s [playerGuid: %s] tried to complete questId [entry: %u] without being in possession of the questId!",
            _player->GetName(), _player->GetGUID().ToString().c_str(), packet.QuestID);
        return;
    }

    if (_player->InBattleground())
        if (Battleground* bg = _player->GetBattleground())
            if (bg->GetTypeID() == MS::Battlegrounds::BattlegroundTypeId::BattlegroundAlteracValley)
                ((BattlegroundAlteracValley*)bg)->HandleQuestComplete(packet.QuestID, _player);

    if (_player->GetQuestStatus(packet.QuestID) != QUEST_STATUS_COMPLETE)
    {
        if (quest->IsRepeatable())
            _player->PlayerTalkClass->SendQuestGiverRequestItems(quest, packet.QuestGiverGUID, _player->CanCompleteRepeatableQuest(quest), false);
        else
            _player->PlayerTalkClass->SendQuestGiverRequestItems(quest, packet.QuestGiverGUID, _player->CanRewardQuest(quest, false), false);
    }
    else
    {
        if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_DELIVER))                  // some items required
            _player->PlayerTalkClass->SendQuestGiverRequestItems(quest, packet.QuestGiverGUID, _player->CanRewardQuest(quest, false), false);
        else                                            // no items required
            _player->PlayerTalkClass->SendQuestGiverOfferReward(quest, packet.QuestGiverGUID, true);
    }
}

void WorldSession::HandleQuestgiverCloseQuest(WorldPackets::Quest::QuestGiverCloseQuest& questGiverCloseQuest)
{
    if (_player->FindQuestSlot(questGiverCloseQuest.QuestID) >= MAX_QUEST_LOG_SIZE)
        return;

    Quest const* quest = sQuestDataStore->GetQuestTemplate(questGiverCloseQuest.QuestID);
    if (!quest)
        return;

    //sScriptMgr->OnQuestAcknowledgeAutoAccept(_player, quest); // @TODO : QuestScript
}

void WorldSession::HandlePushQuestToParty(WorldPackets::Quest::PushQuestToParty& packet)
{
    if (_player->GetQuestStatus(packet.QuestID) == QUEST_STATUS_NONE || _player->GetQuestStatus(packet.QuestID) == QUEST_STATUS_REWARDED)
        return;

    Quest const* quest = sQuestDataStore->GetQuestTemplate(packet.QuestID);
    if (!quest)
        return;

    Group* group = _player->GetGroup();
    if (!group)
        return;

    // Members on another map are updated by another map thread: never touch them from here
    group->ForEachMemberInMap(_player->GetMap(), [&](Player* player)
    {
        if (player == _player || !player->CanContact())
            return;

        if (!player->SatisfyQuestStatus(quest, false))
        {
            _player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_HAVE_QUEST);
            return;
        }

        if (player->GetQuestStatus(packet.QuestID) == QUEST_STATUS_COMPLETE)
        {
            _player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_FINISH_QUEST);
            return;
        }

        if (!player->CanTakeQuest(quest, false))
        {
            _player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_CANT_TAKE_QUEST);
            return;
        }

        if (!player->SatisfyQuestLog(false))
        {
            _player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_LOG_FULL);
            return;
        }

        if (!player->GetPlayerSharingQuest().IsEmpty())
        {
            _player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_BUSY);
            return;
        }

        _player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_SHARING_QUEST);

        if (quest->IsAutoAccept() && player->CanAddQuest(quest, true) && player->CanTakeQuest(quest, true))
            player->AddQuestAndCheckCompletion(quest, _player);

        if (quest->IsAutoComplete() && quest->IsRepeatable() && !quest->IsDailyOrWeekly())
            player->PlayerTalkClass->SendQuestGiverRequestItems(quest, _player->GetGUID(), player->CanCompleteRepeatableQuest(quest), true);
        else
        {
            player->SetPopupQuestId(0);
            player->SetQuestSharingInfo(_player->GetGUID(), quest->GetQuestId());
            player->PlayerTalkClass->SendQuestGiverQuestDetails(quest, player->GetGUID(), true, false);
        }
    });
}

void WorldSession::HandleQuestPushResult(WorldPackets::Quest::QuestPushResult& packet)
{
    if (!_player->GetPlayerSharingQuest().IsEmpty())
    {
        if (_player->GetPlayerSharingQuest() == packet.SenderGUID)
            SendQuestPushResult(_player->GetPlayerSharingQuest(), _player->GetGUID(), static_cast<QuestShareMessages>(packet.Result));

        _player->ClearQuestSharingInfo();
    }
}

void WorldSession::HandleQuestgiverStatusMultipleQuery(WorldPackets::Quest::QuestGiverStatusMultipleQuery& /*packet*/)
{
    _player->SendQuestGiverStatusMultiple();
}

void WorldSession::HandleAdventureJournalOpenQuest(WorldPackets::Quest::AdventureJournalOpenQuest& packet)
{
    auto const entry = sAdventureJournalStore.LookupEntry(packet.AdventureJournalID);
    if (!entry)
        return;

    if (!entry->QuestID)
        return;

    auto quest = sQuestDataStore->GetQuestTemplate(entry->QuestID);
    if (!quest)
        return;

    if (_player->hasQuest(entry->QuestID) || !_player->CanTakeQuest(quest, true))
        return;

    _player->SetPopupQuestId(quest->GetQuestId());
    _player->SetQuestSharingInfo(_player->GetGUID(), quest->GetQuestId());
    PlayerMenu menu(this);
    menu.SendQuestGiverQuestDetails(quest, _player->GetGUID(), true, false);
}

void WorldSession::HandleAdventureJournalStartQuest(WorldPackets::Quest::AdventureJournalStartQuest& packet)
{
    auto questID = packet.QuestID;
    auto player = GetPlayer();

    // Check Adventure Map POI first (Scouting Map / Class Hall map quests)
    bool isAdventureMapQuest = false;
    bool adventureMapPOIAvailable = false;
    for (AdventureMapPOIEntry const* entry : sAdventureMapPOIStore)
    {
        if (entry->Type != 1 || entry->QuestID != questID)
            continue;

        isAdventureMapQuest = true;
        if (sConditionMgr->IsPlayerMeetingCondition(player, entry->PlayerConditionID))
        {
            adventureMapPOIAvailable = true;
            break;
        }
    }

    if (isAdventureMapQuest)
    {
        if (!adventureMapPOIAvailable)
            return;

        auto quest = sQuestDataStore->GetQuestTemplate(questID);
        if (!quest)
            return;

        if (!player->getAdventureQuestID() && player->CanTakeQuest(quest, true) && player->CanAddQuest(quest, true))
        {
            player->setAdventureQuestID(quest->Id);
            player->AddQuest(quest, player);
            if (player->CanCompleteQuest(quest->Id))
                player->CompleteQuest(quest->Id);

            // one zone only: the other zones are told closed at once (the open map still shows them until reopened)
            for (AdventureMapPOIEntry const* entry : sAdventureMapPOIStore)
                if (entry->Type == 1)
                    SendPacket(WorldPackets::Quest::QueryAdventureMapPOIResponse(entry->ID, AdventureMapPOIAvailable(entry->ID)).Write());

            // and the map closes: only the client closes it, when the table it was opened at is out of reach. The table
            // is shown to this hunter alone far above him for an instant, then back in its place
            player->AddDelayedEvent(0, [player]() -> void
            {
                if (!player->IsInWorld())
                    return;

                auto showTableAt = [player](Creature* table, float z, uint32 splineStep)
                {
                    WorldPackets::Movement::MonsterMove packet;
                    packet.MoverGUID = table->GetGUID();
                    packet.Pos = Position(table->GetPositionX(), table->GetPositionY(), z, table->GetOrientation());
                    packet.SplineData.ID = table->movespline->GetId() + splineStep; // a new id each time, or the client drops the move
                    packet.SplineData.StopDistanceTolerance = 2;
                    player->SendDirectMessage(packet.Write());
                };

                std::list<Creature*> tables;
                player->GetCreatureListInGrid(tables, 10.0f);
                for (Creature* table : tables)
                {
                    if (!table->HasFlag(UNIT_FIELD_NPC_FLAGS2, UNIT_NPC_FLAG2_SHIPMENT_CRAFTER) || !player->HaveAtClient(table))
                        continue;

                    showTableAt(table, table->GetPositionZ() + 300.0f, 1);
                    ObjectGuid const tableGuid = table->GetGUID();
                    player->AddDelayedEvent(500, [player, tableGuid, showTableAt]() -> void
                    {
                        if (Creature* back = ObjectAccessor::GetCreature(*player, tableGuid))
                            showTableAt(back, back->GetPositionZ(), 2);
                    });
                }
            });
        }
        return;
    }

    // Fallback: check Adventure Journal (Adventure Guide suggested content)
    bool isJournalQuest = false;
    for (auto const& entry : sAdventureJournalStore)
    {
        if (entry && entry->QuestID == questID)
        {
            isJournalQuest = true;
            break;
        }
    }

    if (!isJournalQuest)
        return;

    auto quest = sQuestDataStore->GetQuestTemplate(questID);
    if (!quest)
        return;

    if (player->hasQuest(questID) || !player->CanTakeQuest(quest, true))
        return;

    // Show quest details — player must click Accept
    player->SetPopupQuestId(quest->GetQuestId());
    player->SetQuestSharingInfo(player->GetGUID(), quest->GetQuestId());
    PlayerMenu menu(this);
    menu.SendQuestGiverQuestDetails(quest, player->GetGUID(), true, false);
}

bool WorldSession::AdventureMapPOIAvailable(uint32 adventureMapPOIID)
{
    auto adventureMapPOIEntry = sAdventureMapPOIStore.LookupEntry(adventureMapPOIID);
    if (!adventureMapPOIEntry)
        return false;

    auto available = false;

    if (sConditionMgr->IsPlayerMeetingCondition(GetPlayer(), adventureMapPOIEntry->PlayerConditionID))
    {
        switch (adventureMapPOIEntry->Type)
        {
        case 1:
            if (auto quest = sQuestDataStore->GetQuestTemplate(adventureMapPOIEntry->QuestID))
                available = !_player->getAdventureQuestID() && _player->CanTakeQuest(quest, false);
            break;
        default:
            break;
        }
    }

    return available;
}

void WorldSession::HandleQueryAdventureMapPOI(WorldPackets::Quest::QueryAdventureMapPOI& packet)
{
    SendPacket(WorldPackets::Quest::QueryAdventureMapPOIResponse(packet.AdventureMapPOIID, AdventureMapPOIAvailable(packet.AdventureMapPOIID)).Write());
}

void WorldSession::HandleGarrisonRequestScoutingMap(WorldPackets::Garrison::GarrisonRequestScoutingMap& scoutingMap)
{
    WorldPackets::Garrison::GarrisonScoutingMapResult result;
    result.ID = scoutingMap.ID;
    result.Active = AdventureMapPOIAvailable(scoutingMap.ID);
    SendPacket(result.Write());
}

void WorldSession::HandleRequestWorldQuestUpdate(WorldPackets::Quest::RequestWorldQuestUpdate& /*packet*/)
{
    TC_LOG_DEBUG("worldquest", "HandleRequestWorldQuestUpdate");

    WorldPackets::Quest::WorldQuestUpdate response;
    if (_player->getLevel() >= MAX_LEVEL && sWorld->getBoolConfig(CONFIG_WORLD_QUEST))
    {
        if (WorldQuestMap const* worldQuestInfo = sQuestDataStore->GetWorldQuestMap())
        {
            for (auto const& itr : *worldQuestInfo)
            {
                for (auto const& iter : itr.second)
                {
                    WorldQuest const* worldQuest = &iter.second;
                    if (!worldQuest || _player->WorldQuestCompleted(worldQuest->QuestID))
                        continue;

                    // Don't send expired world quests to the client
                    if (worldQuest->ResetTime && worldQuest->ResetTime <= GameTime::GetGameTime())
                        continue;

                    if (sQuestDataStore->WorldLegionInvasionZoneID && sQuestDataStore->WorldLegionInvasionZoneID == worldQuest->quest->QuestSortID && !worldQuest->worldQuest->PrimaryID && !worldQuest->quest->IsLegionInvasion())
                        continue;

                    bool isInvasionWQ = worldQuest->quest->QuestInfoID == QUEST_INFO_LEGION_INVASION_WORLD_QUEST || worldQuest->quest->QuestInfoID == QUEST_INFO_LEGION_INVASION_ELITE_WORLD_QUEST;

                    if (QuestV2CliTaskEntry const* questTask = sQuestV2CliTaskStore.LookupEntry(worldQuest->QuestID))
                    {
                        // Skip ConditionID check for Legion Invasion WQs — condition 49404 from DB2
                        // references criteria not fully supported on private servers, and the server
                        // already validates invasion zone + player level
                        if (!isInvasionWQ && !sConditionMgr->IsPlayerMeetingCondition(_player, questTask->ConditionID))
                            continue;

                        // if (questTask->WorldStateExpressionID)
                        // {
                            // auto expressionEntry = sWorldStateExpressionStore.LookupEntry(questTask->WorldStateExpressionID);
                            // if (!expressionEntry || !expressionEntry->Eval(_player))
                                // continue;
                        // }

                        if (questTask->FiltMinSkillID && _player->GetSkillValue(questTask->FiltMinSkillID) < questTask->FiltMinSkillValue)
                            continue;

                        if (questTask->FiltMinLevel != -1 && _player->getLevel() < questTask->FiltMinLevel)
                            continue;

                        if (questTask->FiltMaxLevel != -1 && _player->getLevel() > questTask->FiltMaxLevel)
                            continue;

                        if (questTask->FiltRaces && (questTask->FiltRaces & _player->getRaceMask()) == 0)
                            continue;

                        if (questTask->FiltClasses && (questTask->FiltClasses & _player->getClassMask()) == 0)
                            continue;

                        bool needQuest = false;
                        bool canStart = true;
                        for (auto quest : questTask->FiltCompletedQuest)
                            if (quest)
                                needQuest = true;

                        if (needQuest)
                            for (auto slot : questTask->FiltCompletedQuest)
                                if (slot && _player->GetQuestStatus(slot) != QUEST_STATUS_REWARDED)
                                    canStart = false;

                        //if (questTask->StartItem && !_player->HasAura(questTask->StartItem))
                        //    continue;

                        if (needQuest && !canStart)
                            continue;
                    }

                    if (isInvasionWQ)
                        TC_LOG_DEBUG("worldquest", "HandleRequestWorldQuestUpdate: Invasion WQ %u (QuestInfoID %u) INCLUDED in response, zone %i",
                            worldQuest->QuestID, worldQuest->quest->QuestInfoID, worldQuest->quest->QuestSortID);

                    response.WorldQuestUpdates.emplace_back(worldQuest->StartTime, worldQuest->QuestID, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);
                }
            }
        }
    }

    TC_LOG_DEBUG("worldquest", "HandleRequestWorldQuestUpdate: sending %zu world quests to player %s",
        response.WorldQuestUpdates.size(), _player->GetName());

    SendPacket(response.Write());
}

void WorldSession::HandleRequestAreaPoiUpdate(WorldPackets::Quest::RequestAreaPoiUpdate& /*packet*/)
{
    WorldPackets::Quest::AreaPoiUpdate response;
    bool needSend = false;
    // For activate screen need cast 233539
    if (sQuestDataStore->WorldLegionInvasionZoneID)
    {
        // Legion invasion zone POIs (wrapper quests 45812/45838/45839/45840)
        Quest const* quest = sQuestDataStore->GetQuestTemplate(45812);
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(quest))
        {
            if (worldQuest->ResetTime > GameTime::GetGameTime())
            {
                response.Pois.emplace_back(worldQuest->StartTime, 5210, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);
                response.Pois.emplace_back(worldQuest->StartTime, 5272, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);
                needSend = true;
            }
        }
        quest = sQuestDataStore->GetQuestTemplate(45838);
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(quest))
        {
            if (worldQuest->ResetTime > GameTime::GetGameTime())
            {
                response.Pois.emplace_back(worldQuest->StartTime, 5175, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);
                response.Pois.emplace_back(worldQuest->StartTime, 5273, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);
                needSend = true;
            }
        }
        quest = sQuestDataStore->GetQuestTemplate(45839);
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(quest))
        {
            if (worldQuest->ResetTime > GameTime::GetGameTime())
            {
                response.Pois.emplace_back(worldQuest->StartTime, 5178, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);
                response.Pois.emplace_back(worldQuest->StartTime, 5270, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);
                needSend = true;
            }
        }
        quest = sQuestDataStore->GetQuestTemplate(45840);
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(quest))
        {
            if (worldQuest->ResetTime > GameTime::GetGameTime())
            {
                response.Pois.emplace_back(worldQuest->StartTime, 5177, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);
                response.Pois.emplace_back(worldQuest->StartTime, 5271, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);
                needSend = true;
            }
        }
    }
    // No entry for the Sentinax (AreaPOI 5252 to 5261, events 117 to 125): a timed POI hides its name on hover and only shows
    // a tooltip when it has a description, which the Sentinax has not. Its place comes from world state 13321 (ContributionMgr).
    // Spring Balloon Festival
    if (sGameEventMgr->IsActiveEvent(87))
    {
        GameEventData const* ged = &sGameEventMgr->GetEventMap()[87];
        response.Pois.emplace_back(ged->start, 5191, 0, 13062, 1);
        response.Pois.emplace_back(ged->start, 5192, 0, 13062, 1);
        response.Pois.emplace_back(ged->start, 5193, 0, 13062, 1);
        response.Pois.emplace_back(ged->start, 5195, 0, 13062, 1);
        response.Pois.emplace_back(ged->start, 5196, 0, 13062, 1);
        response.Pois.emplace_back(ged->start, 5197, 0, 13062, 1);
        response.Pois.emplace_back(ged->start, 5198, 0, 13062, 1);
        needSend = true;
    }
    // Glowcap Festival
    if (sGameEventMgr->IsActiveEvent(301))
    {
        GameEventData const* ged = &sGameEventMgr->GetEventMap()[301];
        response.Pois.emplace_back(ged->start, 5055, 0, 13107, 1);
        needSend = true;
    }
    // darkmoon
    if (sGameEventMgr->IsActiveEvent(75))
    {
        GameEventData const* ged = &sGameEventMgr->GetEventMap()[75];
        response.Pois.emplace_back(ged->start, 2704, 0, 6078, 1);
        response.Pois.emplace_back(ged->start, 2705, 0, 6078, 1);
        needSend = true;
    }
    // World Bosses Argus
    if (sGameEventMgr->IsActiveEvent(227))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49171)))
            response.Pois.emplace_back(worldQuest->StartTime, 5380, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    if (sGameEventMgr->IsActiveEvent(226))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49169)))
            response.Pois.emplace_back(worldQuest->StartTime, 5381, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    if (sGameEventMgr->IsActiveEvent(229))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49166)))
            response.Pois.emplace_back(worldQuest->StartTime, 5379, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    if (sGameEventMgr->IsActiveEvent(231))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49167)))
            response.Pois.emplace_back(worldQuest->StartTime, 5377, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    if (sGameEventMgr->IsActiveEvent(228))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49168)))
            response.Pois.emplace_back(worldQuest->StartTime, 5375, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    if (sGameEventMgr->IsActiveEvent(230))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49170)))
            response.Pois.emplace_back(worldQuest->StartTime, 5376, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    // Invasion Point Argus
    if (sGameEventMgr->IsActiveEvent(180))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49098)))
            response.Pois.emplace_back(worldQuest->StartTime, 5359, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    if (sGameEventMgr->IsActiveEvent(181))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49091)))
            response.Pois.emplace_back(worldQuest->StartTime, 5372, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    if (sGameEventMgr->IsActiveEvent(182))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49099)))
            response.Pois.emplace_back(worldQuest->StartTime, 5366, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    if (sGameEventMgr->IsActiveEvent(183))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(48982)))
            response.Pois.emplace_back(worldQuest->StartTime, 5373, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    if (sGameEventMgr->IsActiveEvent(184))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49096)))
            response.Pois.emplace_back(worldQuest->StartTime, 5368, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    if (sGameEventMgr->IsActiveEvent(185))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49091)))
            response.Pois.emplace_back(worldQuest->StartTime, 5360, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    if (sGameEventMgr->IsActiveEvent(186))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49098)))
            response.Pois.emplace_back(worldQuest->StartTime, 5370, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    if (sGameEventMgr->IsActiveEvent(187))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49097)))
            response.Pois.emplace_back(worldQuest->StartTime, 5350, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    if (sGameEventMgr->IsActiveEvent(188))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(48982)))
            response.Pois.emplace_back(worldQuest->StartTime, 5367, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    if (sGameEventMgr->IsActiveEvent(189))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49099)))
            response.Pois.emplace_back(worldQuest->StartTime, 5371, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    if (sGameEventMgr->IsActiveEvent(190))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49096)))
            response.Pois.emplace_back(worldQuest->StartTime, 5374, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }
    if (sGameEventMgr->IsActiveEvent(191))
    {
        if (WorldQuest const* worldQuest = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49097)))
            response.Pois.emplace_back(worldQuest->StartTime, 5369, worldQuest->Timer, worldQuest->VariableID, worldQuest->Value);

        needSend = true;
    }

    if (needSend)
        SendPacket(response.Write());
}
