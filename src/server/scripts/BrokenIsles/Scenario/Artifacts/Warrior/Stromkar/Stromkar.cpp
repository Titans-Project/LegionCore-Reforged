////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Warrior (Arms) artifact: Strom'kar, the Warbreaker (quest 41105, scenario 1037 on map 1539).
    Most of the scenario runs on world data. The script only covers what data cannot do:
    the recoil of the first attempt to draw the sword and the Zakajz boss left at one health point.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SmartAI.h"
#include "Scenario.h"
#include "ScenarioMgr.h"
#include "SpellScript.h"
#include "Player.h"

namespace
{
    enum Stromkar
    {
        MAP_STROMKAR            = 1539,
        SCENARIO_STROMKAR       = 1037,

        NPC_ZAKAJZ              = 104276,

        SPELL_RECOIL            = 206360,
        SPELL_DEAD_POSE         = 130966,

        STEP_ZAKAJZ_FIGHT       = 8,    // "To Kill a C'Thraxxi"
        STEP_EXECUTION          = 9,    // "The Warbreaker"

        ENGAGE_RETRY            = 2000,
        ENGAGE_RANGE            = 100,
        RECOIL_DISTANCE         = 30
    };

    // -1 when the unit is not in the Strom'kar scenario
    int32 GetScenarioStep(Unit const* unit)
    {
        int32 step = -1;
        if (unit && unit->IsInWorld() && unit->GetMapId() == MAP_STROMKAR)
            sScenarioMgr->WithScenario(unit->GetInstanceId(), [&step](Scenario* scenario)
            {
                if (scenario->GetScenarioId() == SCENARIO_STROMKAR)
                    step = int32(scenario->GetCurrentStep());
            });
        return step;
    }
}

// 104276 - Zakajz, summoned when the sword is drawn
struct npc_stromkar_zakajz : public SmartAI
{
    explicit npc_stromkar_zakajz(Creature* creature) : SmartAI(creature) { }

    void Reset() override
    {
        _engageTimer = 0;
        SmartAI::Reset();
    }

    void UpdateAI(uint32 diff) override
    {
        SmartAI::UpdateAI(diff);

        if (!me->IsAlive())
            return;

        int32 const step = GetScenarioStep(me);

        // Nobody is aggroed while the players are thrown back and stunned by the recoil: the fight starts by itself
        if (step == STEP_ZAKAJZ_FIGHT && !me->isInCombat())
        {
            if (_engageTimer > diff)
            {
                _engageTimer -= diff;
                return;
            }

            _engageTimer = ENGAGE_RETRY;

            std::list<Player*> players;
            me->GetPlayerListInGrid(players, float(ENGAGE_RANGE));
            for (Player* player : players)
                if (player->IsAlive() && me->InSamePhase(player) && me->IsValidAttackTarget(player))
                {
                    AttackStart(player);
                    break;
                }
            return;
        }

        // The scripted chain leaves it at one health point and passive: keep it lying down until the execution
        if (step >= STEP_EXECUTION && me->GetHealthPct() <= 1.0f)
        {
            if (me->isRegeneratingHealth())
                me->setRegeneratingHealth(false);
            if (!me->HasAura(SPELL_DEAD_POSE))
                me->AddAura(SPELL_DEAD_POSE, me);
        }
    }

private:
    uint32 _engageTimer = 0;
};

// 206360 - Recoil: the target of the jump is the caster of the channel, so nothing moved the players away
class spell_stromkar_opening_recoil : public SpellScript
{
    PrepareSpellScript(spell_stromkar_opening_recoil);

    void SetLanding(SpellEffIndex effIndex)
    {
        Unit* player = GetCaster();
        Unit* source = GetOriginalTarget();
        WorldLocation* dest = GetHitDest();
        if (!player || !player->IsPlayer() || !source || !dest || GetScenarioStep(player) < 0)
        {
            PreventHitDefaultEffect(effIndex);
            return;
        }

        // straight away from the sword, shortened by the first obstacle
        float const away = source->GetExactDist2d(player) > 0.1f ? source->GetAngle(player) : source->GetOrientation();
        Position const position = player->GetFirstCollisionPosition(float(RECOIL_DISTANCE), away - player->GetOrientation());

        WorldLocation landing(*dest);
        landing.Relocate(position);
        // and facing the sword again once there
        landing.SetOrientation(landing.GetAngle(source));
        dest->Relocate(landing);
        SetExplTargetDest(landing);
    }

    void Register() override
    {
        OnEffectLaunch += SpellEffectFn(spell_stromkar_opening_recoil::SetLanding, EFFECT_0, SPELL_EFFECT_JUMP_DEST);
    }
};

void AddSC_Stromkar()
{
    RegisterCreatureAI(npc_stromkar_zakajz);
    RegisterSpellScript(spell_stromkar_opening_recoil);
}
