////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "battle_for_shattrath.h"

using namespace BattleForShattrath;

class instance_battle_for_shattrath : public InstanceMapScript
{
public:
    instance_battle_for_shattrath() : InstanceMapScript("instance_battle_for_shattrath", MAP_SHATTRATH_SCENARIO) { }

    struct instance_battle_for_shattrath_InstanceMapScript : public InstanceScript
    {
        instance_battle_for_shattrath_InstanceMapScript(InstanceMap* map) : InstanceScript(map) { }

        ObjectGuid _khadgarGUID, _yrelGUID, _durotanGUID, _drakaGUID, _maraadGUID, _orgrimGUID, _blackhandGUID, _brandtGUID;
        ObjectGuid _ironStarGUID, _riderGUID, _crashStarGUID;
        GuidList _waveGUIDs;
        GuidList _frontGUIDs;
        GuidList _shipGuards;
        GuidList _turretGUIDs;
        GuidList _turretGOs;
        uint8 _nextTurret = 0;
        uint8 _nextCatch = 0;
        bool _volleysOver = false;
        uint32 _crystalTimer = 4500;
        float _catchBase = 0.0f;
        bool _catchBaseSet = false;
        uint32 _volleyTimer = 3 * IN_MILLISECONDS;
        ObjectGuid _cannonGUID, _crystalGUID;
        EventMap _events;
        uint32 _crushTimer = 0;
        uint32 _rideTimer = 0;
        bool _rolling = false;
        uint8 _launchTries = 0;
        uint32 _launchCheck = 0;
        Position _launchFrom;
        bool _riderless = false;
        bool _casting = false;
        bool _khadgarJoining = false;
        bool _khadgarJoined = false;
        uint32 _checkTimer = IN_MILLISECONDS;
        uint32 _channelTimer = 0;
        uint32 _waveTimer = 0;
        uint8 _lastStep = STEP_PREPARATIONS;
        bool _channeling = false;
        bool _heroesAtPier = false;
        bool _heroesWalking = false;
        bool _confronted = false;
        bool _confrontDone = false;
        bool _brandtGreeted = false;
        bool _flagshipStarted = false;
        bool _completed = false;
        bool _awaitingFilm = false;
        uint32 _filmTimer = 0;
        bool _restoreChecked = false;

        void OnCreatureCreate(Creature* creature) override
        {
            InstanceScript::OnCreatureCreate(creature);

            switch (creature->GetEntry())
            {
                case NPC_KHADGAR:   _khadgarGUID = creature->GetGUID(); break;
                case NPC_YREL:      _yrelGUID = creature->GetGUID(); break;
                case NPC_DUROTAN:   _durotanGUID = creature->GetGUID(); break;
                case NPC_DRAKA:     _drakaGUID = creature->GetGUID(); break;
                case NPC_MARAAD:    _maraadGUID = creature->GetGUID(); break;
                case NPC_ORGRIM:    _orgrimGUID = creature->GetGUID(); break;
                case NPC_BLACKHAND: _blackhandGUID = creature->GetGUID(); break;
                case NPC_IRON_STAR_RIDE: _ironStarGUID = creature->GetGUID(); break;
                case NPC_MACHINIST_BRANDT:
                    _brandtGUID = creature->GetGUID();
                    // on his cannon, out of reach until his men have fallen, and he does not come down for anyone before
                    creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
                    creature->SetReactState(REACT_PASSIVE);
                    break;
                default:
                    break;
            }

            if (creature->GetEntry() == NPC_BIG_CANNON)
                _cannonGUID = creature->GetGUID();
            if (creature->GetEntry() == NPC_AIR_DEFENSE_CRYSTAL)
            {
                // it hangs high above the docks
                _crystalGUID = creature->GetGUID();
                creature->SetCanFly(true);
                creature->SetDisableGravity(true);
                creature->setActive(true);
            }
            if (creature->GetEntry() == NPC_TURRET_MUZZLE || creature->GetEntry() == NPC_BIG_CANNON)
                creature->SetReactState(REACT_PASSIVE);
            // the Iron Stars standing on the ships are only there to be seen, and to shelter from Blackhand
            if (creature->GetEntry() == NPC_IRON_STAR_SHELTER)
            {
                creature->SetReactState(REACT_PASSIVE);
                creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            }
            if (creature->GetEntry() == NPC_TURRET_MUZZLE)
            {
                _turretGUIDs.push_back(creature->GetGUID());
                creature->setActive(true);
            }
            if ((creature->GetEntry() == NPC_GROMKAR_VANGUARD || creature->GetEntry() == NPC_GROMKAR_GRENADIER) && creature->GetPositionX() > 3850.0f)
                _shipGuards.push_back(creature->GetGUID());

            // Blackhand's forces already standing at the docks wait in line for the heroes
            if (IsWave(creature->GetEntry()) && creature->GetDBTableGUIDLow())
            {
                _frontGUIDs.push_back(creature->GetGUID());
                creature->SetReactState(REACT_PASSIVE);
            }

            // the defeated stay down: nothing of the Iron Horde comes back during a run
            if (IsIronHorde(creature->GetEntry()) || creature->GetEntry() == NPC_MACHINIST_BRANDT)
                creature->SetRespawnDelay(DAY);
        }

        void OnGameObjectCreate(GameObject* go) override
        {
            InstanceScript::OnGameObjectCreate(go);
            if (go->GetEntry() == GO_SHIP_TURRET)
            {
                _turretGOs.push_back(go->GetGUID());
                go->setActive(true);
            }
        }

        void OnPlayerEnter(Player* player) override
        {
            InstanceScript::OnPlayerEnter(player);

            // the ships lie far out: their grids are loaded so their turrets fire from the start
            for (Position const& top : ShipTops)
                instance->LoadGrid(top.GetPositionX(), top.GetPositionY());

            // logging back in after a server restart: the instance is new and nothing built its scenario, it starts here
            if (!GetScenario(instance) && HasBattleQuest(player))
                if (Scenario* scenario = sScenarioMgr->AddScenario(instance, SCENARIO_SHATTRATH))
                {
                    scenario->SetCompleteThroughBonusSteps(false);
                    scenario->SendStepUpdate(player, true);
                }
        }

        Creature* Get(ObjectGuid const& guid) { return instance->GetCreature(guid); }

        void Say(ObjectGuid const& guid, uint8 group, ObjectGuid target = ObjectGuid::Empty)
        {
            if (Creature* creature = Get(guid))
                creature->AI()->Talk(group, target);
        }

        Player* GetReferencePlayer() const
        {
            Map::PlayerList const& players = instance->GetPlayers();
            return players.isEmpty() ? nullptr : players.begin()->getSource();
        }

        void SendScenarioEvent(uint32 eventId)
        {
            Scenario* scenario = GetScenario(instance);
            Player* reference = GetReferencePlayer();
            if (scenario && reference)
                scenario->UpdateAchievementCriteria(CRITERIA_TYPE_SCRIPT_EVENT_2, eventId, 0, 0, nullptr, reference);
        }

        void SetData(uint32 type, uint32 /*data*/) override
        {
            switch (type)
            {
                case DATA_TALKED_TO_THAELIN:   SendScenarioEvent(EVENT_TALK_TO_THAELIN); break;
                case DATA_ENTERED_IRON_STAR:   SendScenarioEvent(EVENT_ENTER_IRON_STAR); break;
                case DATA_RIDE_ARRIVED:        SendScenarioEvent(EVENT_REACH_HARBOR); break;
                case DATA_TALKED_TO_KHADGAR:   SendScenarioEvent(EVENT_TALK_TO_KHADGAR); break;
                case DATA_TELEPORT_BATTLESHIP: SendScenarioEvent(EVENT_TELEPORT_BATTLESHIP); break;
                case DATA_TELEPORT_FLAGSHIP:
                    SendScenarioEvent(EVENT_TELEPORT_FLAGSHIP);
                    break;
                case DATA_BLACKHAND_DONE:      SendScenarioEvent(EVENT_SURVIVE_BLACKHAND); break;
                default:
                    break;
            }
        }

        ObjectGuid GetGuidData(uint32 type) const override
        {
            return type == NPC_BLACKHAND ? _blackhandGUID : ObjectGuid::Empty;
        }

        uint32 GetData(uint32 type) const override
        {
            // Khadgar offers the battleship once he has caught up with the hero at the pier
            return type == DATA_CONFRONTED ? uint32(_khadgarJoined) : 0;
        }

        void OnUnitDeath(Unit* unit) override
        {
            switch (unit->GetEntry())
            {
                case NPC_MACHINIST_BRANDT:
                    SendScenarioEvent(EVENT_BRANDT_DEFEATED);
                    break;
                case NPC_GROMKAR_VANGUARD:
                case NPC_GROMKAR_GRENADIER:
                    // the last of his men down on the battleship: B'randt jumps from his cannon onto the deck
                    if (unit->GetPositionX() > 3850.0f)
                    {
                        for (ObjectGuid const& guid : _shipGuards)
                            if (Creature* guard = Get(guid))
                                if (guard->IsAlive() && guard != unit)
                                    return;
                        BrandtJumpsDown();
                    }
                    break;
                default:
                    break;
            }
        }

        void BrandtJumpsDown()
        {
            Creature* rider = Get(_brandtGUID);
            if (!rider || !rider->IsAlive() || !rider->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC))
                return;

            // the B'randt on the cannon is its accessory, and the core unsummons an accessory that leaves its seat:
            // a B'randt of his own takes his place and makes the leap down onto the deck
            Position const from = rider->GetPosition();
            TempSummon* brandt = rider->SummonCreature(NPC_MACHINIST_BRANDT, from, TEMPSUMMON_CORPSE_TIMED_DESPAWN, MINUTE * IN_MILLISECONDS);
            if (!brandt)
                return;
            rider->DespawnOrUnsummon();
            brandt->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            brandt->SetHomePosition(BrandtLanding);
            brandt->AddDelayedEvent(300, [brandt]() -> void
            {
                brandt->GetMotionMaster()->MoveJump(BrandtLanding, 14.0f, 8.0f);
            });
            brandt->AddDelayedEvent(1800, [brandt]() -> void
            {
                brandt->AI()->Talk(SAY_BRANDT_END_THIS);
                brandt->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                brandt->SetReactState(REACT_AGGRESSIVE);
                if (Player* player = brandt->SelectNearestPlayer(60.0f))
                    brandt->AI()->AttackStart(player);
            });
        }

        // the scenario resumed at the step saved for the player (Scenario::RestoreSavedStep): the battlefield is set to match
        void RestoreStep(Scenario* scenario)
        {
            _restoreChecked = true;
            uint8 step = scenario->GetCurrentStep();
            if (!step || step > STEP_BLACKHAND)
                return;

            // the harbor is already cleared: the heroes wait at the flagship's pier
            if (step >= STEP_THE_WEAPON)
            {
                SendHeroesToPier(true);
                if (Creature* draka = Get(_drakaGUID))
                    draka->DespawnOrUnsummon(0, Seconds(DAY));
            }
            if (step == STEP_BLACKHAND)
            {
                _confronted = true;
                if (Creature* khadgar = Get(_khadgarGUID))
                    khadgar->NearTeleportTo(KhadgarBattleship);
                if (Creature* brandt = Get(_brandtGUID))
                    brandt->DespawnOrUnsummon(0, Seconds(DAY));
            }
        }

        void OnStepChanged(uint8 step)
        {
            switch (step)
            {
                case STEP_IRON_HARBOR:
                    StartChannel();
                    break;
                case STEP_BLACKHAND:
                    // B'randt has fallen: Khadgar joins the hero on the battleship to send them on to Blackhand
                    if (Creature* khadgar = Get(_khadgarGUID))
                    {
                        khadgar->NearTeleportTo(KhadgarBattleship);
                        khadgar->AddDelayedEvent(500, [khadgar]() -> void
                        {
                            khadgar->CastSpell(khadgar, SPELL_TELEPORT_VISUAL, true);
                            khadgar->AI()->Talk(SAY_KHADGAR_HELP_BLACKHAND);
                        });
                    }
                    break;
                default:
                    break;
            }
        }

        void StartChannel()
        {
            if (_channeling)
                return;

            _channeling = true;
            _casting = false;
            _channelTimer = CHANNEL_DURATION;
            _waveTimer = 3 * IN_MILLISECONDS;

            if (Creature* khadgar = Get(_khadgarGUID))
            {
                khadgar->AI()->Talk(SAY_KHADGAR_CLEAR_THE_WAY);
                // nothing may break his spell: the Iron Horde has to get through the heroes and the champion
                khadgar->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                khadgar->SetReactState(REACT_PASSIVE);
            }
            Say(_maraadGUID, SAY_MARAAD_FOR_SHATTRATH);

            // down the stairs to hold their foot: Khadgar casts there, the others stand around him
            ObjectGuid const heroes[5] = { _khadgarGUID, _yrelGUID, _durotanGUID, _drakaGUID, _maraadGUID };
            for (uint8 i = 0; i < 5; ++i)
                if (Creature* hero = Get(heroes[i]))
                {
                    hero->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                    hero->SetHomePosition(HarborSpots[i]);
                    // down the middle of the stairs on foot, then to their place (the hero script chains the two)
                    hero->GetMotionMaster()->MovePoint(POINT_STAIRS_FOOT, 3492.0f, std::max(3118.0f, std::min(3136.0f, HarborSpots[i].GetPositionY())), 2.0f);
                }

            // the line waiting at the docks comes on at the same time
            uint8 n = 0;
            for (ObjectGuid const& guid : _frontGUIDs)
                if (Creature* unit = Get(guid))
                {
                    Position const& spot = HarborSpots[1 + n++ % 4];
                    unit->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                    unit->SetReactState(REACT_AGGRESSIVE);
                    // a few steps forward at a walk: the heroes come down to them
                    unit->SetWalk(true);
                    unit->GetMotionMaster()->MovePoint(1, unit->GetPositionX() - 8.0f, unit->GetPositionY(), unit->GetPositionZ());
                }

            _events.ScheduleEvent(EVENT_CHANNEL_LINE_1, 8 * IN_MILLISECONDS);
            _events.ScheduleEvent(EVENT_CHANNEL_LINE_2, 22 * IN_MILLISECONDS);
            _events.ScheduleEvent(EVENT_CHANNEL_LINE_3, 36 * IN_MILLISECONDS);
            _events.ScheduleEvent(EVENT_CHANNEL_LINE_4, 48 * IN_MILLISECONDS);
            _events.ScheduleEvent(EVENT_CHANNEL_LINE_5, 60 * IN_MILLISECONDS);
            _events.ScheduleEvent(EVENT_CHANNEL_LINE_6, 70 * IN_MILLISECONDS);
        }

        void SendWave()
        {
            Creature* khadgar = Get(_khadgarGUID);
            if (!khadgar)
                return;

            uint32 const pier = urand(0, 2);
            for (uint8 i = 0; i < 3; ++i)
            {
                WaveLane const& lane = WaveLanes[pier][i];
                Position pos(lane.Points[0].x, lane.Points[0].y, lane.Points[0].z);
                if (TempSummon* unit = khadgar->SummonCreature(WaveUnits[urand(0, 4)], pos, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10 * IN_MILLISECONDS))
                {
                    _waveGUIDs.push_back(unit->GetGUID());
                    unit->SetRespawnDelay(DAY);
                    unit->GetMotionMaster()->MoveSmoothPath(1, lane.Points, lane.Size, false);
                }
            }
        }

        // once at the foot of the stairs he rises into the air and begins Time Stop
        void UpdateKhadgarCast(Creature* khadgar)
        {
            if (khadgar->isInCombat())
                khadgar->CombatStop(true);

            if (!_casting)
            {
                if (khadgar->GetExactDist2d(&HarborSpots[0]) > 1.5f || !khadgar->movespline->Finalized())
                    return;
                _casting = true;
                _channelTimer = CHANNEL_DURATION;
                khadgar->SetFacingTo(HarborSpots[0].GetOrientation());
                khadgar->SetCanFly(true);
                khadgar->SetDisableGravity(true);
                // the airborne tier of his animations: he floats instead of standing on thin air
                khadgar->SetAnimTier(AnimTier::Fly);
                khadgar->GetMotionMaster()->MoveTakeoff(4, HarborSpots[0].GetPositionX(), HarborSpots[0].GetPositionY(), HarborSpots[0].GetPositionZ() + KHADGAR_LEVITATION);
                return;
            }

            // still rising
            if (!khadgar->movespline->Finalized())
                return;

            if (khadgar->GetUInt32Value(UNIT_FIELD_EMOTE_STATE) != EMOTE_STATE_SPELL_CHANNEL_OMNI)
                khadgar->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_SPELL_CHANNEL_OMNI);

            if (!khadgar->HasUnitState(UNIT_STATE_CASTING))
            {
                CustomSpellValues values;
                values.AddSpellMod(SPELLVALUE_RADIUS_MOD, int32(TIME_STOP_REACH / 50000.0f * 10000.0f));
                khadgar->CastCustomSpell(SPELL_TIME_STOP, values, khadgar, false);
            }
        }

        // the shock wave of Time Stop: what is left of the Iron Horde at the harbor hangs in the air, beyond any fight
        void FreezeHarbor(Creature* khadgar)
        {
            std::list<Creature*> units;
            for (uint32 entry : IronHordeUnits)
                khadgar->GetCreatureListWithEntryInGridAppend(units, entry, TIME_STOP_REACH);
            for (Creature* unit : units)
            {
                if (!unit->IsAlive() || unit->GetPositionX() > 3850.0f)
                    continue;
                unit->CombatStop(true);
                unit->SetReactState(REACT_PASSIVE);
                unit->GetMotionMaster()->Clear();
                unit->GetMotionMaster()->MoveIdle();
                unit->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                if (!unit->HasAura(SPELL_TIME_STOP))
                    unit->AddAura(SPELL_TIME_STOP, unit);
            }

            std::list<Creature*> allies;
            for (uint32 entry : { uint32(NPC_SHATTRATH_DEFENDER), uint32(NPC_YREL), uint32(NPC_DUROTAN), uint32(NPC_DRAKA), uint32(NPC_MARAAD) })
                khadgar->GetCreatureListWithEntryInGridAppend(allies, entry, TIME_STOP_REACH);
            for (Creature* ally : allies)
            {
                ally->CombatStop(true);
                if (ally->GetEntry() != NPC_SHATTRATH_DEFENDER)
                    continue;
                ally->SetReactState(REACT_PASSIVE);
                ally->SetWalk(false);
                ally->GetMotionMaster()->Clear();
                // to the foot of the stairs first, then straight up them: the street fighter script takes it from there
                ally->GetMotionMaster()->MovePoint(POINT_WITHDRAW_FOOT, StairsFoot.GetPositionX() + frand(-2.0f, 2.0f), StairsFoot.GetPositionY() + frand(-7.0f, 7.0f), StairsFoot.GetPositionZ());
            }
            _waveGUIDs.clear();
        }

        void FinishChannel()
        {
            _channeling = false;

            if (Creature* khadgar = Get(_khadgarGUID))
            {
                khadgar->InterruptNonMeleeSpells(false);
                khadgar->CastSpell(khadgar, SPELL_ARCANE_BLAST_VISUAL, true);
                FreezeHarbor(khadgar);
                // spent, he comes back down to the ground and kneels
                khadgar->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                khadgar->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                khadgar->GetMotionMaster()->MoveLand(5, HarborSpots[0]);
                khadgar->AddDelayedEvent(2500, [khadgar]() -> void
                {
                    khadgar->SetDisableGravity(false);
                    khadgar->SetCanFly(false);
                    khadgar->SetAnimTier(AnimTier::Ground);
                    khadgar->SetStandState(UNIT_STAND_STATE_KNEEL);
                });
                khadgar->AI()->Talk(SAY_KHADGAR_FINISHED);
            }
            SendScenarioEvent(EVENT_PROTECT_KHADGAR);
            _events.ScheduleEvent(EVENT_AFTER_CHANNEL_1, 7 * IN_MILLISECONDS);
        }

        // the heroes walk (or, after a restart, are placed) at the foot of the pier where the flagship ran aground;
        // Khadgar stays behind to rest and only catches up with the hero later
        void SendHeroesToPier(bool instant)
        {
            ObjectGuid const heroes[4] = { _yrelGUID, _durotanGUID, _maraadGUID, _khadgarGUID };
            for (uint8 i = 0; i < 4; ++i)
                if (Creature* hero = Get(heroes[i]))
                {
                    if (!instant && i == 3)
                        continue;
                    hero->SetHomePosition(HeroWays[i].End);
                    if (instant)
                        hero->NearTeleportTo(HeroWays[i].End);
                    else
                        hero->GetMotionMaster()->MoveSmoothPath(1, HeroWays[i].Points, HeroWays[i].Size, false);
                }
            _heroesWalking = !instant;
            _heroesAtPier = instant;
            _khadgarJoined = instant;
        }

        // all three in their row: each turns to face the ship
        void CheckHeroesArrived()
        {
            ObjectGuid const heroes[3] = { _yrelGUID, _durotanGUID, _maraadGUID };
            for (uint8 i = 0; i < 3; ++i)
                if (Creature* hero = Get(heroes[i]))
                    if (hero->GetExactDist2d(&HeroWays[i].End) > 2.5f || !hero->movespline->Finalized())
                        return;

            for (uint8 i = 0; i < 3; ++i)
                if (Creature* hero = Get(heroes[i]))
                    hero->SetFacingTo(HeroWays[i].End.GetOrientation());
            _heroesWalking = false;
            _heroesAtPier = true;
        }

        // Khadgar has regained his strength: he runs to the hero at the pier
        void CheckKhadgarJoined()
        {
            Creature* khadgar = Get(_khadgarGUID);
            if (!khadgar || !_confrontDone || khadgar->GetExactDist2d(&HeroWays[3].End) > 2.5f || !khadgar->movespline->Finalized())
                return;

            _khadgarJoining = false;
            _khadgarJoined = true;
            khadgar->SetSpeed(MOVE_RUN, 1.0f, true);
            Player* player = khadgar->SelectNearestPlayer(60.0f);
            if (player)
                khadgar->SetFacingToObject(player);
            khadgar->AI()->Talk(SAY_KHADGAR_SPEAK_WITH_ME, player ? player->GetGUID() : ObjectGuid::Empty);
        }

        void StartConfrontation()
        {
            _confronted = true;
            _events.ScheduleEvent(EVENT_CONFRONT_1, IN_MILLISECONDS);
        }

        void StartFlagship()
        {
            if (_flagshipStarted)
                return;

            _flagshipStarted = true;
            // the warlord turns on the champion
            if (Creature* blackhand = Get(_blackhandGUID))
            {
                blackhand->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
                blackhand->AI()->Talk(SAY_BLACKHAND_ALL_YOU_HAVE);
            }
            _events.ScheduleEvent(EVENT_FLAGSHIP_1, 3 * IN_MILLISECONDS);
        }

        // Blackhand's blast throws everyone on the deck to the ground and sets the flagship ablaze
        void Blast()
        {
            Creature* blackhand = Get(_blackhandGUID);
            if (!blackhand)
                return;

            blackhand->CombatStop(true);
            blackhand->SetReactState(REACT_PASSIVE);
            blackhand->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            blackhand->CastSpell(blackhand, SPELL_STRUCTURE_EXPLOSION, true);
            for (uint8 i = 0; i < 4; ++i)
            {
                float const a = float(M_PI) / 2.0f * i;
                if (TempSummon* burst = blackhand->SummonCreature(NPC_INVISIBLE_STALKER, blackhand->GetPositionX() + 4.0f * std::cos(a), blackhand->GetPositionY() + 4.0f * std::sin(a), blackhand->GetPositionZ() + 1.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 4 * IN_MILLISECONDS))
                    burst->CastSpell(burst, SPELL_FIRE_EXPLOSION, true);
            }

            ObjectGuid const heroes[3] = { _yrelGUID, _durotanGUID, _maraadGUID };
            for (ObjectGuid const& guid : heroes)
                if (Creature* hero = Get(guid))
                {
                    hero->CombatStop(true);
                    hero->SetReactState(REACT_PASSIVE);
                    hero->KnockbackFrom(blackhand->GetPositionX(), blackhand->GetPositionY(), 12.0f, 7.0f);
                }
            std::list<Player*> players;
            blackhand->GetPlayerListInGrid(players, 40.0f);
            for (Player* player : players)
            {
                player->CombatStop(true);
                player->KnockbackFrom(blackhand->GetPositionX(), blackhand->GetPositionY(), 12.0f, 7.0f);
            }

            for (G3D::Vector3 const& spot : FlagshipFires)
                if (TempSummon* fire = blackhand->SummonCreature(NPC_INVISIBLE_STALKER, spot.x, spot.y, spot.z, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 10 * MINUTE * IN_MILLISECONDS))
                    fire->AddAura(SPELL_DECK_FIRE, fire);

            // the battle is over: Blackhand is gone in the flames and the guns fall silent
            blackhand->DespawnOrUnsummon(3 * IN_MILLISECONDS, Seconds(DAY));
            _volleysOver = true;
        }

        void Complete()
        {
            _completed = true;
            _awaitingFilm = true;
            _filmTimer = 3 * MINUTE * IN_MILLISECONDS;
            instance->ApplyOnEveryPlayer([](Player* player)
            {
                player->SendMovieStart(MOVIE_BATTLE_FOR_SHATTRATH);
            });
        }

        // the film over (or skipped): the heroes are at the foot of the flagship, mourning, and a portal leads home
        void UpdateFilm(uint32 diff)
        {
            if (!_awaitingFilm)
                return;
            bool watching = false;
            instance->ApplyOnEveryPlayer([&](Player* player) { if (player->isWatchingMovie()) watching = true; });
            if (watching && _filmTimer > diff)
            {
                _filmTimer -= diff;
                return;
            }
            _awaitingFilm = false;
            EndingScene();
            _events.ScheduleEvent(EVENT_EXIT_PORTAL, 2 * IN_MILLISECONDS);
        }

        void EndingScene()
        {
            Creature* khadgar = Get(_khadgarGUID);
            if (Creature* maraad = Get(_maraadGUID))
            {
                if (TempSummon* fallen = maraad->SummonCreature(NPC_MARAAD, EndMaraad, TEMPSUMMON_MANUAL_DESPAWN))
                    fallen->Kill(fallen);
                maraad->DespawnOrUnsummon();
            }
            if (Creature* orgrim = Get(_orgrimGUID))
            {
                if (TempSummon* fallen = orgrim->SummonCreature(NPC_ORGRIM, EndOrgrim, TEMPSUMMON_MANUAL_DESPAWN))
                    fallen->Kill(fallen);
                orgrim->DespawnOrUnsummon();
            }
            if (Creature* durotan = Get(_durotanGUID))
            {
                durotan->CombatStop(true);
                durotan->SetReactState(REACT_PASSIVE);
                durotan->NearTeleportTo(EndDurotan);
                durotan->SetHomePosition(EndDurotan);
                durotan->AddDelayedEvent(500, [durotan]() -> void { durotan->SetStandState(UNIT_STAND_STATE_KNEEL); });
            }
            if (khadgar)
            {
                khadgar->CombatStop(true);
                khadgar->NearTeleportTo(EndKhadgar);
                khadgar->SetHomePosition(EndKhadgar);
            }
            if (Creature* yrel = Get(_yrelGUID))
                yrel->DespawnOrUnsummon();
            instance->ApplyOnEveryPlayer([](Player* player)
            {
                player->NearTeleportTo(EndPlayer);
            });
        }

        void ExecuteEvent(uint32 eventId)
        {
            Player* reference = GetReferencePlayer();
            ObjectGuid const target = reference ? reference->GetGUID() : ObjectGuid::Empty;

            switch (eventId)
            {
                case EVENT_CHANNEL_LINE_1: Say(_yrelGUID, SAY_YREL_STEEL_YOURSELVES); break;
                case EVENT_CHANNEL_LINE_2: Say(_durotanGUID, SAY_DUROTAN_DO_NOT_WAVER); break;
                case EVENT_CHANNEL_LINE_3: Say(_maraadGUID, SAY_MARAAD_NO_MERCY); break;
                case EVENT_CHANNEL_LINE_4: Say(_drakaGUID, SAY_DRAKA_GUARD_UP); break;
                case EVENT_CHANNEL_LINE_5: Say(_yrelGUID, SAY_YREL_CANNOT_HOLD); break;
                case EVENT_CHANNEL_LINE_6: Say(_durotanGUID, SAY_DUROTAN_MORE_TIME); break;
                case EVENT_CHANNEL_LINE_7: break;
                case EVENT_AFTER_CHANNEL_1:
                    Say(_durotanGUID, SAY_DUROTAN_SURVIVORS);
                    _events.ScheduleEvent(EVENT_AFTER_CHANNEL_2, 4 * IN_MILLISECONDS);
                    break;
                case EVENT_AFTER_CHANNEL_2:
                    Say(_drakaGUID, SAY_DRAKA_OF_COURSE);
                    // Draka searches the city for survivors and leaves the battle
                    if (Creature* draka = Get(_drakaGUID))
                        draka->DespawnOrUnsummon(4 * IN_MILLISECONDS, Seconds(DAY));
                    _events.ScheduleEvent(EVENT_AFTER_CHANNEL_3, 3 * IN_MILLISECONDS);
                    break;
                case EVENT_AFTER_CHANNEL_3:
                    Say(_maraadGUID, SAY_MARAAD_FORWARD);
                    SendHeroesToPier(false);
                    break;
                case EVENT_CONFRONT_1:
                    Say(_orgrimGUID, SAY_ORGRIM_INNOCENTS);
                    // rested, Khadgar runs to join the hero at the pier
                    if (Creature* khadgar = Get(_khadgarGUID))
                        if (!_khadgarJoined)
                        {
                            khadgar->SetStandState(UNIT_STAND_STATE_STAND);
                            khadgar->SetHomePosition(HeroWays[3].End);
                            khadgar->SetSpeed(MOVE_RUN, KHADGAR_RUN_RATE, true);
                            khadgar->GetMotionMaster()->MoveSmoothPath(1, HeroWays[3].Points, HeroWays[3].Size, false);
                            _khadgarJoining = true;
                        }
                    _events.ScheduleEvent(EVENT_CONFRONT_2, 4 * IN_MILLISECONDS);
                    break;
                case EVENT_CONFRONT_2:
                    Say(_blackhandGUID, SAY_BLACKHAND_INNOCENT);
                    _events.ScheduleEvent(EVENT_CONFRONT_3, 3 * IN_MILLISECONDS);
                    break;
                case EVENT_CONFRONT_3:
                    Say(_orgrimGUID, SAY_ORGRIM_YOU_MUST_STOP);
                    _events.ScheduleEvent(EVENT_CONFRONT_4, 6 * IN_MILLISECONDS);
                    break;
                case EVENT_CONFRONT_4:
                    Say(_blackhandGUID, SAY_BLACKHAND_DRAENOR_BURN);
                    _events.ScheduleEvent(EVENT_CONFRONT_KILL, 4500);
                    _events.ScheduleEvent(EVENT_CONFRONT_5, 9 * IN_MILLISECONDS);
                    break;
                case EVENT_CONFRONT_KILL:
                    // "You, traitor, will not": once said, the hammer comes down on Doomhammer
                    if (Creature* blackhand = Get(_blackhandGUID))
                    {
                        blackhand->HandleEmoteCommand(EMOTE_ONESHOT_ATTACK1H);
                        if (Creature* orgrim = Get(_orgrimGUID))
                            if (orgrim->IsAlive())
                                blackhand->Kill(orgrim);
                    }
                    break;
                case EVENT_CONFRONT_5:
                    Say(_yrelGUID, SAY_YREL_HOLD_OFF_BLACKHAND);
                    _events.ScheduleEvent(EVENT_CONFRONT_6, 3 * IN_MILLISECONDS);
                    break;
                case EVENT_CONFRONT_6:
                {
                    // Blackhand takes the middle of his deck, Yrel, Maraad and Durotan appear around him and hold him there
                    Position const flagship[3] = { YrelFlagship, MaraadFlagship, DurotanFlagship };
                    ObjectGuid const heroes[3] = { _yrelGUID, _maraadGUID, _durotanGUID };
                    _confrontDone = true;
                    if (Creature* blackhand = Get(_blackhandGUID))
                    {
                        blackhand->CastSpell(blackhand, SPELL_TELEPORT_VISUAL, true);
                        blackhand->NearTeleportTo(BlackhandFight);
                        blackhand->SetHomePosition(BlackhandFight);
                        blackhand->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_NPC);
                    }
                    for (uint8 i = 0; i < 3; ++i)
                        if (Creature* hero = Get(heroes[i]))
                        {
                            hero->CastSpell(hero, SPELL_TELEPORT_VISUAL, true);
                            Position const dest = flagship[i];
                            hero->SetHomePosition(dest);
                            hero->AddDelayedEvent(1200, [hero, dest]() -> void
                            {
                                hero->GetMotionMaster()->Clear();
                                hero->NearTeleportTo(dest);
                                hero->CastSpell(hero, SPELL_TELEPORT_VISUAL, true);
                                hero->AddDelayedEvent(1500, [hero]() -> void
                                {
                                    if (InstanceScript* instance = hero->GetInstanceScript())
                                        if (Creature* blackhand = ObjectAccessor::GetCreature(*hero, instance->GetGuidData(NPC_BLACKHAND)))
                                        {
                                            hero->AI()->AttackStart(blackhand);
                                            if (!blackhand->getVictim())
                                                blackhand->AI()->AttackStart(hero);
                                        }
                                });
                            });
                        }
                    SendScenarioEvent(EVENT_CONFRONT_BLACKHAND);
                    break;
                }
                case EVENT_FLAGSHIP_1:
                    Say(_durotanGUID, SAY_DUROTAN_BROTHER);
                    _events.ScheduleEvent(EVENT_FLAGSHIP_2, 3 * IN_MILLISECONDS);
                    break;
                case EVENT_FLAGSHIP_2:
                    Say(_blackhandGUID, SAY_BLACKHAND_WEAK);
                    _events.ScheduleEvent(EVENT_FLAGSHIP_3, 4 * IN_MILLISECONDS);
                    break;
                case EVENT_FLAGSHIP_3:
                    Say(_yrelGUID, SAY_YREL_HAVE_HOPE, target);
                    _events.ScheduleEvent(EVENT_FLAGSHIP_4, 10 * IN_MILLISECONDS);
                    break;
                case EVENT_FLAGSHIP_4:
                    // the laugh, then "Didn't anyone ever tell you not to play with fire?"
                    if (Creature* blackhand = Get(_blackhandGUID))
                    {
                        blackhand->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
                        blackhand->AI()->Talk(SAY_BLACKHAND_PLAY_WITH_FIRE);
                    }
                    // the spoken line, laugh included, lasts about 7 seconds: the ship blows up a few seconds after it
                    _events.ScheduleEvent(EVENT_FLAGSHIP_5, 11 * IN_MILLISECONDS);
                    break;
                case EVENT_FLAGSHIP_5:
                    Blast();
                    _events.ScheduleEvent(EVENT_FLAGSHIP_6, 2 * IN_MILLISECONDS);
                    break;
                case EVENT_FLAGSHIP_6:
                    SetData(DATA_BLACKHAND_DONE, 0);
                    break;
                case EVENT_EXIT_PORTAL:
                    // a portal home waits beside every hero once the film is over
                    instance->ApplyOnEveryPlayer([](Player* player)
                    {
                        float angle = player->GetOrientation();
                        player->SummonGameObject(GO_PORTAL_TO_TALADOR, player->GetPositionX() + 3.0f * std::cos(angle), player->GetPositionY() + 3.0f * std::sin(angle),
                            player->GetPositionZ(), angle + float(M_PI), 0.0f, 0.0f, 0.0f, 0.0f, 0);
                    });
                    break;
                default:
                    break;
            }
        }

        // A click on the Iron Star makes the hero the star: their own body rolls down the avenue on a path laid by the server,
        // with no vehicle and nothing to leave. Near the top of the harbor stairs they get their shape back and an empty
        // star curves away into the parapet on the left.
        void SetGuidData(uint32 type, ObjectGuid guid) override
        {
            if (type != DATA_STAR_RIDER || _riderGUID || _riderless)
                return;

            Player* player = ObjectAccessor::FindPlayer(instance, guid);
            Creature* star = Get(_ironStarGUID);
            if (!player || !star)
                return;

            _riderGUID = guid;
            _rideTimer = 600;
            _rolling = false;
            _launchTries = 0;
            _launchCheck = 0;
            _crushTimer = 300;
            player->RemoveAurasByType(SPELL_AURA_MOUNTED);
            player->CombatStop(true);
            // the teleport resets the client's movement state: without it the star glides along instead of rolling
            player->NearTeleportTo(star->GetPositionX(), star->GetPositionY(), star->GetPositionZ(), star->GetOrientation());
            player->SetDisplayId(DISPLAY_IRON_STAR);
            // it only rolls over them: nothing of the Iron Horde can pick a fight with the hero inside
            player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
            player->SetClientControl(player, false);
            // the parked star is the hero now; it is back shortly for the rest of a group
            star->DespawnOrUnsummon(0, Seconds(20));
            SetData(DATA_ENTERED_IRON_STAR, 0);
        }

        void LaunchRide(Player* rider)
        {
            _rolling = true;
            ++_launchTries;
            _launchFrom.Relocate(rider);
            _launchCheck = 1500;
            Movement::MoveSplineInit init(*rider);
            init.MovebyPath(Movement::PointsArray(IronStarPath, IronStarPath + IronStarPathSize));
            init.SetSmooth();
            init.SetVelocity(16.0f);
            rider->GetMotionMaster()->LaunchMoveSpline(std::move(init), 1, MOTION_SLOT_ACTIVE, POINT_MOTION_TYPE);
        }

        void EndRide(Player* player)
        {
            _riderGUID.Clear();
            _rolling = false;
            if (!player)
                return;
            player->DeMorph();
            player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
            player->SetClientControl(player, true);
        }

        void UpdateIronStar(uint32 diff)
        {
            if (_riderless)
            {
                Creature* empty = Get(_crashStarGUID);
                if (empty && !empty->movespline->Finalized())
                    return;

                // smashed against the parapet: it blows up and takes the Iron Horde nearby with it; the blast
                // comes from a marker left on the spot, and the star's own death is it bursting apart
                _riderless = false;
                if (!empty)
                    return;
                if (TempSummon* blast = empty->SummonCreature(NPC_INVISIBLE_STALKER, empty->GetPositionX(), empty->GetPositionY(), empty->GetPositionZ() + 1.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 4 * IN_MILLISECONDS))
                    blast->CastSpell(blast, SPELL_FIRE_EXPLOSION, true);
                CrushAround(empty, 10.0f);
                empty->Kill(empty);
                empty->DespawnOrUnsummon(5 * IN_MILLISECONDS);
                return;
            }

            if (!_riderGUID)
                return;

            Player* rider = ObjectAccessor::FindPlayer(instance, _riderGUID);
            if (!rider || !rider->IsAlive() || rider->GetMapId() != MAP_SHATTRATH_SCENARIO)
            {
                EndRide(rider);
                return;
            }

            if (rider->GetDisplayId() != DISPLAY_IRON_STAR)
                rider->SetDisplayId(DISPLAY_IRON_STAR);

            if (!_rolling)
            {
                if (_rideTimer > diff || rider->IsBeingTeleported())
                {
                    if (_rideTimer > diff)
                        _rideTimer -= diff;
                    return;
                }
                LaunchRide(rider);
                return;
            }

            // a launch the client swallowed leaves the star standing: it is launched again
            if (_launchCheck)
            {
                if (_launchCheck > diff)
                    _launchCheck -= diff;
                else
                {
                    _launchCheck = 0;
                    if (rider->GetExactDist2d(&_launchFrom) < 2.0f && _launchTries < 3)
                    {
                        LaunchRide(rider);
                        return;
                    }
                }
            }

            if (_crushTimer <= diff)
            {
                _crushTimer = 300;
                CrushAround(rider, 6.0f);
                if (rider->isInCombat())
                    rider->CombatStop(true);
            }
            else
                _crushTimer -= diff;

            if (!rider->movespline->Finalized())
                return;

            // the end of the road: the hero gets their shape back, and an empty star curves away into the parapet
            Position const from = rider->GetPosition();
            EndRide(rider);
            rider->KnockbackFrom(IronStarCrash.GetPositionX(), IronStarCrash.GetPositionY(), 6.0f, 5.0f);
            SetData(DATA_RIDE_ARRIVED, 0);

            if (TempSummon* empty = rider->SummonCreature(NPC_IRON_STAR_SHELTER, from, TEMPSUMMON_TIMED_DESPAWN, 30 * IN_MILLISECONDS))
            {
                _crashStarGUID = empty->GetGUID();
                _riderless = true;
                empty->SetReactState(REACT_PASSIVE);
                Movement::MoveSplineInit init(*empty);
                init.MovebyPath(Movement::PointsArray(IronStarArc, IronStarArc + std::extent<decltype(IronStarArc)>::value));
                init.SetSmooth();
                init.SetVelocity(14.0f);
                empty->GetMotionMaster()->LaunchMoveSpline(std::move(init), 2, MOTION_SLOT_ACTIVE, POINT_MOTION_TYPE);
            }
        }

        void OnPlayerLeave(Player* player) override
        {
            InstanceScript::OnPlayerLeave(player);
            if (player->GetGUID() == _riderGUID)
                EndRide(player);
        }

        // the ships that ran aground keep shelling the harbor from their turrets, for as long as the battle lasts
        void UpdateVolleys(uint32 diff)
        {
            if (_volleysOver)
                return;

            // the ground and the crystal each have their own steady rhythm of fire
            if (_volleyTimer <= diff)
            {
                _volleyTimer = urand(2500, 3500);
                FireShell(false);
            }
            else
                _volleyTimer -= diff;

            if (_crystalTimer <= diff)
            {
                _crystalTimer = urand(2500, 3500);
                if (Get(_crystalGUID))
                    FireShell(true);
            }
            else
                _crystalTimer -= diff;
        }

        // four fixed spots around the crystal, on the side of the ships, where it brings the shells down
        Position CrystalCatchSpot(Creature* crystal)
        {
            if (!_catchBaseSet)
            {
                float sx = 0.0f, sy = 0.0f;
                for (ObjectGuid const& guid : _turretGUIDs)
                    if (Creature* muzzle = Get(guid))
                    {
                        float const a = crystal->GetAngle(muzzle);
                        sx += std::cos(a), sy += std::sin(a);
                    }
                _catchBase = std::atan2(sy, sx);
                _catchBaseSet = true;
            }
            static float const offsets[4] = { -0.50f, -0.17f, 0.17f, 0.50f };
            static float const heights[4] = { 5.0f, -2.0f, 3.0f, 0.0f };
            uint8 const slot = _nextCatch++ % 4;
            float const a = _catchBase + offsets[slot];
            return Position(crystal->GetPositionX() + CRYSTAL_SHIELD_RANGE * std::cos(a), crystal->GetPositionY() + CRYSTAL_SHIELD_RANGE * std::sin(a),
                crystal->GetPositionZ() + heights[slot]);
        }

        void FireShell(bool intercepted)
        {
            // the three turrets fire in turn
            if (_turretGUIDs.empty() || !GetReferencePlayer())
                return;

            auto itr = _turretGUIDs.begin();
            std::advance(itr, _nextTurret++ % _turretGUIDs.size());
            Creature* turret = Get(*itr);
            if (!turret)
                return;

            for (ObjectGuid const& guid : _turretGOs)
                if (GameObject* gun = instance->GetGameObject(guid))
                    if (gun->GetExactDist2d(turret) < 5.0f)
                        gun->SendCustomAnim(1);

            // the crystal's shells are lasered down in the air before they land; the others burst against the spots
            // picked in game, never on the heroes, the defenders or a player
            float x, y, z;
            Creature* crystal = Get(_crystalGUID);
            if (intercepted)
            {
                Position const spot = CrystalCatchSpot(crystal);
                x = spot.GetPositionX(), y = spot.GetPositionY(), z = spot.GetPositionZ();
            }
            else
            {
                G3D::Vector3 const& spot = ShellTargets[urand(0, std::extent<decltype(ShellTargets)>::value - 1)];
                x = spot.x, y = spot.y, z = spot.z;
            }
            if (TempSummon* impact = turret->SummonCreature(NPC_SHELL_IMPACT, x, y, z + 0.5f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 8 * IN_MILLISECONDS))
            {
                // the mark holds where it was put, up in the air for the crystal's catches
                impact->SetCanFly(true);
                impact->SetDisableGravity(true);
                if (!intercepted)
                    impact->SetObjectScale(SHELL_GROUND_SCALE);
                if (!intercepted)
                {
                    std::list<Creature*> nearby;
                    for (uint32 entry : { uint32(NPC_SHATTRATH_DEFENDER), uint32(NPC_YREL), uint32(NPC_DUROTAN), uint32(NPC_DRAKA), uint32(NPC_MARAAD), uint32(NPC_KHADGAR) })
                        impact->GetCreatureListWithEntryInGridAppend(nearby, entry, 12.0f);
                    bool player = false;
                    instance->ApplyOnEveryPlayer([&](Player* p) { if (p->GetDistance2d(x, y) < 12.0f) player = true; });
                    if (!nearby.empty() || player)
                    {
                        impact->DespawnOrUnsummon();
                        return;
                    }
                }
                // the shell leaves from the mark on the turret itself
                Creature* relay = turret;

                if (intercepted)
                {
                    // the crystal fires just before the shell reaches it
                    uint32 const flight = uint32(relay->GetDistance(impact) / SHELL_SPEED * IN_MILLISECONDS) + 1200;
                    ObjectGuid const markGuid = impact->GetGUID();
                    crystal->AddDelayedEvent(flight > 300 ? flight - 300 : flight, [crystal, markGuid]() -> void
                    {
                        if (Creature* mark = ObjectAccessor::GetCreature(*crystal, markGuid))
                            crystal->CastSpell(mark, SPELL_CRYSTAL_LASER, true);
                    });
                }
                // the shell leaves as the guns recoil
                ObjectGuid const impactGuid = impact->GetGUID();
                relay->AddDelayedEvent(1200, [relay, impactGuid]() -> void
                {
                    if (Creature* mark = ObjectAccessor::GetCreature(*relay, impactGuid))
                    {
                        relay->CastSpell(mark, SPELL_SHELL_IN_FLIGHT, true);
                        relay->CastSpell(mark, SPELL_FIRE_EXPLOSION, true);
                    }
                });
            }
        }

        // whatever of the Iron Horde stands in its way is crushed and thrown aside
        void CrushAround(Unit* star, float range)
        {
            std::list<Creature*> victims;
            for (uint32 entry : IronHordeUnits)
                star->GetCreatureListWithEntryInGridAppend(victims, entry, range);
            for (Creature* victim : victims)
                if (victim->IsAlive() && !IsFrozen(victim))
                {
                    victim->KnockbackFrom(star->GetPositionX(), star->GetPositionY(), 12.0f, 8.0f);
                    star->Kill(victim);
                }
        }

        void Update(uint32 diff) override
        {
            _events.Update(diff);
            while (uint32 eventId = _events.ExecuteEvent())
                ExecuteEvent(eventId);

            UpdateIronStar(diff);
            UpdateVolleys(diff);
            UpdateFilm(diff);

            if (_channeling)
            {
                if (_waveTimer <= diff)
                {
                    SendWave();
                    _waveTimer = WAVE_INTERVAL;
                }
                else
                    _waveTimer -= diff;

                // the spell's time only runs once he has taken his place
                if (_casting && _channelTimer <= diff)
                    FinishChannel();
                else
                {
                    if (_casting)
                        _channelTimer -= diff;
                    // if anything still pulled him into a fight, he goes straight back to his spell
                    if (Creature* khadgar = Get(_khadgarGUID))
                        UpdateKhadgarCast(khadgar);
                }
            }

            if (_khadgarJoining)
                CheckKhadgarJoined();

            if (_checkTimer > diff)
            {
                _checkTimer -= diff;
                return;
            }
            _checkTimer = IN_MILLISECONDS;

            Scenario* scenario = GetScenario(instance);
            if (!scenario || _completed)
                return;

            if (scenario->IsCompleted(false))
            {
                Complete();
                return;
            }

            if (!_restoreChecked)
                RestoreStep(scenario);

            uint8 step = scenario->GetCurrentStep();
            if (step != _lastStep)
            {
                _lastStep = step;
                OnStepChanged(step);
            }

            // Khadgar only shows a menu when he has something to offer
            if (Creature* khadgar = Get(_khadgarGUID))
            {
                bool offer = step == STEP_BATTLE_IS_JOINED || (step == STEP_THE_WEAPON && _khadgarJoined) || step == STEP_BLACKHAND;
                if (offer)
                    khadgar->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                else
                    khadgar->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            // the star can only be boarded once Thaelin has sent the hero on
            if (Creature* star = Get(_ironStarGUID))
            {
                if (step == STEP_IRON_STAR && !_riderGUID && !_riderless)
                    star->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                else
                    star->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            // on the flagship, Blackhand turns on the hero who comes near
            if (step == STEP_BLACKHAND && !_flagshipStarted)
                if (Creature* blackhand = Get(_blackhandGUID))
                    if (blackhand->SelectNearestPlayer(25.0f))
                        StartFlagship();

            if (step == STEP_THE_WEAPON)
            {
                // reaching the foot of the pier, where Orgrim stands up to Blackhand
                if (_heroesWalking)
                    CheckHeroesArrived();
                if (!_confronted && _heroesAtPier)
                    if (Creature* yrel = Get(_yrelGUID))
                        if (yrel->SelectNearestPlayer(40.0f))
                            StartConfrontation();

                // landing on the battleship
                if (!_brandtGreeted)
                    if (Creature* brandt = Get(_brandtGUID))
                        if (brandt->SelectNearestPlayer(60.0f))
                        {
                            _brandtGreeted = true;
                            brandt->AI()->Talk(SAY_BRANDT_NUISANCE);
                        }
            }
        }
    };

    InstanceScript* GetInstanceScript(InstanceMap* map) const override
    {
        return new instance_battle_for_shattrath_InstanceMapScript(map);
    }
};

void AddSC_instance_battle_for_shattrath()
{
    new instance_battle_for_shattrath();
}
