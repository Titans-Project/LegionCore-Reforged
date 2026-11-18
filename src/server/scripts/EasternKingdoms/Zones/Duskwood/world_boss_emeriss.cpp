////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Emeriss, one of the four Emerald Dragons, world boss of Duskwood, the Hinterlands, Feralas or Ashenvale in turn
*/

#include "World/EmeraldDragons/emerald_dragons.h"

/*
 * ---
 * --- Dragonspecific scripts and handling: EMERISS
 * ---
 */

enum EmerissTexts
{
    SAY_EMERISS_AGGRO               = 0,
    SAY_EMERISS_CAST_CORRUPTION     = 1,
};

enum EmerissSpells
{
    SPELL_PUTRID_MUSHROOM           = 24904,
    SPELL_CORRUPTION_OF_EARTH       = 24910,
    SPELL_VOLATILE_INFECTION        = 24928,
};

class boss_emeriss : public CreatureScript
{
    public:
        boss_emeriss() : CreatureScript("boss_emeriss") { }

        struct boss_emerissAI : public emerald_dragonAI
        {
            boss_emerissAI(Creature* creature) : emerald_dragonAI(creature)
            {
            }

            void Reset() override
            {
                _stage = 1;
                _Reset();
                emerald_dragonAI::Reset();
                events.ScheduleEvent(EVENT_VOLATILE_INFECTION, 12000);
            }

            void KilledUnit(Unit* who) override
            {
                if (who->GetTypeId() == TYPEID_PLAYER)
                    DoCast(who, SPELL_PUTRID_MUSHROOM, true);
                emerald_dragonAI::KilledUnit(who);
            }

            void EnterCombat(Unit* who) override
            {
                Talk(SAY_EMERISS_AGGRO);
                WorldBossAI::EnterCombat(who);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/, DamageEffectType dmgType) override
            {
                if (!HealthAbovePct(100 - 25 * _stage))
                {
                    Talk(SAY_EMERISS_CAST_CORRUPTION);
                    DoCast(me, SPELL_CORRUPTION_OF_EARTH, true);
                    ++_stage;
                }
            }

            void ExecuteEvent(uint32 const eventId) override
            {
                switch (eventId)
                {
                    case EVENT_VOLATILE_INFECTION:
                        DoCastVictim(SPELL_VOLATILE_INFECTION);
                        events.ScheduleEvent(EVENT_VOLATILE_INFECTION, 120000);
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
            return new boss_emerissAI(creature);
        }
};

void AddSC_world_boss_emeriss()
{
    new boss_emeriss();
}
