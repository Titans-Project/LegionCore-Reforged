////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Mage (Fire) artifact: Felo'melorn.
    The original code's Violet Hold instance script for the mage (map 1494) is kept here unregistered: the map also hosts
    the demon hunter's By Any Means.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

// mage violethold 1494
struct instance_violethold_mage : public InstanceScript
{
    instance_violethold_mage(InstanceMap* map) : InstanceScript(map) {}

    WorldLocation loc_res_pla;

    WorldLocation* GetClosestGraveYard(float x, float y, float z) override
    {
        loc_res_pla.WorldRelocate(1494, x, y, z);

        uint32 graveyardId = 5294;

        if (WorldSafeLocsEntry const* gy = sWorldSafeLocsStore.LookupEntry(graveyardId))
        {
            loc_res_pla.WorldRelocate(gy->MapID, gy->Loc.X, gy->Loc.Y, gy->Loc.Z);
        }
        return &loc_res_pla;
    }
};

void AddSC_Felomelorn()
{
    //RegisterInstanceScript(instance_violethold_mage, 1494);
}
