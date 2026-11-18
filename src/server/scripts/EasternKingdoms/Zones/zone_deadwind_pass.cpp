////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "ScriptMgr.h"
#include "CombatAI.h"
#include "Map.h"
#include "MotionMaster.h"
#include "ObjectMgr.h"

enum DeadwindWidow
{
    NPC_DEADWIND_WIDOW_STALKER  = 51984,

    POINT_WIDOW_DROP            = 1,
    POINT_WIDOW_HANG            = 2,
};

float const WidowThreadReach = 11.0f;   // how far the default AI looks for the stalker to channel the thread to

// A widow hangs only under a stalker, which carries the other end of its thread; the stalkers are looked up in the
// spawn data because the grid may load them after the spider.
static bool HasThreadAnchor(uint16 mapId, Position const& pos)
{
    struct Anchor { uint16 MapId; float X, Y, Z; };
    static std::vector<Anchor> const anchors = []
    {
        std::vector<Anchor> list;
        for (auto const& spawn : *sObjectMgr->GetCreatures())
            if (spawn.second.id == NPC_DEADWIND_WIDOW_STALKER)
                list.push_back({ spawn.second.mapid, spawn.second.posX, spawn.second.posY, spawn.second.posZ });
        return list;
    }();

    for (Anchor const& anchor : anchors)
        if (anchor.MapId == mapId && pos.GetExactDistSq(anchor.X, anchor.Y, anchor.Z) <= WidowThreadReach * WidowThreadReach)
            return true;

    return false;
}

// Most Deadwind Widows are placed under branches and rock arches, 10 to 40 m above the ground, under an invisible
// stalker. They hang there, drop on whoever they fight and climb back once the fight is over. The default AI is kept:
// out of combat it channels the thread (Deadwind Web, creature_action) to the stalker, in combat it casts Weak Poison.
struct npc_deadwind_widow : AggressorAI
{
    explicit npc_deadwind_widow(Creature* creature) : AggressorAI(creature) { }

    void InitializeAI() override
    {
        // The floor search starts 2 m above the given height: from 2.5 m below, a branch just over the spider is not
        // taken for its floor.
        Position const& home = me->GetHomePosition();
        float const floor = me->GetMap()->GetHeight(me->GetPhases(), home.GetPositionX(), home.GetPositionY(), home.GetPositionZ() - 2.5f, true, MAX_FALL_DISTANCE);
        _hangs = floor > INVALID_HEIGHT && home.GetPositionZ() - floor > 1.0f && HasThreadAnchor(me->GetMapId(), home);

        if (_hangs)
            me->SetFlightMovement(CreatureFlightMovementType::DisableGravity);

        AggressorAI::InitializeAI();
    }

    void EnterCombat(Unit* who) override
    {
        AggressorAI::EnterCombat(who);

        if (!_hangs)
        {
            me->InterruptSpell(CURRENT_CHANNELED_SPELL);
            return;
        }

        // Down along the thread, let go of it on the ground
        me->SetFlightMovement(CreatureFlightMovementType::None);
        me->GetMotionMaster()->MoveFall(POINT_WIDOW_DROP);
    }

    void EnterEvadeMode() override
    {
        if (!_hangs)
        {
            AggressorAI::EnterEvadeMode();
            return;
        }

        if (!_EnterEvadeMode())
            return;

        // Straight up the thread: the home point is in the air, out of reach of the ground paths
        me->AddUnitState(UNIT_STATE_EVADE);
        me->SetFlightMovement(CreatureFlightMovementType::DisableGravity);
        me->SetDisableGravity(true);
        me->GetMotionMaster()->Clear(false);
        me->GetMotionMaster()->MovePoint(POINT_WIDOW_HANG, me->GetHomePosition(), false);
        Reset();
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type == EFFECT_MOTION_TYPE && id == POINT_WIDOW_DROP)
        {
            me->InterruptSpell(CURRENT_CHANNELED_SPELL);
            return;
        }

        if (type != POINT_MOTION_TYPE || id != POINT_WIDOW_HANG)
            return;

        me->ClearUnitState(UNIT_STATE_EVADE);
        me->SetFacingTo(me->GetHomePosition().GetOrientation());
        me->GetMotionMaster()->Initialize();
    }

    void JustDied(Unit* killer) override
    {
        AggressorAI::JustDied(killer);

        // The corpse lies where it fell; the respawn has to hang again
        if (_hangs)
            me->SetFlightMovement(CreatureFlightMovementType::DisableGravity);
    }

private:
    bool _hangs = false;
};

// The other Deadwind Widows raid the ogre camps: each one is placed in front of an ogre and they fight for show. Their
// faction is made hostile to monsters in the db (it was friendly with the ogres, so every attack dropped at once). Their
// blows between NPCs are softened so that a fight lasts a few minutes before one of them dies.
struct npc_deadwind_widow_raider : AggressorAI
{
    explicit npc_deadwind_widow_raider(Creature* creature) : AggressorAI(creature) { }

    static void SoftenNpcBlow(Unit const* other, uint32& damage)
    {
        if (other && other->IsCreature() && !other->IsControlledByPlayer())
            damage = std::max<uint32>(1, damage / 5);
    }

    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*damageType*/) override
    {
        SoftenNpcBlow(attacker, damage);
    }

    void DamageDealt(Unit* victim, uint32& damage, DamageEffectType /*damageType*/) override
    {
        SoftenNpcBlow(victim, damage);
    }
};

void AddSC_deadwind_pass()
{
    RegisterCreatureAI(npc_deadwind_widow);
    RegisterCreatureAI(npc_deadwind_widow_raider);
}
