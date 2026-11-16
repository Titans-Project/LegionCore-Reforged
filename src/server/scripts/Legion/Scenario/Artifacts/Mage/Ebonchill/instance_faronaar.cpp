////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Mage (Frost) artifact: the Mage Hunter scenario, Faronaar (map 1616)
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

// frostmage
struct instance_faronaar_mage : public InstanceScript
{
    instance_faronaar_mage(InstanceMap* map) : InstanceScript(map) {}

    WorldLocation loc_res_pla;

    WorldLocation* GetClosestGraveYard(float x, float y, float z) override
    {
        loc_res_pla.WorldRelocate(1616, x, y, z);

        uint32 graveyardId = 5511;

        if (WorldSafeLocsEntry const* gy = sWorldSafeLocsStore.LookupEntry(graveyardId))
        {
            loc_res_pla.WorldRelocate(gy->MapID, gy->Loc.X, gy->Loc.Y, gy->Loc.Z);
        }
        return &loc_res_pla;
    }
};

void AddSC_instance_faronaar()
{
    RegisterInstanceScript(instance_faronaar_mage, 1616);
}
