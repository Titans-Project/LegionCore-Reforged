////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "Cell.h"
#include "CellImpl.h"
#include "CharmInfo.h"
#include "CombatAI.h"
#include "CreatureTextMgr.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "ObjectMgr.h"
#include "ObjectVisitors.hpp"
#include "PassiveAI.h"
#include "PetAI.h"
#include "Player.h"
#include "PlayerDefines.h"
#include "ScriptedCreature.h"
#include "ScriptedFollowerAI.h"
#include "ScriptedEscortAI.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "Vehicle.h"
#include "World.h"
#include "QuestData.h"

/*#####
# npc_spring_rabbit
#####*/

enum rabbitSpells
{
    SPELL_SPRING_FLING          = 61875,
    SPELL_SPRING_RABBIT_JUMP    = 61724,
    SPELL_SPRING_RABBIT_WANDER  = 61726,
    SPELL_SUMMON_BABY_BUNNY     = 61727,
    SPELL_SPRING_RABBIT_IN_LOVE = 61728,
    NPC_SPRING_RABBIT           = 32791
};

class npc_spring_rabbit : public CreatureScript
{
    public:
        npc_spring_rabbit() : CreatureScript("npc_spring_rabbit") { }

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_spring_rabbitAI(creature);
        }

        struct npc_spring_rabbitAI : public ScriptedAI
        {
            npc_spring_rabbitAI(Creature* creature) : ScriptedAI(creature) { }

            bool inLove;
            uint32 jumpTimer;
            uint32 bunnyTimer;
            uint32 searchTimer;
            ObjectGuid rabbitGUID;

            void Reset() override
            {
                inLove = false;
                rabbitGUID.Clear();
                jumpTimer = urand(5000, 10000);
                bunnyTimer = urand(10000, 20000);
                searchTimer = urand(5000, 10000);
                if (Unit* owner = me->GetOwner())
                    me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, float(PET_FOLLOW_ANGLE));
            }

            void EnterCombat(Unit * /*who*/) override { }

            void DoAction(const int32 /*param*/) override
            {
                inLove = true;
                if (Unit* owner = me->GetOwner())
                    owner->CastSpell(owner, SPELL_SPRING_FLING, true);
            }

            void UpdateAI(uint32 diff) override
            {
                if (inLove)
                {
                    if (jumpTimer <= diff)
                    {
                        if (Unit* rabbit = Unit::GetUnit(*me, rabbitGUID))
                            DoCast(rabbit, SPELL_SPRING_RABBIT_JUMP);
                        jumpTimer = urand(5000, 10000);
                    } else jumpTimer -= diff;

                    if (bunnyTimer <= diff)
                    {
                        DoCast(SPELL_SUMMON_BABY_BUNNY);
                        bunnyTimer = urand(20000, 40000);
                    } else bunnyTimer -= diff;
                }
                else
                {
                    if (searchTimer <= diff)
                    {
                        if (Creature* rabbit = me->FindNearestCreature(NPC_SPRING_RABBIT, 10.0f))
                        {
                            if (rabbit == me || rabbit->HasAura(SPELL_SPRING_RABBIT_IN_LOVE))
                                return;

                            me->AddAura(SPELL_SPRING_RABBIT_IN_LOVE, me);
                            DoAction(1);
                            rabbit->AddAura(SPELL_SPRING_RABBIT_IN_LOVE, rabbit);
                            rabbit->AI()->DoAction(1);
                            rabbit->CastSpell(rabbit, SPELL_SPRING_RABBIT_JUMP, true);
                            rabbitGUID = rabbit->GetGUID();
                        }
                        searchTimer = urand(5000, 10000);
                    } else searchTimer -= diff;
                }
            }
        };
};

void AddSC_noblegarden()
{
    new npc_spring_rabbit();
}
