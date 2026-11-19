////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Demon Hunter order hall campaign.
    Securing the Way (42872), at the Illidari Redoubt of Dalaran.
    Campaign scenarios in the client data: The Invasion of Niskara (scenario 1145, map 1489); Confrontation at the Black Temple (scenario 1131, map 1621).
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "GameObjectAI.h"
#include "SpellScript.h"
#include <vector>

namespace SecuringTheWay
{
    enum
    {
        QUEST_SECURING_THE_WAY          = 42872,
        NPC_CREDIT_SOULS_UNLEASHED      = 109230,
        NPC_CAPTURED_WRATHGUARD         = 109247,
        NPC_IMPRISONED_IMP              = 109252,
        NPC_SOUL_FONT                   = 109273,
    };

    uint32 const Executioners[] = { 109344, 109309, 109308, 109314 }; // Kor'vas, Asha, Belath, Allari

    // where the prisoners' souls gather, before the gateway (Wowhead, Dalaran floor 10, at 97.2 68.2 and 97.8 69.0)
    Position const SoulFonts[] =
    {
        { -957.6f, 4073.1f, 650.5f, 0.65f },
        { -961.9f, 4068.2f, 650.5f, 0.65f },
    };

    bool SoulsRemoved(Player* player)
    {
        return player->GetQuestStatus(QUEST_SECURING_THE_WAY) == QUEST_STATUS_INCOMPLETE
            && player->GetReqKillOrCastCurrentCount(QUEST_SECURING_THE_WAY, NPC_CREDIT_SOULS_UNLEASHED) > 0;
    }
}

// 217377 - Sentence to Death: the Illidari of the Redoubt execute the prisoners, whose souls gather in two Soul Fonts by
// the gateway, there for the hunter alone to click and rip the gateway open (217719)
class spell_dh_sentence_to_death : public SpellScript
{
    PrepareSpellScript(spell_dh_sentence_to_death);

    void HandleAfterCast()
    {
        using namespace SecuringTheWay;

        Player* hunter = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        if (!hunter)
            return;

        std::vector<Creature*> executioners;
        for (uint32 entry : Executioners)
            if (Creature* executioner = hunter->FindNearestCreature(entry, 60.0f))
                executioners.push_back(executioner);

        std::list<Creature*> prisoners;
        hunter->GetCreatureListWithEntryInGrid(prisoners, NPC_CAPTURED_WRATHGUARD, 60.0f);
        std::list<Creature*> imps;
        hunter->GetCreatureListWithEntryInGrid(imps, NPC_IMPRISONED_IMP, 60.0f);
        prisoners.splice(prisoners.end(), imps);

        uint32 delay = 300;
        for (Creature* prisoner : prisoners)
        {
            if (!prisoner->IsAlive())
                continue;

            // each prisoner by the Illidari standing nearest to him
            Creature* executioner = nullptr;
            for (Creature* candidate : executioners)
                if (!executioner || prisoner->GetExactDist(candidate) < prisoner->GetExactDist(executioner))
                    executioner = candidate;

            ObjectGuid const prisonerGuid = prisoner->GetGUID();
            ObjectGuid const executionerGuid = executioner ? executioner->GetGUID() : ObjectGuid::Empty;
            prisoner->AddDelayedEvent(delay, [prisoner, prisonerGuid, executionerGuid]() -> void
            {
                if (!prisoner->IsAlive())
                    return;

                Creature* executioner = executionerGuid ? ObjectAccessor::GetCreature(*prisoner, executionerGuid) : nullptr;
                if (executioner && executioner->IsAlive())
                {
                    executioner->SetFacingToObject(prisoner);
                    executioner->HandleEmoteCommand(EMOTE_ONESHOT_ATTACK1H);
                    executioner->Kill(prisoner);
                }
                else
                    prisoner->Kill(prisoner);
            });
            delay += 400;
        }

        for (Position const& pos : SoulFonts)
            hunter->SummonCreature(NPC_SOUL_FONT, pos, TEMPSUMMON_TIMED_DESPAWN, 5 * MINUTE * IN_MILLISECONDS, 0, hunter->GetGUID());
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_dh_sentence_to_death::HandleAfterCast);
    }
};

// 217723 - the gateway ripped open throws the hunter back. Cast by him on himself, its push has no direction of its own
// (a random one): it goes away from the gateway. The Soul Fonts go too
class spell_dh_gateway_ripped_open_knockback : public SpellScript
{
    PrepareSpellScript(spell_dh_gateway_ripped_open_knockback);

    static constexpr uint32 GO_ILLIDARI_GATEWAY = 251579;

    void HandleKnockBack(SpellEffIndex effIndex)
    {
        Unit* target = GetHitUnit();
        GameObject* gateway = target ? target->FindNearestGameObject(GO_ILLIDARI_GATEWAY, 60.0f) : nullptr;
        if (!gateway)
            return;

        PreventHitDefaultEffect(effIndex);

        // the souls are spent on the gateway: the Soul Fonts that held them, his own, are gone with it
        std::list<Creature*> fonts;
        target->GetCreatureListWithEntryInGrid(fonts, SecuringTheWay::NPC_SOUL_FONT, 60.0f);
        for (Creature* font : fonts)
            if (TempSummon* summon = font->ToTempSummon())
                if (summon->GetSummonerGUID() == target->GetGUID())
                    font->DespawnOrUnsummon(500);

        SpellEffectInfo const* effect = GetSpellInfo()->GetEffect(effIndex);
        // as Spell::EffectKnockBack: MiscValue the speed away, the effect's value the speed up, both in tenths
        target->KnockbackFrom(gateway->GetPositionX(), gateway->GetPositionY(), float(effect->MiscValue) * 0.1f, float(GetEffectValue()) * 0.1f);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dh_gateway_ripped_open_knockback::HandleKnockBack, EFFECT_0, SPELL_EFFECT_KNOCK_BACK);
    }
};

// Illidari Gateway (251579), the frame of the gateway: ripped open only once the prisoners' souls are out of them
struct go_illidari_gateway_securing_the_way : public GameObjectAI
{
    go_illidari_gateway_securing_the_way(GameObject* go) : GameObjectAI(go) { }

    bool GossipUse(Player* player) override
    {
        return !SecuringTheWay::SoulsRemoved(player);
    }
};

void AddSC_demon_hunter_campaign()
{
    RegisterSpellScript(spell_dh_sentence_to_death);
    RegisterSpellScript(spell_dh_gateway_ripped_open_knockback);
    RegisterGameObjectAI(go_illidari_gateway_securing_the_way);
}
