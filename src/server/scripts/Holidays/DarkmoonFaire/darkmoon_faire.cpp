////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "Cell.h"
#include "CellImpl.h"
#include "CharmInfo.h"
#include "CombatAI.h"
#include "CreatureTextMgr.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "ObjectMgr.h"
#include "ObjectVisitors.hpp"
#include "PassiveAI.h"
#include "PetAI.h"
#include "Player.h"
#include "PlayerDefines.h"
#include "ScriptedCreature.h"
#include "ScriptedFollowerAI.h"
#include "ScriptedEscortAI.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "Vehicle.h"
#include "World.h"
#include "QuestData.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "Timewalking.h"
#include "ScriptPCH.h"
#include <ScriptMgr.h>
#include "Bag.h"
#include "Channel.h"
#include "ChatLink.h"
#include "GuildMgr.h"
#include "WordFilterMgr.h"
#include "CollectionMgr.h"
#include "AreaTriggerAI.h"
#include "AreaTrigger.h"
#include "BattlegroundMgr.h"
#include "Garrison.h"
#include "Group.h"
#include "InstanceScript.h"
#include "LFGMgr.h"
#include "Packets/ArtifactPackets.h"
#include "Packets/SpellPackets.h"
#include "Packets/NPCPackets.h"
#include "SkillDiscovery.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "GameTime.h"

// 134815 - WHEE! (Darkmoon carousel, cast on boarding a ride): a ride ticket for the 1 hour bonus
class spell_gen_darkmoon_carousel_whee : public SpellScript
{
    PrepareSpellScript(spell_gen_darkmoon_carousel_whee);

    enum
    {
        ITEM_DARKMOON_RIDE_TICKET   = 81055,
        SPELL_WHEE                  = 46668,
        SPELL_CAROUSEL_RIDE_CHEER   = 137471
    };

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        if (!player || !player->HasItemCount(ITEM_DARKMOON_RIDE_TICKET, 1))
            return;

        player->DestroyItemCount(ITEM_DARKMOON_RIDE_TICKET, 1, true);
        player->CastSpell(player, SPELL_WHEE, true);
        player->CastSpell(player, SPELL_CAROUSEL_RIDE_CHEER, true);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_gen_darkmoon_carousel_whee::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

/*######
## npc_sayge
######*/

#define SPELL_DMG       23768                               //dmg
#define SPELL_RES       23769                               //res
#define SPELL_ARM       23767                               //arm
#define SPELL_SPI       23738                               //spi
#define SPELL_INT       23766                               //int
#define SPELL_STM       23737                               //stm
#define SPELL_STR       23735                               //str
#define SPELL_AGI       23736                               //agi
#define SPELL_FORTUNE   23765                               //faire fortune

#define GOSSIP_HELLO_SAYGE  "Yes"
#define GOSSIP_SENDACTION_SAYGE1    "Slay the Man"
#define GOSSIP_SENDACTION_SAYGE2    "Turn him over to liege"
#define GOSSIP_SENDACTION_SAYGE3    "Confiscate the corn"
#define GOSSIP_SENDACTION_SAYGE4    "Let him go and have the corn"
#define GOSSIP_SENDACTION_SAYGE5    "Execute your friend painfully"
#define GOSSIP_SENDACTION_SAYGE6    "Execute your friend painlessly"
#define GOSSIP_SENDACTION_SAYGE7    "Let your friend go"
#define GOSSIP_SENDACTION_SAYGE8    "Confront the diplomat"
#define GOSSIP_SENDACTION_SAYGE9    "Show not so quiet defiance"
#define GOSSIP_SENDACTION_SAYGE10   "Remain quiet"
#define GOSSIP_SENDACTION_SAYGE11   "Speak against your brother openly"
#define GOSSIP_SENDACTION_SAYGE12   "Help your brother in"
#define GOSSIP_SENDACTION_SAYGE13   "Keep your brother out without letting him know"
#define GOSSIP_SENDACTION_SAYGE14   "Take credit, keep gold"
#define GOSSIP_SENDACTION_SAYGE15   "Take credit, share the gold"
#define GOSSIP_SENDACTION_SAYGE16   "Let the knight take credit"
#define GOSSIP_SENDACTION_SAYGE17   "Thanks"

class npc_sayge : public CreatureScript
{
    public:
        npc_sayge() : CreatureScript("npc_sayge") { }

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            if (creature->isQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (player->HasSpellCooldown(SPELL_INT) ||
                player->HasSpellCooldown(SPELL_ARM) ||
                player->HasSpellCooldown(SPELL_DMG) ||
                player->HasSpellCooldown(SPELL_RES) ||
                player->HasSpellCooldown(SPELL_STR) ||
                player->HasSpellCooldown(SPELL_AGI) ||
                player->HasSpellCooldown(SPELL_STM) ||
                player->HasSpellCooldown(SPELL_SPI))
                player->SEND_GOSSIP_MENU(7393, creature->GetGUID());
            else
            {
                player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_HELLO_SAYGE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->SEND_GOSSIP_MENU(7339, creature->GetGUID());
            }

            return true;
        }

        void SendAction(Player* player, Creature* creature, uint32 action)
        {
            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE1,            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE2,            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE3,            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE4,            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                    player->SEND_GOSSIP_MENU(7340, creature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2:
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE5,            GOSSIP_SENDER_MAIN + 1, GOSSIP_ACTION_INFO_DEF);
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE6,            GOSSIP_SENDER_MAIN + 2, GOSSIP_ACTION_INFO_DEF);
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE7,            GOSSIP_SENDER_MAIN + 3, GOSSIP_ACTION_INFO_DEF);
                    player->SEND_GOSSIP_MENU(7341, creature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF + 3:
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE8,            GOSSIP_SENDER_MAIN + 4, GOSSIP_ACTION_INFO_DEF);
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE9,            GOSSIP_SENDER_MAIN + 5, GOSSIP_ACTION_INFO_DEF);
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE10,           GOSSIP_SENDER_MAIN + 2, GOSSIP_ACTION_INFO_DEF);
                    player->SEND_GOSSIP_MENU(7361, creature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF + 4:
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE11,           GOSSIP_SENDER_MAIN + 6, GOSSIP_ACTION_INFO_DEF);
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE12,           GOSSIP_SENDER_MAIN + 7, GOSSIP_ACTION_INFO_DEF);
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE13,           GOSSIP_SENDER_MAIN + 8, GOSSIP_ACTION_INFO_DEF);
                    player->SEND_GOSSIP_MENU(7362, creature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF + 5:
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE14,           GOSSIP_SENDER_MAIN + 5, GOSSIP_ACTION_INFO_DEF);
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE15,           GOSSIP_SENDER_MAIN + 4, GOSSIP_ACTION_INFO_DEF);
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE16,           GOSSIP_SENDER_MAIN + 3, GOSSIP_ACTION_INFO_DEF);
                    player->SEND_GOSSIP_MENU(7363, creature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF:
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_SENDACTION_SAYGE17,           GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                    player->SEND_GOSSIP_MENU(7364, creature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF + 6:
                    creature->CastSpell(player, SPELL_FORTUNE, false);
                    player->SEND_GOSSIP_MENU(7365, creature->GetGUID());
                    break;
            }
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
        {
            player->PlayerTalkClass->ClearMenus();
            switch (sender)
            {
                case GOSSIP_SENDER_MAIN:
                    SendAction(player, creature, action);
                    break;
                case GOSSIP_SENDER_MAIN + 1:
                    creature->CastSpell(player, SPELL_DMG, false);
                    player->AddSpellCooldown(SPELL_DMG, 0, getPreciseTime() + 7200.0);
                    SendAction(player, creature, action);
                    break;
                case GOSSIP_SENDER_MAIN + 2:
                    creature->CastSpell(player, SPELL_RES, false);
                    player->AddSpellCooldown(SPELL_RES, 0, getPreciseTime() + 7200.0);
                    SendAction(player, creature, action);
                    break;
                case GOSSIP_SENDER_MAIN + 3:
                    creature->CastSpell(player, SPELL_ARM, false);
                    player->AddSpellCooldown(SPELL_ARM, 0, getPreciseTime() + 7200.0);
                    SendAction(player, creature, action);
                    break;
                case GOSSIP_SENDER_MAIN + 4:
                    creature->CastSpell(player, SPELL_SPI, false);
                    player->AddSpellCooldown(SPELL_SPI, 0, getPreciseTime() + 7200.0);
                    SendAction(player, creature, action);
                    break;
                case GOSSIP_SENDER_MAIN + 5:
                    creature->CastSpell(player, SPELL_INT, false);
                    player->AddSpellCooldown(SPELL_INT, 0, getPreciseTime() + 7200.0);
                    SendAction(player, creature, action);
                    break;
                case GOSSIP_SENDER_MAIN + 6:
                    creature->CastSpell(player, SPELL_STM, false);
                    player->AddSpellCooldown(SPELL_STM, 0, getPreciseTime() + 7200.0);
                    SendAction(player, creature, action);
                    break;
                case GOSSIP_SENDER_MAIN + 7:
                    creature->CastSpell(player, SPELL_STR, false);
                    player->AddSpellCooldown(SPELL_STR, 0, getPreciseTime() + 7200.0);
                    SendAction(player, creature, action);
                    break;
                case GOSSIP_SENDER_MAIN + 8:
                    creature->CastSpell(player, SPELL_AGI, false);
                    player->AddSpellCooldown(SPELL_AGI, 0, getPreciseTime() + 7200.0);
                    SendAction(player, creature, action);
                    break;
            }
            return true;
        }
};

class npc_steam_tonk : public CreatureScript
{
    public:
        npc_steam_tonk() : CreatureScript("npc_steam_tonk") { }

        struct npc_steam_tonkAI : public ScriptedAI
        {
            npc_steam_tonkAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset() override {}
            void EnterCombat(Unit* /*who*/) override {}

            void OnPossess(bool apply)
            {
                if (apply)
                {
                    // Initialize the action bar without the melee attack command
                    me->InitCharmInfo();
                    me->GetCharmInfo()->InitEmptyActionBar(false);

                    me->SetReactState(REACT_PASSIVE);
                }
                else
                    me->SetReactState(REACT_AGGRESSIVE);
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_steam_tonkAI(creature);
        }
};

#define SPELL_TONK_MINE_DETONATE 25099

class npc_tonk_mine : public CreatureScript
{
    public:
        npc_tonk_mine() : CreatureScript("npc_tonk_mine") { }

        struct npc_tonk_mineAI : public ScriptedAI
        {
            npc_tonk_mineAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            uint32 ExplosionTimer;

            void Reset() override
            {
                ExplosionTimer = 3000;
            }

            void EnterCombat(Unit* /*who*/) override {}
            void AttackStart(Unit* /*who*/) override {}
            void MoveInLineOfSight(Unit* /*who*/) override {}

            void UpdateAI(uint32 diff) override
            {
                if (ExplosionTimer <= diff)
                {
                    DoCast(me, SPELL_TONK_MINE_DETONATE, true);
                    me->setDeathState(DEAD); // unsummon it
                }
                else
                    ExplosionTimer -= diff;
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_tonk_mineAI(creature);
        }
};

// The faire's music (darkmoonfaire_2, 74 s) used to go out every 5 s to the whole visibility range,
// through SoundKit 8440, heard up to 150 yards: a new copy over the zone music every 5 s. It now goes
// once per play-through to players who come within 18 yards, through a copy of that kit that fades
// out over those 18 yards. 8440 itself stays as it is: it is also the Darkmoon Island zone music.
// Meanwhile a near-silent stinger holds their music channel, which keeps the zone music quiet.
struct go_darkmoon_faire_music : public GameObjectAI
{
    enum
    {
        SOUND_KIT_MUSIC_CLOSE   = 98010,
        SOUND_KIT_HUSH          = 98011,
        MUSIC_LENGTH            = 75 * IN_MILLISECONDS,
        HUSH_EVERY              = 8,        // seconds; the stinger lasts 9.8
    };

    go_darkmoon_faire_music(GameObject* go) : GameObjectAI(go)
    {
        events.ScheduleEvent(EVENT_1, 1000);
    }

    EventMap events;
    std::unordered_map<ObjectGuid, uint32> startedAt;
    uint32 tick = 0;

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_1:
            {
                uint32 const now = getMSTime();
                for (auto itr = startedAt.begin(); itr != startedAt.end();)
                    itr = getMSTimeDiff(itr->second, now) >= uint32(MUSIC_LENGTH) ? startedAt.erase(itr) : std::next(itr);

                if (IsHolidayActive(HOLIDAY_DARKMOON_FAIRE))
                {
                    bool const hush = tick++ % HUSH_EVERY == 0;
                    std::list<Player*> playersNearby;
                    go->GetPlayerListInGrid(playersNearby, 18.0f);
                    for (Player* player : playersNearby)
                    {
                        bool const arriving = startedAt.emplace(player->GetGUID(), now).second;
                        if (arriving || hush)
                            player->SendMusic(SOUND_KIT_HUSH);
                        if (arriving)
                            go->PlayDirectSound(SOUND_KIT_MUSIC_CLOSE, player);
                    }
                }

                events.ScheduleEvent(EVENT_1, 1000);
                break;
            }
            default:
                break;
            }
        }
    }
};

class playerScriptDarkmoonMisc : public PlayerScript
{
public:
    playerScriptDarkmoonMisc() : PlayerScript("playerScriptDarkmoonMisc") {}

    void OnLogin(Player* player) override
    {
        player->AddDelayedEvent(100, [player]() -> void
        {
            if (!player)
                return;

            if (!sGameEventMgr->IsActiveEvent(75))
                if (player->GetMapId() == 974)
                    if (!player->isGameMaster())
                        player->CastSpell(player, 188816, true);

            //toy
            if (!player->HasAura(74890))
                player->RemoveAurasDueToSpell(75055);
        });
    }
};

uint32 morph_list_alliance_darkmoon[49] =
{
    1437, 17514, 25521, 1697, 3086,
    1430, 6975, 18295, 1477, 21621,
    2271, 17216, 5042, 25336, 14754,
    17515, 3120, 2258, 5040, 17486,
    2226, 17506, 17241, 2222, 17500,
    3104, 17682, 18296, 17503, 5083,
    1521, 1496, 3052, 17478, 2203,
    2263, 1404, 1449, 10620, 17507,
    3064, 3596, 9823, 10573, 10575,
    8478, 7769, 6844, 15453
};

uint32 morph_list_horde_darkmoon[53] =
{
    1314, 16801, 1358, 4359,
    4537, 4356, 8000, 1680, 27286,
    16824, 11750, 1366, 4064, 1378,
    4532, 4087, 4084, 15570, 11901,
    2671, 12049, 19798, 14759, 3604,
    17671, 16823, 1976, 1394, 1879,
    4363, 30894, 1384, 5846, 1319,
    16781, 7040, 18159, 1329, 4057,
    2648, 16753, 4009, 2666, 1395,
    27293, 4053, 4534, 1317, 15701,
    17278, 4075, 4610, 2629
};

// 70764
class spell_gen_citizen_costume_alliance_darkmoon : public AuraScript
{
    PrepareAuraScript(spell_gen_citizen_costume_alliance_darkmoon);

    void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        uint32 select_morph = morph_list_alliance_darkmoon[urand(0, 48)];

        caster->SetDisplayId(select_morph);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_gen_citizen_costume_alliance_darkmoon::AfterApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
    }
};

// 71084
class spell_gen_citizen_costume_horde_darkmoon : public AuraScript
{
    PrepareAuraScript(spell_gen_citizen_costume_horde_darkmoon);

    void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        uint32 select_morph = morph_list_horde_darkmoon[urand(0, 52)];

        caster->SetDisplayId(select_morph);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_gen_citizen_costume_horde_darkmoon::AfterApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_darkmoon_faire()
{
    RegisterSpellScript(spell_gen_darkmoon_carousel_whee);
    new npc_sayge();
    new npc_steam_tonk();
    new npc_tonk_mine();
    RegisterGameObjectAI(go_darkmoon_faire_music);
    new playerScriptDarkmoonMisc();
    RegisterAuraScript(spell_gen_citizen_costume_alliance_darkmoon);
    RegisterAuraScript(spell_gen_citizen_costume_horde_darkmoon);
}
