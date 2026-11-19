////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Priest: Tuure
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Scenario.h"
#include "ScenarioMgr.h"


class instance_tuure : public InstanceMapScript
{
public:
    instance_tuure() : InstanceMapScript("instance_tuure", 1489) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const override
    {
        return new instance_tuure_InstanceMapScript(map);
    }

    struct instance_tuure_InstanceMapScript : public InstanceScript
    {
        instance_tuure_InstanceMapScript(InstanceMap* map) : InstanceScript(map) {}
        
        WorldLocation loc_res_pla;  // for respawn

        WorldLocation* GetClosestGraveYard(float x, float y, float z) override
        // OnPlayerDeath
        {
            // Init data
            loc_res_pla.WorldRelocate(1489, x, y, z);

            // The map is shared by several acquisitions: the Holy one releases at its own entrance, never at the corpse.
            Scenario* scenario = sScenarioMgr->GetScenario(instance->GetInstanceId());
            if (scenario && scenario->GetScenarioId() == 1085)
                if (WorldSafeLocsEntry const* gy = sWorldSafeLocsStore.LookupEntry(5480))
                    loc_res_pla.WorldRelocate(gy->MapID, gy->Loc.X, gy->Loc.Y, gy->Loc.Z);

            return &loc_res_pla;
        }

        void OnUnitDeath(Unit* creature) override
        {            
            switch (creature->GetEntry())
            {
                case 106201: // the first opponent: its death is the victory, not Boros evading
                    if (Creature* targ = creature->FindNearestCreature(106134, 100.0f, true))
                        targ->AI()->DoAction(3);
                    break;
                case 106221:
                    if (Creature* targ = creature->FindNearestCreature(106134, 100.0f, true))
                        targ->AI()->DoAction(1);
                    break;
            }
        }
         
        void Update(uint32 diff) override {}
    };
};

void AddSC_instance_tuure()
{
    new instance_tuure();
}
