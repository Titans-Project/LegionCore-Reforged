/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRINITY_MAP_INSTANCED_H
#define TRINITY_MAP_INSTANCED_H

#include "Map.h"
#include "InstanceSaveMgr.h"
#include "DBCEnums.h"
#include <functional>
#include <mutex>
#include <vector>

class GarrisonMap;

typedef std::unordered_map< uint32, Map*> InstancedMaps;

class MapInstanced : public Map
{
    friend class MapManager;

    public:

        MapInstanced(uint32 id, time_t expiry);
        ~MapInstanced() {}

        // functions overwrite Map versions
        void Update(const uint32) override;
        void DelayedUpdate(const uint32 diff) override;
        void UpdateTransport(uint32 diff) override;
        void UpdateSessions(uint32 diff) override;
        void StopInstance();

        void UnloadAll() override;
        bool CanEnter(Player* player) override;

        Map* CreateInstanceForPlayer(const uint32 mapId, Player* player);
        Map* CreateZoneForPlayer(const uint32 mapId, Player* player, Position const* dest = nullptr);
        // the pointer is only safe in this map's own thread (instances are destroyed there): other threads use the
        // locked helpers below, which never let a Map* out
        Map* FindInstanceMap(uint32 instanceId) const;
        Map* FindGarrisonMap(uint32 instanceId) const;

        bool DestroyInstance(uint32 instanceId, Map* uMap);
        bool DestroyGarrison(uint32 instanceId, Map* uMap);

        void AddGridMapReference(const GridCoord& p);
        void RemoveGridMapReference(GridCoord const& p);

        // Only for this map's own thread (or shutdown): other threads insert instances meanwhile, use ForEachInstancedMap
        InstancedMaps &GetInstancedMaps() { return m_InstancedMaps; }
        // fn runs under m_lock, so no instance is destroyed meanwhile; it must only read or flag the map (no lock,
        // packet, DB or teleport): Create* hold m_lock while they load a new instance
        void ForEachInstancedMap(std::function<void(Map*)> const& fn) const;
        // hands a reset to the instance's own thread; false when that instance is not loaded. hadPlayers: whether
        // players were inside when it was requested (what InstanceMap::Reset returns). Same lookup as MapManager::FindMap.
        bool RequestInstanceReset(uint32 instanceId, uint8 method, bool* hadPlayers = nullptr);
        bool InstanceHavePlayers(uint32 instanceId) const;
        // Same lookup, the map kept alive by m_lock while used; true when the instance is not loaded. The check and the
        // broadcast still read the instance's player list from the caller's thread: safe from the instance's own thread.
        bool CanEnterInstance(uint32 instanceId, Player* player);
        bool SendToInstancePlayers(uint32 instanceId, WorldPacket const* data) const;
        void InitVisibilityDistance() override;

        void TerminateThread();

        InstancedMaps m_InstancedMaps;
        InstancedMaps m_GarrisonedMaps;

        bool IsIdle() const override { return m_InstancedMaps.empty() && m_GarrisonedMaps.empty() && !HavePlayers(); }
        std::map<uint32, std::thread*> _zoneThreads;

    private:
        InstanceMap* CreateInstance(uint32 InstanceId, InstanceSave* save, Difficulty difficulty);
        GarrisonMap* CreateGarrison(uint32 instanceId, Player* owner);
        BattlegroundMap* CreateBattleground(uint32 InstanceId, Battleground* bg);
        ZoneMap* CreateZoneMap(uint32 zoneId, Player* player);

        // m_lock held
        Map* _FindInstance(uint32 instanceId) const;

        typedef std::vector<std::pair<uint32, Map*>> MapSnapshot;
        // this map's own thread iterates a copy: other threads insert under m_lock, only this thread erases
        MapSnapshot _Snapshot(InstancedMaps const& maps) const;

        // guards insertion, lookup and erasure in m_InstancedMaps / m_GarrisonedMaps. Leaf for the save and group locks:
        // InstanceSave::_playerListLock -> m_lock (UnloadIfEmpty), never the reverse; Create* hold it while loading.
        mutable std::recursive_mutex m_lock;

        uint16 GridMapReference[MAX_NUMBER_OF_GRIDS][MAX_NUMBER_OF_GRIDS];
};
#endif
