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

#include "GameObjectAI.h"

int GameObjectAI::Permissible(const GameObject* go)
{
    if (go->GetAIName() == "GameObjectAI")
        return PERMIT_BASE_SPECIAL;
    return PERMIT_BASE_NO;
}

bool GameObjectAI::GossipUseAsHello(Player* player)
{
    if (go->GetGoType() != GAMEOBJECT_TYPE_GOOBER && go->GetGOInfo()->GetLockId())
        return false;

    return GossipHello(player, true);
}

NullGameObjectAI::NullGameObjectAI(GameObject* g) : GameObjectAI(g) {}
