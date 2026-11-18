////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
 * Demon Invasion Prepatch - Creature AI Scripts
 * Generic demons, Commander/Lieutenants, and Boss
 */

#include "DemonInvasion.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "SpellInfo.h"

enum DemonInvasionSpells
{
    // Generic demons
    SPELL_DI_FEL_STRIKE       = 183345,  // Melee instant attack (common Legion spell)
    SPELL_DI_FEL_BOLT         = 183349,  // Shadow bolt variant
    SPELL_DI_IMMOLATION_AURA  = 187727,  // Infernal AoE fire aura

    // Commander/Lieutenants
    SPELL_DI_SHADOW_CLEAVE    = 183542,  // Frontal cleave
    SPELL_DI_FEL_STOMP        = 200261,  // AoE stomp
    SPELL_DI_ENRAGE           = 8599,    // Standard enrage

    // Boss
    SPELL_DI_METEOR_STRIKE    = 177973,  // Large AoE impact
    SPELL_DI_FEL_FIRE_BREATH  = 183465,  // Cone fire
    SPELL_DI_BERSERK          = 26662,   // Berserk timer
};

enum DemonInvasionEvents
{
    EVENT_DI_FEL_STRIKE = 1,
    EVENT_DI_FEL_BOLT,
    EVENT_DI_IMMOLATION,
    EVENT_DI_SHADOW_CLEAVE,
    EVENT_DI_FEL_STOMP,
    EVENT_DI_ENRAGE_CHECK,
    EVENT_DI_METEOR,
    EVENT_DI_BREATH,
    EVENT_DI_BERSERK,
};

// ============================================================================
// npc_demon_invasion_generic — Stage 1/3 trash mobs
// ============================================================================
class npc_demon_invasion_generic : public CreatureScript
{
public:
    npc_demon_invasion_generic() : CreatureScript("npc_demon_invasion_generic") {}

    struct npc_demon_invasion_genericAI : public ScriptedAI
    {
        npc_demon_invasion_genericAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() override
        {
            events.Reset();
        }

        void CorpseRemoved(uint32& respawnDelay) override
        {
            respawnDelay = sDemonInvasionMgr->GetDemonRespawnDelay(me->GetCurrentZoneID());
        }

        void EnterCombat(Unit* /*who*/) override
        {
            switch (me->GetEntry())
            {
                case NPC_DI_FEL_INVADER:
                    events.RescheduleEvent(EVENT_DI_FEL_STRIKE, urand(4000, 8000));
                    break;
                case NPC_DI_FELSTALKER:
                    events.RescheduleEvent(EVENT_DI_FEL_STRIKE, urand(3000, 5000));
                    break;
                case NPC_DI_FEL_CASTER:
                    events.RescheduleEvent(EVENT_DI_FEL_BOLT, urand(1000, 3000));
                    break;
                case NPC_DI_INFERNAL:
                    events.RescheduleEvent(EVENT_DI_IMMOLATION, 2000);
                    events.RescheduleEvent(EVENT_DI_FEL_STRIKE, urand(5000, 10000));
                    break;
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_DI_FEL_STRIKE:
                        DoCastVictim(SPELL_DI_FEL_STRIKE);
                        events.RescheduleEvent(EVENT_DI_FEL_STRIKE, urand(6000, 12000));
                        break;
                    case EVENT_DI_FEL_BOLT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true))
                            DoCast(target, SPELL_DI_FEL_BOLT);
                        events.RescheduleEvent(EVENT_DI_FEL_BOLT, urand(3000, 5000));
                        break;
                    case EVENT_DI_IMMOLATION:
                        DoCast(me, SPELL_DI_IMMOLATION_AURA);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_demon_invasion_genericAI(creature);
    }
};

// ============================================================================
// npc_demon_invasion_commander — Stage 2 Commander + Lieutenants
// ============================================================================
class npc_demon_invasion_commander : public CreatureScript
{
public:
    npc_demon_invasion_commander() : CreatureScript("npc_demon_invasion_commander") {}

    struct npc_demon_invasion_commanderAI : public ScriptedAI
    {
        npc_demon_invasion_commanderAI(Creature* creature) : ScriptedAI(creature)
        {
            enraged = false;
        }

        void Reset() override
        {
            events.Reset();
            enraged = false;
        }

        void EnterCombat(Unit* /*who*/) override
        {
            events.RescheduleEvent(EVENT_DI_SHADOW_CLEAVE, urand(5000, 8000));
            events.RescheduleEvent(EVENT_DI_FEL_STOMP, urand(10000, 15000));
            events.RescheduleEvent(EVENT_DI_ENRAGE_CHECK, 1000);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_DI_SHADOW_CLEAVE:
                        DoCastVictim(SPELL_DI_SHADOW_CLEAVE);
                        events.RescheduleEvent(EVENT_DI_SHADOW_CLEAVE, urand(8000, 12000));
                        break;
                    case EVENT_DI_FEL_STOMP:
                        DoCastAOE(SPELL_DI_FEL_STOMP);
                        events.RescheduleEvent(EVENT_DI_FEL_STOMP, urand(15000, 20000));
                        break;
                    case EVENT_DI_ENRAGE_CHECK:
                        if (!enraged && HealthBelowPct(30))
                        {
                            DoCast(me, SPELL_DI_ENRAGE);
                            enraged = true;
                        }
                        else
                            events.RescheduleEvent(EVENT_DI_ENRAGE_CHECK, 2000);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
        bool enraged;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_demon_invasion_commanderAI(creature);
    }
};

// ============================================================================
// npc_demon_invasion_boss — stage 4 demon lord at the invasion point
// The 2016 guide names its two dangerous attacks: Shadow Gale (stand by the shadow to be spared) and Flame Fissure.
// ============================================================================
enum DemonLordData
{
    SPELL_DI_SHADOW_GALE            = 75664,  // spared near the stalker (spell_erudax_shadow_gale on 75692)
    SPELL_DI_SHADOW_GALE_SPEED_AURA = 75675,
    NPC_DI_SHADOW_GALE_STALKER      = 40567,
    SPELL_DI_FLAME_FISSURE_SUMMON   = 219013, // summons the fissure 110023
    SPELL_DI_FLAME_FISSURE_AURA     = 219048,
    SPELL_DI_FLAME_FISSURE_DAMAGE   = 219059,
    NPC_DI_FLAME_FISSURE            = 110023,

    EVENT_DI_SHADOW_GALE            = 20,
    EVENT_DI_GALE_END,
    EVENT_DI_FLAME_FISSURE,
};

class npc_demon_invasion_boss : public CreatureScript
{
public:
    npc_demon_invasion_boss() : CreatureScript("npc_demon_invasion_boss") {}

    struct npc_demon_invasion_bossAI : public ScriptedAI
    {
        npc_demon_invasion_bossAI(Creature* creature) : ScriptedAI(creature), summons(creature) {}

        void Reset() override
        {
            events.Reset();
            summons.DespawnAll();
        }

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);
        }

        void JustDied(Unit* /*killer*/) override
        {
            summons.DespawnAll();
        }

        void EnterCombat(Unit* /*who*/) override
        {
            events.RescheduleEvent(EVENT_DI_FLAME_FISSURE, urand(8000, 12000));
            events.RescheduleEvent(EVENT_DI_SHADOW_GALE, urand(25000, 30000));
            events.RescheduleEvent(EVENT_DI_BERSERK, 10 * MINUTE * IN_MILLISECONDS);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_DI_FLAME_FISSURE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true))
                            me->CastSpell(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), SPELL_DI_FLAME_FISSURE_SUMMON, false);
                        events.RescheduleEvent(EVENT_DI_FLAME_FISSURE, urand(15000, 20000));
                        break;
                    case EVENT_DI_SHADOW_GALE:
                    {
                        Position safe = me->GetRandomNearPosition(15.0f);
                        if (Creature* stalker = me->SummonCreature(NPC_DI_SHADOW_GALE_STALKER, safe, TEMPSUMMON_TIMED_DESPAWN, 20000))
                            stalker->CastSpell(stalker, SPELL_DI_SHADOW_GALE_SPEED_AURA, false);
                        DoCast(SPELL_DI_SHADOW_GALE);
                        events.RescheduleEvent(EVENT_DI_SHADOW_GALE, urand(40000, 45000));
                        break;
                    }
                    case EVENT_DI_BERSERK:
                        DoCast(me, SPELL_DI_BERSERK);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
        SummonList summons;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_demon_invasion_bossAI(creature);
    }
};

// Flame Fissure left on the ground by the demon lord: burns whoever stands in it, for half a minute
class npc_demon_invasion_flame_fissure : public CreatureScript
{
public:
    npc_demon_invasion_flame_fissure() : CreatureScript("npc_demon_invasion_flame_fissure") {}

    struct npc_demon_invasion_flame_fissureAI : public ScriptedAI
    {
        npc_demon_invasion_flame_fissureAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetReactState(REACT_PASSIVE);
            SetCombatMovement(false);
        }

        uint32 tick = 1000;
        uint32 lifetime = 30000;

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            DoCast(me, SPELL_DI_FLAME_FISSURE_AURA, true);
        }

        void UpdateAI(uint32 diff) override
        {
            if (lifetime <= diff)
            {
                me->DespawnOrUnsummon();
                return;
            }
            lifetime -= diff;

            if (tick > diff)
            {
                tick -= diff;
                return;
            }
            tick = 1000;

            std::list<Player*> players;
            me->GetPlayerListInGrid(players, 4.0f);
            for (Player* player : players)
                if (player->IsAlive() && !player->isGameMaster())
                    me->CastSpell(player, SPELL_DI_FLAME_FISSURE_DAMAGE, true, nullptr, nullptr, me->ToTempSummon() ? me->ToTempSummon()->GetSummonerGUID() : ObjectGuid::Empty);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_demon_invasion_flame_fissureAI(creature);
    }
};

// ============================================================================
// Registration — called from AddSC_demon_invasion_prepatch()
// We register them separately but they're loaded via the same script loader
// ============================================================================

void AddSC_demon_invasion_creatures()
{
    new npc_demon_invasion_generic();
    new npc_demon_invasion_commander();
    new npc_demon_invasion_boss();
    new npc_demon_invasion_flame_fissure();
}
