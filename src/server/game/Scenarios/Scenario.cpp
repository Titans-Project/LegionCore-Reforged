////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
///      Based on TrinityCore code       ///
///   Copyright 2008-2014 TrinityCore    ///
///   <http://www.trinitycore.org/>      ///
///                                      ///
////////////////////////////////////////////

#include "Group.h"
#include "ScenarioMgr.h"
#include "LFGMgr.h"
#include "InstanceSaveMgr.h"
#include "MapInstanced.h"
#include "WorldSession.h"
#include "ScenarioPackets.h"
#include "InstanceScript.h"
#include "QuestData.h"
#include "OutdoorPvP.h"
#include "Log.h"
#include "DatabaseEnv.h"
#include "GameTime.h"
#include "ObjectMgr.h"
#include <algorithm>

namespace
{
    // steps are indexed by OrderIndex, so the vector can hold holes
    ScenarioStepEntry const* GetStepAt(ScenarioSteps const& steps, size_t index)
    {
        return index < steps.size() ? steps[index] : nullptr;
    }

    void AddActiveStep(std::vector<uint32>& activeSteps, ScenarioStepEntry const* step)
    {
        if (step && std::find(activeSteps.begin(), activeSteps.end(), step->ID) == activeSteps.end())
            activeSteps.push_back(step->ID);
    }
}

Scenario::Scenario(Map* map, lfg::LFGDungeonData const* _dungeonData, Player* player, bool find) : m_achievementMgr(this)
{
    if (!map)
        return;

    curMap = map;
    instanceId = map->GetInstanceId();
    dungeonData = _dungeonData;
    currentStep = 0;
    currentTree = 0;
    bonusRewarded = false;
    rewarded = false;
    scenarioId = dungeonData->dbc->ScenarioID;
    _challenge = nullptr;
    hasbonus = false;

    if (player->GetScenarioId())
    {
        scenarioId = player->GetScenarioId();
        find = true;
        player->SetScenarioId(0);
    }

    if (!find)
        if (ScenarioData const* scenarioData = sObjectMgr->GetScenarioOnMap(dungeonData->map, map->GetDifficultyID(), player->GetTeam(), player->getClass(), dungeonData->id))
            scenarioId = scenarioData->ScenarioID;

    _scenarioEntry = sScenarioStore.LookupEntry(scenarioId);
    if (!_scenarioEntry)
    {
        //ASSERT(_scenarioEntry);
        // TC_LOG_DEBUG("challenge", "Scenario::Scenario instanceId %u scenarioId %u dungeonData %u", instanceId, scenarioId, dungeonData->map);
        return;
    }

    // TC_LOG_DEBUG("challenge", "Scenario::Scenario instanceId %u scenarioId %u dungeonData %u", instanceId, scenarioId, dungeonData->map);

    ScenarioSteps const* _steps = sScenarioMgr->GetScenarioSteps(scenarioId);
    ASSERT(_steps);

    steps = *_steps;
    currentTree = GetScenarioCriteriaByStep(currentStep);
    ActiveSteps.push_back(steps[currentStep]->ID);

    for (auto const& step : steps)
    {
        SetStepState(step, SCENARIO_STEP_NOT_STARTED);
        if (step->IsBonusObjective())
            hasbonus = true;
    }

    if (curMap->isChallenge())
        CreateChallenge(player);

    objectCountInWorld[uint8(HighGuid::Scenario)]++;
}

Scenario::Scenario(Map* map, uint32 _scenarioId) : m_achievementMgr(this)
{
    if (!map)
        return;

    curMap = map;
    instanceId = map->GetInstanceId();
    dungeonData = nullptr;
    currentStep = 0;
    currentTree = 0;
    bonusRewarded = false;
    rewarded = false;
    scenarioId = _scenarioId;
    _challenge = nullptr;
    hasbonus = false;

    _scenarioEntry = sScenarioStore.LookupEntry(scenarioId);
    if (!_scenarioEntry)
    {
        //ASSERT(_scenarioEntry);
        // TC_LOG_DEBUG("challenge", "Scenario::Scenario instanceId %u scenarioId %u dungeonData %u", instanceId, scenarioId, dungeonData->map);
        return;
    }

    // TC_LOG_DEBUG("challenge", "Scenario::Scenario instanceId %u scenarioId %u dungeonData %u", instanceId, scenarioId, dungeonData->map);

    ScenarioSteps const* _steps = sScenarioMgr->GetScenarioSteps(scenarioId);
    ASSERT(_steps);

    steps = *_steps;
    currentTree = GetScenarioCriteriaByStep(currentStep);
    ActiveSteps.push_back(steps[currentStep]->ID);

    for (auto const& step : steps)
    {
        SetStepState(step, SCENARIO_STEP_NOT_STARTED);
        if (step->IsBonusObjective())
            hasbonus = true;
    }

    objectCountInWorld[uint8(HighGuid::Scenario)]++;
}

Scenario::~Scenario()
{
    delete _challenge;
    _challenge = nullptr;

    objectCountInWorld[uint8(HighGuid::Scenario)]--;
}

void Scenario::Update(const uint32 t_diff)
{
    m_achievementMgr.UpdateTimedAchievements(t_diff);
}

void Scenario::CreateChallenge(Player* player)
{
    // TC_LOG_DEBUG("challenge", "CreateChallenge player %u map %u", bool(player), bool(GetMap()));

    Map* map = GetMap();
    if (!player || !map)
        return;

    InstanceMap* instanceMap = map->ToInstanceMap();
    ASSERT(instanceMap);

    MapChallengeModeEntry const* m_challengeEntry = player->GetGroup() ? player->GetGroup()->GetChallengeInfo().Entry : player->m_challengeKeyInfo.challengeEntry;
    if (!m_challengeEntry)
        return;

    _challenge = new Challenge(instanceMap, player, GetInstanceId(), this);

    // TC_LOG_DEBUG("challenge", "CreateChallenge _challenge %u _canRun %u", bool(_challenge), bool(_challenge->_canRun));

    // a start that failed (no key, not a challenge map) must not stay: the creature scaling, the loot and the group
    // code would all read it as a key being run
    if (!_challenge->_canRun)
    {
        delete _challenge;
        _challenge = nullptr;
        return;
    }

    if (InstanceScript* script = instanceMap->GetInstanceScript())
    {
        script->SetChallenge(_challenge);
        _challenge->SetInstanceScript(script);
    }

    if (ScenarioData const* scenarioData = sObjectMgr->GetScenarioOnMap(map->GetId(), DIFFICULTY_MYTHIC_KEYSTONE))
        scenarioId = scenarioData->ScenarioID;

    switch (m_challengeEntry->ID)
    {
        case 197: // Eye of Azshara
            scenarioId = 1169;
            break;
        case 198: // Darkheart Thicket
            scenarioId = 1172;
            break;
        case 199: // Black Rook Hold
            scenarioId = 1166;
            break;
        case 200: // Halls of Valor
            scenarioId = 1046;
            break;
        case 206: // Neltharion's Lair
            scenarioId = 1174;
            break;
        case 207: // Vault of the Wardens
            scenarioId = 1173;
            break;
        case 208: // Maw of Souls
            scenarioId = 1175;
            break;
        case 209: // The Arcway
            scenarioId = 1177;
            break;
        case 210: // Court of Stars
            scenarioId = 1178;
            break;
        case 227: // Return to Karazhan: Lower
            scenarioId = 1309;
            break;
        case 233: // Cathedral of Eternal Night
            scenarioId = 1335;
            break;
        case 234: // Return to Karazhan: Upper
            scenarioId = 1308;
            break;
        default:
            break;
    }

    _scenarioEntry = sScenarioStore.LookupEntry(scenarioId);
    ASSERT(_scenarioEntry);

    ScenarioSteps const* _steps = sScenarioMgr->GetScenarioSteps(scenarioId, _challenge->HasAffix(Affixes::Teeming));
    ASSERT(_steps);

    currentStep = 0;
    steps = *_steps;
    if (steps.size() <= currentStep)
    {
        // TC_LOG_DEBUG("challenge", "CreateChallenge steps %u currentStep %u", steps.size(), currentStep);
        return;
    }

    currentTree = GetScenarioCriteriaByStep(currentStep);
    ActiveSteps.clear();
    ActiveSteps.push_back(steps[currentStep]->ID);

    for (auto const& step : steps)
        SetStepState(step, SCENARIO_STEP_NOT_STARTED);

    SetCurrentStep(0);

    // TC_LOG_ERROR("challenge", "%s %u, mapID: %u, scenarioID: %u", __FUNCTION__, __LINE__, map->GetId(), scenarioId);
}

uint32 Scenario::GetInstanceId() const
{
    return instanceId;
}

Map* Scenario::GetMap()
{
    return curMap;
}

OutdoorPvP* Scenario::GetOutdoorPvP()
{
    return m_outdoorPvp;
}

uint32 Scenario::GetOutdoorPvPZone() const
{
    return m_outdoorPvpZone;
}


uint32 Scenario::GetScenarioId() const
{
    return scenarioId;
}

uint32 Scenario::GetCurrentStep() const
{
    return currentStep;
}

void Scenario::SetStepState(ScenarioStepEntry const* step, ScenarioStepState state)
{
    _stepStates[step] = state;
}

bool Scenario::IsCompleted(bool bonus) const
{
    return currentStep == GetStepCount(bonus);
}

uint8 Scenario::GetStepCount(bool withBonus) const
{
    if (withBonus)
        return steps.size();

    uint8 count = 0;
    for (auto const& v : steps)
        if (!v->IsBonusObjective())
            ++count;

    return count;
}

bool Scenario::KeepsProgress() const
{
    // quest scenarios only: not a dungeon's boss list, a keystone or a garrison invasion
    return curMap && dungeonData && curMap->IsScenario() && !curMap->isChallenge();
}

void Scenario::SaveStep() const
{
    if (_restoringStep || !KeepsProgress())
        return;

    if (IsCompleted(false))
    {
        curMap->ApplyOnEveryPlayer([this](Player* player) { ForgetSavedStep(player); });
        return;
    }

    uint32 const id = scenarioId;
    uint8 const step = currentStep;
    curMap->ApplyOnEveryPlayer([id, step](Player* player)
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CHARACTER_SCENARIO_STEP);
        stmt->setUInt64(0, player->GetGUIDLow());
        stmt->setUInt32(1, id);
        stmt->setUInt8(2, step);
        CharacterDatabase.Execute(stmt);
    });
}

Scenario* Scenario::KeepingProgressOn(Map* map)
{
    if (!map || !map->IsScenario())
        return nullptr;

    Scenario* scenario = sScenarioMgr->GetScenario(map->GetInstanceId());
    return scenario && scenario->GetMap() == map && scenario->KeepsProgress() ? scenario : nullptr;
}

void Scenario::RecordDoneSpawn(uint8 type, uint64 spawn)
{
    if (!spawn || !KeepsProgress())
        return;

    uint32 const id = scenarioId;
    uint8 const step = currentStep;
    curMap->ApplyOnEveryPlayer([id, step, type, spawn](Player* player)
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CHARACTER_SCENARIO_SPAWN);
        stmt->setUInt64(0, player->GetGUIDLow());
        stmt->setUInt32(1, id);
        stmt->setUInt8(2, type);
        stmt->setUInt64(3, spawn);
        stmt->setUInt8(4, step);
        CharacterDatabase.Execute(stmt);
    });
}

// before the grids load: what was killed in a finished step does not come back, what was used is set as it was left
void Scenario::RestoreDoneSpawns(Player* player, uint8 step)
{
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SCENARIO_SPAWNS);
    stmt->setUInt64(0, player->GetGUIDLow());
    stmt->setUInt32(1, scenarioId);
    stmt->setUInt8(2, step);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);
    if (!result)
        return;

    time_t const gone = GameTime::GetGameTime() + WEEK;
    do
    {
        Field* fields = result->Fetch();
        uint8 const type = fields[0].GetUInt8();
        uint64 const spawn = fields[1].GetUInt64();

        if (type == 0)
        {
            if (sObjectMgr->GetCreatureData(spawn))
                curMap->SaveCreatureRespawnTime(spawn, gone);
            continue;
        }

        GameObjectData const* data = sObjectMgr->GetGOData(spawn);
        GameObjectTemplate const* goInfo = data ? sObjectMgr->GetGameObjectTemplate(data->id) : nullptr;
        if (!goInfo)
            continue;

        // an object taken away at use stays away; the others keep their used state (GameObject::LoadGameObjectFromDB)
        if (goInfo->GetDespawnPossibility() || goInfo->IsDespawnAtAction())
            curMap->SaveGORespawnTime(spawn, gone);
        else
            _doneGameObjects.insert(spawn);
    }
    while (result->NextRow());
}

void Scenario::ForgetSavedStep(Player* player) const
{
    if (!player || !KeepsProgress())
        return;

    for (CharacterDatabaseStatements index : { CHAR_DEL_CHARACTER_SCENARIO_STEP, CHAR_DEL_CHARACTER_SCENARIO_SPAWNS })
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(index);
        stmt->setUInt64(0, player->GetGUIDLow());
        stmt->setUInt32(1, scenarioId);
        CharacterDatabase.Execute(stmt);
    }
}

void Scenario::RestoreSavedStep(Player* player)
{
    if (!player || !KeepsProgress() || steps.empty())
        return;

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SCENARIO_STEP);
    stmt->setUInt64(0, player->GetGUIDLow());
    stmt->setUInt32(1, scenarioId);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);
    if (!result)
        return;

    uint8 const step = (*result)[0].GetUInt8();
    if (step <= currentStep || step >= GetStepCount(false))
        return;

    // the steps before it are done; the one reached starts over from its beginning
    for (uint8 i = 0; i < step; ++i)
        if (ScenarioStepEntry const* done = GetStepAt(steps, i))
        {
            SetStepState(done, SCENARIO_STEP_DONE);
            AddActiveStep(ActiveSteps, done);
        }

    _restoringStep = true;
    SetCurrentStep(step);
    _restoringStep = false;

    if (ScenarioStepEntry const* reached = GetStepAt(steps, step))
        SetStepState(reached, SCENARIO_STEP_IN_PROGRESS);

    RestoreDoneSpawns(player, step);
}

void Scenario::SetCurrentStep(uint8 step)
{
    currentStep = step;
    currentTree = GetScenarioCriteriaByStep(currentStep);
    SaveStep();

    SendStepUpdate();

    if (OutdoorPvP* outDoorPvP = GetOutdoorPvP())
        outDoorPvP->SetData(GetOutdoorPvPZone(), currentStep);
    else if (Map *m = GetMap())
    {
        if (InstanceMap* i = m->ToInstanceMap())
        {
            if (InstanceScript *script = i->GetInstanceScript())
            {
                script->setScenarioStep(currentStep);
                script->onScenarionNextStep(currentStep);
                script->UpdatePhasing();
            }
            else
                i->UpdatePhasing(); // if InstanceScript is empty
        }
    }

    ActiveSteps.push_back(steps[currentStep]->ID);
}

void Scenario::UpdateCurrentStep(bool loading)
{
    // i_updateLock.lock();
    uint8 oldStep = currentStep;

    for (ScenarioSteps::const_iterator itr = steps.begin(); itr != steps.end(); ++itr)
    {
        //Not check if ctep already complete
        if (currentStep > (*itr)->OrderIndex)
            continue;

        CriteriaTreeEntry const* criteriaTree = sCriteriaTreeStore.LookupEntry((*itr)->Criteriatreeid);
        if (!criteriaTree)
            continue;

        if (GetAchievementMgr().IsCompletedScenarioTree(criteriaTree))
        {
            currentStep = (*itr)->OrderIndex + 1;
            currentTree = GetScenarioCriteriaByStep(currentStep);
        }
    }

    // TC_LOG_DEBUG("challenge", "UpdateCurrentStep currentStep %u oldStep %u loading %u steps %u", currentStep, oldStep, loading, steps.size());

    if (currentStep != oldStep && !loading)
    {
        if (currentStep != 0 && currentStep < steps.size())
        {
            if (OutdoorPvP* outDoorPvP = GetOutdoorPvP())
                outDoorPvP->SetData(GetOutdoorPvPZone(), currentStep);
            else if (Map *m = GetMap())
            {
                if (InstanceMap* i = m->ToInstanceMap())
                {
                    if (InstanceScript *script = i->GetInstanceScript())
                    {
                        script->setScenarioStep(currentStep);
                        script->onScenarionNextStep(currentStep);
                        script->UpdatePhasing();
                    }
                    else
                        i->UpdatePhasing(); // if InstanceScript is empty
                }
            }
            ActiveSteps.push_back(steps[currentStep]->ID);
        }

        SendStepUpdate();
        SaveStep();

        if (IsCompleted(false))
            Reward(false, oldStep);
        else if (hasbonus && IsCompleted(true))
            Reward(true, oldStep);
    }

    if (currentStep < steps.size())
        SetStepState(steps[currentStep], SCENARIO_STEP_IN_PROGRESS);

    SetStepState(steps[oldStep], SCENARIO_STEP_DONE);
    // i_updateLock.unlock();
    //TC_LOG_DEBUG("challenge", "UpdateCurrentStep currentStep %u oldStep %u loading %u", currentStep, oldStep, loading);
}

uint32 Scenario::GetScenarioCriteriaByStep(uint8 step)
{
    if (steps.empty())
        return 0;

    for (auto const& step_ : steps)
        if (step_ && step == step_->OrderIndex)
            return step_->Criteriatreeid;

    return 0;
}

void Scenario::ResetStepCriteria(uint8 step)
{
    if (CriteriaTree const* tree = sAchievementMgr->GetCriteriaTree(GetScenarioCriteriaByStep(step)))
        m_achievementMgr.ResetCriteriaTreeProgress(tree);
}

void Scenario::Reward(bool bonus, uint32 rewardStep)
{
    // TC_LOG_DEBUG("challenge", "Scenario::Reward bonus %u rewarded %u bonusRewarded %u rewardStep %u", bonus, rewarded, bonusRewarded, rewardStep);

    if (bonus && bonusRewarded)
        return;

    if (!bonus && rewarded)
        return;

    if (bonus)
        bonusRewarded = true;
    else
        rewarded = true;

    ObjectGuid groupGuid;
    Map* map = GetMap();
    OutdoorPvP* pvpMap = GetOutdoorPvP();
    // map not created? bye-bye reward
    if (!pvpMap && !map)
        return;

    Quest const* quest = sQuestDataStore->GetQuestTemplate(steps[rewardStep]->RewardQuestID);

    if (pvpMap)
    {
        pvpMap->ApplyOnEveryPlayerInZone([quest, this](Player* player)
        {
            if (player->CanContact())
            {

                if (quest)
                    player->AddDelayedEvent(100, [player, quest]() -> void { if (player) player->RewardQuest(quest, 0, nullptr, false); });

                player->UpdateAchievementCriteria(CRITERIA_TYPE_COMPLETE_SCENARIO, scenarioId, 1);
                player->UpdateAchievementCriteria(CRITERIA_TYPE_COMPLETE_SCENARIO_COUNT, 1);

                WorldPackets::Scenario::ScenarioCompleted data;
                data.ScenarioID = scenarioId;
                player->SendDirectMessage(data.Write());
            }
        }, GetOutdoorPvPZone());

        pvpMap->SetData(GetOutdoorPvPZone(), 100);
    }
    else if (map)
    {
        map->ApplyOnEveryPlayer([&groupGuid, this, quest](Player* player)
        {
            if (player->CanContact())
            {
                if (groupGuid.IsEmpty())
                    groupGuid = player->GetGroup() ? player->GetGroup()->GetGUID() : ObjectGuid::Empty;

                if (_challenge)
                    player->UpdateAchievementCriteria(CRITERIA_TYPE_COMPLETE_CHALLENGE, _challenge->GetChallengeLevel());

                if (quest)
                    player->AddDelayedEvent(100, [player, quest]() -> void { if (player) player->RewardQuest(quest, 0, nullptr, false); });

                player->UpdateAchievementCriteria(CRITERIA_TYPE_COMPLETE_SCENARIO, scenarioId, 1);
                player->UpdateAchievementCriteria(CRITERIA_TYPE_COMPLETE_SCENARIO_COUNT, 1);

                WorldPackets::Scenario::ScenarioCompleted data;
                data.ScenarioID = scenarioId;
                player->SendDirectMessage(data.Write());
            }
        });
    }

    // should not happen
    if (groupGuid.IsEmpty())
        return;

    // TC_LOG_DEBUG("challenge", "Reward Type %u", _scenarioEntry->Type);

    switch (_scenarioEntry->Type)
    {
        case SCENARIO_TYPE_DEFAULT:
        case SCENARIO_TYPE_USE_DUNGEON_DISPLAY:
        case SCENARIO_TYPE_BOOST_TUTORIAL:
            if (uint32 dungeonId = sLFGMgr->GetDungeon(groupGuid)) // lfg dungeons are rewarded through lfg
            {
                // lfg dungeon that we are in is not current scenario
                if (dungeonId != dungeonData->id)
                    return;

                sLFGMgr->FinishDungeon(groupGuid, dungeonId);
            }
            break;
        case SCENARIO_TYPE_CHALLENGE_MODE:
            if (_challenge)
                _challenge->Complete();
            break;
        case SCENARIO_TYPE_LEGION_INVASION: /// The Broken Shore scenario.
        default:
            break;
    }
}

AchievementMgr<Scenario>& Scenario::GetAchievementMgr()
{
    return m_achievementMgr;
}

AchievementMgr<Scenario> const& Scenario::GetAchievementMgr() const
{
    return m_achievementMgr;
}

ScenarioStepState Scenario::GetStepState(ScenarioStepEntry const* step)
{
    auto const itr = _stepStates.find(step);
    if (itr != _stepStates.end())
        return itr->second;

    return SCENARIO_STEP_INVALID;
}

std::vector<WorldPackets::Scenario::BonusObjectiveData> Scenario::GetBonusObjectivesData()
{
    std::vector<WorldPackets::Scenario::BonusObjectiveData> bonusObjectivesData;
    for (auto const& step : steps)
    {
        if (!step->IsBonusObjective())
            continue;

        if (sAchievementMgr->GetCriteriaTree(step->Criteriatreeid))
        {
            WorldPackets::Scenario::BonusObjectiveData bonusObjectiveData;
            bonusObjectiveData.BonusObjectiveID = step->ID;
            bonusObjectiveData.ObjectiveComplete = GetStepState(step) == SCENARIO_STEP_DONE;
            bonusObjectivesData.push_back(bonusObjectiveData);
        }
    }

    return bonusObjectivesData;
}

void Scenario::SendStepUpdate(Player* player, bool full)
{
    WorldPackets::Scenario::ScenarioState state;
    state.BonusObjectives = GetBonusObjectivesData();
    state.ScenarioID = GetScenarioId();
    state.CurrentStep = currentStep < steps.size() ? steps[currentStep]->ID : -1;
    state.ScenarioComplete = IsCompleted(false) || (_completeThroughBonusSteps && currentStep > GetStepCount(false));
    state.ActiveSteps = ActiveSteps;

    std::vector<ScenarioSpellData> const* scSpells = sObjectMgr->GetScenarioSpells(GetScenarioId());
    if (scSpells)
    {
        for (std::vector<ScenarioSpellData>::const_iterator itr = scSpells->begin(); itr != scSpells->end(); ++itr)
        {
           // if ((*itr).StepId == state.ActiveSteps)
            if ((*itr).StepId == GetCurrentStep())
            {
                WorldPackets::Scenario::ScenarioState::ScenarioSpellUpdate spellUpdate;
                spellUpdate.Usable = true;
                spellUpdate.SpellID = (*itr).Spells;
                state.Spells.emplace_back(spellUpdate);
            }
        }
    }

    if (full)
    {
        CriteriaProgressMap const* progressMap = GetAchievementMgr().GetCriteriaProgressMap();
        if (!progressMap->empty())
        {
            for (CriteriaProgressMap::const_iterator itr = progressMap->begin(); itr != progressMap->end(); ++itr)
            {
                CriteriaProgress const& treeProgress = itr->second;
                CriteriaTreeEntry const* criteriaTreeEntry = sCriteriaTreeStore.LookupEntry(itr->first);
                if (!criteriaTreeEntry)
                    continue;

                WorldPackets::Achievement::CriteriaTreeProgress progress;
                progress.Id = criteriaTreeEntry->CriteriaID;
                progress.Quantity = treeProgress.Counter;
                progress.Player = ObjectGuid::Create<HighGuid::Scenario>(0, GetScenarioId(), 1); // whats the fuck ?
                progress.Flags = 0;
                progress.Date = GameTime::GetGameTime() - treeProgress.date;
                progress.TimeFromStart = GameTime::GetGameTime() - treeProgress.date;
                progress.TimeFromCreate = GameTime::GetGameTime() - treeProgress.date;
                state.Progress.push_back(progress);
            }
        }
    }

    if (player)
        player->SendDirectMessage(state.Write());
    else
        BroadCastPacket(state.Write());

    if (full && _challenge)
    {
        _challenge->SendChallengeModeStart(player);
        _challenge->SendStartElapsedTimer(player);
    }
}

void Scenario::SendFinishPacket(Player * player)
{
    WorldPackets::Scenario::ScenarioState state;
    state.BonusObjectives = GetBonusObjectivesData();
    state.ScenarioID = GetScenarioId();
    state.CurrentStep = currentStep < steps.size() ? steps[currentStep]->ID : -1;
    state.ScenarioComplete = true;
    state.ActiveSteps = ActiveSteps;
    player->SendDirectMessage(state.Write());
}

void Scenario::SendCriteriaUpdate(CriteriaProgress const* progress, uint32 timeElapsed /* = 0*/)
{
    //TC_LOG_DEBUG("challenge", "Scenario::SendCriteriaUpdate criteria %u Counter %u", progress->criteriaTree->criteria, progress->Counter);

    if (!progress || !progress->criteriaTree)
        return;

    WorldPackets::Scenario::ScenarioProgressUpdate update;

    WorldPackets::Achievement::CriteriaTreeProgress& progressUpdate = update.Progress;
    progressUpdate.Id = progress->criteriaTree->CriteriaID;
    progressUpdate.Quantity = progress->Counter;
    progressUpdate.Player = progress->PlayerGUID;
    progressUpdate.Flags = 0;
    progressUpdate.Date = progress->date;
    progressUpdate.TimeFromStart = timeElapsed;
    progressUpdate.TimeFromCreate = timeElapsed;
    BroadCastPacket(update.Write());
}

void Scenario::BroadCastPacket(const WorldPacket* data)
{
    if (m_outdoorPvp)
    {
        m_outdoorPvp->BroadcastPacketByZone(*data, m_outdoorPvpZone);
        return;
    }

    // scenarios built without dungeon data have no map to look up (the dereference crashed); one a script runs on its
    // own instance (garrison invasion) is only driven from that map's thread, so its players are reached directly
    if (!dungeonData)
    {
        if (curMap && curMap->Instanceable())
            curMap->SendToPlayers(data);
        return;
    }

    // the instance may be destroyed meanwhile by its parent map's thread: sent under the instance list lock
    Map* baseMap = sMapMgr->FindBaseMap(dungeonData->map);
    if (!baseMap)
        return;

    if (baseMap->Instanceable() || baseMap->CanCreatedZone())
        static_cast<MapInstanced*>(baseMap)->SendToInstancePlayers(instanceId, data);
    else if (!instanceId && !baseMap->IsMapUnload())
        baseMap->SendToPlayers(data);
}

bool Scenario::CanUpdateCriteria(uint32 criteriaId, uint32 recursTree /*=0*/) const
{
    auto const& cTreeList = sDB2Manager.GetCriteriaTreeList(recursTree ? recursTree : currentTree);
    if (!cTreeList)
        return false;

    for (std::vector<CriteriaTreeEntry const*>::const_iterator itr = cTreeList->begin(); itr != cTreeList->end(); ++itr)
    {
        if (CriteriaTreeEntry const* criteriaTree = *itr)
        {
            if (criteriaTree->CriteriaID == 0)
            {
                if (CanUpdateCriteria(criteriaId, criteriaTree->ID))
                    return true;
            }
            else if (criteriaTree->ID == criteriaId)
                return true;
        }
    }

    return false;
}

Challenge* Scenario::GetChallenge()
{
    return _challenge;
}

void Scenario::SetOutdoorPvP(OutdoorPvP * outdoor, uint32 zone)
{
    m_outdoorPvp = outdoor;
    m_outdoorPvpZone = zone;
}

void Scenario::UpdateAchievementCriteria(CriteriaTypes type, uint32 miscValue1 /*= 0*/, uint32 miscValue2 /*= 0*/, uint32 miscValue3 /*= 0*/, Unit* unit /*= NULL*/, Player* referencePlayer /*= NULL*/)
{
    AchievementCachePtr referenceCache = std::make_shared<AchievementCache>(referencePlayer, unit, type, miscValue1, miscValue2, miscValue3);

    GetAchievementMgr().UpdateAchievementCriteria(referenceCache);
}
