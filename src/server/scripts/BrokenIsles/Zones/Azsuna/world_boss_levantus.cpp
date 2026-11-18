////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Azsuna: Levantus, world boss
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum eSpells
{
    SPELL_GUST_OF_WIND = 217211,
    SPELL_RENDING_WHIRL = 217235,
    SPELL_MASSIVE_SPOUL = 217249,
    SPELL_ELECTRIFY = 217344,
    SPELL_TURBULENT_VORTEX = 217360,
    SPELL_RAMPAGING_TORRENT = 217229,
};

// 108829
class boss_levantus : public CreatureScript
{
public:
    boss_levantus() : CreatureScript("boss_levantus") {}

    struct boss_levantusAI : public ScriptedAI
    {
        boss_levantusAI(Creature* creature) : ScriptedAI(creature), summons(me)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_REMOVE_CLIENT_CONTROL);
        }

        EventMap events;
        SummonList summons;
        void Reset() override
        {
            events.Reset();
            summons.DespawnAll();
            me->RemoveAllAreaObjects();
            me->SetReactState(REACT_AGGRESSIVE);
        }

        void EnterCombat(Unit* unit) override
        {
           // DoCast(SPELL_GUST_OF_WIND);
            events.RescheduleEvent(EVENT_1, urand(7000, 8000)); //SPELL_RENDING_WHIRL + 40
            events.RescheduleEvent(EVENT_2, urand(25000, 27000)); //SPELL_MASSIVE_SPOUL + 43
            events.RescheduleEvent(EVENT_3, urand(50000, 51000)); //SPELL_ELECTRIFY + 43  (доделать)
            events.RescheduleEvent(EVENT_4, 11000); // SPELL_TURBULENT_VORTEX
            events.RescheduleEvent(EVENT_5, 1000); //check
            events.RescheduleEvent(EVENT_6, 3000); // not repeat
        }

        void EnterEvadeMove()
        {
            me->SetReactState(REACT_AGGRESSIVE);
            summons.DespawnAll();
        }

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);
        }

        void SpellHit(Unit* /*caster */, SpellInfo const* spell) override
        {
            if (spell->Id == SPELL_ELECTRIFY)
            {
                std::list<HostileReference*> threatlist = me->getThreatManager().getThreatList();
                if (!threatlist.empty())
                {
                    for (std::list<HostileReference*>::const_iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
                    {
                        if (Player* pl = me->GetPlayer(*me, (*itr)->getUnitGuid()))
                        {
                            if (pl && pl->GetPositionZ() < 0)
                                me->CastSpell(pl, 217352, true);
                        }
                    }
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_RENDING_WHIRL);
                        events.RescheduleEvent(EVENT_1, urand(39000, 41000)); //SPELL_RENDING_WHIRL + 40
                        break;
                    case EVENT_2:
                        DoCast(SPELL_MASSIVE_SPOUL);
                        events.RescheduleEvent(EVENT_2, urand(39000, 41000)); //SPELL_MASSIVE_SPOUL + 43
                        break;
                    case EVENT_3:
                        DoCast(SPELL_ELECTRIFY);
                        events.RescheduleEvent(EVENT_3, urand (39000, 41000));
                        break;
                    case EVENT_4:
                        DoCast(SPELL_TURBULENT_VORTEX);
                        events.RescheduleEvent(EVENT_4, 10000);
                        break;
                    case EVENT_5:
                        if (auto target = me->FindNearestPlayer(100.0f))
                            if (!me->IsWithinMeleeRange(target))
                                DoCast(SPELL_RAMPAGING_TORRENT);
                        events.RescheduleEvent(EVENT_5, 1000); //check
                        break;
                    case EVENT_6:
                        for (int8 i = 0; i < 25; i++)
                            me->SummonCreature(109211, me->GetPositionX() + irand (-30, 30), me->GetPositionY() + irand (-30, 30), 0.0f, 0.0f);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_levantusAI(creature);
    }
};

// 217249
class spell_massive_spoul : public SpellScriptLoader
{
    public:
        spell_massive_spoul() : SpellScriptLoader("spell_massive_spoul") {}

        class spell_massive_spoul_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_massive_spoul_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetCaster() && GetCaster()->ToCreature())
                {
                    GetCaster()->GetMotionMaster()->MoveRotate(20000, ROTATE_DIRECTION_RIGHT);
                    GetCaster()->ToCreature()->AttackStop();
                    GetCaster()->ToCreature()->SetReactState(REACT_PASSIVE);
                }
            }

            void HandleEffectRemove(AuraEffect const * /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetCaster() && GetCaster()->ToCreature())
                {
                    GetCaster()->StopMoving();
                    GetCaster()->GetMotionMaster()->Clear(false);
                    GetCaster()->ToCreature()->SetReactState(REACT_AGGRESSIVE);
                    GetCaster()->ToCreature()->AI()->DoZoneInCombat(GetCaster()->ToCreature(), 150.0f);

                }
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_massive_spoul_AuraScript::OnApply, EFFECT_0, SPELL_AURA_CREATE_AREATRIGGER, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_massive_spoul_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_CREATE_AREATRIGGER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_massive_spoul_AuraScript();
        }
};

void AddSC_world_boss_levantus()
{
    new boss_levantus();
    new spell_massive_spoul();
}
