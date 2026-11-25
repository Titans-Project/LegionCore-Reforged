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

/*######
## go_midsummer_bonfire
######*/

enum MidsummerBonfire
{
    STAMP_OUT_BONFIRE_QUEST_COMPLETE    = 45458,
};

class go_midsummer_bonfire : public GameObjectScript
{
public:
    go_midsummer_bonfire() : GameObjectScript("go_midsummer_bonfire") { }

    bool OnGossipSelect(Player* player, GameObject* /*go*/, uint32 /*sender*/, uint32 /*action*/) override
    {
        player->CastSpell(player, STAMP_OUT_BONFIRE_QUEST_COMPLETE, true);
        player->CLOSE_GOSSIP_MENU();
        return false;
    }
};

struct go_midsummer_music : public GameObjectAI
{
    go_midsummer_music(GameObject* go) : GameObjectAI(go)
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
            {
                if (IsHolidayActive(HOLIDAY_FIRE_FESTIVAL))
                {
                    std::list<Player*> playersNearby;
                    go->GetPlayerListInGrid(playersNearby, go->GetMap()->GetVisibilityRange());
                    for (auto& player : playersNearby)
                    {
                        if (player->GetTeamId() == TEAM_HORDE)
                            go->PlayDirectSound(12325, player);
                        else
                            go->PlayDirectSound(12319, player);
                    }
                }
                events.ScheduleEvent(EVENT_1, 5000);
                break;
            }
            default:
                break;
            }
        }
    }
};

void AddSC_midsummer_fire_festival()
{
    new go_midsummer_bonfire();
    RegisterGameObjectAI(go_midsummer_music);
}
