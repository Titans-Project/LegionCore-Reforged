////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "Containers.h"
#include "GossipDef.h"
#include "MoveSplineInit.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"

/*
    Hallow's End: the Shade of the Horseman sets a starting village on fire.
    "Let the Fires Come!" summons him; he circles above the town lighting fire nodes, the players
    throw water buckets from the barrels, and if every fire is out before the time runs out he lands.
*/

enum HallowsEndVillageFires
{
    NPC_FIRE_NODE                   = 23537,
    NPC_SHADE_OF_THE_HORSEMAN       = 23543,

    ITEM_WATER_BUCKET               = 32971,
    GO_LARGE_JACK_O_LANTERN         = 186887,

    SPELL_FIRE_VISUAL               = 42074,
    SPELL_START_FIRE                = 42132,
    SPELL_GIVE_WATER_BUCKET         = 42349,
    SPELL_WATER_BUCKET_OVERRIDE     = 42336,
    SPELL_CONFLAGRATION             = 42380,
    SPELL_CONFLAGRATION_SOUND       = 48149,
    SPELL_HORSEMAN_CLEAVE           = 42587,
    SPELL_HORSEMAN_MOUNT            = 48025,

    QUEST_STOP_THE_FIRES_A          = 11131,
    QUEST_STOP_THE_FIRES_H          = 11219,
    QUEST_LET_THE_FIRES_COME_A      = 12135,
    QUEST_LET_THE_FIRES_COME_H      = 12139,

    TALK_PREPARE                    = 0,
    TALK_MIDWAY                     = 1,
    TALK_TIME_SHORT                 = 2,
    TALK_FAILED                     = 3,
    TALK_FIRES_OUT                  = 4,
    TALK_CONFLAGRATION              = 5,
    TALK_DEATH                      = 6,

    ACTION_IGNITE                   = 1,
    ACTION_SPLASH                   = 2,
    ACTION_EXTINGUISH               = 3,

    DATA_FIRE_LEVEL                 = 1,
    DATA_PRACTICE_FIRE              = 2,
    DATA_GROWTH_INTERVAL            = 3,
    DATA_FIRES_PHASE                = 4,

    POINT_LANDING                   = 1,
    POINT_LEAVE                     = 2,
};

enum ShadeEvents
{
    EVENT_PREPARE                   = 1,
    EVENT_FIRST_FIRES,
    EVENT_FIRE_PASS,
    EVENT_CONFLAGRATION,
    EVENT_TALK_MIDWAY,
    EVENT_TALK_TIME_SHORT,
    EVENT_TIME_UP,
    EVENT_CLEAVE,
};

enum ShadePhases
{
    PHASE_FIRES                     = 1,
    PHASE_LANDING,
    PHASE_COMBAT,
    PHASE_LEAVING,
};

struct HorsemanVillage
{
    uint32 MapId;
    Position Center;    // above the middle of the burning buildings, at circling height
    float Radius;
    Position Landing;
    Position Practice;  // the five fires of Fire Brigade Practice / Fire Training
};

static HorsemanVillage const Villages[] =
{
    { 0,   { -9464.4f,   62.1f,   88.5f  }, 40.0f, { -9462.71f,  64.41f,    55.92f  }, { -9318.1f,  55.5f,     67.3f  } }, // Goldshire
    { 0,   { -5585.3f,  -494.4f,  433.0f }, 45.0f, { -5616.3f,  -481.89f,   398.99f }, { -5754.8f, -527.4f,    400.1f } }, // Kharanos
    { 0,   {  2264.3f,   284.0f,  65.0f  }, 45.0f, {  2229.4f,   263.1f,    36.13f  }, {  2235.3f,  473.4f,    38.6f  } }, // Brill
    { 530, { -4177.7f, -12499.6f, 70.0f  }, 45.0f, { -4198.1f, -12509.13f,  46.6f   }, { -4201.2f, -12272.6f,  3.1f   } }, // Azure Watch
    { 530, {  9518.0f, -6800.0f,  32.5f  }, 28.0f, {  9532.9f, -6833.8f,    18.5f   }, {  9227.7f, -6775.6f,   25.5f  } }, // Falconwing Square
    { 1,   {  306.7f,  -4727.1f,  45.0f  }, 40.0f, {  313.83f, -4730.23f,   9.9f    }, {  288.6f,  -4564.5f,   39.0f  } }, // Razor Hill
};

uint32 const EVENT_DURATION         = 390 * IN_MILLISECONDS;
uint32 const FIRE_PASS_INTERVAL     = 15 * IN_MILLISECONDS;
uint32 const PRACTICE_RELIGHT_DELAY = 5 * IN_MILLISECONDS;
uint8 const FIRE_MAX_LEVEL          = 3;
float const FIRE_SPREAD_RANGE       = 7.0f;
float const VILLAGE_PLAYER_RANGE    = 100.0f;

static HorsemanVillage const* FindVillage(WorldObject const* object, float maxDist)
{
    for (HorsemanVillage const& village : Villages)
        if (object->GetMapId() == village.MapId && object->GetExactDist2d(village.Center.GetPositionX(), village.Center.GetPositionY()) < maxDist)
            return &village;

    return nullptr;
}

static bool IsPracticeSpot(WorldObject const* object)
{
    for (HorsemanVillage const& village : Villages)
        if (object->GetMapId() == village.MapId && object->GetExactDist2d(village.Practice.GetPositionX(), village.Practice.GetPositionY()) < 25.0f)
            return true;

    return false;
}

// 23537 - Headless Horseman - Fire (DND)
struct npc_hallows_end_fire : public ScriptedAI
{
    npc_hallows_end_fire(Creature* creature) : ScriptedAI(creature) { }

    void InitializeAI() override
    {
        _practice = IsPracticeSpot(me);
        me->SetReactState(REACT_PASSIVE);
        Reset();
    }

    void Reset() override
    {
        Extinguish();
        if (_practice)
            _timer = PRACTICE_RELIGHT_DELAY;
    }

    void EnterEvadeMode() override { }

    void SetGUID(ObjectGuid const& guid, int32 /*id*/) override
    {
        _shadeGUID = guid;
    }

    uint32 GetData(uint32 type) const override
    {
        switch (type)
        {
            case DATA_FIRE_LEVEL:
                return _level;
            case DATA_PRACTICE_FIRE:
                return _practice ? 1 : 0;
            default:
                return 0;
        }
    }

    void DoAction(int32 const action) override
    {
        switch (action)
        {
            case ACTION_IGNITE:
                if (!_level)
                    SetLevel(1);
                break;
            case ACTION_SPLASH:
                if (!_level)
                    break;
                // One bucket puts a fire out, whatever its size
                Extinguish();
                if (_practice)
                    _timer = PRACTICE_RELIGHT_DELAY;
                break;
            case ACTION_EXTINGUISH:
                Extinguish();
                break;
            default:
                break;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!_timer)
            return;

        if (_timer > diff)
        {
            _timer -= diff;
            return;
        }

        _timer = 0;

        if (_practice)
        {
            SetLevel(1);
            return;
        }

        if (!_level)
            return;

        // A fire left behind by a Shade that is gone would burn for the rest of the holiday
        Creature* shade = ObjectAccessor::GetCreature(*me, _shadeGUID);
        if (!shade || !shade->IsAlive() || !shade->AI() || !shade->AI()->GetData(DATA_FIRES_PHASE))
        {
            Extinguish();
            return;
        }

        if (_level < FIRE_MAX_LEVEL)
            SetLevel(_level + 1);
        else
            SpreadFire(shade);

        _timer = shade->AI()->GetData(DATA_GROWTH_INTERVAL);
    }

private:
    void SetLevel(uint8 level)
    {
        bool const wasBurning = _level > 0;
        _level = level;
        me->SetObjectScale(0.1f + 0.5f * level);
        if (!me->HasAura(SPELL_FIRE_VISUAL))
            DoCastSelf(SPELL_FIRE_VISUAL, true);

        if (_practice)
            return;

        if (!wasBurning)
            if (Creature* shade = ObjectAccessor::GetCreature(*me, _shadeGUID))
                if (shade->AI())
                    _timer = shade->AI()->GetData(DATA_GROWTH_INTERVAL);
    }

    void Extinguish()
    {
        _level = 0;
        _timer = 0;
        me->RemoveAllAuras();
        me->SetObjectScale(1.0f);
    }

    void SpreadFire(Creature* shade)
    {
        std::list<Creature*> neighbours;
        me->GetCreatureListWithEntryInGrid(neighbours, NPC_FIRE_NODE, FIRE_SPREAD_RANGE);
        neighbours.remove_if([this](Creature* fire)
        {
            return fire == me || !fire->AI() || fire->AI()->GetData(DATA_FIRE_LEVEL) || fire->AI()->GetData(DATA_PRACTICE_FIRE);
        });

        if (neighbours.empty())
            return;

        Creature* ember = Trinity::Containers::SelectRandomContainerElement(neighbours);
        ember->AI()->SetGUID(shade->GetGUID());
        ember->AI()->DoAction(ACTION_IGNITE);
    }

    ObjectGuid _shadeGUID;
    uint32 _timer = 0;
    uint8 _level = 0;
    bool _practice = false;
};

// 23543 - Shade of the Horseman
struct npc_shade_of_the_horseman : public ScriptedAI
{
    npc_shade_of_the_horseman(Creature* creature) : ScriptedAI(creature) { }

    void IsSummonedBy(Unit* summoner) override
    {
        _village = FindVillage(summoner, 150.0f);
        if (!_village)
        {
            me->DespawnOrUnsummon();
            return;
        }

        std::list<Creature*> fires;
        me->GetCreatureListWithEntryInGrid(fires, NPC_FIRE_NODE, 150.0f);
        for (Creature* fire : fires)
            if (fire->AI() && !fire->AI()->GetData(DATA_PRACTICE_FIRE) && fire->GetExactDist2d(_village->Center.GetPositionX(), _village->Center.GetPositionY()) < 90.0f)
                _fires.push_back(fire->GetGUID());

        // Without its fire nodes the town could never be saved
        if (_fires.empty())
        {
            me->DespawnOrUnsummon();
            return;
        }

        // Out of reach while airborne, but he can still throw his pumpkins at the players; the guards leave him alone
        _phase = PHASE_FIRES;
        me->SetReactState(REACT_PASSIVE);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_NPC);
        DoCastSelf(SPELL_HORSEMAN_MOUNT, true);
        me->SetCanFly(true);
        me->SetDisableGravity(true);
        me->SetSpeed(MOVE_RUN, 1.2f);
        GallopAround();

        _events.ScheduleEvent(EVENT_PREPARE, 3 * IN_MILLISECONDS);
        _events.ScheduleEvent(EVENT_FIRST_FIRES, 8 * IN_MILLISECONDS);
        _events.ScheduleEvent(EVENT_FIRE_PASS, 8 * IN_MILLISECONDS + FIRE_PASS_INTERVAL);
        _events.ScheduleEvent(EVENT_CONFLAGRATION, 18 * IN_MILLISECONDS);
        _events.ScheduleEvent(EVENT_TALK_MIDWAY, 150 * IN_MILLISECONDS);
        _events.ScheduleEvent(EVENT_TALK_TIME_SHORT, EVENT_DURATION - 120 * IN_MILLISECONDS);
        _events.ScheduleEvent(EVENT_TIME_UP, EVENT_DURATION);
    }

    uint32 GetData(uint32 type) const override
    {
        switch (type)
        {
            case DATA_FIRES_PHASE:
                return _phase == PHASE_FIRES ? 1 : 0;
            case DATA_GROWTH_INTERVAL:
                // A fire grows every 30 s with one firefighter, faster as the crowd grows, never under 10 s
                return std::max<uint32>(10 * IN_MILLISECONDS, 120 * IN_MILLISECONDS / (3 + _playerCount));
            default:
                return 0;
        }
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (_phase == PHASE_COMBAT)
            ScriptedAI::MoveInLineOfSight(who);
    }

    void EnterEvadeMode() override
    {
        if (_phase == PHASE_COMBAT)
            me->DespawnOrUnsummon(5 * IN_MILLISECONDS);
    }

    void EnterCombat(Unit* /*who*/) override
    {
        _idleTimer = 120 * IN_MILLISECONDS;
    }

    // Alone against him a player without heals could not win: a fifth less damage while one player is around
    void DamageDealt(Unit* /*victim*/, uint32& damage, DamageEffectType /*damageType*/) override
    {
        if (_phase == PHASE_COMBAT && _playerCount <= 1)
            damage -= damage / 5;
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (id == POINT_LANDING)
        {
            _phase = PHASE_COMBAT;
            me->RemoveAurasDueToSpell(SPELL_HORSEMAN_MOUNT);
            me->SetCanFly(false);
            me->SetDisableGravity(false);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetReactState(REACT_AGGRESSIVE);
            me->SetHomePosition(_village->Landing);
            _idleTimer = 120 * IN_MILLISECONDS;
            _events.ScheduleEvent(EVENT_CLEAVE, 7 * IN_MILLISECONDS);
            _events.ScheduleEvent(EVENT_CONFLAGRATION, 6 * IN_MILLISECONDS);

            UpdatePlayerCount();
            if (Player* player = me->FindNearestPlayer(40.0f))
                AttackStart(player);
        }
        else if (id == POINT_LEAVE)
            me->DespawnOrUnsummon();
    }

    void JustDied(Unit* /*killer*/) override
    {
        Talk(TALK_DEATH);
        // The spell drops the lantern 0.6 m into the ground (its model reaches under its origin): it is set on the ground instead
        float const ground = me->GetMap()->GetHeight(me->GetPhases(), me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 2.0f);
        me->SummonGameObject(GO_LARGE_JACK_O_LANTERN, me->GetPositionX(), me->GetPositionY(), (ground > INVALID_HEIGHT ? ground : me->GetPositionZ()) + 0.63f,
            me->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 300);
        ExtinguishAll();
    }

    void UpdateAI(uint32 diff) override
    {
        // Nobody came to fight him once landed: he leaves after a while
        if (_phase == PHASE_COMBAT && !UpdateVictim())
        {
            if (_idleTimer <= diff)
                me->DespawnOrUnsummon();
            else
                _idleTimer -= diff;
            return;
        }

        _events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_PREPARE:
                    Talk(TALK_PREPARE);
                    break;
                case EVENT_FIRST_FIRES:
                    IgniteFires(true);
                    break;
                case EVENT_FIRE_PASS:
                    if (!CountBurningFires())
                    {
                        FiresOut();
                        return;
                    }
                    IgniteFires(false);
                    _events.Repeat(FIRE_PASS_INTERVAL);
                    break;
                case EVENT_CONFLAGRATION:
                    CastConflagration();
                    _events.Repeat(_phase == PHASE_COMBAT ? 12 * IN_MILLISECONDS : 20 * IN_MILLISECONDS);
                    break;
                case EVENT_TALK_MIDWAY:
                    Talk(TALK_MIDWAY);
                    break;
                case EVENT_TALK_TIME_SHORT:
                    Talk(TALK_TIME_SHORT);
                    break;
                case EVENT_TIME_UP:
                    if (CountBurningFires())
                        Fail();
                    else
                        FiresOut();
                    return;
                case EVENT_CLEAVE:
                    DoCastVictim(SPELL_HORSEMAN_CLEAVE);
                    _events.Repeat(8 * IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }

        if (_phase == PHASE_COMBAT)
            DoMeleeAttackIfReady();
    }

private:
    // A flying spline makes the horse hover still: he gallops round the village on a ground spline, gravity off
    void GallopAround()
    {
        uint8 const steps = 24;
        float angle = _village->Center.GetAngle(me->GetPositionX(), me->GetPositionY());
        Movement::MoveSplineInit init(*me);
        for (uint8 i = 0; i < steps; ++i, angle -= 2.0f * float(M_PI) / steps)
            init.Path().push_back(G3D::Vector3(_village->Center.GetPositionX() + _village->Radius * std::cos(angle),
                _village->Center.GetPositionY() + _village->Radius * std::sin(angle), _village->Center.GetPositionZ()));
        init.SetWalk(false);
        init.SetCyclic();
        init.Launch();
    }

    void UpdatePlayerCount()
    {
        std::list<Player*> players;
        me->GetPlayerListInGrid(players, VILLAGE_PLAYER_RANGE);
        players.remove_if([](Player* player) { return !player->IsAlive() || player->isGameMaster(); });
        _playerCount = uint32(players.size());
    }

    uint32 CountBurningFires() const
    {
        uint32 count = 0;
        for (ObjectGuid const& guid : _fires)
            if (Creature* fire = ObjectAccessor::GetCreature(*me, guid))
                if (fire->AI() && fire->AI()->GetData(DATA_FIRE_LEVEL))
                    ++count;

        return count;
    }

    void IgniteFires(bool firstWave)
    {
        UpdatePlayerCount();

        std::list<Creature*> unlit;
        for (ObjectGuid const& guid : _fires)
            if (Creature* fire = ObjectAccessor::GetCreature(*me, guid))
                if (fire->AI() && !fire->AI()->GetData(DATA_FIRE_LEVEL))
                    unlit.push_back(fire);

        if (unlit.empty())
            return;

        // The more firefighters, the more fires he keeps burning at once
        uint32 count = firstWave ? 2 + _playerCount / 2 : 1 + _playerCount / 3;
        if (!firstWave)
        {
            uint32 const burning = uint32(_fires.size() - unlit.size());
            uint32 const cap = 3 + 2 * _playerCount;
            if (burning >= cap)
                return;
            count = std::min(count, cap - burning);
        }

        Trinity::Containers::RandomResizeList(unlit, std::min<uint32>(count, 8));
        for (Creature* fire : unlit)
        {
            fire->AI()->SetGUID(me->GetGUID());
            fire->AI()->DoAction(ACTION_IGNITE);
            me->CastSpell(fire, SPELL_START_FIRE, true);
        }
    }

    void CastConflagration()
    {
        Unit* target = nullptr;
        if (_phase == PHASE_COMBAT)
            target = SelectTarget(SELECT_TARGET_RANDOM, 0, 30.0f, true);
        else if (_phase == PHASE_FIRES)
        {
            std::list<Player*> players;
            me->GetPlayerListInGrid(players, 35.0f);
            players.remove_if([](Player* player) { return !player->IsAlive() || player->isGameMaster(); });
            if (!players.empty())
                target = Trinity::Containers::SelectRandomContainerElement(players);
        }

        if (!target || me->CastSpell(target, SPELL_CONFLAGRATION, true) != SPELL_CAST_OK)
            return;

        Talk(TALK_CONFLAGRATION);
        target->CastSpell(target, SPELL_CONFLAGRATION_SOUND, true);
    }

    void ExtinguishAll()
    {
        for (ObjectGuid const& guid : _fires)
            if (Creature* fire = ObjectAccessor::GetCreature(*me, guid))
                if (fire->AI())
                    fire->AI()->DoAction(ACTION_EXTINGUISH);
    }

    void FiresOut()
    {
        _phase = PHASE_LANDING;
        _events.Reset();
        Talk(TALK_FIRES_OUT);

        // Putting the fires out is what completes both Fires quests; killing him is a bonus
        std::list<Player*> players;
        me->GetPlayerListInGrid(players, VILLAGE_PLAYER_RANGE);
        for (Player* player : players)
            for (uint32 questId : { QUEST_STOP_THE_FIRES_A, QUEST_STOP_THE_FIRES_H, QUEST_LET_THE_FIRES_COME_A, QUEST_LET_THE_FIRES_COME_H })
                player->AreaExploredOrEventHappens(questId);

        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MovePoint(POINT_LANDING, _village->Landing, false);
    }

    void Fail()
    {
        _phase = PHASE_LEAVING;
        _events.Reset();
        Talk(TALK_FAILED);
        ExtinguishAll();

        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MovePoint(POINT_LEAVE, _village->Center.GetPositionX(), _village->Center.GetPositionY(), _village->Center.GetPositionZ() + 60.0f, false);
        me->DespawnOrUnsummon(20 * IN_MILLISECONDS);
    }

    HorsemanVillage const* _village = nullptr;
    GuidList _fires;
    EventMap _events;
    uint32 _playerCount = 1;
    uint32 _idleTimer = 0;
    uint8 _phase = 0;
};

// 24519 - Costumed Orphan Matron, 23973 - Orphan Matron Battlewail
class npc_hallows_end_orphan_matron : public CreatureScript
{
public:
    npc_hallows_end_orphan_matron() : CreatureScript("npc_hallows_end_orphan_matron") { }

    struct npc_hallows_end_orphan_matronAI : public ScriptedAI
    {
        npc_hallows_end_orphan_matronAI(Creature* creature) : ScriptedAI(creature) { }

        Creature* GetShade() const
        {
            Creature* shade = ObjectAccessor::GetCreature(*me, _shadeGUID);
            return shade && shade->IsAlive() ? shade : nullptr;
        }

        bool IsVillageBurning() const
        {
            Creature* shade = GetShade();
            return shade && shade->AI() && shade->AI()->GetData(DATA_FIRES_PHASE);
        }

        void sQuestAccept(Player* /*player*/, Quest const* quest) override
        {
            if (quest->GetQuestId() == QUEST_LET_THE_FIRES_COME_A || quest->GetQuestId() == QUEST_LET_THE_FIRES_COME_H)
                SummonShade();
        }

        void SummonShade()
        {
            if (GetShade())
                return;

            HorsemanVillage const* village = FindVillage(me, 150.0f);
            if (!village)
                return;

            if (Creature* shade = me->SummonCreature(NPC_SHADE_OF_THE_HORSEMAN, village->Center.GetPositionX() + village->Radius, village->Center.GetPositionY(),
                village->Center.GetPositionZ(), float(M_PI) / 2.0f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60 * IN_MILLISECONDS))
                _shadeGUID = shade->GetGUID();
        }

    private:
        ObjectGuid _shadeGUID;
    };

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        auto ai = dynamic_cast<npc_hallows_end_orphan_matronAI*>(creature->AI());
        if (!ai)
            return false;

        player->PlayerTalkClass->ClearMenus();
        player->PrepareGossipMenu(creature, creature->GetCreatureTemplate()->GossipMenuId, true);

        // The Shade does not outlive a restart: a player still on "Let the Fires Come!" calls him back
        for (uint32 questId : { QUEST_LET_THE_FIRES_COME_A, QUEST_LET_THE_FIRES_COME_H })
            if (player->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE)
                ai->SummonShade();

        // "Let the Fires Come!" calls the Shade, "Stop the Fires!" only makes sense while he is burning the town
        bool const shadePresent = ai->GetShade() != nullptr;
        bool const burning = ai->IsVillageBurning();
        QuestMenu& questMenu = player->PlayerTalkClass->GetQuestMenu();
        std::vector<QuestMenuItem> items;
        for (uint8 i = 0; i < questMenu.GetMenuItemCount(); ++i)
            items.push_back(questMenu.GetItem(i));

        questMenu.ClearMenu();
        for (QuestMenuItem const& item : items)
        {
            bool const offered = player->GetQuestStatus(item.QuestId) == QUEST_STATUS_NONE;
            switch (item.QuestId)
            {
                case QUEST_LET_THE_FIRES_COME_A:
                case QUEST_LET_THE_FIRES_COME_H:
                    if (offered && shadePresent)
                        continue;
                    break;
                case QUEST_STOP_THE_FIRES_A:
                case QUEST_STOP_THE_FIRES_H:
                    if (offered && !burning)
                        continue;
                    break;
                default:
                    break;
            }
            questMenu.AddMenuItem(item.QuestId, item.QuestIcon);
        }

        player->SendPreparedGossip(creature);
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_hallows_end_orphan_matronAI(creature);
    }
};

// 42339 - Bucket Lands
class spell_hallows_end_bucket_lands : public SpellScript
{
    PrepareSpellScript(spell_hallows_end_bucket_lands);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        // A bucket thrown at a friend is caught, but only one of them gets it
        if (Player* player = target->ToPlayer())
        {
            if (!_bucketCaught && player != caster && !player->HasItemCount(ITEM_WATER_BUCKET))
            {
                _bucketCaught = true;
                caster->CastSpell(player, SPELL_GIVE_WATER_BUCKET, true);
            }
            return;
        }

        Creature* fire = target->ToCreature();
        if (!fire || fire->GetEntry() != NPC_FIRE_NODE || !fire->AI() || !fire->AI()->GetData(DATA_FIRE_LEVEL))
            return;

        fire->AI()->DoAction(ACTION_SPLASH);

        if (fire->AI()->GetData(DATA_PRACTICE_FIRE) && !fire->AI()->GetData(DATA_FIRE_LEVEL))
            if (Player* player = caster->ToPlayer())
                player->KilledMonsterCredit(NPC_FIRE_NODE, fire->GetGUID());
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_hallows_end_bucket_lands::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }

    bool _bucketCaught = false;
};

// 42144 - Create Water Bucket, 42349 - Give Water Bucket
class spell_hallows_end_create_water_bucket : public SpellScript
{
    PrepareSpellScript(spell_hallows_end_create_water_bucket);

    // the script effect shows the throw button on screen
    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
            target->CastSpell(target, SPELL_WATER_BUCKET_OVERRIDE, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_hallows_end_create_water_bucket::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 42340 - Throw Bucket
class spell_hallows_end_throw_bucket : public SpellScript
{
    PrepareSpellScript(spell_hallows_end_throw_bucket);

    // the bag already pays for its own bucket; checked before the cast, which clears the cast item
    void HandleBeforeCast()
    {
        _fromBag = GetCastItem() != nullptr;
    }

    // thrown from the on-screen button rather than from the bag, the bucket must still leave the bag
    void HandleAfterCast()
    {
        if (_fromBag)
            return;

        if (Player* player = GetCaster()->ToPlayer())
            player->DestroyItemCount(ITEM_WATER_BUCKET, 1, true);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_hallows_end_throw_bucket::HandleBeforeCast);
        AfterCast += SpellCastFn(spell_hallows_end_throw_bucket::HandleAfterCast);
    }

    bool _fromBag = false;
};

void AddSC_hallows_end_village_fires()
{
    RegisterCreatureAI(npc_hallows_end_fire);
    RegisterCreatureAI(npc_shade_of_the_horseman);
    new npc_hallows_end_orphan_matron();
    RegisterSpellScript(spell_hallows_end_bucket_lands);
    RegisterSpellScript(spell_hallows_end_create_water_bucket);
    RegisterSpellScript(spell_hallows_end_throw_bucket);
}
