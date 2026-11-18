////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "GameTime.h"
#include "GameObject.h"
#include "ObjectAccessor.h"
#include "TemporarySummon.h"

// Camp at the Auchindoun entrance: defenders, knights, soulpriests and rangers salute all together, at the same second, every 20 seconds
struct npc_talador_camp_salute : public ScriptedAI
{
    explicit npc_talador_camp_salute(Creature* creature) : ScriptedAI(creature) { }

    uint32 lastSlot = 0;

    void Reset() override
    {
        lastSlot = GameTime::GetGameTimeMS() / 20000;
    }

    void UpdateAI(uint32 /*diff*/) override
    {
        if (UpdateVictim())
        {
            DoMeleeAttackIfReady();
            return;
        }

        uint32 slot = GameTime::GetGameTimeMS() / 20000;
        if (slot == lastSlot)
            return;

        lastSlot = slot;
        me->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
    }
};

// Laborers and peons of Fort Wrynn and Vol'jin's Pride: a short pause now and then, at a different moment for each,
// so that their hammers and picks do not all strike in time
struct npc_talador_camp_worker : public ScriptedAI
{
    explicit npc_talador_camp_worker(Creature* creature) : ScriptedAI(creature) { }

    uint32 workEmote = 0;
    uint32 pauseTimer = 0;
    uint32 resumeTimer = 0;

    void Reset() override
    {
        pauseTimer = urand(500, 8000);
        resumeTimer = 0;
    }

    void UpdateAI(uint32 diff) override
    {
        if (UpdateVictim())
        {
            DoMeleeAttackIfReady();
            return;
        }

        if (resumeTimer)
        {
            if (resumeTimer > diff)
            {
                resumeTimer -= diff;
                return;
            }
            resumeTimer = 0;
            me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, workEmote);
            pauseTimer = urand(10000, 25000);
            return;
        }

        if (pauseTimer > diff)
        {
            pauseTimer -= diff;
            return;
        }

        // only the ones put to work by their spawn; the others stand as they are
        workEmote = me->GetUInt32Value(UNIT_FIELD_EMOTE_STATE);
        if (!workEmote)
        {
            pauseTimer = 30000;
            return;
        }
        me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
        resumeTimer = urand(600, 2500);
    }
};

// Iron Horde Siege Engines of the Archenon siege yard ("Iron Them Out"): standing whole until sabotaged, then
// wrecked and burning for as long as they stay used (their auto close time), then whole again for the next player
enum SiegeEngine
{
    SIEGE_ENGINE_BURNING_VISUAL = 36045,
    SIEGE_ENGINE_LARGE          = 225757,
    SIEGE_ENGINE_LARGE_WRECK    = 17760,   // 6ih_ironhorde_siegeweapon01_destroyed
    SIEGE_ENGINE_SMALL_WRECK    = 14792,   // 6ih_ironhorde_warmachine02_broken
    NPC_GROMKAR_SIEGESMITH      = 80552,
    ACTION_ENGINE_BLOWN_UP      = 1,       // their SmartAI: catch fire, run off screaming, come back
    NPC_WORLD_TRIGGER           = 22515,
    SPELL_STRUCTURE_EXPLOSION   = 188744,  // Cosmetic - Structure Explosion
    WRECK_DELAY                 = 1200     // the wreck shows once the blast covers the engine
};

class go_talador_iron_horde_siege_engine : public GameObjectScript
{
public:
    go_talador_iron_horde_siege_engine() : GameObjectScript("go_talador_iron_horde_siege_engine") { }

    void OnGameObjectStateChanged(GameObject* go, uint32 state) override
    {
        if (state == GO_STATE_READY)
        {
            go->SetDisplayId(go->GetGOInfo()->displayId);
            go->SetUInt32Value(GAMEOBJECT_FIELD_STATE_SPELL_VISUAL_ID, 0);
            return;
        }

        // the engine blows up first, and only falls apart in the middle of the blast
        uint32 const wreck = go->GetEntry() == SIEGE_ENGINE_LARGE ? SIEGE_ENGINE_LARGE_WRECK : SIEGE_ENGINE_SMALL_WRECK;
        ObjectGuid const engineGuid = go->GetGUID();
        if (TempSummon* blast = go->SummonCreature(NPC_WORLD_TRIGGER, go->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 6000))
        {
            blast->CastSpell(blast, SPELL_STRUCTURE_EXPLOSION, true);
            blast->AddDelayedEvent(WRECK_DELAY, [blast, engineGuid, wreck]()
            {
                if (GameObject* engine = ObjectAccessor::GetGameObject(*blast, engineGuid))
                {
                    if (engine->GetGoState() == GO_STATE_READY)
                        return;
                    engine->SetDisplayId(wreck);
                    engine->SetUInt32Value(GAMEOBJECT_FIELD_STATE_SPELL_VISUAL_ID, SIEGE_ENGINE_BURNING_VISUAL);
                }
            });
        }
        else
        {
            go->SetDisplayId(wreck);
            go->SetUInt32Value(GAMEOBJECT_FIELD_STATE_SPELL_VISUAL_ID, SIEGE_ENGINE_BURNING_VISUAL);
        }

        // the smith mending it is caught in the blast
        std::list<Creature*> smiths;
        go->GetCreatureListWithEntryInGrid(smiths, NPC_GROMKAR_SIEGESMITH, 22.0f);
        for (Creature* smith : smiths)
            if (smith->IsAlive() && !smith->isInCombat() && smith->AI())
                smith->AI()->DoAction(ACTION_ENGINE_BLOWN_UP);
    }
};

void AddSC_wod_talador()
{
    RegisterCreatureAI(npc_talador_camp_salute);
    RegisterCreatureAI(npc_talador_camp_worker);
    new go_talador_iron_horde_siege_engine();
}
