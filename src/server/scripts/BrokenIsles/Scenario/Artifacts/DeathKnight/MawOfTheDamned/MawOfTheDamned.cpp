////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Death Knight (Blood) artifact scene: the pickup scene equips the looted weapon.
    The rest of the acquisition is not scripted in C++ yet: to do.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "../DeathKnightArtifactScene.h"

namespace
{
DeathKnightArtifactScene::Pickup const Pickup = { 1168, 1532, 1489, 940, 9, 40740, 128402 };

class scene_dk_blood_equipment : public SceneTriggerScript
{
public:
    scene_dk_blood_equipment() : SceneTriggerScript("scene_dk_blood_equipment") { }

    bool OnTrigger(Player* player, SpellScene const* scene, std::string trigger) override
    {
        return DeathKnightArtifactScene::Denied(player, scene, trigger, Pickup);
    }
};
}

void AddSC_MawOfTheDamned()
{
    new scene_dk_blood_equipment();
}
