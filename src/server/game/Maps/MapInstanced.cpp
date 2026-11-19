////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
///      Based on TrinityCore code       ///
///   Copyright 2008-2012 TrinityCore    ///
///   <http://www.trinitycore.org/>      ///
///                                      ///
////////////////////////////////////////////

#include "Battleground.h"
#include "DatabaseEnv.h"
#include "GarrisonMap.h"
#include "Group.h"
#include "LFGMgr.h"
#include "InstanceSaveMgr.h"
#include "MapInstanced.h"
#include "MapManager.h"
#include "MMapFactory.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "VMapFactory.h"
#include "World.h"
#include "InstanceScript.h"
#include "ScenarioMgr.h"
#include "WorldStateMgr.h"

MapInstanced::MapInstanced(uint32 id, time_t expiry) : Map(id, expiry, 0, DIFFICULTY_NORMAL)
{
    // initialize instanced maps list
    m_InstancedMaps.clear();
    m_GarrisonedMaps.clear();

    _zoneThreads.clear();
    // fill with zero
    memset(&GridMapReference, 0, MAX_NUMBER_OF_GRIDS*MAX_NUMBER_OF_GRIDS*sizeof(uint16));
}

void MapInstanced::InitVisibilityDistance()
{
    // reached from the world thread on a config reload
    std::lock_guard<std::recursive_mutex> guard(m_lock);

    //initialize visibility distances for all instance copies
    if (!m_InstancedMaps.empty())
        for (InstancedMaps::iterator i = m_InstancedMaps.begin(); i != m_InstancedMaps.end(); ++i)
            (*i).second->InitVisibilityDistance();

    if (!m_GarrisonedMaps.empty())
        for (InstancedMaps::iterator i = m_GarrisonedMaps.begin(); i != m_GarrisonedMaps.end(); ++i)
            (*i).second->InitVisibilityDistance();
}

void MapInstanced::TerminateThread()
{
    if (!m_InstancedMaps.empty())
        for (InstancedMaps::iterator i = m_InstancedMaps.begin(); i != m_InstancedMaps.end(); ++i)
            (*i).second->TerminateThread();

    if (!m_GarrisonedMaps.empty())
        for (InstancedMaps::iterator i = m_GarrisonedMaps.begin(); i != m_GarrisonedMaps.end(); ++i)
            (*i).second->TerminateThread();

    Map::TerminateThread();
}

void MapInstanced::Update(const uint32 t)
{
    volatile uint32 _mapId = GetId();

    // take care of loaded GridMaps (when unused, unload it!)
    Map::Update(t);

    // update the instanced maps
    for (auto const& itr : _Snapshot(m_InstancedMaps))
    {
        Map* const instanced = itr.second;
        if (!instanced || instanced->CanCreatedZone())
            continue;

        if (instanced->CanUnload(t))
            DestroyInstance(itr.first, instanced);
        else
        {
            // update only here, because it may schedule some bad things before delete
            if (threadPool)
                threadPool->schedule([instanced, t] { instanced->Update(t); });
            else
                instanced->Update(t);
        }
    }

    // update the Garrisoned maps
    for (auto const& itr : _Snapshot(m_GarrisonedMaps))
    {
        if (Map* const instanced = itr.second)
        {
            if (instanced->CanUnload(t))
                DestroyGarrison(itr.first, instanced);
            else
            {
                // update only here, because it may schedule some bad things before delete
                if (threadPool)
                    threadPool->schedule([instanced, t] { instanced->Update(t); });
                else
                    instanced->Update(t);
            }
        }
        else
        {
            std::lock_guard<std::recursive_mutex> guard(m_lock);
            auto found = m_GarrisonedMaps.find(itr.first);
            if (found != m_GarrisonedMaps.end() && !found->second)
                m_GarrisonedMaps.erase(found);
        }
    }

    if (threadPool)
        threadPool->wait();
}

void MapInstanced::UpdateSessions(const uint32 diff)
{
    volatile uint32 _mapId = GetId();

    // Session not need thread
    for (auto const& i : _Snapshot(m_InstancedMaps))
        if (Map* const instanced = i.second)
            if (!instanced->IsMapUnload() && !instanced->CanCreatedZone())
                instanced->UpdateSessions(diff);

    for (auto const& i : _Snapshot(m_GarrisonedMaps))
        if (Map* const instanced = i.second)
            if (!instanced->IsMapUnload())
                    instanced->UpdateSessions(diff);
}

void MapInstanced::DelayedUpdate(const uint32 diff)
{
    volatile uint32 _mapId = GetId();

    for (auto const& i : _Snapshot(m_InstancedMaps))
    {
        if (Map* const instanced = i.second)
        {
            if (!instanced->IsMapUnload() && !instanced->CanCreatedZone())
            {
                if (BattlegroundMap* map = instanced->ToBgMap())
                    if (Battleground* bg = map->GetBG())
                    {
                        std::lock_guard<std::recursive_mutex> _bg_lock(bg->m_bg_lock);
                        if (bg && !bg->ToBeDeleted())
                            bg->Update(diff);
                    }

                instanced->DelayedUpdate(diff);
            }
        }
    }

    for (auto const& i : _Snapshot(m_GarrisonedMaps))
        if (Map* const instanced = i.second)
            if (!instanced->IsMapUnload())
                instanced->DelayedUpdate(diff);

    Map::DelayedUpdate(diff); // this may be removed
}

void MapInstanced::UpdateTransport(uint32 diff)
{
    volatile uint32 _mapId = GetId();

    for (auto const& i : _Snapshot(m_InstancedMaps))
        if (Map* instanced = i.second)
            if (!instanced->IsMapUnload() && !instanced->CanCreatedZone())
                instanced->UpdateTransport(diff);

    for (auto const& i : _Snapshot(m_GarrisonedMaps))
        if (Map* instanced = i.second)
            if (!instanced->IsMapUnload())
                instanced->UpdateTransport(diff);

    Map::UpdateTransport(diff); // this may be removed
}

void MapInstanced::UnloadAll()
{
    // Unload instanced maps
    for (InstancedMaps::iterator i = m_InstancedMaps.begin(); i != m_InstancedMaps.end(); ++i)
        i->second->UnloadAll();

    for (InstancedMaps::iterator i = m_GarrisonedMaps.begin(); i != m_GarrisonedMaps.end(); ++i)
        i->second->UnloadAll();

    // Delete the maps only after everything is unloaded to prevent crashes
    for (InstancedMaps::iterator i = m_InstancedMaps.begin(); i != m_InstancedMaps.end(); ++i)
        delete i->second;
    for (InstancedMaps::iterator i = m_GarrisonedMaps.begin(); i != m_GarrisonedMaps.end(); ++i)
        delete i->second;

    m_InstancedMaps.clear();
    m_GarrisonedMaps.clear();

    for (auto thread : _zoneThreads)
    {
        thread.second->join();
        delete thread.second;
    }
    _zoneThreads.clear();

    // Unload own grids (just dummy(placeholder) grids, neccesary to unload GridMaps!)
    Map::UnloadAll();
}

void MapInstanced::StopInstance()
{
    volatile uint32 _mapId = GetId();

    // Session not need thread
    for (auto const& i : _Snapshot(m_InstancedMaps))
        if (Map* const instanced = i.second)
            instanced->SetMapStop();
}

MapInstanced::MapSnapshot MapInstanced::_Snapshot(InstancedMaps const& maps) const
{
    std::lock_guard<std::recursive_mutex> guard(m_lock);
    return MapSnapshot(maps.begin(), maps.end());
}

void MapInstanced::ForEachInstancedMap(std::function<void(Map*)> const& fn) const
{
    std::lock_guard<std::recursive_mutex> guard(m_lock);
    for (auto const& itr : m_InstancedMaps)
        if (itr.second)
            fn(itr.second);
}

Map* MapInstanced::_FindInstance(uint32 instanceId) const
{
    InstancedMaps const& maps = IsGarrison() ? m_GarrisonedMaps : m_InstancedMaps;
    auto itr = maps.find(instanceId);
    return itr == maps.end() ? nullptr : itr->second;
}

bool MapInstanced::RequestInstanceReset(uint32 instanceId, uint8 method, bool* hadPlayers /*= nullptr*/)
{
    // under m_lock: DestroyInstance erases under it before deleting, so the map is alive while flagged
    std::lock_guard<std::recursive_mutex> guard(m_lock);
    Map* map = _FindInstance(instanceId);
    InstanceMap* instance = map ? map->ToInstanceMap() : nullptr;
    if (!instance)
        return false;

    if (hadPlayers)
        *hadPlayers = instance->HavePlayers();

    instance->RequestReset(method);
    return true;
}

bool MapInstanced::InstanceHavePlayers(uint32 instanceId) const
{
    std::lock_guard<std::recursive_mutex> guard(m_lock);
    Map* map = _FindInstance(instanceId);
    return map && map->HavePlayers();
}

// CanEnter only sends the player his own transfer error: a leaf call, as m_lock must stay
bool MapInstanced::CanEnterInstance(uint32 instanceId, Player* player)
{
    std::lock_guard<std::recursive_mutex> guard(m_lock);
    Map* map = _FindInstance(instanceId);
    return !map || map->CanEnter(player);
}

bool MapInstanced::SendToInstancePlayers(uint32 instanceId, WorldPacket const* data) const
{
    std::lock_guard<std::recursive_mutex> guard(m_lock);
    Map* map = _FindInstance(instanceId);
    if (!map || map->IsMapUnload())
        return false;

    map->SendToPlayers(data);
    return true;
}

/*
- return the right instance for the object, based on its InstanceId
- create the instance if it's not created already
- the player is not actually added to the instance (only in InstanceMap::Add)
*/
Map* MapInstanced::CreateInstanceForPlayer(const uint32 mapId, Player* player)
{
    if (GetId() != mapId || !player)
        return nullptr;

    Map* map = nullptr;
    uint32 newInstanceId = 0;                       // instanceId of the resulting map

    if (IsBattlegroundOrArena())
    {
        // instantiate or find existing bg map for player
        // the instance id is set in battlegroundid
        newInstanceId = player->GetBattlegroundId();
        if (!newInstanceId)
            return nullptr;

        map = sMapMgr->FindMap(mapId, newInstanceId);
        if (!map)
        {
            if (Battleground* bg = player->GetBattleground())
                map = CreateBattleground(newInstanceId, bg);
            else
            {
                player->TeleportToBGEntryPoint();
                return nullptr;
            }
        }
    }
    else if (!IsGarrison())
    {
        Group* group = player->GetGroup();
        Difficulty diff = DIFFICULTY_NORMAL;
        InstancePlayerBind pBind;                   // a copy: save == nullptr means no bind

        if (group)
        {
            diff = group->GetDifficultyID(GetEntry());
            // a group finder group goes to its dungeon's own difficulty: an event dungeon is not one a group can pick
            if (group->isLFGGroup())
                if (uint32 dungeonId = sLFGMgr->GetDungeon(group->GetGUID()))
                    if (lfg::LFGDungeonData const* dungeon = sLFGMgr->GetLFGDungeon(dungeonId))
                        if (uint32(dungeon->map) == GetId())
                            diff = Difficulty(dungeon->difficulty);

            if (group->GetLeaderGUID() == player->GetGUID())
                pBind = player->GetBoundInstanceCopy(GetId(), diff);
            else
            {
                // the leader's binds belong to his map's thread: they are read there only when it is ours, else from the database
                bool leaderOnline = false;
                bool leaderInMap = false;
                InstancePlayerBind leaderBind;
                ObjectAccessor::WithPlayer(group->GetLeaderGUID(), [&](Player* leader)
                {
                    leaderOnline = true;
                    if (Map* leaderMap = leader->FindMap())
                        if (leaderMap == player->FindMap())
                        {
                            leaderInMap = true;
                            leaderBind = leader->GetBoundInstanceCopy(GetId(), diff);
                        }
                });

                if (leaderOnline && !leaderInMap)
                {
                    // a leader on another map: his bind as saved, the instance he went to stays the group's one
                    if (QueryResult result = CharacterDatabase.PQuery("SELECT instance, permanent FROM character_instance WHERE guid = %u AND map = %u AND difficulty = %u",
                        group->GetLeaderGUID().GetCounter(), GetId(), uint32(diff)))
                        if (InstanceSave* save = sInstanceSaveMgr->GetInstanceSave((*result)[0].GetUInt32()))
                        {
                            leaderBind.save = save;
                            leaderBind.perm = (*result)[1].GetBool();
                        }
                }

                if (leaderOnline)
                {
                    pBind = leaderBind;

                    if (GetEntry()->ExpansionID < EXPANSION_LEGION)
                    {
                        InstancePlayerBind const tempBind = player->GetBoundInstanceCopy(GetId(), diff);
                        if (pBind.save && tempBind.save && pBind.save != tempBind.save)
                        {
                            uint32 allMask = pBind.save->GetCompletedEncounterMask() & tempBind.save->GetCompletedEncounterMask();
                            if (allMask == tempBind.save->GetCompletedEncounterMask())
                            {
                                player->UnbindInstance(GetId(), diff);
                                player->BindToInstance(pBind.save, pBind.perm);
                            }
                        }
                    }
                }
                else
                    pBind = player->GetBoundInstanceCopy(GetId(), diff);
            }
        }
        else
        {
            diff = player->GetDifficultyID(GetEntry());
            pBind = player->GetBoundInstanceCopy(GetId(), diff);
        }

        InstanceSave* pSave = pBind.save;

        if (pSave && pSave->SaveIsOld() && !pSave->GetExtended())
        {
            player->UnbindInstance(GetId(), diff);
            pSave = nullptr;
            pBind = InstancePlayerBind();
        }

        // the player's permanent player bind is taken into consideration first
        // then the player's group bind and finally the solo bind.
        if (!pBind.save || !pBind.perm || GetEntry()->ExpansionID >= EXPANSION_WARLORDS_OF_DRAENOR)
        {
            InstanceGroupBind groupBind;
            // use the player's difficulty setting (it may not be the same as the group's)
            if (group)
            {
                groupBind = group->GetBoundInstanceCopy(diff, GetId());
                if (groupBind.save)
                    pSave = groupBind.save;
                if (pSave && pSave->SaveIsOld() && !pSave->GetExtended())
                {
                    group->UnbindInstance(GetId(), diff);
                    pSave = nullptr;
                }
            }
        }

        // normal and event dungeons share their binds: an instance of the other one is never taken for this one
        if (pSave && pSave->GetDifficultyID() != diff && (diff == DIFFICULTY_EVENT_DUNGEON || pSave->GetDifficultyID() == DIFFICULTY_EVENT_DUNGEON))
            pSave = nullptr;

        if (pSave)
        {
            // solo/perm/group
            newInstanceId = pSave->GetInstanceId();
            map = FindInstanceMap(newInstanceId);
            // it is possible that the save exists but the map doesn't
            if (!map)
                map = CreateInstance(newInstanceId, pSave, pSave->GetDifficultyID());
        }
        else
        {
            // if no instanceId via group members or instance saves is found
            // the instance will be created for the first time
            newInstanceId = sMapMgr->GenerateInstanceId();

            //Seems it is now possible, but I do not know if it should be allowed
            //ASSERT(!FindInstanceMap(NewInstanceId));
            if (const MapEntry* entry = sMapStore.LookupEntry(mapId))
                if (entry->IsScenario())
                {
                    DifficultyEntry const* difficultyEntry = sDifficultyStore.LookupEntry(diff);
                    if (!difficultyEntry || difficultyEntry->InstanceType != entry->InstanceType)
                        if (MapDifficultyEntry const* defaultDifficulty = sDB2Manager.GetDefaultMapDifficulty(entry->ID))
                            diff = Difficulty(defaultDifficulty->DifficultyID);
                }

            // Find correct difficulty
            MapDifficultyEntry const* mapDiff = sDB2Manager.GetMapDifficultyData(mapId, diff);
            if (!mapDiff)
                mapDiff = sDB2Manager.GetDefaultMapDifficulty(mapId);
            if (mapDiff)
                diff = Difficulty(mapDiff->DifficultyID);

            map = FindInstanceMap(newInstanceId);
            if (!map)
                map = CreateInstance(newInstanceId, nullptr, diff);
            else if(map->GetDifficultyID() != diff)
                map->SetSpawnMode(diff);

            std::string data;
            pSave = sInstanceSaveMgr->AddInstanceSave(GetId(), newInstanceId, diff, 0, data, 0, true);
            if (group)
            {
                group->BindToInstance(pSave, false);
                if (lfg::LFGDungeonData const* dungeon = sLFGMgr->GetLFGDungeon(sLFGMgr->GetDungeon(group->GetGUID())))
                    if (dungeon->dbc && dungeon->dbc->MentorItemLevel && dungeon->dbc->MentorCharLevel && dungeon->difficulty == DIFFICULTY_HEROIC) // Detect timewalk
                        map->SetLootDifficulty(DIFFICULTY_TIMEWALKING);
            }
        }
    }
    else
    {
        newInstanceId = player->GetGUIDLow();
        map = FindGarrisonMap(newInstanceId);
        if (!map)
            map = CreateGarrison(newInstanceId, player);
    }

    if (map)
        sWorldStateMgr.CreateInstanceState(mapId, newInstanceId);

    return map;
}

InstanceMap* MapInstanced::CreateInstance(uint32 InstanceId, InstanceSave* save, Difficulty difficulty)
{
    // load/create a map
    std::lock_guard<std::recursive_mutex> _lock(m_lock);

    // two players teleporting from two maps may both have missed it: the second one joins the first one's map
    auto existing = m_InstancedMaps.find(InstanceId);
    if (existing != m_InstancedMaps.end())
        if (InstanceMap* instance = existing->second->ToInstanceMap())
            return instance;

    // make sure we have a valid map id
    const MapEntry* entry = sMapStore.LookupEntry(GetId());
    if (!entry)
    {
        TC_LOG_ERROR("maps", "CreateInstance: no entry for map %d", GetId());
        return nullptr;
    }
    const InstanceTemplate* iTemplate = sObjectMgr->GetInstanceTemplate(GetId());
    if (!iTemplate && !entry->IsScenario())
    {
        TC_LOG_ERROR("maps", "CreateInstance: no instance template for map %d", GetId());
        return nullptr;
    }

    sDB2Manager.GetDownscaledMapDifficultyData(GetId(), difficulty);

    TC_LOG_DEBUG("maps", "MapInstanced::CreateInstance: %s map instance %d for %d created with difficulty %u", save ? "" : "new ", InstanceId, GetId(), difficulty);

    InstanceMap* map = new InstanceMap(GetId(), GetGridExpiry(), InstanceId, difficulty, this);
    ASSERT(map->IsDungeon());

    map->LoadRespawnTimes();

    map->CreateInstanceData(save);

    sTransportMgr->CreateInstanceTransports(map);

    m_InstancedMaps[InstanceId] = map;
    return map;
}

GarrisonMap* MapInstanced::CreateGarrison(uint32 instanceId, Player* owner)
{
    // load/create a map
    std::lock_guard<std::recursive_mutex> _lock(m_lock);

    auto existing = m_GarrisonedMaps.find(instanceId);
    if (existing != m_GarrisonedMaps.end())
        return static_cast<GarrisonMap*>(existing->second);

    GarrisonMap* map = new GarrisonMap(GetId(), GetGridExpiry(), instanceId, this, owner->GetGUID());
    ASSERT(map->IsGarrison());
    map->CreateInstanceData(nullptr);

    sTransportMgr->CreateInstanceTransports(map);

    m_GarrisonedMaps[instanceId] = map;
    return map;
}

Map* MapInstanced::CreateZoneForPlayer(const uint32 mapId, Player* player, Position const* dest /*= nullptr*/)
{
    if (GetId() != mapId || !player)
        return nullptr;

    // a far teleport is not relocated yet: the zone is the one he goes to, else he lands in the zone of the place he left
    // and is moved again at his first zone update, away from all that was set for his arrival
    Position const pos = dest ? *dest : player->GetPosition();
    uint32 newZoneId = GetZoneId(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ());
    Map* map = FindInstanceMap(newZoneId);
    if (!map)
        map = CreateZoneMap(newZoneId, player);

    return map;
}

// the lookup itself is locked: Create* insert from the threads of the players entering
Map* MapInstanced::FindInstanceMap(uint32 instanceId) const
{
    std::lock_guard<std::recursive_mutex> guard(m_lock);
    InstancedMaps::const_iterator i = m_InstancedMaps.find(instanceId);
    return (i == m_InstancedMaps.end() ? nullptr : i->second);
}

Map* MapInstanced::FindGarrisonMap(uint32 instanceId) const
{
    std::lock_guard<std::recursive_mutex> guard(m_lock);
    InstancedMaps::const_iterator i = m_GarrisonedMaps.find(instanceId);
    return (i == m_GarrisonedMaps.end() ? nullptr : i->second);
}

ZoneMap* MapInstanced::CreateZoneMap(uint32 zoneId, Player* player)
{
    // load/create a map
    std::lock_guard<std::recursive_mutex> _lock(m_lock);

    // two zone threads may enter the same zone at once: a second map would orphan the first and its update thread
    auto existing = m_InstancedMaps.find(zoneId);
    if (existing != m_InstancedMaps.end())
        if (ZoneMap* zone = dynamic_cast<ZoneMap*>(existing->second))
            return zone;

    ZoneMap* map = new ZoneMap(GetId(), GetGridExpiry(), zoneId, this, DIFFICULTY_NONE);
    map->LoadRespawnTimes();
    m_InstancedMaps[zoneId] = map;

    map->UpdateOutdoorPvPScript();

    _zoneThreads[zoneId] = new std::thread(&Map::UpdateLoop, map, zoneId);
    return map;
}

BattlegroundMap* MapInstanced::CreateBattleground(uint32 InstanceId, Battleground* bg)
{
    // load/create a map
    std::lock_guard<std::recursive_mutex> _lock(m_lock);

    TC_LOG_DEBUG("maps", "MapInstanced::CreateBattleground: map bg %d for %d created.", InstanceId, GetId());

    // the fixed population of a world PvP map (Ashran) is spawned for its own difficulty only: hold the game in that one
    Difficulty difficulty = DIFFICULTY_NORMAL;
    if (!sDB2Manager.GetMapDifficultyData(GetId(), DIFFICULTY_NORMAL) && sDB2Manager.GetMapDifficultyData(GetId(), DIFFICULTY_WORLD_PVP_SCENARIO))
        difficulty = DIFFICULTY_WORLD_PVP_SCENARIO;

    BattlegroundMap* map = new BattlegroundMap(GetId(), GetGridExpiry(), InstanceId, this, difficulty);
    ASSERT(map->IsBattlegroundOrArena());
    map->SetBG(bg);
    map->InitVisibilityDistance();
    bg->SetBgMap(map);
    bg->OnMapCreated();

    sTransportMgr->CreateInstanceTransports(map);

    m_InstancedMaps[InstanceId] = map;
    return map;
}

// only called by this map's own thread
bool MapInstanced::DestroyInstance(uint32 instanceId, Map* uMap)
{
    if (!uMap)
        return false;

    uMap->RemoveAllPlayers();
    if (uMap->HavePlayers())
        return false;

    // a world PvP engine goes back to the base map while its instance is still whole
    uMap->ReleaseOutdoorPvP();

    // a reset requested since the last update is applied now (no player left), before UnloadAll which acts on it
    if (InstanceMap* instance = uMap->ToInstanceMap())
    {
        uint32 pendingResets = instance->TakePendingResets();
        for (uint8 method = INSTANCE_RESET_ALL; method <= INSTANCE_RESET_RESPAWN_DELAY; ++method)
            if (pendingResets & (1u << method))
                instance->Reset(method);
    }

    uMap->UnloadAll();

    size_t instanceCount;
    {
        std::lock_guard<std::recursive_mutex> guard(m_lock);
        instanceCount = m_InstancedMaps.size();
    }

    // should only unload VMaps if this is the last instance and grid unloading is enabled
    if (instanceCount <= 1 && CanUnloadMap())
    {
        VMAP::VMapFactory::createOrGetVMapManager()->unloadMap(uMap->GetId());
        MMAP::MMapFactory::createOrGetMMapManager()->unloadMap(uMap->GetId());
        // in that case, unload grids of the base map, too
        // so in the next map creation, (EnsureGridCreated actually) VMaps will be reloaded
        Map::UnloadAll();
    }

    // map-aware: a garrison and a dungeon can share an instance id
    sScenarioMgr->RemoveScenario(uMap);

    sWorldStateMgr.DeleteInstanceState(uMap->GetInstanceId());

    // erased under m_lock before the delete: ForEachInstancedMap / RequestInstanceReset never see a freed map
    {
        std::lock_guard<std::recursive_mutex> guard(m_lock);
        auto itr = m_InstancedMaps.find(instanceId);
        if (itr != m_InstancedMaps.end() && itr->second == uMap)
            m_InstancedMaps.erase(itr);
    }

    // a reset requested after UnloadAll, up to the erasure, leaves with the map: keep its database part
    if (InstanceMap* instance = uMap->ToInstanceMap())
        if (instance->TakePendingResets())
            DeleteRespawnTimesInDB(GetId(), instanceId);

    delete uMap;

    return true;
}

// only called by this map's own thread
bool MapInstanced::DestroyGarrison(uint32 instanceId, Map* uMap)
{
    uMap->RemoveAllPlayers();
    if (uMap->HavePlayers())
        return false;

    uMap->UnloadAll();

    size_t garrisonCount;
    {
        std::lock_guard<std::recursive_mutex> guard(m_lock);
        garrisonCount = m_GarrisonedMaps.size();
    }

    // should only unload VMaps if this is the last instance and grid unloading is enabled
    if (garrisonCount <= 1 && CanUnloadMap())
    {
        VMAP::VMapFactory::createOrGetVMapManager()->unloadMap(uMap->GetId());
        MMAP::MMapFactory::createOrGetMMapManager()->unloadMap(uMap->GetId());
        // in that case, unload grids of the base map, too
        // so in the next map creation, (EnsureGridCreated actually) VMaps will be reloaded
        Map::UnloadAll();
    }

    // a garrison invasion runs a scenario on this map
    sScenarioMgr->RemoveScenario(uMap);

    sWorldStateMgr.DeleteInstanceState(uMap->GetInstanceId());

    {
        std::lock_guard<std::recursive_mutex> guard(m_lock);
        auto itr = m_GarrisonedMaps.find(instanceId);
        if (itr != m_GarrisonedMaps.end() && itr->second == uMap)
            m_GarrisonedMaps.erase(itr);
    }

    delete uMap;

    return true;
}

void MapInstanced::AddGridMapReference(const GridCoord& p)
{
    ++GridMapReference[p.x_coord][p.y_coord];
    SetUnloadReferenceLock(GridCoord(63 - p.x_coord, 63 - p.y_coord), true);
}

void MapInstanced::RemoveGridMapReference(GridCoord const& p)
{
    --GridMapReference[p.x_coord][p.y_coord];
    if (!GridMapReference[p.x_coord][p.y_coord])
        SetUnloadReferenceLock(GridCoord(63 - p.x_coord, 63 - p.y_coord), false);
}

bool MapInstanced::CanEnter(Player* /*player*/)
{
    //ASSERT(false);
    return true;
}
