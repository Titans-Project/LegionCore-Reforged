////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Pit Lord Vilemus, Greater Invasion Point: the boss and his Drain
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "invasion_point_bosses.h"

struct boss_pit_lor_vilemus : public ScriptedAI
{
    boss_pit_lor_vilemus(Creature* creature) : ScriptedAI(creature) {}

    EventMap events;

    void Reset() override
    {
        events.Reset();
        me->RemoveAllAuras();
    }

    void EnterCombat(Unit* /*unit*/) override
    {
        Talk(SAY_AGGRO);
        events.RescheduleEvent(EVENT_FEL_BREATH, 20000);
        events.RescheduleEvent(EVENT_STOMP, 13000);
        events.RescheduleEvent(EVENT_DRAIN, 22000);
    }

    void EnterEvadeMode() override
    {
        ScriptedAI::EnterEvadeMode();
    }

    void JustDied(Unit* /*who*/) override
    {
        me->RemoveAllAreaObjects();
        Talk(SAY_KILL);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
            return;

        events.Update(diff);

        if (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_FEL_BREATH:
                DoCast(SPELL_FEL_BREATH);
                events.RescheduleEvent(EVENT_FEL_BREATH, 19000);
                break;
            case EVENT_STOMP:
                DoCast(SPELL_STOMP);
                events.RescheduleEvent(EVENT_STOMP, 19000);
                break;
            case EVENT_DRAIN:
                DoCast(SPELL_DRAIN);
                events.RescheduleEvent(EVENT_DRAIN, 15400);
                break;
            default:
                break;
            }
        }
        DoMeleeAttackIfReady();
    }
};

//247739
class spell_drain : public AuraScript
{
    PrepareAuraScript(spell_drain);

    void OnTick(AuraEffect const* /*auraEffect*/)
    {
        Unit* target = GetTarget();
        Unit* caster = GetCaster();
        if (!target)
            return;
        if (!caster)
            return;

        caster->CastSpell(target, SPELL_DRAIN_HP, true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_drain::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

void AddSC_invasion_point_boss_vilemus()
{
    RegisterCreatureAI(boss_pit_lor_vilemus);
    RegisterAuraScript(spell_drain);
}
