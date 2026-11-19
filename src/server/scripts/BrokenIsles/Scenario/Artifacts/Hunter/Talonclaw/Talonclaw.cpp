////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Hunter (Survival) artifact: Talonclaw, Spear of the Wild Gods (scenario 973, map 1511).
    The steps up to Dakarr's death run on SmartAI data. This file ends the quest:
    taking the spear, Dorro's farewell, the portal to Dalaran and the return credit.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SmartAI.h"
#include "GameObjectAI.h"
#include "Scenario.h"
#include "ScenarioMgr.h"

namespace
{
enum Talonclaw
{
    QUEST_TALONCLAW         = 40385,
    ITEM_TALONCLAW          = 128808,

    SCENARIO_TALONCLAW      = 973,
    STEP_TAKE_TALONCLAW     = 12,
    MAP_SCENARIO            = 1511,

    NPC_DORRO               = 100005,
    NPC_SPEAR_CARRIER       = 100310,
    GO_RETURN_PORTAL        = 9000575,

    SPELL_RETURN_TO_DALARAN = 190598,

    CREDIT_SPEAR_CLAIMED    = 100112,
    CREDIT_RETURNED         = 103798,

    ZONE_DALARAN            = 7502,

    // Length of the pickup scene (spell 205589)
    PRESENTATION_MS         = 9000,
    PORTAL_LIFETIME_SEC     = 300,

    TEXT_DORRO_FAREWELL     = 1,

    EVENT_FAREWELL          = 1,
    EVENT_PORTAL
};

Position const DorroPosition(3878.35f, 1636.92f, -0.0612079f, 1.89629f);

// A hunter who still has Talonclaw to bring home
bool IsClaimant(Player* player)
{
    return player && player->getClass() == CLASS_HUNTER && !player->GetQuestRewardStatus(QUEST_TALONCLAW) &&
        (player->GetQuestStatus(QUEST_TALONCLAW) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_TALONCLAW) == QUEST_STATUS_COMPLETE);
}

bool InFinalStep(GameObject* go)
{
    Scenario* scenario = sScenarioMgr->GetScenario(go->GetInstanceId());
    return go->GetMapId() == MAP_SCENARIO && scenario && scenario->GetScenarioId() == SCENARIO_TALONCLAW &&
        scenario->GetCurrentStep() >= STEP_TAKE_TALONCLAW;
}

// The pickup scene is over: the spear counts as reclaimed and Dorro comes to say goodbye
void FinishClaim(Player* player)
{
    if (!IsClaimant(player) || !player->HasItemCount(ITEM_TALONCLAW, 1, true))
        return;

    player->KilledMonsterCredit(CREDIT_SPEAR_CLAIMED);

    if (player->IsInWorld() && player->IsAlive() && player->GetMapId() == MAP_SCENARIO)
        player->SummonCreature(NPC_DORRO, DorroPosition.GetPositionX(), DorroPosition.GetPositionY(), DorroPosition.GetPositionZ(),
            DorroPosition.GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
}
}

class go_talonclaw_spear : public GameObjectScript
{
public:
    go_talonclaw_spear() : GameObjectScript("go_talonclaw_spear") { }

    // A copy bought or banked earlier makes the unique weapon unlootable: end the pickup as the loot would have
    bool OnGossipHello(Player* player, GameObject* go) override
    {
        if (!IsClaimant(player) || !player->HasItemCount(ITEM_TALONCLAW, 1, true) || !go->isSpawned() ||
            go->getLootState() == GO_JUST_DEACTIVATED || !InFinalStep(go))
            return false;

        go->SetLootState(GO_JUST_DEACTIVATED, player);
        return true;
    }

    void OnLootStateChanged(GameObject* go, uint32 state, Unit* unit) override
    {
        if (state != GO_JUST_DEACTIVATED)
            return;

        std::list<Creature*> carriers;
        go->GetCreatureListWithEntryInGrid(carriers, NPC_SPEAR_CARRIER, 10.0f);
        for (Creature* carrier : carriers)
            carrier->DespawnOrUnsummon(1000);

        Player* player = unit ? unit->ToPlayer() : nullptr;
        if (!IsClaimant(player) || !player->HasItemCount(ITEM_TALONCLAW, 1, true))
            return;

        player->AddDelayedEvent(PRESENTATION_MS, [player]() { FinishClaim(player); });
    }
};

struct npc_talonclaw_dorro : public SmartAI
{
    explicit npc_talonclaw_dorro(Creature* creature) : SmartAI(creature) { }

    void IsSummonedBy(Unit* summoner) override
    {
        SmartAI::IsSummonedBy(summoner);

        if (!summoner || !summoner->IsPlayer())
            return;

        _owner = summoner->GetGUID();
        _events.ScheduleEvent(EVENT_FAREWELL, 10000);
    }

    void UpdateAI(uint32 diff) override
    {
        SmartAI::UpdateAI(diff);
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_FAREWELL:
                    Talk(TEXT_DORRO_FAREWELL, _owner);
                    _events.ScheduleEvent(EVENT_PORTAL, 6000);
                    break;
                case EVENT_PORTAL:
                {
                    float const orientation = me->GetOrientation();
                    me->SummonGameObject(GO_RETURN_PORTAL, me->GetNearPosition(2.0f, float(M_PI_2)), orientation, 0.0f, 0.0f,
                        std::sin(orientation * 0.5f), std::cos(orientation * 0.5f), PORTAL_LIFETIME_SEC, _owner);
                    me->DespawnOrUnsummon(PORTAL_LIFETIME_SEC * IN_MILLISECONDS);
                    break;
                }
                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
    ObjectGuid _owner;
};

class go_talonclaw_return_portal : public GameObjectScript
{
public:
    go_talonclaw_return_portal() : GameObjectScript("go_talonclaw_return_portal") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/) override
    {
        if (IsClaimant(player) && player->GetQuestObjectiveData(QUEST_TALONCLAW, CREDIT_SPEAR_CLAIMED) >= 1)
            player->CastSpell(player, SPELL_RETURN_TO_DALARAN, true);

        return true;
    }
};

// "Return to Dalaran" is credited on arrival, whichever way the hunter got there
class player_talonclaw_return : public PlayerScript
{
public:
    player_talonclaw_return() : PlayerScript("player_talonclaw_return") { }

    void OnUpdateZone(Player* player, uint32 newZone, uint32 /*newArea*/) override
    {
        CreditReturn(player, newZone);
    }

    void OnLogin(Player* player) override
    {
        if (player)
            CreditReturn(player, player->GetZoneId());
    }

private:
    static void CreditReturn(Player* player, uint32 zone)
    {
        if (zone != ZONE_DALARAN || !IsClaimant(player) ||
            player->GetQuestObjectiveData(QUEST_TALONCLAW, CREDIT_SPEAR_CLAIMED) < 1 ||
            player->GetQuestObjectiveData(QUEST_TALONCLAW, CREDIT_RETURNED) >= 1)
            return;

        player->KilledMonsterCredit(CREDIT_RETURNED);
    }
};

void AddSC_Talonclaw()
{
    new go_talonclaw_spear();
    new go_talonclaw_return_portal();
    new player_talonclaw_return();
    RegisterCreatureAI(npc_talonclaw_dorro);
}
