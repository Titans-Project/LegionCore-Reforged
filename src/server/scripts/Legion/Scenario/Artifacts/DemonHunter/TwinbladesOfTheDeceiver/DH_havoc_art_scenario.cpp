////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Demon Hunter (Havoc) artifact: By Any Means (Violet Hold, map 1494) and The Hunt (Felsoul Hold, map 1498)
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

// By Any Means: the demon hunter's copy of the Violet Hold (map 1494) is his own; leaving it unbinds him, so that his next
// visit starts the scene again instead of finding it played or left halfway
class player_violet_hold_quest_copy : public PlayerScript
{
public:
    player_violet_hold_quest_copy() : PlayerScript("player_violet_hold_quest_copy") { }

    static constexpr uint32 MAP_VIOLET_HOLD_QUEST_COPY = 1494;

    void OnMapChanged(Player* player) override
    {
        if (player->getClass() != CLASS_DEMON_HUNTER || player->GetMapId() == MAP_VIOLET_HOLD_QUEST_COPY)
            return;

        for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
        {
            Player::BoundInstancesMap const binds = player->GetBoundInstancesCopy(Difficulty(i));
            if (binds.find(MAP_VIOLET_HOLD_QUEST_COPY) != binds.end())
                player->UnbindInstance(MAP_VIOLET_HOLD_QUEST_COPY, Difficulty(i));
        }
    }
};

void AddSC_DH_havoc_art_scenario()
{
    new player_violet_hold_quest_copy();
}
