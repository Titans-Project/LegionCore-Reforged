////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Val'sharah: Shar'thos, world boss
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum eSpells
{
    SPELL_TAIL_LASH_SHARTOS = 215806,
    SPELL_CRY_OF_THE_TORMENTED = 216044,
    SPELL_DREAD_FLAME = 216043,
    SPELL_NIGHTMARE_BREATH = 215821,
    SPELL_BURNING_EARTH = 215856,
};

// 108678
class boss_shartos : public CreatureScript
{
public:
    boss_shartos() : CreatureScript("boss_shartos") {}

    struct boss_shartosAI : public ScriptedAI
    {
        boss_shartosAI(Creature* creature) : ScriptedAI(creature){}

        EventMap events;

        void Reset() override
        {
            events.Reset();
            me->RemoveAllAreaObjects();
        }

        void EnterCombat(Unit* unit) override
        {
            events.RescheduleEvent(EVENT_1, 5000);
            events.RescheduleEvent(EVENT_2, 45000);
            events.RescheduleEvent(EVENT_3, 20000);
            events.RescheduleEvent(EVENT_5, 24000);
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
                        DoCast(SPELL_TAIL_LASH_SHARTOS);
                        events.RescheduleEvent(EVENT_1, urand(7000, 10000));
                        break;
                    case EVENT_2:
                        DoCast(SPELL_CRY_OF_THE_TORMENTED);
                        events.RescheduleEvent(EVENT_2, 50000);
                        break;
                    case EVENT_3:
                        DoCast(SPELL_DREAD_FLAME);
                        events.RescheduleEvent(EVENT_3, 20000);
                        events.RescheduleEvent(EVENT_4, 10000);
                        break;
                    case EVENT_4:
                        DoCast(SPELL_NIGHTMARE_BREATH);
                        break;
                    case EVENT_5:
                        DoCast(SPELL_BURNING_EARTH);  // �� �������...
                        events.RescheduleEvent(EVENT_5, 25000);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_shartosAI(creature);
    }
};

void AddSC_world_boss_shartos()
{
    new boss_shartos();
}
