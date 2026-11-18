////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Blasted Lands: Lord Kazzak, the original world boss of the Burning Legion (Vanilla)
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum LordKazzak
{
    SPELL_SHADOW_BOLT_VOLLEY    = 21341,
    SPELL_CLEAVE                = 20691,
    SPELL_THUNDERCLAP           = 26554,
    SPELL_VOID_BOLT             = 21066,
    SPELL_MARK_OF_KAZZAK        = 21056,
    SPELL_TWISTED_REFLECTION    = 21063,
    SPELL_CAPTURE_SOUL          = 21054,    // each kill heals him
    SPELL_FRENZY                = 28747,
};

enum LordKazzakEvents
{
    EVENT_SHADOW_BOLT_VOLLEY = 1,
    EVENT_CLEAVE,
    EVENT_THUNDERCLAP,
    EVENT_VOID_BOLT,
    EVENT_MARK_OF_KAZZAK,
    EVENT_TWISTED_REFLECTION,
    EVENT_FRENZY,
};

struct boss_lord_kazzak : public ScriptedAI
{
    boss_lord_kazzak(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() override
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_SHADOW_BOLT_VOLLEY, urand(6000, 10000));
        events.ScheduleEvent(EVENT_CLEAVE, 7000);
        events.ScheduleEvent(EVENT_THUNDERCLAP, urand(14000, 18000));
        events.ScheduleEvent(EVENT_VOID_BOLT, 30000);
        events.ScheduleEvent(EVENT_MARK_OF_KAZZAK, 25000);
        events.ScheduleEvent(EVENT_TWISTED_REFLECTION, 33000);
        events.ScheduleEvent(EVENT_FRENZY, 60000);
    }

    void KilledUnit(Unit* victim) override
    {
        if (victim->IsPlayer())
            DoCast(me, SPELL_CAPTURE_SOUL, true);
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
                case EVENT_SHADOW_BOLT_VOLLEY:
                    DoCastVictim(SPELL_SHADOW_BOLT_VOLLEY);
                    events.ScheduleEvent(EVENT_SHADOW_BOLT_VOLLEY, urand(4000, 6000));
                    break;
                case EVENT_CLEAVE:
                    DoCastVictim(SPELL_CLEAVE);
                    events.ScheduleEvent(EVENT_CLEAVE, urand(7000, 10000));
                    break;
                case EVENT_THUNDERCLAP:
                    DoCastVictim(SPELL_THUNDERCLAP);
                    events.ScheduleEvent(EVENT_THUNDERCLAP, urand(10000, 14000));
                    break;
                case EVENT_VOID_BOLT:
                    DoCastVictim(SPELL_VOID_BOLT);
                    events.ScheduleEvent(EVENT_VOID_BOLT, urand(15000, 18000));
                    break;
                case EVENT_MARK_OF_KAZZAK:
                    // on a caster, whose mana the mark eats away
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                        DoCast(target, SPELL_MARK_OF_KAZZAK);
                    events.ScheduleEvent(EVENT_MARK_OF_KAZZAK, 20000);
                    break;
                case EVENT_TWISTED_REFLECTION:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                        DoCast(target, SPELL_TWISTED_REFLECTION);
                    events.ScheduleEvent(EVENT_TWISTED_REFLECTION, 15000);
                    break;
                case EVENT_FRENZY:
                    DoCast(me, SPELL_FRENZY);
                    events.ScheduleEvent(EVENT_FRENZY, 30000);
                    break;
                default:
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

void AddSC_world_boss_lord_kazzak()
{
    RegisterCreatureAI(boss_lord_kazzak);
}
