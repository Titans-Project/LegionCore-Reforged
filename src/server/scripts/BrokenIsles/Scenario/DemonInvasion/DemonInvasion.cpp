////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
 * Demon Invasion Prepatch - Legion Pre-expansion Event
 * Implementation: DemonInvasionMgr, OutdoorPvP handlers, GM commands, the .demoninvasion player command
 *
 * World map markers: the invaded zones are shown on the world map only with the AreaPOI rows 5565-5570, which the
 * 7.3.5 client files no longer have (they were removed after 7.0) and which a hotfix cannot add, the client listing its
 * map points when it loads the file. Without that file served to the client (CASCHost), no portal nor outline shows
 * on the map: the players learn where the invasions are from the announcements in game and from .demoninvasion.
 */

#include "DemonInvasion.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "OutdoorPvPMgr.h"
#include "GameEventMgr.h"
#include "Chat.h"
#include "Player.h"
#include "World.h"
#include "Log.h"
#include "DB2Stores.h"
#include "Creature.h"
#include "DatabaseEnv.h"
#include "Item.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "Scenario.h"
#include "ScenarioPackets.h"
#include "WorldStateMgr.h"
#include <sstream>
#include <cstdarg>

// ============================================================================
// Zone data tables
// ============================================================================

static const DemonInvasionZoneInfo s_zoneInfos[DI_ZONE_MAX] =
{
    { 16,  1, "Azshara"          }, // 0 - Kalimdor
    { 1,   0, "Dun Morogh"       }, // 1 - EK
    { 267, 0, "Hillsbrad"        }, // 2 - EK
    { 17,  1, "Northern Barrens" }, // 3 - Kalimdor
    { 440, 1, "Tanaris"          }, // 4 - Kalimdor
    { 40,  0, "Westfall"         }, // 5 - EK
};

// Same order as s_zoneInfos
static const DemonInvasionScenarioInfo s_scenarioInfos[DI_ZONE_MAX] =
{
    { 958, 46009, 375, 53355, 53356, 2, 53358, 53359, 1600, 53360 }, // Azshara: Defend the Rear Gate
    { 959, 46007, 370, 52445, 52404, 2, 52253, 52254, 1250, 52855 }, // Dun Morogh: Defend Kharanos
    { 916, 46006, 375, 52896, 52897, 2, 52922, 52923, 1650, 53002 }, // Hillsbrad: Defend Tarren Mill
    { 907, 45729, 525, 51385, 51425, 2, 51162, 51163, 2150, 51382 }, // Northern Barrens: Defend the Crossroads
    { 806, 46005, 300, 51823, 51810, 2, 50990, 50991, 1900, 52035 }, // Tanaris: Defend Gadgetzan
    { 897, 46003, 440, 50503, 50532, 3, 50815, 50816, 2000, 51365 }, // Westfall: Defend Sentinel Hill
};

// Kills one player needs to fill the defend and repel bars (the official totals were made for crowds); each more
// player adds 35 % up to ten players, and the demons respawn faster so that there is enough to fight
static constexpr uint32 DI_SOLO_DEFEND_KILLS     = 50;
static constexpr uint32 DI_SOLO_REPEL_KILLS      = 100;
static constexpr uint32 DI_EXTRA_PLAYER_PCT      = 35;
static constexpr uint32 DI_MAX_COUNTED_PLAYERS   = 10;
static constexpr uint32 DI_SOLO_RESPAWN_SECS     = 300;
static constexpr uint32 DI_MIN_RESPAWN_SECS      = 60;
// from level 100 the invasion gives half the experience, so that it does not race the new characters through 100-110
static constexpr uint32 DI_XP_ABOVE_100_PCT      = 50;

static const uint32 s_killThresholds[DI_STAGE_MAX] =
{
    0,    // INACTIVE - unused
    50,   // DEFEND - kill demons around hub
    3,    // COMMANDER - kill commander + 2 lieutenants
    100,  // REPEL - clear zone
    1,    // BOSS - kill the final boss
};

static const char* s_stageNames[DI_STAGE_MAX] =
{
    "Inactive", "Defend", "Commander", "Repel", "Boss"
};

// Vendor item catalog
static const DemonInvasionVendorItem s_vendorItems[] =
{
    { ITEM_DI_CLOTH_SET,      "Fel-Touched Cloth Set",            200 },
    { ITEM_DI_LEATHER_SET,    "Fel-Touched Leather Set",          200 },
    { ITEM_DI_MAIL_SET,       "Fel-Touched Mail Set",             200 },
    { ITEM_DI_PLATE_SET,      "Fel-Touched Plate Set",            200 },
    { ITEM_DI_CLOAK,          "Demon Commander's Cloak (ilvl 700)", 50 },
    { ITEM_DI_RING,           "Infernal Signet (ilvl 700)",        50 },
    { ITEM_DI_NECK,           "Felstalker's Pendant (ilvl 700)",   50 },
    { ITEM_DI_TRINKET,        "Legion Doomguard Trinket (ilvl 700)", 50 },
    { ITEM_DI_FELSTONE,       "Felstone (dmg proc)",                3 },
    { ITEM_DI_PROTECTION_POT, "Potion of Demonic Protection",       3 },
    { ITEM_DI_BANDAGE,        "Fel-Mended Bandage",                 1 },
};
static constexpr uint32 DI_VENDOR_ITEM_COUNT = sizeof(s_vendorItems) / sizeof(s_vendorItems[0]);

// ============================================================================
// DemonInvasionMgr — singleton implementation
// ============================================================================

DemonInvasionMgr::DemonInvasionMgr()
    : m_rotationTimer(0)
    , m_initialized(false)
{
    for (uint8 i = 0; i < DI_ZONE_MAX; ++i)
    {
        m_zoneStates[i].stage = DI_STAGE_INACTIVE;
        m_zoneStates[i].stageProgress = 0;
    }
    for (uint8 i = 0; i < DI_ZONE_MAX; ++i)
    {
        m_scenarios[i] = nullptr;
        m_pointCarry[i] = 0;
    }
    m_outdoorByMap[0] = m_outdoorByMap[1] = nullptr;
}

DemonInvasionMgr* DemonInvasionMgr::instance()
{
    static DemonInvasionMgr inst;
    return &inst;
}

void DemonInvasionMgr::Initialize()
{
    if (m_initialized)
        return;

    // world states are saved: a marker left lit by a crash or by a disabled event must not survive the restart
    static bool markersCleared = false;
    if (!markersCleared)
    {
        markersCleared = true;
        for (uint8 i = 0; i < DI_ZONE_MAX; ++i)
            SetMapMarker(i, false);
    }

    // Pre-patch event: off by default, it has no place on a realm opening on established
    // Legion content.
    if (!sWorld->getBoolConfig(CONFIG_DEMON_INVASION_PREPATCH_ENABLE))
        return;

    m_initialized = true;
    m_rotationTimer = 10 * 1000; // First rotation 10s after init
    LoadCreatureRoles();

    TC_LOG_INFO("server.loading", "[DemonInvasion] Prepatch invasion system initialized.");
}

void DemonInvasionMgr::Update(uint32 diff)
{
    for (Scenario* scenario : m_endedScenarios)
        delete scenario;
    m_endedScenarios.clear();

    if (!m_initialized)
        return;

    if (m_rotationTimer <= diff)
    {
        RotateInvasions();  // sets the next delay
    }
    else
        m_rotationTimer -= diff;
}

// ============================================================================
// Static helpers
// ============================================================================

const DemonInvasionZoneInfo& DemonInvasionMgr::GetZoneInfo(uint8 index)
{
    static const DemonInvasionZoneInfo s_invalid = { 0, 0, "Unknown" };
    if (index >= DI_ZONE_MAX)
        return s_invalid;
    return s_zoneInfos[index];
}

uint16 DemonInvasionMgr::GetGameEventId(uint8 zoneIndex, uint8 stage)
{
    // stage 1..4 -> offset 0..3
    if (stage < 1 || stage > 4 || zoneIndex >= DI_ZONE_MAX)
        return 0;
    return DI_GAME_EVENT_BASE + (zoneIndex * 4) + (stage - 1);
}

uint32 DemonInvasionMgr::GetKillThreshold(uint8 stage)
{
    if (stage >= DI_STAGE_MAX)
        return 0;
    return s_killThresholds[stage];
}

bool DemonInvasionMgr::IsInvasionCreature(uint32 entry)
{
    return sDemonInvasionMgr->GetCreatureRole(entry) != DI_ROLE_NONE;
}

void DemonInvasionMgr::LoadCreatureRoles()
{
    m_roles.clear();
    if (QueryResult result = WorldDatabase.Query("SELECT `entry`, `role` FROM `demon_invasion_creature`"))
    {
        do
        {
            Field* fields = result->Fetch();
            uint8 role = fields[1].GetUInt8();
            if (role > DI_ROLE_NONE && role < DI_ROLE_MAX)
                m_roles[fields[0].GetUInt32()] = role;
        }
        while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", "[DemonInvasion] %u invasion creatures loaded.", uint32(m_roles.size()));
}

DemonInvasionRole DemonInvasionMgr::GetCreatureRole(uint32 entry) const
{
    auto itr = m_roles.find(entry);
    return itr != m_roles.end() ? DemonInvasionRole(itr->second) : DI_ROLE_NONE;
}

// the demons come back faster when the zone is crowded (respawn delay used when the corpse is removed)
void DemonInvasionMgr::OnInvasionCreatureDied(Creature* creature) const
{
    if (!creature)
        return;

    DemonInvasionRole role = GetCreatureRole(creature->GetEntry());
    if (role == DI_ROLE_DEMON || role == DI_ROLE_ELITE)
        creature->SetRespawnDelay(GetDemonRespawnDelay(creature->GetCurrentZoneID()));
}

// ============================================================================
// Core invasion mechanics
// ============================================================================

void DemonInvasionMgr::StartInvasion(uint8 zoneIndex)
{
    if (zoneIndex >= DI_ZONE_MAX)
        return;

    auto& state = m_zoneStates[zoneIndex];
    state.stage = DI_STAGE_DEFEND;
    state.stageProgress = 0;

    uint16 eventId = GetGameEventId(zoneIndex, DI_STAGE_DEFEND);
    if (eventId)
        sGameEventMgr->StartEvent(eventId, true);

    CreateScenario(zoneIndex);
    UpdateInvasionSkyInZone(zoneIndex);
    SetMapMarker(zoneIndex, true);

    TC_LOG_INFO("scripts", "[DemonInvasion] Invasion STARTED in %s (zone %u), event %u",
        s_zoneInfos[zoneIndex].name, s_zoneInfos[zoneIndex].zoneId, eventId);

    BroadcastToZone(zoneIndex,
        "|cFFFF0000[Demon Invasion]|r The Burning Legion is invading %s! Defend the area!",
        s_zoneInfos[zoneIndex].name);
}

void DemonInvasionMgr::StopInvasion(uint8 zoneIndex, bool repelled)
{
    if (zoneIndex >= DI_ZONE_MAX)
        return;

    auto& state = m_zoneStates[zoneIndex];

    // Stop current stage event
    if (state.stage > DI_STAGE_INACTIVE && state.stage < DI_STAGE_MAX)
    {
        uint16 eventId = GetGameEventId(zoneIndex, state.stage);
        if (eventId)
            sGameEventMgr->StopEvent(eventId, true);
    }

    state.stage = DI_STAGE_INACTIVE;
    state.stageProgress = 0;

    DestroyScenario(zoneIndex, repelled);
    UpdateInvasionSkyInZone(zoneIndex);
    SetMapMarker(zoneIndex, false);

    TC_LOG_INFO("scripts", "[DemonInvasion] Invasion STOPPED in %s",
        s_zoneInfos[zoneIndex].name);

    BroadcastToZone(zoneIndex,
        "|cFF00FF00[Demon Invasion]|r The invasion in %s has been repelled!",
        s_zoneInfos[zoneIndex].name);
}

void DemonInvasionMgr::AdvanceStage(uint8 zoneIndex)
{
    if (zoneIndex >= DI_ZONE_MAX)
        return;

    auto& state = m_zoneStates[zoneIndex];

    // If boss stage just completed -> invasion over
    if (state.stage == DI_STAGE_BOSS)
    {
        BroadcastToZone(zoneIndex,
            "|cFF00FF00[Demon Invasion]|r The demon lord in %s has been vanquished!",
            s_zoneInfos[zoneIndex].name);

        // Phase 4: Boss kill rewards (nethershards + big XP + achievement tracking)
        OnBossKilled(zoneIndex);

        StopInvasion(zoneIndex, true);
        return;
    }

    if (state.stage == DI_STAGE_INACTIVE)
        return;

    // Stop current stage event
    uint16 oldEventId = GetGameEventId(zoneIndex, state.stage);
    if (oldEventId)
        sGameEventMgr->StopEvent(oldEventId, true);

    uint8 oldStage = static_cast<uint8>(state.stage);

    // Advance to next stage
    state.stage = static_cast<DemonInvasionStage>(static_cast<uint8>(state.stage) + 1);
    state.stageProgress = 0;

    // Start new stage event
    uint16 newEventId = GetGameEventId(zoneIndex, state.stage);
    if (newEventId)
        sGameEventMgr->StartEvent(newEventId, true);

    TC_LOG_INFO("scripts", "[DemonInvasion] %s advanced to stage %u (%s), event %u",
        s_zoneInfos[zoneIndex].name, state.stage, s_stageNames[state.stage], newEventId);

    BroadcastToZone(zoneIndex,
        "|cFFFFFF00[Demon Invasion]|r %s - Stage %u: %s!",
        s_zoneInfos[zoneIndex].name, state.stage, s_stageNames[state.stage]);

    // Phase 4: Stage completion rewards (nethershards + XP to all players in zone)
    uint32 xpMult = (oldStage == DI_STAGE_REPEL) ? DI_XP_STAGE_REPEL : DI_XP_STAGE_ADVANCE;
    RewardPlayersInZone(zoneIndex, DI_SHARDS_STAGE_BONUS, xpMult);
}

// Official rhythm of the 2016 pre-patch, picked in the .conf: two zones every four hours (first week), three every
// two hours (second week), every zone at once (last week)
uint8 DemonInvasionMgr::GetInvasionWeek()
{
    return uint8(sWorld->getIntConfig(CONFIG_DEMON_INVASION_PREPATCH_WEEK));
}

void DemonInvasionMgr::RotateInvasions()
{
    uint8 week = GetInvasionWeek();

    if (week >= 3)
    {
        // every zone at once: a repelled zone comes back, the fights in progress go on
        for (uint8 i = 0; i < DI_ZONE_MAX; ++i)
            if (m_zoneStates[i].stage == DI_STAGE_INACTIVE)
                StartInvasion(i);

        m_rotationTimer = DI_CONSTANT_RESTART;
        return;
    }

    for (uint8 i = 0; i < DI_ZONE_MAX; ++i)
        if (m_zoneStates[i].stage != DI_STAGE_INACTIVE)
            StopInvasion(i);

    std::vector<uint8> candidates;
    candidates.reserve(DI_ZONE_MAX);
    for (uint8 i = 0; i < DI_ZONE_MAX; ++i)
        candidates.push_back(i);

    for (int32 i = int32(candidates.size()) - 1; i > 0; --i)
        std::swap(candidates[i], candidates[urand(0, uint32(i))]);

    uint8 count = week == 1 ? 2 : 3;
    for (uint8 i = 0; i < count; ++i)
        StartInvasion(candidates[i]);

    m_rotationTimer = (week == 1 ? 4 : 2) * HOUR * IN_MILLISECONDS;

    TC_LOG_INFO("scripts", "[DemonInvasion] Week %u: %u zones invaded, next rotation in %u hours.",
        week, count, m_rotationTimer / (HOUR * IN_MILLISECONDS));
}

void DemonInvasionMgr::OnCreatureKill(uint32 zoneId, uint32 creatureEntry, Player* killer)
{
    uint8 zi = GetZoneIndexByZoneId(zoneId);
    if (zi >= DI_ZONE_MAX)
        return;

    auto& state = m_zoneStates[zi];
    if (state.stage == DI_STAGE_INACTIVE)
        return;

    if (!IsInvasionCreature(creatureEntry))
        return;

    // Phase 4: Nethershard drop to killer
    if (killer)
    {
        uint32 shards = GetNethershardDropAmount(creatureEntry, state.stage);
        if (shards > 0)
        {
            killer->AddItem(ITEM_NETHERSHARD, shards);
            ChatHandler(killer->GetSession()).PSendSysMessage(
                "|cFF00FF00+%u Nethershard%s|r", shards, shards > 1 ? "s" : "");
        }
    }

    // the official scenario counts the kills and moves the stages (OnScenarioStep); the demon lord ends it here
    if (SendKillToScenario(zi, creatureEntry, killer))
    {
        if (state.stage == DI_STAGE_BOSS && GetCreatureRole(creatureEntry) == DI_ROLE_DEMON_LORD)
            AdvanceStage(zi);
        return;
    }

    ++state.stageProgress;

    uint32 threshold = GetKillThreshold(state.stage);
    if (threshold > 0 && state.stageProgress >= threshold)
    {
        TC_LOG_INFO("scripts", "[DemonInvasion] %s stage %u complete (%u/%u kills)",
            s_zoneInfos[zi].name, state.stage, state.stageProgress, threshold);
        AdvanceStage(zi);
    }
}

// ============================================================================
// GM Commands
// ============================================================================

bool DemonInvasionMgr::ForceStart(uint32 zoneId)
{
    uint8 zi = GetZoneIndexByZoneId(zoneId);
    if (zi >= DI_ZONE_MAX)
        return false;

    if (!m_initialized)
        Initialize();

    if (m_zoneStates[zi].stage != DI_STAGE_INACTIVE)
        StopInvasion(zi);

    StartInvasion(zi);
    return true;
}

void DemonInvasionMgr::ForceStop()
{
    for (uint8 i = 0; i < DI_ZONE_MAX; ++i)
        if (m_zoneStates[i].stage != DI_STAGE_INACTIVE)
            StopInvasion(i);
}

void DemonInvasionMgr::ForceStage(uint8 zoneIndex, uint8 stage)
{
    if (zoneIndex >= DI_ZONE_MAX || stage >= DI_STAGE_MAX)
        return;

    auto& state = m_zoneStates[zoneIndex];

    // Stop current event if active
    if (state.stage > DI_STAGE_INACTIVE && state.stage < DI_STAGE_MAX)
    {
        uint16 oldEvent = GetGameEventId(zoneIndex, state.stage);
        if (oldEvent)
            sGameEventMgr->StopEvent(oldEvent, true);
    }

    if (stage == DI_STAGE_INACTIVE)
    {
        state.stage = DI_STAGE_INACTIVE;
        state.stageProgress = 0;
        DestroyScenario(zoneIndex, false);
        UpdateInvasionSkyInZone(zoneIndex);
        SetMapMarker(zoneIndex, false);
        return;
    }

    state.stage = static_cast<DemonInvasionStage>(stage);
    state.stageProgress = 0;

    uint16 newEvent = GetGameEventId(zoneIndex, state.stage);
    if (newEvent)
        sGameEventMgr->StartEvent(newEvent, true);

    if (!m_scenarios[zoneIndex])
        CreateScenario(zoneIndex);
    if (Scenario* scenario = m_scenarios[zoneIndex])
        scenario->SetCurrentStep(stage - 1);
    UpdateInvasionSkyInZone(zoneIndex);
    SetMapMarker(zoneIndex, true);

    TC_LOG_INFO("scripts", "[DemonInvasion] GM forced %s to stage %u (%s), event %u",
        s_zoneInfos[zoneIndex].name, stage, s_stageNames[stage], newEvent);
}

void DemonInvasionMgr::ForceRotate()
{
    RotateInvasions();
}

// ============================================================================
// Queries
// ============================================================================

bool DemonInvasionMgr::IsZoneActive(uint8 zoneIndex) const
{
    if (zoneIndex >= DI_ZONE_MAX)
        return false;
    return m_zoneStates[zoneIndex].stage != DI_STAGE_INACTIVE;
}

DemonInvasionStage DemonInvasionMgr::GetZoneStage(uint8 zoneIndex) const
{
    if (zoneIndex >= DI_ZONE_MAX)
        return DI_STAGE_INACTIVE;
    return m_zoneStates[zoneIndex].stage;
}

uint8 DemonInvasionMgr::GetZoneIndexByZoneId(uint32 zoneId) const
{
    for (uint8 i = 0; i < DI_ZONE_MAX; ++i)
        if (s_zoneInfos[i].zoneId == zoneId)
            return i;
    return DI_ZONE_MAX;
}

std::string DemonInvasionMgr::GetStatusString() const
{
    std::ostringstream ss;
    ss << "|cFFFFD700=== Demon Invasion Status ===|r\n";
    ss << "Rotation timer: " << (m_rotationTimer / 1000) << "s ("
       << (m_rotationTimer / 60000) << " min)\n";
    ss << "Rhythm (week): " << uint32(GetInvasionWeek()) << "\n";
    for (uint8 i = 0; i < DI_ZONE_MAX; ++i)
    {
        ss << "  " << s_zoneInfos[i].name
           << " (zone " << s_zoneInfos[i].zoneId << "): "
           << s_stageNames[m_zoneStates[i].stage];
        if (m_zoneStates[i].stage != DI_STAGE_INACTIVE)
        {
            ss << " [" << m_zoneStates[i].stageProgress
               << "/" << s_killThresholds[m_zoneStates[i].stage] << " kills]";
        }
        ss << "\n";
    }
    return ss.str();
}

void DemonInvasionMgr::BroadcastToZone(uint8 zoneIndex, const char* format, ...)
{
    char buffer[512];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (zoneIndex < DI_ZONE_MAX)
    {
        uint32 zoneId = s_zoneInfos[zoneIndex].zoneId;
        sWorld->SendZoneText(zoneId, buffer);
    }
}

void DemonInvasionMgr::UpdateInvasionSky(Player* player, uint32 zoneId) const
{
    if (!player)
        return;

    uint8 zoneIndex = GetZoneIndexByZoneId(zoneId);
    bool invaded = zoneIndex < DI_ZONE_MAX && IsZoneActive(zoneIndex);
    if (invaded && !player->HasAura(SPELL_DI_INVASION_SKY))
        player->AddAura(SPELL_DI_INVASION_SKY, player);
    else if (!invaded)
        player->RemoveAurasDueToSpell(SPELL_DI_INVASION_SKY);
}

template<class Fn>
void DemonInvasionMgr::ForEachPlayerInZone(uint8 zoneIndex, Fn&& fn) const
{
    if (zoneIndex >= DI_ZONE_MAX)
        return;

    DemonInvasionZoneInfo const& info = s_zoneInfos[zoneIndex];
    Map* map = sMapMgr->FindMap(info.mapId, 0);
    if (!map)
        return;

    Map::PlayerList const& players = map->GetPlayers();
    for (auto itr = players.begin(); itr != players.end(); ++itr)
        if (Player* player = itr->getSource())
            if (player->GetCurrentZoneID() == info.zoneId)
                fn(player);
}

void DemonInvasionMgr::UpdateInvasionSkyInZone(uint8 zoneIndex) const
{
    if (zoneIndex >= DI_ZONE_MAX)
        return;

    uint32 zoneId = s_zoneInfos[zoneIndex].zoneId;
    ForEachPlayerInZone(zoneIndex, [this, zoneId](Player* player) { UpdateInvasionSky(player, zoneId); });
}

void DemonInvasionMgr::RegisterOutdoorPvP(uint32 mapId, OutdoorPvP* outdoor)
{
    if (mapId < 2)
        m_outdoorByMap[mapId] = outdoor;
}

void DemonInvasionMgr::CreateScenario(uint8 zoneIndex)
{
    if (zoneIndex >= DI_ZONE_MAX || m_scenarios[zoneIndex])
        return;

    DemonInvasionZoneInfo const& info = s_zoneInfos[zoneIndex];
    OutdoorPvP* outdoor = info.mapId < 2 ? m_outdoorByMap[info.mapId] : nullptr;
    Map* map = outdoor ? outdoor->GetMap() : nullptr;
    if (!map)
        return;

    Scenario* scenario = new Scenario(map, s_scenarioInfos[zoneIndex].scenarioId);
    scenario->SetOutdoorPvP(outdoor, info.zoneId);
    m_scenarios[zoneIndex] = scenario;
    m_pointCarry[zoneIndex] = 0;
    scenario->SetCurrentStep(0);

    ForEachPlayerInZone(zoneIndex, [scenario](Player* player) { scenario->SendStepUpdate(player, true); });
}

void DemonInvasionMgr::DestroyScenario(uint8 zoneIndex, bool repelled)
{
    if (zoneIndex >= DI_ZONE_MAX || !m_scenarios[zoneIndex])
        return;

    Scenario* scenario = m_scenarios[zoneIndex];
    m_scenarios[zoneIndex] = nullptr;

    uint32 scenarioId = s_scenarioInfos[zoneIndex].scenarioId;
    // a repelled invasion has already shown its completion (Scenario::Reward): leave it on screen a moment
    ForEachPlayerInZone(zoneIndex, [scenarioId, repelled](Player* player)
    {
        auto boot = [player, scenarioId]()
        {
            WorldPackets::Scenario::ScenarioBoot packet;
            packet.ScenarioID = scenarioId;
            player->SendDirectMessage(packet.Write());
        };

        if (repelled)
            player->AddDelayedEvent(10 * IN_MILLISECONDS, boot);
        else
            boot();
    });

    m_endedScenarios.push_back(scenario);
}

void DemonInvasionMgr::SendScenarioEvent(uint8 zoneIndex, uint32 eventId, uint32 count, Player* reference)
{
    if (zoneIndex < DI_ZONE_MAX && m_scenarios[zoneIndex] && eventId && count && reference)
        m_scenarios[zoneIndex]->UpdateAchievementCriteria(CRITERIA_TYPE_SCRIPT_EVENT_2, eventId, count, 0, nullptr, reference);
}

bool DemonInvasionMgr::SendKillToScenario(uint8 zoneIndex, uint32 creatureEntry, Player* killer)
{
    if (zoneIndex >= DI_ZONE_MAX || !m_scenarios[zoneIndex] || !killer)
        return false;

    DemonInvasionScenarioInfo const& info = s_scenarioInfos[zoneIndex];
    uint32 scalePct = 100 + DI_EXTRA_PLAYER_PCT * (GetParticipantCount(s_zoneInfos[zoneIndex].zoneId) - 1);
    uint32 defendKills = DI_SOLO_DEFEND_KILLS * scalePct / 100;
    uint32 repelKills = DI_SOLO_REPEL_KILLS * scalePct / 100;

    DemonInvasionRole role = GetCreatureRole(creatureEntry);
    uint32 weight = role == DI_ROLE_ELITE ? 3 : 1;

    switch (m_zoneStates[zoneIndex].stage)
    {
        case DI_STAGE_DEFEND:
            if (role == DI_ROLE_DEMON || role == DI_ROLE_ELITE)
                SendScenarioEvent(zoneIndex, info.defendEvent, TakeBarPoints(zoneIndex, info.defendPoints, defendKills, weight), killer);
            break;
        case DI_STAGE_COMMANDER:
            if (role == DI_ROLE_COMMANDER)
                SendScenarioEvent(zoneIndex, info.commanderEvent, 1, killer);
            else if (role == DI_ROLE_LIEUTENANT)
                SendScenarioEvent(zoneIndex, info.lieutenantEvent, (info.lieutenantCount + 1) / 2, killer); // two lieutenants are spawned
            break;
        case DI_STAGE_REPEL:  // the repel bar reuses the defend counter, already full
            if (role == DI_ROLE_DEMON || role == DI_ROLE_ELITE)
                SendScenarioEvent(zoneIndex, info.defendEvent, TakeBarPoints(zoneIndex, info.repelPoints - info.defendPoints, repelKills, weight), killer);
            break;
        case DI_STAGE_BOSS:
            if (role == DI_ROLE_DEMON_LORD)
                SendScenarioEvent(zoneIndex, info.demonLordEvent, 1, killer);
            break;
        default:
            return false;
    }

    return true;
}

void DemonInvasionMgr::SetMapMarker(uint8 zoneIndex, bool invaded) const
{
    if (zoneIndex < DI_ZONE_MAX)
        sWorldStateMgr.SetWorldState(DI_MAP_MARKER_WORLD_STATE + zoneIndex, 0, invaded ? 1 : 0);
}

uint32 DemonInvasionMgr::TakeBarPoints(uint8 zoneIndex, uint32 barPoints, uint32 kills, uint32 killWeight)
{
    m_pointCarry[zoneIndex] += barPoints * 1000 * killWeight / std::max<uint32>(kills, 1);
    uint32 points = m_pointCarry[zoneIndex] / 1000;
    m_pointCarry[zoneIndex] %= 1000;
    return points;
}

void DemonInvasionMgr::OnPlayerEnterZone(Player* player, uint32 zoneId)
{
    if (!player)
        return;

    UpdateInvasionSky(player, zoneId);

    uint8 zoneIndex = GetZoneIndexByZoneId(zoneId);
    if (zoneIndex < DI_ZONE_MAX && m_scenarios[zoneIndex])
        m_scenarios[zoneIndex]->SendStepUpdate(player, true);
}

void DemonInvasionMgr::OnPlayerLeaveZone(Player* player, uint32 zoneId)
{
    if (!player)
        return;

    player->RemoveAurasDueToSpell(SPELL_DI_INVASION_SKY);

    uint8 zoneIndex = GetZoneIndexByZoneId(zoneId);
    if (zoneIndex < DI_ZONE_MAX && m_scenarios[zoneIndex])
    {
        WorldPackets::Scenario::ScenarioBoot boot;
        boot.ScenarioID = s_scenarioInfos[zoneIndex].scenarioId;
        player->SendDirectMessage(boot.Write());
    }
}

uint32 DemonInvasionMgr::GetParticipantCount(uint32 zoneId) const
{
    uint32 count = 0;
    ForEachPlayerInZone(GetZoneIndexByZoneId(zoneId), [&count](Player* player)
    {
        if (!player->isGameMaster())
            ++count;
    });

    return std::min(std::max<uint32>(count, 1), DI_MAX_COUNTED_PLAYERS);
}

uint32 DemonInvasionMgr::GetDemonRespawnDelay(uint32 zoneId) const
{
    return std::max(DI_SOLO_RESPAWN_SECS / GetParticipantCount(zoneId), DI_MIN_RESPAWN_SECS);
}

void DemonInvasionMgr::OnScenarioStep(uint32 zoneId, uint32 step)
{
    uint8 zoneIndex = GetZoneIndexByZoneId(zoneId);
    if (zoneIndex >= DI_ZONE_MAX)
        return;

    // scenario step 0..3 = stage 1..4; the demon lord step never reaches here (the scenario completes instead)
    auto& state = m_zoneStates[zoneIndex];
    while (state.stage != DI_STAGE_INACTIVE && state.stage < DI_STAGE_BOSS && uint32(state.stage) < step + 1)
        AdvanceStage(zoneIndex);
}

// ============================================================================
// Phase 4: Static helpers
// ============================================================================

uint32 DemonInvasionMgr::GetBaseQuestXP(uint8 level)
{
    // Approximate XP per quest at various level brackets
    if (level >= 100) return 15000;
    if (level >= 90)  return 13000;
    if (level >= 80)  return 10000;
    if (level >= 70)  return 8000;
    if (level >= 60)  return 6000;
    if (level >= 40)  return 4000;
    if (level >= 20)  return 2000;
    return level * 100;
}

uint32 DemonInvasionMgr::GetNethershardDropAmount(uint32 creatureEntry, uint8 stage)
{
    switch (sDemonInvasionMgr->GetCreatureRole(creatureEntry))
    {
        case DI_ROLE_DEMON_LORD:
            return DI_SHARDS_BOSS_KILL;
        case DI_ROLE_COMMANDER:
        case DI_ROLE_LIEUTENANT:
            return urand(5, 10);
        case DI_ROLE_ELITE:
            return stage == DI_STAGE_REPEL ? urand(5, 15) : 0; // official hotfix: minibosses of the repel stage
        case DI_ROLE_DEMON:
            return stage == DI_STAGE_REPEL ? urand(1, 2) : 0;
        default:
            return 0;
    }
}

// ============================================================================
// Phase 4: RewardPlayersInZone — distributes nethershards + XP to zone
// ============================================================================

void DemonInvasionMgr::RewardPlayersInZone(uint8 zoneIndex, uint32 nethershards, uint32 xpMultiplier)
{
    if (zoneIndex >= DI_ZONE_MAX)
        return;

    uint32 zoneId = s_zoneInfos[zoneIndex].zoneId;
    SessionMap const& sessions = sWorld->GetAllSessions();

    for (auto const& itr : sessions)
    {
        if (!itr.second || !itr.second->GetPlayer())
            continue;

        Player* player = itr.second->GetPlayer();
        if (!player->IsInWorld() || player->GetCurrentZoneID() != zoneId)
            continue;

        // Nethershards
        if (nethershards > 0)
        {
            player->AddItem(ITEM_NETHERSHARD, nethershards);
            ChatHandler(player->GetSession()).PSendSysMessage(
                "|cFF00FF00+%u Nethershard%s (stage bonus)|r", nethershards, nethershards > 1 ? "s" : "");
        }

        // XP
        if (xpMultiplier > 0 && player->getLevel() < MAX_LEVEL)
        {
            uint32 xp = GetBaseQuestXP(player->getLevel()) * xpMultiplier;
            if (player->getLevel() >= 100)
                xp = xp * DI_XP_ABOVE_100_PCT / 100;
            if (!xp)
                continue;
            player->GiveXP(xp, nullptr);
            ChatHandler(player->GetSession()).PSendSysMessage(
                "|cFF8080FF+%u XP (invasion bonus)|r", xp);
        }
    }
}

// ============================================================================
// Phase 4: OnBossKilled — boss death rewards + achievement tracking
// ============================================================================

void DemonInvasionMgr::OnBossKilled(uint8 zoneIndex)
{
    if (zoneIndex >= DI_ZONE_MAX)
        return;

    uint32 zoneId = s_zoneInfos[zoneIndex].zoneId;
    SessionMap const& sessions = sWorld->GetAllSessions();

    for (auto const& itr : sessions)
    {
        if (!itr.second || !itr.second->GetPlayer())
            continue;

        Player* player = itr.second->GetPlayer();
        if (!player->IsInWorld() || player->GetCurrentZoneID() != zoneId)
            continue;

        // Boss kill nethershards
        player->AddItem(ITEM_NETHERSHARD, DI_SHARDS_BOSS_KILL);
        ChatHandler(player->GetSession()).PSendSysMessage(
            "|cFF00FF00+%u Nethershards (boss kill)|r", DI_SHARDS_BOSS_KILL);

        // Boss kill XP
        if (player->getLevel() < MAX_LEVEL)
        {
            uint32 xp = GetBaseQuestXP(player->getLevel()) * DI_XP_BOSS_KILL;
            player->GiveXP(xp, nullptr);
            ChatHandler(player->GetSession()).PSendSysMessage(
                "|cFF8080FF+%u XP (boss kill)|r", xp);
        }

        // Achievement tracking
        TrackInvasionCompletion(player, zoneIndex);
    }
}

// ============================================================================
// Phase 4: Achievement tracking — character_demon_invasion_progress
// ============================================================================

void DemonInvasionMgr::TrackInvasionCompletion(Player* player, uint8 zoneIndex)
{
    if (!player || zoneIndex >= DI_ZONE_MAX)
        return;

    uint32 guid = player->GetGUID().GetCounter();
    uint32 now = static_cast<uint32>(time(nullptr));

    // Insert/update completion record (async)
    CharacterDatabase.PExecute(
        "REPLACE INTO character_demon_invasion_progress (guid, zone_index, completed_time) VALUES (%u, %u, %u)",
        guid, static_cast<uint32>(zoneIndex), now);

    TC_LOG_DEBUG("scripts", "[DemonInvasion] Player %s (%u) completed invasion in %s",
        player->GetName(), guid, s_zoneInfos[zoneIndex].name);

    // Check if all 6 zones completed
    if (HasCompletedAllInvasions(player))
    {
        // Announce to the player
        ChatHandler(player->GetSession()).PSendSysMessage(
            "|cFFFFD700[Demon Invasion]|r Congratulations! You have repelled the Legion across all six zones!");

        // Zone-wide announcement
        BroadcastToZone(zoneIndex,
            "|cFFFFD700[Demon Invasion]|r %s has completed all six demon invasions! A true defender of Azeroth!",
            player->GetName());

        // Bonus reward: extra nethershards
        player->AddItem(ITEM_NETHERSHARD, 100);
        ChatHandler(player->GetSession()).PSendSysMessage(
            "|cFF00FF00+100 Nethershards (all invasions complete)|r");
    }
}

bool DemonInvasionMgr::HasCompletedAllInvasions(Player* player)
{
    if (!player)
        return false;

    uint32 guid = player->GetGUID().GetCounter();

    QueryResult result = CharacterDatabase.PQuery(
        "SELECT COUNT(DISTINCT zone_index) FROM character_demon_invasion_progress WHERE guid = %u", guid);

    if (!result)
        return false;

    uint32 completedZones = (*result)[0].GetUInt32();
    return completedZones >= DI_ZONE_MAX;
}

// ============================================================================
// Phase 4: npc_demon_invasion_vendor — Nethershard Gossip Vendor
// ============================================================================

class npc_demon_invasion_vendor : public CreatureScript
{
public:
    npc_demon_invasion_vendor() : CreatureScript("npc_demon_invasion_vendor") {}

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        player->PlayerTalkClass->ClearMenus();

        // Show nethershard balance
        uint32 balance = player->GetItemCount(ITEM_NETHERSHARD);

        // Header option (non-clickable info)
        char header[128];
        snprintf(header, sizeof(header), "[Your Nethershards: %u]", balance);
        player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, header,
            GOSSIP_SENDER_MAIN, 0);

        // Vendor items
        for (uint32 i = 0; i < DI_VENDOR_ITEM_COUNT; ++i)
        {
            char label[256];
            snprintf(label, sizeof(label), "%s — %u Nethershards",
                s_vendorItems[i].name, s_vendorItems[i].nethershardCost);

            player->ADD_GOSSIP_ITEM(GossipOptionNpc::Vendor, label,
                GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1 + i);
        }

        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();

        // Header click (action 0) — just refresh
        if (action == 0)
        {
            OnGossipHello(player, creature);
            return true;
        }

        uint32 itemIndex = action - (GOSSIP_ACTION_INFO_DEF + 1);
        if (itemIndex >= DI_VENDOR_ITEM_COUNT)
        {
            player->CLOSE_GOSSIP_MENU();
            return true;
        }

        auto const& vendorItem = s_vendorItems[itemIndex];
        uint32 balance = player->GetItemCount(ITEM_NETHERSHARD);

        if (balance < vendorItem.nethershardCost)
        {
            ChatHandler(player->GetSession()).PSendSysMessage(
                "|cFFFF0000Not enough Nethershards! You have %u, need %u.|r",
                balance, vendorItem.nethershardCost);
            OnGossipHello(player, creature);
            return true;
        }

        // Check if player can receive the item
        ItemPosCountVec dest;
        InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, vendorItem.itemEntry, 1);
        if (msg != EQUIP_ERR_OK)
        {
            ChatHandler(player->GetSession()).PSendSysMessage(
                "|cFFFF0000Your inventory is full!|r");
            OnGossipHello(player, creature);
            return true;
        }

        // Deduct nethershards
        player->DestroyItemCount(ITEM_NETHERSHARD, vendorItem.nethershardCost, true);

        // Give item
        player->AddItem(vendorItem.itemEntry, 1);
        ChatHandler(player->GetSession()).PSendSysMessage(
            "|cFF00FF00Purchased: %s for %u Nethershards.|r",
            vendorItem.name, vendorItem.nethershardCost);

        TC_LOG_INFO("scripts", "[DemonInvasion] Player %s purchased %s (%u) for %u Nethershards",
            player->GetName(), vendorItem.name, vendorItem.itemEntry, vendorItem.nethershardCost);

        // Refresh menu
        OnGossipHello(player, creature);
        return true;
    }
};

// ============================================================================
// OutdoorPvP: Eastern Kingdoms (Dun Morogh, Hillsbrad, Westfall)
// ============================================================================

bool OutdoorPvPDemonInvasionEK::SetupOutdoorPvP()
{
    sDemonInvasionMgr->RegisterOutdoorPvP(0, this);
    RegisterZone(1);    // Dun Morogh
    RegisterZone(267);  // Hillsbrad
    RegisterZone(40);   // Westfall
    return true;
}

void OutdoorPvPDemonInvasionEK::HandlePlayerEnterZone(ObjectGuid guid, uint32 zone)
{
    OutdoorPvP::HandlePlayerEnterZone(guid, zone);
    sDemonInvasionMgr->OnPlayerEnterZone(ObjectAccessor::FindPlayer(guid), zone);
}

void OutdoorPvPDemonInvasionEK::HandlePlayerLeaveZone(ObjectGuid guid, uint32 zone)
{
    sDemonInvasionMgr->OnPlayerLeaveZone(ObjectAccessor::FindPlayer(guid), zone);
    OutdoorPvP::HandlePlayerLeaveZone(guid, zone);
}

void OutdoorPvPDemonInvasionEK::SetData(uint32 zone, uint32 step)
{
    sDemonInvasionMgr->OnScenarioStep(zone, step);
}

void OutdoorPvPDemonInvasionEK::BroadcastPacketByZone(WorldPacket const& data, uint32 zone)
{
    // the base class would send a zone's scenario to the whole continent
    if (Map* map = GetMap())
    {
        Map::PlayerList const& players = map->GetPlayers();
        for (auto itr = players.begin(); itr != players.end(); ++itr)
            if (Player* player = itr->getSource())
                if (player->GetCurrentZoneID() == zone)
                    player->SendDirectMessage(&data);
    }
}

bool OutdoorPvPDemonInvasionEK::Update(uint32 diff)
{
    // EK instance drives the DemonInvasionMgr timer
    if (!sDemonInvasionMgr->IsInitialized())
        sDemonInvasionMgr->Initialize();

    sDemonInvasionMgr->Update(diff);
    return true;
}

void OutdoorPvPDemonInvasionEK::HandleKillImpl(Player* killer, Unit* killed)
{
    if (!killed || !killed->ToCreature())
        return;

    sDemonInvasionMgr->OnInvasionCreatureDied(killed->ToCreature());

    sDemonInvasionMgr->OnCreatureKill(
        killed->GetCurrentZoneID(),
        killed->GetEntry(),
        killer
    );
}

// ============================================================================
// OutdoorPvP: Kalimdor (Azshara, Northern Barrens, Tanaris)
// ============================================================================

bool OutdoorPvPDemonInvasionKal::SetupOutdoorPvP()
{
    sDemonInvasionMgr->RegisterOutdoorPvP(1, this);
    RegisterZone(16);   // Azshara
    RegisterZone(17);   // Northern Barrens
    RegisterZone(440);  // Tanaris
    return true;
}

void OutdoorPvPDemonInvasionKal::HandlePlayerEnterZone(ObjectGuid guid, uint32 zone)
{
    OutdoorPvP::HandlePlayerEnterZone(guid, zone);
    sDemonInvasionMgr->OnPlayerEnterZone(ObjectAccessor::FindPlayer(guid), zone);
}

void OutdoorPvPDemonInvasionKal::HandlePlayerLeaveZone(ObjectGuid guid, uint32 zone)
{
    sDemonInvasionMgr->OnPlayerLeaveZone(ObjectAccessor::FindPlayer(guid), zone);
    OutdoorPvP::HandlePlayerLeaveZone(guid, zone);
}

void OutdoorPvPDemonInvasionKal::SetData(uint32 zone, uint32 step)
{
    sDemonInvasionMgr->OnScenarioStep(zone, step);
}

void OutdoorPvPDemonInvasionKal::BroadcastPacketByZone(WorldPacket const& data, uint32 zone)
{
    // the base class would send a zone's scenario to the whole continent
    if (Map* map = GetMap())
    {
        Map::PlayerList const& players = map->GetPlayers();
        for (auto itr = players.begin(); itr != players.end(); ++itr)
            if (Player* player = itr->getSource())
                if (player->GetCurrentZoneID() == zone)
                    player->SendDirectMessage(&data);
    }
}

bool OutdoorPvPDemonInvasionKal::Update(uint32 diff)
{
    // Kalimdor doesn't drive the timer - EK does
    return true;
}

void OutdoorPvPDemonInvasionKal::HandleKillImpl(Player* killer, Unit* killed)
{
    if (!killed || !killed->ToCreature())
        return;

    sDemonInvasionMgr->OnInvasionCreatureDied(killed->ToCreature());

    sDemonInvasionMgr->OnCreatureKill(
        killed->GetCurrentZoneID(),
        killed->GetEntry(),
        killer
    );
}

// ============================================================================
// GM Command Script: .invasion start/stop/stage/status/rotate
// ============================================================================

class demon_invasion_commandscript : public CommandScript
{
public:
    demon_invasion_commandscript() : CommandScript("demon_invasion_commandscript") {}

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> invasionCommandTable =
        {
            { "start",   SEC_GAMEMASTER, false, &HandleInvasionStartCommand,  "" },
            { "stop",    SEC_GAMEMASTER, false, &HandleInvasionStopCommand,   "" },
            { "stage",   SEC_GAMEMASTER, false, &HandleInvasionStageCommand,  "" },
            { "status",  SEC_GAMEMASTER, false, &HandleInvasionStatusCommand, "" },
            { "rotate",  SEC_GAMEMASTER, false, &HandleInvasionRotateCommand, "" },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "invasion", SEC_GAMEMASTER, false, NULL, "", invasionCommandTable },
            { "demoninvasion", SEC_PLAYER, false, &HandleDemonInvasionCommand, "" },
        };

        return commandTable;
    }

    // .demoninvasion, for every player: where the demons are, at which stage and for how long, or when they come next
    static bool HandleDemonInvasionCommand(ChatHandler* handler, char const* /*args*/)
    {
        LocaleConstant const locale = handler->GetSessionDbcLocale();
        bool const french = locale == LOCALE_frFR;

        if (!sWorld->getBoolConfig(CONFIG_DEMON_INVASION_PREPATCH_ENABLE) || !sDemonInvasionMgr->IsInitialized())
        {
            handler->SendSysMessage(french ? "Aucune invasion démoniaque n'a lieu en ce moment." : "No demon invasion is taking place.");
            return true;
        }

        static char const* const stageFr[DI_STAGE_MAX] = { "", "défendre", "le commandant et ses lieutenants", "repousser les démons", "le seigneur démon" };
        static char const* const stageEn[DI_STAGE_MAX] = { "", "defend", "the commander and his lieutenants", "push the demons back", "the demon lord" };

        uint8 const week = DemonInvasionMgr::GetInvasionWeek();
        std::string const timeLeft = secsToTimeString(sDemonInvasionMgr->GetRotationTimeLeft() / IN_MILLISECONDS, true);

        uint32 active = 0;
        for (uint8 i = 0; i < DI_ZONE_MAX; ++i)
        {
            if (!sDemonInvasionMgr->IsZoneActive(i))
                continue;

            ++active;
            DemonInvasionZoneInfo const& info = DemonInvasionMgr::GetZoneInfo(i);
            AreaTableEntry const* area = sAreaTableStore.LookupEntry(info.zoneId);
            char const* zoneName = area ? area->AreaName->Str[locale] : info.name;
            uint8 const stage = sDemonInvasionMgr->GetZoneStage(i);
            char const* stageName = stage < DI_STAGE_MAX ? (french ? stageFr[stage] : stageEn[stage]) : "";

            // the third rhythm keeps every zone invaded until it is pushed back: no end to tell
            if (week >= 3)
                handler->PSendSysMessage(french ? "|cFFFF4040%s|r : %s, jusqu'à ce que la zone soit repoussée." : "|cFFFF4040%s|r: %s, until the zone is pushed back.", zoneName, stageName);
            else
                handler->PSendSysMessage(french ? "|cFFFF4040%s|r : %s, encore %s." : "|cFFFF4040%s|r: %s, %s left.", zoneName, stageName, timeLeft.c_str());
        }

        if (!active)
            handler->PSendSysMessage(french ? "Aucune invasion démoniaque en cours. La prochaine commence dans %s." : "No demon invasion in progress. The next one starts in %s.", timeLeft.c_str());
        else if (week >= 3 && active < DI_ZONE_MAX)
            handler->PSendSysMessage(french ? "Les zones repoussées sont de nouveau envahies dans %s." : "The zones pushed back are invaded again in %s.", timeLeft.c_str());

        return true;
    }

    // .invasion start [zoneId]
    static bool HandleInvasionStartCommand(ChatHandler* handler, char const* args)
    {
        if (!sDemonInvasionMgr->IsInitialized())
            sDemonInvasionMgr->Initialize();

        uint32 zoneId = 0;
        if (args && args[0])
            zoneId = static_cast<uint32>(atoi(args));
        else
        {
            // Random zone
            zoneId = DemonInvasionMgr::GetZoneInfo(urand(0, DI_ZONE_MAX - 1)).zoneId;
        }

        if (sDemonInvasionMgr->ForceStart(zoneId))
            handler->PSendSysMessage("[DemonInvasion] Invasion started in zone %u (%s).",
                zoneId, DemonInvasionMgr::GetZoneInfo(
                    sDemonInvasionMgr->GetZoneIndexByZoneId(zoneId)).name);
        else
            handler->PSendSysMessage("[DemonInvasion] Invalid zone ID %u. Valid: 16 1 267 10 440 40", zoneId);
        return true;
    }

    // .invasion stop
    static bool HandleInvasionStopCommand(ChatHandler* handler, char const* /*args*/)
    {
        sDemonInvasionMgr->ForceStop();
        handler->PSendSysMessage("[DemonInvasion] All invasions stopped.");
        return true;
    }

    // .invasion stage <zoneIndex 0-5> <stage 0-4>
    static bool HandleInvasionStageCommand(ChatHandler* handler, char const* args)
    {
        if (!args || !args[0])
        {
            handler->PSendSysMessage("Usage: .invasion stage <zoneIndex 0-5> <stage 0-4>");
            handler->PSendSysMessage("Zones: 0=Azshara 1=DunMorogh 2=Hillsbrad 3=Barrens 4=Tanaris 5=Westfall");
            handler->PSendSysMessage("Stages: 0=Inactive 1=Defend 2=Commander 3=Repel 4=Boss");
            return true;
        }

        uint32 zoneIndex = 0, stage = 0;
        if (sscanf(args, "%u %u", &zoneIndex, &stage) != 2)
        {
            handler->PSendSysMessage("Usage: .invasion stage <zoneIndex 0-5> <stage 0-4>");
            return true;
        }

        if (zoneIndex >= DI_ZONE_MAX || stage >= DI_STAGE_MAX)
        {
            handler->PSendSysMessage("[DemonInvasion] Invalid zone index (0-5) or stage (0-4).");
            return true;
        }

        sDemonInvasionMgr->ForceStage(static_cast<uint8>(zoneIndex), static_cast<uint8>(stage));
        handler->PSendSysMessage("[DemonInvasion] %s forced to stage %u (%s).",
            DemonInvasionMgr::GetZoneInfo(static_cast<uint8>(zoneIndex)).name,
            stage, s_stageNames[stage]);
        return true;
    }

    // .invasion status
    static bool HandleInvasionStatusCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (!sDemonInvasionMgr->IsInitialized())
        {
            handler->PSendSysMessage("[DemonInvasion] System not initialized. Use .invasion start or .invasion rotate first.");
            return true;
        }

        handler->SendSysMessage(sDemonInvasionMgr->GetStatusString().c_str());
        return true;
    }

    // .invasion rotate
    static bool HandleInvasionRotateCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (!sDemonInvasionMgr->IsInitialized())
            sDemonInvasionMgr->Initialize();

        sDemonInvasionMgr->ForceRotate();
        handler->PSendSysMessage("[DemonInvasion] Rotation forced. Check .invasion status for active zones.");
        return true;
    }
};

// ============================================================================
// OutdoorPvP Script Registration
// ============================================================================

class OutdoorPvP_DemonInvasion_EK : public OutdoorPvPScript
{
public:
    OutdoorPvP_DemonInvasion_EK() : OutdoorPvPScript("outdoorpvp_demon_invasion_ek") {}

    OutdoorPvP* GetOutdoorPvP() const override
    {
        return new OutdoorPvPDemonInvasionEK();
    }
};

class OutdoorPvP_DemonInvasion_Kal : public OutdoorPvPScript
{
public:
    OutdoorPvP_DemonInvasion_Kal() : OutdoorPvPScript("outdoorpvp_demon_invasion_kal") {}

    OutdoorPvP* GetOutdoorPvP() const override
    {
        return new OutdoorPvPDemonInvasionKal();
    }
};

// ============================================================================
// Script registration entry point
// ============================================================================

// Forward declaration from DemonInvasion_creatures.cpp
void AddSC_demon_invasion_creatures();

// From level 100 the invasion gives less experience (demons and stages), so that it does not race the new characters
// through 100-110
class demon_invasion_xp : public PlayerScript
{
public:
    demon_invasion_xp() : PlayerScript("demon_invasion_xp") {}

    void OnGiveXP(Player* player, uint32& amount, Unit* victim) override
    {
        if (!victim || !victim->ToCreature() || player->getLevel() < 100)
            return;

        if (sDemonInvasionMgr->GetCreatureRole(victim->GetEntry()) == DI_ROLE_NONE)
            return;

        amount = amount * DI_XP_ABOVE_100_PCT / 100;
    }
};

void AddSC_demon_invasion_prepatch()
{
    new OutdoorPvP_DemonInvasion_EK();
    new OutdoorPvP_DemonInvasion_Kal();
    new demon_invasion_commandscript();
    new npc_demon_invasion_vendor();
    new demon_invasion_xp();
    AddSC_demon_invasion_creatures();
}
