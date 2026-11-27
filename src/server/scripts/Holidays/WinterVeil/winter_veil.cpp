////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "Cell.h"
#include "CellImpl.h"
#include "CharmInfo.h"
#include "CombatAI.h"
#include "CreatureTextMgr.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "ObjectMgr.h"
#include "ObjectVisitors.hpp"
#include "PassiveAI.h"
#include "PetAI.h"
#include "Player.h"
#include "PlayerDefines.h"
#include "ScriptedCreature.h"
#include "ScriptedFollowerAI.h"
#include "ScriptedEscortAI.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "Vehicle.h"
#include "World.h"
#include "QuestData.h"

/*####
## npc_winter_reveler
####*/

enum WinterReveler
{
    SPELL_MISTLETOE_DEBUFF       = 26218,
    SPELL_CREATE_MISTLETOE       = 26206,
    SPELL_CREATE_HOLLY           = 26207,
    SPELL_CREATE_SNOWFLAKES      = 45036,
};

class npc_winter_reveler : public CreatureScript
{
    public:
        npc_winter_reveler() : CreatureScript("npc_winter_reveler") { }

        struct npc_winter_revelerAI : public ScriptedAI
        {
            npc_winter_revelerAI(Creature* creature) : ScriptedAI(creature) {}

            void ReceiveEmote(Player* player, uint32 emote) override
            {
                if (player->HasAura(SPELL_MISTLETOE_DEBUFF))
                    return;

                if (!IsHolidayActive(HOLIDAY_FEAST_OF_WINTER_VEIL))
                    return;

                if (emote == TEXT_EMOTE_KISS)
                {
                    uint32 spellId = RAND(SPELL_CREATE_MISTLETOE, SPELL_CREATE_HOLLY, SPELL_CREATE_SNOWFLAKES);
                    me->CastSpell(player, spellId, false);
                    me->CastSpell(player, SPELL_MISTLETOE_DEBUFF, false);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_winter_revelerAI(creature);
        }
};

/*####
## npc_mechanical_greench
####*/

// The toy's summon properties are shared with 123 other spells and make no guardian of it: it would stay level 1 where it appears
class npc_mechanical_greench : public CreatureScript
{
    public:
        npc_mechanical_greench() : CreatureScript("npc_mechanical_greench") { }

        struct npc_mechanical_greenchAI : public ScriptedAI
        {
            npc_mechanical_greenchAI(Creature* creature) : ScriptedAI(creature) {}

            void ScaleToOwner(Unit* owner)
            {
                uint8 const level = owner->getLevel();
                CreatureTemplate const* cInfo = me->GetCreatureTemplate();
                CreatureBaseStats const* stats = sObjectMgr->GetCreatureBaseStats(level, cInfo->unit_class);
                if (!stats)
                    return;

                uint8 const expansion = level <= 60 ? EXPANSION_CLASSIC : level <= 70 ? EXPANSION_THE_BURNING_CRUSADE
                    : level <= 80 ? EXPANSION_WRATH_OF_THE_LICH_KING : level <= 85 ? EXPANSION_CATACLYSM
                    : level <= 90 ? EXPANSION_MISTS_OF_PANDARIA : level <= 100 ? EXPANSION_WARLORDS_OF_DRAENOR : EXPANSION_LEGION;

                me->SetLevel(level);

                uint64 const health = uint64(stats->BaseHealth[expansion] * cInfo->HpMulti * sWorld->getRate(RATE_CREATURE_NORMAL_HP));
                me->SetCreateHealth(health);
                me->SetMaxHealth(health);
                me->SetHealth(health);
                me->SetModifierValue(UNIT_MOD_HEALTH, BASE_VALUE, float(health));

                float const damage = stats->BaseDamage[expansion];
                me->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, damage);
                me->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, damage * 1.5f);
                me->SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, float(stats->AttackPower));
                me->UpdateAllStats();
            }

            void FollowOwner(Unit* owner)
            {
                if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() != FOLLOW_MOTION_TYPE)
                    me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, me->GetFollowAngle());
            }

            void IsSummonedBy(Unit* owner) override
            {
                ScaleToOwner(owner);
                FollowOwner(owner);
            }

            void EnterEvadeMode() override
            {
                me->CombatStop(true);
                if (Unit* owner = me->GetOwner())
                    FollowOwner(owner);
            }

            void UpdateAI(uint32 /*diff*/) override
            {
                Unit* owner = me->GetOwner();
                if (!owner)
                    return;

                Unit* victim = me->getVictim();
                if (!victim || !victim->IsAlive() || !me->IsValidAttackTarget(victim))
                {
                    Unit* target = owner->getVictim();
                    if (!target || !me->IsValidAttackTarget(target))
                        target = owner->getAttackerForHelper();

                    if (target && me->IsValidAttackTarget(target))
                        AttackStart(target);
                    else if (me->isInCombat() || me->getVictim())
                        EnterEvadeMode();
                    else
                        FollowOwner(owner);
                    return;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_mechanical_greenchAI(creature);
        }
};

void AddSC_winter_veil()
{
    new npc_winter_reveler();
    new npc_mechanical_greench();
}
