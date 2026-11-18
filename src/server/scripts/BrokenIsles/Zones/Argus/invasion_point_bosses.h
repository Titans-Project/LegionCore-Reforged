////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

// Greater Invasion Points of Argus: the texts, spells and events their six bosses share

#ifndef INVASION_POINT_BOSSES_H
#define INVASION_POINT_BOSSES_H

enum eSays
{
    SAY_AGGRO   = 0,
    SAY_KILL    = 1
};

enum eSpells
{
    SPELL_BEGUILING_CHARM       = 247549,
    SPELL_BEGUILING_CHARM_AURA  = 247551,
    SPELL_HEART_BREAKER         = 247517,
    SPELL_FEL_LASH              = 247604,
    SPELL_SADIST                = 247590,
    SPELL_SADIST_PROC           = 247544,

    SPELL_FEL_BREATH            = 247731,
    SPELL_STOMP                 = 247733,
    SPELL_DRAIN                 = 247739,
    SPELL_DRAIN_HP              = 247742,

    SPELL_SEARING_GAZE          = 247320,
    SPELL_GUSHING_WOUND         = 247318,
    SPELL_LASH                  = 247325,
    SPELL_EYE_SORE              = 247332,
    SPELL_PHANTASM              = 247393,
    SPELL_PHANTASM_TRIGGER      = 247370,

    SPELL_REAP                  = 247492,
    SPELL_SOW                   = 247577,
    SPELL_SOW_DMG               = 247495,
    SPELL_DEATH_FIELD           = 247632,
    SPELL_DEATH_FIELD_VISUAL    = 247635,
    SPELL_SEEDS_OF_CHAOS        = 247586,

    SPELL_SILENCE               = 247698,
    SPELL_SOUL_CLEAVE           = 247410,
    SPELL_CAVITATION            = 247415,
    SPELL_SEED_OF_DESTRUCTION   = 247437,
    SPELL_WAKE_OF_DESTRUCTION   = 247440,

    SPELL_FEL_BLAST             = 254147,
    SPELL_INFECTED_CLAWS        = 247362,
    SPELL_INFECTED_CLAWS_PROC   = 247361,
    SPELL_NAUSEA                = 247441,
    SPELL_SLIMBERING_GASP       = 247379,
    SPELL_GROTESQUE_GREEN       = 247447,
    SPELL_GROTESQUE_RED         = 247497,
    SPELL_GROTESQUE_SPAWN       = 247443,
    SPELL_POOL_OF_FIRE          = 254802,

    SPELL_FEL_FIREBOLT          = 247423,
    SPELL_SCORCHING_PRESENCE    = 247401
};

enum eEvents
{
    EVENT_HEART_BREAKER         = 1,
    EVENT_FEL_LASH              = 2,
    EVENT_BEGUILING_CHARM       = 3,

    EVENT_FEL_BREATH            = 4,
    EVENT_STOMP                 = 5,
    EVENT_DRAIN                 = 6,

    EVENT_SEARING_GAZE          = 7,
    EVENT_GUSHING_WOUND         = 8,
    EVENT_LASH                  = 9,
    EVENT_EYE_SORE              = 10,
    EVENT_PHANTASM              = 11,

    EVENT_REAP                  = 12,
    EVENT_SOW                   = 13,
    EVENT_DEATH_FIELD           = 14,
    EVENT_SEEDS_OF_CHAOS        = 15,

    EVENT_SILENCE               = 16,
    EVENT_SOUL_CLEAVE           = 17,
    EVENT_CAVITATION            = 18,
    EVENT_SEED_OF_DESTRUCTION   = 19,

    EVENT_FEL_BLAST             = 20,
    EVENT_GROTESQUE_SPAWN       = 21
};

enum Phase
{
    PHASE_1 = 1,
    PHASE_2
};

#endif
