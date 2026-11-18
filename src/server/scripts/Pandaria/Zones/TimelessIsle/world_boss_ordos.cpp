////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Timeless Isle: Ordos, Fire-God of the Yaungol, world boss atop the Timeless Isle, and the Ancient Flames he leaves
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Ordos
{
    NPC_ANCIENT_FLAME           = 72059,

    SAY_AGGRO                   = 0,
    SAY_BURNING_SOUL            = 1,
    SAY_ANCIENT_FLAME           = 3,
    SAY_ETERNAL_AGONY           = 4,
    SAY_DEATH                   = 5,

    SPELL_MAGMA_CRUSH           = 144688,
    SPELL_BURNING_SOUL          = 144689,
    SPELL_ANCIENT_FLAME         = 144695,
    SPELL_ETERNAL_AGONY         = 144696,   // after five minutes: the fight is lost
    SPELL_ANCIENT_FLAME_AURA    = 144691,

    DATA_DESPAWN                = 1,
};

enum OrdosEvents
{
    EVENT_MAGMA_CRUSH = 1,
    EVENT_BURNING_SOUL,
    EVENT_ANCIENT_FLAME,
    EVENT_ETERNAL_AGONY,
};

struct boss_ordos : public ScriptedAI
{
    boss_ordos(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() override
    {
        events.Reset();
        me->SetReactState(REACT_AGGRESSIVE);
    }

    void EnterCombat(Unit* /*who*/) override
    {
        Talk(SAY_AGGRO);
        events.ScheduleEvent(EVENT_MAGMA_CRUSH, 10000);
        events.ScheduleEvent(EVENT_BURNING_SOUL, 22000);
        events.ScheduleEvent(EVENT_ANCIENT_FLAME, 42000);
        events.ScheduleEvent(EVENT_ETERNAL_AGONY, 5 * MINUTE * IN_MILLISECONDS);
    }

    // the flames go out with him, whether he falls or the fight is dropped
    void PutOutFlames()
    {
        std::list<Creature*> flames;
        me->GetCreatureListWithEntryInGrid(flames, NPC_ANCIENT_FLAME, 200.0f);
        for (Creature* flame : flames)
            if (flame->AI())
                flame->AI()->SetData(DATA_DESPAWN, 1);
    }

    void JustDied(Unit* /*killer*/) override
    {
        Talk(SAY_DEATH);
        PutOutFlames();
    }

    void EnterEvadeMode() override
    {
        PutOutFlames();
        ScriptedAI::EnterEvadeMode();
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);
        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_MAGMA_CRUSH:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                        DoCast(target, SPELL_MAGMA_CRUSH);
                    events.ScheduleEvent(EVENT_MAGMA_CRUSH, 15000);
                    break;
                case EVENT_BURNING_SOUL:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                    {
                        DoCast(target, SPELL_BURNING_SOUL);
                        Talk(SAY_BURNING_SOUL);
                    }
                    events.ScheduleEvent(EVENT_BURNING_SOUL, 26000);
                    break;
                case EVENT_ANCIENT_FLAME:
                    DoCast(me, SPELL_ANCIENT_FLAME);
                    Talk(SAY_ANCIENT_FLAME);
                    events.ScheduleEvent(EVENT_ANCIENT_FLAME, 42000);
                    break;
                case EVENT_ETERNAL_AGONY:
                    // five minutes: his agony ends the fight, he stops fighting back and leaves later
                    DoCast(me, SPELL_ETERNAL_AGONY);
                    Talk(SAY_ETERNAL_AGONY);
                    events.Reset();
                    me->SetReactState(REACT_PASSIVE);
                    me->DespawnOrUnsummon(5 * MINUTE * IN_MILLISECONDS);
                    return;
                default:
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// the Ancient Flame Ordos summons: it burns where it stands until he tells it to go out
struct npc_ordos_ancient_flame : public ScriptedAI
{
    npc_ordos_ancient_flame(Creature* creature) : ScriptedAI(creature) { }

    void IsSummonedBy(Unit* /*summoner*/) override
    {
        me->SetReactState(REACT_PASSIVE);
        DoCast(me, SPELL_ANCIENT_FLAME_AURA, true);
    }

    void SetData(uint32 type, uint32 /*value*/) override
    {
        if (type == DATA_DESPAWN)
            me->DespawnOrUnsummon();
    }

    void UpdateAI(uint32 /*diff*/) override { }
};

void AddSC_world_boss_ordos()
{
    RegisterCreatureAI(boss_ordos);
    RegisterCreatureAI(npc_ordos_ancient_flame);
}
