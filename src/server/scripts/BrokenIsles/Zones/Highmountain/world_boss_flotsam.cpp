////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Highmountain: Flotsam, world boss
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum eSpells
{
    SPELL_STALKER = 220277,
    SPELL_BREAKSAM = 223317,
    SPELL_YAKSAM = 223373,
    SPELL_GETSAM = 220340,
    SPELL_JETSAM = 220295,
    SPELL_REGEN = 223304,
};

// 99929
class boss_flotsam : public CreatureScript
{
public:
    boss_flotsam() : CreatureScript("boss_flotsam") {}

    struct boss_flotsamAI : public ScriptedAI
    {
        boss_flotsamAI(Creature* creature) : ScriptedAI(creature), summons(me) {}

        EventMap events;
        SummonList summons;

        void Reset() override
        {
            events.Reset();
            me->SetMaxPower(POWER_MANA, 100);
            me->SetPower(POWER_MANA, 0);
            summons.DespawnAll();
            me->RemoveAura(SPELL_REGEN);
        }

        void EnterCombat(Unit* unit) override
        {
            events.RescheduleEvent(EVENT_1, urand(8000, 9000)); //триггер на рандома SPELL_STALKER линк SPELL_JETSAM
            events.RescheduleEvent(EVENT_2, urand (16000, 19000)); // SPELL_BREAKSAM
            events.RescheduleEvent(EVENT_3, 40000); //SPELL_YAKSAM
            events.RescheduleEvent(EVENT_4, 1000); //проверка маны. Если 100 - каст SPELL_GETSAM
            me->SetPower(POWER_MANA, 0);
            DoCast(SPELL_REGEN);
        }

        void JustDied(Unit* who) override
        {
            Talk(1);
        }

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);
            DoZoneInCombat(summon, 150.0f);
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
                    case EVENT_1:
                        if (auto target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true))
                            DoCast(target, SPELL_STALKER, true);
                        events.RescheduleEvent(EVENT_1, urand(8000, 9000)); //триггер на рандома SPELL_STALKER ЛИНК SPELL_JETSAM
                        break;
                    case EVENT_2:
                        DoCast(SPELL_BREAKSAM);
                        events.RescheduleEvent(EVENT_2, urand (16000, 19000)); // SPELL_BREAKSAM
                        break;
                    case EVENT_3:
                        DoCast(SPELL_YAKSAM);
                        Talk(0);
                        events.RescheduleEvent(EVENT_3, 40000); //SPELL_YAKSAM
                        events.RescheduleEvent(EVENT_5, 6000); // adds
                        break;
                    case EVENT_4:
                        if (me->GetPower(POWER_MANA) == 100) //не понятно, когда и как копится энергия и почему стоит МАНАкост. (снифы нужны каста)
                            DoCast(SPELL_GETSAM);
                        events.RescheduleEvent(EVENT_4, 1000);
                        break;
                    case EVENT_5:
                        DoCast(223391);
                        DoCast(223403);
                        DoCast(223404);
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_flotsamAI(creature);
    }
};

// 110772
class npc_jetsam_stalker : public CreatureScript
{
public:
    npc_jetsam_stalker() : CreatureScript("npc_jetsam_stalker") {}

    struct npc_jetsam_stalkerAI : public ScriptedAI
    {
        npc_jetsam_stalkerAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_REMOVE_CLIENT_CONTROL);
            me->SetReactState(REACT_PASSIVE);
           // me->ApplySpellImmune(220354, IMMUNITY_DAMAGE, 0, true);
           first = false;
        }

        bool first;

        void IsSummonedBy(Unit* summoner) override
        {
            summoner->CastSpell(me, SPELL_JETSAM);
        }

        void SpellHit(Unit* /*caster */, SpellInfo const* spell) override
        {
            if (spell->Id == 220340 && !first)
            {
                first = true;
                DoCast(220367);
                me->DespawnOrUnsummon(100);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_jetsam_stalkerAI(creature);
    }
};

void AddSC_world_boss_flotsam()
{
    new boss_flotsam();
    new npc_jetsam_stalker();
}
