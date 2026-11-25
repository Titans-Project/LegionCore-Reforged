////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"
#include "CellImpl.h"
#include "GameObjectAI.h"
#include "ObjectVisitors.hpp"
#include "LFGMgr.h"
#include "Group.h"
#include "EasternKingdoms/Dungeons/ShadowfangKeep/shadowfang_keep.h"

// 45102 Romantic Picnic
enum SpellsPicnic
{
    SPELL_BASKET_CHECK              = 45119, // Holiday - Valentine - Romantic Picnic Near Basket Check
    SPELL_MEAL_PERIODIC             = 45103, // Holiday - Valentine - Romantic Picnic Meal Periodic - effect dummy
    SPELL_MEAL_EAT_VISUAL           = 45120, // Holiday - Valentine - Romantic Picnic Meal Eat Visual
    //SPELL_MEAL_PARTICLE             = 45114, // Holiday - Valentine - Romantic Picnic Meal Particle - unused
    SPELL_DRINK_VISUAL              = 45121, // Holiday - Valentine - Romantic Picnic Drink Visual
    SPELL_ROMANTIC_PICNIC_ACHIEV    = 45123, // Romantic Picnic periodic = 5000
};

class spell_love_is_in_the_air_romantic_picnic : public SpellScriptLoader
{
    public:
        spell_love_is_in_the_air_romantic_picnic() : SpellScriptLoader("spell_love_is_in_the_air_romantic_picnic") { }

        class spell_love_is_in_the_air_romantic_picnic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_love_is_in_the_air_romantic_picnic_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                target->SetStandState(UNIT_STAND_STATE_SIT);
                target->CastSpell(target, SPELL_MEAL_PERIODIC, false);
            }

            void OnPeriodic(AuraEffect const* /*aurEff*/)
            {
                // Every 5 seconds
                Unit* target = GetTarget();
                Unit* caster = GetCaster();

                // If our player is no longer sit, remove all auras
                if (target->getStandState() != UNIT_STAND_STATE_SIT)
                {
                    target->RemoveAura(SPELL_ROMANTIC_PICNIC_ACHIEV);
                    target->RemoveAura(GetAura());
                    return;
                }

                target->CastSpell(target, SPELL_BASKET_CHECK, false); // unknown use, it targets Romantic Basket
                target->CastSpell(target, RAND(SPELL_MEAL_EAT_VISUAL, SPELL_DRINK_VISUAL), false);

                bool foundSomeone = false;
                // For nearby players, check if they have the same aura. If so, cast Romantic Picnic (45123)
                // required by achievement and "hearts" visual
                std::list<Player*> playerList;
                Trinity::AnyPlayerInObjectRangeCheck checker(target, INTERACTION_DISTANCE*2);
                Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(target, playerList, checker);
                Trinity::VisitNearbyWorldObject(target, INTERACTION_DISTANCE*2, searcher);
                for (std::list<Player*>::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                {
                    if ((*itr) != target && (*itr)->HasAura(GetId())) // && (*itr)->getStandState() == UNIT_STAND_STATE_SIT)
                    {
                        if (caster)
                        {
                            caster->CastSpell(*itr, SPELL_ROMANTIC_PICNIC_ACHIEV, true);
                            caster->CastSpell(target, SPELL_ROMANTIC_PICNIC_ACHIEV, true);
                        }
                        foundSomeone = true;
                        // break;
                    }
                }

                if (!foundSomeone && target->HasAura(SPELL_ROMANTIC_PICNIC_ACHIEV))
                    target->RemoveAura(SPELL_ROMANTIC_PICNIC_ACHIEV);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_love_is_in_the_air_romantic_picnic_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_love_is_in_the_air_romantic_picnic_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_love_is_in_the_air_romantic_picnic_AuraScript();
        }
};

class npc_apothecary_hummel : public CreatureScript
{
    public:
        npc_apothecary_hummel() : CreatureScript("npc_apothecary_hummel") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_apothecary_hummelAI (pCreature);
        }

        bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
        {
            pPlayer->PlayerTalkClass->ClearMenus();
            pPlayer->CLOSE_GOSSIP_MENU();

            if (uiAction == 1)
            {
                if (Creature* pFrye = pCreature->FindNearestCreature(NPC_APOTHECARY_FRYE, 100.0f))
                    pFrye->setFaction(14);
                if (Creature* pBaxter = pCreature->FindNearestCreature(NPC_APOTHECARY_BAXTER, 100.0f))
                    pBaxter->setFaction(14);
                pCreature->setFaction(14);
            }
            return false;
        }

        struct npc_apothecary_hummelAI : public ScriptedAI
        {
            npc_apothecary_hummelAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
            }

            void JustDied(Unit* /*killer*/)
            {
                Map::PlayerList const& players = me->GetMap()->GetPlayers();
                if (!players.isEmpty())
                {
                    Player* pPlayer = players.begin()->getSource();
                    if (pPlayer && pPlayer->GetGroup())
                        sLFGMgr->FinishDungeon(pPlayer->GetGroup()->GetGUID(), 288);
                }
            }
        };
};

void AddSC_love_is_in_the_air()
{
    new spell_love_is_in_the_air_romantic_picnic();
    new npc_apothecary_hummel();
}
