////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Timeless Isle: Yu'lon, the Jade Serpent, Celestial world boss
*/

#include "ScriptMgr.h"

enum Says
{
    SAY_ENTER_POS = 0,
    SAY_AGGRO = 1,
    SAY_YULON_PLAYER_DEATH = 2,
    SAY_YULON_WALL_1 = 3,
    SAY_YULON_WALL_2 = 4,
    SAY_YULON_END = 5,
};

enum Npc
{
    NPC_JADEFIRE_WALL = 72020,
};

enum Spells
{
    SPELL_CELESTIAL_SPAWN = 149307,
    SPELL_JADEFLAME_BUFFET = 144630,
    SPELL_JADEFIRE_BREATH = 144530,
    SPELL_JADEFIRE_BOLT = 144545,
    SPELL_JADEFIRE_WALL = 144533,
};

enum Events
{
    EVENT_JADEFLAME_BUFFET = 3,
    EVENT_JADEFIRE_BREATH = 4,
    EVENT_JADEFIRE_BOLT = 5,
    EVENT_FIRE_WALL = 6,
};

enum Actions
{
    ACTION_MOVE_CENTR_POSS = 1,
    ACTION_YULON_END = 6,
    ACTION_YULON_FAIL = 7,
};

const Position summonPos[4] = 
{
    {-553.17f, -4949.02f, -6.27f, -2.30f},
    {-740.80f, -4955.08f, -6.27f, 5.63f},
    {-746.32f, -5078.48f, -6.27f, 0.69f},
    {-551.16f, -5080.01f, -6.27f, 2.28f}
};

const Position CentrPos[1] =
{
    {-650.14f, -5024.67f, -6.27f}
};

const Position WallPos[7] =
{
    {-680.63f, -5091.60f, -6.19f, 1.58f},
    {-713.89f, -5091.50f, -6.19f, 1.57f},
    {-580.55f, -5091.66f, -6.19f, 1.58f},
    {-613.91f, -5091.65f, -6.19f, 1.54f},
    {-547.26f, -5091.75f, -6.19f, 1.56f},
    {-647.20f, -5091.63f, -6.19f, 1.57f},
    {-747.19f, -5091.65f, -6.19f, 1.59f}
};

class boss_yulon : public CreatureScript
{
public:
    boss_yulon() : CreatureScript("boss_yulon") {}

    struct boss_yulonAI : public ScriptedAI
    {
        boss_yulonAI(Creature* creature) : ScriptedAI(creature), summons(me) {}

        EventMap events;
        SummonList summons;
        bool EventProgress;
        bool wallrand;

        void Reset() 
        {
            EventProgress = false;
            me->setFaction(35);
            me->RemoveAllAuras();
        }

        void EnterCombat(Unit* who)
        {
            Talk(SAY_AGGRO);
            events.RescheduleEvent(EVENT_JADEFLAME_BUFFET, 20000);
            events.RescheduleEvent(EVENT_JADEFIRE_BREATH, 4000);
            events.RescheduleEvent(EVENT_JADEFIRE_BOLT, 21000);
            events.RescheduleEvent(EVENT_FIRE_WALL, 46000);
            me->SetReactState(REACT_AGGRESSIVE);
        }

        void EnterEvadeMode()
        {
            me->SetReactState(REACT_PASSIVE);
            events.Reset();
            summons.DespawnAll();
            me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());

            if (EventProgress)
            {
                me->NearTeleportTo(summonPos[2].GetPositionX(), summonPos[2].GetPositionY(), summonPos[2].GetPositionZ(), summonPos[2].GetOrientation());
                me->SetHomePosition(summonPos[2]);
                if (me->ToTempSummon() && me->ToTempSummon()->GetSummoner())
                    if (auto Shao = me->ToTempSummon()->GetSummoner()->ToCreature())
                        Shao->AI()->DoAction(ACTION_YULON_FAIL);
            }
            else
                events.RescheduleEvent(EVENT_2, 5000);

            ScriptedAI::EnterEvadeMode();
        }

        void KilledUnit(Unit* victim)
        {
            if (victim && victim->ToPlayer())
                Talk(SAY_YULON_PLAYER_DEATH);
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
                case ACTION_MOVE_CENTR_POSS:
                    EventProgress = true;
                    me->SetHomePosition(CentrPos[0]);
                    me->GetMotionMaster()->MovePoint(1, CentrPos[0]);
                    break;
            }
        }

        void DamageTaken(Unit* who, uint32& damage, DamageEffectType dmgType)
        {
            if (!who || !who->IsPlayer())
                return;

            if (damage >= me->GetHealth())
            {
                damage = 0;

                if (EventProgress)
                {
                    EventProgress = false;
                    QuestCredit();
                    me->setFaction(35);
                    me->RemoveAllAuras();
                    me->GeneratePersonalLoot(me, (Player*)who);
                    Talk(SAY_YULON_END);
                    if (auto Shao = me->GetAnyOwner())
                        Shao->GetAI()->DoAction(ACTION_YULON_END);
                }
                EnterEvadeMode();
            }
        }

        void QuestCredit()
        {
            std::list<HostileReference*> ThreatList = me->getThreatManager().getThreatList();
            for (std::list<HostileReference*>::const_iterator itr = ThreatList.begin(); itr != ThreatList.end(); ++itr)
            {
                Player *pTarget = Player::GetPlayer(*me, (*itr)->getUnitGuid());
                if (!pTarget)
                    return;

                pTarget->KilledMonsterCredit(me->GetEntry());
            }
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);

            summon->SetReactState(REACT_PASSIVE);
            summon->SetDisplayId(11686);

            if (summon->GetEntry() == NPC_JADEFIRE_WALL)
            {
                summon->AI()->DoCast(SPELL_JADEFIRE_WALL);
                float x, y, z;
                summon->GetClosePoint(x, y, z, me->GetObjectSize(), 150.0f);
                summon->GetMotionMaster()->MovePoint(1, x, y, z);
                summon->DespawnOrUnsummon(15000);
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == 1)
            {
                Talk(SAY_ENTER_POS);
                me->SetFacingTo(1.5f);
                events.RescheduleEvent(EVENT_1, 5000);
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim() && me->isInCombat())
                return;

            events.Update(diff);
            EnterEvadeIfOutOfCombatArea(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, 768);
                        me->setFaction(190);
                        break;
                    case EVENT_JADEFLAME_BUFFET:
                        DoCast(SPELL_JADEFLAME_BUFFET);
                        events.RescheduleEvent(EVENT_JADEFLAME_BUFFET, 20000);
                        break;
                    case EVENT_JADEFIRE_BREATH:
                        if (auto target = me->getVictim())
                            DoCast(target, SPELL_JADEFIRE_BREATH);
                        events.RescheduleEvent(EVENT_JADEFIRE_BREATH, 18000);
                        break;
                    case EVENT_JADEFIRE_BOLT:
                        DoCast(SPELL_JADEFIRE_BOLT);
                        events.RescheduleEvent(EVENT_JADEFIRE_BOLT, 18000);
                        break;
                    case EVENT_FIRE_WALL:
                    {
                        Talk(urand(SAY_YULON_WALL_1, SAY_YULON_WALL_2));
                        bool wall = true;
                        for(uint8 i = 0; i < 7; i++)
                        {
                            if (wall && i != 7 && roll_chance_i(15))
                                wall = false;
                            else
                                me->SummonCreature(72020, WallPos[i]);
                        }
                        events.RescheduleEvent(EVENT_FIRE_WALL, 60000);
                        break;
                    }
                    case EVENT_2:
                        DoCast(SPELL_CELESTIAL_SPAWN);
                        me->NearTeleportTo(summonPos[2].GetPositionX(), summonPos[2].GetPositionY(), summonPos[2].GetPositionZ(), summonPos[2].GetOrientation());
                        me->SetHomePosition(summonPos[2]);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_yulonAI (creature);
    }
};

class spell_jadefire_bolt : public SpellScriptLoader
{
public:
    spell_jadefire_bolt() : SpellScriptLoader("spell_jadefire_bolt") { }

    class spell_jadefire_bolt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_jadefire_bolt_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Trinity::Containers::RandomResizeList(targets, 4);
        }

        void HandleOnHit()
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, GetSpellInfo()->Effects[EFFECT_0]->BasePoints);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_jadefire_bolt_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnHit += SpellHitFn(spell_jadefire_bolt_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_jadefire_bolt_SpellScript();
    }
};

void AddSC_world_boss_yulon()
{
    new boss_yulon();
    new spell_jadefire_bolt();
}
