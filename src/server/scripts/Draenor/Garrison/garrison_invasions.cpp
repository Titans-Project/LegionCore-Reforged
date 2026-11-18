////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "Chat.h"
#include "Containers.h"
#include "DatabaseEnv.h"
#include "DB2Stores.h"
#include "Garrison.h"
#include "GarrisonInvasion.h"
#include "GossipDef.h"
#include "Item.h"
#include "Mail.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "QuestData.h"
#include "Random.h"
#include "Scenario.h"
#include "ScenarioMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "TemporarySummon.h"
#include <cmath>
#include <limits>

/*
    Garrison invasions (patch 6.1). Taking an Apexis assault angers a faction (Player::MarkGarrisonInvasion); its
    invasion quest is then offered by the garrison sergeant until the weekly reset. Talking to him starts the
    client's scenario on the garrison map, driven here through its scenario events:
    follow the sergeant, an ambush, a timed holdout (waves, casters assaulting buildings, terrified townsfolk,
    spies), the cleanup, the calm in the town center and the final boss. Points give the weekly bags.
*/

namespace
{
    using InvasionType = GarrisonInvasion::Type;

    enum InvasionData
    {
        NPC_INVASION_CREDIT             = 80686,    // "Repel the Invasion"

        NPC_TERRIFIED_PEASANT           = 86089,
        NPC_TERRIFIED_PEON              = 86098,
        NPC_HORDE_OPPORTUNIST           = 87025,    // the spy in an Alliance garrison
        NPC_ALLIANCE_OPPORTUNIST        = 87040,
        NPC_LUNARFALL_FOOTMAN           = 77370,
        NPC_FROSTWALL_GRUNT             = 79781,

        // scenario events of the six invasion scenarios (CriteriaTree type 92), the same in all of them
        SCENARIO_EVENT_FOLLOW           = 39813,
        SCENARIO_EVENT_AMBUSH           = 41735,
        SCENARIO_EVENT_HOLDOUT          = 41721,
        SCENARIO_EVENT_CLEANUP          = 43412,
        SCENARIO_EVENT_CALM             = 41931,
        SCENARIO_EVENT_BOSS             = 41933,
        SPELL_HOLDOUT_TIMER             = 167886,   // starts the 8 min timer the holdout step shows

        BROADCAST_START_CONFIRM         = 84130,    // "Start the garrison invasion?"
        BROADCAST_PROTECT_STRUCTURES    = 86885,
        BROADCAST_ALMOST_GOT_THEM       = 90555,
        BROADCAST_STAY_FOCUSED          = 90445,
        BROADCAST_THIEF                 = 87635,
        BROADCAST_HELP                  = 81777,
        BROADCAST_ZUG_ZUG               = 91851,
        BROADCAST_FOR_THE_ALLIANCE      = 91856,
        BROADCAST_FOR_THE_HORDE         = 91853,

        GOSSIP_SENDER_INVASION          = 7110,
        DATA_TOWNSFOLK_SAVED            = 1,
        POINT_RALLY                     = 1,
        POINT_TOWN_CENTER               = 2,
    };

    // trinity_string
    uint32 const TEXT_SCORE             = 12107;
    uint32 const TEXT_SUMMARY           = 12108;
    uint32 const TEXT_ABANDONED         = 12109;
    uint32 const TEXT_BUILDING_DAMAGED  = 12110;
    uint32 const TEXT_SPY_ESCAPED       = 12111;
    uint32 const TEXT_MAIL_SUBJECT      = 12112;
    uint32 const TEXT_MAIL_BODY         = 12113;

    uint32 const MedalEvents[GarrisonInvasion::MAX_MEDALS] = { 40385, 42061, 42062, 43998 };

    uint32 const HOLDOUT_DURATION       = 8 * MINUTE * IN_MILLISECONDS;
    uint32 const STORY_HOLDOUT_DURATION = 5 * MINUTE * IN_MILLISECONDS;   // the first invasion of a tier 2 garrison
    uint32 const WAVE_INTERVAL          = 40 * IN_MILLISECONDS;
    uint32 const BUILDING_ASSAULT_TIME  = 60 * IN_MILLISECONDS;     // a caster left alive that long damages its building
    uint32 const TOWNSFOLK_FLEE_TIME    = 90 * IN_MILLISECONDS;
    uint32 const SPY_ESCAPE_TIME        = 75 * IN_MILLISECONDS;
    uint32 const LEAD_TIMEOUT           = 45 * IN_MILLISECONDS;
    uint32 const BOSS_DELAY             = 8 * IN_MILLISECONDS;
    uint32 const ABANDON_DELAY          = 2 * MINUTE * IN_MILLISECONDS;
    uint32 const SCENARIO_LINGER        = MINUTE * IN_MILLISECONDS;
    uint32 const TICK                   = IN_MILLISECONDS;

    uint32 const WAVE_SIZE              = 4;        // plus one per extra party member
    uint32 const TOUGH_WAVE_EVERY       = 3;
    uint32 const MAX_PARTY              = 3;
    uint32 const DEFENDER_COUNT         = 4;
    uint32 const BOSS_HEALTH_PCT        = 8;        // of the template health, sized for retail groups; plus 4 per extra member
    float const TOWN_CENTER_RANGE       = 15.0f;

    uint32 const POINTS_KILL            = 10;
    uint32 const POINTS_TOUGH           = 25;
    uint32 const POINTS_CASTER          = 25;
    uint32 const POINTS_SPY             = 40;
    uint32 const POINTS_TOWNSFOLK       = 25;
    uint32 const POINTS_BOSS            = 200;
    uint32 const POINTS_BUILDING        = 50;       // per assaulted building left intact
    uint32 const POINTS_DEFENDER        = 25;       // per defender alive at the end

    uint32 const BuildingAssaultTimes[] = { 60 * IN_MILLISECONDS, 180 * IN_MILLISECONDS, 300 * IN_MILLISECONDS };
    uint32 const TownsfolkTimes[]       = { 100 * IN_MILLISECONDS, 220 * IN_MILLISECONDS, 340 * IN_MILLISECONDS };
    uint32 const SpyTimes[]             = { 150 * IN_MILLISECONDS, 330 * IN_MILLISECONDS };

    struct InvasionRoster
    {
        uint32 Normal[4];
        uint32 Tough[2];
        uint32 Caster;          // channels on a building
        uint32 Ambush[2];       // three of the first, two of the second
        uint32 Boss;
        uint32 BossAdd;         // two of them come with the boss
        uint32 GossipText;      // "Prepare to battle ..."
        uint32 AlarmAlliance;
        uint32 AlarmHorde;
        uint32 Posts;
        uint32 AmbushDone;
        uint32 Calm;
        uint32 BossAlliance;
        uint32 BossHorde;
    };

    // creatures of the Lunarfall / Frostwall invasion lists that the 6.1 data left unspawned
    InvasionRoster const Rosters[GarrisonInvasion::MAX_TYPES] =
    {
        // Iron Horde: Commander Dro'gan
        { { 84339, 84340, 89241, 91599 }, { 89239, 85940 }, 89231, { 84339, 89241 }, 90841, 84339,
          87623, 87495, 83383, 90425, 90553, BROADCAST_STAY_FOCUSED, 90446, 90446 },
        // Shadow Council: Lady Fleshsear, with the Abyssal Invader the last step names
        { { 89207, 89208, 89211, 89228 }, { 89210, 89256 }, 89205, { 89207, 89211 }, 91012, 86796,
          88221, 88391, 88391, 88392, 90424, BROADCAST_STAY_FOCUSED, 88393, 88393 },
        // Ogre: Mage Lord Gogg'nathog
        { { 85937, 85938, 85936, 89170 }, { 85939, 89244 }, 89214, { 85937, 85936 }, 90995, 85938,
          87620, 87627, 87627, 87628, 87631, 87633, 87634, 90394 },
        // Primals: Teluur and its tenders
        { { 84310, 84313, 89268, 84311 }, { 84312, 84309 }, 89268, { 84310, 84310 }, 90946, 84313,
          85891, 90415, 90415, 90416, 90424, BROADCAST_STAY_FOCUSED, 90556, 91636 },
        // Breakers: Gaur
        { { 83737, 82336, 89143, 82779 }, { 80980, 82686 }, 89187, { 83737, 82336 }, 90943, 80980,
          84129, 90411, 83384, 90412, 90400, BROADCAST_STAY_FOCUSED, 90556, 91635 },
        // Shadowmoon: Annihilon
        { { 88604, 88605, 89186, 85470 }, { 88609, 89178 }, 85469, { 88604, 89277 }, 90802, 89167,
          86825, 86879, 86879, 86880, 86884, 86887, 86888, 91636 },
    };

    // where the invaders come from, around each garrison; the town center is the sergeant's spawn
    struct InvasionSite
    {
        uint32 MapId;
        Position Entries[4];
    };

    InvasionSite const Sites[] =
    {
        { 1158, { { 1730.00f,  134.23f,  75.98f }, { 1892.76f,   97.48f,  84.22f }, { 1982.00f,  165.81f,  82.47f }, { 1937.98f,  349.62f,  88.55f } } },
        { 1331, { { 1803.31f,  291.86f,  76.72f }, { 1713.25f,  120.58f,  76.54f }, { 1940.51f,   76.76f,  85.51f }, { 2023.01f,  187.30f,  84.57f } } },
        { 1159, { { 1754.04f,  291.62f,  78.50f }, { 1933.23f,   60.98f,  85.62f }, { 2025.15f,  166.87f,  84.57f }, { 1912.98f,  350.53f,  89.05f } } },
        { 1152, { { 5761.24f, 4480.75f, 132.77f }, { 5456.35f, 4638.83f, 134.58f }, { 5581.63f, 4390.08f, 138.88f }, { 5711.65f, 4629.60f, 146.06f } } },
        { 1330, { { 5713.26f, 4603.64f, 144.31f }, { 5424.56f, 4580.40f, 136.51f }, { 5571.96f, 4403.08f, 135.98f }, { 5809.83f, 4506.75f, 126.61f } } },
        { 1153, { { 5727.08f, 4596.88f, 142.76f }, { 5424.56f, 4580.40f, 136.51f }, { 5768.41f, 4483.16f, 136.41f }, { 5382.35f, 4419.62f, 151.36f } } },
    };

    InvasionSite const* FindSite(uint32 mapId)
    {
        for (InvasionSite const& site : Sites)
            if (site.MapId == mapId)
                return &site;

        return nullptr;
    }

    // the first invasion of a tier 2 garrison, part of its questline: shorter, without medals nor weekly bags
    struct StoryInvasion
    {
        uint32 QuestId;
        InvasionType Type;
        uint32 ScenarioId;
    };

    StoryInvasion const StoryInvasions[] =
    {
        { 37289, GarrisonInvasion::TYPE_SHADOWMOON, 745 },  // Shadowmoon Invasion! (Alliance), "Against the Void"
        { 37291, GarrisonInvasion::TYPE_IRON_HORDE, 742 },  // Thunderlord Invasion! (Horde), "Thunder in Frostfire"
    };

    struct InvasionChoice
    {
        InvasionType Type = GarrisonInvasion::MAX_TYPES;
        uint32 QuestId = 0;
        uint32 ScenarioId = 0;
        bool Story = false;
    };

    std::string GetBroadcastText(uint32 id, Player const* player)
    {
        BroadcastTextEntry const* text = sBroadcastTextStore.LookupEntry(id);
        if (!text)
            return "";

        return DB2Manager::GetBroadcastTextValue(text, player->GetSession()->GetSessionDbLocaleIndex(), player->getGender());
    }

    uint32 GetInvasionQuest(InvasionType type, Player const* player)
    {
        return player->GetTeam() == ALLIANCE ? GarrisonInvasion::Types[type].QuestAlliance : GarrisonInvasion::Types[type].QuestHorde;
    }

    enum InvasionStage
    {
        STAGE_IDLE,
        STAGE_LEAD,
        STAGE_AMBUSH,
        STAGE_HOLDOUT,
        STAGE_CLEANUP,
        STAGE_CALM,
        STAGE_BOSS,
    };

    enum UnitRole
    {
        ROLE_INVADER,
        ROLE_TOUGH,
        ROLE_CASTER,
        ROLE_BOSS,
        ROLE_SPY,
        ROLE_TOWNSFOLK,
        ROLE_DEFENDER,
    };

    enum SergeantEvents
    {
        EVENT_TICK = 1,
        EVENT_LEAD_TIMEOUT,
        EVENT_WAVE,
        EVENT_HOLDOUT_END,
        EVENT_BUILDING_ASSAULT,
        EVENT_TOWNSFOLK,
        EVENT_SPY,
        EVENT_BOSS_ARRIVES,
        EVENT_REMOVE_SCENARIO,
    };

    struct InvasionUnit
    {
        UnitRole Role;
        uint32 Timer;       // casters, spies and townsfolk act when it runs out
    };
}

// 78564 Sergeant Crowler, 79774 Sergeant Grimjaw (tier 1 and 3), 88223 / 88228 (tier 2)
class npc_garrison_invasion_sergeant : public CreatureScript
{
public:
    npc_garrison_invasion_sergeant() : CreatureScript("npc_garrison_invasion_sergeant") { }

    struct npc_garrison_invasion_sergeantAI : public ScriptedAI
    {
        npc_garrison_invasion_sergeantAI(Creature* creature) : ScriptedAI(creature) { }

        InvasionChoice GetStartableInvasion(Player* player) const
        {
            InvasionChoice choice;
            if (_stage != STAGE_IDLE || !FindSite(me->GetMapId()))
                return choice;

            for (StoryInvasion const& story : StoryInvasions)
            {
                if (player->GetQuestStatus(story.QuestId) == QUEST_STATUS_INCOMPLETE)
                {
                    choice = { story.Type, story.QuestId, story.ScenarioId, true };
                    return choice;
                }
            }

            for (uint8 type = 0; type < GarrisonInvasion::MAX_TYPES; ++type)
            {
                uint32 const questId = GetInvasionQuest(InvasionType(type), player);
                if (player->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE)
                {
                    choice = { InvasionType(type), questId, GarrisonInvasion::Types[type].ScenarioId, false };
                    return choice;
                }
            }

            return choice;
        }

        void StartInvasion(Player* starter, uint32 questId)
        {
            InvasionChoice const choice = GetStartableInvasion(starter);
            if (!choice.QuestId || choice.QuestId != questId)
                return;

            InvasionType const type = choice.Type;

            Map* map = me->GetMap();
            // what is left of the previous invasion of this garrison
            sScenarioMgr->RemoveScenario(map);
            Scenario* scenario = sScenarioMgr->AddScenario(map, choice.ScenarioId);
            if (!scenario)
                return;

            _site = FindSite(me->GetMapId());
            _type = type;
            _story = choice.Story;
            _questId = questId;
            _roster = &Rosters[type];
            _alliance = starter->GetTeam() == ALLIANCE;
            _starterGUID = starter->GetGUID();
            _home = me->GetHomePosition();
            _units.clear();
            _points = 0;
            _killed = 0;
            _townsfolkSaved = 0;
            _spiesCaught = 0;
            _buildingsAssaulted = 0;
            _buildingsDamaged = 0;
            _defendersLost = 0;
            _waves = 0;
            _emptyTimer = 0;
            _bossCalled = false;
            _partySize = std::min(MAX_PARTY, std::max(1u, CountPlayers(true)));

            scenario->SetCurrentStep(0);

            // the "Speak with the sergeant" objective of everyone here on the same invasion
            bool const story = _story;
            map->ApplyOnEveryPlayer([type, story, questId](Player* player)
            {
                if (Quest const* quest = sQuestDataStore->GetQuestTemplate(story ? questId : GetInvasionQuest(type, player)))
                    if (player->GetQuestStatus(quest->GetQuestId()) == QUEST_STATUS_INCOMPLETE)
                        for (QuestObjective const& objective : quest->GetObjectives())
                            if (objective.Type == QUEST_OBJECTIVE_MONSTER && objective.ObjectID != NPC_INVASION_CREDIT)
                                player->KilledMonsterCredit(objective.ObjectID);
            });

            Yell(_alliance ? _roster->AlarmAlliance : _roster->AlarmHorde);

            _stage = STAGE_LEAD;
            me->setActive(true);
            me->SetReactState(REACT_PASSIVE);
            me->SetWalk(false);
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MovePoint(POINT_RALLY, GetRallyPoint());

            _events.Reset();
            _events.ScheduleEvent(EVENT_TICK, TICK);
            _events.ScheduleEvent(EVENT_LEAD_TIMEOUT, LEAD_TIMEOUT);
        }

        void SetGUID(ObjectGuid const& guid, int32 id) override
        {
            if (id == DATA_TOWNSFOLK_SAVED)
                RescueTownsfolk(guid);
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == POINT_MOTION_TYPE && id == POINT_RALLY && _stage == STAGE_LEAD)
                StartAmbush();
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType /*dmgType*/) override
        {
            // he gives the quests and sells the garrison's goods: the invaders only knock him down
            if (damage >= me->GetHealth())
                damage = me->GetHealth() > 1 ? uint32(me->GetHealth() - 1) : 0;
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/) override
        {
            auto itr = _units.find(summon->GetGUID());
            if (itr == _units.end())
                return;

            UnitRole const role = itr->second.Role;
            _units.erase(itr);

            switch (role)
            {
                case ROLE_INVADER:
                    _points += POINTS_KILL;
                    ++_killed;
                    break;
                case ROLE_TOUGH:
                    _points += POINTS_TOUGH;
                    ++_killed;
                    break;
                case ROLE_CASTER:
                    _points += POINTS_CASTER;
                    ++_killed;
                    break;
                case ROLE_BOSS:
                    _points += POINTS_BOSS;
                    ++_killed;
                    break;
                case ROLE_SPY:
                    _points += POINTS_SPY;
                    ++_spiesCaught;
                    break;
                case ROLE_DEFENDER:
                    ++_defendersLost;
                    break;
                default:
                    break;
            }

            CheckStageEnd();
        }

        void SummonedCreatureDespawn(Creature* summon) override
        {
            _units.erase(summon->GetGUID());
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_TICK:
                        Tick();
                        if (_stage != STAGE_IDLE)
                            _events.ScheduleEvent(EVENT_TICK, TICK);
                        break;
                    case EVENT_LEAD_TIMEOUT:
                        // a path that cannot be walked must not stall the invasion
                        if (_stage == STAGE_LEAD)
                            StartAmbush();
                        break;
                    case EVENT_WAVE:
                        SpawnWave();
                        if (++_waves * WAVE_INTERVAL + 20 * IN_MILLISECONDS < GetHoldoutDuration())
                            _events.ScheduleEvent(EVENT_WAVE, WAVE_INTERVAL);
                        break;
                    case EVENT_HOLDOUT_END:
                        StartCleanup();
                        break;
                    case EVENT_BUILDING_ASSAULT:
                        SpawnBuildingAssault();
                        break;
                    case EVENT_TOWNSFOLK:
                        SpawnTownsfolk();
                        break;
                    case EVENT_SPY:
                        SpawnSpy();
                        break;
                    case EVENT_BOSS_ARRIVES:
                        SpawnBoss();
                        break;
                    case EVENT_REMOVE_SCENARIO:
                        if (_stage == STAGE_IDLE)
                            sScenarioMgr->RemoveScenario(me->GetMap());
                        break;
                    default:
                        break;
                }
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

    private:
        uint32 GetHoldoutDuration() const
        {
            return _story ? STORY_HOLDOUT_DURATION : HOLDOUT_DURATION;
        }

        void Yell(uint32 broadcastTextId)
        {
            if (broadcastTextId)
                me->Talk(broadcastTextId, CHAT_MSG_MONSTER_YELL, 150.0f, nullptr);
        }

        uint32 CountPlayers(bool fightersOnly) const
        {
            uint32 count = 0;
            me->GetMap()->ApplyOnEveryPlayer([&count, fightersOnly](Player* player)
            {
                if (!fightersOnly || !player->isGameMaster())
                    ++count;
            });
            return count;
        }

        // the scenario only counts what a player outside GM mode does
        Player* GetReferencePlayer() const
        {
            Player* reference = nullptr;
            me->GetMap()->ApplyOnEveryPlayer([&reference](Player* player)
            {
                if (!reference && !player->isGameMaster())
                    reference = player;
            });
            return reference;
        }

        // scenarios are stored by instance id, which a dungeon can share with a garrison (its owner's guid)
        Scenario* GetOwnScenario() const
        {
            Scenario* scenario = sScenarioMgr->GetScenario(me->GetMap()->GetInstanceId());
            return scenario && scenario->GetMap() == me->GetMap() ? scenario : nullptr;
        }

        void SendScenarioEvent(uint32 eventId)
        {
            Scenario* scenario = GetOwnScenario();
            Player* reference = GetReferencePlayer();
            if (scenario && reference)
                scenario->UpdateAchievementCriteria(CRITERIA_TYPE_SCRIPT_EVENT_2, eventId, 0, 0, nullptr, reference);
        }

        void AnnounceScore()
        {
            uint32 const points = _points;
            me->GetMap()->ApplyOnEveryPlayer([points](Player* player)
            {
                ChatHandler(player->GetSession()).PSendSysMessage(TEXT_SCORE, points);
            });
        }

        void Announce(uint32 textId)
        {
            me->GetMap()->ApplyOnEveryPlayer([textId](Player* player)
            {
                ChatHandler(player->GetSession()).SendSysMessage(int32(textId));
            });
        }

        Position GetRallyPoint() const
        {
            Position const& entry = _site->Entries[0];
            float x = _home.GetPositionX() + (entry.GetPositionX() - _home.GetPositionX()) * 0.6f;
            float y = _home.GetPositionY() + (entry.GetPositionY() - _home.GetPositionY()) * 0.6f;
            float z = std::max(_home.GetPositionZ(), entry.GetPositionZ()) + 5.0f;
            me->UpdateGroundPositionZ(x, y, z);
            return Position(x, y, z, _home.GetAngle(&entry));
        }

        Position Scatter(Position const& center, float radius) const
        {
            float x = center.GetPositionX() + frand(-radius, radius);
            float y = center.GetPositionY() + frand(-radius, radius);
            float z = center.GetPositionZ() + 3.0f;
            me->UpdateGroundPositionZ(x, y, z);
            return Position(x, y, z, center.GetOrientation());
        }

        // a point between a building and the town center, out of the building's walls
        Position NearBuilding(Position const& building, float distance) const
        {
            float const angle = building.GetAngle(&_home);
            float x = building.GetPositionX() + std::cos(angle) * distance;
            float y = building.GetPositionY() + std::sin(angle) * distance;
            float z = std::max(building.GetPositionZ(), _home.GetPositionZ()) + 5.0f;
            me->UpdateGroundPositionZ(x, y, z);
            return Position(x, y, z, Position::NormalizeOrientation(angle + float(M_PI)));
        }

        // plots of the garrison of the player who started it; the built ones first
        std::vector<Position> GetBuildingPositions() const
        {
            Garrison* garrison = nullptr;
            if (Player* starter = ObjectAccessor::GetPlayer(*me, _starterGUID))
                if (starter->GetGarrisonPtr() && starter->GetGarrisonPtr()->GetGarrisonMapID() == int32(me->GetMapId()))
                    garrison = starter->GetGarrisonPtr();

            if (!garrison)
                me->GetMap()->ApplyOnEveryPlayer([this, &garrison](Player* player)
                {
                    if (!garrison && player->GetGarrisonPtr() && player->GetGarrisonPtr()->GetGarrisonMapID() == int32(me->GetMapId()))
                        garrison = player->GetGarrisonPtr();
                });

            std::vector<Position> built, all;
            if (garrison)
            {
                for (Plot* plot : garrison->GetPlots())
                {
                    if (plot->PacketInfo.GarrPlotInstanceID == GarrisonConst::Globals::ShipyardPlotID)
                        continue;

                    Position const pos = plot->PacketInfo.PlotPos;
                    all.push_back(pos);
                    if (plot->BuildingInfo.PacketInfo)
                        built.push_back(pos);
                }
            }

            return built.empty() ? all : built;
        }

        Unit* PickTarget() const
        {
            std::vector<Unit*> targets;
            me->GetMap()->ApplyOnEveryPlayer([&targets](Player* player)
            {
                if (player->IsAlive() && !player->isGameMaster())
                    targets.push_back(player);
            });

            if (targets.empty())
                for (auto const& unit : _units)
                    if (unit.second.Role == ROLE_DEFENDER)
                        if (Creature* defender = ObjectAccessor::GetCreature(*me, unit.first))
                            if (defender->IsAlive())
                                targets.push_back(defender);

            if (targets.empty())
                return me;

            return Trinity::Containers::SelectRandomContainerElement(targets);
        }

        Creature* Summon(uint32 entry, Position const& pos, UnitRole role, uint32 timer = 0)
        {
            // the living stay until the invasion sends them away, the dead are cleared
            TempSummon* summon = me->SummonCreature(entry, pos, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15 * IN_MILLISECONDS);
            if (!summon)
                return nullptr;

            // several of these templates keep the sniffed state of a scripted entrance (unselectable, feigning death)
            if (role != ROLE_TOWNSFOLK)
            {
                summon->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_PACIFIED | UNIT_FLAG_STUNNED);
                summon->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
            }

            _units[summon->GetGUID()] = { role, timer };
            return summon;
        }

        Creature* SummonAttacker(uint32 entry, Position const& pos, UnitRole role)
        {
            Creature* invader = Summon(entry, pos, role);
            if (invader && invader->IsAIEnabled)
                if (Unit* target = PickTarget())
                    invader->AI()->AttackStart(target);

            return invader;
        }

        bool IsInvader(UnitRole role) const
        {
            return role == ROLE_INVADER || role == ROLE_TOUGH || role == ROLE_CASTER || role == ROLE_BOSS;
        }

        uint32 CountInvaders()
        {
            uint32 count = 0;
            for (auto itr = _units.begin(); itr != _units.end();)
            {
                Creature* unit = ObjectAccessor::GetCreature(*me, itr->first);
                if (!unit || !unit->IsAlive())
                {
                    itr = _units.erase(itr);
                    continue;
                }

                if (IsInvader(itr->second.Role))
                    ++count;
                ++itr;
            }
            return count;
        }

        void DespawnRole(UnitRole role)
        {
            for (auto itr = _units.begin(); itr != _units.end();)
            {
                if (itr->second.Role != role)
                {
                    ++itr;
                    continue;
                }

                ObjectGuid const guid = itr->first;
                itr = _units.erase(itr);
                if (Creature* unit = ObjectAccessor::GetCreature(*me, guid))
                    unit->DespawnOrUnsummon();
            }
        }

        void Tick()
        {
            if (_stage == STAGE_IDLE)
                return;

            // nobody left to defend the garrison
            if (!CountPlayers(false))
            {
                _emptyTimer += TICK;
                if (_emptyTimer >= ABANDON_DELAY)
                {
                    Abandon();
                    return;
                }
            }
            else
                _emptyTimer = 0;

            std::vector<std::pair<ObjectGuid, UnitRole>> expired;
            for (auto& unit : _units)
            {
                if (!unit.second.Timer)
                    continue;

                if (unit.second.Timer > TICK)
                    unit.second.Timer -= TICK;
                else
                {
                    unit.second.Timer = 0;
                    expired.emplace_back(unit.first, unit.second.Role);
                }
            }

            for (auto const& unit : expired)
            {
                Creature* creature = ObjectAccessor::GetCreature(*me, unit.first);
                if (!creature || !creature->IsAlive())
                    continue;

                switch (unit.second)
                {
                    case ROLE_CASTER:
                        // the building is damaged: the caster turns on the defenders
                        ++_buildingsDamaged;
                        Announce(TEXT_BUILDING_DAMAGED);
                        creature->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                        creature->SetReactState(REACT_AGGRESSIVE);
                        if (creature->IsAIEnabled)
                            if (Unit* target = PickTarget())
                                creature->AI()->AttackStart(target);
                        break;
                    case ROLE_SPY:
                        Announce(TEXT_SPY_ESCAPED);
                        _units.erase(unit.first);
                        creature->DespawnOrUnsummon();
                        break;
                    case ROLE_TOWNSFOLK:
                        _units.erase(unit.first);
                        creature->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }
            }

            CheckStageEnd();

            if (_stage == STAGE_CALM && !_bossCalled)
            {
                bool inCenter = false;
                me->GetMap()->ApplyOnEveryPlayer([this, &inCenter](Player* player)
                {
                    if (!player->isGameMaster() && player->IsAlive() && player->GetExactDist2d(&_home) < TOWN_CENTER_RANGE)
                        inCenter = true;
                });

                if (inCenter)
                {
                    _bossCalled = true;
                    SendScenarioEvent(SCENARIO_EVENT_CALM);
                    _events.ScheduleEvent(EVENT_BOSS_ARRIVES, BOSS_DELAY);
                }
            }
        }

        void CheckStageEnd()
        {
            switch (_stage)
            {
                case STAGE_AMBUSH:
                    if (!CountInvaders())
                        StartHoldout();
                    break;
                case STAGE_CLEANUP:
                    if (!CountInvaders())
                        StartCalm();
                    break;
                case STAGE_BOSS:
                    if (!CountInvaders())
                        Victory();
                    break;
                default:
                    break;
            }
        }

        void StartAmbush()
        {
            _events.CancelEvent(EVENT_LEAD_TIMEOUT);
            SendScenarioEvent(SCENARIO_EVENT_FOLLOW);
            _stage = STAGE_AMBUSH;

            // he holds the rally point until the ambush is beaten
            me->SetHomePosition(me->GetPosition());
            me->SetReactState(REACT_AGGRESSIVE);
            Yell(_roster->Posts);

            for (uint8 i = 0; i < 5; ++i)
                SummonAttacker(_roster->Ambush[i < 3 ? 0 : 1], Scatter(_site->Entries[0], 6.0f), ROLE_INVADER);
        }

        void StartHoldout()
        {
            SendScenarioEvent(SCENARIO_EVENT_AMBUSH);
            _stage = STAGE_HOLDOUT;
            Yell(_roster->AmbushDone);
            AnnounceScore();

            me->SetHomePosition(_home);
            if (!me->isInCombat())
                me->GetMotionMaster()->MovePoint(POINT_TOWN_CENTER, _home);

            if (Scenario* scenario = GetOwnScenario())
                scenario->GetAchievementMgr().StartTimedAchievement(CRITERIA_TIMED_TYPE_SPELL_TARGET2, SPELL_HOLDOUT_TIMER);

            uint32 const defender = _alliance ? NPC_LUNARFALL_FOOTMAN : NPC_FROSTWALL_GRUNT;
            for (uint8 i = 0; i < DEFENDER_COUNT; ++i)
            {
                float const angle = 2.0f * float(M_PI) * i / DEFENDER_COUNT;
                float x = _home.GetPositionX() + 6.0f * std::cos(angle);
                float y = _home.GetPositionY() + 6.0f * std::sin(angle);
                float z = _home.GetPositionZ() + 3.0f;
                me->UpdateGroundPositionZ(x, y, z);
                Summon(defender, Position(x, y, z, angle), ROLE_DEFENDER);
            }

            _events.ScheduleEvent(EVENT_WAVE, 5 * IN_MILLISECONDS);
            _events.ScheduleEvent(EVENT_HOLDOUT_END, GetHoldoutDuration());
            for (uint32 time : BuildingAssaultTimes)
                _events.ScheduleEvent(EVENT_BUILDING_ASSAULT, time);
            for (uint32 time : TownsfolkTimes)
                _events.ScheduleEvent(EVENT_TOWNSFOLK, time);
            for (uint32 time : SpyTimes)
                _events.ScheduleEvent(EVENT_SPY, time);
        }

        void SpawnWave()
        {
            if (_stage != STAGE_HOLDOUT)
                return;

            Position const& entry = _site->Entries[_waves % 4];
            uint32 const count = WAVE_SIZE + _partySize - 1;
            for (uint32 i = 0; i < count; ++i)
                SummonAttacker(_roster->Normal[urand(0, 3)], Scatter(entry, 6.0f), ROLE_INVADER);

            if ((_waves + 1) % TOUGH_WAVE_EVERY == 0)
                SummonAttacker(_roster->Tough[urand(0, 1)], Scatter(entry, 4.0f), ROLE_TOUGH);
        }

        void SpawnBuildingAssault()
        {
            if (_stage != STAGE_HOLDOUT)
                return;

            std::vector<Position> buildings = GetBuildingPositions();
            if (buildings.empty())
                return;

            Position const& building = buildings[_buildingsAssaulted % buildings.size()];
            Creature* caster = Summon(_roster->Caster, NearBuilding(building, 10.0f), ROLE_CASTER, BUILDING_ASSAULT_TIME);
            if (!caster)
                return;

            ++_buildingsAssaulted;
            caster->SetReactState(REACT_PASSIVE);
            caster->SetFacingTo(caster->GetAngle(&building));
            caster->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_SPELL_CHANNEL_DIRECTED);
            Yell(BROADCAST_PROTECT_STRUCTURES);
        }

        void SpawnTownsfolk()
        {
            if (_stage != STAGE_HOLDOUT)
                return;

            std::vector<Position> buildings = GetBuildingPositions();
            if (buildings.empty())
                return;

            Position const& building = Trinity::Containers::SelectRandomContainerElement(buildings);
            Creature* townsfolk = Summon(_alliance ? NPC_TERRIFIED_PEASANT : NPC_TERRIFIED_PEON, NearBuilding(building, 7.0f), ROLE_TOWNSFOLK, TOWNSFOLK_FLEE_TIME);
            if (!townsfolk)
                return;

            townsfolk->SetReactState(REACT_PASSIVE);
            townsfolk->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            townsfolk->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_COWER);
            townsfolk->Talk(BROADCAST_HELP, CHAT_MSG_MONSTER_SAY, 40.0f, nullptr);
        }

        void RescueTownsfolk(ObjectGuid const& guid)
        {
            auto itr = _units.find(guid);
            if (itr == _units.end() || itr->second.Role != ROLE_TOWNSFOLK)
                return;

            _units.erase(itr);
            _points += POINTS_TOWNSFOLK;
            ++_townsfolkSaved;

            if (Creature* townsfolk = ObjectAccessor::GetCreature(*me, guid))
            {
                townsfolk->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                townsfolk->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                if (!_alliance)
                    townsfolk->Talk(BROADCAST_ZUG_ZUG, CHAT_MSG_MONSTER_SAY, 40.0f, nullptr);
                townsfolk->SetWalk(false);
                townsfolk->GetMotionMaster()->MovePoint(0, _home);
                townsfolk->DespawnOrUnsummon(8 * IN_MILLISECONDS);
            }
        }

        void SpawnSpy()
        {
            if (_stage != STAGE_HOLDOUT)
                return;

            std::vector<Position> buildings = GetBuildingPositions();
            if (buildings.empty())
                return;

            Position const& building = Trinity::Containers::SelectRandomContainerElement(buildings);
            Creature* spy = Summon(_alliance ? NPC_HORDE_OPPORTUNIST : NPC_ALLIANCE_OPPORTUNIST, NearBuilding(building, 6.0f), ROLE_SPY, SPY_ESCAPE_TIME);
            if (!spy)
                return;

            spy->SetReactState(REACT_DEFENSIVE);
            spy->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_LOOT);
            Yell(BROADCAST_THIEF);
        }

        void StartCleanup()
        {
            SendScenarioEvent(SCENARIO_EVENT_HOLDOUT);
            _stage = STAGE_CLEANUP;
            _events.CancelEvent(EVENT_WAVE);
            _events.CancelEvent(EVENT_BUILDING_ASSAULT);
            _events.CancelEvent(EVENT_TOWNSFOLK);
            _events.CancelEvent(EVENT_SPY);

            // the ones not reached in time have run off
            DespawnRole(ROLE_TOWNSFOLK);
            DespawnRole(ROLE_SPY);

            Yell(BROADCAST_ALMOST_GOT_THEM);
            AnnounceScore();
            CheckStageEnd();
        }

        void StartCalm()
        {
            SendScenarioEvent(SCENARIO_EVENT_CLEANUP);
            _stage = STAGE_CALM;
            Yell(_roster->Calm);
            AnnounceScore();

            if (!me->isInCombat())
                me->GetMotionMaster()->MovePoint(POINT_TOWN_CENTER, _home);
        }

        void SpawnBoss()
        {
            if (_stage != STAGE_CALM)
                return;

            _stage = STAGE_BOSS;
            Yell(_alliance ? _roster->BossAlliance : _roster->BossHorde);

            Position const& entry = _site->Entries[1];
            if (Creature* boss = SummonAttacker(_roster->Boss, Scatter(entry, 2.0f), ROLE_BOSS))
            {
                uint64 const health = std::max<uint64>(1, boss->GetMaxHealth() * (BOSS_HEALTH_PCT + 4 * (_partySize - 1)) / 100);
                boss->SetCreateHealth(uint32(std::min<uint64>(health, std::numeric_limits<uint32>::max())));
                boss->SetMaxHealth(health);
                boss->SetFullHealth();
            }

            for (uint8 i = 0; i < 2; ++i)
                SummonAttacker(_roster->BossAdd, Scatter(entry, 5.0f), ROLE_TOUGH);
        }

        void Victory()
        {
            uint32 const intact = _buildingsAssaulted > _buildingsDamaged ? _buildingsAssaulted - _buildingsDamaged : 0;
            uint32 const defendersAlive = DEFENDER_COUNT > _defendersLost ? DEFENDER_COUNT - _defendersLost : 0;
            _points += intact * POINTS_BUILDING + defendersAlive * POINTS_DEFENDER;

            // the last main step completes the scenario, the medals are its bonus steps after it
            SendScenarioEvent(SCENARIO_EVENT_BOSS);
            for (uint8 medal = 0; !_story && medal < GarrisonInvasion::MAX_MEDALS && _points >= GarrisonInvasion::Medals[medal].Points; ++medal)
                SendScenarioEvent(MedalEvents[medal]);

            InvasionType const type = _type;
            bool const story = _story;
            uint32 const questId = _questId;
            uint32 const sergeant = me->GetEntry();
            uint32 const points = _points, killed = _killed, saved = _townsfolkSaved, spies = _spiesCaught, assaulted = _buildingsAssaulted;
            me->GetMap()->ApplyOnEveryPlayer([=](Player* player)
            {
                if (player->GetQuestStatus(story ? questId : GetInvasionQuest(type, player)) == QUEST_STATUS_INCOMPLETE)
                    player->KilledMonsterCredit(NPC_INVASION_CREDIT);

                // a bag of each medal reached, once a week each
                for (uint8 medal = 0; !story && medal < GarrisonInvasion::MAX_MEDALS && points >= GarrisonInvasion::Medals[medal].Points; ++medal)
                    if (!player->IsQuestWeekRewarded(GarrisonInvasion::Medals[medal].RewardTracking))
                        GiveBag(player, GarrisonInvasion::Medals[medal], sergeant);

                ChatHandler(player->GetSession()).PSendSysMessage(TEXT_SUMMARY, points, killed, saved, spies, intact, assaulted, defendersAlive, DEFENDER_COUNT);
            });

            Yell(_alliance ? BROADCAST_FOR_THE_ALLIANCE : BROADCAST_FOR_THE_HORDE);
            EndInvasion();
        }

        // in the bags, or by mail when they are full
        static void GiveBag(Player* player, GarrisonInvasion::MedalInfo const& medal, uint32 senderEntry)
        {
            player->SetWeeklyTrackingQuest(medal.RewardTracking);

            ItemPosCountVec dest;
            if (player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, medal.Item, 1) == EQUIP_ERR_OK)
            {
                if (Item* item = player->StoreNewItem(dest, medal.Item, true, Item::GenerateItemRandomPropertyId(medal.Item, player->GetLootSpecID())))
                    player->SendNewItem(item, 1, true, false);
                return;
            }

            LocaleConstant const locale = player->GetSession()->GetSessionDbLocaleIndex();
            CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
            MailDraft draft(sObjectMgr->GetTrinityString(TEXT_MAIL_SUBJECT, locale), sObjectMgr->GetTrinityString(TEXT_MAIL_BODY, locale));
            if (Item* item = Item::CreateItem(medal.Item, 1, player))
            {
                item->SaveToDB(trans);
                draft.AddItem(item);
            }
            draft.SendMailTo(trans, player, MailSender(MAIL_CREATURE, uint64(senderEntry)), MAIL_CHECK_MASK_COPIED);
            CharacterDatabase.CommitTransaction(trans);
        }

        void Abandon()
        {
            Announce(TEXT_ABANDONED);
            EndInvasion();
            sScenarioMgr->RemoveScenario(me->GetMap());
        }

        void EndInvasion()
        {
            // unsummoning calls SummonedCreatureDespawn, which edits the list
            std::unordered_map<ObjectGuid, InvasionUnit> units;
            units.swap(_units);
            for (auto const& unit : units)
                if (Creature* creature = ObjectAccessor::GetCreature(*me, unit.first))
                    creature->DespawnOrUnsummon(unit.second.Role == ROLE_DEFENDER ? 10 * IN_MILLISECONDS : 0);

            _stage = STAGE_IDLE;
            _events.Reset();
            _events.ScheduleEvent(EVENT_REMOVE_SCENARIO, SCENARIO_LINGER);

            me->setActive(false);
            me->SetReactState(REACT_AGGRESSIVE);
            me->SetHomePosition(_home);
            if (!me->isInCombat())
                me->GetMotionMaster()->MovePoint(POINT_TOWN_CENTER, _home);
        }

        EventMap _events;
        std::unordered_map<ObjectGuid, InvasionUnit> _units;
        InvasionSite const* _site = nullptr;
        InvasionRoster const* _roster = nullptr;
        InvasionType _type = GarrisonInvasion::MAX_TYPES;
        uint32 _questId = 0;
        bool _story = false;
        InvasionStage _stage = STAGE_IDLE;
        ObjectGuid _starterGUID;
        Position _home;
        bool _alliance = true;
        uint32 _partySize = 1;
        uint32 _points = 0;
        uint32 _killed = 0;
        uint32 _townsfolkSaved = 0;
        uint32 _spiesCaught = 0;
        uint32 _buildingsAssaulted = 0;
        uint32 _buildingsDamaged = 0;
        uint32 _defendersLost = 0;
        uint32 _waves = 0;
        uint32 _emptyTimer = 0;
        bool _bossCalled = false;
    };

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        auto ai = dynamic_cast<npc_garrison_invasion_sergeantAI*>(creature->AI());
        if (!ai)
            return false;

        // the sergeant is also a quest objective to talk to (New Goods)
        player->TalkedToCreature(creature->GetEntry(), creature->GetGUID());

        player->PlayerTalkClass->ClearMenus();
        player->PrepareGossipMenu(creature, creature->GetCreatureTemplate()->GossipMenuId, true);

        InvasionChoice const choice = ai->GetStartableInvasion(player);
        if (choice.QuestId)
            player->PlayerTalkClass->GetGossipMenu().AddMenuItem(-1, GossipOptionNpc::None, GetBroadcastText(Rosters[choice.Type].GossipText, player),
                GOSSIP_SENDER_INVASION, choice.QuestId, GetBroadcastText(BROADCAST_START_CONFIRM, player), 0);

        player->SendPreparedGossip(creature);
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        if (sender != GOSSIP_SENDER_INVASION)
            return false;

        player->PlayerTalkClass->SendCloseGossip();

        if (auto ai = dynamic_cast<npc_garrison_invasion_sergeantAI*>(creature->AI()))
            ai->StartInvasion(player, action);

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_garrison_invasion_sergeantAI(creature);
    }
};

// 86089 Terrified Peasant, 86098 Terrified Peon: talking to them sends them to the town center
class npc_garrison_invasion_townsfolk : public CreatureScript
{
public:
    npc_garrison_invasion_townsfolk() : CreatureScript("npc_garrison_invasion_townsfolk") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        player->PlayerTalkClass->SendCloseGossip();

        if (TempSummon* summon = creature->ToTempSummon())
            if (Unit* summoner = summon->GetSummoner())
                if (Creature* sergeant = summoner->ToCreature())
                    if (sergeant->IsAIEnabled)
                        sergeant->AI()->SetGUID(creature->GetGUID(), DATA_TOWNSFOLK_SAVED);

        return true;
    }
};

void AddSC_garrison_invasions()
{
    new npc_garrison_invasion_sergeant();
    new npc_garrison_invasion_townsfolk();
}
