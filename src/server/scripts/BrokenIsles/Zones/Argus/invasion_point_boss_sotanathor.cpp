////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Sotanathor, Greater Invasion Point: the boss, his Soul Cleave and Seed of Destruction
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "invasion_point_bosses.h"

struct boss_sotanathor : public ScriptedAI
{
    boss_sotanathor(Creature* creature) : ScriptedAI(creature) {}

    EventMap events;

    void Reset() override
    {
        events.Reset();
        me->RemoveAllAuras();
        me->RemoveAllAreaObjects();
    }

    void EnterEvadeMode() override
    {
        ScriptedAI::EnterEvadeMode();
    }

    void EnterCombat(Unit* /*unit*/) override
    {
        Talk(SAY_AGGRO);
        events.RescheduleEvent(EVENT_SOUL_CLEAVE, 17000);
        events.RescheduleEvent(EVENT_SILENCE, 23000);
        events.RescheduleEvent(EVENT_CAVITATION, 21000);
        events.RescheduleEvent(EVENT_SEED_OF_DESTRUCTION, 16000);
    }

    void JustDied(Unit* /*who*/) override
    {
        Talk(4);
    }

    void SpellHitTarget(Unit* target, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
        case SPELL_CAVITATION:
            if (target->IsPlayer() && target->isInCombat())
            {
                Position pos;
                float angle = 0.0f;
                for (uint8 i = 0; i < 4; i++)
                {
                    pos = target->GetNearPosition(5.0f, angle);
                    target->CastSpell(pos, SPELL_WAKE_OF_DESTRUCTION, true, 0, 0, me->GetGUID());
                    angle += 1.57f;
                }
            }
            break;
        case SPELL_SEED_OF_DESTRUCTION:
            Talk(1, target->GetGUID());
            break;
        }
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
            case EVENT_SOUL_CLEAVE:
                DoCast(SPELL_SOUL_CLEAVE);
                events.RescheduleEvent(SPELL_SOUL_CLEAVE, 28000);
                break;
            case EVENT_SILENCE:
                DoCast(SPELL_SILENCE);
                events.RescheduleEvent(EVENT_SILENCE, 28000);
                break;
            case EVENT_CAVITATION:
                ZoneTalk(2);
                DoCast(SPELL_CAVITATION);
                events.RescheduleEvent(EVENT_CAVITATION, 27000);
                break;
            case EVENT_SEED_OF_DESTRUCTION:
                DoCast(SPELL_SEED_OF_DESTRUCTION);
                events.RescheduleEvent(EVENT_SEED_OF_DESTRUCTION, 36000);
                break;
            default:
                break;
            }
        }
        DoMeleeAttackIfReady();
    }
};

//247410
class spell_soul_cleave : public SpellScript
{
    PrepareSpellScript(spell_soul_cleave);

    uint8 targetsCount = 0;

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targetsCount = targets.size();
    }

    void HandleDamage(SpellEffIndex /*effectIndex*/)
    {
        if (targetsCount)
            SetHitDamage(GetHitDamage() / targetsCount);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_soul_cleave::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
        OnEffectHitTarget += SpellEffectFn(spell_soul_cleave::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

//247437
class spell_argus_seed_of_destruction : public AuraScript
{
    PrepareAuraScript(spell_argus_seed_of_destruction);

    void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        if (!GetCaster() || !GetTarget() || !GetCaster()->isInCombat() || GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
            return;

        Position pos;
        float angle = 0.5f;
        for (uint8 i = 0; i < 4; ++i)
        {
            pos = GetTarget()->GetNearPosition(5.0f, angle);
            GetTarget()->CastSpell(pos, SPELL_WAKE_OF_DESTRUCTION, true);
            pos = GetTarget()->GetNearPosition(5.0f, -angle);
            GetTarget()->CastSpell(pos, SPELL_WAKE_OF_DESTRUCTION, true);
            angle += 1.0f;
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_argus_seed_of_destruction::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_invasion_point_boss_sotanathor()
{
    RegisterCreatureAI(boss_sotanathor);
    RegisterSpellScript(spell_soul_cleave);
    RegisterAuraScript(spell_argus_seed_of_destruction);
}
