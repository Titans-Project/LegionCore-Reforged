////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Stormheim: Nithogg, world boss
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum eSpells
{
    SPELL_TAIL_LASH = 212835,
    SPELL_STORM_BREATH = 212852,
    SPELL_ELECTRICAL_STORM = 212867,
    SPELL_STATIC_CHARGE = 212887,
    SPELL_CRACKLING_JOLT = 212837,
};

// 107023
class boss_nithogg : public CreatureScript
{
public:
    boss_nithogg() : CreatureScript("boss_nithogg") {}

    struct boss_nithoggAI : public ScriptedAI
    {
        boss_nithoggAI(Creature* creature) : ScriptedAI(creature), summons(me), healthPct(0)
        {
        }

        EventMap events;
        SummonList summons;
        uint32 healthPct;

        void Reset() override
        {
            events.Reset();
            summons.DespawnAll();
            me->RemoveAllAreaObjects();
            healthPct = 80;
        }

        void EnterCombat(Unit* unit) override
        {
            events.RescheduleEvent(EVENT_1, urand(2000, 4000)); // SPELL_TAIL_LASH  скилл проверяет перед, а не зад. Должен зад
            events.RescheduleEvent(EVENT_2, urand(20000, 21000)); // SPELL_STORM_BREATH
            events.RescheduleEvent(EVENT_3, 30000); // SPELL_ELECTRICAL_STORM
            events.RescheduleEvent(EVENT_4, 41000); // SPELL_STATIC_CHARGE
        }

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);
            summon->SetReactState(REACT_PASSIVE);
            if (summon->GetEntry() == 107338)
                summon->GetMotionMaster()->MoveRandom(7.0f);
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType dmgType) override
        {
            if (me->HealthBelowPct(healthPct))
            {
                healthPct -= 20;
                events.RescheduleEvent(EVENT_5, 10);
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
                    case EVENT_1:
                        DoCast(SPELL_TAIL_LASH);
                        events.RescheduleEvent(EVENT_1, 5000);
                        break;
                    case EVENT_2:
                        DoCast(SPELL_STORM_BREATH);
                        events.RescheduleEvent(EVENT_2, urand(20000, 21000));
                        break;
                    case EVENT_3:
                        DoCast(SPELL_ELECTRICAL_STORM);
                        events.RescheduleEvent(EVENT_3, 30000);
                        break;
                    case EVENT_4:
                        {
                            for (int8 i = 0; i < 5; i++)
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40, true))
                                    if (!target->HasAura(SPELL_STATIC_CHARGE))
                                        me->CastSpell(target, SPELL_STATIC_CHARGE);
                            }
                            events.RescheduleEvent(EVENT_4, 41000);
                        }
                        break;
                    case EVENT_5:
                        DoCast(SPELL_CRACKLING_JOLT);
                        std::list<HostileReference*> threatlist = me->getThreatManager().getThreatList();
                        if (!threatlist.empty())
                        {
                            for (std::list<HostileReference*>::const_iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
                            {
                                    if (Player* pl = me->GetPlayer(*me, (*itr)->getUnitGuid()))
                                        Talk(0, pl->GetGUID());
                            }
                        }
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_nithoggAI(creature);
    }
};

// 107353
class npc_static_orb : public CreatureScript
{
public:
    npc_static_orb() : CreatureScript("npc_static_orb") {}

    struct npc_static_orbAI : public ScriptedAI
    {
        npc_static_orbAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetReactState(REACT_PASSIVE);
            CheckComplete = false;
            checkTimer = 2000;
            moveComplete = true;
            DoCast(212915);
        }

        ObjectGuid targetMove;
        bool moveComplete;
        uint32 moveTimer;
        bool CheckComplete;
        uint32 checkTimer;

        void UpdateAI(uint32 diff) override
        {
            if (!moveComplete)
            {
                if (moveTimer <= diff)
                {
                    Player* target = ObjectAccessor::GetPlayer(*me, targetMove);
                    if (!target)
                    {
                        me->DespawnOrUnsummon();
                        return;
                    }

                    if (me->GetDistance(target) < 3.0f)
                    {
                        moveComplete = true;
                        me->CastStop();
                        DoCast(target, 212948, true);
                        me->DespawnOrUnsummon(1000);
                    }
                    moveTimer = 1000;
                }
                else
                    moveTimer -= diff;
            }

            if (!CheckComplete)
            {
                if (checkTimer <= diff)
                {
                    std::list<Player*> targets;
                    targets.clear();
                    GetPlayerListInGrid(targets, me, 70.0f);
                    if (!targets.empty())
                    {
                        std::list<Player*>::const_iterator itr = targets.begin();
                        std::advance(itr, urand(0, targets.size() - 1));
                        if (*itr)
                            targetMove = (*itr)->GetGUID();

                        if ((*itr) && !(*itr)->HasAura(212943) && !(*itr)->HasAura(212887))
                        {
                            moveTimer = 1000;
                            moveComplete = false;
                            CheckComplete = true;
                            if (!me->HasUnitState(UNIT_STATE_STUNNED))
                            {
                                me->GetMotionMaster()->MoveFollow((*itr), PET_FOLLOW_DIST, (*itr)->GetFollowAngle());
                                me->CastSpell((*itr), 212943);
                            }
                        }
                        else
                        {
                            checkTimer = 500;
                        }
                    }
                }
                else
                    checkTimer -= diff;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_static_orbAI(creature);
    }
};

// 212887
class spell_static_charge : public SpellScriptLoader
{
    public:
        spell_static_charge() : SpellScriptLoader("spell_static_charge") {}

        class spell_static_charge_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_static_charge_AuraScript);

            void HandleEffectRemove(AuraEffect const * /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetCaster() && GetCaster()->ToCreature() && GetTarget())
                {
                    Unit* caster = GetCaster()->ToCreature();
                    if (caster && caster->GetEntry() == 107023)
                    {
                        caster->SummonCreature(107353, GetTarget()->GetPositionX(), GetTarget()->GetPositionY(), GetTarget()->GetPositionZ(), GetTarget()->GetOrientation());
                    }
                }
            }

            void Register() override
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_static_charge_AuraScript::HandleEffectRemove, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_static_charge_AuraScript();
        }
};

void AddSC_world_boss_nithogg()
{
    new boss_nithogg();
    new npc_static_orb();
    new spell_static_charge();
}
