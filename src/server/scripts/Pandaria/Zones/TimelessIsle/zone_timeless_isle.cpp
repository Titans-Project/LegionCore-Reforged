////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum TimelessSpiritSpells
{
    SPELL_BANISHMENT            = 148705,
    SPELL_GHOSTLY_VOID          = 147495,
    SPELL_DESATURATE            = 129290,
    SPELL_SPIRIT_STRANGLE       = 144059
};

class npc_timeless_spirit : public CreatureScript
{
public:
    npc_timeless_spirit() : CreatureScript("npc_timeless_spirit") {}

    struct npc_timeless_spiritAI : public ScriptedAI
    {
        npc_timeless_spiritAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 SpiritStrangle_timer;

        void Reset() 
        {
            SpiritStrangle_timer = 0;
            //me->GetMotionMaster()->MoveRandom(10.0f);
            DoCast(SPELL_GHOSTLY_VOID);
            DoCast(SPELL_DESATURATE);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (Unit* pTarget = me->getVictim())
            {
                if (me->GetDistance(pTarget) >= 15.0f)
                    EnterEvadeMode();

                if (SpiritStrangle_timer <= diff)
                {
                    DoCast(pTarget, SPELL_SPIRIT_STRANGLE);
    
                    SpiritStrangle_timer = 7000;
                }
                else SpiritStrangle_timer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_timeless_spiritAI (creature);
    }
};

class at_ordos_entrance : public AreaTriggerScript
{
public:
    at_ordos_entrance() : AreaTriggerScript("at_ordos_entrance") { }

    bool OnTrigger(Player* pPlayer, const AreaTriggerEntry* /*pAt*/, bool /*enter*/)
    {
        if (pPlayer->isGameMaster())
            return false;

        // A Pandaren Legend
        if (pPlayer->GetQuestStatus(33104) != QUEST_STATUS_REWARDED && !pPlayer->HasAchieved(8325))
            pPlayer->CastSpell(pPlayer, SPELL_BANISHMENT, true);

        return false;
    }
};

enum eZarimEvents
{
    QUEST_BONE_APART_INTRO      = 33348,
    NPC_ZARIM                   = 71876,
    SPELL_BONE_APART_INTRO      = 149122,
};
// AT - 9211
class at_tom_bone_apart : public AreaTriggerScript
{
    public:
        at_tom_bone_apart() : AreaTriggerScript("at_tom_bone_apart") {}

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/, bool /*enter*/)
        {
            if (Creature* Zarim = GetClosestCreatureWithEntry(player, NPC_ZARIM, 30.0f))
            {
                if (player->GetDailyQuestStatus(QUEST_BONE_APART_INTRO) != QUEST_STATUS_REWARDED)
                {
                    player->CastSpell(player, SPELL_BONE_APART_INTRO, true);
                    Zarim->AI()->Talk(0);
                }
            }
            return false;
        }
};


void AddSC_timeless_isle()
{
    new npc_timeless_spirit();
    //new at_ordos_entrance();
    //new at_tom_bone_apart();
}