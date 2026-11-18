////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Greater Invasion Points of Argus: what their bosses share, the fiery adds and the way into the rift
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "QuestData.h"
#include "invasion_point_bosses.h"

struct npc_fiery_trickster : ScriptedAI
{
    npc_fiery_trickster(Creature* creature) : ScriptedAI(creature) {}

    EventMap events;

    void Reset() override
    {
        events.Reset();
    }

    void IsSummonedBy(Unit* /*summoner*/) override
    {
        std::list<Player*> playerList;
        GetPlayerListInGrid(playerList, me, 300.0f);
        Trinity::Containers::RandomResizeList(playerList, 1);
        for (std::list<Player*>::iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
            if (!playerList.empty())
                me->AI()->AttackStart((*itr));

        events.RescheduleEvent(EVENT_1, 2000);
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;
        
        if (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_1:
                if (auto target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                    DoCast(target, SPELL_FEL_FIREBOLT, false);
                events.RescheduleEvent(EVENT_1, 2500);
                break;
            }
        }
    }
};

struct npc_felblaze_maniacr : ScriptedAI
{
    npc_felblaze_maniacr(Creature* creature) : ScriptedAI(creature)
    {
        me->SetReactState(REACT_PASSIVE);
    }

    EventMap events;

    void Reset() override
    {
        events.Reset();
    }

    void IsSummonedBy(Unit* /*summoner*/) override
    {
        DoCast(SPELL_SCORCHING_PRESENCE);
        me->GetMotionMaster()->MoveRandom(5.0f);
        events.RescheduleEvent(EVENT_1, 2500);
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);

        if (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_1:
                me->GetMotionMaster()->MoveRandom(5.0f);
                events.RescheduleEvent(EVENT_1, 2500);
                break;
            }
        }
    }
};

//247069
class spell_enter_rift : public SpellScript
{
    PrepareSpellScript(spell_enter_rift);

    SpellCastResult CheckCast()
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            if (player->FindNearestCreature(125849, 10.0f) && !player->isGameMaster())
            {
                if (WorldQuest const* wq = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(48982)))
                    if (!player->WorldQuestCompleted(48982) && player->HasAchieved(12085))
                        return SPELL_CAST_OK;
            }
            else if (player->FindNearestCreature(126547, 10.0f) && !player->isGameMaster())
            {
                if (WorldQuest const* wq = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49099)))
                    if (!player->WorldQuestCompleted(49099) && player->HasAchieved(12085))
                        return SPELL_CAST_OK;
            }
            else if (player->FindNearestCreature(126593, 10.0f) && !player->isGameMaster())
            {
                if (WorldQuest const* wq = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49096)))
                    if (!player->WorldQuestCompleted(49096) && player->HasAchieved(12085))
                        return SPELL_CAST_OK;
            }
            else if (player->FindNearestCreature(126499, 10.0f) && !player->isGameMaster())
            {
                if (WorldQuest const* wq = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49091)))
                    if (!player->WorldQuestCompleted(49091) && player->HasAchieved(12085))
                        return SPELL_CAST_OK;
            }
            else if (player->FindNearestCreature(126120, 10.0f) && !player->isGameMaster())
            {
                if (WorldQuest const* wq = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49098)))
                    if (!player->WorldQuestCompleted(49098) && player->HasAchieved(12085))
                        return SPELL_CAST_OK;
            }
            else if (player->FindNearestCreature(125863, 10.0f) && !player->isGameMaster())
            {
                if (WorldQuest const* wq = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(49097)))
                    if (!player->WorldQuestCompleted(49097) && player->HasAchieved(12085))
                        return SPELL_CAST_OK;
            }
            else if (player->FindNearestCreature(127531, 10.0f) && !player->isGameMaster())
            {
                if (player->GetQuestRewardStatus(48513))
                    return SPELL_CAST_OK;
            }
            else if (player->FindNearestCreature(127536, 10.0f) && !player->isGameMaster())
            {
                if (player->GetQuestRewardStatus(48513))
                    return SPELL_CAST_OK;
            }
            else if (player->FindNearestCreature(127533, 10.0f) && !player->isGameMaster())
            {
                if (player->GetQuestRewardStatus(48513))
                    return SPELL_CAST_OK;
            }
            else if (player->FindNearestCreature(127535, 10.0f) && !player->isGameMaster())
            {
                if (player->GetQuestRewardStatus(48513))
                    return SPELL_CAST_OK;
            }
            else if (player->FindNearestCreature(127532, 10.0f) && !player->isGameMaster())
            {
                if (player->GetQuestRewardStatus(48513))
                    return SPELL_CAST_OK;
            }
            else if (player->FindNearestCreature(127528, 10.0f) && !player->isGameMaster())
            {
                if (player->GetQuestRewardStatus(48513))
                    return SPELL_CAST_OK;
            }
            else
                return SPELL_CAST_OK;
        }

        SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_CANT_CAST_THAT_RIGHT_NOW);
            return SPELL_FAILED_CUSTOM_ERROR;
    }

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        Player* player = GetCaster()->ToPlayer();
        if (!player)
            return;

        // Invasion Points World Bosses
        if (player->FindNearestCreature(127531, 10.0f))
            player->TeleportTo(1779, -9893.78f, -4994.144f, 128.9254f, 0.94049f);
        else if (player->GetCurrentZoneID() == 9296)
            player->TeleportTo(1669, -2472.86f, 9164.73f, -158.6602f, 4.78481f);
        else if (player->FindNearestCreature(127536, 10.0f))
            player->TeleportTo(1779, 5276.58f, -9767.488f, 9.806942f, 1.15248f);
        else if (player->GetCurrentZoneID() == 9300)
            player->TeleportTo(1669, -2686.8990f, 8990.405f, -137.7017f, 0.73607f);
        else if (player->FindNearestCreature(127533, 10.0f))
            player->TeleportTo(1779, -10027.7265f, 2789.3244f, 18.655104f, 5.460755f);
        else if (player->GetCurrentZoneID() == 9298)
            player->TeleportTo(1669, -2921.87f, 9390.2724f, -161.97367f, 2.11761f);
        else if (player->FindNearestCreature(127535, 10.0f))
            player->TeleportTo(1779, 5702.83f, -1427.244f, 25.7616f, 4.14519f);
        else if (player->GetCurrentZoneID() == 9297)
            player->TeleportTo(1669, -3228.58f, 9115.177f, -164.4702f, 2.28962f);
        else if (player->FindNearestCreature(127532, 10.0f))
            player->TeleportTo(1779, -1398.3713f, 8368.603f, 90.93035f, 2.95033f);
        else if (player->GetCurrentZoneID() == 9299)
            player->TeleportTo(1669, 5529.8891f, 10086.2578f, -90.1988f, 0.95757f);
        else if (player->FindNearestCreature(127528, 10.0f))
            player->TeleportTo(1779, 4478.4077f, 6588.98f, 41.9538f, 3.08555f);
        else if (player->GetCurrentZoneID() == 9295)
            player->TeleportTo(1669, 2025.9199f, 1506.299f, 408.5905f, 5.33963f);

        // Insavion Points
        else if (player->FindNearestCreature(126547, 10.0f))
            player->TeleportTo(1779, -3791.71f, -8176.95f, 10.38279f, 0.967909f);
        else if (player->GetCurrentZoneID() == 9180)
        {
            if (sGameEventMgr->IsActiveEvent(182))
                player->TeleportTo(1669, 6200.979f, 9566.7851f, -80.97315f, 3.506402f);
            else if (sGameEventMgr->IsActiveEvent(189))
                player->TeleportTo(1669, -2229.58f, 9368.0429f, -62.1973f, 4.82296f);
        }
        else if (player->FindNearestCreature(125849, 10.0f))
            player->TeleportTo(1779, -4027.05f, -4973.6f, 129.74f, 1.55695f);
        else if (player->GetCurrentZoneID() == 9100)
        {
            if (sGameEventMgr->IsActiveEvent(183))
                player->TeleportTo(1669, -3359.614f, 9068.9697f, -169.6225f, 5.15407f);
            else if (sGameEventMgr->IsActiveEvent(188))
                player->TeleportTo(1669, 6343.798f, 10271.27f, 36.76093f, 4.09881f);
        }
        else if (player->FindNearestCreature(126120, 10.0f))
            player->TeleportTo(1779, 746.3156f, 727.73101f, 41.13044f, 3.67524f);
        else if (player->GetCurrentZoneID() == 9126)
        {
            if (sGameEventMgr->IsActiveEvent(180))
                player->TeleportTo(1669, 647.5516f, 1222.51f, 441.88827f, 0.25902f);
            else if (sGameEventMgr->IsActiveEvent(186))
                player->TeleportTo(1669, -2761.37f, 9217.5722f, -170.2415f, 4.81119f);
        }
        else if (player->FindNearestCreature(126499, 10.0f))
            player->TeleportTo(1779, -4156.705f, 652.0654f, 16.91975f, 5.70575f);
        else if (player->GetCurrentZoneID() == 9127)
        {
            if (sGameEventMgr->IsActiveEvent(181))
                player->TeleportTo(1669, -3257.6542f, 8791.98f, -151.2981f, 5.64684f);
            else if (sGameEventMgr->IsActiveEvent(185))
                player->TeleportTo(1669, 749.7797f, 2010.853f, 389.763f, 6.19852f);
        }
        else if (player->FindNearestCreature(126593, 10.0f))
            player->TeleportTo(1779, -1762.29f, -1423.4097f, 27.41837f, 4.59284f);
        else if (player->GetCurrentZoneID() == 9102)
        {
            if (sGameEventMgr->IsActiveEvent(190))
                player->TeleportTo(1669, -2457.0305f, 9379.4101f, -128.84635f, 4.981937f);
            else if (sGameEventMgr->IsActiveEvent(184))
                player->TeleportTo(1669, 5774.88f, 9258.8242f, -30.23626f, 2.43533f);
        }
        else if (player->FindNearestCreature(125863, 10.0f))
            player->TeleportTo(1779, -1405.4796f, 900.4859f, 89.52139f, 3.31297f);
        else if (player->GetCurrentZoneID() == 9128)
        {
            if (sGameEventMgr->IsActiveEvent(187))
                player->TeleportTo(1669, 1827.7225f, 1064.06f, 499.74401f, 1.76106f);
            else if (sGameEventMgr->IsActiveEvent(191))
                player->TeleportTo(1669, -2560.2529f, 8992.1601f, -139.5325f, 3.64879f);
        }
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_enter_rift::CheckCast);
        OnEffectHitTarget += SpellEffectFn(spell_enter_rift::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

void AddSC_invasion_point_bosses()
{
    RegisterCreatureAI(npc_fiery_trickster);
    RegisterCreatureAI(npc_felblaze_maniacr);
    RegisterSpellScript(spell_enter_rift);
}
