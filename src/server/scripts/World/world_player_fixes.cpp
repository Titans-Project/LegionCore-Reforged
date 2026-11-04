////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "Spell.h"
#include "SpellInfo.h"

class npc_monk_art_xuen_1 : public CreatureScript
{
public:
    npc_monk_art_xuen_1() : CreatureScript("npc_monk_art_xuen_1") {}

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)  override
    {
        player->PlayerTalkClass->ClearMenus();
        player->UpdateAchievementCriteria(CRITERIA_TYPE_SCRIPT_EVENT_2, 47723);
        player->CLOSE_GOSSIP_MENU();
        return true;
    }
};

class player_monk_quest_43062 : public PlayerScript
{
public:
    player_monk_quest_43062() : PlayerScript("player_monk_quest_43062") {}

    void OnSpellCast(Player* player, Spell* spell, bool /*skipCheck*/)
    {
        switch (spell->GetSpellInfo()->Id)
        {
        case 115008:
        case 109132:
        {
            if (player->GetQuestStatus(43062) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(109597);
            }
            break;
        }
        case 117952:
        {
            if (player->GetQuestStatus(43062) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(109599);
            }
            break;
        }
        case 100784:
        case 205523:
        {
            if (player->GetQuestStatus(43062) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(109598);
            }
            break;
        }
        }
    }
};

class player_remove_flight_if_not_mounted : public PlayerScript
{
public:
    player_remove_flight_if_not_mounted() : PlayerScript("player_remove_flight_if_not_mounted") {}

    uint32 timer = 2000;

    void OnLogin(Player* player) // fix some bugs with flying without mount
    {
        if (!player->IsMounted() && player->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED))
            player->RemoveAurasByType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED);
    }
};

void AddSC_world_player_fixes()
{
    new npc_monk_art_xuen_1();
    new player_monk_quest_43062();
    new player_remove_flight_if_not_mounted();
}
