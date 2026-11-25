////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "Player.h"

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

void AddSC_darkmoon_faire()
{
    RegisterSpellScript(spell_gen_darkmoon_carousel_whee);
}
