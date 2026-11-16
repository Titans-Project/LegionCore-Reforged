////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Rogue (Outlaw) artifact: the Dread Admiral scenario, Azsuna (map 1545)
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

//rogue dreadblades 1545
struct instance_dreadblades : public InstanceScript
{
    instance_dreadblades(InstanceMap* map) : InstanceScript(map) {}
    
    WorldLocation loc_res_pla;

    WorldLocation* GetClosestGraveYard(float x, float y, float z) override
    {
        loc_res_pla.WorldRelocate(1545, x, y, z);

        uint32 graveyardId = 5299;

        switch (getScenarionStep())
        {
        case 0:
        case 1:
        case 2:
            graveyardId = 5299;
            break;
        case 3:
        case 4:
            graveyardId = 5360;
            break;
        }

        if (WorldSafeLocsEntry const* gy = sWorldSafeLocsStore.LookupEntry(graveyardId))
        {
            loc_res_pla.WorldRelocate(gy->MapID, gy->Loc.X, gy->Loc.Y, gy->Loc.Z);
        }
        return &loc_res_pla;
    }
};

void AddSC_instance_dreadblades()
{
    RegisterInstanceScript(instance_dreadblades, 1545);
}
