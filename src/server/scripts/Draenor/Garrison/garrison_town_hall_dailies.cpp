////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "ObjectAccessor.h"
#include "PhaseMgr.h"
#include "Player.h"
#include "Random.h"
#include "ScriptMgr.h"
#include "World.h"

/*
    Town Hall dailies of the level 3 garrisons (patch 6.1). Each day a player gets one category (dungeon, raid,
    bounty, relic, profession), kept as a hidden daily quest; only the NPC of that category is in his phase. Within
    a category the quest is the same for the whole realm: drawn here into world states read by the quest
    `conditions` (the raid quest once a week, the profession trader in a fixed rotation).
*/

namespace
{
    enum TownHallData
    {
        WS_TOWN_HALL_DAILY_RESET    = 20203,    // daily reset the draws below belong to
        WS_TOWN_HALL_DUNGEON        = 20204,    // 1..8
        WS_TOWN_HALL_BOUNTY         = 20205,    // 1..7
        WS_TOWN_HALL_RELIC          = 20206,    // 1..6
        WS_TOWN_HALL_RAID           = 20207,    // 1..12, weekly
        WS_TOWN_HALL_TRADER         = 20208,    // 1..5: leather, fur, dust, herbs, ore
        WS_TOWN_HALL_WEEKLY_RESET   = 20209,

        DUNGEON_COUNT               = 8,
        BOUNTY_COUNT                = 7,
        RELIC_COUNT                 = 6,
        RAID_COUNT                  = 12,
        TRADER_COUNT                = 5,
        CATEGORY_COUNT              = 5,

        ZONE_LUNARFALL              = 7078,
        ZONE_FROSTWALL              = 7004,
        MIN_LEVEL_TOWN_HALL         = 90        // professions from 90, the other categories ask 100 through their quests
    };

    // hidden daily quests: dungeon, raid, bounty, relic, profession
    uint32 const CategoryTracking[] = { 50260, 50261, 50262, 50263, 50264 };

    uint32 GetDay()
    {
        return uint32(sWorld->GetNextDailyQuestsResetTime() / DAY);
    }

    void AssignCategory(Player* player)
    {
        if (player->getLevel() < MIN_LEVEL_TOWN_HALL)
            return;

        for (uint32 questId : CategoryTracking)
            if (player->IsQuestDailyRewarded(questId))
                return;

        // the same answer all day long for a character, without anything to store but the result
        uint32 const seed = uint32(player->GetGUIDLow()) * 2654435761u + GetDay() * 40503u;
        player->SetDailyQuestStatus(CategoryTracking[(seed >> 7) % CATEGORY_COUNT]);

        PhaseUpdateData phaseUpdateData;
        phaseUpdateData.AddConditionType(CONDITION_QUESTREWARDED);
        player->GetPhaseMgr().NotifyConditionChanged(phaseUpdateData);
    }

    bool DrawIfNeeded()
    {
        bool drawn = false;

        uint32 const dailyReset = uint32(sWorld->GetNextDailyQuestsResetTime());
        if (sWorld->getWorldState(WS_TOWN_HALL_DAILY_RESET) != dailyReset)
        {
            sWorld->setWorldState(WS_TOWN_HALL_DUNGEON, urand(1, DUNGEON_COUNT));
            sWorld->setWorldState(WS_TOWN_HALL_BOUNTY, urand(1, BOUNTY_COUNT));
            sWorld->setWorldState(WS_TOWN_HALL_RELIC, urand(1, RELIC_COUNT));
            sWorld->setWorldState(WS_TOWN_HALL_TRADER, GetDay() % TRADER_COUNT + 1);
            sWorld->setWorldState(WS_TOWN_HALL_DAILY_RESET, dailyReset);
            drawn = true;
        }

        uint32 const weeklyReset = uint32(sWorld->GetNextWeeklyResetTime());
        if (sWorld->getWorldState(WS_TOWN_HALL_WEEKLY_RESET) != weeklyReset)
        {
            sWorld->setWorldState(WS_TOWN_HALL_RAID, urand(1, RAID_COUNT));
            sWorld->setWorldState(WS_TOWN_HALL_WEEKLY_RESET, weeklyReset);
            drawn = true;
        }

        return drawn;
    }
}

class world_garrison_town_hall_dailies : public WorldScript
{
public:
    world_garrison_town_hall_dailies() : WorldScript("world_garrison_town_hall_dailies") { }

    void OnStartup() override
    {
        DrawIfNeeded();
    }

    void OnUpdate(uint32 /*diff*/) override
    {
        if (!DrawIfNeeded())
            return;

        // after the daily reset has cleared the hidden quests of the players online
        ObjectAccessor::PostToAllPlayers([](Player* player)
        {
            AssignCategory(player);

            PhaseUpdateData phaseUpdateData;
            phaseUpdateData.AddConditionType(CONDITION_WORLD_STATE);
            player->GetPhaseMgr().NotifyConditionChanged(phaseUpdateData);
        }, 5 * IN_MILLISECONDS);
    }
};

class player_garrison_town_hall_dailies : public PlayerScript
{
public:
    player_garrison_town_hall_dailies() : PlayerScript("player_garrison_town_hall_dailies") { }

    void OnUpdateZone(Player* player, uint32 newZone, uint32 /*newArea*/) override
    {
        if (newZone == ZONE_LUNARFALL || newZone == ZONE_FROSTWALL)
            AssignCategory(player);
    }
};

void AddSC_garrison_town_hall_dailies()
{
    new world_garrison_town_hall_dailies();
    new player_garrison_town_hall_dailies();
}
