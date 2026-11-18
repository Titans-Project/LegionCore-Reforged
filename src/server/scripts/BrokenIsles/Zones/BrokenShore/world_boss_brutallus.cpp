////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Broken Shore: Brutallus, world boss
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "AreaTriggerAI.h"

// 117239
class boss_bs_brutallus : public CreatureScript
{
public:
    boss_bs_brutallus() : CreatureScript("boss_bs_brutallus") {}

    struct boss_bs_brutallusAI : public ScriptedAI
    {
        boss_bs_brutallusAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* unit) override
        {
            events.RescheduleEvent(EVENT_1, 19000); // 233484
            events.RescheduleEvent(EVENT_2, 30000); // 233566
            events.RescheduleEvent(EVENT_3, 20000); // 233515
        }


        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(233484);
                        events.RescheduleEvent(EVENT_1, 19000); // 233484
                        break;
                    case EVENT_2:
                        DoCast(233566);
                        events.RescheduleEvent(EVENT_2, 18000); // 233566
                        break;
                    case EVENT_3:
                        DoCast(233515);
                        events.RescheduleEvent(EVENT_3, 20+18000); // 233515
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_bs_brutallusAI(creature);
    }
};

//233484
class spell_meteor_slash : public SpellScript
{
    PrepareSpellScript(spell_meteor_slash);

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
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_meteor_slash::FilterTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
        OnEffectHitTarget += SpellEffectFn(spell_meteor_slash::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

void AddSC_world_boss_brutallus()
{
    new boss_bs_brutallus();
    RegisterSpellScript(spell_meteor_slash);
}
