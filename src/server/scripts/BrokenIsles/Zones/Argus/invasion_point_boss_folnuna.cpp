////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Matron Folnuna, Greater Invasion Point: the boss, her Infected Claws, Nausea and Slumbering Gasp
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "invasion_point_bosses.h"

uint32 RANDOM_GROTESQUE_SPAWN[2] =
{
    SPELL_GROTESQUE_GREEN,
    SPELL_GROTESQUE_RED
};

struct boss_matron_folnuna : public ScriptedAI
{
    boss_matron_folnuna(Creature* creature) : ScriptedAI(creature)
    {
        SetCombatMovement(false);
    }

    EventMap events;
    uint32 checktimer;
    ObjectGuid tankGUID;
    uint8 eventPhase;
    bool slimbering;

    void Reset() override
    {
        events.Reset();
        DespawnAllSummons();
        me->RemoveAllAuras();
        me->SetMaxPower(POWER_ENERGY, 100);
        me->SetPower(POWER_ENERGY, 0);
        me->SetReactState(REACT_AGGRESSIVE);
        DoCast(SPELL_INFECTED_CLAWS);
        DoCast(SPELL_POOL_OF_FIRE);

        eventPhase = 0;
        slimbering = false;
    }

    void EnterCombat(Unit* /*unit*/) override
    {
        Talk(SAY_AGGRO);
        DoCast(SPELL_NAUSEA);
        StartEvents(PHASE_1);
    }

    void EnterEvadeMode() override
    {
        ScriptedAI::EnterEvadeMode();
    }

    void JustDied(Unit* /*who*/) override
    {
        DespawnAllSummons();
        Talk(SAY_KILL);
    }

    void DespawnAllSummons()
    {
        std::list<Creature*> list;
        list.clear();
        me->GetCreatureListWithEntryInGrid(list, 124552, 300.0f);
        me->GetCreatureListWithEntryInGrid(list, 124551, 300.0f);
        if (!list.empty())
            for (auto cre : list)
                cre->DespawnOrUnsummon();
    }

    void SetPower(Powers power, int32 value) override
    {
        if (power != POWER_ENERGY || !me->isInCombat())
            return;

        if (value == 100)
        {
            if (!me->HasUnitState(UNIT_STATE_CASTING))
            {
                if (!slimbering)
                {
                    slimbering = true;
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    StartEvents(PHASE_2);

                    if (auto trigger = me->SummonCreature(124537, 4408.702f, 6490.092f, 40.38989f, 1.38464f, TEMPSUMMON_TIMED_DESPAWN, 15000))
                        DoCast(trigger, SPELL_SLIMBERING_GASP, false);

                    me->AddDelayedCombat(500, [=]() -> void { me->SetPower(POWER_ENERGY, 0); });
                }
            }
        }
    }

    void DoAction(int32 const action) override
    {
        if (action == ACTION_1)
            StartEvents(PHASE_1);
    }

    void StartEvents(uint8 phase)
    {
        events.Reset();
        eventPhase = phase;

        switch (phase)
        {
        case PHASE_1:
            slimbering = false;
            me->SetReactState(REACT_AGGRESSIVE);
            events.RescheduleEvent(EVENT_FEL_BLAST, 2000);
            events.RescheduleEvent(EVENT_GROTESQUE_SPAWN, 8000);
            break;
        case PHASE_2:
            events.CancelEvent(EVENT_FEL_BLAST);
            events.CancelEvent(EVENT_GROTESQUE_SPAWN);
            break;
        }
    }

    void SpellHitTarget(Unit* /*target*/, SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
        case SPELL_GROTESQUE_SPAWN:
            auto threatlist = me->getThreatManager().getThreatList();
            if (!threatlist.empty())
            {
                auto& itr = Trinity::Containers::SelectRandomContainerElement(threatlist);

                if (Unit* target = itr->getTarget())
                    for (uint8 i = 0; i < 4; ++i)
                        target->CastSpell(me, RANDOM_GROTESQUE_SPAWN[urand(0, 1)], false, 0, 0, me->GetGUID());
            }
            break;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        if (me->HasUnitState(UNIT_STATE_CASTING) && eventPhase != PHASE_2)
            return;

        events.Update(diff);

        if (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_FEL_BLAST:
                if (auto target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                    DoCast(target, SPELL_FEL_BLAST, false);

                events.RescheduleEvent(EVENT_FEL_BLAST, 10000);
                break;
            case EVENT_GROTESQUE_SPAWN:
                DoCast(SPELL_GROTESQUE_SPAWN);
                events.RescheduleEvent(EVENT_GROTESQUE_SPAWN, 35000);
                break;
            default:
                break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

//247362
class spell_infected_claws : public AuraScript
{
    PrepareAuraScript(spell_infected_claws);

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        if (Unit* target = caster->getVictim())
            caster->CastSpell(target, SPELL_INFECTED_CLAWS_PROC, false);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_infected_claws::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

//247441
class spell_folnuna_nausea : public AuraScript
{
    PrepareAuraScript(spell_folnuna_nausea);

    uint8 power{};
    std::vector<uint32> energy = { 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 1 };
    uint32 i = 0;

    void Tick(AuraEffect const* aurEff)
    {
        if (auto folnuna = GetCaster()->ToCreature())
        {
            power = folnuna->GetPower(POWER_ENERGY);
            folnuna->SetPower(POWER_ENERGY, power + energy[i]);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_folnuna_nausea::Tick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

//247379
class spell_slimbering_gasp : public AuraScript
{
    PrepareAuraScript(spell_slimbering_gasp);

    void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster()->ToCreature();
        if (!caster)
            return;

        caster->GetAI()->DoAction(ACTION_1);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_slimbering_gasp::OnRemove, EFFECT_1, SPELL_AURA_CREATE_AREATRIGGER, AURA_EFFECT_HANDLE_REAL);
    }
};

//124537
struct npc_slumbering_gasp : ScriptedAI
{
    npc_slumbering_gasp(Creature* creature) : ScriptedAI(creature)
    {
        me->SetReactState(REACT_PASSIVE);
    }

    void Reset() override
    {
        me->GetMotionMaster()->MovePath(124537, false);
    }
};

void AddSC_invasion_point_boss_folnuna()
{
    RegisterCreatureAI(boss_matron_folnuna);
    RegisterAuraScript(spell_infected_claws);
    RegisterAuraScript(spell_folnuna_nausea);
    RegisterAuraScript(spell_slimbering_gasp);
    RegisterCreatureAI(npc_slumbering_gasp);
}
