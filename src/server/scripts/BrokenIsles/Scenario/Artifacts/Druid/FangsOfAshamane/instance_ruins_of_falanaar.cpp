////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Druid (Feral) artifact: the Fangs of Ashamane scenario, Ruins of Falanaar (map 1612)
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

// feraldruid
struct instance_ruins_of_falanaar_feral : public InstanceScript
{
    instance_ruins_of_falanaar_feral(InstanceMap* map) : InstanceScript(map) {}

    WorldLocation loc_res_pla;

    WorldLocation* GetClosestGraveYard(float x, float y, float z) override
    {
        loc_res_pla.WorldRelocate(1612, x, y, z);

        uint32 graveyardId = 5514;

        if (WorldSafeLocsEntry const* gy = sWorldSafeLocsStore.LookupEntry(graveyardId))
        {
            loc_res_pla.WorldRelocate(gy->MapID, gy->Loc.X, gy->Loc.Y, gy->Loc.Z);
        }
        return &loc_res_pla;
    }
};

void AddSC_instance_ruins_of_falanaar()
{
    RegisterInstanceScript(instance_ruins_of_falanaar_feral, 1612);
}
