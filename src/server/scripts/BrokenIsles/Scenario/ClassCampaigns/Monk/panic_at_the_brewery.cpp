////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Monk class campaign: Panic at the Brewery (quest 45404), Stormstout Brewery (map 1693)
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

//116849
struct npc_stormstout_brewer_q45404 : public ScriptedAI
{
    npc_stormstout_brewer_q45404(Creature* creature) : ScriptedAI(creature) 
    {
        me->SetReactState(REACT_PASSIVE);
        detoxed = false;
    }

    bool detoxed;

    void SpellHit(Unit* caster, const SpellInfo* spell) override
    {
        if ((spell->Id == 218164 || spell->Id == 115450) && !detoxed)
        {
            if (auto plr = caster->ToPlayer())
            {
                detoxed = true;
                Talk(0);
                plr->UpdateAchievementCriteria(CRITERIA_TYPE_KILL_CREATURE, 117025, 1, 0, plr, true);
                me->GetMotionMaster()->MoveRandom(12.f);
                me->DespawnOrUnsummon(7000);
            }
        }
    }
};

void AddSC_panic_at_the_brewery()
{
    RegisterCreatureAI(npc_stormstout_brewer_q45404);
}
