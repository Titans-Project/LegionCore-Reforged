////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Hunter (Beast Mastery) artifact: Titanstrike.
    Scenario 1068 "Thunder of the Titans" (map 1495) and the Mimiron fusion in Ulduar (map 1579).
    The story is driven by SmartAI data; the scripts below only fix what the data cannot express.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SmartAI.h"
#include "AreaTriggerAI.h"
#include "Player.h"
#include "Scenario.h"
#include "ScenarioMgr.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "TemporarySummon.h"
#include "Conversation.h"
#include "ConditionMgr.h"
#include "GameObject.h"

namespace Titanstrike
{
    enum Ids : uint32
    {
        SCENARIO_THUNDER_OF_THE_TITANS  = 1068,
        MAP_SCENARIO                    = 1495,
        MAP_ULDUAR_WORKSHOP             = 1579,

        STEP_TOMB_RAIDER                = 1,

        NPC_PRUSTAGA                    = 104949,
        NPC_XR947                       = 105016,
        NPC_MIMIRON_HOLOGRAM            = 106376,
        NPC_MIMIRON                     = 106558,
        NPC_FUSION_CORE                 = 106613,
        NPC_FUSION_CORE_BUNNY           = 106925,

        GO_PANEL_A                      = 249765,
        GO_PANEL_B                      = 249773,

        DATA_FUSION_READY               = 0x735002,

        // SmartAI data ids of the native scripts
        DATA_RITUAL_START               = 2,
        DATA_GUARDS_DOWN                = 3,
        DATA_TRACKERS_DOWN              = 5,
        DATA_XR_BOOT                    = 1,
        DATA_XR_FUNCTIONAL              = 2,

        CONVERSATION_MIMIRON_LINES      = 9002055,
        // the first Mimiron line of the workshop conversation comes this long after the "functional" line
        MIMIRON_LINES_DELAY             = 14150
    };

    // SetGUID ids private to this file (no native SmartAI event uses them)
    enum Actions : int32
    {
        ACTION_CORE_ACTIVATED           = 0x735001,
        ACTION_FOE_DIED                 = 0x735003
    };

    bool IsThunderOfTheTitans(WorldObject const* object)
    {
        if (object->GetMapId() != MAP_SCENARIO)
            return false;

        Scenario* progress = sScenarioMgr->GetScenario(object->GetInstanceId());
        return progress && progress->GetScenarioId() == SCENARIO_THUNDER_OF_THE_TITANS;
    }
}

// Enemies of the rune ritual and of the XR9-47 workshop. The native death events only count a kill when no other
// creature of the entry stands near the killer, which loses the wave when the killing blow comes from range, from a
// pet or from a damage over time. The enemy reports its own death to whoever tracks the fight.
struct npc_titanstrike_foe : public SmartAI
{
    explicit npc_titanstrike_foe(Creature* creature) : SmartAI(creature) { }

    void JustDied(Unit* killer) override
    {
        SmartAI::JustDied(killer);

        Creature* tracker = nullptr;
        if (TempSummon* summon = me->ToTempSummon())
        {
            if (Unit* summoner = summon->GetSummoner())
                tracker = summoner->ToCreature();
        }
        else
            tracker = me->FindNearestCreature(Titanstrike::NPC_XR947, 200.0f, true);

        if (tracker && tracker->AI())
            tracker->AI()->SetGUID(me->GetGUID(), Titanstrike::ACTION_FOE_DIED);
    }
};

// Prustaga runs the rune ritual: three waves (2 tombguards, 3 worgs, 2 trackers) that each end with a numbered
// SmartAI data message which starts the next wave and removes a rune. The wave is over when its fixed number of
// enemies is dead, whoever killed them.
struct npc_titanstrike_prustaga : public SmartAI
{
    explicit npc_titanstrike_prustaga(Creature* creature) : SmartAI(creature) { }

    uint8 wave = 0;
    uint8 killed = 0;

    static uint8 WaveSize(uint8 index)
    {
        static uint8 const sizes[3] = { 2, 3, 2 };
        return sizes[index - 1];
    }

    void SetData(uint32 id, uint32 value) override
    {
        if (Titanstrike::IsThunderOfTheTitans(me))
        {
            if (id == Titanstrike::DATA_RITUAL_START && value == Titanstrike::DATA_RITUAL_START)
            {
                wave = 1;
                killed = 0;
            }
            // The native enemy death events, which depend on where the killer stands, never advance a wave
            else if (id >= Titanstrike::DATA_GUARDS_DOWN && id <= Titanstrike::DATA_TRACKERS_DOWN && id == value)
                return;
        }

        SmartAI::SetData(id, value);
    }

    void SetGUID(ObjectGuid const& guid, int32 id) override
    {
        if (id != Titanstrike::ACTION_FOE_DIED)
        {
            SmartAI::SetGUID(guid, id);
            return;
        }

        if (!wave || ++killed < WaveSize(wave))
            return;

        uint32 const message = Titanstrike::DATA_GUARDS_DOWN + (wave - 1);
        wave = wave < 3 ? wave + 1 : 0;
        killed = 0;
        SmartAI::SetData(message, message);
    }
};

// XR9-47 wakes three constructs and becomes functional when all of them are dead. The native timer asked for no
// construct within 30 yards of the relay, which is true while the farthest one is still fighting.
struct npc_titanstrike_xr947 : public SmartAI
{
    explicit npc_titanstrike_xr947(Creature* creature) : SmartAI(creature) { }

    GuidSet constructs;
    bool booted = false;
    bool functional = false;
    uint32 functionalAt = 0;

    void SetData(uint32 id, uint32 value) override
    {
        SmartAI::SetData(id, value);

        if (id != Titanstrike::DATA_XR_BOOT || value != 1 || booted || !Titanstrike::IsThunderOfTheTitans(me))
            return;

        booted = true;
        std::list<Creature*> found;
        for (uint32 entry : { 106352u, 106354u })
            me->GetCreatureListWithEntryInGridAppend(found, entry, 200.0f);

        for (Creature* construct : found)
            if (construct->IsAlive() && me->InSamePhaseId(construct))
                constructs.insert(construct->GetGUID());

        if (constructs.empty())
            BecomeFunctional();
    }

    void SetGUID(ObjectGuid const& guid, int32 id) override
    {
        if (id != Titanstrike::ACTION_FOE_DIED)
        {
            SmartAI::SetGUID(guid, id);
            return;
        }

        if (booted && !functional && constructs.erase(guid) && constructs.empty())
            BecomeFunctional();
    }

    void BecomeFunctional()
    {
        functional = true;
        functionalAt = getMSTime();
        SmartAI::SetData(Titanstrike::DATA_XR_FUNCTIONAL, Titanstrike::DATA_XR_FUNCTIONAL);
    }

    // Mimiron's lines are a conversation of their own: his hologram does not exist when XR9-47 speaks, and a
    // conversation whose actor is missing is never created.
    void JustSummoned(Creature* summon) override
    {
        SmartAI::JustSummoned(summon);

        if (summon->GetEntry() != Titanstrike::NPC_MIMIRON_HOLOGRAM)
            return;

        uint32 const elapsed = functional ? getMSTime() - functionalAt : 0;
        uint32 const delay = elapsed < Titanstrike::MIMIRON_LINES_DELAY ? Titanstrike::MIMIRON_LINES_DELAY - elapsed : 100;
        summon->AddDelayedEvent(delay, [summon]()
        {
            Conversation* conversation = new Conversation;
            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(),
                Titanstrike::CONVERSATION_MIMIRON_LINES, summon, nullptr, *summon))
                delete conversation;
        });
    }
};

// Spirit Winds: the floor of the tomb corridor pushes players back while the guardians live. Two overlapping boxes
// share the corridor, so a box only removes the force it applied itself and the other one retries.
struct at_titanstrike_spirit_winds : public AreaTriggerAI
{
    explicit at_titanstrike_spirit_winds(AreaTrigger* trigger) : AreaTriggerAI(trigger) { }

    bool Active() const
    {
        if (!Titanstrike::IsThunderOfTheTitans(at))
            return false;

        Scenario* progress = sScenarioMgr->GetScenario(at->GetInstanceId());
        return progress->GetCurrentStep() == Titanstrike::STEP_TOMB_RAIDER;
    }

    // Each emitter publishes the visual of the whole corridor; the one nearer the entrance would draw a second
    // copy over the entrance stairs, so only the inner emitter keeps its visual.
    void CalculateDuration(int32& /*duration*/) override
    {
        if (!Titanstrike::IsThunderOfTheTitans(at))
            return;

        if (at->GetPositionY() < 310.0f)
        {
            at->SetUInt32Value(AREATRIGGER_FIELD_SPELL_FOR_VISUALS, 0);
            at->SetUInt32Value(AREATRIGGER_FIELD_SPELL_XSPELL_VISUAL_ID, 0);
        }
    }

    bool CreateUnitList(std::list<Unit*>& units) override
    {
        for (auto const& ref : at->GetMap()->GetPlayers())
            if (Player* player = ref.getSource())
                if (player->IsAlive() && at->InSamePhaseId(player))
                    units.push_back(player);

        return true; // the oriented box is still checked by the core
    }

    void Release(Unit* unit)
    {
        if (Player* player = unit->ToPlayer())
            if (player->GetForceGUID() == at->GetGUID())
                player->SendMovementForce(at);
    }

    void Apply(Unit* unit)
    {
        Player* player = unit->ToPlayer();
        if (!player)
            return;

        if (!Active() || !player->IsAlive() || player->GetVehicle() || player->isInFlight() ||
            player->HasAuraType(SPELL_AURA_DISABLE_MOVEMENT_FORCE))
        {
            Release(player);
            return;
        }

        if (player->GetForceGUID().IsEmpty())
        {
            AreaTriggerInfo const& data = at->GetAreaTriggerInfo();
            player->SendMovementForce(at, Position(data.windX, data.windY, data.windZ), data.windSpeed, data.windType, true);
        }
    }

    void OnUnitEnter(Unit* unit) override { Apply(unit); }
    void OnUnitExit(Unit* unit) override { Release(unit); }
    void BeforeRemove(Unit* unit) override { Release(unit); }

    void OnUpdate(uint32 /*diff*/) override
    {
        // a player who left an overlapping box or a vehicle gets the force of the box he still stands in
        for (ObjectGuid const& guid : *at->GetAffectedPlayers())
            if (Unit* unit = ObjectAccessor::GetUnit(*at, guid))
                Apply(unit);
    }
};

// Mimiron's fusion: the quest credit comes only after both panels were used on the two cores he summoned. The
// native check (no core of the other panel nearby) also passes when a panel is used before the cores exist.
namespace TitanstrikeFusion
{
    bool Scoped(Creature const* creature)
    {
        return creature && creature->IsInWorld() && creature->IsAlive() && creature->GetMapId() == Titanstrike::MAP_ULDUAR_WORKSHOP;
    }

    int Slot(uint32 entry)
    {
        return entry == Titanstrike::NPC_FUSION_CORE ? 0 : entry == Titanstrike::NPC_FUSION_CORE_BUNNY ? 1 : -1;
    }
}

struct npc_titanstrike_fusion_mimiron : public SmartAI
{
    explicit npc_titanstrike_fusion_mimiron(Creature* creature) : SmartAI(creature) { }

    ObjectGuid cores[2];
    uint8 receipts = 0;
    bool active = false;
    bool released = false;

    void Clear()
    {
        cores[0].Clear();
        cores[1].Clear();
        receipts = 0;
        active = false;
        released = false;
    }

    void Reset() override
    {
        Clear();
        SmartAI::Reset();
    }

    void sGossipSelect(Player* player, uint32 sender, uint32 action) override
    {
        // "Begin the fusion" (menu 19589, option 0)
        if (sender == 19589 && action == 0 && player && player->GetQuestStatus(42158) == QUEST_STATUS_INCOMPLETE)
        {
            Clear();
            active = true;
        }

        SmartAI::sGossipSelect(player, sender, action);
    }

    void JustSummoned(Creature* summon) override
    {
        if (active)
            if (int slot = TitanstrikeFusion::Slot(summon->GetEntry()); slot >= 0)
                cores[slot] = summon->GetGUID();

        SmartAI::JustSummoned(summon);
    }

    void SetGUID(ObjectGuid const& guid, int32 id) override
    {
        if (id == Titanstrike::ACTION_CORE_ACTIVATED && active && !released)
            for (int slot = 0; slot < 2; ++slot)
                if (cores[slot] == guid)
                    receipts |= uint8(1 << slot);

        SmartAI::SetGUID(guid, id);
    }

    uint32 GetData(uint32 id) const override
    {
        if (id == Titanstrike::DATA_FUSION_READY)
            return active && !released && receipts == 3;

        return SmartAI::GetData(id);
    }

    void SetData(uint32 id, uint32 value) override
    {
        // the panels' hand-off to Mimiron
        if (id == 1 && value == 1)
        {
            if (!GetData(Titanstrike::DATA_FUSION_READY))
                return;

            released = true;
        }

        SmartAI::SetData(id, value);
    }
};

struct npc_titanstrike_fusion_core : public SmartAI
{
    explicit npc_titanstrike_fusion_core(Creature* creature) : SmartAI(creature) { }

    void SetData(uint32 id, uint32 value) override
    {
        // a panel was used: tell the summoner that this core took the charge
        if (id == 1 && value == 1)
            if (TempSummon* summon = me->ToTempSummon())
                if (Creature* owner = summon->GetSummoner() ? summon->GetSummoner()->ToCreature() : nullptr)
                    if (owner->GetEntry() == Titanstrike::NPC_MIMIRON && owner->AI())
                        owner->AI()->SetGUID(me->GetGUID(), Titanstrike::ACTION_CORE_ACTIVATED);

        SmartAI::SetData(id, value);
    }
};

class condition_titanstrike_fusion_ready : public ConditionScript
{
public:
    condition_titanstrike_fusion_ready() : ConditionScript("condition_titanstrike_fusion_ready") { }

    bool OnConditionCheck(Condition* /*condition*/, ConditionSourceInfo& sourceInfo) override
    {
        GameObject* panel = sourceInfo.mConditionTargets[1] ? sourceInfo.mConditionTargets[1]->ToGameObject() : nullptr;
        if (!panel || panel->GetMapId() != Titanstrike::MAP_ULDUAR_WORKSHOP ||
            (panel->GetEntry() != Titanstrike::GO_PANEL_A && panel->GetEntry() != Titanstrike::GO_PANEL_B))
            return false;

        Creature* mimiron = panel->FindNearestCreature(Titanstrike::NPC_MIMIRON, 50.0f, true);
        return TitanstrikeFusion::Scoped(mimiron) && mimiron->AI() && mimiron->AI()->GetData(Titanstrike::DATA_FUSION_READY);
    }
};

void AddSC_Titanstrike()
{
    RegisterCreatureAI(npc_titanstrike_foe);
    RegisterCreatureAI(npc_titanstrike_prustaga);
    RegisterCreatureAI(npc_titanstrike_xr947);
    RegisterAreaTriggerAI(at_titanstrike_spirit_winds);
    RegisterCreatureAI(npc_titanstrike_fusion_mimiron);
    RegisterCreatureAI(npc_titanstrike_fusion_core);
    new condition_titanstrike_fusion_ready();
}
