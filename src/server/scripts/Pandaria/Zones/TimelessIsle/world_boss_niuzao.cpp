////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Timeless Isle: Niuzao, the Black Ox, Celestial world boss
*/

#include "ScriptMgr.h"

enum Says
{
    SAY_ENTER_POS = 0,
    SAY_AGGRO = 1,
    SAY_NIUZAO_CHARGE_1 = 2,
    SAY_NIUZAO_PLAYER_DEATH = 3,
    SAY_NIUZAO_QUAKE = 4,
    SAY_NIUZAO_CHARGE_2 = 5,
    SAY_NIUZAO_END = 6,
};

enum Spells
{
    SPELL_CELESTIAL_SPAWN = 149307,
    SPELL_MASSIVE_QUAKE = 144611,
    SPELL_OXEN_FORTITUDE = 144606,
    SPELL_HEADBUTT = 144610,
    SPELL_CHARGE = 144608,
};

enum Events
{
    EVENT_MASSIVE_QUAKE = 3,
    EVENT_OXEN_FORTITUDE = 4,
    EVENT_HEADBUTT = 5,
    EVENT_MOVE_PATH_1 = 6,
    EVENT_MOVE_PATH_2 = 7,
};

enum Actions
{
    ACTION_MOVE_CENTR_POSS = 1,
    ACTION_NIUZAO_END = 8,
    ACTION_NIUZAO_FAIL = 9,
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

Position const niuzaoWP[8] =
{
    { -573.247f, -5024.76f, -6.27667f, 0.0f },
    { -574.627f, -5074.96f, -6.27667f, 0.0f },
    { -674.698f, -5074.79f, -6.27722f, 0.0f },
    { -741.14f, -5074.97f, -6.27722f, 0.0f },
    { -741.446f, -5017.58f, -6.27723f, 0.0f },
    { -741.128f, -4958.48f, -6.27723f, 0.0f },
    { -652.9f, -4958.67f, -6.27723f, 0.0f },
    { -572.782f, -4960.81f, -6.27723f, 0.0f }
};

class boss_niuzao : public CreatureScript
{
public:
    boss_niuzao() : CreatureScript("boss_niuzao") {}

    struct boss_niuzaoAI : public ScriptedAI
    {
        boss_niuzaoAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;
        bool EventProgress;
        bool bEvent_1;
        bool bEvent_2;
        bool ChargeEvent;

        void Reset() 
        {
            EventProgress = false;
            bEvent_1 = false;
            bEvent_2 = false;
            ChargeEvent = false;
            me->SetReactState(REACT_AGGRESSIVE);
            me->setFaction(35);
            me->RemoveAllAuras();
        }

        void EnterCombat(Unit* who)
        {
            Talk(SAY_AGGRO);
            events.RescheduleEvent(EVENT_MASSIVE_QUAKE, 45000);
            events.RescheduleEvent(EVENT_OXEN_FORTITUDE, 30000);
            events.RescheduleEvent(EVENT_HEADBUTT, 30000);
        }

        void EnterEvadeMode()
        {
            me->SetReactState(REACT_PASSIVE);
            events.Reset();
            me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());

            if (EventProgress)
            {
                me->NearTeleportTo(summonPos[3].GetPositionX(), summonPos[3].GetPositionY(), summonPos[3].GetPositionZ(), summonPos[3].GetOrientation());
                me->SetHomePosition(summonPos[3]);
                if (auto Shao = me->GetAnyOwner())
                    Shao->GetAI()->DoAction(ACTION_NIUZAO_FAIL);
            }
            else
                events.RescheduleEvent(EVENT_2, 5000);

            ScriptedAI::EnterEvadeMode();
        }

        void KilledUnit(Unit* victim)
        {
            if (victim && victim->ToPlayer())
                Talk(SAY_NIUZAO_PLAYER_DEATH);
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
                case ACTION_MOVE_CENTR_POSS:
                    me->SetSpeed(MAX_MOVE_TYPE, 2.0f);
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

            if (me->HealthBelowPct(66) && !bEvent_1 && !ChargeEvent)
            {
                bEvent_1 = true;
                Talk(SAY_NIUZAO_CHARGE_1);
                events.RescheduleEvent(EVENT_MOVE_PATH_1, 100);
            }

            if (me->HealthBelowPct(33) && !bEvent_2 && !ChargeEvent)
            {
                bEvent_2 = true;
                Talk(SAY_NIUZAO_CHARGE_2);
                events.RescheduleEvent(EVENT_MOVE_PATH_1, 100);
            }

            if (damage >= me->GetHealth())
            {
                damage = 0;

                if (EventProgress)
                {
                    EventProgress = false;
                    ChargeEvent = false;
                    QuestCredit();
                    me->setFaction(35);
                    me->RemoveAllAuras();
                    me->GeneratePersonalLoot(me, (Player*)who);
                    Talk(SAY_NIUZAO_END);
                    if (auto Shao = me->GetAnyOwner())
                        Shao->GetAI()->DoAction(ACTION_NIUZAO_END);
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

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == POINT_MOTION_TYPE)
            {
                if (id == 1)
                {
                    Talk(SAY_ENTER_POS);
                    me->SetFacingTo(1.5f);
                    events.RescheduleEvent(EVENT_1, 5000);
                }
                if (id == 2)
                {
                    DoCast(SPELL_CHARGE);
                    events.RescheduleEvent(EVENT_MOVE_PATH_2, 3000);
                }
                if (id == 3)
                    me->AddDelayedCombat(100, [=]() -> void
                    {
                        me->StopAttack(true);
                        me->GetMotionMaster()->MovePoint(4, niuzaoWP[1]);
                    });
                if (id == 4)
                    me->AddDelayedCombat(100, [=]() -> void
                    {
                        me->StopAttack(true);
                        me->GetMotionMaster()->MovePoint(5, niuzaoWP[2]);
                    });
                if (id == 5)
                    me->AddDelayedCombat(100, [=]() -> void
                    {
                        me->StopAttack(true);
                        me->GetMotionMaster()->MovePoint(6, niuzaoWP[3]);
                    });
                if (id == 6)
                    me->AddDelayedCombat(100, [=]() -> void
                    {
                        me->StopAttack(true);
                        me->GetMotionMaster()->MovePoint(7, niuzaoWP[4]);
                    });
                if (id == 7)
                    me->AddDelayedCombat(100, [=]() -> void
                    {
                        me->StopAttack(true);
                        me->GetMotionMaster()->MovePoint(8, niuzaoWP[5]);
                    });
                if (id == 8)
                    me->AddDelayedCombat(100, [=]() -> void
                    {
                        me->StopAttack(true);
                        me->GetMotionMaster()->MovePoint(9, niuzaoWP[6]);
                    });
                if (id == 9)
                    me->AddDelayedCombat(100, [=]() -> void
                    {
                        me->StopAttack(true);
                        me->GetMotionMaster()->MovePoint(10, niuzaoWP[7]);
                    });
                if (id == 10)
                {
                    me->RemoveAura(SPELL_CHARGE);
                    me->SetReactState(REACT_AGGRESSIVE);
                    ChargeEvent = false;
                }
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim() && me->isInCombat())
                return;

            EnterEvadeIfOutOfCombatArea(diff);
            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING) || ChargeEvent)
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, 768);
                        me->setFaction(190);
                        break;
                    case EVENT_MASSIVE_QUAKE:
                        Talk(SAY_NIUZAO_QUAKE);
                        DoCast(SPELL_MASSIVE_QUAKE);
                        events.RescheduleEvent(EVENT_MASSIVE_QUAKE, 48000);
                        break;
                    case EVENT_OXEN_FORTITUDE:
                        DoCast(SPELL_OXEN_FORTITUDE);
                        events.RescheduleEvent(EVENT_OXEN_FORTITUDE, 46000);
                        break;
                    case EVENT_HEADBUTT:
                        if (auto target = me->getVictim())
                        {
                            DoCast(target, SPELL_HEADBUTT);
                            DoModifyThreatPercent(target, -100);
                        }
                        events.RescheduleEvent(EVENT_HEADBUTT, 30000);
                        break;
                    case EVENT_MOVE_PATH_1:
                        me->StopAttack(true);
                        me->GetMotionMaster()->MovePoint(2, CentrPos[0]);
                        break;
                    case EVENT_MOVE_PATH_2:
                        me->GetMotionMaster()->MovePoint(3, niuzaoWP[0]);
                        ChargeEvent = true;
                        break;
                    case EVENT_2:
                        DoCast(SPELL_CELESTIAL_SPAWN);
                        me->NearTeleportTo(summonPos[3].GetPositionX(), summonPos[3].GetPositionY(), summonPos[3].GetPositionZ(), summonPos[3].GetOrientation());
                        me->SetHomePosition(summonPos[3]);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_niuzaoAI (creature);
    }
};

void AddSC_world_boss_niuzao()
{
    new boss_niuzao();
}
