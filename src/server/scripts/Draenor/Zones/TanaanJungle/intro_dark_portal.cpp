/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// ================================================================
// dark_portal.cpp — WoD intro scenario (Map 1265, Zone 7025)
//
// Quest chain:
//   Q35933 → Q34392 → Q34393 → Q34420 → Q34422 → Q34423
//   → Q34425 → Q34429 → Q34434/34740 → Q34741/34436
//   → Q34439 → Q34437 → Q34445 → Q35747
//
// Phase handling: playerscript_wod_portal_phases
//   (replaces the 45 phase_definitions rows of zone 7025)
// ================================================================

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "CreatureTextMgr.h"
#include "GameObjectAI.h"
#include "GossipDef.h"
#include "QuestData.h"

enum misc
{
    SPELL_UPDATE_PHASE_SHIFT = 82238,
};

enum __data
{
    SPELL_CHAIN_LIGHT = 15305,    //Chain Lightning
    SPELL_COMMAND_EARTH = 167470,   //Command Earth
    SPELL_COMMAND_LIGHT = 167014,   //Command Lightning
    SPELL_SAVAGERY = 167432,   //Savagery
    SPELL_PROTECTOR = 166114,
};

const uint32 spells[3] = { SPELL_CHAIN_LIGHT, SPELL_COMMAND_EARTH, SPELL_COMMAND_LIGHT };

struct arena_friendly_classAI : public ScriptedAI
{
    EventMap events;

    arena_friendly_classAI(Creature* creature) : ScriptedAI(creature)
    {
    }

    enum data
    {
        EVENT_COMBAT_SPELL_1 = 1,
        EVENT_COMBAT_SAVAGERY = 2,
        EVENT_BEGIN_1,
    };

    void Reset() override
    {

    }

    void JustRespawned() override
    {
        ScriptedAI::JustRespawned();
        switch (me->GetCurrentAreaID())
        {
            case 7037:
                events.RescheduleEvent(EVENT_BEGIN_1, 1000);
            case 7040:  //arena
                me->setFaction(2580);
                break;
        }
    }

    void EnterEvadeMode() override
    {
        events.CancelEvent(EVENT_COMBAT_SPELL_1);
        events.CancelEvent(EVENT_COMBAT_SAVAGERY);
        ScriptedAI::EnterEvadeMode();
    }

    void EnterCombat(Unit* /*victim*/) override
    {
        events.RescheduleEvent(EVENT_COMBAT_SPELL_1, 5000);
        events.RescheduleEvent(EVENT_COMBAT_SAVAGERY, 1000);
    }
};

class mob_wod_thrall : public CreatureScript
{
public:
    mob_wod_thrall() : CreatureScript("mob_wod_thrall") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_thrallAI(creature);
    }

    struct mob_wod_thrallAI : public arena_friendly_classAI
    {
        mob_wod_thrallAI(Creature* creature) : arena_friendly_classAI(creature)
        {
        }

        void UpdateAI(uint32 diff) override
        {
            UpdateVictim();

            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_COMBAT_SPELL_1:
                    {
                        if (Unit* victim = me->getVictim())
                            me->CastSpell(victim, spells[urand(0, 2)], false);
                        events.RescheduleEvent(EVENT_COMBAT_SPELL_1, 5000);
                        break;
                    }
                    case EVENT_COMBAT_SAVAGERY:
                        me->CastSpell(me, SPELL_SAVAGERY, false);
                        events.RescheduleEvent(EVENT_COMBAT_SAVAGERY, 30000);
                        break;
                    case EVENT_BEGIN_1:
                        events.RescheduleEvent(EVENT_BEGIN_1, 60000);
                        sCreatureTextMgr->SendChat(me, TEXT_GENERIC_1, ObjectGuid::Empty);
                        me->CastSpell(me, SPELL_PROTECTOR, false);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

class mob_wod_cordona_welsong : public CreatureScript
{
public:
    mob_wod_cordona_welsong() : CreatureScript("mob_wod_cordona_welsong") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_cordona_welsongAI(creature);
    }

    struct mob_wod_cordona_welsongAI : public arena_friendly_classAI
    {
        mob_wod_cordona_welsongAI(Creature* creature) : arena_friendly_classAI(creature)
        {
        }

        void UpdateAI(uint32 diff) override
        {
            UpdateVictim();

            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_COMBAT_SPELL_1:
                {
                    if (Unit* victim = me->getVictim())
                        me->CastSpell(victim, 166911, false);
                    events.RescheduleEvent(EVENT_COMBAT_SPELL_1, 5000);
                    break;
                }
                case EVENT_COMBAT_SAVAGERY:
                    me->CastSpell(me, SPELL_SAVAGERY, false);
                    events.RescheduleEvent(EVENT_COMBAT_SAVAGERY, 30000);
                    break;

                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

class mob_wod_archimage_khadgar : public CreatureScript
{
public:
    mob_wod_archimage_khadgar() : CreatureScript("mob_wod_archimage_khadgar") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_archimage_khadgarAI(creature);
    }

    struct mob_wod_archimage_khadgarAI : public arena_friendly_classAI
    {
        mob_wod_archimage_khadgarAI(Creature* creature) : arena_friendly_classAI(creature)
        {
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};

class mob_wod_olin_oberhind : public CreatureScript
{
public:
    mob_wod_olin_oberhind() : CreatureScript("mob_wod_olin_oberhind") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_olin_oberhindAI(creature);
    }

    struct mob_wod_olin_oberhindAI : public arena_friendly_classAI
    {
        mob_wod_olin_oberhindAI(Creature* creature) : arena_friendly_classAI(creature)
        {
        }

        void UpdateAI(uint32 diff) override
        {
            UpdateVictim();

            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_COMBAT_SPELL_1:
                {
                    if (Unit* victim = me->getVictim())
                        me->CastSpell(victim, 166969, false);
                    events.RescheduleEvent(EVENT_COMBAT_SPELL_1, 5000);
                    break;
                }
                case EVENT_COMBAT_SAVAGERY:
                    me->CastSpell(me, SPELL_SAVAGERY, false);
                    events.RescheduleEvent(EVENT_COMBAT_SAVAGERY, 30000);
                    break;

                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

class mob_wod_intro_enemy_at_portal : public CreatureScript
{
public:
    mob_wod_intro_enemy_at_portal() : CreatureScript("mob_wod_intro_enemy_at_portal") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_intro_enemy_at_portalAI(creature);
    }

    struct mob_wod_intro_enemy_at_portalAI : public ScriptedAI
    {
        uint32 _searchTimer = 0;

        mob_wod_intro_enemy_at_portalAI(Creature* creature) : ScriptedAI(creature)
        {
            // Prevent CallAssistance() from triggering group aggro when a player
            // attacks one creature — these are scene decorations, not a pack.
            me->SetNoCallAssistance(true);
        }

        void SearchAndAttackEnemy()
        {
            std::list<Unit*> list;
            me->GetAttackableUnitListInRange(list, 20.0f);
            for (auto enemy : list)
            {
                if (!me->IsValidAttackTarget(enemy))
                    continue;
                // Stay in place — do not chase
                DoStartNoMovement(enemy);
                break;
            }
        }

        void Reset() override
        {
            _searchTimer = 0;
            me->GetMotionMaster()->MoveIdle();
            if (me->getVictim() || me->IsInEvadeMode())
                return;
            SearchAndAttackEnemy();
        }

        void MoveInLineOfSight(Unit* who) override
        {
            if (!who || !me->IsValidAttackTarget(who))
                return;
            if (me->GetExactDist2d(who) <= 20.0f)
                DoStartNoMovement(who);
        }

        void UpdateAI(uint32 diff) override
        {
            if (me->getVictim())
            {
                // Leash only when chasing a player
                if (me->getVictim()->ToPlayer())
                {
                    Position home = me->GetHomePosition();
                    if (me->GetExactDist2d(&home) > 30.0f)
                    {
                        EnterEvadeMode();
                        return;
                    }
                }
                DoMeleeAttackIfReady();
                return;
            }

            if (_searchTimer <= diff)
            {
                _searchTimer = 3000;
                SearchAndAttackEnemy();
            }
            else
                _searchTimer -= diff;
        }

        void DamageDealt(Unit* victim, uint32& damage, DamageEffectType /*damageType*/) override
        {
            if (victim->ToPlayer())
                damage /= 4;
        }

        void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType dmgType) override
        {
            if (attacker->ToPlayer())
            {
                damage *= 2;
                me->AI()->AttackStart(attacker); // Chase the player
                return;
            }
            damage /= 2;
        }
    };
};

class mob_wod_shadowmoon_ritualist : public CreatureScript
{
public:
    mob_wod_shadowmoon_ritualist() : CreatureScript("mob_wod_shadowmoon_ritualist") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_shadowmoon_ritualistAI(creature);
    }

    struct mob_wod_shadowmoon_ritualistAI : public ScriptedAI
    {
        enum : uint32
        {
            SPELL_VOID_BOLT = 157355,
            SPELL_SAVAGERY  = 167432,

            EVENT_VOID_BOLT = 1,
            EVENT_SAVAGERY  = 2,
        };

        EventMap events;

        mob_wod_shadowmoon_ritualistAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetNoCallAssistance(true);
        }

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/) override
        {
            events.ScheduleEvent(EVENT_VOID_BOLT, urand(2000, 4000));
            events.ScheduleEvent(EVENT_SAVAGERY,  urand(8000, 12000));
        }

        void MoveInLineOfSight(Unit* who) override
        {
            if (!who || !me->IsValidAttackTarget(who))
                return;
            if (me->GetExactDist2d(who) <= 20.0f)
                DoStartNoMovement(who);
        }

        void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*dmgType*/) override
        {
            if (attacker->ToPlayer())
            {
                damage *= 2;
                me->AI()->AttackStart(attacker);
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
                    case EVENT_VOID_BOLT:
                        DoCastVictim(SPELL_VOID_BOLT);
                        events.ScheduleEvent(EVENT_VOID_BOLT, urand(5000, 8000));
                        break;
                    case EVENT_SAVAGERY:
                        DoCast(me, SPELL_SAVAGERY);
                        events.ScheduleEvent(EVENT_SAVAGERY, urand(15000, 20000));
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

// ============================================================
// Q34393 — Portal Energy
// GO 233104 (Stasis Rune) : voir gob_stasis_rune plus bas.
// ============================================================
class mob_wod_intro_guldan : public CreatureScript
{
public:
    mob_wod_intro_guldan() : CreatureScript("mob_wod_intro_guldan") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_intro_guldanAI(creature);
    }

    struct mob_wod_intro_guldanAI : public ScriptedAI
    {

        enum data
        {
            SPELL_SCENE_INTRO       = 163807,
            SPELL_PORTAL_CREDIT     = 166319,
            QUEST_PORTAL_ENERGY     = 34393,
            OBJECTIVE_PORTAL_CREDIT = 82573,
        };

        mob_wod_intro_guldanAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        void Reset()
        {
        }

        void MoveInLineOfSight(Unit* who)
        {
            Player *player = who->ToPlayer();
            if (!player || !me->IsWithinDistInMap(who, 51.0f) || who->GetPositionZ() > 79.0f)
                return;

            if (player->GetQuestStatus(QUEST_PORTAL_ENERGY) != QUEST_STATUS_INCOMPLETE)
                return;

            if (player->HasAura(SPELL_SCENE_INTRO) || player->GetQuestObjectiveData(QUEST_PORTAL_ENERGY, OBJECTIVE_PORTAL_CREDIT))
                return;

            player->CastSpell(player, SPELL_PORTAL_CREDIT, true);
            player->CastSpell(player, SPELL_SCENE_INTRO, false);
            return;
        }
    };
};

class mob_wod_frostwolf_slave : public CreatureScript
{
public:
    mob_wod_frostwolf_slave() : CreatureScript("mob_wod_frostwolf_slave") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_frostwolf_slaveAI(creature);
    }

    struct mob_wod_frostwolf_slaveAI : public ScriptedAI
    {

        mob_wod_frostwolf_slaveAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        enum data
        {
            QUEST1 = 34421,
            QUEST2 = 35240,
        };

        GuidSet players;

        void Reset()
        {
        }

        void SetGUID(ObjectGuid const& guid, int32 /*id*/ = 0)
        {
            players.insert(guid);
        }

        void MoveInLineOfSight(Unit* who)
        {
            Player *player = who->ToPlayer();
            if (!player || !me->IsWithinDistInMap(who, 51.0f) || who->GetPositionZ() > 79.0f)
                return;

            if (player->GetQuestStatus(QUEST1) != QUEST_STATUS_INCOMPLETE && player->GetQuestStatus(QUEST2) != QUEST_STATUS_INCOMPLETE)
                return;

            auto data = players.find(who->GetGUID());
            if (data != players.end())
                return;

            players.insert(who->GetGUID());
            sCreatureTextMgr->SendChat(me, 0, who->GetGUID());

            // already trigered player for nearest
            std::list<Creature*> creatureList;
            me->GetCreatureListWithEntryInGrid(creatureList, me->GetEntry(), 50);
            for (auto c : creatureList)
                c->AI()->SetGUID(who->GetGUID(), 0);
        }
    };
};

// 159126 - eastern
class go_wod_slaves_cage : public GameObjectScript
{
public:
    go_wod_slaves_cage() : GameObjectScript("go_wod_slaves_cage") { }

    struct go_wod_slaves_cageAI : public GameObjectAI
    {
        go_wod_slaves_cageAI(GameObject* go) : GameObjectAI(go)
        {

        }
        
        enum data
        {
            SOUTH_SCENE = 159127,
            EASTERN_SCENE = 159126
        };
        bool GossipHello(Player* player, bool isUse) override
        {
            if (!isUse)
                return true;

            if (go->GetEntry() == 229353)
            {
                player->CastSpell(player, EASTERN_SCENE, true);
                player->KilledMonsterCredit(85142, ObjectGuid::Empty);
                player->KilledMonsterCredit(78529, ObjectGuid::Empty);
            }
            else
            {
                player->CastSpell(player, SOUTH_SCENE, true);
                player->KilledMonsterCredit(82871, ObjectGuid::Empty);
                player->KilledMonsterCredit(85141, ObjectGuid::Empty);
            }

            return true;
        }

    };

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_wod_slaves_cageAI(go);
    }
};

// ============================================================
// Q34423 — Altar Altercation  (NPC 78540)
// Ariok escorts the player → credit 159278 → summon 161625.
// See also: spell_wod_destroying (door destruction spell).
// ============================================================
class mob_wod_ariok : public CreatureScript
{
public:
    mob_wod_ariok() : CreatureScript("mob_wod_ariok") { }

    enum data
    {
        QUEST = 34423,
        SPELL_CREDIT = 159278,
        SPELL_SUMMON = 161625,
        PHASE_ARIOK_DB = 3394,
    };
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == 1)
        {
            player->CLOSE_GOSSIP_MENU();
            if (player->GetQuestStatus(QUEST) == QUEST_STATUS_INCOMPLETE)
            {
                player->CastSpell(player, SPELL_CREDIT, true);
                player->CastSpell(player, SPELL_SUMMON, true);
                std::set<uint32> phase = player->GetPhases();
                phase.erase(PHASE_ARIOK_DB);
                player->SetPhaseId(phase, true);
            }
        }

        return true;
    }
};

// ============================================================
// Q34392 — Onslaught's End
// GO 233056 (Mark of the Shadowmoon, north, guid 105850) → credit 82606
// GO 233057 (Mark of the Bleeding Hollow, south, guid 105853) → credit 82607
// GO 231985 (Plunger) → blows up the chamber doors
// NPC 78569 (Hansel Heavyhands) — start signal
// ============================================================
// NPC 78569 (Hansel Heavyhands) — "Go!" when a player on Q34392 walks by
class mob_wod_hansel : public CreatureScript
{
public:
    mob_wod_hansel() : CreatureScript("mob_wod_hansel") { }

    enum { QUEST_ONSLAUGHT = 34392 };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new mob_wod_hanselAI(creature);
    }

    struct mob_wod_hanselAI : public ScriptedAI
    {
        mob_wod_hanselAI(Creature* c) : ScriptedAI(c) {}

        std::set<ObjectGuid> _greeted;

        void Reset() override
        {
            _greeted.clear();
        }

        void MoveInLineOfSight(Unit* who) override
        {
            Player* player = who->ToPlayer();
            if (!player || !me->IsWithinDistInMap(who, 15.0f))
                return;
            if (player->GetQuestStatus(QUEST_ONSLAUGHT) != QUEST_STATUS_INCOMPLETE)
                return;
            if (_greeted.count(player->GetGUID()))
                return;
            _greeted.insert(player->GetGUID());
            // GroupID=0: "Go!"
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, player->GetGUID());
        }
    };
};

// phase spell - 165052
// end quest - 165053
class mob_wod_ariok_mover : public CreatureScript
{
public:
    mob_wod_ariok_mover() : CreatureScript("mob_wod_ariok_mover") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_ariok_moverAI(creature);
    }

    struct mob_wod_ariok_moverAI : public ScriptedAI
    {
        EventMap events;
        ObjectGuid playerGuid;

        mob_wod_ariok_moverAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        enum data
        {
            SPELL_ON_ALTAR_CREDIT = 161637,
            SPELL_SCENE_EYE_CILLROG = 164877,
            SPELL_SCENE = 165061,

            NPC_ORB = 83670,

            EVENT_1 = 1,
            EVENT_2,
            EVENT_3,
            EVENT_4,
            EVENT_5,
            EVENT_6,
            EVENT_CHECK_PHASE_1
        };

        void OnCharmed(bool /*apply*/)
        {
        }

        void IsSummonedBy(Unit* summoner)
        {
            Player *player = summoner->ToPlayer();
            if (!player)
            {
                me->MonsterSay("SCRIPT::mob_wod_ariok_moverAI summoner is not player", LANG_UNIVERSAL, ObjectGuid::Empty);
                return;
            }

            playerGuid = summoner->GetGUID();
            me->AddPlayerInPersonnalVisibilityList(summoner->GetGUID());

            uint32 t = 0;                                                       //
            events.RescheduleEvent(EVENT_1, t += 2000);               //09:14:54.000
            events.RescheduleEvent(EVENT_2, t += 6000);               //09:15:00.000
            events.RescheduleEvent(EVENT_3, t += 7000);               //09:15:07.000

            events.RescheduleEvent(EVENT_CHECK_PHASE_1, t += 1000);
        }

        void DoAction(int32 const /*param*/)
        {
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_5, playerGuid);
            me->DespawnOrUnsummon(1000);
        }

        void UpdateAI(uint32 diff)
        {
            UpdateVictim();

            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_4:
                    if (Player* player = sObjectAccessor->FindPlayer(playerGuid))
                    {
                        player->CastSpell(player, SPELL_ON_ALTAR_CREDIT, true);
                        player->CastSpell(player, SPELL_SCENE_EYE_CILLROG, true);
                    }
                    //break;    no break;
                case EVENT_5:
                case EVENT_1:
                case EVENT_2:
                case EVENT_3:
                    sCreatureTextMgr->SendChat(me, /*TEXT_GENERIC_0*/eventId-1, playerGuid);
                    break;
                case EVENT_6:
                    if (Player* player = sObjectAccessor->FindPlayer(playerGuid))
                        player->CastSpell(player, SPELL_SCENE, true);
                    break;
                case EVENT_CHECK_PHASE_1:

                    if (me->GetDistance(3979.26f, -2918.31f, 60.8725f) < 80.0f)
                    {
                        uint32 t = 0;
                        events.RescheduleEvent(EVENT_4, t += 1000);               //09:16:18.000
                        events.RescheduleEvent(EVENT_5, t += 5000);               //09:16:23.000
                        events.RescheduleEvent(EVENT_6, t += 60000);              //09:17:27.000
                    }else
                        events.RescheduleEvent(EVENT_CHECK_PHASE_1, 5000);
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

// Destroy - 167955
class spell_wod_destroying : public SpellScriptLoader
{
public:
    spell_wod_destroying() : SpellScriptLoader("spell_wod_destroying") { }

    class spell_wod_destroying_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_wod_destroying_SpellScript);

        enum data
        {
            SPELL_GUARD_SUMMON_SPELL = 161618,
            NPC_SUMMON_ID = 80087,
            QUEST = 34423,
            OBJECTIVE_CREDIT = 78966,
        };

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(EFFECT_1);

            if (Unit* caster = GetCaster())
            {
                Player *player = caster->ToPlayer();
                if (!player)
                    return;

                if (Unit * target = GetHitUnit())
                {
                    target->AddToHideList(caster->GetGUID());
                    target->DestroyForPlayer(player);
                }

                if (player->GetQuestObjectiveData(QUEST, OBJECTIVE_CREDIT) >= 3)
                {
                    if (Creature *c = player->GetMinionByEntry(NPC_SUMMON_ID))
                        c->AI()->DoAction(0);
                }
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_wod_destroying_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_wod_destroying_SpellScript();
    }
};

// ============================================================
// Q34425 — (Khadgar escort)
// NPC Khadgar escort → bridge scene ("Bridge" trigger)
// Phases: entry 17 (start) → 18 (after the Bridge trigger) → 19 (rewarded)
// ============================================================
class mob_khadgar_q34425 : public CreatureScript
{
public:
    mob_khadgar_q34425() : CreatureScript("mob_khadgar_q34425") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_khadgar_q34425AI(creature);
    }

    struct mob_khadgar_q34425AI : public npc_escortAI
    {
        mob_khadgar_q34425AI(Creature* creature) : npc_escortAI(creature)
        {}

        ObjectGuid playerGuid;
        EventMap events;

        enum events
        {
            SPELL_SHOWER = 165864,
            NPC_CREDIT = 80244,

            EVENT_1 = 1,
            EVENT_2 = 2,
            EVENT_JECK_DEAD = 3,
        };

        void Reset()
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void SetGUID(ObjectGuid const& guid, int32)
        {
            playerGuid = guid;
        }

        void IsSummonedBy(Unit* summoner)
        {
            me->CastSpell(me, SPELL_SHOWER, false); //09:20:28.000
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, playerGuid);

            me->AddPlayerInPersonnalVisibilityList(summoner->GetGUID());
            playerGuid = summoner->GetGUID();
            events.RescheduleEvent(EVENT_1, 8000);    //09:20:36.000
        }

        void WaypointReached(uint32 waypointId)
        {
            if (waypointId == 3)
            {
                if (Player* player = ObjectAccessor::GetPlayer(*me, playerGuid))
                    player->KilledMonsterCredit(NPC_CREDIT);
                me->DespawnOrUnsummon(1000);
            }
        }

        void UpdateAI(uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        sCreatureTextMgr->SendChat(me, TEXT_GENERIC_1, playerGuid);
                        Start(false, true);
                        break;
                    default:
                        break;
                }
            }
        }
    };
};

// ============================================================
// Phase Management — replaces the 45 phase_definitions rows of zone 7025
// Matching SQL DELETE:
//   sql/updates/world/2026_03_07_07_phase_definitions_zone7025_remove.sql
//
// UpdatePhaseForPlayer checks the quest states in reverse order
// (most advanced condition first) and applies the exact phase set
// of the original DB.
// ============================================================
class playerscript_wod_portal_phases : public PlayerScript
{
public:
    playerscript_wod_portal_phases() : PlayerScript("playerscript_wod_portal_phases") {}

    enum
    {
        MAP_WOD_INTRO = 1265,
        AREA_7041     = 7041,   // inner portal area (Q34420 mid-progress)
        // Quests in chain order
        Q_34398 = 34398,   // Warlords of Draenor: The Dark Portal
        Q_35933 = 35933,   // Azeroth's Last Stand
        Q_34392 = 34392,   // Onslaught's End
        Q_34393 = 34393,   // Portal Energy
        Q_34420 = 34420,   // Into the Portal
        Q_34421 = 34421,   // optional side quest (adds phases 3209/3210)
        Q_34422 = 34422,
        Q_34423 = 34423,
        Q_34425 = 34425,
        Q_34429 = 34429,   // Arena
        Q_34436 = 34436,   // Gate — Alliance
        Q_34437 = 34437,
        Q_34439 = 34439,
        Q_34442 = 34442,
        Q_34445 = 34445,
        Q_34741 = 34741,   // Gate — Horde
        Q_35747 = 35747,   // Thaelin
        Q_34446 = 34446,   // End — Alliance
        Q_35884 = 35884,   // End — Horde
    };

    static void UpdatePhaseForPlayer(Player* player)
    {
        if (player->GetMapId() != MAP_WOD_INTRO)
            return;

        player->GetPhaseMgr().RemoveUpdateFlag(PHASE_UPDATE_FLAG_ZONE_UPDATE);
        player->GetPhaseMgr().RemoveUpdateFlag(PHASE_UPDATE_FLAG_AREA_UPDATE);

        // ── base ──────────────────────────
        std::set<uint32> phases = {3248,3263,3519,3712,3752,3824,3948,4200};

        // ── 34398 complete ───────────────────────────────────
        if (player->GetQuestStatus(Q_34398) == QUEST_STATUS_COMPLETE)
            phases = {3248,3263,3519,3712,3752,3834,3948,4200};

        // ── 35933 incomplete ─────────────────────────────────
        if (player->GetQuestStatus(Q_35933) == QUEST_STATUS_INCOMPLETE)
            phases = {3248,3263,3519,3712,3752,3834,3948,4200};

        // ── 35933 completed ───────────────────────────────
        if (player->GetQuestStatus(Q_35933) == QUEST_STATUS_COMPLETE)
            phases = {3248,3263,3519,3712,3752,3834,3948,4200};

        // ── 34392 incomplete ─────────────────────────────────
        if (player->GetQuestStatus(Q_34392) == QUEST_STATUS_INCOMPLETE)
            phases = {3248,3263,3824,3948,4200};

        // ── 34392 complete ───────────────────────────────
        if (player->GetQuestStatus(Q_34392) == QUEST_STATUS_COMPLETE)
        phases = {3248,3263,3712,3948,4150,4151,4200};

        // ── Entry 5: 34393 incomplete ───────────────────────────────────
        if (player->GetQuestStatus(Q_34393) == QUEST_STATUS_INCOMPLETE)
            phases = {3248,3263,3519,3712,3752,3824,3948,4150,4151,4200}; //3248,3263,3712,3948,4150,4151,4200

        // ── Entry 6: 34393 complete ─────────────────────────────────────
        if (player->GetQuestStatus(Q_34393) == QUEST_STATUS_COMPLETE)
            phases = {3248,3263,3519,3712,3752,3764,3824,4150,4151};

        // ── Entry 7: 34393 rewarded or Q34420 incomplete (general) ────
        if (player->IsQuestRewarded(Q_34393) || player->GetQuestStatus(Q_34420) == QUEST_STATUS_INCOMPLETE)
            phases = {3248,3263,3519,3712,3752,3764,3824,4150,4151};

        // ── Entry 8: 34420 incomplete + area 7041 explored ───────────
        if (player->GetQuestStatus(Q_34420) == QUEST_STATUS_INCOMPLETE &&
            player->GetCurrentAreaID() == AREA_7041)
            phases = {3236,3626,3670,3693,3712,3794,3824,3833,3834,3856,3857,4150,4151,4200};

        // ── Entry 9: 34420 incomplete + scene 621 seen (area 7041, post-scene)
        // Note: telling 8 from 9 (scene 621) is done by sceneTrigger_q34429.
        // Stateless calls stay on entry 8 here.

        // ── Entry 10: 34420 rewarded ─────────────────────────────────
        if (player->IsQuestRewarded(Q_34420))
            phases = {3236,3394,3395,3396,3480,3626,3670,3693,3712,3794,3824,3833,3834,3856,3857,4150,4151,4200};

        // ── Entry 13: 34421 accepted (additive: +3209, +3210) ──────────
        if (player->GetQuestStatus(Q_34421) == QUEST_STATUS_INCOMPLETE)
        { phases.insert(3209); phases.insert(3210); }

        // ── Entry 15: 34422 rewarded ─────────────────────────────────
        if (player->IsQuestRewarded(Q_34422))
            phases = {3237,3265,3394,3395,3396,3480,3626,3655,3670,3693,3712,3794,3824,3833,3834,3856,3857,3911,4150,4151,4200};

        // ── Entry 16: 34423 incomplete + objective ≥ 3 ──────────────────
        if (player->GetQuestStatus(Q_34423) == QUEST_STATUS_INCOMPLETE &&
            player->GetQuestObjectiveData(Q_34423, 0) >= 3)
            phases = {3237,3266,3394,3395,3396,3414,3480,3626,3693,3712,3794,3824,3833,3834,3856,3857,4006,4150,4151,4200};

        // ── Entry 17: 34425 incomplete ──────────────────────────────────
        if (player->GetQuestStatus(Q_34425) == QUEST_STATUS_INCOMPLETE)
            phases = {3266,3394,3395,3396,3480,3693,3694,3712,3794,3824,3833,3834,4006,4017,4150,4151,4200};

        // ── Entry 18: 34425 incomplete + "Bridge" scene trigger ─────────
        // (applied directly by sceneTrigger_q34425 — not repeated here)

        // ── Entry 19: 34425 complete or rewarded ──────────────────────
        if (player->IsQuestRewarded(Q_34425) || player->GetQuestStatus(Q_34425) == QUEST_STATUS_COMPLETE)
            phases = {3266,3317,3349,3358,3359,3394,3395,3396,3416,3481,3693,3694,3712,3824,3833,3834,4006,4017,4150,4151,4200};

        // ── Entry 21: 34429 incomplete ──────────────────────────────────
        if (player->GetQuestStatus(Q_34429) == QUEST_STATUS_INCOMPLETE)
            phases = {3317,3349,3350,3358,3359,3394,3395,3396,3481,3712,3824,3833,3834,4017,4150,4151,4200};

        // ── Entry 23: 34429 complete ────────────────────────────────────
        if (player->GetQuestStatus(Q_34429) == QUEST_STATUS_COMPLETE)
            phases = {3267,3317,3330,3394,3395,3396,3481,3693,3712,3720,3752,3790,3824,3833,3834,4015,4017,4150,4151,4200};

        // ── Entry 25: 34429 rewarded ─────────────────────────────────
        if (player->IsQuestRewarded(Q_34429))
            phases = {3267,3317,3334,3356,3394,3395,3396,3481,3693,3712,3720,3752,3790,3824,3833,3834,3936,4015,4017,4150,4151,4200};

        // ── Entry 28: 34741 (Horde) or Q34436 (Alliance) incomplete ─────
        if (player->GetQuestStatus(Q_34741) == QUEST_STATUS_INCOMPLETE ||
            player->GetQuestStatus(Q_34436) == QUEST_STATUS_INCOMPLETE)
            phases = {3267,3317,3334,3394,3395,3396,3481,3693,3712,3720,3752,3790,3824,3833,3834,3936,4015,4017,4150,4151,4200};

        // ── Entries 29-30: 34741 / 34436 complete or rewarded ─────────
        if (player->IsQuestRewarded(Q_34741) || player->GetQuestStatus(Q_34741) == QUEST_STATUS_COMPLETE ||
            player->IsQuestRewarded(Q_34436) || player->GetQuestStatus(Q_34436) == QUEST_STATUS_COMPLETE)
            phases = {3268,3317,3334,3394,3395,3396,3481,3497,3498,3499,3594,3693,3712,3752,3824,3833,3834,3936,4019,4150,4151,4200};

        // ── Entry 32: 34439 incomplete (start) ──────────────────────────
        if (player->GetQuestStatus(Q_34439) == QUEST_STATUS_INCOMPLETE)
            phases = {3334,3394,3395,3396,3481,3498,3594,3693,3712,3752,3824,3833,3834,3936,4022,4150,4151,4200};

        // ── Entry 34: 34439 incomplete + objective done ──────────────
        if (player->GetQuestStatus(Q_34439) == QUEST_STATUS_INCOMPLETE &&
            player->GetQuestObjectiveData(Q_34439, 0) > 0)
            phases = {3269,3334,3394,3395,3396,3423,3481,3498,3505,3594,3693,3712,3752,3833,3834,3936,4026,4150,4151,4200};

        // ── Entry 36: 34442 incomplete ──────────────────────────────────
        if (player->GetQuestStatus(Q_34442) == QUEST_STATUS_INCOMPLETE)
            phases = {3269,3334,3394,3395,3396,3423,3481,3498,3505,3551,3594,3693,3712,3752,3833,3834,3936,4026,4150,4151,4200};

        // ── Entry 38: 34437 incomplete ──────────────────────────────────
        if (player->GetQuestStatus(Q_34437) == QUEST_STATUS_INCOMPLETE)
            phases = {3269,3394,3395,3396,3423,3481,3498,3505,3579,3581,3594,3693,3712,3752,3833,3834,3936,4026,4150,4151,4200};

        // ── Entry 40: 35747 objective done or rewarded ────────────
        if (player->IsQuestRewarded(Q_35747) || player->GetQuestObjectiveData(Q_35747, 0) > 0)
            phases = {3269,3394,3395,3396,3481,3498,3542,3583,3604,3693,3712,3752,3833,3834,3936,4150,4151,4200};

        // ── Entry 42: 34445 complete or rewarded ──────────────────────
        if (player->IsQuestRewarded(Q_34445) || player->GetQuestStatus(Q_34445) == QUEST_STATUS_COMPLETE)
            phases = {3394,3395,3396,3481,3498,3519,3583,3604,3693,3712,3752,3833,3834,3936,4028,4150,4151,4201};

        // ── Entries 43-44: 34446 / 35884 complete or rewarded ─────────
        if (player->IsQuestRewarded(Q_34446) || player->GetQuestStatus(Q_34446) == QUEST_STATUS_COMPLETE ||
            player->IsQuestRewarded(Q_35884) || player->GetQuestStatus(Q_35884) == QUEST_STATUS_COMPLETE)
            phases = {3394,3395,3396,3481,3498,3693,3712,3752,3834,3936,4028,4072,4150,4151,4201};

        player->SetPhaseId(phases, true);
    }

    // ── Hooks PlayerScript ────────────────────────────────────────────────

    void OnQuestAccept(Player* player, Quest const* /*quest*/) override
    {
        if (player->GetMapId() != MAP_WOD_INTRO)
            return;
        UpdatePhaseForPlayer(player);
    }

    void OnMapChanged(Player* player) override
    {
        if (player->GetMapId() != MAP_WOD_INTRO)
            return;
        UpdatePhaseForPlayer(player);
    }

    void OnLogin(Player* player) override
    {
        if (player->GetMapId() != MAP_WOD_INTRO)
            return;
        UpdatePhaseForPlayer(player);
    }

    void OnQuestComplete(Player* player, Quest const* /*quest*/) override
    {
        if (player->GetMapId() != MAP_WOD_INTRO)
            return;
        UpdatePhaseForPlayer(player);
    }

    void OnQuestReward(Player* player, Quest const* /*quest*/) override
    {
        if (player->GetMapId() != MAP_WOD_INTRO)
            return;
        UpdatePhaseForPlayer(player);
    }

    void OnQuestAbandon(Player* player, Quest const* /*quest*/) override
    {
        if (player->GetMapId() != MAP_WOD_INTRO)
            return;
        UpdatePhaseForPlayer(player);
    }

    void OnUpdateZone(Player* player, uint32 /*newZone*/, uint32 /*newArea*/) override
    {
        if (player->GetMapId() != MAP_WOD_INTRO)
            return;
        UpdatePhaseForPlayer(player);
    }
};

// ============================================================
// Q34425 — SceneTrigger (bridge, scene 621)
// ============================================================
class sceneTrigger_q34425 : public SceneTriggerScript
{
public:
    sceneTrigger_q34425() : SceneTriggerScript("sceneTrigger_q34425")
    {}

    bool OnTrigger(Player* player, SpellScene const* /*trigger*/, std::string type) override
    {
        // "Bridge" trigger = end of the bridge crossing → phase update
        if (type == "Bridge")
            playerscript_wod_portal_phases::UpdatePhaseForPlayer(player);
        return true;
    }
};

// ============================================================
// Q34429 — Arena  (NPC mob_arena_combatant_q34429)
// Spell 168182 (credit), summon spell 167314
// ============================================================
class mob_arena_combatant_q34429 : public CreatureScript
{
public:
    mob_arena_combatant_q34429() : CreatureScript("mob_arena_combatant_q34429") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_arena_combatant_q34429AI(creature);
    }

    struct mob_arena_combatant_q34429AI : public ScriptedAI
    {

        mob_arena_combatant_q34429AI(Creature* creature) : ScriptedAI(creature)
        {
        }

        enum data
        {
            QUEST = 34429,
            NPC_CREDIT = 82066,
            SPELL_SUMMON = 167314,
            SPELL_CREDIT = 168182,
        };

        void Reset()
        {
            if (me->getVictim() || me->IsInEvadeMode())
                return;

            std::list<Unit*> list;
            me->GetAttackableUnitListInRange(list, 70.0f);
            for (auto enemy : list)
            {
                if (enemy->ToPlayer())
                    continue;
                me->AI()->AttackStart(enemy);
                break;
            }
        }

        void MoveInLineOfSight(Unit* /*who*/) override {};
        void DamageDealt(Unit* victim, uint32& damage, DamageEffectType /*damageType*/) override
        {
            if (victim->ToPlayer())
                damage /= 10;
            else
                damage = 0;
        }

        void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType dmgType) override
        {
            if (attacker->ToPlayer())
            {
                damage *= 2;
                return;
            }
            damage /= 2;
        }

        void JustDied(Unit* /*killer*/)
        {
            std::list<Player*> lList;
            me->GetPlayerListInGrid(lList, 200.0f);
            for (auto player : lList)
            {
                if (player->GetQuestStatus(QUEST) == QUEST_STATUS_INCOMPLETE && player->GetQuestObjectiveData(QUEST, NPC_CREDIT) < 99)
                    player->CastSpell(player, SPELL_CREDIT, true);
            }

            std::list<Creature*> creatureList;
            me->GetAliveCreatureListWithEntryInGrid(creatureList, me->GetEntry(), 20.0f);
            if (creatureList.size() < 20)
                me->CastSpell(me, SPELL_SUMMON, false);

        }
    };
};

class sceneTrigger_q34429 : public SceneTriggerScript
{
public:
    sceneTrigger_q34429() : SceneTriggerScript("sceneTrigger_q34429")
    {}

    enum data
    {
        SPELL_CREDIT_100 = 168182,
        SPELL_PHASE = 167960,
        SPELL_CREDIT_ESCAPE = 165265,
    };

    bool OnTrigger(Player* player, SpellScene const* trigger, std::string type) override
    {
        if (type == "Update")
        {
            player->CastSpell(player, SPELL_PHASE, false);
        }
        else if (type == "Hundred")
        {
            player->CastSpell(player, SPELL_CREDIT_100, true);
        }
        else if (type == "Credit")
        {
            player->CastSpell(player, SPELL_CREDIT_ESCAPE, false);

            if (Quest const* qInfo = sQuestDataStore->GetQuestTemplate(34429))
            {
                for (QuestObjective const& obj : qInfo->GetObjectives())
                {
                    if (obj.ObjectID != 82142)
                        continue;
                    player->SetQuestObjectiveData(qInfo, &obj, 99);
                    if (player->CanCompleteQuest(34429))
                        player->CompleteQuest(34429);
                    break;
                }
            }
        }
        playerscript_wod_portal_phases::UpdatePhaseForPlayer(player);
        return true;
    }
};

//- NPC 78994
class mob_wod_irel : public CreatureScript
{
public:
    mob_wod_irel() : CreatureScript("mob_wod_irel") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_irelAI(creature);
    }

    enum data
    {
        SPELL_CREDIT = 160774,
        OBJECTIVE_CREDIT = 79593,
        Q_H = 34737,
        Q_A = 34431,
    };

    struct mob_wod_irelAI : public ScriptedAI
    {
        mob_wod_irelAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        void Reset()
        {
        }

        void MoveInLineOfSight(Unit* who)
        {
            Player *player = who->ToPlayer();
            if (!player || !me->IsWithinDistInMap(who, 25.0f))
                return;
            
            if (player->GetQuestStatus(player->GetTeam() == ALLIANCE ? Q_A : Q_H) != QUEST_STATUS_INCOMPLETE)
                return;

            if (player->GetQuestObjectiveData(player->GetTeam() == ALLIANCE ? Q_A : Q_H, OBJECTIVE_CREDIT))
                return;

            player->CastSpell(player, SPELL_CREDIT, true);
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, player->GetGUID());
            return;
        }
    };
};

class mob_wod_q34434_q34740 : public CreatureScript
{
public:
    mob_wod_q34434_q34740() : CreatureScript("mob_wod_q34434_q34740") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_q34434_q34740AI(creature);
    }

    struct mob_wod_q34434_q34740AI : public ScriptedAI
    {
        EventMap events;
        ObjectGuid playerGuid;

        mob_wod_q34434_q34740AI(Creature* creature) : ScriptedAI(creature)
        {
        }

        enum data
        {
            EVENT_1 = 1,
            EVENT_2,
            EVENT_3,

            EVENT_CHECK_PHASE_1
        };

        void OnCharmed(bool /*apply*/)
        {
        }

        void IsSummonedBy(Unit* summoner)
        {
            Player *player = summoner->ToPlayer();
            if (!player)
            {
                me->MonsterSay("SCRIPT::mob_wod_q34434_q34740AI summoner is not player", LANG_UNIVERSAL, ObjectGuid::Empty);
                return;
            }

            playerGuid = summoner->GetGUID();
            me->AddPlayerInPersonnalVisibilityList(summoner->GetGUID());

            uint32 t = 0;                                                       //
            events.RescheduleEvent(EVENT_1, t += 2000);               //09:34:23.000 
            events.RescheduleEvent(EVENT_2, t += 6000);               //09:34:28.000

            events.RescheduleEvent(EVENT_CHECK_PHASE_1, t += 1000);
        }

        void DoAction(int32 const /*param*/)
        {

        }

        void UpdateAI(uint32 diff)
        {
            UpdateVictim();

            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_1:
                case EVENT_2:
                case EVENT_3:
                    sCreatureTextMgr->SendChat(me, /*TEXT_GENERIC_0*/eventId - 1, playerGuid);
                    break;
                case EVENT_CHECK_PHASE_1:
                    if (me->GetDistance(4516.582f, -2495.618f, 25.87184f) < 40.0f && !me->getVictim())
                    {
                        if (Player* player = sObjectAccessor->FindPlayer(playerGuid))
                            player->KilledMonsterCredit(79794, ObjectGuid::Empty);

                        me->GetMotionMaster()->MovePoint(1, 4516.582f, -2495.618f, 25.87184f);
                        sCreatureTextMgr->SendChat(me, TEXT_GENERIC_2, playerGuid);
                        me->DespawnOrUnsummon(15000);
                    }
                    else
                        events.RescheduleEvent(EVENT_CHECK_PHASE_1, 5000);
                    break;
                default:
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};
// ============================================================
// Q34741 (Horde) / Q34436 (Alliance) — Citadel gate
// GO 233197 (gate) — opens if the player does not have the quest yet
// NPC mob_wod_q34741_34436 — fight / escort through the gate
// ============================================================
//go - 233197 Q: 34741, 34436
class go_wod_gate_q34741_34436 : public GameObjectScript
{
public:
    go_wod_gate_q34741_34436() : GameObjectScript("go_wod_gate_q34741_34436") { }

    struct go_wod_gate_q34741_34436_AI : public GameObjectAI
    {
        go_wod_gate_q34741_34436_AI(GameObject* go) : GameObjectAI(go)
        {
            events.RescheduleEvent(1, 5000);
        }

        enum data
        {
            Q_H = 34741,
            Q_A = 34436,
        };

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                events.RescheduleEvent(1, 1000);

                std::list<Player*> playerList;
                go->GetPlayerListInGrid(playerList, 10.0f);
                for (std::list<Player*>::iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                {
                    if ((*itr)->GetQuestStatus((*itr)->GetTeam() == ALLIANCE ? Q_A : Q_H) == QUEST_STATUS_NONE)
                        continue;
                    if (go->GetGoState() != GO_STATE_ACTIVE_ALTERNATIVE)
                        go->EnableOrDisableGo(true, true);
                    return;
                }
                if (go->GetGoState() == GO_STATE_ACTIVE_ALTERNATIVE)
                    go->EnableOrDisableGo(false, false);                
            }
        }

    private:
        EventMap events;
    };

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_wod_gate_q34741_34436_AI(go);
    }
};

class mob_wod_q34741_34436 : public CreatureScript
{
public:
    mob_wod_q34741_34436() : CreatureScript("mob_wod_q34741_34436") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_q34741_34436_AI(creature);
    }

    struct mob_wod_q34741_34436_AI : public ScriptedAI
    {
        EventMap events;
        bool _event;
        mob_wod_q34741_34436_AI(Creature* creature) : ScriptedAI(creature)
        {
        }

        enum data
        {
            SPELL_VOID_SHELL = 165880, //Void Shell
            SPELL_VOID_BOLT_VOLLEY = 165875, //Void Bolt Volley
            SPELL_VOID_VACUUM = 165871, //Void Vacuum

            SPELL_VOID_COSMETIC_MISLE = 165316,

            EVENT_COMBAT_SPELL_1 = 1,
            EVENT_SHELL,
            EVENT_3,
            EVENT_4,
            EVENT_5,
            EVENT_6,

        };

        void Reset() override
        {
            
        }

        void JustDied(Unit* killer)
        {
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_3, ObjectGuid::Empty);
        }

        void EnterEvadeMode() override
        {
            events.CancelEvent(EVENT_COMBAT_SPELL_1);
            events.CancelEvent(EVENT_SHELL);
            ScriptedAI::EnterEvadeMode();
        }

        void EnterCombat(Unit* /*victim*/) override
        {
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, ObjectGuid::Empty);
            events.RescheduleEvent(EVENT_COMBAT_SPELL_1, 5000);
            events.RescheduleEvent(EVENT_SHELL, 8000);
        }

        void UpdateAI(uint32 diff)
        {
            UpdateVictim();

            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_COMBAT_SPELL_1:
                    {
                        if (Unit* victim = me->getVictim())
                            me->CastSpell(victim, 166911, false);
                        events.RescheduleEvent(EVENT_COMBAT_SPELL_1, 5000);
                        break;
                    }
                    case EVENT_SHELL:
                        sCreatureTextMgr->SendChat(me, TEXT_GENERIC_2, ObjectGuid::Empty);
                        me->CastSpell(me, SPELL_VOID_VACUUM, false);
                        events.RescheduleEvent(EVENT_SHELL, 20000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

class sceneTrigger_q34741_34436 : public SceneTriggerScript
{
public:
    sceneTrigger_q34741_34436() : SceneTriggerScript("sceneTrigger_q34741_34436")
    {}

    enum data
    {
        SPELL_CREDIT_100 = 168182,
        SPELL_PHASE = 167960,
        SPELL_CREDIT_ESCAPE = 165265,
    };

    bool OnTrigger(Player* player, SpellScene const* trigger, std::string type) override
    {
        if (type == "Teleport")
        {
            player->TeleportTo(1265, 4519.2f, -2294.0f, 33.8118f, 1.343904f);
        }
        playerscript_wod_portal_phases::UpdatePhaseForPlayer(player);
        return true;
    }
};

// ============================================================
// Q34439 — (past the gate)
// Spells: 167891 (Big), 167890 (Small) — scene effects
// ============================================================
class sceneTrigger_q34439 : public SceneTriggerScript
{
public:
    sceneTrigger_q34439() : SceneTriggerScript("sceneTrigger_q34439")
    {}

    enum data
    {
        SPELL_BIG = 167891,
        SPELL_SMALL = 167890,
    };

    bool OnTrigger(Player* player, SpellScene const* trigger, std::string type) override
    {
        if (type == "Big")
        {
            player->CastSpell(player, SPELL_BIG, false);
        }
        else if (type == "Small")
        {
            player->CastSpell(player, SPELL_SMALL, false);
        }
        playerscript_wod_portal_phases::UpdatePhaseForPlayer(player);
        return true;
    }
};

class sceneTrigger_q34987 : public SceneTriggerScript
{
public:
    sceneTrigger_q34987() : SceneTriggerScript("sceneTrigger_q34987")
    {}

    enum data
    {
        NPC__ = 78569,
        SPELL_TALKA = 163678,
        SPELL_SMALL = 167890,
    };

    bool OnTrigger(Player* player, SpellScene const* trigger, std::string type) override
    {
        if (type == "TalkA")
        {
            if (Creature *c = player->FindNearestCreature(NPC__, 100.0f))
                sCreatureTextMgr->SendChat(c, TEXT_GENERIC_1, player->GetGUID());
        }
        else if (type == "TalkB")
        {
            if (Creature *c = player->FindNearestCreature(NPC__, 100.0f))
                sCreatureTextMgr->SendChat(c, TEXT_GENERIC_2, player->GetGUID());
            player->CastSpell(player, SPELL_SMALL, false);
        }
        else if (type == "TalkC")
        {
            if (Creature *c = player->FindNearestCreature(NPC__, 100.0f))
                sCreatureTextMgr->SendChat(c, TEXT_GENERIC_3, player->GetGUID());
        }
        else if (type == "TalkD")
        {
            if (Creature *c = player->FindNearestCreature(NPC__, 100.0f))
                sCreatureTextMgr->SendChat(c, TEXT_GENERIC_4, player->GetGUID());
        }
        return true;
    }
};

// ============================================================
// Q35747 — (Thaelin Darkanvil — NPC 78558)
// Credit 80880 through the gossip dialogue.
// Also handles Q34392 Onslaught check (isActive guard).
// ============================================================
class mob_wod_thaelin_darkanvil : public CreatureScript
{
public:
    mob_wod_thaelin_darkanvil() : CreatureScript("mob_wod_thaelin_darkanvil") { }

    enum data
    {
        QUEST = 35747,
        CREDIT = 80880,
        QUEST_ONSLAUGHT = 34392,
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new mob_wod_thaelin_darkanvilAI(creature);
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == 1)
        {
            player->CLOSE_GOSSIP_MENU();
            if (player->GetQuestStatus(QUEST) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(CREDIT, ObjectGuid::Empty);
                player->GetPhaseMgr().RemoveUpdateFlag(PHASE_UPDATE_FLAG_ZONE_UPDATE);
                return true;
            }
        }
        return false;
    }

    struct mob_wod_thaelin_darkanvilAI : public ScriptedAI
    {
        mob_wod_thaelin_darkanvilAI(Creature* c) : ScriptedAI(c) {}

        std::set<ObjectGuid> _greeted;

        void Reset() override
        {
            _greeted.clear();
        }

        void MoveInLineOfSight(Unit* who) override
        {
            Player* player = who->ToPlayer();
            if (!player || !me->IsWithinDistInMap(who, 15.0f))
                return;
            if (player->GetQuestStatus(QUEST_ONSLAUGHT) != QUEST_STATUS_INCOMPLETE)
                return;
            if (_greeted.count(player->GetGUID()))
                return;
            _greeted.insert(player->GetGUID());
            // GroupID=1: "Don't worry. We're here to cover you!"
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_1, player->GetGUID());
        }
    };
};

class mob_wod_q35747 : public CreatureScript
{
public:
    mob_wod_q35747() : CreatureScript("mob_wod_q35747") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wod_q35747AI(creature);
    }

    struct mob_wod_q35747AI : public ScriptedAI
    {
        EventMap events;
        ObjectGuid playerGuid;
        uint8 state;

        mob_wod_q35747AI(Creature* creature) : ScriptedAI(creature)
        {
            state = 0;
        }

        enum data
        {
            EVENT_1 = 1,
            EVENT_2,
            EVENT_3,

            EVENT_CHECK_PHASE_1
        };

        void OnCharmed(bool /*apply*/)
        {
        }

        void IsSummonedBy(Unit* summoner)
        {
            Player *player = summoner->ToPlayer();
            if (!player)
            {
                me->MonsterSay("SCRIPT::mob_wod_q35747 summoner is not player", LANG_UNIVERSAL, ObjectGuid::Empty);
                return;
            }

            playerGuid = summoner->GetGUID();
            me->AddPlayerInPersonnalVisibilityList(summoner->GetGUID());

            me->SetEntry(78568);
            me->SetDisplayId(52540);
            me->SetNativeDisplayId(54576);

            //! after entry set.
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, playerGuid);

            events.RescheduleEvent(EVENT_CHECK_PHASE_1, 5000);
        }

        void UpdateAI(uint32 diff)
        {
            UpdateVictim();

            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHECK_PHASE_1:
                    if (me->GetDistance(4065.0f, -1955.0f, 27.0f) < 40.0f && !state)
                    {
                        state = 1;
                        sCreatureTextMgr->SendChat(me, TEXT_GENERIC_1, playerGuid);      
                    }

                    if (state == 1 && me->GetDistance(4063.77f, -2020.122f, 75.47333f) < 10.0f)
                    {
                        sCreatureTextMgr->SendChat(me, TEXT_GENERIC_2, playerGuid);
                        me->GetMotionMaster()->MovePoint(1, 4063.77f, -2020.122f, 75.47333f);
                        me->DespawnOrUnsummon(1000);
                        if (Player* player = sObjectAccessor->FindPlayer(playerGuid))
                            player->KilledMonsterCredit(80887, ObjectGuid::Empty);
                        return;
                    }
                    events.RescheduleEvent(EVENT_CHECK_PHASE_1, 5000);
                    break;
                default:
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

// ============================================================
// Q34445 — (Khadgar's Watch cinematic)
// GO 34445 (go_wod_q34445) → spell 176159 (cinematic scene)
// SceneTrigger: spells 176104 (instructions), 161527 (bloodlust), 164043 (GO)
// ============================================================
class sceneTrigger_q34445 : public SceneTriggerScript
{
public:
    sceneTrigger_q34445() : SceneTriggerScript("sceneTrigger_q34445")
    {}

    enum data
    {
        NPC__ = 80521,
        CREDIT_ = 80016,
        SPEL_INST = 176104,
        SPELL_BLOOD = 161527,
        SPELL_GO = 164043,
        //167976 
    };

    bool OnTrigger(Player* player, SpellScene const* trigger, std::string type) override
    {
        if (type == "Instructions")
        {
            player->CastSpell(player, SPEL_INST, false);
            if (Creature *c = player->FindNearestCreature(NPC__, 100.0f))
                sCreatureTextMgr->SendChat(c, TEXT_GENERIC_1, player->GetGUID());
        }
        else if (type == "Credit")
        {
            player->KilledMonsterCredit(CREDIT_, ObjectGuid::Empty);
        }
        else if (type == "Bloodlust")
        {
            player->CastSpell(player, SPELL_BLOOD, false);
        }
        else if (type == "CancelGame")
        {
            player->CastSpell(player, SPELL_GO, false);
        }
        playerscript_wod_portal_phases::UpdatePhaseForPlayer(player);
        return true;
    }
};

class go_wod_q34445 : public GameObjectScript
{
public:
    go_wod_q34445() : GameObjectScript("go_wod_q34445") { }

    struct go_wod_q34445AI : public GameObjectAI
    {
        go_wod_q34445AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            SPELL_CINEMA = 176159,

        };
        bool GossipHello(Player* player, bool isUse) override
        {
            if (!isUse)
                return true;

            if (player->GetQuestStatus(34445) == QUEST_STATUS_INCOMPLETE)
                player->CastSpell(player, SPELL_CINEMA, true);
            return false;
        }

    };

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_wod_q34445AI(go);
    }
};

//! 167421 Khadgar's Watch
//! Q: 34446 35884
class spell_wod_khadgar_watch : public SpellScriptLoader
{
public:
    spell_wod_khadgar_watch() : SpellScriptLoader("spell_wod_khadgar_watch") { }

    class spell_wod_khadgar_watch_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_wod_khadgar_watch_AuraScript);

        enum data
        {
            SPELL_A = 163524,
            SPELL_B = 163525,
            SPELL_C = 163526,
            SPELL_D = 163527,
            SPELL_E = 163528,
            SPELL_F = 163529,
            SPELL_G = 163530,

            QUEST_A = 34446,
            QUEST_B = 35884,

            CREDIT_ = 81024,
        };

        void OnTick(AuraEffect const* aurEff)
        {
            if (!GetCaster())
                return;

            if (Player* player = GetCaster()->ToPlayer())
            {
                if (player->GetQuestStatus(QUEST_A) == QUEST_STATUS_INCOMPLETE ||
                    player->GetQuestStatus(QUEST_B) == QUEST_STATUS_INCOMPLETE)
                {
                    if (!player->IsInWorld())
                        return;

                    if (player->GetDistance(4021.0f, -2000.0f, 30.0f) < 20.0f && !player->HasAura(SPELL_A))
                        player->CastSpell(player, SPELL_A, true);

                    if (player->GetDistance(3970.787f, -2010.345f, 28.65494f) < 20.0f && !player->HasAura(SPELL_B))
                        player->CastSpell(player, SPELL_B, true);

                    if (player->GetDistance(3888.591f, -2025.13f, 19.50598f) < 20.0f && !player->HasAura(SPELL_C))
                        player->CastSpell(player, SPELL_C, true);

                    if (player->GetDistance(3888.591f, -2025.13f, 19.50598f) < 20.0f && !player->HasAura(SPELL_D))
                        player->CastSpell(player, SPELL_D, true);

                    if (player->GetDistance(3792.147f, -2065.47f, 14.06284f) < 20.0f && !player->HasAura(SPELL_E))
                        player->CastSpell(player, SPELL_E, true);

                    if (player->GetDistance(3792.147f, -2065.47f, 14.06284f) < 20.0f && !player->HasAura(SPELL_F))
                        player->CastSpell(player, SPELL_F, true);

                    if (player->GetDistance(3704.125f, -2049.134f, 9.72922f) < 20.0f && !player->HasAura(SPELL_G))
                        player->CastSpell(player, SPELL_G, true);

                    if (player->GetDistance(3554.205f, -2120.661f, 19.31005f) < 20.0f)
                        player->KilledMonsterCredit(CREDIT_, ObjectGuid::Empty);
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_wod_khadgar_watch_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_wod_khadgar_watch_AuraScript();
    }
};

// GO 231985 — Plunger — Q: 34392 "Onslaught's End"
// Animation only: Thaelin speaks → the door blows up and disappears.
// The quest credit comes from the Fel Spires (already working).
class go_wod_q34392_plunger : public GameObjectScript
{
public:
    go_wod_q34392_plunger() : GameObjectScript("go_wod_q34392_plunger") { }

    struct go_wod_q34392_plungerAI : public GameObjectAI
    {
        go_wod_q34392_plungerAI(GameObject* go) : GameObjectAI(go) { }

        enum
        {
            NPC_THAELIN     = 78558,
            GO_CHAMBER_DOOR = 233614,
            QUEST_ONSLAUGHT = 34392,
            SPELL_EXPLOSION = 68975,
        };

        bool GossipHello(Player* player, bool isUse) override
        {
            if (!isUse)
                return true;

            if (player->GetQuestStatus(QUEST_ONSLAUGHT) != QUEST_STATUS_INCOMPLETE)
                return true;

            // Already used
            if (go->GetGoState() == GO_STATE_ACTIVE)
                return true;

            bool isEast = go->GetPositionX() > 4065.0f;

            // Thaelin comments on the ignition (TEXT_GENERIC_2 = east, TEXT_GENERIC_3 = west)
            if (Creature* thaelin = go->FindNearestCreature(NPC_THAELIN, 150.0f, true))
                sCreatureTextMgr->SendChat(thaelin,
                    isEast ? TEXT_GENERIC_2 : TEXT_GENERIC_3,
                    player->GetGUID());

            // Visual explosion + door removal
            if (GameObject* door = go->FindNearestGameObject(GO_CHAMBER_DOOR, 100.0f))
            {
                door->CastSpell(nullptr, SPELL_EXPLOSION);
                door->Delete();
            }

            // Disable the plunger once used
            go->SetFlag(11, GO_FLAG_NOT_SELECTABLE);
            go->SetGoState(GO_STATE_ACTIVE);

            return true;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_wod_q34392_plungerAI(go);
    }
};

// GO 233056 (Mark of the Shadowmoon, X=4167, north) → credit 82606 (Q34392)
// GO 233057 (Mark of the Bleeding Hollow, X=3964, south) → credit 82607 (Q34392)
class gob_mark_of_tanaan : public GameObjectScript
{
public:
    gob_mark_of_tanaan() : GameObjectScript("gob_mark_of_tanaan") {}

    enum
    {
        QUEST_ONSLAUGHT = 34392,
        CREDIT_NORTH    = 82606,
        CREDIT_SOUTH    = 82607,
    };

    struct gob_mark_of_tanaanAI : public GameObjectAI
    {
        gob_mark_of_tanaanAI(GameObject* go) : GameObjectAI(go), _deleteTimer(0) {}

        uint32 _deleteTimer;

        void UpdateAI(uint32 diff) override
        {
            if (!_deleteTimer)
                return;
            if (diff >= _deleteTimer)
            {
                go->SetRespawnTime(0);
                go->Delete();
            }
            else
                _deleteTimer -= diff;
        }

        bool GossipHello(Player* player, bool isUse) override
        {
            if (!isUse) return false;
            if (player->GetQuestStatus(QUEST_ONSLAUGHT) != QUEST_STATUS_INCOMPLETE) return false;
            if (go->GetGoState() == GO_STATE_ACTIVE) return false;

            bool isNorth = (go->GetEntry() == 233056);
            uint32 credit = isNorth ? CREDIT_NORTH : CREDIT_SOUTH;
            if (player->GetQuestObjectiveData(QUEST_ONSLAUGHT, credit) > 0) return false;

            player->KilledMonsterCredit(credit, ObjectGuid::Empty);
            playerscript_wod_portal_phases::UpdatePhaseForPlayer(player);
            go->SetFlag(11, GO_FLAG_NOT_SELECTABLE);
            go->SetGoState(GO_STATE_ACTIVE);
            go->SetAnimKitId(0);
            go->UpdateObjectVisibility();
            _deleteTimer = 2000; // despawns 2s after use
            return true;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new gob_mark_of_tanaanAI(go);
    }
};

// GO 229598 → credit 78885 (Mark of the Burning Blade,  Q34393)
// GO 229599 → credit 78886 (Mark of the Shattered Hand, Q34393)
// GO 229600 → credit 78887 (Mark of the Blackrock,      Q34393)
class gob_q34393_mark : public GameObjectScript
{
public:
    gob_q34393_mark() : GameObjectScript("gob_q34393_mark") {}

    enum { QUEST_PORTAL = 34393 };

    struct gob_q34393_markAI : public GameObjectAI
    {
        gob_q34393_markAI(GameObject* go) : GameObjectAI(go) {}

        bool GossipHello(Player* player, bool isUse) override
        {
            if (!isUse) return false;
            if (player->GetQuestStatus(QUEST_PORTAL) != QUEST_STATUS_INCOMPLETE) return false;
            if (go->GetGoState() == GO_STATE_ACTIVE) return false;

            uint32 credit = 0;
            uint8  objIdx = 0;
            switch (go->GetEntry())
            {
                case 229598: credit = 78885; objIdx = 0; break;
                case 229599: credit = 78886; objIdx = 1; break;
                case 229600: credit = 78887; objIdx = 2; break;
                default: return false;
            }
            if (player->GetQuestObjectiveData(QUEST_PORTAL, objIdx) > 0) return false;

            player->KilledMonsterCredit(credit, ObjectGuid::Empty);
            go->SetFlag(11, GO_FLAG_NOT_SELECTABLE);
            go->SetGoState(GO_STATE_ACTIVE);
            go->SetAnimKitId(0);           // stops the crystal glow animation
            go->UpdateObjectVisibility();  // forces the state change to be sent to clients
            return true;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new gob_q34393_markAI(go);
    }
};

// GO 233104 (Stasis Rune): checks the 3 Q34393 marks, grants credit 78333, plays the Gul'dan scene
// Scene: spell 163807 → MiscValue 756 → SceneScriptPackageID 925
class gob_stasis_rune : public GameObjectScript
{
public:
    gob_stasis_rune() : GameObjectScript("gob_stasis_rune") {}

    enum
    {
        QUEST_PORTAL       = 34393,
        CREDIT_STASIS_RUNE = 78333,
        SPELL_GULDAN_SCENE = 163807,
    };

    struct gob_stasis_runeAI : public GameObjectAI
    {
        gob_stasis_runeAI(GameObject* go) : GameObjectAI(go) {}

        bool GossipHello(Player* player, bool isUse) override
        {
            if (!isUse) return false;
            if (player->GetQuestStatus(QUEST_PORTAL) != QUEST_STATUS_INCOMPLETE) return false;
            if (go->GetGoState() == GO_STATE_ACTIVE) return false;

            // All 3 marks must be done (objectives idx 0, 1, 2)
            if (player->GetQuestObjectiveData(QUEST_PORTAL, 0) == 0) return false;
            if (player->GetQuestObjectiveData(QUEST_PORTAL, 1) == 0) return false;
            if (player->GetQuestObjectiveData(QUEST_PORTAL, 2) == 0) return false;

            player->KilledMonsterCredit(CREDIT_STASIS_RUNE, ObjectGuid::Empty);
            player->CastSpell(player, SPELL_GULDAN_SCENE, true);
            go->SetFlag(11, GO_FLAG_NOT_SELECTABLE);
            go->SetGoState(GO_STATE_ACTIVE);
            return true;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new gob_stasis_runeAI(go);
    }
};

// Ambient scene "Iron Grunts pushing prisoners toward the portal" (spell 163341 = SPELL_AURA_ACTIVATE_SCENE).
// spell_area removes this spell on its own when Q34393 is rewarded (quest_end=34393, quest_end_status=66).
// This script keeps it until Q34420 (Into the Portal) is rewarded:
//   - OnMapChanged  : applies the spell on arrival on map 1265 (if Q34420 is not rewarded)
//   - OnQuestReward : reapplies it on Q34393 (undoes the spell_area removal)
//                     removes the spell on Q34420
class playerscript_wod_portal_ambient : public PlayerScript
{
public:
    playerscript_wod_portal_ambient() : PlayerScript("playerscript_wod_portal_ambient") {}

    enum
    {
        MAP_WOD_INTRO       = 1265,
        SPELL_AMBIENT_SCENE = 163341,   // SPELL_AURA_ACTIVATE_SCENE — grunts/prisoners
        QUEST_34393         = 34393,    // Portal Energy (spell_area removes 163341 when it is rewarded)
        QUEST_STOP          = 34420,    // Into the Portal → end of the scene
    };

    void OnMapChanged(Player* player) override
    {
        if (player->GetMapId() != MAP_WOD_INTRO)
            return;
        if (player->IsQuestRewarded(QUEST_STOP))
            return;
        player->CastSpell(player, SPELL_AMBIENT_SCENE, true);
    }

    void OnQuestReward(Player* player, Quest const* quest) override
    {
        if (player->GetMapId() != MAP_WOD_INTRO)
            return;

        switch (quest->GetQuestId())
        {
            case QUEST_34393:
                // spell_area just removed 163341; put it back until Q34420.
                if (!player->IsQuestRewarded(QUEST_STOP))
                    player->CastSpell(player, SPELL_AMBIENT_SCENE, true);
                break;
            case QUEST_STOP:
                player->RemoveAurasDueToSpell(SPELL_AMBIENT_SCENE);
                break;
            default:
                break;
        }
    }
};

void AddSC_wod_dark_portal()
{
    new mob_wod_thrall();
    new mob_wod_cordona_welsong();
    new mob_wod_archimage_khadgar();
    new mob_wod_olin_oberhind();
    new mob_wod_intro_enemy_at_portal();
    new mob_wod_shadowmoon_ritualist();
    new mob_wod_intro_guldan();
    new mob_wod_frostwolf_slave();
    new go_wod_slaves_cage();
    new mob_wod_ariok();
    new mob_wod_hansel();
    new mob_wod_ariok_mover();
    new spell_wod_destroying();
    new mob_khadgar_q34425();
    new sceneTrigger_q34425();
    new mob_arena_combatant_q34429();
    new sceneTrigger_q34429();
    new mob_wod_irel();
    new mob_wod_q34434_q34740();
    new go_wod_gate_q34741_34436();
    new mob_wod_q34741_34436();
    new sceneTrigger_q34741_34436();
    new sceneTrigger_q34439();
    new sceneTrigger_q34987();
    new mob_wod_thaelin_darkanvil();
    new mob_wod_q35747();
    new sceneTrigger_q34445();
    new go_wod_q34445();
    new go_wod_q34392_plunger();
    new gob_mark_of_tanaan();
    new gob_q34393_mark();
    new gob_stasis_rune();
    new spell_wod_khadgar_watch();
    new playerscript_wod_portal_ambient();
    new playerscript_wod_portal_phases();
}