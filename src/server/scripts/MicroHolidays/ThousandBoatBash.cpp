////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
*/

#include "GameEventMgr.h"
#include "OutdoorPvP.h"
#include "QuestData.h"

class OutdoorPvPThousandNeedles : public OutdoorPvP
{
public:
    OutdoorPvPThousandNeedles()
    {
        m_TypeId = OUTDOOR_PVP_THOUSAND_NEEDLES;
    }

    ~OutdoorPvPThousandNeedles() = default;

    bool SetupOutdoorPvP() override
    {
        RegisterZone(400);
        return true;
    }

    void HandlePlayerLeaveZone(ObjectGuid guid, uint32 zone) override
    {
        m_told.erase(guid);
        OutdoorPvP::HandlePlayerLeaveZone(guid, zone);
    }

    // the festival is read from the calendar, so the boats are handed out after a restart too and stop with it
    bool Update(uint32 diff) override
    {
        if (m_boatCheckTimer > diff)
        {
            m_boatCheckTimer -= diff;
            return true;
        }

        m_boatCheckTimer = 2000;
        if (!sGameEventMgr->IsActiveEvent(EVENT_THOUSAND_BOAT_BASH))
        {
            m_told.clear();
            return true;
        }

        // the day's boat is announced once on entering the zone; its aura only takes on the water
        ApplyOnEveryPlayerInZone([this](Player* player) -> void
        {
            if (m_told.insert(player->GetGUID()).second)
            {
                if (auto entry = sBroadcastTextStore.LookupEntry(BROADCAST_FREE_BOAT))
                {
                    std::string text = DB2Manager::GetBroadcastTextValue(entry, player->GetSession()->GetSessionDbLocaleIndex());
                    player->BossWhisper(text, LANG_UNIVERSAL, player->GetGUID());
                }
            }

            if (player->HasAura(SPELL_BOAT_DAY) || player->HasAura(SPELL_BOAT_DAY_ALT))
                return;

            if (player->IsInWater())
                player->CastSpell(player, SPELL_BOAT_DAY, true);
        });

        return true;
    }

private:
    enum
    {
        EVENT_THOUSAND_BOAT_BASH = 307,
        SPELL_BOAT_DAY           = 234458,
        SPELL_BOAT_DAY_ALT       = 234449,
        BROADCAST_FREE_BOAT      = 126573,
    };

    uint32 m_boatCheckTimer = 2000;
    std::set<ObjectGuid> m_told;
};

class OutdoorPvP_ThousandNeedles : public OutdoorPvPScript
{
public:
    OutdoorPvP_ThousandNeedles() : OutdoorPvPScript("outdoorpvp_thousandneedles") {}

    OutdoorPvP* GetOutdoorPvP() const override
    {
        return new OutdoorPvPThousandNeedles();
    }
};

class sceneTrigger_beach_ball : public SceneTriggerScript
{
public:
    sceneTrigger_beach_ball() : SceneTriggerScript("sceneTrigger_beach_ball") {}

    bool OnTrigger(Player* player, SpellScene const* trigger, std::string type) override
    {
        if (type == "credit")
            player->CastSpell(player, 234524, false);

        return true;
    }
};

//234449
class spell_bating : public AuraScript
{
    PrepareAuraScript(spell_bating);

    void OnProc(AuraEffect const* /*aurEff*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        LiquidData liquid_status;
        ZLiquidStatus Zliquid_status = caster->GetMap()->getLiquidStatus(caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ(), MAP_ALL_LIQUIDS, &liquid_status);

        if (Zliquid_status & (LIQUID_MAP_WATER_WALK | LIQUID_MAP_UNDER_WATER | LIQUID_MAP_IN_WATER))
            caster->CastSpell(caster, 234450, true);
        else
            caster->RemoveAura(234449);
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        caster->RemoveAura(234450);
    }

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        caster->RemoveAura(234458);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_bating::OnProc, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(spell_bating::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectApply += AuraEffectApplyFn(spell_bating::OnApply, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

//234458
class spell_boat_day : public AuraScript
{
    PrepareAuraScript(spell_boat_day);

    void OnProc(AuraEffect const* /*aurEff*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        LiquidData liquid_status;
        ZLiquidStatus Zliquid_status = caster->GetMap()->getLiquidStatus(caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ(), MAP_ALL_LIQUIDS, &liquid_status);

        if (Zliquid_status & (LIQUID_MAP_WATER_WALK | LIQUID_MAP_UNDER_WATER | LIQUID_MAP_IN_WATER))
            caster->CastSpell(caster, 234460, true);
        else
            caster->RemoveAura(234458);
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        caster->RemoveAura(234460);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_boat_day::OnProc, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(spell_boat_day::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

//114996
struct npc_fizzle_brassbolts : ScriptedAI
{
    explicit npc_fizzle_brassbolts(Creature* creature) : ScriptedAI(creature) {}

    void MoveInLineOfSight(Unit* who) override
    {
        if (!who->IsPlayer())
            return;

        if (me->GetDistance(who) > 15.0f)
            return;

        if (!sGameEventMgr->IsActiveEvent(307))
            return;

        if (Player* player = who->ToPlayer())
        {
            if (WorldQuest const* wq = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(45806)))
            {
                if (!player->WorldQuestCompleted(45806) && player->GetQuestStatus(45806) == QUEST_STATUS_INCOMPLETE)
                {
                    uint32 credit = player->GetQuestObjectiveData(45806, 118140);

                    if (credit != 1)
                    {
                        player->KilledMonsterCredit(118140);
                        player->CreateConversation(4349);
                    }
                }
            }
        }
    }
};

//118157
struct npc_tu_luak : ScriptedAI
{
    explicit npc_tu_luak(Creature* creature) : ScriptedAI(creature) {}

    void MoveInLineOfSight(Unit* who) override
    {
        if (!who->IsPlayer())
            return;

        if (me->GetDistance(who) > 15.0f)
            return;

        if (!sGameEventMgr->IsActiveEvent(307))
            return;

        if (Player* player = who->ToPlayer())
        {
            if (WorldQuest const* wq = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(45806)))
            {
                if (!player->WorldQuestCompleted(45806) && player->GetQuestStatus(45806) == QUEST_STATUS_INCOMPLETE)
                {
                    uint32 credit = player->GetQuestObjectiveData(45806, 118141);

                    if (credit != 1)
                    {
                        player->KilledMonsterCredit(118141);
                        player->CreateConversation(4352);
                    }
                }
            }
        }
    }
};

//118146
struct npc_theldurin_the_lost : ScriptedAI
{
    explicit npc_theldurin_the_lost(Creature* creature) : ScriptedAI(creature) {}

    void MoveInLineOfSight(Unit* who) override
    {
        if (!who->IsPlayer())
            return;

        if (me->GetDistance(who) > 15.0f)
            return;

        if (!sGameEventMgr->IsActiveEvent(307))
            return;

        if (Player* player = who->ToPlayer())
        {
            if (WorldQuest const* wq = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(45806)))
            {
                if (!player->WorldQuestCompleted(45806) && player->GetQuestStatus(45806) == QUEST_STATUS_INCOMPLETE)
                {
                    uint32 credit = player->GetQuestObjectiveData(45806, 118143);

                    if (credit != 1)
                    {
                        player->KilledMonsterCredit(118143);
                        player->CreateConversation(4351);
                    }
                }
            }
        }
    }
};

//118145
struct npc_ilthine_sunsong : ScriptedAI
{
    explicit npc_ilthine_sunsong(Creature* creature) : ScriptedAI(creature) {}

    void MoveInLineOfSight(Unit* who) override
    {
        if (!who->IsPlayer())
            return;

        if (me->GetDistance(who) > 30.0f)
            return;

        if (!sGameEventMgr->IsActiveEvent(307))
            return;

        if (Player* player = who->ToPlayer())
        {
            if (WorldQuest const* wq = sQuestDataStore->GetWorldQuest(sQuestDataStore->GetQuestTemplate(45806)))
            {
                if (!player->WorldQuestCompleted(45806) && player->GetQuestStatus(45806) == QUEST_STATUS_INCOMPLETE)
                {
                    uint32 credit = player->GetQuestObjectiveData(45806, 118144);

                    if (credit != 1)
                    {
                        player->KilledMonsterCredit(118144);
                        player->CreateConversation(4350);
                    }
                }
            }
        }
    }
};

// Climb Rope Ladder is a jump to a destination the ladder must give: the deck of the boat behind it, read from the
// collision of the boat models
class go_thousand_boat_bash_rope_ladder : public GameObjectScript
{
public:
    go_thousand_boat_bash_rope_ladder() : GameObjectScript("go_thousand_boat_bash_rope_ladder") {}

    bool OnGossipHello(Player* player, GameObject* go) override
    {
        float const baseZ = go->GetPositionZ();
        float bestZ = baseZ + 2.0f;
        float bestX = 0.0f;
        float bestY = 0.0f;
        bool found = false;

        for (float angle : { go->GetOrientation(), go->GetOrientation() + float(M_PI) })
        {
            for (float dist : { 2.0f, 3.5f, 5.0f })
            {
                float const x = go->GetPositionX() + std::cos(angle) * dist;
                float const y = go->GetPositionY() + std::sin(angle) * dist;
                float const z = go->GetMap()->GetHeight(go->GetPhases(), x, y, baseZ + 30.0f, true, 40.0f);
                if (z > bestZ && z < baseZ + 30.0f)
                {
                    bestZ = z;
                    bestX = x;
                    bestY = y;
                    found = true;
                }
            }
        }

        if (found)
            player->CastSpell(bestX, bestY, bestZ, SPELL_CLIMB_ROPE_LADDER, true);

        return true;
    }

private:
    enum
    {
        SPELL_CLIMB_ROPE_LADDER = 234602,
    };
};

void AddSC_ThousandBoatBash()
{
    new sceneTrigger_beach_ball();
    new OutdoorPvP_ThousandNeedles();
    new go_thousand_boat_bash_rope_ladder();
    RegisterAuraScript(spell_bating);
    RegisterAuraScript(spell_boat_day);
    RegisterCreatureAI(npc_fizzle_brassbolts);
    RegisterCreatureAI(npc_tu_luak);
    RegisterCreatureAI(npc_theldurin_the_lost);
    RegisterCreatureAI(npc_ilthine_sunsong);
}