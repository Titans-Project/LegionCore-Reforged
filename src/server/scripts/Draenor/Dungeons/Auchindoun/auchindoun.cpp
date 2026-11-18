////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "ScriptedCreature.h"
#include "auchindoun.hpp"
#include "Object.h"

Position const g_PositionKaatharCrystalPosition = {1911.5f, 3182.5f, 55.6f, 5.66f};

/// 1st Starting Event
class EventTuulaniIntroduction : public BasicEvent
{
public:

    explicit EventTuulaniIntroduction(Unit* unit, int value, InstanceScript* p_Instance) : BasicEvent(), m_InstanceScript(p_Instance), m_Obj(unit), m_Modifier(value), m_Event(0)
    {
    }

    bool Execute(uint64 /*p_CurrTime*/, uint32 /*diff*/) override
    {
        if (!m_Obj)
            return false;

        if (!m_InstanceScript)
            return false;

        if (Creature* l_Tuulina = m_InstanceScript->instance->GetCreature(m_InstanceScript->GetGuidData(DataTuulani)))
        {
            if (Creature* l_Nyami = m_InstanceScript->instance->GetCreature(m_InstanceScript->GetGuidData(DataNyami)))
            {
                if (Creature* l_Guard = m_InstanceScript->instance->GetCreature(m_InstanceScript->GetGuidData(DataGuard)))
                {
                    if (l_Tuulina->IsAIEnabled && l_Guard->IsAIEnabled)
                    {
                        switch (m_Modifier)
                        {
                            case 0:
                            {
                                l_Nyami->SetReactState(REACT_PASSIVE);
                                l_Nyami->AddAura(SpellDarkFire, l_Nyami);
                                l_Nyami->CastSpell(l_Nyami, SpellPrisonAura);
                                l_Nyami->AddAura(SpellLevitateNyami, l_Nyami);
                                l_Nyami->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                                if (Creature* l_Trigger = l_Nyami->FindNearestCreature(CreatureLeftCrystalTrigger, 40.0f, true))
                                {
                                    l_Trigger->AddAura(SpellVoidFormTriggerBuff, l_Trigger);
                                    l_Nyami->CastSpell(l_Trigger, SpellShadowBeam);
                                }

                                l_Tuulina->AI()->Talk(TUULANITALK2);
                                l_Tuulina->GetMotionMaster()->MovePoint(MovementInformTuulani02, g_PositionTuulaniMovements[1]);
                                break;
                            }
                            case 1:
                            {
                                l_Tuulina->AI()->Talk(TUULANITALK3);
                                l_Tuulina->GetMotionMaster()->MovePoint(MovementInformTuulani03, g_PositionTuulaniMovements[2]);
                                break;
                            }
                            case 3:
                            {
                                l_Guard->SetFacingToObject(l_Tuulina);
                                l_Guard->RemoveAura(SpellKneel);
                                l_Guard->AI()->Talk(AUCHENAIDEFENDERTALK1);
                                l_Guard->m_Events.AddEvent(new EventTuulaniIntroduction(l_Guard, 4, m_InstanceScript), l_Guard->m_Events.CalculateTime(7 * IN_MILLISECONDS));
                                break;
                            }
                            case 4:
                            {
                                l_Tuulina->GetMotionMaster()->MovePoint(MovementInformTuulani04, g_PositionTuulaniMovements[4]);
                                break;
                            }
                            case 5:
                            {
                                l_Tuulina->AI()->Talk(TUULANITALK4);
                                l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 6, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(4 * IN_MILLISECONDS));
                                break;
                            }
                            case 6:
                            {
                                l_Tuulina->CastSpell(l_Tuulina, SpellTuulaniUnlock);
                                l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 7, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(7 * IN_MILLISECONDS));
                                break;
                            }
                            case 7:
                            {
                                /// the gate she stood before opens when her spell is over
                                if (GameObject* l_Gate = l_Tuulina->FindNearestGameObject(GameobjectHolyGate, 30.0f))
                                    l_Gate->Delete();

                                l_Tuulina->AI()->Talk(TUULANITALK18);
                                l_Tuulina->InterruptNonMeleeSpells(false);       /// her unlock beam has no end of its own: the wardens holding the barriers use it for good
                                l_Tuulina->GetMotionMaster()->MovePoint(MovementInformTuulani05, g_PositionTuulaniMovements[5]);
                                break;
                            }
                            case 8:
                            {
                                l_Tuulina->AI()->Talk(TUULANITALK5);
                                l_Tuulina->GetMotionMaster()->MovePoint(MovementInformTuulani06, g_PositionTuulaniMovements[6]);
                                break;
                            }
                            case 9:
                            {
                                l_Tuulina->GetMotionMaster()->MovePoint(MovementInformTuulani07, g_PositionTuulaniMovements[7]);
                                break;
                            }
                            case 10:
                            {
                                l_Tuulina->GetMotionMaster()->MovePoint(MovementInformTuulani08, g_PositionTuulaniMovements[8]);
                                break;
                            }
                            case 11:
                            {
                                /*
                                std::list<Creature*> l_ListCreatures;
                                uint32 l_Entries[7] = { CreatureSoulBinderTuulani ,CreatureSoulBinderNyami, BossKaathar,CreatureSargeriZealot, CreatureSargeriSoulBinder, CreatureSargeriRitualist, CreatureSargeiHoplite };

                                for (uint8 l_I = 0; l_I < 7; l_I++)
                                {
                                    l_Tuulina->GetCreatureListWithEntryInGrid(l_ListCreatures, l_Entries[l_I], 200.0f);
                                }

                                if (!l_ListCreatures.empty())
                                {
                                    for (Creature* itr : l_ListCreatures)
                                    {
                                        if (!itr)
                                            continue;

                                        itr->SetPhaseMask(4, true);
                                    }
                                }
                                */

                                std::list<Player*> playerList;
                                l_Tuulina->GetPlayerListInGrid(playerList, 600.0f/*, true*/);
                                if (!playerList.empty())
                                {
                                    for (Player* itr : playerList)
                                    {
                                        if (!itr)
                                            continue;

                                        itr->NearTeleportTo(*l_Tuulina);
                                        /// Tuulani running to the boss room (scene 913, flags 17 from spell_scene)
                                        Position l_ScenePosition = l_Tuulina->GetPosition();
                                        itr->SendSpellScene(SpellAuchindounSceneTulaaniReachNyami, nullptr, true, &l_ScenePosition);
                                    }
                                }

                                l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 15, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(12 * IN_MILLISECONDS));
                                l_Tuulina->GetMotionMaster()->MovePoint(MovementInformTuulani09, g_PositionTuulaniMovements[9]);

                                l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 12, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(5 * IN_MILLISECONDS));

                                break;
                            }
                            case 12:
                            {
                                //l_Tuulina->AI()->Talk(TUULANITALK6);      /// the scene already plays the voices
                                l_Tuulina->GetMotionMaster()->MovePoint(MovementInformTuulani10, g_PositionTuulaniMovements[10]);
                                break;
                            }
                            case 13:
                            {
                                l_Tuulina->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
                                l_Tuulina->AddAura(SpellTuulaniCapturedVoidPrison, l_Tuulina);
                                l_Tuulina->AI()->DoAction(ActionTuulaniImprisoned);
                                l_Tuulina->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_REMOVE_CLIENT_CONTROL);
                                l_Tuulina->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                                //l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 15, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(7 * IN_MILLISECONDS));
                                break;
                            }
                            case 15:
                            {
                                /*
                                std::list<Creature*> l_ListCreatures;
                                uint32 l_Entries[7] = { CreatureSoulBinderTuulani, CreatureSoulBinderNyami, BossKaathar, CreatureSargeriZealot, CreatureSargeriSoulBinder, CreatureSargeriRitualist, CreatureSargeiHoplite };

                                for (uint8 l_I = 0; l_I < 7; l_I++)
                                {
                                    l_Tuulina->GetCreatureListWithEntryInGrid(l_ListCreatures, l_Entries[l_I], 200.0f);
                                }

                                if (!l_ListCreatures.empty())
                                {
                                    for (Creature* itr : l_ListCreatures)
                                    {
                                        if (!itr)
                                            continue;

                                        itr->SetPhaseMask(1, true);
                                    }
                                }
                                */

                                //l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 16, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(2 * IN_MILLISECONDS));      /// Nyami's lines are in the scene
                                break;
                            }
                            case 16:
                            {
                                l_Nyami->AI()->Talk(NYAMITALK1);
                                l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 17, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(6 * IN_MILLISECONDS));
                                break;
                            }
                            case 17:
                            {
                                l_Nyami->AI()->Talk(NYAMITALK2);
                                l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 18, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(9 * IN_MILLISECONDS));
                                break;
                            }
                            case 18:
                            {
                                l_Nyami->AI()->Talk(NYAMITALK3);
                                l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 19, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(9 * IN_MILLISECONDS));
                                break;
                            }
                            case 19:
                            {
                                l_Nyami->AI()->Talk(NYAMITALK4);
                                l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 20, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(9 * IN_MILLISECONDS));
                                break;
                            }
                            case 20:
                            {
                                l_Tuulina->AI()->Talk(TUULANITALK8);
                                l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 21, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(9 * IN_MILLISECONDS));
                                break;
                            }
                            case 21:
                            {
                                l_Nyami->AI()->Talk(NYAMITALK5);
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
            }
        }

        return true;
    }

private:
    InstanceScript* m_InstanceScript;
    Unit* m_Obj;
    int m_Modifier;
    int m_Event;
};

/// Tuulani - 79248
class auchindoun_mob_tuulani : public CreatureScript
{
public:

    auchindoun_mob_tuulani() : CreatureScript("auchindoun_mob_tuulani") {}

    struct auchindoun_mob_tuulaniAI : ScriptedAI
    {
        auchindoun_mob_tuulaniAI(Creature* creature) : ScriptedAI(creature), m_FirstDiff(0)
        {
            m_Instance = creature->GetInstanceScript();
            m_First = true;
            m_Started = false;
            m_PendingStep = -1;
            m_Cried = false;
            m_SceneOver = false;
            m_PendingFirstLine = false;
        }

        InstanceScript* m_Instance;
        bool m_First;
        bool m_Started;
        bool m_Cried;
        bool m_SceneOver;
        bool m_PendingFirstLine;
        int32 m_PendingStep;
        uint32 m_FirstDiff;
        EventMap events;

        void Reset() override
        {
            events.Reset();
            m_First = false;
            m_Started = false;
            m_PendingStep = -1;
            me->setFaction(FriendlyFaction);
            me->SetSpeed(MOVE_RUN, 1.2f, true);
            me->SetSpeed(MOVE_WALK, 1.2f, true);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
        }

        /// her cry once the scene is over (watched to the end or skipped) and she is caught in the void: once, whatever the number of players
        void DoAction(int32 const p_Action) override
        {
            if (p_Action == ActionTuulaniSkipIntro)
            {
                m_Started = true;
                m_PendingStep = -1;
                m_PendingFirstLine = false;
                return;
            }

            if (p_Action == ActionTuulaniSceneOver)
                m_SceneOver = true;
            else if (p_Action != ActionTuulaniImprisoned)
                return;

            if (m_Cried || !m_SceneOver || !me->HasAura(SpellTuulaniCapturedVoidPrison))
                return;

            m_Cried = true;
            Talk(TUULANITALK10);
        }

        /// a player who walked about ten yards in from the portal of the entrance
        bool IsPlayerPastEntrance() const
        {
            Map::PlayerList const& l_Players = me->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator l_Itr = l_Players.begin(); l_Itr != l_Players.end(); ++l_Itr)
            {
                Player* l_Player = l_Itr->getSource();
                if (l_Player && l_Player->IsInWorld() && !l_Player->IsBeingTeleported() && l_Player->GetExactDist2d(1487.36f, 2953.42f) >= 10.0f)
                    return true;
            }

            return false;
        }

        /// once a player has passed the entrance, her whole walk goes on by itself: no step waits for the players any more
        void UpdateAI(uint32 /*diff*/) override
        {
            if (m_PendingStep >= 0)
            {
                me->m_Events.AddEvent(new EventTuulaniIntroduction(me, uint32(m_PendingStep), m_Instance), me->m_Events.CalculateTime(1 * IN_MILLISECONDS));
                m_PendingStep = -1;
            }

            if (m_PendingFirstLine)
            {
                m_PendingFirstLine = false;
                Talk(TUULANITALK1);
                me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 0, m_Instance), me->m_Events.CalculateTime(5 * IN_MILLISECONDS));
            }

            if (m_Started)
                return;

            if (m_Instance && (m_Instance->GetData(DataPreKaatharCleared) || m_Instance->GetBossState(DataBossKathaar) == DONE))
            {
                m_Started = true;       /// the instance put her back where she belongs
                return;
            }

            if (!IsPlayerPastEntrance())
                return;

            m_Started = true;
            me->GetMotionMaster()->MovePoint(MovementInformTuulani01, g_PositionTuulaniMovements[0]);
        }

        void MovementInform(uint32 /*moveType*/, uint32 id) override
        {
            if (!m_Instance)
                return;

                switch (id)
                {
                    case MovementInformTuulani01:
                        m_PendingFirstLine = true;
                        break;
                    case MovementInformTuulani02:
                        m_PendingStep = 1;
                        break;
                    case MovementInformTuulani03:
                        if (m_Instance)
                        {
                            if (Creature* l_Guard = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataGuard)))
                                l_Guard->m_Events.AddEvent(new EventTuulaniIntroduction(l_Guard, 3, m_Instance), l_Guard->m_Events.CalculateTime(1 * IN_MILLISECONDS));
                            break;
                        }
                    case MovementInformTuulani04:
                        m_PendingStep = 5;
                        break;
                    case MovementInformTuulani05:
                        m_PendingStep = 8;
                        break;
                    case MovementInformTuulani06:
                        m_PendingStep = 9;
                        break;
                    case MovementInformTuulani07:
                        m_PendingStep = 10;
                        break;
                    case MovementInformTuulani08:
                        me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 11, m_Instance), me->m_Events.CalculateTime(1 * IN_MILLISECONDS));       /// the scene brings the players to her
                        break;
                    case MovementInformTuulani09:
                        me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 12, m_Instance), me->m_Events.CalculateTime(1 * IN_MILLISECONDS));
                        break;
                    case MovementInformTuulani10:
                        me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 13, m_Instance), me->m_Events.CalculateTime(1 * IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }
            }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_tuulaniAI(creature);
    }
};

/// Sargerei Soulbinder - 77812
class auchindoun_mob_sargerei_soulbinder : public CreatureScript
{
public:

    auchindoun_mob_sargerei_soulbinder() : CreatureScript("auchindoun_mob_sargerei_soulbinder") { }

    struct auchindoun_mob_sargerei_soulbinderAI : public ScriptedAI
    {
        auchindoun_mob_sargerei_soulbinderAI(Creature* creature) : ScriptedAI(creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        EventMap events;

        enum eSargereiSoulbinderEvents
        {
            EventMindShear = 1,
            EventBendWill
        };

        enum eSargereiSoulbinderSpells
        {
            SpellMindSear = 157170,
            SpellBendWill = 154527
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* atacker) override
        {
            events.RescheduleEvent(EventMindShear, 8 * IN_MILLISECONDS);
            events.RescheduleEvent(EventBendWill, 18 * IN_MILLISECONDS);
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (m_Instance)
            {
                if (Creature* l_Kaathar = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataBossKathaar)))
                {
                    if (l_Kaathar->IsAlive() && l_Kaathar->IsAIEnabled)
                        l_Kaathar->AI()->DoAction(ActionCountPre1StBossKill);
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case EventMindShear:
                    if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Target, SpellMindSear);
                    events.RescheduleEvent(EventMindShear, 15 * IN_MILLISECONDS);
                    break;
                case EventBendWill:
                    if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Target, SpellBendWill);
                    events.RescheduleEvent(EventBendWill, 18 * IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_sargerei_soulbinderAI(creature);
    }
};

/// Sargerei Cleric - 77134
class auchindoun_mob_sargerei_cleric : public CreatureScript
{
public:

    auchindoun_mob_sargerei_cleric() : CreatureScript("auchindoun_mob_sargerei_cleric") { }

    struct auchindoun_mob_sargerei_clericAI : public ScriptedAI
    {
        auchindoun_mob_sargerei_clericAI(Creature* creature) : ScriptedAI(creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        EventMap events;
        enum eSargereiClericEvents
        {
            EventVoidShell = 1
        };

        enum eSargereiCleircSpells
        {
            SpellVoidShell = 160312
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*atacker*/) override
        {
            events.RescheduleEvent(eSargereiClericEvents::EventVoidShell, 15 * IN_MILLISECONDS);
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (m_Instance)
            {
                if (Creature* l_Kaathar = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataBossKathaar)))
                {
                    if (l_Kaathar->IsAlive() && l_Kaathar->IsAIEnabled)
                        l_Kaathar->AI()->DoAction(ActionCountPre1StBossKill);
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eSargereiClericEvents::EventVoidShell:
                    me->CastSpell(me, eSargereiCleircSpells::SpellVoidShell);
                    events.RescheduleEvent(eSargereiClericEvents::EventVoidShell, urand(12 * IN_MILLISECONDS, 16 * IN_MILLISECONDS));
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_sargerei_clericAI(creature);
    }
};

/// Sargerei Ritualist - 77130
class auchindoun_mob_sargerei_ritualist : public CreatureScript
{
public:

    auchindoun_mob_sargerei_ritualist() : CreatureScript("auchindoun_mob_sargerei_ritualist") { }

    struct auchindoun_mob_sargerei_ritualistAI : public ScriptedAI
    {
        auchindoun_mob_sargerei_ritualistAI(Creature* creature) : ScriptedAI(creature)
        {
            m_Instance = me->GetInstanceScript();
        }
        EventMap events;

        enum eSargereiRitualistEvents
        {
            EventMindSpike = 1
        };

        enum eSargereiRitualistSpells
        {
            SpellShadowBeam = 156862,
            SpellDarkFire = 156955,
            SpellMindSpike = 157043
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();

            if (Creature* l_Trigger = me->SummonCreature(CreatureShadowBeam, g_PositionKaatharCrystalPosition, TEMPSUMMON_MANUAL_DESPAWN))
                me->CastSpell(l_Trigger, eSargereiRitualistSpells::SpellShadowBeam);

            me->AddAura(eSargereiRitualistSpells::SpellDarkFire, me);
        }

        void EnterCombat(Unit* atacker) override
        {
            me->CastStop();
            me->RemoveAllAuras();
            events.RescheduleEvent(eSargereiRitualistEvents::EventMindSpike, urand(6 * IN_MILLISECONDS, 8 * IN_MILLISECONDS));
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (m_Instance)
            {
                if (Creature* l_Kaathar = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataBossKathaar)))
                {
                    if (l_Kaathar->IsAlive() && l_Kaathar->IsAIEnabled)
                        l_Kaathar->AI()->DoAction(ActionCountPre1StBossKill);
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eSargereiRitualistEvents::EventMindSpike:
                    if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Target, eSargereiRitualistSpells::SpellMindSpike);
                    events.RescheduleEvent(eSargereiRitualistEvents::EventMindSpike, 6 * IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_sargerei_ritualistAI(creature);
    }
};

/// Sargerei Zealot - 77132
class auchindoun_mob_sargerei_zealot : public CreatureScript
{
public:

    auchindoun_mob_sargerei_zealot() : CreatureScript("auchindoun_mob_sargerei_zealot") { }

    struct auchindoun_mob_sargerei_zealotAI : public ScriptedAI
    {
        auchindoun_mob_sargerei_zealotAI(Creature* creature) : ScriptedAI(creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        EventMap events;

        enum eSargereiZealotSpells
        {
            SpellSeverTendonAura = 157165
        };

        enum eSargereiZealotEvents
        {
            EventSeverTendom = 1
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* atacker) override
        {
            events.RescheduleEvent(eAuchindounEvents::EventSeverTendom, 5 * IN_MILLISECONDS);
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (m_Instance)
            {
                if (Creature* Kaathar = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataBossKathaar)))
                {
                    if (Kaathar->IsAlive() && Kaathar->IsAIEnabled)
                        Kaathar->AI()->DoAction(ActionCountPre1StBossKill);
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eAuchindounEvents::EventSeverTendom:
                    if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Target, SpellSeverTendonAura);
                    events.RescheduleEvent(eAuchindounEvents::EventSeverTendom, urand(9 * IN_MILLISECONDS, 14 * IN_MILLISECONDS));
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_sargerei_zealotAI(creature);
    }
};

/// Sargerei Spirit Tender - 77131
class auchindoun_mob_sargerei_spirit_tender : public CreatureScript
{
public:

    auchindoun_mob_sargerei_spirit_tender() : CreatureScript("auchindoun_mob_sargerei_spirit_tender") { }

    struct auchindoun_mob_sargerei_spirit_tenderAI : public ScriptedAI
    {
        auchindoun_mob_sargerei_spirit_tenderAI(Creature* creature) : ScriptedAI(creature)
        {
            m_Instance = me->GetInstanceScript();
        }


        EventMap events;

        enum eSpiritTenderSpells
        {
            SpellVoidMendingDummy = 154623,
            SpellVoidShiftDummy = 155524
        };

        enum eSpiritTenderEvents
        {
            EventVoidMending = 1,
            EventVoidShift
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* atacker) override
        {
            events.RescheduleEvent(eSpiritTenderEvents::EventVoidMending, 10 * IN_MILLISECONDS);
            events.RescheduleEvent(eSpiritTenderEvents::EventVoidShift, 16 * IN_MILLISECONDS);
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (m_Instance)
            {
                if (Creature* l_Kaathar = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataBossKathaar)))
                {
                    if (l_Kaathar->IsAlive() && l_Kaathar->IsAIEnabled)
                        l_Kaathar->AI()->DoAction(ActionCountPre1StBossKill);
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eSpiritTenderEvents::EventVoidMending:
                    if (Unit* l_FriendUnit = DoSelectLowestHpFriendly(85))
                        me->CastSpell(l_FriendUnit, eSpiritTenderSpells::SpellVoidMendingDummy);
                    events.RescheduleEvent(eSpiritTenderEvents::EventVoidMending, 10 * IN_MILLISECONDS);
                    break;
                case eSpiritTenderEvents::EventVoidShift:
                    me->CastSpell(me, eSpiritTenderSpells::SpellVoidShiftDummy);
                    events.RescheduleEvent(eSpiritTenderEvents::EventVoidShift, 16 * IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_sargerei_spirit_tenderAI(creature);
    };
};

/// Sargerei Hopilite - 77133
class auchindoun_mob_sargerei_hopilite : public CreatureScript
{
public:

    auchindoun_mob_sargerei_hopilite() : CreatureScript("auchindoun_mob_sargerei_hopilite") { }

    struct auchindoun_mob_sargerei_hopiliteAI : public ScriptedAI
    {
        auchindoun_mob_sargerei_hopiliteAI(Creature* creature) : ScriptedAI(creature)
        {
            m_Instance = me->GetInstanceScript();
        }


        EventMap events;

        enum eSargereiHopiliteSpells
        {
            SpellShieldBash = 157159,
            SpellVoidStrikes = 166749
        };

        enum eSargereiHopiliteEvents
        {
            EventShieldBash = 1,
            EventVoidStrikes
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* atacker) override
        {
            events.RescheduleEvent(eSargereiHopiliteEvents::EventShieldBash, urand(8 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
            events.RescheduleEvent(eSargereiHopiliteEvents::EventVoidStrikes, 18 * IN_MILLISECONDS);
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (m_Instance)
            {
                if (Creature* l_Kaathar = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataBossKathaar)))
                {
                    if (l_Kaathar->IsAlive() && l_Kaathar->IsAIEnabled)
                        l_Kaathar->AI()->DoAction(ActionCountPre1StBossKill);
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eSargereiHopiliteEvents::EventShieldBash:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eSargereiHopiliteSpells::SpellShieldBash);
                    events.RescheduleEvent(eSargereiHopiliteEvents::EventShieldBash, urand(8 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
                    break;
                case eSargereiHopiliteEvents::EventVoidStrikes:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eSargereiHopiliteSpells::SpellVoidStrikes);
                    events.RescheduleEvent(eSargereiHopiliteEvents::EventVoidStrikes, 18 * IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_sargerei_hopiliteAI(creature);
    }
};

/// Sargerei Defender - 77042
class auchindoun_mob_sargerei_defender : public CreatureScript
{
public:

    auchindoun_mob_sargerei_defender() : CreatureScript("auchindoun_mob_sargerei_defender") { }

    struct auchindoun_mob_sargerei_defenderAI : public ScriptedAI
    {
        auchindoun_mob_sargerei_defenderAI(Creature* creature) : ScriptedAI(creature)
        {
            m_Instance = me->GetInstanceScript();
            m_False = true;
        }

        EventMap events;

        enum eSargereiDefenderSpells
        {
            SpellAvengersShield = 165715,
            SpellCrusaderStirke = 176931
        };

        enum eSargereiDefenderEvents
        {
            EventAvengersShield = 1,
            EventCrusaderStirke
        };

        InstanceScript* m_Instance;
        bool m_False;

        void Reset() override
        {
            events.Reset();

            if (m_False)
            {
                m_False = false;
            }
        }

        void EnterCombat(Unit* atacker) override
        {
            events.RescheduleEvent(eSargereiDefenderEvents::EventAvengersShield, urand(10 * IN_MILLISECONDS, 16 * IN_MILLISECONDS));
            events.RescheduleEvent(eSargereiDefenderEvents::EventCrusaderStirke, urand(5 * IN_MILLISECONDS, 9 * IN_MILLISECONDS));
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eSargereiDefenderEvents::EventAvengersShield:
                    if (Unit* l_Random = SelectTarget(SELECT_TARGET_FARTHEST, 0, 50.0f, true))
                        me->CastSpell(l_Random, eSargereiDefenderSpells::SpellAvengersShield);
                    events.RescheduleEvent(eSargereiDefenderEvents::EventAvengersShield, urand(10 * IN_MILLISECONDS, 16 * IN_MILLISECONDS));
                    break;
                case eSargereiDefenderEvents::EventCrusaderStirke:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eSargereiDefenderSpells::SpellCrusaderStirke);
                    events.RescheduleEvent(eSargereiDefenderEvents::EventCrusaderStirke, urand(5 * IN_MILLISECONDS, 9 * IN_MILLISECONDS));
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_sargerei_defenderAI(creature);
    }
};

/// Sargerei Magus - 76263
class auchindoun_mob_sargerei_magus : public CreatureScript
{
public:

    auchindoun_mob_sargerei_magus() : CreatureScript("auchindoun_mob_sargerei_magus") { }

    struct auchindoun_mob_sargerei_magusAI : public ScriptedAI
    {
        auchindoun_mob_sargerei_magusAI(Creature* creature) : ScriptedAI(creature)
        {
            m_False = true;
            m_Instance = me->GetInstanceScript();
        }


        EventMap events;

        enum eSargereiMagusSpells
        {
            SpellArcaneChanneling = 161383,
            SpellVoidChannelingHands = 160677,
            SpellArcaneBombDummy = 157652,
            SpellArcaneBoltPeriod = 157505,
            SpellArcaneBoltProje = 157931
        };

        enum eSargereiMagusEvents
        {
            EventArcaneBomb = 1,
            EventArcaneBolt
        };

        InstanceScript* m_Instance;
        bool m_False;
        std::list<ObjectGuid> l_Prisoners;

        void Reset() override
        {
            if (m_False)
            {
                m_False = false;
            }

            events.Reset();
            me->CastSpell(me, eSargereiMagusSpells::SpellArcaneChanneling);
            me->AddAura(eSargereiMagusSpells::SpellVoidChannelingHands, me);       /// the void in his hands: the arcane he channels is corrupted
        }

        void MovementInform(uint32 /*moveType*/, uint32 id) override
        {
            switch (id)
            {
                case MovementInformFallMagusPrisoners:
                    if (!l_Prisoners.empty())
                    {
                        for (auto itr : l_Prisoners)
                        {
                            if (!itr)
                                continue;

                            if (Creature* l_Mob = Creature::GetCreature(*me, itr))
                            {
                                l_Mob->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
                                l_Mob->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        void EnterCombat(Unit* atacker) override
        {
            me->RemoveAura(eSargereiMagusSpells::SpellArcaneChanneling);
            me->RemoveAura(eSargereiMagusSpells::SpellVoidChannelingHands);
            /// the arcane bombs belong to the Auchenai magi, out of combat: the Sargerei only cast their bolts
            events.RescheduleEvent(eSargereiMagusEvents::EventArcaneBolt, 6 * IN_MILLISECONDS);

            std::list<Creature*> l_mobsPrisoners;
            me->GetCreatureListWithEntryInGrid(l_mobsPrisoners, CreatureAucheniSoulPriest, 20.0f);
            if (!l_mobsPrisoners.empty())
            {
                for (Creature* itr : l_mobsPrisoners)
                {
                    itr->Kill(itr);
                    itr->RemoveAllAuras();
                    itr->GetMotionMaster()->MoveFall(MovementInformFallMagusPrisoners);
                    itr->DespawnOrUnsummon(1 * IN_MILLISECONDS);
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eSargereiMagusEvents::EventArcaneBomb:
                    me->CastSpell(me, eSargereiMagusSpells::SpellArcaneBombDummy);
                    events.RescheduleEvent(eSargereiMagusEvents::EventArcaneBomb, 13 * IN_MILLISECONDS);
                    break;
                case eSargereiMagusEvents::EventArcaneBolt:
                    me->CastSpell(me, eSargereiMagusSpells::SpellArcaneBoltPeriod);
                    events.RescheduleEvent(eSargereiMagusEvents::EventArcaneBolt, 14 * IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_sargerei_magusAI(creature);
    }
};

/// Sargerei Soul Priest - 76595
class auchindoun_mob_soul_priest : public CreatureScript
{
public:

    auchindoun_mob_soul_priest() : CreatureScript("auchindoun_mob_soul_priest") { }

    struct auchindoun_mob_soul_priestAI : public ScriptedAI
    {
        auchindoun_mob_soul_priestAI(Creature* creature) : ScriptedAI(creature)
        {
            m_Instance = me->GetInstanceScript();
            m_False = true;
        }


        EventMap events;

        enum eSoulPriestSpells
        {
            SpellShadowWordPainPriest = 176518,
            SpellPsychicTerrorDummy = 154356
        };

        enum eSoulPriestEvents
        {
            EventShadowWordPainSoulPriest = 1,
            EventPsychicTerrors
        };

        InstanceScript* m_Instance;
        bool m_False;

        void Reset() override
        {
            events.Reset();

            if (m_False)
            {
                m_False = false;
            }
        }

        void EnterCombat(Unit* atacker) override
        {
            events.RescheduleEvent(eSoulPriestEvents::EventPsychicTerrors, 15 * IN_MILLISECONDS);
            events.RescheduleEvent(eSoulPriestEvents::EventShadowWordPainSoulPriest, urand(8 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eSoulPriestEvents::EventShadowWordPainSoulPriest:
                    if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true, -SpellShadowWordPainPriest))
                        me->CastSpell(l_Target, eSoulPriestSpells::SpellShadowWordPainPriest);
                    events.RescheduleEvent(eSoulPriestEvents::EventShadowWordPainSoulPriest, urand(8 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
                    break;
                case eSoulPriestEvents::EventPsychicTerrors:
                    if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        me->CastSpell(l_Target, eSoulPriestSpells::SpellPsychicTerrorDummy);
                    events.RescheduleEvent(eSoulPriestEvents::EventPsychicTerrors, 15 * IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_soul_priestAI(creature);
    }
};

/// Sargerei Warden - 77935
class auchindoun_mob_sargeri_warden : public CreatureScript
{
public:

    auchindoun_mob_sargeri_warden() : CreatureScript("auchindoun_mob_sargeri_warden") { }

    struct auchindoun_mob_sargeri_wardenAI : public ScriptedAI
    {
        auchindoun_mob_sargeri_wardenAI(Creature* creature) : ScriptedAI(creature)
        {
            m_Instance = me->GetInstanceScript();
            m_False = true;
        }

        EventMap events;

        enum eWardenSpells
        {
            SpellWardenThrowHammer = 154730,
            SpellWardenChainDot = 154831
        };

        enum eWardenEvents
        {
            EventWardenHammer = 1,
            EventWardenChain
        };

        InstanceScript* m_Instance;
        bool m_False;

        void Reset() override
        {
            events.Reset();

            if (m_False)
            {
                m_False = false;
            }
        }

        void EnterCombat(Unit* atacker) override
        {
            events.RescheduleEvent(eWardenEvents::EventWardenChain, 5 * IN_MILLISECONDS);
            events.RescheduleEvent(eWardenEvents::EventWardenHammer, urand(12 * IN_MILLISECONDS, 16 * IN_MILLISECONDS));
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eWardenEvents::EventWardenHammer:
                    if (Unit* l_Random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Random, eWardenSpells::SpellWardenThrowHammer);
                    events.RescheduleEvent(eWardenEvents::EventWardenHammer, urand(12 * IN_MILLISECONDS, 16 * IN_MILLISECONDS));
                    break;
                case eWardenEvents::EventWardenChain:
                    if (Unit* l_Random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Random, eWardenSpells::SpellWardenChainDot);
                    events.RescheduleEvent(eWardenEvents::EventWardenChain, 20 * IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_sargeri_wardenAI(creature);
    }
};

/// Felborne Abyssal - 77905
class auchindoun_mob_felborne_abyssal : public CreatureScript
{
public:

    auchindoun_mob_felborne_abyssal() : CreatureScript("auchindoun_mob_felborne_abyssal") { }

    struct auchindoun_mob_felborne_abyssalAI : public ScriptedAI
    {
        auchindoun_mob_felborne_abyssalAI(Creature* creature) : ScriptedAI(creature), m_Fixated(false)
        {
            m_Instance = me->GetInstanceScript();
        }

        EventMap events;

        enum eFelborneAbyssalSpells
        {
            SpellFixate = 157168
        };

        enum eFelborneAbyssalEvents
        {
            EventFixate = 1
        };

        InstanceScript* m_Instance;
        bool m_Fixated;
        ObjectGuid m_FixatedTargetGUID;

        void Reset() override
        {
            events.Reset();
            m_Fixated = false;
            m_FixatedTargetGUID = ObjectGuid::Empty;
            me->SetReactState(REACT_AGGRESSIVE);
        }

        void EnterCombat(Unit* atacker) override
        {
            events.RescheduleEvent(eFelborneAbyssalEvents::EventFixate, urand(16 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
        }

        void DoAction(int32 const actionID) override
        {
            switch (actionID)
            {
                case ActionDeactivateFixation:
                    m_Fixated = false;
                    break;
                default:
                    break;
            }
        }

        //void OnAddThreat(Unit* victim, float& p_fThreat, SpellSchoolMask /*p_SchoolMask*/, SpellInfo const /*p_ThreatSpell*/)
        //{
        //    if (m_Fixated)
        //        p_fThreat = 0;
        //    return;
        //}

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            if (m_Fixated)
            {
                if (m_FixatedTargetGUID)
                {
                    if (Unit* l_Target = Unit::GetUnit(*me, m_FixatedTargetGUID))
                        me->AddThreat(l_Target, 500.0f);
                }
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eAuchindounEvents::EventFixate:
                    if (Unit* l_Random = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    {
                        m_Fixated = true;
                        m_FixatedTargetGUID = l_Random->GetGUID();
                        me->CastSpell(l_Random, eFelborneAbyssalSpells::SpellFixate);
                    }

                    events.RescheduleEvent(eFelborneAbyssalEvents::EventFixate, urand(16 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_felborne_abyssalAI(creature);
    }
};

/// Cackling Pyrmoaniac - 76260
class auchindoun_mob_cackling_pyromaniac : public CreatureScript
{
public:

    auchindoun_mob_cackling_pyromaniac() : CreatureScript("auchindoun_mob_cackling_pyromaniac") { }

    struct auchindoun_mob_cackling_pyromaniacAI : public ScriptedAI
    {
        auchindoun_mob_cackling_pyromaniacAI(Creature* creature) : ScriptedAI(creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        EventMap events;

        enum eCacklingPyromaniacSpells
        {
            SpellFelBlast = 174422,
            SpellAbyssalVisual = 159610
        };

        enum eCacklingPyromaniacEvents
        {
            EventFelBlast = 1
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->CastSpell(me, eCacklingPyromaniacSpells::SpellAbyssalVisual);
        }

        void EnterCombat(Unit* atacker) override
        {
            me->InterruptNonMeleeSpells(false);       /// the summoning circle has no end of its own
            events.RescheduleEvent(eCacklingPyromaniacEvents::EventFelBlast, 6 * IN_MILLISECONDS);
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (m_Instance)
            {
                if (Creature* l_Azzakel = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataBossAzzakael)))
                    if (l_Azzakel->isInCombat() && l_Azzakel->IsAlive() && l_Azzakel->IsAIEnabled)
                        l_Azzakel->GetAI()->DoAction(ActionDemonSoulsAchievement);

                if (Creature* l_Trigger = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataTriggerAzzakelController)))
                    if (l_Trigger->IsWithinDistInMap(me, 30.0f) && l_Trigger->IsAIEnabled)
                        l_Trigger->AI()->DoAction(ActionCountPre3StBossKill);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eCacklingPyromaniacEvents::EventFelBlast:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eCacklingPyromaniacSpells::SpellFelBlast);
                    events.RescheduleEvent(eCacklingPyromaniacEvents::EventFelBlast, 6 * IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_cackling_pyromaniacAI(creature);
    }
};

/// Blazing Trickster - 79511
class auchindoun_mob_blazing_trickster : public CreatureScript
{
public:

    auchindoun_mob_blazing_trickster() : CreatureScript("auchindoun_mob_blazing_trickster") { }

    struct auchindoun_mob_blazing_tricksterAI : public ScriptedAI
    {
        auchindoun_mob_blazing_tricksterAI(Creature* creature) : ScriptedAI(creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        EventMap events;

        enum eBlazingTricksterSpells
        {
            SpellConfligirate = 154981
        };

        enum eBlazingTricksterEvents
        {
            EventConfligrate = 1
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->SetReactState(REACT_AGGRESSIVE);
            me->SetDefaultMovementType(MovementGeneratorType::RANDOM_MOTION_TYPE);
            events.RescheduleEvent(eBlazingTricksterEvents::EventConfligrate, urand(8 * IN_MILLISECONDS, 15 * IN_MILLISECONDS));
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (m_Instance)
            {
                if (Creature* l_Azzakel = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataBossAzzakael)))
                    if (l_Azzakel->isInCombat() && l_Azzakel->IsAlive() && l_Azzakel->IsAIEnabled)
                        l_Azzakel->GetAI()->DoAction(ActionDemonSoulsAchievement);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eBlazingTricksterEvents::EventConfligrate:
                    me->CastSpell(me, eBlazingTricksterSpells::SpellConfligirate);
                    events.RescheduleEvent(eBlazingTricksterEvents::EventConfligrate, urand(8 * IN_MILLISECONDS, 16 * IN_MILLISECONDS));
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_blazing_tricksterAI(creature);
    }
};

/// Felguard - 76259
class auchindoun_mob_felguard : public CreatureScript
{
public:

    auchindoun_mob_felguard() : CreatureScript("auchindoun_mob_felguard") { }

    struct auchindoun_mob_felguardAI : public ScriptedAI
    {
        auchindoun_mob_felguardAI(Creature* creature) : ScriptedAI(creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        EventMap events;

        enum eFelguardSpells
        {
            SpellFelStomp = 157173
        };

        enum eFelguardEvents
        {
            EventFelStomp = 1
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->SetReactState(REACT_AGGRESSIVE);
        }

        void EnterCombat(Unit* atacker) override
        {
            events.RescheduleEvent(eFelguardEvents::EventFelStomp, 10 * IN_MILLISECONDS);
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (m_Instance)
            {
                if (Creature* l_Azzakel = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataBossAzzakael)))
                    if (l_Azzakel->isInCombat() && l_Azzakel->IsAlive() && l_Azzakel->IsAIEnabled)
                        l_Azzakel->GetAI()->DoAction(ActionDemonSoulsAchievement);

                if (Creature* l_Trigger = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataTriggerAzzakelController)))
                    if (l_Trigger->IsWithinDistInMap(me, 30.0f) && l_Trigger->IsAIEnabled)
                        l_Trigger->AI()->DoAction(ActionCountPre3StBossKill);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eFelguardEvents::EventFelStomp:
                    me->CastSpell(me, eFelguardSpells::SpellFelStomp);
                    events.RescheduleEvent(eFelguardEvents::EventFelStomp, urand(12 * IN_MILLISECONDS, 16 * IN_MILLISECONDS));
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_felguardAI(creature);
    }
};

/// Warden Hammer - 76655
class auchindoun_mob_warden_hammer : public CreatureScript
{
public:

    auchindoun_mob_warden_hammer() : CreatureScript("auchindoun_mob_warden_hammer") { }

    struct auchindoun_mob_warden_hammerAI : public ScriptedAI
    {
        auchindoun_mob_warden_hammerAI(Creature* creature) : ScriptedAI(creature), m_DiffHammer(0)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eWardenHammerSpells
        {
            SpellWardenHammerLightningVisual = 154775,
            SpellWardenHammerDamage = 154773
        };

        InstanceScript* m_Instance;
        uint32 m_DiffHammer;

        void Reset() override
        {
            me->setFaction(HostileFaction);
            m_DiffHammer = 1 * IN_MILLISECONDS;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
        }

        void UpdateAI(uint32 diff) override
        {
            if (m_DiffHammer <= diff)
            {
                std::list<Player*> l_ListNerbyPlayers;
                me->GetPlayerListInGrid(l_ListNerbyPlayers, 3.0f);
                if (!l_ListNerbyPlayers.empty())
                {
                    for (Player* itr : l_ListNerbyPlayers)
                    {
                        if (!itr)
                            continue;

                        if (itr && itr->IsInWorld())
                        {
                            me->CastSpell(itr, eWardenHammerSpells::SpellWardenHammerLightningVisual);
                            me->CastSpell(itr, eWardenHammerSpells::SpellWardenHammerDamage);
                        }
                    }
                }

                m_DiffHammer = 1 * IN_MILLISECONDS;
            }
            else
                m_DiffHammer -= diff;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_warden_hammerAI(creature);
    }
};

/// Void Mending - 154623 
class auchindoun_spell_void_mending : public SpellScriptLoader
{
public:

    auchindoun_spell_void_mending() : SpellScriptLoader("auchindoun_spell_void_mending") { }

    class auchindoun_spell_void_mending_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_spell_void_mending_SpellScript);

        void HandleDummy(SpellEffIndex /*effectIndex*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* l_Target = GetHitUnit())
                    caster->AddAura(SpellVoidMendingAura, l_Target);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(auchindoun_spell_void_mending_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_spell_void_mending_SpellScript();
    }
};

/// Psychic Terrors - 154356  
class auchindoun_spell_psychic_terror : public SpellScriptLoader
{
public:

    auchindoun_spell_psychic_terror() : SpellScriptLoader("auchindoun_spell_psychic_terror") { }

    class auchindoun_spell_psychic_terror_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_spell_psychic_terror_SpellScript);

        void HandleDummy(SpellEffIndex /*effectIndex*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    std::list<Player*> playerList;
                    caster->GetPlayerListInGrid(playerList, 4.0f);
                    if (!playerList.empty())
                    {
                        for (Player* itr : playerList)
                        {
                            itr->AddAura(SpellPsychicTerrorFear, itr);
                        }
                    }
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(auchindoun_spell_psychic_terror_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_spell_psychic_terror_SpellScript();
    }
};

/// Warden's Chain - 154683 
class auchindoun_spell_warden_chain : public SpellScriptLoader
{
public:

    auchindoun_spell_warden_chain() : SpellScriptLoader("auchindoun_spell_warden_chain") { }

    class auchindoun_spell_warden_chain_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_spell_warden_chain_SpellScript);

        enum eWardenChainSpells
        {
            SpellWardenChainJump = 154639,
            SpellWardenChainDot = 154831
        };

        void HandleDummy(SpellEffIndex /*effectIndex*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* l_Target = GetExplTargetUnit())
                {
                    caster->AddAura(eWardenChainSpells::SpellWardenChainJump, l_Target);
                    caster->AddAura(eWardenChainSpells::SpellWardenChainDot, l_Target);
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(auchindoun_spell_warden_chain_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_spell_warden_chain_SpellScript();
    }
};

/// Warden Chain Aura - 154831 
class auchindoun_warden_chain_aura : public SpellScriptLoader
{
public:

    auchindoun_warden_chain_aura() : SpellScriptLoader("auchindoun_warden_chain_aura") { }

    class auchindoun_warden_chain_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(auchindoun_warden_chain_aura_AuraScript);

        enum eWardenChainAuras
        {
            SpellWardenChainRoot = 154263
        };

        void HandlePeriodic(AuraEffect const* auraEffect)
        {
            if (Unit* l_Target = GetTarget())
                l_Target->AddAura(eWardenChainAuras::SpellWardenChainRoot, l_Target);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(auchindoun_warden_chain_aura_AuraScript::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new auchindoun_warden_chain_aura_AuraScript();
    }
};

/// Void Shift - 155524
class auchindoun_spell_void_shift : public SpellScriptLoader
{
public:

    auchindoun_spell_void_shift() : SpellScriptLoader("auchindoun_spell_void_shift") { }

    class auchindoun_spell_void_shift_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_spell_void_shift_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            //if (!GetCaster())
            //    return;

            //auto caster = GetCaster();
            //int32 calcDamage = 8000;

            //std::list<Unit*> playerList;
            //Trinity::AnyUnitInObjectRangeCheck check(caster, 30.0f);
            //Trinity::UnitListSearcher<Trinity::AnyUnitInObjectRangeCheck> searcher(caster, playerList, check);
            //caster->VisitNearbyObject(30.0f, searcher);
            //if (!playerList.empty())
            //{
            //    for (auto itr = playerList.cbegin(); itr != playerList.cend(); ++itr)
            //    {
            //        if (!*itr)
            //            continue;

            //        if ((*itr)->IsPlayer())
            //            caster->CastCustomSpell(*itr, SpellVoidShiftDamage, &calcDamage, nullptr, nullptr, true, nullptr);
            //        else
            //            caster->CastCustomSpell(*itr, SpellVoidShiftHeal, &calcDamage, nullptr, nullptr, true, nullptr);
            //    }
            //}
        }

        void Register() override
        {
            OnEffectLaunch += SpellEffectFn(auchindoun_spell_void_shift_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_spell_void_shift_SpellScript();
    }
};

/// Void Shell - 160312
class auchindoun_spell_void_shell_filter : public SpellScriptLoader
{
public:
    auchindoun_spell_void_shell_filter() : SpellScriptLoader("auchindoun_spell_void_shell_filter") { }

    class auchindoun_spell_void_shell_filter_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_spell_void_shell_filter_SpellScript);

        void CorrectTargets(std::list<WorldObject*>& targets)
        {
            //targets.clear();

            //if (!GetCaster())
            //    return;

            //auto caster = GetCaster();

            //std::list<Unit*> targetList;
            //Trinity::AnyFriendlyUnitInObjectRangeCheck u_check(caster, caster, 10.0f);
            //Trinity::UnitListSearcher<Trinity::AnyFriendlyUnitInObjectRangeCheck> searcher(caster, targetList, u_check);
            //caster->VisitNearbyObject(10.0f, searcher);
            //if (!targetList.empty())
            //{
            //    for (auto itr : targetList)
            //    {
            //        if (!itr)
            //            continue;

            //        if (itr->IsPlayer())
            //            continue;

            //        if (itr && itr->IsInWorld())
            //            targets.push_back(itr);
            //    }
            //}
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(auchindoun_spell_void_shell_filter_SpellScript::CorrectTargets, SpellEffIndex::EFFECT_0, Targets::TARGET_UNIT_SRC_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_spell_void_shell_filter_SpellScript();
    }
};

/// Tuulani Unlock Gate - 160415
class auchindoun_spell_tuulani_unlock : public SpellScriptLoader
{
public:

    auchindoun_spell_tuulani_unlock() : SpellScriptLoader("auchindoun_spell_tuulani_unlock") { }

    class auchindoun_spell_tuulani_unlock_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_spell_tuulani_unlock_SpellScript);

        void CorrectTargets(std::list<WorldObject*>& targets)
        {
            /// Clears all targets at start, fetching new ones
            targets.clear();
            Unit* l_Caster = GetCaster();
            std::list<Creature*> l_ListTriggerWall;
            l_Caster->GetCreatureListWithEntryInGrid(l_ListTriggerWall, CreatureLightWallTargets, 15.0f);

            /// the wardens and clerics holding a barrier side by side each take the points closer to them than to the others
            std::list<Creature*> l_Peers;
            if (l_Caster->GetEntry() != CreatureSoulBinderTuulani)
                l_Caster->GetCreatureListWithEntryInGrid(l_Peers, l_Caster->GetEntry(), 30.0f);

            for (Creature* itr : l_ListTriggerWall)
            {
                if (!itr || !itr->IsInWorld())
                    continue;

                bool l_Mine = true;
                for (Creature* l_Peer : l_Peers)
                    if (l_Peer != l_Caster && l_Peer->IsAlive() && l_Peer->GetExactDist2d(itr) < l_Caster->GetExactDist2d(itr))
                        l_Mine = false;

                if (l_Mine)
                    targets.push_back(itr->ToUnit());
            }
        }

        void HandleAfterCast()
        {
            if (Unit* caster = GetCaster())
            {
                if (caster->GetEntry() != CreatureSoulBinderTuulani)
                    return;

                if (GameObject * l_GameObject = caster->FindNearestGameObject(GameobjectHolyBarrierEntra, 5.0f))
                {
                    l_GameObject->Delete();
                }
            }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(auchindoun_spell_tuulani_unlock_SpellScript::CorrectTargets, SpellEffIndex::EFFECT_0, Targets::TARGET_UNIT_DEST_AREA_ENTRY);
            AfterCast += SpellCastFn(auchindoun_spell_tuulani_unlock_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_spell_tuulani_unlock_SpellScript();
    }
};

/// Arcane Bolt - 157505 
class auchindoun_spell_arcane_bolt : public SpellScriptLoader
{
public:

    auchindoun_spell_arcane_bolt() : SpellScriptLoader("auchindoun_spell_arcane_bolt") { }

    class auchindoun_spell_arcane_bolt_AuraScript : public AuraScript
    {
        PrepareAuraScript(auchindoun_spell_arcane_bolt_AuraScript);

        enum eArcaneBoltSpells
        {
            SpellArcaneBoltPeriod = 157505,
            SpellArcaneBoltProje = 157931
        };

        void HandlePeriodic(AuraEffect const* auraEffect)
        {
            if (Unit* caster = GetCaster())
            {
                if (caster->IsAIEnabled)
                {
                    if (Unit* l_Target = caster->GetAI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        caster->CastSpell(l_Target, eArcaneBoltSpells::SpellArcaneBoltProje, true);
                }
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(auchindoun_spell_arcane_bolt_AuraScript::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new auchindoun_spell_arcane_bolt_AuraScript();
    }
};

/// Talador Portal - 236689
class auchindoun_gob_talador_portal : public GameObjectScript
{
public:

    auchindoun_gob_talador_portal() : GameObjectScript("auchindoun_gob_talador_portal") { }

    bool OnGossipHello(Player* p_Player, GameObject* p_Gameobject) override
    {
        p_Player->TeleportTo(1116, 1488.52f, 3077.65f, 108.920f, 4.653427f);
        return true;
    }
};

/// Auchenai Cleric - 77947: two of them hold the barrier near the entrance, but only once the Sargerei have closed it (second phase)
class auchindoun_mob_barrier_keeper : public CreatureScript
{
public:

    auchindoun_mob_barrier_keeper() : CreatureScript("auchindoun_mob_barrier_keeper") { }

    struct auchindoun_mob_barrier_keeperAI : public ScriptedAI
    {
        explicit auchindoun_mob_barrier_keeperAI(Creature* creature) : ScriptedAI(creature), m_Active(false), m_Timer(0) { }

        bool m_Active;
        uint32 m_Timer;

        bool IsKeeper() const
        {
            uint64 l_Spawn = me->GetDBTableGUIDLow();
            return l_Spawn == 10530211 || l_Spawn == 10530215;
        }

        void Reset() override
        {
            m_Active = false;
            m_Timer = 0;
        }

        /// the instance holds the barrier with Tuulani's beam now: the holy beam only shone on the cleric himself, and never ended
        void DoAction(int32 const /*p_Action*/) override { }

        void UpdateAI(uint32 p_Diff) override
        {
            if (UpdateVictim())
            {
                DoMeleeAttackIfReady();
                return;
            }

            if (!m_Active)
                return;

            if (m_Timer > p_Diff)
            {
                m_Timer -= p_Diff;
                return;
            }

            m_Timer = 1000;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            std::list<Creature*> l_Stalkers;
            me->GetCreatureListWithEntryInGrid(l_Stalkers, CreatureLightWallTargets, 25.0f);
            if (l_Stalkers.empty())
                return;

            std::list<Creature*>::iterator l_Itr = l_Stalkers.begin();
            std::advance(l_Itr, urand(0, uint32(l_Stalkers.size()) - 1));
            me->CastSpell(*l_Itr, SpellHolyBeam, false);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_mob_barrier_keeperAI(creature);
    }
};

/// Scene 913: Tuulani runs to the boss room. When it ends the players stand at its last place
class auchindoun_scene_tuulani_reach_nyami : public SceneTriggerScript
{
public:

    auchindoun_scene_tuulani_reach_nyami() : SceneTriggerScript("auchindoun_scene_tuulani_reach_nyami") { }

    bool OnTrigger(Player* p_Player, SpellScene const* /*p_Trigger*/, std::string p_Type) override
    {
        if (p_Type != "complete")
            return false;

        p_Player->NearTeleportTo(1848.0f, 3196.0f, 29.4f, 0.4f);

        /// skipping the scene ends it too: the client reports a cancel, which completes it
        if (InstanceScript* l_Instance = p_Player->GetInstanceScript())
            if (Creature* l_Tuulani = l_Instance->instance->GetCreature(l_Instance->GetGuidData(DataTuulani)))
                if (l_Tuulani->IsAIEnabled)
                    l_Tuulani->AI()->DoAction(ActionTuulaniSceneOver);

        return false;
    }
};

/// Teron'gor arrives (scene 914, after Azzakel): he and his followers are there once it is over, watched or skipped
class auchindoun_scene_teronogor_arrival : public SceneTriggerScript
{
public:
    auchindoun_scene_teronogor_arrival() : SceneTriggerScript("auchindoun_scene_teronogor_arrival") { }

    bool OnTrigger(Player* p_Player, SpellScene const* /*p_Trigger*/, std::string p_Type) override
    {
        if (p_Type == "complete")
            AuchindounRevealTeronogor(p_Player);

        return false;
    }
};

void AddSC_auchindoun()
{
    new auchindoun_scene_teronogor_arrival();       ///< scene 914
    new auchindoun_mob_tuulani();                   ///< 79248
    new auchindoun_mob_barrier_keeper();            ///< 77947
    new auchindoun_scene_tuulani_reach_nyami();     ///< scene 913
    new auchindoun_mob_sargerei_soulbinder();       ///< 77812  
    new auchindoun_mob_sargerei_cleric();           ///< 77134
    new auchindoun_mob_sargerei_ritualist();        ///< 77130
    new auchindoun_mob_sargerei_zealot();           ///< 77132
    new auchindoun_mob_sargerei_spirit_tender();    ///< 77131
    new auchindoun_mob_sargerei_hopilite();         ///< 77133
    new auchindoun_mob_sargeri_warden();            ///< 77935
    new auchindoun_mob_sargerei_magus();            ///< 76263
    new auchindoun_mob_sargerei_defender();         ///< 77042
    new auchindoun_mob_felborne_abyssal();          ///< 77905
    new auchindoun_mob_soul_priest();               ///< 76595
    new auchindoun_mob_felguard();                  ///< 76259
    new auchindoun_mob_cackling_pyromaniac();       ///< 76260
    new auchindoun_mob_blazing_trickster();         ///< 79511
    new auchindoun_mob_warden_hammer();             ///< 76655
    new auchindoun_spell_void_shift();              ///< 155524
    new auchindoun_spell_void_mending();            ///< 154623
    new auchindoun_spell_void_shell_filter();       ///< 160312
    new auchindoun_spell_psychic_terror();          ///< 154356
    new auchindoun_spell_tuulani_unlock();          ///< 160415
    new auchindoun_spell_arcane_bolt();             ///< 157505
    new auchindoun_gob_talador_portal();            ///< 236689
}