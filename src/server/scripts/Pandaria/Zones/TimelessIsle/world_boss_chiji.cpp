////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Timeless Isle: Chi-Ji, the Red Crane, Celestial world boss
*/

#include "ScriptMgr.h"

enum Says
{
    SAY_ENTER_POS = 0,
    SAY_AGGRO = 1,
    SAY_CHIJI_BEACON = 2,
    SAY_CHIJI_RUSH_1 = 4,
    SAY_CHIJI_BLAZ_SONG = 3,
    SAY_CHIJI_RUSH_2 = 5,
    SAY_CHIJI_END = 6,
};

enum Npc
{
    NPC_FIRESTORM = 71971,
    NPC_BEACON_OF_HOPE = 71978,
    NPC_CHILD_OF_CHIJI = 71990,
};

enum Spells
{
    SPELL_CELESTIAL_SPAWN = 149307,
    SPELL_FIRESTORM = 144461,
    SPELL_FIRESTORM_AURA = 144463,
    SPELL_INSPIRING_SONG = 144468,
    SPELL_BEACON_OF_HOPE = 144473,
    SPELL_BEACON_OF_HOPE_AURA = 144474,
    SPELL_BLAZING_SONG = 144471,
    SPELL_BLAZING_NOVA = 144493,
    SPELL_CRANE_RUSH = 144470,
};

enum Events
{
    EVENT_FIRESTORM = 3,
    EVENT_INSPIRING_SONG = 4,
    EVENT_BEACON = 5,
    EVENT_BLAZING_SONG = 6,
};

enum Actions
{
    ACTION_MOVE_CENTR_POSS = 1,
    ACTION_CHIJI_END = 2,
    ACTION_CHIJI_FAIL = 3,
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

class boss_chiji : public CreatureScript
{
public:
    boss_chiji() : CreatureScript("boss_chiji") {}

    struct boss_chijiAI : public ScriptedAI
    {
        boss_chijiAI(Creature* creature) : ScriptedAI(creature), summons(me) {}

        EventMap events;
        SummonList summons;
        uint32 orient;

        bool EventProgress;
        bool bEvent_1;
        bool bEvent_2;

        void Reset() 
        {
            bEvent_1 = false;
            bEvent_2 = false;
            EventProgress = false;
            me->setFaction(35);
            me->RemoveAllAuras();
            summons.DespawnAll();
        }

        void EnterCombat(Unit* who)
        {
            Talk(SAY_AGGRO);
            events.RescheduleEvent(EVENT_FIRESTORM, 10000);
            events.RescheduleEvent(EVENT_INSPIRING_SONG, 20000);
            events.RescheduleEvent(EVENT_BEACON, 40000);
            events.RescheduleEvent(EVENT_BLAZING_SONG, 44000);
            me->SetReactState(REACT_AGGRESSIVE);
        }

        void EnterEvadeMode()
        {
            me->SetReactState(REACT_PASSIVE);
            events.Reset();
            me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());

            if (EventProgress)
            {
                me->NearTeleportTo(summonPos[0].GetPositionX(), summonPos[0].GetPositionY(), summonPos[0].GetPositionZ(), summonPos[0].GetOrientation());
                me->SetHomePosition(summonPos[0]);
                if (auto Shao = me->GetAnyOwner())
                    Shao->GetAI()->DoAction(ACTION_CHIJI_FAIL);
            }
            else
                events.RescheduleEvent(EVENT_2, 5000);

            ScriptedAI::EnterEvadeMode();
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

            if (me->HealthBelowPct(66) && !bEvent_1)
            {
                bEvent_1 = true;
                Talk(SAY_CHIJI_RUSH_1);
                DoCast(SPELL_CRANE_RUSH);
            }

            if (me->HealthBelowPct(33) && !bEvent_2)
            {
                bEvent_2 = true;
                Talk(SAY_CHIJI_RUSH_2);
                DoCast(SPELL_CRANE_RUSH);
            }

            if (damage >= me->GetHealth())
            {
                damage = 0;

                if (EventProgress)
                {
                    EventProgress = false;
                    QuestCredit();
                    me->setFaction(35);
                    me->RemoveAllAuras();
                    summons.DespawnAll();
                    me->GeneratePersonalLoot(me, (Player*)who);
                    Talk(SAY_CHIJI_END);
                    if (auto Shao = me->GetAnyOwner())
                        Shao->GetAI()->DoAction(ACTION_CHIJI_END);
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

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);

            summon->SetReactState(REACT_PASSIVE);

            if (summon->GetEntry() == NPC_FIRESTORM)
                summon->AI()->DoCast(SPELL_FIRESTORM_AURA);

            if (summon->GetEntry() == NPC_BEACON_OF_HOPE)
                summon->AI()->DoCast(SPELL_BEACON_OF_HOPE_AURA);

            if (summon->GetEntry() == NPC_CHILD_OF_CHIJI)
            {
                summon->AI()->DoCast(SPELL_BLAZING_NOVA);
                orient = summon->GetOrientation();
                summon->SetOrientation(urand(0, 6));
                float x, y, z;
                summon->GetClosePoint(x, y, z, me->GetObjectSize(), 70.0f);
                summon->GetMotionMaster()->MovePoint(1, x, y, z);
                summon->DespawnOrUnsummon(10000);
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim() && me->isInCombat())
                return;

            EnterEvadeIfOutOfCombatArea(diff);
            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING) || me->HasAura(SPELL_CRANE_RUSH))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, 768);
                        me->setFaction(190);
                        break;
                    case EVENT_FIRESTORM:
                        DoCast(SPELL_FIRESTORM);
                        events.RescheduleEvent(EVENT_FIRESTORM, 20000);
                        break;
                    case EVENT_INSPIRING_SONG:
                        DoCast(SPELL_INSPIRING_SONG);
                        events.RescheduleEvent(EVENT_INSPIRING_SONG, 28000);
                        break;
                    case EVENT_BEACON:
                        Talk(SAY_CHIJI_BEACON);
                        if (auto pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true))
                            DoCast(pTarget, SPELL_BEACON_OF_HOPE);
                        events.RescheduleEvent(EVENT_BEACON, 76000);
                        break;
                    case EVENT_BLAZING_SONG:
                        Talk(SAY_CHIJI_BLAZ_SONG);
                        DoCast(SPELL_BLAZING_SONG);
                        events.RescheduleEvent(EVENT_BLAZING_SONG, 76000);
                        break;
                    case EVENT_2:
                        DoCast(SPELL_CELESTIAL_SPAWN);
                        me->NearTeleportTo(summonPos[0].GetPositionX(), summonPos[0].GetPositionY(), summonPos[0].GetPositionZ(), summonPos[0].GetOrientation());
                        me->SetHomePosition(summonPos[0]);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_chijiAI (creature);
    }
};

class spell_chi_barrage : public SpellScriptLoader
{
public:
    spell_chi_barrage() : SpellScriptLoader("spell_chi_barrage") { }

    class spell_chi_barrage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_chi_barrage_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Trinity::Containers::RandomResizeList(targets, 6);
        }

        void HandleOnHit()
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, GetSpellInfo()->Effects[EFFECT_0]->BasePoints);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_chi_barrage_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnHit += SpellHitFn(spell_chi_barrage_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_chi_barrage_SpellScript();
    }
};

void AddSC_world_boss_chiji()
{
    new boss_chiji();
    new spell_chi_barrage();
}
