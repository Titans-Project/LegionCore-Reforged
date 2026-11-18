////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Inquisitor Meto, Greater Invasion Point: the boss and his Seeds of Chaos
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "AreaTriggerAI.h"
#include "invasion_point_bosses.h"

struct boss_inquisitor_meto : public ScriptedAI
{
    boss_inquisitor_meto(Creature* creature) : ScriptedAI(creature) {}

    EventMap events;

    void Reset() override
    {
        events.Reset();
        me->RemoveAllAuras();
        me->RemoveAllAreaObjects();
    }

    void EnterEvadeMode() override
    {
        ScriptedAI::EnterEvadeMode();
    }

    void EnterCombat(Unit* /*unit*/) override
    {
        Talk(SAY_AGGRO);
        events.RescheduleEvent(EVENT_REAP, 8000);
        events.RescheduleEvent(EVENT_SOW, 5000);
        events.RescheduleEvent(EVENT_DEATH_FIELD, 12000);
        events.RescheduleEvent(EVENT_SEEDS_OF_CHAOS, 26000);
    }

    void JustDied(Unit* /*who*/) override
    {
        me->RemoveAllAreaObjects();
        Talk(SAY_KILL);
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
            case EVENT_REAP:
                DoCast(SPELL_REAP);
                events.RescheduleEvent(EVENT_REAP, 20000);
                break;
            case EVENT_SOW:
                DoCast(SPELL_SOW);
                DoCast(SPELL_SOW_DMG);
                events.RescheduleEvent(EVENT_SOW, 15000);
                break;
            case EVENT_DEATH_FIELD:
            {
                Position pos;
                float angle = me->GetAngle(me);
                for (uint8 i = 0; i < 9; i++)
                {
                    angle += 0.1f;
                    pos = me->GetNearPosition(35.0f, angle);
                    me->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_DEATH_FIELD_VISUAL, true);
                }
                DoCast(SPELL_DEATH_FIELD);
                events.RescheduleEvent(EVENT_DEATH_FIELD, 15000);
                break;
            }
            case EVENT_SEEDS_OF_CHAOS:
            {
                for (uint8 i = 0; i < 3; ++i)
                {
                    Position pos;
                    float angle = 6.28f / 3 * i;
                    pos = me->GetNearPosition(10.0f, angle);
                    me->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_SEEDS_OF_CHAOS, true);
                }
                events.RescheduleEvent(EVENT_SEEDS_OF_CHAOS, 30000);
                break;
            }
            default:
                break;
            }
        }
        DoMeleeAttackIfReady();
    }
};

struct at_seeds_of_chaos : AreaTriggerAI
{
    at_seeds_of_chaos(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) {}

    bool OnPlayerEnter = false;
    uint32 scaleTimer = 1000;

    void OnUnitEnter(Unit* player) override
    {
        if (!player->IsPlayer())
            return;

        OnPlayerEnter = true;
    }

    void OnUnitExit(Unit* player) override
    {
        if (!player->IsPlayer())
            return;

        OnPlayerEnter = false;
    }

    void ActionOnUpdate(GuidList& affectedPlayers) override
    {
        if (OnPlayerEnter)
        {
            at->SetSphereScale(-0.1f, 100, false);

            if (at->GetRadius() <= 0.7)
                at->Despawn();
        }
    }

    void OnUpdate(uint32 diff) override
    {
        if (scaleTimer)
        {
            if (scaleTimer <= diff)
            {
                scaleTimer = 1000;

                if (!OnPlayerEnter)
                    at->SetSphereScale(0.1f, 100, false);
            }
            else
                scaleTimer -= diff;
        }
    }
};

void AddSC_invasion_point_boss_meto()
{
    RegisterCreatureAI(boss_inquisitor_meto);
    RegisterAreaTriggerAI(at_seeds_of_chaos);
}
