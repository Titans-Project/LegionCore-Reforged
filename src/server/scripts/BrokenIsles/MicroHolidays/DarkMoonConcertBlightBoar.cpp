////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "MoveSpline.h"
#include "MoveSplineInit.h"
#include "Group.h"
#include "QuestData.h"
#include "SpellAuraEffects.h"
#include <numeric>

enum eSay
{
    SAY_JAY_MAGUIRE_20_MIN_LEFT         = 0,
    SAY_JAY_MAGUIRE_10_MIN_LEFT,
    SAY_JAY_MAGUIRE_1_MIN_LEFT,
    SAY_JAY_MAGUIRE_START_CONCERT,
    SAY_JAY_MAGUIRE_30_MIN_LEFT,

    SAY_DEATH_METAL_KNIGHT_INTRO_1      = 0,
    SAY_DEATH_METAL_KNIGHT_INTRO_2,
    SAY_DEATH_METAL_KNIGHT_SUMMON_BANSHEE,
    SAY_DEATH_METAL_KNIGHT_DEATH_PLAYER,
    SAY_DEATH_METAL_KNIGHT_DEATHKNIGHT_ARMYOFTHEDEAD,
    SAY_DEATH_METAL_KNIGHT_PHASE_2,
    SAY_DEATH_METAL_KNIGHT_DEATH,

    SAY_DEVLYNN_STYX_SCREAM             = 0
};

enum eCreatures
{
    NPC_MANAGER                 = 125089,
    NPC_JAY_MAGUIRE             = 120928,
    NPC_DEATH_METAL_KNIGHT      = 122899,
    NPC_JON_GRAVES              = 121529,
    NPC_DEVLYNN_STYX            = 121527,
    NPC_CAGE_HEAD               = 121528,
    NPC_VFX_BUNNY               = 122323,
    NPC_INVISIBLE_STALKER       = 121295,
    NPC_DRUMMER                 = 121561,
    NPC_METAL_FAN               = 121535,
    NPC_METAL_FAN2              = 121537,
    NPC_METAL_FAN3              = 123691,
    NPC_GHOUL                   = 122943
};

enum eSpells
{
    //DEATH METAL KNIGHT
    SPELL_GRAND_ENTRANCE            = 244950,
    SPELL_ARMY_OF_THE_DEAD          = 245162,
    SPELL_COLD_AS_ICE               = 245025,
    SPELL_SUMMON_GHOUL              = 245067,
    SPELL_SWORD                     = 246038,
    SPELL_WAILING_WALL              = 244955,
    //Jon Graves
    SPELL_MIND_BLOWN                = 245034,
    SPELL_GUITAR                    = 243196,
    //DEVLYNN STYX
    SPELL_MICROPHONE                = 242932,
    SPELL_BASSIST                   = 246584,
    SPELL_GUITARIST                 = 246583,
    SPELL_VOCALS                    = 246586,
    SPELL_DRUMMER                   = 246585,
    SPELL_STAGE_DIVE                = 245105,
    SPELL_IN_THE_CROWD              = 245148,
    SPELL_WHOOOOOOAH                = 245155,
    SPELL_YEAAAAH                   = 245152,
    SPELL_WOOOH_YEAH                = 245154,
    SPELL_SCORE_UP                  = 248246,
    SPELL_SCORE_DOWN                = 248247,
    SPELL_NUMBER_ONE_FAN            = 245102,
    // server-side only, the criteria of Mosh Pit and Perfect Performance
    SPELL_CREDIT_MOSH_PIT           = 246668,
    SPELL_CREDIT_PERFECT            = 246679,
    //CAGE HEAD
    SPELL_BASS                      = 245160,
    //VFX BUNNY
    SPELL_LIGHT                     = 243893,
    //MANAGER
    SPELL_PLAY_MUSIC                = 247666,
    SPELL_END_SONG                  = 247667,
    //INVISIBLE STALKER
    SPELL_COSMETIC_FEL_EXPLOSION    = 240894,
    //DRUMMER
    SPELL_DRUM_STICKS               = 242908,
    //GENERAL
    SPELL_ACHIEVEMENT               = 245363,
    SPELL_ACTIVATE_GAMEOBJECTS      = 252525,
    SPELL_CASUAL_FAN                = 245063,
    SPELL_SERIOUS_FAN               = 245065,
    SPELL_HARDCORE_FUN              = 245066,
    //GHOUL
    SPELL_DEATHLY_PALLOR            = 245255,
    SPELL_DEATHS_ADVANCE            = 245256,
    SPELL_TOUCH_OF_THE_GRAVE        = 245245
};

enum eEvents
{
    //Jay Maguire
    EVENT_PREPARE_30_MIN        = 1,
    EVENT_PREPARE_20_MIN        = 2,
    EVENT_PREPARE_10_MIN        = 3,
    EVENT_PREPARE_1_MIN         = 4,
    EVENT_READY                 = 5,
    EVENT_ENDED                 = 6,
    EVENT_STARTED               = 7,
    //MANAGER
    EVENT_START_CONCERT         = 8,
    EVENT_END_CONCERT           = 9,
    //Death Metal Knight
    EVENT_BANSHEE_AT            = 10,
    EVENT_INTRO_WP              = 11,
    EVENT_ARMY_OF_THE_DEAD      = 12,
    //DEVLYNN STYX
    EVENT_JUMP_SCREAM           = 13,
    EVENT_WHOOOOOOAH            = 14,
    EVENT_UPDATE_FANS           = 15,
    EVENT_FLAG_SPECIAL_LOOT     = 16,
    EVENT_POWER_CHORDS          = 17,
    EVENT_STYX_RETURN           = 18
};

enum eConcert
{
    // the rare loot of the knight once per character and per Darkmoon Faire
    QUEST_DEATH_METAL_KNIGHT    = 47767,

    GO_PROPHETIC_GRAVE          = 269654,
    GO_STAGE_EFFECT             = 272825,

    ACTION_ARMY_OF_THE_DEAD     = 1,
    ACTION_GHOUL_REACHED_STAGE  = 2,
    ACTION_STYX_INTO_CROWD      = 3,
    DATA_GHOUL_KILLED           = 1,
    DATA_KNIGHT_LOOTER          = 2,
    DATA_WALL_HIT               = 3,

    SPELL_POWER_CHORD_MISSILE   = 245032,
    SPELL_WAILING_WALL_DAMAGE   = 244954,
    SPELL_WEAK_LINK             = 140949,

    SCORE_GHOUL                 = 10,
    SCORE_KNIGHT                = 50,
    SCORE_GHOUL_REACHED         = -10,
    SCORE_CHORD_CAUGHT          = 5,
    SCORE_WALL_HIT              = -10,
    SCORE_SERIOUS_FAN           = 30,
    SCORE_HARDCORE_FAN          = 60
};

float const CONCERT_FAN_RANGE = 50.0f;
float const POWER_CHORD_RADIUS = 8.0f;   // radius of Mind Blown
uint32 const POWER_CHORD_PERIOD = 20000; // period of Power Chords (245031)
uint32 const KNIGHT_CORPSE_TIME = 120000; // left to loot the knight, even past the end of the show
int32 const ARMY_HEALTH_PCT[] = { 60, 30 }; // a wave of ghouls at each threshold
uint8 const GHOULS_PER_WAVE = 3;

static Creature* GetConcertManager(Unit* from)
{
    return from->FindNearestCreature(NPC_MANAGER, 150.0f, true);
}

static void FlagKnightSpecialLoot(Player* player)
{
    Quest const* quest = sQuestDataStore->GetQuestTemplate(QUEST_DEATH_METAL_KNIGHT);
    if (!quest || player->IsQuestRewarded(QUEST_DEATH_METAL_KNIGHT))
        return;

    player->AddQuest(quest, nullptr);
    player->CompleteQuest(QUEST_DEATH_METAL_KNIGHT);
    player->RewardQuest(quest, 0, player, false);
}

static Position destSummon[10] =
{
    { -4189.54f, 6037.1f, 6.59233f, 5.19811f },
    { -4190.14f, 6014.1f, 5.78008f, 1.09447f },
    { -4180.64f, 6009.92f, 5.60332f, 1.23409f },
    { -4191.35f, 6035.88f, 6.48239f, 5.18861f },
    { -4186.57f, 6012.41f, 5.7644f, 1.16428f },
    { -4165.23f, 6019.99f, 6.80102f, 2.67399f },
    { -4166.17f, 6014.87f, 6.86055f, 3.0499f },
    { -4165.81f, 6002.39f, 6.94393f, 3.11536f },
    { -4183.02f, 6011.02f, 5.75034f, 1.21664f },
    { -4193.4f, 6034.81f, 6.682f, 5.21955f }
};

Position const ATSpawnPosition[20] =
{
    { -4169.229f, 6052.789f, 6.260829f, 4.371895f },
    { -4172.854f, 6054.479f, 6.260829f, 4.43865f},
    { -4167.416f, 6051.943f, 6.260829f, 4.336551f },
    { -4165.604f, 6051.098f, 6.260829f, 4.273721f },
    { -4163.791f, 6050.253f, 6.260829f, 4.297285f},
    { -4175.737f, 5996.241f, 6.260829f, 1.108567f },
    { -4173.925f, 5995.396f, 6.260829f, 1.128202f },
    { -4172.112f, 5994.551f, 6.260829f, 1.104645f },
    { -4181.175f, 5998.777f, 6.260829f, 1.085010f },
    { -4179.362f, 5997.932f, 6.260829f, 1.085010f },
    { -4177.55f, 5997.086f, 6.260829f, 1.085010f },
    { -4170.3f, 5993.706f, 6.260829f, 1.085010f },
    { -4184.8f, 6000.467f, 6.260829f, 1.253874f },
    { -4197.487f, 6006.384f, 6.260829f, 1.324560f },
    { -4199.3f, 6007.229f, 6.260829f, 1.316706f },
    { -4201.112f, 6008.074f, 6.260829f, 1.316706f },
    { -4202.925f, 6008.919f, 6.260829f, 1.316708f },
    { -4204.737f, 6009.765f, 6.260829f, 1.112502f },
    { -4195.675f, 6005.539f, 6.260829f, 1.112502f },
    { -4206.55f, 6010.61f, 6.260829f, 1.112502f }
};

struct npc_jay_maguire : public ScriptedAI
{
    npc_jay_maguire(Creature* creature) : ScriptedAI(creature) {}

    EventMap events;
    bool finishwp;

    void Reset() override
    {
        events.Reset();
        events.RescheduleEvent(EVENT_PREPARE_30_MIN, 1000);
        events.RescheduleEvent(EVENT_PREPARE_20_MIN, 1000);
        events.RescheduleEvent(EVENT_PREPARE_10_MIN, 1000);
        events.RescheduleEvent(EVENT_PREPARE_1_MIN, 1000);
        events.RescheduleEvent(EVENT_STARTED, 1000);
        finishwp = false;
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);

        // on the stage once his walk there is over: whether an event fires then or not
        if (finishwp && me->IsSplineFinished())
        {
            finishwp = false;
            me->SetOrientation(1.598367f);
            me->SetFacingTo(1.598367f);
            Talk(SAY_JAY_MAGUIRE_START_CONCERT);
            events.RescheduleEvent(EVENT_READY, 4000);
        }

        if (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_PREPARE_30_MIN:
                if (sGameEventMgr->IsActiveEvent(74))
                    Talk(SAY_JAY_MAGUIRE_30_MIN_LEFT);
                else
                    events.RescheduleEvent(EVENT_PREPARE_30_MIN, 1000);
                break;
            case EVENT_PREPARE_20_MIN:
                if (sGameEventMgr->IsActiveEvent(71))
                    Talk(SAY_JAY_MAGUIRE_20_MIN_LEFT);
                else
                    events.RescheduleEvent(EVENT_PREPARE_20_MIN, 1000);
                break;
            case EVENT_PREPARE_10_MIN:
                if (sGameEventMgr->IsActiveEvent(72))
                    Talk(SAY_JAY_MAGUIRE_10_MIN_LEFT);
                else
                    events.RescheduleEvent(EVENT_PREPARE_10_MIN, 1000);
                break;
            case EVENT_PREPARE_1_MIN:
                if (sGameEventMgr->IsActiveEvent(73))
                    Talk(SAY_JAY_MAGUIRE_1_MIN_LEFT);
                else
                    events.RescheduleEvent(EVENT_PREPARE_1_MIN, 1000);
                break;
            case EVENT_READY:
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_DANCE);
                events.RescheduleEvent(EVENT_ENDED, 312000);
                break;
            case EVENT_ENDED:
                me->NearTeleportTo(-4155.22f, 6067.37f, 8.65011f, 2.19868f);
                me->DespawnOrUnsummon(1000);
                break;
            case EVENT_STARTED:
                if (sGameEventMgr->IsActiveEvent(79))
                {
                    me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_ONESHOT_NONE);
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    Movement::MoveSplineInit init(*me);
                    init.Path().push_back(G3D::Vector3(-4155.821f, 6066.204f, 8.775227f));
                    init.Path().push_back(G3D::Vector3(-4157.071f, 6065.454f, 9.275227f));
                    init.Path().push_back(G3D::Vector3(-4159.321f, 6063.204f, 9.775227f));
                    init.Path().push_back(G3D::Vector3(-4161.071f, 6061.204f, 9.025227f));
                    init.Path().push_back(G3D::Vector3(-4164.071f, 6056.704f, 8.275227f));
                    init.Path().push_back(G3D::Vector3(-4164.821f, 6049.704f, 7.525226f));
                    init.Path().push_back(G3D::Vector3(-4166.071f, 6047.704f, 7.275226f));
                    init.Path().push_back(G3D::Vector3(-4168.071f, 6038.454f, 8.025227f));
                    init.Path().push_back(G3D::Vector3(-4168.321f, 6033.454f, 7.275226f));
                    init.Path().push_back(G3D::Vector3(-4169.071f, 6031.204f, 6.775226f));
                    init.Path().push_back(G3D::Vector3(-4172.071f, 6019.704f, 6.275226f));
                    init.Path().push_back(G3D::Vector3(-4174.071f, 6008.204f, 6.025226f));
                    init.Path().push_back(G3D::Vector3(-4174.42f, 6003.042f, 5.900339f));
                    init.SetWalk(false);
                    init.Launch();
                    finishwp = true;
                }
                else
                    events.RescheduleEvent(EVENT_STARTED, 1000);
                break;
            }

        }
    }
};

struct npc_darkmoon_manager : public ScriptedAI
{
    npc_darkmoon_manager(Creature* creature) : ScriptedAI(creature), summons(me) {}

    EventMap events;
    SummonList summons;
    ObjectGuid InvisibleStalkerGUID;
    bool startEvent;
    bool concertRunning;
    bool ghoulsSummoned;
    bool ghoulReachedStage;
    std::map<ObjectGuid, int32> fanScores;
    GuidSet knightLooters;
    // the band role: rhythm = Wailing Walls avoided, notes = own Power Chords caught
    std::map<ObjectGuid, uint32> wallHits;
    std::map<ObjectGuid, uint32> lastWallHit;
    std::map<ObjectGuid, uint32> missedChords;
    std::vector<std::pair<Position, ObjectGuid>> pendingChords;

    void Reset() override
    {
        events.Reset();
        startEvent = false;
        concertRunning = false;
        ghoulsSummoned = false;
        ghoulReachedStage = false;
        fanScores.clear();
        knightLooters.clear();
        wallHits.clear();
        lastWallHit.clear();
        missedChords.clear();
        pendingChords.clear();
    }

    void DoAction(int32 const action) override
    {
        switch (action)
        {
            case ACTION_ARMY_OF_THE_DEAD:
                ghoulsSummoned = true;
                // the singer leaps into the crowd, where the ghouls go for her
                for (ObjectGuid const& guid : summons)
                    if (Creature* styx = ObjectAccessor::GetCreature(*me, guid))
                        if (styx->GetEntry() == NPC_DEVLYNN_STYX && styx->IsAlive() && styx->IsAIEnabled)
                            styx->AI()->DoAction(ACTION_STYX_INTO_CROWD);
                break;
            case ACTION_GHOUL_REACHED_STAGE:
                ghoulReachedStage = true;
                for (auto const& fan : fanScores)
                    AddScore(fan.first, SCORE_GHOUL_REACHED);
                break;
            default:
                break;
        }
    }

    void SetGUID(ObjectGuid const& guid, int32 id) override
    {
        if (!concertRunning)
            return;

        switch (id)
        {
            case DATA_GHOUL_KILLED:
                AddScore(guid, SCORE_GHOUL);
                break;
            case DATA_KNIGHT_LOOTER:
                if (knightLooters.insert(guid).second)
                    AddScore(guid, SCORE_KNIGHT);
                break;
            case DATA_WALL_HIT:
            {
                // the wall is a row of touching wraiths: one hit per wave
                uint32 const now = getMSTime();
                auto itr = lastWallHit.find(guid);
                if (itr != lastWallHit.end() && getMSTimeDiff(itr->second, now) < 3000)
                    break;
                lastWallHit[guid] = now;
                ++wallHits[guid];
                AddScore(guid, SCORE_WALL_HIT);
                break;
            }
            default:
                break;
        }
    }

    // one chord falls near each fan, who has to stand under it
    void DropPowerChords()
    {
        Unit* caster = me;
        for (ObjectGuid const& guid : summons)
            if (Creature* guitarist = ObjectAccessor::GetCreature(*me, guid))
                if (guitarist->GetEntry() == NPC_JON_GRAVES && guitarist->IsAlive())
                    caster = guitarist;

        for (auto const& fan : fanScores)
        {
            Player* player = ObjectAccessor::GetPlayer(*me, fan.first);
            if (!player || !player->IsAlive() || !player->IsWithinDist(me, CONCERT_FAN_RANGE))
                continue;

            Position dest = player->GetPosition();
            player->MovePositionToFirstCollision(dest, frand(4.0f, 9.0f), frand(0.0f, float(2 * M_PI)));
            pendingChords.emplace_back(dest, fan.first);
            caster->CastSpell(dest, SPELL_POWER_CHORD_MISSILE, true);
        }
    }

    void PowerChordLanded(Position const& dest)
    {
        auto chord = std::min_element(pendingChords.begin(), pendingChords.end(), [&dest](auto const& a, auto const& b)
        {
            return a.first.GetExactDist(&dest) < b.first.GetExactDist(&dest);
        });
        if (chord == pendingChords.end() || chord->first.GetExactDist(&dest) > 2.0f)
            return;

        ObjectGuid const owner = chord->second;
        pendingChords.erase(chord);
        if (!concertRunning)
            return;

        bool ownerCaught = false;
        for (auto const& fan : fanScores)
        {
            Player* player = ObjectAccessor::GetPlayer(*me, fan.first);
            if (!player || !player->IsAlive() || player->GetExactDist(&dest) > POWER_CHORD_RADIUS)
                continue;

            AddScore(fan.first, SCORE_CHORD_CAUGHT);
            if (fan.first == owner)
                ownerCaught = true;
        }

        if (!ownerCaught)
            ++missedChords[owner];
    }

    void AddScore(ObjectGuid const& guid, int32 points)
    {
        Player* player = ObjectAccessor::GetPlayer(*me, guid);
        if (!player)
            return;

        int32& score = fanScores[guid];
        score = std::max(0, score + points);
        me->CastSpell(player, points > 0 ? SPELL_SCORE_UP : SPELL_SCORE_DOWN, true);
        UpdateFanAura(player, score);
    }

    // Casual, Serious then Hardcore Fan; the tooltip of the aura shows the score
    void UpdateFanAura(Player* player, int32 score)
    {
        uint32 tier = score >= SCORE_HARDCORE_FAN ? SPELL_HARDCORE_FUN : score >= SCORE_SERIOUS_FAN ? SPELL_SERIOUS_FAN : SPELL_CASUAL_FAN;
        for (uint32 spellId : { SPELL_CASUAL_FAN, SPELL_SERIOUS_FAN, SPELL_HARDCORE_FUN })
            if (spellId != tier)
                player->RemoveAurasDueToSpell(spellId);

        if (!player->HasAura(tier))
            player->CastSpell(player, tier, true);

        if (Aura* aura = player->GetAura(tier))
            if (AuraEffect* effect = aura->GetEffect(EFFECT_0))
                effect->ChangeAmount(float(score));
    }

    void UpdateFans()
    {
        std::list<Player*> players;
        me->GetPlayerListInGrid(players, CONCERT_FAN_RANGE);
        for (Player* player : players)
        {
            if (!player->IsAlive() || player->isGameMaster() || fanScores.count(player->GetGUID()))
                continue;

            fanScores[player->GetGUID()] = 0;
            UpdateFanAura(player, 0);
        }
    }

    void TallyScores()
    {
        int32 best = 0;
        for (auto const& fan : fanScores)
            best = std::max(best, fan.second);

        bool const moshPit = ghoulsSummoned && !ghoulReachedStage;

        for (auto const& fan : fanScores)
        {
            Player* player = ObjectAccessor::GetPlayer(*me, fan.first);
            if (!player)
                continue;

            int32 const score = fan.second;
            for (uint32 spellId : { SPELL_CASUAL_FAN, SPELL_SERIOUS_FAN, SPELL_HARDCORE_FUN })
                player->RemoveAurasDueToSpell(spellId);

            player->UpdateAchievementCriteria(CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_ACHIEVEMENT, 0, 0, me);

            bool const rhythm = !wallHits[fan.first];
            bool const notes = !missedChords[fan.first];
            bool const perfect = moshPit && rhythm && notes;
            if (moshPit)
                player->UpdateAchievementCriteria(CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_CREDIT_MOSH_PIT, 0, 0, me);
            if (perfect)
                player->UpdateAchievementCriteria(CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_CREDIT_PERFECT, 0, 0, me);

            uint32 role = rhythm ? (notes ? SPELL_DRUMMER : SPELL_BASSIST) : (notes ? SPELL_VOCALS : SPELL_GUITARIST);
            for (uint32 spellId : { SPELL_GUITARIST, SPELL_BASSIST, SPELL_VOCALS, SPELL_DRUMMER })
                player->RemoveAurasDueToSpell(spellId);
            player->CastSpell(player, role, true);

            if (best > 0 && score == best)
                player->CastSpell(player, SPELL_NUMBER_ONE_FAN, true);
        }
    }


    void JustSummoned(Creature* sum) override
    {
        summons.Summon(sum);

        switch (sum->GetEntry())
        {
        case NPC_JON_GRAVES:
            sum->CastSpell(sum, SPELL_GUITAR, true);
            sum->SetAnimKitId(13199);
            break;
        case NPC_DEVLYNN_STYX:
            sum->CastSpell(sum, SPELL_MICROPHONE, true);
            sum->SetAnimKitId(13113);
            break;
        case NPC_CAGE_HEAD:
            sum->CastSpell(sum, SPELL_BASS, true);
            sum->SetAnimKitId(13414);
            break;
        case NPC_DRUMMER:
            sum->CastSpell(sum, SPELL_DRUM_STICKS, true);
            sum->SetAnimKitId(13099);
            break;
        case NPC_INVISIBLE_STALKER:
            InvisibleStalkerGUID.Clear();
            InvisibleStalkerGUID = sum->GetGUID();
            sum->CastSpell(sum, SPELL_COSMETIC_FEL_EXPLOSION, true);
            break;
        case NPC_METAL_FAN:
            sum->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_DANCE);
            break;
        case NPC_METAL_FAN2:
            sum->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_DANCE);
            break;
        case NPC_METAL_FAN3:
            sum->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_DANCE);
            break;
        default:
            break;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);

        if (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_START_CONCERT:
                DoCast(SPELL_PLAY_MUSIC);
                // the graves and the stage play for the length of the show; 252525 would make them clickable
                {
                    std::list<GameObject*> stage;
                    me->GetGameObjectListWithEntryInGrid(stage, GO_PROPHETIC_GRAVE, 100.0f);
                    me->GetGameObjectListWithEntryInGridAppend(stage, GO_STAGE_EFFECT, 100.0f);
                    for (GameObject* go : stage)
                        go->UseDoorOrButton();
                }
                me->SummonCreatureGroup(0);
                concertRunning = true;
                UpdateFans();
                events.RescheduleEvent(EVENT_UPDATE_FANS, 2000);
                events.RescheduleEvent(EVENT_POWER_CHORDS, POWER_CHORD_PERIOD);
                events.RescheduleEvent(EVENT_END_CONCERT, 312300);
                break;
            case EVENT_UPDATE_FANS:
                if (concertRunning)
                {
                    UpdateFans();
                    events.RescheduleEvent(EVENT_UPDATE_FANS, 2000);
                }
                break;
            case EVENT_POWER_CHORDS:
                // the last chords must land before the scores are tallied
                if (concertRunning && events.GetTimeUntilEvent(EVENT_END_CONCERT) > 15000)
                {
                    DropPowerChords();
                    events.RescheduleEvent(EVENT_POWER_CHORDS, POWER_CHORD_PERIOD);
                }
                break;
            case EVENT_END_CONCERT:
                if (auto stalker = me->GetCreature(*me, InvisibleStalkerGUID))
                    stalker->CastSpell(stalker, SPELL_COSMETIC_FEL_EXPLOSION, true);

                // startEvent stays set: the manager is gone before the next show
                concertRunning = false;
                TallyScores();
                DoCast(SPELL_END_SONG);
                events.RescheduleEvent(EVENT_FLAG_SPECIAL_LOOT, 2000);
                break;
            case EVENT_FLAG_SPECIAL_LOOT:
                me->DespawnOrUnsummon();
                break;
            }
        }

        if ((sGameEventMgr->IsActiveEvent(79)) && !startEvent)
        {
            startEvent = true;
            events.RescheduleEvent(EVENT_START_CONCERT, 18000);
        }
    }
};

struct npc_death_metal_knight : public ScriptedAI
{
    npc_death_metal_knight(Creature* creature) : ScriptedAI(creature)
    {
        introDone = false;
        introDone2 = false;
    }

    EventMap events;
    bool armyCasting;
    uint8 armyWaves;
    bool introDone;
    bool introDone2;

    void Reset() override
    {
        events.Reset();
        DespawnAllSummons();
        me->RemoveAllAreaObjects();
        // passive only until he reaches the crowd, or he would never fight back after a wipe
        me->SetReactState(introDone2 ? REACT_AGGRESSIVE : REACT_PASSIVE);
        DoCast(SPELL_SWORD);
        armyCasting = false;
        armyWaves = 0;
    }

    void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/, DamageEffectType /*dmgType*/) override
    {
        if (armyWaves < sizeof(ARMY_HEALTH_PCT) / sizeof(ARMY_HEALTH_PCT[0]) &&me->HealthBelowPct(ARMY_HEALTH_PCT[armyWaves]))
        {
            ++armyWaves;
            events.RescheduleEvent(EVENT_ARMY_OF_THE_DEAD, 500);
        }
    }

    void EnterEvadeMode() override
    {
        if (auto styx = !me->FindNearestCreature(NPC_DEVLYNN_STYX, 100.0f, true))
            me->DespawnOrUnsummon();

        ScriptedAI::EnterEvadeMode();
    }

    void EnterCombat(Unit* /*who*/) override
    {
        DoCast(SPELL_COLD_AS_ICE);
        events.RescheduleEvent(EVENT_BANSHEE_AT, 27000);
    }

    void JustDied(Unit* /*killer*/) override
    {
        Talk(SAY_DEATH_METAL_KNIGHT_DEATH);
        me->RemoveAllAreaObjects();
        DespawnAllSummons();

        // summoned for the length of the show: the corpse would vanish with it, loot included
        uint32 corpseTime = KNIGHT_CORPSE_TIME;
        if (TempSummon* summon = me->ToTempSummon())
        {
            summon->SetTempSummonType(TEMPSUMMON_CORPSE_TIMED_DESPAWN);
            if (summon->GetTimer() < KNIGHT_CORPSE_TIME)
                summon->AddDuration(KNIGHT_CORPSE_TIME - summon->GetTimer());
            corpseTime = summon->GetTimer();
        }

        // everyone who received a share of the personal loot
        GuidSet looters;
        for (ObjectGuid const& guid : *me->GetSaveThreatList())
        {
            Player* looter = ObjectAccessor::GetPlayer(*me, guid);
            if (!looter || !me->isTappedBy(looter))
                continue;

            looters.insert(guid);
            if (Group* group = looter->GetGroup())
                group->ForEachMemberInMap(me->GetMap(), [&](Player* member)
                {
                    if (member->IsAtGroupRewardDistance(me))
                        looters.insert(member->GetGUID());
                });
        }

        Creature* manager = GetConcertManager(me);
        for (ObjectGuid const& guid : looters)
        {
            if (manager && manager->IsAIEnabled)
                manager->AI()->SetGUID(guid, DATA_KNIGHT_LOOTER);

            // the rare loot once per Faire: flagged once the corpse is gone, or its loot would vanish under the looter's eyes
            if (Player* looter = ObjectAccessor::GetPlayer(*me, guid))
                looter->AddDelayedEvent(corpseTime + 2000, [looter]() -> void { FlagKnightSpecialLoot(looter); });
        }
    }

    void DespawnAllSummons()
    {
        std::list<Creature*> list;
        list.clear();
        me->GetCreatureListWithEntryInGrid(list, NPC_GHOUL, 200.0f);
        if (!list.empty())
            for (auto& cre : list)
                cre->DespawnOrUnsummon();
    }

    void IsSummonedBy(Unit* /*owner*/) override
    {
        if (!introDone)
        {
            introDone = true;
            DoCast(SPELL_GRAND_ENTRANCE);
            Talk(SAY_DEATH_METAL_KNIGHT_INTRO_1);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NOT_ATTACKABLE_1);
            me->AddDelayedEvent(15000, [=]() -> void
            {
                me->SetWalk(true);
                me->GetMotionMaster()->MovePoint(1, -4179.04f, 6026.6616f, 6.154604f);
            });
        }
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        switch (id)
        {
        case 1:
            if (!introDone2)
            {
                introDone2 = true;
                Talk(SAY_DEATH_METAL_KNIGHT_INTRO_2);
                me->SetReactState(REACT_AGGRESSIVE);
                me->SetHomePosition(-4179.04f, 6026.6616f, 6.154604f, 4.30094f);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NOT_ATTACKABLE_1);
            }
            break;
        default:
            break;
        }
    }

    void SpellFinishCast(SpellInfo const* spell) override
    {
        if (spell->Id == SPELL_ARMY_OF_THE_DEAD)
        {
            armyCasting = false;
            me->SetReactState(REACT_AGGRESSIVE);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        // Army of the Dead interrupted: SpellFinishCast never comes
        if (armyCasting)
        {
            armyCasting = false;
            me->SetReactState(REACT_AGGRESSIVE);
        }

        if (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_BANSHEE_AT:
                Talk(SAY_DEATH_METAL_KNIGHT_SUMMON_BANSHEE);

                for (uint8 i = 0; i < 20; ++i)
                    me->CastSpell(ATSpawnPosition[i], SPELL_WAILING_WALL, true);

                events.RescheduleEvent(EVENT_BANSHEE_AT, 25000);
                break;
            case EVENT_ARMY_OF_THE_DEAD:
                me->CastStop();
                me->SetReactState(REACT_PASSIVE);
                DoCast(SPELL_ARMY_OF_THE_DEAD);
                armyCasting = true;
                if (Creature* manager = GetConcertManager(me))
                    if (manager->IsAIEnabled)
                        manager->AI()->DoAction(ACTION_ARMY_OF_THE_DEAD);
                Talk(SAY_DEATH_METAL_KNIGHT_DEATHKNIGHT_ARMYOFTHEDEAD);
                break;
            }
        }
        DoMeleeAttackIfReady();
    }
};

struct npc_darkmoon_ghoul : public ScriptedAI
{
    npc_darkmoon_ghoul(Creature* creature) : ScriptedAI(creature), reachedStage(false) {}

    bool reachedStage;
    ObjectGuid targetGUID;
    uint32 followCheck = 1000;

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
    }

    // hit or knocked back by Death's Advance, they keep walking to the singer instead of standing still
    void EnterEvadeMode() override {}
    void AttackStart(Unit* /*who*/) override {}

    // always the singer, never the knight who raised them
    void FollowTarget()
    {
        Creature* styx = me->FindNearestCreature(NPC_DEVLYNN_STYX, 100.0f, true);
        if (!styx)
            return;

        targetGUID = styx->GetGUID();
        me->SetWalk(true);
        me->GetMotionMaster()->MoveFollow(styx, 0.5f, 1.5f, MOTION_SLOT_ACTIVE);
    }

    void UpdateAI(uint32 diff) override
    {
        if (reachedStage)
            return;

        if (followCheck > diff)
        {
            followCheck -= diff;
            return;
        }
        followCheck = 1000;

        if (me->HasUnitState(UNIT_STATE_CONTROLLED | UNIT_STATE_JUMPING) || !me->movespline->Finalized())
            return;

        // Weak Link leaps them back on every blow, and the follow does not resume after it
        Unit* target = ObjectAccessor::GetUnit(*me, targetGUID);
        if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() != FOLLOW_MOTION_TYPE || !target || !me->IsWithinDist(target, 3.0f))
        {
            me->GetMotionMaster()->Clear(false);
            FollowTarget();
        }
    }

    void JustDied(Unit* killer) override
    {
        if (reachedStage || !killer)
            return;

        if (Player* player = killer->GetCharmerOrOwnerPlayerOrPlayerItself())
            if (Creature* manager = GetConcertManager(me))
                if (manager->IsAIEnabled)
                    manager->AI()->SetGUID(player->GetGUID(), DATA_GHOUL_KILLED);
    }

    void IsSummonedBy(Unit* /*owner*/) override
    {
        DoCast(SPELL_DEATHLY_PALLOR);
        DoCast(SPELL_DEATHS_ADVANCE);

        // they shamble towards the singer, giving the crowd time to stop them
        // 0.75 yard per second, walking or running, so that a lone player can stop the three of them
        me->SetSpeed(MOVE_WALK, 0.3f);
        me->SetSpeed(MOVE_RUN, 0.3f * baseMoveSpeed[MOVE_WALK] / baseMoveSpeed[MOVE_RUN]);
        FollowTarget();
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type != FOLLOW_MOTION_TYPE || reachedStage)
            return;

        reachedStage = true;
        if (Creature* manager = GetConcertManager(me))
            if (manager->IsAIEnabled)
                manager->AI()->DoAction(ACTION_GHOUL_REACHED_STAGE);
        DoCast(SPELL_TOUCH_OF_THE_GRAVE);
    }
};

struct npc_devlynn_styx : public ScriptedAI
{
    npc_devlynn_styx(Creature* creature) : ScriptedAI(creature) {}

    EventMap events;
    bool inCrowd = false;
    uint32 crowdTime = 0;

    void Reset() override
    {
        events.Reset();
        inCrowd = false;
        // she screams at any ghoul close enough, on the stage or in the crowd
        events.RescheduleEvent(EVENT_WHOOOOOOAH, 6000);
    }

    void DoAction(int32 const action) override
    {
        if (action != ACTION_STYX_INTO_CROWD || inCrowd)
            return;

        // she leaps into the middle of the cave, where the ghouls go for her
        Creature* manager = GetConcertManager(me);
        if (!manager)
            return;

        inCrowd = true;
        crowdTime = 0;
        me->CastSpell(manager, SPELL_STAGE_DIVE, true);
        // should the dive find no landing, she jumps there all the same
        Position const landing = manager->GetPosition();
        me->AddDelayedEvent(2500, [this, landing]() -> void
        {
            Position const home = me->GetHomePosition();
            if (inCrowd && me->GetExactDist(&home) < 5.0f)
                me->GetMotionMaster()->MoveJump(landing.GetPositionX(), landing.GetPositionY(), landing.GetPositionZ(), landing.GetOrientation(), 20.0f, 15.0f, EVENT_JUMP, true);
        });
        Talk(SAY_DEVLYNN_STYX_SCREAM);
        me->AddDelayedEvent(1000, [=]() -> void { DoCast(SPELL_IN_THE_CROWD); });
        events.RescheduleEvent(EVENT_WHOOOOOOAH, 3000);
        events.RescheduleEvent(EVENT_STYX_RETURN, 2000);
    }

    // the cries are a cone against her enemies, but the band is friendly to everyone: the spell finds no
    // target and fails, so it is only the voice and the wave, the blow is dealt here
    void Scream(Creature* towards)
    {
        me->SetOrientation(me->GetAngle(towards));
        me->SetFacingTo(towards);
        me->CastSpell(towards, RAND(SPELL_YEAAAAH, SPELL_WOOOH_YEAH, SPELL_WHOOOOOOAH), true);

        std::list<Creature*> ghouls;
        me->GetCreatureListWithEntryInGrid(ghouls, NPC_GHOUL, 30.0f);
        for (Creature* ghoul : ghouls)
            if (ghoul->IsAlive() && (ghoul == towards || me->HasInArc(float(M_PI) / 2, ghoul)))
            {
                me->DealDamage(ghoul, uint32(ghoul->CountPctFromMaxHealth(10)));
                // the blast throws them back, as a blow from the crowd does
                if (ghoul->IsAlive())
                    ghoul->CastSpell(ghoul, SPELL_WEAK_LINK, true);
            }
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);
        if (inCrowd)
            crowdTime += diff;

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        if (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_WHOOOOOOAH:
                // her scream hits the ghouls in front of her
                if (Creature* ghoul = me->FindNearestCreature(NPC_GHOUL, 25.0f, true))
                    Scream(ghoul);
                events.RescheduleEvent(EVENT_WHOOOOOOAH, 6000);
                break;
            case EVENT_STYX_RETURN:
                // back on the stage once the wave is over
                if (crowdTime >= 10000 && (!me->FindNearestCreature(NPC_GHOUL, 100.0f, true) || crowdTime >= 60000))
                {
                    inCrowd = false;
                    me->RemoveAurasDueToSpell(SPELL_IN_THE_CROWD);
                    float x, y, z, o;
                    me->GetHomePosition(x, y, z, o);
                    me->GetMotionMaster()->MoveJump(x, y, z, o, 20.0f, 15.0f, EVENT_JUMP, true);
                }
                else
                    events.RescheduleEvent(EVENT_STYX_RETURN, 2000);
                break;
            }
        }
    }
};

//245162
class spell_army_of_the_dead : public AuraScript
{
    PrepareAuraScript(spell_army_of_the_dead);

    void Tick(AuraEffect const* aurEff)
    {
        // a few ghouls together, each out of a different grave
        if (aurEff->GetTickNumber() != 1)
            return;

        Creature* deathmetal = GetCaster() ? GetCaster()->ToCreature() : nullptr;
        if (!deathmetal)
            return;

        std::vector<uint8> graves(10);
        std::iota(graves.begin(), graves.end(), 0);
        Trinity::Containers::RandomShuffle(graves);
        for (uint8 i = 0; i < GHOULS_PER_WAVE; ++i)
            deathmetal->CastSpell(destSummon[graves[i]], SPELL_SUMMON_GHOUL, true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_army_of_the_dead::Tick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 245032 - Power Chords: tells the concert where the chord landed
class spell_darkmoon_power_chord : public SpellScript
{
    PrepareSpellScript(spell_darkmoon_power_chord);

    void HandleLanded(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        WorldLocation const* dest = GetHitDest();
        if (!caster || !dest)
            return;

        if (Creature* manager = GetConcertManager(caster))
            if (npc_darkmoon_manager* ai = CAST_AI(npc_darkmoon_manager, manager->AI()))
                ai->PowerChordLanded(*dest);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_darkmoon_power_chord::HandleLanded, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
    }
};

// 244954 - Wailing Wall: a fan caught by the wraiths loses the rhythm
class spell_darkmoon_wailing_wall : public SpellScript
{
    PrepareSpellScript(spell_darkmoon_wailing_wall);

    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        Player* player = GetHitUnit() ? GetHitUnit()->ToPlayer() : nullptr;
        if (!player)
            return;

        if (Creature* manager = GetConcertManager(player))
            if (manager->IsAIEnabled)
                manager->AI()->SetGUID(player->GetGUID(), DATA_WALL_HIT);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_darkmoon_wailing_wall::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

void AddSC_DarkMoonConcertBlightBoar()
{
    RegisterCreatureAI(npc_death_metal_knight);
    RegisterCreatureAI(npc_darkmoon_manager);
    RegisterCreatureAI(npc_jay_maguire);
    RegisterCreatureAI(npc_darkmoon_ghoul);
    RegisterCreatureAI(npc_devlynn_styx);
    RegisterAuraScript(spell_army_of_the_dead);
    RegisterSpellScript(spell_darkmoon_power_chord);
    RegisterSpellScript(spell_darkmoon_wailing_wall);
}