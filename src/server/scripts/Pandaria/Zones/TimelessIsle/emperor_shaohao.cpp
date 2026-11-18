////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Timeless Isle: Emperor Shaohao, who calls the Celestials one after the other
*/

#include "ScriptMgr.h"

enum Says
{
    SAY_CHIJI_START = 0,
    SAY_CHIJI_FINISH = 1,
    SAY_XUEN_START = 2,
    SAY_XUEN_FINISH = 3,
    SAY_YULON_START = 4,
    SAY_YULON_FINISH = 5,
    SAY_NIUZAO_START = 6,
    SAY_NIUZAO_FINISH = 7,
};

enum Npc
{
    NPC_CHIJI = 71952,
    NPC_XUEN = 71953,
    NPC_YULON = 71955,
    NPC_NIUZAO = 71954,
};

enum Events
{
    EVENT_CHIJI_START = 1,
    EVENT_XUEN_START = 2,
    EVENT_YULON_START = 3,
    EVENT_NIUZAO_START = 4,
    EVENT_CHIJI_END = 5,
    EVENT_XUEN_END = 6,
    EVENT_YULON_END = 7,
    EVENT_NIUZAO_END = 8,
};

enum Actions
{
    ACTION_MOVE_CENTR_POSS = 1,
    ACTION_CHIJI_END = 2,
    ACTION_CHIJI_FAIL = 3,
    ACTION_XUEN_END = 4,
    ACTION_XUEN_FAIL = 5,
    ACTION_YULON_END = 6,
    ACTION_YULON_FAIL = 7,
    ACTION_NIUZAO_END = 8,
    ACTION_NIUZAO_FAIL = 9,
};

const Position summonPos[4] = 
{
    {-553.17f, -4949.02f, -6.27f, -2.30f},
    {-740.80f, -4955.08f, -6.27f, 5.63f},
    {-746.32f, -5078.48f, -6.27f, 0.69f},
    {-551.16f, -5080.01f, -6.27f, 2.28f}
};

class npc_emperor_shaohao : public CreatureScript
{
public:
    npc_emperor_shaohao() : CreatureScript("npc_emperor_shaohao") {}

    struct npc_emperor_shaohaoAI : public ScriptedAI
    {
        npc_emperor_shaohaoAI(Creature* creature) : ScriptedAI(creature), summons(me) {}

        EventMap events;
        SummonList summons;

        void Reset() 
        {
            me->SummonCreature(NPC_CHIJI, summonPos[0]);
            me->SummonCreature(NPC_XUEN, summonPos[1]);
            me->SummonCreature(NPC_YULON, summonPos[2]);
            me->SummonCreature(NPC_NIUZAO, summonPos[3]);

            events.RescheduleEvent(urand(EVENT_CHIJI_START, EVENT_NIUZAO_START), 60000);
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
                case ACTION_CHIJI_END:
                    events.RescheduleEvent(EVENT_CHIJI_END, 10000);
                    break;
                case ACTION_CHIJI_FAIL:
                    events.RescheduleEvent(EVENT_CHIJI_START, 30000);
                    break;
                case ACTION_XUEN_END:
                    events.RescheduleEvent(EVENT_XUEN_END, 10000);
                    break;
                case ACTION_XUEN_FAIL:
                    events.RescheduleEvent(EVENT_XUEN_START, 30000);
                    break;
                case ACTION_YULON_END:
                    events.RescheduleEvent(EVENT_YULON_END, 10000);
                    break;
                case ACTION_YULON_FAIL:
                    events.RescheduleEvent(EVENT_YULON_START, 30000);
                    break;
                case ACTION_NIUZAO_END:
                    events.RescheduleEvent(EVENT_NIUZAO_END, 10000);
                    break; 
                case ACTION_NIUZAO_FAIL:
                    events.RescheduleEvent(EVENT_NIUZAO_START, 30000);
                    break;
            }
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CHIJI_START:
                    {
                        Talk(SAY_CHIJI_START);
                        EntryCheckPredicate pred1(NPC_CHIJI);
                        summons.DoAction(ACTION_MOVE_CENTR_POSS, pred1);
                        break;
                    }
                    case EVENT_CHIJI_END:
                        Talk(SAY_CHIJI_FINISH);
                        events.RescheduleEvent(EVENT_XUEN_START, 60000);
                        break;
                    case EVENT_XUEN_START:
                    {
                        Talk(SAY_XUEN_START);
                        EntryCheckPredicate pred1(NPC_XUEN);
                        summons.DoAction(ACTION_MOVE_CENTR_POSS, pred1);
                        break;
                    }
                    case EVENT_XUEN_END:
                        Talk(SAY_XUEN_FINISH);
                        events.RescheduleEvent(EVENT_YULON_START, 60000);
                        break;
                    case EVENT_YULON_START:
                    {
                        Talk(SAY_YULON_START);
                        EntryCheckPredicate pred1(NPC_YULON);
                        summons.DoAction(ACTION_MOVE_CENTR_POSS, pred1);
                        break;
                    }
                    case EVENT_YULON_END:
                    {
                        Talk(SAY_YULON_FINISH);
                        events.RescheduleEvent(EVENT_NIUZAO_START, 60000);
                        break;
                    }
                    case EVENT_NIUZAO_START:
                    {
                        Talk(SAY_NIUZAO_START);
                        EntryCheckPredicate pred1(NPC_NIUZAO);
                        summons.DoAction(ACTION_MOVE_CENTR_POSS, pred1);
                        break;
                    }
                    case EVENT_NIUZAO_END:
                    {
                        Talk(SAY_NIUZAO_FINISH);
                        events.RescheduleEvent(EVENT_CHIJI_START, 60000);
                        break;
                    }
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_emperor_shaohaoAI (creature);
    }
};

void AddSC_emperor_shaohao()
{
    new npc_emperor_shaohao();
}
