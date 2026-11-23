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

/*######
## npc_hogger
######*/

enum HoggerSpellData
{
    SPELL_SUMMON_MINIONS = 87366,
    SPELL_VICIOUS_SLICE = 87337,
    SPELL_EATING = 87351,
    SPELL_TELEPORT_VISUAL_ONLY_1 = 87459,
    SPELL_TELEPORT_VISUAL_ONLY_2 = 64446,
    SPELL_BLOODY_STRIKE = 87359
};

enum HoggerEventData
{
    EVENT_VICIOUS_SLICE = 1,
    EVENT_HAMMOND_GROUP_START_WALKING = 2,
    EVENT_DISMOUNT_HAMMOND_CLAY = 3,
    EVENT_HOGGER_SAY_GRR = 4,
    EVENT_CLAYS_EXPLAINATION = 5,
    EVENT_CLAY_SAYS_TAKE_HIM = 6,
    EVENT_HOGGER_SAYS_NOO = 7,
    EVENT_CLAY_SPEAKS_TO_ANDROMATH = 8,
    EVENT_TELEPORT_BACK = 9,
    EVENT_CHECK_EAT_RANGE = 10,
    EVENT_BLOODY_STRIKE = 11
};

enum HoggerTextData
{
    SAY_HOGGER_SUMMON_MINIONS = 2,
    SAY_EATING = 3,
    SAY_STUNNED = 4,
    SAY_BEG = 5,
    SAY_RAND = 6,
    SAY_FINAL = 7
};

enum HammondTextData
{
    YELL_OPENING = 0,
    SAY_EXPLAINATION = 1,
    SAY_TAKE_HIM = 2,
    SAY_TO_ANDROMATH = 3
};

enum RagamuffinTextData
{
    SAY_CLAY = 0,
    SAY_WOW = 1,
};

enum HoggerCreatures
{
    NPC_GENERAL_HAMMOND_CLAY = 65153,
    NPC_ANDROMATH = 46941,
    NPC_DUMAS = 46940,
    NPC_HOGGER = 448,
    NPC_EATING_TARGET = 45979,
    NPC_RAGAMUFFIN = 46943,
    // NPC_HOGGER_MINION = 46932
};

enum HoggerMiscData
{
    DISPLAYID_GENERAL_HAMMOND_CLAYS_MOUNT = 2410
};

// validated positions
static const Position generalHammondClayPositions[4] =
{
    { -10125.35f, 650.7324f, 36.05776f },
    { -10128.3f,  656.4648f, 36.05776f },
    { -10131.25f, 662.1973f, 36.05776f },
    { -10135.73f, 668.389f,  35.74807f }
};

// validated positions
static const Position andromathPositions[3] =
{
    { -10119.2f, 647.913f, 36.36745f },
    { -10123.0f, 656.875f, 36.05776f },
    { -10126.8f, 665.837f, 35.74807f }
};

// validated positions
static const Position dumasPositions[3] =
{
    { -10130.1f, 647.7671f, 36.04665f },
    { -10132.9f, 653.5605f, 36.05776f },
    { -10135.7f, 659.354f,  36.06887f }
};

// validated positions
static const Position hoggerPositions[1] =
{
    { -10136.9f, 670.009f, 36.03682f }
};

// validated positions
static const Position ragamuffinPositions[2] =
{
    { -10127.00f, 651.0f, 36.05776f },
    { -10123.0f, 651.0f,  36.06887f }
};

struct npc_hogger : public ScriptedAI
{
    npc_hogger(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        _events.Reset();

        _minionsSummoned = false;
        _endingSceneActive = false;
        _alreadyEaten = false;
        _isEating = false;

        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
        me->SetReactState(REACT_AGGRESSIVE);
        me->SetWalk(false);
    }

    void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType /*dmgType*/) override
    {
        if (_endingSceneActive)
        {
            damage = 0;
            return;
        }

        if (!_minionsSummoned && me->HealthBelowPctDamaged(50, damage))
            SummonMinions();

        if (!_alreadyEaten && me->HealthBelowPctDamaged(30, damage))
            MoveToEatingPosition();


        if (me->GetHealth() <= damage)
        {
            damage = 0;
            me->SetHealth(1);
            StartEndingScene();
            RewardPlayers();
        }

        if (_isEating)
        {
            _isEating = false;
            me->SetReactState(REACT_AGGRESSIVE);
            Talk(SAY_STUNNED);
        }
    }

    void EnterCombat(Unit* /*who*/) override
    {
        _events.ScheduleEvent(EVENT_VICIOUS_SLICE, 3000);
    }

    void MoveToEatingPosition()
    {
        _alreadyEaten = true;

        if (Creature* target = me->FindNearestCreature(NPC_EATING_TARGET, 100.0f))
        {
            me->SetReactState(REACT_PASSIVE);
            me->GetMotionMaster()->MovePoint(0, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), true);
            _events.ScheduleEvent(EVENT_CHECK_EAT_RANGE, 200);
        }
    }

    void StartEndingScene()
    {
        _endingSceneActive = true;
        _events.Reset();

        me->SetReactState(REACT_PASSIVE);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
        me->StopMoving();
        me->AttackStop();

        Talk(SAY_BEG);

        SummonGeneralHammondClay();
        SummonAndromath();
        SummonDumas();

        _events.ScheduleEvent(EVENT_HAMMOND_GROUP_START_WALKING, 1000);
    }

    void RewardPlayers()
    {
        for (auto itr : me->getThreatManager().getThreatList())
            if (Player* player = ObjectAccessor::GetPlayer(*me, itr->getUnitGuid()))
                player->RewardPlayerAndGroupAtEvent(NPC_HOGGER, me);
    }

    void SummonGeneralHammondClay()
    {
        if (TempSummon* hammond = me->SummonCreature(NPC_GENERAL_HAMMOND_CLAY, generalHammondClayPositions[0]))
        {
            _generalHammondGUID = hammond->GetGUID();
            hammond->CastSpell(hammond, SPELL_TELEPORT_VISUAL_ONLY_1, true);
            hammond->Mount(DISPLAYID_GENERAL_HAMMOND_CLAYS_MOUNT);
            hammond->AI()->Talk(YELL_OPENING);
        }
        // summon ragamuffins and do text
        if (TempSummon* ragamuffin1 = me->SummonCreature(NPC_RAGAMUFFIN, ragamuffinPositions[0], TEMPSUMMON_TIMED_DESPAWN, 5000))
        {
            ragamuffin1->AI()->Talk(SAY_CLAY);
        }
        if (TempSummon* ragamuffin2 = me->SummonCreature(NPC_RAGAMUFFIN, ragamuffinPositions[1], TEMPSUMMON_TIMED_DESPAWN, 5000))
        {
            ragamuffin2->AI()->Talk(SAY_WOW);
        }
    }

    void MoveGeneralHammondClay()
    {
        if (GetHammond())
        {
            GetHammond()->SetWalk(true);
            GetHammond()->GetMotionMaster()->MovePoint(0, generalHammondClayPositions[2], true);
            _events.ScheduleEvent(EVENT_DISMOUNT_HAMMOND_CLAY, 8500);
        }
    }

    void SummonAndromath()
    {
        TempSummon* andromath = me->SummonCreature(NPC_ANDROMATH, andromathPositions[0]);
        if (andromath)
        {
            _andromathGUID = andromath->GetGUID();
            andromath->CastSpell(andromath, SPELL_TELEPORT_VISUAL_ONLY_1, true);
        }
    }

    void MoveAndromath()
    {
        if (GetAndromath())
        {
            GetAndromath()->SetWalk(true);
            GetAndromath()->GetMotionMaster()->MovePoint(0, andromathPositions[2], true);
        }
    }

    void SummonDumas()
    {
        TempSummon* dumas = me->SummonCreature(NPC_DUMAS, dumasPositions[0]);
        if (dumas)
        {
            _dumasGUID = dumas->GetGUID();
            dumas->CastSpell(dumas, SPELL_TELEPORT_VISUAL_ONLY_1, true);
        }
    }

    void MoveDumas()
    {
        if (GetDumas())
        {
            GetDumas()->SetWalk(true);
            GetDumas()->GetMotionMaster()->MovePoint(0, dumasPositions[2], true);
        }
    }

    void SummonMinions()
    {
        me->CastStop();
        Talk(SAY_HOGGER_SUMMON_MINIONS);
        DoCastSelf(SPELL_SUMMON_MINIONS, true);
        _minionsSummoned = true;
    }

    void MoveHoggerToFinalPosition()
    {
        me->SetWalk(true);
        me->GetMotionMaster()->MovePoint(0, hoggerPositions[0]);
    }

    void TeleportBack()
    {
        if (GetHammond() && GetAndromath() && GetDumas())
        {
            GetHammond()->CastSpell(GetHammond(), SPELL_TELEPORT_VISUAL_ONLY_2, true);
            GetAndromath()->CastSpell(GetAndromath(), SPELL_TELEPORT_VISUAL_ONLY_2, true);
            GetDumas()->CastSpell(GetDumas(), SPELL_TELEPORT_VISUAL_ONLY_2, true);
            DoCastSelf(SPELL_TELEPORT_VISUAL_ONLY_2, true);

            me->DisappearAndDie();
            GetHammond()->DisappearAndDie();
            GetAndromath()->DisappearAndDie();
            GetDumas()->DisappearAndDie();
        }
    }

    Creature* GetHammond()
    {
        return me->GetMap()->GetCreature(_generalHammondGUID);
    }

    Creature* GetAndromath()
    {
        return me->GetMap()->GetCreature(_andromathGUID);
    }

    Creature* GetDumas()
    {
        return me->GetMap()->GetCreature(_dumasGUID);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim() && !_endingSceneActive)
            return;

        _events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_VICIOUS_SLICE:
                DoCastVictim(SPELL_VICIOUS_SLICE);
                _events.Repeat(3s);
                break;

            case EVENT_HAMMOND_GROUP_START_WALKING:
                MoveGeneralHammondClay();
                MoveAndromath();
                MoveDumas();
                MoveHoggerToFinalPosition();
                break;

            case EVENT_DISMOUNT_HAMMOND_CLAY:
                if (GetHammond())
                {
                    me->SetFacingToObject(GetHammond());

                    GetHammond()->Dismount();
                    GetHammond()->GetMotionMaster()->MovePoint(0, generalHammondClayPositions[3], true);

                    _events.ScheduleEvent(EVENT_HOGGER_SAY_GRR, 3000);
                }
                break;

            case EVENT_HOGGER_SAY_GRR:
                Talk(SAY_RAND);
                _events.ScheduleEvent(EVENT_CLAYS_EXPLAINATION, 3000);
                break;

            case EVENT_CLAYS_EXPLAINATION:
                if (GetHammond())
                    GetHammond()->AI()->Talk(SAY_EXPLAINATION);
                _events.ScheduleEvent(EVENT_CLAY_SAYS_TAKE_HIM, 4500);
                break;

            case EVENT_CLAY_SAYS_TAKE_HIM:
                if (GetHammond())
                    GetHammond()->AI()->Talk(SAY_TAKE_HIM);
                _events.ScheduleEvent(EVENT_HOGGER_SAYS_NOO, 2000);
                break;

            case EVENT_HOGGER_SAYS_NOO:
                Talk(SAY_FINAL);
                _events.ScheduleEvent(EVENT_CLAY_SPEAKS_TO_ANDROMATH, 3000);
                break;

            case EVENT_CLAY_SPEAKS_TO_ANDROMATH:
                if (GetHammond() && GetAndromath())
                {
                    GetHammond()->SetFacingToObject(GetAndromath());
                    GetAndromath()->SetFacingToObject(GetHammond());
                    GetHammond()->AI()->Talk(SAY_TO_ANDROMATH);
                }
                _events.ScheduleEvent(EVENT_TELEPORT_BACK, 4000);
                break;

            case EVENT_TELEPORT_BACK:
                TeleportBack();
                break;

            case EVENT_CHECK_EAT_RANGE:
                if (!me->FindNearestCreature(NPC_EATING_TARGET, 3.0f))
                {
                    _events.ScheduleEvent(EVENT_CHECK_EAT_RANGE, 200);
                    break;
                }
                else
                {
                    DoCast(SPELL_EATING);
                    Talk(SAY_EATING);
                    _events.ScheduleEvent(EVENT_BLOODY_STRIKE, 100);
                    _isEating = true;
                }
                break;

            case EVENT_BLOODY_STRIKE:
                if (_isEating)
                {
                    if (Creature* dummy = me->FindNearestCreature(NPC_EATING_TARGET, 10.0f))
                        DoCast(dummy, SPELL_BLOODY_STRIKE, true);
                    _events.ScheduleEvent(EVENT_BLOODY_STRIKE, 1000);
                }

            default:
                break;
            }
        }

        if (!_endingSceneActive)
            DoMeleeAttackIfReady();
    }

private:
    EventMap _events;

    bool _minionsSummoned;
    bool _endingSceneActive;
    bool _alreadyEaten;
    bool _isEating;

    ObjectGuid _generalHammondGUID;
    ObjectGuid _andromathGUID;
    ObjectGuid _dumasGUID;
};

/*######
## npc_hogger_minion
######*/

enum HoggerMinionSpellData
{
    SPELL_ADVENTURERS_RUSH = 87402
};

struct npc_hogger_minion : public ScriptedAI
{
    npc_hogger_minion(Creature* creature) : ScriptedAI(creature){ }

    void Reset() override
    {
        me->SetReactState(REACT_AGGRESSIVE);

        if (Creature* hogger = me->FindNearestCreature(NPC_HOGGER, 35.0f, true))
            me->CastSpell(hogger, SPELL_ADVENTURERS_RUSH, true);
    }
};

// Northshire: the Stormwind infantry really fight the Blackrock worgs and win; Brother Paxton tends the wounded
enum NorthshireBattle
{
    NPC_STORMWIND_INFANTRY      = 49869,
    NPC_BLACKROCK_BATTLE_WORG   = 49871,
    NPC_BLACKROCK_SPY           = 49874,
    NPC_BROTHER_PAXTON          = 951,
    SAY_SPY_AGGRO               = 0,
    SPELL_PAXTON_RENEW          = 93094,
    SPELL_PAXTON_FORTITUDE      = 13864,
    SAY_PAXTON_HEAL             = 0,
    SAY_INFANTRY_CALL_FOR_HEAL  = 0,
    EVENT_PAXTON_TEND           = 1,
    EVENT_INFANTRY_LOOK         = 1,
    EVENT_INFANTRY_RECOVER      = 2,
};

// Each infantryman stands by his own worg (1 to 4 yards) and fights that one only, remembered by its spawn
float constexpr INFANTRY_ENGAGE_RANGE = 6.0f;
int32 constexpr INFANTRY_HEALTH_FLOOR_PCT = 30;

struct npc_stormwind_infantry : public ScriptedAI
{
    npc_stormwind_infantry(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;
    bool calledForHeal = false;
    uint32 fightPace = 12;
    uint64 ownWorgSpawn = 0;

    Creature* FindOwnWorg()
    {
        std::list<Creature*> worgs;
        me->GetCreatureListWithEntryInGrid(worgs, NPC_BLACKROCK_BATTLE_WORG, 20.0f);
        if (!ownWorgSpawn)
        {
            // the worg whose spawn point is nearest to the soldier's post is his for good
            Position const post = me->GetHomePosition();
            Creature* nearest = nullptr;
            float nearestDist = INFANTRY_ENGAGE_RANGE;
            for (Creature* worg : worgs)
            {
                if (!worg->GetDBTableGUIDLow())
                    continue;
                float const dist = post.GetExactDist2d(worg->GetHomePosition());
                if (dist <= nearestDist)
                {
                    nearest = worg;
                    nearestDist = dist;
                }
            }
            if (!nearest)
                return nullptr;
            ownWorgSpawn = nearest->GetDBTableGUIDLow();
        }

        for (Creature* worg : worgs)
            if (worg->GetDBTableGUIDLow() == ownWorgSpawn && worg->IsAlive() && !worg->IsInEvadeMode())
                return worg;
        return nullptr;
    }

    void Reset() override
    {
        // the core would give back most of the health at once out of combat: the soldier heals slowly instead
        me->setRegeneratingHealth(false);
        events.Reset();
        events.ScheduleEvent(EVENT_INFANTRY_LOOK, 1500);
        events.ScheduleEvent(EVENT_INFANTRY_RECOVER, 1000);
        calledForHeal = false;
        // between two worgs the soldier stays on guard, weapon raised
        me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_READY1H);
    }

    void JustReachedHome() override
    {
        me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_READY1H);
    }

    // Each fight draws its own pace, from 5 to 17 blows, so the soldiers never all finish together
    void EnterCombat(Unit* /*who*/) override
    {
        fightPace = urand(6, 18);
        me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_ONESHOT_NONE);
    }

    // The fight is paced in shares of health, whatever the levels; the worg barely scratches the soldier,
    // who never dies from it
    void DamageDealt(Unit* victim, uint32& damage, DamageEffectType /*damageType*/) override
    {
        if (damage && victim->GetEntry() == NPC_BLACKROCK_BATTLE_WORG)
            damage = std::max<uint32>(1, victim->CountPctFromMaxHealth(urand(fightPace - 3, fightPace + 3)));
    }

    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*dmgType*/) override
    {
        if (!damage || !attacker || attacker->GetEntry() != NPC_BLACKROCK_BATTLE_WORG)
            return;

        // fights follow one another every few seconds: a worg never takes a soldier below 30 % of his health
        uint64 const floor = me->CountPctFromMaxHealth(INFANTRY_HEALTH_FLOOR_PCT);
        damage = std::max<uint32>(1, me->CountPctFromMaxHealth(urand(1, 3)));
        if (me->GetHealth() <= floor)
            damage = 0;
        else if (me->GetHealth() - damage < floor)
            damage = uint32(me->GetHealth() - floor);
    }

    // Worgs are hostile now: without this, every soldier would run after all the worgs of the field.
    // His own worg is engaged by the look-out below
    void MoveInLineOfSight(Unit* who) override
    {
        if (who->GetEntry() == NPC_BLACKROCK_BATTLE_WORG)
            return;
        // the spies hide in the bushes: they are the players' business
        if (who->GetEntry() == NPC_BLACKROCK_SPY)
            return;

        ScriptedAI::MoveInLineOfSight(who);
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);
        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_INFANTRY_LOOK:
                    if (!me->isInCombat() && !me->IsInEvadeMode())
                        if (Creature* worg = FindOwnWorg())
                            AttackStart(worg);
                    events.ScheduleEvent(EVENT_INFANTRY_LOOK, 1500);
                    break;
                case EVENT_INFANTRY_RECOVER:
                    // back to full health from the 30 % floor in about seven seconds, before the worg returns
                    if (!me->isInCombat() && me->IsAlive() && me->GetHealth() < me->GetMaxHealth())
                        me->ModifyHealth(std::max<uint32>(1, me->CountPctFromMaxHealth(10)));
                    events.ScheduleEvent(EVENT_INFANTRY_RECOVER, 1000);
                    break;
                default:
                    break;
            }
        }

        if (!UpdateVictim())
            return;

        if (!calledForHeal && HealthBelowPct(55))
        {
            calledForHeal = true;
            Talk(SAY_INFANTRY_CALL_FOR_HEAL);
        }

        DoMeleeAttackIfReady();
    }
};

struct npc_blackrock_battle_worg : public ScriptedAI
{
    npc_blackrock_battle_worg(Creature* creature) : ScriptedAI(creature) { }

    // Felled by a soldier, the worg comes back 7 to 10 seconds later so that the fight goes on;
    // killed by a player, it keeps its corpse for the loot and the usual respawn
    void JustDied(Unit* killer) override
    {
        if (killer && killer->GetEntry() == NPC_STORMWIND_INFANTRY)
            me->DespawnOrUnsummon(2000, Seconds(urand(5, 8)));
    }

    // Players are fair game at the usual range; the soldiers start the fights themselves
    void MoveInLineOfSight(Unit* who) override
    {
        if (who->GetEntry() == NPC_STORMWIND_INFANTRY || who->GetEntry() == NPC_BROTHER_PAXTON)
            return;

        ScriptedAI::MoveInLineOfSight(who);
    }
};

// Hostile to players, but the soldiers and Paxton count as players for the factions: the spy leaves them alone
struct npc_blackrock_spy : public ScriptedAI
{
    npc_blackrock_spy(Creature* creature) : ScriptedAI(creature) { }

    void MoveInLineOfSight(Unit* who) override
    {
        if (who->GetEntry() == NPC_STORMWIND_INFANTRY || who->GetEntry() == NPC_BROTHER_PAXTON)
            return;

        ScriptedAI::MoveInLineOfSight(who);
    }

    void EnterCombat(Unit* who) override
    {
        if (who && who->IsPlayer() && roll_chance_i(25))
            Talk(SAY_SPY_AGGRO, who->GetGUID());
    }
};

struct npc_brother_paxton : public ScriptedAI
{
    npc_brother_paxton(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() override
    {
        events.Reset();
        events.ScheduleEvent(EVENT_PAXTON_TEND, urand(4000, 8000));
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);
        if (events.ExecuteEvent() == EVENT_PAXTON_TEND)
        {
            TendInfantry();
            events.ScheduleEvent(EVENT_PAXTON_TEND, urand(8000, 12000));
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

    // Paxton heals, he does not join the brawl around him
    void MoveInLineOfSight(Unit* who) override
    {
        if (who->GetEntry() == NPC_BLACKROCK_BATTLE_WORG || who->GetEntry() == NPC_BLACKROCK_SPY)
            return;

        ScriptedAI::MoveInLineOfSight(who);
    }

    void TendInfantry()
    {
        if (me->isInCombat() || me->HasUnitState(UNIT_STATE_CASTING))
            return;

        std::list<Creature*> soldiers;
        me->GetCreatureListWithEntryInGrid(soldiers, NPC_STORMWIND_INFANTRY, 30.0f);

        Creature* hurt = nullptr;
        Creature* unblessed = nullptr;
        for (Creature* soldier : soldiers)
        {
            if (!soldier->IsAlive())
                continue;
            if (soldier->GetHealthPct() < 100.0f && !soldier->HasAura(SPELL_PAXTON_RENEW) && (!hurt || soldier->GetHealthPct() < hurt->GetHealthPct()))
                hurt = soldier;
            if (!unblessed && !soldier->HasAura(SPELL_PAXTON_FORTITUDE))
                unblessed = soldier;
        }

        if (hurt)
        {
            me->SetFacingToObject(hurt);
            DoCast(hurt, SPELL_PAXTON_RENEW);
            // One heal in three is cried out, otherwise the yard would never be quiet
            if (roll_chance_i(33))
                Talk(SAY_PAXTON_HEAL, hurt->GetGUID());
        }
        else if (unblessed)
        {
            me->SetFacingToObject(unblessed);
            DoCast(unblessed, SPELL_PAXTON_FORTITUDE);
        }
    }
};

void AddSC_elwynn_forest()
{
    RegisterCreatureAI(npc_hogger);
    RegisterCreatureAI(npc_hogger_minion);
    RegisterCreatureAI(npc_brother_paxton);
    RegisterCreatureAI(npc_stormwind_infantry);
    RegisterCreatureAI(npc_blackrock_battle_worg);
    RegisterCreatureAI(npc_blackrock_spy);
}
