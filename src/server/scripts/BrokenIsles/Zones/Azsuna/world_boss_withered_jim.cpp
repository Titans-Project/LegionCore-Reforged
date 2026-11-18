////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Azsuna: Withered J'im, world boss
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum eSpells
{
    SPELL_MORE_MORE_MORE = 223715,
    SPELL_NIGHTSHIFTED_BOLTS = 223623,
    SPELL_RESONANCE = 223614,
    SPELL_NIGHTSTABLE_ENERGY = 223689,
};

// 112350
class boss_withered_jim : public CreatureScript
{
public:
    boss_withered_jim() : CreatureScript("boss_withered_jim") {}

    struct boss_withered_jimAI : public ScriptedAI
    {
        boss_withered_jimAI(Creature* creature) : ScriptedAI(creature), summons(me), countclons(0)
        {
        }

        EventMap events;
        SummonList summons;
        uint8 countclons;

        void Reset() override
        {
            events.Reset();
            me->RemoveAllAreaObjects();
            summons.DespawnAll();
            countclons=0;
        }

        void EnterCombat(Unit* unit) override
        {
            events.RescheduleEvent(EVENT_1, 18000);
            events.RescheduleEvent(EVENT_2, 24000);
            events.RescheduleEvent(EVENT_3, 22000);
            if (me->GetEntry() == 102075)
                events.RescheduleEvent(EVENT_4, 30000);
            DoCast(223632); // AT
            DoCast(223599);
        }

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);
            DoZoneInCombat(me, 150.0f);
            if (summon->GetEntry() == 112350)
                summon->CastSpell(summon, 223599);
            if (summon->GetEntry() == 112342)
                summon->DespawnOrUnsummon(9000);
        }

        void JustDied(Unit* who) override
        {
            summons.DespawnAll();
        }

        void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType dmgType) override
        {
            if (attacker->GetTypeId() != TYPEID_PLAYER)
                return;

            if (attacker->GetPositionZ() >= 60.0f)
                me->Kill(attacker); //cheaters and others
        }


        void UpdateAI(uint32 diff) override
        {
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_NIGHTSHIFTED_BOLTS);
                        events.RescheduleEvent(EVENT_1, urand(25000, 30000));
                        break;
                    case EVENT_2:
                        DoCast(SPELL_RESONANCE);
                        events.RescheduleEvent(EVENT_1, urand(24000, 30000));
                        break;
                    case EVENT_3:
                        DoCast(SPELL_NIGHTSTABLE_ENERGY);
                        events.RescheduleEvent(EVENT_1, urand(29000, 34000));
                        break;
                    case EVENT_4:
                        ++countclons;
                        DoCast(SPELL_MORE_MORE_MORE);
                        if (countclons < 5)
                            events.RescheduleEvent(EVENT_4, 30000);
                        events.RescheduleEvent(EVENT_5, 1000);
                        break;
                    case EVENT_5:
                        if (me->HasAura(SPELL_MORE_MORE_MORE))
                            events.RescheduleEvent(EVENT_5, 1000);
                        else
                        {
                            if (auto add = me->SummonCreature(112350, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation()))
                                add->CastSpell(add, 223723);
                        }
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_withered_jimAI(creature);
    }
};

// 223614
class spell_resonance : public SpellScriptLoader
{
public:
    spell_resonance() : SpellScriptLoader("spell_resonance") {}

    class spell_resonance_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_resonance_AuraScript);

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* target = GetTarget();
            if (!target)
                return;
            target->CastSpell(target, 223616);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_resonance_AuraScript::OnProc, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_resonance_AuraScript();
    }
};

void AddSC_world_boss_withered_jim()
{
    new boss_withered_jim();
    new spell_resonance();
}
