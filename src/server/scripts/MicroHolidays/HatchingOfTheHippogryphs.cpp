////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

// The eggs, the hatching and Lorthalium run on SmartAI; only the Spectral Feather needs a script.

#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "Unit.h"

enum HatchingOfTheHippogryphs
{
    SPELL_FRAYFEATHER_HATCHLING     = 229687,
    SPELL_FRAYFEATHER_HATCHLING_ALT = 237356,
};

// 229709 - Extend Hatchling Duration (Spectral Feather, Lorthalium's drop): the hatchling stays five days instead of one
class spell_extend_hatchling_duration : public SpellScript
{
    PrepareSpellScript(spell_extend_hatchling_duration);

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();
        if (!caster || (!caster->HasAura(SPELL_FRAYFEATHER_HATCHLING) && !caster->HasAura(SPELL_FRAYFEATHER_HATCHLING_ALT)))
            return SPELL_FAILED_CASTER_AURASTATE;

        return SPELL_CAST_OK;
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        // the effect value is the number of days
        int32 const duration = GetEffectValue() * DAY * IN_MILLISECONDS;
        for (uint32 spellId : { SPELL_FRAYFEATHER_HATCHLING, SPELL_FRAYFEATHER_HATCHLING_ALT })
        {
            if (Aura* aura = caster->GetAura(spellId))
            {
                aura->SetMaxDuration(duration);
                aura->SetDuration(duration);
            }
        }
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_extend_hatchling_duration::CheckCast);
        OnEffectHit += SpellEffectFn(spell_extend_hatchling_duration::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

void AddSC_HatchingOfTheHippogryphs()
{
    RegisterSpellScript(spell_extend_hatchling_duration);
}
