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

#ifndef TRINITY_MAPMANAGER_H
#define TRINITY_MAPMANAGER_H

#include "Common.h"
#include "Map.h"
#include "Position.h"
#include <mutex>
#include <unordered_map>

class WorldLocation;
class ChatHandler;

class TC_GAME_API MapManager
{
    typedef std::vector<Map*> MapMapType;

    MapManager();
    ~MapManager();

    MapManager(const MapManager &) = delete;
    MapManager& operator=(const MapManager &) = delete;

    public:
        static MapManager* instance();

        Map* CreateBaseMap(uint32 mapId);
        Map* FindBaseNonInstanceMap(uint32 mapId) const;
        Map* CreateMap(uint32 mapId, Player* player, Position const* dest = nullptr);
        Map* FindMap(uint32 mapId, uint32 instanceId) const;
        // for threads other than the instance's own: the instance is used under its parent's lock, no Map* is returned
        bool RequestInstanceReset(uint32 mapId, uint32 instanceId, uint8 method, bool* hadPlayers = nullptr);
        bool InstanceHavePlayers(uint32 mapId, uint32 instanceId) const;

        uint32 GetAreaId(uint32 mapid, float x, float y, float z) const;
        uint32 GetZoneId(uint32 mapid, float x, float y, float z) const;
        void GetZoneAndAreaId(uint32& zoneid, uint32& areaid, uint32 mapid, float x, float y, float z);

        void Initialize();
        void Update(uint32);

        void SetGridCleanUpDelay(uint32 t);

        void SetMapUpdateInterval(uint32 t);

        //void LoadGrid(int mapid, int instId, float x, float y, const WorldObject* obj, bool no_unload = false);
        void UnloadAll();

        static bool ExistMapAndVMap(uint32 mapid, float x, float y);
        static bool IsValidMAP(uint32 mapid, bool startUp);

        static bool IsValidMapCoord(uint32 mapid, float x, float y);
        static bool IsValidMapCoord(uint32 mapid, float x, float y, float z);
        static bool IsValidMapCoord(uint32 mapid, float x, float y, float z, float o);
        static bool IsValidMapCoord(WorldLocation const& loc);

        void DoDelayedMovesAndRemoves();

        bool CanPlayerEnter(uint32 mapid, Player* player, bool loginCheck = false);

        // Hourly instance limit of an account (AccountInstancesPerHour): a new instance counts for an hour,
        // going back into one already counted is always allowed.
        bool CanEnterInstanceThisHour(uint32 accountId, uint32 instanceId);
        void AddInstanceEnter(uint32 accountId, uint32 instanceId);
        void InitializeVisibilityDistanceInfo();

        /* statistics */
        uint32 GetNumInstances();
        uint32 GetNumPlayersInInstances();

        // Instance ID management
        void InitInstanceIds();
        uint32 GenerateInstanceId();

        void FindSessionInAllMaps(uint32 accId, ChatHandler* handler);
        void LogInfoAllMaps();
        void SetUnloadGarrison(uint32 lowGuid);

        Map* FindBaseMap(uint32 mapId) const { return i_maps[mapId]; }
        std::vector<std::thread*> _mapThreads;
        uint16 _mapCount;

        uint32 IncreaseScheduledScriptsCount() { return ++_scheduledScripts; }
        uint32 DecreaseScheduledScriptCount() { return --_scheduledScripts; }
        uint32 DecreaseScheduledScriptCount(size_t count) { return _scheduledScripts -= count; }
        bool IsScriptScheduled() const { return _scheduledScripts > 0; }

    private:

        uint32 i_gridCleanUpDelay;
        MapMapType i_maps;
        // creation of a base map (FindBaseMap, construction, i_maps/_mapThreads slot, thread start)
        std::recursive_mutex _createMapLock;

        std::atomic<uint32> _nextInstanceId; // map threads create instances in parallel
        uint8 _mapInfoCounter;

        // atomic op counter for active scripts amount
        std::atomic<uint32> _scheduledScripts;

        // account -> instance -> time it stops counting; map threads add players in parallel
        std::mutex _instanceEnterLock;
        std::unordered_map<uint32, std::unordered_map<uint32, time_t>> _instanceEnters;
};
#define sMapMgr MapManager::instance()
#endif
