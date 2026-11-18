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
#include <functional>

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

// By Any Means: the demon hunter's copy of the Violet Hold (map 1494) is his own; leaving it unbinds him, so that his next
// visit starts the scene again instead of finding it played or left halfway
class player_violet_hold_quest_copy : public PlayerScript
{
public:
    player_violet_hold_quest_copy() : PlayerScript("player_violet_hold_quest_copy") { }

    static constexpr uint32 MAP_VIOLET_HOLD_QUEST_COPY = 1494;

    void OnMapChanged(Player* player) override
    {
        if (player->getClass() != CLASS_DEMON_HUNTER || player->GetMapId() == MAP_VIOLET_HOLD_QUEST_COPY)
            return;

        for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
        {
            Player::BoundInstancesMap const binds = player->GetBoundInstancesCopy(Difficulty(i));
            if (binds.find(MAP_VIOLET_HOLD_QUEST_COPY) != binds.end())
                player->UnbindInstance(MAP_VIOLET_HOLD_QUEST_COPY, Difficulty(i));
        }
    }
};

void AddSC_DH_havoc_art_scenario()
{
    new player_violet_hold_quest_copy();
    RegisterCreatureAI(npc_taldath_by_any_means);
}
