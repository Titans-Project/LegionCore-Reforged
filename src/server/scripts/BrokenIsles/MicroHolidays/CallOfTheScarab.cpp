////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "Group.h"
#include "OutdoorPvP.h"
#include "Packets/WorldStatePackets.h"
#include "QuestData.h"
#include "World.h"
#include "ScriptPCH.h"

enum Misc
{
    EVENT_HORDE_FLAG            = 76,
    EVENT_ALLIANCE_FLAG         = 77,
    EVENT_CALL_OF_THE_SCARAB    = 78,
    EVENT_WINNER_TEAM           = 306,

    WS_SHOW_SCORES              = 12951,
    WS_HORDE_SCORE              = 12952,
    WS_ALLIANCE_SCORE           = 12953,

    SPELL_TIME_TRAVELLING       = 255152,   // Zidormi's marker: whoever carries it stands before the Wound
    ZONE_SILITHUS               = 1377,
    QUEST_INTRO_HORDE           = 45785,
    QUEST_INTRO_ALLIANCE        = 45787,

    SPELL_SILITHYST             = 29519,    // carried from a geyser
    SPELL_TRACES_OF_SILITHYST   = 29534,
    NPC_SILITHYST_CREDIT        = 117587,
    NPC_SILITHYST_TURNIN_HORDE    = 18199,  // the camps of the old world PvP: Hive'Regal for the Horde
    NPC_SILITHYST_TURNIN_ALLIANCE = 17090,  // Hive'Zora for the Alliance

    HOLIDAY_LEVEL               = 110,      // top of old Silithus while the holiday runs, as in January 2018
    OLD_ZONE_MAX_LEVEL          = 60
};

float const HOLIDAY_TOP_HEALTH  = 2.0f;     // against a level 110 player only
float const HOLIDAY_RARE_HEALTH = 1.5f;     // the rares of old Silithus are frailer than some common creatures

// The holiday takes place before the Wound: everyone is sent there when it starts and at each login while it
// runs, Zidormi taking back to the present whoever prefers it. Once it is over everyone returns to the present.
// The ground is only read when a map loads, hence the journey in place on Kalimdor each time the timeline changes.
namespace ScarabTimeline
{
    void Travel(Player* player, bool toThePast)
    {
        if (player->HasAura(SPELL_TIME_TRAVELLING) == toThePast)
            return;

        if (toThePast)
            player->AddAura(SPELL_TIME_TRAVELLING, player);
        else
            player->RemoveAurasDueToSpell(SPELL_TIME_TRAVELLING);

        PhaseUpdateData phaseUpdateData;
        phaseUpdateData.AddConditionType(CONDITION_AURA);
        player->GetPhaseMgr().NotifyConditionChanged(phaseUpdateData);

        // the same journey in place as Zidormi's
        if (player->GetMapId() == MAP_KALIMDOR)
            player->TeleportTo(player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(),
                               player->GetOrientation(), TELE_TO_ZONE_MAP);
    }

    // the holiday announces itself with the quest that sends the player to the gong, pushed as the game did
    void OfferIntroduction(Player* player)
    {
        uint32 questId = player->GetTeam() == HORDE ? QUEST_INTRO_HORDE : QUEST_INTRO_ALLIANCE;
        Quest const* quest = sQuestDataStore->GetQuestTemplate(questId);
        if (quest && player->GetQuestStatus(questId) == QUEST_STATUS_NONE && player->CanTakeQuest(quest, false) && player->CanAddQuest(quest, false))
            player->AddQuestAndCheckCompletion(quest, nullptr);
    }

    // each player in his own thread
    void TravelEveryone(bool toThePast)
    {
        ObjectAccessor::PostToAllPlayers([toThePast](Player* player)
        {
            Travel(player, toThePast);
            if (toThePast)
                OfferIntroduction(player);
        });
    }

    // the absent come back to the present through the database
    void ClearAbsent()
    {
        CharacterDatabase.PExecute("DELETE FROM character_aura WHERE spell = %u", SPELL_TIME_TRAVELLING);
    }
}

// While the holiday runs old Silithus follows the players up to level 110, as it did in 2018. The creatures are looked
// for around the players: a creature is created before it knows its zone, so the engine of the zone never hears of it.
namespace ScarabLevel
{
    // creatures made for old Silithus, up to level 60 (the lords of the wind stones scale by themselves);
    // critters, triggers, pets and summons of players stay out of it
    bool IsOldZoneCreature(Creature* creature)
    {
        CreatureTemplate const* cInfo = creature->GetCreatureTemplate();
        if (creature->isPet() || creature->GetOwnerGUID().IsPlayer() || creature->GetCreatureType() == CREATURE_TYPE_CRITTER
            || (cInfo->flags_extra & CREATURE_FLAG_EXTRA_TRIGGER))
            return false;

        // only what one fights: the friendly, the vendors and the invisible helpers keep their level
        FactionTemplateEntry const* faction = creature->getFactionTemplateEntry();
        if (!faction || !(faction->IsHostileToPlayers() || faction->IsNeutralToAll()) || cInfo->npcflag
            || creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE))
            return false;

        return cInfo->maxlevel <= OLD_ZONE_MAX_LEVEL;
    }

    // The core scales a creature down to the player, never up: the creature is set at 110 with the statistics of Legion
    // (its template uses their table, the same as the old one up to level 60) and the core brings it down to each player
    // within its range, whose bottom stays. The bonuses of rank and difficulty carry over, and so does its share of
    // health. Its template gives everything back when the holiday is over.
    void SetHolidayLevel(Creature* creature, bool raised)
    {
        CreatureTemplate const* cInfo = creature->GetCreatureTemplate();
        float healthPct = creature->GetHealthPct();

        if (!raised)
        {
            creature->SetSpellBaseLevel(0);
            creature->SelectLevel(cInfo);
            creature->UpdateAllStats();
            creature->SetHealth(std::max<uint64>(1, uint64(creature->GetMaxHealth() * healthPct / 100.0f)));
            return;
        }

        CreatureBaseStats const* own = sObjectMgr->GetCreatureBaseStats(creature->getLevel(), cInfo->unit_class);
        CreatureBaseStats const* holiday = sObjectMgr->GetCreatureBaseStats(HOLIDAY_LEVEL, cInfo->unit_class);
        if (!own || !holiday)
            return;

        double healthMod = double(creature->GetCreateHealth()) / std::max<uint64>(1, own->GenerateHealth(cInfo));
        float damageMod = creature->GetWeaponDamageRange(BASE_ATTACK, MINDAMAGE) / std::max(1.0f, own->GenerateBaseDamage(cInfo));

        if (cInfo->Classification == CREATURE_CLASSIFICATION_RARE)
            healthMod *= HOLIDAY_RARE_HEALTH;

        uint8 const madeFor = creature->getLevel();
        creature->SetLevel(HOLIDAY_LEVEL);
        creature->SetUInt32Value(UNIT_FIELD_SCALING_LEVEL_MAX, HOLIDAY_LEVEL);
        if (!creature->HasScalableLevels())      // nothing to scale it with: it stands at the top
            creature->SetUInt32Value(UNIT_FIELD_SCALING_LEVEL_MIN, HOLIDAY_LEVEL);

        uint64 health = uint64(holiday->BaseHealth[EXPANSION_LEGION] * cInfo->HpMulti * healthMod * HOLIDAY_TOP_HEALTH);
        creature->SetCreateHealth(uint32(health));
        creature->SetMaxHealth(health);
        creature->SetModifierValue(UNIT_MOD_HEALTH, BASE_VALUE, float(health));

        float damage = holiday->BaseDamage[EXPANSION_LEGION] * damageMod;

        // the core reads the stats of a scaled creature level by level, from its template's range only: the levels up
        // to the holiday's are added, with the health of Legion, and the template's own ones take the rares' bonus
        if (creature->HasScalableLevels())
        {
            uint8 const templateMax = cInfo->levelScaling->MaxLevel;
            if (creature->m_levelStat.size() <= HOLIDAY_LEVEL)
                creature->m_levelStat.resize(HOLIDAY_LEVEL + 1);

            for (uint8 level = cInfo->levelScaling->MinLevel; level <= HOLIDAY_LEVEL; ++level)
            {
                CreatureLevelStat& stat = creature->m_levelStat[level];
                if (level <= templateMax)
                {
                    if (cInfo->Classification == CREATURE_CLASSIFICATION_RARE)
                        stat.healthMax = uint64(stat.healthMax * HOLIDAY_RARE_HEALTH);
                    continue;
                }

                CreatureBaseStats const* stats = sObjectMgr->GetCreatureBaseStats(level, cInfo->unit_class);
                stat.baseHP = uint32(stats->BaseHealth[EXPANSION_LEGION] * cInfo->HpMulti);
                stat.baseMP = stats->GenerateMana(cInfo);
                stat.healthMax = uint64(stat.baseHP * healthMod);
                stat.baseMinDamage = stats->BaseDamage[EXPANSION_LEGION] * damageMod;
                stat.baseMaxDamage = stat.baseMinDamage * 1.5f;
                stat.AttackPower = stats->AttackPower;
                stat.RangedAttackPower = stats->RangedAttackPower;
                stat.BaseArmor = stats->GenerateArmor(cInfo);
            }

            CreatureLevelStat& top = creature->m_levelStat[HOLIDAY_LEVEL];
            top.healthMax = health;
            top.baseMinDamage = damage;
            top.baseMaxDamage = damage * 1.5f;

            // a creature drawn a level below its target faces a level 110 player at 109: the doubled health goes with it
            int32 const seenByTop = std::max<int32>(cInfo->levelScaling->MinLevel,
                std::min<int32>(HOLIDAY_LEVEL, HOLIDAY_LEVEL + creature->GetInt32Value(UNIT_FIELD_SCALING_LEVEL_DELTA)));
            if (seenByTop != HOLIDAY_LEVEL)
                creature->m_levelStat[seenByTop].healthMax = uint64(creature->m_levelStat[seenByTop].healthMax * HOLIDAY_TOP_HEALTH);
        }

        // the spells keep their flat values of old: they follow the melee damage, target by target
        creature->SetSpellBaseLevel(madeFor);
        for (WeaponAttackType attack : { BASE_ATTACK, OFF_ATTACK, RANGED_ATTACK })
        {
            creature->SetBaseWeaponDamage(attack, MINDAMAGE, damage);
            creature->SetBaseWeaponDamage(attack, MAXDAMAGE, damage * 1.5f);
        }

        creature->SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, float(holiday->AttackPower));
        creature->SetModifierValue(UNIT_MOD_ATTACK_POWER_RANGED, BASE_VALUE, float(holiday->RangedAttackPower));
        creature->UpdateAllStats();
        creature->SetHealth(std::max<uint64>(1, uint64(creature->GetMaxHealth() * healthPct / 100.0f)));
    }
}

class OutdoorPvPSilithus : public OutdoorPvP
{
public:
    OutdoorPvPSilithus()
    {
        m_TypeId = OUTDOOR_PVP_SILITHUS;
    }

    ~OutdoorPvPSilithus() = default;

    bool SetupOutdoorPvP() override
    {
        for (auto zone : { 5695, 1377 })
            RegisterZone(zone);

        // the engine is loaded after the game events: scores left by a holiday that ended while the server was down are settled here
        m_holidayRunning = sGameEventMgr->IsActiveEvent(EVENT_CALL_OF_THE_SCARAB);
        if (!m_holidayRunning)
        {
            // scores still standing: the holiday ended while the server was down, its players are still in the past
            if (sWorld->getWorldState(WS_SCORE_CALL_OF_THE_SCARAB_ALLINCE) || sWorld->getWorldState(WS_SCORE_CALL_OF_THE_SCARAB_HORDE))
                ScarabTimeline::ClearAbsent();

            ChooseWinner();

            // the world quests are saved with their timer: those of a holiday stopped by hand outlive a restart
            for (uint32 questId : { 45651, 45654, 45655, 45656, 45657, 45674, 45675, 45736, 45737, 45738, 45739, 45740, 45741, 45742 })
                if (Quest const* quest = sQuestDataStore->GetQuestTemplate(questId))
                    if (sQuestDataStore->GetWorldQuest(quest))
                        sQuestDataStore->ResetWorldQuest(questId);
        }

        return true;
    }

    void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet) override
    {
        bool active = sGameEventMgr->IsActiveEvent(EVENT_CALL_OF_THE_SCARAB);
        packet.Worldstates.emplace_back(static_cast<WorldStates>(WS_SHOW_SCORES), active ? 1 : 0);
        packet.Worldstates.emplace_back(static_cast<WorldStates>(WS_HORDE_SCORE), active ? sWorld->getWorldState(WS_SCORE_CALL_OF_THE_SCARAB_HORDE) : 0);
        packet.Worldstates.emplace_back(static_cast<WorldStates>(WS_ALLIANCE_SCORE), active ? sWorld->getWorldState(WS_SCORE_CALL_OF_THE_SCARAB_ALLINCE) : 0);
    }

    void HandlePlayerEnterZone(ObjectGuid guid, uint32 zone) override
    {
        OutdoorPvP::HandlePlayerEnterZone(guid, zone);

        if (Player* player = ObjectAccessor::GetObjectInMap(guid, m_map, (Player*)nullptr))
            SendScores(player);
    }

    void HandlePlayerLeaveZone(ObjectGuid guid, uint32 zone) override
    {
        OutdoorPvP::HandlePlayerLeaveZone(guid, zone);

        if (Player* player = ObjectAccessor::GetObjectInMap(guid, m_map, (Player*)nullptr))
            player->SendUpdateWorldState(WS_SHOW_SCORES, 0);
    }

    bool Update(uint32 diff) override
    {
        if (m_updateTimer > diff)
        {
            m_updateTimer -= diff;
            return true;
        }

        m_updateTimer = 2 * IN_MILLISECONDS;

        bool active = sGameEventMgr->IsActiveEvent(EVENT_CALL_OF_THE_SCARAB);
        bool changed = active != m_holidayRunning;
        if (changed)
        {
            m_holidayRunning = active;
            ScarabTimeline::TravelEveryone(active);
            if (!active)
                ScarabTimeline::ClearAbsent();
        }

        // a respawn rebuilds a creature from its template, hence the search on every pass while the holiday runs
        if (active)
        {
            for (GuidSet const& team : m_players)
                for (ObjectGuid const& guid : team)
                {
                    Player* player = ObjectAccessor::GetObjectInMap(guid, m_map, (Player*)nullptr);
                    if (!player || player->GetZoneId() != ZONE_SILITHUS)
                        continue;

                    // the silithyst flags its carrier for PvP and is handed in at the camp of one's faction, the whole
                    // group being credited; the area triggers of the old turn-in are gone from the client
                    if (player->HasAura(SPELL_SILITHYST))
                    {
                        player->UpdatePvP(true);
                        if (Creature* turnIn = player->FindNearestCreature(player->GetTeam() == HORDE ? NPC_SILITHYST_TURNIN_HORDE : NPC_SILITHYST_TURNIN_ALLIANCE, 15.0f))
                        {
                            player->RemoveAurasDueToSpell(SPELL_SILITHYST);
                            player->CastSpell(player, SPELL_TRACES_OF_SILITHYST, true);
                            if (Group* group = player->GetGroup())
                                group->ForEachMemberInMap(m_map, [turnIn](Player* member)
                                {
                                    if (member->IsAtGroupRewardDistance(turnIn))
                                        member->KilledMonsterCredit(NPC_SILITHYST_CREDIT);
                                });
                            else
                                player->KilledMonsterCredit(NPC_SILITHYST_CREDIT);
                        }
                    }

                    std::list<Creature*> around;
                    player->GetCreatureListInGrid(around, 250.0f);
                    for (Creature* creature : around)
                        if (creature->GetZoneId() == ZONE_SILITHUS && creature->IsAlive() && creature->getLevel() != HOLIDAY_LEVEL
                            && ScarabLevel::IsOldZoneCreature(creature))
                        {
                            ScarabLevel::SetHolidayLevel(creature, true);
                            m_raised.insert(creature->GetGUID());
                        }
                }
        }
        else if (changed)
        {
            for (ObjectGuid const& guid : m_raised)
                if (Creature* creature = m_map->GetCreature(guid))
                    if (creature->IsAlive() && creature->getLevel() == HOLIDAY_LEVEL)
                        ScarabLevel::SetHolidayLevel(creature, false);
            m_raised.clear();
        }

        uint32 alliance = active ? sWorld->getWorldState(WS_SCORE_CALL_OF_THE_SCARAB_ALLINCE) : 0;
        uint32 horde = active ? sWorld->getWorldState(WS_SCORE_CALL_OF_THE_SCARAB_HORDE) : 0;
        if (active == m_shownActive && alliance == m_shownAlliance && horde == m_shownHorde)
            return true;

        m_shownActive = active;
        m_shownAlliance = alliance;
        m_shownHorde = horde;
        ApplyOnEveryPlayerInZone([this](Player* player) { SendScores(player); });
        return true;
    }

    // called from the thread of the game events, which alone may start and stop the flags
    void HandleGameEventStart(uint32 eventId) override
    {
        if (eventId == EVENT_WINNER_TEAM)
            ChooseWinner();
    }

private:
    void SendScores(Player* player) const
    {
        bool active = sGameEventMgr->IsActiveEvent(EVENT_CALL_OF_THE_SCARAB);
        player->SendUpdateWorldState(WS_SHOW_SCORES, active ? 1 : 0);
        player->SendUpdateWorldState(WS_HORDE_SCORE, active ? sWorld->getWorldState(WS_SCORE_CALL_OF_THE_SCARAB_HORDE) : 0);
        player->SendUpdateWorldState(WS_ALLIANCE_SCORE, active ? sWorld->getWorldState(WS_SCORE_CALL_OF_THE_SCARAB_ALLINCE) : 0);
    }

    // the winning faction keeps its flag by the gong until the next holiday; a tie leaves the flags as they stand
    static void ChooseWinner()
    {
        uint32 alliance = sWorld->getWorldState(WS_SCORE_CALL_OF_THE_SCARAB_ALLINCE);
        uint32 horde = sWorld->getWorldState(WS_SCORE_CALL_OF_THE_SCARAB_HORDE);
        if (alliance != horde)
        {
            uint16 winnerFlag = alliance > horde ? EVENT_ALLIANCE_FLAG : EVENT_HORDE_FLAG;
            uint16 loserFlag = alliance > horde ? EVENT_HORDE_FLAG : EVENT_ALLIANCE_FLAG;

            // a start equal to the end keeps the losing flag down, even across a restart
            WorldDatabase.PExecute("UPDATE game_event SET start_time = end_time WHERE eventEntry = %u", loserFlag);
            WorldDatabase.PExecute("UPDATE game_event SET start_time = NOW() WHERE eventEntry = %u", winnerFlag);

            if (sGameEventMgr->IsActiveEvent(loserFlag))
                sGameEventMgr->StopEvent(loserFlag, true);

            if (sGameEventMgr->IsActiveEvent(winnerFlag))
                sGameEventMgr->StopEvent(winnerFlag);
            sGameEventMgr->StartEvent(winnerFlag, true);
        }

        sWorld->setWorldState(WS_SCORE_CALL_OF_THE_SCARAB_ALLINCE, 0);
        sWorld->setWorldState(WS_SCORE_CALL_OF_THE_SCARAB_HORDE, 0);
        CharacterDatabase.PExecute("DELETE FROM character_currency WHERE currency IN (1325, 1324)");
        // the introduction and the trophies of the dukes and lords can be done again at the next holiday
        CharacterDatabase.PExecute("DELETE FROM character_queststatus_rewarded WHERE quest IN (45785, 45787) OR quest BETWEEN 45747 AND 45762");
        CharacterDatabase.PExecute("DELETE FROM character_queststatus WHERE quest IN (45785, 45787) OR quest BETWEEN 45747 AND 45762");
    }

    uint32 m_updateTimer = 0;
    bool m_holidayRunning = false;
    GuidSet m_raised;
    bool m_shownActive = false;
    uint32 m_shownAlliance = 0;
    uint32 m_shownHorde = 0;
};

class player_call_of_the_scarab_timeline : public PlayerScript
{
public:
    player_call_of_the_scarab_timeline() : PlayerScript("player_call_of_the_scarab_timeline") {}

    void OnLogin(Player* player) override
    {
        if (sGameEventMgr->IsActiveEvent(EVENT_CALL_OF_THE_SCARAB))
        {
            ScarabTimeline::Travel(player, true);
            ScarabTimeline::OfferIntroduction(player);
        }
    }
};

class OutdoorPvP_Silithus : public OutdoorPvPScript
{
public:
    OutdoorPvP_Silithus() : OutdoorPvPScript("outdoorpvp_silithus") {}

    OutdoorPvP* GetOutdoorPvP() const override
    {
        return new OutdoorPvPSilithus();
    }
};

enum Creatures
{
    NPC_TEMPLAR_FIRE = 15209,
    NPC_TEMPLAR_WATER = 15211,
    NPC_TEMPLAR_AIR = 15212,
    NPC_TEMPLAR_EARTH = 15307
};

enum WindStoneItems
{
    ITEM_ABYSSAL_CREST                  = 143865,
    ITEM_RING_OF_UNSPOKEN_NAMES         = 143866,
    ITEM_MEDALLION_OF_TRANSMISSION      = 143867
};

class go_wind_stone : public GameObjectScript
{
public:
    go_wind_stone() : GameObjectScript("go_wind_stone") {}

    void SummonNPC(GameObject* go, Player* player, uint32 npc/*, uint32 spell*/)
    {
        //go->CastSpell(player, spell);
        TempSummon* summons = go->SummonCreature(npc, go->GetPositionX(), go->GetPositionY(), go->GetPositionZ(), player->GetOrientation() - M_PI, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 10 * 60 * 1000);
        if (!summons)
            return;

        // the lords scale with the player by themselves; their spells follow from the level they were made for
        if (CreatureTemplate const* cInfo = summons->GetCreatureTemplate())
            if (cInfo->levelScaling.has_value())
                summons->SetSpellBaseLevel(uint8(cInfo->levelScaling->MinLevel));

        summons->CastSpell(summons, 25035, false);
        summons->AI()->Talk(0, player->GetGUID());
        summons->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        summons->SendMeleeAttackStart(player);
        summons->CombatStart(player);
    }

public:
    static bool IsTwilightCultist(Player* player)
    {
        return player->HasAura(234185) && player->HasAura(234184) && player->HasAura(234182);
    }

    bool OnGossipHello(Player* player, GameObject* go) override
    {
        if (!IsTwilightCultist(player))
        {
            go->CastSpell(player, 24803);
            return true;
        }

        return false;
    }

    bool OnGossipSelect(Player* player, GameObject* go, uint32 /*sender*/, uint32 action) override
    {
        if (!IsTwilightCultist(player))
        {
            go->CastSpell(player, 24803);
            player->CLOSE_GOSSIP_MENU();
            return true;
        }

        uint32 c_aura[3] = { 234185, 234184, 234182 };
        uint32 c_templar[4] = { NPC_TEMPLAR_FIRE, NPC_TEMPLAR_AIR, NPC_TEMPLAR_EARTH, NPC_TEMPLAR_WATER };
        uint32 c_duke[4] = { 117663, 117665, 117662, 117664 };
        uint32 c_royal[4] = { 117666, 117667, 117670, 117672 };

        switch (go->GetEntry())
        {
        case 180529:
        case 180456:
        case 180518:
        case 180544:
        case 180549:
        case 180564:
            player->RemoveAura(c_aura[urand(0, 2)]);
            SummonNPC(go, player, c_templar[urand(0, 3)]/*, 24745*/);
            break;
        case 180534:
        case 180461:
        case 180554:
            // the gossip option already asks for it, the choice sent back is checked again
            if (!player->HasItemCount(ITEM_MEDALLION_OF_TRANSMISSION))
            {
                go->CastSpell(player, 24803);
                break;
            }
            player->RemoveAura(c_aura[urand(0, 2)]);
            SummonNPC(go, player, c_duke[urand(0, 3)]/*, 24762*/);
            break;
        case 180466:
        case 180539:
        case 180559:
            if (!player->HasItemCount(ITEM_RING_OF_UNSPOKEN_NAMES) || !player->HasItemCount(ITEM_MEDALLION_OF_TRANSMISSION))
            {
                go->CastSpell(player, 24803);
                break;
            }
            player->RemoveAura(c_aura[urand(0, 2)]);
            SummonNPC(go, player, c_royal[urand(0, 3)]/*, 24785*/);
            break;
        default:
            break;
        }

        player->CLOSE_GOSSIP_MENU();
        return true;
    }
};

// 15209, 15211, 15212, 15307
// The templar of a lesser wind stone hands an Abyssal Crest to every member of the group that defeats it.
struct npc_wind_stone_templar : public ScriptedAI
{
    npc_wind_stone_templar(Creature* creature) : ScriptedAI(creature) {}

    void JustDied(Unit* /*killer*/) override
    {
        if (Group* group = me->GetLootRecipientGroup())
        {
            group->ForEachMemberInMap(me->GetMap(), [this](Player* member)
            {
                if (member->IsAtGroupRewardDistance(me))
                    member->AddItem(ITEM_ABYSSAL_CREST, 1);
            });
        }
        else if (Player* player = me->GetLootRecipient())
            player->AddItem(ITEM_ABYSSAL_CREST, 1);
    }
};

//117491,117490,117489
struct npc_sillithis_colossus : public ScriptedAI
{
    npc_sillithis_colossus(Creature* creature) : ScriptedAI(creature) {}

    uint32 cast;

    void Reset() override
    {
        cast = 0;
    }

    void EnterCombat(Unit* /*killer*/) override
    {
        cast = 50000;
    }

    void EnterEvadeMode() override
    {
        ScriptedAI::EnterEvadeMode();
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        if (cast)
        {
            if (cast <= diff)
            {
                ZoneTalk(0);
                DoCast(26167);

                cast = 57000;
            }
            else
                cast -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

//29519
class spell_silithyst : public AuraScript
{
    PrepareAuraScript(spell_silithyst);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_INTERRUPT)
            caster->CastSpell(caster, 29533, true);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_silithyst::OnRemove, EFFECT_0, SPELL_AURA_EFFECT_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_CallOfTheScarab()
{
    new OutdoorPvP_Silithus();
    new player_call_of_the_scarab_timeline();
    new go_wind_stone();
    RegisterCreatureAI(npc_sillithis_colossus);
    RegisterCreatureAI(npc_wind_stone_templar);
    RegisterAuraScript(spell_silithyst);
}