////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////
#include "ScriptedEscortAI.h"
#include "SpellPackets.h"
#include "MoveSpline.h"
#include "CreatureTextMgr.h"
#include "MapManager.h"
#include "ScriptMgr.h"
#include "GameObjectAI.h"
#include "SpellAuraEffects.h"
#include "WaypointManager.h"
#include "Containers.h"
#include "QuestData.h"
#include <deque>

// The Illidari of "The Invasion Begins" (40077): on accepting, copies seen by the player alone take the places of the
// fixed spawns, so the phase change does not show, then charge toward the battlefield after the quest's lines
struct IllidariPlace
{
    uint32 Entry;
    Position Place;
    uint32 DeparturePath;
    bool Ready;                 // stands on guard, glaives out, like the fixed spawn
};

static IllidariPlace const IllidariPlaces[] =
{
    { 93011, { 1179.57f, 3202.61f, 51.4265f, 4.87377f }, 10267107, false }, // Kayn Sunfury
    { 98228, { 1182.36f, 3202.91f, 51.5215f, 4.88566f }, 10267108, false },
    { 98227, { 1177.00f, 3203.07f, 51.4470f, 4.88746f }, 10267109, true  },
    { 99918, { 1172.92f, 3207.82f, 52.4768f, 3.73185f }, 10267110, true  },
    { 98292, { 1170.74f, 3204.71f, 51.6260f, 3.36744f }, 10267111, true  },
    { 98290, { 1171.49f, 3203.69f, 51.3145f, 3.45640f }, 10267112, false },
};

enum IllidariRun
{
    DATA_ILLIDARI_DEPART        = 1,

    QUEST_ILLIDAN_SPOKE         = 40076,
    QUEST_INVASION_BEGINS       = 40077,
    CONVERSATION_ILLIDAN_CUE    = 705,
    CONVERSATION_KAYN_LINES     = 922,

    KAYN_LINES_TIME             = 14500,    // conversation 922: three lines, "Kill them all!" at 12.5 s
    DEPARTURE_RUN_TIME          = 16000,

    // the departure paths stop at the stream's bank and go on, as path + 100, from the far one: in between, a fel rush
    POINT_STREAM_RUSH           = 1,
    FAR_BANK_PATH_OFFSET        = 100,
    SPELL_FEL_RUSH              = 197922,
};

static Position const StreamBank = { 1167.5f, 3170.5f, 34.1f };

// A copy seen by the player alone, on the place of the fixed spawn of the same entry
static Creature* SummonIllidariCopy(Player* player, IllidariPlace const& place, Position const& at)
{
    Creature* copy = player->SummonCreature(place.Entry, at, TEMPSUMMON_MANUAL_DESPAWN, 0, 0, player->GetGUID());
    if (copy)
    {
        copy->AddPlayerInPersonnalVisibilityList(player->GetGUID());
        if (place.Ready)
        {
            copy->SetSheath(SHEATH_STATE_MELEE);
            copy->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_READY1H);
        }
    }
    return copy;
}

class sceneTrigger_dh_init : public SceneTriggerScript
{
public:
    sceneTrigger_dh_init() : SceneTriggerScript("sceneTrigger_dh_init")
    {}

    bool OnTrigger(Player* player, SpellScene const* /*trigger*/, std::string type) override
    {
        // Illidan speaks while the scene's own Illidari run to their places
        if (type == "CUEILLIDANTH")
        {
            Conversation* conversation = new Conversation;
            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), CONVERSATION_ILLIDAN_CUE, player, NULL, *player))
                delete conversation;
        }
        // The scene is over: its actors make way for the fixed group, shown by this quest, which lets Kayn offer the next one
        else if (type == "complete" && !player->GetQuestRewardStatus(QUEST_ILLIDAN_SPOKE))
            player->CompleteQuest(QUEST_ILLIDAN_SPOKE);

        return true;
    }
};

// 250560 - Legion Banner
class go_q40077 : public GameObjectScript
{
public:
    go_q40077() : GameObjectScript("go_q40077") { }

    struct go_q40077AI : public GameObjectAI
    {
        go_q40077AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            SCENE = 191677,
            SCENE_INVASION_BEGINS = 1116,
            NPC_WRATH_WARRIOR = 98486,
            SPAWN_PLANTED_BANNER = 600200,
        };

        // the planted banner is the same object as the one to plant, but it stays a mere decoration
        void Reset() override
        {
            if (go->GetDBTableGUIDLow() == SPAWN_PLANTED_BANNER)
                go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
        }

        // the banner has a lock: using it opens the lock, which calls GossipUse, not GossipHello
        bool GossipUse(Player* player) override
        {
            return GossipHello(player, true);
        }

        bool GossipHello(Player* player, bool isUse) override
        {
            if (!isUse)
                return true;

            /*

            ServerToClient: SMSG_PLAY_SCENE (0x2651) Length: 34 ConnIdx: 0 Time: 02/22/2016 12:57:13.116 Number: 11210
            SceneID: 1116
            PlaybackFlags: 27
            SceneInstanceID: 2
            SceneScriptPackageID: 1493
            TransportGUID: Full: 0x0
            Pos: X: 1002.885 Y: 2956.695 Z: -10.55672
            Facing: 4.76053
            */
            // planted once, after the demons are slain, since planting clears the battlefield
            if (player->GetQuestStatus(40077) == QUEST_STATUS_INCOMPLETE && !player->GetQuestObjectiveData(40077, go->GetEntry())
                && player->GetQuestObjectiveData(40077, NPC_WRATH_WARRIOR) >= 15)
            {
                player->CastSpell(player, SCENE, true);
                bool hasAura = player->HasAura(SCENE);
                if (!hasAura)
                {
                    Position pos = go->GetPosition();
                    player->SendSpellScene(SCENE_INVASION_BEGINS, nullptr, true, &pos);
                }
                // the credit switches the battlefield to its cleared phase as the scene starts
                player->KillCreditGO(go->GetEntry(), go->GetGUID());
            }
            return true;
        }

    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q40077AI(go);
    }
};

//93011
class npc_q40077 : public CreatureScript
{
public:
    npc_q40077() : CreatureScript("npc_q40077") { }

    bool OnQuestAccept(Player* player, Creature* /*creature*/, Quest const* quest) override
    {
        if (quest->GetQuestId() != QUEST_INVASION_BEGINS)
            return true;

        // The fixed group leaves the player's phase as the copies take its places, then Kayn's lines are spoken
        for (IllidariPlace const& place : IllidariPlaces)
            if (Creature* copy = SummonIllidariCopy(player, place, place.Place))
                copy->AI()->SetData(DATA_ILLIDARI_DEPART, place.DeparturePath);

        player->GetMap()->LoadGrid(1170.74f, 3204.71f);
        Conversation* conversation = new Conversation;
        if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), CONVERSATION_KAYN_LINES, player, NULL, *player))
            delete conversation;

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override;
};

// Kayn and the five Illidari: once the quest's lines are spoken, the copies charge toward the battlefield
struct npc_illidari_invasion_begins : public ScriptedAI
{
    npc_illidari_invasion_begins(Creature* creature) : ScriptedAI(creature) { }

    void SetData(uint32 type, uint32 pathId) override
    {
        if (type != DATA_ILLIDARI_DEPART)
            return;

        _path = pathId;
        _departTimer = KAYN_LINES_TIME;
    }

    void UpdateAI(uint32 diff) override
    {
        UpdateStreamLeap();
        if (!_departTimer)
            return;

        if (_departTimer > diff)
        {
            _departTimer -= diff;
            return;
        }

        _departTimer = 0;
        me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_ONESHOT_NONE);
        me->GetMotionMaster()->MovePath(_path, false);
        me->DespawnOrUnsummon(DEPARTURE_RUN_TIME);
        _toBank = true;
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type != POINT_MOTION_TYPE || id != POINT_STREAM_RUSH)
            return;

        me->RemoveAurasDueToSpell(SPELL_FEL_RUSH);
        me->GetMotionMaster()->MovePath(_path + FAR_BANK_PATH_OFFSET, false);
    }

    // the stream is crossed as a demon hunter does it: a fel rush from the edge of the bank straight to the far one
    void UpdateStreamLeap()
    {
        if (!_toBank || me->GetExactDist2d(&StreamBank) > 1.5f)
            return;

        _toBank = false;
        WaypointPath const* farBank = sWaypointMgr->GetPath(_path + FAR_BANK_PATH_OFFSET);
        if (!farBank || farBank->empty())
            return;

        WaypointData const& landing = farBank->front();
        me->GetMotionMaster()->Clear();
        me->SetFacingTo(me->GetAngle(landing.x, landing.y));
        me->CastSpell(me, SPELL_FEL_RUSH, true);
        me->GetMotionMaster()->MoveCharge(landing.x, landing.y, landing.z, 30.0f, POINT_STREAM_RUSH);
    }

private:
    uint32 _path = 0;
    uint32 _departTimer = 0;
    bool _toBank = false;
};

CreatureAI* npc_q40077::GetAI(Creature* creature) const
{
    return new npc_illidari_invasion_begins(creature);
}

class npc_q40077_1 : public CreatureScript
{
public:
    npc_q40077_1() : CreatureScript("npc_q40077_1") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_illidari_invasion_begins(creature);
    }
};

// 241751
class go_q40378 : public GameObjectScript
{
public:
    go_q40378() : GameObjectScript("go_q40378") { }

    struct go_q40378AI : public GameObjectAI
    {
        go_q40378AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            SCENE = 189261,
            QUEST = 40378,
            CREDIT = 88872,
        };

        // the activator has a lock: opening it calls GossipUse, never GossipHello
        bool GossipUse(Player* player) override
        {
            return GossipHello(player, true);
        }

        bool GossipHello(Player* player, bool isUse) override
        {
            if (!isUse)
                return true;

            if (player->GetQuestStatus(QUEST) != QUEST_STATUS_INCOMPLETE || player->GetReqKillOrCastCurrentCount(QUEST, CREDIT))
                return true;

            /*
            ClientToServer: CMSG_GAME_OBJ_REPORT_USE (0x34DE) Length: 15 ConnIdx: 2 Time: 02/06/2016 22:39:25.012 Number: 16325
            GameObjectGUID: Full: 0x2C2090B920EEB5C00000100001364D15; HighType: GameObject; Low: 20335893; Map: 1481; Entry: 244439;
            */
            player->Dismount();
            player->KilledMonsterCredit(CREDIT);
            player->CastSpell(player, SCENE, true);
            
            player->SummonCreature(99916, 1023.25f, 2849.71f, 5.42f, 1.75f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
            //    sayer->AddPlayerInPersonnalVisibilityList(player->GetGUID());
            if (Creature* mount = player->SummonCreature(101518, 1016.71f, 2849.31f, 5.48f, 1.38f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))   // 16
            {
              //  mount->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                mount->SetVisible(false);
            }
            // the core then activates the gateway activator, which plays its animation
            return false;
        }
 
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q40378AI(go);
    }
};


/*
before OBJECT_FIELD_DYNAMIC_FLAGS: 4294902018
after OBJECT_FIELD_DYNAMIC_FLAGS: 2473853184

*/

//244439, 244440, 244441, 243873
class go_q39279 : public GameObjectScript
{
public:
    go_q39279() : GameObjectScript("go_q39279") { }

    struct go_q39279AI : public GameObjectAI
    {
        go_q39279AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 39279,
            GO_244439 = 244439,
            GO_244440 = 244440,
            GO_244441 = 244441,
            GO_243873 = 243873,
            NPC_CONV_GO_244439 = 558,
            NPC_CONV_GO_244440 = 583,
            SPELL_PORTAL_EXPLOSION = 190610,
            NPC_EREDAR_SUMMONER = 102714,
            TEXT_PORTAL_LOST = 1,       // creature_text group of the summoners
            DATA_PORTAL_LOST = 1,       // npc_mardum_eredar_summoner: the portal-lost line replaces its combat one
        };

        uint32 _despawnTimer = 0;

        bool ObjectiveDone(Player* player, uint32 entry) const
        {
            Quest const* quest = sQuestDataStore->GetQuestTemplate(QUEST);
            if (!quest)
                return true;

            for (QuestObjective const& obj : quest->GetObjectives())
                if (obj.ObjectID == int32(entry))
                    return player->GetQuestObjectiveData(quest, obj.StorageIndex) >= obj.Amount;

            return false;
        }

        // the destroyed portal vanishes once its destroy animation has played, until it respawns
        void UpdateAI(uint32 diff) override
        {
            if (!_despawnTimer)
                return;

            if (_despawnTimer > diff)
            {
                _despawnTimer -= diff;
                return;
            }

            _despawnTimer = 0;
            // hidden for the respawn time of its spawn: the core loads these objects as never despawning, with no delay;
            // hidden before going back to the ready state, which in sight would play the rebuild animation
            int32 delay = 120;
            if (GameObjectData const* data = go->GetGOData())
                if (data->spawntimesecs > 0)
                    delay = data->spawntimesecs;
            go->SetRespawnTime(delay);
            go->SetGoState(GO_STATE_READY);
            go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);
        }

        // the objects have a lock: opening them calls GossipUse, never GossipHello; the core then gives the credit
        bool GossipUse(Player* player) override
        {
            return GossipHello(player, true);
        }

        bool GossipHello(Player* player, bool isUse) override
        {
            if (!isUse)
                return true;

            // only during the bonus objective; it counts several objects of each kind, so only a completed objective stops the use
            if (player->GetQuestStatus(QUEST) != QUEST_STATUS_INCOMPLETE || ObjectiveDone(player, go->GetEntry()))
                return true;

            if (_despawnTimer)
                return true;

            switch (go->GetEntry())
            {
                case GO_244439:
                {
                    Conversation* conversation = new Conversation;
                    if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), NPC_CONV_GO_244439, player, NULL, *player))
                        delete conversation;
                    break;
                }
                case GO_244440:
                {
                    Conversation* conversation = new Conversation;
                    if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), NPC_CONV_GO_244440, player, NULL, *player))
                        delete conversation;
                    break;
                }
                case GO_244441:
                    break;
                case GO_243873:
                    // the portal plays its own destroy animation and blows up; the core would only switch it on and off
                    player->KillCreditGO(go->GetEntry(), go->GetGUID());
                    player->CastSpell(player, SPELL_PORTAL_EXPLOSION, true);
                    go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);
                    // the alternative state ("opened by cannon fire") is what plays the model's Destroy animation
                    go->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
                    _despawnTimer = 1950;    // the Destroy animation lasts 2 s
                    // the summoners that kept the portal turn on the player, one of them crying out
                    {
                        std::list<Creature*> summoners;
                        go->GetCreatureListWithEntryInGrid(summoners, NPC_EREDAR_SUMMONER, 30.0f);
                        bool spoke = false;
                        for (Creature* summoner : summoners)
                        {
                            if (!summoner->IsAlive() || summoner->isInCombat() || !summoner->IsValidAttackTarget(player))
                                continue;
                            summoner->AI()->SetData(DATA_PORTAL_LOST, 1);
                            if (!spoke)
                            {
                                summoner->AI()->Talk(TEXT_PORTAL_LOST, player->GetGUID());
                                spoke = true;
                            }
                            summoner->AI()->AttackStart(player);
                        }
                    }
                    return true;
            }

            // a used communicator vanishes a moment later; the core gives the credit
            _despawnTimer = 1000;
            return false;
        }

    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q39279AI(go);
    }
};

/*
ClientToServer: CMSG_SPELL_CLICK (0x348C) Length: 16 ConnIdx: 2 Time: 02/06/2016 22:40:30.961 Number: 17163
SpellClickUnitGUID: Full: 0x202090B9205EDD800000100002B64D15; HighType: Creature; Low: 45501717; Map: 1481; Entry: 97142;
TryAutoDismount: True

conv - 581
*/

// Destroying Fel Spreader - 191827
class spell_legion_q39279 : public SpellScriptLoader
{
public:
    spell_legion_q39279() : SpellScriptLoader("spell_legion_q39279") { }

    class spell_legion_q39279_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_legion_q39279_SpellScript);

        enum data
        {
            QUEST = 39279,
            NPC_CONV = 581,
            ANIMKIT_DESTROY = 2739,     // a single segment playing animation 150 (Destroy)
        };

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(EFFECT_0);
            if (Unit* caster = GetCaster())
            {
                Player *player = caster->ToPlayer();
                if (!player)
                    return;

                if (Unit * target = GetHitUnit())
                {
                    // the spreader model has no death animation but a destroy one (4 s): it plays, then the spreader
                    // vanishes and comes back a minute later
                    player->KilledMonsterCredit(target->GetEntry());
                    if (Creature* spreader = target->ToCreature())
                    {
                        spreader->RemoveFlag64(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                        spreader->PlayOneShotAnimKit(ANIMKIT_DESTROY);
                        spreader->DespawnOrUnsummon(3900, Seconds(60));
                    }

                    Conversation* conversation = new Conversation;
                    if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), NPC_CONV, player, NULL, *player))
                        delete conversation;
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_legion_q39279_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_legion_q39279_SpellScript();
    }
};

//94410
class npc_q40378 : public CreatureScript
{
public:
    npc_q40378() : CreatureScript("npc_q40378") { }
    
    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == 39049) 
            sCreatureTextMgr->SendChat(creature, 1, player->GetGUID());
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q40378AI(creature);
    }

    enum data
    {
        QUEST = 40378,
    };

    struct npc_q40378AI : public ScriptedAI
    {
        npc_q40378AI(Creature* creature) : ScriptedAI(creature)
        {
            timer = 1000;
        }
        
        uint32 timer;

        void Reset() override
        {
            if (Creature* target = me->FindNearestCreature(105316, 40.0f))
                me->CastSpell(target, 188437, true); // visual
        }

        void MoveInLineOfSight(Unit* who) override
        {
            Player *player = who->ToPlayer();
            if (!player || !me->IsWithinDistInMap(who, 100.0f))
                return;

            if (player->GetQuestStatus(QUEST) != QUEST_STATUS_INCOMPLETE)
                return;

            if (player->GetQuestObjectiveData(QUEST, me->GetEntry()))
                return;

            player->KilledMonsterCredit(me->GetEntry());
            sCreatureTextMgr->SendChat(me, 0, player->GetGUID());
            return;
        }
        
        void UpdateAI(uint32 diff) override
        {
            if (timer <= diff)
            {
                // the beam is an endless channel: casting it again would cut it and start it over
                if (!me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                    if (Creature* target = me->FindNearestCreature(105316, 40.0f))
                        me->CastSpell(target, 188437, true); // visual
                timer = 3000;
            } else
                timer -= diff;
                
        }
    };
};


class conversation_announcer : public CreatureScript
{
public:
    conversation_announcer() : CreatureScript("conversation_announcer") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new conversation_announcerAI(creature);
    }

    struct conversation_announcerAI : public ScriptedAI
    {
        conversation_announcerAI(Creature* creature) : ScriptedAI(creature)
        {
            SetCanSeeEvenInPassiveMode(true);
            if (me->GetEntry() == 101748)
                events.RescheduleEvent(EVENT_3, 6000);
        }

        void Reset() override
        {
            conversationEntry = 0;
            targetGUID.Clear();
            //me->SetReactState(REACT_PASSIVE);
        }

        enum events
        {
            EVENT_1 = 1,
            EVENT_2_ANNOUNCER6 = 2,
            EVENT_CLEAR = 3,
            EVENT_3 = 4,
        };

        enum npcs
        {
            NPC_ANNOUNCER_1 = 101748, //583
            NPC_ANNOUNCER_2 = 101781, //1542
            NPC_ANNOUNCER_3 = 100510, //531
            NPC_BRIDGE_CREDIT = 97400,  // the broken bridge to the Soul Engine: Tyranna taunts, Cyana answers
        };

        uint32 conversationEntry;
        ObjectGuid targetGUID;
        GuidSet m_player_for_event;
        EventMap events;
        void MoveInLineOfSight(Unit* who) override
        {
if (who->GetTypeId() != TYPEID_PLAYER || who->IsOnVehicle())
                return;

            GuidSet::iterator itr = m_player_for_event.find(who->GetGUID());
            if (itr != m_player_for_event.end())
                return;

            if (!me->IsWithinDistInMap(who, 100.0f))
                return;

            uint32 eTimer = 1000;
            /*
            area 7742
            [1] Object Guid: Full: 0x1C2090B9200084C000109C00004B0C80; HighType: Conversation; Low: 4918400; Map: 1481; Entry: 531;
            [1] Stationary Position: X: 1561.098 Y: 2623.586 Z: 30.56277

            area 7705
            [0] Object Guid: Full: 0x1C2090B92000BAC000109C00004B0CE6; HighType: Conversation; Low: 4918502; Map: 1481; Entry: 747;
            [0] Stationary Position: X: 1429.753 Y: 2362.509 Z: 61.22379
            */
            switch (me->GetEntry())
            {
                case NPC_ANNOUNCER_1:
                    if (me->GetAreaId() == 7742) //Quest 38766
                    {
                        // several geysers stand around Beliash: Jace speaks once per player, not once per geyser
                        static GuidSet heardJace;
                        if (who->ToPlayer()->GetQuestStatus(38766) == QUEST_STATUS_REWARDED || !heardJace.insert(who->GetGUID()).second)
                            return;
                        conversationEntry = 531;
                        events.RescheduleEvent(EVENT_1, eTimer);
                    }
                    break;
                case NPC_ANNOUNCER_2:
                    conversationEntry = 1542;
                    events.RescheduleEvent(EVENT_2, eTimer);
                    break;
                case NPC_BRIDGE_CREDIT:
                {
                    // on the way to the Soul Engine with "Their Numbers Are Legion", once per player: the hidden objective keeps it
                    Player* player = who->ToPlayer();
                    // the way runs over the bridge and under it: the height does not count
                    if (me->GetExactDist2d(who) > 25.0f || player->GetQuestStatus(38819) != QUEST_STATUS_INCOMPLETE
                        || player->GetQuestObjectiveData(38819, NPC_BRIDGE_CREDIT))
                        return;
                    player->KilledMonsterCredit(NPC_BRIDGE_CREDIT);
                    conversationEntry = 569;    // Tyranna taunts from afar, Cyana answers, Kayn calls his captains
                    events.RescheduleEvent(EVENT_1, 500);
                    break;
                }
                default:
                    break;
            }

            if (!conversationEntry)
                return;

            if (me->GetEntry() != NPC_BRIDGE_CREDIT)    // it keeps its own count
                m_player_for_event.insert(who->GetGUID());
            //events.RescheduleEvent(EVENT_CLEAR, 300000);
            targetGUID = who->GetGUID();
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                    {
                        if (Player* player = sObjectAccessor->FindPlayer(targetGUID))
                        {
                            Conversation* conversation = new Conversation;
                            bool const created = conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), conversationEntry, player, NULL, *player);
                            if (!created)
                                delete conversation;
                        }
                        break;
                    }
                    case EVENT_2:
                    {
                        if (Player* player = sObjectAccessor->FindPlayer(targetGUID))
                        {
                            player->SendSpellScene(conversationEntry, nullptr, true, player);
                        }
                        break;
                    }

                    case EVENT_CLEAR:
                        m_player_for_event.clear();
                        break;
                    case EVENT_3:
                        DoCast(200680);
                        events.RescheduleEvent(EVENT_3, 10000);
                        break;
                }
            }
        }
    };
};

//93105
class npc_q39049 : public CreatureScript
{
public:
    npc_q39049() : CreatureScript("npc_q39049") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q39049AI(creature);
    }

    enum data
    {
        QUEST       = 39049,
        CREDIT      = 105946,
        SPELL_LEARN = 195447,
        
        SPELL_1     = 194519,  // 2 + 5
        SPELL_2     = 195058,  // 5
        SPELL_PHASE = 192665, // 53%
        SPELL_INF   = 192709,
    };

    struct npc_q39049AI : public Scripted_NoMovementAI
    {
        npc_q39049AI(Creature* creature) : Scripted_NoMovementAI(creature), summons(me), phase2(false)
        {
            creature->SetCorpseDelay(30);
            creature->SetRespawnDelay(15);
        }

        EventMap events;
        SummonList summons;
        bool phase2;
        
        void Reset() override
        {
            phase2 = false;
            me->SetReactState(REACT_DEFENSIVE);
            SetFlyMode(false);
            summons.DespawnAll();
            events.Reset();
        }

        void EnterCombat(Unit* victim) override
        {
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0);
            events.RescheduleEvent(EVENT_1, 2000);
            events.RescheduleEvent(EVENT_2, 5000);
        }

        void JustDied(Unit* attacker) override
        {
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_2);

            Player *player = attacker->ToPlayer();
            if (!player)
                return;

            if (player->GetQuestStatus(QUEST) != QUEST_STATUS_INCOMPLETE)
                return;

            player->KilledMonsterCredit(CREDIT);
            player->CastSpell(player, SPELL_LEARN, false);
        }
        
        void JustSummoned(Creature* summon) override
        { 
            summons.Summon(summon);
            // summon->CastSpell(summon, 195051, false);
            summon->GetMotionMaster()->MoveRotate(20000, ROTATE_DIRECTION_RIGHT);
        }
        
        void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType dmgType) override
        {
            if (me->HealthBelowPct(53) && !phase2)
            {
                phase2 = true;
                me->SetReactState(REACT_PASSIVE);
                DoCast(SPELL_PHASE);
                sCreatureTextMgr->SendChat(me, TEXT_GENERIC_1);
                summons.DespawnAll();
                SetFlyMode(true);
                me->GetMotionMaster()->MovePoint(0, 593.33f, 2433.02f, -64.88f);
                events.CancelEvent(EVENT_1);
                events.CancelEvent(EVENT_2);
                events.RescheduleEvent(EVENT_3, 3000);
                events.RescheduleEvent(EVENT_4, 15000);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);
            
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_1);
                        events.RescheduleEvent(EVENT_1, 5000);
                        break;
                    case EVENT_2:
                        {
                            float radius = 5.0f; // растояние от кастера
                            float coneAngle = 0.0f; // угол куда поставится моб с АТ
                            Position position = me->GetNearPosition(radius, coneAngle);
                            me->CastSpell(position.GetPositionX(), position.GetPositionY(), position.GetPositionZ(), 195061, true);
                            coneAngle = 2.0f; // угол куда поставится моб с АТ
                            position = me->GetNearPosition(radius, coneAngle);
                            me->CastSpell(position.GetPositionX(), position.GetPositionY(), position.GetPositionZ(), 195061, true);
                            coneAngle = 4.0f; // угол куда поставится моб с АТ
                            position = me->GetNearPosition(radius, coneAngle);
                            me->CastSpell(position.GetPositionX(), position.GetPositionY(), position.GetPositionZ(), 195061, true);
                        }
                        break;
                    case EVENT_3:
                        if (Creature* inf = me->FindNearestCreature(96159, 300.0f))
                            inf->CastSpell(inf, SPELL_INF);
                        events.RescheduleEvent(EVENT_3, 2500);
                        break;
                    case EVENT_4:
                        me->RemoveAura(SPELL_PHASE);
                        events.CancelEvent(EVENT_3);
                        events.RescheduleEvent(EVENT_1, 3000);
                        events.RescheduleEvent(EVENT_2, 6000);
                        me->SetReactState(REACT_DEFENSIVE);
                        SetFlyMode(false);
                        me->GetMotionMaster()->MovePoint(1, 592.58f, 2432.88f, -71.28f);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

//242989 244916 242987 242990
class go_q38759 : public GameObjectScript
{
public:
    go_q38759() : GameObjectScript("go_q38759") { }

    struct go_q38759AI : public GameObjectAI
    {
        go_q38759AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 38759,
            CAGE_OPEN_TIME = 35,    // seconds, as long as the freed prisoner stays
        };

        uint32 _closeTimer = 0;
        uint32 _hideTimer = 0;
        ObjectGuid _freer;

        // the opened cage vanishes for its freer while the prisoner walks out, and closes again once the prisoner is gone
        void UpdateAI(uint32 diff) override
        {
            if (_hideTimer)
            {
                if (_hideTimer > diff)
                    _hideTimer -= diff;
                else
                {
                    _hideTimer = 0;
                    if (Player* player = ObjectAccessor::GetPlayer(*go, _freer))
                        go->DestroyForPlayer(player);
                }
            }

            if (!_closeTimer)
                return;

            if (_closeTimer > diff)
            {
                _closeTimer -= diff;
                return;
            }

            _closeTimer = 0;
            go->SetGoState(GO_STATE_READY);
        }

        // the cage has a lock: opening it calls GossipUse, never GossipHello
        bool GossipUse(Player* player) override
        {
            return GossipHello(player, true);
        }

        bool GossipHello(Player* player, bool isUse) override
        {
            if (!isUse)
                return true;

            std::map<uint32, uint32> _data;
            _data[242989] = 94400;
            _data[244916] = 94377;
            _data[242987] = 93117;
            _data[242990] = 93230;

            if (player->GetQuestObjectiveData(QUEST, _data[go->GetEntry()]))
                return true;

            if (Creature *c = player->FindNearestCreature(_data[go->GetEntry()], 10.0f))
            {
                go->SetGoState(GO_STATE_ACTIVE);
                _closeTimer = CAGE_OPEN_TIME * IN_MILLISECONDS;
                _hideTimer = 2 * IN_MILLISECONDS;
                _freer = player->GetGUID();
                player->KilledMonsterCredit(c->GetEntry());
                c->DestroyForPlayer(player);
                if (auto temp = player->SummonCreature(_data[go->GetEntry()], c->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 35000, 0, player->GetGUID()))
                {
                    ObjectGuid playerGuid = player->GetGUID();
                    temp->AddDelayedEvent(500, [temp, playerGuid]()-> void
                    {
                        if (auto player = ObjectAccessor::FindPlayer(playerGuid))
                            temp->AI()->Talk(1, player->GetGUID());

                        // the freed prisoner walks out of the open cage, on the ground in front of it
                        temp->AddDelayedEvent(4000, [temp]() -> void
                        {
                            switch (temp->GetEntry())
                            {
                            case 94377:
                                temp->GetMotionMaster()->MovePoint(1, 770.80f, 2556.10f, -70.33f);
                                break;
                            case 94400:
                                temp->GetMotionMaster()->MovePoint(1, 733.90f, 2457.70f, -63.77f);
                                break;
                            case 93230:
                                temp->GetMotionMaster()->MovePoint(1, 896.88f, 2469.98f, -65.63f);
                                break;
                            case 93117:
                                temp->GetMotionMaster()->MovePoint(1, 870.91f, 2576.41f, -55.96f);
                                break;
                            }
                        });
                    });
                }
            }

            return true;
        }

    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q38759AI(go);
    }
};

//99914
class npc_q40379 : public CreatureScript
{
public:
    npc_q40379() : CreatureScript("npc_q40379") { }

    enum Credit
    {
        QUEST = 40379,
        SPELL = 196724,
    };
    // menu 19015 has a single option, and an option of the database comes with its type (0) as action
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 /*action*/) override
    {
        player->PlayerTalkClass->ClearMenus();
        {
            player->CLOSE_GOSSIP_MENU();
            if (player->GetQuestStatus(QUEST) == QUEST_STATUS_INCOMPLETE && !player->GetReqKillOrCastCurrentCount(QUEST, creature->GetEntry()))
            {
            /*
            ClientToServer: CMSG_CLOSE_INTERACTION(0x348A) Length : 15 ConnIdx : 2 Time : 02 / 22 / 2016 13 : 10 : 10.127 Number : 17187
            Guid : Full : 0x202090B92061928000109C00004AF10C; HighType: Creature; Low: 4911372; Map: 1481; Entry: 99914;
            */
                player->CastSpell(creature, SPELL, false);
                player->KilledMonsterCredit(creature->GetEntry());
                // the player sees a copy of the mystic die where it stood; the others keep seeing the mystic
                if (TempSummon* copy = player->SummonCreature(creature->GetEntry(), creature->GetPosition(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 20000, 0, player->GetGUID()))
                {
                    copy->RemoveFlag64(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    copy->AddDelayedEvent(1500, [copy]() -> void
                    {
                        if (copy->IsAlive())
                            copy->Kill(copy, false);
                    });
                }
                creature->DestroyForPlayer(player);
            }
        }

        return true;
    }
};

class go_q40379 : public GameObjectScript
{
public:
    go_q40379() : GameObjectScript("go_q40379") { }

    struct go_q40379AI : public GameObjectAI
    {
        go_q40379AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST  = 40379,
            SPELL  = 190793,
            CREDIT = 94406,
            CREDIT_REQUARE = 99914,
        };

        // the activator has a lock: opening it calls GossipUse, never GossipHello
        bool GossipUse(Player* player) override
        {
            return GossipHello(player, true);
        }

        bool GossipHello(Player* player, bool isUse) override
        {
            if (!isUse)
                return true;

            if (player->GetReqKillOrCastCurrentCount(QUEST, CREDIT) || !player->GetReqKillOrCastCurrentCount(QUEST, CREDIT_REQUARE))
                return true;

            player->KilledMonsterCredit(CREDIT);
            player->CastSpell(player, SPELL, false);
            player->SummonCreature(99917, 756.76f, 2401.41f, -60.91f, 1.06f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
            //    targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
            return false;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q40379AI(go);
    }
};

//243335
class go_q39050 : public GameObjectScript
{
public:
    go_q39050() : GameObjectScript("go_q39050") { }

    struct go_q39050AI : public GameObjectAI
    {
        go_q39050AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 39050,
            SPELL = 188539,
            CREDIT = 100722,
        };

        // the object has a lock: opening it calls GossipUse, never GossipHello
        bool GossipUse(Player* player) override
        {
            return GossipHello(player, true);
        }

        bool GossipHello(Player* player, bool isUse) override
        {
            if (!isUse)
                return true;

            if (player->GetQuestStatus(QUEST) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(CREDIT);
                player->CastSpell(player, SPELL, false);
            }
            return false;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q39050AI(go);
    }
};

// 99915
class npc_q38765 : public CreatureScript
{
public:
    npc_q38765() : CreatureScript("npc_q38765") { }

    enum Credit
    {
        QUEST = 38765,
        SPELL = 196724,
        SPELL_1= 196731,
    };
    
    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (player->GetQuestStatus(QUEST) == QUEST_STATUS_INCOMPLETE && !player->GetReqKillOrCastCurrentCount(QUEST, creature->GetEntry()))
        {
            player->ADD_GOSSIP_ITEM_DB(19016, 0, GOSSIP_SENDER_MAIN, 2);
            player->ADD_GOSSIP_ITEM_DB(19016, 1, GOSSIP_SENDER_MAIN, 1);
            player->SEND_GOSSIP_MENU(27770, creature->GetGUID());
        }
        return true;
    }
    
    
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        switch(action)
        {
            case 1:
                player->ADD_GOSSIP_ITEM_DB(19132, 0, GOSSIP_SENDER_MAIN, 3);  // меня
                player->ADD_GOSSIP_ITEM_DB(19132, 1, GOSSIP_SENDER_MAIN, 4);
                player->SEND_GOSSIP_MENU(27999, creature->GetGUID());
                break;
            case 2:
                player->ADD_GOSSIP_ITEM_DB(19133, 0, GOSSIP_SENDER_MAIN, 5);  // его
                player->ADD_GOSSIP_ITEM_DB(19133, 1, GOSSIP_SENDER_MAIN, 4);
                player->SEND_GOSSIP_MENU(28000, creature->GetGUID());
                break;
            case 4:
                player->ADD_GOSSIP_ITEM_DB(19016, 0, GOSSIP_SENDER_MAIN, 2);
                player->ADD_GOSSIP_ITEM_DB(19016, 1, GOSSIP_SENDER_MAIN, 1);
                player->SEND_GOSSIP_MENU(27770, creature->GetGUID());
                break;
                
            case 3:
                creature->CastSpell(player, SPELL, false);
                creature->Kill(player);
                player->KilledMonsterCredit(creature->GetEntry());
                player->SummonCreature(99915, creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ(), creature->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
                //    targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                // talk
                player->CLOSE_GOSSIP_MENU();
                break;
            case 5:
                sCreatureTextMgr->SendChat(creature, 1, player->GetGUID());
                player->CastSpell(creature, SPELL_1, false);
                // Talk
                player->KilledMonsterCredit(creature->GetEntry());
                creature->DestroyForPlayer(player);
                player->CLOSE_GOSSIP_MENU();
                break;
            }
        

        return true;
    }
    
    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q38765AI(creature);
    }
    struct npc_q38765AI : public ScriptedAI
    {
        npc_q38765AI(Creature* creature) : ScriptedAI(creature) { }
        
        GuidSet m_player_for_event;
        
        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                me->RemoveAllAuras();
                me->CastSpell(summoner, SPELL, false);
                sCreatureTextMgr->SendChat(me, 2, summoner->GetGUID());
                me->DespawnOrUnsummon(2000);
                me->HandleEmoteCommand(16);
            }
            else 
                me->DespawnOrUnsummon();
        }
        
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 15.0f))
                return;

            GuidSet::iterator itr = m_player_for_event.find(who->GetGUID());
            if (itr != m_player_for_event.end())
                return;

            if (who->ToPlayer()->GetQuestStatus(38765) != QUEST_STATUS_INCOMPLETE)
                return;
           

            m_player_for_event.insert(who->GetGUID());
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, who->GetGUID());
        }
    };
};

//241757
class go_q38765 : public GameObjectScript
{
public:
    go_q38765() : GameObjectScript("go_q38765") { }

    struct go_q38765AI : public GameObjectAI
    {
        go_q38765AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 38765,
            SPELL = 190851,
            CREDIT = 94407,
        };

        // the activator has a lock: opening it calls GossipUse, never GossipHello
        bool GossipUse(Player* player) override
        {
            return GossipHello(player, true);
        }

        bool GossipHello(Player* player, bool isUse) override
        {
            if (!isUse)
                return true;

            if (player->GetReqKillOrCastCurrentCount(QUEST, CREDIT))
                return true;

            player->KilledMonsterCredit(CREDIT);
            player->CastSpell(player, SPELL, false);
            return false;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q38765AI(go);
    }
};

//! 96675
class npc_questgiver_93759 : public CreatureScript
{
public:
    npc_questgiver_93759() : CreatureScript("npc_questgiver_93759") {}
    enum data
    {
        QUEST = 40379,
        QUEST2 = 39050,
        QUEST_03 = 38766,
        CONVERSATION = 735
    };

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_03) // The Lesson of the Burning Scroll
        {
            Conversation* conversation = new Conversation;
            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), CONVERSATION, player, NULL, *player))
                delete conversation;
        }
        else if (quest->GetQuestId() == QUEST2)
        {
            sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_1, player->GetGUID());
        }

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_questgiver_93759AI(creature);
    }
    struct npc_questgiver_93759AI : public ScriptedAI
    {
        npc_questgiver_93759AI(Creature* creature) : ScriptedAI(creature)
        {
            DoCast(188485);
        }
        GuidSet m_player_for_event;
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 20.0f))
                return;

            GuidSet::iterator itr = m_player_for_event.find(who->GetGUID());
            if (itr != m_player_for_event.end())
                return;

            if (who->ToPlayer()->GetQuestStatus(QUEST) != QUEST_STATUS_COMPLETE)
                return;

            m_player_for_event.insert(who->GetGUID());
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, who->GetGUID());
        }
    };
};

// Before We're Overrun: the fire patches of the Spires of Woe, lit by the spires' beams while Doom Commander Beliash fights
enum MardumSpireFires
{
    NPC_SPIRE_OF_WOE_FIRE_A     = 97629,
    NPC_SPIRE_OF_WOE_FIRE_B     = 97634,
    NPC_SPIRE_OF_WOE_TOP        = 97624,
    GO_SPIRE_OF_WOE_A           = 246566,   // Beliash's two spires
    GO_SPIRE_OF_WOE_B           = 246567,
    NPC_SOUL_ENGINE_FIRE_A      = 101318,
    NPC_SOUL_ENGINE_FIRE_B      = 101321,
    NPC_SOUL_ENGINE_SPIRE_TOP   = 101320,
    GO_SOUL_ENGINE_SPIRE_A      = 246485,   // the two spires on the way to the Soul Engine, burning all the time
    GO_SOUL_ENGINE_SPIRE_B      = 246486,
    ACTION_SPIRE_FIRES_ON       = 1,
    ACTION_SPIRE_FIRES_OFF      = 2,
};

static uint32 MardumSpireOfFire(uint32 fireEntry)
{
    switch (fireEntry)
    {
        case NPC_SPIRE_OF_WOE_FIRE_A: return GO_SPIRE_OF_WOE_A;
        case NPC_SPIRE_OF_WOE_FIRE_B: return GO_SPIRE_OF_WOE_B;
        case NPC_SOUL_ENGINE_FIRE_A:  return GO_SOUL_ENGINE_SPIRE_A;
        case NPC_SOUL_ENGINE_FIRE_B:  return GO_SOUL_ENGINE_SPIRE_B;
        default:                      return 0;
    }
}

static uint32 MardumFireOfSpire(uint32 spireEntry)
{
    switch (spireEntry)
    {
        case GO_SPIRE_OF_WOE_A:      return NPC_SPIRE_OF_WOE_FIRE_A;
        case GO_SPIRE_OF_WOE_B:      return NPC_SPIRE_OF_WOE_FIRE_B;
        case GO_SOUL_ENGINE_SPIRE_A: return NPC_SOUL_ENGINE_FIRE_A;
        case GO_SOUL_ENGINE_SPIRE_B: return NPC_SOUL_ENGINE_FIRE_B;
        default:                     return 0;
    }
}

// the crystal at the top of a spire, which casts the beams
static Creature* MardumSpireTop(GameObject* spire)
{
    if (!spire)
        return nullptr;
    if (Creature* top = spire->FindNearestCreature(NPC_SPIRE_OF_WOE_TOP, 35.0f))
        return top;
    return spire->FindNearestCreature(NPC_SOUL_ENGINE_SPIRE_TOP, 35.0f);
}

static void SetMardumSpireFires(Creature* source, int32 action)
{
    std::list<Creature*> fires;
    source->GetCreatureListWithEntryInGrid(fires, NPC_SPIRE_OF_WOE_FIRE_A, 80.0f);
    source->GetCreatureListWithEntryInGridAppend(fires, NPC_SPIRE_OF_WOE_FIRE_B, 80.0f);
    for (Creature* fire : fires)
        if (fire->IsAlive() && fire->AI())
            fire->AI()->DoAction(action);
}

//93221 Doom Commander Beliash
class npc_q93221_beliash : public CreatureScript
{
public:
    npc_q93221_beliash() : CreatureScript("npc_q93221_beliash") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q93221_beliashAI(creature);
    }

    struct npc_q93221_beliashAI : public ScriptedAI
    {
        EventMap events;
        ObjectGuid playerGuid;
        ObjectGuid TyrannaGuid;

        npc_q93221_beliashAI(Creature* creature) : ScriptedAI(creature)
        {
            creature->SetCorpseDelay(30);
            creature->SetRespawnDelay(15);
        }

        enum data
        {
            QUEST           = 38766,
            SPELL_AT_DEATH  = 210093,
            CREDIT          = 106003,
            SPELL_TYRANNA_TELEPORT = 188658,    // her invisibility: she leaves when the fight begins
            SPELL_SHADOW_BLAZE_ZONE = 195402,   // Shadow Retreat's zone of Shadow Blaze, laid at a point
            POINT_SHADOW_RETREAT    = 1,
            SPELL_SHADOW_BOLT_VOLLEY = 196677,
        };
        
        void Reset() override
        {
            events.Reset();
            me->RemoveAllAreaObjects();
            // Reset also runs at spawn and after every evade: one Tyranna at a time
            Creature* tyranna = ObjectAccessor::GetCreature(*me, TyrannaGuid);
            if (!tyranna || !tyranna->IsAlive())
                if (Creature* Tyranna = me->SummonCreature(95048, 1599.33f, 2704.51f, 19.23f, 2.53f))
                    TyrannaGuid = Tyranna->GetGUID();
        }

        void EnterCombat(Unit* victim) override
        {
            if (victim->GetTypeId() != TYPEID_PLAYER)
                return;

            if (Creature* Tyranna = ObjectAccessor::GetCreature(*me, TyrannaGuid))
            {
                if (Tyranna->AI())
                    Tyranna->AI()->Talk(TEXT_GENERIC_0, victim->GetGUID());
                // her line said, she vanishes into the shadows
                Tyranna->AddDelayedEvent(3000, [Tyranna]() -> void
                {
                    Tyranna->CastSpell(Tyranna, SPELL_TYRANNA_TELEPORT, true);
                });
                Tyranna->DespawnOrUnsummon(5000);
                TyrannaGuid = ObjectGuid::Empty;
            }
            
            events.RescheduleEvent(EVENT_1, 3000);
            events.RescheduleEvent(EVENT_2, 2000);
            events.RescheduleEvent(EVENT_3, 8000);
            SetMardumSpireFires(me, ACTION_SPIRE_FIRES_ON);
        }

        // Shadow Retreat: leaps back away from its foe and leaves Shadow Blaze along the whole leap
        void ShadowRetreat()
        {
            Unit* victim = me->getVictim();
            if (!victim)
                return;

            float const distance = 15.0f;
            float const speedXY = 20.0f;
            float const step = 3.0f;    // the zones are 3 yards wide

            Position start = me->GetPosition();
            float away = victim->GetAngle(me);
            Position landing = me->GetFirstCollisionPosition(distance, away - me->GetOrientation());
            float landingZ = landing.GetPositionZ();
            me->UpdateGroundPositionZ(landing.GetPositionX(), landing.GetPositionY(), landingZ);
            landing.Relocate(landing.GetPositionX(), landing.GetPositionY(), landingZ);
            float length = start.GetExactDist2d(landing);
            if (length < 2.0f * step)   // backed against a wall
                return;

            me->SetFacingToObject(victim);
            me->GetMotionMaster()->MoveJump(landing, speedXY, 10.0f, POINT_SHADOW_RETREAT);

            uint32 steps = uint32(length / step);
            uint32 flight = uint32(length / speedXY * IN_MILLISECONDS);
            Creature* self = me;
            for (uint32 i = 0; i <= steps; ++i)
            {
                float f = float(i) / float(steps);
                float x = start.GetPositionX() + (landing.GetPositionX() - start.GetPositionX()) * f;
                float y = start.GetPositionY() + (landing.GetPositionY() - start.GetPositionY()) * f;
                float z = start.GetPositionZ() + (landing.GetPositionZ() - start.GetPositionZ()) * f;
                me->UpdateGroundPositionZ(x, y, z);
                me->AddDelayedEvent(flight * i / steps + 1, [self, x, y, z]() -> void
                {
                    self->CastSpell(x, y, z, SPELL_SHADOW_BLAZE_ZONE, true);
                });
            }
        }

        // landed: a Shadow Bolt Volley at once, then back after its foe, drawing it through the trail
        void MovementInform(uint32 type, uint32 id) override
        {
            if (type != EFFECT_MOTION_TYPE || id != POINT_SHADOW_RETREAT || !me->isInCombat())
                return;

            DoCast(SPELL_SHADOW_BOLT_VOLLEY);
            events.RescheduleEvent(EVENT_1, 17000);
        }

        void EnterEvadeMode() override
        {
            me->RemoveAllAreaObjects();
            SetMardumSpireFires(me, ACTION_SPIRE_FIRES_OFF);
            ScriptedAI::EnterEvadeMode();
        }

        void JustDied(Unit* killer) override
        {
            SetMardumSpireFires(me, ACTION_SPIRE_FIRES_OFF);
            if (Creature* Tyranna = ObjectAccessor::GetCreature(*me, TyrannaGuid))
            {
                Tyranna->DespawnOrUnsummon(500);
                TyrannaGuid = ObjectGuid::Empty;
            }
            Player* player = killer->ToPlayer();
            if (!player)
                return;

            Talk(TEXT_GENERIC_1, player->GetGUID());

            std::list<Player*> playerList;
            me->GetPlayerListInGrid(playerList, 60.0f);
            for (auto plr : playerList)
            {
                plr->KilledMonsterCredit(CREDIT);
                plr->CastSpell(plr, SPELL_AT_DEATH, true);
            }
        }
        
        // to-do
        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                if (eventId == EVENT_1)
                {
                    DoCast(196677);
                    events.RescheduleEvent(EVENT_1, 17000);
                }
                if (eventId == EVENT_2)
                    Talk(0);
                if (eventId == EVENT_3)
                {
                    ShadowRetreat();
                    events.RescheduleEvent(EVENT_3, urand(12000, 16000));
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

//96441 Fel Lord Caza
class npc_q39495_caza : public CreatureScript
{
public:
    npc_q39495_caza() : CreatureScript("npc_q39495_caza") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q39495_cazaAI(creature);
    }

    struct npc_q39495_cazaAI : public ScriptedAI
    {
        EventMap events;
        ObjectGuid playerGuid;

        npc_q39495_cazaAI(Creature* creature) : ScriptedAI(creature)
        {
            // the corpse stays a minute to be looted, he is back 15 seconds after it is gone
            creature->SetCorpseDelay(60);
            creature->SetRespawnDelay(15);
        }

        enum data
        {
            QUEST = 39495,
            SPELL_AT_DEATH = 210107,
            CREDIT = 106014,
            SPELL_1 = 197002, // 7
            SPELL_2 = 196876, // 16 (+ talk)
            SPELL_3 = 197180,
            SPELL_DISARMED = 196892,        // Throw Axe leaves him without his axe for 10 seconds
            SPELL_THROW_AXE_KNOCKBACK = 196955, // the axe hits and throws back whoever stands in its way
            NPC_AXE_THROW_STALKER = 100061, // spots on the cave walls
            NPC_FEL_AXE = 100062,           // where the axe lands, burning with Violent Fel Energy
            EVENT_RETRIEVE_AXE = 10,
            EVENT_THROW_AXE = 11,
            EVENT_LEAVE_WALL = 12,
            POINT_WALL_LEAP = 1,
        };

        // the stalkers sit on the rock itself: he lands at the foot of the wall below each of them, with room for his size
        static std::vector<Position> const& WallLandings()
        {
            static std::vector<Position> const landings =
            {
                { 1120.54f, 1620.57f, 116.90f },
                { 1107.69f, 1629.30f, 116.91f },
                { 1121.61f, 1607.71f, 118.69f },
                { 1103.06f, 1630.86f, 117.10f },
                { 1088.95f, 1621.75f, 116.96f },
                { 1087.33f, 1613.81f, 119.25f },
                { 1091.20f, 1602.28f, 117.22f },
            };
            return landings;
        }

        ObjectGuid _axe;
        uint32 _axeItem = 0;
        Position const* _lastWall = nullptr;
Position _throwFrom;
        Position _throwTo;

        void EnterCombat(Unit* victim) override
        {
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, victim->GetGUID());
            events.RescheduleEvent(EVENT_1, 7000);
            events.RescheduleEvent(EVENT_2, 15000);
            events.RescheduleEvent(EVENT_3, 9000);

            if (auto dh = me->FindNearestCreature(101790, 20.0f, true))
                dh->AI()->AttackStart(me);
        }

        void Reset() override
        {
            events.Reset();
            DropAxe();
            me->SetControlled(false, UNIT_STATE_ROOT);
        }

        // the axe left on the ground goes, and he holds it again
        void DropAxe()
        {
            if (Creature* axe = ObjectAccessor::GetCreature(*me, _axe))
                axe->DespawnOrUnsummon();
            _axe.Clear();
            me->RemoveAurasDueToSpell(SPELL_DISARMED);
            if (_axeItem)
            {
                me->SetVirtualItem(0, _axeItem);
                _axeItem = 0;
            }
        }

        // the axe stays stuck where it landed: it only burns around it
        void JustSummoned(Creature* summon) override
        {
            if (summon->GetEntry() != NPC_FEL_AXE)
                return;

            _axe = summon->GetGUID();
            summon->SetReactState(REACT_PASSIVE);
            summon->AttackStop();
            summon->SetControlled(true, UNIT_STATE_ROOT);
        }

        // Throw Axe: he leaps to the nearest wall, stands there, then throws his axe at the far side of the cave
        void LeapToWall()
        {
            // any wall within reach, never the same twice in a row
            std::vector<Position const*> walls;
            Position const* nearest = nullptr;
            for (Position const& pos : WallLandings())
            {
                if (me->GetDistance(pos) <= 5.0f || &pos == _lastWall)
                    continue;
                if (me->GetDistance(pos) <= 30.0f)
                    walls.push_back(&pos);
                if (!nearest || me->GetDistance(pos) < me->GetDistance(*nearest))
                    nearest = &pos;
            }
            Position const* landing = walls.empty() ? nearest : Trinity::Containers::SelectRandomContainerElement(walls);
            if (!landing)
                return;
            _lastWall = landing;

            Talk(1);
            me->RemoveAurasDueToSpell(SPELL_3);
            // nothing else while he leaps, waits and throws
            events.DelayEvents(5000);
            me->GetMotionMaster()->MoveJump(*landing, 20.0f, 10.0f, POINT_WALL_LEAP);
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type != EFFECT_MOTION_TYPE || id != POINT_WALL_LEAP)
                return;

            // he stays where he landed until the axe has left his hand
            me->SetControlled(true, UNIT_STATE_ROOT);
            events.RescheduleEvent(EVENT_THROW_AXE, 1500);
            events.RescheduleEvent(EVENT_LEAVE_WALL, 5000);
        }

        void ThrowAxe()
        {
            std::list<Creature*> stalkers;
            GetCreatureListWithEntryInGrid(stalkers, me, NPC_AXE_THROW_STALKER, 60.0f);
            std::vector<Creature*> across;
            Creature* farthest = nullptr;
            for (Creature* stalker : stalkers)
            {
                if (me->GetDistance(stalker) > 25.0f)
                    across.push_back(stalker);
                if (!farthest || me->GetDistance(stalker) > me->GetDistance(farthest))
                    farthest = stalker;
            }
            Creature* target = across.empty() ? farthest : Trinity::Containers::SelectRandomContainerElement(across);
            if (!target)
                return;

            _throwFrom = me->GetPosition();
            _throwTo = target->GetPosition();
            me->SetFacingToObject(target);
            // 2 seconds to step out of the way
            me->CastSpell(_throwTo.GetPositionX(), _throwTo.GetPositionY(), _throwTo.GetPositionZ(), SPELL_2, false);
        }

        void SpellFinishCast(SpellInfo const* spell) override
        {
            if (spell->Id != SPELL_2)
                return;

            me->SetControlled(false, UNIT_STATE_ROOT);
            SweepAxePath();

            // Disarmed would leave his blows at 1 to 3 damage: he only shows his empty hand
            if (!_axeItem)
                _axeItem = me->GetVirtualItemId(0);
            me->SetVirtualItem(0, 0);
            Creature* self = me;
            me->AddDelayedEvent(200, [self]() -> void
            {
                self->RemoveAurasDueToSpell(SPELL_DISARMED);
            });
            events.RescheduleEvent(EVENT_RETRIEVE_AXE, 10000);
        }

        // the axe flies at the speed of its missile: whoever it crosses is hit, once
        void SweepAxePath()
        {
            float const speed = 40.0f;
            float const step = 2.0f;
            float const reach = 3.0f;

            float length = _throwFrom.GetExactDist2d(&_throwTo);
            uint32 steps = std::max<uint32>(1, uint32(length / step));
            auto hit = std::make_shared<GuidSet>();
            Creature* self = me;
            for (uint32 i = 0; i <= steps; ++i)
            {
                float f = float(i) / float(steps);
                float x = _throwFrom.GetPositionX() + (_throwTo.GetPositionX() - _throwFrom.GetPositionX()) * f;
                float y = _throwFrom.GetPositionY() + (_throwTo.GetPositionY() - _throwFrom.GetPositionY()) * f;
                float z = _throwFrom.GetPositionZ() + (_throwTo.GetPositionZ() - _throwFrom.GetPositionZ()) * f;
                me->AddDelayedEvent(uint32(length * f / speed * IN_MILLISECONDS) + 1, [self, x, y, z, reach, hit]() -> void
                {
                    for (HostileReference* ref : self->getThreatManager().getThreatList())
                    {
                        Player* player = ObjectAccessor::GetPlayer(*self, ref->getUnitGuid());
                        if (!player || !player->IsAlive() || hit->count(player->GetGUID()))
                            continue;
                        // the throw goes from wall to wall, above the cave floor
                        if (player->GetExactDist2d(x, y) > reach || std::fabs(player->GetPositionZ() - z) > 8.0f)
                            continue;
                        hit->insert(player->GetGUID());
                        self->CastSpell(player, SPELL_THROW_AXE_KNOCKBACK, true);
                    }
                });
            }
        }

        void JustDied(Unit* killer) override
        {
            DropAxe();
            Player *player = killer->ToPlayer();
            if (!player)
                return;
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_2, player->GetGUID());
            player->KilledMonsterCredit(CREDIT);
            player->CastSpell(player, SPELL_AT_DEATH, false);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_1);
                        events.RescheduleEvent(EVENT_1, 12000);
                        break;
                    case EVENT_2:
                        LeapToWall();
                        events.RescheduleEvent(EVENT_2, 23000);
                        break;
                    case EVENT_THROW_AXE:
                        ThrowAxe();
                        break;
                    case EVENT_LEAVE_WALL:
                        // the throw was interrupted: he does not stay stuck to the wall
                        me->SetControlled(false, UNIT_STATE_ROOT);
                        break;
                    case EVENT_RETRIEVE_AXE:
                        // the axe comes back to him when the disarm ends
                        if (_axe.IsEmpty())
                            if (Creature* axe = me->FindNearestCreature(NPC_FEL_AXE, 60.0f))
                                _axe = axe->GetGUID();
                        DropAxe();
                        break;
                    case EVENT_3:
                        DoCast(SPELL_3);
                        events.RescheduleEvent(EVENT_3, 9000);
                        break;
                }                        
            }
            DoMeleeAttackIfReady();
        }
    };
};

// 243965 243968 243967
// Q: 38727
// Stop the Bombardment: the allies a banner calls come forward to their devastator and stop short of it, then channel on it;
// without a spell they only slip up to it
static void MardumBannerAlly(Creature* ally, Creature* devastator, float stopDistance, uint32 channelSpell, uint32 channelVisual, uint32 channelTime = 0)
{
    if (!devastator)
        return;

    float const step = std::max(0.0f, ally->GetDistance2d(devastator) - stopDistance);
    if (step > 1.0f)
    {
        Position const dest = ally->GetFirstCollisionPosition(step, ally->GetAngle(devastator) - ally->GetOrientation());
        ally->SetWalk(false);
        ally->GetMotionMaster()->MovePoint(0, dest);
    }

    ObjectGuid const devastatorGuid = devastator->GetGUID();
    uint32 const arrival = step > 1.0f ? uint32(step / 7.0f * IN_MILLISECONDS) + 600 : 300;
    ally->AddDelayedEvent(arrival, [ally, devastatorGuid, channelSpell, channelVisual]() -> void
    {
        Creature* target = ObjectAccessor::GetCreature(*ally, devastatorGuid);
        if (!target)
            return;
        ally->StopMoving();
        ally->SetFacingToObject(target);
        if (!channelSpell)
            return;
        ally->AddChannelObject(devastatorGuid);
        ally->SetChannelSpellId(channelSpell);
        ally->SetChannelSpellXSpellVisualId(channelVisual);
        ally->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_SPELL_CHANNEL_DIRECTED);    // the beam alone leaves them standing idle
    });

    if (channelSpell && channelTime > arrival)
        ally->AddDelayedEvent(channelTime, [ally]() -> void
        {
            ally->ClearDynamicValue(UNIT_DYNAMIC_FIELD_CHANNEL_OBJECTS);
            ally->SetChannelSpellId(0);
            ally->SetChannelSpellXSpellVisualId(0);
            ally->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
        });
}

// A devastator blown up: shells burst all over its hull, then it falls. The Fel Explosion visual (51680) is not drawn on its own,
// the impact of the devastator's own shell is
static void MardumDevastatorBlast(Creature* devastator)
{
    static float const bursts[][3] = { { 0.0f, 0.0f, 9.0f }, { 6.0f, 1.6f, 6.0f }, { 5.0f, 4.2f, 12.0f }, { 7.0f, 3.1f, 3.0f } };
    std::list<Player*> around;
    devastator->GetPlayerListInGrid(around, 533.0f);
    std::vector<ObjectGuid> players;
    for (Player* player : around)
        players.push_back(player->GetGUID());

    uint32 delay = 0;
    for (auto const& burst : bursts)
    {
        float const angle = devastator->GetOrientation() + burst[1];
        Position const target(devastator->GetPositionX() + burst[0] * std::cos(angle), devastator->GetPositionY() + burst[0] * std::sin(angle), devastator->GetPositionZ() + burst[2]);
        devastator->AddDelayedEvent(delay + 1, [devastator, target, players]() -> void
        {
            WorldPackets::Spells::PlayOrphanSpellVisual shell;
            shell.SourceLocation = Position(target.GetPositionX(), target.GetPositionY(), target.GetPositionZ() + 4.0f);
            shell.SourceRotation = Position(0.0f, 0.0f, 0.0f);
            shell.TargetLocation = target;
            shell.SpellVisualID = 49571;
            shell.TravelSpeed = 40.0f;
            shell.UnkFloat = 0.0f;
            shell.SpeedAsTime = false;
            WorldPacket const* data = shell.Write();
            for (ObjectGuid const& guid : players)
                if (Player* player = ObjectAccessor::GetPlayer(*devastator, guid))
                    if (player->InSamePhase(devastator))
                        player->SendDirectMessage(data);
        });
        delay += 350;
    }
    devastator->AddDelayedEvent(delay + 200, [devastator]() -> void
    {
        if (devastator->IsAlive())
            devastator->Kill(devastator, false);
    });
}

// Stop the Bombardment, per player: what the banners hid for him and what they brought; abandoning the quest puts it all back
struct MardumBombardmentState
{
    GuidSet hidden;
    GuidSet brought;
};
static std::map<ObjectGuid, MardumBombardmentState> sMardumBombardment;

static void MardumBombardmentReset(Player* player)
{
    auto itr = sMardumBombardment.find(player->GetGUID());
    if (itr == sMardumBombardment.end())
        return;

    Map* map = player->GetMap();
    for (ObjectGuid const& guid : itr->second.brought)
    {
        if (guid.IsGameObject())
        {
            if (GameObject* object = map->GetGameObject(guid))
            {
                object->SetRespawnTime(0);
                object->Delete();
            }
        }
        else if (Creature* creature = map->GetCreature(guid))
            creature->DespawnOrUnsummon();
    }
    for (ObjectGuid const& guid : itr->second.hidden)
    {
        WorldObject* object = guid.IsGameObject() ? static_cast<WorldObject*>(map->GetGameObject(guid)) : map->GetCreature(guid);
        if (object)
            object->RemoveFromHideList(player->GetGUID());
    }
    sMardumBombardment.erase(itr);
    player->UpdateObjectVisibility();
}

class player_mardum_stop_the_bombardment : public PlayerScript
{
public:
    player_mardum_stop_the_bombardment() : PlayerScript("player_mardum_stop_the_bombardment") { }

    void OnQuestAbandon(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == 38727)
            MardumBombardmentReset(player);
    }
};

class go_q38727 : public GameObjectScript
{
public:
    go_q38727() : GameObjectScript("go_q38727") { }
    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q38727AI(go);
    }

    struct go_q38727AI : public GameObjectAI
    {
        go_q38727AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 38727,
            CREDIT = 94407,
        };

        // the object has a lock: opening it calls GossipUse, never GossipHello
        bool GossipUse(Player* player) override
        {
            return GossipHello(player, true);
        }

        bool GossipHello(Player* player, bool isUse) override
        {
            if (!isUse)
                return true;

            // only for the quest
            if (player->GetQuestStatus(QUEST) != QUEST_STATUS_INCOMPLETE)
                return true;

            uint32 credit1 = 0;
            uint32 credit2 = 0;

            switch (go->GetEntry())
            {
                case 243965:
                {
                    credit1 = 93762;
                    credit2 = 96692;
                    if (player->GetQuestObjectiveData(QUEST, credit1))
                        return true;
                    
                    if (Creature* t = go->FindNearestCreature(93762, 50.0f))
                    {
                        t->AddToHideList(player->GetGUID());
                        t->DestroyForPlayer(player);
                        sMardumBombardment[player->GetGUID()].hidden.insert(t->GetGUID());
                    }
                    
                    // it stays there, frozen, for the player who froze it
                    Creature* devastator = player->SummonCreature(93762, 1800.39f, 1569.82f, 87.04f, 2.61f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
                    if (devastator)
                        sMardumBombardment[player->GetGUID()].brought.insert(devastator->GetGUID());

                    // the sea-callers called by the banner stand where they appear and freeze the devastator with a Frost Beam
                    // (191555, its target is a nearby creature: the beam is shown, the devastator freezes itself)
                    auto frostBeam = [devastator](Creature* caller)
                    {
                        if (!devastator)
                            return;

                        // they come forward to some 15 yards of it, then channel
                        float const step = std::min(12.0f, std::max(0.0f, caller->GetDistance2d(devastator) - 15.0f));
                        if (step > 1.0f)
                        {
                            Position const dest = caller->GetFirstCollisionPosition(step, caller->GetAngle(devastator) - caller->GetOrientation());
                            caller->SetWalk(false);
                            caller->GetMotionMaster()->MovePoint(0, dest);
                        }

                        ObjectGuid const devastatorGuid = devastator->GetGUID();
                        caller->AddDelayedEvent(step > 1.0f ? uint32(step / 7.0f * IN_MILLISECONDS) + 600 : 300, [caller, devastatorGuid]() -> void
                        {
                            Creature* target = ObjectAccessor::GetCreature(*caller, devastatorGuid);
                            if (!target)
                                return;
                            caller->StopMoving();
                            caller->SetFacingToObject(target);
                            caller->AddChannelObject(devastatorGuid);
                            caller->SetChannelSpellId(191555);
                            caller->SetChannelSpellXSpellVisualId(87892);
                            caller->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_SPELL_CHANNEL_DIRECTED);
                        });
                    };

                    if (Creature* targ = go->SummonCreature(96503, 1838.30f, 1527.21f, 87.45f, 2.20f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                        frostBeam(targ);
                        targ->DespawnOrUnsummon(14000);
                    }
                    if (Creature* targ = go->SummonCreature(96561, 1850.74f, 1533.68f, 91.67f, 2.45f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                       // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                        targ->DespawnOrUnsummon(10000);
                    }
                    if (Creature* targ = go->SummonCreature(96503, 1856.08f, 1554.44f, 94.00f, 2.88f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                        frostBeam(targ);
                        targ->DespawnOrUnsummon(14000);
                    }
                    if (Creature* targ = go->SummonCreature(96503, 1848.49f, 1579.76f, 89.97f, 3.55f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                        frostBeam(targ);
                        targ->DespawnOrUnsummon(14000);
                    }
                    break;
                }
                case 243968:
                {
                    credit1 = 96732;
                    credit2 = 96734;
                    if (player->GetQuestObjectiveData(QUEST, credit1))
                        return true;
                    
                    if (Creature* t = go->FindNearestCreature(96732, 50.0f))
                    {
                        t->AddToHideList(player->GetGUID());
                        t->DestroyForPlayer(player);
                        sMardumBombardment[player->GetGUID()].hidden.insert(t->GetGUID());
                    }
                    
                    // the Ashtongue stalkers come in camouflaged and slip up to the devastator, plant a charge at its foot,
                    // and it blows up when the charge goes off
                    Creature* devastator = player->SummonCreature(96732, 1382.39f, 1452.39f, 37.0f, 1.22061f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
                    if (devastator)
                        sMardumBombardment[player->GetGUID()].brought.insert(devastator->GetGUID());
                    if (devastator)
                    {
                        // the charge is planted once the stalkers are there, and goes off 5.5 seconds later (npc_q38727 blows the devastator)
                        devastator->DespawnOrUnsummon(17000);
                        Creature* self = devastator;
                        ObjectGuid const viewer = player->GetGUID();
                        devastator->AddDelayedEvent(7500, [self, viewer]() -> void
                        {
                            float const o = self->GetOrientation();
                            float const x = self->GetPositionX() + 3.0f * std::cos(o);
                            float const y = self->GetPositionY() + 3.0f * std::sin(o);
                            float z = self->GetPositionZ();
                            self->UpdateGroundPositionZ(x, y, z);
                            if (Creature* charge = self->SummonCreature(93199, x, y, z, o, TEMPSUMMON_TIMED_DESPAWN, 5500, viewer))
                            {
                                charge->SetReactState(REACT_PASSIVE);
                                charge->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                                // the countdown read over the charge until it goes off with the devastator
                                for (uint32 second = 5; second; --second)
                                {
                                    Creature* bomb = charge;
                                    charge->AddDelayedEvent(500 + (5 - second) * 1000, [bomb, second]() -> void
                                    {
                                        std::string const count = std::to_string(second) + "...";
                                        bomb->MonsterSay(count.c_str(), LANG_UNIVERSAL, ObjectGuid::Empty);
                                    });
                                }
                            }
                        });
                    }
                    if (Creature* targ = player->SummonCreature(96562, 1342.57f, 1421.92f, 39.34f, 0.33f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                       // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                        MardumBannerAlly(targ, devastator, 4.0f, 0, 0);
                        Position const start = targ->GetPosition();
                        Creature* stalker = targ;
                        targ->AddDelayedEvent(6000, [stalker]() -> void
                        {
                            stalker->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_USE_STANDING);
                        });
                        targ->AddDelayedEvent(8000, [stalker, start]() -> void
                        {
                            stalker->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                            stalker->SetWalk(false);
                            stalker->GetMotionMaster()->MovePoint(0, start);
                        });
                        targ->DespawnOrUnsummon(14000);
                    }
                    if (Creature* targ = player->SummonCreature(96562, 1350.38f, 1410.88f, 39.14f, 0.87f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                      //  targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                        MardumBannerAlly(targ, devastator, 4.0f, 0, 0);
                        Position const start = targ->GetPosition();
                        Creature* stalker = targ;
                        targ->AddDelayedEvent(6000, [stalker]() -> void
                        {
                            stalker->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_USE_STANDING);
                        });
                        targ->AddDelayedEvent(8000, [stalker, start]() -> void
                        {
                            stalker->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                            stalker->SetWalk(false);
                            stalker->GetMotionMaster()->MovePoint(0, start);
                        });
                        targ->DespawnOrUnsummon(14000);
                    }
                    if (Creature* targ = player->SummonCreature(96562, 1354.43f, 1402.99f, 39.50f, 0.96f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                       // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                        MardumBannerAlly(targ, devastator, 4.0f, 0, 0);
                        Position const start = targ->GetPosition();
                        Creature* stalker = targ;
                        targ->AddDelayedEvent(6000, [stalker]() -> void
                        {
                            stalker->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_USE_STANDING);
                        });
                        targ->AddDelayedEvent(8000, [stalker, start]() -> void
                        {
                            stalker->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                            stalker->SetWalk(false);
                            stalker->GetMotionMaster()->MovePoint(0, start);
                        });
                        targ->DespawnOrUnsummon(14000);
                    }
                }
                    break;
                case 243967:
                {
                    credit1 = 96731;
                    credit2 = 96733;
                    
                    if (player->GetQuestObjectiveData(QUEST, credit1))
                        return true;
                    
                    if (Creature* t = go->FindNearestCreature(96731, 100.0f))
                    {
                        t->AddToHideList(player->GetGUID());
                        t->DestroyForPlayer(player);
                        sMardumBombardment[player->GetGUID()].hidden.insert(t->GetGUID());
                    }
                    
                    // the Shivarra priestesses come forward and hold the devastator in an arcane channel
                    Creature* devastator = player->SummonCreature(96731, 1524.58f, 1248.48f, 70.8699f, 1.72f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
                    if (devastator)
                        sMardumBombardment[player->GetGUID()].brought.insert(devastator->GetGUID());
                    if (devastator)
                    {
                        // the channel over, it blows up (seen only: the spell itself would kill everyone around)
                        Creature* self = devastator;
                        devastator->AddDelayedEvent(9500, [self]() -> void
                        {
                            MardumDevastatorBlast(self);
                        });
                        devastator->DespawnOrUnsummon(14500);
                    }
                    if (Creature* targ = player->SummonCreature(96563, 1538.09f, 1212.63f, 71.13f, 2.07f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                       // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                        MardumBannerAlly(targ, devastator, 15.0f, 191537, 87887, 9000);
                        targ->DespawnOrUnsummon(14000);
                    }
                    if (Creature* targ = player->SummonCreature(96564, 1524.91f, 1212.91f, 71.02f, 1.38f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
                    {
                       // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                        MardumBannerAlly(targ, devastator, 15.0f, 191537, 87887, 9000);
                        targ->DespawnOrUnsummon(14000);
                    }
                }
                    break;
            }

            player->KilledMonsterCredit(credit1);
            player->KilledMonsterCredit(credit2); //Hack. Special event.

            // planted: for this player the outline gives way to the banner itself (243013), which stays
            go->AddToHideList(player->GetGUID());
            go->DestroyForPlayer(player);
            MardumBombardmentState& state = sMardumBombardment[player->GetGUID()];
            state.hidden.insert(go->GetGUID());
            if (GameObject* banner = go->SummonGameObject(243013, go->GetPosition(), go->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, DAY, player->GetGUID()))
                state.brought.insert(banner->GetGUID());
            return false;
        }
    };
};

//103429
class npc_q40222_progres1 : public CreatureScript
{
public:
    npc_q40222_progres1() : CreatureScript("npc_q40222_progres1") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q40222_progres1AI(creature);
    }

    struct npc_q40222_progres1AI : public ScriptedAI
    {
        npc_q40222_progres1AI(Creature* creature) : ScriptedAI(creature)
        {
            SetCanSeeEvenInPassiveMode(true);
        }

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);
        }


        enum data
        {
            CREDIT = 103429,
            QUEST = 40222,
        };
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 50.0f))
                return;

            if (who->ToPlayer()->GetQuestObjectiveData(QUEST, CREDIT))
                return;

            who->ToPlayer()->KilledMonsterCredit(CREDIT);
        }
    };
};
//98986 Prolifica
class npc_q39495_prolifica : public CreatureScript
{
public:
    npc_q39495_prolifica() : CreatureScript("npc_q39495_prolifica") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q39495_prolificaAI(creature);
    }

    //! Scripted_NoMovementAI HACK!
    struct npc_q39495_prolificaAI : public Scripted_NoMovementAI
    {
        EventMap events;
        ObjectGuid playerGuid;

        npc_q39495_prolificaAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            creature->SetCorpseDelay(30);
            creature->SetRespawnDelay(15);
            if(GameObject* gob = me->FindNearestGameObject(GO, 100.0f))
                gob->SetPhaseMask(1, true);  // invis
        }

        enum data
        {
            GO = 245169,
            SPELL_1 = 197217,
            SPELL_2 = 197240,
        };
        
        void Reset() override
        {
            events.Reset();
            if(GameObject* gob = me->FindNearestGameObject(GO, 100.0f))
                gob->SetPhaseMask(1, true);  // invis
        }

        void EnterCombat(Unit* victim) override
        {
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, victim->GetGUID());
            if(GameObject* gob = me->FindNearestGameObject(GO, 100.0f))
                gob->SetPhaseMask(2, true);  // invis
            events.RescheduleEvent(EVENT_1, 13000);
            events.RescheduleEvent(EVENT_2, 5000); // 20
        }

        void JustDied(Unit* killer) override
        {
            Player *pl = killer->ToPlayer();
            if (!pl)
                return;
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_2, pl->GetGUID());
            
            Conversation* conversation = new Conversation;
            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), 585, pl, NULL, *pl))
                delete conversation;
            if (Creature* tr = pl->SummonCreature(103432, 1807.34f, 1245.43f, 87.27f, 4.85f, TEMPSUMMON_MANUAL_DESPAWN, 0, pl->GetGUID(), NULL))
            {
              //  tr->AddPlayerInPersonnalVisibilityList(pl->GetGUID());
                tr->GetMotionMaster()->MovePoint(0, 1814.97f, 1211.79f, 83.19f);
            }
            if (Creature* tr = pl->SummonCreature(103432, 1805.83f, 1241.35f, 87.03f, 4.85f, TEMPSUMMON_MANUAL_DESPAWN, 0, pl->GetGUID(), NULL))
            {
              //  tr->AddPlayerInPersonnalVisibilityList(pl->GetGUID());
                tr->GetMotionMaster()->MovePoint(0, 1808.91f, 1204.31f, 82.97f);
            }
            if (Creature* tr = pl->SummonCreature(96278, 1809.17f, 1238.59f, 85.88f, 4.85f, TEMPSUMMON_MANUAL_DESPAWN, 0, pl->GetGUID(), NULL))
            {
              //  tr->AddPlayerInPersonnalVisibilityList(pl->GetGUID());
                tr->GetMotionMaster()->MovePoint(0, 1811.69f, 1195.42f, 80.83f);
            }

        }
        
        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_1);
                        events.RescheduleEvent(EVENT_1, 13000);
                        break;
                    case EVENT_2:
                        DoCast(SPELL_2);
                        events.RescheduleEvent(EVENT_2, 20000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

//93802 tyranna
class npc_q38728_tyranna : public CreatureScript
{
public:
    npc_q38728_tyranna() : CreatureScript("npc_q38728_tyranna") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q38728_tyrannaAI(creature);
    }

    struct npc_q38728_tyrannaAI : public ScriptedAI
    {
        EventMap events;
        ObjectGuid playerGuid;
        
        uint32 healthPct;
        SummonList summons;
        uint32 count;
        std::unordered_set<uint32> currentPlayers;

        npc_q38728_tyrannaAI(Creature* creature) : ScriptedAI(creature), healthPct(0), summons(me), count(0)
        {
            // back a minute and a half after her death: 30 seconds of corpse, then 60
            creature->SetCorpseDelay(30);
            creature->SetRespawnDelay(60);
        }

        enum data
        {
            SPELL_1 = 197627,  // 8
            SPELL_2 = 197414,  // phase -35%
            SPELL_QUEENS_BITE = 197486,
            NPC_ADD = 100333,
            
        };
        
        void Reset() override
        {
            healthPct = 65;
            summons.DespawnAll();
            events.Reset();
            count = 0;
            currentPlayers.clear();
        }
        
        // her blows and spells on the player hit at a quarter of their strength
        void DamageDealt(Unit* victim, uint32& damage, DamageEffectType /*damageType*/) override
        {
            if (victim && (victim->IsPlayer() || victim->IsControlledByPlayer()))
                damage = damage / 4;
        }

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);
            if (summon->GetEntry() != NPC_ADD)     // the broodlings join the fight once down their threads
                DoZoneInCombat(summon, 30.0f);
        }

        // the broodlings come down the threads hung from the vault: the General Purpose Bunnies up there are where they start
        void BroodlingsFromThreads(uint32 count)
        {
            std::list<Creature*> anchors;
            me->GetCreatureListWithEntryInGrid(anchors, 24021, 70.0f);
            anchors.remove_if([this](Creature* anchor)
            {
                return anchor->GetPositionZ() < me->GetPositionZ() + 15.0f || anchor->GetExactDist2d(me) > 60.0f;
            });
            Trinity::Containers::RandomResizeList(anchors, count);
            for (Creature* anchor : anchors)
                if (Creature* broodling = me->SummonCreature(NPC_ADD, anchor->GetPositionX(), anchor->GetPositionY(), anchor->GetPositionZ() - 1.0f, frand(0.0f, 2.0f * float(M_PI)), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000))
                {
                    // its thread hangs from that anchor
                    if (broodling->AI())
                        broodling->AI()->SetGUID(anchor->GetGUID(), 3);
                }
        }

        // Queen's Bite: she channels it on the demon hunter, who hangs choking in the air for its 4 seconds, then drops back into the fight.
        // (cast by the hunter on her, the spell seated him on her like a passenger)
        void QueensBite(Unit* victim)
        {
            Creature* ally = victim->ToCreature();
            if (!ally)
                return;

            SpellInfo const* bite = sSpellMgr->GetSpellInfo(SPELL_QUEENS_BITE);
            me->StopMoving();
            me->SetFacingToObject(ally);
            uint32 const visual = bite ? bite->GetSpellXSpellVisualId(me, ally) : 0;
            me->AddChannelObject(ally->GetGUID());
            me->SetChannelSpellId(SPELL_QUEENS_BITE);
            me->SetChannelSpellXSpellVisualId(visual);
            me->AddUnitState(UNIT_STATE_CASTING);
            WorldPackets::Spells::SpellChannelStart start;
            start.CasterGUID = me->GetGUID();
            start.SpellID = SPELL_QUEENS_BITE;
            start.SpellXSpellVisualID = visual;
            start.ChannelDuration = 4000;
            me->SendMessageToSet(start.Write(), true);

            ally->AttackStop();
            ally->SetReactState(REACT_PASSIVE);
            // held up in the air before her, at the height of her face
            float const o = me->GetOrientation();
            float const hold = me->GetCombatReach() + 2.0f;
            ally->SetDisableGravity(true);
            ally->GetMotionMaster()->Clear(false);
            ally->GetMotionMaster()->MoveTakeoff(0, me->GetPositionX() + hold * std::cos(o), me->GetPositionY() + hold * std::sin(o), me->GetPositionZ() + 4.0f);
            ally->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_STRANGULATE);
            // once up there, held: a stun at once would stop the lift on the ground
            ally->AddDelayedEvent(900, [ally]() -> void
            {
                if (ally->IsAlive() && ally->GetUInt32Value(UNIT_FIELD_EMOTE_STATE) == EMOTE_STATE_STRANGULATE)
                    ally->AddUnitState(UNIT_STATE_STUNNED);
            });

            Creature* queen = me;
            ObjectGuid const allyGuid = ally->GetGUID();
            me->AddDelayedEvent(4000, [queen, allyGuid]() -> void
            {
                queen->ClearDynamicValue(UNIT_DYNAMIC_FIELD_CHANNEL_OBJECTS);
                queen->SetChannelSpellId(0);
                queen->SetChannelSpellXSpellVisualId(0);
                queen->ClearUnitState(UNIT_STATE_CASTING);
                // the end of the channel is what lets the client drop the one held in the bite
                WorldPackets::Spells::SpellChannelUpdate update;
                update.CasterGUID = queen->GetGUID();
                update.TimeRemaining = 0;
                queen->SendMessageToSet(update.Write(), true);

                Creature* ally = ObjectAccessor::GetCreature(*queen, allyGuid);
                if (!ally)
                    return;
                ally->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                ally->ClearUnitState(UNIT_STATE_STUNNED);
                ally->SetDisableGravity(false);
                ally->GetMotionMaster()->MoveFall();
                ally->SetReactState(REACT_AGGRESSIVE);
                if (queen->IsAlive() && ally->IsAlive() && ally->AI())
                    ally->AddDelayedEvent(800, [ally, queen]() -> void
                    {
                        if (queen->IsAlive() && ally->IsAlive() && ally->AI())
                            ally->AI()->AttackStart(queen);
                    });
            });
        }

        // Kayn, Jace, Allari and Kor'vas are not waiting at her feet: when a player comes before her they run in and charge;
        // left alone after a fight she is again without them until the next one comes
        GuidList allies;
        bool alliesDue = true;

        void InitializeAI() override
        {
            ScriptedAI::InitializeAI();
            alliesDue = true;
        }

        void JustRespawned() override
        {
            alliesDue = true;
        }

        void EnterEvadeMode() override
        {
            ScriptedAI::EnterEvadeMode();
            DismissAllies(0);
            alliesDue = true;
        }

        void MoveInLineOfSight(Unit* who) override
        {
            if (alliesDue && me->IsAlive() && who->IsPlayer() && !who->ToPlayer()->isGameMaster() && me->GetExactDist2d(who) < 45.0f
                && std::abs(who->GetPositionZ() - me->GetPositionZ()) < 12.0f)
                CallAllies();
            ScriptedAI::MoveInLineOfSight(who);
        }

        void DismissAllies(uint32 delay)
        {
            for (ObjectGuid const& guid : allies)
                if (Creature* ally = ObjectAccessor::GetCreature(*me, guid))
                    ally->DespawnOrUnsummon(delay);
            allies.clear();
        }

        void CallAllies()
        {
            alliesDue = false;
            DismissAllies(0);

            static uint32 const entries[] = { 97244, 97959, 97962, 98712 };   // Kayn, Jace, Allari, Kor'vas
            static float const sides[] = { -4.5f, -1.5f, 1.5f, 4.5f };
            Position const front = me->GetFirstCollisionPosition(26.0f, 0.0f);
            float const back = me->GetOrientation() + float(M_PI);
            for (uint8 i = 0; i < 4; ++i)
            {
                float const across = me->GetOrientation() + float(M_PI) / 2.0f;
                float x = front.GetPositionX() + sides[i] * std::cos(across);
                float y = front.GetPositionY() + sides[i] * std::sin(across);
                float z = front.GetPositionZ();
                me->UpdateGroundPositionZ(x, y, z);
                TempSummon* ally = me->GetMap()->SummonCreature(entries[i], Position(x, y, z, back));
                if (!ally)
                    continue;
                // summoned by the map, with no phase: they would not see her
                ally->SetPhaseId(me->GetPhases(), true);
                allies.push_back(ally->GetGUID());
                Creature* queen = me;
                ally->AddDelayedEvent(400 + i * 250, [ally, queen]() -> void
                {
                    if (!queen->IsAlive() || !ally->IsAlive())
                        return;
                    ally->SetWalk(false);
                    ally->SetReactState(REACT_AGGRESSIVE);
                    ally->SetInCombatWith(queen);
                    queen->SetInCombatWith(ally);
                    ally->AddThreat(queen, 100.0f);
                    if (ally->AI())
                        ally->AI()->AttackStart(queen);
                    ally->GetMotionMaster()->MoveChase(queen);
                });
            }
        }

        // the Illidari stay in the fight: on the broodlings down from the threads while there are any, on her when they are dead
        void SteerAllies()
        {
            std::list<Creature*> broodlings;
            for (ObjectGuid const& guid : summons)
                if (Creature* broodling = ObjectAccessor::GetCreature(*me, guid))
                    if (broodling->GetEntry() == NPC_ADD && broodling->IsAlive() && !broodling->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
                        broodlings.push_back(broodling);

            for (ObjectGuid const& guid : allies)
            {
                Creature* ally = ObjectAccessor::GetCreature(*me, guid);
                if (!ally || !ally->IsAlive() || !ally->AI() || ally->HasUnitState(UNIT_STATE_STUNNED))
                    continue;

                Unit* target = nullptr;
                float best = 50.0f;
                for (Creature* broodling : broodlings)
                    if (ally->GetDistance(broodling) < best && ally->IsValidAttackTarget(broodling))
                    {
                        best = ally->GetDistance(broodling);
                        target = broodling;
                    }
                if (!target && !me->HasAura(SPELL_2))
                    target = me;
                if (!target || ally->getVictim() == target)
                    continue;
                if (Unit* victim = ally->getVictim())
                    if (victim->GetEntry() == NPC_ADD && victim->IsAlive())
                        continue;   // finishing the broodling it is on

                ally->AddThreat(target, 100.0f);
                ally->AI()->AttackStart(target);
            }
        }

        void EnterCombat(Unit* victim) override
        {
            events.RescheduleEvent(EVENT_1, 8000);
            events.RescheduleEvent(EVENT_4, 10000);
            BroodlingsFromThreads(6);
        }

        void AttackedBy(Unit* attacker) override
        {
            // for every player in combat with this boss add 800k HP
            if (attacker->IsPlayer() && currentPlayers.count(attacker->GetGUIDLow()) == 0)
            {
                currentPlayers.insert(attacker->GetGUIDLow());
                me->SetMaxHealth(me->GetMaxHealth() + 800000);
                me->SetHealth(me->GetHealth() + 800000);
            }
        }

        void JustDied(Unit* killer) override
        {
            Talk(3);
            DismissAllies(20000);

            // remove Queen's Bite Transformation if any
            me->CastSpell(me, 208121, true);

            //TODO: Threat does not properly reset for the npc demon hunters due to the faction change
        }
        
        void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType dmgType) override
        {
            if (me->HasAura(SPELL_2))
                damage = 0;

            // in this fight the player draws back 8 % of what he deals as life
            if (attacker && attacker->IsPlayer() && damage && attacker->IsAlive())
                attacker->ModifyHealth(int64(damage) * 8 / 100);

            // the Illidari at her feet only wear her down, some 3000 a blow, and the kill is left to the player
            if (attacker && attacker->IsCreature() && !attacker->IsControlledByPlayer())
            {
                damage = std::min<uint32>(damage, urand(2800, 3200));
                if (me->GetHealth() <= damage || me->HealthBelowPctDamaged(5, damage))
                    damage = 0;
            }

            if (me->HealthBelowPct(healthPct))
            {
                if (healthPct == 65)
                    healthPct -= 30;
                else 
                    healthPct = 0;
                
                events.RescheduleEvent(EVENT_2, 500);
                events.CancelEvent(EVENT_1);
                events.CancelEvent(EVENT_4);
            }
        }
        
        uint32 steerTimer = 1500;

        void UpdateAI(uint32 diff) override
        {
            // also while she is gone into the shadows, when she has no victim of her own
            if (me->isInCombat() || !summons.empty())
            {
                if (steerTimer <= diff)
                {
                    SteerAllies();
                    steerTimer = 1500;
                }
                else
                    steerTimer -= diff;
            }

            if (!UpdateVictim())
                return;            

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_1);
                        events.RescheduleEvent(EVENT_1, 8000);
                        break;
                    case EVENT_2:
                        me->SetReactState(REACT_PASSIVE);
                        DoCast(SPELL_2);
                        Talk(1);
                        Talk(2);
                        count = 0;
                        events.RescheduleEvent(EVENT_3, 3000);
                        events.RescheduleEvent(EVENT_1, 27000);
                        events.RescheduleEvent(EVENT_4, 23000);
                        break;
                    case EVENT_3:
                        count++;
                        
                        BroodlingsFromThreads(6);
                        
                        if (count < 3)
                            events.RescheduleEvent(EVENT_3, 5000);
                        else
                            me->SetReactState(REACT_DEFENSIVE);
                        break;
                    case EVENT_4:
                        events.RescheduleEvent(EVENT_4, 55000);
                        Talk(0);

                        std::list<Unit*> targetList;
                        me->GetAttackableUnitListInRange(targetList, 20.0f);
                        targetList.remove_if([](WorldObject* target)
                        {
                            if (target->GetEntry() == 97244)
                                return false;
                            if (target->GetEntry() == 97959)
                                return false;
                            if (target->GetEntry() == 97962)
                                return false;
                            if (target->GetEntry() == 98712)
                                return false;

                            return true;
                        });

                        if (!targetList.empty())
                        {
                            std::vector<Unit*> targetVector(targetList.begin(), targetList.end());
                            Unit* bitten = targetVector[urand(0, targetVector.size() - 1)];
                            QueensBite(bitten);
                        }

                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

class spell_legion_197486 : public SpellScriptLoader
{
public:
    spell_legion_197486() : SpellScriptLoader("spell_legion_197486") { }

    class spell_legion_197486_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_legion_197486_SpellScript);

        void RemoveVehicleEnter()
        {
            if (Unit* caster = GetCaster())
                if (Creature* tyranna = caster->FindNearestCreature(93802, 150.0f, true))
                    tyranna->RemoveAura(197493);
        }

        void AddVehicleEnter(SpellEffIndex /*p_EffIndex*/)
        {
            if (Unit* caster = GetCaster())
                if (Creature* tyranna = caster->FindNearestCreature(93802, 150.0f, true))
                    caster->CastSpell(tyranna, 197493, true);
        }

        void Register() override
        {
            //TODO: this is still a mess...
            //OnEffectHitTarget += SpellEffectFn(spell_legion_197486_SpellScript::AddVehicleEnter, EFFECT_0, SPELL_EFFECT_KNOCK_BACK);
            //AfterHit += SpellHitFn(spell_legion_197486_SpellScript::RemoveVehicleEnter);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_legion_197486_SpellScript();
    }
};

class spell_legion_208121 : public SpellScriptLoader
{
public:
    spell_legion_208121() : SpellScriptLoader("spell_legion_208121") { }

    class spell_legion_208121_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_legion_208121_SpellScript);

        //TODO: debug this...
        void CorrectTargetsEff0(std::list<WorldObject*>& targets)
        {
            targets.remove_if([this](WorldObject* target)
            {
                if (Creature* creature = target->ToCreature())
                    if (creature->HasAura(this->GetSpellInfo()->GetEffect(0)->TriggerSpell))
                        return false;

                return true;
            });
        }

        void CorrectTargetsEff1(std::list<WorldObject*>& targets)
        {
            targets.remove_if([this](WorldObject* target)
            {
                if (Creature* creature = target->ToCreature())
                    if (creature->HasAura(this->GetSpellInfo()->GetEffect(1)->TriggerSpell))
                        return false;

                return true;
            });
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_legion_208121_SpellScript::CorrectTargetsEff0, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_legion_208121_SpellScript::CorrectTargetsEff1, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_legion_208121_SpellScript();
    }
};

class spell_legion_197523 : public SpellScriptLoader
{
public:
    spell_legion_197523() : SpellScriptLoader("spell_legion_197523") { }

    class spell_legion_197523_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_legion_197523_SpellScript);

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            if (Unit* unit = GetCaster())
            {
                if (unit->GetEntry() == 97244 || unit->GetEntry() == 97959)
                    unit->CastSpell(unit, 197505);
                else
                    unit->CastSpell(unit, 197598);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_legion_197523_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_legion_197523_SpellScript();
    }
};

class spell_legion_197505_197598 : public SpellScriptLoader
{
public:
    spell_legion_197505_197598() : SpellScriptLoader("spell_legion_197505_197598") { }

    class spell_legion_197505_197598_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_legion_197505_197598_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                Unit* caster = GetCaster();
                if(!caster)
                    return;

                //TODO: use dbc values here instead of hardcoded
                //if (caster->GetHealthPct() < aurEff->GetBaseAmount())
                //    caster->RemoveAura(aurEff->GetBase());
                if (caster->GetHealthPct() < 50.0f)
                {
                    caster->RemoveAura(197505);
                    caster->RemoveAura(197598);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_legion_197505_197598_AuraScript::OnTick, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

    AuraScript* GetAuraScript() const override
    {
        return new spell_legion_197505_197598_AuraScript();
    }
};

//101760
class npc_q38728_progres1 : public CreatureScript
{
public:
    npc_q38728_progres1() : CreatureScript("npc_q38728_progres1") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q38728_progres1AI(creature);
    }

    struct npc_q38728_progres1AI : public ScriptedAI
    {
        npc_q38728_progres1AI(Creature* creature) : ScriptedAI(creature)
        {
            SetCanSeeEvenInPassiveMode(true);
        }

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);
        }


        enum data
        {
            CREDIT = 101760,
            QUEST = 38728,
        };
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 20.0f))
                return;

            if (who->ToPlayer()->GetQuestObjectiveData(QUEST, CREDIT))
                return;

            WorldLocation loc;
            loc.m_mapId = 1481;
            loc.m_positionX = 1467.47f;
            loc.m_positionY = 1412.78f;
            loc.m_positionZ = 243.96f;
            loc.SetOrientation(who->GetOrientation());
            who->ToPlayer()->SetHomebind(loc, 7749);
            who->ToPlayer()->SendBindPointUpdate();

            who->ToPlayer()->KilledMonsterCredit(CREDIT);
        }
    };
};

//245728
class go_q38729 : public GameObjectScript
{
public:
    go_q38729() : GameObjectScript("go_q38729") { }

    struct go_q38729AI : public GameObjectAI
    {
        go_q38729AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 38729,
            CREDIT = 100651,
        };

        // the object has a lock: opening it calls GossipUse, never GossipHello
        bool GossipUse(Player* player) override
        {
            return GossipHello(player, true);
        }

        bool GossipHello(Player* player, bool isUse) override
        {
            if (!isUse)
                return true;

            if (player->GetReqKillOrCastCurrentCount(QUEST, CREDIT))
                return true;

            player->KilledMonsterCredit(CREDIT);
            return false;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q38729AI(go);
    }
};

// 245045
class go_q39262 : public GameObjectScript
{
public:
    go_q39262() : GameObjectScript("go_q39262") { }

    struct go_q39262AI : public GameObjectAI
    {
        go_q39262AI(GameObject* go) : GameObjectAI(go), ow(nullptr)
        {
            check = false;
            timer = 0;
        }

        bool check;
        uint32 timer;
        Player* ow;
        
        void UpdateAI(uint32 diff) override
        {
            std::list<Player*> list;
            list.clear();
            go->GetPlayerListInGrid(list, 15.0f);
            if (!list.empty())
            {
               for (std::list<Player*>::const_iterator itr = list.begin(); itr != list.end(); ++itr)
               {
                   // the cave is looked into once Jace has asked for it
                   if ((*itr)->HasAura(188501) && (*itr)->GetQuestObjectiveData(39262, 96436) > 0)
                       (*itr)->KilledMonsterCredit(96437);
               }
            }
            
            if (check)
                if (timer <= diff)
                {
                    check = false;
                    if (ow)
                        go->DestroyForPlayer(ow);
                    go->SetLootState(GO_JUST_DEACTIVATED);
                } else
                    timer -= diff;
        }
        
        void IsSummonedBy(Unit* summoner)
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                ow = summoner->ToPlayer();
                check = true;
                timer = 5500;
            }
            else
                go->SetLootState(GO_JUST_DEACTIVATED);
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q39262AI(go);
    }
};

// 99916
class npc_q40378_1 : public CreatureScript
{
public:
    npc_q40378_1() : CreatureScript("npc_q40378_1") {}

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q40378_1AI(creature);
    }
    struct npc_q40378_1AI : public ScriptedAI
    {
        npc_q40378_1AI(Creature* creature) : ScriptedAI(creature) 
        {
            check = false;
        }
        
        GuidSet m_player_for_event;
        bool check;
        
        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                sCreatureTextMgr->SendChat(me, TEXT_GENERIC_1, summoner->GetGUID());
                check = true;
                me->GetMotionMaster()->MovePoint(1, 913.92f, 2848.63f, -0.69f);
                me->Mount(64385);
                me->DespawnOrUnsummon(10000);
            }
            else 
                me->DespawnOrUnsummon();
        }
        
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 10.0f))
                return;

            GuidSet::iterator itr = m_player_for_event.find(who->GetGUID());
            if (itr != m_player_for_event.end())
                return;

            if (who->ToPlayer()->GetQuestStatus(40378) != QUEST_STATUS_INCOMPLETE)
                return;
            
            if (check)
                return;

            m_player_for_event.insert(who->GetGUID());
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, who->GetGUID());
        }
    };
};


class npc_q40378_2 : public CreatureScript
{
public:
    npc_q40378_2() : CreatureScript("npc_q40378_2") {}

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q40378_2AI(creature);
    }
    struct npc_q40378_2AI : public ScriptedAI
    {
        npc_q40378_2AI(Creature* creature) : ScriptedAI(creature), timer(0)
        {
            check = false;
        }

        uint32 timer;
        bool check;
        
        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                timer = 16000;
                check = true;
            }
            else 
                me->DespawnOrUnsummon();
        }
        
        void UpdateAI(uint32 diff) override
        {
            if (check)
            {
                if (timer <= diff)
                {
                    me->SetVisible(true);
                    check = false;
                } else
                    timer -= diff;
            }
        }
        
        // the gift teaches the felsaber mount; the player then rides it at once
        void OnSpellClick(Unit* clicker) override
        {
            me->DespawnOrUnsummon(10000);
            if (Player* player = clicker->ToPlayer())
                player->AddDelayedEvent(500, [player]() -> void
                {
                    if (player->HasSpell(SPELL_FELSABER_MOUNT) && !player->IsMounted())
                        player->CastSpell(player, SPELL_FELSABER_MOUNT, true);
                });
        }

        enum
        {
            SPELL_FELSABER_MOUNT = 200175,
        };
    };
};

// 100982
class npc_q40379_1: public CreatureScript
{
public:
    npc_q40379_1() : CreatureScript("npc_q40379_1") {}

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == 40379) 
            player->SummonCreature(100982, 826.90f, 2758.64f, -30.50f, 1.62f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
              //  target->AddPlayerInPersonnalVisibilityList(player->GetGUID());
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q40379_1AI(creature);
    }
    struct npc_q40379_1AI : public ScriptedAI
    {
        npc_q40379_1AI(Creature* creature) : ScriptedAI(creature) {}
        
        
        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                sCreatureTextMgr->SendChat(me, 0, summoner->GetGUID());
                me->Mount(64385);
                me->GetMotionMaster()->MovePath(10267113, false);
                me->DespawnOrUnsummon(10000);
            }
            else 
                me->DespawnOrUnsummon();
        }
    };
};


class npc_q38759_1 : public CreatureScript
{
public:
    npc_q38759_1() : CreatureScript("npc_q38759_1") {}

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q38759_1AI(creature);
    }
    struct npc_q38759_1AI : public ScriptedAI
    {
        npc_q38759_1AI(Creature* creature) : ScriptedAI(creature) { }
        
        GuidSet m_player_for_event;
        
        void MoveInLineOfSight(Unit* who) override
        {
            if (auto owner = me->GetAnyOwner())
                if (owner->IsPlayer())
                    return;

            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 15.0f))
                return;

            GuidSet::iterator itr = m_player_for_event.find(who->GetGUID());
            if (itr != m_player_for_event.end())
                return;

            if (who->ToPlayer()->GetQuestStatus(38759) != QUEST_STATUS_INCOMPLETE)
                return;
           

            m_player_for_event.insert(who->GetGUID());
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, who->GetGUID());
        }
    };
};

// 99917
class npc_q40379_2 : public CreatureScript
{
public:
    npc_q40379_2() : CreatureScript("npc_q40379_2") {}

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q40379_2AI(creature);
    }
    struct npc_q40379_2AI : public ScriptedAI
    {
        npc_q40379_2AI(Creature* creature) : ScriptedAI(creature) { }
        
        GuidSet m_player_for_event;
        
        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                sCreatureTextMgr->SendChat(me, 2, summoner->GetGUID());
                me->Mount(64385);
                me->GetMotionMaster()->MovePoint(0, 825.57f, 2488.19f, -59.78f);
                me->DespawnOrUnsummon(10000);
            }
            else 
                me->DespawnOrUnsummon();
        }
        
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 15.0f))
                return;

            GuidSet::iterator itr = m_player_for_event.find(who->GetGUID());
            if (itr != m_player_for_event.end())
                return;

            if (who->ToPlayer()->GetQuestStatus(40379) != QUEST_STATUS_INCOMPLETE)
                return;
           

            m_player_for_event.insert(who->GetGUID());
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, who->GetGUID());
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_1, who->GetGUID());
        }
    };
};

class npc_96654 : public CreatureScript
{
public:
    npc_96654() : CreatureScript("npc_96654") {}

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_96654AI(creature);
    }
    struct npc_96654AI : public ScriptedAI
    {
        npc_96654AI(Creature* creature) : ScriptedAI(creature) { }
        
        GuidSet m_player_for_event;
                
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 10.0f))
                return;

            GuidSet::iterator itr = m_player_for_event.find(who->GetGUID());
            if (itr != m_player_for_event.end())
                return;
            m_player_for_event.insert(who->GetGUID());
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, who->GetGUID());
        }
    };
};

// 96436
class npc_96436 : public CreatureScript
{
public:
    npc_96436() : CreatureScript("npc_96436") {}
    
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        player->CLOSE_GOSSIP_MENU();

        sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_0, player->GetGUID());
        player->KilledMonsterCredit(96436);      

        return true;
    }
    
    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == 39495)
        {
            if (Creature* Jais = player->SummonCreature(96436, creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ(), creature->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
            {
              //  Jais->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                Jais->CastSpell(Jais, 194326);
            }
            
            if (Creature* targ = player->SummonCreature(101788, 1267.46f, 1627.08f, 104.10f, 2.66f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
            {
               // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                targ->CastSpell(targ, 194326);
            }
            if (Creature* targ = player->SummonCreature(101787, 1262.82f, 1624.67f, 104.28f, 2.59f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
            {
               // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                targ->CastSpell(targ, 194326);
            }
            if (Creature* targ = player->SummonCreature(101790, 1256.11f, 1623.56f, 103.58f, 2.43f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
            {
              //  targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                targ->CastSpell(targ, 194326);
            }
            if (Creature* targ = player->SummonCreature(101789, 1260.16f, 1641.68f, 100.90f, 3.04f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
            {
              //  targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                targ->CastSpell(targ, 194326);
            }
            if (Creature* targ = player->SummonCreature(101787, 1262.44f, 1646.57f, 100.38f, 3.39f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
            {
               // targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                targ->CastSpell(targ, 194326);
                targ->AddDelayedEvent(3800, [targ]()-> void
                {
                    targ->CastSpell(targ, 194366);
                });
            }

            player->SummonCreature(96504, 1270.82f, 1629.88f, 103.63f, 2.80f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);            
            player->SummonCreature(96503, 1260.15f, 1622.65f, 104.51f, 2.54f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);            
            player->SummonCreature(96500, 1264.43f, 1643.56f, 100.36f, 3.09f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);            
            player->SummonCreature(96500, 1262.93f, 1649.17f, 100.10f, 3.39f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);            
            player->SummonCreature(96501, 1265.69f, 1645.30f, 99.90f, 3.22f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL);
            
            if (GameObject* targ = player->SummonGameObject(245045, 1237.15f, 1642.62f,    103.152f, 5.80559f, 0.0f, 0.0f, 0.0f, 0.0f, DAY, player->GetGUID()))
            {
                targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                ObjectGuid playerGuid = player->GetGUID();
                targ->AddDelayedEvent(3800, [targ, playerGuid]() -> void
                {
                    targ->SetLootState(GO_READY);
                    if (auto player = ObjectAccessor::FindPlayer(playerGuid))
                        targ->UseDoorOrButton(10000, false, player);
                });
            }
            
        }

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_96436AI(creature);
    }
    struct npc_96436AI : public ScriptedAI
    {
        npc_96436AI(Creature* creature) : ScriptedAI(creature) 
        {
            check = false;
            timer = 0;
        }
        
        bool check;
        uint32 timer;
        
        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                timer = 5500;
                check = true;
            }
            else 
                me->DespawnOrUnsummon();
        }
        
        void UpdateAI(uint32 diff) override
        {
            if (check)
                if (timer <= diff)
                {
                    Talk(1);
                    me->GetMotionMaster()->MovePoint(0, 1223.62f, 1649.79f, 101.29f);
                    me->DespawnOrUnsummon(4000);
                    check = false;
                } else timer -= diff;
        }

    };
};

// 96501, 96500, 96503, 96504, 101787, 101789, 101790, 101788
class npc_q39495 : public CreatureScript
{
public:
    npc_q39495() : CreatureScript("npc_q39495") {}

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q39495AI(creature);
    }
    struct npc_q39495AI : public ScriptedAI
    {
        npc_q39495AI(Creature* creature) : ScriptedAI(creature)
        {
            check = false;
            timer = 0;
        }
        
        bool check;
        uint32 timer;
        EventMap events;
        
        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                timer = 5500;
                check = true;
            }
            else 
                me->DespawnOrUnsummon();
        }
        
        void Reset() override
        {
            events.Reset();
        }
        void EnterCombat(Unit* who) override
        {
            if (me->GetEntry() == 96501)
            {
                events.RescheduleEvent(EVENT_1, 3000); // 200880
                events.RescheduleEvent(EVENT_2, 11000); // 197744
            }
            if (me->GetEntry() == 101787)
            {
                events.RescheduleEvent(EVENT_3, 34000); // 197641
                events.RescheduleEvent(EVENT_4, 12000); // 197639
            }
                
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType dmgType) override
        {
            damage /= 20;
        }

        void UpdateAI(uint32 diff) override
        {
            if (check)
                if (timer <= diff)
                {
                    me->GetMotionMaster()->MovePoint(0, 1223.62f, 1649.79f, 101.29f);
                    me->DespawnOrUnsummon(4000);
                    check = false;
                } else timer -= diff;
                
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;            

            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(200880);
                        events.RescheduleEvent(EVENT_1, 3000);
                        break;
                    case EVENT_2:
                        DoCast(197744);
                        events.RescheduleEvent(EVENT_2, 11000);
                        break;
                    case EVENT_3:
                        DoCast(197641);
                        events.RescheduleEvent(EVENT_3, 34000); 
                        break;
                    case EVENT_4:
                        DoCast(197639);
                        events.RescheduleEvent(EVENT_4, 12000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
            
        }

    };
};

// 93762 96732 96731
class npc_q38727 : public CreatureScript
{
public:
    npc_q38727() : CreatureScript("npc_q38727") {}

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q38727AI(creature);
    }
    struct npc_q38727AI : public ScriptedAI
    {
        npc_q38727AI(Creature* creature) : ScriptedAI(creature)
        {
            check1 = false;
            check2 = false;
            timer = 0;
            bombardTimer = urand(5000, 30000);
            summoned = false;
        }

        bool check1;
        bool check2;
        uint32 timer;
        uint32 bombardTimer;
        bool summoned;

        // the devastators of the battlefield shell the Illidari outpost about every 30 seconds, each at its own moment
        // (the fire is a dummy: the shell is seen, nothing is hit)
        void Bombard()
        {
            static Position const targets[] =
            {
                { 1421.22f, 1666.32f, 40.29f },
                { 1472.84f, 1688.98f, 34.69f },
                { 1494.37f, 1772.05f, 44.52f },
            };
            Position const& target = targets[urand(0, 2)];
            float const x = target.GetPositionX() + frand(-8.0f, 8.0f);
            float const y = target.GetPositionY() + frand(-8.0f, 8.0f);
            float z = me->GetMap()->GetHeight(me->GetPhases(), x, y, target.GetPositionZ() + 10.0f, true, 30.0f);
            if (z <= INVALID_HEIGHT)
                z = target.GetPositionZ();
            // the shell and its impact only: the fire spell (191454) picks its own target near the caster and fell back on the devastator
            // itself. Played on its own from the muzzle (the model has no attachment 20 for the visual to start from, and a client too far to
            // draw the devastator still sees the shell fly): the barrel ends 17 yards ahead and 11.5 up in the model, scaled 1.5
            float const o = me->GetOrientation();
            WorldPackets::Spells::PlayOrphanSpellVisual shell;
            shell.SourceLocation = Position(me->GetPositionX() + 25.5f * std::cos(o), me->GetPositionY() + 25.5f * std::sin(o), me->GetPositionZ() + 17.3f);
            shell.SourceRotation = Position(0.0f, 0.0f, o);
            shell.TargetLocation = Position(x, y, z);
            shell.SpellVisualID = 49571;
            shell.TravelSpeed = 60.0f;
            shell.UnkFloat = 0.0f;
            shell.SpeedAsTime = false;
            WorldPacket const* data = shell.Write();

            // not to a player who froze it: for him this devastator is hidden, its frozen copy stands in its place
            std::list<Player*> players;
            me->GetPlayerListInGrid(players, 533.0f);
            for (Player* player : players)
                if (player->InSamePhase(me) && !me->ShouldHideFor(player->GetGUID()))
                    player->SendDirectMessage(data);
        }

        void IsSummonedBy(Unit* summoner) override
        {
            summoned = true;
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                if (me->GetEntry() == 93762)
                {
                    check1 = true;
                    timer = 9000;
                    // the devastator stands where the banner calls it, it does not come for the player
                    me->SetReactState(REACT_PASSIVE);
                    me->SetControlled(true, UNIT_STATE_ROOT);
                }
                if (me->GetEntry() == 96732)
                {
                    check2 = true;
                    timer = 13000;  // when the charge the stalkers planted goes off
                    me->SetReactState(REACT_PASSIVE);
                    me->SetControlled(true, UNIT_STATE_ROOT);
                }
            }
            else 
                me->DespawnOrUnsummon();
        }
        
        void UpdateAI(uint32 diff) override
        {
            if (!summoned)
            {
                if (bombardTimer <= diff)
                {
                    Bombard();
                    bombardTimer = urand(28000, 32000);
                }
                else
                    bombardTimer -= diff;
            }

            if (check2)
                if (timer <= diff)
                {
                    MardumDevastatorBlast(me);
                    check2 = false;
                } else timer -= diff;
            if (check1)
                if (timer <= diff)
                {
                    DoCast(191568);
                    DoCast(198908);
                    check1 = false;
                } else timer -= diff;
        }

    };
};

// On Felbat Wings: the captains' own lanes, a few yards aside of one another, and their order of departure
static void MardumCaptainLane(uint32 entry, float& side, float& rise, uint32& start)
{
    side = 0.0f; rise = 0.0f; start = 0;
    switch (entry)
    {
        case 93127: side = -1.5f; start = 0; break;                 // Kayn
        case 96420: side = 1.5f; start = 900; break;                // Cyana
        case 99045: side = -3.5f; rise = 3.0f; start = 1800; break;  // Kor'vas
        case 96655: side = 3.5f; rise = 3.0f; start = 2700; break;   // Allari
        default: break;
    }
}

// a point of a path moved aside of its line, the way along the previous and next points
static G3D::Vector3 MardumLanePoint(G3D::Vector3 const* points, size_t count, size_t i, float side)
{
    G3D::Vector3 const& next = points[std::min(i + 1, count - 1)];
    G3D::Vector3 const& prev = points[i ? i - 1 : 0];
    float const dx = next.x - prev.x, dy = next.y - prev.y;
    float const len = std::sqrt(dx * dx + dy * dy);
    G3D::Vector3 point = points[i];
    if (len > 0.1f)
    {
        point.x += -dy / len * side;
        point.y += dx / len * side;
    }
    return point;
}

static void MardumCaptainFlight(Creature* captain, bool flying, uint32 elapsed);

// along their path (10267121) to Izal Whitemoon, each in his lane, one after the other
static void MardumCaptainRide(Creature* captain)
{
    static G3D::Vector3 const ride[] =
    {
        { 1453.49f, 1765.18f, 54.44f }, { 1447.18f, 1764.17f, 54.44f }, { 1439.44f, 1760.43f, 54.49f }, { 1430.16f, 1754.24f, 54.49f },
        { 1425.37f, 1745.35f, 54.18f }, { 1431.95f, 1729.92f, 52.53f }, { 1445.79f, 1714.18f, 49.66f }, { 1449.67f, 1705.49f, 48.11f },
        { 1452.22f, 1689.68f, 47.23f },
    };
    float side, rise;
    uint32 start;
    MardumCaptainLane(captain->GetEntry(), side, rise, start);

    size_t const count = std::extent<decltype(ride)>::value;
    std::vector<G3D::Vector3> path(1, G3D::Vector3(captain->GetPositionX(), captain->GetPositionY(), captain->GetPositionZ()));
    for (size_t i = 0; i < count; ++i)
    {
        G3D::Vector3 point = MardumLanePoint(ride, count, i, side);
        captain->UpdateGroundPositionZ(point.x, point.y, point.z);
        path.push_back(point);
    }

    captain->AddDelayedEvent(start + 1, [captain, path]() -> void
    {
        captain->SetWalk(false);
        captain->GetMotionMaster()->MoveSmoothPath(0, path.data(), path.size(), false);
        MardumCaptainFlight(captain, false, 0);
    });
}

// at the end of the ride the felbat, and he is gone at the end of the flight
static void MardumCaptainFlight(Creature* captain, bool flying, uint32 elapsed)
{
    captain->AddDelayedEvent(250, [captain, flying, elapsed]() -> void
    {
        if (!flying && elapsed >= 500 && captain->movespline->Finalized())
        {
            // the rest of their path (points 10 to 22) flown for real: on the waypoint movement the felbat would walk on the air
            static G3D::Vector3 const flight[] =
            {
                { 1457.9f, 1686.92f, 49.0f }, { 1465.69f, 1683.24f, 56.19f }, { 1497.64f, 1649.48f, 66.41f }, { 1552.47f, 1600.74f, 90.13f },
                { 1614.68f, 1544.52f, 118.18f }, { 1691.57f, 1461.91f, 160.38f }, { 1728.71f, 1372.97f, 195.96f }, { 1653.56f, 1239.86f, 248.04f },
                { 1577.23f, 1214.63f, 266.97f }, { 1487.19f, 1214.9f, 283.74f }, { 1429.3f, 1278.13f, 285.25f }, { 1416.55f, 1326.68f, 279.51f },
                { 1433.87f, 1384.9f, 263.34f },
            };
            captain->GetMotionMaster()->Clear(false);
            captain->StopMoving();
            captain->Mount(65307);      // Illidari Fel Bat, the mount of the flight path
            captain->SetCanFly(true);
            captain->SetDisableGravity(true);
            captain->SetSpeed(MOVE_FLIGHT, 3.0f);
            // side by side rather than in a file: the lanes of the ride, opened wider once clear of the ground
            float side, rise;
            uint32 start;
            MardumCaptainLane(captain->GetEntry(), side, rise, start);
            std::vector<G3D::Vector3> path(1, G3D::Vector3(captain->GetPositionX(), captain->GetPositionY(), captain->GetPositionZ() + 1.5f));
            size_t const count = std::extent<decltype(flight)>::value;
            for (size_t i = 0; i < count; ++i)
            {
                float const open = std::min(1.0f, float(i) / 3.0f);   // the lanes open over the first points, above the foothold
                G3D::Vector3 point = MardumLanePoint(flight, count, i, side * (1.0f + 3.0f * open));
                point.z += rise * open;
                path.push_back(point);
            }
            captain->GetMotionMaster()->MoveSmoothFlyPath(0, path.data(), path.size());
            MardumCaptainFlight(captain, true, elapsed + 250);
            return;
        }
        // the flight over (each lane ends a few yards from the others)
        if (flying && captain->movespline->Finalized())
        {
            captain->DespawnOrUnsummon();
            return;
        }
        if (elapsed < 110000)
            MardumCaptainFlight(captain, flying, elapsed + 250);
    });
}

// 93127
class npc_93127 : public CreatureScript
{
public:
    npc_93127() : CreatureScript("npc_93127") {}

    enum data
    {
        GOSSIP_MENU_KAYN_MARDUM = 18993,
        GOSSIP_TEXT_KAYN_MARDUM = 27283,
    };

    // the options of the database all reach the script with the same action: the menu is built here, one action per option
    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        player->ADD_GOSSIP_ITEM_DB(18435, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        if (player->GetQuestStatus(39516) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(39515) == QUEST_STATUS_INCOMPLETE)
            player->ADD_GOSSIP_ITEM_DB(18435, 1, GOSSIP_SENDER_MAIN, 1);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF)
        {
            // what Kayn knows of Mardum, then "Anything else?" back to his menu
            player->ADD_GOSSIP_ITEM_DB(GOSSIP_MENU_KAYN_MARDUM, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(GOSSIP_TEXT_KAYN_MARDUM, creature->GetGUID());
        }
        else if (action == GOSSIP_ACTION_INFO_DEF + 1)
            OnGossipHello(player, creature);
        else if (action != 1)
            player->CLOSE_GOSSIP_MENU();
        else
        {
            player->CLOSE_GOSSIP_MENU();

            player->CastSpell(player, 195020, true);
            sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_3, player->GetGUID());
            // the power taken in: he roars, wrapped in fel flames
            creature->HandleEmoteCommand(EMOTE_ONESHOT_BATTLE_ROAR);
            creature->CastSpell(creature, 178740, true);  
        }

        return true;
    }
    
    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        switch(quest->GetQuestId()) 
        {
            case 38813:
                sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_0, player->GetGUID());  
                break;
            case 38727:
                sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_1, player->GetGUID());  
                break;
            case 39516:
                sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_2, player->GetGUID());  
                break;
            case 39663:
            {
                // the captains ride their own path (10267121) to Izal Whitemoon, take their felbats there and fly on along it to the Fel Hammer
                struct Rider { uint32 entry; Position pos; };
                Rider const riders[] =
                {
                    { 93127, creature->GetPosition() },
                    { 99045, { 1454.29f, 1763.44f, 54.52f, 5.56f } },
                    { 96420, { 1453.28f, 1762.33f, 54.52f, 0.26f } },
                    { 96655, { 1458.17f, 1763.06f, 54.52f, 3.16f } },
                };
                for (Rider const& rider : riders)
                {
                    Creature* targ = player->SummonCreature(rider.entry, rider.pos, TEMPSUMMON_MANUAL_DESPAWN, 0, 0, player->GetGUID());
                    if (!targ)
                        continue;
                    if (rider.entry == 93127)
                        sCreatureTextMgr->SendChat(targ, TEXT_GENERIC_4, player->GetGUID());
                    targ->Mount(64385);
                    MardumCaptainRide(targ);
                    targ->DespawnOrUnsummon(120000);
                }
            }
                break;
            case 39262:
            {
                Conversation* conversation = new Conversation;
                if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), 486, player, NULL, *player))
                    delete conversation;
            }
                break;
            default:
                break;
        }

        return true;
    }
};

class npc_c100161 : public CreatureScript
{
public:
    npc_c100161() : CreatureScript("npc_c100161") {}

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_c100161AI(creature);
    }

    struct npc_c100161AI : public ScriptedAI
    {
        npc_c100161AI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetReactState(REACT_PASSIVE);
            me->setActive(true);
        }

        uint32 timer = 2000;

        void UpdateAI(uint32 diff) override
        {
            if (timer <= diff)
            {
                auto pos = me->GetPosition();
                me->GetMap()->LoadGrid(pos);
                const auto& players = me->GetMap()->GetPlayers();
                for (auto & playerItr : players)
                    if (auto player = playerItr.getSource())
                        if (player->GetAreaId() == 7705)
                            if (me->GetDistance2d(player) <= 270.0f)
                                if (player->GetQuestStatus(38766) == QUEST_STATE_COMPLETE)
                                    me->CastSpell(player, 191669);

                timer = urand(5000, 10000);
            }
            else
                timer -= diff;

        }

    };
};



// 104842 - Legion Ship over Mardum's battlefield: volleys of Fel Bombardment shots from its cannon onto the ground spots laid
// ahead of the Illidari camp, each exploding as it lands; for the sight only, nothing is cast.
enum MardumLegionShip
{
    SPELL_VISUAL_SHIP_SHOT          = 50142,    // Fel Bombardment of the Broken Shore ships, drawn at scale 15
    SPELL_VISUAL_KIT_SHOT_IMPACT    = 60268,    // Fel Bombardment's explosion, played on the explosion marker so that it takes its size
    NPC_MARDUM_LEGION_SHIP          = 104842,
    NPC_MARDUM_FEL_BURST            = 600100,   // invisible ground spot aimed at
    NPC_MARDUM_SHOT_EXPLOSION       = 600102,   // invisible marker on every ground spot, scale 1.2: the explosion's size (its core is capped at 1.5)
    NPC_MARDUM_SHIP_CANNON          = 600101,   // invisible, where the shots start

    SHOT_FLIGHT_TIME                = 4500,
};

struct npc_mardum_legion_ship : public ScriptedAI
{
    npc_mardum_legion_ship(Creature* creature) : ScriptedAI(creature) { }

    void InitializeAI() override
    {
        me->SetReactState(REACT_PASSIVE);
        _volleyTimer = urand(1000, 3000);
    }

    void UpdateAI(uint32 diff) override
    {
        for (auto itr = _impacts.begin(); itr != _impacts.end();)
        {
            if (itr->Timer > diff)
            {
                itr->Timer -= diff;
                ++itr;
                continue;
            }

            if (Creature* spot = ObjectAccessor::GetCreature(*me, itr->Spot))
            {
                Creature* marker = spot->FindNearestCreature(NPC_MARDUM_SHOT_EXPLOSION, 3.0f);
                (marker ? marker : spot)->SendPlaySpellVisualKit(SPELL_VISUAL_KIT_SHOT_IMPACT, 0);
            }
            itr = _impacts.erase(itr);
        }

        if (!_shots.empty())
        {
            if (_shots.front().Delay > diff)
            {
                _shots.front().Delay -= diff;
                return;
            }

            uint32 const late = diff - _shots.front().Delay;
            Fire(_shots.front().Spot);
            _shots.pop_front();
            if (!_shots.empty())
                _shots.front().Delay = _shots.front().Delay > late ? _shots.front().Delay - late : 0;
            return;
        }

        if (_volleyTimer > diff)
        {
            _volleyTimer -= diff;
            return;
        }

        PlanVolley();
        _volleyTimer = urand(4000, 9000);
    }

private:
    struct Shot
    {
        ObjectGuid Spot;
        uint32 Delay;
    };

    struct Impact
    {
        ObjectGuid Spot;
        uint32 Timer;
    };

    // 1 to 4 shots at distinct spots, some leaving together, the others spread over a few seconds
    void PlanVolley()
    {
        std::list<Creature*> spots;
        me->GetCreatureListWithEntryInGrid(spots, NPC_MARDUM_FEL_BURST, 600.0f);
        if (spots.empty())
            return;

        uint32 const roll = urand(0, 99);
        uint32 count = roll < 40 ? 1 : roll < 70 ? 2 : roll < 90 ? 3 : 4;
        count = std::min<uint32>(count, spots.size());

        Trinity::Containers::RandomResizeList(spots, count);
        bool first = true;
        for (Creature* spot : spots)
        {
            _shots.push_back({ spot->GetGUID(), first || roll_chance_i(30) ? urand(0, 150) : urand(400, 1800) });
            first = false;
        }
    }

    void Fire(ObjectGuid spotGuid)
    {
        Creature* spot = ObjectAccessor::GetCreature(*me, spotGuid);
        if (!spot)
            return;

        Unit* gunner = me->FindNearestCreature(NPC_MARDUM_SHIP_CANNON, 500.0f);
        if (!gunner)
            gunner = me;

        // an orphan visual starts exactly at the given position, where the unit's own one would add the missile's built-in
        // offset; the travel speed is in yards per second, so that every shot takes the same time whatever the distance
        gunner->PlayOrphanSpellVisual(*gunner, Position(0.0f, 0.0f, gunner->GetAngle(spot)), *spot, SPELL_VISUAL_SHIP_SHOT,
            gunner->GetExactDist(spot) * 1000.0f / SHOT_FLIGHT_TIME, ObjectGuid::Empty, false);
        _impacts.push_back({ spotGuid, SHOT_FLIGHT_TIME });
    }

    std::deque<Shot> _shots;
    std::list<Impact> _impacts;
    uint32 _volleyTimer = 0;
};

// 102714 Eredar Summoner: channels at its portal, warns its allies when attacked, and turns on whoever destroys the portal
class npc_mardum_eredar_summoner : public CreatureScript
{
public:
    npc_mardum_eredar_summoner() : CreatureScript("npc_mardum_eredar_summoner") { }

    enum data
    {
        SPELL_FEL_CHANNEL = 188560,
        TEXT_DEFEND_PORTAL = 0,
        DATA_PORTAL_LOST = 1,
    };

    struct npc_mardum_eredar_summonerAI : public ScriptedAI
    {
        npc_mardum_eredar_summonerAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            _portalLost = false;
        }

        // set by the portal script, which makes the summoner cry out its own line
        void SetData(uint32 type, uint32 value) override
        {
            if (type == DATA_PORTAL_LOST)
                _portalLost = value != 0;
        }

        void EnterCombat(Unit* who) override
        {
            me->RemoveAurasDueToSpell(SPELL_FEL_CHANNEL);
            if (!_portalLost && roll_chance_i(40))
                Talk(TEXT_DEFEND_PORTAL, who->GetGUID());
        }

        void UpdateAI(uint32 /*diff*/) override
        {
            if (!UpdateVictim())
            {
                if (!me->HasAura(SPELL_FEL_CHANNEL) && !me->HasUnitState(UNIT_STATE_CASTING))
                    DoCast(me, SPELL_FEL_CHANNEL, true);
                return;
            }

            DoMeleeAttackIfReady();
        }

    private:
        bool _portalLost = false;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_mardum_eredar_summonerAI(creature);
    }
};

// 244601, 244604 Spire Stabilizer: breaking it brings down its Spire of Woe and puts out the spire's fire patches, for everyone, for two minutes
struct go_mardum_spire_stabilizer : public GameObjectAI
{
    go_mardum_spire_stabilizer(GameObject* go) : GameObjectAI(go) { }

    enum data
    {
        QUEST_BEFORE_WE_RE_OVERRUN  = 38766,
        QUEST_THEIR_NUMBERS_ARE_LEGION = 38819,
        GO_SOUL_ENGINE_STABILIZER   = 245995,
        NPC_SPIRE_STABILIZER_CREDIT = 101319,   // "Their Numbers Are Legion" Spire of Woe Stabilizer Destroyed
        SPELL_SPIRE_OF_WOE_BURN     = 192603,   // damage taken while standing in a fire patch
        SPIRE_DOWN_TIME             = 120,
        HIDE_STABILIZER_DELAY       = 950,      // its Destroy animation lasts 1 s
        HIDE_SPIRE_DELAY            = 3900,     // the spire's lasts 4 s
    };

    // the objects have a lock: opening them calls GossipUse, never GossipHello
    bool GossipUse(Player* player) override
    {
        return GossipHello(player, true);
    }

    bool GossipHello(Player* player, bool isUse) override
    {
        bool const soulEngine = go->GetEntry() == GO_SOUL_ENGINE_STABILIZER;
        if (!isUse || _downTimer || player->GetQuestStatus(soulEngine ? QUEST_THEIR_NUMBERS_ARE_LEGION : QUEST_BEFORE_WE_RE_OVERRUN) != QUEST_STATUS_INCOMPLETE)
            return true;

        GameObject* spire = NearestSpire();
        if (!spire)
            return true;

        _spire = spire->GetGUID();
        go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE | GO_FLAG_NOT_SELECTABLE);
        // the alternative state plays the Destroy animation of both models; once it has played the client rebuilds them in sight, so they are hidden
        go->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
        spire->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);

        if (soulEngine)
            player->KilledMonsterCredit(NPC_SPIRE_STABILIZER_CREDIT);

        std::list<Creature*> triggers;
        spire->GetCreatureListWithEntryInGrid(triggers, MardumFireOfSpire(spire->GetEntry()), 35.0f);
        spire->GetCreatureListWithEntryInGridAppend(triggers, NPC_SPIRE_OF_WOE_TOP, 35.0f);
        spire->GetCreatureListWithEntryInGridAppend(triggers, NPC_SOUL_ENGINE_SPIRE_TOP, 35.0f);
        for (Creature* trigger : triggers)
        {
            trigger->RemoveAllAuras();
            trigger->DespawnOrUnsummon(0, Seconds(SPIRE_DOWN_TIME));
        }

        // the burn of a patch that just went out would otherwise only end when its victim walks off
        std::list<Player*> players;
        spire->GetPlayerListInGrid(players, 50.0f);
        for (Player* plr : players)
            plr->RemoveAurasDueToSpell(SPELL_SPIRE_OF_WOE_BURN);

        _hideTimer = HIDE_STABILIZER_DELAY;
        _spireHideTimer = HIDE_SPIRE_DELAY;
        _downTimer = SPIRE_DOWN_TIME * IN_MILLISECONDS;
        return true;
    }

    static void Hide(GameObject* object, int32 seconds)
    {
        // hidden before going back to the ready state, which in sight would play the rebuild animation
        object->SetRespawnTime(seconds);
        object->SetGoState(GO_STATE_READY);
        object->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE | GO_FLAG_NOT_SELECTABLE);
    }

    // keeps running while the object is hidden
    void UpdateAI(uint32 diff) override
    {
        if (_spireHideTimer)
        {
            if (_spireHideTimer > diff)
                _spireHideTimer -= diff;
            else
            {
                _spireHideTimer = 0;
                if (GameObject* spire = ObjectAccessor::GetGameObject(*go, _spire))
                    Hide(spire, SPIRE_DOWN_TIME - HIDE_SPIRE_DELAY / IN_MILLISECONDS);
                _spire.Clear();
            }
        }

        if (_hideTimer)
        {
            if (_hideTimer > diff)
                _hideTimer -= diff;
            else
            {
                _hideTimer = 0;
                Hide(go, SPIRE_DOWN_TIME);
            }
        }

        if (_downTimer)
            _downTimer = _downTimer > diff ? _downTimer - diff : 0;
    }

private:
    GameObject* NearestSpire() const
    {
        GameObject* best = nullptr;
        for (uint32 entry : { GO_SPIRE_OF_WOE_A, GO_SPIRE_OF_WOE_B, GO_SOUL_ENGINE_SPIRE_A, GO_SOUL_ENGINE_SPIRE_B })
            if (GameObject* spire = go->FindNearestGameObject(entry, 40.0f))
                if (!best || go->GetDistance(spire) < go->GetDistance(best))
                    best = spire;
        return best;
    }

    ObjectGuid _spire;
    uint32 _hideTimer = 0;
    uint32 _spireHideTimer = 0;
    uint32 _downTimer = 0;
};

// 97629, 97634, 101318, 101321 Spire of Woe (ground): a fire patch that wanders around its spire, held by a beam from the spire's
// top; Beliash's while he fights, the Soul Engine's all the time
struct npc_mardum_spire_of_woe_fire : public ScriptedAI
{
    npc_mardum_spire_of_woe_fire(Creature* creature) : ScriptedAI(creature) { }

    enum data
    {
        SPELL_FIRE_PATCH        = 192604,
        SPELL_FEL_BEAM          = 192664,
        SPELL_X_VISUAL_FEL_BEAM = 88535,    // a channel: the client draws the beam from the top's channel, not from an aura
        POINT_WANDER            = 1,
        WANDER_RADIUS           = 10,
    };

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
        _lit = false;
        _wanderTimer = 0;
        // no boss to light them: they burn from the start
        if (me->GetEntry() == NPC_SOUL_ENGINE_FIRE_A || me->GetEntry() == NPC_SOUL_ENGINE_FIRE_B)
            _lightTimer = urand(1000, 3000);
    }

    void DoAction(int32 const action) override
    {
        if (action == ACTION_SPIRE_FIRES_ON && !_lit)
        {
            _lit = true;
            me->CastSpell(me, SPELL_FIRE_PATCH, true);
            if (Creature* top = SpireTop())
            {
                top->AddChannelObject(me->GetGUID());
                top->SetChannelSpellId(SPELL_FEL_BEAM);
                top->SetChannelSpellXSpellVisualId(SPELL_X_VISUAL_FEL_BEAM);
            }
            _wanderTimer = urand(200, 1500);
        }
        else if (action == ACTION_SPIRE_FIRES_OFF && _lit)
        {
            _lit = false;
            _wanderTimer = 0;
            me->RemoveAurasDueToSpell(SPELL_FIRE_PATCH);
            if (Creature* top = SpireTop())
            {
                top->ClearDynamicValue(UNIT_DYNAMIC_FIELD_CHANNEL_OBJECTS);
                top->SetChannelSpellId(0);
                top->SetChannelSpellXSpellVisualId(0);
            }
            me->GetMotionMaster()->MovePoint(0, me->GetHomePosition(), false);
        }
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type == POINT_MOTION_TYPE && id == POINT_WANDER && _lit)
            _wanderTimer = urand(1, 1200);
    }

    void UpdateAI(uint32 diff) override
    {
        if (_lightTimer)
        {
            if (_lightTimer > diff)
                _lightTimer -= diff;
            else
            {
                _lightTimer = 0;
                DoAction(ACTION_SPIRE_FIRES_ON);
            }
        }

        if (!_wanderTimer)
            return;

        if (_wanderTimer > diff)
        {
            _wanderTimer -= diff;
            return;
        }

        _wanderTimer = 0;
        Position const& home = me->GetHomePosition();
        float angle = frand(0.0f, 2.0f * float(M_PI));
        float dist = frand(3.0f, float(WANDER_RADIUS));
        float x = home.GetPositionX() + dist * std::cos(angle);
        float y = home.GetPositionY() + dist * std::sin(angle);
        float z = home.GetPositionZ();
        me->UpdateGroundPositionZ(x, y, z);
        me->GetMotionMaster()->MovePoint(POINT_WANDER, x, y, z, false);
    }

private:
    // the spire whose fire this is, then the crystal at its top
    Creature* SpireTop() const
    {
        GameObject* spire = me->FindNearestGameObject(MardumSpireOfFire(me->GetEntry()), 60.0f);
        Creature* top = MardumSpireTop(spire);
        return top;
    }

    bool _lit = false;
    uint32 _wanderTimer = 0;
    uint32 _lightTimer = 0;
};

// 93716 Doom Slayer, 96930 Demon Hunter: a slayer and a demon hunter standing close fight to the death to bring the zone to life;
// the demon hunter hits a little harder and wins, goes back to its place, and the fallen one comes back 20 seconds later
struct npc_mardum_sparring_foes : public ScriptedAI
{
    npc_mardum_sparring_foes(Creature* creature) : ScriptedAI(creature) { }

    enum data
    {
        NPC_DOOM_SLAYER             = 93716,
        NPC_DEMON_HUNTER            = 96930,
        SPELL_NO_NPC_DAMAGE_BELOW   = 151597,   // NPC hits stop at 45-90% health: it would keep them from ever dying
        SPELL_CHAOS_STRIKE          = 197639,   // the demon hunter's attacks
        SPELL_EYE_BEAM              = 197641,
        EVENT_LOOK_FOR_FOE          = 1,
        EVENT_CHAOS_STRIKE          = 2,
        EVENT_EYE_BEAM              = 3,
    };

    void Reset() override
    {
        _events.Reset();
        me->RemoveAurasDueToSpell(SPELL_NO_NPC_DAMAGE_BELOW);
        _events.ScheduleEvent(EVENT_LOOK_FOR_FOE, urand(1000, 3000));
    }

    bool IsFoe(Unit* unit) const
    {
        return unit && unit->GetEntry() == (me->GetEntry() == NPC_DOOM_SLAYER ? NPC_DEMON_HUNTER : NPC_DOOM_SLAYER);
    }

    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*dmgType*/) override
    {
        if (!IsFoe(attacker))
            return;

        // the demon hunter hits a tenth harder than the slayer
        if (me->GetEntry() == NPC_DOOM_SLAYER)
            damage += damage / 10;
    }

    void JustDied(Unit* killer) override
    {
        // fallen to its foe: the corpse goes after 3 s and the fighter is back 15 s after its death
        if (IsFoe(killer))
        {
            me->SetRespawnTime(15);
            me->SaveRespawnTime();
            me->DespawnOrUnsummon(3000, Seconds(12));
        }
    }

    // the winner leaves the fight at once and walks back to its place
    void KilledUnit(Unit* victim) override
    {
        if (IsFoe(victim))
            EnterEvadeMode();
    }

    // back at its place, out of the fight: it has its health back before the next duel
    void JustReachedHome() override
    {
        me->SetFullHealth();
    }

    void EnterCombat(Unit* /*who*/) override
    {
        _events.CancelEvent(EVENT_LOOK_FOR_FOE);
        if (me->GetEntry() == NPC_DEMON_HUNTER)
        {
            _events.ScheduleEvent(EVENT_CHAOS_STRIKE, urand(6000, 7000));
            _events.ScheduleEvent(EVENT_EYE_BEAM, urand(5000, 7000));
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        if (!UpdateVictim())
        {
            if (_events.ExecuteEvent() == EVENT_LOOK_FOR_FOE)
            {
                // a corpse laid out on the ground stays down
                if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PREVENT_EMOTES))
                    if (Creature* foe = me->FindNearestCreature(me->GetEntry() == NPC_DOOM_SLAYER ? NPC_DEMON_HUNTER : NPC_DOOM_SLAYER, 6.0f))
                        if (!foe->isInCombat() && !foe->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PREVENT_EMOTES))
                        {
                            // the core starts a fight with no threat at all, which the target selection drops at once
                            AttackStart(foe);
                            me->AddThreat(foe, 100.0f);
                            if (foe->AI())
                                foe->AI()->AttackStart(me);
                            foe->AddThreat(me, 100.0f);
                            return;
                        }
                _events.ScheduleEvent(EVENT_LOOK_FOR_FOE, 3000);
            }
            return;
        }

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        switch (_events.ExecuteEvent())
        {
            case EVENT_CHAOS_STRIKE:
                DoCastVictim(SPELL_CHAOS_STRIKE);
                _events.ScheduleEvent(EVENT_CHAOS_STRIKE, urand(6000, 7000));
                break;
            case EVENT_EYE_BEAM:
                DoCastVictim(SPELL_EYE_BEAM);
                _events.ScheduleEvent(EVENT_EYE_BEAM, urand(26000, 27000));
                break;
            default:
                break;
        }

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
};

// 100543 100545 100548 100549 Demon Hunter: illidari left for dead in the Soul Engine. The player bandages them (spell click,
// a channel): at the end of it they get up, say a word and run back to the fight, for "Their Numbers Are Legion"
struct npc_mardum_injured_demon_hunter : public ScriptedAI
{
    npc_mardum_injured_demon_hunter(Creature* creature) : ScriptedAI(creature) { }

    enum data
    {
        SPELL_INJURED           = 192264,
        SPELL_BANDAGE           = 192262,
        NPC_INJURED_DH_CREDIT   = 97395,
        TALK_THANKS             = 3,
        POINT_RUN_OFF           = 1,
        BANDAGE_MIN_TIME        = 2500,     // a bandage stopped sooner was cancelled
    };

    ObjectGuid _healer;
    uint32 _bandageTime = 0;
    uint32 _runTimer = 0;
    bool _healed = false;

    void Reset() override
    {
        _healed = false;
        _healer.Clear();
        _runTimer = 0;
        if (!me->HasAura(SPELL_INJURED))
            me->CastSpell(me, SPELL_INJURED, true);
        me->SetStandState(UNIT_STAND_STATE_DEAD);
        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
    }

    void SpellHit(Unit* caster, SpellInfo const* spell) override
    {
        if (_healed || spell->Id != SPELL_BANDAGE || !caster->IsPlayer())
            return;

        _healer = caster->GetGUID();
        _bandageTime = 0;
    }

    void GetUp(Player* player)
    {
        _healed = true;
        player->KilledMonsterCredit(NPC_INJURED_DH_CREDIT);
        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        me->RemoveAurasDueToSpell(SPELL_INJURED);
        me->SetStandState(UNIT_STAND_STATE_STAND);
        me->SetFacingToObject(player);
        if (sCreatureTextMgr->TextExist(me->GetEntry(), TALK_THANKS))
            sCreatureTextMgr->SendChat(me, TALK_THANKS, player->GetGUID());
        _runTimer = 2500;
    }

    // back to the fight: a run of some 25 yards away from the player, then gone; another one lies here a minute later
    void RunOff()
    {
        float away = me->GetOrientation() + float(M_PI) + frand(-0.6f, 0.6f);
        if (Player* player = ObjectAccessor::GetPlayer(*me, _healer))
            away = player->GetAngle(me) + frand(-0.6f, 0.6f);
        Position dest = me->GetFirstCollisionPosition(frand(20.0f, 30.0f), away - me->GetOrientation());
        me->SetWalk(false);
        me->GetMotionMaster()->MovePoint(POINT_RUN_OFF, dest);
        me->DespawnOrUnsummon(10000, Seconds(60));
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type == POINT_MOTION_TYPE && id == POINT_RUN_OFF)
            me->DespawnOrUnsummon(0, Seconds(60));
    }

    void UpdateAI(uint32 diff) override
    {
        if (_runTimer)
        {
            if (_runTimer <= diff)
            {
                _runTimer = 0;
                RunOff();
            }
            else
                _runTimer -= diff;
            return;
        }

        if (_healed || _healer.IsEmpty())
            return;

        _bandageTime += diff;
        if (me->HasAura(SPELL_BANDAGE))
            return;

        // the channel is over: gone to its end, he gets up; cut short, he stays down
        Player* player = ObjectAccessor::GetPlayer(*me, _healer);
        if (player && _bandageTime >= BANDAGE_MIN_TIME)
            GetUp(player);
        else
            _healer.Clear();
    }
};

// 101947 Doom Fortress Stabilizer: a machine, it breaks apart when destroyed instead of standing there whole
struct npc_mardum_doom_fortress_stabilizer : public ScriptedAI
{
    npc_mardum_doom_fortress_stabilizer(Creature* creature) : ScriptedAI(creature) { }

    enum data
    {
        ANIMKIT_DESTROY_THEN_DESTROYED = 976,   // animation 150 (Destroy), then the broken pose held
    };

    void JustDied(Unit* /*killer*/) override
    {
        me->PlayOneShotAnimKit(ANIMKIT_DESTROY_THEN_DESTROYED);
    }

    void UpdateAI(uint32 /*diff*/) override { }
};

// the Mannethrel each player saw burst, sitting again for him once the teaching quest is abandoned
static std::map<ObjectGuid, ObjectGuid> sMannethrelHiddenFor;

class player_mardum_mannethrel_back : public PlayerScript
{
public:
    player_mardum_mannethrel_back() : PlayerScript("player_mardum_mannethrel_back") { }

    void OnQuestAbandon(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() != 39515 && quest->GetQuestId() != 39516)
            return;

        auto itr = sMannethrelHiddenFor.find(player->GetGUID());
        if (itr == sMannethrelHiddenFor.end())
            return;

        if (Creature* mannethrel = player->GetMap()->GetCreature(itr->second))
            mannethrel->RemoveFromHideList(player->GetGUID());
        sMannethrelHiddenFor.erase(itr);
        player->UpdateObjectVisibility();
    }
};

// 96652 - Mannethrel Darkstar cannot hold the secrets he is taught: he rises wrapped in fel, bursts, and leaves only his warglaives;
// Kayn sees him go. Played by a copy for the player who teaches him: the others keep their Mannethrel sitting
class npc_mardum_mannethrel_darkstar : public CreatureScript
{
public:
    npc_mardum_mannethrel_darkstar() : CreatureScript("npc_mardum_mannethrel_darkstar") { }

    enum data
    {
        QUEST_VENGEANCE         = 39515,
        QUEST_HAVOC             = 39516,
        NPC_KAYN                = 93127,
        SPELL_MANNETHREL_TAUGHT = 195022,
        SPELL_FEL_AURA          = 178740,   // Immolation Aura: the fel flames about his body
        VISUAL_FEL_BURST        = 49571,    // the devastator shell's impact, a burst of fel
        GO_WARGLAIVE            = 243267,
        SAY_CANT_CONTAIN        = 0,
        SAY_SCREAM              = 1,
        SAY_KAYN_NO             = 6,
    };

    static bool Teaching(Player* player)
    {
        for (uint32 quest : { QUEST_VENGEANCE, QUEST_HAVOC })
            if (player->GetQuestStatus(quest) == QUEST_STATUS_INCOMPLETE && !player->GetQuestObjectiveData(quest, 96652))
                return true;
        return false;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 /*action*/) override
    {
        player->CLOSE_GOSSIP_MENU();
        if (!Teaching(player))
            return true;

        // the credit is the player's own spell, cast on himself
        player->CastSpell(player, SPELL_MANNETHREL_TAUGHT, true);
        if (Teaching(player))
            player->KilledMonsterCredit(creature->GetEntry());

        Creature* copy = player->SummonCreature(creature->GetEntry(), creature->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0, 0, player->GetGUID());
        if (!copy)
            return true;

        creature->AddToHideList(player->GetGUID());
        creature->DestroyForPlayer(player);
        sMannethrelHiddenFor[player->GetGUID()] = creature->GetGUID();

        copy->SetUInt32Value(UNIT_FIELD_NPC_FLAGS, 0);
        copy->SetUInt32Value(UNIT_FIELD_NPC_FLAGS + 1, 0);
        copy->SetReactState(REACT_PASSIVE);
        copy->SetStandState(UNIT_STAND_STATE_SIT);
        ObjectGuid const viewer = player->GetGUID();

        copy->AddDelayedEvent(500, [copy, viewer]() -> void
        {
            sCreatureTextMgr->SendChat(copy, SAY_CANT_CONTAIN, viewer);
        });
        copy->AddDelayedEvent(3500, [copy]() -> void
        {
            copy->SetStandState(UNIT_STAND_STATE_STAND);
            copy->CastSpell(copy, SPELL_FEL_AURA, true);
        });
        copy->AddDelayedEvent(7000, [copy, viewer]() -> void
        {
            sCreatureTextMgr->SendChat(copy, SAY_SCREAM, viewer);
        });
        copy->AddDelayedEvent(9000, [copy, viewer]() -> void
        {
            Player* player = ObjectAccessor::GetPlayer(*copy, viewer);
            if (!player)
            {
                copy->DespawnOrUnsummon();
                return;
            }

            // he is gone a blink before the burst lands (6 yards at 40 yards a second), then nothing but his two warglaives on the ground
            copy->DestroyForPlayer(player);
            // two bursts close together: the visual has no scale of its own, their spread makes the size
            for (float height : { 1.35f, 1.85f })
            {
                WorldPackets::Spells::PlayOrphanSpellVisual burst;
                burst.SourceLocation = Position(copy->GetPositionX(), copy->GetPositionY(), copy->GetPositionZ() + height + 6.0f);
                burst.SourceRotation = Position(0.0f, 0.0f, 0.0f);
                burst.TargetLocation = Position(copy->GetPositionX(), copy->GetPositionY(), copy->GetPositionZ() + height);
                burst.SpellVisualID = VISUAL_FEL_BURST;
                burst.TravelSpeed = 40.0f;
                burst.UnkFloat = 0.0f;
                burst.SpeedAsTime = false;
                player->SendDirectMessage(burst.Write());
            }

            for (float side : { -0.9f, 0.8f })
            {
                float const angle = copy->GetOrientation() + side * float(M_PI) / 2.0f;
                float x = copy->GetPositionX() + 0.8f * std::cos(angle);
                float y = copy->GetPositionY() + 0.8f * std::sin(angle);
                float z = copy->GetPositionZ();
                copy->UpdateGroundPositionZ(x, y, z);
                player->SummonGameObject(GO_WARGLAIVE, x, y, z + 0.1f, frand(0.0f, 2.0f * float(M_PI)), 0.0f, 0.0f, 0.0f, 0.0f, 120, viewer);
            }

            if (Creature* kayn = copy->FindNearestCreature(NPC_KAYN, 60.0f))
                kayn->AddDelayedEvent(1500, [kayn, viewer]() -> void
                {
                    sCreatureTextMgr->SendChat(kayn, SAY_KAYN_NO, viewer);
                });

            copy->DespawnOrUnsummon(500);
        });
        return true;
    }
};

// 100333 - Skittering Broodling: there is none to see until Tyranna is fought, then they come down their threads from under
// the Fel Hammer's vault onto her foe, and are gone again if she is left alone; the ones she calls drop the same way
struct npc_mardum_skittering_broodling : public ScriptedAI
{
    npc_mardum_skittering_broodling(Creature* creature) : ScriptedAI(creature) { }

    float const HangHeight = 16.0f;

    // these two kinds hang on their threads; the Red Broodlings on the floor are another entry and fight where they stand
    bool OnThread() const { return true; }

    void Reset() override
    {
        if (me->isSummon() || !OnThread())
        {
            _down = true;
            return;
        }
        Hang();
    }

    void IsSummonedBy(Unit* /*summoner*/) override
    {
        Hang();
        me->AddDelayedEvent(urand(200, 1200), [this]() -> void { Descend(nullptr); });
    }

    void Hang()
    {
        _down = false;
        me->SetReactState(REACT_PASSIVE);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
        me->SetDisableGravity(true);
        // some are spawned up under the vault already, the others on the floor: each hangs 16 yards over the floor below it
        Position const& home = me->isSummon() ? me->GetPosition() : me->GetHomePosition();
        float ground = me->GetMap()->GetHeight(me->GetPhases(), home.GetPositionX(), home.GetPositionY(), home.GetPositionZ() + 1.0f, true, 80.0f);
        if (ground <= INVALID_HEIGHT || ground > home.GetPositionZ() + 1.0f)
            ground = home.GetPositionZ();
        _floor.Relocate(home.GetPositionX(), home.GetPositionY(), ground, home.GetOrientation());
        float const hang = std::max(home.GetPositionZ(), ground + HangHeight);
        me->NearTeleportTo(home.GetPositionX(), home.GetPositionY(), hang, home.GetOrientation());
        // out of the fight there is no broodling to see: they only show up coming down their threads
        me->SetVisible(false);
    }

    void Descend(Unit* enemy)
    {
        if (_down)
            return;
        _down = true;
        _enemy = enemy ? enemy->GetGUID() : ObjectGuid::Empty;
        me->SetVisible(true);
        // its thread: the anchor up there spins the filament down to it (the Spiderweb Filament of Beth'tilac)
        if (!_anchor.IsEmpty())
            if (Creature* anchor = ObjectAccessor::GetCreature(*me, _anchor))
                anchor->CastSpell(me, 98623, true);
        me->GetMotionMaster()->MoveLand(1, _floor);
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type != EFFECT_MOTION_TYPE || id != 1)
            return;

        me->SetDisableGravity(false);
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
        // down: a moment later the thread is cut
        if (!_anchor.IsEmpty())
        {
            ObjectGuid const anchorGuid = _anchor;
            _anchor.Clear();
            me->AddDelayedEvent(1000, [this, anchorGuid]() -> void
            {
                if (Creature* anchor = ObjectAccessor::GetCreature(*me, anchorGuid))
                    anchor->InterruptNonMeleeSpells(false);
            });
        }
        me->SetReactState(REACT_AGGRESSIVE);
        Unit* enemy = _enemy.IsEmpty() ? nullptr : ObjectAccessor::GetUnit(*me, _enemy);
        if (!enemy)
            enemy = me->SelectNearestPlayer(40.0f);
        if (!enemy)
            if (TempSummon* summon = me->ToTempSummon())
                if (Unit* queen = summon->GetSummoner())
                    enemy = queen->getVictim();
        if (enemy && me->IsValidAttackTarget(enemy))
            AttackStart(enemy);
    }

    // Tyranna's fight: 1 begins (the guid is her foe), 2 is over
    void SetGUID(ObjectGuid const& guid, int32 id) override
    {
        if (id == 3)
        {
            _anchor = guid;
            return;
        }

        if (id == 1 && !me->isSummon() && OnThread())
        {
            // only the ones over the floor the fight is on
            Unit* foe = ObjectAccessor::GetUnit(*me, guid);
            if (!foe || std::abs(_floor.GetPositionZ() - foe->GetPositionZ()) < 12.0f)
                Descend(foe);
        }
    }

    void DoAction(int32 const action) override
    {
        if (action == 2 && _down && !me->isSummon() && me->IsAlive() && OnThread())
        {
            me->CombatStop(true);
            me->DeleteThreatList();
            Hang();
        }
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (_down)
            ScriptedAI::MoveInLineOfSight(who);
    }

    // Tyranna's fight: the player draws back 8 % of what he deals as life
    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*dmgType*/) override
    {
        if (attacker && attacker->IsPlayer() && damage && attacker->IsAlive())
            attacker->ModifyHealth(int64(damage) * 8 / 100);
    }

    void EnterEvadeMode() override
    {
        ScriptedAI::EnterEvadeMode();
        if (me->isSummon())
            me->DespawnOrUnsummon();
    }

    // back home after a fight: up its thread again
    void JustReachedHome() override
    {
        if (!me->isSummon() && OnThread())
            Hang();
    }

    void UpdateAI(uint32 /*diff*/) override
    {
        if (!_down || !UpdateVictim())
            return;
        DoMeleeAttackIfReady();
    }

private:
    bool _down = false;
    Position _floor;
    ObjectGuid _enemy;
    ObjectGuid _anchor;
};

void AddSC_Mardum()
{
    new sceneTrigger_dh_init();
    new go_q40077();
    new npc_q40077();
    new npc_q40077_1();
    RegisterCreatureAI(npc_mardum_legion_ship);
    new go_q40378();
    new go_q39279();
    new spell_legion_q39279();
    new spell_legion_197486();
    new spell_legion_197505_197598();
    new spell_legion_197523();
    new spell_legion_208121();
    new npc_q40378();
    new conversation_announcer();
    new npc_q39049();
    new go_q38759();
    new npc_q40379();
    new go_q40379();
    new go_q39050();
    new npc_q38765();
    new go_q38765();
    new npc_questgiver_93759();
    new npc_q93221_beliash();
    new npc_q39495_caza();
    new go_q38727();
    new npc_q40222_progres1();
    new npc_q39495_prolifica();
    new npc_q38728_tyranna();
    new npc_q38728_progres1();
    new go_q38729();
    new go_q39262();
    new npc_q40378_1();
    new npc_q40378_2();
    new npc_q40379_1();
    new npc_q38759_1();
    new npc_q40379_2();
    new npc_96654();
    new npc_96436();
    new npc_q39495();
    new npc_q38727();
    new npc_93127();
    new npc_c100161();
    new npc_mardum_eredar_summoner();
    RegisterGameObjectAI(go_mardum_spire_stabilizer);
    RegisterCreatureAI(npc_mardum_spire_of_woe_fire);
    RegisterCreatureAI(npc_mardum_sparring_foes);
    RegisterCreatureAI(npc_mardum_injured_demon_hunter);
    RegisterCreatureAI(npc_mardum_doom_fortress_stabilizer);
    RegisterCreatureAI(npc_mardum_skittering_broodling);
    new npc_mardum_mannethrel_darkstar();
    new player_mardum_stop_the_bombardment();
    new player_mardum_mannethrel_back();
}