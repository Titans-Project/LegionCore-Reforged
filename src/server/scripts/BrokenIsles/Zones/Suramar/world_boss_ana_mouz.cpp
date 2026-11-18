////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Suramar: Ana-Mouz, world boss
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum eSpells
{
    SPELL_FEL_GEYZER = 218823,
    SPELL_IMPISH_FLAMES = 218888,
    SPELL_GASEOUS_BREATH = 219255,
    SPELL_MOTHERS_EMBRACE = 219045,
    SPELL_REMOVE_CHARM = 227351,
};

// 109943
class boss_ana_muz : public CreatureScript
{
public:
    boss_ana_muz() : CreatureScript("boss_ana_muz") {}

    struct boss_ana_muzAI : public ScriptedAI
    {
        boss_ana_muzAI(Creature* creature) : ScriptedAI(creature), summons(me)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_REMOVE_CLIENT_CONTROL);
        }

        EventMap events;
        SummonList summons;

        void Reset() override
        {
            events.Reset();
            me->RemoveAllAreaObjects();
            summons.DespawnAll();
            DoCast(SPELL_REMOVE_CHARM);
        }

        void EnterCombat(Unit* unit) override
        {
            events.RescheduleEvent(EVENT_1, 4000); // SPELL_FEL_GEYZER
            events.RescheduleEvent(EVENT_3, 7000); // SPELL_GASEOUS_BREATH
            events.RescheduleEvent(EVENT_4, 31000); // SPELL_MOTHERS_EMBRACE
        }

        void JustDied(Unit* who) override
        {
            summons.DespawnAll();
            DoCast(SPELL_REMOVE_CHARM);
        }

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);
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
                        DoCast(SPELL_FEL_GEYZER);
                        events.RescheduleEvent(EVENT_1, 22000);
                        events.RescheduleEvent(EVENT_2, 3000);
                        break;
                    case EVENT_2:
                        DoCast(SPELL_IMPISH_FLAMES);
                        break;
                    case EVENT_3:
                        DoCast(SPELL_GASEOUS_BREATH);
                        events.RescheduleEvent(EVENT_3, 31000);
                        break;
                    case EVENT_4:
                        DoCast(SPELL_MOTHERS_EMBRACE);
                        events.RescheduleEvent(EVENT_4, 65000);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_ana_muzAI(creature);
    }
};

// 219045
class spell_mothers_embrace : public SpellScriptLoader
{
    public:
        spell_mothers_embrace() : SpellScriptLoader("spell_mothers_embrace") {}

        class spell_mothers_embrace_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mothers_embrace_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster() || !GetTarget() || GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_ENEMY_SPELL)
                    return;
                if (GetCaster()->IsAlive() && GetTarget()->IsAlive())
                    GetCaster()->CastSpell(GetTarget(), 219068, true);
            }

            void Register() override
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_mothers_embrace_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_mothers_embrace_AuraScript();
        }
};

void AddSC_world_boss_ana_mouz()
{
    new boss_ana_muz();
    new spell_mothers_embrace();
}
