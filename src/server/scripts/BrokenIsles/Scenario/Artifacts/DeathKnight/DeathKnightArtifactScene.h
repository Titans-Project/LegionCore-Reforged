////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#ifndef DEATH_KNIGHT_ARTIFACT_SCENE_H
#define DEATH_KNIGHT_ARTIFACT_SCENE_H

#include "Player.h"
#include "Scenario.h"
#include "ScenarioMgr.h"
#include "SpellMgr.h"

namespace DeathKnightArtifactScene
{
struct Pickup
{
    uint32 scene, package, map, scenario, firstStep, quest, item;
};

// The completed pickup scene equips the artifact the player already looted. It is
// admitted only inside the acquisition scenario, so a late completion after a map
// change or a replayed packet never reaches the equip spell.
inline bool Denied(Player* player, SpellScene const* scene, std::string const& trigger, Pickup const& pickup)
{
    if (!scene || uint32(scene->MiscValue) != pickup.scene || trigger != "complete")
        return false;

    if (uint32(scene->SceneScriptPackageID) != pickup.package || !player || !player->IsInWorld() ||
        !player->CanContact() || !player->IsAlive() || player->IsBeingTeleported() ||
        player->getClass() != CLASS_DEATH_KNIGHT || player->GetMapId() != pickup.map ||
        !player->HasSceneStatus(pickup.scene, SCENE_LAUNCH) || !player->HasItemCount(pickup.item, 1, false))
        return true;

    QuestStatus const status = player->GetQuestStatus(pickup.quest);
    if (status != QUEST_STATUS_INCOMPLETE && status != QUEST_STATUS_COMPLETE)
        return true;

    Scenario* scenario = sScenarioMgr->GetScenario(player->GetInstanceId());
    return !scenario || scenario->GetScenarioId() != pickup.scenario ||
        scenario->GetCurrentStep() < pickup.firstStep || scenario->GetCurrentStep() > pickup.firstStep + 1;
}
}

#endif
