////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "Containers.h"
#include "GossipDef.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "QuestData.h"
#include "Random.h"
#include "ScriptMgr.h"
#include "World.h"
#include <algorithm>

/*
    Apexis assaults of the Warlords garrisons. Each day the War Planning Map offers two assaults, the same for the
    whole realm, of which a player takes one. The assault quest (or a Scouting Missive one) opens the daily task of
    its area; when that task is done it credits the assault quest, handed in to Scout Valdez / Shadow Hunter Ukambe.
    Which task may start is told by the `conditions` of the tasks (Player::UpdateAreaQuestTasks).
*/

namespace
{
    enum ApexisAssaultData
    {
        // world states (characters `worldstates`): the daily reset the offers were drawn for, then the two offers,
        // kept as the Alliance assault quest of their site so that a restart keeps the day's offers
        WS_APEXIS_OFFERS_RESET      = 20200,
        WS_APEXIS_OFFER_FIRST       = 20201,
        WS_APEXIS_OFFER_SECOND      = 20202,

        GOSSIP_SENDER_WAR_PLANNING  = 1,
        NPC_TEXT_NO_ASSAULT_LEFT    = 926001    // "The scouts have no other assault to report today."
    };

    struct AssaultSite
    {
        uint32 PlanAlliance;        // offered by the War Planning Map
        uint32 PlanHorde;
        uint32 MissiveAlliance;     // started by a Scouting Missive
        uint32 MissiveHorde;
        uint32 TaskAlliance;        // daily task of the assault area
        uint32 TaskHorde;
        uint32 Credit;              // objective of the assault and missive quests
        bool Group;
    };

    AssaultSite const Sites[] =
    {
        { 36648, 36669, 38189, 38176, 35637, 35641,  91953, false },    // Stonefury Cliffs
        { 36675, 36697, 38190, 38177, 36826, 36827,  91952, false },    // Magnarok
        { 36674, 36696, 38191, 38178, 36822, 36823,  91955, false },    // Iron Siegeworks
        { 36676, 36695, 38192, 38179, 36815, 36814,  91966, false },    // Everbloom Wilds
        { 36677, 36694, 38193, 38180, 36816, 36817,  91965, false },    // Broken Precipice
        { 37523, 37524, 38194, 38202, 38204, 38205,  91963, false },    // Gorian Proving Grounds
        { 36678, 36693, 38195, 38181, 36818, 36819,  91964, false },    // Mok'gol Watchpost
        { 36679, 36692, 38196, 38182, 35974, 35977,  91962, false },    // Darktide Roost
        { 36680, 36691, 38197, 38183, 36348, 36347,  91961, false },    // Socrethar's Rise
        { 36681, 36690, 38198, 38184, 36860, 36860,  91960, false },    // Lost Veil Anzu
        { 36682, 36689, 38199, 38185, 36490, 36491,  91958, false },    // Pillars of Fate
        { 36683, 36688, 38200, 38186, 36069, 36070,  91957, false },    // Skettis
        { 36649, 36667, 38201, 38187, 36808, 36338,  91959, false },    // Shattrath Harbor
        { 36685, 36699, 40944, 40943, 36809, 36807, 103087, true  },    // Heart of Shattrath
        { 36686, 36701, 40942, 40941, 36383, 36489, 103086, true  },    // the Pit
    };

    AssaultSite const* FindSiteByPlan(uint32 alliancePlanQuest)
    {
        for (AssaultSite const& site : Sites)
            if (site.PlanAlliance == alliancePlanQuest)
                return &site;
        return nullptr;
    }

    AssaultSite const* FindSiteByOpeningQuest(uint32 questId)
    {
        for (AssaultSite const& site : Sites)
            if (questId == site.PlanAlliance || questId == site.PlanHorde || questId == site.MissiveAlliance || questId == site.MissiveHorde)
                return &site;
        return nullptr;
    }

    AssaultSite const* FindSiteByTask(uint32 questId)
    {
        for (AssaultSite const& site : Sites)
            if (questId == site.TaskAlliance || questId == site.TaskHorde)
                return &site;
        return nullptr;
    }

    // one solo assault and, most days, one group assault; drawn once per daily reset
    bool DrawOffersIfNeeded()
    {
        uint32 const reset = uint32(sWorld->GetNextDailyQuestsResetTime());
        if (sWorld->getWorldState(WS_APEXIS_OFFERS_RESET) == reset && FindSiteByPlan(sWorld->getWorldState(WS_APEXIS_OFFER_FIRST))
            && FindSiteByPlan(sWorld->getWorldState(WS_APEXIS_OFFER_SECOND)))
            return false;

        std::vector<AssaultSite const*> solo, group;
        for (AssaultSite const& site : Sites)
            (site.Group ? group : solo).push_back(&site);

        AssaultSite const* first = Trinity::Containers::SelectRandomContainerElement(solo);
        AssaultSite const* second = nullptr;
        if (roll_chance_i(67))
            second = Trinity::Containers::SelectRandomContainerElement(group);
        else
        {
            solo.erase(std::remove(solo.begin(), solo.end(), first), solo.end());
            second = Trinity::Containers::SelectRandomContainerElement(solo);
        }

        sWorld->setWorldState(WS_APEXIS_OFFER_FIRST, first->PlanAlliance);
        sWorld->setWorldState(WS_APEXIS_OFFER_SECOND, second->PlanAlliance);
        sWorld->setWorldState(WS_APEXIS_OFFERS_RESET, reset);
        return true;
    }

    std::vector<AssaultSite const*> GetOffers()
    {
        std::vector<AssaultSite const*> offers;
        for (uint32 ws : { WS_APEXIS_OFFER_FIRST, WS_APEXIS_OFFER_SECOND })
            if (AssaultSite const* site = FindSiteByPlan(sWorld->getWorldState(ws)))
                offers.push_back(site);
        return offers;
    }

    uint32 GetPlanQuest(AssaultSite const* site, Player const* player)
    {
        return player->GetTeam() == ALLIANCE ? site->PlanAlliance : site->PlanHorde;
    }
}

class world_garrison_apexis_assaults : public WorldScript
{
public:
    world_garrison_apexis_assaults() : WorldScript("world_garrison_apexis_assaults") { }

    void OnStartup() override
    {
        DrawOffersIfNeeded();
    }

    void OnUpdate(uint32 /*diff*/) override
    {
        if (!DrawOffersIfNeeded())
            return;

        // an assault chosen yesterday and not yet accepted is no longer on offer
        ObjectAccessor::PostToAllPlayers([](Player* player)
        {
            if (FindSiteByOpeningQuest(player->GetPopupQuestId()))
                player->SetPopupQuestId(0);
        });
    }
};

// 236165 (Lunarfall), 236206 (Frostwall): the map only lists the day's offers; a goober cannot hand out quests,
// so the chosen one is offered as a popup from the player, as quests started by area triggers are
class go_garrison_war_planning_map : public GameObjectScript
{
public:
    go_garrison_war_planning_map() : GameObjectScript("go_garrison_war_planning_map") { }

    bool OnGossipHello(Player* player, GameObject* go) override
    {
        player->PlayerTalkClass->ClearMenus();
        if (FindSiteByOpeningQuest(player->GetPopupQuestId()))
            player->SetPopupQuestId(0);

        for (AssaultSite const* site : GetOffers())
        {
            Quest const* quest = sQuestDataStore->GetQuestTemplate(GetPlanQuest(site, player));
            if (!quest || !player->CanTakeQuest(quest, false))
                continue;

            std::string title = quest->LogTitle;
            if (QuestTemplateLocale const* locale = sQuestDataStore->GetQuestLocale(quest->GetQuestId()))
                ObjectMgr::GetLocaleString(locale->LogTitle, player->GetSession()->GetSessionDbLocaleIndex(), title);

            player->PlayerTalkClass->GetGossipMenu().AddMenuItem(-1, GossipOptionNpc::None, title, GOSSIP_SENDER_WAR_PLANNING, quest->GetQuestId(), "", 0);
        }

        bool const empty = player->PlayerTalkClass->GetGossipMenu().Empty();
        player->PlayerTalkClass->SendGossipMenu(empty ? uint32(NPC_TEXT_NO_ASSAULT_LEFT) : uint32(DEFAULT_GOSSIP_MESSAGE), go->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, GameObject* /*go*/, uint32 sender, uint32 action) override
    {
        if (sender != GOSSIP_SENDER_WAR_PLANNING)
            return false;

        player->PlayerTalkClass->SendCloseGossip();

        std::vector<AssaultSite const*> const offers = GetOffers();
        bool const offered = std::any_of(offers.begin(), offers.end(), [&](AssaultSite const* site) { return GetPlanQuest(site, player) == action; });
        Quest const* quest = sQuestDataStore->GetQuestTemplate(action);
        if (!offered || !quest || !player->CanTakeQuest(quest, true))
            return true;

        player->SetPopupQuestId(quest->GetQuestId());
        player->PlayerTalkClass->SendQuestGiverQuestDetails(quest, player->GetGUID(), true, false);
        return true;
    }
};

class player_garrison_apexis_assaults : public PlayerScript
{
public:
    player_garrison_apexis_assaults() : PlayerScript("player_garrison_apexis_assaults") { }

    // the assault area may already be around the player
    void OnQuestAccept(Player* player, Quest const* quest) override
    {
        if (FindSiteByOpeningQuest(quest->GetQuestId()))
        {
            player->UpdateAreaQuestTasks(player->GetZoneId(), player->GetZoneId());
            player->UpdateAreaQuestTasks(player->GetAreaId(), player->GetAreaId());
        }
    }

    void OnQuestReward(Player* player, Quest const* quest) override
    {
        if (AssaultSite const* site = FindSiteByTask(quest->GetQuestId()))
            player->KilledMonsterCredit(site->Credit);
    }
};

void AddSC_garrison_apexis_assaults()
{
    new world_garrison_apexis_assaults();
    new go_garrison_war_planning_map();
    new player_garrison_apexis_assaults();
}
