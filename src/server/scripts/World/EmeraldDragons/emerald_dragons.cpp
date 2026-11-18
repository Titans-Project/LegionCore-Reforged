////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    The Emerald Dragons: the Dream Fog they leave behind and its sleep, common to all four
*/

#include "emerald_dragons.h"

/*
 * --- NPC: Dream Fog
 */

class npc_dream_fog : public CreatureScript
{
    public:
        npc_dream_fog() : CreatureScript("npc_dream_fog") { }

        struct npc_dream_fogAI : public ScriptedAI
        {
            npc_dream_fogAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset() override
            {
                _roamTimer = 0;
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (!_roamTimer)
                {
                    // Chase target, but don't attack - otherwise just roam around
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                    {
                        _roamTimer = urand(15000, 30000);
                        me->GetMotionMaster()->Clear(false);
                        me->GetMotionMaster()->MoveChase(target, 0.2f);
                    }
                    else
                    {
                        _roamTimer = 2500;
                        me->GetMotionMaster()->Clear(false);
                        me->GetMotionMaster()->MoveRandom(25.0f);
                    }
                    // Seeping fog movement is slow enough for a player to be able to walk backwards and still outpace it
                    me->SetWalk(true);
                    me->SetSpeed(MOVE_WALK, 0.75f);
                }
                else
                    _roamTimer -= diff;
            }

        private:
            uint32 _roamTimer;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_dream_fogAI(creature);
        }
};

/*
 * --- Spell: Dream Fog
 */

class DreamFogTargetSelector
{
    public:
        DreamFogTargetSelector() { }

        bool operator()(WorldObject* object) const
        {
            return object->ToUnit() && object->ToUnit()->HasAura(SPELL_SLEEP);
        }
};

class spell_dream_fog_sleep : public SpellScriptLoader
{
    public:
        spell_dream_fog_sleep() : SpellScriptLoader("spell_dream_fog_sleep") { }

        class spell_dream_fog_sleep_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dream_fog_sleep_SpellScript);

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                unitList.remove_if(DreamFogTargetSelector());
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dream_fog_sleep_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dream_fog_sleep_SpellScript();
        }
};

void AddSC_emerald_dragons()
{
    new npc_dream_fog();
    new spell_dream_fog_sleep();
}
