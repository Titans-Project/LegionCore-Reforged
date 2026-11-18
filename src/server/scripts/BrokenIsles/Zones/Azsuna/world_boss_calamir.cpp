////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Azsuna: Calamir, world boss
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum eSpells
{
    SPELL_PHASE_FIRE = 217563,
    SPELL_PHASE_FROST = 217831,
    SPELL_PHASE_ARCANE = 217834,
    SPELL_BURNING_BOMB = 217874,
    SPELL_HOWLING_GALE = 217966,
    SPELL_ARCANOPULSE = 218005,
    SPELL_WRATHFUL_FLAMES = 217893,
    SPELL_ICY_COMET = 217919,
    SPELL_ARCANE_DESOLATION = 217986,
};

uint32 const phaseSpell[3]
{
    SPELL_PHASE_FIRE,
    SPELL_PHASE_FROST,
    SPELL_PHASE_ARCANE
};

// 109331
class boss_calamir : public CreatureScript
{
public:
    boss_calamir() : CreatureScript("boss_calamir") {}

    struct boss_calamirAI : public ScriptedAI
    {
        boss_calamirAI(Creature* creature) : ScriptedAI(creature)
        {
            timerphase = 100;
            phase = 0;
        }

        EventMap events;
        uint32 timerphase;
        uint8 phase;

        void Reset() override
        {
            events.Reset();
            me->RemoveAllAreaObjects();
        }

        void EnterCombat(Unit* unit) override
        {
            events.RescheduleEvent(EVENT_1, urand(5000, 6000)); // SPELL_1 13
            events.RescheduleEvent(EVENT_2, urand(15000, 16000)); // SPELL_2 25
        }

        void UpdateAI(uint32 diff) override
        {
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (timerphase <= diff)
            {
                DoCast(me, phaseSpell[phase], true);

                if (phase >= 2)
                    phase = 0;
                else
                    phase++;

                timerphase = 25000;

            } else timerphase -= diff;

            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        if (me->HasAura(SPELL_PHASE_FIRE) || me->HasAura(SPELL_PHASE_FROST) || me->HasAura(SPELL_PHASE_ARCANE))
                        {
                            if (me->HasAura(SPELL_PHASE_FIRE))
                                DoCast(SPELL_BURNING_BOMB);
                            else if (me->HasAura(SPELL_PHASE_FROST))
                                DoCast(SPELL_HOWLING_GALE);
                            // else if (me->HasAura(SPELL_PHASE_ARCANE))
                            //    DoCast(SPELL_ARCANOPULSE);

                            events.RescheduleEvent(EVENT_1, 13000);
                        }
                        else
                            events.RescheduleEvent(EVENT_1, 1000);

                        break;
                    case EVENT_2:
                         if (me->HasAura(SPELL_PHASE_FIRE) || me->HasAura(SPELL_PHASE_FROST) || me->HasAura(SPELL_PHASE_ARCANE))
                        {
                            if (me->HasAura(SPELL_PHASE_FIRE))
                                DoCast(SPELL_WRATHFUL_FLAMES);
                            else if (me->HasAura(SPELL_PHASE_FROST))
                                DoCast(SPELL_ICY_COMET);
                            else if (me->HasAura(SPELL_PHASE_ARCANE))
                                DoCast(SPELL_ARCANE_DESOLATION);

                            events.RescheduleEvent(EVENT_2, 25000);
                        }
                        else
                            events.RescheduleEvent(EVENT_2, 1000);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_calamirAI(creature);
    }
};

void AddSC_world_boss_calamir()
{
    new boss_calamir();
}
