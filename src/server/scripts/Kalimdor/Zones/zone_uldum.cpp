/*
 * Uldum.
 *
 * Introduction (quest 27003 "Easy Money", started in Tanaris): clicking Lady Humps casts Initialize Uldum Intro.
 * Adarrah stops the player from riding her camel, the screen fades to black while the caravan is ambushed by
 * pygmies who sell everyone to the Neferset, and the player wakes up caged in the Lost City of the Tol'vir.
 * Handing the quest in, Adarrah picks the lock, the cage opens and a jailer comes in; once it is dead, Prince
 * Nadun calls the player over.
 *
 * Traitors! (27922): hiding behind the Neferset Frond, the screen fades to black while Siamat rewards the traitors,
 * then the secret is uncovered. Blizzard later remade this as a scene (4.0 [UTR] Uldum Traitors, package 2381), absent
 * from the 7.3.5 client: a fade to black stands in for it.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "Player.h"
#include "GameObject.h"
#include "TemporarySummon.h"

enum UldumIntro
{
    QUEST_EASY_MONEY                = 27003,

    SPELL_INITIALIZE_ULDUM_INTRO    = 86748,    // spellclick on Lady Humps: 3 s stun + script effect
    SPELL_FADE_TO_BLACK             = 87107,    // 8 s screen effect

    NPC_ADARRAH_TANARIS             = 44833,
    NPC_ADARRAH_CAGE                = 46873,
    NPC_PYGMY_AMBUSHER              = 46719,
    NPC_NEFERSET_JAILER             = 48029,
    NPC_PRINCE_NADUN                = 46872,
    NPC_CARAVAN_ESCORTED_CREDIT     = 44833,    // quest objective "Caravan Escorted"

    GO_ADARRAH_CAGE_DOOR            = 206953,

    SAY_ADARRAH_SILLY               = 0,        // Adarrah (Tanaris): "No, no, silly $r..."
    SAY_ADARRAH_NO_ONE_RIDES        = 1,        //                    "No one rides the Lady!"
    SAY_PYGMY_ATTACK                = 0,        // Pygmy Ambusher: "Attack!"
    SAY_PYGMY_COCONUTS              = 1,        //                 "How many coconuts can we get for the ugly one?"
    SAY_PYGMY_THREE                 = 2,        //                 "THREE? He woulda paid five, easy... Dummy!"
    SAY_ADARRAH_LOCK_PICK           = 0,        // Adarrah (cage): "Crap! That was my last lock pick!"
    SAY_NADUN_CAPTORS               = 0,        // Prince Nadun: "Our captors have allied with Deathwing, $r."
    SAY_NADUN_NO_MERCY              = 1,        //               "No mercy remains in them. Make your peace."
};

enum LostCityBetrayal
{
    QUEST_TRAITORS                  = 27922,
    NPC_SIAMAT_TERRACE              = 47285,
    NPC_BETRAYAL_CREDIT             = 47466,
};

Position const CagePlayerPos  = { -10994.9f, -1256.07f, 13.24f, 4.5f };
Position const JailerSpawnPos = { -11015.0f, -1262.0f, 13.4f, 2.6f };

namespace
{
    void TalkTo(Creature* creature, uint8 group, Player* player)
    {
        if (creature && creature->AI())
            creature->AI()->Talk(group, player->GetGUID());
    }

    Creature* SummonPygmy(Player* player, float angle, uint32 lifeMs)
    {
        Position pos = player->GetNearPosition(4.0f, angle);
        Creature* pygmy = player->SummonCreature(NPC_PYGMY_AMBUSHER, pos, TEMPSUMMON_TIMED_DESPAWN, lifeMs);
        if (pygmy)
        {
            // Visible on purpose: the voice of a unit the player cannot see is not sent. They are gone before the
            // screen clears, and must not pick a fight meanwhile.
            pygmy->SetReactState(REACT_PASSIVE);
            pygmy->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
        }
        return pygmy;
    }

    void CloseCage(Player* player)
    {
        // The cage door is spawned open (start open, as in the TDBs): shut it on the prisoner; Adarrah's lock pick
        // opens it again when the quest is handed in.
        if (GameObject* door = player->FindNearestGameObject(GO_ADARRAH_CAGE_DOOR, 10.0f))
            door->SetGoState(GO_STATE_READY);
    }
}

// 86748 - Initialize Uldum Intro
class spell_uldum_initialize_intro : public SpellScriptLoader
{
public:
    spell_uldum_initialize_intro() : SpellScriptLoader("spell_uldum_initialize_intro") { }

    class spell_uldum_initialize_intro_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_uldum_initialize_intro_SpellScript);

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
            if (!player || player->GetQuestStatus(QUEST_EASY_MONEY) != QUEST_STATUS_INCOMPLETE)
                return;

            ObjectGuid const playerGuid = player->GetGUID();
            ObjectGuid adarrahGuid;
            if (Creature* adarrah = player->FindNearestCreature(NPC_ADARRAH_TANARIS, 40.0f))
            {
                adarrahGuid = adarrah->GetGUID();
                TalkTo(adarrah, SAY_ADARRAH_SILLY, player);
            }

            // Each step re-reads the player and the actors: the player may log out or die meanwhile, and the
            // delayed events belong to the player, so they go with it.
            player->AddDelayedEvent(2500, [playerGuid, adarrahGuid]() -> void
            {
                Player* player = ObjectAccessor::FindPlayer(playerGuid);
                if (!player)
                    return;
                if (Creature* adarrah = ObjectAccessor::GetCreature(*player, adarrahGuid))
                    TalkTo(adarrah, SAY_ADARRAH_NO_ONE_RIDES, player);
            });

            player->AddDelayedEvent(5000, [playerGuid]() -> void
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                    player->AddAura(SPELL_FADE_TO_BLACK, player);
            });

            player->AddDelayedEvent(6500, [playerGuid]() -> void
            {
                Player* player = ObjectAccessor::FindPlayer(playerGuid);
                if (!player || player->GetQuestStatus(QUEST_EASY_MONEY) != QUEST_STATUS_INCOMPLETE)
                    return;
                player->TeleportTo(player->GetMapId(), CagePlayerPos.GetPositionX(), CagePlayerPos.GetPositionY(),
                    CagePlayerPos.GetPositionZ(), CagePlayerPos.GetOrientation());

                // Once the client has landed (the server still places the player in Tanaris right after the teleport):
                // keep the screen black for the ambush and lock the cage.
                player->AddDelayedEvent(1500, [playerGuid]() -> void
                {
                    if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                    {
                        player->RemoveAurasDueToSpell(SPELL_FADE_TO_BLACK);
                        player->AddAura(SPELL_FADE_TO_BLACK, player);       // 8 s, until the end of the ambush
                        CloseCage(player);
                        // Working at the lock until Adarrah gets it open (cleared when the quest is handed in)
                        player->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_USE_STANDING);
                    }
                });

                // The ambush, heard in the dark: three pygmies around the caravan, all gone before the screen clears.
                player->AddDelayedEvent(2000, [playerGuid]() -> void
                {
                    if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                        TalkTo(SummonPygmy(player, 0.0f, 6500), SAY_PYGMY_ATTACK, player);
                });
                player->AddDelayedEvent(3800, [playerGuid]() -> void
                {
                    if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                        TalkTo(SummonPygmy(player, float(M_PI) / 2, 4700), SAY_PYGMY_COCONUTS, player);
                });
                player->AddDelayedEvent(5600, [playerGuid]() -> void
                {
                    if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                        TalkTo(SummonPygmy(player, float(M_PI), 2900), SAY_PYGMY_THREE, player);
                });
                player->AddDelayedEvent(9600, [playerGuid]() -> void
                {
                    if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                    {
                        // An aura timing out across the teleport can leave the client playing it: end them here.
                        player->RemoveAurasDueToSpell(SPELL_INITIALIZE_ULDUM_INTRO);
                        player->RemoveAurasDueToSpell(SPELL_FADE_TO_BLACK);
                        CloseCage(player);
                        player->KilledMonsterCredit(NPC_CARAVAN_ESCORTED_CREDIT);
                    }
                });
            });
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_uldum_initialize_intro_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_uldum_initialize_intro_SpellScript();
    }
};

// 46873 - Adarrah, in the player's cage
class npc_uldum_adarrah_cage : public CreatureScript
{
public:
    npc_uldum_adarrah_cage() : CreatureScript("npc_uldum_adarrah_cage") { }

    bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 /*opt*/) override
    {
        if (quest->GetQuestId() != QUEST_EASY_MONEY)
            return false;

        TalkTo(creature, SAY_ADARRAH_LOCK_PICK, player);
        player->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_ONESHOT_NONE);
        if (GameObject* door = creature->FindNearestGameObject(GO_ADARRAH_CAGE_DOOR, 8.0f))
            door->SetGoState(GO_STATE_ACTIVE);

        // The jailer walks in on the escape, for this player only.
        if (Creature* jailer = player->SummonCreature(NPC_NEFERSET_JAILER, JailerSpawnPos, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000))
            if (jailer->AI())
                jailer->AI()->AttackStart(player);
        return false;
    }
};

// 48029 - Neferset Jailer, summoned when the cage opens
class npc_uldum_neferset_jailer : public CreatureScript
{
public:
    npc_uldum_neferset_jailer() : CreatureScript("npc_uldum_neferset_jailer") { }

    struct npc_uldum_neferset_jailerAI : public ScriptedAI
    {
        npc_uldum_neferset_jailerAI(Creature* creature) : ScriptedAI(creature) { }

        void JustDied(Unit* killer) override
        {
            Player* player = killer ? killer->GetCharmerOrOwnerPlayerOrPlayerItself() : nullptr;
            if (!player)
                return;
            Creature* nadun = me->FindNearestCreature(NPC_PRINCE_NADUN, 40.0f);
            if (!nadun)
                return;
            TalkTo(nadun, SAY_NADUN_CAPTORS, player);
            ObjectGuid const playerGuid = player->GetGUID(), nadunGuid = nadun->GetGUID();
            player->AddDelayedEvent(4000, [playerGuid, nadunGuid]() -> void
            {
                Player* player = ObjectAccessor::FindPlayer(playerGuid);
                if (!player)
                    return;
                if (Creature* nadun = ObjectAccessor::GetCreature(*player, nadunGuid))
                    TalkTo(nadun, SAY_NADUN_NO_MERCY, player);
            });
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_uldum_neferset_jailerAI(creature);
    }
};

// 206579 - Neferset Frond (Traitors!)
class go_uldum_neferset_frond : public GameObjectScript
{
public:
    go_uldum_neferset_frond() : GameObjectScript("go_uldum_neferset_frond") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/) override
    {
        if (player->GetQuestStatus(QUEST_TRAITORS) != QUEST_STATUS_INCOMPLETE)
            return true;

        player->AddAura(SPELL_FADE_TO_BLACK, player);   // 8 s
        // Siamat's own SmartAI says both lines once its data 0 is set to 1.
        if (Creature* siamat = player->FindNearestCreature(NPC_SIAMAT_TERRACE, 80.0f))
            if (siamat->AI())
                siamat->AI()->SetData(0, 1);

        ObjectGuid const playerGuid = player->GetGUID();
        player->AddDelayedEvent(9000, [playerGuid]() -> void
        {
            if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
            {
                player->RemoveAurasDueToSpell(SPELL_FADE_TO_BLACK);
                player->KilledMonsterCredit(NPC_BETRAYAL_CREDIT);
            }
        });
        return true;
    }
};

void AddSC_uldum()
{
    new go_uldum_neferset_frond();
    new spell_uldum_initialize_intro();
    new npc_uldum_adarrah_cage();
    new npc_uldum_neferset_jailer();
}
