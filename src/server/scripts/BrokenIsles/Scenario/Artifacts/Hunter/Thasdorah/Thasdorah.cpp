////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Hunter (Marksmanship) artifact: Thas'dorah, Legacy of the Windrunners.
    The scenario itself (Vereesa, Orestes, Torvis, the Herald, Qormaladon, the bow chest) runs on SmartAI and
    the data in sql/updates/world/2026_11_19_21_thasdorah.sql. The only script needed is the pickup scene:
    when it completes, spell_scene_event equips the bow the player has just looted.
*/

#include "ScriptMgr.h"
#include "Player.h"
#include "Scenario.h"
#include "ScenarioMgr.h"
#include "SpellMgr.h"

namespace
{
enum Thasdorah
{
    MAP_ACQUISITION         = 1489,
    SCENARIO_THASDORAH      = 972,
    STEP_BOW_OF_LEGEND      = 7,        // last step, "A Bow of Legend"; the scenario reads one past it once complete
    SCENE_THASDORAH_PICKUP  = 1169,
    ITEM_THASDORAH          = 128826,
    QUEST_LEGACY            = 40419
};

class scene_marksmanship_equipment : public SceneTriggerScript
{
public:
    scene_marksmanship_equipment() : SceneTriggerScript("scene_marksmanship_equipment") { }

    // Returning true drops the scene event: the bow is equipped only for the hunter who looted it inside this scenario,
    // so a scene completion that arrives late (map change, replayed packet) never reaches the equip spell.
    bool OnTrigger(Player* player, SpellScene const* scene, std::string trigger) override
    {
        if (!scene || scene->MiscValue != SCENE_THASDORAH_PICKUP || trigger != "complete")
            return false;

        if (!player || !player->IsInWorld() || !player->IsAlive() || player->IsBeingTeleported() ||
            player->getClass() != CLASS_HUNTER || player->GetMapId() != MAP_ACQUISITION ||
            !player->HasSceneStatus(SCENE_THASDORAH_PICKUP, SCENE_LAUNCH) || !player->HasItemCount(ITEM_THASDORAH, 1, false))
            return true;

        QuestStatus const status = player->GetQuestStatus(QUEST_LEGACY);
        if (status != QUEST_STATUS_INCOMPLETE && status != QUEST_STATUS_COMPLETE)
            return true;

        Scenario* scenario = sScenarioMgr->GetScenario(player->GetInstanceId());
        return !scenario || scenario->GetScenarioId() != SCENARIO_THASDORAH || scenario->GetCurrentStep() < STEP_BOW_OF_LEGEND;
    }
};
}

void AddSC_Thasdorah()
{
    new scene_marksmanship_equipment();
}
