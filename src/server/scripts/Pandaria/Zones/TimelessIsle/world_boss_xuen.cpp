////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Timeless Isle: Xuen, the White Tiger, Celestial world boss
*/

#include "ScriptMgr.h"

enum Says
{
    SAY_ENTER_POS = 0,
    SAY_AGGRO = 1,
    SAY_XUEN_BARRAGE = 2,
    SAY_XUEN_LIGHTNING = 3,
    SAY_XUEN_PLAYER_DEATH = 4,
    SAY_XUEN_AGGILITY = 5,
    SAY_XUEN_END = 6,
};

enum Spells
{
    SPELL_CELESTIAL_SPAWN = 149307,
    SPELL_CHI_BARRAGE = 144642,
    SPELL_CRACKLING_LIGHTNING = 144635,
    SPELL_LEAP = 144640,
    SPELL_SPECTRAL_SWIPE = 144638,
    SPELL_AGILITY = 144631,
};

enum Events
{
    EVENT_CHI_BARRAGE = 3,
    EVENT_LIGHTNING = 4,
    EVENT_LEAP = 5,
    EVENT_SPECTRAL_SWIPE = 6,
    EVENT_AGILITY = 7,
};

enum Actions
{
    ACTION_MOVE_CENTR_POSS = 1,
    ACTION_XUEN_END = 4,
    ACTION_XUEN_FAIL = 5,
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

class boss_xuen : public CreatureScript
{
public:
    boss_xuen() : CreatureScript("boss_xuen") {}

    struct boss_xuenAI : public ScriptedAI
    {
        boss_xuenAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;
        bool EventProgress;

        void Reset() 
        {
            EventProgress = false;
            me->setFaction(35);
            me->RemoveAllAuras();
        }

        void EnterCombat(Unit* who)
        {
            Talk(SAY_AGGRO);
            events.RescheduleEvent(EVENT_CHI_BARRAGE, 18000);
            events.RescheduleEvent(EVENT_LIGHTNING, 36000);
            events.RescheduleEvent(EVENT_LEAP, 12000);
            events.RescheduleEvent(EVENT_SPECTRAL_SWIPE, 8000);
            events.RescheduleEvent(EVENT_AGILITY, 28000);
            me->SetReactState(REACT_AGGRESSIVE);
        }

        void EnterEvadeMode()
        {
            me->SetReactState(REACT_PASSIVE);
            events.Reset();
            me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());

            if (EventProgress)
            {
                me->NearTeleportTo(summonPos[1].GetPositionX(), summonPos[1].GetPositionY(), summonPos[1].GetPositionZ(), summonPos[1].GetOrientation());
                me->SetHomePosition(summonPos[1]);
                if (auto Shao = me->GetAnyOwner())
                    Shao->GetAI()->DoAction(ACTION_XUEN_FAIL);
            }
            else
                events.RescheduleEvent(EVENT_2, 5000);

            ScriptedAI::EnterEvadeMode();
        }

        void KilledUnit(Unit* victim)
        {
            if (victim && victim->ToPlayer())
                Talk(SAY_XUEN_PLAYER_DEATH);
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
                    Talk(SAY_XUEN_END);
                    if (auto Shao = me->GetAnyOwner())
                        Shao->GetAI()->DoAction(ACTION_XUEN_END);
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
                    case EVENT_CHI_BARRAGE:
                        Talk(SAY_XUEN_BARRAGE);
                        DoCast(SPELL_CHI_BARRAGE);
                        events.RescheduleEvent(EVENT_CHI_BARRAGE, 34000);
                        break;
                    case EVENT_LIGHTNING:
                        Talk(SAY_XUEN_LIGHTNING);
                        DoCast(SPELL_CRACKLING_LIGHTNING);
                        events.RescheduleEvent(EVENT_LIGHTNING, 46000);
                        break;
                    case EVENT_LEAP:
                        if (auto pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 70.0f, true))
                            DoCast(pTarget, SPELL_LEAP);
                        events.RescheduleEvent(EVENT_LEAP, 48000);
                        break;
                    case EVENT_SPECTRAL_SWIPE:
                        if (auto target = me->getVictim())
                            DoCast(target, SPELL_SPECTRAL_SWIPE);
                        events.RescheduleEvent(EVENT_SPECTRAL_SWIPE, 14000);
                        break;
                    case EVENT_AGILITY:
                        Talk(SAY_XUEN_AGGILITY);
                        DoCast(SPELL_AGILITY);
                        events.RescheduleEvent(EVENT_AGILITY, 60000);
                        break;
                    case EVENT_2:
                        DoCast(SPELL_CELESTIAL_SPAWN);
                        me->NearTeleportTo(summonPos[1].GetPositionX(), summonPos[1].GetPositionY(), summonPos[1].GetPositionZ(), summonPos[1].GetOrientation());
                        me->SetHomePosition(summonPos[1]);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_xuenAI (creature);
    }
};

class spell_crackling_lightning : public SpellScriptLoader
{
public:
    spell_crackling_lightning() : SpellScriptLoader("spell_crackling_lightning") { }

    class spell_crackling_lightning_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_crackling_lightning_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Trinity::Containers::RandomResizeList(targets, 1);
        }

        void HandleOnHit()
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, GetSpellInfo()->Effects[EFFECT_0]->BasePoints);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_crackling_lightning_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnHit += SpellHitFn(spell_crackling_lightning_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_crackling_lightning_SpellScript();
    }
};

void AddSC_world_boss_xuen()
{
    new boss_xuen();
    new spell_crackling_lightning();
}
