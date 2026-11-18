////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Mistress Alluradel, Greater Invasion Point: the boss, her Beguiling Charm and Sadist
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "invasion_point_bosses.h"

struct boss_mistress_alluradel : public ScriptedAI
{
    boss_mistress_alluradel(Creature* creature) : ScriptedAI(creature) {}

    EventMap events;
    uint32 checktimer;
    ObjectGuid tankGUID;

    void Reset() override
    {
        events.Reset();
        me->RemoveAllAuras();
        me->SetPower(POWER_ENERGY, 0);
        DoCast(SPELL_SADIST);
    }

    void EnterCombat(Unit* /*unit*/) override
    {
        Talk(SAY_AGGRO);
        events.RescheduleEvent(EVENT_HEART_BREAKER, 4000);
        events.RescheduleEvent(EVENT_FEL_LASH, 15000);
        events.RescheduleEvent(EVENT_BEGUILING_CHARM, 29000);
        events.RescheduleEvent(EVENT_PHANTASM, 33000);
    }

    void EnterEvadeMode() override
    {
        ScriptedAI::EnterEvadeMode();
    }

    void JustDied(Unit* /*who*/) override
    {
        Talk(2);
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
            case EVENT_HEART_BREAKER:
                DoCast(SPELL_HEART_BREAKER);
                events.RescheduleEvent(EVENT_HEART_BREAKER, 22000);
                break;
            case EVENT_FEL_LASH:
                DoCast(SPELL_FEL_LASH);
                events.RescheduleEvent(EVENT_FEL_LASH, 32000);
                break;
            case EVENT_BEGUILING_CHARM:
                ZoneTalk(1);
                DoCast(SPELL_BEGUILING_CHARM);
                events.RescheduleEvent(EVENT_BEGUILING_CHARM, 40000);
                break;
            default:
                break;
            }
        }

        if (me->HasAura(SPELL_SADIST))
        {
            checktimer = 0;
            if (checktimer <= diff)
            {
                if (Unit* victim = me->getVictim())
                {
                    if (victim->GetGUID() != tankGUID)
                    {
                        tankGUID = victim->GetGUID();
                        me->RemoveAura(SPELL_SADIST_PROC);
                        checktimer = 500;
                    }
                }
            }
            else checktimer -= diff;
        }
        DoMeleeAttackIfReady();
    }
};

//247549
class spell_beguiling_charm : public SpellScript
{
    PrepareSpellScript(spell_beguiling_charm);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        targets.remove_if([caster](WorldObject* object) -> bool
        {
            Unit* unit = object->ToUnit();
            if (!unit)
                return true;

            if (unit->isInFront(caster, float(M_PI / 2)))
                return false;

            return true;
        });
    }

    void HandleDummy(SpellEffIndex /*effIndex */)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;
        Unit* target = GetHitUnit();
        if (!target)
            return;

        caster->CastSpell(target, SPELL_BEGUILING_CHARM_AURA, false);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_beguiling_charm::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        OnEffectHitTarget += SpellEffectFn(spell_beguiling_charm::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

//247590
class spell_sadist : public AuraScript
{
    PrepareAuraScript(spell_sadist);

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        if (!caster)
            return;

        caster->CastSpell(caster, SPELL_SADIST_PROC, false);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_sadist::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

void AddSC_invasion_point_boss_alluradel()
{
    RegisterCreatureAI(boss_mistress_alluradel);
    RegisterSpellScript(spell_beguiling_charm);
    RegisterAuraScript(spell_sadist);
}
