/*
    Dungeon : Violet Hold Legion 100-110
*/

#ifndef VIOLET_HOLD_LEGION_H_
#define VIOLET_HOLD_LEGION_H_

#define DataHeader "VHL"

enum eData
{
    DATA_KAAHRJ             = 0,
    DATA_MILLIFICENT        = 1,
    DATA_FESTERFACE         = 2,
    DATA_SHIVERMAW          = 3,
    DATA_ANUBESSET          = 4,
    DATA_SAELORN            = 5,
    DATA_THALENA            = 6,
    DATA_BETRUG             = 7,
    MAX_ENCOUNTER,

    DATA_MAIN_EVENT_PHASE,
    DATA_1ST_BOSS_EVENT,
    DATA_2ND_BOSS_EVENT,
    DATA_BETRUG_EVENT,
    DATA_ADD_TRASH_MOB,
    DATA_DEL_TRASH_MOB,
    DATA_START_BOSS_ENCOUNTER,
    DATA_FIRST_BOSS,
    DATA_SECOND_BOSS,
    DATA_FINAL_BOSS,
    DATA_SINCLARI,
    DATA_STEP,
    DATA_STEP_BOSS,
    DATA_WAVES_ACTIVE,
    DATA_PORTAL_CLOSED,
    DATA_INTRO_CRYSTAL,
    DATA_MALGATH_DIED,
    DATA_MAIN_DOOR,
    DATA_DOOR_INTEGRITY,
    DATA_KAAHRJ_CELL,
    DATA_MILLIFICENT_CELL,
    DATA_FESTERFACE_CELL,
    DATA_SHIVERMAW_CELL,
    DATA_ANUBESSET_CELL,
    DATA_SAELORN_CELL,
    DATA_THALENA_CELL,
};

enum eCreatures
{
    NPC_LIEUTENANT_SINCLARI         = 102278,
    NPC_VIOLET_HOLD_GUARD           = 102266,
    NPC_TELEPORTATION_PORTAL        = 102279,
    NPC_INTRO_PORTAL                = 102281,
    NPC_DEFENSE_SYSTEM              = 30837,
    NPC_LORD_MALGATH                = 102282,

    //Portal summons
    NPC_PORTAL_GUARDIAN_1           = 102335,
    NPC_PORTAL_GUARDIAN_2           = 102337,
    NPC_PORTAL_KEEPER_1             = 102302,
    NPC_PORTAL_KEEPER_2             = 102336,
    NPC_FELGUARD_DESTROYER_1        = 102272,
    NPC_FELGUARD_DESTROYER_2        = 102368,
    NPC_SHADOW_COUNCIL_WARLOCK      = 102380,
    NPC_INFILTRATOR_ASSASSIN        = 102395,
    NPC_EREDAR_SHADOW_MENDER        = 102400,
    NPC_WRATHLORD_BULWARK           = 102397,
    NPC_BLAZING_INFERNAL            = 102398,
    NPC_FELSTALKER_RAVENER_1        = 102269,
    NPC_FELSTALKER_RAVENER_2        = 102369,
    NPC_EREDAR_INVADER_1            = 102270,
    NPC_EREDAR_INVADER_2            = 102370,

    //Mythic elite squad
    NPC_WRATHGUARD_DECIMATOR        = 112741,
    NPC_SHADOWY_OVERFIEND           = 112739,
    NPC_BROOD_OF_SAELORN            = 112732,
    NPC_VENOMHIDE_SHADOWSPINNER     = 112733,
    NPC_ACOLYTE_OF_SAELORN          = 112738,

    //Lord Malgath summons
    NPC_SHADOW_BEAST                = 103561,

    //Anubesset summons
    NPC_SPITTING_SCARAB             = 102271,

    //Sael'orn summons
    NPC_PHASE_SPIDER                = 102434,

    //Betrug summons
    NPC_STASIS_CRYSTAL              = 103672,

    //Encounters
    NPC_MINDFLAYER_KAAHRJ           = 101950,  // Talks +
    NPC_MILLIFICENT_MANASTORM       = 101976,  // Talks and event+
    NPC_FESTERFACE                  = 101995,  //Warnings and talks+
    NPC_SHIVERMAW                   = 101951,  //Warnings+
    NPC_ANUBESSET                   = 102246,  //Talks empty
    NPC_SAELORN                     = 102387,  //Talks+
    NPC_PRINCESS_THALENA            = 102431,  //Talks+
    NPC_FEL_LORD_BETRUG             = 102446,  //Talks+
};

enum eGameObjects
{
    GO_MAIN_DOOR                    = 247002,
    GO_INTRO_ACTIVATION_CRYSTAL     = 193615,
    GO_ACTIVATION_CRYSTAL           = 193611,

    GO_KAAHRJ_DOOR                  = 247000,
    GO_MILLIFICENT_DOOR             = 246990,
    GO_FESTERFACE_DOOR              = 246998,
    GO_SHIVERMAW_DOOR               = 246999,
    GO_ANUBESSET_DOOR               = 247385,
    GO_SAELORN_DOOR                 = 246993,
    GO_THALENA_DOOR                 = 246997,
};

enum otherSpells
{
    SPELL_ARCANE_LIGHTNING          = 57930,
    SPELL_FEL_SHIELD                = 202311,
    SPELL_PORTAL_CHANNEL            = 202268,
    SPELL_PORTAL_PERIODIC           = 201901, //Caster 102282, HitUnit 102279
    SPELL_SHIELD_DESTRUCTION        = 202312,
    SPELL_DESTROY_DOOR_SEAL         = 202708,
};

enum Events
{
    EVENT_ACTIVATE_CRYSTAL = 20001,

    ACTION_REMOVE_LOOT = 100,
    ACTION_MALGATH_OPEN_CELL = 101,
    ACTION_MALGATH_FIGHT = 102,
    ACTION_MALGATH_OPEN_PORTAL = 103,
    DATA_PORTAL_KIND = 104,                 // 0 elite squad, 1 portal guardian or keeper
};

enum MalgathSays
{
    SAY_MALGATH_INTRO       = 0,
    SAY_MALGATH_THIRD_STEP  = 7,
    SAY_MALGATH_AGGRO       = 8,
    SAY_MALGATH_KILL        = 9,
    SAY_MALGATH_DEATH       = 10,
};

// Lord Malgath introduces the prisoner he releases, indexed by boss data id (Sael'orn and Betrug are never released by him)
uint8 const malgathCellSay[7] = { 1, 2, 3, 4, 5, 0, 6 };

// Scenario 1027 / 1251: "Invasion Forces" needs 75 weighted points per step, Lord Malgath is worth 25 in the last one
uint32 const INVASION_FORCES_POINTS = 75;
uint32 const MALGATH_POINTS         = 25;

// SCRIPT_EVENT_2 assets of the "boss defeated" criteria of each step
uint32 const stepBossCriteria[3] = { 41195, 41196, 48726 };

uint32 const eliteSquadEntries[4] = { NPC_EREDAR_SHADOW_MENDER, NPC_BLAZING_INFERNAL, NPC_INFILTRATOR_ASSASSIN, NPC_WRATHLORD_BULWARK };
uint32 const portalTrashEntries[4] = { NPC_FELGUARD_DESTROYER_2, NPC_FELSTALKER_RAVENER_2, NPC_EREDAR_INVADER_2, NPC_SHADOW_COUNCIL_WARLOCK };
uint32 const introDemonEntries[3] = { NPC_FELGUARD_DESTROYER_1, NPC_FELSTALKER_RAVENER_1, NPC_EREDAR_INVADER_1 };

inline uint32 GetInvasionForcesWeight(uint32 entry)
{
    switch (entry)
    {
        case NPC_PORTAL_GUARDIAN_1:
        case NPC_PORTAL_GUARDIAN_2:
        case NPC_PORTAL_KEEPER_1:
        case NPC_PORTAL_KEEPER_2:
            return 15;
        case NPC_EREDAR_SHADOW_MENDER:
        case NPC_BLAZING_INFERNAL:
        case NPC_INFILTRATOR_ASSASSIN:
        case NPC_WRATHLORD_BULWARK:
        case NPC_WRATHGUARD_DECIMATOR:
        case NPC_SHADOWY_OVERFIEND:
        case NPC_BROOD_OF_SAELORN:
        case NPC_VENOMHIDE_SHADOWSPINNER:
        case NPC_ACOLYTE_OF_SAELORN:
            return 5;
        case NPC_LORD_MALGATH:
            return MALGATH_POINTS;
        default:
            return 0;
    }
}

Position const PortalLocation[5] =
{
    {4682.28f, 4013.68f, 91.80f, 3.14f},
    {4653.47f, 4041.61f, 78.08f, 3.74f},
    {4636.86f, 3963.92f, 87.05f, 2.05f},
    {4623.71f, 4060.67f, 82.72f, 4.76f},
    {4604.43f, 3980.73f, 78.04f, 1.10f}
};

Position const bossStartMove[7] =
{
    {4638.79f, 3955.60f, 86.97f}, //Mindflayer Kaahrj
    {4650.04f, 4051.10f, 77.97f}, //Millificent Manastorm
    {4669.23f, 4060.56f, 86.48f}, //Festerface
    {4598.52f, 3973.44f, 77.96f}, //Shivermaw
    {4672.41f, 3976.15f, 69.96f}, //Anub'esset
    {4691.27f, 4014.39f, 91.72f}, //Sael'orn
    {4619.39f, 4071.58f, 82.64f}  //Blood-Princess Thal'ena
};

Position const saboMovePos[7] =
{
    {4638.01f, 3962.11f, 95.58f, 4.80f}, //Cage Mindflayer Kaahrj
    {4644.70f, 4043.72f, 85.37f, 1.02f}, //Cage Millificent Manastorm
    {4662.63f, 4052.92f, 97.89f, 0.88f}, //Cage Festerface
    {4603.96f, 3978.66f, 85.50f, 4.04f}, //Cage Shivermaw
    {4665.21f, 3984.07f, 75.94f, 5.49f}, //Cage Anub'esset
    {4679.61f, 4016.60f, 98.52f, 6.19f}, //Cage Sael'orn
    {4620.95f, 4063.44f, 89.05f, 1.84f}  //Cage Blood-Princess Thal'ena
};

Position const centrPos = {4628.95f, 4013.66f, 77.97f, 3.08f};
Position const MiddleRoomSaboLoc = {4637.53f, 4016.16f, 98.52f, 6.25f};
Position const saboFightPos = {4663.58f, 4014.95f, 82.72f, 3.01f};

#endif