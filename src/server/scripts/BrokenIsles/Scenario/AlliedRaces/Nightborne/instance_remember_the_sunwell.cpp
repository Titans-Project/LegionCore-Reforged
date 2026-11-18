////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Nightborne allied race: the Remember the Sunwell scenario (quest 49354, map 1840)
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

//nightborne open quest scenario 1840
struct instance_nightborne_sunwell : public InstanceScript
{
    instance_nightborne_sunwell(InstanceMap* map) : InstanceScript(map) {}

    WorldLocation loc_res_pla;

    WorldLocation* GetClosestGraveYard(float x, float y, float z) override
    {
        loc_res_pla.WorldRelocate(1840, x, y, z);

        uint32 graveyardId = 6303;

        if (WorldSafeLocsEntry const* gy = sWorldSafeLocsStore.LookupEntry(graveyardId))
        {
            loc_res_pla.WorldRelocate(gy->MapID, gy->Loc.X, gy->Loc.Y, gy->Loc.Z);
        }
        return &loc_res_pla;
    }
};

void AddSC_instance_remember_the_sunwell()
{
    RegisterInstanceScript(instance_nightborne_sunwell, 1840);
}
