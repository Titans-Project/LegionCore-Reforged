////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Broken Shore: Si'vash, world boss
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "AreaTriggerAI.h"

// 117470
class boss_sivash : public CreatureScript
{
public:
    boss_sivash() : CreatureScript("boss_sivash") {}

    struct boss_sivashAI : public ScriptedAI
    {
        boss_sivashAI(Creature* creature) : ScriptedAI(creature), summons(me) {}

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
            events.RescheduleEvent(EVENT_1, 25000); // summons 233968
            events.RescheduleEvent(EVENT_2, 13000); // 241433
            events.RescheduleEvent(EVENT_3, 23000); // 233996 sea with help 233977
        }

        void JustDied(Unit* who) override
        {
            summons.DespawnAll();
        }

        void JustSummoned(Creature* summon) override
        {
            if (summon->GetEntry() != 117487)
                return;

            summons.Summon(summon);
            if (auto target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true))
                summon->AI()->AttackStart(target);
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
                        me->SummonCreature(117487, me->GetPositionX() + irand(-35, 35), me->GetPositionY() + irand(-35, 35), me->GetPositionZ());
                        me->SummonCreature(117487, me->GetPositionX() + irand(-35, 35), me->GetPositionY() + irand(-35, 35), me->GetPositionZ());
                        events.RescheduleEvent(EVENT_1, 25000);
                        break;
                    case EVENT_2:
                        DoCast(241433);
                        events.RescheduleEvent(EVENT_2, 13000);
                        break;
                    case EVENT_3:
                        DoCast(233996);
                        for (uint8 i = 0; i < urand(1, 2); ++i)
                            me->CastSpell(me->GetPositionX() + irand(-35, 35), me->GetPositionY() + irand(-35, 35), me->GetPositionZ(), 233970, true);
                        events.RescheduleEvent(EVENT_3, 23000);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_sivashAI(creature);
    }
};

// 13481
class areatrigger_at_tidal_wave : public AreaTriggerScript
{
    public:
        areatrigger_at_tidal_wave() : AreaTriggerScript("areatrigger_at_tidal_wave") {}

    struct areatrigger_at_tidal_waveAI : AreaTriggerAI
    {
        areatrigger_at_tidal_waveAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) {}

        bool CalculateSpline(Position const* pos, Position& startPos, Position& endPos, std::vector<Position>& path) override
        {
            endPos.m_positionX = pos->GetPositionX() + irand(-30, 30);
            endPos.m_positionY = pos->GetPositionY() + 50;
            endPos.m_positionZ = pos->GetPositionZ() - 5;
            if (Unit* caster = at->GetCaster())
            {
                startPos.m_positionX = caster->GetPositionX() + irand(-55, 55);
                startPos.m_positionY = caster->GetPositionY() - 90;
                startPos.m_positionZ = caster->GetPositionZ() - 7;
                return true;
            }
            else
                return false;
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new areatrigger_at_tidal_waveAI(areatrigger);
    }
};

void AddSC_world_boss_sivash()
{
    new boss_sivash();
    new areatrigger_at_tidal_wave();
}
