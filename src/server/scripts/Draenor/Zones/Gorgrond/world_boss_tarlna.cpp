////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Gorgrond: Tarlna the Ageless, world boss, and her Giant Lashers
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "DB2Stores.h"
#include "ObjectAccessor.h"
#include "Player.h"

enum eSpells
{
    SPELL_COLOSSAL_SLAM_TARLNA = 175973,
    SPELL_SUMMON_MANDRAGORA = 176013,
    SPELL_SAVAGE_VINES = 176001,
    SPELL_SUMMON_MAIN_ADDS = 175975,
    SPELL_GENESIS = 175979,
    SPELL_WAITED_FINISH = 175123,
};

// 81535
class boss_tarlna : public CreatureScript
{
public:
    boss_tarlna() : CreatureScript("boss_tarlna") { }

    struct boss_tarlnaAI : public ScriptedAI
    {
        boss_tarlnaAI(Creature* creature) : ScriptedAI(creature), summons(me) {}

        EventMap events;
        SummonList summons;

        void Reset()
        {
            events.Reset();
            summons.DespawnAll();
        }

        void EnterCombat(Unit* unit)
        {
            events.RescheduleEvent(EVENT_1, 16000);
            events.RescheduleEvent(EVENT_2, 36000);
            events.RescheduleEvent(EVENT_3, 26000); // 16
            events.RescheduleEvent(EVENT_4, 49000);
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
            DoZoneInCombat(summon, 150.0f);
            if (summon->GetEntry() == 88142)
                summon->CastSpell(summon, SPELL_WAITED_FINISH);
        }

        void OnApplyOrRemoveAura(uint32 spellId, AuraRemoveMode mode, bool apply)
        {
            if (!apply)
                return;

            if (spellId == SPELL_GENESIS)
            {
                for (uint8 i = urand(2,4); i > 0; --i)
                    me->CastSpell(me->GetPositionX() + irand(-16, 16), me->GetPositionY() + irand(-16, 16), me->GetPositionZ(), SPELL_SUMMON_MAIN_ADDS);

                std::list<HostileReference*> threat_list = me->getThreatManager().getThreatList();

                if (!threat_list.empty())
                    for (std::list<HostileReference*>::const_iterator itr = threat_list.begin(); itr!= threat_list.end(); ++itr)
                    {
                        if(Unit* target = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
                            for (uint8 i = urand(2,4); i > 0; --i)
                                me->CastSpell(target->GetPositionX() + irand(-6, 6), target->GetPositionY() + irand(-6, 6), target->GetPositionZ(), SPELL_SUMMON_MAIN_ADDS);
                    }
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_COLOSSAL_SLAM_TARLNA);
                        events.RescheduleEvent(EVENT_1, 16000);
                        break;
                    case EVENT_2:
                        DoCast(SPELL_SUMMON_MANDRAGORA);
                        events.RescheduleEvent(EVENT_2, 46000);
                        break;
                    case EVENT_3:
                        DoCast(SPELL_SAVAGE_VINES);
                        events.RescheduleEvent(EVENT_3, 16000); // 16
                        break;
                    case EVENT_4:
                        DoCast(SPELL_GENESIS);
                        events.RescheduleEvent(EVENT_4, 49000);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_tarlnaAI(creature);
    }
};

// 88142
class npc_giant_lasher : public CreatureScript
{
public:
    npc_giant_lasher() : CreatureScript("npc_giant_lasher") { }

    struct npc_giant_lasherAI : public ScriptedAI
    {
        npc_giant_lasherAI(Creature* creature) : ScriptedAI(creature)
        {
            justCasted = false;
        }

        EventMap events;
        bool justCasted;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* unit)
        {
            events.RescheduleEvent(EVENT_1, 4000);
        }

        void IsSummonedBy(Unit* summoner)
        {
            me->AddDelayedEvent(13000, [this] () -> void
            {
                me->RemoveAurasDueToSpell(SPELL_WAITED_FINISH);
                if (Player* pl = me->FindNearestPlayer(70.0f, true))
                    AttackStart(pl);
            });
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER && !justCasted && me->IsWithinDistInMap(who, 2.0f) && me->HasAura(SPELL_WAITED_FINISH))
            {
                justCasted = true;
                who->CastSpell(me, 175986);
                who->Kill(me);
                me->DespawnOrUnsummon(2000);
            }
        }


        void UpdateAI(uint32 diff)
        {
            if (me->HasAura(SPELL_WAITED_FINISH))
                return;

            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(176000);
                        events.RescheduleEvent(EVENT_1, 4000);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_giant_lasherAI(creature);
    }
};

void AddSC_world_boss_tarlna()
{
    new boss_tarlna();
    new npc_giant_lasher();
}
