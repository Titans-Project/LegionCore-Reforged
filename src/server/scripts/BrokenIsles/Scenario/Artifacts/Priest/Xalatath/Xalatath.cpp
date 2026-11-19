////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Priest (Shadow) artifact: Xal'atath, Blade of the Black Empire.
    The chest, its loot and the scene are data (smart scripts, sql); only the equip after the scene needs code.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Scenario.h"
#include "ScenarioMgr.h"
#include "SpellMgr.h"

enum Xalatath
{
    QUEST_BLADE_IN_TWILIGHT     = 40710,
    ITEM_XALATH                 = 128827,
    SCENARIO_XALATATH           = 991,
    SCENE_XALATATH_PICKUP       = 1187,
    SPELL_XALATATH_EQUIP        = 201778
};

// The scene shows the blade but does not equip it: the real item, with its paired child, is force-equipped when the scene ends.
class scene_xalatath_pickup : public SceneTriggerScript
{
public:
    scene_xalatath_pickup() : SceneTriggerScript("scene_xalatath_pickup") { }

    bool OnTrigger(Player* player, SpellScene const* scene, std::string trigger) override
    {
        if (!scene || scene->MiscValue != SCENE_XALATATH_PICKUP || trigger != "complete")
            return false;

        if (!player || !player->IsAlive() || player->getClass() != CLASS_PRIEST || player->GetMapId() != 1539 ||
            player->GetQuestRewardStatus(QUEST_BLADE_IN_TWILIGHT) || !player->GetItemByEntry(ITEM_XALATH))
            return false;

        QuestStatus status = player->GetQuestStatus(QUEST_BLADE_IN_TWILIGHT);
        if (status != QUEST_STATUS_INCOMPLETE && status != QUEST_STATUS_COMPLETE)
            return false;

        Scenario* scenario = sScenarioMgr->GetScenario(player->GetInstanceId());
        if (!scenario || scenario->GetScenarioId() != SCENARIO_XALATATH)
            return false;

        player->CastSpell(player, SPELL_XALATATH_EQUIP, true);
        return false;
    }
};

void AddSC_Xalatath()
{
    new scene_xalatath_pickup();
}
