////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Ysondre, one of the four Emerald Dragons, world boss of Duskwood, the Hinterlands, Feralas or Ashenvale in turn
*/

#include "World/EmeraldDragons/emerald_dragons.h"

/*
 * ---
 * --- Dragonspecific scripts and handling: YSONDRE
 * ---
 */

enum YsondreNPC
{
    NPC_DEMENTED_DRUID              = 15260,
};

enum YsondreTexts
{
    SAY_YSONDRE_AGGRO               = 0,
    SAY_YSONDRE_SUMMON_DRUIDS       = 1,
};

enum YsondreSpells
{
    SPELL_LIGHTNING_WAVE            = 24819,
    SPELL_SUMMON_DRUID_SPIRITS      = 24795,
};

class boss_ysondre : public CreatureScript
{
    public:
        boss_ysondre() : CreatureScript("boss_ysondre") { }

        struct boss_ysondreAI : public emerald_dragonAI
        {
            boss_ysondreAI(Creature* creature) : emerald_dragonAI(creature)
            {
            }

            void Reset() override
            {
                _stage = 1;
                _Reset();
                emerald_dragonAI::Reset();
                events.ScheduleEvent(EVENT_LIGHTNING_WAVE, 12000);
            }

            void EnterCombat(Unit* who) override
            {
                Talk(SAY_YSONDRE_AGGRO);
                WorldBossAI::EnterCombat(who);
            }

            // Summon druid spirits on 75%, 50% and 25% health
            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/, DamageEffectType dmgType) override
            {
                if (!HealthAbovePct(100 - 25 * _stage))
                {
                    Talk(SAY_YSONDRE_SUMMON_DRUIDS);

                    for (uint8 i = 0; i < 10; ++i)
                        DoCast(me, SPELL_SUMMON_DRUID_SPIRITS, true);
                    ++_stage;
                }
            }

            void ExecuteEvent(uint32 const eventId) override
            {
                switch (eventId)
                {
                    case EVENT_LIGHTNING_WAVE:
                        DoCastVictim(SPELL_LIGHTNING_WAVE);
                        events.ScheduleEvent(EVENT_LIGHTNING_WAVE, urand(10000, 20000));
                        break;
                    default:
                        emerald_dragonAI::ExecuteEvent(eventId);
                        break;
                }
            }

        private:
            uint8   _stage;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new boss_ysondreAI(creature);
        }
};

void AddSC_world_boss_ysondre()
{
    new boss_ysondre();
}
