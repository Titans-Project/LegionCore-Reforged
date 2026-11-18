////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
*/

#include <QuestData.h>

struct npc_march_of_the_tadpoles : public ScriptedAI
{
    npc_march_of_the_tadpoles(Creature* creature) : ScriptedAI(creature) {}

    // Summon <tadpole>: 235315 for Murgly, then 235322 onward in the order of the tadpoles from Birbit (118575) to
    // Grrlgmr (118605); Tim (118593, 235340) is the credit of the quest, not a tadpole to talk to
    static uint32 SummonSpellOf(uint32 entry)
    {
        if (entry == 118571)
            return 235315;
        if (entry >= 118575 && entry <= 118605 && entry != 118593)
            return 235322 + (entry - 118575);
        return 0;
    }

    void sGossipSelect(Player* player, uint32 /*sender*/, uint32 action) override
    {
        if (action != 0)
            return;

        player->CLOSE_GOSSIP_MENU();

        uint32 const spell = SummonSpellOf(me->GetEntry());
        if (!spell)
            return;

        player->CastSpell(player, spell, true);
        if (player->GetQuestStatus(46061) == QUEST_STATUS_INCOMPLETE)
            player->KilledMonsterCredit(118593);
    }
};

void AddSC_MarchOfTheTadpoles()
{
    RegisterCreatureAI(npc_march_of_the_tadpoles);
}