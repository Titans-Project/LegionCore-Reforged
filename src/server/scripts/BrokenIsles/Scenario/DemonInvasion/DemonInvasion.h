////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
 * Demon Invasion Prepatch - Legion Pre-expansion Event
 * 6 zones (Azshara, Dun Morogh, Hillsbrad, Northern Barrens, Tanaris, Westfall)
 * 4-stage invasions with rotation system
 */

#ifndef DEMON_INVASION_H
#define DEMON_INVASION_H

#include "OutdoorPvP.h"
#include <unordered_map>

class Creature;
class Player;
class Scenario;

// ============================================================================
// Enums
// ============================================================================

enum DemonInvasionZoneIndex : uint8
{
    DI_ZONE_AZSHARA          = 0, // Kalimdor, map 1
    DI_ZONE_DUN_MOROGH       = 1, // EK, map 0
    DI_ZONE_HILLSBRAD        = 2, // EK, map 0
    DI_ZONE_NORTHERN_BARRENS = 3, // Kalimdor, map 1 (zone 17)
    DI_ZONE_TANARIS           = 4, // Kalimdor, map 1
    DI_ZONE_WESTFALL          = 5, // EK, map 0
    DI_ZONE_MAX               = 6,
};

enum DemonInvasionStage : uint8
{
    DI_STAGE_INACTIVE  = 0,
    DI_STAGE_DEFEND    = 1, // Phase 1: Defend the hub
    DI_STAGE_COMMANDER = 2, // Phase 2: Kill commander + lieutenants
    DI_STAGE_REPEL     = 3, // Phase 3: Repel demons zone-wide
    DI_STAGE_BOSS      = 4, // Phase 4: Kill the final boss
    DI_STAGE_MAX       = 5,
};

// What a creature is worth to the invasion (table demon_invasion_creature): the official NPCs and the old custom ones
enum DemonInvasionRole : uint8
{
    DI_ROLE_NONE        = 0,
    DI_ROLE_DEMON       = 1,
    DI_ROLE_ELITE       = 2, // infernals, doomguards, named demons: worth three kills
    DI_ROLE_COMMANDER   = 3,
    DI_ROLE_LIEUTENANT  = 4,
    DI_ROLE_DEMON_LORD  = 5,
    DI_ROLE_MAX         = 6,
};

// NPC entries (900001-900099 range)
enum DemonInvasionNpcs : uint32
{
    // Stage 1/3 generic demons
    NPC_DI_FEL_INVADER       = 900001,
    NPC_DI_FELSTALKER        = 900002,
    NPC_DI_FEL_CASTER        = 900003,
    NPC_DI_INFERNAL          = 900004,
    // Stage 2 elites
    NPC_DI_COMMANDER         = 900010,
    NPC_DI_LIEUTENANT_A      = 900011,
    NPC_DI_LIEUTENANT_B      = 900012,
    // Stage 4 boss
    NPC_DI_BOSS              = 900020,
    // Vendor
    NPC_DI_NETHERSHARD_VENDOR = 900050,
};

// Item entries
enum DemonInvasionItems : uint32
{
    ITEM_NETHERSHARD          = 900001,
    // Vendor items
    ITEM_DI_CLOTH_SET         = 900101,
    ITEM_DI_LEATHER_SET       = 900102,
    ITEM_DI_MAIL_SET          = 900103,
    ITEM_DI_PLATE_SET         = 900104,
    ITEM_DI_CLOAK             = 900105,
    ITEM_DI_RING              = 900106,
    ITEM_DI_NECK              = 900107,
    ITEM_DI_TRINKET           = 900108,
    ITEM_DI_FELSTONE          = 900109,
    ITEM_DI_PROTECTION_POT    = 900110,
    ITEM_DI_BANDAGE           = 900111,
};

// Nethershard reward amounts
enum DemonInvasionShardRewards : uint32
{
    DI_SHARDS_ELITE_KILL      = 7,    // 5-10 range, per elite in stage 2
    DI_SHARDS_TRASH_KILL      = 1,    // per mob in stage 3
    DI_SHARDS_BOSS_KILL       = 20,   // boss kill in stage 4
    DI_SHARDS_STAGE_BONUS     = 5,    // bonus per stage advance (zone-wide)
};

// XP multipliers (in terms of "quest equivalents")
enum DemonInvasionXPMultipliers : uint32
{
    DI_XP_STAGE_ADVANCE       = 2,    // ~2 quests worth per stage 1->2, 2->3
    DI_XP_STAGE_REPEL         = 3,    // ~3 quests worth for stage 3->4
    DI_XP_BOSS_KILL           = 5,    // ~5 quests worth for boss kill
};

// Vendor item data
struct DemonInvasionVendorItem
{
    uint32 itemEntry;
    const char* name;
    uint32 nethershardCost;
};

// ============================================================================
// Constants
// ============================================================================

// GameEvent IDs: 900-923 (6 zones x 4 stages)
//  Zone 0 Azshara:          900-903
//  Zone 1 Dun Morogh:       904-907
//  Zone 2 Hillsbrad:        908-911
//  Zone 3 Northern Barrens: 912-915
//  Zone 4 Tanaris:          916-919
//  Zone 5 Westfall:         920-923
static constexpr uint16 DI_GAME_EVENT_BASE     = 900;
static constexpr uint32 DI_CONSTANT_RESTART = HOUR * IN_MILLISECONDS; // last week: delay before a repelled zone is invaded again
static constexpr uint32 SPELL_DI_INVASION_SKY  = 305164; // hotfix spell: screen effect 1265, Legion invasion skybox
static constexpr uint32 DI_MAP_MARKER_WORLD_STATE = 19701; // + zone index: lights the zone's invasion marker (AreaPOI 5565-5570 served by CASCHost)

// ============================================================================
// Zone data
// ============================================================================

struct DemonInvasionZoneInfo
{
    uint32 zoneId;
    uint32 mapId;
    const char* name;
};

// Official invasion scenario of a zone (client Scenario.db2, type 4) and the scenario events (CriteriaTree type 92)
// its steps count: defend, commander and lieutenants, repel (weight 1, 10 and 25), demon lord.
struct DemonInvasionScenarioInfo
{
    uint32 scenarioId;
    uint32 defendEvent;
    uint32 defendPoints;
    uint32 commanderEvent;
    uint32 lieutenantEvent;
    uint32 lieutenantCount;
    uint32 repelEliteEvent;
    uint32 repelStructureEvent;
    uint32 repelPoints;
    uint32 demonLordEvent;
};

struct DemonInvasionZoneState
{
    DemonInvasionStage stage;
    uint32 stageProgress;
};

// ============================================================================
// DemonInvasionMgr — singleton, shared state between both OutdoorPvP instances
// ============================================================================

class DemonInvasionMgr
{
    DemonInvasionMgr();

public:
    static DemonInvasionMgr* instance();

    void Initialize();
    void Update(uint32 diff);

    // Core mechanics
    void StartInvasion(uint8 zoneIndex);
    void StopInvasion(uint8 zoneIndex, bool repelled = false);
    void AdvanceStage(uint8 zoneIndex);
    void RotateInvasions();
    static uint8 GetInvasionWeek();

    void OnCreatureKill(uint32 zoneId, uint32 creatureEntry, Player* killer);
    void OnInvasionCreatureDied(Creature* creature) const;
    void LoadCreatureRoles();
    DemonInvasionRole GetCreatureRole(uint32 entry) const;

    // Phase 4: Rewards
    void RewardPlayersInZone(uint8 zoneIndex, uint32 nethershards, uint32 xpMultiplier);
    void OnBossKilled(uint8 zoneIndex);
    void TrackInvasionCompletion(Player* player, uint8 zoneIndex);
    bool HasCompletedAllInvasions(Player* player);

    // Legion sky while the player stands in an invaded zone
    void UpdateInvasionSky(Player* player, uint32 zoneId) const;

    // Official scenario of each invaded zone, carried by the OutdoorPvP of its continent
    void RegisterOutdoorPvP(uint32 mapId, OutdoorPvP* outdoor);
    void OnPlayerEnterZone(Player* player, uint32 zoneId);
    void OnPlayerLeaveZone(Player* player, uint32 zoneId);
    void OnScenarioStep(uint32 zoneId, uint32 step);

    // The event follows the crowd: players in the zone (GMs aside) lengthen the bars and quicken the respawns
    uint32 GetParticipantCount(uint32 zoneId) const;
    uint32 GetDemonRespawnDelay(uint32 zoneId) const;

    // Helpers
    static uint32 GetBaseQuestXP(uint8 level);
    static uint32 GetNethershardDropAmount(uint32 creatureEntry, uint8 stage);

    // GM commands
    bool ForceStart(uint32 zoneId);
    void ForceStop();
    void ForceStage(uint8 zoneIndex, uint8 stage);
    void ForceRotate();

    // Queries
    bool IsZoneActive(uint8 zoneIndex) const;
    DemonInvasionStage GetZoneStage(uint8 zoneIndex) const;
    uint8 GetZoneIndexByZoneId(uint32 zoneId) const;
    std::string GetStatusString() const;
    bool IsInitialized() const { return m_initialized; }

    // Static helpers
    static const DemonInvasionZoneInfo& GetZoneInfo(uint8 index);
    static uint16 GetGameEventId(uint8 zoneIndex, uint8 stage);
    static uint32 GetKillThreshold(uint8 stage);
    static bool IsInvasionCreature(uint32 entry);

private:
    void BroadcastToZone(uint8 zoneIndex, const char* format, ...);
    void UpdateInvasionSkyInZone(uint8 zoneIndex) const;
    void SetMapMarker(uint8 zoneIndex, bool invaded) const;
    void CreateScenario(uint8 zoneIndex);
    void DestroyScenario(uint8 zoneIndex, bool repelled);
    void SendScenarioEvent(uint8 zoneIndex, uint32 eventId, uint32 count, Player* reference);
    bool SendKillToScenario(uint8 zoneIndex, uint32 creatureEntry, Player* killer);
    uint32 TakeBarPoints(uint8 zoneIndex, uint32 barPoints, uint32 kills, uint32 killWeight);
    template<class Fn> void ForEachPlayerInZone(uint8 zoneIndex, Fn&& fn) const;

    DemonInvasionZoneState m_zoneStates[DI_ZONE_MAX];
    uint32 m_rotationTimer;
    Scenario* m_scenarios[DI_ZONE_MAX];
    uint32 m_pointCarry[DI_ZONE_MAX];  // thousandths of a bar point not yet sent, so small shares per kill add up exactly
    std::vector<Scenario*> m_endedScenarios;  // deleted on the next update, never from inside a scenario call
    OutdoorPvP* m_outdoorByMap[2];
    std::unordered_map<uint32, uint8> m_roles;
    bool m_initialized;
};

#define sDemonInvasionMgr DemonInvasionMgr::instance()

// ============================================================================
// OutdoorPvP classes — one per continent
// ============================================================================

class OutdoorPvPDemonInvasionEK : public OutdoorPvP
{
public:
    OutdoorPvPDemonInvasionEK()
    {
        m_TypeId = OUTDOOR_PVP_DEMON_INVASION_EK;
    }

    bool SetupOutdoorPvP() override;
    void HandlePlayerEnterZone(ObjectGuid guid, uint32 zone) override;
    void HandlePlayerLeaveZone(ObjectGuid guid, uint32 zone) override;
    void SetData(uint32 zone, uint32 step) override;
    void BroadcastPacketByZone(WorldPacket const& data, uint32 zone) override;
    bool Update(uint32 diff) override;
    void HandleKillImpl(Player* killer, Unit* killed) override;
};

class OutdoorPvPDemonInvasionKal : public OutdoorPvP
{
public:
    OutdoorPvPDemonInvasionKal()
    {
        m_TypeId = OUTDOOR_PVP_DEMON_INVASION_KAL;
    }

    bool SetupOutdoorPvP() override;
    void HandlePlayerEnterZone(ObjectGuid guid, uint32 zone) override;
    void HandlePlayerLeaveZone(ObjectGuid guid, uint32 zone) override;
    void SetData(uint32 zone, uint32 step) override;
    void BroadcastPacketByZone(WorldPacket const& data, uint32 zone) override;
    bool Update(uint32 diff) override;
    void HandleKillImpl(Player* killer, Unit* killed) override;
};

#endif // DEMON_INVASION_H
