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

#include "ScenarioMgr.h"
#include "LFGMgr.h"
#include "Log.h"
#include "WorldSession.h"

ScenarioMgr::ScenarioMgr()
{
    m_stepVector.assign(sScenarioStore.GetNumRows() + 1, nullptr);

    for (ScenarioStepEntry const* entry : sScenarioStepStore)
    {
        if (!sScenarioStore.LookupEntry(entry->ScenarioID))
            continue;

        if (HasAffixesTeeming(entry->Criteriatreeid)) // Affixes::Teeming
        {
            uint8 orderIndex = 0/*entry->OrderIndex*/; // allways step 0
            if (m_stepTeemingMap[entry->ScenarioID].size() <= orderIndex)
                m_stepTeemingMap[entry->ScenarioID].resize(orderIndex + 1);

            m_stepTeemingMap[entry->ScenarioID][orderIndex] = entry;
            continue;
        }

        if (m_stepMap[entry->ScenarioID].size() <= entry->OrderIndex)
            m_stepMap[entry->ScenarioID].resize(entry->OrderIndex + 1);

        m_stepMap[entry->ScenarioID][entry->OrderIndex] = entry;
        m_stepVector[entry->ScenarioID] = &m_stepMap[entry->ScenarioID];
    }
}

ScenarioMgr::~ScenarioMgr()
{
    for (auto v : _scenarioStore)
        delete v.second;
}

void ScenarioMgr::UnloadAll()
{
    std::shared_lock<std::shared_mutex> lock(_scenarioLock);
    for (auto v : _scenarioStore)
        v.second->GetAchievementMgr().ClearMap();
}

ScenarioMgr* ScenarioMgr::instance()
{
    static ScenarioMgr instance;
    return &instance;
}

void ScenarioMgr::RemoveScenario(uint32 instanceId)
{
    Scenario* scenario;
    {
        // waits for the WithScenario readers; nobody can find it once it is out of the store
        std::unique_lock<std::shared_mutex> lock(_scenarioLock);
        ScenarioMap::iterator itr = _scenarioStore.find(instanceId);
        if (itr == _scenarioStore.end())
            return;

        scenario = itr->second;
        _scenarioStore.erase(itr);
    }

    // outside the lock: the destructor ends the challenge, which posts to players and calls other managers
    delete scenario;
}

ScenarioSteps const* ScenarioMgr::GetScenarioSteps(uint32 scenarioId, bool Teeming)
{
    if (Teeming)
        if (ScenarioSteps const* steps = Trinity::Containers::MapGetValuePtr(m_stepTeemingMap, scenarioId))
            return steps;
    return m_stepVector[scenarioId];
}

Scenario* ScenarioMgr::AddScenario(Map* map, lfg::LFGDungeonData const* dungeonData, Player* player, bool find)
{
    // only the thread of the map adds its scenario, so the check cannot be outrun; built outside the lock
    {
        std::shared_lock<std::shared_mutex> lock(_scenarioLock);
        if (_scenarioStore.find(map->GetInstanceId()) != _scenarioStore.end())
            return nullptr;
    }

    Scenario* scenario = new Scenario(map, dungeonData, player, find);
    {
        std::unique_lock<std::shared_mutex> lock(_scenarioLock);
        _scenarioStore[map->GetInstanceId()] = scenario;
    }

    map->m_scenarios.insert(scenario);
    scenario->RestoreSavedStep(player);
    return scenario;
}

Scenario* ScenarioMgr::AddScenario(Map* map, uint32 scenarioId)
{
    ScenarioSteps const* steps = GetScenarioSteps(scenarioId);
    if (!map || !steps || steps->empty())
        return nullptr;

    {
        std::shared_lock<std::shared_mutex> lock(_scenarioLock);
        ScenarioMap::const_iterator itr = _scenarioStore.find(map->GetInstanceId());
        if (itr != _scenarioStore.end())
        {
            // the store is keyed by instance id only, which a garrison (owner's guid) can share with a dungeon
            if (itr->second->GetMap() != map)
                TC_LOG_ERROR("scenario", "ScenarioMgr::AddScenario: scenario %u not started on map %u instance %u, the id is held by scenario %u of another map",
                    scenarioId, map->GetId(), map->GetInstanceId(), itr->second->GetScenarioId());
            return nullptr;
        }
    }

    Scenario* scenario = new Scenario(map, scenarioId);
    // the medals of a garrison invasion are bonus steps walked after its last main step
    scenario->SetCompleteThroughBonusSteps(true);
    {
        std::unique_lock<std::shared_mutex> lock(_scenarioLock);
        _scenarioStore[map->GetInstanceId()] = scenario;
    }

    map->m_scenarios.insert(scenario);
    return scenario;
}

void ScenarioMgr::RemoveScenario(Map* map)
{
    if (!map)
        return;

    Scenario* scenario;
    {
        std::unique_lock<std::shared_mutex> lock(_scenarioLock);
        ScenarioMap::iterator itr = _scenarioStore.find(map->GetInstanceId());
        if (itr == _scenarioStore.end())
            return;

        // another map with the same instance id owns it; one built without a map belongs to nobody else either
        if (itr->second->GetMap() && itr->second->GetMap() != map)
            return;

        scenario = itr->second;
        _scenarioStore.erase(itr);
    }

    // the map updates its scenarios every tick: it must forget this one before it is deleted
    map->m_scenarios.erase(scenario);
    delete scenario;
}

Scenario* ScenarioMgr::GetScenario(uint32 instanceId)
{
    std::shared_lock<std::shared_mutex> lock(_scenarioLock);
    return Trinity::Containers::MapGetValuePtr(_scenarioStore, instanceId);
}

bool ScenarioMgr::HasScenarioStep(lfg::LFGDungeonData const* _dungeonData, Player* player)
{
    uint32 scenarioId = 0;
    if (player->GetScenarioId())
        scenarioId = player->GetScenarioId();
    else if (ScenarioData const* scenarioData = sObjectMgr->GetScenarioOnMap(_dungeonData->map, _dungeonData->difficulty, player->GetTeam(), player->getClass(), _dungeonData->id))
        scenarioId = scenarioData->ScenarioID;
    else if (_dungeonData->dbc->ScenarioID)
        scenarioId = _dungeonData->dbc->ScenarioID;

    if (!scenarioId)
        return false;

    return m_stepVector[scenarioId] != nullptr;
}

bool ScenarioMgr::HasAffixesTeeming(uint16 CriteriaTreeID)
{
    switch (CriteriaTreeID)
    {
        case 47415:
        case 50599:
        case 51246:
        case 51710:
        case 51880:
        case 52278:
        case 52327:
        case 52427:
        case 52471:
        case 57810:
        case 57866:
        case 58715:
        case 60933:
            return true;
    }

    return false;
}
