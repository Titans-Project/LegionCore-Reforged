////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "QuestData.h"

struct go_pirate_day_music : public GameObjectAI
{
    go_pirate_day_music(GameObject* go) : GameObjectAI(go)
    {
        events.ScheduleEvent(EVENT_1, 1000);
    }

    EventMap events;

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_1:
                if (IsHolidayActive(HOLIDAY_PIRATES_DAY))
                    go->PlayDirectSound(12845);

                events.ScheduleEvent(EVENT_1, 5000);
                break;
            default:
                break;
            }
        }
    }
};

void AddSC_pirates_day()
{
    RegisterGameObjectAI(go_pirate_day_music);
}
