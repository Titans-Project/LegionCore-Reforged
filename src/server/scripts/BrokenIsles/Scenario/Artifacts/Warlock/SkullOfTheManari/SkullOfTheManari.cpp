////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Warlock (Demonology) artifact: Skull of the Man'ari, Dark Whispers scenario.
    The scenario itself runs on SmartAI; only the skull handoff needs code.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "Vehicle.h"

namespace DarkWhispers
{
enum
{
    NPC_MEPHISTROTH             = 106692,
    NPC_THALKIEL_SKULL          = 106693,
    SPELL_CONTROL_VEHICLE_SKULL = 211702
};

bool IsPair(Unit* carrier, Unit* skull)
{
    return carrier && skull && carrier->GetEntry() == NPC_MEPHISTROTH && skull->GetEntry() == NPC_THALKIEL_SKULL &&
        skull->GetMap() == carrier->GetMap() && carrier->InSamePhase(skull);
}
}

// 211703 - Mephistroth takes the skull in his hand
class spell_dark_whispers_ride_skull : public SpellScript
{
    PrepareSpellScript(spell_dark_whispers_ride_skull);

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        Unit* carrier = GetCaster();
        Unit* skull = GetHitUnit();
        if (!DarkWhispers::IsPair(carrier, skull) || !carrier->IsAlive() || !skull->IsAlive() ||
            !carrier->GetVehicleKit() || carrier->GetVehicleKit()->GetPassenger(0) || skull->GetVehicle())
            return;

        // The effect value is the control-vehicle aura, which the skull itself has to cast.
        skull->CastSpell(carrier, DarkWhispers::SPELL_CONTROL_VEHICLE_SKULL, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dark_whispers_ride_skull::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 211704 - Mephistroth leaves and the skull in his hand goes with him
class spell_dark_whispers_despawn_skull : public SpellScript
{
    PrepareSpellScript(spell_dark_whispers_despawn_skull);

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        Unit* carrier = GetCaster();
        if (!carrier || carrier->GetEntry() != DarkWhispers::NPC_MEPHISTROTH || !carrier->GetVehicleKit())
            return;

        Unit* skull = carrier->GetVehicleKit()->GetPassenger(0);
        if (!DarkWhispers::IsPair(carrier, skull) || !skull->ToCreature())
            return;

        skull->ExitVehicle();
        skull->ToCreature()->DespawnOrUnsummon();
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dark_whispers_despawn_skull::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

void AddSC_SkullOfTheManari()
{
    RegisterSpellScript(spell_dark_whispers_ride_skull);
    RegisterSpellScript(spell_dark_whispers_despawn_skull);
}
