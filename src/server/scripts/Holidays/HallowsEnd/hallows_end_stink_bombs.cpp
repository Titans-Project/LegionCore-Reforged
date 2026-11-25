////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "CellImpl.h"
#include "ChatPackets.h"
#include "ChatTextBuilder.h"
#include "Containers.h"
#include "DB2Stores.h"
#include "GridNotifiersImpl.h"
#include "MoveSplineInit.h"
#include "MovementPackets.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SmartScriptMgr.h"
#include "SpellScript.h"
#include "Vehicle.h"
#include "World.h"
#include "WorldSession.h"

/*
    Hallow's End, "Stink Bombs Away!": the Fenlow witches lend a broom that flies on its own over the
    rival capital while the rider drops stink bombs, until "Return Home" brings it back to them.
*/

enum HallowsEndStinkBombs
{
    NPC_HAGS_BROOM                  = 51710,    // Alliance, bombs the Undercity
    NPC_CRONES_BROOM                = 53761,    // Horde, bombs Stormwind

    QUEST_STINK_BOMBS_AWAY_A        = 29054,
    QUEST_STINK_BOMBS_AWAY_H        = 29374,

    ZONE_UNDERCITY                  = 1497,
    ZONE_STORMWIND_CITY             = 1519,

    SPELL_RIDE_VEHICLE              = 46598,
    SPELL_OVER_UNDERCITY            = 96005,    // its aura is what the broom's two spells require
    SPELL_OVER_STORMWIND            = 100028,

    // trinity_string: what a bystander says under a stink bomb (BystanderRemarks)
    TEXT_BYSTANDER_UNDEAD           = 600330,
    MAX_UNDEAD_REMARKS              = 2,        // per flight, so that it stays a surprise

    MAX_RIDER_CACKLES               = 4,        // per flight, about one bomb in seven

    NPC_STINK_BOMB_CREDIT           = 51833,    // [DND] Broomstick Bombing Run Quest Credit

    DATA_LAST_REMARK                = 1,
    DATA_UNDEAD_REMARKS,
    DATA_RIDER_CACKLES,

    ACTION_RETURN_HOME              = 1,

    EVENT_BOARD_CHECK               = 1,
    EVENT_TAKEOFF_DONE,
    EVENT_JOIN_CIRCUIT,
    EVENT_START_CIRCUIT,
    EVENT_FLY_HOME,
    EVENT_LANDED,
    EVENT_LOCK_CONTROL,
};

static float constexpr BOMB_REACH = 3.0f;         // from where the bomb lands to the edge of whoever it hits
static float constexpr PARTNER_REACH = 5.0f;      // from a speaker to the neighbour who answers

enum BroomPhases
{
    BROOM_WAITING,
    BROOM_TAKEOFF,
    BROOM_CIRCUIT,
    BROOM_RETURNING,
};

struct BroomRoute
{
    uint32 Entry;
    uint32 Quest;
    uint32 TargetZone;                  // the rival capital
    Position Camp;                      // on the ground beside the witches
    std::vector<G3D::Vector3> Takeoff;  // from the witch up into the sky, before the teleport
    Position Away;                      // arrival in the rival capital
    uint32 AwayAura;                    // marks the broom as over it
    uint32 CircuitPath;                 // `waypoints` entry flown in a loop over it
    Position Home;                      // back above the witch's camp
    std::vector<G3D::Vector3> Landing;  // down to the witch, ending just above the ground
};

// Teleports done here rather than with 96005/96006/100028: a spell teleport of a creature into an unloaded
// grid is dropped server side while the client already shows the arrival, and 100027 has no destination
static BroomRoute const BroomRoutes[] =
{
    { NPC_HAGS_BROOM, QUEST_STINK_BOMBS_AWAY_A, ZONE_UNDERCITY, { -9109.5f, 418.0f, 94.4f, 3.74f },
      { { -9090.6f, 417.6f, 105.3f }, { -9028.8f, 452.7f, 117.6f } },
      { 1639.14f, 393.865f, 16.9f, 6.0f }, SPELL_OVER_UNDERCITY, 517100, { -9028.83f, 452.683f, 117.645f, 3.6f },
      { { -9060.0f, 438.0f, 110.0f }, { -9090.0f, 424.5f, 100.0f }, { -9104.0f, 419.5f, 96.0f }, { -9109.5f, 418.0f, 94.6f } } },
    { NPC_CRONES_BROOM, QUEST_STINK_BOMBS_AWAY_H, ZONE_STORMWIND_CITY, { 1814.5f, 219.5f, 60.6f, 0.23f },
      { { 1810.0f, 205.0f, 80.0f }, { 1804.2f, 189.2f, 95.0f } },
      { -8992.63f, 509.98f, 137.7f, 0.18f }, SPELL_OVER_STORMWIND, 537610, { 1680.0f, 320.0f, 105.0f, 5.5f },
      { { 1720.0f, 285.0f, 90.0f }, { 1775.0f, 240.0f, 78.0f }, { 1803.0f, 225.0f, 66.0f }, { 1814.5f, 219.5f, 60.6f } } },
};

enum BystanderKind : uint8
{
    BYSTANDER_ANY,
    BYSTANDER_UNDEAD,
    BYSTANDER_HUMAN,
};

// With a Reply, a neighbour of the same kind answers the Line; never drawn without one
struct BystanderRemark
{
    uint32 Line;
    uint8 Kind;
    uint32 Reply;
    uint32 ReplyEmote;
};

static BystanderRemark const BystanderRemarks[] =
{
    { 600300, BYSTANDER_ANY, 600302, EMOTE_ONESHOT_NO },            // Who farted?! - It wasn't me, I swear!
    { 600301 }, { 600303 }, { 600304 }, { 600306 }, { 600307 }, { 600309 }, { 600312 }, { 600313 },
    { 600314 }, { 600315 }, { 600316 }, { 600317 }, { 600318 }, { 600319 }, { 600320 }, { 600321 },
    { 600322 }, { 600323 },
    { 600331, BYSTANDER_UNDEAD },
    { 600340, BYSTANDER_ANY, 600341, EMOTE_ONESHOT_LAUGH },         // What did you just let rip?! - That's nature!
    { 600342, BYSTANDER_ANY, 600343, EMOTE_ONESHOT_QUESTION },      // I tasted your smell... - Well seasoned at least?
    { 600344, BYSTANDER_ANY, 600345, EMOTE_ONESHOT_EXCLAMATION },   // Marinated under that broom?! - Granny's recipe!
    { 600346, BYSTANDER_UNDEAD, 600347, EMOTE_ONESHOT_LAUGH },      // My nose just melted! - You weren't using it anyway!
    { 600348, BYSTANDER_HUMAN, 600349, EMOTE_ONESHOT_POINT },       // Even a dead rat would complain! - Don't snitch!
    { 600350, BYSTANDER_ANY, 600351, EMOTE_ONESHOT_BEG },           // That's an assault! - Not guilty, it was an accident!
    { 600352, BYSTANDER_UNDEAD, 600353, EMOTE_ONESHOT_POINT },      // My nostrils are burning! - You don't even have a nose!
    { 600354, BYSTANDER_UNDEAD, 600355, EMOTE_ONESHOT_EXCLAMATION },// Not washed since 1845! - Respect my microbiome!
    { 600356, BYSTANDER_UNDEAD, 600357, EMOTE_ONESHOT_FLEX },       // Bin juice in the sun! - It's vintage, very trendy!
    { 600358, BYSTANDER_HUMAN, 600359, EMOTE_ONESHOT_SHY },         // What happened in your pants?! - Between me and my doctor!
};

static uint8 GetDisplayRace(Creature const* creature)
{
    if (CreatureDisplayInfoEntry const* info = sCreatureDisplayInfoStore.LookupEntry(creature->GetDisplayId()))
        if (CreatureDisplayInfoExtraEntry const* extra = sCreatureDisplayInfoExtraStore.LookupEntry(info->ExtendedDisplayInfoID))
            return extra->DisplayRaceID;

    return 0;
}

static bool IsBystanderKind(Creature const* creature, uint8 kind)
{
    switch (kind)
    {
        // Most Forsaken are humanoids: only their model tells them apart
        case BYSTANDER_UNDEAD:  return creature->GetCreatureType() == CREATURE_TYPE_UNDEAD || GetDisplayRace(creature) == RACE_UNDEAD_PLAYER;
        case BYSTANDER_HUMAN:   return GetDisplayRace(creature) == RACE_HUMAN;
        default:                return true;
    }
}

static BroomRoute const* GetBroomRoute(uint32 entry)
{
    for (BroomRoute const& route : BroomRoutes)
        if (route.Entry == entry)
            return &route;

    return nullptr;
}

// 51710 - Hag's Broom, 53761 - Crone's Broom
struct npc_hallows_end_broom : public ScriptedAI
{
    npc_hallows_end_broom(Creature* creature) : ScriptedAI(creature) { }

    void InitializeAI() override
    {
        _route = GetBroomRoute(me->GetEntry());
        me->SetReactState(REACT_PASSIVE);
        me->SetCanFly(true);
        me->SetDisableGravity(true);
    }

    void Reset() override { }
    void EnterEvadeMode() override { }
    void OnCharmed(bool /*apply*/) override { }
    void MoveInLineOfSight(Unit* /*who*/) override { }

    void IsSummonedBy(Unit* summoner) override
    {
        if (summoner->IsPlayer())
            _riderGUID = summoner->GetGUID();

        _events.ScheduleEvent(EVENT_BOARD_CHECK, 1500);
    }

    void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply) override
    {
        Player* player = who->ToPlayer();
        if (!player)
            return;

        if (!apply)
        {
            // Off the broom, whatever the reason: it has no more purpose
            me->DespawnOrUnsummon(1000);
            return;
        }

        _riderGUID = player->GetGUID();
        if (_phase != BROOM_WAITING || !_route)
            return;

        // Keeps the grids under the broom loaded wherever it flies
        me->setActive(true);

        // The seat gives the rider the controls: the broom flies its own route instead
        SetControl(false);
        _phase = BROOM_TAKEOFF;
        _events.ScheduleEvent(EVENT_TAKEOFF_DONE, FlyAlong(_route->Takeoff));
        _events.ScheduleEvent(EVENT_LOCK_CONTROL, 500);
    }

    // Besides the two DATA_ values, a remark's text id gives how many times the rider heard it
    uint32 GetData(uint32 type) const override
    {
        switch (type)
        {
            case DATA_LAST_REMARK:      return _lastRemark;
            case DATA_UNDEAD_REMARKS:   return _undeadRemarks;
            default:
            {
                auto itr = _remarksHeard.find(type);
                return itr != _remarksHeard.end() ? itr->second : 0;
            }
        }
    }

    void SetData(uint32 type, uint32 value) override
    {
        switch (type)
        {
            case DATA_LAST_REMARK:      _lastRemark = value; break;
            case DATA_UNDEAD_REMARKS:   _undeadRemarks = value; break;
            default:                    _remarksHeard[type] = value; break;
        }
    }

    void DoAction(int32 const action) override
    {
        if (action != ACTION_RETURN_HOME || !_route || _phase == BROOM_RETURNING || _phase == BROOM_WAITING)
            return;

        _phase = BROOM_RETURNING;
        _events.CancelEvent(EVENT_TAKEOFF_DONE);
        _events.CancelEvent(EVENT_JOIN_CIRCUIT);
        _events.CancelEvent(EVENT_START_CIRCUIT);
        me->StopMoving();
        me->GetMotionMaster()->Clear();
        SetControl(false);
        _events.ScheduleEvent(EVENT_LOCK_CONTROL, 500);
        TeleportTo(_route->Home);
        _events.ScheduleEvent(EVENT_FLY_HOME, 1500);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_BOARD_CHECK:
                    // The summon normally seats the rider; make sure of it, or leave
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _riderGUID))
                    {
                        if (player->GetVehicleBase() != me && _phase == BROOM_WAITING && player->IsWithinDist(me, 30.0f))
                            player->CastSpell(me, SPELL_RIDE_VEHICLE, true);
                    }
                    else
                        me->DespawnOrUnsummon();
                    break;
                case EVENT_TAKEOFF_DONE:
                    TeleportTo(_route->Away);
                    _events.ScheduleEvent(EVENT_JOIN_CIRCUIT, 1500);
                    break;
                case EVENT_JOIN_CIRCUIT:
                    JoinCircuit();
                    break;
                case EVENT_START_CIRCUIT:
                    StartCircuit();
                    break;
                case EVENT_FLY_HOME:
                    if (!SettleAfterTeleport())
                    {
                        _events.ScheduleEvent(EVENT_FLY_HOME, 1000);
                        break;
                    }
                    _events.ScheduleEvent(EVENT_LANDED, FlyAlong(_route->Landing));
                    break;
                case EVENT_LANDED:
                    _events.CancelEvent(EVENT_LOCK_CONTROL);
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _riderGUID))
                    {
                        player->SetClientControl(me, true);
                        if (player->GetVehicleBase() == me)
                            player->ExitVehicle();
                    }
                    me->DespawnOrUnsummon(1000);
                    break;
                case EVENT_LOCK_CONTROL:
                    // The client hands the controls back after a teleport or a seat change
                    SetControl(false);
                    _events.ScheduleEvent(EVENT_LOCK_CONTROL, 3000);
                    break;
                default:
                    break;
            }
        }
    }

private:
    // Only locked while no spline holds the broom: see StartCircuit for giving it back
    void SetControl(bool allow)
    {
        // Only the packet: Player::SetClientControl also resets the mover, which stops the broom mid-flight
        if (Player* player = ObjectAccessor::GetPlayer(*me, _riderGUID))
        {
            if (player->GetVehicleBase() != me)
                return;

            WorldPackets::Movement::ControlUpdate update;
            update.Guid = me->GetGUID();
            update.On = allow;
            player->SendDirectMessage(update.Write());
        }
    }

    void TeleportTo(Position const& dest)
    {
        // An unloaded destination grid makes the map ignore the relocation
        _teleportDest = dest;
        me->GetMap()->LoadGrid(dest.GetPositionX(), dest.GetPositionY());
        me->NearTeleportTo(dest);
    }

    bool SettleAfterTeleport()
    {
        // A movement the client sent before seeing the teleport can still pull the broom back
        if (me->GetExactDist2d(_teleportDest.GetPositionX(), _teleportDest.GetPositionY()) > 40.0f && ++_teleportRetries <= 3)
        {
            TeleportTo(_teleportDest);
            return false;
        }

        _teleportRetries = 0;

        // The rider only follows the broom on its next move inside a cell: bring them along now
        if (Vehicle* vehicle = me->GetVehicleKit())
            vehicle->RelocatePassengers();
        SetControl(false);
        return true;
    }

    uint32 FlyAlong(std::vector<G3D::Vector3> const& points)
    {
        Movement::MoveSplineInit init(*me);
        init.Path().push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()));
        for (G3D::Vector3 const& point : points)
            init.Path().push_back(point);

        init.SetFly();
        init.SetSmooth();
        init.SetUncompressed();

        int32 duration = init.Launch();
        return uint32(std::max(duration, 500));
    }

    bool LoadCircuit()
    {
        _circuit.clear();
        if (WPPath* path = sSmartWaypointMgr->GetPath(_route->CircuitPath))
        {
            for (uint32 id = 1; id <= path->size(); ++id)
            {
                auto itr = path->find(id);
                if (itr == path->end())
                    break;

                // A zero-length segment makes the whole spline invalid (the sniffed loop repeats a point)
                G3D::Vector3 point(itr->second->x, itr->second->y, itr->second->z);
                if (_circuit.empty() || (point - _circuit.back()).length() >= 1.0f)
                    _circuit.push_back(point);
            }
        }

        while (_circuit.size() > 1 && (_circuit.back() - _circuit.front()).length() < 1.0f)
            _circuit.pop_back();

        return _circuit.size() >= 3;
    }

    void JoinCircuit()
    {
        if (!SettleAfterTeleport())
        {
            _events.ScheduleEvent(EVENT_JOIN_CIRCUIT, 1000);
            return;
        }

        // The teleport spell's own aura, without its teleport (see BroomRoutes)
        if (Aura* aura = me->AddAura(_route->AwayAura, me))
        {
            aura->SetMaxDuration(-1);
            aura->SetDuration(-1);
        }

        if (!LoadCircuit())
        {
            DoAction(ACTION_RETURN_HOME);
            return;
        }

        _phase = BROOM_CIRCUIT;
        _events.ScheduleEvent(EVENT_START_CIRCUIT, FlyAlong({ _circuit.front() }));
    }

    void StartCircuit()
    {
        // Without control the client also drops the broom as its mover and greys out its spells: hand both
        // back in full, before the spline since resetting the mover stops the broom
        _events.CancelEvent(EVENT_LOCK_CONTROL);
        if (Player* player = ObjectAccessor::GetPlayer(*me, _riderGUID))
            if (player->GetVehicleBase() == me)
                player->SetClientControl(me, true);

        // The spline starts from the broom, now on the first point, and closes the loop back to it
        Movement::MoveSplineInit init(*me);
        for (G3D::Vector3 const& point : _circuit)
            init.Path().push_back(point);
        init.SetFly();
        init.SetSmooth();
        init.SetUncompressed();
        init.SetCyclic();
        // The client follows a spline on the unit it moves and ignores its own input meanwhile
        if (init.Launch() <= 0)
            DoAction(ACTION_RETURN_HOME);
    }

    BroomRoute const* _route = nullptr;
    std::vector<G3D::Vector3> _circuit;
    Position _teleportDest;
    uint8 _teleportRetries = 0;
    uint32 _lastRemark = 0;
    uint32 _undeadRemarks = 0;
    std::unordered_map<uint32, uint32> _remarksHeard;
    ObjectGuid _riderGUID;
    EventMap _events;
    uint8 _phase = BROOM_WAITING;
};

// 96017, 100030 - Return Home
class spell_hallows_end_broom_return_home : public SpellScript
{
    PrepareSpellScript(spell_hallows_end_broom_return_home);

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        // Cast from the vehicle bar: the caster is the broom, or its rider on some clients
        Unit* broom = caster->IsVehicle() ? caster : caster->GetVehicleBase();
        if (Creature* creature = broom ? broom->ToCreature() : nullptr)
            if (creature->AI())
                creature->AI()->DoAction(ACTION_RETURN_HOME);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_hallows_end_broom_return_home::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 96024 - Toss Stink Bomb (the bomb lands)
class spell_hallows_end_stink_bomb_lands : public SpellScript
{
    PrepareSpellScript(spell_hallows_end_stink_bomb_lands);

    void HandleLanding(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        WorldLocation const* dest = GetHitDest();
        if (!caster || !dest)
            return;

        // Whoever stands under the cloud sniffs, shakes their head or recoils
        static uint32 const Reactions[] = { EMOTE_ONESHOT_SNIFF, EMOTE_ONESHOT_NO, EMOTE_ONESHOT_EXCLAMATION, EMOTE_ONESHOT_COWER };

        float const reach = caster->GetExactDist(dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ()) + 8.0f;
        std::list<Unit*> bystanders;
        std::list<Creature*> creatures;
        caster->GetCreatureListInGrid(creatures, reach);
        bystanders.insert(bystanders.end(), creatures.begin(), creatures.end());
        std::list<Player*> players;
        caster->GetPlayerListInGrid(players, reach);
        bystanders.insert(bystanders.end(), players.begin(), players.end());

        bool hitSomeone = false;
        std::vector<Creature*> speakers;
        speakers.reserve(bystanders.size());
        for (Unit* unit : bystanders)
        {
            // In 3D: a bomb in the street does not reach the floor above
            if (!unit->IsAlive() || unit->GetVehicleKit() || unit->GetVehicleBase() || unit->GetExactDist(dest) - unit->GetCombatReach() > BOMB_REACH)
                continue;

            if (Creature* creature = unit->ToCreature())
                if (creature->isTrigger() || creature->isWorldBoss() || creature->GetCreatureType() == CREATURE_TYPE_CRITTER)
                    continue;

            hitSomeone = true;
            if (unit->isInCombat())
                continue;

            // Only those who can talk: no bat asking who farted
            if (Creature* creature = unit->ToCreature())
                if (CanTalk(creature))
                    speakers.push_back(creature);

            unit->HandleEmoteCommand(Trinity::Containers::SelectRandomContainerElement(Reactions));
        }

        // The broom remembers what was said and how often the rider cackled during this flight
        Unit* broomUnit = caster->GetVehicleKit() ? caster : caster->GetVehicleBase();
        Creature* broom = broomUnit ? broomUnit->ToCreature() : nullptr;
        if (broom && (!GetBroomRoute(broom->GetEntry()) || !broom->AI()))
            broom = nullptr;

        Player* rider = caster->ToPlayer();
        if (!rider && broom)
            if (Vehicle* vehicle = broom->GetVehicleKit())
                if (Unit* passenger = vehicle->GetPassenger(0))
                    rider = passenger->ToPlayer();

        // Only a bomb that lands on someone counts (96117 no longer credits a throw at the bare ground)
        if (!hitSomeone)
            return;

        if (rider)
            rider->RewardPlayerAndGroupAtEvent(NPC_STINK_BOMB_CREDIT, rider);

        if (broom && rider)
            RiderCackle(broom, rider);

        if (speakers.empty())
            return;

        Creature* speaker = Trinity::Containers::SelectRandomContainerElement(speakers);
        uint32 lastRemark = broom ? broom->AI()->GetData(DATA_LAST_REMARK) : 0;

        // A partner of each kind, looked up once
        Creature* partners[3] = { };
        bool partnerSought[3] = { };
        auto partnerOfKind = [&](uint8 kind) -> Creature*
        {
            if (!partnerSought[kind])
            {
                partners[kind] = FindPartner(speaker, speakers, kind);
                partnerSought[kind] = true;
            }
            return partners[kind];
        };

        std::vector<BystanderRemark const*> pool;
        for (BystanderRemark const& remark : BystanderRemarks)
            if (remark.Line != lastRemark && IsBystanderKind(speaker, remark.Kind) && (!remark.Reply || partnerOfKind(remark.Kind)))
                pool.push_back(&remark);

        // The least heard first: over the 25 bombs of a flight the rider hears nearly every line, none a third time
        std::vector<BystanderRemark const*> choices;
        uint32 fewest = std::numeric_limits<uint32>::max();
        for (BystanderRemark const* remark : pool)
        {
            uint32 heard = broom ? broom->AI()->GetData(remark->Line) : 0;
            if (heard < fewest)
            {
                fewest = heard;
                choices.clear();
            }
            if (heard == fewest)
                choices.push_back(remark);
        }

        if (choices.empty())
            return;

        BystanderRemark const* remark = Trinity::Containers::SelectRandomContainerElement(choices);
        uint32 textId = remark->Line;
        if (broom && roll_chance_i(15) && IsBystanderKind(speaker, BYSTANDER_UNDEAD))
        {
            uint32 undeadRemarks = broom->AI()->GetData(DATA_UNDEAD_REMARKS);
            if (undeadRemarks < MAX_UNDEAD_REMARKS)
            {
                remark = nullptr;
                textId = TEXT_BYSTANDER_UNDEAD;
                broom->AI()->SetData(DATA_UNDEAD_REMARKS, undeadRemarks + 1);
            }
        }

        if (broom)
        {
            broom->AI()->SetData(DATA_LAST_REMARK, textId);
            broom->AI()->SetData(textId, broom->AI()->GetData(textId) + 1);
        }

        SayRemark(speaker, textId);

        if (remark && remark->Reply)
        {
            Creature* partner = partnerOfKind(remark->Kind);
            partner->AddDelayedEvent(2500, [partner, remark]() -> void
            {
                if (!partner->IsAlive() || partner->isInCombat())
                    return;

                partner->HandleEmoteCommand(remark->ReplyEmote);
                SayRemark(partner, remark->Reply);
            });
        }
    }

    // A witch's cackle now and then, never enough to get tiresome
    static void RiderCackle(Creature* broom, Player* rider)
    {
        uint32 cackles = broom->AI()->GetData(DATA_RIDER_CACKLES);
        if (cackles >= MAX_RIDER_CACKLES || !roll_chance_i(14))
            return;

        EmotesTextEntry const* cackle = sEmotesTextStore.LookupEntry(TEXT_EMOTE_CACKLE);
        if (!cackle)
            return;

        broom->AI()->SetData(DATA_RIDER_CACKLES, cackles + 1);
        rider->HandleEmoteCommand(cackle->EmoteID);

        // The text emote is what carries the character's own laughing voice
        WorldPackets::Chat::STextEmote textEmote;
        textEmote.SourceGUID = rider->GetGUID();
        textEmote.SourceAccountGUID = rider->GetSession()->GetAccountGUID();
        textEmote.EmoteID = TEXT_EMOTE_CACKLE;
        rider->SendMessageToSetInRange(textEmote.Write(), sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_TEXTEMOTE), true);
    }

    static bool CanTalk(Creature const* creature)
    {
        return creature->GetCreatureType() == CREATURE_TYPE_HUMANOID || creature->GetCreatureType() == CREATURE_TYPE_UNDEAD;
    }

    // Someone actually standing with the speaker: same floor, close by, in sight
    static bool IsPartnerOf(Creature const* speaker, Creature* other, uint8 kind)
    {
        return other != speaker && other->IsAlive() && !other->isInCombat() && !other->isTrigger() && !other->GetVehicleKit()
            && CanTalk(other) && IsBystanderKind(other, kind)
            && speaker->GetExactDist(other) <= PARTNER_REACH && std::fabs(speaker->GetPositionZ() - other->GetPositionZ()) <= 2.5f
            && speaker->IsWithinLOSInMap(other);
    }

    static Creature* FindPartner(Creature* speaker, std::vector<Creature*> const& speakers, uint8 kind)
    {
        for (Creature* other : speakers)
            if (IsPartnerOf(speaker, other, kind))
                return other;

        std::list<Creature*> nearby;
        speaker->GetCreatureListInGrid(nearby, PARTNER_REACH);
        for (Creature* other : nearby)
            if (IsPartnerOf(speaker, other, kind))
                return other;

        return nullptr;
    }

    static void SayRemark(Creature* speaker, uint32 textId)
    {
        Trinity::TrinityStringChatBuilder builder(speaker, CHAT_MSG_MONSTER_SAY, textId);
        Trinity::LocalizedPacketDo<Trinity::TrinityStringChatBuilder> localizer(builder);
        // Bombs fly up to 40 yards from a rider who is high up: a plain say range would not reach them
        float const range = std::max(sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_SAY), 60.0f);
        Trinity::PlayerDistWorker<Trinity::LocalizedPacketDo<Trinity::TrinityStringChatBuilder>> worker(speaker, range, localizer);
        CellCoord p = Trinity::ComputeCellCoord(speaker->GetPositionX(), speaker->GetPositionY());
        Cell cell(p);
        cell.SetNoCreate();
        cell.Visit(p, Trinity::makeWorldVisitor(worker), *speaker->GetMap(), *speaker, range);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_hallows_end_stink_bomb_lands::HandleLanding, EFFECT_0, SPELL_EFFECT_TRANS_DOOR);
    }
};

// 96117 - Stink Bomb Credit: thrown with every bomb, the credit is now given by the landing, only on a hit
class spell_hallows_end_stink_bomb_credit : public SpellScript
{
    PrepareSpellScript(spell_hallows_end_stink_bomb_credit);

    void PreventCredit(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_hallows_end_stink_bomb_credit::PreventCredit, EFFECT_0, SPELL_EFFECT_KILL_CREDIT);
    }
};

// A rider cut off in flight (disconnection, client or server crash) comes back without the broom, in mid-air
class player_hallows_end_stink_bombs : public PlayerScript
{
public:
    player_hallows_end_stink_bombs() : PlayerScript("player_hallows_end_stink_bombs") { }

    void OnLogin(Player* player) override
    {
        if (player->GetMapId() != 0 || player->GetVehicleBase())
            return;

        for (BroomRoute const& route : BroomRoutes)
        {
            QuestStatus status = player->GetQuestStatus(route.Quest);
            if (status != QUEST_STATUS_INCOMPLETE && status != QUEST_STATUS_COMPLETE)
                continue;

            // Over the rival capital, or in the air above the camp during takeoff or landing
            bool overTarget = player->GetZoneId() == route.TargetZone;
            bool aboveCamp = player->GetExactDist2d(&route.Camp) < 200.0f && player->GetPositionZ() > route.Camp.GetPositionZ() + 15.0f;
            if (overTarget || aboveCamp)
                player->TeleportTo(0, route.Camp.GetPositionX(), route.Camp.GetPositionY(), route.Camp.GetPositionZ(), route.Camp.GetOrientation());
            return;
        }
    }
};

void AddSC_hallows_end_stink_bombs()
{
    new player_hallows_end_stink_bombs();
    RegisterCreatureAI(npc_hallows_end_broom);
    RegisterSpellScript(spell_hallows_end_broom_return_home);
    RegisterSpellScript(spell_hallows_end_stink_bomb_lands);
    RegisterSpellScript(spell_hallows_end_stink_bomb_credit);
}
