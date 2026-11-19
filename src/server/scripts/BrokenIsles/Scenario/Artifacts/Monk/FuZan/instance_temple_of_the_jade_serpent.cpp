////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Monk Specialization: Brewmaster
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SmartAI.h"
#include "SpellScript.h"

enum FuZanData
{
    NPC_YIGTHREGOR          = 109820,
    SPELL_FELFIRE_BOMB_AT   = 217787
};

// Yu'lon's reply to the cleared wave is driven by Korithis's text (SmartAI), so the instance
// does not count the wave deaths itself: a raw counter played that one-shot reply too early.
class instance_temple_of_the_jade_serpent : public InstanceMapScript
{
public:
    instance_temple_of_the_jade_serpent() : InstanceMapScript("instance_temple_of_the_jade_serpent", 1625) {}

    InstanceScript* GetInstanceScript(InstanceMap* map) const override
    {
        return new instance_temple_of_the_jade_serpent_InstanceMapScript(map);
    }

    struct instance_temple_of_the_jade_serpent_InstanceMapScript : public InstanceScript
    {
        instance_temple_of_the_jade_serpent_InstanceMapScript(InstanceMap* map) : InstanceScript(map) {}

        void onScenarionNextStep(uint32 newStep) override {}
    };
};

// Yigthregor keeps no Felfire Bomb field once he is dead or reset
struct npc_fuzan_yigthregor : public SmartAI
{
    explicit npc_fuzan_yigthregor(Creature* creature) : SmartAI(creature) { }

    void Reset() override
    {
        me->RemoveAreaObject(SPELL_FELFIRE_BOMB_AT);
        SmartAI::Reset();
    }

    void JustDied(Unit* killer) override
    {
        me->CastStop();
        me->RemoveAreaObject(SPELL_FELFIRE_BOMB_AT);
        SmartAI::JustDied(killer);
    }
};

// A bomb still in flight when its caster dies must not leave a new field behind
class spell_fuzan_felfire_bomb : public SpellScript
{
    PrepareSpellScript(spell_fuzan_felfire_bomb);

    void HandleField(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();
        if (caster && caster->GetEntry() == NPC_YIGTHREGOR && !caster->IsAlive())
            PreventHitDefaultEffect(effIndex);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_fuzan_felfire_bomb::HandleField, EFFECT_0, SPELL_EFFECT_CREATE_AREATRIGGER);
    }
};

void AddSC_instance_temple_of_the_jade_serpent()
{
    new instance_temple_of_the_jade_serpent();
    RegisterCreatureAI(npc_fuzan_yigthregor);
    RegisterSpellScript(spell_fuzan_felfire_bomb);
}
