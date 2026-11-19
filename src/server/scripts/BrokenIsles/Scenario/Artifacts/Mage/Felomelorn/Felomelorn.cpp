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
    The Karazhan-side boss fight (scenario 957, map 1480) lives in the database; the script below only covers the
    one case it cannot express.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "../MageArtifactKillCredit.h"

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

enum LyandraFight
{
    NPC_AETHAS_SUNREAVER = 99763
};

// Lyandra Sunstrider - 99615
// Aethas, who walks out of the fight to open the way back, is only summoned by the half-health phase. A boss killed
// from above half health in one blow never reaches it, and the return portal is never created.
struct npc_fire_artifact_lyandra : public npc_mage_artifact_kill_credit
{
    explicit npc_fire_artifact_lyandra(Creature* creature) : npc_mage_artifact_kill_credit(creature) { }

    void JustDied(Unit* killer) override
    {
        npc_mage_artifact_kill_credit::JustDied(killer);

        if (me->FindNearestCreature(NPC_AETHAS_SUNREAVER, 100.0f))
            return;

        // Same position, type and lifetime as the summon of the half-health phase
        if (Creature* aethas = me->SummonCreature(NPC_AETHAS_SUNREAVER, 4657.79f, 2785.37f, 361.19f, 5.38595f, TEMPSUMMON_MANUAL_DESPAWN))
            if (aethas->AI())
                aethas->AI()->SetData(3, 3); // the data the death sequence sent to nobody
    }
};

void AddSC_Felomelorn()
{
    //RegisterInstanceScript(instance_violethold_mage, 1494);
    RegisterCreatureAI(npc_fire_artifact_lyandra);
}
