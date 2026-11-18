////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Occularus, Greater Invasion Point: the boss and his Eye Sore
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "invasion_point_bosses.h"

struct boss_occularus : public ScriptedAI
{
    boss_occularus(Creature* creature) : ScriptedAI(creature) {}

    EventMap events;

    void Reset() override
    {
        events.Reset();
        me->RemoveAllAuras();
        me->RemoveAllAreaObjects();
    }

    void EnterCombat(Unit* /*unit*/) override
    {
        Talk(SAY_AGGRO);
        events.RescheduleEvent(EVENT_SEARING_GAZE, 7000);
        events.RescheduleEvent(EVENT_GUSHING_WOUND, 10000);
        events.RescheduleEvent(EVENT_LASH, 45500);
        events.RescheduleEvent(EVENT_EYE_SORE, 24000);
        events.RescheduleEvent(EVENT_PHANTASM, 27000);
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

    void SpellHitTarget(Unit* target, SpellInfo const* spell) override
    {
        if (spell->Id == SPELL_PHANTASM)
        {
            Position pos;
            float dist = 0.0f;

            for (uint8 i = 0; i < 4; ++i)
            {
                dist += 2.0f;
                pos = target->GetNearPosition(dist, 0.0f);
                me->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_PHANTASM_TRIGGER, true);
            }
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
            case EVENT_SEARING_GAZE:
                if (auto target = me->getVictim())
                    DoCast(target, SPELL_SEARING_GAZE, false);
                events.RescheduleEvent(EVENT_SEARING_GAZE, 10000);
                break;
            case EVENT_GUSHING_WOUND:
                if (auto target = me->getVictim())
                    DoCast(target, SPELL_GUSHING_WOUND, false);
                events.RescheduleEvent(EVENT_GUSHING_WOUND, 11000);
            case EVENT_LASH:
                if (auto target = me->getVictim())
                    DoCast(target, SPELL_LASH, false);
                events.RescheduleEvent(EVENT_LASH, 18000);
                break;
            case EVENT_EYE_SORE:
                DoCast(SPELL_EYE_SORE);
                break;
            case EVENT_PHANTASM:
                DoCast(SPELL_PHANTASM);
                events.RescheduleEvent(EVENT_PHANTASM, 39000);
                break;
            default:
                break;
            }
        }
        DoMeleeAttackIfReady();
    }
};

//247332
class spell_eye_sore : public SpellScript
{
    PrepareSpellScript(spell_eye_sore);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        uint32 count = 5;

        if (targets.size() > count)
            targets.resize(count);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_eye_sore::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

void AddSC_invasion_point_boss_occularus()
{
    RegisterCreatureAI(boss_occularus);
    RegisterSpellScript(spell_eye_sore);
}
