////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Death Knight (Frost) artifact scene: the pickup scene equips the looted weapon.
    The rest of the acquisition is not scripted in C++ yet: to do.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "../DeathKnightArtifactScene.h"

namespace
{
DeathKnightArtifactScene::Pickup const Pickup = { 1071, 1466, 1480, 901, 6, 38990, 128292 };

class scene_dk_frost_equipment : public SceneTriggerScript
{
public:
    scene_dk_frost_equipment() : SceneTriggerScript("scene_dk_frost_equipment") { }

    bool OnTrigger(Player* player, SpellScene const* scene, std::string trigger) override
    {
        return DeathKnightArtifactScene::Denied(player, scene, trigger, Pickup);
    }
};
}

void AddSC_BladesOfTheFallenPrince()
{
    new scene_dk_frost_equipment();
}
