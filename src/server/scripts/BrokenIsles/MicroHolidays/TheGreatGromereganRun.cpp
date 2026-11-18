////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
*/

struct npc_great_gnomeregan_race_gate : ScriptedAI
{
    npc_great_gnomeregan_race_gate(Creature* creature) : ScriptedAI(creature) {}

    std::set<ObjectGuid> PlayerList;

    void MoveInLineOfSight(Unit* who) override
    {
        if (!who || !who->IsPlayer())
            return;

        if (me->GetDistance(who) > 7.0f)
            return;

        // only during the race; a racer who has finished, or given it up, may run it again once he takes it anew
        if (who->ToPlayer()->GetQuestStatus(47709) != QUEST_STATUS_INCOMPLETE)
        {
            PlayerList.erase(who->GetGUID());
            return;
        }

        if (PlayerList.find(who->GetGUID()) == PlayerList.end())
        {
            PlayerList.insert(who->GetGUID());
            DoCast(246069);
            me->CastSpell(who, 245989, true);
        }
    }
};

struct npc_great_gnomeregan_race_gate_start : ScriptedAI
{
    npc_great_gnomeregan_race_gate_start(Creature* creature) : ScriptedAI(creature) {}

    std::set<ObjectGuid> PlayerList;

    void MoveInLineOfSight(Unit* who) override
    {
        if (!who || !who->IsPlayer())
            return;

        if (me->GetDistance(who) > 7.0f)
            return;

        // only during the race; a racer who has finished, or given it up, may run it again once he takes it anew
        if (who->ToPlayer()->GetQuestStatus(47709) != QUEST_STATUS_INCOMPLETE)
        {
            PlayerList.erase(who->GetGUID());
            return;
        }

        if (PlayerList.find(who->GetGUID()) == PlayerList.end())
        {
            PlayerList.insert(who->GetGUID());
            DoCast(246070);
            me->CastSpell(who, 245999, true);
        }
    }
};

struct npc_great_gnomeregan_race_gate_finish : ScriptedAI
{
    npc_great_gnomeregan_race_gate_finish(Creature* creature) : ScriptedAI(creature) {}

    std::set<ObjectGuid> PlayerList;

    void MoveInLineOfSight(Unit* who) override
    {
        if (!who || !who->IsPlayer())
            return;

        if (me->GetDistance(who) > 7.0f)
            return;

        // only during the race; a racer who has finished, or given it up, may run it again once he takes it anew
        if (who->ToPlayer()->GetQuestStatus(47709) != QUEST_STATUS_INCOMPLETE)
        {
            PlayerList.erase(who->GetGUID());
            return;
        }

        if (PlayerList.find(who->GetGUID()) == PlayerList.end())
        {
            PlayerList.insert(who->GetGUID());
            DoCast(246070);
            me->CastSpell(who, 246000, true);
        }
    }
};

void AddSC_TheGreatGromereganRun()
{
    RegisterCreatureAI(npc_great_gnomeregan_race_gate);
    RegisterCreatureAI(npc_great_gnomeregan_race_gate_start);
    RegisterCreatureAI(npc_great_gnomeregan_race_gate_finish);
}