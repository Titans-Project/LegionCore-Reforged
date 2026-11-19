////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#pragma once

#include "Player.h"
#include "SmartAI.h"

// The death sequences of the mage artifact scenarios give their scenario credit, conversation and energy to the
// invoker, which only a player can receive. A killing blow from a pet or a guardian (water elemental, mirror
// images) names that pet as the invoker, so the owner takes its place.
struct npc_mage_artifact_kill_credit : public SmartAI
{
    explicit npc_mage_artifact_kill_credit(Creature* creature) : SmartAI(creature) { }

    void JustDied(Unit* killer) override
    {
        Player* owner = killer ? killer->GetCharmerOrOwnerPlayerOrPlayerItself() : nullptr;
        SmartAI::JustDied(owner ? owner : killer);
    }
};
