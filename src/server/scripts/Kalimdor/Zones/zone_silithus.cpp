////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Silithus
SD%Complete: 100
SDComment: Quest support: 7785, 8304.
SDCategory: Silithus
EndScriptData */

/* ContentData
npc_highlord_demitrian
npcs_rutgar_and_frankal
npc_zidormi_silithus
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Group.h"
#include "Player.h"
#include "Creature.h"
#include "GossipDef.h"
#include "World.h"
#include "PhaseMgr.h"
#include "ConditionMgr.h"
#include "Map.h"
#include "GameEventMgr.h"
/*###
## npc_highlord_demitrian
###*/

#define GOSSIP_DEMITRIAN1 "What do you know of it?"
#define GOSSIP_DEMITRIAN2 "I am listening, Demitrian."
#define GOSSIP_DEMITRIAN3 "Continue, please."
#define GOSSIP_DEMITRIAN4 "A battle?"
#define GOSSIP_DEMITRIAN5 "<Nod>"
#define GOSSIP_DEMITRIAN6 "Caught unaware? How?"
#define GOSSIP_DEMITRIAN7 "So what did Ragnaros do next?"

class npc_highlord_demitrian : public CreatureScript
{
public:
    npc_highlord_demitrian() : CreatureScript("npc_highlord_demitrian") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
        case GOSSIP_ACTION_INFO_DEF:
            player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_DEMITRIAN2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(6842, creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_DEMITRIAN3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            player->SEND_GOSSIP_MENU(6843, creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_DEMITRIAN4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            player->SEND_GOSSIP_MENU(6844, creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_DEMITRIAN5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            player->SEND_GOSSIP_MENU(6867, creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_DEMITRIAN6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            player->SEND_GOSSIP_MENU(6868, creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_DEMITRIAN7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
            player->SEND_GOSSIP_MENU(6869, creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            player->SEND_GOSSIP_MENU(6870, creature->GetGUID());

            ItemPosCountVec dest;
            uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 19016, 1);
            if (msg == EQUIP_ERR_OK)
                player->StoreNewItem(dest, 19016, true);
            break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestStatus(7785) == QUEST_STATUS_NONE &&
            (player->HasItemCount(18563, 1, false) || player->HasItemCount(18564, 1, false)))
            player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_DEMITRIAN1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        player->SEND_GOSSIP_MENU(6812, creature->GetGUID());
            return true;
    }

};

/*###
## npcs_rutgar_and_frankal
###*/

//gossip item text best guess
#define GOSSIP_ITEM1 "I seek information about Natalia"

#define GOSSIP_ITEM2 "That sounds dangerous!"
#define GOSSIP_ITEM3 "What did you do?"
#define GOSSIP_ITEM4 "Who?"
#define GOSSIP_ITEM5 "Women do that. What did she demand?"
#define GOSSIP_ITEM6 "What do you mean?"
#define GOSSIP_ITEM7 "What happened next?"

#define GOSSIP_ITEM11 "Yes, please continue"
#define GOSSIP_ITEM12 "What language?"
#define GOSSIP_ITEM13 "The Priestess attacked you?!"
#define GOSSIP_ITEM14 "I should ask the monkey about this"
#define GOSSIP_ITEM15 "Then what..."

enum RutgarAndFrankal //trigger creatures to kill
{
    TRIGGER_FRANKAL     = 15221,
    TRIGGER_RUTGAR      = 15222
};

class npcs_rutgar_and_frankal : public CreatureScript
{
public:
    npcs_rutgar_and_frankal() : CreatureScript("npcs_rutgar_and_frankal") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF:
                player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_ITEM2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->SEND_GOSSIP_MENU(7755, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 1:
                player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_ITEM3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                player->SEND_GOSSIP_MENU(7756, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
                player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_ITEM4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                player->SEND_GOSSIP_MENU(7757, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 3:
                player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_ITEM5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                player->SEND_GOSSIP_MENU(7758, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 4:
                player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_ITEM6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                player->SEND_GOSSIP_MENU(7759, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 5:
                player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_ITEM7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                player->SEND_GOSSIP_MENU(7760, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 6:
                player->SEND_GOSSIP_MENU(7761, creature->GetGUID());
                                                                //'kill' our trigger to update quest status
                player->KilledMonsterCredit(TRIGGER_RUTGAR, ObjectGuid::Empty);
                break;

            case GOSSIP_ACTION_INFO_DEF + 9:
                player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_ITEM11, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
                player->SEND_GOSSIP_MENU(7762, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 10:
                player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_ITEM12, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
                player->SEND_GOSSIP_MENU(7763, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 11:
                player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_ITEM13, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
                player->SEND_GOSSIP_MENU(7764, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 12:
                player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_ITEM14, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
                player->SEND_GOSSIP_MENU(7765, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 13:
                player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_ITEM15, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
                player->SEND_GOSSIP_MENU(7766, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 14:
                player->SEND_GOSSIP_MENU(7767, creature->GetGUID());
                                                                //'kill' our trigger to update quest status
                player->KilledMonsterCredit(TRIGGER_FRANKAL, ObjectGuid::Empty);
                break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestStatus(8304) == QUEST_STATUS_INCOMPLETE &&
            creature->GetEntry() == 15170 &&
            !player->GetReqKillOrCastCurrentCount(8304, TRIGGER_RUTGAR))
            player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        if (player->GetQuestStatus(8304) == QUEST_STATUS_INCOMPLETE &&
            creature->GetEntry() == 15171 &&
            player->GetReqKillOrCastCurrentCount(8304, TRIGGER_RUTGAR))
            player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+9);

        player->SEND_GOSSIP_MENU(7754, creature->GetGUID());

        return true;
    }

};

/*
 * Silithus: The Wound - time travel (custom content)
 *
 * Zidormi, 128607, has always stood in Silithus but had neither menu nor script. The Legion
 * terrain is there too: phase_definitions swaps zone 1377 to map 1817, and since 2026_09_08_03
 * it fills VisibleMapID, without which the ground stayed the 2006 one.
 *
 * Aura 255152 means "Silithus before the world was wounded": wearing it puts you in the old zone,
 * not wearing it is the present, with the sword planted.
 *
 * It is permanent and saved, and nothing removes it on its own: a player keeps their version
 * when leaving the zone, changing map and logging back in.
 *
 * Custom.SilithusWound.Default only says whether the travel is open. At 1 Zidormi offers the one
 * missing option - the past to whoever is in the present, the way back to whoever is in the past.
 * At 0 she has nothing to say and the zone stays in the present, the sword planted as patch 7.3
 * left it.
 */

enum SilithusWound
{
    NPC_ZIDORMI_SILITHUS   = 128607,
    ZONE_SILITHUS          = 1377,
    SPELL_TIME_TRAVELLING  = 255152,   // wearing it means being before the Wound; without it, the sword
    GOSSIP_TEXT_ZIDORMI    = 14065,    // Zidormi's text, the one the Northrend Zidormi already uses
    EVENT_CALL_OF_THE_SCARAB = 78,     // the holiday takes place before the Wound and sends everyone there once

    ACTION_TO_THE_PAST     = GOSSIP_ACTION_INFO_DEF + 1,   // adds the aura: travel to before the Wound
    ACTION_TO_THE_PRESENT  = GOSSIP_ACTION_INFO_DEF + 2    // removes it: come back, the sword reappears
};

#define GOSSIP_SHOW_PAST    "Montre-moi Silithus avant la Plaie."
#define GOSSIP_SHOW_PRESENT "Ramene-moi au present."

class npc_zidormi_silithus : public CreatureScript
{
public:
    npc_zidormi_silithus() : CreatureScript("npc_zidormi_silithus") {}

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        // Travel closed: she offers nothing.
        if (!sWorld->getBoolConfig(CONFIG_SILITHUS_WOUND_DEFAULT))
            return false;

        // A single option, the one leading away from where the player stands.
        if (player->HasAura(SPELL_TIME_TRAVELLING))
            player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SHOW_PRESENT, GOSSIP_SENDER_MAIN, ACTION_TO_THE_PRESENT);
        else
            player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SHOW_PAST, GOSSIP_SENDER_MAIN, ACTION_TO_THE_PAST);

        player->SEND_GOSSIP_MENU(GOSSIP_TEXT_ZIDORMI, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();

        if (action == ACTION_TO_THE_PAST)
            player->AddAura(SPELL_TIME_TRAVELLING, player);
        else if (action == ACTION_TO_THE_PRESENT)
            player->RemoveAurasDueToSpell(SPELL_TIME_TRAVELLING);
        else
        {
            player->PlayerTalkClass->SendCloseGossip();
            return true;
        }

        // The core never notifies the phase manager of an aura change: no aura code path mentions
        // CONDITION_AURA. Without this call the phase is only recomputed on the next zone change -
        // so the sword stayed on click, then switched when leaving Silithus and switched back on
        // return.
        PhaseUpdateData phaseUpdateData;
        phaseUpdateData.AddConditionType(CONDITION_AURA);
        player->GetPhaseMgr().NotifyConditionChanged(phaseUpdateData);

        player->PlayerTalkClass->SendCloseGossip();

        // The recompute above is enough for objects and creatures, not for the ground: the client
        // only reads a VisibleMapID when the map loads, so the terrain stayed the old one until
        // the player left the zone and came back. A teleport on the spot makes it reload, which is
        // also what the game does - Zidormi's switch goes through a short loading screen.
        //
        // TELE_TO_ZONE_MAP is required: without it SafeTeleport sees the same map and takes the
        // near path, which moves the player without reloading anything - so no loading screen and
        // no new ground. This flag forces the far path on the spot, and it also spares the auras
        // carrying AURA_INTERRUPT_FLAG_CHANGE_MAP, which matters here since the time marker must
        // survive the travel it triggers.
        player->TeleportTo(player->GetMapId(), player->GetPositionX(), player->GetPositionY(),
                           player->GetPositionZ(), player->GetOrientation(), TELE_TO_ZONE_MAP);
        return true;
    }
};

class player_silithus_wound_gate : public PlayerScript
{
public:
    player_silithus_wound_gate() : PlayerScript("player_silithus_wound_gate") {}

    void OnUpdateZone(Player* player, uint32 newZone, uint32 /*newArea*/) override
    {
        if (!player)
            return;

        if (newZone != ZONE_SILITHUS)
            return;

        // Closing the travel must not leave anyone stranded in the past: a marker taken before the
        // option was turned off is removed on entering the zone. The present is then the only
        // possible state, with the sword planted.
        // Call of the Scarab sends everyone to the past whatever the option: the holiday takes place there.
        if (!sWorld->getBoolConfig(CONFIG_SILITHUS_WOUND_DEFAULT) && player->HasAura(SPELL_TIME_TRAVELLING)
            && !sGameEventMgr->IsActiveEvent(EVENT_CALL_OF_THE_SCARAB))
        {
            player->RemoveAurasDueToSpell(SPELL_TIME_TRAVELLING);

            PhaseUpdateData phaseUpdateData;
            phaseUpdateData.AddConditionType(CONDITION_AURA);
            player->GetPhaseMgr().NotifyConditionChanged(phaseUpdateData);
        }
    }

    void OnLogin(Player* player) override
    {
        if (!player)
            return;

        // The grid of the sword effect is loaded by force. Otherwise the object does not exist
        // until someone has walked by: it only enters the world with its grid, and AddMaxVisible
        // ignores what is not there. Once created it marks itself active and its grid no longer
        // unloads - it only had to be loaded once.
        if (player->GetMapId() == 1)
            if (Map* map = player->GetMap())
                map->LoadGrid(-7128.0f, 930.0f);
    }


    // Nothing here reloads the terrain on entering the zone any more. That reload existed because
    // leaving Silithus dropped the terrain swap, phase_definitions being indexed by zone; since
    // 2026_09_08_11 the definition is given to every Kalimdor zone, so the swap no longer drops
    // and crossing a border changes nothing. Only Zidormi decides, and only she triggers a loading
    // screen.
};

void AddSC_silithus()
{
    new npc_highlord_demitrian();
    new npcs_rutgar_and_frankal();
    new npc_zidormi_silithus();
    new player_silithus_wound_gate();
}
