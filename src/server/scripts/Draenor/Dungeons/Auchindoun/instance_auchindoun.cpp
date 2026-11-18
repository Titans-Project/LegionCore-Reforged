////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "auchindoun.hpp"

class instance_auchindoun : public InstanceMapScript
{
public:

    instance_auchindoun() : InstanceMapScript("instance_auchindoun", 1182) { }

    struct instance_auchindoun_InstanceMapScript : InstanceScript
    {
        instance_auchindoun_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
        {
            SetHeaders(DataHeader);
            SetBossNumber(DataMaxBosses);
            m_KaatharDied = false;
            m_TuulaniSummoned = true;
        }

        InstanceScript* m_Instance = this;
        uint32 m_auiEncounter[4];
        ObjectGuid m_NyamiGuid;
        ObjectGuid m_Tuulani02Guid;
        ObjectGuid m_UniqueGuardGuid;
        ObjectGuid m_TuulaniGuid;
        ObjectGuid m_WardenGuid;
        ObjectGuid m_GromtashGuid;
        ObjectGuid m_DuragGuid;
        ObjectGuid m_GulkoshGuid;
        ObjectGuid m_ElumGuid;
        ObjectGuid m_IruunGuid;
        ObjectGuid m_JoraGuid;
        ObjectGuid m_AssainatingGuardGuid;
        ObjectGuid m_AssainatedGuardGuid;
        ObjectGuid m_KaatharGuid;
        ObjectGuid m_NyamibossGuid;
        ObjectGuid m_AzzakelGuid;
        ObjectGuid m_TeronogorGuid;
        ObjectGuid m_HolyBarrierKathaarObjectGuid;
        ObjectGuid m_CrystalKaatharGuid;
        ObjectGuid m_WindowGuid;
        ObjectGuid m_FelBarrierAzzakelObjectGuid;
        ObjectGuid m_FelPortalGuid;
        ObjectGuid m_SoulTransportStartGuid;
        ObjectGuid m_SoulTransport01Guid;
        ObjectGuid m_SoulTransport02Guid;
        ObjectGuid m_SoulTransport03Guid;
        ObjectGuid m_AuchindounCrystal;
        ObjectGuid m_TriggerBubbleMiddleNyamiGuid;
        ObjectGuid m_TriggerAzzakelFelPortalGuid;
        bool m_KaatharDied;
        bool m_TuulaniSummoned;

        uint32 m_PreKaatharCleared = 0;
        uint32 m_RevealTimer = 0;
        uint32 m_AmbianceTimer = 0;
        bool m_ExecutionStarted = false;
        bool m_ExecutionDone[2] = { false, false };
        uint32 m_ExecutionElapsed[2] = { 0, 0 };
        std::map<uint64, ObjectGuid> m_AmbianceSpawns;
        std::vector<ObjectGuid> m_AuchenaiMagi;
        bool m_SparringEnded = false;
        std::map<ObjectGuid, std::pair<uint8, int32>> m_MagiSpheres;       ///< spheres raised in the current round, time before the next

        struct MagusCircle
        {
            ObjectGuid Mark;        ///< the centre the three spheres turn round
            Position Centre;
            float FirstAngle = 0.0f;
            uint32 FirstSalvo = 0;
        };
        std::map<ObjectGuid, MagusCircle> m_MagiCircles;

        void SetData(uint32 p_Type, uint32 p_Data) override
        {
            if (p_Type == DataPreKaatharCleared)
            {
                m_PreKaatharCleared = p_Data;
                SaveToDB();
            }
        }

        uint32 GetData(uint32 p_Type) const override
        {
            return p_Type == DataPreKaatharCleared ? m_PreKaatharCleared : 0;
        }

        void WriteSaveDataMore(std::ostringstream& p_Data) override
        {
            p_Data << m_PreKaatharCleared << ' ';
        }

        void ReadSaveDataMore(std::istringstream& p_Data) override
        {
            p_Data >> m_PreKaatharCleared;
        }

        /// the second phase is rebuilt a little after the first player came back, once the grid is loaded
        void OnPlayerEnter(Player* /*p_Player*/) override
        {
            if (m_PreKaatharCleared || GetBossState(DataBossKathaar) == DONE)
                m_RevealTimer = 3000;
        }

        void Update(uint32 p_Diff) override
        {
            if (m_RevealTimer)
            {
                if (m_RevealTimer > p_Diff)
                    m_RevealTimer -= p_Diff;
                else
                {
                    m_RevealTimer = 0;
                    Map::PlayerList const& l_Players = instance->GetPlayers();
                    if (!l_Players.isEmpty())
                        if (Player* l_Player = l_Players.begin()->getSource())
                        {
                            AuchindounRevealSecondPhase(l_Player);
                            RestoreProgress(l_Player);
                        }
                }
            }

            if (m_AmbianceTimer > p_Diff)
            {
                m_AmbianceTimer -= p_Diff;
                return;
            }

            m_AmbianceTimer = 500;
            UpdateDoorWardens();

            if (!m_PreKaatharCleared && GetBossState(DataBossKathaar) != DONE)
            {
                UpdateAuchenaiMagi();
                return;
            }

            UpdateCaptivesKeeper();
            UpdateBarrierClerics();
            UpdateSparring();
            UpdateExecutions();

            if (GetBossState(DataBossAzzakael) == DONE)
                UpdateTeronogorChannels();
        }

        /// a reloaded instance starts from its spawns: what the scripts had changed since is put back
        void RestoreProgress(Player* p_Player)
        {
            bool l_KaatharDone = GetBossState(DataBossKathaar) == DONE;

            /// Tuulani opened her gate on her way to Nyami, and the barrier at the door of Kaathar fell with the crystal
            std::list<GameObject*> l_Gates;
            p_Player->GetGameObjectListWithEntryInGrid(l_Gates, GameobjectHolyGate, 1000.0f);
            AuchindounGameObjectsAnyPhase(p_Player, l_Gates, GameobjectKaatharRoomBarrier, 1000.0f);
            for (GameObject* l_Gate : l_Gates)
                l_Gate->Delete();

            if (GetBossState(DataBossNyami) == DONE)
            {
                std::list<GameObject*> l_Barriers;
                AuchindounGameObjectsAnyPhase(p_Player, l_Barriers, GameobjectHolyBarrierEntra, 1000.0f);
                AuchindounGameObjectsAnyPhase(p_Player, l_Barriers, GameobjectHolyGateSouth, 1000.0f);
                for (GameObject* l_Barrier : l_Barriers)
                    l_Barrier->Delete();

                /// the two infernals have already broken the floor of the eastern wing
                std::list<GameObject*> l_Sections;
                AuchindounGameObjectsAnyPhase(p_Player, l_Sections, GameobjectAuchindounSection, 1000.0f);
                for (GameObject* l_Section : l_Sections)
                    l_Section->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);
            }

            Creature* l_Tuulani = instance->GetCreature(m_TuulaniGuid);
            if (!l_Tuulani)
                return;

            if (l_Tuulani->IsAIEnabled)
                l_Tuulani->AI()->DoAction(ActionTuulaniSkipIntro);

            if (l_KaatharDone)
                AuchindounRevealAzzakelRoom(p_Player);

            if (GetBossState(DataBossAzzakael) == DONE)
            {
                AuchindounRevealTeronogor(p_Player);
                OpenTransporter(DataSoulTransportStart);
            }

            if (l_KaatharDone)
            {
                /// freed, she waits in the room of Kaathar, or kneels by Nyami once Nyami is dead; the way out to Talador is open
                if (GetBossState(DataBossNyami) == DONE)
                {
                    l_Tuulani->NearTeleportTo(1662.0f, 2962.0f, 35.2f, 4.71f);
                    l_Tuulani->SetStandState(UNIT_STAND_STATE_KNEEL);
                }
                else
                    l_Tuulani->NearTeleportTo(g_PositionTuulaniMovements[10].GetPositionX(), g_PositionTuulaniMovements[10].GetPositionY(), g_PositionTuulaniMovements[10].GetPositionZ(), l_Tuulani->GetOrientation());       /// freed, where she was held

                std::list<GameObject*> l_Portals;
                p_Player->GetGameObjectListWithEntryInGrid(l_Portals, GameobjectTaladorPortal, 1000.0f);
                if (l_Portals.empty())
                    l_Tuulani->SummonGameObject(GameobjectTaladorPortal, g_PositionTuulaniGobjectPortalSpawn.GetPositionX(), g_PositionTuulaniGobjectPortalSpawn.GetPositionY(), g_PositionTuulaniGobjectPortalSpawn.GetPositionZ(), g_PositionTuulaniGobjectPortalSpawn.GetOrientation(), 0, 0, 0, 0, 0);

                if (GetBossState(DataBossNyami) != DONE && !instance->GetCreature(m_NyamibossGuid))
                    l_Tuulani->SummonCreature(BossNyami, g_PositionNyamiSpawn, TEMPSUMMON_MANUAL_DESPAWN);
            }
            else
            {
                /// Kaathar woke up but stands: she is still caught in the void at the door of his room
                l_Tuulani->NearTeleportTo(g_PositionTuulaniMovements[10].GetPositionX(), g_PositionTuulaniMovements[10].GetPositionY(), g_PositionTuulaniMovements[10].GetPositionZ(), l_Tuulani->GetOrientation());
                l_Tuulani->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
                l_Tuulani->AddAura(SpellTuulaniCapturedVoidPrison, l_Tuulani);
            }
        }

        /// second phase scenery, driven by spawn: these entries also fight elsewhere with their own AI
        static bool IsAmbianceSpawn(uint64 p_Spawn)
        {
            static uint64 const l_Spawns[] = {10530116, 10530119, 10530123, 10530178, 10530179, 10530176, 10530175, 10530278, 10530281, 10530283, 10530299, 10530273, 10530277,
                10530197, 10530203, 10530207, 10530200, 10530149, 10530181, 10530165, 10530169, 10530211, 10530215,
                10530276, 10530284, 10530280, 10530282, 10530274, 10530279, 10530285, 10530222, 10530221, 10530214, 10530223};
            return std::find(std::begin(l_Spawns), std::end(l_Spawns), p_Spawn) != std::end(l_Spawns);
        }

        Creature* GetSpawn(uint64 p_Spawn) const
        {
            auto l_Itr = m_AmbianceSpawns.find(p_Spawn);
            return l_Itr != m_AmbianceSpawns.end() ? instance->GetCreature(l_Itr->second) : nullptr;
        }

        static bool IsIdle(Creature* p_Creature)
        {
            return p_Creature && p_Creature->IsAlive() && !p_Creature->isInCombat() && !p_Creature->HasUnitState(UNIT_STATE_CASTING);
        }

        /// Tuulani's beam on the points of the barrier, made endless: the wardens and clerics never stop holding it
        /// the spell only starts on the next update of the holder: the channel is made endless once it runs
        void HoldBarrier(Creature* p_Holder)
        {
            if (!p_Holder || !p_Holder->IsAlive() || p_Holder->isInCombat())
                return;

            if (Spell* l_Channel = p_Holder->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
            {
                if (l_Channel->GetSpellInfo()->Id == SpellTuulaniUnlock && l_Channel->GetRemainingCastTime() >= 0)
                    l_Channel->SendChannelStart(uint32(-1));
                return;
            }

            if (p_Holder->HasUnitState(UNIT_STATE_CASTING))
                return;

            p_Holder->CastSpell(p_Holder, SpellTuulaniUnlock, false);
        }

        /// a cosmetic channel kept for good on its target, cast again whenever it stopped
        void HoldChannel(Creature* p_Holder, Unit* p_Target, uint32 p_SpellId)
        {
            /// not before the scene of his arrival has shown them
            if (!p_Holder || !p_Holder->IsAlive() || p_Holder->isInCombat() || !(p_Holder->GetPhaseMask() & 1) || !p_Target || !p_Target->IsAlive())
                return;

            if (Spell* l_Channel = p_Holder->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
            {
                if (l_Channel->GetSpellInfo()->Id == p_SpellId && l_Channel->GetRemainingCastTime() >= 0)
                    l_Channel->SendChannelStart(uint32(-1));
                return;
            }

            if (p_Holder->HasUnitState(UNIT_STATE_CASTING))
                return;

            p_Holder->CastSpell(p_Target, p_SpellId, false);
        }

        /// the three warlocks of the platforms feed the shield of Teron'gor with their beam, Teron'gor drains the high soulpriest Durem;
        /// when the three are dead nothing feeds the shield any more
        void UpdateTeronogorChannels()
        {
            static uint32 const l_SpellTeronogorShield = 157017;

            Creature* l_Teronogor = instance->GetCreature(m_TeronogorGuid);
            if (!l_Teronogor || !l_Teronogor->IsAlive())
                return;

            bool l_Fed = false;
            for (ObjectGuid const& l_Guid : { m_DuragGuid, m_GulkoshGuid, m_GromtashGuid })
            {
                Creature* l_Warlock = instance->GetCreature(l_Guid);
                if (!l_Warlock || !l_Warlock->IsAlive())
                    continue;

                l_Fed = true;
                HoldChannel(l_Warlock, l_Teronogor, SpellCosmeticShadowChannel);
            }

            if (!l_Fed && l_Teronogor->HasAura(l_SpellTeronogorShield))
                l_Teronogor->RemoveAura(l_SpellTeronogorShield);

            HoldChannel(l_Teronogor, instance->GetCreature(m_ElumGuid), SpellShadowBeam);
        }

        /// the three wardens of the eastern door keep it shut with their light, each one on the points of his own third of the barrier
        void UpdateDoorWardens()
        {
            static uint64 const l_WardenSpawns[3] = {10530123, 10530116, 10530119};

            Creature* l_Wardens[3];
            for (uint8 l_I = 0; l_I < 3; ++l_I)
                l_Wardens[l_I] = GetSpawn(l_WardenSpawns[l_I]);

            for (uint8 l_I = 0; l_I < 3; ++l_I)
            {
                HoldBarrier(l_Wardens[l_I]);
            }
        }

        /// before the betrayal the Auchenai magi practise: they send three arcane salvos into the air, left, ahead and right,
        /// each one makes a sphere appear up there, and the spheres implode in the order they came
        void UpdateAuchenaiMagi()
        {
            static int32 const l_VisualArcaneSalvo = 37447;        ///< the visual of the salvo (157653), whose own target is a creature nobody defined
            static uint32 const l_SpellArcaneSphere = 154174;      ///< the sphere, created at the point it is given

            /// in the corridor of the entrance only one magus practises, in the central hall only one too
            static uint64 const l_Still[] = {10530032, 10530228, 10530229, 10530209, 10530057, 10530070, 10530073};

            for (ObjectGuid const& l_Guid : m_AuchenaiMagi)
            {
                Creature* l_Magus = instance->GetCreature(l_Guid);
                if (!IsIdle(l_Magus) || std::find(std::begin(l_Still), std::end(l_Still), l_Magus->GetDBTableGUIDLow()) != std::end(l_Still))
                    continue;

                /// no sphere before someone is there to see it cast
                bool l_Watched = false;
                Map::PlayerList const& l_Players = instance->GetPlayers();
                for (Map::PlayerList::const_iterator l_Itr = l_Players.begin(); l_Itr != l_Players.end(); ++l_Itr)
                    if (Player* l_Player = l_Itr->getSource())
                        if (l_Player->IsInWorld() && !l_Player->IsBeingTeleported() && l_Player->GetExactDist2d(l_Magus) <= 60.0f)
                            l_Watched = true;

                if (!l_Watched)
                    continue;

                std::pair<uint8, int32>& l_Round = m_MagiSpheres[l_Guid];
                if (l_Round.second > 500)
                {
                    l_Round.second -= 500;
                    continue;
                }

                if (l_Round.first == 3)
                {
                    l_Round.first = 0;
                    l_Round.second = urand(6000, 12000);
                    continue;
                }

                /// the three spheres turn on one circle above him, a third of a turn apart: each one appears where it keeps
                /// that gap with the first, which has moved on since (clockwise, at the speed of the areatrigger 1569)
                static float const l_CircleRadius = 4.0f;
                static float const l_AngularSpeed = 2.98639f / l_CircleRadius;
                MagusCircle& l_Circle = m_MagiCircles[l_Guid];
                if (l_Round.first == 0)
                {
                    l_Circle.Mark = ObjectGuid::Empty;
                    l_Circle.Centre = Position(l_Magus->GetPositionX(), l_Magus->GetPositionY(), l_Magus->GetPositionZ() + 8.0f);
                    l_Circle.FirstAngle = l_Magus->GetOrientation();
                    l_Circle.FirstSalvo = getMSTime();
                }

                float l_Angle = l_Circle.FirstAngle - l_AngularSpeed * float(getMSTimeDiff(l_Circle.FirstSalvo, getMSTime())) / 1000.0f + float(l_Round.first) * 2.0f * float(M_PI) / 3.0f;

                /// the salvo flies from his hands to its point in the air, and the sphere appears there when it arrives
                Position l_Hands(l_Magus->GetPositionX(), l_Magus->GetPositionY(), l_Magus->GetPositionZ() + 2.0f);
                Position l_Sky(l_Circle.Centre.GetPositionX() + l_CircleRadius * std::cos(l_Angle), l_Circle.Centre.GetPositionY() + l_CircleRadius * std::sin(l_Angle), l_Circle.Centre.GetPositionZ());

                /// it does not fly straight: a detour by a point drawn aside of the way
                float l_Side = l_Angle + float(M_PI) / 2.0f;
                float l_Swing = frand(-3.0f, 3.0f);
                Position l_Detour((l_Hands.GetPositionX() + l_Sky.GetPositionX()) / 2.0f + l_Swing * std::cos(l_Side),
                    (l_Hands.GetPositionY() + l_Sky.GetPositionY()) / 2.0f + l_Swing * std::sin(l_Side),
                    (l_Hands.GetPositionZ() + l_Sky.GetPositionZ()) / 2.0f + frand(-1.0f, 2.0f));

                l_Magus->HandleEmoteCommand(EMOTE_ONESHOT_SPELL_CAST_OMNI);
                l_Magus->PlayOrphanSpellVisual(l_Hands, Position(), l_Detour, l_VisualArcaneSalvo, 0.5f, ObjectGuid::Empty, true);
                l_Magus->AddDelayedEvent(500, [l_Magus, l_Detour, l_Sky]() -> void
                {
                    l_Magus->PlayOrphanSpellVisual(l_Detour, Position(), l_Sky, l_VisualArcaneSalvo, 0.5f, ObjectGuid::Empty, true);
                });
                /// the circle of a sphere turns round its caster: a mark left at the centre, in the air, casts the three of them
                l_Magus->AddDelayedEvent(1 * IN_MILLISECONDS, [this, l_Magus, l_Guid, l_Sky]() -> void
                {
                    MagusCircle& l_Circle = m_MagiCircles[l_Guid];
                    Creature* l_Mark = l_Circle.Mark.IsEmpty() ? nullptr : ObjectAccessor::GetCreature(*l_Magus, l_Circle.Mark);
                    if (!l_Mark)
                    {
                        l_Mark = l_Magus->SummonCreature(CreatureLightWallTargets, l_Circle.Centre, TEMPSUMMON_TIMED_DESPAWN, 20 * IN_MILLISECONDS);
                        if (!l_Mark)
                            return;

                        l_Circle.Mark = l_Mark->GetGUID();
                    }

                    l_Mark->CastSpell(l_Sky, l_SpellArcaneSphere, true);
                });

                ++l_Round.first;
                l_Round.second = l_Round.first == 3 ? 8000 : 2000;      /// after the third, the time for the three to implode
            }
        }

        /// the Sargerei soulpriest beside the three captives of the corridor channels the void, on no one in particular
        void UpdateCaptivesKeeper()
        {
            Creature* l_Keeper = GetSpawn(10530178);
            if (IsIdle(l_Keeper))
                l_Keeper->CastSpell(l_Keeper, SpellVoidChannelingCosmetic, false);
        }

        /// the two clerics of the western barrier, shut again in the second phase, hold it with their light
        void UpdateBarrierClerics()
        {
            for (uint64 l_Spawn : {10530211, 10530215})
            {
                Creature* l_Cleric = GetSpawn(l_Spawn);
                if (!l_Cleric || !l_Cleric->IsAlive() || l_Cleric->isInCombat())
                    continue;

                /// Nyami is dead: the clerics let the entrance go and stand guard before the northern barrier, ready to fight
                if (GetBossState(DataBossNyami) == DONE)
                {
                    if (l_Cleric->GetPositionX() < 1625.0f && !l_Cleric->isMoving())
                    {
                        l_Cleric->InterruptNonMeleeSpells(false);
                        l_Cleric->GetMotionMaster()->MovePoint(0, 1690.0f, l_Cleric->GetPositionY(), 35.2f);
                    }
                    else if (l_Cleric->GetPositionX() >= 1685.0f && l_Cleric->GetUInt32Value(UNIT_FIELD_EMOTE_STATE) != EMOTE_STATE_READY1H)
                        l_Cleric->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_READY1H);
                    continue;
                }

                HoldBarrier(l_Cleric);
            }
        }

        /// behind the eastern door and in the corridor of the entrance the Sargerei and the loyal Auchenai fight, without blood: one side would soon be dead
        void UpdateSparring()
        {
            static uint64 const l_Sargerei[] = {10530278, 10530281, 10530283, 10530299, 10530273, 10530277,
                10530276, 10530284, 10530280, 10530282, 10530274, 10530279, 10530285};
            static uint64 const l_Auchenai[] = {10530197, 10530203, 10530207, 10530200, 10530222, 10530221, 10530214, 10530223};

            /// Nyami is dead and the barriers are down: the loyal ones win, the traitors fall and the Auchenai go back to their posts
            if (GetBossState(DataBossNyami) == DONE)
            {
                if (m_SparringEnded)
                    return;

                m_SparringEnded = true;
                /// they really fight each other, their factions are enemies: the traitors fall even in the middle of it
                for (uint64 l_Spawn : l_Sargerei)
                    if (Creature* l_Traitor = GetSpawn(l_Spawn))
                        if (l_Traitor->IsAlive())
                            l_Traitor->Kill(l_Traitor);

                for (uint64 l_Spawn : l_Auchenai)
                {
                    Creature* l_Loyal = GetSpawn(l_Spawn);
                    if (!l_Loyal || !l_Loyal->IsAlive())
                        continue;

                    l_Loyal->CombatStop(true);
                    l_Loyal->DeleteThreatList();
                    l_Loyal->GetMotionMaster()->MovePoint(0, l_Loyal->GetHomePosition());
                }
                return;
            }

            for (uint64 l_SargereiSpawn : l_Sargerei)
            {
                Creature* l_Traitor = GetSpawn(l_SargereiSpawn);
                if (!IsIdle(l_Traitor) || urand(0, 2))
                    continue;

                Creature* l_Foe = nullptr;
                float l_Best = 10.0f;
                for (uint64 l_AuchenaiSpawn : l_Auchenai)
                {
                    Creature* l_Loyal = GetSpawn(l_AuchenaiSpawn);
                    if (!IsIdle(l_Loyal) || l_Traitor->GetDistance(l_Loyal) >= l_Best)
                        continue;

                    l_Best = l_Traitor->GetDistance(l_Loyal);
                    l_Foe = l_Loyal;
                }

                if (!l_Foe)
                    continue;

                l_Traitor->SetFacingToObject(l_Foe);
                l_Foe->SetFacingToObject(l_Traitor);
                l_Traitor->HandleEmoteCommand(l_Traitor->GetEntry() == 87074 ? EMOTE_ONESHOT_SPELL_CAST_OMNI : EMOTE_ONESHOT_ATTACK1H);
                l_Foe->HandleEmoteCommand(l_Foe->GetEntry() == CreatureAucheniMagus ? EMOTE_ONESHOT_SPELL_CAST_OMNI : EMOTE_ONESHOT_PARRY1H);
            }
        }

        /// near the room of Kaathar, a Sargerei defender and arbiter come from behind the Auchenai defender and zealot beside them and strike them down: the traitors are shown
        void UpdateExecutions()
        {
            static uint64 const l_Victims[2] = {10530149, 10530181};
            static uint64 const l_Executioners[2] = {10530165, 10530169};

            if (!m_ExecutionStarted)
            {
                bool l_Near = false;
                Map::PlayerList const& l_Players = instance->GetPlayers();
                for (Map::PlayerList::const_iterator l_Itr = l_Players.begin(); l_Itr != l_Players.end(); ++l_Itr)
                    if (Player* l_Player = l_Itr->getSource())
                        if (l_Player->IsInWorld() && !l_Player->IsBeingTeleported() && l_Player->GetExactDist2d(1775.0f, 3166.0f) <= 50.0f)
                            l_Near = true;

                if (!l_Near)
                    return;

                m_ExecutionStarted = true;
                for (uint8 l_I = 0; l_I < 2; ++l_I)
                {
                    Creature* l_Victim = GetSpawn(l_Victims[l_I]);
                    Creature* l_Executioner = GetSpawn(l_Executioners[l_I]);
                    if (!l_Victim || !l_Victim->IsAlive() || !IsIdle(l_Executioner))
                        continue;

                    float l_Angle = l_Victim->GetOrientation() + float(M_PI);       /// from behind
                    l_Executioner->GetMotionMaster()->MovePoint(0, l_Victim->GetPositionX() + 1.5f * std::cos(l_Angle), l_Victim->GetPositionY() + 1.5f * std::sin(l_Angle), l_Victim->GetPositionZ());
                }

                return;
            }

            for (uint8 l_I = 0; l_I < 2; ++l_I)
            {
                if (m_ExecutionDone[l_I])
                    continue;

                Creature* l_Victim = GetSpawn(l_Victims[l_I]);
                Creature* l_Executioner = GetSpawn(l_Executioners[l_I]);
                if (!l_Victim || !l_Victim->IsAlive() || !l_Executioner || !l_Executioner->IsAlive())
                {
                    m_ExecutionDone[l_I] = true;
                    continue;
                }

                m_ExecutionElapsed[l_I] += 1 * IN_MILLISECONDS;
                if (l_Executioner->GetExactDist2d(l_Victim) > 3.0f && m_ExecutionElapsed[l_I] < 8 * IN_MILLISECONDS && !l_Executioner->isInCombat())
                    continue;

                m_ExecutionDone[l_I] = true;
                l_Executioner->SetFacingToObject(l_Victim);
                l_Executioner->HandleEmoteCommand(EMOTE_ONESHOT_ATTACK1H);
                l_Victim->CastSpell(l_Victim, SpellBloodSplatter, true);
                l_Executioner->Kill(l_Victim);

                if (!l_Executioner->isInCombat())
                    l_Executioner->GetMotionMaster()->MovePoint(0, l_Executioner->GetHomePosition());
            }
        }

        /// none of the game objects of the dungeon was ever registered: barriers, portal, window and soul transporters did nothing
        void OnGameObjectCreate(GameObject* p_GameObject) override
        {
            switch (p_GameObject->GetEntry())
            {
                case 230399: m_HolyBarrierKathaarObjectGuid = p_GameObject->GetGUID(); break;
                case GameobjectFelBarrier: m_FelBarrierAzzakelObjectGuid = p_GameObject->GetGUID(); break;
                case GameobjectDemonicPortal: m_FelPortalGuid = p_GameObject->GetGUID(); break;
                case GameobjectAuchindounWindow: m_WindowGuid = p_GameObject->GetGUID(); break;
                case GameobjectSoulTransportStart: m_SoulTransportStartGuid = p_GameObject->GetGUID(); break;
                case GameobjectSoulTransport1: m_SoulTransport01Guid = p_GameObject->GetGUID(); break;
                case GameobjectSoulTransport2: m_SoulTransport02Guid = p_GameObject->GetGUID(); break;
                case GameobjectSoulTransport3: m_SoulTransport03Guid = p_GameObject->GetGUID(); break;
                default: break;
            }
        }

        /// a soul transporter comes to life once the one guarding it is dead
        void OpenTransporter(uint32 p_Data)
        {
            if (GameObject* l_Transporter = instance->GetGameObject(GetGuidData(p_Data)))
            {
                l_Transporter->SetGoState(GO_STATE_READY);
                l_Transporter->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
            }
        }

        void OnCreatureCreate(Creature* creature) override
        {
            if (creature && IsAmbianceSpawn(creature->GetDBTableGUIDLow()))
                m_AmbianceSpawns[creature->GetDBTableGUIDLow()] = creature->GetGUID();

            if (creature && (creature->GetEntry() == CreatureAucheniMagus2 || creature->GetEntry() == CreatureAucheniMagus))
                m_AuchenaiMagi.push_back(creature->GetGUID());

            if (creature)
            {
                switch (creature->GetEntry())
                {
                    case CreatureSoulBinderTuulani01:
                        m_Tuulani02Guid = creature->GetGUID();
                        break;
                    case CreatureIruun:
                        m_IruunGuid = creature->GetGUID();
                        break;
                    case CreatureJoraa:
                        m_JoraGuid = creature->GetGUID();
                        break;
                    case CreatureDurem:
                        m_ElumGuid = creature->GetGUID();
                        break;
                    case CreatureAuchenaiDefenderUnique:
                        /// of the sixteen sniffed guards, only this one speaks to Tuulani
                        if (creature->GetDBTableGUIDLow() == 10530210)
                            m_UniqueGuardGuid = creature->GetGUID();
                        break;
                    case BossKaathar:
                        m_KaatharGuid = creature->GetGUID();
                        break;
                    case BossNyami:
                        m_NyamibossGuid = creature->GetGUID();
                        /// the captive warden floats before the door still shut, the one towards Azzakel
                        /// summoned by the map: what Nyami summons herself is sent away at her death, and he is the one who speaks then
                        instance->SummonCreature(CreatureWardenAzzakael, Position(1665.0f, 2904.0f, 44.0f, 1.57f));
                        break;
                    case BossAzaakel:
                        m_AzzakelGuid = creature->GetGUID();
                        break;
                    case BossTeronogor:
                        m_TeronogorGuid = creature->GetGUID();
                        break;
                    case CreatureSoulBinderTuulani:
                        m_TuulaniGuid = creature->GetGUID();
                        break;
                    case CreatureSoulBinderNyami:
                        m_NyamiGuid = creature->GetGUID();
                        break;
                    case CreatureShieldSpot:
                        m_TriggerBubbleMiddleNyamiGuid = creature->GetGUID();
                        break;
                    case CreatureWardenAzzakael:
                        m_WardenGuid = creature->GetGUID();
                        break;
                    case CreatureDemonsSummoner:
                        m_TriggerAzzakelFelPortalGuid = creature->GetGUID();
                        break;
                    case CreatureGulkosh:
                        m_GulkoshGuid = creature->GetGUID();
                        break;
                    case CreatureGromtashTheDestructor:
                        m_GromtashGuid = creature->GetGUID();
                        break;
                    case CreatureDuragTheDominator:
                        m_DuragGuid = creature->GetGUID();
                        break;
                    case CreatureSargereiAssasinating:
                        m_AssainatingGuardGuid = creature->GetGUID();
                        break;
                    case CreatureAuchenaiAssainated:
                        m_AssainatedGuardGuid = creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }
        }

        void OnUnitDeath(Unit* unit) override
        {
            if (!m_Instance)
                return;

            Creature* creature = unit->ToCreature();
            if (!creature)
                return;

            /// the Sargerei soulpriest held the three captives of the corridor: their shells break, they fall and die
            if (creature->GetDBTableGUIDLow() == 10530178)
            {
                for (uint64 l_Spawn : {10530179, 10530176, 10530175})
                {
                    Creature* l_Captive = GetSpawn(l_Spawn);
                    if (!l_Captive || !l_Captive->IsAlive())
                        continue;

                    l_Captive->RemoveAllAuras();
                    l_Captive->SetDisableGravity(false);
                    l_Captive->GetMotionMaster()->MoveFall();
                    l_Captive->AddDelayedEvent(1500, [l_Captive]() -> void
                    {
                        l_Captive->Kill(l_Captive);
                    });
                }
            }

            switch (creature->GetEntry())
            {
                case BossKaathar:
                {
                    if (GameObject* l_Holybarrier = instance->GetGameObject(GetGuidData(DataHolyBarrier)))
                    {
                        l_Holybarrier->Delete();
                        m_KaatharDied = true;
                    }
                    break;
                }
                case BossAzaakel:
                {
                    if (GameObject* l_Felbarrier = instance->GetGameObject(GetGuidData(DataFelBarrier)))
                        l_Felbarrier->Delete();

                    OpenTransporter(DataSoulTransportStart);

                    if (instance->GetGameObject(GetGuidData(DataSoulTransportStart)))
                    {
                        if (Creature* l_Teronogor = instance->GetCreature(GetGuidData(DataBossTeronogor)))
                        {
                            if (l_Teronogor->IsAIEnabled)
                                l_Teronogor->GetAI()->DoAction(ActionSoulMove1);
                        }
                    }
                    break;
                }
                /// Soul Transport
                case CreatureGromtashTheDestructor:
                    OpenTransporter(DataSoulTransport3);
                    if (instance->GetGameObject(GetGuidData(DataSoulTransport3)))
                    {
                        if (Creature* l_Teronogor = instance->GetCreature(GetGuidData(DataBossTeronogor)))
                        {
                            if (l_Teronogor->IsAIEnabled)
                                l_Teronogor->GetAI()->DoAction(ActionSoulMove4);
                        }
                    }
                    break;
                case CreatureGulkosh:
                    OpenTransporter(DataSoulTransport2);
                    if (instance->GetGameObject(GetGuidData(DataSoulTransport2)))
                    {
                        if (Creature* l_Teronogor = instance->GetCreature(GetGuidData(DataBossTeronogor)))
                        {
                            if (l_Teronogor->IsAIEnabled)
                                l_Teronogor->GetAI()->DoAction(ActionSoulMove3);
                        }
                    }
                    break;
                case CreatureDuragTheDominator:
                {
                    OpenTransporter(DataSoulTransport1);
                    if (instance != nullptr)
                    {
                        if (instance->GetGameObject(GetGuidData(DataSoulTransport1)))
                        {
                            if (Creature* l_Teronogor = instance->GetCreature(GetGuidData(DataBossTeronogor)))
                            {
                                if (l_Teronogor->IsAIEnabled)
                                    l_Teronogor->GetAI()->DoAction(ActionSoulMove2);
                            }
                        }
                    }
                    break;
                }
                case BossTeronogor:
                {
                    if (creature->GetMap() && creature->GetMap()->IsHeroic())
                        DoCompleteAchievement(AchievementAuchindounHeroic);
                    else
                        DoCompleteAchievement(AchievementAuchindounNormal);

                    /// Curtain flames achievement, No Tags Backs! (9552)
                    UnitList targets;
                    //Trinity::AnyUnitHavingBuffInObjectRangeCheck u_check(creature, creature, 100, 153392, true);
                    //Trinity::UnitListSearcher<Trinity::AnyUnitHavingBuffInObjectRangeCheck> searcher(creature, targets, u_check);
                    //creature->VisitNearbyObject(100, searcher);

                    if (!targets.empty())
                        DoCompleteAchievement(AchievementNoTagBacks);
                    break;
                }
                default:
                    break;
            }
        }

        ObjectGuid GetGuidData(uint32 p_Data) const override
        {
            switch (p_Data)
            {
                case DataCrystal:
                    return m_AuchindounCrystal;
                case DataTuulani02:
                    return m_Tuulani02Guid;
                case DataGuard:
                    return m_UniqueGuardGuid;
                case DataElum:
                    return m_ElumGuid;
                case DataIruun:
                    return m_IruunGuid;
                case DataJorra:
                    return m_JoraGuid;
                case DataSoulTransportStart:
                    return m_SoulTransportStartGuid;
                case DataSoulTransport1:
                    return m_SoulTransport01Guid;
                case DataSoulTransport2:
                    return m_SoulTransport02Guid;
                case DataSoulTransport3:
                    return m_SoulTransport03Guid;
                case DataHolyBarrier:
                    return m_HolyBarrierKathaarObjectGuid;
                case DataAuchindounWindow:
                    return m_WindowGuid;
                case DataFelBarrier:
                    return m_FelBarrierAzzakelObjectGuid;
                case DataFelPortal:
                    return m_FelPortalGuid;
                case DataBossKathaar:
                    return m_KaatharGuid;
                case DataBossAzzakael:
                    return m_AzzakelGuid;
                case DataBossNyami:
                    return m_NyamibossGuid;
                case DataBossTeronogor:
                    return m_TeronogorGuid;
                case DataNyami:
                    return m_NyamiGuid;
                case DataTuulani:
                    return m_TuulaniGuid;
                case DataWarden:
                    return m_WardenGuid;
                case DataGulkosh:
                    return m_GulkoshGuid;
                case DataGromtash:
                    return m_GromtashGuid;
                case DataDurag:
                    return m_DuragGuid;
                case DataTriggerMiddleNyamiFightBubble:
                    return m_TriggerBubbleMiddleNyamiGuid;
                case DataTriggerAzzakelController:
                    return m_TriggerAzzakelFelPortalGuid;
                case DataAssinatingGuard:
                    return m_AssainatingGuardGuid;
                case DataAssinatedGuard:
                    return m_AssainatedGuardGuid;
                default:
                    break;
            }

            return ObjectGuid::Empty;
        }
    };

    InstanceScript* GetInstanceScript(InstanceMap* map) const override
    {
        return new instance_auchindoun_InstanceMapScript(map);
    }
};

void AddSC_instance_auchindoun()
{
    new instance_auchindoun;
}

