////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Lethon, one of the four Emerald Dragons, world boss of Duskwood, the Hinterlands, Feralas or Ashenvale in turn
*/

#include "World/EmeraldDragons/emerald_dragons.h"

/*
 * ---
 * --- Dragonspecific scripts and handling: LETHON
 * ---
 *
 * TODO:
 * - NPC helper for spirit shades(?)
 *   - Spirit shade NPC moves towards Lethon and heals him if close enough (each shade heals for 15000 HP)
 * - Spell: Shadow bolt whirl casts needs custom handling (spellscript)
 */

enum LethonTexts
{
    SAY_LETHON_AGGRO                = 0,
    SAY_LETHON_DRAW_SPIRIT          = 1,
};

enum LethonSpells
{
    SPELL_DRAW_SPIRIT               = 24811,
    SPELL_SHADOW_BOLT_WHIRL         = 24834,
    SPELL_SPIRIT_SHADE_VISUAL       = 24908,
};

class boss_lethon : public CreatureScript
{
    public:
        boss_lethon() : CreatureScript("boss_lethon") { }

        struct boss_lethonAI : public emerald_dragonAI
        {
            boss_lethonAI(Creature* creature) : emerald_dragonAI(creature)
            {
            }

            void Reset() override
            {
                _stage = 1;
                _Reset();
                emerald_dragonAI::Reset();
                events.ScheduleEvent(EVENT_SHADOW_BOLT_WHIRL, 10000);
            }

            void EnterCombat(Unit* who) override
            {
                Talk(SAY_LETHON_AGGRO);
                WorldBossAI::EnterCombat(who);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/, DamageEffectType dmgType) override
            {
                if (!HealthAbovePct(100 - 25 * _stage))
                {
                    Talk(SAY_LETHON_DRAW_SPIRIT);
                    DoCast(me, SPELL_DRAW_SPIRIT);
                    ++_stage;
                }
            }

            void ExecuteEvent(uint32 const eventId) override
            {
                switch (eventId)
                {
                    case EVENT_SHADOW_BOLT_WHIRL:
                        DoCast(me, SPELL_SHADOW_BOLT_WHIRL, true);
                        events.ScheduleEvent(EVENT_SHADOW_BOLT_WHIRL, urand(15000, 30000));
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
            return new boss_lethonAI(creature);
        }
};

void AddSC_world_boss_lethon()
{
    new boss_lethon();
}
