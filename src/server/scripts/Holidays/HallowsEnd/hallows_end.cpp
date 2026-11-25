////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "QuestData.h"
#include "AreaTriggerAI.h"
#include "AreaTrigger.h"
#include "BattlegroundMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "Garrison.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "InstanceScript.h"
#include "LFGMgr.h"
#include "Packets/ArtifactPackets.h"
#include "Packets/SpellPackets.h"
#include "Packets/NPCPackets.h"
#include "SkillDiscovery.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "Vehicle.h"
#include "GameTime.h"
#include "ObjectVisitors.hpp"

// Quest: Clean Up in Stormwind - 29144 and Clean Up in Undercity - 29375
class go_forsaken_stink_bomb : public GameObjectScript
{
    public:
        go_forsaken_stink_bomb() : GameObjectScript("go_forsaken_stink_bomb") { }

        bool OnGossipHello(Player* player, GameObject* go) override
        {
            go->SetLootState(GO_JUST_DEACTIVATED);
            player->CastSpell(player, 97157, true);
            return true;
        }
};

// 24751 Trick or Treat
enum TrickOrTreatSpells
{
    SPELL_TRICK                 = 24714,
    SPELL_TREAT                 = 24715,
    SPELL_TRICKED_OR_TREATED    = 24755,
    SPELL_TRICKY_TREAT_SPEED    = 42919,
    SPELL_TRICKY_TREAT_TRIGGER  = 42965,
    SPELL_UPSET_TUMMY           = 42966
};

class spell_gen_trick_or_treat : public SpellScriptLoader
{
    public:
        spell_gen_trick_or_treat() : SpellScriptLoader("spell_gen_trick_or_treat") {}

        class spell_gen_trick_or_treat_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_trick_or_treat_SpellScript);

            bool Validate(SpellInfo const* /*SpellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_TRICK) || !sSpellMgr->GetSpellInfo(SPELL_TREAT) || !sSpellMgr->GetSpellInfo(SPELL_TRICKED_OR_TREATED))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Player* target = GetHitPlayer())
                {
                    caster->CastSpell(target, roll_chance_i(50) ? SPELL_TRICK : SPELL_TREAT, true, NULL);
                    caster->CastSpell(target, SPELL_TRICKED_OR_TREATED, true, NULL);
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_trick_or_treat_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_trick_or_treat_SpellScript();
        }
};

// Hallowen: Q29075 Q29376
enum hallow
{
    GO_THE_WICKERMAN        = 180433, //The Wickerman
    GO_WICKERMAN_EMBER      = 180437, //Wickerman Ember
    GO_TORCH                = 208186,
    Q29075                  = 29075,
    Q29376                  = 29376,
};

enum evens
{
    ENABLE_WICKERMAN            = 1,
    END_BURN_WICKERMAN          = 2,
};

class spell_hallowen_torch_wickerman : public SpellScriptLoader
{
    public:
        spell_hallowen_torch_wickerman() : SpellScriptLoader("spell_hallowen_torch_wickerman") { }

        class sspell_hallowen_torch_wickerman_SpellScript : public SpellScript
        {
            PrepareSpellScript(sspell_hallowen_torch_wickerman_SpellScript);

            void HandleOnHit(SpellEffIndex /*effIndex*/)
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (GameObject* go = _player->FindNearestGameObject(GO_THE_WICKERMAN, 100.0f))
                    {
                        go->AI()->DoAction(ENABLE_WICKERMAN);
                        go->AI()->SetGUID(_player->GetGUID(), 0);
                    }
                }
            }

            void Register() override
            {
                OnEffectLaunchTarget += SpellEffectFn(sspell_hallowen_torch_wickerman_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new sspell_hallowen_torch_wickerman_SpellScript();
        }
};

class go_hallow_wickerman : public GameObjectScript
{
public:
    go_hallow_wickerman() : GameObjectScript("go_hallow_wickerman") { }

    struct go_hallow_wickermanAI : public GameObjectAI
    {
        go_hallow_wickermanAI(GameObject* go) : GameObjectAI(go)
        {
            emberGUID.Clear();
            playerGUID.Clear();
        }

        EventMap events;
        ObjectGuid emberGUID;
        ObjectGuid playerGUID;

        void SetGUID(ObjectGuid const& guid, int32 /*id = 0 */) override
        {
            if (playerGUID)
                return;

            playerGUID = guid;
        }

        void DoAction(const int32 param) override
        {
            if (playerGUID)
                return;

            events.RescheduleEvent(END_BURN_WICKERMAN, 10000);
            go->EnableOrDisableGo(true, true);
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case END_BURN_WICKERMAN:
                    {
                        go->EnableOrDisableGo(false, false);

                        if (Player* player = ObjectAccessor::GetPlayer(*go, playerGUID))
                            if (GameObject * e = player->SummonGameObject(GO_WICKERMAN_EMBER, go->GetPositionX(), go->GetPositionY(), go->GetPositionZ(), go->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, DAY))
                                emberGUID = e->GetGUID();                            
                        events.RescheduleEvent(ENABLE_WICKERMAN, 20000);
                        go->UpdateObjectVisibility();
                        go->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM, SEC_GAMEMASTER);
                        break;
                    }
                    case ENABLE_WICKERMAN:
                    {
                        playerGUID.Clear();
                        go->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM, SEC_PLAYER);
                        go->UpdateObjectVisibility();
                        if (GameObject* e = ObjectAccessor::GetGameObject(*go, emberGUID))
                        {
                            e->Delete();
                            emberGUID.Clear();
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    };
    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_hallow_wickermanAI(go);
    }
};

class spell_gen_tricky_treat : public SpellScriptLoader
{
    public:
        spell_gen_tricky_treat() : SpellScriptLoader("spell_gen_tricky_treat") {}

        class spell_gen_tricky_treat_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_tricky_treat_SpellScript);

            bool Validate(SpellInfo const* /*SpellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_TRICKY_TREAT_SPEED))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_TRICKY_TREAT_TRIGGER))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_UPSET_TUMMY))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                    if (caster->HasAura(SPELL_TRICKY_TREAT_TRIGGER) && caster->GetAuraCount(SPELL_TRICKY_TREAT_SPEED) > 3 && roll_chance_i(33))
                        caster->CastSpell(caster, SPELL_UPSET_TUMMY, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_tricky_treat_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_tricky_treat_SpellScript();
        }
};

void AddSC_hallows_end()
{
    new spell_gen_tricky_treat();
    new go_forsaken_stink_bomb();
    new spell_gen_trick_or_treat();
    new spell_hallowen_torch_wickerman();
    new go_hallow_wickerman();
}
