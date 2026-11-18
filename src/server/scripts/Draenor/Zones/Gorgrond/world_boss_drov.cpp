////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Gorgrond: Drov the Ruiner, world boss
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "DB2Stores.h"
#include "ObjectAccessor.h"
#include "Player.h"

enum eSpells
{
    SPELL_COLOSSAL_SLAM = 175791,
    SPELL_GROUND_PUNCH = 155294,
    SPELL_CALL_OF_EARTH = 175827,
    SPELL_SUMMON_GORENS = 175911,
};

// 81252
class boss_drov : public CreatureScript
{
public:
    boss_drov() : CreatureScript("boss_drov") { }

    struct boss_drovAI : public ScriptedAI
    {
        boss_drovAI(Creature* creature) : ScriptedAI(creature), summons(me) {}

        EventMap events;
        SummonList summons;

        void Reset()
        {
            events.Reset();
            summons.DespawnAll();
        }

        void EnterCombat(Unit* unit)
        {
            events.RescheduleEvent(EVENT_1, 18000);
            events.RescheduleEvent(EVENT_2, 10000);
            events.RescheduleEvent(EVENT_3, 90000);
            events.RescheduleEvent(EVENT_4, 45000);
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
            if (Player* pl = summon->FindNearestPlayer(70.0f, true))
                summon->AI()->AttackStart(pl);
        }

        void OnApplyOrRemoveAura(uint32 spellId, AuraRemoveMode mode, bool apply)
        {
            if (spellId == SPELL_CALL_OF_EARTH && !apply)
				summons.DespawnEntry(88106);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_COLOSSAL_SLAM);
                        events.RescheduleEvent(EVENT_1, 18000);
                        break;
                    case EVENT_2:
                        DoCast(SPELL_GROUND_PUNCH);
                        events.RescheduleEvent(EVENT_2, 10000);
                        break;
                    case EVENT_3:
                        DoCast(SPELL_CALL_OF_EARTH);
                        events.RescheduleEvent(EVENT_3, 90000);
                        break;
                    case EVENT_4:
                        DoCast(SPELL_SUMMON_GORENS);
                        events.RescheduleEvent(EVENT_4, 45000);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_drovAI(creature);
    }
};

void AddSC_world_boss_drov()
{
    new boss_drov();
}
