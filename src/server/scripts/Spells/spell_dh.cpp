////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
///      Based on TrinityCore code       ///
///   Copyright 2008-2012 TrinityCore    ///
///   <http://www.trinitycore.org/>      ///
///                                      ///
////////////////////////////////////////////

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "AreaTriggerAI.h"
#include "AreaTrigger.h"

enum Spells
{
    SoulFragment1 = 204255,
    SoulFragment2 = 203795,
    SoulFragment3 = 204062,
};
//- 131347
class spell_dh_glide : public SpellScriptLoader
{
public:
    spell_dh_glide() : SpellScriptLoader("spell_dh_glide") { }

    class sspell_dh_glide_AuraScript : public AuraScript
    {
        PrepareAuraScript(sspell_dh_glide_AuraScript);

        void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
            {
                caster->CastSpell(caster, 196353, true);
                caster->CastSpell(caster, 197154, true, NULL, aurEff);
               // caster->CastSpell(caster, 199303, true, NULL, aurEff);
                //
            }
        }
        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* target = GetTarget())
            {
            //    target->RemoveAurasDueToSpell(199303);
            }
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(sspell_dh_glide_AuraScript::OnApply, EFFECT_0, SPELL_AURA_FEATHER_FALL, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(sspell_dh_glide_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_FEATHER_FALL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new sspell_dh_glide_AuraScript();
    }

    class spell_dh_glide_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dh_glide_SpellScript);

        SpellCastResult CheckRequirement()
        {
            if (auto player = GetCaster()->ToPlayer())
                if (player->IsMounted())
                    return SPELL_FAILED_NOT_ON_MOUNTED;

            return SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dh_glide_SpellScript::CheckRequirement);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dh_glide_SpellScript();
    }
};

// Nemesis - 206491
class spell_dh_nemesis : public SpellScriptLoader
{
    public:
    spell_dh_nemesis() : SpellScriptLoader("spell_dh_nemesis") {}

    class spell_dh_nemesis_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dh_nemesis_AuraScript);

        void OnProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* target = GetUnitOwner())
                {
                    uint32 spellId = 0;
                    uint32 creatureTypeMask = (1 << target->GetCreatureType());
                    
                    if (creatureTypeMask & (1 << CREATURE_TYPE_HUMANOID))
                        spellId = 208605;
                    else if (creatureTypeMask & (1 << CREATURE_TYPE_DEMON))
                        spellId = 208579;
                    else if (creatureTypeMask & (1 << CREATURE_TYPE_ABBERATION))
                        spellId = 208607;
                    else if (creatureTypeMask & (1 << CREATURE_TYPE_BEAST))
                        spellId = 208608;
                    else if (creatureTypeMask & (1 << CREATURE_TYPE_CRITTER))
                        spellId = 208609;
                    else if (creatureTypeMask & (1 << CREATURE_TYPE_DRAGONKIN))
                        spellId = 208610;
                    else if (creatureTypeMask & (1 << CREATURE_TYPE_ELEMENTAL))
                        spellId = 208611;
                    else if (creatureTypeMask & (1 << CREATURE_TYPE_GIANT))
                        spellId = 208612;
                    else if (creatureTypeMask & (1 << CREATURE_TYPE_MECHANICAL))
                        spellId = 208613;
                    else if (creatureTypeMask & (1 << CREATURE_TYPE_UNDEAD))
                        spellId = 208614;

                    if (spellId)
                    {
                        if (Aura* aura = GetAura())
                        {
                            int32 curDuration = aura->GetDuration();

                            caster->AddAura(spellId, caster, nullptr, 0, curDuration, curDuration);
                        }
                    }
                }
            }
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dh_nemesis_AuraScript::OnProc, EFFECT_0, SPELL_AURA_INCREASE_SCHOOL_DAMAGE_TAKEN);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dh_nemesis_AuraScript();
    }
};

// Last Resort - 209258
class spell_dh_disable_absorb : public SpellScriptLoader
{
    public:
        spell_dh_disable_absorb() : SpellScriptLoader("spell_dh_disable_absorb") { }

        class spell_dh_disable_absorb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dh_disable_absorb_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, float& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffect* /*AuraEffect**/, DamageInfo& dmgInfo, float& absorbAmount)
            {
                if (Unit* target = GetTarget())
                {
                    absorbAmount = 0;

                    if (dmgInfo.GetDamage() < target->GetHealth())
                        return;

                    if (target->HasAura(209261))
                        return;

                    target->CastSpell(target, 209261, true);
                    target->CastSpell(target, 187827, true);

                    uint64 health30 = target->CountPctFromMaxHealth(30);

                    // hp > 10% - absorb hp till 10%
                    if (target->GetHealth() > health30)
                        absorbAmount = dmgInfo.GetDamage() - target->GetHealth() + health30;
                    // hp lower than 10% - absorb everything
                    else
                        absorbAmount = dmgInfo.GetDamage();
                }
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dh_disable_absorb_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dh_disable_absorb_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dh_disable_absorb_AuraScript();
        }
};

// Metamorphosis - 200166
class spell_dh_metamorphosis : public SpellScriptLoader
{
    public:
    spell_dh_metamorphosis() : SpellScriptLoader("spell_dh_metamorphosis") {}

    class spell_dh_metamorphosis_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dh_metamorphosis_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (Unit* caster = GetCaster())
            {
                std::vector<WorldObject*> removeList;

                for (auto itr : targets)
                {
                    if (Player* plr = itr->ToPlayer())
                    {
                        caster->CastSpell(plr, 247121, true);
                        removeList.push_back(itr);
                    }
                }

                if (!removeList.empty())
                {
                    for (auto itr : removeList)
                        targets.remove(itr);
                }
            }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dh_metamorphosis_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dh_metamorphosis_SpellScript();
    }
};

// Blade Dance - 199552, 200685, 210153, 210155
class spell_dh_blade_dance : public SpellScript
{
    PrepareSpellScript(spell_dh_blade_dance);

    void HandleOnHit()
    {
        Player* caster = GetCaster()->ToPlayer();
        if (!caster)
            return;

        int32 damage = GetHitDamage();

        if (AuraEffect const* aurEff = caster->GetAuraEffect(201470, EFFECT_0)) // Balanced Blades
            damage += CalculatePct(damage, aurEff->GetAmount() * GetSpell()->GetTargetCount());

        if (AuraEffect* aurEf = caster->GetAuraEffect(206416, EFFECT_0)) // First Blood
        {
            if (Unit* target = GetHitUnit())
            {
                if (target->GetGUID() == caster->GetSelection())
                    damage += CalculatePct(damage, aurEf->GetAmount());
            }
        }

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_dh_blade_dance::HandleOnHit);
    }
};

// Fel Rush - 195072
class spell_dh_fel_rush_main : public SpellScriptLoader
{
public:
    spell_dh_fel_rush_main() : SpellScriptLoader("spell_dh_fel_rush_main") {}

    class spell_dh_fel_rush_main_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dh_fel_rush_main_SpellScript);

        void HandleBeforeStartCast()
        {
            if (Unit* caster = GetCaster())
            {
                if (caster->HasUnitMovementFlag(MOVEMENTFLAG_FALLING))
                {
                    caster->CastSpell(caster, 197923, true);
                }
                else
                {
                    caster->CastSpell(caster, 197922, true);
                }
            }
        }

        void Register() override
        {
            BeforeStartCast += SpellCastFn(spell_dh_fel_rush_main_SpellScript::HandleBeforeStartCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dh_fel_rush_main_SpellScript();
    }
};

// Desperate Instincts - 205478
class spell_dh_desperate_instincts : public SpellScriptLoader
{
    public:
        spell_dh_desperate_instincts() : SpellScriptLoader("spell_dh_desperate_instincts") { }

        class spell_dh_desperate_instincts_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dh_desperate_instincts_AuraScript)

            void HandleTriggerSpell(AuraEffect const* aurEff)
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->GetHealthPct() < 35.f)
                    {
                        if (!caster->HasSpellCooldown(198589))
                        {
                            uint32 triggerFlags = TRIGGERED_FULL_MASK;
                            triggerFlags &= ~TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD;

                            caster->CastSpell(caster, 198589, TriggerCastFlags(triggerFlags)); // Blur
                        }
                    }
                    else
                    {
                        GetAura()->Remove();
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dh_desperate_instincts_AuraScript::HandleTriggerSpell, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dh_desperate_instincts_AuraScript();
        }
};

// Illidan's Grasp: Visual - 208813
class spell_dh_illidans_grasp_visual : public SpellScriptLoader
{
    public:
    spell_dh_illidans_grasp_visual() : SpellScriptLoader("spell_dh_illidans_grasp_visual") {}

    class spell_dh_illidans_grasp_visual_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dh_illidans_grasp_visual_SpellScript);

        void HandleBeforeStartCast()
        {
            if (Unit* caster = GetCaster())
            {
                std::list<Unit*> targetList;
                std::vector<uint32> aura = {208175};
                caster->TargetsWhoHasMyAuras(targetList, aura);

                if (!targetList.empty())
                {
                    for (auto itr : targetList)
                    {
                        if (Unit* target = itr)
                        {
                            caster->RemoveIdInWHMAList(target->GetGUID(), 208175);
                            target->CastSpell(caster, 208618, true);
                            break;
                        }
                    }
                }
            }
        }

        void Register() override
        {
            BeforeCast += SpellCastFn(spell_dh_illidans_grasp_visual_SpellScript::HandleBeforeStartCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dh_illidans_grasp_visual_SpellScript();
    }
};

// Illidan's Grasp: Throw - 208173
class spell_dh_illidans_grasp_throw : public SpellScriptLoader
{
    public:
    spell_dh_illidans_grasp_throw() : SpellScriptLoader("spell_dh_illidans_grasp_throw") {}

    class spell_dh_illidans_grasp_throw_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dh_illidans_grasp_throw_SpellScript);

        void HandleBeforeStartCast()
        {
            if (Unit* caster = GetCaster())
            {
                std::list<Unit*> targetList;
                std::vector<uint32> aura = {205630};
                caster->TargetsWhoHasMyAuras(targetList, aura);

                if (!targetList.empty())
                {
                    for (auto itr : targetList)
                    {
                        if (Unit* target = itr)
                        {
                            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(208175))
                            {
                                CustomSpellValues values;
                                target->m_whoHasMyAuras[caster->GetGUID()].push_back(spellInfo->Id);
                                target->CastSpell(GetSpell()->m_targets, spellInfo, &values, TRIGGERED_FULL_MASK);
                                break;
                            }
                        }
                    }
                }
            }
        }

        void Register() override
        {
            BeforeStartCast += SpellCastFn(spell_dh_illidans_grasp_throw_SpellScript::HandleBeforeStartCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dh_illidans_grasp_throw_SpellScript();
    }
};

// Soul Cleave - 228477
class spell_dh_soul_cleave : public SpellScriptLoader
{
    public:
        spell_dh_soul_cleave() : SpellScriptLoader("spell_dh_soul_cleave") { }

        class spell_dh_soul_cleave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dh_soul_cleave_SpellScript);

            void HandleOnCast()
            {
                if (Unit* caster = GetCaster())
                {
                    std::list<AreaTrigger*> list;
                    std::vector<uint32> spellIdList = {SoulFragment1, SoulFragment2, SoulFragment3};
                    caster->GetAreaObjectList(list, spellIdList);

                    bool eruptingSouls = caster->HasAura(238082); // Erupting Souls
                    if(!list.empty())
                    {
                        for (std::list<AreaTrigger*>::iterator itr = list.begin(); itr != list.end(); ++itr)
                        {
                            if(AreaTrigger* areaObj = (*itr))
                            {
                                if (caster->GetDistance(areaObj) > GetSpellInfo()->Effects[EFFECT_0]->BasePoints)
                                    continue;

                                areaObj->CastAction();

                                // One Erupting Souls impact per fragment actually consumed.
                                if (eruptingSouls)
                                    if (Unit* target = GetExplTargetUnit())
                                        caster->CastSpell(target, 243160, true);
                            }
                        }
                    }

                    // Feast of Souls is applied once per cast, whatever the targets, health or pain spent.
                    if (AuraEffect const* feast = caster->GetAuraEffect(207697, EFFECT_0))
                    {
                        float heal = caster->GetTotalAttackPowerValue(BASE_ATTACK) * feast->GetAmount() / 100.0f;
                        caster->CastCustomSpell(caster, 207693, &heal, nullptr, nullptr, true);
                    }
                }
            }

            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Player* plr = caster->ToPlayer())
                    {
                        // Heal and damage scale from 1x to 2x with the pain spent (250 to 500).
                        int32 painCost = std::max(250, std::min(500, GetSpell()->GetPowerCost(POWER_PAIN)));
                        SetEffectValue(int32(7.26f * plr->GetTotalAttackPowerValue(BASE_ATTACK) * float(painCost) / 250.0f));
                    }
                }
            }

            void Register() override
            {
                OnCast += SpellCastFn(spell_dh_soul_cleave_SpellScript::HandleOnCast);
                OnEffectLaunchTarget += SpellEffectFn(spell_dh_soul_cleave_SpellScript::HandleHeal, EFFECT_3, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dh_soul_cleave_SpellScript();
        }
};

// Soul Cleave - 228478
class spell_dh_soul_cleave_damage : public SpellScriptLoader
{
    public:
        spell_dh_soul_cleave_damage() : SpellScriptLoader("spell_dh_soul_cleave_damage") { }

        class spell_dh_soul_cleave_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dh_soul_cleave_damage_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->IsPlayer())
                    {
                        int32 painCost = std::max(250, std::min(500, GetSpell()->GetPowerCost(POWER_PAIN)));
                        SetHitDamage(int32(GetHitDamage() * (float(painCost) / 250.0f)));
                    }
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dh_soul_cleave_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dh_soul_cleave_damage_SpellScript();
        }
};

// Demon Spikes - 203720
class spell_dh_demon_spikes : public SpellScript
{
    PrepareSpellScript(spell_dh_demon_spikes);

    void HandleOnCast()
    {
        Unit* caster = GetCaster();
        SpellInfo const* spikes = sSpellMgr->GetSpellInfo(203819);
        if (!caster || !spikes)
            return;

        // A new activation extends the running buff, up to three base durations.
        int32 baseDuration = caster->CalcSpellDuration(spikes);
        if (Aura* aura = caster->GetAura(203819))
        {
            int32 duration = std::min(aura->GetDuration() + baseDuration, baseDuration * 3);
            aura->SetMaxDuration(std::max(aura->GetMaxDuration(), duration));
            aura->SetDuration(duration);
        }
        else
            caster->CastSpell(caster, 203819, true);

        // Defensive Spikes: unmodified parry bonus on every activation.
        if (caster->HasAura(212829))
            caster->CastSpell(caster, 212871, true);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dh_demon_spikes::HandleOnCast);
    }
};

// Razor Spikes - 209400
class spell_dh_razor_spikes : public AuraScript
{
    PrepareAuraScript(spell_dh_razor_spikes);

    void OnProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
    {
        // A spell-modifier aura has no proc handler of its own: slow the victim of every melee hit while Demon Spikes is up.
        Unit* owner = GetTarget();
        Unit* victim = eventInfo.GetActionTarget();
        if (owner && victim && victim != owner && owner->HasAura(203819))
            owner->CastSpell(victim, 210003, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dh_razor_spikes::OnProc, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER);
    }
};

// Shatter Soul - 209980 (Vengeance), 228533 (Havoc) -> (lesser fragments)
// Shatter Soul - 210038, 209981 (big fragment for Vengeance)
// Shatter Soul - 237867, 209651 (big fragment for Havoc)
class spell_dh_shatter_soul : public SpellScript
{
    PrepareSpellScript(spell_dh_shatter_soul);

    void HandleHitTarget(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
        {
            if (WorldLocation const* loc = GetExplTargetDest())
            {
                switch (GetSpellInfo()->Id)
                {
                    case 209980: // Lesser fragment for Vengeance
                    {
                        caster->CastSpell(loc, 226258, true);
                        return;
                    }
                    case 228533: // Lesser fragment for Havoc
                    {
                        caster->CastSpell(loc, 228536, true);
                        return;
                    }
                    case 210038: // Big fragment for Vengeance (demon)
                    {
                        caster->CastSpell(loc, 226264, true);
                        return;
                    }
                    case 209981: // Big fragment for Vengeance
                    {
                        caster->CastSpell(loc, 226263, true);
                        return;
                    }
                    case 237867: // Big fragment for Havoc (demon)
                    {
                        caster->CastSpell(loc, 226259, true);
                        return;
                    }
                    case 209651: // Big fragment for Havoc
                    {
                        caster->CastSpell(loc, 226370, true);
                        return;
                    }
                }
            }
        }
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_dh_shatter_soul::HandleHitTarget, EFFECT_1, SPELL_EFFECT_TRIGGER_MISSILE);
    }
};

// Spirit Bomb - 247455
class spell_dh_spirit_bomb_damage : public SpellScriptLoader
{
    public:
    spell_dh_spirit_bomb_damage() : SpellScriptLoader("spell_dh_spirit_bomb_damage") {}

    class spell_dh_spirit_bomb_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dh_spirit_bomb_damage_SpellScript);

        void HandleBeforeHit()
        {
            // Frailty goes on first so the explosion already benefits from it.
            if (Unit* caster = GetCaster())
                if (Unit* target = GetHitUnit())
                    caster->AddAura(247456, target);
        }

        void HandleOnHit()
        {
            // The fragment count consumed by Spirit Bomb travels in the third base point.
            uint32 fragmentCount = std::max(1, int32(GetSpellValue()->EffectBasePoints[EFFECT_2]));
            SetHitDamage(GetHitDamage() * fragmentCount);
        }

        void Register() override
        {
            BeforeHit += SpellHitFn(spell_dh_spirit_bomb_damage_SpellScript::HandleBeforeHit);
            OnHit += SpellHitFn(spell_dh_spirit_bomb_damage_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dh_spirit_bomb_damage_SpellScript();
    }
};

// Spirit Bomb - 247454
class spell_dh_spirit_bomb : public SpellScriptLoader
{
    public:
        spell_dh_spirit_bomb() : SpellScriptLoader("spell_dh_spirit_bomb") {}

        class spell_dh_spirit_bomb_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dh_spirit_bomb_SpellScript);

            SpellCastResult CheckCast()
            {
                if (Unit* caster = GetCaster())
                {
                    std::list<AreaTrigger*> list;
                    std::vector<uint32> spellIdList = {SoulFragment1, SoulFragment2, SoulFragment3};
                    caster->GetAreaObjectList(list, spellIdList);

                    if (!list.empty())
                    {
                        float dist = GetSpellInfo()->Effects[EFFECT_0]->BasePoints;

                        for (auto itr : list)
                        {
                            if (caster->GetDistance(itr) <= dist)
                                return SPELL_FAILED_SUCCESS;
                        }
                    }
                }

                return SPELL_FAILED_DONT_REPORT;
            }

            void HandleOnCast()
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                std::list<AreaTrigger*> list;
                std::vector<uint32> spellIdList = {SoulFragment1, SoulFragment2, SoulFragment3};
                caster->GetAreaObjectList(list, spellIdList);

                uint32 fragmentCount = 0;
                float range = GetSpellInfo()->Effects[EFFECT_0]->BasePoints;
                for (AreaTrigger* fragment : list)
                {
                    if (caster->GetDistance(fragment) > range)
                        continue;

                    fragment->CastAction();
                    ++fragmentCount;
                }

                if (!fragmentCount)
                    return;

                // The fragments are consumed now, the explosion follows one second later.
                caster->AddDelayedEvent(1000, [caster, fragmentCount, mapId = caster->GetMapId(), instanceId = caster->GetInstanceId()]()
                {
                    if (!caster->IsInWorld() || caster->GetMapId() != mapId || caster->GetInstanceId() != instanceId)
                        return;

                    float multiplier = float(fragmentCount);
                    caster->CastCustomSpell(caster, 247455, nullptr, nullptr, &multiplier, true);
                });
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_dh_spirit_bomb_SpellScript::CheckCast);
                OnCast += SpellCastFn(spell_dh_spirit_bomb_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dh_spirit_bomb_SpellScript();
        }
};

// Nether Bond - 207810
class spell_dh_nether_bond_dummy : public SpellScriptLoader
{
public:
    spell_dh_nether_bond_dummy() : SpellScriptLoader("spell_dh_nether_bond_dummy") { }

    class sspell_dh_nether_bond_dummy_AuraScript : public AuraScript
    {
        PrepareAuraScript(sspell_dh_nether_bond_dummy_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Unit* target = GetTarget())
            {
                float casterPct = caster->GetHealthPct();
                float targetPct = target->GetHealthPct();
                float basePct = (casterPct + targetPct) / 2;
                float casterDamage = 0, targetDamage = 0;

                float targetHeal = target->CountPctFromMaxHealth(int32(basePct));
                float casterHeal = caster->CountPctFromMaxHealth(int32(basePct));

                if(target->GetHealth() > uint32(targetHeal))
                {
                    targetDamage = target->GetHealth() - targetHeal;
                    targetHeal = 0;
                }
                else
                    targetHeal -= target->GetHealth();

                if(caster->GetHealth() > uint32(casterHeal))
                {
                    casterDamage = caster->GetHealth() - casterHeal;
                    casterHeal = 0;
                }
                else
                    casterHeal -= caster->GetHealth();

                if (casterDamage || casterHeal)
                    caster->CastCustomSpell(caster, 207812, &casterDamage, &casterHeal, NULL, true);
                if (targetDamage || targetHeal)
                    caster->CastCustomSpell(target, 207812, &targetDamage, &targetHeal, NULL, true);

                if (Aura* aura = caster->GetAura(207811))
                    aura->AddEffectTarget(target->GetGUID());
            }
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(sspell_dh_nether_bond_dummy_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY , AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new sspell_dh_nether_bond_dummy_AuraScript();
    }
};

// Nether Bond - 207811
class spell_dh_nether_bond : public SpellScriptLoader
{
public:
    spell_dh_nether_bond() : SpellScriptLoader("spell_dh_nether_bond") { }

    class sspell_dh_nether_bond_AuraScript : public AuraScript
    {
        PrepareAuraScript(sspell_dh_nether_bond_AuraScript);

        void OnTick(AuraEffect const* /*aurEff*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Unit* target = ObjectAccessor::GetUnit(*caster, GetAura()->GetRndEffectTarget()))
            {
                float basePct = (caster->GetHealthPct() + target->GetHealthPct()) / 2;
                float casterDamage = 0, targetDamage = 0;

                float targetHeal = target->CountPctFromMaxHealth(int32(basePct));
                float casterHeal = caster->CountPctFromMaxHealth(int32(basePct));

                if(target->GetHealth() > uint32(targetHeal))
                {
                    targetDamage = target->GetHealth() - targetHeal;
                    targetHeal = 0;
                }
                else
                    targetHeal -= target->GetHealth();

                if(caster->GetHealth() > uint32(casterHeal))
                {
                    casterDamage = caster->GetHealth() - casterHeal;
                    casterHeal = 0;
                }
                else
                    casterHeal -= caster->GetHealth();

                if (casterDamage || casterHeal)
                    caster->CastCustomSpell(caster, 207812, &casterDamage, &casterHeal, NULL, true);
                if (targetDamage || targetHeal)
                    caster->CastCustomSpell(target, 207812, &targetDamage, &targetHeal, NULL, true);
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(sspell_dh_nether_bond_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new sspell_dh_nether_bond_AuraScript();
    }
};

// Soul Barrier - 227225
class spell_dh_soul_barrier : public SpellScriptLoader
{
    public:
        spell_dh_soul_barrier() : SpellScriptLoader("spell_dh_soul_barrier") { }

        class spell_dh_soul_barrier_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dh_soul_barrier_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, float& amount, bool& canBeRecalculated)
            {
                Unit* caster = GetCaster();
                if (Player* plr = caster ? caster->ToPlayer() : nullptr)
                {
                    // Fragments are consumed here and later ones raise the pool, so a recalculation must not redo it.
                    canBeRecalculated = false;
                    amount = GetSpellInfo()->Effects[EFFECT_0]->BonusCoefficientFromAP * plr->GetTotalAttackPowerValue(BASE_ATTACK);

                    std::list<AreaTrigger*> list;
                    std::vector<uint32> spellIdList = {SoulFragment1, SoulFragment2, SoulFragment3};
                    plr->GetAreaObjectList(list, spellIdList);

                    uint32 count = 0;
                    if(!list.empty())
                    {
                        for (std::list<AreaTrigger*>::iterator itr = list.begin(); itr != list.end(); ++itr)
                        {
                            if(AreaTrigger* areaObj = (*itr))
                                if (plr->GetDistance(areaObj) <= GetSpellInfo()->Effects[EFFECT_3]->BasePoints)
                                {
                                    areaObj->CastAction();
                                    count++;
                                }
                        }
                    }
                    amount += (GetSpellInfo()->Effects[EFFECT_1]->BonusCoefficientFromAP * plr->GetTotalAttackPowerValue(BASE_ATTACK)) * count;
                }
            }

            void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, float& absorbAmount)
            {
                if (AuraEffect* limit = GetAura()->GetEffect(EFFECT_2))
                {
                    if ((aurEff->GetAmount() - int32(dmgInfo.GetDamage())) < limit->GetAmount())
                    {
                        if (aurEff->GetAmount() > limit->GetAmount())
                            absorbAmount = aurEff->GetAmount() - limit->GetAmount();
                        else
                            return;

                        return;
                    }

                    absorbAmount = dmgInfo.GetDamage();
                }
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dh_soul_barrier_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dh_soul_barrier_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dh_soul_barrier_AuraScript();
        }
};

// Darkness - 209426 
class spell_dh_darkness : public SpellScriptLoader
{
    public:
        spell_dh_darkness() : SpellScriptLoader("spell_dh_darkness") { }

        class spell_dh_darkness_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dh_darkness_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, float& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffect* /*AuraEffect**/, DamageInfo& dmgInfo, float& absorbAmount)
            {
                absorbAmount = 0;

                if (!roll_chance_i(GetSpellInfo()->Effects[EFFECT_0]->CalcValue(GetCaster())))
                    return;

                absorbAmount = dmgInfo.GetDamage();
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dh_darkness_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dh_darkness_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dh_darkness_AuraScript();
        }
};

// Reverse Magic (Honor Talent) - 205604
class spell_dh_reverse_magic : public SpellScriptLoader
{
    public:
        spell_dh_reverse_magic() : SpellScriptLoader("spell_dh_reverse_magic") { }

        class spell_dh_reverse_magic_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dh_reverse_magic_SpellScript);

            void HandleOnHit()
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (!caster || !target)
                    return;

                uint32 dispelMask = (1 << DISPEL_MAGIC) | (1 << DISPEL_POISON);
                DispelChargesList dispel_list;
                target->GetDispellableAuraList(caster, dispelMask, dispel_list);
                if (dispel_list.empty())
                    return;

                for (DispelChargesList::iterator itr = dispel_list.begin(); itr != dispel_list.end(); ++itr)
                {
                    Aura* aura = itr->first;
                    if (!aura)
                        continue;
                    Unit* casterBuff = aura->GetCaster();
                    if (casterBuff && caster->GetDistance(casterBuff) <= GetSpellInfo()->Effects[EFFECT_0]->BasePoints)
                    {
                        if (Aura* targetAura = casterBuff->AddAura(aura->GetSpellInfo()->Id, casterBuff, NULL, aura->GetStackAmount(), aura->GetDuration(), aura->GetDuration()))
                        {
                            for (int i = 0; i < MAX_SPELL_EFFECTS; ++i)
                                if (AuraEffect* targetEff = targetAura->GetEffect(i))
                                    if (AuraEffect* Eff = aura->GetEffect(i))
                                    {
                                        targetEff->SetAmount(Eff->GetAmount());
                                        targetEff->SetPeriodicTimer(Eff->GetPeriodicTimer());
                                    }
                        }
                    }
                    target->RemoveAurasDueToSpellByDispel(aura->GetId(), 205604, aura->GetCasterGUID(), caster);
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dh_reverse_magic_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dh_reverse_magic_SpellScript();
        }
};

// Fel Lance (Honor Talent) - 206966
class spell_dh_fel_lance : public SpellScriptLoader
{
    public:
        spell_dh_fel_lance() : SpellScriptLoader("spell_dh_fel_lance") { }

        class spell_dh_fel_lance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dh_fel_lance_SpellScript);

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                    SetHitDamage(target->CountPctFromMaxHealth(GetSpellInfo()->Effects[EFFECT_0]->CalcValue(GetCaster())));
            }

            void Register() override
            {
                OnEffectLaunchTarget += SpellEffectFn(spell_dh_fel_lance_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dh_fel_lance_SpellScript();
        }
};

// Eye of Leotheras (Honor Talent) - 206650
class spell_dh_eye_of_leotheras : public SpellScriptLoader
{
    public:
        spell_dh_eye_of_leotheras() : SpellScriptLoader("spell_dh_eye_of_leotheras") { }

        class spell_dh_eye_of_leotheras_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dh_eye_of_leotheras_SpellScript);

            void HandleOnHit(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    SetHitDamage(target->CountPctFromMaxHealth(sSpellMgr->GetSpellInfo(206649)->Effects[EFFECT_0]->CalcValue(GetCaster())));
                    if (Aura* aura = target->GetAura(206649))
                    {
                        aura->SetMaxDuration(aura->GetMaxDuration());
                        aura->SetDuration(aura->GetMaxDuration());
                    }
                }
            }

            void Register() override
            {
                OnEffectLaunchTarget += SpellEffectFn(spell_dh_eye_of_leotheras_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dh_eye_of_leotheras_SpellScript();
        }
};

// Mana Break (Honor Talent) - 203704
class spell_dh_mana_break : public SpellScriptLoader
{
public:
    spell_dh_mana_break() : SpellScriptLoader("spell_dh_mana_break") { }

    class spell_dh_mana_break_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dh_mana_break_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
            {
                int32 perc = GetSpellInfo()->Effects[EFFECT_0]->CalcValue(GetCaster());
                if (target->GetMaxPower(POWER_MANA) > 0)
                {
                    float mana = 100.0f - target->GetPowerPct(POWER_MANA);
                    perc += int32(mana * 0.2f);
                    if (perc > GetSpellInfo()->Effects[EFFECT_3]->CalcValue(GetCaster()))
                        perc = GetSpellInfo()->Effects[EFFECT_3]->CalcValue(GetCaster());
                }
                SetHitDamage(target->CountPctFromMaxHealth(perc));
            }
        }

        void Register() override
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_dh_mana_break_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dh_mana_break_SpellScript();
    }
};

// Mana Rift (Honor Talent) - 235904
class spell_dh_mana_rift : public SpellScriptLoader
{
    public:
        spell_dh_mana_rift() : SpellScriptLoader("spell_dh_mana_rift") { }

        class spell_dh_mana_rift_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dh_mana_rift_SpellScript);

            void HandleDamage(SpellEffIndex effIndex)
            {
                if (Unit* target = GetHitUnit())
                    SetHitDamage(target->CountPctFromMaxHealth(GetSpellInfo()->Effects[effIndex]->CalcValue(GetCaster())));
            }

            void HandleMana(SpellEffIndex effIndex)
            {
                if (Unit* target = GetHitUnit())
                    SetEffectValue(target->CountPctFromMaxMana(GetSpellInfo()->Effects[effIndex]->CalcValue(GetCaster())));
            }

            void Register() override
            {
                OnEffectLaunchTarget += SpellEffectFn(spell_dh_mana_rift_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnEffectLaunchTarget += SpellEffectFn(spell_dh_mana_rift_SpellScript::HandleMana, EFFECT_1, SPELL_EFFECT_POWER_BURN);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dh_mana_rift_SpellScript();
        }
};

// Anguish - 202443
class spell_dh_anguish : public SpellScriptLoader
{
public:
    spell_dh_anguish() : SpellScriptLoader("spell_dh_anguish") { }

    class sspell_dh_anguish_AuraScript : public AuraScript
    {
        PrepareAuraScript(sspell_dh_anguish_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            // The burst only happens when the debuff runs out.
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                return;

            Unit* caster = GetCaster();
            Unit* target = GetTarget();
            if (!caster || !target)
                return;

            // Stacks travel in the second base point; the first one is the damage itself.
            float stacks = GetStackAmount();
            caster->CastCustomSpell(target, 202446, nullptr, &stacks, nullptr, true);
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(sspell_dh_anguish_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new sspell_dh_anguish_AuraScript();
    }
};

// Anguish - 202446
class spell_dh_anguish_damage : public SpellScriptLoader
{
    public:
        spell_dh_anguish_damage() : SpellScriptLoader("spell_dh_anguish_damage") { }

        class spell_dh_anguish_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dh_anguish_damage_SpellScript);

            void HandleOnHit()
            {
                if (Unit* target = GetHitUnit())
                    SetHitDamage(GetHitDamage() * GetSpellValue()->EffectBasePoints[EFFECT_1]);
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dh_anguish_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dh_anguish_damage_SpellScript();
        }
};

// Consume Soul (Havoc) - 178963, 228532 and the demon variant 202644
// The 35 Fury trigger only exists with Demonic Appetite.
class spell_dh_consume_soul : public SpellScript
{
    PrepareSpellScript(spell_dh_consume_soul);

    void PreventFury(SpellEffIndex effIndex)
    {
        // Only the Fury energize trigger (210041) is gated, the demon damage buff stays.
        if (GetSpellInfo()->Effects[effIndex]->TriggerSpell != 210041)
            return;

        if (Unit* caster = GetCaster())
            if (!caster->HasAura(206478)) // Demonic Appetite
                PreventHitDefaultEffect(effIndex);
    }

    void Register() override
    {
        // The trigger-spell effect fires in the launch phase, with or without a unit target.
        OnEffectLaunch += SpellEffectFn(spell_dh_consume_soul::PreventFury, EFFECT_ALL, SPELL_EFFECT_TRIGGER_SPELL);
        OnEffectLaunchTarget += SpellEffectFn(spell_dh_consume_soul::PreventFury, EFFECT_ALL, SPELL_EFFECT_TRIGGER_SPELL);
    }
};

// Consume Soul (Vengeance) - 203794, 210042
class spell_dh_consume_soul_vengeance : public SpellScript
{
    PrepareSpellScript(spell_dh_consume_soul_vengeance);

    void HandleOnCast()
    {
        // Fragments consumed while Soul Barrier is up add 2.5 attack power each to its absorb pool.
        Unit* caster = GetCaster();
        if (!caster)
            return;

        if (Aura* barrier = caster->GetAura(227225))
            if (AuraEffect* absorb = barrier->GetEffect(EFFECT_0))
                absorb->SetAmount(absorb->GetAmount() + 2.5f * caster->GetTotalAttackPowerValue(BASE_ATTACK));
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dh_consume_soul_vengeance::HandleOnCast);
    }
};

// Rage of the Illidari - 201467
class areatrigger_rage_of_the_illidari : public AreaTriggerScript
{
    public:
    areatrigger_rage_of_the_illidari() : AreaTriggerScript("areatrigger_rage_of_the_illidari") {}

    struct areatrigger_rage_of_the_illidariAI : AreaTriggerAI
    {
        areatrigger_rage_of_the_illidariAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger)
        {
        }

        uint32 savedDamage;
        uint32 savedDiff;
        uint32 updateDelay;
        float posX;
        float posY;
        float posZ;

        void OnCreate() override
        {
            savedDiff = 0;
            savedDamage = 0;
            updateDelay = at->GetAreaTriggerInfo().updateDelay;
            posX = at->GetPositionX();
            posY = at->GetPositionY();
            posZ = at->GetPositionZ();
        }

        uint32 CallSpecialFunction(uint32 damage) override
        {
            savedDamage += damage;
            return 0;
        }

        void OnRemove() override
        {
            if (Unit* caster = at->GetCaster())
            {
                if (AuraEffect* eff = caster->GetAuraEffect(201472, EFFECT_0))
                {
                    caster->CastSpell(posX, posY, posZ, 226948, TriggerCastFlags(TRIGGERED_FULL_MASK | TRIGGERED_CASTED_BY_AREATRIGGER));

                    float bp = CalculatePct(savedDamage, eff->GetAmount());
                    float x = posX;
                    float y = posY;
                    float z = posZ;

                    caster->AddDelayedCombat(500, [caster, bp, x, y, z]() -> void
                    {
                        caster->CastCustomSpell(x, y, z, 217070, &bp, NULL, NULL, TriggerCastFlags(TRIGGERED_FULL_MASK | TRIGGERED_CASTED_BY_AREATRIGGER));
                    });
                }
            }
        }

        void OnUpdate(uint32 diff) override
        {
            savedDiff += diff;

            if (savedDiff >= updateDelay)
            {
                if (Unit* caster = at->GetCaster())
                {
                    caster->CastSpell(posX, posY, posZ, 201628, TriggerCastFlags(TRIGGERED_FULL_MASK | TRIGGERED_CASTED_BY_AREATRIGGER));
                    caster->CastSpell(posX, posY, posZ, 201789, TriggerCastFlags(TRIGGERED_FULL_MASK | TRIGGERED_CASTED_BY_AREATRIGGER));
                }
                savedDiff -= updateDelay;
            }
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new areatrigger_rage_of_the_illidariAI(areatrigger);
    }
};

// Empower Wards - 218256
class spell_dh_empower_wards : public SpellScriptLoader
{
    public:
        spell_dh_empower_wards() : SpellScriptLoader("spell_dh_empower_wards") { }

        class spell_dh_empower_wards_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dh_empower_wards_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, float& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffect* /*AuraEffect**/, DamageInfo& dmgInfo, float& absorbAmount)
            {
                absorbAmount = 0;
                if (Unit* target = GetTarget())
                {
                    AuraEffect const* aurEff = target->GetAuraEffect(218910, EFFECT_0);
                    if (!aurEff)
                        return;

                    float bp0 = dmgInfo.GetDamage();
                    if (AuraEffect const* aurEff3 = target->GetAuraEffect(218713, EFFECT_0))
                        bp0 += aurEff3->GetAmount();

                    float perc = (bp0 * 100) / target->GetMaxHealth();

                    if (perc > aurEff->GetAmount())
                        perc = aurEff->GetAmount();

                    target->CastCustomSpell(target, 218713, &bp0, NULL, NULL, true);
                    target->CastCustomSpell(target, 218561, &perc, NULL, NULL, true);
                }
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dh_empower_wards_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dh_empower_wards_AuraScript::Absorb, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dh_empower_wards_AuraScript();
        }
};

// Fel Rush - 192611
class spell_dh_fel_rush : public SpellScriptLoader
{
    public:
        spell_dh_fel_rush() : SpellScriptLoader("spell_dh_fel_rush") { }

        class spell_dh_fel_rush_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dh_fel_rush_SpellScript);

            uint32 countTarget = 0;
            void HandleOnHit()
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (!caster || !target)
                    return;

                countTarget++;
                if (AuraEffect const* aurEff = caster->GetAuraEffect(209002, EFFECT_0)) // Loramus Thalipedes' Sacrifice
                    SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), aurEff->GetAmount() * countTarget));
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dh_fel_rush_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dh_fel_rush_SpellScript();
        }
};

// 191427
class spell_dh_metamorphosis_main : public SpellScriptLoader
{
public:
    spell_dh_metamorphosis_main() : SpellScriptLoader("spell_dh_metamorphosis_main") { }

    class spell_dh_metamorphosis_main_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dh_metamorphosis_main_SpellScript);

        SpellCastResult CheckElevation()
        {
            Unit* caster = GetCaster();
            WorldLocation* dest = const_cast<WorldLocation*>(GetExplTargetDest());
            Player* player = caster->ToPlayer();

            if (!player || !dest)
                return SPELL_FAILED_DONT_REPORT;

            if (player->GetMap()->IsBattlegroundOrArena())
            {
                if (Battleground* bg = player->GetBattleground())
                {
                    if (bg->GetStatus() != STATUS_IN_PROGRESS)
                        return SPELL_FAILED_NOT_READY;
                }
            }

            // battleground <bad> positions
            if (player->GetMap()->IsBattleground())
            {
                switch (player->GetMapId())
                {
                case 489:
                    if ((dest->GetPositionZ() >= 375.0f) || (player->GetPositionZ() <= 361.0f && dest->GetPositionZ() >= 364.0f && dest->GetPositionY() <= 1370.0f)
                        || (dest->GetPositionY() <= 1266.0f) || (dest->GetPositionX() <= 991.0f && dest->GetPositionZ() >= 368.0f)
                        || (dest->GetPositionY() <= 1396.0f && dest->GetPositionZ() >= 353.0f) || (dest->GetPositionY() >= 1652.0f && dest->GetPositionZ() >= 354.0f))
                        return SPELL_FAILED_NOPATH;
                    break;
                case 529:
                    if (player->GetCurrentAreaID() == 3424)
                        if ((dest->GetPositionX() <= 1199.0f && dest->GetPositionY() <= 1199.0f && dest->GetPositionZ() >= -51.0f) || (dest->GetPositionZ() >= 13.9f))
                            return SPELL_FAILED_NOPATH;
                    if (player->GetCurrentAreaID() == 3421)
                        if ((dest->GetPositionZ() >= -40.0f))
                            return SPELL_FAILED_NOPATH;
                    if (player->GetCurrentAreaID() == 3422)
                        if ((dest->GetPositionZ() >= 18.8f))
                            return SPELL_FAILED_NOPATH;
                    break;
                case 726:
                    if (((dest->GetPositionX() < 1855.0f && dest->GetPositionX() > 1534.0f) && (dest->GetPositionY() < 183.0f && dest->GetPositionY() > 78.0f) && dest->GetPositionZ() >= -4.8f)
                        || ((dest->GetPositionX() < 1926.0f && dest->GetPositionX() > 1818.0f) && (dest->GetPositionY() < 510.0f && dest->GetPositionY() > 407.0f) && dest->GetPositionZ() >= -10.0f))
                        return SPELL_FAILED_NOPATH;
                    break;
                case 998:
                    if ((dest->GetPositionX() < 1733.0f && dest->GetPositionY() > 1394.0f && dest->GetPositionZ() >= 23.4f)
                        || (dest->GetPositionX() > 1824.0f && dest->GetPositionZ() > 20.4f)
                        || ((dest->GetPositionX() > 1719.0f && dest->GetPositionX() < 1846.7f) && (dest->GetPositionY() > 1253.0f
                            && dest->GetPositionY() < 1416.0f) && dest->GetPositionZ() >= 11.0f))
                        return SPELL_FAILED_NOPATH;
                    break;
                case 607:
                    if (((dest->GetPositionX() < 1475.0f && dest->GetPositionX() > 1383.7f) && (dest->GetPositionY() < -177.0f && dest->GetPositionY() > -262.0f) && dest->GetPositionZ() >= 38.0f)
                        || ((dest->GetPositionX() < 1468.0f && dest->GetPositionX() > 1357.7f) && (dest->GetPositionY() < 160.0f && dest->GetPositionY() > 59.0f) && dest->GetPositionZ() >= 36.0f)
                        || ((dest->GetPositionX() < 1262.0f && dest->GetPositionX() > 1158.0f) && (dest->GetPositionY() < 137.0f && dest->GetPositionY() > 29.0f) && dest->GetPositionZ() >= 58.1f)
                        || ((dest->GetPositionX() < 1277.0f && dest->GetPositionX() > 1169.0f) && (dest->GetPositionY() < -149.0f && dest->GetPositionY() > -272.0f) && dest->GetPositionZ() >= 58.0f)
                        || ((dest->GetPositionX() < 1092.0f && dest->GetPositionX() > 1012.0f) && (dest->GetPositionY() < -55.0f && dest->GetPositionY() > -154.0f) && dest->GetPositionZ() >= 89.77f))
                        return SPELL_FAILED_NOPATH;
                    break;
                }
            }

            return SPELL_FAILED_SUCCESS;
        }

        void HandleTriggerSpell(SpellEffIndex effIndex)
        {
            Unit* caster = GetCaster();
            Aura* aur = caster ? caster->GetAura(162264) : nullptr;
            if (!aur)
                return; // not active yet: the effect applies Metamorphosis normally

            // Recasting while active adds a full duration. The effect is stopped here: the
            // trigger would refresh the aura and throw the extension away.
            PreventHitDefaultEffect(effIndex);

            int32 newDuration = aur->GetDuration() + aur->CalcMaxDuration(caster);
            aur->SetMaxDuration(std::max(aur->GetMaxDuration(), newDuration));
            aur->SetDuration(newDuration);
            // A real Metamorphosis cast allows the next Eye Beam to grant Demonic's extension again.
            aur->SetCustomData(0);
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dh_metamorphosis_main_SpellScript::CheckElevation);
            OnEffectLaunch += SpellEffectFn(spell_dh_metamorphosis_main_SpellScript::HandleTriggerSpell, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
            OnEffectLaunchTarget += SpellEffectFn(spell_dh_metamorphosis_main_SpellScript::HandleTriggerSpell, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dh_metamorphosis_main_SpellScript();
    }
};

// 189110
class spell_dh_infernal_strike_main : public SpellScriptLoader
{
public:
    spell_dh_infernal_strike_main() : SpellScriptLoader("spell_dh_infernal_strike_main") { }

    class spell_dh_infernal_strike_main_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dh_infernal_strike_main_SpellScript);

        SpellCastResult CheckElevation()
        {
            Unit* caster = GetCaster();
            WorldLocation* dest = const_cast<WorldLocation*>(GetExplTargetDest());
            Player* player = caster->ToPlayer();

            if (!player || !dest)
                return SPELL_FAILED_DONT_REPORT;

            if (player->GetMap()->IsBattlegroundOrArena())
            {
                if (Battleground* bg = player->GetBattleground())
                {
                    if (bg->GetStatus() != STATUS_IN_PROGRESS)
                        return SPELL_FAILED_NOT_READY;
                }
            }

            // battleground <bad> positions
            if (player->GetMap()->IsBattleground())
            {
                switch (player->GetMapId())
                {
                case 489:
                    if ((dest->GetPositionZ() >= 375.0f) || (player->GetPositionZ() <= 361.0f && dest->GetPositionZ() >= 364.0f && dest->GetPositionY() <= 1370.0f)
                        || (dest->GetPositionY() <= 1266.0f) || (dest->GetPositionX() <= 991.0f && dest->GetPositionZ() >= 368.0f)
                        || (dest->GetPositionY() <= 1396.0f && dest->GetPositionZ() >= 353.0f) || (dest->GetPositionY() >= 1652.0f && dest->GetPositionZ() >= 354.0f))
                        return SPELL_FAILED_NOPATH;
                    break;
                case 529:
                    if (player->GetCurrentAreaID() == 3424)
                        if ((dest->GetPositionX() <= 1199.0f && dest->GetPositionY() <= 1199.0f && dest->GetPositionZ() >= -51.0f) || (dest->GetPositionZ() >= 13.9f))
                            return SPELL_FAILED_NOPATH;
                    if (player->GetCurrentAreaID() == 3421)
                        if ((dest->GetPositionZ() >= -40.0f))
                            return SPELL_FAILED_NOPATH;
                    if (player->GetCurrentAreaID() == 3422)
                        if ((dest->GetPositionZ() >= 18.8f))
                            return SPELL_FAILED_NOPATH;
                    break;
                case 726:
                    if (((dest->GetPositionX() < 1855.0f && dest->GetPositionX() > 1534.0f) && (dest->GetPositionY() < 183.0f && dest->GetPositionY() > 78.0f) && dest->GetPositionZ() >= -4.8f)
                        || ((dest->GetPositionX() < 1926.0f && dest->GetPositionX() > 1818.0f) && (dest->GetPositionY() < 510.0f && dest->GetPositionY() > 407.0f) && dest->GetPositionZ() >= -10.0f))
                        return SPELL_FAILED_NOPATH;
                    break;
                case 998:
                    if ((dest->GetPositionX() < 1733.0f && dest->GetPositionY() > 1394.0f && dest->GetPositionZ() >= 23.4f)
                        || (dest->GetPositionX() > 1824.0f && dest->GetPositionZ() > 20.4f)
                        || ((dest->GetPositionX() > 1719.0f && dest->GetPositionX() < 1846.7f) && (dest->GetPositionY() > 1253.0f
                            && dest->GetPositionY() < 1416.0f) && dest->GetPositionZ() >= 11.0f))
                        return SPELL_FAILED_NOPATH;
                    break;
                case 607:
                    if (((dest->GetPositionX() < 1475.0f && dest->GetPositionX() > 1383.7f) && (dest->GetPositionY() < -177.0f && dest->GetPositionY() > -262.0f) && dest->GetPositionZ() >= 38.0f)
                        || ((dest->GetPositionX() < 1468.0f && dest->GetPositionX() > 1357.7f) && (dest->GetPositionY() < 160.0f && dest->GetPositionY() > 59.0f) && dest->GetPositionZ() >= 36.0f)
                        || ((dest->GetPositionX() < 1262.0f && dest->GetPositionX() > 1158.0f) && (dest->GetPositionY() < 137.0f && dest->GetPositionY() > 29.0f) && dest->GetPositionZ() >= 58.1f)
                        || ((dest->GetPositionX() < 1277.0f && dest->GetPositionX() > 1169.0f) && (dest->GetPositionY() < -149.0f && dest->GetPositionY() > -272.0f) && dest->GetPositionZ() >= 58.0f)
                        || ((dest->GetPositionX() < 1092.0f && dest->GetPositionX() > 1012.0f) && (dest->GetPositionY() < -55.0f && dest->GetPositionY() > -154.0f) && dest->GetPositionZ() >= 89.77f))
                        return SPELL_FAILED_NOPATH;
                    break;
                }
            }

            return SPELL_FAILED_SUCCESS;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dh_infernal_strike_main_SpellScript::CheckElevation);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dh_infernal_strike_main_SpellScript();
    }
};

// 178940, 204254 - Shattered Souls (Havoc), (Vengeance)
class spell_dh_ss : public AuraScript
{
    PrepareAuraScript(spell_dh_ss);

    void OnProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
    {
        if (Unit* caster = GetCaster())
        {
            if (Unit* target = eventInfo.GetActionTarget())
            {
                if (GetId() == 178940)
                {
                    if (target->GetCreatureType() == CREATURE_TYPE_DEMON)
                        caster->CastSpell(caster, 237867, true);
                    else
                        caster->CastSpell(caster, 209651, true);
                }
                else if (GetId() == 204254)
                {
                    if (target->GetCreatureType() == CREATURE_TYPE_DEMON)
                        caster->CastSpell(caster, 210038, true);
                    else
                        caster->CastSpell(caster, 209981, true);
                }
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dh_ss::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 238118 - FLaming Soul
class spell_dh_flaming_soul : public AuraScript
{
    PrepareAuraScript(spell_dh_flaming_soul);

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Unit* caster = GetCaster())
        {
            if (Unit* target = eventInfo.GetProcTarget())
            {
                if (Aura* aura = target->GetAura(caster->HasAura(207739) ? 207771 : 207744, caster->GetGUID()))
                {
                    int32 duration = aura->GetDuration() + aurEff->GetAmount();
                    aura->SetMaxDuration(std::max(aura->GetMaxDuration(), duration));
                    aura->SetDuration(duration);

                    if (Aura* aur = target->GetAura(212818, caster->GetGUID()))
                    {
                        aur->SetMaxDuration(std::max(aur->GetMaxDuration(), duration));
                        aur->SetDuration(duration);
                    }
                }
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dh_flaming_soul::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 198013 - Eye Beam
class spell_dh_eye_beam : public AuraScript
{
    PrepareAuraScript(spell_dh_eye_beam);

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        if (!caster || !caster->HasAura(213410)) // Demonic
            return;

        if (Aura* aur = caster->GetAura(162264))
        {
            // Only the first Eye Beam of the current Metamorphosis window extends it.
            if (aur->GetCustomData() != 0)
                return;

            int32 newDuration = aur->GetDuration() + 8000;
            aur->SetMaxDuration(std::max(aur->GetMaxDuration(), newDuration));
            aur->SetDuration(newDuration);
            aur->SetCustomData(1);
        }
        else
        {
            // Metamorphosis starts with the channel, so the channel benefits from it, and lasts eight seconds past it.
            uint32 duration = 8000 + std::max(0, GetAura()->GetMaxDuration());
            caster->CastSpellDuration(caster, 162264, true, duration);

            if (Aura* meta = caster->GetAura(162264))
                meta->SetCustomData(1);
        }
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        // Betrayer's Fury (T21 Havoc 4P) only follows a channel that ran to its end.
        if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
            return;

        Unit* caster = GetTarget();
        if (caster && caster->HasAura(251769))
            caster->CastSpell(caster, 252165, true);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_dh_eye_beam::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_dh_eye_beam::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

// 203783 - Shear fragment generation
class spell_dh_shear_fragment : public AuraScript
{
    PrepareAuraScript(spell_dh_shear_fragment);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Spell const* spell = eventInfo.GetSpell();
        return spell && (spell->GetSpellInfo()->Id == 203782 || spell->GetSpellInfo()->Id == 235964);
    }

    void OnProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        Unit* caster = GetCaster();
        Spell const* spell = eventInfo.GetSpell();
        if (!caster || !spell)
            return;

        // Sever, the Metamorphosis form of Shear, always shatters a Lesser Soul Fragment.
        if (spell->GetSpellInfo()->Id == 235964)
        {
            caster->CastSpell(caster, 209980, true);
            GetAura()->SetCustomData(0);
            return;
        }

        // Chance grows with every failed Shear since the last fragment; custom data counts them.
        static float const shatterChance[8] = {4.0f, 12.0f, 25.0f, 40.0f, 60.0f, 80.0f, 90.0f, 100.0f};
        uint32 failedAttempts = std::min<uint32>(GetAura()->GetCustomData(), 7);
        float chance = shatterChance[failedAttempts] * 1.10f;

        // Shatter the Souls (artifact trait) below half health.
        if (caster->GetHealthPct() < 50.0f)
            if (AuraEffect const* shatterSouls = caster->GetAuraEffect(212827, EFFECT_0))
                chance += shatterSouls->GetAmount();

        if (roll_chance_f(chance))
        {
            caster->CastSpell(caster, 209980, true);
            GetAura()->SetCustomData(0);
        }
        else
            GetAura()->SetCustomData(failedAttempts + 1);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dh_shear_fragment::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_dh_shear_fragment::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

// 213017 - Fueled by Pain
class spell_dh_fueled_by_pain : public AuraScript
{
    PrepareAuraScript(spell_dh_fueled_by_pain);

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Unit* caster = eventInfo.GetActor())
        {
            uint32 basedur = uint32(aurEff->GetAmount()) * uint32(IN_MILLISECONDS);
            if (AuraEffect* eff = caster->GetAuraEffect(238046, EFFECT_0)) // Lingering Ordeal
                basedur += eff->GetAmount();

            if (Aura* aura = caster->GetAura(187827))
            {
                int32 _duration = int32(aura->GetDuration() + basedur);
                aura->SetMaxDuration(std::max(aura->GetMaxDuration(), _duration));
                aura->SetDuration(_duration);
            }
            else
                caster->CastSpellDuration(caster, 187827, true, basedur);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dh_fueled_by_pain::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 205629 - Demonic Trample (Honor Talent)
class spell_dh_demonic_trample : public AuraScript
{
    PrepareAuraScript(spell_dh_demonic_trample);

    void CalculateAmount(AuraEffect const* /*aurEff*/, float& amount, bool& /*canBeRecalculated*/)
    {
        if (Unit* caster = GetCaster())
            if (caster->HasAuraType(SPELL_AURA_BATTLEGROUND_FLAG) || caster->HasAuraType(SPELL_AURA_BATTLEGROUND_FLAG_2))
                amount /= 2.f;
    }

    void CalculateAmount1(AuraEffect const* /*aurEff*/, float& amount, bool& /*canBeRecalculated*/)
    {
        if (Unit* caster = GetCaster())
            if (caster->HasAuraType(SPELL_AURA_BATTLEGROUND_FLAG) || caster->HasAuraType(SPELL_AURA_BATTLEGROUND_FLAG_2))
                amount /= 1.5f;
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dh_demonic_trample::CalculateAmount, EFFECT_2, SPELL_AURA_MOD_SPEED_NOT_STACK);
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dh_demonic_trample::CalculateAmount1, EFFECT_3, SPELL_AURA_MOD_MINIMUM_SPEED);
    }
};

// Until patch 9.0.1 a demon hunter started with Artisan Riding: given at login to those created without it
class player_dh_artisan_riding : public PlayerScript
{
public:
    player_dh_artisan_riding() : PlayerScript("player_dh_artisan_riding") { }

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        if (player->getClass() != CLASS_DEMON_HUNTER || player->HasSpell(SPELL_ARTISAN_RIDING))
            return;

        for (uint32 spellId : { SPELL_APPRENTICE_RIDING, SPELL_JOURNEYMAN_RIDING, SPELL_EXPERT_RIDING, SPELL_ARTISAN_RIDING })
            if (!player->HasSpell(spellId))
                player->learnSpell(spellId, false);
    }

private:
    static constexpr uint32 SPELL_APPRENTICE_RIDING  = 33388;
    static constexpr uint32 SPELL_JOURNEYMAN_RIDING  = 33391;
    static constexpr uint32 SPELL_EXPERT_RIDING      = 34090;
    static constexpr uint32 SPELL_ARTISAN_RIDING     = 34091;
};

void AddSC_demonhunter_spell_scripts()
{
    new player_dh_artisan_riding();
    new spell_dh_glide();
    new spell_dh_disable_absorb();
    RegisterSpellScript(spell_dh_blade_dance);
    new spell_dh_desperate_instincts();
    new spell_dh_soul_cleave();
    new spell_dh_spirit_bomb();
    new spell_dh_nether_bond_dummy();
    new spell_dh_nether_bond();
    new spell_dh_soul_barrier();
    new spell_dh_soul_cleave_damage();
    new spell_dh_darkness();
    new spell_dh_reverse_magic();
    new spell_dh_fel_lance();
    new spell_dh_eye_of_leotheras();
    new spell_dh_mana_break();
    new spell_dh_illidans_grasp_throw();
    new spell_dh_mana_rift();
    new spell_dh_anguish();
    new spell_dh_anguish_damage();
    RegisterSpellScript(spell_dh_consume_soul);
    RegisterSpellScript(spell_dh_consume_soul_vengeance);
    RegisterSpellScript(spell_dh_demon_spikes);
    RegisterAuraScript(spell_dh_razor_spikes);
    new spell_dh_empower_wards();
    new areatrigger_rage_of_the_illidari();
    RegisterSpellScript(spell_dh_shatter_soul);
    new spell_dh_fel_rush();
    new spell_dh_fel_rush_main();
    new spell_dh_metamorphosis();
    new spell_dh_nemesis();
    new spell_dh_spirit_bomb_damage();
    new spell_dh_illidans_grasp_visual();
    new spell_dh_metamorphosis_main();
    new spell_dh_infernal_strike_main();
    RegisterAuraScript(spell_dh_ss);
    RegisterAuraScript(spell_dh_shear_fragment);
    RegisterAuraScript(spell_dh_flaming_soul);
    RegisterAuraScript(spell_dh_eye_beam);
    RegisterAuraScript(spell_dh_fueled_by_pain);
    RegisterAuraScript(spell_dh_demonic_trample);
}