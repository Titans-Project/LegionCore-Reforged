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
#include "TemporarySummon.h"
#include "DB2Stores.h"
#include "Item.h"
#include "Challenge.h"
#include "ChallengeMgr.h"
#include "AreaTriggerAI.h"
#include "GameTime.h"
#include "InstanceScript.h"
#include "SkillDiscovery.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "AreaTrigger.h"
#include "BattlegroundMgr.h"
#include "Garrison.h"
#include "Group.h"
#include "LFGMgr.h"
#include "Packets/ArtifactPackets.h"
#include "Packets/SpellPackets.h"
#include "Packets/NPCPackets.h"

// the racing ram and its paces, shared by the NPCs, the reins and the speed aura
enum BrewfestRamSpells
{
    SPELL_BREWFEST_RAM          = 43880,
    SPELL_RAM_FATIGUE           = 43052,
    SPELL_SPEED_RAM_GALLOP      = 42994,
    SPELL_SPEED_RAM_CANTER      = 42993,
    SPELL_SPEED_RAM_TROT        = 42992,
    SPELL_SPEED_RAM_NORMAL      = 43310,
    SPELL_SPEED_RAM_EXHAUSED    = 43332
};

/*####
## npc_brewfest_reveler
####*/

class npc_brewfest_reveler : public CreatureScript
{
    public:
        npc_brewfest_reveler() : CreatureScript("npc_brewfest_reveler") { }

        struct npc_brewfest_revelerAI : public ScriptedAI
        {
            npc_brewfest_revelerAI(Creature* creature) : ScriptedAI(creature) {}
            void ReceiveEmote(Player* player, uint32 emote) override
            {
                if (!IsHolidayActive(HOLIDAY_BREWFEST))
                    return;

                if (emote == TEXT_EMOTE_DANCE)
                    me->CastSpell(player, 41586, false);
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_brewfest_revelerAI(creature);
        }
};

/*####
## npc_brewfest_trigger
####*/
enum eBrewfestBarkQuests
{
    BARK_FOR_THE_THUNDERBREWS       = 11294,
    BARK_FOR_TCHALIS_VOODOO_BREWERY = 11408,
    BARK_FOR_THE_BARLEYBREWS        = 11293,
    BARK_FOR_DROHNS_DISTILLERY      = 11407
};

class npc_brewfest_trigger : public CreatureScript
{
public:
    npc_brewfest_trigger() : CreatureScript("npc_brewfest_trigger") { }

    struct npc_brewfest_triggerAI : public ScriptedAI
    {
        npc_brewfest_triggerAI(Creature* c) : ScriptedAI(c) {}
        void MoveInLineOfSight(Unit *who) override
        {
            Player *pPlayer = who->ToPlayer();
            if (!pPlayer)
                return;
            if (pPlayer->GetQuestStatus(BARK_FOR_THE_THUNDERBREWS) == QUEST_STATUS_INCOMPLETE
                || pPlayer->GetQuestStatus(BARK_FOR_TCHALIS_VOODOO_BREWERY) == QUEST_STATUS_INCOMPLETE
                || pPlayer->GetQuestStatus(BARK_FOR_THE_BARLEYBREWS) == QUEST_STATUS_INCOMPLETE
                || pPlayer->GetQuestStatus(BARK_FOR_DROHNS_DISTILLERY) == QUEST_STATUS_INCOMPLETE)
                pPlayer->KilledMonsterCredit(me->GetEntry(), ObjectGuid::Empty);
        }
    };

    CreatureAI *GetAI(Creature *creature) const override
    {
        return new npc_brewfest_triggerAI(creature);
    }
};

/*####
## npc_brewfest_apple_trigger
####*/


class npc_brewfest_apple_trigger : public CreatureScript
{
public:
    npc_brewfest_apple_trigger() : CreatureScript("npc_brewfest_apple_trigger") { }

    struct npc_brewfest_apple_triggerAI : public ScriptedAI
    {
        npc_brewfest_apple_triggerAI(Creature* c) : ScriptedAI(c) {}
        void MoveInLineOfSight(Unit *who) override
        {
            Player *pPlayer = who->ToPlayer();
            if (!pPlayer)
                return;
            if (pPlayer->HasAura(SPELL_RAM_FATIGUE) && me->GetDistance(pPlayer->GetPositionX(),pPlayer->GetPositionY(),pPlayer->GetPositionZ()) <= 7.5f)
                pPlayer->RemoveAura(SPELL_RAM_FATIGUE);
        }
    };

    CreatureAI *GetAI(Creature *creature) const override
    {
        return new npc_brewfest_apple_triggerAI(creature);
    }
};

/*####
## npc_brewfest_keg_thrower
####*/

enum eBrewfestKegThrower
{
    SPELL_THROW_KEG      = 43660,
    ITEM_BREWFEST_KEG    = 33797
};

class npc_brewfest_keg_thrower : public CreatureScript
{
public:
    npc_brewfest_keg_thrower() : CreatureScript("npc_brewfest_keg_thrower") { }

    struct npc_brewfest_keg_throwerAI : public ScriptedAI
    {
        npc_brewfest_keg_throwerAI(Creature* c) : ScriptedAI(c) {}
        void MoveInLineOfSight(Unit *who) override
        {
            Player *pPlayer = who->ToPlayer();
            if (!pPlayer)
                return;
            if (pPlayer->HasAura(SPELL_BREWFEST_RAM) 
                && me->GetDistance(pPlayer->GetPositionX(),pPlayer->GetPositionY(),pPlayer->GetPositionZ()) <= 25.0f
                && !pPlayer->HasItemCount(ITEM_BREWFEST_KEG,1))
            {
                me->CastSpell(pPlayer,SPELL_THROW_KEG,false);
                me->CastSpell(pPlayer,42414,false);
            }
        }
    };

    CreatureAI *GetAI(Creature *creature) const override
    {
        return new npc_brewfest_keg_throwerAI(creature);
    }
};

/*####
## npc_brewfest_keg_receiver
####*/

enum eBrewfestKegReceiver
{
    SPELL_CREATE_TICKETS            = 44501,
    QUEST_THERE_AND_BACK_AGAIN_A    = 11122,
    QUEST_THERE_AND_BACK_AGAIN_H    = 11412,
    NPC_BREWFEST_DELIVERY_BUNNY     = 24337   
};

class npc_brewfest_keg_receiver : public CreatureScript
{
public:
    npc_brewfest_keg_receiver() : CreatureScript("npc_brewfest_keg_receiver") { }

    struct npc_brewfest_keg_receiverAI : public ScriptedAI
    {
        npc_brewfest_keg_receiverAI(Creature* c) : ScriptedAI(c) {}
        void MoveInLineOfSight(Unit *who) override
        {
            Player *pPlayer = who->ToPlayer();
            if (!pPlayer)
                return;
            if (pPlayer->HasAura(SPELL_BREWFEST_RAM) 
                && me->GetDistance(pPlayer->GetPositionX(),pPlayer->GetPositionY(),pPlayer->GetPositionZ()) <= 5.0f
                && pPlayer->HasItemCount(ITEM_BREWFEST_KEG,1)) 
            {
                pPlayer->CastSpell(me,SPELL_THROW_KEG,true);
                pPlayer->DestroyItemCount(ITEM_BREWFEST_KEG,1,true);
                pPlayer->GetAura(SPELL_BREWFEST_RAM)->SetDuration(pPlayer->GetAura(SPELL_BREWFEST_RAM)->GetDuration() + 30000);
                if (pPlayer->GetQuestRewardStatus(QUEST_THERE_AND_BACK_AGAIN_A) 
                    || pPlayer->GetQuestRewardStatus(QUEST_THERE_AND_BACK_AGAIN_H))
                {
                    pPlayer->CastSpell(pPlayer,SPELL_CREATE_TICKETS,true);
                }
                else
                {
                    pPlayer->KilledMonsterCredit(NPC_BREWFEST_DELIVERY_BUNNY, ObjectGuid::Empty);
                    if (pPlayer->GetQuestStatus(QUEST_THERE_AND_BACK_AGAIN_A) == QUEST_STATUS_INCOMPLETE) 
                        pPlayer->AreaExploredOrEventHappens(QUEST_THERE_AND_BACK_AGAIN_A);
                    if (pPlayer->GetQuestStatus(QUEST_THERE_AND_BACK_AGAIN_H) == QUEST_STATUS_INCOMPLETE) 
                        pPlayer->AreaExploredOrEventHappens(QUEST_THERE_AND_BACK_AGAIN_H);
                    if (pPlayer->GetQuestStatus(QUEST_THERE_AND_BACK_AGAIN_A) == QUEST_STATUS_COMPLETE
                        || pPlayer->GetQuestStatus(QUEST_THERE_AND_BACK_AGAIN_H) == QUEST_STATUS_COMPLETE)
                        pPlayer->RemoveAura(SPELL_BREWFEST_RAM);
                }
            }
        }
    };

    CreatureAI *GetAI(Creature *creature) const override
    {
        return new npc_brewfest_keg_receiverAI(creature);
    }
};

/*####
## npc_brewfest_ram_master
####*/
#define GOSSIP_ITEM_RAM             "Do you have additional work?"
#define GOSSIP_ITEM_RAM_REINS       "Give me another Ram Racing Reins"
#define SPELL_BREWFEST_SUMMON_RAM   43720

class npc_brewfest_ram_master : public CreatureScript
{
public:
    npc_brewfest_ram_master() : CreatureScript("npc_brewfest_ram_master") { }

    bool OnGossipHello(Player *pPlayer, Creature *pCreature) override
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

            if (pPlayer->HasSpellCooldown(SPELL_BREWFEST_SUMMON_RAM) 
                && !pPlayer->GetQuestRewardStatus(QUEST_THERE_AND_BACK_AGAIN_A) 
                && !pPlayer->GetQuestRewardStatus(QUEST_THERE_AND_BACK_AGAIN_H)
                && (pPlayer->GetQuestStatus(QUEST_THERE_AND_BACK_AGAIN_A) == QUEST_STATUS_INCOMPLETE
                || pPlayer->GetQuestStatus(QUEST_THERE_AND_BACK_AGAIN_H) == QUEST_STATUS_INCOMPLETE))
                pPlayer->RemoveSpellCooldown(SPELL_BREWFEST_SUMMON_RAM);

            if (!pPlayer->HasAura(SPELL_BREWFEST_RAM) && ((pPlayer->GetQuestStatus(QUEST_THERE_AND_BACK_AGAIN_A) == QUEST_STATUS_INCOMPLETE 
            || pPlayer->GetQuestStatus(QUEST_THERE_AND_BACK_AGAIN_H) == QUEST_STATUS_INCOMPLETE 
            || (!pPlayer->HasSpellCooldown(SPELL_BREWFEST_SUMMON_RAM) && 
                (pPlayer->GetQuestRewardStatus(QUEST_THERE_AND_BACK_AGAIN_A) 
                || pPlayer->GetQuestRewardStatus(QUEST_THERE_AND_BACK_AGAIN_H))))))
            pPlayer->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_ITEM_RAM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

            if ((pPlayer->GetQuestRewardStatus(QUEST_THERE_AND_BACK_AGAIN_A) 
                || pPlayer->GetQuestRewardStatus(QUEST_THERE_AND_BACK_AGAIN_H))
                && !pPlayer->HasItemCount(33306,1,true))
            pPlayer->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_ITEM_RAM_REINS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);



        pPlayer->SEND_GOSSIP_MENU(384, pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction) override
    {
        pPlayer->PlayerTalkClass->SendCloseGossip();
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            if (pPlayer->HasItemCount(ITEM_BREWFEST_KEG,1)) 
                pPlayer->DestroyItemCount(ITEM_BREWFEST_KEG,1,true);
            pPlayer->CastSpell(pPlayer,SPELL_BREWFEST_SUMMON_RAM,true);
            pPlayer->AddSpellCooldown(SPELL_BREWFEST_SUMMON_RAM,0, getPreciseTime() + 64800.0);
        }
        if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
        {
            pPlayer->CastSpell(pPlayer,44371,false);
        }
        return true;
    }
};

enum BrewfestMusic
{
    EVENT_BREWFESTDWARF01 = 11810, // 1.35 min
    EVENT_BREWFESTDWARF02 = 11812, // 1.55 min
    EVENT_BREWFESTDWARF03 = 11813, // 0.23 min
    EVENT_BREWFESTGOBLIN01 = 11811, // 1.08 min
    EVENT_BREWFESTGOBLIN02 = 11814, // 1.33 min
    EVENT_BREWFESTGOBLIN03 = 11815 // 0.28 min
};

enum BrewfestMusicTime
{
    EVENT_BREWFESTDWARF01_TIME = 95000,
    EVENT_BREWFESTDWARF02_TIME = 155000,
    EVENT_BREWFESTDWARF03_TIME = 23000,
    EVENT_BREWFESTGOBLIN01_TIME = 68000,
    EVENT_BREWFESTGOBLIN02_TIME = 93000,
    EVENT_BREWFESTGOBLIN03_TIME = 28000
};

enum BrewfestMusicAreas
{
    SILVERMOON = 3430, // Horde
    UNDERCITY = 1497,
    ORGRIMMAR_1 = 1296,
    ORGRIMMAR_2 = 14,
    THUNDERBLUFF = 1638,
    IRONFORGE_1 = 809, // Alliance
    IRONFORGE_2 = 1,
    STORMWIND = 12,
    EXODAR = 3557,
    DARNASSUS = 1657
};

enum BrewfestMusicEvents
{
    EVENT_BM_SELECT_MUSIC = 1,
    EVENT_BM_START_MUSIC = 2
};

struct go_brewfest_music : public GameObjectAI
{
    go_brewfest_music(GameObject* go) : GameObjectAI(go)
    {
        events.ScheduleEvent(EVENT_BM_SELECT_MUSIC, 1000);
        events.ScheduleEvent(EVENT_BM_START_MUSIC, 2000);
    }

    EventMap events;
    uint32 rnd = 0;
    uint32 musicTime = 1000;

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_BM_SELECT_MUSIC:
                if (IsHolidayActive(HOLIDAY_BREWFEST))
                    rnd = urand(0, 2);

                events.ScheduleEvent(EVENT_BM_SELECT_MUSIC, musicTime);
                break;
            case EVENT_BM_START_MUSIC:
                if (IsHolidayActive(HOLIDAY_BREWFEST))
                {
                    if (go->GetAreaId() == SILVERMOON || go->GetAreaId() == UNDERCITY || go->GetAreaId() == ORGRIMMAR_1 || go->GetAreaId() == ORGRIMMAR_2 || go->GetAreaId() == THUNDERBLUFF)
                    {
                        switch (rnd)
                        {
                        case 0:
                            go->PlayDirectSound(EVENT_BREWFESTGOBLIN01);
                            musicTime = EVENT_BREWFESTGOBLIN01_TIME;
                            break;
                        case 1:
                            go->PlayDirectSound(EVENT_BREWFESTGOBLIN02);
                            musicTime = EVENT_BREWFESTGOBLIN02_TIME;
                            break;
                        case 2:
                            go->PlayDirectSound(EVENT_BREWFESTGOBLIN03);
                            musicTime = EVENT_BREWFESTGOBLIN03_TIME;
                            break;
                        }
                    }
                    if (go->GetAreaId() == IRONFORGE_1 || go->GetAreaId() == IRONFORGE_2 || go->GetAreaId() == STORMWIND || go->GetAreaId() == EXODAR || go->GetAreaId() == DARNASSUS)
                    {
                        switch (rnd)
                        {
                        case 0:
                            go->PlayDirectSound(EVENT_BREWFESTDWARF01);
                            musicTime = EVENT_BREWFESTDWARF01_TIME;
                            break;
                        case 1:
                            go->PlayDirectSound(EVENT_BREWFESTDWARF02);
                            musicTime = EVENT_BREWFESTDWARF02_TIME;
                            break;
                        case 2:
                            go->PlayDirectSound(EVENT_BREWFESTDWARF03);
                            musicTime = EVENT_BREWFESTDWARF03_TIME;
                            break;
                        }
                    }
                }
                events.ScheduleEvent(EVENT_BM_START_MUSIC, 5000);
                break;
            default:
                break;
            }
        }
    }
};

enum eBrewfestSampler
{
    QUEST_CHUG_AND_CHUCK_A    = 12022,
    QUEST_CHUG_AND_CHUCK_H    = 12191,
    NPC_BREWFEST_STOUT        = 24108
};
class item_brewfest_sampler : public ItemScript
{
public:
    item_brewfest_sampler() : ItemScript("item_brewfest_sampler") { }

    bool OnUse(Player* pPlayer, Item* pItem, const SpellCastTargets & /*pTargets*/) override
    {
        if (pPlayer->GetQuestStatus(QUEST_CHUG_AND_CHUCK_A) == QUEST_STATUS_INCOMPLETE
            || pPlayer->GetQuestStatus(QUEST_CHUG_AND_CHUCK_H) == QUEST_STATUS_INCOMPLETE)
        {
            if (Creature* pStout = pPlayer->FindNearestCreature(NPC_BREWFEST_STOUT, 10.0f)) // spell range
            {
                return false;
            } else
                pPlayer->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, pItem, NULL);
        } else
            pPlayer->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT ,pItem, NULL);
        return true;
    }
};

class item_brewfest_ram_reins : public ItemScript
{
public:
    item_brewfest_ram_reins() : ItemScript("item_brewfest_ram_reins") { }

    bool OnUse(Player* pPlayer, Item* pItem, const SpellCastTargets & /*pTargets*/) override
    {
        if (pPlayer->HasAura(SPELL_BREWFEST_RAM) && !pPlayer->HasAura(SPELL_SPEED_RAM_EXHAUSED))
        {
            if (pPlayer->HasAura(SPELL_SPEED_RAM_NORMAL))
                pPlayer->CastSpell(pPlayer,SPELL_SPEED_RAM_TROT,false);
            else if (pPlayer->HasAura(SPELL_SPEED_RAM_TROT))
            {
                if (pPlayer->GetAura(SPELL_SPEED_RAM_TROT)->GetDuration() < 3000)
                    pPlayer->GetAura(SPELL_SPEED_RAM_TROT)->SetDuration(4000);
                else
                  pPlayer->CastSpell(pPlayer,SPELL_SPEED_RAM_CANTER,false);
            } else if (pPlayer->HasAura(SPELL_SPEED_RAM_CANTER))
            {
                if (pPlayer->GetAura(SPELL_SPEED_RAM_CANTER)->GetDuration() < 3000)
                    pPlayer->GetAura(SPELL_SPEED_RAM_CANTER)->SetDuration(4000);
                else
                  pPlayer->CastSpell(pPlayer,SPELL_SPEED_RAM_GALLOP,false);
            } else if (pPlayer->HasAura(SPELL_SPEED_RAM_GALLOP))
                pPlayer->GetAura(SPELL_SPEED_RAM_GALLOP)->SetDuration(4000);
        } else
            pPlayer->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT ,pItem, NULL);
        return true;
    }
};

/*######
## at_brewfest
######*/

enum Brewfest
{
    NPC_TAPPER_SWINDLEKEG       = 24711,
    NPC_IPFELKOFER_IRONKEG      = 24710,

    AT_BREWFEST_DUROTAR         = 4829,
    AT_BREWFEST_DUN_MOROGH      = 4820,

    SAY_WELCOME                 = 4,

    AREATRIGGER_TALK_COOLDOWN   = 5, // in seconds
};

class AreaTrigger_at_brewfest : public AreaTriggerScript
{
    public:
        AreaTrigger_at_brewfest() : AreaTriggerScript("at_brewfest")
        {
            // Initialize for cooldown
            _triggerTimes[AT_BREWFEST_DUROTAR] = _triggerTimes[AT_BREWFEST_DUN_MOROGH] = 0;
        }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger, bool /*enter*/) override
        {
            uint32 triggerId = trigger->ID;
            // Second trigger happened too early after first, skip for now
            if (GameTime::GetGameTime() - _triggerTimes[triggerId] < AREATRIGGER_TALK_COOLDOWN)
                return false;

            switch (triggerId)
            {
                case AT_BREWFEST_DUROTAR:
                    if (Creature* tapper = player->FindNearestCreature(NPC_TAPPER_SWINDLEKEG, 20.0f))
                        tapper->AI()->Talk(SAY_WELCOME, player->GetGUID());
                    break;
                case AT_BREWFEST_DUN_MOROGH:
                    if (Creature* ipfelkofer = player->FindNearestCreature(NPC_IPFELKOFER_IRONKEG, 20.0f))
                        ipfelkofer->AI()->Talk(SAY_WELCOME, player->GetGUID());
                    break;
                default:
                    break;
            }

            _triggerTimes[triggerId] = GameTime::GetGameTime();
            return false;
        }

    private:
        std::map<uint32, time_t> _triggerTimes;
};

enum BrewfestMountTransformation
{
    SPELL_MOUNT_RAM_100                         = 43900,
    SPELL_MOUNT_RAM_60                          = 43899,
    SPELL_MOUNT_KODO_100                        = 49379,
    SPELL_MOUNT_KODO_60                         = 49378,
    SPELL_BREWFEST_MOUNT_TRANSFORM              = 49357,
    SPELL_BREWFEST_MOUNT_TRANSFORM_REVERSE      = 52845,
};

class spell_item_brewfest_mount_transformation : public SpellScriptLoader
{
    public:
        spell_item_brewfest_mount_transformation() : SpellScriptLoader("spell_item_brewfest_mount_transformation") { }

        class spell_item_brewfest_mount_transformation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_brewfest_mount_transformation_SpellScript);

            bool Validate(SpellInfo const* /*spell*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MOUNT_RAM_100) || !sSpellMgr->GetSpellInfo(SPELL_MOUNT_RAM_60) || !sSpellMgr->GetSpellInfo(SPELL_MOUNT_KODO_100) || !sSpellMgr->GetSpellInfo(SPELL_MOUNT_KODO_60))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Player* caster = GetCaster()->ToPlayer();
                if (caster->HasAuraType(SPELL_AURA_MOUNTED))
                {
                    caster->RemoveAurasByType(SPELL_AURA_MOUNTED);
                    uint32 spell_id;

                    switch (GetSpellInfo()->Id)
                    {
                        case SPELL_BREWFEST_MOUNT_TRANSFORM:
                            if (caster->GetSpeedRate(MOVE_RUN) >= 2.0f)
                                spell_id = caster->GetTeam() == ALLIANCE ? SPELL_MOUNT_RAM_100 : SPELL_MOUNT_KODO_100;
                            else
                                spell_id = caster->GetTeam() == ALLIANCE ? SPELL_MOUNT_RAM_60 : SPELL_MOUNT_KODO_60;
                            break;
                        case SPELL_BREWFEST_MOUNT_TRANSFORM_REVERSE:
                            if (caster->GetSpeedRate(MOVE_RUN) >= 2.0f)
                                spell_id = caster->GetTeam() == HORDE ? SPELL_MOUNT_RAM_100 : SPELL_MOUNT_KODO_100;
                            else
                                spell_id = caster->GetTeam() == HORDE ? SPELL_MOUNT_RAM_60 : SPELL_MOUNT_KODO_60;
                            break;
                        default:
                            return;
                    }
                    caster->CastSpell(caster, spell_id, true);
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_brewfest_mount_transformation_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_item_brewfest_mount_transformation_SpellScript();
        }
};

class spell_brewfest_speed : public SpellScriptLoader
{
public:
    spell_brewfest_speed() : SpellScriptLoader("spell_brewfest_speed") {}

    class spell_brewfest_speed_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_brewfest_speed_AuraScript)
        bool Validate(SpellInfo const * /*SpellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_RAM_FATIGUE))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_BREWFEST_RAM))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SPEED_RAM_GALLOP))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SPEED_RAM_CANTER))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SPEED_RAM_TROT))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SPEED_RAM_NORMAL))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SPEED_RAM_GALLOP))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SPEED_RAM_EXHAUSED))
                return false;
            return true;
        }

        void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
        {
            if (GetId() == SPELL_SPEED_RAM_EXHAUSED)
                return;
            Player* pCaster = GetCaster()->ToPlayer();
            if (!pCaster)
                return;
            int i;
            switch (GetId())
            {
                case SPELL_SPEED_RAM_GALLOP:
                    for (i = 0; i < 5; i++)
                       pCaster->AddAura(SPELL_RAM_FATIGUE,pCaster);
                    break;
                case SPELL_SPEED_RAM_CANTER:
                    pCaster->AddAura(SPELL_RAM_FATIGUE,pCaster);
                    break;
                case SPELL_SPEED_RAM_TROT:
                    if (pCaster->HasAura(SPELL_RAM_FATIGUE))
                        if (pCaster->GetAura(SPELL_RAM_FATIGUE)->GetStackAmount() <= 2)
                            pCaster->RemoveAura(SPELL_RAM_FATIGUE);
                        else
                            pCaster->GetAura(SPELL_RAM_FATIGUE)->ModStackAmount(-2);
                    break;
                case SPELL_SPEED_RAM_NORMAL:
                    if (pCaster->HasAura(SPELL_RAM_FATIGUE))
                        if (pCaster->GetAura(SPELL_RAM_FATIGUE)->GetStackAmount() <= 4)
                            pCaster->RemoveAura(SPELL_RAM_FATIGUE);
                        else
                            pCaster->GetAura(SPELL_RAM_FATIGUE)->ModStackAmount(-4);
                    break;
            }
            if (pCaster->HasAura(SPELL_RAM_FATIGUE))
                if (pCaster->GetAura(SPELL_RAM_FATIGUE)->GetStackAmount() >= 100)
                    pCaster->CastSpell(pCaster,SPELL_SPEED_RAM_EXHAUSED, false);
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* pCaster = GetCaster()->ToPlayer();
            if (!pCaster)
                return;
            if (!pCaster->HasAura(SPELL_BREWFEST_RAM))
                return;
            if (GetId() == SPELL_SPEED_RAM_EXHAUSED) 
            {
                if (pCaster->HasAura(SPELL_RAM_FATIGUE))
                    pCaster->GetAura(SPELL_RAM_FATIGUE)->ModStackAmount(-15);
            } else if (!pCaster->HasAura(SPELL_RAM_FATIGUE) || pCaster->GetAura(SPELL_RAM_FATIGUE)->GetStackAmount() < 100)
                switch (GetId())
                {
                    case SPELL_SPEED_RAM_GALLOP:
                        if (!pCaster->HasAura(SPELL_SPEED_RAM_EXHAUSED))
                            pCaster->CastSpell(pCaster,SPELL_SPEED_RAM_CANTER, false);
                        break;
                    case SPELL_SPEED_RAM_CANTER:
                        if (!pCaster->HasAura(SPELL_SPEED_RAM_GALLOP))
                            pCaster->CastSpell(pCaster,SPELL_SPEED_RAM_TROT, false);
                        break;
                    case SPELL_SPEED_RAM_TROT:
                        if (!pCaster->HasAura(SPELL_SPEED_RAM_CANTER))
                            pCaster->CastSpell(pCaster,SPELL_SPEED_RAM_NORMAL, false);
                        break;
                }
        }

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* pCaster = GetCaster()->ToPlayer();
            if (!pCaster)
                return;
            switch (GetId())
            {
                case SPELL_SPEED_RAM_GALLOP:
                    pCaster->GetAura(SPELL_SPEED_RAM_GALLOP)->SetDuration(4000);
                    break;
                case SPELL_SPEED_RAM_CANTER:
                    pCaster->GetAura(SPELL_SPEED_RAM_CANTER)->SetDuration(4000);
                    break;
                case SPELL_SPEED_RAM_TROT:
                    pCaster->GetAura(SPELL_SPEED_RAM_TROT)->SetDuration(4000);
                    break;
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_brewfest_speed_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_brewfest_speed_AuraScript::HandleEffectPeriodic,EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            OnEffectRemove += AuraEffectRemoveFn(spell_brewfest_speed_AuraScript::HandleEffectRemove, EFFECT_2, SPELL_AURA_USE_NORMAL_MOVEMENT_SPEED, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_brewfest_speed_AuraScript();
    }
};

void AddSC_brewfest()
{
    new npc_brewfest_reveler();
    new npc_brewfest_trigger();
    new npc_brewfest_apple_trigger();
    new npc_brewfest_keg_thrower();
    new npc_brewfest_keg_receiver();
    new npc_brewfest_ram_master();
    RegisterGameObjectAI(go_brewfest_music);
    new item_brewfest_sampler();
    new item_brewfest_ram_reins();
    new AreaTrigger_at_brewfest();
    new spell_item_brewfest_mount_transformation();
    new spell_brewfest_speed();
}
