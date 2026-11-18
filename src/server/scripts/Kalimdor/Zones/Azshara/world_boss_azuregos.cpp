////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Azshara: Azuregos, the blue dragon of the original world bosses (Vanilla)
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Azuregos
{
    SAY_TELEPORT                = 0,

    SPELL_MARK_OF_FROST         = 23182,    // left on those he kills: back near him, they freeze
    SPELL_AURA_OF_FROST         = 23186,
    SPELL_MANA_STORM            = 21097,
    SPELL_CHILL                 = 21098,
    SPELL_FROST_BREATH          = 21099,
    SPELL_REFLECTION            = 22067,
    SPELL_CLEAVE                = 8255,
    SPELL_ARCANE_VACUUM         = 21147,
    SPELL_FRENZY                = 23537,
};

enum AzuregosEvents
{
    EVENT_MANA_STORM = 1,
    EVENT_CHILL,
    EVENT_FROST_BREATH,
    EVENT_ARCANE_VACUUM,
    EVENT_REFLECTION,
    EVENT_CLEAVE,
};

struct boss_azuregos : public ScriptedAI
{
    boss_azuregos(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;
    bool frenzied = false;

    void Reset() override
    {
        events.Reset();
        frenzied = false;
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_MANA_STORM, urand(5000, 17000));
        events.ScheduleEvent(EVENT_CHILL, urand(10000, 30000));
        events.ScheduleEvent(EVENT_FROST_BREATH, urand(2000, 8000));
        events.ScheduleEvent(EVENT_ARCANE_VACUUM, 30000);
        events.ScheduleEvent(EVENT_REFLECTION, urand(15000, 30000));
        events.ScheduleEvent(EVENT_CLEAVE, 7000);
    }

    void KilledUnit(Unit* victim) override
    {
        if (victim->IsPlayer())
            victim->CastSpell(victim, SPELL_MARK_OF_FROST, true);
    }

    // a player he killed who comes back for more is frozen on the spot
    void MoveInLineOfSight(Unit* who) override
    {
        if (who->IsPlayer() && who->HasAura(SPELL_MARK_OF_FROST) && !who->HasAura(SPELL_AURA_OF_FROST) && me->IsWithinDistInMap(who, 50.0f))
            who->CastSpell(who, SPELL_AURA_OF_FROST, true);

        ScriptedAI::MoveInLineOfSight(who);
    }

    void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType /*dmgType*/) override
    {
        if (!frenzied && me->HealthBelowPctDamaged(26, damage))
        {
            frenzied = true;
            DoCast(me, SPELL_FRENZY, true);
        }
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
                case EVENT_MANA_STORM:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                        DoCast(target, SPELL_MANA_STORM);
                    events.ScheduleEvent(EVENT_MANA_STORM, urand(7500, 12500));
                    break;
                case EVENT_CHILL:
                    DoCastVictim(SPELL_CHILL);
                    events.ScheduleEvent(EVENT_CHILL, urand(13000, 25000));
                    break;
                case EVENT_FROST_BREATH:
                    DoCastVictim(SPELL_FROST_BREATH);
                    events.ScheduleEvent(EVENT_FROST_BREATH, urand(10000, 15000));
                    break;
                case EVENT_ARCANE_VACUUM:
                    // everyone fighting him is drawn to him, and his anger starts anew
                    Talk(SAY_TELEPORT);
                    DoCast(me, SPELL_ARCANE_VACUUM);
                    DoResetThreat();
                    events.ScheduleEvent(EVENT_ARCANE_VACUUM, 30000);
                    break;
                case EVENT_REFLECTION:
                    DoCast(me, SPELL_REFLECTION);
                    events.ScheduleEvent(EVENT_REFLECTION, urand(20000, 35000));
                    break;
                case EVENT_CLEAVE:
                    DoCastVictim(SPELL_CLEAVE);
                    events.ScheduleEvent(EVENT_CLEAVE, 7000);
                    break;
                default:
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

void AddSC_world_boss_azuregos()
{
    RegisterCreatureAI(boss_azuregos);
}
