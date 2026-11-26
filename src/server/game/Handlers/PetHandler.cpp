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

#include "PetPackets.h"
#include "QueryPackets.h"
#include "CharmInfo.h"
#include "Group.h"
#include "CharacterData.h"
#include "DatabaseEnv.h"
#include "SpellPackets.h"
#include "CreatureAI.h"

void WorldSession::HandleDismissCritter(WorldPackets::PetPackets::DismissCritter& packet)
{
    Unit* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, packet.CritterGUID);
    if (!pet)
        return;

    if (_player->GetCritterGUID() == pet->GetGUID())
    {
        if (pet->IsCreature() && pet->ToCreature()->isSummon())
            pet->ToTempSummon()->UnSummon();
    }
}

void WorldSession::HandlePetAction(WorldPackets::PetPackets::PetAction& packet)
{
    uint32 spellID = UNIT_ACTION_BUTTON_ACTION(packet.Action);
    uint8 flag = UNIT_ACTION_BUTTON_TYPE(packet.Action); //delete = 0x07 CastSpell = C1

    TC_LOG_DEBUG("network", "HandlePetAction: Pet %u - flag: %u, spellID: %u, target: %u.", packet.PetGUID.GetGUIDLow(), uint32(flag), spellID, packet.TargetGUID.GetGUIDLow());

    if (_player->IsMounted())
        return;

    Unit* pet = ObjectAccessor::GetUnit(*_player, packet.PetGUID);
    if (!pet)
    {
        TC_LOG_ERROR("network", "HandlePetAction: Pet (GUID: %u) doesn't exist for player '%s'", packet.PetGUID.GetGUIDLow(), GetPlayer()->GetName());
        return;
    }

    if (pet != GetPlayer()->GetFirstControlled())
    {
        TC_LOG_ERROR("network", "HandlePetAction: Pet (GUID: %u) does not belong to player '%s'", packet.PetGUID.GetGUIDLow(), GetPlayer()->GetName());
        return;
    }

    if (!pet->IsAlive())
    {
        SpellInfo const* spell = (flag == ACT_ENABLED || flag == ACT_PASSIVE) ? sSpellMgr->GetSpellInfo(spellID) : nullptr;
        if (!spell)
            return;

        if (!(spell->HasAttribute(SPELL_ATTR0_CASTABLE_WHILE_DEAD)))
            return;
    }

    if (pet->HasAuraType(SPELL_AURA_DISABLE_AUTO_ATTACK) || pet->HasAuraType(SPELL_AURA_DISABLE_ATTACK_AND_CAST))
        return;

    //TODO: allow control charmed player?
    if (pet->IsPlayer() && !(flag == ACT_COMMAND && spellID == COMMAND_ATTACK))
        return;

    if (GetPlayer()->m_Controlled.size() == 1)
        HandlePetActionHelper(pet, packet.PetGUID, spellID, flag, packet.TargetGUID, packet.ActionPosition);
    else
    {
        //If a pet is dismissed, m_Controlled will change
        std::vector<Unit*> controlled;
        for (auto const& guid : GetPlayer()->m_Controlled)
        {
            Unit* unit = ObjectAccessor::GetUnit(*GetPlayer(), guid);
            if (!unit)
                continue;
            if ((unit->GetEntry() == pet->GetEntry() || unit->ToCreature() && unit->ToCreature()->m_isHati) &&
                    unit->IsAlive())
            {
                if (unit->ToCreature())
                {
                    if (unit->HasUnitTypeMask(UNIT_MASK_CONTROLABLE_GUARDIAN))
                        controlled.push_back(unit);
                }
                else
                    controlled.push_back(unit);
            }
        }

        for (auto const& itr : controlled)
            HandlePetActionHelper(itr, packet.PetGUID, spellID, flag, packet.TargetGUID, packet.ActionPosition);
    }
}

void WorldSession::HandleStopAttack(WorldPackets::PetPackets::StopAttack& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Unit* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*player, packet.PetGUID);
    if (!pet)
        return;

    if (pet != player->GetPet() && pet != player->GetCharm())
        return;

    if (!pet->IsAlive())
        return;

    pet->AttackStop();
}

void WorldSession::HandleQueryPetName(WorldPackets::Query::QueryPetName& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    WorldPackets::Query::QueryPetNameResponse response;
    response.UnitGUID = packet.UnitGUID;

    Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*player, packet.UnitGUID);
    if (!pet)
    {
        player->SendDirectMessage(response.Write());
        return;
    }

    response.Allow = pet->isPet();
    response.Name = pet->GetName();

    if (Pet* playerPet = pet->ToPet())
    {
        if (DeclinedName const* declinedNames = playerPet->GetDeclinedNames())
        {
            response.HasDeclined = true;
            for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
                if (!declinedNames->name[i].empty())
                    response.DeclinedNames.name[i] = declinedNames->name[i];
        }
        else
            response.HasDeclined = false;

        response.Timestamp = playerPet->GetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP);
    }

    player->SendDirectMessage(response.Write());
}

void WorldSession::HandlePetSetAction(WorldPackets::PetPackets::PetSetAction& packet)
{
    Unit* pet = ObjectAccessor::GetUnit(*_player, packet.PetGUID);
    if (!pet || pet != _player->GetFirstControlled())
    {
        TC_LOG_ERROR("network", "HandlePetSetAction: Unknown pet (GUID: %u) or pet owner (GUID: %u)", packet.PetGUID.GetGUIDLow(), _player->GetGUIDLow());
        return;
    }

    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
    {
        TC_LOG_ERROR("network", "WorldSession::HandlePetSetAction: object (GUID: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!", pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    if (packet.Index >= MAX_UNIT_ACTION_BAR_INDEX)
        return;

    uint32 spellID = UNIT_ACTION_BUTTON_ACTION(packet.Action);
    uint8 actState = UNIT_ACTION_BUTTON_TYPE(packet.Action);

    if (actState == ACT_DECIDE && !charmInfo->GetActionBarEntry(packet.Index))
        return;

    TC_LOG_DEBUG("network", "Player %s has changed pet spell action. Position: %u, Spell: %u, State: 0x%X HasSpell %i",
        _player->GetName(), packet.Index, spellID, uint32(actState), pet->HasSpell(spellID));

    //if it's act for spell (en/disable/cast) and there is a spell given (0 = remove spell) which pet doesn't know, don't add
    if (!((actState == ACT_ENABLED || actState == ACT_DISABLED || actState == ACT_PASSIVE) && spellID && !pet->HasSpell(spellID)))
    {
        if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellID))
        {
            if (actState == ACT_ENABLED)
            {
                if (pet->GetCharmInfo())
                {
                    if (Pet* playerPet = pet->ToPet())
                        playerPet->ToggleAutocast(spellInfo, true);
                }
                else
                    for (auto const& guid : GetPlayer()->m_Controlled)
                        if(Unit* unit = ObjectAccessor::GetUnit(*GetPlayer(), guid))
                            if (unit->GetEntry() == pet->GetEntry())
                                unit->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, true);
            }
            else if (actState == ACT_DISABLED)
            {
                if (pet->GetCharmInfo())
                {
                    if (Pet* playerPet = pet->ToPet())
                        playerPet->ToggleAutocast(spellInfo, false);
                }
                else
                    for (auto const& guid : GetPlayer()->m_Controlled)
                        if(Unit* unit = ObjectAccessor::GetUnit(*GetPlayer(), guid))
                            if (unit->GetEntry() == pet->GetEntry())
                                unit->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, false);
            }
        }

        charmInfo->SetActionBar(packet.Index, spellID, ActiveStates(actState));
    }
}

void WorldSession::HandlePetRename(WorldPackets::PetPackets::PetRename& packet)
{
    ObjectGuid petguid = packet.RenameData.PetGUID;

    std::string name = packet.RenameData.NewName;
    Optional<DeclinedName> const& declinedname = packet.RenameData.DeclinedNames;

    PetStable* petStable = _player->GetPetStable();
    Pet* pet = ObjectAccessor::GetPet(*_player, petguid);

    if (!pet || !pet->isPet() || pet->getPetType() != HUNTER_PET ||
        !pet->HasByteFlag(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PET_FLAGS, UNIT_CAN_BE_RENAMED) ||
        pet->GetOwnerGUID() != _player->GetGUID() || !pet->GetCharmInfo() ||
        !petStable || !petStable->GetCurrentPet() || petStable->GetCurrentPet()->PetNumber != pet->GetCharmInfo()->GetPetNumber())
        return;

    PetNameInvalidReason res = sCharacterDataStore->CheckPetName(packet.RenameData.NewName);
    if (res != PET_NAME_SUCCESS)
    {
        SendPetNameInvalid(res, petguid, packet.RenameData.NewName, {});
        return;
    }

    if (sCharacterDataStore->IsReservedName(packet.RenameData.NewName))
    {
        SendPetNameInvalid(PET_NAME_RESERVED, petguid, packet.RenameData.NewName, {});
        return;
    }

    if (declinedname && sWorld->getBoolConfig(CONFIG_DECLINED_NAMES_USED))
    {
        std::wstring wname;
        if (!Utf8toWStr(packet.RenameData.NewName, wname))
            return;

        if (!sCharacterDataStore->CheckDeclinedNames(wname, *declinedname))
        {
            SendPetNameInvalid(PET_NAME_DECLENSION_DOESNT_MATCH_BASE_NAME, petguid, packet.RenameData.NewName, declinedname);
            return;
        }
    }

    pet->SetName(packet.RenameData.NewName);

    pet->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_PET_NAME);

    pet->RemoveByteFlag(UNIT_FIELD_BYTES_2, UNIT_BYTES_2_OFFSET_PET_FLAGS, UNIT_CAN_BE_RENAMED);

    petStable->GetCurrentPet()->Name = name;
    petStable->GetCurrentPet()->WasRenamed = true;

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
    if (declinedname && sWorld->getBoolConfig(CONFIG_DECLINED_NAMES_USED))
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_PET_DECLINEDNAME);
        stmt->setUInt32(0, pet->GetCharmInfo()->GetPetNumber());
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_ADD_CHAR_PET_DECLINEDNAME);
        stmt->setUInt32(0, pet->GetCharmInfo()->GetPetNumber());
        stmt->setUInt64(1, _player->GetGUID().GetGUIDLow());

        for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; i++)
            stmt->setString(i + 2, declinedname->name[i]);

        trans->Append(stmt);
    }

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_NAME);
    stmt->setString(0, packet.RenameData.NewName);
    stmt->setUInt64(1, _player->GetGUID().GetGUIDLow());
    stmt->setUInt32(2, pet->GetCharmInfo()->GetPetNumber());
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);

    pet->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, uint32(GameTime::GetGameTime())); // cast can't be helped
}

void WorldSession::HandlePetAbandon(WorldPackets::PetPackets::PetAbandon& packet)
{
    if (!_player->IsInWorld())
        return;

    if (auto pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, packet.Pet))
    {
        // the guid comes from the client: only the player's own hunter pet can be abandoned
        if (pet->isPet())
        {
            if (pet->GetOwnerGUID() != _player->GetGUID() || !pet->isHunterPet())
                return;

            _player->RemovePet((Pet*)pet, PET_SAVE_AS_DELETED);
            _player->GetSession()->SendStablePet();
        }
        else if (pet->GetGUID() == _player->GetCharmGUID())
            _player->StopCastingCharm();
    }
}

void WorldSession::HandlePetSpellAutocast(WorldPackets::PetPackets::PetSpellAutocast& packet)
{
    if (!_player->GetGuardianPet() && !_player->GetCharm())
        return;

    if (ObjectAccessor::FindPlayer(packet.PetGUID))
        return;

    Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, packet.PetGUID);
    if (!pet || (pet != _player->GetGuardianPet() && pet != _player->GetCharm()))
    {
        TC_LOG_ERROR("network", "HandlePetSpellAutocast.Pet %u isn't pet of player %s .", packet.PetGUID.GetGUIDLow(), GetPlayer()->GetName());
        return;
    }

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(packet.SpellID);
    if (!spellInfo || !pet->HasSpell(packet.SpellID) || !spellInfo->IsAutocastable())
        return;

    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
    {
        TC_LOG_ERROR("network", "WorldSession::HandlePetSpellAutocastOpcod: object (GUID: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!", pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    if (pet->isPet())
        pet->ToPet()->ToggleAutocast(spellInfo, packet.AutocastEnabled);
    else
        pet->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, packet.AutocastEnabled);

    charmInfo->SetSpellAutocast(spellInfo, packet.AutocastEnabled);
}

void WorldSession::HandlePetCastSpellOpcode(WorldPackets::Spells::PetCastSpell& cast)
{
    if (!_player->GetGuardianPet() && !_player->GetCharm())
        return;

    Unit* caster = ObjectAccessor::GetUnit(*_player, cast.PetGUID);

    if (!caster || (caster != _player->GetGuardianPet() && caster != _player->GetCharm()))
    {
        TC_LOG_ERROR("network", "HandlePetCastSpellOpcode: Pet %u isn't pet of player %s .", cast.PetGUID.GetGUIDLow(), GetPlayer()->GetName());
        return;
    }

    if (caster->HasAuraType(SPELL_AURA_DISABLE_ATTACK_AND_CAST))
        return;

    SpellCastTargets targets(caster, cast.Cast);

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(cast.Cast.SpellID);
    if (!spellInfo)
    {
        TC_LOG_ERROR("network", "WORLD: unknown PET spell id %i", cast.Cast.SpellID);
        return;
    }

    // Aimed vehicle and charmed-creature spells (siege cannons, quest turrets) are often missing from the
    // creature's spell list. They stay castable, but only from such a caster and with every normal check.
    bool aimedFromVehicle = false;
    if (caster != _player->GetGuardianPet() && (caster->IsVehicle() || caster == _player->GetCharm()))
    {
        for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (spellInfo->EffectMask < uint32(1 << i))
                break;

            if (spellInfo->Effects[i]->TargetA.GetTarget() == TARGET_DEST_TRAJ || spellInfo->Effects[i]->TargetB.GetTarget() == TARGET_DEST_TRAJ || spellInfo->Effects[i]->Effect == SPELL_EFFECT_TRIGGER_MISSILE)
                aimedFromVehicle = true;
        }
    }

    // do not cast not learned spells
    if ((!caster->HasSpell(cast.Cast.SpellID) && !aimedFromVehicle) || spellInfo->IsPassive())
    {
        TC_LOG_ERROR("network", "HandlePetCastSpellOpcode: !HasSpell or IsPassive");
        return;
    }

    caster->ClearUnitState(UNIT_STATE_FOLLOW);

    TriggerCastData triggerData;
    triggerData.triggerFlags = TRIGGERED_NONE;
    triggerData.miscData0 = cast.Cast.Misc[0];
    triggerData.miscData1 = cast.Cast.Misc[1];
    triggerData.spellGuid = cast.Cast.SpellGuid;

    Spell* spell = new Spell(caster, spellInfo, triggerData);
    spell->m_targets = targets;

    if (spellInfo->Categories.StartRecoveryCategory) // Check if spell is affected by GCD
    {
        if (caster->IsCreature() && caster->GetGlobalCooldownMgr().HasGlobalCooldown(spellInfo))
        {
            TC_LOG_ERROR("network", "HandlePetCastSpellOpcode: Check if spell is affected by GCD");
            spell->SendCastResult(GetPlayer(), spellInfo, SPELL_FAILED_NOT_READY, SPELL_CUSTOM_ERROR_NONE, nullptr, true);
            spell->finish(false);
            delete spell;
            return;
        }
    }

    // TODO: need to check victim?
    SpellCastResult result;
    if (caster->GetPlayerMovingMe())
        result = spell->CheckPetCast(caster->GetPlayerMovingMe()->GetSelectedUnit());
    else
        result = spell->CheckPetCast(nullptr);
    if (result == SPELL_CAST_OK)
    {
        if (caster->IsCreature())
        {
            Creature* pet = caster->ToCreature();
            pet->AddCreatureSpellCooldown(cast.Cast.SpellID);
            if (pet->isPet())
            {
                auto p = pet->ToPet();
                // 10% chance to play special pet attack talk, else growl
                // actually this only seems to happen on special spells, fire shield for imp, torment for voidwalker, but it's stupid to check every spell
                if (p->getPetType() == SUMMON_PET && (urand(0, 100) < 10))
                    pet->SendPetTalk(static_cast<uint32>(PET_TALK_SPECIAL_SPELL));
                else
                    pet->SendPetAIReaction(cast.PetGUID);
            }
        }

        spell->prepare(&(spell->m_targets));
    }
    else
    {
        Creature* pet = caster->ToCreature();
        bool sendPet = !pet || !(pet->isPossessed() || pet->IsVehicle());
        spell->SendCastResult(GetPlayer(), spellInfo, result, SPELL_CUSTOM_ERROR_NONE, nullptr, sendPet);

        if (caster->IsPlayer())
        {
            if (!caster->ToPlayer()->HasSpellCooldown(cast.Cast.SpellID))
                GetPlayer()->SendClearCooldown(cast.Cast.SpellID, caster);
        }
        else
        {
            if (!caster->ToCreature()->HasCreatureSpellCooldown(cast.Cast.SpellID))
                GetPlayer()->SendClearCooldown(cast.Cast.SpellID, caster);
        }

        spell->finish(false);
        delete spell;
    }
}

void WorldSession::HandleSetPetSlot(WorldPackets::PetPackets::SetPetSlot& packet)
{
    if (!_player->GetNPCIfCanInteractWith(packet.NpcGUID, UNIT_NPC_FLAG_STABLEMASTER) || packet.NewSlot >= PET_SAVE_LAST_STABLE_SLOT)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    if (_player->IsMounted())
        _player->RemoveAurasByType(SPELL_AURA_MOUNTED);

    PetStable* petStable = _player->GetPetStable();
    if (!petStable)
    {
        SendStableResult(STABLE_ERR_INTERNAL);
        return;
    }

    std::pair<PetStable::PetInfo const*, PetSaveMode> const srcInfo = Pet::GetLoadPetInfo(*petStable, 0, packet.PetIndex, {});
    PetStable::PetInfo const* srcPet = srcInfo.first;
    PetSaveMode const srcPetSlot = srcInfo.second;
    PetSaveMode dstPetSlot = PetSaveMode(packet.NewSlot);
    PetStable::PetInfo const* dstPet = Pet::GetLoadPetInfo(*petStable, 0, 0, dstPetSlot).first;

    if (!srcPet || srcPet->Type != HUNTER_PET || (dstPet && dstPet->Type != HUNTER_PET) || srcPetSlot == dstPetSlot)
    {
        SendStableResult(STABLE_ERR_INTERNAL);
        return;
    }

    Optional<PetStable::PetInfo>* src = nullptr;
    Optional<PetStable::PetInfo>* dst = nullptr;
    Optional<uint32> newActivePetIndex;

    // A pet leaving the active slots is dismissed first, and one joining them must still be controllable
    auto dismissIfCurrent = [&](PetSaveMode activeSlot) -> bool
    {
        if (petStable->GetCurrentActivePetIndex() != uint32(activeSlot))
            return true;

        Pet* oldPet = _player->GetPet();
        if (oldPet && !oldPet->IsAlive())
            return false;

        _player->RemovePet(oldPet, PET_SAVE_NOT_IN_SLOT);
        return true;
    };
    auto isControllable = [&](PetStable::PetInfo const* info) -> bool
    {
        CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(info->CreatureId);
        return creatureInfo && creatureInfo->isTameable(_player);
    };

    if (IsActivePetSlot(srcPetSlot) && IsActivePetSlot(dstPetSlot))
    {
        src = &petStable->ActivePets[srcPetSlot - PET_SAVE_FIRST_ACTIVE_SLOT];
        dst = &petStable->ActivePets[dstPetSlot - PET_SAVE_FIRST_ACTIVE_SLOT];

        if (petStable->GetCurrentActivePetIndex() == uint32(srcPetSlot))
            newActivePetIndex = dstPetSlot;
        else if (petStable->GetCurrentActivePetIndex() == uint32(dstPetSlot))
            newActivePetIndex = srcPetSlot;
    }
    else if (IsStabledPetSlot(srcPetSlot) && IsStabledPetSlot(dstPetSlot))
    {
        src = &petStable->StabledPets[srcPetSlot - PET_SAVE_FIRST_STABLE_SLOT];
        dst = &petStable->StabledPets[dstPetSlot - PET_SAVE_FIRST_STABLE_SLOT];
    }
    else if (IsActivePetSlot(srcPetSlot) && IsStabledPetSlot(dstPetSlot))
    {
        if (dstPet && !isControllable(dstPet))
        {
            SendStableResult(STABLE_ERR_EXOTIC);
            return;
        }

        if (!dismissIfCurrent(srcPetSlot))
        {
            SendStableResult(STABLE_ERR_INTERNAL);
            return;
        }

        src = &petStable->ActivePets[srcPetSlot - PET_SAVE_FIRST_ACTIVE_SLOT];
        dst = &petStable->StabledPets[dstPetSlot - PET_SAVE_FIRST_STABLE_SLOT];
    }
    else if (IsStabledPetSlot(srcPetSlot) && IsActivePetSlot(dstPetSlot))
    {
        if (!isControllable(srcPet))
        {
            SendStableResult(STABLE_ERR_EXOTIC);
            return;
        }

        if (!dismissIfCurrent(dstPetSlot))
        {
            SendStableResult(STABLE_ERR_INTERNAL);
            return;
        }

        src = &petStable->StabledPets[srcPetSlot - PET_SAVE_FIRST_STABLE_SLOT];
        dst = &petStable->ActivePets[dstPetSlot - PET_SAVE_FIRST_ACTIVE_SLOT];
    }
    else
    {
        SendStableResult(STABLE_ERR_INVALID_SLOT);
        return;
    }

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);
    stmt->setInt16(0, int16(dstPetSlot));
    stmt->setUInt64(1, _player->GetGUID().GetGUIDLow());
    stmt->setUInt32(2, srcPet->PetNumber);
    trans->Append(stmt);

    if (dstPet)
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SLOT_BY_ID);
        stmt->setInt16(0, int16(srcPetSlot));
        stmt->setUInt64(1, _player->GetGUID().GetGUIDLow());
        stmt->setUInt32(2, dstPet->PetNumber);
        trans->Append(stmt);
    }

    bool const toStable = IsStabledPetSlot(dstPetSlot);
    AddTransactionCallback(CharacterDatabase.AsyncCommitTransaction(trans)).AfterComplete(
        [this, playerGuid = _player->GetGUID(), npcGuid = packet.NpcGUID, src, dst, newActivePetIndex, toStable](bool success)
    {
        if (!_player || _player->GetGUID() != playerGuid)
            return;

        if (!success)
        {
            SendStableResult(STABLE_ERR_INTERNAL);
            return;
        }

        std::swap(*src, *dst);
        if (newActivePetIndex)
            _player->GetPetStable()->SetCurrentActivePetIndex(*newActivePetIndex);

        SendStableResult(toStable ? STABLE_SUCCESS_STABLE : STABLE_SUCCESS_UNSTABLE);
        // Refreshes the stable window and the Call Pet icons
        SendStablePet(npcGuid);
    });
}

void WorldSession::SendStableResult(StableResultCode res)
{
    WorldPackets::PetPackets::StableResult stableResult;
    stableResult.Result = res;
    SendPacket(stableResult.Write());
}

void WorldSession::HandlePetActionHelper(Unit* pet, ObjectGuid petGuid, uint32 spellid, uint16 flag, ObjectGuid targetGuid, Position const& pos)
{
    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
    {
        TC_LOG_ERROR("network", "WorldSession::HandlePetAction(petGuid: " UI64FMTD ", tagGuid: " UI64FMTD ", spellId: %u, flag: %u): object (entry: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!", petGuid.GetCounter(), targetGuid.GetCounter(), spellid, flag, pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    switch (flag)
    {
        case ACT_COMMAND:                                   //0x07
            switch (spellid)
            {
                case COMMAND_STAY:                          //flat=1792  //STAY
                    // pet->AttackStop();
                    pet->CombatStop(true);
                    pet->StopMoving();
                    pet->GetMotionMaster()->Clear(false);
                    pet->GetMotionMaster()->MoveIdle();
                    charmInfo->SetCommandState(COMMAND_STAY);

                    charmInfo->SetIsCommandAttack(false);
                    charmInfo->SetIsAtStay(true);
                    charmInfo->SetIsFollowing(false);
                    charmInfo->SetIsReturning(false);
                    charmInfo->SaveStayPosition();
                    break;
                case COMMAND_FOLLOW:                        //spellid=1792  //FOLLOW
                    // pet->AttackStop();
                    pet->CombatStop(true);
                    // pet->InterruptNonMeleeSpells(false);
                    pet->GetMotionMaster()->MoveFollow(_player, pet->GetFollowDistance(), pet->GetFollowAngle());
                    charmInfo->SetCommandState(COMMAND_FOLLOW);

                    charmInfo->SetIsCommandAttack(false);
                    charmInfo->SetIsAtStay(false);
                    // Already beside the owner no follow move ends, and the flag would block every attack until the owner moves
                    charmInfo->SetIsReturning(pet->GetDistance(_player) > sWorld->getRate(RATE_TARGET_POS_RECALCULATION_RANGE));
                    charmInfo->SetIsFollowing(false);
                    break;
                case COMMAND_ATTACK:                        //spellid=1792  //ATTACK
                {
                    // Can't attack if owner is pacified
                    if (_player->HasAuraType(SPELL_AURA_MOD_PACIFY))
                    {
                        //pet->SendPetCastFail(spellid, SPELL_FAILED_PACIFIED);
                        //TODO: Send proper error message to client
                        return;
                    }

                    // only place where pet can be player
                    Unit* TargetUnit = ObjectAccessor::GetUnit(*_player, targetGuid);
                    if (!TargetUnit)
                        return;

                    if (Unit* owner = pet->GetOwner())
                        if (owner->GetDistance(TargetUnit) > SIGHT_RANGE_UNIT || !owner->IsValidAttackTarget(TargetUnit))
                            return;

                    // Not let attack through obstructions
                    if (sWorld->getBoolConfig(CONFIG_PET_LOS))
                    {
                        if (!pet->IsWithinLOSInMap(TargetUnit))
                            return;
                    }

                    pet->ClearUnitState(UNIT_STATE_FOLLOW);
                    // This is true if pet has no target or has target but targets differs.
                    if (pet->getVictim() != TargetUnit || (pet->getVictim() == TargetUnit && !pet->GetCharmInfo()->IsCommandAttack()))
                    {
                        if (pet->getVictim())
                            pet->AttackStop();

                        if (!pet->IsPlayer() && pet->ToCreature()->IsAIEnabled)
                        {
                            charmInfo->SetIsCommandAttack(true);
                            charmInfo->SetIsAtStay(false);
                            charmInfo->SetIsFollowing(false);
                            charmInfo->SetIsReturning(false);

                            pet->ToCreature()->AI()->AttackStart(TargetUnit);

                            //10% chance to play special pet attack talk, else growl
                            if (pet->ToCreature()->isPet() && pet->ToPet()->getPetType() == SUMMON_PET && pet != TargetUnit && urand(0, 100) < 10)
                                pet->SendPetTalk(static_cast<uint32>(PET_TALK_ATTACK));
                            else
                                pet->SendPetAIReaction(petGuid); // 90% chance for pet and 100% chance for charmed creature
                        }
                        else                                // charmed player
                        {
                            if (pet->getVictim() && pet->getVictim() != TargetUnit)
                                pet->AttackStop();

                            charmInfo->SetIsCommandAttack(true);
                            charmInfo->SetIsAtStay(false);
                            charmInfo->SetIsFollowing(false);
                            charmInfo->SetIsReturning(false);

                            pet->Attack(TargetUnit, true);
                            pet->SendPetAIReaction(petGuid);
                        }
                    }
                    break;
                }
                case COMMAND_ABANDON:                       // abandon (hunter pet) or dismiss (summoned pet)
                    if (pet->GetCharmerGUID() == GetPlayer()->GetGUID())
                        _player->StopCastingCharm();
                    else if (pet->GetOwnerGUID() == GetPlayer()->GetGUID())
                    {
                        ASSERT(pet->GetTypeId() == TYPEID_UNIT);
                        if (pet->isPet())
                        {
                            if (((Pet*)pet)->getPetType() == HUNTER_PET)
                                GetPlayer()->RemovePet((Pet*)pet, PET_SAVE_AS_DELETED);
                            else
                                //dismissing a summoned pet is like killing them (this prevents returning a soulshard...)
                                //pet->setDeathState(CORPSE);
                                GetPlayer()->RemovePet((Pet*)pet, PET_SAVE_NOT_IN_SLOT);
                        }
                        else if (pet->HasUnitTypeMask(UNIT_MASK_MINION))
                        {
                            ((Minion*)pet)->UnSummon();
                        }
                    }
                    break;
                case COMMAND_MOVE_TO:
                    pet->CombatStop(true);
                    pet->StopMoving();
                    pet->GetMotionMaster()->Clear(false);
                    pet->GetMotionMaster()->MovePoint(0, pos);
                    charmInfo->SetCommandState(COMMAND_MOVE_TO);
                    charmInfo->SetIsCommandAttack(false);
                    charmInfo->SetIsAtStay(true);
                    charmInfo->SetIsFollowing(false);
                    charmInfo->SetIsReturning(false);
                    charmInfo->SaveStayPosition();
                    break;

                default:
                    TC_LOG_ERROR("network", "WORLD: unknown PET flag Action %i and spellid %i.", uint32(flag), spellid);
            }
            break;
        case ACT_REACTION:                                  // 0x6
            switch (spellid)
            {
                case REACT_PASSIVE:                         //passive
                    pet->CombatStop(true);
                    charmInfo->SetIsCommandAttack(false);
                    // A following pet comes back to its owner, one told to stay goes back to its spot
                    if (charmInfo->HasCommandState(COMMAND_FOLLOW))
                        pet->GetMotionMaster()->MoveFollow(_player, pet->GetFollowDistance(), pet->GetFollowAngle());
                    else if (!charmInfo->IsAtStay())
                    {
                        float x, y, z;
                        charmInfo->GetStayPosition(x, y, z);
                        charmInfo->SetIsReturning(true);
                        pet->GetMotionMaster()->Clear(false);
                        pet->GetMotionMaster()->MovePoint(pet->GetGUIDLow(), x, y, z);
                    }
                case REACT_DEFENSIVE:                       //recovery
                case REACT_HELPER:
                case REACT_AGGRESSIVE:
                    if (pet->IsCreature())
                        pet->ToCreature()->SetReactState(ReactStates(spellid));
                    break;
                default:
                    break;
            }
            break;
        case ACT_DISABLED:                                  // 0x81    spell (disabled), ignore
        case ACT_PASSIVE:                                   // 0x01
        case ACT_ENABLED:                                   // 0xC1    spell
        {
            Unit* unit_target = nullptr;

            if (targetGuid)
                unit_target = ObjectAccessor::GetUnit(*_player, targetGuid);

            // do not cast unknown spells
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellid);
            if (!spellInfo)
            {
                TC_LOG_ERROR("network", "WORLD: unknown PET spell id %i", spellid);
                return;
            }

            if (spellInfo->Categories.StartRecoveryCategory)
            {
                if (pet->GetGlobalCooldownMgr().HasGlobalCooldown(spellInfo))
                    return;
            }

            for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            {
                if (spellInfo->EffectMask < uint32(1 << i))
                    break;

                if (spellInfo->Effects[i]->TargetA.GetTarget() == TARGET_UNIT_SRC_AREA_ENEMY || spellInfo->Effects[i]->TargetA.GetTarget() == TARGET_UNIT_DEST_AREA_ENEMY || spellInfo->Effects[i]->TargetA.GetTarget() == TARGET_DEST_DYNOBJ_ENEMY)
                    return;
            }

            // do not cast not learned spells
            if (!pet->HasSpell(spellid) || spellInfo->IsPassive())
                return;

            //  Clear the flags as if owner clicked 'attack'. AI will reset them
            //  after AttackStart, even if spell failed
            //  Only for a spell cast at an enemy: a heal or a buff would leave a passive or staying pet commanded to attack
            if (pet->GetCharmInfo() && unit_target && unit_target != pet && !spellInfo->IsPositive() && _player->IsValidAttackTarget(unit_target))
            {
                pet->GetCharmInfo()->SetIsAtStay(false);
                pet->GetCharmInfo()->SetIsCommandAttack(true);
                pet->GetCharmInfo()->SetIsReturning(false);
                pet->GetCharmInfo()->SetIsFollowing(false);
            }

            TriggerCastData triggerData;
            Spell* spell = new Spell(pet, spellInfo, triggerData);
            spell->preparePetCast(&(spell->m_targets), unit_target, pet, petGuid, GetPlayer());
            break;
        }
        default:
            TC_LOG_ERROR("network", "WORLD: unknown PET flag Action %i and spellid %i.", uint32(flag), spellid);
    }
}

void WorldSession::SendPetNameInvalid(uint32 error, ObjectGuid const& guid, std::string const& name, Optional<DeclinedName> const& declinedName)
{
    WorldPackets::PetPackets::PetNameInvalid petNameInvalid;
    petNameInvalid.Result = error;
    petNameInvalid.RenameData.NewName = name;
    petNameInvalid.RenameData.PetGUID = guid;
    petNameInvalid.RenameData.DeclinedNames = declinedName;

    SendPacket(petNameInvalid.Write());
}

void WorldSession::SendStablePet(ObjectGuid const& guid /*= ObjectGuid::Empty*/)
{
    WorldPackets::PetPackets::StableList packet;

    packet.StableMaster = guid;

    PetStable* petStable = GetPlayer()->GetPetStable();
    if (!petStable)
    {
        SendPacket(packet.Write());
        return;
    }

    for (uint32 petSlot = 0; petSlot < petStable->ActivePets.size(); ++petSlot)
    {
        if (!petStable->ActivePets[petSlot])
            continue;

        PetStable::PetInfo const& pet = *petStable->ActivePets[petSlot];
        WorldPackets::PetPackets::StableInfo& stableEntry = packet.Stables.emplace_back();
        stableEntry.PetSlot = petSlot + PET_SAVE_FIRST_ACTIVE_SLOT;
        stableEntry.PetNumber = pet.PetNumber;
        stableEntry.CreatureID = pet.CreatureId;
        stableEntry.DisplayID = pet.DisplayId;
        stableEntry.ExperienceLevel = pet.Level;
        stableEntry.PetFlags = PET_STABLE_ACTIVE;
        stableEntry.PetName = pet.Name;
    }

    for (uint32 petSlot = 0; petSlot < petStable->StabledPets.size(); ++petSlot)
    {
        if (!petStable->StabledPets[petSlot])
            continue;

        PetStable::PetInfo const& pet = *petStable->StabledPets[petSlot];
        WorldPackets::PetPackets::StableInfo& stableEntry = packet.Stables.emplace_back();
        stableEntry.PetSlot = petSlot + PET_SAVE_FIRST_STABLE_SLOT;
        stableEntry.PetNumber = pet.PetNumber;
        stableEntry.CreatureID = pet.CreatureId;
        stableEntry.DisplayID = pet.DisplayId;
        stableEntry.ExperienceLevel = pet.Level;
        stableEntry.PetFlags = PET_STABLE_INACTIVE;
        stableEntry.PetName = pet.Name;
    }

    SendPacket(packet.Write());
}