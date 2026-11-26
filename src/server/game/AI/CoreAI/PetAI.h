////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
///      Based on TrinityCore code       ///
///   Copyright 2008-2012 TrinityCore    ///
///   <http://www.trinitycore.org/>      ///
///   Copyright 2005-2009 MaNGOS         ///
///   <http://getmangos.com/>            ///
///                                      ///
////////////////////////////////////////////

#ifndef TRINITY_PETAI_H
#define TRINITY_PETAI_H

#include "CreatureAI.h"
#include "Timer.h"

class Creature;
class Spell;

class PetAI : public CreatureAI
{
public:

    explicit PetAI(Creature* c);

    void InitializeAI() override;
    void EnterEvadeMode() override;
    void JustDied(Unit* /*who*/) override;

    void UpdateAI(uint32) override;
    static int Permissible(const Creature*);

    void KilledUnit(Unit* /*victim*/) override;
    void AttackStart(Unit* target) override;
    void MovementInform(uint32 moveType, uint32 data) override;
    void AttackedBy(Unit* attacker) override;
    void OwnerDamagedBy(Unit* attacker) override;
    void OwnerAttacked(Unit* target) override;
    void ReceiveEmote(Player* player, uint32 textEmote) override;

private:
    bool _needToStop();
    void _stopAttack();

    void UpdateAllies();

    GuidSet m_AllySet;
    uint32 m_updateAlliesTimer;
    uint32 m_timeCheckSelf;

    Unit* SelectNextTarget();
    void HandleReturnMovement();
    void DoAttack(Unit* target, bool chase);
    bool CanAttack(Unit* target);
};
#endif

