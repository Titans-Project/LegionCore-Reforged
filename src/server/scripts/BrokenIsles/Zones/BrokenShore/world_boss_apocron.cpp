////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Broken Shore: Apocron, world boss
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "AreaTriggerAI.h"

// 121124
class boss_apocron : public CreatureScript
{
public:
    boss_apocron() : CreatureScript("boss_apocron") {}

    struct boss_apocronAI : public ScriptedAI
    {
        boss_apocronAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* unit) override
        {
            events.RescheduleEvent(EVENT_1, 23000); // 241458
            events.RescheduleEvent(EVENT_2, 35000); // 241518 (random)
            events.RescheduleEvent(EVENT_3, 11000); // 241498 script (241507)
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            if (auto victim = me->getVictim())
            {
                if (abs(victim->GetPositionZ() - me->GetPositionZ()) >= 4.0f)
                {
                    EnterEvadeMode();
                    return;
                }
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(241458);
                        events.RescheduleEvent(EVENT_1, 23000); // 241458
                        break;
                    case EVENT_2:
                        DoCast(241518);
                        events.RescheduleEvent(EVENT_2, 24000); // 241518 (random)
                        break;
                    case EVENT_3:
                        DoCast(241498);
                        me->AddDelayedEvent(2100, [this] () -> void
                        {
                            uint8 r = urand(5, 11);
                            for (uint8 i = 0; i < r; ++i)
                                DoCast(241507);
                        });

                        me->AddDelayedEvent(3100, [this] () -> void
                        {
                            uint8 r = urand(5, 11);
                            for (uint8 i = 0; i < r; ++i)
                                DoCast(241507);
                        });

                        me->AddDelayedEvent(4100, [this] () -> void
                        {
                            uint8 r = urand(8, 11);
                            for (uint8 i = 0; i < r; ++i)
                                DoCast(241507);
                        });

                        events.RescheduleEvent(EVENT_3, 16000); // 241498 script (241507)
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_apocronAI(creature);
    }
};

void AddSC_world_boss_apocron()
{
    new boss_apocron();
}
