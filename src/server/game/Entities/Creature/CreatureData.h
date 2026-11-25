////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
///      Based on TrinityCore code       ///
///   Copyright 2008-2012 TrinityCore    ///
///   <http://www.trinitycore.org/>      ///
///                                      ///
////////////////////////////////////////////

#ifndef CREATURE_DATA_H
#define CREATURE_DATA_H

#include "Define.h"

enum class CreatureGroundMovementType : uint8
{
    None,
    Run,
    Hover,

    Max
};

enum class CreatureFlightMovementType : uint8
{
    None,
    DisableGravity,
    CanFly,

    Max
};

enum class CreatureRandomMovementType : uint8
{
    Walk,
    CanRun,
    AlwaysRun,

    Max
};

struct TC_GAME_API CreatureMovementData
{
    CreatureMovementData();
    CreatureGroundMovementType Ground;
    CreatureFlightMovementType Flight;
    bool Swim;
    bool Rooted;
    CreatureRandomMovementType Random;
    uint32 InteractionPauseTimer;

    bool IsGroundAllowed() const { return Ground != CreatureGroundMovementType::None; }
    bool IsSwimAllowed() const { return Swim; }
    bool IsFlightAllowed() const { return Flight != CreatureFlightMovementType::None; }
    bool IsRooted() const { return Rooted; }
    bool IsGravityDisabled() const { return Flight == CreatureFlightMovementType::DisableGravity; }
    bool CanFly() const { return Flight == CreatureFlightMovementType::CanFly; }
    bool IsHoverEnabled() const { return Ground == CreatureGroundMovementType::Hover; }

    CreatureRandomMovementType GetRandom() const { return Random; }

    // Without a value of its own, an NPC stands still for a minute after a player talks to it
    uint32 GetInteractionPauseTimer() const { return InteractionPauseTimer ? InteractionPauseTimer : 60000; }

    std::string ToString() const;
};

const uint32 MAX_CREATURE_SPELLS = 8;

#endif
