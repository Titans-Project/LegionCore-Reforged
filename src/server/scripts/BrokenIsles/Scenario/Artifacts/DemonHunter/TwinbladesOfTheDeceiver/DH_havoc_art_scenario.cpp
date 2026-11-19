////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Demon Hunter (Havoc) artifact: By Any Means (Violet Hold, map 1494) and The Hunt (Felsoul Hold, map 1498)
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "GameObjectAI.h"
#include "ScenarioMgr.h"
#include "Scenario.h"
#include "SpellScript.h"
#include "Vehicle.h"
#include "MoveSpline.h"
#include "MoveSplineInit.h"
#include <algorithm>
#include <functional>
#include <limits>
#include <map>
#include <mutex>
#include <set>

// By Any Means: Taldath the Destroyer in the demon hunter's copy of the Violet Hold. Warden Alturas starts him with data
// 1 (he sees the hunter) and 2 (the cell opens); a scripted AI rather than SmartAI, so that the grip, the throw down, the
// teleport and the fight follow each other in order
enum TaldathByAnyMeans
{
    NPC_WARDEN_ALTURAS              = 96313,
    QUEST_BY_ANY_MEANS              = 39051,

    SAY_TALDATH_SEES_HUNTER         = 0,
    SAY_TALDATH_FOOL                = 1,
    SAY_TALDATH_VAREDIS             = 2,
    SAY_TALDATH_MY_LORD             = 3,
    SAY_TALDATH_SURAMAR             = 4,
    SAY_TALDATH_DEATH               = 5,

    SPELL_GRIP_OF_AGONY             = 191030,
    SPELL_SHADOW_NOVA               = 191028,
    SPELL_TELEPORT_VISUAL           = 216540,
    SPELL_FELSTOMP                  = 202583,
    SPELL_FEL_SLAGBLAST             = 202590,
    SPELL_CONV_WHERE_IS_VAREDIS     = 201105,
    SPELL_CONV_FINISH_HIM           = 201106,

    EVENT_INTRODUCED                = 1,
    EVENT_LEAVE_CELL,
    EVENT_SHADOW_NOVA,
    EVENT_TELEPORT,
    EVENT_RELEASE_GRIP,
    EVENT_FIGHT,
    EVENT_FELSTOMP,
    EVENT_FEL_SLAGBLAST,
    EVENT_SAY_MY_LORD,
    EVENT_SAY_SURAMAR,
    EVENT_INTERROGATION_OVER,

    POINT_OUT_OF_CELL               = 1,
};

Position const TaldathOutOfCell = { 1417.44f, -263.561f, 52.3996f };
Position const TaldathBeforeHunter = { 1366.5f, -263.5f, 39.0f, float(M_PI) };

struct npc_taldath_by_any_means : public ScriptedAI
{
    npc_taldath_by_any_means(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;
    bool sequence = false;
    bool interrogated = false;
    bool interrogationOver = false;
    bool finishHim = false;
    bool released = false;      // the cell was opened once: kept when he goes back to it
    bool started = false;

    void ForEachHunter(float range, std::function<void(Player*)> const& fn)
    {
        std::list<Player*> players;
        me->GetPlayerListInGrid(players, range);
        for (Player* player : players)
            fn(player);
    }

    void SetData(uint32 type, uint32 value) override
    {
        if (type == 1 && value == 1)
        {
            Player* hunter = me->FindNearestPlayer(60.0f);
            Talk(SAY_TALDATH_SEES_HUNTER, hunter ? hunter->GetGUID() : ObjectGuid::Empty);
            events.ScheduleEvent(EVENT_INTRODUCED, 11500);
        }
        else if (type == 2 && value == 2 && !started)
        {
            started = true;
            released = true;
            // weakened by his captivity, and he does not heal before the fight
            me->setRegeneratingHealth(false);
            me->SetHealth(me->CountPctFromMaxHealth(26));
            Talk(SAY_TALDATH_FOOL);
            events.ScheduleEvent(EVENT_LEAVE_CELL, 3500);
        }
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type != POINT_MOTION_TYPE || id != POINT_OUT_OF_CELL)
            return;

        sequence = true;
        me->SetReactState(REACT_PASSIVE);
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_STUNNED);
        DoCast(me, SPELL_GRIP_OF_AGONY, true);
        Talk(SAY_TALDATH_VAREDIS);
        // the grip shows a moment before the Shadow Nova throws the hunter down
        events.ScheduleEvent(EVENT_SHADOW_NOVA, 1500);
    }

    void EnterEvadeMode() override
    {
        // the hunter is out of reach while he is thrown down: no evade during the sequence
        if (sequence)
            return;
        ScriptedAI::EnterEvadeMode();
    }

    void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType /*dmgType*/) override
    {
        uint64 const left = me->GetHealth() > damage ? me->GetHealth() - damage : 0;
        float const pct = float(left) * 100.0f / float(me->GetMaxHealth());

        // he is released at a quarter of his health: 80% and 20% of that quarter
        if (!interrogated && pct <= 21.0f)
        {
            interrogated = true;
            ForEachHunter(100.0f, [](Player* player) { player->CastSpell(player, SPELL_CONV_WHERE_IS_VAREDIS, true); });
            events.ScheduleEvent(EVENT_SAY_MY_LORD, 5900);
        }
        if (interrogationOver && !finishHim && pct <= 5.0f)
            FinishHim();
    }

    void FinishHim()
    {
        finishHim = true;
        ForEachHunter(100.0f, [](Player* player) { player->CastSpell(player, SPELL_CONV_FINISH_HIM, true); });
    }

    // back in his cell after the hunter died, left or turned gamemaster: whole again, out of reach, ready to start over
    void JustReachedHome() override
    {
        events.Reset();
        sequence = interrogated = interrogationOver = finishHim = started = false;
        me->setRegeneratingHealth(true);
        me->SetFullHealth();
        me->SetReactState(REACT_AGGRESSIVE);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
    }

    // once the cell has been opened, a hunter on the quest coming back to it starts the fight again
    void MoveInLineOfSight(Unit* who) override
    {
        if (released && !started && !me->isInCombat() && !me->IsInEvadeMode())
            if (Player* player = who->ToPlayer())
                if (!player->isGameMaster() && player->GetQuestStatus(QUEST_BY_ANY_MEANS) == QUEST_STATUS_INCOMPLETE && me->IsWithinDistInMap(player, 20.0f))
                    SetData(2, 2);

        ScriptedAI::MoveInLineOfSight(who);
    }

    void JustDied(Unit* /*killer*/) override
    {
        Talk(SAY_TALDATH_DEATH);
        events.Reset();
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_INTRODUCED:
                    if (Creature* warden = me->FindNearestCreature(NPC_WARDEN_ALTURAS, 100.0f))
                        warden->AI()->SetData(3, 3);
                    break;
                case EVENT_LEAVE_CELL:
                    me->SetWalk(true);
                    me->GetMotionMaster()->MovePoint(POINT_OUT_OF_CELL, TaldathOutOfCell);
                    break;
                case EVENT_SHADOW_NOVA:
                    DoCast(me, SPELL_SHADOW_NOVA, true);
                    // he ends his line about Varedis (9.2 s in French, the longest voice) before he teleports
                    events.ScheduleEvent(EVENT_TELEPORT, 8200);
                    break;
                case EVENT_TELEPORT:
                    me->GetMotionMaster()->Clear();
                    me->NearTeleportTo(TaldathBeforeHunter);
                    DoCast(me, SPELL_TELEPORT_VISUAL, true);
                    events.ScheduleEvent(EVENT_RELEASE_GRIP, 500);
                    break;
                case EVENT_RELEASE_GRIP:
                    ForEachHunter(100.0f, [](Player* player) { player->RemoveAurasDueToSpell(SPELL_GRIP_OF_AGONY); });
                    events.ScheduleEvent(EVENT_FIGHT, 500);
                    break;
                case EVENT_FIGHT:
                    sequence = false;
                    me->SetReactState(REACT_AGGRESSIVE);
                    if (Player* hunter = me->FindNearestPlayer(100.0f))
                        AttackStart(hunter);
                    events.ScheduleEvent(EVENT_FELSTOMP, urand(5000, 7000));
                    events.ScheduleEvent(EVENT_FEL_SLAGBLAST, urand(9000, 11000));
                    break;
                case EVENT_FELSTOMP:
                    DoCastVictim(SPELL_FELSTOMP);
                    events.ScheduleEvent(EVENT_FELSTOMP, urand(12000, 15000));
                    break;
                case EVENT_FEL_SLAGBLAST:
                    DoCastVictim(SPELL_FEL_SLAGBLAST);
                    events.ScheduleEvent(EVENT_FEL_SLAGBLAST, urand(15000, 18000));
                    break;
                case EVENT_SAY_MY_LORD:
                    Talk(SAY_TALDATH_MY_LORD);
                    events.ScheduleEvent(EVENT_SAY_SURAMAR, 10800);
                    break;
                case EVENT_SAY_SURAMAR:
                    Talk(SAY_TALDATH_SURAMAR);
                    events.ScheduleEvent(EVENT_INTERROGATION_OVER, 9500);
                    break;
                case EVENT_INTERROGATION_OVER:
                    interrogationOver = true;
                    if (!finishHim && me->GetHealthPct() <= 5.0f)
                        FinishHim();
                    break;
                default:
                    break;
            }
        }

        if (!sequence && UpdateVictim())
            DoMeleeAttackIfReady();
    }
};

// The Hunt: the Illidari fel bat of Krasus' Landing. In Dalaran it takes off, then Teleport to Felsoul Hold takes the hunter
// to the scenario, where a bat summoned on arrival flies him over the hold with two escorts the Felsoul Obliterators shoot
// down; at the end the Dive button throws him, in Metamorphosis, onto the camp below
enum TheHunt
{
    MAP_DALARAN_BROKEN_ISLES        = 1220,
    MAP_FELSOUL_HOLD_SCENARIO       = 1498,
    QUEST_THE_HUNT                  = 39247,

    NPC_ILLIDARI_FEL_BAT_HUNT       = 94324,
    NPC_ILLIDARI_RIDER_1            = 94772,
    NPC_ILLIDARI_RIDER_2            = 94831,
    NPC_FELSOUL_OBLITERATOR         = 101695,
    NPC_VAREDIS_FELSOUL             = 94836,
    NPC_DIVE                        = 94348,

    SPELL_RIDE_VEHICLE_HARDCODED    = 46598,
    SPELL_FLY_TO_FELSOUL_HOLD       = 186709,
    SPELL_DIVE_METAMORPHOSIS        = 200651,
    SPELL_DIVE_IMPACT               = 200166,
    SPELL_DH_CAMP_EXPLOSION         = 186771,
    SPELL_CHAOS_BOMB_MISSILE        = 189716,
    SPELL_CHAOS_BOMB_DEATH          = 190205,
    SPELL_CONV_WARDS                = 200668,
    SPELL_CONV_SPOTTED              = 200670,
    SPELL_CONV_NO_TIME_TO_LAND      = 200673,

    // scenario step 1, Fly into Felsoul Hold
    CRITERIA_FLY_INTO_FELSOUL_HOLD  = 45563,

    // Havoc Metamorphosis models, 200651 itself always shows the female one
    DISPLAY_METAMORPHOSIS_MALE      = 68671,
    DISPLAY_METAMORPHOSIS_FEMALE    = 68670,

    EVENT_BOARD                     = 1,
    EVENT_ESCORT_DOWN,
    EVENT_AUTO_DIVE,
    EVENT_DIVE_JUMP,
    EVENT_IMPACT,
    EVENT_GONE,
    EVENT_TELEPORT_HUNTER,
    EVENT_CANNON_FIRE,

    POINT_LEAVE                     = 100,
    POINT_DIVE                      = 101,
};

float const HuntFlightSpeed = 14.0f;
float const HuntDiveSpeed = 38.0f;

// takeoff from Krasus' Landing, on the east edge of the city: up and out over the edge, then a turn to the left, north
// towards Suramar, before the loading screen
Position const HuntDalaranPath[] =
{
    { -845.0f, 4240.0f, 758.0f },
    { -843.5f, 4210.0f, 771.0f },
    { -834.0f, 4180.0f, 781.0f },
    { -811.0f, 4159.0f, 788.0f },
    { -777.0f, 4149.0f, 794.0f },
    { -738.0f, 4148.0f, 798.0f },
};

Position const HuntArrival = { 753.71f, 4963.45f, 124.5f, 5.92892f };

// over Felsoul Hold, down to the camp of the third Obliterator
Position const HuntFlightPath[] =
{
    {  775.433f, 4955.64f, 142.804f }, {  791.62f,  4949.78f, 145.948f }, {  820.647f, 4938.9f,  148.227f },
    {  856.059f, 4923.71f, 144.645f }, {  866.872f, 4920.59f, 142.084f }, {  901.093f, 4917.54f, 138.154f },
    {  918.858f, 4916.62f, 135.935f }, {  944.145f, 4922.75f, 133.764f }, {  955.788f, 4929.62f, 133.373f },
    {  985.558f, 4947.99f, 132.301f }, { 1037.11f,  4977.64f, 129.836f }, { 1085.18f,  5010.5f,  128.179f },
    { 1101.24f,  5024.3f,  128.873f }, { 1132.98f,  5054.3f,  130.085f }, { 1174.7f,   5099.71f, 128.778f },
    { 1203.82f,  5139.08f, 130.317f }, { 1230.73f,  5175.46f, 131.738f },
};

Position const HuntImpact = { 1263.34f, 5236.33f, 93.8f };
Position const HuntDiveEnd = { 1263.34f, 5236.33f, 95.3f };
Position const HuntBatLeaves = { 1330.0f, 5330.0f, 160.0f };

uint32 const HuntPathSize = std::extent<decltype(HuntFlightPath)>::value;
uint32 const HuntDalaranPathSize = std::extent<decltype(HuntDalaranPath)>::value;

// points reached (1-based) where the Kayn talking heads start and where each escort is shot down; the first one falls
// as the bat reaches the 11th point, and Kayn's "You've been spotted" follows its fall
uint32 const HuntPointWards = 2;
uint32 const HuntPointFirstEscortDown = 10;
uint32 const HuntPointSecondEscortDown = 13;
uint32 const HuntPointNoTimeToLand = 16;

// a shot meant to miss the hunter: a point 18 to 28 yards away from him, above or below, on his sides or behind him,
// never ahead where his bat is flying to
// A shot at a flying bat that misses: it bursts short of the bat, or flies on wide past it. The whole way of the missile
// keeps clear of where the bat is and will be over the next seconds, a shot aimed only behind it crossing its path
Position NearMiss(Unit const* shooter, Unit const* target)
{
    G3D::Vector3 const from(shooter->GetPositionX(), shooter->GetPositionY(), shooter->GetPositionZ());
    G3D::Vector3 const at(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
    G3D::Vector3 const ahead(std::cos(target->GetOrientation()), std::sin(target->GetOrientation()), 0.0f);

    G3D::Vector3 aim = at - from;
    aim.z = 0.0f;
    float const flat = aim.length();
    aim = flat > 0.1f ? aim / flat : ahead;
    G3D::Vector3 const side(-aim.y, aim.x, 0.0f);

    // how near the missile's straight way passes to the bat, now and in the next three seconds
    auto clearance = [&](G3D::Vector3 const& to) -> float
    {
        G3D::Vector3 const way = to - from;
        float const length2 = way.squaredLength();
        float nearest = std::numeric_limits<float>::max();
        for (float t = 0.0f; t <= 3.0f; t += 0.5f)
        {
            G3D::Vector3 const bat = at + ahead * (HuntFlightSpeed * t);
            float const along = length2 > 0.0f ? G3D::clamp((bat - from).dot(way) / length2, 0.0f, 1.0f) : 0.0f;
            nearest = std::min(nearest, (from + way * along - bat).length());
        }
        return nearest;
    };

    G3D::Vector3 best = at + side * 20.0f;
    float bestClearance = -1.0f;
    for (uint8 i = 0; i < 12 && bestClearance < 12.0f; ++i)
    {
        float const sideways = (urand(0, 1) ? 1.0f : -1.0f);
        G3D::Vector3 to;
        if (urand(0, 9) < 6)
            to = from + (at - from) * frand(0.35f, 0.7f) + side * (sideways * frand(8.0f, 15.0f)) + G3D::Vector3(0.0f, 0.0f, frand(-10.0f, -3.0f));
        else
            to = at + aim * frand(18.0f, 30.0f) + side * (sideways * frand(12.0f, 20.0f)) + G3D::Vector3(0.0f, 0.0f, frand(-10.0f, 10.0f));

        float const clear = clearance(to);
        if (clear > bestClearance)
        {
            bestClearance = clear;
            best = to;
        }
    }
    return { best.x, best.y, best.z };
}

void FlySpline(Creature* bat, std::vector<Position> const& points, float speed);

struct npc_illidari_fel_bat_the_hunt : public ScriptedAI
{
    npc_illidari_fel_bat_the_hunt(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;
    ObjectGuid hunterGuid;
    ObjectGuid hiddenWingmen[2];
    ObjectGuid leaderGuid;
    ObjectGuid riderGuid;
    ObjectGuid escortGuids[2];
    ObjectGuid diveGuid;
    uint8 escortsDown = 0;
    bool flying = false;
    bool diving = false;
    int8 side = 0;
    uint8 boardTries = 0;

    Player* GetHunter() const { return ObjectAccessor::GetPlayer(*me, hunterGuid); }

    void IsSummonedBy(Unit* summoner) override
    {
        me->SetCanFly(true);
        me->SetDisableGravity(true);
        me->SetReactState(REACT_PASSIVE);

        if (Player* player = summoner->ToPlayer())
        {
            hunterGuid = player->GetGUID();
            // in the scenario the bat waits for the end of the loading screen before the hunter mounts
            if (me->GetMapId() == MAP_DALARAN_BROKEN_ISLES)
                player->CastSpell(me, SPELL_RIDE_VEHICLE_HARDCODED, true);
            else
                events.ScheduleEvent(EVENT_BOARD, 1500);
            return;
        }

        // an escort: the hunter's bat leads it, and gives it its side and its rider
        leaderGuid = summoner->GetGUID();
    }

    void SetSide(int8 value)
    {
        side = value;
        uint32 const riderEntry = side < 0 ? NPC_ILLIDARI_RIDER_1 : NPC_ILLIDARI_RIDER_2;
        if (Creature* rider = me->SummonCreature(riderEntry, me->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN))
        {
            rider->SetReactState(REACT_PASSIVE);
            rider->EnterVehicle(me, 0);
            riderGuid = rider->GetGUID();
        }
    }

    // escorts fly on the same legs, well apart from the hunter: the left one ahead of him, the right one a little closer
    // behind him
    static Position EscortPoint(uint32 index, int8 escortSide)
    {
        Position const& to = HuntFlightPath[index];
        Position const& from = index ? HuntFlightPath[index - 1] : HuntArrival;
        float const angle = std::atan2(to.GetPositionY() - from.GetPositionY(), to.GetPositionX() - from.GetPositionX());
        float const sideAngle = angle + float(escortSide) * float(M_PI) / 2.0f;
        float const ahead = escortSide < 0 ? 10.0f : -7.0f;
        return { to.GetPositionX() + 14.0f * std::cos(sideAngle) + ahead * std::cos(angle),
                 to.GetPositionY() + 14.0f * std::sin(sideAngle) + ahead * std::sin(angle), to.GetPositionZ() + 2.0f };
    }

    void FlyTo(uint32 index)
    {
        me->GetMotionMaster()->MovePoint(index + 1, HuntFlightPath[index], false, HuntFlightSpeed);
        for (uint8 i = 0; i < 2; ++i)
            if (Creature* escort = ObjectAccessor::GetCreature(*me, escortGuids[i]))
                if (escort->IsAlive())
                    escort->GetMotionMaster()->MovePoint(index + 1, EscortPoint(index, i ? 1 : -1), false, HuntFlightSpeed);
    }

    void SummonEscorts()
    {
        for (uint8 i = 0; i < 2; ++i)
        {
            int8 const escortSide = i ? 1 : -1;
            Position pos = EscortPoint(0, escortSide);
            pos.m_positionZ = HuntArrival.GetPositionZ() + 2.0f;
            if (TempSummon* escort = me->SummonCreature(NPC_ILLIDARI_FEL_BAT_HUNT, pos, TEMPSUMMON_MANUAL_DESPAWN))
            {
                escortGuids[i] = escort->GetGUID();
                if (auto ai = CAST_AI(npc_illidari_fel_bat_the_hunt, escort->AI()))
                    ai->SetSide(escortSide);
            }
        }
    }

    void ShootDownEscort()
    {
        uint8 const i = escortsDown++;
        Creature* escort = ObjectAccessor::GetCreature(*me, escortGuids[i]);
        if (!escort || !escort->IsAlive())
            return;

        if (Creature* obliterator = escort->FindNearestCreature(NPC_FELSOUL_OBLITERATOR, 250.0f))
            obliterator->CastSpell(escort->GetPositionX(), escort->GetPositionY(), escort->GetPositionZ(), SPELL_CHAOS_BOMB_MISSILE, true);
        events.ScheduleEvent(EVENT_ESCORT_DOWN + (i << 4), 1500);
    }

    void EscortDown(uint8 i)
    {
        Creature* escort = ObjectAccessor::GetCreature(*me, escortGuids[i]);
        if (!escort || !escort->IsAlive())
            return;

        if (auto ai = CAST_AI(npc_illidari_fel_bat_the_hunt, escort->AI()))
            ai->Fall();

        if (i == 0)
            if (Player* hunter = GetHunter())
                hunter->CastSpell(hunter, SPELL_CONV_SPOTTED, true);
    }

    // shot down: the rider falls with the bat, and both are gone a little later
    void Fall()
    {
        if (Creature* rider = ObjectAccessor::GetCreature(*me, riderGuid))
        {
            rider->ExitVehicle();
            rider->SetDisableGravity(true);
            rider->CastSpell(rider, SPELL_CHAOS_BOMB_DEATH, true);
            rider->DespawnOrUnsummon(8000);
        }
        me->CastSpell(me, SPELL_CHAOS_BOMB_DEATH, true);
        me->DespawnOrUnsummon(8000);
    }

    void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply) override
    {
        if (!who->IsPlayer())
            return;

        if (apply)
        {
            who->SetDisableGravity(false);
            if (flying)
                return;
            flying = true;
            if (me->GetMapId() == MAP_DALARAN_BROKEN_ISLES)
                TakeOff();
            else
            {
                SummonEscorts();
                FlyTo(0);
                events.ScheduleEvent(EVENT_CANNON_FIRE, 4000);
            }
            return;
        }

        // the hunter left before his dive (teleport, logout, death): the flight ends
        if (!diving)
            End(0);
    }

    // the takeoff from Krasus' Landing is one smooth curve through its points, then the teleport to Felsoul Hold
    void TakeOff()
    {
        // the flight ends far out over the sea where the hunter leaves: with nobody near, a creature is no longer updated
        // and would hang there, its despawn never come, until he is back at that very spot on his way home
        me->setActive(true);
        Movement::MoveSplineInit init(*me);
        init.Path().push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()));
        for (Position const& point : HuntDalaranPath)
            init.Path().push_back(G3D::Vector3(point.GetPositionX(), point.GetPositionY(), point.GetPositionZ()));
        init.SetSmooth();
        init.SetFly();
        init.SetUncompressed();
        init.SetVelocity(HuntFlightSpeed);
        init.Launch();
        uint32 const duration = uint32(me->movespline->Duration());
        events.ScheduleEvent(EVENT_TELEPORT_HUNTER, duration);
        TakeOffWingmen(duration);
    }

    // Lyana and Illysanna wait on their bats at the edge of the landing and follow the hunter once he is off, on his left,
    // a little behind. Copies only he sees: the two at the edge stay there for everyone else, hidden from him meanwhile
    void TakeOffWingmen(uint32 duration)
    {
        Player* hunter = GetHunter();
        if (!hunter)
            return;

        static constexpr uint32 NPC_LYANA_DARKSORROW = 103491;
        static constexpr uint32 NPC_ILLYSANNA_RAVENCREST = 103492;
        struct Wingman { uint32 entry; float aside; float behind; uint32 delay; };
        static Wingman const wingmen[2] = { { NPC_ILLYSANNA_RAVENCREST, 9.0f, 4.0f, 700 }, { NPC_LYANA_DARKSORROW, 18.0f, 9.0f, 1200 } };

        for (uint8 i = 0; i < 2; ++i)
        {
            Creature* original = me->FindNearestCreature(wingmen[i].entry, 60.0f);
            if (!original)
                continue;

            original->AddToHideList(hunter->GetGUID());
            original->DestroyForPlayer(hunter);
            hiddenWingmen[i] = original->GetGUID();

            TempSummon* copy = me->SummonCreature(wingmen[i].entry, original->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, duration + 2000, 0, hunter->GetGUID());
            if (!copy)
                continue;

            copy->setActive(true);
            copy->SetReactState(REACT_PASSIVE);
            copy->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER);
            copy->SetCanFly(true);
            copy->SetDisableGravity(true);

            // the hunter's way, moved to his left and back: each point beside the leg that leads to it
            std::vector<Position> path;
            Position from = me->GetPosition();
            for (Position const& to : HuntDalaranPath)
            {
                float const angle = std::atan2(to.GetPositionY() - from.GetPositionY(), to.GetPositionX() - from.GetPositionX());
                float const left = angle + float(M_PI) / 2.0f;
                path.push_back({ to.GetPositionX() + wingmen[i].aside * std::cos(left) - wingmen[i].behind * std::cos(angle),
                    to.GetPositionY() + wingmen[i].aside * std::sin(left) - wingmen[i].behind * std::sin(angle), to.GetPositionZ() + 1.0f });
                from = to;
            }

            copy->AddDelayedEvent(wingmen[i].delay, [copy, path]() -> void { FlySpline(copy, path, HuntFlightSpeed); });
        }
    }

    // once he is gone from the landing, the two at the edge are his to see again
    void RestoreWingmen()
    {
        for (ObjectGuid& guid : hiddenWingmen)
        {
            if (Creature* original = ObjectAccessor::GetCreature(*me, guid))
                original->RemoveFromHideList(hunterGuid);
            guid.Clear();
        }
    }

    void End(uint32 delay)
    {
        RestoreWingmen();
        for (ObjectGuid const& guid : escortGuids)
            if (Creature* escort = ObjectAccessor::GetCreature(*me, guid))
                if (escort->IsAlive())
                {
                    if (auto ai = CAST_AI(npc_illidari_fel_bat_the_hunt, escort->AI()))
                        ai->Despawn(delay);
                }
        Despawn(delay);
    }

    void Despawn(uint32 delay)
    {
        if (Creature* rider = ObjectAccessor::GetCreature(*me, riderGuid))
            rider->DespawnOrUnsummon(delay + 100);
        me->DespawnOrUnsummon(delay + 100);
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type != POINT_MOTION_TYPE || !hunterGuid || id == POINT_LEAVE)
            return;

        Player* hunter = GetHunter();
        if (!hunter)
            return;

        if (me->GetMapId() == MAP_DALARAN_BROKEN_ISLES)
            return;

        switch (id)
        {
            case HuntPointWards:
                hunter->CastSpell(hunter, SPELL_CONV_WARDS, true);
                break;
            case HuntPointFirstEscortDown:
            case HuntPointSecondEscortDown:
                ShootDownEscort();
                break;
            case HuntPointNoTimeToLand:
                hunter->CastSpell(hunter, SPELL_CONV_NO_TIME_TO_LAND, true);
                break;
            default:
                break;
        }

        if (id < HuntPathSize)
        {
            FlyTo(id);
            return;
        }

        // over the camp, Fly into Felsoul Hold is done and the hunter dives at once
        hunter->UpdateAchievementCriteria(CRITERIA_TYPE_SCRIPT_EVENT_2, CRITERIA_FLY_INTO_FELSOUL_HOLD, 1);
        me->SetFacingTo(me->GetAngle(HuntImpact.GetPositionX(), HuntImpact.GetPositionY()));
        events.ScheduleEvent(EVENT_AUTO_DIVE, 800);
    }

    // the official dive: the hunter leaves the bat for the invisible Dive vehicle, whose seat holds him head first, and
    // falls onto the camp in Metamorphosis
    void Dive()
    {
        Player* hunter = GetHunter();
        if (diving || !hunter)
            return;

        diving = true;
        events.CancelEvent(EVENT_CANNON_FIRE);
        if (TempSummon* dive = me->SummonCreature(NPC_DIVE, hunter->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN))
        {
            dive->SetCanFly(true);
            dive->SetDisableGravity(true);
            dive->SetReactState(REACT_PASSIVE);
            diveGuid = dive->GetGUID();
        }
        hunter->ExitVehicle();
        events.ScheduleEvent(EVENT_DIVE_JUMP, 100);
        me->GetMotionMaster()->MovePoint(POINT_LEAVE, HuntBatLeaves, false, HuntFlightSpeed);
    }

    // the impact throws the demons of the camp into the air, and they die where they fall; the Obliterator is destroyed
    // and disappears
    void Impact(Player* hunter)
    {
        Position const ground = { HuntImpact.GetPositionX(), HuntImpact.GetPositionY(), HuntImpact.GetPositionZ(), hunter->GetOrientation() };
        hunter->ExitVehicle(&ground);
        hunter->RemoveAurasDueToSpell(SPELL_DIVE_METAMORPHOSIS);
        if (Creature* dive = ObjectAccessor::GetCreature(*me, diveGuid))
            dive->DespawnOrUnsummon(500);

        hunter->CastSpell(hunter, SPELL_DIVE_IMPACT, true);
        hunter->CastSpell(hunter, SPELL_DH_CAMP_EXPLOSION, true);

        std::list<Creature*> creatures;
        hunter->GetCreatureListInGrid(creatures, 40.0f);
        for (Creature* creature : creatures)
        {
            switch (creature->GetEntry())
            {
                case NPC_VAREDIS_FELSOUL:
                case NPC_ILLIDARI_FEL_BAT_HUNT:
                case NPC_DIVE:
                case NPC_ILLIDARI_RIDER_1:
                case NPC_ILLIDARI_RIDER_2:
                    continue;
                default:
                    break;
            }
            if (!creature->IsAlive() || creature->isTrigger() || creature->IsFriendlyTo(hunter))
                continue;

            if (creature->GetEntry() == NPC_FELSOUL_OBLITERATOR)
            {
                creature->CastSpell(creature, SPELL_CHAOS_BOMB_DEATH, true);
                creature->DespawnOrUnsummon(1500);
                continue;
            }

            float const away = HuntImpact.GetAngle(creature);
            float const dist = frand(8.0f, 14.0f);
            Position pos = { creature->GetPositionX() + dist * std::cos(away), creature->GetPositionY() + dist * std::sin(away),
                             creature->GetPositionZ() };
            creature->UpdateGroundPositionZ(pos.m_positionX, pos.m_positionY, pos.m_positionZ);
            creature->SetReactState(REACT_PASSIVE);
            creature->AttackStop();
            creature->GetMotionMaster()->MoveJump(pos, 15.0f, 25.0f);
            // dead once fallen back (the jump lasts dist / 15 s); the arrival cast of the jump did not fire here
            uint32 const fall = uint32(creature->GetExactDist2d(&pos) / 15.0f * IN_MILLISECONDS) + 400;
            creature->AddDelayedEvent(fall, [creature]() -> void
            {
                if (creature->IsAlive())
                    creature->Kill(creature);
            });
        }
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId & 0xF)
            {
                case EVENT_BOARD:
                {
                    Player* hunter = GetHunter();
                    if (!hunter || hunter->GetMapId() != me->GetMapId() || ++boardTries > 40)
                    {
                        if (hunter)
                            hunter->SetDisableGravity(false);
                        End(0);
                        break;
                    }
                    if (hunter->GetVehicleBase() == me)
                        break;
                    if (!hunter->IsBeingTeleported())
                    {
                        // a client still loading Felsoul Hold ignored the first order and fell once it was done: hold him
                        // in the air again, and back beside the bat if he drifted out of the ride's reach
                        hunter->SetDisableGravity(true);
                        if (hunter->GetExactDist(me) > 3.0f)
                            hunter->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                        else
                            hunter->CastSpell(me, SPELL_RIDE_VEHICLE_HARDCODED, true);
                    }
                    events.ScheduleEvent(EVENT_BOARD, 400);
                    break;
                }
                case EVENT_ESCORT_DOWN:
                    EscortDown(uint8(eventId >> 4));
                    break;
                case EVENT_AUTO_DIVE:
                    Dive();
                    break;
                case EVENT_DIVE_JUMP:
                {
                    Player* hunter = GetHunter();
                    Creature* dive = ObjectAccessor::GetCreature(*me, diveGuid);
                    if (!hunter || !dive)
                    {
                        End(0);
                        break;
                    }
                    hunter->CastSpell(dive, SPELL_RIDE_VEHICLE_HARDCODED, true);
                    hunter->CastSpell(hunter, SPELL_DIVE_METAMORPHOSIS, true);
                    hunter->SetDisplayId(hunter->getGender() == GENDER_MALE ? DISPLAY_METAMORPHOSIS_MALE : DISPLAY_METAMORPHOSIS_FEMALE);
                    dive->GetMotionMaster()->MovePoint(POINT_DIVE, HuntDiveEnd, false, HuntDiveSpeed);
                    uint32 const fall = uint32(dive->GetExactDist(&HuntDiveEnd) / HuntDiveSpeed * IN_MILLISECONDS);
                    events.ScheduleEvent(EVENT_IMPACT, fall + 100);
                    break;
                }
                case EVENT_IMPACT:
                    if (Player* hunter = GetHunter())
                        Impact(hunter);
                    events.ScheduleEvent(EVENT_GONE, 4000);
                    break;
                case EVENT_CANNON_FIRE:
                    // the Obliterators fire at the hunter's bat all along the flight, and miss
                    if (Creature* cannon = me->FindNearestCreature(NPC_FELSOUL_OBLITERATOR, 250.0f))
                    {
                        Position const miss = NearMiss(cannon, me);
                        cannon->CastSpell(miss.GetPositionX(), miss.GetPositionY(), miss.GetPositionZ(), SPELL_CHAOS_BOMB_MISSILE, true);
                    }
                    events.ScheduleEvent(EVENT_CANNON_FIRE, urand(3000, 5000));
                    break;
                case EVENT_TELEPORT_HUNTER:
                    RestoreWingmen();
                    if (Player* hunter = GetHunter())
                        hunter->CastSpell(hunter, SPELL_FLY_TO_FELSOUL_HOLD, true);
                    break;
                case EVENT_GONE:
                    End(0);
                    break;
                default:
                    break;
            }
        }
    }
};

// The Hunt: the Felsoul doomlords patrol over the hold, and stop to fire at the Illidari bats that come near
struct npc_felsoul_doomlord_the_hunt : public ScriptedAI
{
    npc_felsoul_doomlord_the_hunt(Creature* creature) : ScriptedAI(creature) { }

    static constexpr uint32 SPELL_FEL_BOLT_AT_BATS = 200740;
    static constexpr float AIM_RANGE = 45.0f;

    ObjectGuid targetGuid;
    uint32 lookTimer = 1000;
    uint32 fireTimer = 0;

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
        targetGuid.Clear();
        // they hover with their wings beating, also while they stop to fire
        me->SetAnimTier(AnimTier::Fly);
    }

    void UpdateAI(uint32 diff) override
    {
        if (fireTimer > diff)
            fireTimer -= diff;
        else
            fireTimer = 0;

        if (lookTimer > diff)
        {
            lookTimer -= diff;
            return;
        }
        lookTimer = 500;

        Creature* bat = me->FindNearestCreature(NPC_ILLIDARI_FEL_BAT_HUNT, AIM_RANGE);
        if (!bat)
        {
            if (!targetGuid.IsEmpty())
            {
                targetGuid.Clear();
                me->ResumeMovement();
            }
            return;
        }

        if (targetGuid.IsEmpty())
            me->PauseMovement();
        targetGuid = bat->GetGUID();
        me->SetFacingToObject(bat);
        if (!fireTimer)
        {
            Position const miss = NearMiss(me, bat);
            me->CastSpell(miss.GetPositionX(), miss.GetPositionY(), miss.GetPositionZ(), SPELL_FEL_BOLT_AT_BATS, true);
            fireTimer = 2500;
        }
    }
};

// The Hunt, step 3: the bunnies above Varedis' three wards, chained in the scenario's order (Ward Beam, 189204): the first
// ward (middle) to the second (east), the second to the third (west); each blasts the hunter near its ward, and when its
// ward is destroyed, removes it and leaves
struct npc_felsoul_ward_bunny : public ScriptedAI
{
    npc_felsoul_ward_bunny(Creature* creature) : ScriptedAI(creature) { }

    static constexpr uint32 SPELL_WARD_BEAM = 189204;
    static constexpr uint32 SPELL_WARD_BLAST = 205726;
    static constexpr uint64 SPAWN_WEST = 376290;
    static constexpr uint64 SPAWN_MIDDLE = 376297;
    static constexpr uint64 SPAWN_EAST = 376294;
    static constexpr uint32 NPC_WARD_BUNNY = 600112;
    static constexpr uint32 GO_FEL_BARRIER = 243467;

    uint32 beamTimer = 2000;
    uint32 blastTimer = 10000;
    bool wardChecked = false;

    uint64 BeamTargetSpawn() const
    {
        switch (me->GetDBTableGUIDLow())
        {
            case SPAWN_MIDDLE: return SPAWN_EAST;
            case SPAWN_EAST: return SPAWN_WEST;
            default: return 0;
        }
    }

    Creature* FindBunny(uint64 spawnId) const
    {
        std::list<Creature*> bunnies;
        me->GetCreatureListWithEntryInGrid(bunnies, NPC_WARD_BUNNY, 400.0f);
        for (Creature* bunny : bunnies)
            if (bunny->GetDBTableGUIDLow() == spawnId)
                return bunny;
        return nullptr;
    }

    void SetData(uint32 type, uint32 value) override
    {
        if (type != 1 || value != 1)
            return;

        // the ward plays its destruction and stays destroyed: taken out of the goober cycle, which would set it ready
        // again once its cooldown is over, and no longer selectable
        for (uint32 ward : { 243428u, 243472u, 243473u })
            if (GameObject* go = me->FindNearestGameObject(ward, 40.0f))
            {
                go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
                // after the use that called us has set it activated
                go->AddDelayedEvent(300, [go]() -> void { go->SetLootState(GO_NOT_READY); });
            }
        LowerBarrierIfAllWardsDown();
        me->DespawnOrUnsummon(500);
    }

    // the Fel Barrier in front of Varedis' courtyard falls with the last of the three wards; nothing else removes it, the
    // whole scenario sharing one phase
    void LowerBarrierIfAllWardsDown()
    {
        for (uint32 ward : { 243428u, 243472u, 243473u })
        {
            GameObject* go = me->FindNearestGameObject(ward, 600.0f);
            if (!go || !go->HasFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE))
                return;
        }

        if (GameObject* barrier = me->FindNearestGameObject(GO_FEL_BARRIER, 600.0f))
            barrier->Delete();
    }

    void UpdateAI(uint32 diff) override
    {
        // a resumed scenario brings its ward back destroyed (Scenario::RestoreDoneSpawns): the bunny has nothing left to do
        if (!wardChecked)
        {
            wardChecked = true;
            for (uint32 ward : { 243428u, 243472u, 243473u })
                if (GameObject* go = me->FindNearestGameObject(ward, 40.0f))
                    if (go->HasFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE))
                    {
                        LowerBarrierIfAllWardsDown();
                        me->DespawnOrUnsummon();
                        return;
                    }
        }

        if (blastTimer <= diff)
        {
            // the flight of step 1 and the dive pass within reach: a hunter on his bat is not the wards' to blast
            if (Player* player = me->FindNearestPlayer(60.0f))
                if (!player->GetVehicle())
                    me->CastSpell(player, SPELL_WARD_BLAST, true);
            blastTimer = 10000;
        }
        else
            blastTimer -= diff;

        if (beamTimer > diff)
        {
            beamTimer -= diff;
            return;
        }
        beamTimer = 5000;

        uint64 const targetSpawn = BeamTargetSpawn();
        if (!targetSpawn)
            return;

        Creature* target = FindBunny(targetSpawn);
        if (!target || target->HasAura(SPELL_WARD_BEAM, me->GetGUID()))
            return;

        me->CastSpell(target, SPELL_WARD_BEAM, TRIGGERED_FULL_MASK);
    }
};

// The Hunt, step 3: the Fist of the Deceiver, out of the portal, with its Legion Slash and Fel Crash
struct npc_fist_of_the_deceiver : public ScriptedAI
{
    npc_fist_of_the_deceiver(Creature* creature) : ScriptedAI(creature) { }

    static constexpr uint32 SPELL_SPAWN_FROM_PORTAL = 189447;
    static constexpr uint32 SPELL_LEGION_SLASH = 182146;
    static constexpr uint32 SPELL_FEL_CRASH = 190697;
    // soul weight of the step, as its SmartAI gave it
    static constexpr uint32 CRITERIA_FIST_SOULS = 54742;

    enum { EVENT_LEGION_SLASH = 1, EVENT_FEL_CRASH };

    EventMap events;

    void IsSummonedBy(Unit* /*summoner*/) override
    {
        DoCast(me, SPELL_SPAWN_FROM_PORTAL, true);
    }

    void Reset() override
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_LEGION_SLASH, 6000);
        events.ScheduleEvent(EVENT_FEL_CRASH, 10000);
    }

    void JustDied(Unit* /*killer*/) override
    {
        if (Player* player = me->FindNearestPlayer(100.0f))
            player->UpdateAchievementCriteria(CRITERIA_TYPE_SCRIPT_EVENT_2, CRITERIA_FIST_SOULS, 0, 0, me, true);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_LEGION_SLASH:
                    DoCastVictim(SPELL_LEGION_SLASH);
                    events.ScheduleEvent(EVENT_LEGION_SLASH, urand(14000, 17000));
                    break;
                case EVENT_FEL_CRASH:
                    if (Unit* victim = me->getVictim())
                        me->CastSpell(victim->GetPositionX(), victim->GetPositionY(), victim->GetPositionZ(), SPELL_FEL_CRASH, false);
                    events.ScheduleEvent(EVENT_FEL_CRASH, urand(18000, 22000));
                    break;
                default:
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// The Hunt, step 3: the Felsoul Crusher, an infernal. Its arrival (185302) hides it about two seconds as an invisible
// stalker, then lands it with its impact and spawn animation. The cast must reach a client that already knows the
// creature, or the impact is sent to nobody: the infernal waits half a second wearing an invisible model, so it is
// never seen standing there before it lands
struct npc_felsoul_crusher : public ScriptedAI
{
    npc_felsoul_crusher(Creature* creature) : ScriptedAI(creature) { }

    static constexpr uint32 SPELL_INFERNAL_ARRIVAL = 185302;
    static constexpr uint32 SPELL_FEL_STREAK = 190083;
    static constexpr uint32 CRITERIA_CRUSHER_SOULS = 54742;
    static constexpr uint32 MODEL_INVISIBLE_STALKER = 11686;

    EventMap events;
    uint32 arrivalTimer = 0;

    void IsSummonedBy(Unit* /*summoner*/) override
    {
        me->SetDisplayId(MODEL_INVISIBLE_STALKER);
        me->SetReactState(REACT_PASSIVE);
        arrivalTimer = 500;
    }

    void Reset() override
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(1, urand(5000, 10000));
    }

    void JustDied(Unit* /*killer*/) override
    {
        if (Player* player = me->FindNearestPlayer(100.0f))
            player->UpdateAchievementCriteria(CRITERIA_TYPE_SCRIPT_EVENT_2, CRITERIA_CRUSHER_SOULS, 0, 0, me, true);
    }

    void UpdateAI(uint32 diff) override
    {
        if (arrivalTimer)
        {
            if (arrivalTimer > diff)
            {
                arrivalTimer -= diff;
                return;
            }
            arrivalTimer = 0;
            // the transform of the arrival takes over, and gives back the infernal's own model when it ends
            DoCast(me, SPELL_INFERNAL_ARRIVAL, true);
            me->SetReactState(REACT_AGGRESSIVE);
        }

        if (!UpdateVictim())
            return;

        events.Update(diff);
        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        if (events.ExecuteEvent() == 1)
        {
            DoCastVictim(SPELL_FEL_STREAK);
            events.ScheduleEvent(1, urand(10000, 15000));
        }

        DoMeleeAttackIfReady();
    }
};

// The Hunt: the Fel Winds of Fel Commander Igrius (193909), a whirlwind that roams where it rose and throws back whoever it
// touches. With no AI it chased and struck the hunter like any foe
struct npc_fel_winds_igrius : public ScriptedAI
{
    npc_fel_winds_igrius(Creature* creature) : ScriptedAI(creature) { }

    std::map<ObjectGuid, uint32> thrownAt;
    uint32 touchTimer = 500;
    uint32 lifeTime = 0;

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
    }

    void IsSummonedBy(Unit* /*summoner*/) override
    {
        me->GetMotionMaster()->MoveRandom(10.0f);
    }

    void AttackStart(Unit* /*who*/) override { }
    void MoveInLineOfSight(Unit* /*who*/) override { }

    void UpdateAI(uint32 diff) override
    {
        lifeTime += diff;
        if (touchTimer > diff)
        {
            touchTimer -= diff;
            return;
        }
        touchTimer = 300;

        std::list<Player*> players;
        me->GetPlayerListInGrid(players, 4.0f);
        for (Player* player : players)
        {
            if (!player->IsAlive() || player->isGameMaster())
                continue;

            // once every two seconds for each, the time to be thrown clear
            auto itr = thrownAt.find(player->GetGUID());
            if (itr != thrownAt.end() && lifeTime < itr->second + 2000)
                continue;

            thrownAt[player->GetGUID()] = lifeTime;
            player->KnockbackFrom(me->GetPositionX(), me->GetPositionY(), 12.0f, 6.0f);
        }
    }
};

// The Hunt, step 4: Varedis Felsoul. When the step starts with the hunter near, his two talking heads, the bridge, the
// scene (1054) and only then the fight; each health line once. He cannot be killed: at a tenth of his health he stuns the
// hunter, drops the Twinblades, Kil'jaeden calls him back to Argus and he leaves through a portal
// the end of The Hunt: the spot before the fel pool where Varedis stops to open his portal, the portal a few steps ahead
// of him, and the step into it where he is gone
Position const VaredisWaitPos = { 957.11f, 4801.13f, 34.83f, 4.333f };
Position const VaredisPortalPos = { 955.25f, 4796.48f, 34.6f, 4.333f };
Position const VaredisLeavePos = { 955.65f, 4797.5f, 34.7f, 4.333f };
// where the scene (1054) leaves Varedis: hidden while it plays, he takes over there once it ends
Position const VaredisAfterScenePos = { 968.638f, 4828.03f, 34.7073f, 1.07898f };
// Empowered Chaos Nova: Varedis' stun at the end of The Hunt, held until he is gone
uint32 const SPELL_VAREDIS_FINAL_STUN = 218930;

struct npc_varedis_felsoul_the_hunt : public ScriptedAI
{
    npc_varedis_felsoul_the_hunt(Creature* creature) : ScriptedAI(creature) { }

    static constexpr uint32 STEP_UNLEASHED_HAVOC = 3;
    static constexpr uint32 SPELL_CONV_STAND_AGAINST_ME = 200893;
    static constexpr uint32 SPELL_CONV_POWER_OF_THE_DECEIVER = 200894;
    static constexpr uint32 SPELL_SCENE_VAREDIS = 189475;
    static constexpr uint32 SPELL_FEL_RUSH = 202817;        // a dash straight ahead
    static constexpr uint32 SPELL_FEL_RUSH_AT_TARGET = 202829; // a dash onto the target
    static constexpr uint32 SPELL_DEMONS_BITE = 190226;
    static constexpr uint32 SPELL_CHAOS_NOVA = 190246;
    static constexpr uint32 GO_BRIDGE = 243468;
    static constexpr uint32 GO_TWINBLADES = 251878;
    static constexpr uint32 NPC_TWINBLADES = 109978;
    static constexpr uint32 CRITERIA_VAREDIS_SLAIN = 45717;
    // the Twinblades flung from his hands to where they fall (a missile onto the Twinblades holder)
    static constexpr uint32 SPELL_FLING_TWINBLADES = 218933;
    // the Shadow Portal of Open Shadow Portal (200603) and its look (200604, without its shadow bolts: SpellMgr)
    static constexpr uint32 NPC_SHADOW_PORTAL = 101696;
    static constexpr uint32 SPELL_SHADOW_PORTAL = 200604;
    // his two lines (conversation lines 6068 and 6065), 9.3 s, then Kil'jaeden's on its own (6063), 6.9 s: the client
    // shows no line of a conversation after one that ends its chain, and holds the next talking head until the end the
    // server gives the previous conversation (its actors' durations and five seconds more)
    static constexpr uint32 CONV_NO_NEED_FOR_ARMAMENTS = 600001;
    static constexpr uint32 CONV_RETURN_TO_ARGUS = 600002;

    enum { SAY_AGGRO = 0, SAY_75 = 1, SAY_50 = 2, SAY_25 = 3, SAY_GLADLY_MY_MASTER = 8 };
    enum
    {
        EVENT_INTRO_SECOND = 1, EVENT_INTRO_SCENE, EVENT_DEMONS_BITE, EVENT_CHAOS_NOVA, EVENT_FEL_RUSH,
        EVENT_END_FLING, EVENT_END_BLADES_DOWN, EVENT_END_KILJAEDEN, EVENT_END_GLADLY, EVENT_END_PORTAL, EVENT_END_WALK, EVENT_END_GONE
    };
    enum { POINT_PORTAL = 1, POINT_WAIT };

    EventMap events;
    uint32 watchTimer = 1000;
    bool introStarted = false;
    bool introDone = false;
    bool sceneWatched = false;
    bool ending = false;
    uint8 healthLines = 0;
    ObjectGuid hunterGuid;
    ObjectGuid portalGuid;

    void Reset() override
    {
        events.Reset();
        healthLines = 0;
        if (introDone)
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
    }

    Player* Hunter() const { return me->FindNearestPlayer(90.0f); }

    void EnterCombat(Unit* who) override
    {
        Talk(SAY_AGGRO, who->GetGUID());
        DoCast(who, SPELL_FEL_RUSH_AT_TARGET, true);
        events.ScheduleEvent(EVENT_DEMONS_BITE, 8000);
        events.ScheduleEvent(EVENT_CHAOS_NOVA, 12000);
        events.ScheduleEvent(EVENT_FEL_RUSH, 14000);
    }

    void EnterEvadeMode() override
    {
        if (ending)
            return;
        ScriptedAI::EnterEvadeMode();
    }

    void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType /*dmgType*/) override
    {
        if (ending)
        {
            damage = 0;
            return;
        }

        uint64 const floor = me->CountPctFromMaxHealth(10);
        if (me->GetHealth() <= floor + damage)
        {
            damage = me->GetHealth() > floor ? uint32(me->GetHealth() - floor) : 0;
            StartEnding();
            return;
        }

        uint64 const left = me->GetHealth() > damage ? me->GetHealth() - damage : 0;
        float const pct = float(left) * 100.0f / float(me->GetMaxHealth());
        static float const thresholds[3] = { 75.0f, 50.0f, 25.0f };
        static uint8 const lines[3] = { SAY_75, SAY_50, SAY_25 };
        while (healthLines < 3 && pct <= thresholds[healthLines])
            Talk(lines[healthLines++]);
    }

    Player* EndHunter() const { return ObjectAccessor::GetPlayer(*me, hunterGuid); }

    void EndOfScene()
    {
        sceneWatched = false;
        introDone = true;
        me->NearTeleportTo(VaredisAfterScenePos.GetPositionX(), VaredisAfterScenePos.GetPositionY(), VaredisAfterScenePos.GetPositionZ(), VaredisAfterScenePos.GetOrientation());
        me->SetHomePosition(VaredisAfterScenePos);
        me->SetVisible(true);
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (!ending || type != POINT_MOTION_TYPE)
            return;

        if (id == POINT_WAIT)
        {
            me->SetFacingTo(VaredisWaitPos.GetOrientation());
            events.RescheduleEvent(EVENT_END_PORTAL, 500);
        }
        else if (id == POINT_PORTAL)
            events.RescheduleEvent(EVENT_END_GONE, 200);
    }

    // at a tenth of his health: the hunter is stunned until Varedis is gone, and with his first line the Twinblades leave
    // his hands for where they fall
    void StartEnding()
    {
        ending = true;
        events.Reset();
        me->SetReactState(REACT_PASSIVE);
        me->AttackStop();
        me->StopMoving();
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);

        Player* hunter = me->getVictim() ? me->getVictim()->ToPlayer() : nullptr;
        if (!hunter)
            hunter = Hunter();
        if (hunter)
        {
            hunterGuid = hunter->GetGUID();
            hunter->AttackStop();
            me->SetFacingToObject(hunter);
            // the nova shows round him; its stun, without end, is lifted when he is gone
            me->CastSpell(me, SPELL_VAREDIS_FINAL_STUN, true);
            if (Aura* stun = hunter->GetAura(SPELL_VAREDIS_FINAL_STUN))
            {
                stun->SetMaxDuration(-1);
                stun->SetDuration(-1);
            }
            else
                me->AddAura(SPELL_VAREDIS_FINAL_STUN, hunter, nullptr, 0, -1, -1);
            hunter->CreateConversation(CONV_NO_NEED_FOR_ARMAMENTS);
        }

        events.ScheduleEvent(EVENT_END_FLING, 1000);
        events.ScheduleEvent(EVENT_END_KILJAEDEN, 9400);
        events.ScheduleEvent(EVENT_END_GLADLY, 16600);
    }

    // they fall at their official place, clear of the hunter
    static Position const& TwinbladesPos()
    {
        static Position const blades = { 984.354f, 4826.77f, 34.7617f, 2.80852f };
        return blades;
    }

    void DropTwinblades()
    {
        Position const& blades = TwinbladesPos();
        me->SummonGameObject(GO_TWINBLADES, blades.GetPositionX(), blades.GetPositionY(), blades.GetPositionZ(), blades.GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 3600);
    }

    // they leave his hands: flung onto their holder, they lie there once landed
    void FlingTwinblades()
    {
        me->SetVirtualItem(0, 0);
        me->SetVirtualItem(1, 0);
        me->SetVirtualItem(2, 0);
        if (Creature* holder = me->SummonCreature(NPC_TWINBLADES, TwinbladesPos(), TEMPSUMMON_MANUAL_DESPAWN))
            me->CastSpell(holder, SPELL_FLING_TWINBLADES, true);
        events.ScheduleEvent(EVENT_END_BLADES_DOWN, 700);
    }

    void UpdateEnding()
    {
        switch (events.ExecuteEvent())
        {
            case EVENT_END_FLING:
                FlingTwinblades();
                break;
            case EVENT_END_BLADES_DOWN:
                DropTwinblades();
                break;
            case EVENT_END_KILJAEDEN:
                if (Player* hunter = EndHunter())
                    hunter->CreateConversation(CONV_RETURN_TO_ARGUS);
                break;
            case EVENT_END_GLADLY:
                Talk(SAY_GLADLY_MY_MASTER, hunterGuid);
                // before the fel pool, where he opens the portal (MovementInform), or after this if the way is blocked
                me->SetWalk(false);
                me->GetMotionMaster()->MovePoint(POINT_WAIT, VaredisWaitPos);
                events.ScheduleEvent(EVENT_END_PORTAL, 10000);
                break;
            case EVENT_END_PORTAL:
                me->StopMoving();
                me->SetFacingTo(VaredisWaitPos.GetOrientation());
                // he channels while it opens
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_SPELL_CHANNEL_DIRECTED);
                if (Creature* portal = me->SummonCreature(NPC_SHADOW_PORTAL, VaredisPortalPos, TEMPSUMMON_TIMED_DESPAWN, 30000))
                {
                    portalGuid = portal->GetGUID();
                    portal->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                    portal->SetReactState(REACT_PASSIVE);
                    portal->CastSpell(portal, SPELL_SHADOW_PORTAL, true);
                }
                // he waits for it to be open before he goes
                events.ScheduleEvent(EVENT_END_WALK, 3500);
                break;
            case EVENT_END_WALK:
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_ONESHOT_NONE);
                me->SetWalk(true);
                me->GetMotionMaster()->MovePoint(POINT_PORTAL, VaredisLeavePos);
                // he is gone on stepping in (MovementInform), or after this if the way is blocked
                events.ScheduleEvent(EVENT_END_GONE, 8000);
                break;
            case EVENT_END_GONE:
                if (Player* hunter = EndHunter())
                {
                    hunter->RemoveAurasDueToSpell(SPELL_VAREDIS_FINAL_STUN);
                    hunter->UpdateAchievementCriteria(CRITERIA_TYPE_SCRIPT_EVENT_2, CRITERIA_VAREDIS_SLAIN, 0, 0, me, true);
                }
                // the portal closes as soon as he is through, and is gone once its closing has played
                if (Creature* portal = ObjectAccessor::GetCreature(*me, portalGuid))
                {
                    portal->RemoveAurasDueToSpell(SPELL_SHADOW_PORTAL);
                    portal->DespawnOrUnsummon(1000);
                }
                me->DespawnOrUnsummon(100);
                break;
            default:
                break;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!introStarted)
        {
            if (watchTimer > diff)
            {
                watchTimer -= diff;
                return;
            }
            watchTimer = 1000;

            uint32 step = 0;
            sScenarioMgr->WithScenario(me->GetInstanceId(), [&step](Scenario* scenario) { step = scenario->GetCurrentStep(); });
            // a scenario resumed after his escape: he is gone and the Twinblades wait where he dropped them
            if (step > STEP_UNLEASHED_HAVOC)
            {
                introStarted = true;
                if (!me->FindNearestGameObject(GO_TWINBLADES, 200.0f))
                {
                    me->SummonCreature(NPC_TWINBLADES, TwinbladesPos(), TEMPSUMMON_MANUAL_DESPAWN);
                    DropTwinblades();
                }
                me->DespawnOrUnsummon();
                return;
            }

            Player* hunter = Hunter();
            if (step != STEP_UNLEASHED_HAVOC || !hunter)
                return;

            introStarted = true;
            hunter->CastSpell(hunter, SPELL_CONV_STAND_AGAINST_ME, true);
            // the second talking head as soon as the first one has closed
            events.ScheduleEvent(EVENT_INTRO_SECOND, 8900);
            return;
        }

        events.Update(diff);

        if (ending)
        {
            UpdateEnding();
            return;
        }

        // the scene ends when its aura leaves the hunter (completed, cancelled or the hunter gone)
        if (sceneWatched)
        {
            Player* hunter = EndHunter();
            if (!hunter || !hunter->HasAura(SPELL_SCENE_VAREDIS))
                EndOfScene();
            return;
        }

        if (!introDone)
        {
            switch (events.ExecuteEvent())
            {
                case EVENT_INTRO_SECOND:
                    if (Player* hunter = Hunter())
                        hunter->CastSpell(hunter, SPELL_CONV_POWER_OF_THE_DECEIVER, true);
                    // the scene waits four seconds after the second voice (7.4 s at most)
                    events.ScheduleEvent(EVENT_INTRO_SCENE, 11400);
                    break;
                case EVENT_INTRO_SCENE:
                    if (GameObject* bridge = me->FindNearestGameObject(GO_BRIDGE, 50.0f))
                        bridge->UseDoorOrButton();
                    if (Player* hunter = Hunter())
                    {
                        // the scene has its own Varedis: this one is not seen until it ends
                        hunterGuid = hunter->GetGUID();
                        me->AddAura(SPELL_SCENE_VAREDIS, hunter);
                        me->SetVisible(false);
                        sceneWatched = true;
                    }
                    else
                        EndOfScene();
                    break;
                default:
                    break;
            }
            return;
        }

        if (!UpdateVictim())
            return;

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_DEMONS_BITE:
                    // half again his weapon damage (about 10 000 for blows of 6 000 to 7 000)
                    if (Unit* victim = me->getVictim())
                    {
                        float const weaponPct = 150.0f;
                        me->CastCustomSpell(victim, SPELL_DEMONS_BITE, &weaponPct, nullptr, nullptr, false);
                    }
                    events.ScheduleEvent(EVENT_DEMONS_BITE, urand(5000, 11000));
                    break;
                case EVENT_FEL_RUSH:
                    if (Unit* victim = me->getVictim())
                    {
                        if (me->GetDistance(victim) > 6.0f)
                            DoCast(victim, SPELL_FEL_RUSH_AT_TARGET);
                        else
                            DoCast(me, SPELL_FEL_RUSH);
                    }
                    events.ScheduleEvent(EVENT_FEL_RUSH, urand(12000, 16000));
                    break;
                case EVENT_CHAOS_NOVA:
                    DoCast(me, SPELL_CHAOS_NOVA);
                    events.ScheduleEvent(EVENT_CHAOS_NOVA, urand(15000, 20000));
                    break;
                default:
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// The quest copies of the demon hunter's artifact (the Violet Hold of By Any Means, map 1494, and Felsoul Hold of The
// Hunt, map 1498) are his own; leaving one unbinds him, so that his next visit starts it again instead of finding it played
// or left halfway. Arriving in Felsoul Hold at the first step, he finds the bat that carries him over the hold
// The Hunt, the way back: once the Twinblades are taken, Kayn's talking head, then a fel bat comes down by the bridge
// (summoned high above it by the Twinblades); clicked, it flies off toward Dalaran, the hunter is sent there and another
// bat brings him down on Krasus' Landing
Position const HuntReturnLanding = { 971.377f, 4841.86f, 35.038f, 0.0f };
Position const HuntReturnTakeOff[] =
{
    { 955.0f, 4838.0f, 50.0f },
    { 925.0f, 4830.0f, 70.0f },
    { 885.0f, 4818.0f, 92.0f },
    { 840.0f, 4805.0f, 115.0f },
};
// HuntDalaranPath flown backwards, down to where the bat of Kayn waited
Position const HuntDalaranArrival = { -738.0f, 4148.0f, 798.0f, 3.12f };
Position const HuntDalaranLanding = { -832.811f, 4262.63f, 746.252f, 1.53999f };

std::mutex HuntReturningLock;
std::set<ObjectGuid> HuntReturningHunters;

enum HuntReturn
{
    NPC_HUNT_RETURN_BAT             = 101902,
    NPC_HUNT_RETURN_MOUNT           = 101904,
    SPELL_CONV_WELL_DONE            = 201034,
    SPELL_RIDE_RETURN_MOUNT         = 46598,
};

void FlySpline(Creature* bat, std::vector<Position> const& points, float speed)
{
    Movement::MoveSplineInit init(*bat);
    init.Path().push_back(G3D::Vector3(bat->GetPositionX(), bat->GetPositionY(), bat->GetPositionZ()));
    // a point where the bat already is (the way back starts where the hunter arrived) is a leg of no length: the whole
    // flight would be refused and last no time at all
    for (Position const& point : points)
    {
        G3D::Vector3 const next(point.GetPositionX(), point.GetPositionY(), point.GetPositionZ());
        if ((next - init.Path().back()).length() > 1.0f)
            init.Path().push_back(next);
    }
    if (init.Path().size() < 2)
        return;
    init.SetSmooth();
    init.SetFly();
    init.SetUncompressed();
    init.SetVelocity(speed);
    init.Launch();
}

// the bat by the bridge: hidden while Kayn speaks and the scene of the Twinblades plays, then it comes down and waits to
// be clicked
struct npc_illidari_fel_bat_hunt_return : public ScriptedAI
{
    npc_illidari_fel_bat_hunt_return(Creature* creature) : ScriptedAI(creature) { }

    enum { EVENT_COME_DOWN = 1, EVENT_LANDED };

    static constexpr uint32 SPELL_SCENE_TWINBLADES = 189916;

    EventMap events;
    ObjectGuid hunterGuid;
    uint8 sceneWaits = 0;
    bool started = false;
    bool taken = false;

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
        me->SetCanFly(true);
        me->SetDisableGravity(true);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!started)
        {
            started = true;
            me->SetVisible(false);
            if (Player* hunter = me->FindNearestPlayer(150.0f))
            {
                hunterGuid = hunter->GetGUID();
                hunter->CastSpell(hunter, SPELL_CONV_WELL_DONE, true);
            }
            events.ScheduleEvent(EVENT_COME_DOWN, 4500);
        }

        events.Update(diff);
        switch (events.ExecuteEvent())
        {
            case EVENT_COME_DOWN:
                // the scene plays while its aura is on the hunter: not before it is over (a minute at most)
                if (Player* hunter = ObjectAccessor::GetPlayer(*me, hunterGuid))
                    if (hunter->HasAura(SPELL_SCENE_TWINBLADES) && ++sceneWaits < 120)
                    {
                        events.ScheduleEvent(EVENT_COME_DOWN, 500);
                        break;
                    }
                me->SetVisible(true);
                FlySpline(me, { HuntReturnLanding }, 14.0f);
                events.ScheduleEvent(EVENT_LANDED, uint32(me->movespline->Duration()));
                break;
            case EVENT_LANDED:
                me->SetDisableGravity(false);
                me->SetCanFly(false);
                me->SetFacingTo(me->GetAngle(HuntReturnTakeOff[0].GetPositionX(), HuntReturnTakeOff[0].GetPositionY()));
                break;
            default:
                break;
        }
    }

    // its own spell (201016) has no place to summon its mount on this server: the mount is set here instead
    void OnSpellClick(Unit* clicker) override
    {
        Player* hunter = clicker->ToPlayer();
        if (taken || !hunter || hunter->GetVehicle())
            return;

        taken = true;
        std::list<Creature*> strays;
        me->GetCreatureListWithEntryInGrid(strays, NPC_HUNT_RETURN_MOUNT, 100.0f);
        for (Creature* stray : strays)
            stray->DespawnOrUnsummon();

        if (TempSummon* mount = me->SummonCreature(NPC_HUNT_RETURN_MOUNT, me->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN))
        {
            ObjectGuid const mountGuid = mount->GetGUID();
            hunter->AddDelayedEvent(100, [hunter, mountGuid]() -> void
            {
                if (Creature* mount = ObjectAccessor::GetCreature(*hunter, mountGuid))
                    hunter->CastSpell(mount, SPELL_RIDE_RETURN_MOUNT, true);
            });
        }
        me->DespawnOrUnsummon(100);
    }
};

// the mount of the way back: away from Felsoul Hold toward Dalaran, then down on Krasus' Landing
struct npc_illidari_fel_bat_hunt_mount : public ScriptedAI
{
    npc_illidari_fel_bat_hunt_mount(Creature* creature) : ScriptedAI(creature) { }

    enum { EVENT_TO_DALARAN = 1, EVENT_LAND };

    EventMap events;
    ObjectGuid hunterGuid;
    // a mount nobody got on (its hunter gone, or replaced by another) does not stay hanging in the sky
    uint32 unboardedTimer = 60000;

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
        me->SetCanFly(true);
        me->SetDisableGravity(true);
        // in flight from the moment it is there, wings beating, not standing as on the ground
        me->SetAnimTier(AnimTier::Fly);
    }

    void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) override
    {
        if (!passenger->IsPlayer())
            return;

        if (!apply)
        {
            // taking the bat from his client also moved his view onto it: both given back to him, or once the bat is gone
            // he can neither move nor see what is around him. Again a moment later, after the vehicle's own release
            Player* hunter = passenger->ToPlayer();
            hunter->SetClientControl(hunter, true);
            ObjectGuid const guid = hunter->GetGUID();
            hunter->AddDelayedEvent(200, [hunter, guid]() -> void
            {
                if (hunter->GetGUID() == guid && hunter->IsInWorld() && !hunter->GetVehicle())
                    hunter->SetClientControl(hunter, true);
            });
            return;
        }

        if (hunterGuid)
            return;

        hunterGuid = passenger->GetGUID();
        // its seat hands the bat over to the rider's client, which held it still in the air and ignored the flight until
        // it gave it back: the demon hunter rides, he does not steer
        passenger->ToPlayer()->SetClientControl(me, false);
        FlyOff();
    }

    void FlyOff()
    {
        if (me->GetMapId() == MAP_FELSOUL_HOLD_SCENARIO)
        {
            FlySpline(me, std::vector<Position>(std::begin(HuntReturnTakeOff), std::end(HuntReturnTakeOff)), HuntFlightSpeed);
            events.ScheduleEvent(EVENT_TO_DALARAN, uint32(me->movespline->Duration()));
        }
        else
        {
            std::vector<Position> path(std::rbegin(HuntDalaranPath), std::rend(HuntDalaranPath));
            path.push_back(HuntDalaranLanding);
            FlySpline(me, path, HuntFlightSpeed);
            events.ScheduleEvent(EVENT_LAND, uint32(me->movespline->Duration()));
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!hunterGuid)
        {
            if (unboardedTimer <= diff)
            {
                me->DespawnOrUnsummon();
                return;
            }
            unboardedTimer -= diff;
        }

        events.Update(diff);
        switch (events.ExecuteEvent())
        {
            case EVENT_TO_DALARAN:
                if (Player* hunter = ObjectAccessor::GetPlayer(*me, hunterGuid))
                {
                    {
                        std::lock_guard<std::mutex> lock(HuntReturningLock);
                        HuntReturningHunters.insert(hunter->GetGUID());
                    }
                    hunter->ExitVehicle();
                    hunter->TeleportTo(MAP_DALARAN_BROKEN_ISLES, HuntDalaranArrival.GetPositionX(), HuntDalaranArrival.GetPositionY(), HuntDalaranArrival.GetPositionZ(), HuntDalaranArrival.GetOrientation());
                }
                me->DespawnOrUnsummon(500);
                break;
            case EVENT_LAND:
                if (Player* hunter = ObjectAccessor::GetPlayer(*me, hunterGuid))
                {
                    hunter->ExitVehicle(&HuntDalaranLanding);
                    // set down on Krasus' Landing from the sky over the sea: his zone, and with it the map of the landing's
                    // people, taken from where he stands now
                    hunter->AddDelayedEvent(500, [hunter]() -> void
                    {
                        if (!hunter->IsInWorld() || hunter->GetMapId() != MAP_DALARAN_BROKEN_ISLES)
                            return;
                        uint32 zoneId, areaId;
                        hunter->GetZoneAndAreaId(zoneId, areaId);
                        hunter->UpdateZone(zoneId, areaId);
                    });
                }
                me->DespawnOrUnsummon(1000);
                break;
            default:
                break;
        }
    }
};

// The Twinblades Varedis dropped: taken at once, no loot window. The hunter gets them, the scene of 189916 plays, the
// scenario ends, Kayn and the bat of the way back come, and the blades on the ground are gone
struct go_twinblades_of_the_deceiver_the_hunt : public GameObjectAI
{
    go_twinblades_of_the_deceiver_the_hunt(GameObject* go) : GameObjectAI(go) { }

    static constexpr uint32 ITEM_TWINBLADES = 127829;
    static constexpr uint32 SPELL_WIELD_TWINBLADES = 189916;
    static constexpr uint32 SPELL_EQUIP_TWINBLADES = 201010;
    static constexpr uint32 CRITERIA_TWINBLADES_TAKEN = 52805;
    static constexpr uint32 NPC_TWINBLADES = 109978;
    static constexpr uint32 NPC_KAYN_RETURN = 101903;

    bool taken = false;

    // the object has a lock: using it opens the lock, which calls GossipUse, not GossipHello
    bool GossipUse(Player* player) override
    {
        return GossipHello(player, true);
    }

    bool GossipHello(Player* player, bool /*isUse*/) override
    {
        if (taken)
            return true;

        taken = true;
        if (!player->HasItemCount(ITEM_TWINBLADES, 1, true))
            player->AddItem(ITEM_TWINBLADES, 1);
        player->CastSpell(player, SPELL_WIELD_TWINBLADES, true);
        // 189916 equips them too, through its periodic 201012: equipped at once all the same
        player->CastSpell(player, SPELL_EQUIP_TWINBLADES, true);
        player->UpdateAchievementCriteria(CRITERIA_TYPE_SCRIPT_EVENT_2, CRITERIA_TWINBLADES_TAKEN, 1);

        // the bat starts high above the bridge and comes down once Kayn has spoken (npc_illidari_fel_bat_hunt_return)
        go->SummonCreature(NPC_HUNT_RETURN_BAT, Position(930.0f, 4836.0f, 72.0f, 4.24381f), TEMPSUMMON_MANUAL_DESPAWN);
        go->SummonCreature(NPC_KAYN_RETURN, Position(981.161f, 4841.15f, 35.038f, 4.25779f), TEMPSUMMON_MANUAL_DESPAWN);

        std::list<Creature*> holders;
        go->GetCreatureListWithEntryInGrid(holders, NPC_TWINBLADES, 30.0f);
        for (Creature* holder : holders)
            holder->DespawnOrUnsummon();

        go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
        go->AddDelayedEvent(500, [go = go]() -> void { go->SetRespawnTime(0); go->Delete(); });
        return true;
    }
};

class player_violet_hold_quest_copy : public PlayerScript
{
public:
    player_violet_hold_quest_copy() : PlayerScript("player_violet_hold_quest_copy") { }

    static constexpr uint32 MAP_VIOLET_HOLD_QUEST_COPY = 1494;
    static constexpr uint32 NPC_KAYN_SUNFURY = 94902;

    // The Hunt taken in front of the Violet Hold: the scene phase ends at once and Kayn with it, so a copy only the hunter
    // sees, made in his new phase, takes Kayn's place and runs off down the street, away from the hold
    void OnQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() != QUEST_THE_HUNT || player->GetMapId() != MAP_DALARAN_BROKEN_ISLES)
            return;

        Creature* kayn = player->FindNearestCreature(NPC_KAYN_SUNFURY, 40.0f);
        if (!kayn)
            return;

        Position const from = kayn->GetPosition();
        player->AddDelayedEvent(400, [player, from]() -> void
        {
            // straight down the free middle of the street (2.4 yards wide clear over 60 yards, measured on the navmesh)
            Position const street = { -932.7f, 4358.0f, 740.2f };
            if (TempSummon* runner = player->SummonCreature(NPC_KAYN_SUNFURY, from, TEMPSUMMON_TIMED_DESPAWN, 4500, 0, player->GetGUID()))
            {
                runner->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER);
                runner->SetReactState(REACT_PASSIVE);
                runner->SetWalk(false);
                runner->SetFacingTo(runner->GetAngle(&street));
                runner->GetMotionMaster()->MovePoint(1, street, false);
            }
        });
    }

    void OnMapChanged(Player* player) override
    {
        if (player->getClass() != CLASS_DEMON_HUNTER)
            return;

        // Varedis' stun lasts until he is gone: never carried out of his scenario nor kept over a reconnection
        player->RemoveAurasDueToSpell(SPELL_VAREDIS_FINAL_STUN);

        if (player->GetMapId() == MAP_FELSOUL_HOLD_SCENARIO)
        {
            SummonHuntBat(player);
            return;
        }

        // the arrival is handled outside the map's thread: the mount is summoned at the hunter's first update, in it
        if (player->GetMapId() == MAP_DALARAN_BROKEN_ISLES)
            player->AddDelayedEvent(0, [player]() -> void { BringHunterBack(player); });

        for (uint32 mapId : { MAP_VIOLET_HOLD_QUEST_COPY, uint32(MAP_FELSOUL_HOLD_SCENARIO) })
        {
            if (player->GetMapId() == mapId)
                continue;

            for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
            {
                Player::BoundInstancesMap const binds = player->GetBoundInstancesCopy(Difficulty(i));
                if (binds.find(mapId) != binds.end())
                    player->UnbindInstance(mapId, Difficulty(i));
            }
        }
    }

    // the hunter is seated on his bat the moment his loading screen goes, so he never sees the ground: the bat of Felsoul
    // Hold on the way in, the return mount over Dalaran on the way back (both summoned as he reached the map). Not before:
    // a client still loading drops him off the vehicle again as it finishes
    void OnLoadingScreenEnd(Player* player) override
    {
        if (player->getClass() != CLASS_DEMON_HUNTER)
            return;

        // this packet is handled by the world thread while the map runs in its own: the boarding, and with it the escorts
        // and the flight, is left to the hunter's next update, in the map's thread
        player->AddDelayedEvent(0, [player]() -> void { BoardOwnMount(player); });
    }

    static void BoardOwnMount(Player* player)
    {
        if (!player->IsInWorld() || player->GetVehicle())
            return;

        uint32 const entry = player->GetMapId() == MAP_FELSOUL_HOLD_SCENARIO ? uint32(NPC_ILLIDARI_FEL_BAT_HUNT)
            : player->GetMapId() == MAP_DALARAN_BROKEN_ISLES ? uint32(NPC_HUNT_RETURN_MOUNT) : 0;
        if (!entry)
            return;

        std::list<Creature*> mounts;
        player->GetCreatureListWithEntryInGrid(mounts, entry, 100.0f);
        for (Creature* mount : mounts)
            if (TempSummon* summon = mount->ToTempSummon())
                if (summon->GetSummonerGUID() == player->GetGUID())
                {
                    player->SetDisableGravity(true);
                    player->CastSpell(mount, SPELL_RIDE_VEHICLE_HARDCODED, true);
                    if (entry == NPC_HUNT_RETURN_MOUNT)
                        BoardReturnMount(player, mount->GetGUID(), 0, 400);
                    return;
                }
    }

    // back from Felsoul Hold: a bat takes the hunter where he arrived, high over Dalaran, and brings him down on Krasus' Landing
    static void BringHunterBack(Player* player)
    {
        if (!player->IsInWorld() || player->GetMapId() != MAP_DALARAN_BROKEN_ISLES)
            return;

        {
            std::lock_guard<std::mutex> lock(HuntReturningLock);
            bool const returning = HuntReturningHunters.erase(player->GetGUID()) != 0;
            if (!returning)
                return;
        }

        TempSummon* summoned = player->SummonCreature(NPC_HUNT_RETURN_MOUNT, HuntDalaranArrival, TEMPSUMMON_MANUAL_DESPAWN);
        if (TempSummon* mount = summoned)
        {
            // he hangs in the air where he arrived until he is on its back, at the end of his loading screen; these tries
            // only if the client never says it is done
            player->SetDisableGravity(true);
            BoardReturnMount(player, mount->GetGUID(), 0, 8000);
        }
    }

    static void BoardReturnMount(Player* player, ObjectGuid mountGuid, uint8 tries, uint32 delay = 400)
    {
        player->AddDelayedEvent(delay, [player, mountGuid, tries]() -> void
        {
            Creature* mount = ObjectAccessor::GetCreature(*player, mountGuid);
            // left behind in another zone map of Dalaran (the hunter moved to his zone after it was summoned): a new one
            if (!mount && !player->GetVehicle() && !player->IsBeingTeleported() && tries < 40
                && player->GetMapId() == MAP_DALARAN_BROKEN_ISLES && player->GetExactDist(&HuntDalaranArrival) < 30.0f)
                if (TempSummon* summon = player->SummonCreature(NPC_HUNT_RETURN_MOUNT, HuntDalaranArrival, TEMPSUMMON_MANUAL_DESPAWN))
                {
                    BoardReturnMount(player, summon->GetGUID(), tries + 1);
                    return;
                }

            if (mount && player->GetVehicleBase() != mount && tries < 40)
            {
                if (!player->IsBeingTeleported())
                {
                    // held in the air again, and back beside the bat if he drifted out of the ride's reach
                    player->SetDisableGravity(true);
                    if (player->GetExactDist(mount) > 3.0f)
                        player->NearTeleportTo(mount->GetPositionX(), mount->GetPositionY(), mount->GetPositionZ(), mount->GetOrientation());
                    else
                        player->CastSpell(mount, SPELL_RIDE_RETURN_MOUNT, true);
                }
                BoardReturnMount(player, mountGuid, tries + 1);
                return;
            }

            player->SetDisableGravity(false);
        });
    }

    static void SummonHuntBat(Player* player)
    {
        if (player->GetQuestStatus(QUEST_THE_HUNT) != QUEST_STATUS_INCOMPLETE)
            return;

        uint32 step = 0;
        sScenarioMgr->WithScenario(player->GetInstanceId(), [&step](Scenario* scenario) { step = scenario->GetCurrentStep(); });
        if (step)
            return;

        // he arrives high over the hold: held there until the bat has him, the bat there before his loading screen is gone
        player->SetDisableGravity(true);
        player->AddDelayedEvent(100, [player]() -> void
        {
            if (player->GetMapId() != MAP_FELSOUL_HOLD_SCENARIO || player->GetVehicle()
                || player->FindNearestCreature(NPC_ILLIDARI_FEL_BAT_HUNT, 100.0f))
            {
                // a bat still boarding him holds him up itself at its next try
                if (!player->GetVehicle())
                    player->SetDisableGravity(false);
                return;
            }

            player->SummonCreature(NPC_ILLIDARI_FEL_BAT_HUNT, HuntArrival, TEMPSUMMON_MANUAL_DESPAWN);
        });
    }
};

// Kor'vas of Krasus' Landing, there before The Hunt. Once the hunter has it, and after he turned it in, the Kor'vas he
// deals with (102799, shown by a phase of his own from then on) stands a few steps away: this one is no longer his to see
struct npc_korvas_bloodthorn_krasus_landing : public ScriptedAI
{
    npc_korvas_bloodthorn_krasus_landing(Creature* creature) : ScriptedAI(creature) { }

    static constexpr uint32 QUEST_THE_HUNT_VENGEANCE = 41863;

    bool IsHiddenFor(Player const* player) const override
    {
        for (uint32 questId : { uint32(QUEST_THE_HUNT), QUEST_THE_HUNT_VENGEANCE })
        {
            QuestStatus const status = player->GetQuestStatus(questId);
            if (status == QUEST_STATUS_INCOMPLETE || status == QUEST_STATUS_COMPLETE || status == QUEST_STATUS_FAILED
                || player->GetQuestRewardStatus(questId))
                return true;
        }
        return false;
    }
};

void AddSC_DH_havoc_art_scenario()
{
    RegisterCreatureAI(npc_korvas_bloodthorn_krasus_landing);
    new player_violet_hold_quest_copy();
    RegisterCreatureAI(npc_taldath_by_any_means);
    RegisterCreatureAI(npc_illidari_fel_bat_the_hunt);
    RegisterCreatureAI(npc_felsoul_doomlord_the_hunt);
    RegisterCreatureAI(npc_felsoul_ward_bunny);
    RegisterCreatureAI(npc_fist_of_the_deceiver);
    RegisterCreatureAI(npc_felsoul_crusher);
    RegisterCreatureAI(npc_varedis_felsoul_the_hunt);
    RegisterCreatureAI(npc_illidari_fel_bat_hunt_return);
    RegisterCreatureAI(npc_illidari_fel_bat_hunt_mount);
    RegisterGameObjectAI(go_twinblades_of_the_deceiver_the_hunt);
    RegisterCreatureAI(npc_fel_winds_igrius);
}
