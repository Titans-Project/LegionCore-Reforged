////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Highmountain: Drugon the Frostblood, world boss
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum eSpells
{
    SPELL_ICE_HURL = 219803,
    SPELL_SNOW_CRASH = 219493,
    SPELL_AVALANCHE = 219542,
    SPELL_SNOW_PLOW = 219601,
    SPELL_SNOW_PLOW_AURA = 219602,
    SPELL_SNOW_PLOW_STUN = 219610,
    SPELL_AVALANCHE_1 = 219687,
    SPELL_AVALANCHE_2 = 219689,
};

// 110378
class boss_drugon_the_frostblood : public CreatureScript
{
public:
    boss_drugon_the_frostblood() : CreatureScript("boss_drugon_the_frostblood") {}

    struct boss_drugon_the_frostbloodAI : public ScriptedAI
    {
        boss_drugon_the_frostbloodAI(Creature* creature) : ScriptedAI(creature), summons(me), timer_phase(0), inphase(false)
        {
        }

        EventMap events;
        SummonList summons;
        uint32 timer_phase;
        bool inphase;

        void Reset() override
        {
            events.Reset();
            me->RemoveAllAreaObjects();
            summons.DespawnAll();
            timer_phase = 3000;
            inphase = false;
        }

        void EnterCombat(Unit* unit) override
        {
            events.RescheduleEvent(EVENT_1, 6000);
            events.RescheduleEvent(EVENT_2, 11000);
            events.RescheduleEvent(EVENT_3, 20000);
            events.RescheduleEvent(EVENT_4, 33000);
        }

        void JustDied(Unit* who) override
        {
            summons.DespawnAll();
        }

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);
            if (summon->GetEntry() == 110452)
            {
                for (uint8 i = 0; i < urand(3,5); ++i)
                    summon->CastSpell(summon, SPELL_AVALANCHE_1);

                std::list<Player*> targets;
                targets.clear();
                GetPlayerListInGrid(targets, summon, 30.0f);
                if (!targets.empty())
                    for (std::list<Player*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                    {
                        if (urand(0, 1) == 1)
                            summon->CastSpell((*itr), SPELL_AVALANCHE_2);
                    }

            }
        }

        void SpellHitTarget(Unit* target, SpellInfo const* spell) override
        {
            if (spell->Id == SPELL_SNOW_PLOW)
            {
                me->StopAttack();
                AttackStart(target);
                inphase = true;
            }
            if (spell->Id == SPELL_SNOW_CRASH)
                target->CastSpell(target, 219497, true);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            if (inphase)
            {
                if (timer_phase <= diff)
                {
                    if (me->HasAura(SPELL_SNOW_PLOW_AURA))
                    {
                        if (auto target = me->getVictim())
                        {
                            if (me->IsWithinMeleeRange(target, me->GetAttackDist()))
                            {
                                me->RemoveAura(SPELL_SNOW_PLOW_AURA);
                                me->CastSpell(target, SPELL_SNOW_PLOW_STUN);
                                me->SetReactState(REACT_AGGRESSIVE);
                                Talk(1);
                                inphase = false;
                            }
                        }
                        else
                        {
                            me->RemoveAura(SPELL_SNOW_PLOW_AURA);
                            me->SetReactState(REACT_AGGRESSIVE);
                        }
                    }
                    else
                    {
                        inphase = false;
                        me->SetReactState(REACT_AGGRESSIVE);
                    }
                    timer_phase = 1000;
                }
                else
                    timer_phase -= diff;

                return;
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_ICE_HURL);
                        events.RescheduleEvent(EVENT_1, 10000);
                        break;
                    case EVENT_2:
                        DoCast(SPELL_SNOW_CRASH);
                        events.RescheduleEvent(EVENT_2, 11000);
                        break;
                    case EVENT_3:
                        if (auto target = SelectTarget(SELECT_TARGET_RANDOM, 0, 70.0f, true))
                            me->CastSpell(target, SPELL_AVALANCHE);
                        events.RescheduleEvent(EVENT_3, 20000);
                        break;
                    case EVENT_4:
                        if (auto target = SelectTarget(SELECT_TARGET_RANDOM, 0, 70.0f, true))
                            me->CastSpell(target, SPELL_SNOW_PLOW);
                        Talk(0);
                        inphase = true;
                        timer_phase = 3000;
                        events.RescheduleEvent(EVENT_4, 34000);
                        break;

                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_drugon_the_frostbloodAI(creature);
    }
};

void AddSC_world_boss_drugon()
{
    new boss_drugon_the_frostblood();
}
