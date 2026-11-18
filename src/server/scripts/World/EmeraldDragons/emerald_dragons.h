////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

// The four Emerald Dragons: what they share (spells, events, their common AI) and the Mark of Nature target selector

#ifndef EMERALD_DRAGONS_H
#define EMERALD_DRAGONS_H

/*
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "Spell.h"
#include "SpellAuraEffects.h"

//
//  Emerald Dragon NPCs and IDs (kept here for reference)
//

enum EmeraldDragonNPC
{
    NPC_DREAM_FOG                   = 15224,
    DRAGON_YSONDRE                  = 14887,
    DRAGON_LETHON                   = 14888,
    DRAGON_EMERISS                  = 14889,
    DRAGON_TAERAR                   = 14890,
};

//
// Emerald Dragon Spells (used for the dragons)
//

enum EmeraldDragonSpells
{
    SPELL_TAIL_SWEEP                = 15847,    // tail sweep - slap everything behind dragon (2 seconds interval)
    SPELL_SUMMON_PLAYER             = 24776,    // teleport highest threat player in front of dragon if wandering off
    SPELL_DREAM_FOG                 = 24777,    // auraspell for Dream Fog NPC (15224)
    SPELL_SLEEP                     = 24778,    // sleep triggerspell (used for Dream Fog)
    SPELL_SEEPING_FOG_LEFT          = 24813,    // dream fog - summon left
    SPELL_SEEPING_FOG_RIGHT         = 24814,    // dream fog - summon right
    SPELL_NOXIOUS_BREATH            = 24818,
    SPELL_MARK_OF_NATURE            = 25040,    // Mark of Nature trigger (applied on target death - 15 minutes of being suspectible to Aura Of Nature)
    SPELL_MARK_OF_NATURE_AURA       = 25041,    // Mark of Nature (passive marker-test, ticks every 10 seconds from boss, triggers spellID 25042 (scripted)
    SPELL_AURA_OF_NATURE            = 25043,    // Stun for 2 minutes (used when SPELL_MARK_OF_NATURE exists on the target)
};

//
// Emerald Dragon Eventlists (shared and specials)
//

enum Events
{
    // General events for all dragons
    EVENT_SEEPING_FOG = 1,
    EVENT_NOXIOUS_BREATH,
    EVENT_TAIL_SWEEP,

    // Ysondre
    EVENT_LIGHTNING_WAVE,
    EVENT_SUMMON_DRUID_SPIRITS,

    // Lethon
    EVENT_SHADOW_BOLT_WHIRL,

    // Emeriss
    EVENT_VOLATILE_INFECTION,
    EVENT_CORRUPTION_OF_EARTH,

    // Taerar
    EVENT_ARCANE_BLAST,
    EVENT_BELLOWING_ROAR,
};

/*
 * ---
 * --- Emerald Dragons : Base AI-structure used for all the Emerald dragons
 * ---
 */

struct emerald_dragonAI : public WorldBossAI
{
    emerald_dragonAI(Creature* creature) : WorldBossAI(creature)
    {
//        me->m_CombatDistance = 12.0f;
//        me->m_SightDistance  = 60.0f;
    }

    void Reset() override
    {
        _Reset();
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE);
        me->SetReactState(REACT_AGGRESSIVE);
        DoCast(me, SPELL_MARK_OF_NATURE_AURA, true);
        events.ScheduleEvent(EVENT_TAIL_SWEEP, 4000);
        events.ScheduleEvent(EVENT_NOXIOUS_BREATH, urand(7500, 15000));
        events.ScheduleEvent(EVENT_SEEPING_FOG, urand(12500, 20000));
    }

    // Target killed during encounter, mark them as suspectible for Aura Of Nature
    void KilledUnit(Unit* who) override
    {
        who->CastSpell(who, SPELL_MARK_OF_NATURE, true);
    }

    // Execute and reschedule base events shared between all Emerald Dragons
    void ExecuteEvent(uint32 const eventId) override
    {
        switch (eventId)
        {
            case EVENT_SEEPING_FOG:
                // Seeping Fog appears only as "pairs", and only ONE pair at any given time!
                // Despawntime is 2 minutes, so reschedule it for new cast after 2 minutes + a minor "random time" (30 seconds at max)
                DoCast(me, SPELL_SEEPING_FOG_LEFT, true);
                DoCast(me, SPELL_SEEPING_FOG_RIGHT, true);
                events.ScheduleEvent(EVENT_SEEPING_FOG, urand(120000, 150000));
                break;
            case EVENT_NOXIOUS_BREATH:
                // Noxious Breath is cast on random intervals, no less than 7.5 seconds between
                DoCast(me, SPELL_NOXIOUS_BREATH);
                events.ScheduleEvent(EVENT_NOXIOUS_BREATH, urand(7500, 15000));
                break;
            case EVENT_TAIL_SWEEP:
                // Tail Sweep is cast every two seconds, no matter what goes on in front of the dragon
                DoCast(me, SPELL_TAIL_SWEEP);
                events.ScheduleEvent(EVENT_TAIL_SWEEP, 2000);
                break;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
            ExecuteEvent(eventId);

        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, -50.0f, true))
            DoCast(target, SPELL_SUMMON_PLAYER);

        DoMeleeAttackIfReady();
    }
};

/*
 * --- Spell: Mark of Nature
 */

class MarkOfNatureTargetSelector
{
    public:
        MarkOfNatureTargetSelector() { }

        bool operator()(WorldObject* object) const
        {
            if (Unit* unit = object->ToUnit())
                // return anyone that isn't tagged or already under the influence of Aura of Nature
                return !(unit->HasAura(SPELL_MARK_OF_NATURE) && !unit->HasAura(SPELL_AURA_OF_NATURE));
            return true;
        }
};

#endif
