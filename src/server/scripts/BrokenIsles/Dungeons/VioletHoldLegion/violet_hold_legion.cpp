/*
    Dungeon : Violet Hold Legion 100-110
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "violet_hold_legion.h"

#define GOSSIP_I_WANT_IN    "I'm not fighting, so send me in now!"
#define SPAWN_TIME          20000

enum Sinclari
{
    SAY_SINCLARI_1              = 0,
    SAY_SINCLARI_2              = 1,
    SAY_KEEPER                  = 2,
    SAY_ELITE                   = 3,
    SAY_GUARDIAN                = 4,
};

Position const exitPos = {4556.62f, 4015.16f, 83.67f};
Position const plrTeleportPos = {4577.63f, 4015.38f, 83.60f, 6.27f};
Position const movDoorPos = {4585.82f, 4015.32f, 83.47f, 3.14f};

//102278
class npc_sinclari_vh_leg : public CreatureScript
{
public:
    npc_sinclari_vh_leg() : CreatureScript("npc_sinclari_vh_leg") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (InstanceScript* instance = creature->GetInstanceScript())
        {
            switch (instance->GetData(DATA_MAIN_EVENT_PHASE))
            {
                case NOT_STARTED:
                case FAIL: // Allow to start event if not started or wiped
                    player->ADD_GOSSIP_ITEM_DB(19229, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                    player->SEND_GOSSIP_MENU(28225, creature->GetGUID());
                    break;
                case IN_PROGRESS: // Allow to teleport inside if event is in progress
                    player->ADD_GOSSIP_ITEM(GossipOptionNpc::None, GOSSIP_I_WANT_IN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
                    player->SEND_GOSSIP_MENU(28225, creature->GetGUID());
                    break;
                default:
                    player->SEND_GOSSIP_MENU(13910, creature->GetGUID());
                    break;
            }
        }
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();

        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->ADD_GOSSIP_ITEM_DB(19230, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                player->SEND_GOSSIP_MENU(28226, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->CLOSE_GOSSIP_MENU();
                // only from a fresh or failed event: a menu left open must not restart a running one
                if (InstanceScript* instance = creature->GetInstanceScript())
                    if (instance->GetData(DATA_MAIN_EVENT_PHASE) != NOT_STARTED && instance->GetData(DATA_MAIN_EVENT_PHASE) != FAIL)
                        break;
                CAST_AI(npc_sinclari_vh_leg::npc_sinclariAI, (creature->AI()))->uiPhase = 1;
                if (InstanceScript* instance = creature->GetInstanceScript())
                    instance->SetData(DATA_MAIN_EVENT_PHASE, SPECIAL);
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                player->NearTeleportTo(plrTeleportPos.GetPositionX(), plrTeleportPos.GetPositionY(), plrTeleportPos.GetPositionZ(), plrTeleportPos.GetOrientation(), true);
                player->CLOSE_GOSSIP_MENU();
                break;
        }
        return true;
    }

    struct npc_sinclariAI : public ScriptedAI
    {
        npc_sinclariAI(Creature* creature) : ScriptedAI(creature), summons(me), uiPhase(0), uiTimer(0)
        {
            instance = creature->GetInstanceScript();
            me->SetReactState(REACT_PASSIVE);
        }

        InstanceScript* instance;
        SummonList summons;

        uint8  uiPhase;
        uint32 uiTimer;

        void Reset() override
        {
            uiPhase = 0;
            uiTimer = 0;
            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

            std::list<Creature*> GuardList;
            me->GetCreatureListWithEntryInGrid(GuardList, NPC_VIOLET_HOLD_GUARD, 300.0f);
            if (!GuardList.empty())
            {
                for (std::list<Creature*>::const_iterator itr = GuardList.begin(); itr != GuardList.end(); ++itr)
                {
                    if (Creature* pGuard = *itr)
                    {
                        pGuard->DisappearAndDie();
                        pGuard->Respawn();
                        pGuard->SetVisible(true);
                    }
                }
            }
        }

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);
        }

        void DoAction(int32 const action) override
        {
            summons.DespawnAll();
        }

        void UpdateAI(uint32 uiDiff) override
        {
            ScriptedAI::UpdateAI(uiDiff);

            if (uiPhase)
            {
                if (uiTimer <= uiDiff)
                {
                    switch (uiPhase)
                    {
                        case 1:
                            Talk(SAY_SINCLARI_1);
                            uiTimer = 4000;
                            uiPhase = 2;
                            break;
                        // the guards fall back first, then the crystal clears the demons
                        case 2:
                        {
                            std::list<Creature*> GuardList;
                            me->GetCreatureListWithEntryInGrid(GuardList, NPC_VIOLET_HOLD_GUARD, 300.0f);
                            if (!GuardList.empty())
                                for (std::list<Creature*>::const_iterator itr = GuardList.begin(); itr != GuardList.end(); ++itr)
                                {
                                    if (Creature* pGuard = *itr)
                                    {
                                        // they run for the exit without fighting back, whatever hits them
                                        pGuard->CombatStop(true);
                                        pGuard->SetReactState(REACT_PASSIVE);
                                        pGuard->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE); // the demons keep after them
                                        pGuard->SetWalk(false);
                                        pGuard->GetMotionMaster()->MovePoint(0, exitPos);
                                    }
                                }
                            uiTimer = 3000;
                            uiPhase = 3;
                            break;
                        }
                        case 3:
                            if (instance)
                                instance->SetData(DATA_INTRO_CRYSTAL, 1);
                            uiTimer = 3000;
                            uiPhase = 4;
                            break;
                        case 4:
                        {
                            std::list<Creature*> GuardList;
                            me->GetCreatureListWithEntryInGrid(GuardList, NPC_VIOLET_HOLD_GUARD, 300.0f);
                            if (!GuardList.empty())
                                for (std::list<Creature*>::const_iterator itr = GuardList.begin(); itr != GuardList.end(); ++itr)
                                {
                                    if (Creature* pGuard = *itr)
                                    {
                                        pGuard->SetVisible(false);
                                        pGuard->SetReactState(REACT_PASSIVE);
                                    }
                                }
                            uiTimer = 2000;
                            uiPhase = 5;
                            break;
                        }
                        case 5:
                            Talk(SAY_SINCLARI_2);
                            me->GetMotionMaster()->MovePoint(0, exitPos);
                            uiTimer = 4000;
                            uiPhase = 6;
                            break;
                        case 6:
                            if (instance)
                                instance->SetData(DATA_MAIN_EVENT_PHASE, IN_PROGRESS);
                            uiTimer = 0;
                            uiPhase = 0;
                            break;
                    }
                }
                else uiTimer -= uiDiff;
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_sinclariAI(creature);
    }
};

//102279
class npc_teleportation_portal_vh_leg : public CreatureScript
{
public:
    npc_teleportation_portal_vh_leg() : CreatureScript("npc_teleportation_portal_vh_leg") { }

    struct npc_teleportation_portalAI : public ScriptedAI
    {
        npc_teleportation_portalAI(Creature* creature) : ScriptedAI(creature), uiSpawnTimer(10000), summons(me)
        {
            instance = creature->GetInstanceScript();
            uiTypeOfMobsPortal = urand(0, 99) < 30 ? 0 : 1; // 0 - elite squad (30%)   1 - portal guardian or portal keeper with regular mobs
            // an elite squad comes out while Malgath still channels, the portal closes as he leaves
            uiSpawnTimer = uiTypeOfMobsPortal ? 10000 : 4000;
            bPortalGuardianOrKeeperOrEliteSpawn = false;
        }

        uint32 uiSpawnTimer;
        bool bPortalGuardianOrKeeperOrEliteSpawn;
        uint8 uiTypeOfMobsPortal;
        ObjectGuid guardianGUID;

        // the instance decides the kind of portal when it opens it (OpenPortal)
        void SetData(uint32 id, uint32 value) override
        {
            if (id != DATA_PORTAL_KIND)
                return;

            uiTypeOfMobsPortal = uint8(value);
            uiSpawnTimer = uiTypeOfMobsPortal ? 10000 : 4000;
        }

        SummonList summons;

        InstanceScript* instance;

        void Reset() override
        {
            uiSpawnTimer = uiTypeOfMobsPortal ? 10000 : 4000;
            bPortalGuardianOrKeeperOrEliteSpawn = false;
        }

        void UpdateAI(uint32 diff) override
        {
            if (!instance) //Massive usage of instance, global check
                return;

            // no new enemies once the step's invasion forces are beaten
            bool wavesActive = instance->GetData(DATA_WAVES_ACTIVE) != 0;
            uint8 step = instance->GetData(DATA_STEP);

            switch (uiTypeOfMobsPortal)
            {
                // spawn the elite squad and then set portals visibility to make it look like it dissapeard
                case 0:
                    if (!bPortalGuardianOrKeeperOrEliteSpawn)
                    {
                        if (!wavesActive)
                            break;

                        if (uiSpawnTimer <= diff)
                        {
                            if (Creature* announcer = me->FindNearestCreature(NPC_LIEUTENANT_SINCLARI, 500.0f))
                                announcer->AI()->ZoneTalk(SAY_ELITE);
                            bPortalGuardianOrKeeperOrEliteSpawn = true;
                            uint8 k = step < 2 ? 2 : 3;
                            uint8 first = urand(0, 3);
                            for (uint8 i = 0; i < k; ++i)
                                DoSummon(eliteSquadEntries[(first + i) % 4], me, 2.0f, 20000, TEMPSUMMON_CORPSE_TIMED_DESPAWN);
                            me->SetVisible(false);
                        } else uiSpawnTimer -= diff;
                    }
                    else
                    {
                        // if all spawned elites have died kill portal
                        if (summons.empty())
                        {
                            me->Kill(me, false);
                            me->RemoveCorpse();
                        }
                    }
                    break;
                // spawn portal guardian or portal keeper with regular mobs
                case 1:
                    if (uiSpawnTimer <= diff)
                    {
                        if (bPortalGuardianOrKeeperOrEliteSpawn)
                        {
                            uint8 k = step < 2 ? 3 : 4;
                            for (uint8 i = 0; i < k && wavesActive; ++i)
                                DoSummon(portalTrashEntries[urand(0, 3)], me, 2.0f, 20000, TEMPSUMMON_CORPSE_TIMED_DESPAWN);
                        }
                        else if (wavesActive)
                        {
                            bPortalGuardianOrKeeperOrEliteSpawn = true;
                            uint32 entry = RAND(NPC_PORTAL_GUARDIAN_1, NPC_PORTAL_GUARDIAN_2, NPC_PORTAL_KEEPER_1, NPC_PORTAL_KEEPER_2);
                            if (Creature* pPortalKeeper = DoSummon(entry, me, 2.0f, 20000, TEMPSUMMON_CORPSE_TIMED_DESPAWN))
                            {
                                guardianGUID = pPortalKeeper->GetGUID();
                                me->CastSpell(pPortalKeeper, SPELL_PORTAL_CHANNEL, true);
                                pPortalKeeper->AI()->Talk(0);
                                if (Creature* announcer = me->FindNearestCreature(NPC_LIEUTENANT_SINCLARI, 500.0f))
                                {
                                    if (entry == NPC_PORTAL_GUARDIAN_1 || entry == NPC_PORTAL_GUARDIAN_2)
                                        announcer->AI()->ZoneTalk(SAY_GUARDIAN);
                                    else
                                        announcer->AI()->ZoneTalk(SAY_KEEPER);
                                }
                            }
                        }
                        uiSpawnTimer = SPAWN_TIME;
                    } else uiSpawnTimer -= diff;

                    // the portal stays open, sending reinforcements, until its guardian dies
                    Creature* guardian = guardianGUID.IsEmpty() ? nullptr : ObjectAccessor::GetCreature(*me, guardianGUID);
                    if (bPortalGuardianOrKeeperOrEliteSpawn && (!guardian || !guardian->IsAlive()))
                    {
                        me->Kill(me, false);
                        me->RemoveCorpse();
                    }
                    break;
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (instance)
                instance->SetGuidData(DATA_PORTAL_CLOSED, me->GetGUID());
        }

        void JustSummoned(Creature* summoned) override
        {
            summons.Summon(summoned);
            if (summoned)
                instance->SetGuidData(DATA_ADD_TRASH_MOB, summoned->GetGUID());
        }

        void SummonedCreatureDies(Creature* summoned, Unit* /*killer*/) override
        {
            summons.Despawn(summoned);
            if (summoned)
                instance->SetGuidData(DATA_DEL_TRASH_MOB, summoned->GetGUID());
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_teleportation_portalAI(creature);
    }
};

//102282
class npc_lord_malgath : public CreatureScript
{
public:
    npc_lord_malgath() : CreatureScript("npc_lord_malgath") {}

    struct npc_lord_malgathAI : public ScriptedAI
    {
        enum Modes
        {
            MODE_NONE       = 0,
            MODE_OPEN_CELL  = 1,
            MODE_FIGHT      = 2,
        };

        npc_lord_malgathAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
            uiBoss = 0;
            mode = MODE_NONE;
            eFight = false;
        }

        InstanceScript* instance;
        EventMap events;
        ObjectGuid portalGUID;

        bool eFight;
        uint8 uiBoss;
        uint8 mode;

        void Reset() override {}

        void EnterCombat(Unit* who) override
        {
            events.RescheduleEvent(EVENT_6, urand(12000, 17000)); //205046
            events.RescheduleEvent(EVENT_7, 30000); //204962
            events.RescheduleEvent(EVENT_8, 18000); //204963
            events.RescheduleEvent(EVENT_9, 19000); //204966
        }

        void DoAction(int32 const action) override
        {
            switch (action)
            {
                // end of the first two steps: he frees a prisoner
                case ACTION_MALGATH_OPEN_CELL:
                    uiBoss = instance ? instance->GetData(DATA_STEP_BOSS) : 0;
                    if (uiBoss >= DATA_BETRUG)
                    {
                        me->DespawnOrUnsummon();
                        return;
                    }
                    mode = MODE_OPEN_CELL;
                    Talk(malgathCellSay[uiBoss]);
                    events.RescheduleEvent(EVENT_1, 2000);
                    break;
                // end of the last step: he comes down himself
                case ACTION_MALGATH_FIGHT:
                    mode = MODE_FIGHT;
                    Talk(SAY_MALGATH_AGGRO);
                    events.RescheduleEvent(EVENT_4, 2000);
                    break;
                default:
                    break;
            }
        }

        void SetGUID(ObjectGuid const& guid, int32 id) override
        {
            if (id != ACTION_MALGATH_OPEN_PORTAL)
                return;

            portalGUID = guid;
            events.RescheduleEvent(EVENT_10, 1000);
        }

        void KilledUnit(Unit* victim) override
        {
            if (victim->IsPlayer())
                Talk(SAY_MALGATH_KILL);
        }

        void JustDied(Unit* /*killer*/) override
        {
            Talk(SAY_MALGATH_DEATH);

            if (!instance)
                return;

            instance->SetGuidData(DATA_DEL_TRASH_MOB, me->GetGUID());
            instance->SetData(DATA_MALGATH_DIED, DONE);
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (mode == MODE_OPEN_CELL)
                events.RescheduleEvent(EVENT_2, 1000);
            else if (mode == MODE_FIGHT)
                events.RescheduleEvent(EVENT_5, 1000);
        }

        void JustReachedHome() override
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
            me->RemoveAurasDueToSpell(SPELL_FEL_SHIELD);
            me->SetReactState(REACT_AGGRESSIVE);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() && eFight)
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        me->GetMotionMaster()->MovePoint(uiBoss, saboMovePos[uiBoss].GetPositionX(),
                                                                 saboMovePos[uiBoss].GetPositionY(),
                                                                 saboMovePos[uiBoss].GetPositionZ(), false);
                        break;
                    case EVENT_2:
                        DoCast(me, SPELL_SHIELD_DESTRUCTION, true);
                        events.RescheduleEvent(EVENT_3, 7000);
                        break;
                    case EVENT_3:
                        if (instance && instance->GetData(DATA_MAIN_EVENT_PHASE) == IN_PROGRESS)
                            instance->SetData(DATA_START_BOSS_ENCOUNTER, 1);
                        me->DespawnOrUnsummon(1000);
                        break;
                    case EVENT_4:
                        me->GetMotionMaster()->MovePoint(1, saboFightPos);
                        break;
                    case EVENT_5:
                        eFight = true;
                        me->SetDisableGravity(false);
                        me->SetCanFly(false);
                        instance->SetGuidData(DATA_ADD_TRASH_MOB, me->GetGUID());
                        me->SetOrientation(3.13f);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
                        me->RemoveAurasDueToSpell(SPELL_FEL_SHIELD);
                        me->SetReactState(REACT_AGGRESSIVE);
                        DoZoneInCombat(me, 100.0f);
                        me->SetHomePosition(me->GetPosition());
                        break;
                    case EVENT_6:
                        DoCast(205046);
                        events.RescheduleEvent(EVENT_6, urand(12000, 17000)); //205046
                        break;
                    case EVENT_7:
                        DoCast(204962);
                        events.RescheduleEvent(EVENT_7, 30000); //204962
                        break;
                    case EVENT_8:
                        DoCast(204963);
                        events.RescheduleEvent(EVENT_8, 18000); //204963
                        break;
                    case EVENT_9:
                        DoCast(204966);
                        events.RescheduleEvent(EVENT_9, 19000); //204966
                        break;
                    // he only shows up to open the portal, then leaves
                    case EVENT_10:
                        if (Creature* portal = ObjectAccessor::GetCreature(*me, portalGUID))
                        {
                            me->SetFacingToObject(portal);
                            DoCast(portal, SPELL_PORTAL_PERIODIC, true);
                        }
                        me->DespawnOrUnsummon(5000);
                        break;
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_lord_malgathAI (creature);
    }
};

//102272,102368,102380,102395,102400,102397,102398,102269,102369,102270,102370, 102336, 102302, 102337, 102335
class npc_violet_hold_trash : public CreatureScript
{
public:
    npc_violet_hold_trash() : CreatureScript("npc_violet_hold_trash") {}

    struct npc_violet_hold_trashAI : public ScriptedAI
    {
        npc_violet_hold_trashAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            me->SetReactState(REACT_DEFENSIVE);
            attackDoorTimer = 0;
            intro = false;
        }

        InstanceScript* instance;
        EventMap events;

        uint32 attackDoorTimer;
        bool intro;

        void AttackNearestGuard()
        {
            if (Creature* guard = me->FindNearestCreature(NPC_VIOLET_HOLD_GUARD, 100.0f))
            {
                me->SetReactState(REACT_AGGRESSIVE);
                AttackStart(guard);
            }
        }

        void Reset() override {}

        void EnterCombat(Unit* who) override
        {
            // a pet often opens the fight: the abilities have to start then too, or never at all
            if (!who->GetCharmerOrOwnerPlayerOrPlayerItself())
                return;

            attackDoorTimer = 0;
            me->SetReactState(REACT_AGGRESSIVE);
            if (me->GetEntry() == 102337)
            {
                events.RescheduleEvent(EVENT_1, 4000); // 204028
                events.RescheduleEvent(EVENT_2, 25000); // 204032
                events.RescheduleEvent(EVENT_3, 20000);// while under 204140 and get damage - cast 204208
            }
            if (me->GetEntry() == 102335)
            {
                Talk(1);
                events.RescheduleEvent(EVENT_4, 16000); // 204517
                events.RescheduleEvent(EVENT_5, 18000); // 204493   
            }
            if (me->GetEntry() == 102302)
            {
                events.RescheduleEvent(EVENT_6, 31000); // 204722  33
                events.RescheduleEvent(EVENT_7, 33000); // 204876
                events.RescheduleEvent(EVENT_8, 23000); // 204895
            }
            if (me->GetEntry() == 102336)
            {
                events.RescheduleEvent(EVENT_9, 9000); // 204951
                events.RescheduleEvent(EVENT_10, 27000); // 204901
                events.RescheduleEvent(EVENT_11, 14000); // 204948
            }
            if (me->GetEntry() == 102368 || me->GetEntry() == 102380 || me->GetEntry() == 102272 || me->GetEntry() == 102370 || me->GetEntry() == 102270)
                events.RescheduleEvent(EVENT_12, 15000); // 205115 205123
            if (me->GetEntry() == 102395)
                events.RescheduleEvent(EVENT_13, 27000); // 205097 and cast 205093
            if (me->GetEntry() == 102400)
            {
                DoCast(205099);
                events.RescheduleEvent(EVENT_14, 20000); //205103
                events.RescheduleEvent(EVENT_15, 13000); //205102
            }
            if (me->GetEntry() == 102397)
            {
                DoCast(182405);
                events.RescheduleEvent(EVENT_16, 20000); // 205082
                events.RescheduleEvent(EVENT_17, 21000); // 205080
            }
            if (me->GetEntry() == 102369 || me->GetEntry() == 102269)
                events.RescheduleEvent(EVENT_18, 22000); // 205108
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (me->GetEntry() == 102337)
                Talk(urand(1, 5));
            if (me->GetEntry() == 102335)
                Talk(urand(2, 8));
            if (me->GetEntry() == 102302)
                Talk(2);
            if (me->GetEntry() == 102336)
                Talk(urand(1,2));
          
            me->RemoveAllAreaObjects();
        }
        
        void IsSummonedBy(Unit* summoner) override
        {
           if (summoner->GetEntry() == NPC_INTRO_PORTAL)
           {
               intro = true;
               AttackNearestGuard();
               return;
           }

           if (me->GetEntry() !=102336 && me->GetEntry() != 102302 && me->GetEntry() != 102337 && me->GetEntry() != 102335)
           {
               me->GetMotionMaster()->MovePoint(1, movDoorPos.GetPositionX() + irand(-5,5), movDoorPos.GetPositionY() + irand(-5,5), movDoorPos.GetPositionZ());
               me->SetHomePosition(movDoorPos);
           }
        }

        void JustReachedHome() override
        {
           if (intro)
           {
               AttackNearestGuard();
               return;
           }

           if (me->GetEntry() !=102336 && me->GetEntry() != 102302 && me->GetEntry() != 102337 && me->GetEntry() != 102335)
               CreatureStartAttackDoor();
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == 1 && !intro)
                CreatureStartAttackDoor();
        }

        void CreatureStartAttackDoor()
        {
            DoCast(me, SPELL_DESTROY_DOOR_SEAL, true);
            attackDoorTimer = 2000;
        }

        void UpdateAI(uint32 diff) override
        {
            if (attackDoorTimer)
            {
                if (attackDoorTimer <= diff)
                {
                    if (instance)
                        if (instance->GetData(DATA_DOOR_INTEGRITY) >= 1)
                            instance->SetData(DATA_DOOR_INTEGRITY, instance->GetData(DATA_DOOR_INTEGRITY) -1);
                    attackDoorTimer = 2000;
                }
                else
                    attackDoorTimer -= diff;
            }
            
            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING) || !UpdateVictim())
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_1:
                    DoCast(204028);
                    events.RescheduleEvent(EVENT_1, 4000); //204028
                    break;
                case EVENT_2:
                    DoCast(204032);
                    events.RescheduleEvent(EVENT_2, 25000); // 204032
                    break;
                case EVENT_3:
                    DoCast(204140);
                    events.RescheduleEvent(EVENT_3, 20000);// while under 204140 and get damage - cast 204208
                    break;
               case EVENT_4:
                    DoCast(204517);
                    events.RescheduleEvent(EVENT_4, 16000); // 204517
                    break;
                case EVENT_5:
                    DoCast(204493);
                    events.RescheduleEvent(EVENT_5, 18000); // 204493         
                    break;
                case EVENT_6:
                    DoCast(204722);
                    events.RescheduleEvent(EVENT_6, 33000); // 204722  33
                    break;
                case EVENT_7:
                    DoCast(204876);
                    events.RescheduleEvent(EVENT_7, 33000); // 204876
                    break;
                case EVENT_8:
                    DoCast(204895);
                    events.RescheduleEvent(EVENT_8, 23000); // 204895    
                    break;
                case EVENT_9:
                    DoCast(204951);
                    events.RescheduleEvent(EVENT_9, 9000); // 204951
                    break;
                case EVENT_10:
                    DoCast(204901);
                    events.RescheduleEvent(EVENT_10, 27000); // 204901
                    break;
                case EVENT_11:
                    DoCast(204948);
                    events.RescheduleEvent(EVENT_11, 14000); // 204948 
                    break;
                case EVENT_12:
                    if (me->GetEntry() == 102368 || me->GetEntry() == 102272)
                        DoCast(205115);
                    if (me->GetEntry() == 102380)
                        DoCast(205123);
                    if (me->GetEntry() == 102370 || me->GetEntry() == 102270)
                        DoCast(205513);
                    events.RescheduleEvent(EVENT_12, 15000); // 205115 205123
                    break;
                case EVENT_13:
                    DoCast(205097);
                    DoCast(205093);
                    events.RescheduleEvent(EVENT_13, 27000); // 205097 and cast 205093
                    break;
                case EVENT_14:
                    DoCast(205103);
                    events.RescheduleEvent(EVENT_14, 20000); //205103
                    break;
                case EVENT_15:
                    DoCast(205102);
                    events.RescheduleEvent(EVENT_15, 13000); //205102    
                    break;
                case EVENT_16:
                    DoCast(205082);
                    events.RescheduleEvent(EVENT_16, 20000); // 205082
                    break;
                case EVENT_17:
                    DoCast(205080);
                    events.RescheduleEvent(EVENT_17, 21000); // 205080        
                    break;
                case EVENT_18:
                    DoCast(205108);
                    events.RescheduleEvent(EVENT_18, 22000); // 205108
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_violet_hold_trashAI(creature);
    }
};

//102671
class npc_vh_prison_cell_mover : public CreatureScript
{
public:
    npc_vh_prison_cell_mover() : CreatureScript("npc_vh_prison_cell_mover") {}

    struct npc_vh_prison_cell_moverAI : public ScriptedAI
    {
        npc_vh_prison_cell_moverAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void Reset() override
        {
            for (int8 i = 0; i < 7; i++)
                if (me->GetDistance(bossStartMove[i]) < 14.0f)
                {
                    me->GetMotionMaster()->MoveIdle();
                    me->GetMotionMaster()->MovePath(me->GetEntry() * 100 + i, true);
                }
        }

        void UpdateAI(uint32 diff) override {}
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_vh_prison_cell_moverAI(creature);
    }
};

// 103312
class npc_soul_vortex : public CreatureScript
{
public:
    npc_soul_vortex() : CreatureScript("npc_soul_vortex") {}

    struct npc_soul_vortexAI : public ScriptedAI
    {
        npc_soul_vortexAI(Creature* creature) : ScriptedAI(creature) 
        {
           DoCast(204465);
           SetCombatMovement(false);
        }
        
          void UpdateAI(uint32 diff) override
          {
            if (Unit* target = me->FindNearestPlayer(100.0f))
            {
               if (me->IsWithinMeleeRange(target))
               {
                  DoCast(204498);
                  me->RemoveAreaObject(204465);
                  me->DespawnOrUnsummon(1);
               }
            }
          }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_soul_vortexAI(creature);
    }
};

class spell_shadow_bomb : public SpellScriptLoader
{
    public:
        spell_shadow_bomb() : SpellScriptLoader("spell_shadow_bomb") { }

        class spell_shadow_bomb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_shadow_bomb_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                    return;

                Unit* target = GetTarget();
                if (!target)
                    return;

                if (Creature* malgath = target->FindNearestCreature(102282, 500, true))
                    malgath->CastSpell(target, 204961, true);
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_shadow_bomb_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_shadow_bomb_AuraScript();
        }
};

// 102266 - Violet Hold Guard: before the start each hit takes 4% of the demon's health, so the fight in front of
// the intro portals lasts and looks real
struct npc_violet_hold_guard_leg : public ScriptedAI
{
    npc_violet_hold_guard_leg(Creature* creature) : ScriptedAI(creature) { }

    uint32 searchTimer = 0;
    ObjectGuid finishTarget;

    // Taken straight from the health: the core cancels NPC on NPC damage below 85% for creatures with action data
    // (Unit::DealDamage, IsNoDamage), which left the Felguard Destroyers stuck at 84%
    void DamageDealt(Unit* victim, uint32& damage, DamageEffectType /*damageType*/) override
    {
        if (victim->GetTypeId() != TYPEID_UNIT)
            return;

        uint32 hit = std::max<uint32>(1, uint32(victim->GetMaxHealth() * 0.04f));
        damage = 0;
        if (victim->GetHealth() > hit)
            victim->ModifyHealth(-int32(hit));
        else
            finishTarget = victim->GetGUID();
    }

    // the guards stay on the demons until they are dead instead of walking back to their post
    void UpdateAI(uint32 diff) override
    {
        if (me->HasReactState(REACT_PASSIVE))         // falling back when the event starts
            return;

        if (!UpdateVictim())
        {

            if (searchTimer <= diff)
            {
                searchTimer = 1000;
                if (Unit* demon = me->SelectNearestTarget(30.0f))
                    AttackStart(demon);
            }
            else
                searchTimer -= diff;
            return;
        }

        if (!finishTarget.IsEmpty())
        {
            if (Creature* demon = ObjectAccessor::GetCreature(*me, finishTarget))
                if (demon->IsAlive())
                    me->Kill(demon);
            finishTarget.Clear();
        }

        DoMeleeAttackIfReady();
    }
};

void AddSC_violet_hold_legion()
{
    RegisterCreatureAI(npc_violet_hold_guard_leg);
    new npc_sinclari_vh_leg();
    new npc_teleportation_portal_vh_leg();
    new npc_lord_malgath();
    new npc_violet_hold_trash();
    new npc_vh_prison_cell_mover();
    new npc_soul_vortex();
    new spell_shadow_bomb();
}