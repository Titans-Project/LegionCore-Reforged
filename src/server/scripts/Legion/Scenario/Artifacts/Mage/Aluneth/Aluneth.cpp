////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Mage (Arcane) artifact: Aluneth, Greatstaff of the Magna.
    Its acquisition runs in the Nexus Vault of Dalaran (map 1583), shared with the priest's Light's Wrath (quest 41631):
    a map has a single instance script, so the vault's one lives here for both classes.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

//mage & priest nexus 1583
struct instance_priest_mage_nexus_art : public InstanceScript
{
    instance_priest_mage_nexus_art(InstanceMap* map) : InstanceScript(map) {}

    WorldLocation loc_res_pla;

    WorldLocation* GetClosestGraveYard(float x, float y, float z) override
    {
        loc_res_pla.WorldRelocate(1583, x, y, z);

        uint32 graveyardId = 5442;

        if (WorldSafeLocsEntry const* gy = sWorldSafeLocsStore.LookupEntry(graveyardId))
        {
            loc_res_pla.WorldRelocate(gy->MapID, gy->Loc.X, gy->Loc.Y, gy->Loc.Z);
        }
        return &loc_res_pla;
    }
};

void AddSC_Aluneth()
{
    RegisterInstanceScript(instance_priest_mage_nexus_art, 1583);
}
