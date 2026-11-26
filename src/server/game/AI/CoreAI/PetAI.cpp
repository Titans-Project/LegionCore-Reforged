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

#include "PetAI.h"
#include "Pet.h"
#include "Player.h"
#include "Spell.h"
#include "ObjectAccessor.h"
#include "SpellMgr.h"
#include "Creature.h"
#include "World.h"
#include "Util.h"
#include "Group.h"
#include "SpellInfo.h"
#include "CharmInfo.h"

int PetAI::Permissible(const Creature* creature)
{
    if (creature->isPet())
        return PERMIT_BASE_SPECIAL;

    return PERMIT_BASE_NO;
}

PetAI::PetAI(Creature* c) : CreatureAI(c)
{
    m_AllySet.clear();
    UpdateAllies();
    m_timeCheckSelf = 0;
}

void PetAI::EnterEvadeMode()
{
}

void PetAI::JustDied(Unit*)
{
    _stopAttack();
}

void PetAI::InitializeAI()
{
    if (PetStats const* pStats = sObjectMgr->GetPetStats(me->GetEntry()))
    {
        if (me->ToPet() && me->ToPet()->GetDuration())
            me->SetReactState(ReactStates(pStats->state));
    }
    if (me->GetReactState() == REACT_AGGRESSIVE)
    {
        if (Unit* victim = me->GetTargetUnit())
        {
            Unit* owner = me->GetCharmerOrOwner();
            if (me->Attack(victim, !me->GetCasterPet()))
            {
                if (owner && !owner->isInCombat())
                    owner->SetInCombatWith(victim);

                if (CharmInfo* charmInfo = me->GetCharmInfo())
                {
                    charmInfo->SetIsAtStay(false);
                    charmInfo->SetIsFollowing(false);
                    charmInfo->SetIsReturning(false);
                }
                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MoveChase(victim, me->GetAttackDist() - 0.5f);
            }
        }
    }

    CreatureAI::InitializeAI();

    // Update speed as needed to prevent dropping too far behind and despawning
    me->UpdateSpeed(MOVE_RUN, true);
    me->UpdateSpeed(MOVE_WALK, true);
    me->UpdateSpeed(MOVE_FLIGHT, true);
}

bool PetAI::_needToStop()
{
    // This is needed for charmed creatures, as once their target was reset other effects can trigger threat
    if (me->isCharmed() && me->getVictim() == me->GetCharmer())
        return true;

    return !me->IsValidAttackTarget(me->getVictim());
}

void PetAI::_stopAttack()
{
    if (!me->IsAlive())
    {
        TC_LOG_DEBUG("misc", "Creature stoped attacking cuz his dead [guid=%s]", me->GetGUID().ToString().c_str());
        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MoveIdle();
        me->CombatStop();
        me->getHostileRefManager().deleteReferences();
        return;
    }

    me->AttackStop();
    if (CharmInfo* charmInfo = me->GetCharmInfo())
        charmInfo->SetIsCommandAttack(false);
    HandleReturnMovement();
}

void PetAI::UpdateAI(uint32 diff)
{
    if (!me->IsAlive())
        return;

    Unit* owner = me->GetCharmerOrOwner();

    if (m_updateAlliesTimer <= diff)
        // UpdateAllies self set update timer
        UpdateAllies();
    else
        m_updateAlliesTimer -= diff;

    // me->getVictim() can't be used for check in case stop fighting, me->getVictim() clear at Unit death etc.
    if (owner && owner->IsMounted())
    {
        if (!me->HasUnitState(UNIT_STATE_FOLLOW))
            me->GetMotionMaster()->MoveFollow(owner, me->GetFollowDistance(), me->GetFollowAngle());
    }
    else if (me->getVictim())
    {
        // is only necessary to stop casting, the pet must not exit combat
        if (me->getVictim()->HasCrowdControlAura(me))
        {
            me->InterruptNonMeleeSpells(false);
            return;
        }

        if (_needToStop())
        {
            TC_LOG_DEBUG("misc", "Pet AI stopped attacking [guid=%s]", me->GetGUID().ToString().c_str());
            _stopAttack();
            return;
        }

        if (owner && !owner->isInCombat())
            owner->SetInCombatWith(me->getVictim());

        if (!me->GetCasterPet())
            DoMeleeAttackIfReady();
    }
    else if (owner && me->GetCharmInfo()) //no victim
    {
        //TC_LOG_DEBUG("misc", "PetAI::UpdateAI [guid=%u] no victim GetCasterPet %i", me->GetGUIDLow(), me->GetCasterPet());
        // Only aggressive pets do target search every update.
        // Defensive pets do target search only in these cases:
        //  * Owner attacks something - handled by OwnerAttacked()
        //  * Owner receives damage - handled by OwnerDamagedBy()
        //  * Pet is in combat and current target dies - handled by KilledUnit()
        if (me->HasReactState(REACT_AGGRESSIVE))
        {
            Unit* nextTarget = SelectNextTarget();

            if (nextTarget)
                AttackStart(nextTarget);
            else
                HandleReturnMovement();
        }
        else
            HandleReturnMovement();
    }
    else if (owner && !me->HasUnitState(UNIT_STATE_FOLLOW)) // no charm info and no victim
        me->GetMotionMaster()->MoveFollow(owner, me->GetFollowDistance(), me->GetFollowAngle());

    if (!me->GetCharmInfo() || me->m_isHati)
        return;

    // Autocast (casted only in combat or persistent spells in any state)
    if (!me->HasUnitState(UNIT_STATE_CASTING))
    {
        typedef std::vector<std::pair<Unit*, Spell*> > TargetSpellList;
        TargetSpellList targetSpellStore;
        // TC_LOG_DEBUG("misc", "PetAI::UpdateAI GetPetAutoSpellSize %i", me->GetPetAutoSpellSize());

        for (uint8 i = 0; i < me->GetPetAutoSpellSize(); ++i)
        {
            uint32 spellID = me->GetPetAutoSpellOnPos(i);
            if (!spellID)
                continue;

            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellID);
            if (!spellInfo)
                continue;

            // TC_LOG_DEBUG("misc", "PetAI::UpdateAI spellID %i, Cooldown %i IsPositive %i CanBeUsedInCombat %i GUID %u",
            // spellID, me->HasCreatureSpellCooldown(spellID), spellInfo->IsPositive(), spellInfo->CanBeUsedInCombat(), me->GetGUIDLow());

            if (me->GetCharmInfo() && me->GetGlobalCooldownMgr().HasGlobalCooldown(spellInfo))
                continue;

            if (spellInfo->IsPositive())
            {
                // check spell cooldown
                if (me->HasCreatureSpellCooldown(spellInfo->Id))
                    continue;

                // a combat buff (Dash...) is only for a fight: checked before the self-cast below, which looped out of combat
                if (spellInfo->CanBeUsedInCombat())
                {
                    // Check if we're in combat or commanded to attack
                    if (!me->isInCombat() && !me->GetCharmInfo()->IsCommandAttack())
                        continue;
                }

                if (spellInfo->IsSelfTargets())
                {
                    if (m_timeCheckSelf >= 1000) // Stop spamm
                    {
                        TriggerCastData triggerData;
                        targetSpellStore.push_back(std::make_pair(me, new Spell(me, spellInfo, triggerData)));
                        m_timeCheckSelf = 0;
                        break;
                    }
                    m_timeCheckSelf += diff;
                }

                TriggerCastData triggerData;
                auto spell = new Spell(me, spellInfo, triggerData);

                // Some spells can target enemy or friendly (DK Ghoul's Leap)
                // Enemy side: only the pet's own target. Any attacker of the pet or the owner made the ghoul
                // leap at the end of a fight onto a mob nobody had engaged yet, starting a new one.
                Unit* target = me->getVictim();
                if (target && !target->IsAlive())
                    target = nullptr;

                if (target)
                {
                    if (CanAttack(target) && spell->CanAutoCast(target))
                    {
                        targetSpellStore.push_back(std::make_pair(target, spell));
                        break;
                    }
                }

                // No enemy, check friendly
                bool spellUsed = false;
                for (auto tar : m_AllySet)
                {
                    Unit* ally = ObjectAccessor::GetUnit(*me, tar);

                    //only buff targets that are in combat, unless the spell can only be cast while out of combat
                    if (!ally)
                        continue;

                    if (spell->CanAutoCast(ally))
                    {
                        targetSpellStore.push_back(std::make_pair(ally, spell));
                        spellUsed = true;
                        break;
                    }
                }

                // No valid targets at all
                if (!spellUsed)
                    delete spell;
            }
            else if (spellInfo->CanBeUsedInCombat())
            {
                Unit* target = me->getAttackerForHelper();
                Unit* targetOwner = target;
                if (owner)
                    targetOwner = owner->getAttackerForHelper();
                if ((target && CanAttack(target)) || (targetOwner && targetOwner != target && CanAttack(targetOwner)))
                {
                    TriggerCastData triggerData;
                    auto spell = new Spell(me, spellInfo, triggerData);
                    if (target && spell->CanAutoCast(target))
                        targetSpellStore.push_back(std::make_pair(target, spell));
                    else if (targetOwner && targetOwner != target && spell->CanAutoCast(targetOwner))
                        targetSpellStore.push_back(std::make_pair(targetOwner, spell));
                    else
                        delete spell;
                }
            }
        }

        //found units to cast on to
        if (!targetSpellStore.empty())
        {
            uint32 index = urand(0, targetSpellStore.size() - 1);

            Spell* spell = targetSpellStore[index].second;
            Unit*  target = targetSpellStore[index].first;

            targetSpellStore.erase(targetSpellStore.begin() + index);

            SpellCastTargets targets;
            targets.SetCaster(target);
            targets.SetUnitTarget(target);

            if (!me->HasInArc(float(M_PI), target))
            {
                me->SetInFront(target);
                if (target && target->IsPlayer())
                    me->SendUpdateToPlayer(target->ToPlayer());

                if (owner && owner->IsPlayer())
                    me->SendUpdateToPlayer(owner->ToPlayer());
            }

            me->AddCreatureSpellCooldown(spell->m_spellInfo->Id);
            spell->prepare(&targets);
        }

        // deleted cached Spell objects
        for (TargetSpellList::const_iterator itr = targetSpellStore.begin(); itr != targetSpellStore.end(); ++itr)
            delete itr->second;
    }
}

void PetAI::UpdateAllies()
{
    Unit* owner = me->GetCharmerOrOwner();
    Group* group = nullptr;

    m_updateAlliesTimer = 10 * IN_MILLISECONDS;                //update friendly targets every 10 seconds, lesser checks increase performance

    if (!owner)
        return;
    if (owner->IsPlayer())
        group = owner->ToPlayer()->GetGroup();

    //only pet and owner/not in group->ok
    if (m_AllySet.size() == 2 && !group)
        return;
    //owner is in group; group members filled in already (no raid -> subgroupcount = whole count)
    if (group && !group->isRaidGroup() && m_AllySet.size() == (group->GetMembersCount() + 1))
        return;

    m_AllySet.clear();
    m_AllySet.insert(me->GetGUID());
    m_AllySet.insert(owner->GetGUID());
    if (group)                                              //add group
    {
        group->ForEachMemberInMap(me->GetMap(), [&](Player* Target)
        {
            if (group->SameSubGroup(owner->ToPlayer(), Target))
                m_AllySet.insert(Target->GetGUID());
        });
    }
}

void PetAI::KilledUnit(Unit* victim)
{
    // Called from Unit::Kill() in case where pet or owner kills something
    // if owner killed this victim, pet may still be attacking something else
    if (me->getVictim() && me->getVictim() != victim)
        return;

    // Clear target just in case. May help problem where health / focus / mana
    // regen gets stuck. Also resets attack command.
    // Can't use _stopAttack() because that activates movement handlers and ignores
    // next target selection
    me->AttackStop();
    if (CharmInfo* charmInfo = me->GetCharmInfo())
        charmInfo->SetIsCommandAttack(false);
    me->SendMeleeAttackStop();  // Stops the pet's 'Attack' button from flashing

    Unit* nextTarget = SelectNextTarget();

    if (nextTarget)
        AttackStart(nextTarget);
    else
        HandleReturnMovement(); // Return
}

void PetAI::AttackStart(Unit* target)
{
    // Overrides Unit::AttackStart to correctly evaluate Pet states

    // Check all pet states to decide if we can attack this target
    if (!CanAttack(target))
        return;

    if (Unit* owner = me->GetOwner())
        owner->SetInCombatWith(target);

    DoAttack(target, true);
}

void PetAI::OwnerDamagedBy(Unit* attacker)
{
    // Called when owner takes damage. Allows defensive pets to know
    //  that their owner might need help

    if (!attacker)
        return;

    // Passive pets don't do anything
    if (me->HasReactState(REACT_PASSIVE))
        return;

    // Prevent pet from disengaging from current target
    if (me->getVictim() && me->getVictim()->IsAlive())
        return;

    // Continue to evaluate and attack if necessary
    AttackStart(attacker);
}

void PetAI::AttackedBy(Unit* attacker)
{
    // A pet hit while its owner is not fights back, on Assist as on Defensive
    if (!attacker || !me->IsAlive() || me->HasReactState(REACT_PASSIVE))
        return;

    if (me->getVictim() && me->getVictim()->IsAlive())
        return;

    AttackStart(attacker);
}

void PetAI::OwnerAttacked(Unit* target)
{
    // Called when owner attacks something. Allows defensive pets to know
    //  that they need to assist

    // Target might be NULL if called from spell with invalid cast targets
    if (!target)
        return;

    // Since Legion only Assist joins the owner's target: Defensive waits for the owner or the pet to be attacked
    if (!me->HasReactState(REACT_HELPER) && !me->HasReactState(REACT_AGGRESSIVE))
        return;

    // Prevent pet from disengaging from current target
    if (me->getVictim() && me->getVictim()->IsAlive())
        return;

    // Continue to evaluate and attack if necessary
    AttackStart(target);
}

Unit* PetAI::SelectNextTarget()
{
    // Provides next target selection after current target death

    // Passive pets don't do next target selection
    if (me->HasReactState(REACT_PASSIVE))
        return nullptr;

    // Check pet attackers first so we don't drag a bunch of targets to the owner
    if (Unit* myAttacker = me->getAttackerForHelper())
        if (!myAttacker->HasCrowdControlAura())
            return myAttacker;

    // Not sure why we wouldn't have an owner but just in case...
    if (!me->GetCharmerOrOwner())
        return nullptr;

    // Check owner attackers
    if (Unit* ownerAttacker = me->GetCharmerOrOwner()->getAttackerForHelper())
        if (!ownerAttacker->HasCrowdControlAura())
            return ownerAttacker;

    // Check owner victim, Assist only
    if (me->HasReactState(REACT_HELPER) || me->HasReactState(REACT_AGGRESSIVE))
        if (Unit* ownerVictim = me->GetCharmerOrOwner()->getVictim())
            return ownerVictim;

    // Default - no valid targets
    return nullptr;
}

void PetAI::HandleReturnMovement()
{
    if (!me->GetCharmInfo())
        return;

    // Handles moving the pet back to stay or owner
    //TC_LOG_DEBUG("misc", "PetAI::HandleReturnMovement [guid=%u] GetCommandState %i", me->GetGUIDLow(), me->GetCharmInfo()->GetCommandState());

    // Move To saves its point as the stay position
    if (me->GetCharmInfo()->HasCommandState(COMMAND_STAY) || me->GetCharmInfo()->HasCommandState(COMMAND_MOVE_TO))
    {
        if (!me->GetCharmInfo()->IsAtStay() && !me->GetCharmInfo()->IsReturning())
        {
            // Return to previous position where stay was clicked
            if (!me->GetCharmInfo()->IsCommandAttack())
            {
                float x, y, z;

                me->GetCharmInfo()->GetStayPosition(x, y, z);
                me->GetCharmInfo()->SetIsReturning(true);
                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MovePoint(me->GetGUIDLow(), x, y, z);
            }
        }
    }
    else if (me->GetCharmerOrOwner()) // COMMAND_FOLLOW
    {
        if (!me->GetCharmInfo()->IsFollowing() && !me->GetCharmInfo()->IsReturning() && me->GetDistance(me->GetCharmerOrOwner()) > sWorld->getRate(RATE_TARGET_POS_RECALCULATION_RANGE))
        {
            //TC_LOG_DEBUG("misc", "PetAI::HandleReturnMovement Pet %u", me->GetEntry());

            //if (!me->GetCharmInfo()->IsCommandAttack())
            {
                me->GetCharmInfo()->SetIsReturning(true);
                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MoveFollow(me->GetCharmerOrOwner(), me->GetFollowDistance(), me->GetFollowAngle());
            }
        }
    }
}

void PetAI::DoAttack(Unit* target, bool chase)
{
    // Handles attack with or without chase and also resets all
    // PetAI flags for next update / creature kill

    // me->GetCharmInfo()->SetIsCommandAttack(false);

    // The following conditions are true if chase == true
    // (Follow && (Aggressive || Defensive))
    // ((Stay || Follow) && (Passive && player clicked attack))

    if (chase)
    {
        if (me->Attack(target, true))
        {
            me->GetCharmInfo()->SetIsAtStay(false);
            me->GetCharmInfo()->SetIsFollowing(false);
            me->GetCharmInfo()->SetIsReturning(false);
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MoveChase(target, me->GetAttackDist() - 0.5f);
        }
    }
    else // (Stay && ((Aggressive || Defensive) && In Melee Range)))
    {
        me->GetCharmInfo()->SetIsAtStay(true);
        me->GetCharmInfo()->SetIsFollowing(false);
        me->GetCharmInfo()->SetIsReturning(false);
        me->Attack(target, !me->GetCasterPet());
    }
}

void PetAI::MovementInform(uint32 moveType, uint32 data)
{
    if (!me->GetCharmInfo())
        return;

    //TC_LOG_DEBUG("misc", "PetAI::MovementInform Pet %u moveType %i data %i", me->GetEntry(), moveType, data);
    // Receives notification when pet reaches stay or follow owner
    switch (moveType)
    {
    case POINT_MOTION_TYPE:
    {
        // Pet is returning to where stay was clicked. data should be
        if (me->GetCharmInfo()->IsReturning())
        {
            me->GetCharmInfo()->SetIsAtStay(true);
            me->GetCharmInfo()->SetIsReturning(false);
            me->GetCharmInfo()->SetIsFollowing(false);
            me->GetCharmInfo()->SetIsCommandAttack(false);
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MoveIdle();
        }
        else
        {
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MoveIdle();
            if (me->GetCharmInfo() && me->GetCharmInfo()->IsCommandAttack() && me->getVictim())
                me->GetMotionMaster()->MoveChase(me->getVictim(), me->GetAttackDist() - 0.5f);
            if (me->GetCharmerOrOwner() && me->GetCharmInfo() && me->GetCharmInfo()->IsFollowing())
                me->GetMotionMaster()->MoveFollow(me->GetCharmerOrOwner(), me->GetFollowDistance(), me->GetFollowAngle());
        }
        break;
    }
    case FOLLOW_MOTION_TYPE:
    {
        // If data is owner's GUIDLow then we've reached follow point,
        // otherwise we're probably chasing a creature
        if (me->GetCharmerOrOwner() && me->GetCharmInfo() && data == me->GetCharmerOrOwner()->GetGUIDLow() && me->GetCharmInfo()->IsReturning())
        {
            me->GetCharmInfo()->SetIsAtStay(false);
            me->GetCharmInfo()->SetIsReturning(false);
            me->GetCharmInfo()->SetIsFollowing(false);
            me->GetCharmInfo()->SetIsCommandAttack(false);
            //me->GetMotionMaster()->Clear();
        }
        break;
    }
    default:
        break;
    }
}

bool PetAI::CanAttack(Unit* target)
{
    // CrashFix
    if (!me->GetCharmInfo())
        return false;

    // Evaluates wether a pet can attack a specific
    // target based on CommandState, ReactState and other flags

    // Returning - check first since pets returning ignore attacks
    if (me->GetCharmInfo()->IsReturning())
        return false;

    // Passive - check now so we don't have to worry about passive in later checks
    if (me->HasReactState(REACT_PASSIVE))
        return me->GetCharmInfo()->IsCommandAttack();

    //  Pets commanded to attack should not stop their approach if attacked by another creature
    if (me->getVictim() && (me->getVictim() != target) && (!me->HasReactState(REACT_HELPER) && (!me->GetCharmerOrOwner() || !me->GetCharmerOrOwner()->getVictim())))
        return !me->GetCharmInfo()->IsCommandAttack();

    // From this point on, pet will always be either aggressive or defensive

    // Stay or Move To - can attack if target is within range or commanded to
    if (me->GetCharmInfo()->HasCommandState(COMMAND_STAY) || me->GetCharmInfo()->HasCommandState(COMMAND_MOVE_TO))
        return (me->IsWithinMeleeRange(target, me->GetAttackDist()) || me->GetCharmInfo()->IsCommandAttack());

    // Follow
    if (me->GetCharmInfo()->HasCommandState(COMMAND_FOLLOW))
        return true;

    // default, though we shouldn't ever get here
    return false;
}

void PetAI::ReceiveEmote(Player* player, uint32 emote)
{
    if (!me->GetOwnerGUID().IsEmpty() && me->GetOwnerGUID() == player->GetGUID())
        switch (emote)
        {
        case TEXT_EMOTE_COWER:
            if (me->isPet() && me->ToPet()->IsPetGhoul())
                me->HandleEmoteCommand(EMOTE_ONESHOT_OMNICAST_GHOUL);
            break;
        case TEXT_EMOTE_ANGRY:
            if (me->isPet() && me->ToPet()->IsPetGhoul())
                me->HandleEmoteCommand(EMOTE_STATE_STUN);
            break;
        case TEXT_EMOTE_GLARE:
            if (me->isPet() && me->ToPet()->IsPetGhoul())
                me->HandleEmoteCommand(EMOTE_STATE_STUN);
            break;
        case TEXT_EMOTE_SOOTHE:
            if (me->isPet() && me->ToPet()->IsPetGhoul())
                me->HandleEmoteCommand(EMOTE_ONESHOT_OMNICAST_GHOUL);
            break;
        }
}
