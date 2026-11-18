////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Azsuna: Levantus, world boss of the Azsuna coast (Adventure Guide encounter 1769)

    She fights in the open water. Gusts of wind keep rising around her: touching one throws a player up and lets them
    walk on the water until they are hit (Gust of Wind, 217206, from the Gust of Wind creature's area trigger 7535).
    Her abilities, as the guide gives them:
    - Rending Whirl: a 6 s whirl, then damage and a bleed to everyone close;
    - Massive Spout: a 20 s breath in front of her while she spins in a random direction (area trigger 7537);
    - Electrify: lightning along the water, damage and a 6 s stun to everyone in it; those walking on it are spared;
    - Turbulent Vortex: whirlpools under the players, frost damage every second;
    - Rampaging Torrent: when nobody is in melee range she thrashes about, frost damage to all, stacking.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"

enum LevantusSpells
{
    SPELL_GUST_OF_WIND_SUMMON   = 217212,   // a Gust of Wind (109211) rises in the water near her
    SPELL_GUST_OF_WIND_WALK     = 217206,   // what a gust grants: thrown up, then walking on the water
    SPELL_RENDING_WHIRL         = 217235,
    SPELL_MASSIVE_SPOUT         = 217249,
    SPELL_ELECTRIFY             = 217344,
    SPELL_ELECTRIFY_HIT         = 217352,
    SPELL_TURBULENT_VORTEX      = 217361,   // a whirlpool falls where the player stands (217362)
    SPELL_RAMPAGING_TORRENT     = 217229,
};

enum LevantusEvents
{
    EVENT_GUSTS = 1,
    EVENT_RENDING_WHIRL,
    EVENT_MASSIVE_SPOUT,
    EVENT_ELECTRIFY,
    EVENT_TURBULENT_VORTEX,
    EVENT_MELEE_CHECK,
};

// 108829
struct boss_levantus : public ScriptedAI
{
    boss_levantus(Creature* creature) : ScriptedAI(creature), summons(me) { }

    EventMap events;
    SummonList summons;

    void Reset() override
    {
        events.Reset();
        summons.DespawnAll();
        me->RemoveAllAreaObjects();
        me->SetReactState(REACT_AGGRESSIVE);
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_GUSTS, 1000);
        events.ScheduleEvent(EVENT_RENDING_WHIRL, urand(7000, 8000));
        events.ScheduleEvent(EVENT_TURBULENT_VORTEX, 11000);
        events.ScheduleEvent(EVENT_MASSIVE_SPOUT, urand(25000, 27000));
        events.ScheduleEvent(EVENT_ELECTRIFY, urand(45000, 50000));
        events.ScheduleEvent(EVENT_MELEE_CHECK, 5000);
    }

    void JustSummoned(Creature* summon) override
    {
        summons.Summon(summon);
    }

    void EnterEvadeMode() override
    {
        summons.DespawnAll();
        me->RemoveAllAreaObjects();
        ScriptedAI::EnterEvadeMode();
    }

    void JustDied(Unit* /*killer*/) override
    {
        summons.DespawnAll();
        me->RemoveAllAreaObjects();
    }

    // Electrify strikes those in the water; walking on it, flying or jumping above it, they are spared
    void SpellFinishCast(SpellInfo const* spell) override
    {
        if (spell->Id != SPELL_ELECTRIFY)
            return;

        std::list<HostileReference*> const& threats = me->getThreatManager().getThreatList();
        for (HostileReference* ref : threats)
        {
            Player* player = ObjectAccessor::GetPlayer(*me, ref->getUnitGuid());
            if (!player || !player->IsAlive() || !me->IsWithinDistInMap(player, 100.0f))
                continue;

            if (player->IsInWater() && !player->HasAuraType(SPELL_AURA_WATER_WALK))
                me->CastSpell(player, SPELL_ELECTRIFY_HIT, true);
        }
    }

    bool AnyoneInMeleeRange() const
    {
        std::list<HostileReference*> const& threats = me->getThreatManager().getThreatList();
        for (HostileReference* ref : threats)
            if (Unit* unit = ref->getTarget())
                if (unit->IsAlive() && me->IsWithinMeleeRange(unit))
                    return true;
        return false;
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);
        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_GUSTS:
                    // new gusts keep rising around her for the whole fight, each one lasting 35 s
                    for (uint8 i = 0; i < 3; ++i)
                        DoCast(me, SPELL_GUST_OF_WIND_SUMMON, true);
                    events.ScheduleEvent(EVENT_GUSTS, 12000);
                    break;
                case EVENT_RENDING_WHIRL:
                    DoCast(me, SPELL_RENDING_WHIRL);
                    events.ScheduleEvent(EVENT_RENDING_WHIRL, urand(39000, 41000));
                    return;
                case EVENT_MASSIVE_SPOUT:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->SetFacingToObject(target);
                    DoCast(me, SPELL_MASSIVE_SPOUT);
                    events.ScheduleEvent(EVENT_MASSIVE_SPOUT, urand(42000, 44000));
                    // nothing else while she spouts
                    events.DelayEvents(24000);
                    return;
                case EVENT_ELECTRIFY:
                    DoCast(me, SPELL_ELECTRIFY);
                    events.ScheduleEvent(EVENT_ELECTRIFY, urand(42000, 44000));
                    return;
                case EVENT_TURBULENT_VORTEX:
                {
                    // a whirlpool under a few of the players around her
                    std::list<Unit*> targets;
                    SelectTargetList(targets, 3, SELECT_TARGET_RANDOM, 60.0f, true);
                    for (Unit* target : targets)
                        me->CastSpell(target, SPELL_TURBULENT_VORTEX, true);
                    events.ScheduleEvent(EVENT_TURBULENT_VORTEX, 10000);
                    break;
                }
                case EVENT_MELEE_CHECK:
                    if (!AnyoneInMeleeRange())
                    {
                        DoCast(me, SPELL_RAMPAGING_TORRENT);
                        events.ScheduleEvent(EVENT_MELEE_CHECK, 3000);
                        return;
                    }
                    events.ScheduleEvent(EVENT_MELEE_CHECK, 1000);
                    break;
                default:
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// 217249 - Massive Spout: she stops fighting and spins in a random direction while she breathes
class spell_levantus_massive_spout : public AuraScript
{
    PrepareAuraScript(spell_levantus_massive_spout);

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Creature* levantus = GetCaster() ? GetCaster()->ToCreature() : nullptr;
        if (!levantus)
            return;

        levantus->AttackStop();
        levantus->SetReactState(REACT_PASSIVE);
        levantus->GetMotionMaster()->MoveRotate(GetMaxDuration(), urand(0, 1) ? ROTATE_DIRECTION_LEFT : ROTATE_DIRECTION_RIGHT);
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Creature* levantus = GetCaster() ? GetCaster()->ToCreature() : nullptr;
        if (!levantus || !levantus->IsAlive())
            return;

        levantus->StopMoving();
        levantus->GetMotionMaster()->Clear(false);
        levantus->SetReactState(REACT_AGGRESSIVE);
        if (levantus->AI())
            levantus->AI()->DoZoneInCombat(levantus, 150.0f);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_levantus_massive_spout::OnApply, EFFECT_0, SPELL_AURA_CREATE_AREATRIGGER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_levantus_massive_spout::OnRemove, EFFECT_0, SPELL_AURA_CREATE_AREATRIGGER, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_world_boss_levantus()
{
    RegisterCreatureAI(boss_levantus);
    RegisterAuraScript(spell_levantus_massive_spout);
}
