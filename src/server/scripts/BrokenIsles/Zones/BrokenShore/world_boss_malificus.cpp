////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Broken Shore: Malificus, world boss
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "AreaTriggerAI.h"

// 117303
class boss_malificus : public CreatureScript
{
public:
    boss_malificus() : CreatureScript("boss_malificus") {}

    struct boss_malificusAI : public ScriptedAI
    {
        boss_malificusAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* unit) override
        {
            events.RescheduleEvent(EVENT_1, 17000); // 234452
            events.RescheduleEvent(EVENT_2, 15000); // 233614
            events.RescheduleEvent(EVENT_3, 16000); // 233569
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
                        DoCast(234452);
                        events.RescheduleEvent(EVENT_1, 19000); // 234452
                        break;
                    case EVENT_2:
                        DoCast(233614);
                        events.RescheduleEvent(EVENT_2, 16000); // 233614
                        break;
                    case EVENT_3:
                        DoCast(233569);
                        events.RescheduleEvent(EVENT_3, 18000); // 233569
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_malificusAI(creature);
    }
};

void AddSC_world_boss_malificus()
{
    new boss_malificus();
}
