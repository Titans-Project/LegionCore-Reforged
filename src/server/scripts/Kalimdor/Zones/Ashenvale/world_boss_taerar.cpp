////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Taerar, one of the four Emerald Dragons, world boss of Duskwood, the Hinterlands, Feralas or Ashenvale in turn
*/

#include "World/EmeraldDragons/emerald_dragons.h"

/*
 * ---
 * --- Dragonspecific scripts and handling: TAERAR
 * ---
 */

enum TaerarTexts
{
    SAY_TAERAR_AGGRO                = 0,
    SAY_TAERAR_SUMMON_SHADES        = 1,
};

enum TaerarSpells
{
    SPELL_BELLOWING_ROAR            = 22686,
    SPELL_SHADE                     = 24313,
    SPELL_SUMMON_SHADE_1            = 24841,
    SPELL_SUMMON_SHADE_2            = 24842,
    SPELL_SUMMON_SHADE_3            = 24843,
    SPELL_ARCANE_BLAST              = 24857,
};

uint32 const TaerarShadeSpells[] =
{
    SPELL_SUMMON_SHADE_1, SPELL_SUMMON_SHADE_2, SPELL_SUMMON_SHADE_3
};

class boss_taerar : public CreatureScript
{
    public:
        boss_taerar() : CreatureScript("boss_taerar") { }

        struct boss_taerarAI : public emerald_dragonAI
        {
            boss_taerarAI(Creature* creature) : emerald_dragonAI(creature)
            {
                _stage = 1;
                _shades = 0;
                _banished = false;
                _banishedTimer = 0;
            }

            void Reset() override
            {
                me->RemoveAurasDueToSpell(SPELL_SHADE);
                _stage = 1;

                _shades = 0;
                _banished = false;
                _banishedTimer = 0;

                _Reset();
                emerald_dragonAI::Reset();
                events.ScheduleEvent(EVENT_ARCANE_BLAST, 12000);
                events.ScheduleEvent(EVENT_BELLOWING_ROAR, 30000);
            }

            void EnterCombat(Unit* who) override
            {
                Talk(SAY_TAERAR_AGGRO);
                emerald_dragonAI::EnterCombat(who);
            }

            void SummonedCreatureDies(Creature* /*summon*/, Unit* /*killer*/) override
            {
                --_shades;
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/, DamageEffectType dmgType) override
            {
                // At 75, 50 or 25 percent health, we need to activate the shades and go "banished"
                // Note: _stage holds the amount of times they have been summoned
                if (!_banished && !HealthAbovePct(100 - 25 * _stage))
                {
                    _banished = true;
                    _banishedTimer = 60000;

                    me->InterruptNonMeleeSpells(false);
                    me->StopAttack();

                    Talk(SAY_TAERAR_SUMMON_SHADES);

                    uint32 count = sizeof(TaerarShadeSpells) / sizeof(uint32);
                    for (uint32 i = 0; i < count; ++i)
                        DoCastVictim(TaerarShadeSpells[i], true);
                    _shades += count;

                    DoCast(SPELL_SHADE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE);

                    ++_stage;
                }
            }

            void ExecuteEvent(uint32 const eventId) override
            {
                switch (eventId)
                {
                    case EVENT_ARCANE_BLAST:
                        DoCast(SPELL_ARCANE_BLAST);
                        events.ScheduleEvent(EVENT_ARCANE_BLAST, urand(7000, 12000));
                        break;
                    case EVENT_BELLOWING_ROAR:
                        DoCast(SPELL_BELLOWING_ROAR);
                        events.ScheduleEvent(EVENT_BELLOWING_ROAR, urand(20000, 30000));
                        break;
                    default:
                        emerald_dragonAI::ExecuteEvent(eventId);
                        break;
                }
            }

            void UpdateAI(uint32 diff) override
            {
                if (!me->isInCombat())
                    return;

                if (_banished)
                {
                    // If all three shades are dead, OR it has taken too long, end the current event and get Taerar back into business
                    if (_banishedTimer <= diff || !_shades)
                    {
                        _banished = false;

                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE);
                        me->RemoveAurasDueToSpell(SPELL_SHADE);
                        me->SetReactState(REACT_AGGRESSIVE);
                    }
                    // _banishtimer has not expired, and we still have active shades:
                    else
                        _banishedTimer -= diff;

                    // Update the events before we return (handled under emerald_dragonAI::UpdateAI(diff); if we're not inside this check)
                    events.Update(diff);

                    return;
                }

                emerald_dragonAI::UpdateAI(diff);
            }

        private:
            bool    _banished;                              // used for shades activation testing
            uint32  _banishedTimer;                         // counter for banishment timeout
            uint8   _shades;                                // keep track of how many shades are dead
            uint8   _stage;                                 // check which "shade phase" we're at (75-50-25 percentage counters)
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new boss_taerarAI(creature);
        }
};

void AddSC_world_boss_taerar()
{
    new boss_taerar();
}
