/*
    Dungeon : Violet Hold Legion 100-110
*/

#include "ScriptMgr.h"
#include "ScenarioMgr.h"
#include "Scenario.h"
#include "WorldStatePackets.h"
#include "violet_hold_legion.h"

enum StepPhases
{
    STEP_PHASE_WAVES        = 0,
    STEP_PHASE_BOSS         = 1,
    STEP_PHASE_MALGATH      = 2,
    STEP_PHASE_FINAL_BOSS   = 3,
};

uint8 const BOSS_NOT_DRAWN      = 0xFF;
uint8 const MAX_OPEN_PORTALS    = 2;
uint32 const PORTAL_INTERVAL    = 90000;
uint32 const PORTAL_REOPEN      = 5000;

class instance_violet_hold_legion : public InstanceMapScript
{
public:
    instance_violet_hold_legion() : InstanceMapScript("instance_violet_hold_legion", 1544) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const override
    {
        return new instance_violet_hold_legion_InstanceMapScript(map);
    }

    struct instance_violet_hold_legion_InstanceMapScript : public InstanceScript
    {
        instance_violet_hold_legion_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
        {
            SetHeaders(DataHeader);
            SetBossNumber(MAX_ENCOUNTER);
        }

        ObjectGuid uiKaahrj;
        ObjectGuid uiMillificent;
        ObjectGuid uiFesterface;
        ObjectGuid uiShivermaw;
        ObjectGuid uiAnubesset;
        ObjectGuid uiSaelorn;
        ObjectGuid uiThalena;
        ObjectGuid uiMalgath;
        ObjectGuid uiBetrug;

        ObjectGuid uiKaahrjCell;
        ObjectGuid uiMillificentCell;
        ObjectGuid uiFesterfaceCell;
        ObjectGuid uiShivermawCell;
        ObjectGuid uiAnubessetCell;
        ObjectGuid uiSaelornCell;
        ObjectGuid uiThalenaCell;

        ObjectGuid uiMainDoor;
        ObjectGuid uiActivationCrystal[4];
        ObjectGuid uiIntroCrystal;
        ObjectGuid uiSinclari;

        std::set<ObjectGuid> trashMobs;
        std::set<ObjectGuid> portals;
        std::set<ObjectGuid> introPortals;
        std::set<ObjectGuid> usedCrystals;      // one use each per instance, a restart after a failure included
        std::set<ObjectGuid> introMobs;

        uint8 uiMainEventPhase;
        uint8 uiStep;
        uint8 uiStepPhase;
        uint8 uiFirstBoss;
        uint8 uiSecondBoss;
        uint8 uiFinalBoss;
        uint8 uiDoorIntegrity;
        uint8 uiCountActivationCrystals;

        uint16 m_auiEncounter[MAX_ENCOUNTER];

        uint32 uiInvasionPoints;
        uint32 uiPortalTimer;
        uint32 uiCheckTimer;
        uint32 uiIntroTimer;
        uint32 uiFinalBossTimer;

        bool bIntroDone;
        bool bFirstPortalOfStep;

        std::string str_data;

        void Initialize() override
        {
            uiKaahrj.Clear();
            uiMillificent.Clear();
            uiFesterface.Clear();
            uiShivermaw.Clear();
            uiAnubesset.Clear();
            uiSaelorn.Clear();
            uiThalena.Clear();
            uiMalgath.Clear();
            uiBetrug.Clear();

            uiKaahrjCell.Clear();
            uiMillificentCell.Clear();
            uiFesterfaceCell.Clear();
            uiShivermawCell.Clear();
            uiAnubessetCell.Clear();
            uiSaelornCell.Clear();
            uiThalenaCell.Clear();

            uiMainDoor.Clear();
            uiIntroCrystal.Clear();
            uiSinclari.Clear();

            trashMobs.clear();
            portals.clear();
            introPortals.clear();
            introMobs.clear();

            uiStep = 0;
            uiStepPhase = STEP_PHASE_WAVES;
            uiFirstBoss = BOSS_NOT_DRAWN;
            uiSecondBoss = BOSS_NOT_DRAWN;
            uiFinalBoss = BOSS_NOT_DRAWN;
            uiCountActivationCrystals = 0;

            uiDoorIntegrity = 100;
            uiInvasionPoints = 0;
            uiPortalTimer = PORTAL_REOPEN;
            uiCheckTimer = 1000;
            uiIntroTimer = 5000;
            uiFinalBossTimer = 0;

            bIntroDone = false;
            bFirstPortalOfStep = true;
            uiMainEventPhase = NOT_STARTED;

            memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
        }

        void OnCreatureCreate(Creature* creature) override
        {
            switch (creature->GetEntry())
            {
                case NPC_SHADOW_BEAST:
                    SetGuidData(DATA_ADD_TRASH_MOB, creature->GetGUID());
                    break;
                case NPC_LIEUTENANT_SINCLARI:
                    uiSinclari = creature->GetGUID();
                    break;
                case NPC_INTRO_PORTAL:
                    introPortals.insert(creature->GetGUID());
                    if (bIntroDone)
                        creature->SetVisible(false);
                    break;
                case NPC_MINDFLAYER_KAAHRJ:
                    uiKaahrj = creature->GetGUID();
                    break;
                case NPC_MILLIFICENT_MANASTORM:
                    uiMillificent = creature->GetGUID();
                    break;
                case NPC_FESTERFACE:
                    uiFesterface = creature->GetGUID();
                    break;
                case NPC_SHIVERMAW:
                    uiShivermaw = creature->GetGUID();
                    break;
                case NPC_ANUBESSET:
                    uiAnubesset = creature->GetGUID();
                    break;
                case NPC_SAELORN:
                    uiSaelorn = creature->GetGUID();
                    break;
                case NPC_PRINCESS_THALENA:
                    uiThalena = creature->GetGUID();
                    break;
                case NPC_LORD_MALGATH:
                    uiMalgath = creature->GetGUID();
                    break;
                case NPC_FEL_LORD_BETRUG:
                    uiBetrug = creature->GetGUID();
                    break;
                default:
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* go) override
        {
            switch (go->GetEntry())
            {
                case GO_MAIN_DOOR:
                    uiMainDoor = go->GetGUID();
                    break;
                case GO_KAAHRJ_DOOR:
                    uiKaahrjCell = go->GetGUID();
                    break;
                case GO_MILLIFICENT_DOOR:
                    uiMillificentCell = go->GetGUID();
                    break;
                case GO_FESTERFACE_DOOR:
                    uiFesterfaceCell = go->GetGUID();
                    break;
                case GO_SHIVERMAW_DOOR:
                    uiShivermawCell = go->GetGUID();
                    break;
                case GO_ANUBESSET_DOOR:
                    uiAnubessetCell = go->GetGUID();
                    break;
                case GO_SAELORN_DOOR:
                    uiSaelornCell = go->GetGUID();
                    break;
                case GO_THALENA_DOOR:
                    uiThalenaCell = go->GetGUID();
                    break;
                case GO_INTRO_ACTIVATION_CRYSTAL:
                    uiIntroCrystal = go->GetGUID();
                    break;
                case GO_ACTIVATION_CRYSTAL:
                    go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                    if (uiCountActivationCrystals < 4)
                        uiActivationCrystal[uiCountActivationCrystals++] = go->GetGUID();
                    break;
                default:
                    break;
            }
        }

        Scenario* GetScenario() const
        {
            return sScenarioMgr->GetScenario(instance->GetInstanceId());
        }

        Player* GetCreditPlayer() const
        {
            Map::PlayerList const& players = instance->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                if (Player* player = itr->getSource())
                    if (!player->isGameMaster() && player->CanContact())
                        return player;

            return nullptr;
        }

        // Sum of the weighted kills of the current step's "Invasion Forces" criteria, read from the scenario so the
        // boss comes when the client's bar is full
        uint32 GetInvasionPoints()
        {
            Scenario* scenario = GetScenario();
            if (!scenario)
                return uiInvasionPoints;

            CriteriaTree const* stepTree = sAchievementMgr->GetCriteriaTree(scenario->GetScenarioCriteriaByStep(uiStep));
            if (!stepTree)
                return uiInvasionPoints;

            CriteriaProgressMap const* progressMap = scenario->GetAchievementMgr().GetCriteriaProgressMap();
            for (CriteriaTree const* node : stepTree->Children)
            {
                if (node->Entry->Operator != CRITERIA_TREE_OPERATOR_SCENARIO)
                    continue;

                uint32 points = 0;
                for (CriteriaTree const* child : node->Children)
                {
                    auto itr = progressMap->find(child->ID);
                    if (itr != progressMap->end())
                        points += uint32(itr->second.Counter) * child->Entry->Amount;
                }
                // the scenario gets no credit for a kill by a game master: the script's own count still ends the waves
                return std::max(points, uiInvasionPoints);
            }

            return uiInvasionPoints;
        }

        void CreatureDies(Creature* creature, Unit* /*killer*/) override
        {
            if (creature->GetEntry() == NPC_SHADOW_BEAST)
                SetGuidData(DATA_DEL_TRASH_MOB, creature->GetGUID());

            if (uiMainEventPhase != IN_PROGRESS || !GetInvasionForcesWeight(creature->GetEntry()))
                return;

            uiInvasionPoints += GetInvasionForcesWeight(creature->GetEntry());

            // a kill no player took part in (activation crystal) gets no kill credit: given here so the bar still moves
            if (!creature->IsDamageEnoughForLootingAndReward())
                if (Scenario* scenario = GetScenario())
                    if (Player* player = GetCreditPlayer())
                        scenario->UpdateAchievementCriteria(CRITERIA_TYPE_KILL_CREATURE, creature->GetEntry(), 1, 0, creature, player);
        }

        bool IsEncounterInProgress() const override
        {
            for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                if (m_auiEncounter[i] == IN_PROGRESS)
                    return true;

            return false;
        }

        uint8 GetStepBoss() const
        {
            switch (uiStep)
            {
                case 0:
                    return uiFirstBoss;
                case 1:
                    return uiSecondBoss;
                default:
                    return uiFinalBoss;
            }
        }

        bool SetBossState(uint32 type, EncounterState state) override
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;

            if (state == DONE && type == GetStepBoss())
            {
                if ((uiStep < 2 && uiStepPhase == STEP_PHASE_BOSS) || (uiStep == 2 && uiStepPhase == STEP_PHASE_FINAL_BOSS))
                    CompleteStep();
            }

            return true;
        }

        void CompleteStep()
        {
            DoUpdateAchievementCriteria(CRITERIA_TYPE_SCRIPT_EVENT_2, stepBossCriteria[uiStep]);
            m_auiEncounter[uiStep] = DONE;

            if (uiStep == 2)
            {
                // a missed credit must not cost the group its end-of-dungeon reward
                if (Scenario* scenario = GetScenario())
                    if (!scenario->IsCompleted(false))
                        scenario->Reward(false, 2);

                uiMainEventPhase = DONE;
                SetData(DATA_MAIN_DOOR, GO_STATE_ACTIVE);
                DoUpdateWorldState(WorldStates::WORLD_STATE_VH, 0);
                for (uint8 i = 0; i < 4; ++i)
                    if (GameObject* crystal = instance->GetGameObject(uiActivationCrystal[i]))
                        crystal->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
            }
            else
            {
                ++uiStep;
                SyncScenarioStep();
                // 30 seconds of rest after each prisoner
                StartStep(30000);
            }

            SaveToDB();
        }

        // The scenario is not saved (it starts over after a restart while the instance kept its steps), and a step
        // left incomplete by a missed credit would block the next one
        void SyncScenarioStep()
        {
            if (Scenario* scenario = GetScenario())
                if (scenario->GetCurrentStep() < uiStep && uiStep < scenario->GetStepCount(false))
                    scenario->SetCurrentStep(uiStep);
        }

        void StartStep(uint32 firstPortalDelay)
        {
            uiStepPhase = STEP_PHASE_WAVES;
            uiInvasionPoints = 0;
            uiPortalTimer = firstPortalDelay;
            uiCheckTimer = 1000;
            uiFinalBossTimer = 0;
            bFirstPortalOfStep = true;
        }

        static bool IsCellBoss(uint8 boss)
        {
            return boss < MAX_ENCOUNTER && boss != DATA_SAELORN && boss != DATA_BETRUG;
        }

        void DrawBosses()
        {
            std::vector<uint8> pool = { DATA_KAAHRJ, DATA_MILLIFICENT, DATA_FESTERFACE, DATA_SHIVERMAW, DATA_ANUBESSET, DATA_THALENA };

            if (!IsCellBoss(uiFirstBoss))
                uiFirstBoss = pool[urand(0, pool.size() - 1)];

            pool.erase(std::remove(pool.begin(), pool.end(), uiFirstBoss), pool.end());

            if (!IsCellBoss(uiSecondBoss) || uiSecondBoss == uiFirstBoss)
                uiSecondBoss = pool[urand(0, pool.size() - 1)];

            if (uiFinalBoss != DATA_SAELORN && uiFinalBoss != DATA_BETRUG)
                uiFinalBoss = urand(0, 1) ? DATA_SAELORN : DATA_BETRUG;
        }

        void SetData(uint32 type, uint32 data) override
        {
            switch (type)
            {
                case DATA_START_BOSS_ENCOUNTER:
                    if (uiStep < 2 && uiStepPhase == STEP_PHASE_BOSS)
                        StartBossEncounter(GetStepBoss());
                    break;
                case DATA_MALGATH_DIED:
                    if (uiStepPhase == STEP_PHASE_MALGATH)
                    {
                        uiStepPhase = STEP_PHASE_FINAL_BOSS;
                        uiFinalBossTimer = 5000;
                    }
                    break;
                case DATA_INTRO_CRYSTAL:
                    ActivateIntroCrystal();
                    break;
                case DATA_MAIN_DOOR:
                    if (GameObject* pMainDoor = instance->GetGameObject(uiMainDoor))
                        pMainDoor->SetGoState(static_cast<GOState>(data));
                    break;
                case DATA_DOOR_INTEGRITY:
                    uiDoorIntegrity = data;
                    DoUpdateWorldState(WorldStates::WORLD_STATE_VH_PRISON_STATE, uiDoorIntegrity);
                    break;
                case DATA_MAIN_EVENT_PHASE:
                    uiMainEventPhase = data;
                    if (data == SPECIAL)
                    {
                        DrawBosses();
                        SaveToDB();
                    }
                    else if (data == IN_PROGRESS) // Start event, at the step the group stopped at
                    {
                        DrawBosses();
                        SetData(DATA_MAIN_DOOR, GO_STATE_READY);
                        uiDoorIntegrity = 100;
                        DoUpdateWorldState(WorldStates::WORLD_STATE_VH, 1);
                        DoUpdateWorldState(WorldStates::WORLD_STATE_VH_PRISON_STATE, uiDoorIntegrity);
                        for (int i = 0; i < 4; ++i)
                            if (GameObject* crystal = instance->GetGameObject(uiActivationCrystal[i]))
                            {
                                crystal->EnableOrDisableGo(false);
                                if (!usedCrystals.count(crystal->GetGUID()))
                                    crystal->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                            }

                        SyncScenarioStep();
                        StartStep(PORTAL_REOPEN);
                    }
                    break;
                default:
                    break;
            }
        }

        void SetGuidData(uint32 type, ObjectGuid data) override
        {
            switch (type)
            {
                case DATA_ADD_TRASH_MOB:
                    trashMobs.insert(data);
                    break;
                case DATA_DEL_TRASH_MOB:
                    trashMobs.erase(data);
                    break;
                case DATA_PORTAL_CLOSED:
                    portals.erase(data);
                    if (portals.empty() && uiPortalTimer > PORTAL_REOPEN)
                        uiPortalTimer = PORTAL_REOPEN;
                    break;
            }
        }

        uint32 GetData(uint32 type) const override
        {
            switch (type)
            {
                case DATA_MAIN_EVENT_PHASE:
                    return uiMainEventPhase;
                case DATA_1ST_BOSS_EVENT:
                    return m_auiEncounter[0];
                case DATA_2ND_BOSS_EVENT:
                    return m_auiEncounter[1];
                case DATA_BETRUG_EVENT:
                    return m_auiEncounter[2];
                case DATA_DOOR_INTEGRITY:
                    return uiDoorIntegrity;
                case DATA_STEP:
                    return uiStep;
                case DATA_STEP_BOSS:
                    return GetStepBoss();
                case DATA_WAVES_ACTIVE:
                    return uiMainEventPhase == IN_PROGRESS && uiStepPhase == STEP_PHASE_WAVES ? 1 : 0;
                case DATA_FIRST_BOSS:
                    return uiFirstBoss;
                case DATA_SECOND_BOSS:
                    return uiSecondBoss;
                case DATA_FINAL_BOSS:
                    return uiFinalBoss;
            }

            return 0;
        }

        ObjectGuid GetGuidData(uint32 type) const override
        {
            switch (type)
            {
                case DATA_SINCLARI:
                    return uiSinclari;
                case DATA_KAAHRJ:
                    return uiKaahrj;
                case DATA_MILLIFICENT:
                    return uiMillificent;
                case DATA_FESTERFACE:
                    return uiFesterface;
                case DATA_SHIVERMAW:
                    return uiShivermaw;
                case DATA_ANUBESSET:
                    return uiAnubesset;
                case DATA_SAELORN:
                    return uiSaelorn;
                case DATA_THALENA:
                    return uiThalena;
                case DATA_BETRUG:
                    return uiBetrug;
                case DATA_MAIN_DOOR:
                    return uiMainDoor;
                //Door Data
                case DATA_KAAHRJ_CELL:
                    return uiKaahrjCell;
                case DATA_MILLIFICENT_CELL:
                    return uiMillificentCell;
                case DATA_FESTERFACE_CELL:
                    return uiFesterfaceCell;
                case DATA_SHIVERMAW_CELL:
                    return uiShivermawCell;
                case DATA_ANUBESSET_CELL:
                    return uiAnubessetCell;
                case DATA_SAELORN_CELL:
                    return uiSaelornCell;
                case DATA_THALENA_CELL:
                    return uiThalenaCell;
            }
            return ObjectGuid::Empty;
        }

        void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet) override
        {
            packet.Worldstates.emplace_back(WorldStates::WORLD_STATE_VH, uiMainEventPhase == IN_PROGRESS ? 1 : 0);
            packet.Worldstates.emplace_back(WorldStates::WORLD_STATE_VH_PRISON_STATE, uiDoorIntegrity);
        }

        // He only flies to open a portal, hanging over the centre of the hold (without this he fell to the floor and
        // bounced back up); he frees the prisoners and fights on the ground
        Creature* SummonMalgath(bool airborne)
        {
            Creature* pSinclari = instance->GetCreature(uiSinclari);
            if (!pSinclari)
                return nullptr;

            Position pos = MiddleRoomSaboLoc;
            if (!airborne)
                pSinclari->UpdateGroundPositionZ(pos.m_positionX, pos.m_positionY, pos.m_positionZ);

            Creature* malgath = pSinclari->SummonCreature(NPC_LORD_MALGATH, pos, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 20000);
            if (malgath)
            {
                malgath->SetCanFly(airborne);
                malgath->SetDisableGravity(airborne);
                malgath->CastSpell(malgath, SPELL_FEL_SHIELD, true);
            }

            return malgath;
        }

        // Lord Malgath appears over the hold and opens the portal himself
        void OpenPortal()
        {
            Creature* pSinclari = instance->GetCreature(uiSinclari);
            if (!pSinclari)
                return;

            std::vector<uint8> freeLocations;
            for (uint8 i = 0; i < 5; ++i)
            {
                bool used = false;
                for (ObjectGuid const& guid : portals)
                    if (Creature* portal = instance->GetCreature(guid))
                        if (portal->GetDistance(PortalLocation[i]) < 5.0f)
                            used = true;

                if (!used)
                    freeLocations.push_back(i);
            }

            if (freeLocations.empty())
                return;

            Creature* portal = pSinclari->SummonCreature(NPC_TELEPORTATION_PORTAL, PortalLocation[freeLocations[urand(0, freeLocations.size() - 1)]]);
            if (!portal)
                return;

            portals.insert(portal->GetGUID());
            // 30% elite squads, 70% portal guardians or keepers
            portal->AI()->SetData(DATA_PORTAL_KIND, urand(0, 99) < 30 ? 0 : 1);

            if (Creature* malgath = SummonMalgath(true))
            {
                if (bFirstPortalOfStep && uiStep != 1)
                    malgath->AI()->Talk(uiStep ? SAY_MALGATH_THIRD_STEP : SAY_MALGATH_INTRO);
                malgath->AI()->SetGUID(portal->GetGUID(), ACTION_MALGATH_OPEN_PORTAL);
            }

            bFirstPortalOfStep = false;
        }

        // The step's Invasion Forces are beaten: Malgath releases a prisoner, or comes down himself in the last step
        void EndWaves()
        {
            uiStepPhase = uiStep < 2 ? STEP_PHASE_BOSS : STEP_PHASE_MALGATH;

            for (ObjectGuid const& guid : portals)
                if (Creature* portal = instance->GetCreature(guid))
                    portal->DespawnOrUnsummon();
            portals.clear();

            if (Creature* malgath = SummonMalgath(false))
                malgath->AI()->DoAction(uiStep < 2 ? ACTION_MALGATH_OPEN_CELL : ACTION_MALGATH_FIGHT);
            else if (uiStep < 2)
                StartBossEncounter(GetStepBoss());
            else
            {
                uiStepPhase = STEP_PHASE_FINAL_BOSS;
                uiFinalBossTimer = 5000;
            }
        }

        void StartFinalBoss()
        {
            if (uiFinalBoss == DATA_BETRUG)
            {
                if (Creature* betrug = instance->GetCreature(uiBetrug))
                    betrug->AI()->DoAction(1);
            }
            else
                StartBossEncounter(DATA_SAELORN);
        }

        void StartBossEncounter(uint8 uiBoss)
        {
            if (uiBoss >= DATA_BETRUG)
                return;

            HandleGameObject(GetGuidData(DATA_KAAHRJ_CELL + uiBoss), true);

            Creature* pBoss = instance->GetCreature(GetGuidData(uiBoss));
            if (!pBoss)
                return;

            if (pBoss->isDead())
            {
                // respawn but avoid to be looted again
                pBoss->Respawn();
                pBoss->AI()->DoAction(ACTION_REMOVE_LOOT);
            }

            pBoss->SetHomePosition(centrPos);
            pBoss->GetMotionMaster()->MovePoint(1, bossStartMove[uiBoss]);
            if (uiBoss == DATA_MILLIFICENT) //manastorm event
                pBoss->AI()->DoAction(1);
            else
            {
                pBoss->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_NON_ATTACKABLE);
                pBoss->SetReactState(REACT_AGGRESSIVE);
            }
        }

        // Sends a released prisoner (or the hidden Betrug) back to its place; a defeated one stays dead
        void ResetBoss(uint8 uiBoss)
        {
            if (uiBoss >= MAX_ENCOUNTER || GetBossState(uiBoss) == DONE)
                return;

            if (uiBoss != DATA_BETRUG)
                HandleGameObject(GetGuidData(DATA_KAAHRJ_CELL + uiBoss), false);

            Creature* pBoss = instance->GetCreature(GetGuidData(uiBoss));
            if (!pBoss)
                return;

            if (pBoss->isDead())
                pBoss->Respawn();
            else if (pBoss->isInCombat())
                pBoss->AI()->EnterEvadeMode();

            pBoss->SetReactState(REACT_PASSIVE);
            pBoss->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);

            // the home position was moved to the middle of the hold when it was released
            float x, y, z, o;
            pBoss->GetRespawnPosition(x, y, z, &o);
            if (uiBoss == DATA_BETRUG)
                o = 2.99f;
            pBoss->SetHomePosition(x, y, z, o);
            pBoss->NearTeleportTo(x, y, z, o);

            if (uiBoss == DATA_BETRUG)
                pBoss->SetVisible(false);
        }

        std::string GetSaveData() override
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << "V H " << (uint16)m_auiEncounter[0]
                << ' ' << (uint16)m_auiEncounter[1]
                << ' ' << (uint16)m_auiEncounter[2]
                << ' ' << (uint16)uiFirstBoss
                << ' ' << (uint16)uiSecondBoss
                << ' ' << (uint16)uiFinalBoss;

            str_data = saveStream.str();

            OUT_SAVE_INST_DATA_COMPLETE;
            return str_data;
        }

        void Load(const char* in) override
        {
            if (!in)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(in);

            char dataHead1, dataHead2;
            uint16 data0 = 0, data1 = 0, data2 = 0, data3 = BOSS_NOT_DRAWN, data4 = BOSS_NOT_DRAWN, data5 = BOSS_NOT_DRAWN;

            std::istringstream loadStream(in);
            loadStream >> dataHead1 >> dataHead2 >> data0 >> data1 >> data2 >> data3 >> data4 >> data5;

            if (dataHead1 == 'V' && dataHead2 == 'H')
            {
                m_auiEncounter[0] = data0;
                m_auiEncounter[1] = data1;
                m_auiEncounter[2] = data2;

                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    if (m_auiEncounter[i] == IN_PROGRESS)
                        m_auiEncounter[i] = NOT_STARTED;

                uiFirstBoss = uint8(data3);
                uiSecondBoss = uint8(data4);
                uiFinalBoss = uint8(data5);

                uiStep = 0;
                while (uiStep < 2 && m_auiEncounter[uiStep] == DONE)
                    ++uiStep;

                if (m_auiEncounter[2] == DONE)
                    uiMainEventPhase = DONE;

                // the intro only plays before the first start
                bIntroDone = uiStep > 0;
            } else OUT_LOAD_INST_DATA_FAIL;

            OUT_LOAD_INST_DATA_COMPLETE;
        }

        // The seal broke: the current step is replayed from its start, the steps already won are kept
        void Reset_Event()
        {
            uiMainEventPhase = NOT_STARTED;
            uiDoorIntegrity = 100;
            DoUpdateWorldState(WorldStates::WORLD_STATE_VH, 0);
            DoUpdateWorldState(WorldStates::WORLD_STATE_VH_PRISON_STATE, uiDoorIntegrity);

            ResetBoss(GetStepBoss());
            SetData(DATA_MAIN_DOOR, GO_STATE_ACTIVE);

            for (int i = 0; i < 4; ++i)
                if (GameObject* crystal = instance->GetGameObject(uiActivationCrystal[i]))
                    crystal->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);

            std::set<ObjectGuid> tempMobs(trashMobs);
            for (auto itr = tempMobs.begin(); itr != tempMobs.end(); ++itr)
            {
                if (Creature* creature = instance->GetCreature(*itr))
                    if (creature && creature->IsAlive())
                        creature->DespawnOrUnsummon();
            }

            trashMobs.clear();
            portals.clear();
            StartStep(PORTAL_REOPEN);

            if (Scenario* scenario = GetScenario())
                scenario->ResetStepCriteria(uiStep);

            if (Creature* pSinclari = instance->GetCreature(uiSinclari))
            {
                pSinclari->SetVisible(true);
                std::list<Creature*> GuardList;
                pSinclari->GetCreatureListWithEntryInGrid(GuardList, NPC_VIOLET_HOLD_GUARD, 300.0f);
                if (!GuardList.empty())
                {
                    for (std::list<Creature*>::const_iterator itr = GuardList.begin(); itr != GuardList.end(); ++itr)
                    {
                        if (Creature* pGuard = *itr)
                        {
                            // back at their post, the way they stood before the start
                            pGuard->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NOT_SELECTABLE);
                            pGuard->GetMotionMaster()->Clear();
                            pGuard->NearTeleportTo(pGuard->GetHomePosition());
                            pGuard->SetVisible(true);
                            pGuard->SetReactState(REACT_AGGRESSIVE);
                        }
                    }
                }
                pSinclari->GetMotionMaster()->MovePoint(1, pSinclari->GetHomePosition());
                pSinclari->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                pSinclari->AI()->DoAction(true);
            }
        }

        // Before the start, demons pour out of the three intro portals and fight the prison guards
        void UpdateIntro(uint32 diff)
        {
            if (uiIntroTimer > diff)
            {
                uiIntroTimer -= diff;
                return;
            }

            // a demon every 15-20 seconds once each portal has sent its first one
            uiIntroTimer = urand(15000, 20000);

            for (auto itr = introMobs.begin(); itr != introMobs.end();)
            {
                Creature* demon = instance->GetCreature(*itr);
                if (!demon || !demon->IsAlive())
                    itr = introMobs.erase(itr);
                else
                    ++itr;
            }

            // the players arrive on a fight already going on: one demon per portal at once, then one at a time
            std::vector<ObjectGuid> portalList(introPortals.begin(), introPortals.end());
            // the portal model is small at its own size: Portal Periodic, which Malgath channels on the wave portals,
            // scales it up 300% into the open portal
            for (ObjectGuid const& guid : portalList)
                if (Creature* portal = instance->GetCreature(guid))
                    if (!portal->HasAura(SPELL_PORTAL_PERIODIC))
                        portal->AddAura(SPELL_PORTAL_PERIODIC, portal);
            uint32 toSend = introMobs.empty() ? uint32(portalList.size()) : uint32(introMobs.size() < portalList.size() * 2);
            for (uint32 i = 0; i < toSend && !portalList.empty(); ++i)
                if (Creature* portal = instance->GetCreature(introMobs.empty() || toSend > 1 ? portalList[i] : portalList[urand(0, portalList.size() - 1)]))
                    if (Creature* demon = portal->SummonCreature(introDemonEntries[urand(0, 2)], portal->GetPosition(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000))
                        introMobs.insert(demon->GetGUID());

            // the guards hold the line until the players take over
            if (Creature* pSinclari = instance->GetCreature(uiSinclari))
            {
                std::list<Creature*> GuardList;
                pSinclari->GetCreatureListWithEntryInGrid(GuardList, NPC_VIOLET_HOLD_GUARD, 300.0f);
                for (Creature* pGuard : GuardList)
                    if (pGuard->IsAlive() && pGuard->HealthBelowPct(30))
                        pGuard->SetFullHealth();
            }
        }

        void Update(uint32 diff) override
        {
            if (!instance->HavePlayers())
                return;

            if (uiMainEventPhase == NOT_STARTED && !bIntroDone)
                UpdateIntro(diff);

            if (uiMainEventPhase != IN_PROGRESS)
                return;

            if (!uiDoorIntegrity)
            {
                Reset_Event();
                return;
            }

            switch (uiStepPhase)
            {
                case STEP_PHASE_WAVES:
                {
                    if (uiCheckTimer <= diff)
                    {
                        uiCheckTimer = 1000;
                        uint32 goal = uiStep < 2 ? INVASION_FORCES_POINTS : INVASION_FORCES_POINTS - MALGATH_POINTS;
                        if (GetInvasionPoints() >= goal)
                        {
                            EndWaves();
                            break;
                        }
                    }
                    else
                        uiCheckTimer -= diff;

                    if (uiPortalTimer <= diff)
                    {
                        if (portals.size() < MAX_OPEN_PORTALS)
                            OpenPortal();
                        uiPortalTimer = PORTAL_INTERVAL;
                    }
                    else
                        uiPortalTimer -= diff;
                    break;
                }
                case STEP_PHASE_FINAL_BOSS:
                    if (uiFinalBossTimer)
                    {
                        if (uiFinalBossTimer <= diff)
                        {
                            uiFinalBossTimer = 0;
                            StartFinalBoss();
                        }
                        else
                            uiFinalBossTimer -= diff;
                    }
                    break;
                default:
                    break;
            }
        }

        void ActivateCrystal()
        {
            // just to make things easier we'll get the gameobject from the map
            GameObject* invoker = instance->GetGameObject(uiActivationCrystal[0]);
            if (!invoker)
                return;

            // the orb
            TempSummon* trigger = invoker->SummonCreature(NPC_DEFENSE_SYSTEM, MiddleRoomSaboLoc, TEMPSUMMON_TIMED_DESPAWN, 5000);
            if (!trigger)
                return;

            // visuals
            trigger->CastSpell(trigger, SPELL_ARCANE_LIGHTNING, true, 0, 0, trigger->GetGUID());

            // Kills every normal invader in the hold; Malgath and the prisoners are left alone
            std::set<ObjectGuid> tempMobs(trashMobs);
            for (auto itr = tempMobs.begin(); itr != tempMobs.end(); ++itr)
            {
                Creature* creature = instance->GetCreature(*itr);
                if (creature && creature->IsAlive() && creature->GetEntry() != NPC_LORD_MALGATH)
                    trigger->Kill(creature);
            }
        }

        // Sinclari fires the intro crystal: the defense system wipes out the demons fighting the guards
        void ActivateIntroCrystal()
        {
            if (bIntroDone)
                return;

            bIntroDone = true;

            TempSummon* trigger = nullptr;
            if (GameObject* crystal = instance->GetGameObject(uiIntroCrystal))
                trigger = crystal->SummonCreature(NPC_DEFENSE_SYSTEM, MiddleRoomSaboLoc, TEMPSUMMON_TIMED_DESPAWN, 5000);

            if (trigger)
                trigger->CastSpell(trigger, SPELL_ARCANE_LIGHTNING, true, 0, 0, trigger->GetGUID());

            for (ObjectGuid const& guid : introMobs)
                if (Creature* demon = instance->GetCreature(guid))
                    if (demon->IsAlive())
                    {
                        if (trigger)
                            trigger->Kill(demon);
                        else
                            demon->DespawnOrUnsummon();
                    }

            introMobs.clear();

            for (ObjectGuid const& guid : introPortals)
                if (Creature* portal = instance->GetCreature(guid))
                    portal->SetVisible(false);
        }

        void ProcessEvent(WorldObject* source, uint32 uiEventId) override
        {
            switch (uiEventId)
            {
                case EVENT_ACTIVATE_CRYSTAL:
                {
                    // each crystal works once per instance
                    GameObject* used = source ? source->ToGameObject() : nullptr;
                    if (!used && source)
                        used = source->FindNearestGameObject(GO_ACTIVATION_CRYSTAL, 10.0f);
                    if (used)
                    {
                        if (used->HasFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE))
                            break;
                        used->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                        usedCrystals.insert(used->GetGUID());
                    }
                    ActivateCrystal();
                    break;
                }
            }
        }
    };
};

void AddSC_instance_violet_hold_legion()
{
    new instance_violet_hold_legion();
}
