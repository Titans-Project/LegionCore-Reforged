////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "GridNotifiers.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "MoveSplineInit.h"
#include "SpellScript.h"
#include "auchindoun.hpp"

enum eKaatharSpells
{
    SpellHallowedGround                = 154526,
    SpellHallowedGroundsTriggerMissile = 155646,
    SpellConsecratedLight              = 153006,
    SpellConsecratedLightDamage        = 156746,
    SpellHolyShieldThrow               = 153002,
    SpellHolyShieldLos                 = 153028,
    SpellHolyShieldLosSpells           = 153452,
    SpellHolyShieldAreatrigger         = 153478,
    SpellHolyShieldOffHandDisarm       = 174205,
    SpellHolyShieldTwoHandDisarm       = 174206,
    SpellHolyShieldKnockBack           = 153481,
    SpellHolyShieldDamageFromKnockBack = 153480,
    SpellHolyShieldAreaTrigger         = 153478,
    SpellSanctifiedStrikeDummy         = 152954,
    SpellSanctifiedGroundTickDamage    = 161457,
    SpellSanctifiedGroundAura          = 153430,
    SpellSanctifiedGroundDamageTick    = 161457,
    SpellSanctifiedStrikeAreaTrigger   = 165064,
    SpellSanctifiedStrikeAreaTrigger2  = 163559,
    SpellSancitfiedStrikeAreaTrigger3  = 165055,
    SpellSanctifiedStrikeAreaTrigger4  = 165065,
    SpellFate                          = 157465
};

enum eKaatharEvents
{
    EventHallowedGround = 1,
    EventHolyShield,
    EventConsecratedLight,
    EventSanctifiedStrike,
    EventHolyShieldReturn,
    EventFate,
    EventCheckPlayer,
    EventDecreaseSize
};

enum eKaatharActions
{
    ActionActivateBoss = 1,
    ActionFateHallowedGround,
    ActionDespawnCreatures
};

enum eKaatharTalks
{
    VigilantKaatherIntro  = 18,   ///< None Live who Assault The Holy Auchenai. (46436)
    VigilantKaatherAgro   = 19,   ///< I Will Strike You Down. (46434)
    VigilantKaatherSpell1 = 20,   ///< Bathe In The Glory Of The Light! (46438)
    VigilantKaatherSpell2 = 21,   ///< The Light Guide My Hand! (46439)
    VigilantKaatherKill   = 22,   ///< Light Guide You. (46434)
    VigilantKaatherDeath  = 23,   ///< Auchindoun...Is...Doomed...(46435)
    VigilantKaatherShieldEmote = 30,      ///< raid boss emotes, in the language of each player (creature_text and its locales)
    VigilantKaatherLightEmote  = 31,
    VigilantKaatherFateEmote   = 32
};

enum eKaatharCreatures
{
    TriggerHallowedGround  = 537324,
    TriggerHolyShield      = 76071,
    GameobjectHolyShieldWall = 225749,
    TriggerFissureSummoner = 543536
};

Position const g_KaatharNewHomePosition = {1911.47f, 3152.13f, 30.972f, 1.166194f};


/// 1st Starting Event
class EventKaatharDespawnCreatures : public BasicEvent
{
public:

    explicit EventKaatharDespawnCreatures(Unit* unit, int value) : BasicEvent(), m_InstanceScript(nullptr), m_Obj(unit), m_Modifier(value), m_Event(0)
    {
    }

    bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/) override
    {
        if (m_Obj)
        {
            if (auto instance = m_Obj->GetInstanceScript())
            {
                if (auto l_Kaathar = instance->instance->GetCreature(instance->GetGuidData(DataBossKathaar)))
                {
                    if (l_Kaathar->IsAIEnabled)
                        l_Kaathar->GetAI()->DoAction(ActionDespawnCreatures);
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

class EventNyamiEscape : public BasicEvent
{
public:

    explicit EventNyamiEscape(Unit* unit, int value) : BasicEvent(), m_Obj(unit), m_Modifier(value), m_Event(0)
    {
    }

    bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/) override
    {
        if (m_Obj)
        {
            if (InstanceScript* instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Tuulani = instance->instance->GetCreature(instance->GetGuidData(DataTuulani)))
                {
                    if (Creature* l_Nyami = instance->instance->GetCreature(instance->GetGuidData(DataNyami)))
                    {
                        if (Creature* l_Kaathar = instance->instance->GetCreature(instance->GetGuidData(DataBossKathaar)))
                        {
                            if (l_Tuulani->IsAIEnabled && l_Nyami->IsAIEnabled && l_Kaathar->IsAIEnabled)
                            {
                                switch (m_Modifier)
                                {
                                    case 0:
                                    {
                                        l_Nyami->AI()->Talk(NYAMITALK6);
                                        l_Nyami->m_Events.AddEvent(new EventNyamiEscape(l_Nyami, 1), l_Nyami->m_Events.CalculateTime(5 * IN_MILLISECONDS));
                                        break;
                                    }
                                    case 1:
                                    {
                                        /// her line is over: she turns into her small ball, out of her cage, and goes in front of the boss, out of the way of the crystal
                                        l_Nyami->RemoveAurasDueToSpell(SpellVoidShellNyami);
                                        l_Nyami->RemoveAurasDueToSpell(SpellPrisonAura);
                                        l_Nyami->RemoveAurasDueToSpell(SpellDarkFire);
                                        l_Nyami->RemoveAurasDueToSpell(SpellLevitateNyami);
                                        l_Nyami->InterruptNonMeleeSpells(true);
                                        l_Nyami->AddAura(SpellDispersionVisualNyami, l_Nyami);
                                        l_Nyami->GetMotionMaster()->MovePoint(MovementInformNyamiEscape06, g_PositionNyamiEscapeMovement[1].GetPositionX(), g_PositionNyamiEscapeMovement[1].GetPositionY(), g_PositionNyamiEscapeMovement[1].GetPositionZ() + 4.0f);

                                        /// the big crystal (the Soul Aegis creature, no collision) loses its void, breaks and falls
                                        if (Creature* l_Crystal = l_Nyami->FindNearestCreature(77832, 100.0f))
                                        {
                                            l_Crystal->RemoveAllAuras();
                                            l_Crystal->Kill(l_Crystal);
                                            l_Crystal->DespawnOrUnsummon(3500);
                                        }

                                        /// the crystal is broken: the barrier at the door of the room falls with it
                                        std::list<GameObject*> l_Barriers;
                                        AuchindounGameObjectsAnyPhase(l_Nyami, l_Barriers, GameobjectKaatharRoomBarrier, 200.0f);
                                        for (GameObject* l_Barrier : l_Barriers)
                                            l_Barrier->Delete();

                                        /// the triggers of the crystal keep no void effect
                                        std::list<Creature*> l_Stalkers;
                                        l_Nyami->GetCreatureListWithEntryInGrid(l_Stalkers, CreatureLeftCrystalTrigger, 200.0f);
                                        for (Creature* l_Stalker : l_Stalkers)
                                        {
                                            if (l_Stalker->GetExactDist2d(1911.7f, 3183.6f) > 20.0f)
                                                continue;

                                            l_Stalker->InterruptNonMeleeSpells(true);
                                            l_Stalker->RemoveAllAuras();
                                        }

                                        /// the shake and the fall of the players come one second after the crystal began to break
                                        l_Nyami->m_Events.AddEvent(new EventNyamiEscape(l_Nyami, 5), l_Nyami->m_Events.CalculateTime(1 * IN_MILLISECONDS));

                                        /// the void orbs of the crystal fly to the central hall, where Teron'gor is
                                        if (Creature* l_Teronoger = instance->instance->GetCreature(instance->GetGuidData(DataBossTeronogor)))
                                        {
                                            for (uint8 l_I = 0; l_I < 20; l_I++)
                                            {
                                                Position l_Source(1911.741f, 3183.639f, 56.50413f);
                                                Position l_Dest(l_Teronoger->GetPositionX(), l_Teronoger->GetPositionY(), 59.996f);
                                                Position l_Orientation(0.0f, 0.0f, 0.0f);

                                                l_Nyami->PlayOrphanSpellVisual(l_Source, l_Orientation, l_Dest, SpellVisualKitBlackOrbFallingDownInSpiral, 8.0f);
                                            }
                                        }

                                        l_Nyami->m_Events.AddEvent(new EventNyamiEscape(l_Nyami, 2), l_Nyami->m_Events.CalculateTime(3 * IN_MILLISECONDS));
                                        break;
                                    }
                                    case 5:
                                    {
                                        /// the crystal hits the floor: shake, and the players are stunned and thrown down
                                        if (Creature* l_Crystal = l_Nyami->FindNearestCreature(77832, 100.0f, false))
                                        {
                                            l_Crystal->CastSpell(l_Crystal, SpellNyamiExplodeCrystal, true);
                                            l_Crystal->CastSpell(l_Crystal, SpellCrystalEarthquake, true);
                                        }

                                        std::list<Player*> l_Players;
                                        l_Nyami->GetPlayerListInGrid(l_Players, 120.0f);
                                        for (Player* l_Player : l_Players)
                                            l_Player->SetControlled(true, UNIT_STATE_STUNNED);

                                        break;
                                    }
                                    case 2:
                                    {
                                        std::list<Player*> l_Players;
                                        l_Nyami->GetPlayerListInGrid(l_Players, 120.0f);
                                        for (Player* l_Player : l_Players)
                                            l_Player->SetControlled(false, UNIT_STATE_STUNNED);

                                        /// second wave of orbs
                                        if (Creature* l_Teronoger = instance->instance->GetCreature(instance->GetGuidData(DataBossTeronogor)))
                                        {
                                            for (uint8 l_I = 0; l_I < 20; l_I++)
                                            {
                                                Position l_Source(1911.741f, 3183.639f, 56.50413f);
                                                Position l_Dest(l_Teronoger->GetPositionX(), l_Teronoger->GetPositionY(), 59.996f);
                                                Position l_Orientation(0.0f, 0.0f, 0.0f);

                                                l_Nyami->PlayOrphanSpellVisual(l_Source, l_Orientation, l_Dest, SpellVisualKitBlackOrbFallingDownInSpiral, 8.0f);
                                            }
                                        }

                                        l_Nyami->AI()->Talk(NYAMITALK7);
                                        l_Nyami->m_Events.AddEvent(new EventNyamiEscape(l_Nyami, 50), l_Nyami->m_Events.CalculateTime(10 * IN_MILLISECONDS));
                                        break;
                                    }
                                    case 50:
                                    {
                                        l_Nyami->AI()->Talk(NYAMITALK8);
                                        l_Nyami->CastSpell(l_Kaathar, SpellHolyBeam, false);
                                        l_Tuulani->m_Events.AddEvent(new EventNyamiEscape(l_Tuulani, 4), l_Tuulani->m_Events.CalculateTime(3500));
                                        break;
                                    }
                                    case 3:
                                    {
                                        /// she goes round the room, then leaves the way the players came, and is gone a few seconds later
                                        Movement::PointsArray l_Tour;
                                        l_Tour.emplace_back(l_Nyami->GetPositionX(), l_Nyami->GetPositionY(), l_Nyami->GetPositionZ());
                                        for (uint8 l_I = 1; l_I < 5; ++l_I)
                                            l_Tour.emplace_back(g_PositionNyamiEscapeMovement[l_I].GetPositionX(), g_PositionNyamiEscapeMovement[l_I].GetPositionY(), g_PositionNyamiEscapeMovement[l_I].GetPositionZ());
                                        l_Tour.emplace_back(1801.23f, 3178.36f, 45.0f);
                                        l_Tour.emplace_back(1765.926f, 3178.208f, 57.904f);

                                        l_Nyami->SetCanFly(true);
                                        l_Nyami->SetSpeed(UnitMoveType::MOVE_FLIGHT, 14.0f, true);
                                        l_Nyami->SetSpeed(UnitMoveType::MOVE_RUN, 14.0f, true);
                                        Movement::MoveSplineInit l_Init(*l_Nyami);
                                        l_Init.MovebyPath(l_Tour);
                                        l_Init.SetFly();
                                        l_Init.Launch();
                                        l_Nyami->m_Events.AddEvent(new EventNyamiEscape(l_Nyami, 70), l_Nyami->m_Events.CalculateTime(9 * IN_MILLISECONDS));
                                        l_Tuulani->m_Events.AddEvent(new EventNyamiEscape(l_Tuulani, 7), l_Tuulani->m_Events.CalculateTime(6 * IN_MILLISECONDS));
                                        break;
                                    }
                                    case 4:
                                    {
                                        /// the boss wakes up after her last line: he leaves his statue stance and speaks
                                        l_Nyami->InterruptNonMeleeSpells(false);       /// the light that woke Kaathar stops
                                        instance->SetData(DataPreKaatharCleared, 1);
                                        l_Kaathar->RemoveAura(SpellVigilantSubmergeState);
                                        l_Kaathar->RemoveAura(SpellGuard);
                                        l_Kaathar->setFaction(HostileFaction);
                                        l_Kaathar->SetReactState(REACT_AGGRESSIVE);
                                        l_Kaathar->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                                        l_Kaathar->AI()->Talk(VigilantKaatherIntro);
                                        l_Tuulani->m_Events.AddEvent(new EventNyamiEscape(l_Tuulani, 6), l_Tuulani->m_Events.CalculateTime(3500));
                                        break;
                                    }
                                    case 6:
                                    {
                                        l_Kaathar->GetMotionMaster()->MoveJump(g_PositionKaatharCombatJump.GetPositionX(), g_PositionKaatharCombatJump.GetPositionY(), g_PositionKaatharCombatJump.GetPositionZ(), 10.0f, 10.0f, 10.0f, 0);
                                        l_Tuulani->m_Events.AddEvent(new EventNyamiEscape(l_Tuulani, 61), l_Tuulani->m_Events.CalculateTime(1500));
                                        break;
                                    }
                                    case 61:
                                    {
                                        /// he lands on the platform: the ground shakes, then she is gone
                                        l_Kaathar->CastSpell(l_Kaathar, SpellCrystalEarthquake, true);
                                        l_Nyami->m_Events.AddEvent(new EventNyamiEscape(l_Nyami, 3), l_Nyami->m_Events.CalculateTime(1 * IN_MILLISECONDS));
                                        break;
                                    }
                                    case 70:
                                    {
                                        l_Nyami->DespawnOrUnsummon();
                                        break;
                                    }
                                    case 7:
                                    {
                                        l_Tuulani->AI()->Talk(TUULANITALK9);
                                        break;
                                    }
                                    case 9:
                                    {
                                        l_Tuulani->AI()->Talk(TUULANITALK9);
                                        break;
                                    }
                                    default:
                                        break;
                                }
                            }
                        }
                    }
                }
            }
        }

        return true;
    }

private:
    Unit* m_Obj;
    int m_Modifier;
    int m_Event;
};


class EventPostKaathar : public BasicEvent
{
public:
    explicit EventPostKaathar(Unit* unit, int value) : BasicEvent(), m_Obj(unit), m_Modifier(value), m_Event(0)
    {
    }

    bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/) override
    {
        if (m_Obj)
        {
            if (auto instance = m_Obj->GetInstanceScript())
            {
                if (auto l_Tuulani = instance->instance->GetCreature(instance->GetGuidData(DataTuulani)))
                {
                    if (l_Tuulani->IsAIEnabled)
                    {
                        switch (m_Modifier)
                        {
                            case 0:
                            {
                                l_Tuulani->RemoveAura(SpellTuulaniCapturedVoidPrison);
                                l_Tuulani->RemoveUnitMovementFlag(MOVEMENTFLAG_ROOT);
                                l_Tuulani->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_REMOVE_CLIENT_CONTROL);
                                l_Tuulani->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                                l_Tuulani->m_Events.AddEvent(new EventPostKaathar(l_Tuulani, 3), l_Tuulani->m_Events.CalculateTime(5 * IN_MILLISECONDS));

                                for (int8 l_I = 0; l_I < 4; l_I++)
                                {
                                    if (Creature* l_Defender = m_Obj->SummonCreature(CreatureAucheniDefender, g_PositionFourMagesThatSpawnAfterKaatharIsKaaput[l_I], TEMPSUMMON_DEAD_DESPAWN))
                                    {
                                        l_Defender->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 505);
                                        l_Defender->GetMotionMaster()->MovePoint(0, g_PositionFourMagesThatSpawnAfterKaatharIsKaaput[l_I]);
                                    }
                                }

                                if (Creature* l_Nyami = instance->instance->GetCreature(instance->GetGuidData(DataBossNyami)))
                                    l_Nyami->SetPhaseMask(1, true);

                                if (Creature* l_Magus = m_Obj->SummonCreature(CreatureAucheniMagus, g_PositionMageSpawning, TEMPSUMMON_DEAD_DESPAWN))
                                {
                                    l_Magus->GetMotionMaster()->MovePoint(0, g_PositionMageMoveTo);
                                    l_Magus->m_Events.AddEvent(new EventPostKaathar(l_Magus, 1), l_Magus->m_Events.CalculateTime(7 * IN_MILLISECONDS));
                                    l_Magus->m_Events.AddEvent(new EventPostKaathar(l_Magus, 2), l_Magus->m_Events.CalculateTime(55 * IN_MILLISECONDS));
                                }
                                break;
                            }
                            case 1:
                            {

                                /// the corridor, the walls and the wardens of the second phase are spawned in the database: only Nyami comes here
                                l_Tuulani->SummonCreature(BossNyami, g_PositionNyamiSpawn, TEMPSUMMON_MANUAL_DESPAWN);

                                m_Obj->CastSpell(m_Obj, SpellArcaneChanneling);
                                m_Obj->SummonGameObject(GameobjectTaladorPortal, g_PositionTuulaniGobjectPortalSpawn.GetPositionX(), g_PositionTuulaniGobjectPortalSpawn.GetPositionY(), g_PositionTuulaniGobjectPortalSpawn.GetPositionZ(), g_PositionTuulaniGobjectPortalSpawn.GetOrientation(), 0, 0, 0, 0, 0);
                                break;
                            }
                            case 3:
                            {
                                l_Tuulani->AI()->Talk(TUULANITALK11);
                                l_Tuulani->m_Events.AddEvent(new EventPostKaathar(l_Tuulani, 4), l_Tuulani->m_Events.CalculateTime(5 * IN_MILLISECONDS));
                                break;
                            }
                            case 4:
                            {
                                l_Tuulani->AI()->Talk(TUULANITALK12);
                                l_Tuulani->m_Events.AddEvent(new EventPostKaathar(l_Tuulani, 5), l_Tuulani->m_Events.CalculateTime(6 * IN_MILLISECONDS));
                                break;
                            }
                            case 5:
                            {
                                l_Tuulani->AI()->Talk(TUULANITALK13);
                                break;
                            }
                            case 2:
                            {
                                /// the demons of the room of Azzakel; Teron'gor and his followers come with the scene after Azzakel
                                AuchindounRevealAzzakelRoom(l_Tuulani);
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
    Unit* m_Obj;
    int m_Modifier;
    int m_Event;
};

/// Nyami Mob - 77810
class auchindoun_kaathar_mob_nyami : public CreatureScript
{
public:

    auchindoun_kaathar_mob_nyami() : CreatureScript("auchindoun_kaathar_mob_nyami") {}

    struct auchindoun_kaathar_mob_nyamiAI : public ScriptedAI
    {
        auchindoun_kaathar_mob_nyamiAI(Creature* creature) : ScriptedAI(creature)
        {
            m_Instance = creature->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->setFaction(FriendlyFaction);
            me->SetReactState(REACT_PASSIVE);
            me->CastSpell(me, SpellLevitateNyami);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_kaathar_mob_nyamiAI(creature);
    }
};

/// Vigilant Kaathar - 75839
class boss_kaathar : public CreatureScript
{
public:

    boss_kaathar() : CreatureScript("boss_kaathar") { }

    struct boss_kaatharAI : public BossAI
    {
        boss_kaatharAI(Creature* creature) : BossAI(creature, DataBossKathaar), m_Counting(0), m_IntroDone(false), m_Intro(false)
        {
            m_Instance = me->GetInstanceScript();
            m_False = true;
        }

        InstanceScript* m_Instance;
        uint32 m_Counting;
        bool m_IntroDone;
        bool m_Intro;
        bool m_False;

        void Reset() override
        {
            _Reset();
            events.Reset();
            KillAllDelayedEvents();

            if (m_Instance && m_Instance->GetBossState(DataBossKathaar) == DONE)
            {
                me->SetPhaseMask(2, true);
                HideFirstRoom();
                return;
            }
            me->SetCurrentEquipmentId(1);

            ActivateDoors();
            SetBarrierPhase(2);      /// the barrier only exists once the fight has started

            /// the instance was reloaded after the first room was cleared: Kaathar is awake, the room is empty
            if (m_Instance && m_Instance->GetData(DataPreKaatharCleared) && m_Instance->GetBossState(DataBossKathaar) != DONE)
            {
                m_False = false;
                m_IntroDone = true;
                m_Intro = true;
                me->setFaction(HostileFaction);
                me->RemoveAura(SpellVigilantSubmergeState);
                me->RemoveAura(SpellGuard);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->NearTeleportTo(g_PositionKaatharCombatJump);
                me->SetHomePosition(g_PositionKaatharCombatJump);
                HideFirstRoom();
            }

            if (m_False)
            {
                m_Counting = 0;
                m_Intro = false;
                m_False = false;
                m_IntroDone = false;
                me->setFaction(FriendlyFaction);
                me->CastSpell(me, SpellGuard);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            }

            if (m_Instance)
                m_Instance->DoRemoveAurasDueToSpellOnPlayers(SpellSanctifiedGroundAura);
        }

        /// the Sargerei of the first room, Nyami and the crystal are gone once the room was cleared
        void HideFirstRoom()
        {
            std::list<Creature*> l_Creatures;
            for (uint32 l_Entry : { 77130, 77131, 77132, 77133, 77134, 77812, 77810, 77832 })
                me->GetCreatureListWithEntryInGrid(l_Creatures, l_Entry, 300.0f);

            for (Creature* l_Creature : l_Creatures)
            {
                l_Creature->SetReactState(REACT_PASSIVE);
                l_Creature->SetPhaseMask(2, true);
            }
        }

        /// the barrier of the first boss (230399) only exists once the fight has started
        void SetBarrierPhase(uint32 p_Mask)
        {
            std::list<GameObject*> l_Doors;
            me->GetGameObjectListWithEntryInGrid(l_Doors, 230399, 300.0f);
            for (GameObject* l_Door : l_Doors)
                l_Door->SetPhaseMask(p_Mask, true);
        }

        void ActivateDoors()
        {
            if (m_Instance)
            {
                if (GameObject* l_HolyBarrier = m_Instance->instance->GetGameObject(m_Instance->GetGuidData(DataHolyBarrier)))
                {
                    l_HolyBarrier->SetLootState(GO_READY);
                    l_HolyBarrier->UseDoorOrButton();
                }
            }
        }

        void JustReachedHome() override
        {
            _JustReachedHome();
            summons.DespawnAll();
        }

        /// one segment of fissure every 2.2 yards, wandering a little; the branches split off to both sides, shorter each time
        void SummonFissureBranch(float p_X, float p_Y, float p_Angle, uint8 p_Length, uint8 p_Depth)
        {
            for (uint8 l_I = 0; l_I < p_Length; ++l_I)
            {
                p_Angle = Position::NormalizeOrientation(p_Angle + frand(-0.45f, 0.45f));
                me->SummonCreature(TriggerFissureSummoner, p_X, p_Y, me->GetPositionZ(), p_Angle, TEMPSUMMON_TIMED_DESPAWN, 15 * IN_MILLISECONDS);

                /// the branches leave at a third and at two thirds of the crack, one on each side, so they do not crowd near the boss
                if (p_Depth && p_Length >= 3 && (l_I == p_Length / 3 || l_I == 2 * p_Length / 3))
                {
                    float l_Side = l_I == p_Length / 3 ? 1.0f : -1.0f;
                    SummonFissureBranch(p_X, p_Y, p_Angle + l_Side * frand(0.8f, 1.3f), p_Length / 2 + 1, p_Depth - 1);
                }

                /// a segment covers 2.5 yards: the next one starts at its end, the crack stays in one piece
                p_X += 2.4f * std::cos(p_Angle);
                p_Y += 2.4f * std::sin(p_Angle);
            }
        }

        void MoveInLineOfSight(Unit* who) override
        {
            if (who && who->IsInWorld() && who->IsPlayer() && !m_Intro && me->IsWithinDistInMap(who, 10.0f) && m_IntroDone)
            {
                m_Intro = true;     /// his line was already said when Nyami woke him
                me->SetHomePosition(g_KaatharNewHomePosition);
            }

            BossAI::MoveInLineOfSight(who);
        }

        void KilledUnit(Unit* who) override
        {
            if (who && who->IsPlayer())
                Talk(VigilantKaatherKill);
        }

        void EnterCombat(Unit* who) override
        {
            _EnterCombat();
            ActivateDoors();
            SetBarrierPhase(1);

            if (me->GetMap() && me->GetMap()->IsHeroic())
                events.RescheduleEvent(EventFate, 45 * IN_MILLISECONDS);

            events.RescheduleEvent(EventHallowedGround, urand(12 * IN_MILLISECONDS, 17 * IN_MILLISECONDS));
            events.RescheduleEvent(EventSanctifiedStrike, 8 * IN_MILLISECONDS);
            events.RescheduleEvent(EventHolyShield, 30 * IN_MILLISECONDS);

            if (m_Instance)
                m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            Talk(VigilantKaatherAgro);
        }

        void DoAction(int32 const actionID) override
        {
            switch (actionID)
            {
                case ActionCountPre1StBossKill:
                {
                    m_Counting = m_Counting + 1;

                    if (m_Counting >= 12)
                    {
                        me->RemoveAura(SpellGuard);

                        if (m_Instance)
                        {
                            if (Creature* l_Tuulani = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataTuulani)))
                            {
                                if (Creature* l_Nyami = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataNyami)))
                                {
                                    l_Tuulani->m_Events.KillAllEvents(true);
                                    l_Nyami->m_Events.KillAllEvents(true);
                                }
                            }
                        }

                        m_IntroDone = true;
                        me->m_Events.AddEvent(new EventNyamiEscape(me, 0), me->m_Events.CalculateTime(1 * IN_MILLISECONDS));
                    }
                    break;
                }
                case ActionDespawnCreatures:
                    DespawnAllAucheniDraeneis();
                    break;
                default:
                    break;
            }
        }

        /// Responsible for the phase change after Kaathar fight - despawning
        void DespawnAllAucheniDraeneis()
        {
            /// second phase: the Sargerei come back and shut the barrier near the entrance
            AuchindounRevealSecondPhase(me);

            /// the two clerics holding the barrier start their beam
            std::list<Creature*> l_Keepers;
            me->GetCreatureListWithEntryInGrid(l_Keepers, CreatureAucheniCleric, 700.0f);
            for (Creature* itr : l_Keepers)
                if (itr->IsAIEnabled)
                    itr->AI()->DoAction(ActionBarrierKeeperStart);

            if (m_Instance)
            {
                if (Creature* l_AssinatingGuard = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataAssinatingGuard)))
                {
                    if (Creature* l_AssinatedGuard = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataAssinatedGuard)))
                    {
                        l_AssinatingGuard->GetMotionMaster()->MovePoint(0, *l_AssinatedGuard);
                        l_AssinatingGuard->Attack(l_AssinatedGuard, true);
                        l_AssinatedGuard->Kill(l_AssinatedGuard);
                    }
                }
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();

            std::list<Player*> l_ListPlayers;
            me->GetPlayerListInGrid(l_ListPlayers, 600.0f/*, true*/);
            if (!l_ListPlayers.empty())
            {
                for (Player* itr : l_ListPlayers)
                {
                    if (!itr)
                        continue;

                    //itr->PlayScene(SpellAuchindounSceneTeronogorSpawn, itr);
                }
            }

            DespawnAllAucheniDraeneis();

            /// Remove the auchenai shield npc
            if (Creature* l_Nearest = me->FindNearestCreature(CreatureAuchenaiShield, 300.0f))
                l_Nearest->DespawnOrUnsummon();

            if (m_Instance)
                m_Instance->DoRemoveAurasDueToSpellOnPlayers(SpellSanctifiedGroundAura);

            Talk(VigilantKaatherDeath);
            me->m_Events.AddEvent(new EventPostKaathar(me, 0), me->m_Events.CalculateTime(2 * IN_MILLISECONDS));
        }

        void UpdateAI(uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case EventFate:
                {
                    me->CastSpell(me, SpellFate);
                    Talk(VigilantKaatherFateEmote);
                    events.RescheduleEvent(EventFate, 30 * IN_MILLISECONDS);
                    break;
                }
                case EventHallowedGround:
                {
                    Position l_Position = me->GetRandomNearPosition(20.0f);
                    me->SummonCreature(TriggerHallowedGround, l_Position, TEMPSUMMON_TIMED_DESPAWN, 30 * IN_MILLISECONDS);
                    events.RescheduleEvent(EventHallowedGround, 8 * IN_MILLISECONDS);
                    break;
                }
                case EventHolyShield:
                {
                    if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    {
                        Talk(VigilantKaatherSpell2);
                        me->SetFacingToObject(l_Target);
                        me->CastSpell(l_Target, SpellHolyShieldThrow);
                        if (me->HasAura(SpellHolyShieldOffHandDisarm))
                            me->AddAura(SpellHolyShieldTwoHandDisarm, me);
                        else
                            me->AddAura(SpellHolyShieldOffHandDisarm, me);

                        Talk(VigilantKaatherShieldEmote, l_Target->GetGUID());
                        events.RescheduleEvent(EventConsecratedLight, 4 * IN_MILLISECONDS);
                        events.RescheduleEvent(EventHolyShieldReturn, 14 * IN_MILLISECONDS);
                    }
                    events.RescheduleEvent(EventHolyShield, 30 * IN_MILLISECONDS);
                    break;
                }
                case EventHolyShieldReturn:
                {
                    /// he takes his shield back even when the invisible creature that held it is already gone; its wall goes with it
                    me->SetCurrentEquipmentId(1); /// Equipment Id
                    DespawnCreaturesInArea(TriggerHolyShield, me);

                    if (GameObject* l_Wall = me->FindNearestGameObject(GameobjectHolyShieldWall, 100.0f))
                        l_Wall->Delete();

                    if (me->HasAura(SpellHolyShieldOffHandDisarm))
                        me->RemoveAura(SpellHolyShieldOffHandDisarm);
                    else if (me->HasAura(SpellHolyShieldTwoHandDisarm))
                        me->RemoveAura(SpellHolyShieldTwoHandDisarm);
                    break;
                }
                case EventConsecratedLight:
                {
                    Talk(VigilantKaatherSpell1);
                    DoCast(SpellConsecratedLight);
                    Talk(VigilantKaatherLightEmote);
                    break;
                }
                case EventSanctifiedStrike:
                {
                    DoCastVictim(SpellSanctifiedStrikeDummy);

                    float l_Angle = me->getVictim() ? me->GetAngle(me->getVictim()) : me->GetOrientation();
                    SummonFissureBranch(me->GetPositionX() + 2.0f * std::cos(l_Angle), me->GetPositionY() + 2.0f * std::sin(l_Angle), l_Angle, 9, 2);
                    events.RescheduleEvent(EventSanctifiedStrike, 8 * IN_MILLISECONDS);
                    break;
                }
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_kaatharAI(creature);
    }
};

/// Hallowed Ground - 537324
class auchindoun_kaathar_mob_hallowed_ground : public CreatureScript
{
public:

    auchindoun_kaathar_mob_hallowed_ground() : CreatureScript("auchindoun_kaathar_mob_hallowed_ground") { }

    struct auchindoun_kaathar_mob_hallowed_groundAI : public Scripted_NoMovementAI
    {
        auchindoun_kaathar_mob_hallowed_groundAI(Creature* creature) : Scripted_NoMovementAI(creature), m_VisualDiff(0), m_HasExploded(false)
        {
            m_First = false;
        }

        uint32 m_VisualDiff;
        bool m_HasExploded;
        bool m_First;
        EventMap events;

        void Reset() override
        {
            events.Reset();
            m_HasExploded = false;
            me->setFaction(HostileFaction);
            m_VisualDiff = 1 * IN_MILLISECONDS;
            events.RescheduleEvent(EventCheckPlayer, 4 * IN_MILLISECONDS); // Takes 4 seconds to charge

            if (!m_First)
            {
                me->SetObjectScale(1.0f);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);
            }
        }

        void DoAction(int32 const actionID) override
        {
            switch (actionID)
            {
                case ActionFateHallowedGround:
                    DoCast(me, SpellHallowedGround);
                    me->DespawnOrUnsummon(1 * IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(uint32 p_Diff) override
        {
            events.Update(p_Diff);

            // Visual
            if (m_VisualDiff <= p_Diff)
            {
                me->CastSpell(me, SpellHallowedGroundsTriggerMissile);
                m_VisualDiff = 1 * IN_MILLISECONDS;
            }
            else
                m_VisualDiff -= p_Diff;

            switch (events.ExecuteEvent())
            {
                case EventCheckPlayer:
                    if (m_HasExploded)
                        return;

                    if (Player* player = me->FindNearestPlayer(4.0f, true))
                    {
                        if (player->IsWithinDistInMap(me, 4.0f))
                        {
                            m_HasExploded = true;

                            DoCast(me, SpellHallowedGround);
                            me->DespawnOrUnsummon(1 * IN_MILLISECONDS);
                        }
                    }

                    events.RescheduleEvent(EventCheckPlayer, 1 * IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_kaathar_mob_hallowed_groundAI(creature);
    }
};

/// Fissure Spawner Trigger - 543536
class auchindoun_kaathar_mob_spawn_fissures : public CreatureScript
{
public:
    auchindoun_kaathar_mob_spawn_fissures() : CreatureScript("auchindoun_kaathar_mob_spawn_fissures") { }

    struct auchindoun_kaathar_mob_spawn_fissuresAI : public Scripted_NoMovementAI
    {
        auchindoun_kaathar_mob_spawn_fissuresAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            m_Instance = creature->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->setFaction(HostileFaction);
            me->SetDisplayId(InvisibleDisplay);
            me->CastSpell(me, SpellSanctifiedStrikeAreaTrigger4);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_kaathar_mob_spawn_fissuresAI(creature);
    }
};

/// Post Fight Summoner to Center - 324235
class auchindoun_kaathar_mob_teleport_players : public CreatureScript
{
public:

    auchindoun_kaathar_mob_teleport_players() : CreatureScript("auchindoun_kaathar_mob_teleport_players") { }

    struct auchindoun_kaathar_mob_teleport_playersAI : public ScriptedAI
    {
        auchindoun_kaathar_mob_teleport_playersAI(Creature* creature) : ScriptedAI(creature)
        {
            m_Instance = creature->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_REMOVE_CLIENT_CONTROL | UNIT_FLAG_IMMUNE_TO_NPC);
        }

        void UpdateAI(uint32 p_Diff) override
        {
            if (m_Instance)
            {
                if (Creature* l_Kaathar = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataBossKathaar)))
                {
                    if (l_Kaathar->isDead())
                    {
                        if (Player * player = me->FindNearestPlayer(10.0f, true))
                            player->TeleportTo(1182, 1904.29f, 3185.111f, 30.799f, 3.34086f);
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_kaathar_mob_teleport_playersAI(creature);
    }
};

/// Holy Shield - 76071
class auchindoun_kaathar_mob_holy_shield : public CreatureScript
{
public:
    auchindoun_kaathar_mob_holy_shield() : CreatureScript("auchindoun_kaathar_mob_holy_shield") { }

    struct auchindoun_kaathar_mob_holy_shieldAI : public Scripted_NoMovementAI
    {
        auchindoun_kaathar_mob_holy_shieldAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            m_Instance = creature->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->setFaction(HostileFaction);
            me->SetDisplayId(InvisibleDisplay);

            /// the wall takes the orientation of the creature: it has to face Kaathar, whatever the way he threw it
            if (m_Instance)
                if (Creature* l_Kaathar = m_Instance->instance->GetCreature(m_Instance->GetGuidData(DataBossKathaar)))
                    me->SetOrientation(Position::NormalizeOrientation(me->GetAngle(l_Kaathar) + float(M_PI) / 2.0f));       /// the model of the wall spreads along its side

            me->CastSpell(me, SpellHolyShieldLos);
            me->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
            me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_REMOVE_CLIENT_CONTROL);
            /// Knockback
            std::list<Player*> l_ListPlayers;
            me->GetPlayerListInGrid(l_ListPlayers, 5.0f);
            if (!l_ListPlayers.empty())
            {
                for (Player* l_itr : l_ListPlayers)
                {
                    if (!l_itr)
                        continue;

                    l_itr->CastSpell(l_itr, SpellHolyShieldKnockBack);
                    me->CastSpell(l_itr, SpellHolyShieldDamageFromKnockBack);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new auchindoun_kaathar_mob_holy_shieldAI(creature);
    }
};

/// Consecrated Light - 153006
class auchindoun_kaathar_spell_consecrated_light : public SpellScriptLoader
{
public:

    auchindoun_kaathar_spell_consecrated_light() : SpellScriptLoader("auchindoun_kaathar_spell_consecrated_light") { }

    class auchindoun_kaathar_spell_consecrated_light_SpellScript : public AuraScript
    {
        PrepareAuraScript(auchindoun_kaathar_spell_consecrated_light_SpellScript);

        void HandlePeriodic(AuraEffect const* /*auraEffect*/)
        {
            if (GetCaster())
            {
                if (InstanceScript* m_Instance = GetCaster()->GetInstanceScript())
                {
                    std::list<Player*> l_ListPlayers;
                    GetCaster()->GetPlayerListInGrid(l_ListPlayers, 200.0f);
                    if (!l_ListPlayers.empty())
                    {
                        for (Player* itr : l_ListPlayers)
                        {
                            if (!itr)
                                continue;

                            itr->CastSpell(itr, SpellConsecratedLightDamage, true);
                        }
                    }
                }
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(auchindoun_kaathar_spell_consecrated_light_SpellScript::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new auchindoun_kaathar_spell_consecrated_light_SpellScript();
    }
};

/// Consecrated Light - 156746
class auchindoun_kaathar_spell_conscreated_damage : public SpellScriptLoader
{
public:

    auchindoun_kaathar_spell_conscreated_damage() : SpellScriptLoader("auchindoun_kaathar_spell_conscreated_damage") { }

    class auchindoun_kaathar_spell_conscreated_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_kaathar_spell_conscreated_damage_SpellScript);

        void RecalculateDamage(SpellEffIndex /*effectIndex*/)
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            /// no test of the damage here: at launch it is not computed yet, it read zero and nothing was ever checked
            if (InstanceScript* instance = GetCaster()->GetInstanceScript())
            {
                /// the wall of the shield is what protects: the invisible creature that raised it does not last as long
                if (Creature* l_Kaathar = instance->instance->GetCreature(instance->GetGuidData(DataBossKathaar)))
                    if (GameObject* l_Shield = GetHitUnit()->FindNearestGameObject(GameobjectHolyShieldWall, 200.0f))
                        if (l_Shield->IsInBetween(GetHitUnit(), l_Kaathar, 3.5f))       /// half the width of the wall
                            PreventHitDefaultEffect(EFFECT_0);
            }
        }

        void Register() override
        {
            /// the damage of this core is dealt at launch: a hook at the hit came too late
            OnEffectLaunchTarget += SpellEffectFn(auchindoun_kaathar_spell_conscreated_damage_SpellScript::RecalculateDamage, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_kaathar_spell_conscreated_damage_SpellScript();
    }
};

/// Sanctified Ground Periodic Dummy - 153430
class auchindoun_kaathar_spell_sanctified_ground : public SpellScriptLoader
{
public:

    auchindoun_kaathar_spell_sanctified_ground() : SpellScriptLoader("auchindoun_kaathar_spell_sanctified_ground") { }

    class auchindoun_kaathar_spell_sanctified_ground_AuraScript : public AuraScript
    {
        PrepareAuraScript(auchindoun_kaathar_spell_sanctified_ground_AuraScript);

        void HandlePeriodic(AuraEffect const* auraEffect)
        {
            PreventDefaultAction();

            if (GetCaster() && GetTarget())
            {
                GetTarget()->CastSpell(GetTarget(), SpellSanctifiedGroundTickDamage);
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(auchindoun_kaathar_spell_sanctified_ground_AuraScript::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new auchindoun_kaathar_spell_sanctified_ground_AuraScript();
    }
};

/// Fate - 157465
class auchindoun_kaathar_spell_fate : public SpellScriptLoader
{
public:

    auchindoun_kaathar_spell_fate() : SpellScriptLoader("auchindoun_kaathar_spell_fate") { }

    class auchindoun_kaathar_spell_fate_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_kaathar_spell_fate_SpellScript);

        void HandleDummy(SpellEffIndex effectIndex)
        {
            if (Unit* l_Caster = GetCaster())
            {
                std::list<Creature*> l_HallowedGroundCreatures;
                GetCaster()->GetCreatureListWithEntryInGrid(l_HallowedGroundCreatures, TriggerHallowedGround, 150.0f);
                if (!l_HallowedGroundCreatures.empty())
                {
                    for (Creature* itr : l_HallowedGroundCreatures)
                    {
                        if (itr->IsAIEnabled)
                            itr->GetAI()->DoAction(ActionFateHallowedGround);
                    }
                }
            }
        }

        void Register() override
        {
            OnEffectLaunch += SpellEffectFn(auchindoun_kaathar_spell_fate_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_kaathar_spell_fate_SpellScript();
    }
};

/// Holy Shield - 153000: the shield lands with the orientation of the point it was thrown at, that is of its target.
/// It is set across the line to Kaathar, so the players can hide behind it.
class auchindoun_kaathar_spell_holy_shield_landing : public SpellScriptLoader
{
public:
    auchindoun_kaathar_spell_holy_shield_landing() : SpellScriptLoader("auchindoun_kaathar_spell_holy_shield_landing") { }

    class auchindoun_kaathar_spell_holy_shield_landing_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_kaathar_spell_holy_shield_landing_SpellScript);

        void FaceAcross(SpellEffIndex /*effIndex*/)
        {
            WorldLocation* l_Dest = GetHitDest();
            Unit* l_Kaathar = GetCaster();
            if (!l_Dest || !l_Kaathar)
                return;

            l_Dest->SetOrientation(Position::NormalizeOrientation(l_Dest->GetAngle(l_Kaathar) + float(M_PI) / 2.0f));
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(auchindoun_kaathar_spell_holy_shield_landing_SpellScript::FaceAcross, EFFECT_0, SPELL_EFFECT_SUMMON);
            OnEffectHit += SpellEffectFn(auchindoun_kaathar_spell_holy_shield_landing_SpellScript::FaceAcross, EFFECT_3, SPELL_EFFECT_CREATE_AREATRIGGER);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_kaathar_spell_holy_shield_landing_SpellScript();
    }
};

void AddSC_boss_kaathar()
{
    new auchindoun_kaathar_spell_holy_shield_landing();               ///< 153000
    new boss_kaathar();                                                 ///< 75839
    new auchindoun_kaathar_mob_spawn_fissures();                        ///< 543536
    new auchindoun_kaathar_mob_hallowed_ground();                       ///< 537324
    new auchindoun_kaathar_mob_holy_shield();                           ///< 76071
    //new auchindoun_kaathar_mob_nyami();                                 ///< 77810
    //new auchindoun_kaathar_mob_teleport_players();                      ///< 3242352
    new auchindoun_kaathar_spell_consecrated_light();                   ///< 153006
    new auchindoun_kaathar_spell_fate();                                ///< 157465
    new auchindoun_kaathar_spell_sanctified_ground();                   ///< 153430
    new auchindoun_kaathar_spell_conscreated_damage();                  ///< 156746
}
