////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Suramar: Na'zak the Fiend, world boss
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum eSpells
{
    SPELL_CORRODING_SPRAY = 219349,
    SPELL_WEB_WRAP = 219861,
    SPELL_FOUNDATIONAL_COLLAPSE = 219591,
    SPELL_ABSORB_LEYSTONES = 219813,
    SPELL_LEY_INFUSION = 219836,
};

// 110321
class boss_nazak_the_fiend : public CreatureScript
{
public:
    boss_nazak_the_fiend() : CreatureScript("boss_nazak_the_fiend") {}

    struct boss_nazak_the_fiendAI : public ScriptedAI
    {
        boss_nazak_the_fiendAI(Creature* creature) : ScriptedAI(creature), summons(me) {}

        EventMap events;
        SummonList summons;

        void Reset() override
        {
            events.Reset();
            me->RemoveAllAreaObjects();
            summons.DespawnAll();
        }

        void EnterCombat(Unit* unit) override
        {
            events.RescheduleEvent(EVENT_1, 23000);
            events.RescheduleEvent(EVENT_2, 42000);
            events.RescheduleEvent(EVENT_3, 57000);
        }

        void JustDied(Unit* who) override
        {
            summons.DespawnAll();
        }

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);
        }

        void SpellHitTarget(Unit* /*target*/, SpellInfo const* spell) override
        {
            if (spell->Id == SPELL_ABSORB_LEYSTONES)
            {
                std::list<Creature*> adds;
                GetCreatureListWithEntryInGrid(adds, me, 110417, 60.0f);
                if (!adds.empty())
                    for (std::list<Creature*>::iterator itr = adds.begin(); itr != adds.end(); ++itr)
                    {
                        if ((*itr)->IsAlive())
                        {
                            (*itr)->DespawnOrUnsummon(1000);
                            DoCast(me, SPELL_LEY_INFUSION, true);
                        }
                    }
            }
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
                        DoCast(SPELL_CORRODING_SPRAY);
                        events.RescheduleEvent(EVENT_1, 23000);
                        break;
                    case EVENT_2:
                        DoCast(SPELL_WEB_WRAP);
                        events.RescheduleEvent(EVENT_2, 42000);
                        break;
                    case EVENT_3:
                        DoCast(SPELL_FOUNDATIONAL_COLLAPSE);
                        events.RescheduleEvent(EVENT_3, 57000);
                        events.RescheduleEvent(EVENT_4, 2000);
                        break;
                    case EVENT_4:
                        DoCast(SPELL_ABSORB_LEYSTONES);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_nazak_the_fiendAI(creature);
    }
};

// 219591
class spell_foundational_collapse : public SpellScriptLoader
{
    public:
        spell_foundational_collapse() : SpellScriptLoader("spell_foundational_collapse") {}

        class spell_foundational_collapse_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundational_collapse_AuraScript)

            void OnPereodic(AuraEffect const* aurEff)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                Position pos = GetCaster()->GetRandomNearPosition(27.0f);
                GetCaster()->CastSpell(pos, GetSpellInfo()->Effects[EFFECT_0]->TriggerSpell, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_foundational_collapse_AuraScript::OnPereodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundational_collapse_AuraScript();
        }
};

void AddSC_world_boss_nazak()
{
    new boss_nazak_the_fiend();
    new spell_foundational_collapse();
}
