////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "ScriptedEscortAI.h"
#include "CreatureTextMgr.h"
#include "MapManager.h"
#include "ScriptMgr.h"
#include "GameObjectAI.h"
#include "PathGenerator.h"
#include "MMapFactory.h"
#include "MoveSpline.h"
#include "warden_prison_routes.h"
#include <functional>

static void VaultWardOpenFor(WorldObject* from, ObjectGuid const& viewer, uint32 duration);

// the demon hunter reaches the Vault stripped of the warglaives of Mardum, gone for good; Maiev's Our Last Hope gives the new ones
static void VaultStripWarglaives(Player* player)
{
    if (player->GetQuestStatus(38669) != QUEST_STATUS_NONE || player->IsQuestRewarded(38669) || player->IsQuestRewarded(38672))
        return;

    for (uint8 slot : { EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_OFFHAND })
        if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
            player->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
}

// the run of the freed Kayn and Altruis to the next hall
enum { VAULT_RUN_TO_HALL = 50, VAULT_PORTAL_SENDS_BACK = 51 };

// Scenery fights of the Vault: a demon the demon hunter cannot attack (immune to players on its spawn) fights the allies
// around it, freed Illidari and Vault Wardens, without wounding them nor being wounded, so that every floor looks under attack
static bool VaultSceneryDemon(Creature const* demon)
{
    // the slain demons lying on the floors (seen dead: UNIT_FLAG_PREVENT_EMOTES) stay down
    return demon->GetMapId() == 1468 && demon->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC)
        && !demon->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PREVENT_EMOTES);
}

static void VaultHoldAllies(Creature* demon)
{
    for (uint32 entry : { 96656u, 96645u, 100665u, 100667u, 97090u })
    {
        std::list<Creature*> allies;
        demon->GetCreatureListWithEntryInGrid(allies, entry, 10.0f);
        for (Creature* ally : allies)
        {
            if (!ally->IsAlive() || !ally->AI() || !ally->IsValidAttackTarget(demon))
                continue;
            if (ally->getThreatManager().getThreat(demon) < 1.0f)
                ally->AddThreat(demon, 1.0f);
            if (demon->getThreatManager().getThreat(ally) < 1.0f)
                demon->AddThreat(ally, 1.0f);
            if (!ally->getVictim())
                ally->AI()->AttackStart(demon);
            if (!demon->getVictim() && demon->AI())
                demon->AI()->AttackStart(ally);
        }
    }
}

// a runner disappears once at the end of its way (with the respawn delay given to a spawn of the world)
static void VaultVanishOnArrival(Creature* runner, Seconds respawn)
{
    runner->AddDelayedEvent(1000, [runner, respawn]() -> void
    {
        if (runner->movespline->Finalized())
            runner->DespawnOrUnsummon(0, respawn);
        else
            VaultVanishOnArrival(runner, respawn);
    });
}

static void VaultWhenArrived(Creature* runner, std::function<void()> then)
{
    runner->AddDelayedEvent(500, [runner, then]() -> void
    {
        if (runner->movespline->Finalized())
            then();
        else
            VaultWhenArrived(runner, then);
    });
}

// a runner reaches the start of its road over the open floor around it, then follows the road drawn off the walls and
// along the stairs (warden_prison_routes.h); only its first points when a count is given
static void VaultFollowRoad(Creature* runner, VaultRoad const& road, bool walk, Seconds respawn, uint32 points = 0)
{
    uint32 const size = points ? std::min(points, road.Size) : road.Size;
    std::vector<G3D::Vector3> path;
    path.reserve(size);
    for (uint32 i = 0; i < size; ++i)
        path.emplace_back(road.Points[i][0], road.Points[i][1], road.Points[i][2]);

    runner->SetWalk(walk);
    std::function<void()> const follow = [runner, path, walk, respawn]() -> void
    {
        runner->GetMotionMaster()->MoveSmoothPath(2, path.data(), path.size(), walk);
        VaultVanishOnArrival(runner, respawn);
    };
    if (runner->GetExactDist(path[0].x, path[0].y, path[0].z) < 2.0f)
    {
        follow();
        return;
    }
    runner->GetMotionMaster()->MovePoint(1, path[0].x, path[0].y, path[0].z);
    VaultWhenArrived(runner, follow);
}

// the end of the Vault: Kayn (choice 40374) or Altruis (choice 40375) goes on with the demon hunter
static bool VaultChoseKayn(Player const* player)
{
    return player->GetQuestRewardStatus(40374);
}

static Position const VaultCourtKayn = { 4272.4f, -450.3f, 259.6f, 4.7305f };
static Position const VaultCourtAltruis = { 4272.2f, -453.4f, 259.6f, 1.5876f };
static Position const VaultCourtKorvas = { 4282.9f, -451.8f, 259.7f, 3.1669f };
static Position const VaultChamberLeader = { 4186.1f, -610.0f, 255.3f, 4.6113f };
static Position const VaultChamberKorvas = { 4147.0f, -628.3f, 255.2f, 0.0f };

// a path of the navmesh hugs corners and edges: every 2 m it is pushed back to keep the given margin from walls and drops
static Movement::PointsArray VaultKeepOffWalls(Unit const* unit, Movement::PointsArray const& points, float margin)
{
    dtNavMeshQuery const* query = MMAP::MMapFactory::createOrGetMMapManager()->GetNavMeshQuery(unit->GetMapId(), unit->GetInstanceId());
    if (!query || points.size() < 2)
        return points;

    dtQueryFilter filter;
    float const extents[3] = { 2.0f, 4.0f, 2.0f };
    Movement::PointsArray out;
    for (size_t i = 1; i < points.size(); ++i)
    {
        G3D::Vector3 const& from = points[i - 1];
        G3D::Vector3 const& to = points[i];
        uint32 const steps = std::max<uint32>(1, uint32((to - from).length() / 2.0f));
        for (uint32 k = 1; k <= steps; ++k)
        {
            G3D::Vector3 point = from + (to - from) * (float(k) / float(steps));
            if (i + 1 < points.size() || k < steps)
            {
                float const pos[3] = { point.y, point.z, point.x };
                dtPolyRef ref = 0;
                float nearest[3];
                float dist = 0.0f, hit[3], normal[3];
                if (dtStatusSucceed(query->findNearestPoly(pos, extents, &filter, &ref, nearest)) && ref
                    && dtStatusSucceed(query->findDistanceToWall(ref, nearest, margin + 0.5f, &filter, &dist, hit, normal)) && dist < margin)
                {
                    float const moved[3] = { nearest[0] + normal[0] * (margin - dist), nearest[1], nearest[2] + normal[2] * (margin - dist) };
                    dtPolyRef movedRef = 0;
                    float onMesh[3];
                    if (dtStatusSucceed(query->findNearestPoly(moved, extents, &filter, &movedRef, onMesh)) && movedRef)
                    {
                        float height = onMesh[1];
                        query->getPolyHeight(movedRef, onMesh, &height);
                        point = G3D::Vector3(onMesh[2], onMesh[0], height);
                    }
                }
            }
            out.push_back(point);
        }
    }
    return out;
}

// Breaking Out, per player: the cells, walls and warglaives hidden for him; abandoning the quest puts them all back
static std::map<ObjectGuid, GuidSet> sVaultHiddenFor;

static void VaultHideFor(WorldObject* object, Player* player)
{
    object->AddToHideList(player->GetGUID());
    object->DestroyForPlayer(player);
    sVaultHiddenFor[player->GetGUID()].insert(object->GetGUID());
}

class player_vault_breaking_out_reset : public PlayerScript
{
public:
    player_vault_breaking_out_reset() : PlayerScript("player_vault_breaking_out_reset") { }

    void OnQuestAbandon(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() != 38672)
            return;

        // Kayn and Altruis freed by him go back to nothing
        for (uint32 entry : { 99631u, 99632u })
        {
            std::list<Creature*> list;
            player->GetCreatureListWithEntryInGrid(list, entry, 100.0f);
            for (Creature* creature : list)
                if (TempSummon* summon = creature->ToTempSummon())
                    if (summon->GetSummonerGUID() == player->GetGUID())
                        creature->DespawnOrUnsummon();
        }

        auto itr = sVaultHiddenFor.find(player->GetGUID());
        if (itr != sVaultHiddenFor.end())
        {
            for (ObjectGuid const& guid : itr->second)
            {
                WorldObject* object = guid.IsGameObject() ? static_cast<WorldObject*>(player->GetMap()->GetGameObject(guid)) : player->GetMap()->GetCreature(guid);
                if (object)
                    object->RemoveFromHideList(player->GetGUID());
            }
            sVaultHiddenFor.erase(itr);
        }
        player->UpdateObjectVisibility();
    }
};

// Return to the Black Temple turned in: the cinematic of the capture plays, the warglaives go while it hides the character
class player_vault_stripped_of_warglaives : public PlayerScript
{
public:
    player_vault_stripped_of_warglaives() : PlayerScript("player_vault_stripped_of_warglaives") { }

    void OnQuestReward(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == 38729)
            VaultStripWarglaives(player);
    }
};

//245728
const uint32 q38690[8] = {97614, 97622, 97618, 97616, 97615, 97620, 97617, 97619};

class go_q38690 : public GameObjectScript
{
public:
    go_q38690() : GameObjectScript("go_q38690") { }

    struct go_q38690AI : public GameObjectAI
    {
        go_q38690AI(GameObject* go) : GameObjectAI(go) { }

        enum data
        {
            QUEST           = 38690,
            SAY_FREED       = 3,
            HIDE_AFTER      = 25000,    // the broken cell stays in sight, then goes away...
            BACK_AFTER      = 5,        // ...and stands again whole a few seconds later, without building itself up
        };

        uint32 _hideTimer = 0;

        // the Illidari held in the cell steps out, says a word and runs off
        bool GossipUse(Player* player) override
        {
            if (go->HasFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE) || !go->isSpawned() || player->GetQuestStatus(QUEST) != QUEST_STATUS_INCOMPLETE)
                return false;

            uint32 entry = 0;
            for (uint32 freed : q38690)
                if (!player->GetReqKillOrCastCurrentCount(QUEST, freed))
                {
                    entry = freed;
                    break;
                }
            if (!entry)
                entry = q38690[urand(0, std::size(q38690) - 1)];
            else
                player->KilledMonsterCredit(entry);

            _hideTimer = HIDE_AFTER;

            Creature* illidari = go->SummonCreature(entry, go->GetPositionX(), go->GetPositionY(), go->GetPositionZ(), go->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 60000);
            if (!illidari)
                return false;

            ObjectGuid const playerGuid = player->GetGUID();
            illidari->AddDelayedEvent(1200, [illidari, playerGuid]() -> void { sCreatureTextMgr->SendChat(illidari, SAY_FREED, playerGuid); });
            // down the steps of its platform to the middle of the hall, where it joins the others and goes away
            illidari->AddDelayedEvent(3000, [illidari]() -> void
            {
                float const angle = frand(0.0f, 2.0f * float(M_PI));
                float x = 4322.0f + frand(0.0f, 5.0f) * std::cos(angle);
                float y = -490.0f + frand(0.0f, 5.0f) * std::sin(angle);
                float z = -286.0f;
                illidari->UpdateGroundPositionZ(x, y, z);
                PathGenerator path(illidari);
                path.CalculatePath(x, y, z);
                Movement::PointsArray const road = VaultKeepOffWalls(illidari, path.GetPath(), 1.0f);
                if (road.empty())
                {
                    illidari->DespawnOrUnsummon(500);
                    return;
                }
                float length = 0.0f;
                G3D::Vector3 last(illidari->GetPositionX(), illidari->GetPositionY(), illidari->GetPositionZ());
                for (G3D::Vector3 const& point : road)
                {
                    length += (point - last).length();
                    last = point;
                }
                float const speed = std::max(1.0f, illidari->GetSpeed(MOVE_RUN));
                illidari->GetMotionMaster()->MoveSmoothPath(1, road.data(), road.size(), false);
                illidari->DespawnOrUnsummon(uint32(length / speed * IN_MILLISECONDS) + 500);
            });
            return false;   // the cell itself breaks open as usual
        }

        void UpdateAI(uint32 diff) override
        {
            if (!_hideTimer)
                return;
            if (_hideTimer > diff)
            {
                _hideTimer -= diff;
                return;
            }
            _hideTimer = 0;

            // out of sight first, then made whole again: when it comes back the cell is simply there
            go->SetRespawnTime(BACK_AFTER);
            go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);
            go->SetLootState(GO_READY);
            go->SetGoState(GO_STATE_READY);
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q38690AI(go);
    }
};

//! 92782 92776
class npc_q38689 : public CreatureScript
{
public:
    npc_q38689() : CreatureScript("npc_q38689") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q38689AI(creature);
    }

    struct npc_q38689AI : public ScriptedAI
    {

        npc_q38689AI(Creature* creature) : ScriptedAI(creature)
        {
            creature->SetCorpseDelay(30);
            creature->SetRespawnDelay(15);
        }
        
        EventMap events;

        enum data
        {
            QUEST = 38689,
            SPELL_CREDIT = 133511,
            SPELL_CREDIT2 = 133509,
        };

        bool _comesBack = false;

        void Reset() override
        {
            events.Reset();
            if (_comesBack)
            {
                _comesBack = false;
                if (Creature* portal = me->FindNearestCreature(99501, 60.0f))
                    if (portal->AI())
                    {
                        portal->AI()->SetGUID(me->GetGUID(), VAULT_PORTAL_SENDS_BACK);
                        return;
                    }
            }
            // a slain demon lying on the floor (UNIT_FLAG_PREVENT_EMOTES) never gets up
            if (me->getVictim() || me->IsInEvadeMode() || VaultSceneryDemon(me) || me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PREVENT_EMOTES))
                return;

            std::list<Unit*> list;
            me->GetAttackableUnitListInRange(list, 70.0f);
            for (auto enemy : list)
            {
                if (enemy->ToPlayer())
                    continue;
                me->AI()->AttackStart(enemy);
                break;
            }
        }
        
        void EnterCombat(Unit* who) override
        {
            if (me->GetEntry() == 92782)
                events.RescheduleEvent(EVENT_1, 15000); // 200992
            if (me->GetEntry() == 92776)
            {
                if (!VaultSceneryDemon(me) && urand(0, 1) == 1)
                    Talk(urand(0, 10));
                events.RescheduleEvent(EVENT_2, 14000); // 200930
                events.RescheduleEvent(EVENT_3, 20000); // 200963
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            // in the halls with Legion Portals the demons placed there come back soon, one per spot, so that every freed Illidari
            // always has a foe: the portals send the rest
            if (!me->isSummon() && me->GetMapId() == 1468 && me->FindNearestCreature(99501, 60.0f))
            {
                me->SetRespawnDelay(30);   // also what the corpse removal would set again
                me->SetRespawnTime(30);
                me->SaveRespawnTime();
                // it comes back through the nearest portal, as a new demon would
                _comesBack = true;
            }

            std::list<Player*> lList;
            me->GetPlayerListInGrid(lList, 5.0f);
            for (auto player : lList)
            {
                if (player->GetQuestStatus(QUEST) == QUEST_STATUS_INCOMPLETE)
                {
                    uint32 ra = urand(2, 10);
                    for (uint8 i = 0; i < ra; i++)
                    {
                        player->CastSpell(player, SPELL_CREDIT, true);
                        player->CastSpell(player, SPELL_CREDIT2, true);
                    }
                }
            }
        }
        
        uint32 _holdTimer = 1000;

        void DamageDealt(Unit* victim, uint32& damage, DamageEffectType /*damageType*/) override
        {
            if (VaultSceneryDemon(me) && !victim->IsControlledByPlayer())
                damage = 0;
        }

        void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*damageType*/) override
        {
            if (VaultSceneryDemon(me) && (!attacker || !attacker->IsControlledByPlayer()))
                damage = 0;
        }

        void UpdateAI(uint32 diff) override
        {
            if (VaultSceneryDemon(me))
            {
                if (_holdTimer <= diff)
                {
                    _holdTimer = 1000;
                    if (me->GetDistance(me->GetHomePosition()) > 20.0f)
                        EnterEvadeMode();
                    else
                        VaultHoldAllies(me);
                }
                else
                    _holdTimer -= diff;
            }

            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(200992);
                        events.RescheduleEvent(EVENT_1, 15000); // 200992
                        break;
                    case EVENT_2:
                        DoCast(200930);
                        events.RescheduleEvent(EVENT_2, 14000); // 200930
                        break;
                    case EVENT_3:
                        DoCast(200963);
                        events.RescheduleEvent(EVENT_3, 20000); // 200963
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }

    };
};

//! 187864
class spell_q38723 : public SpellScriptLoader
{
public:
    spell_q38723() : SpellScriptLoader("spell_q38723") { }

    class spell_q38723_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_q38723_AuraScript);

        // the door and its seal stay shut while the scene plays (phases of zone 7814, entries 102 and 103)
        static void RefreshPhases(Player* player)
        {
            player->AddDelayedEvent(100, [player]() -> void
            {
                PhaseUpdateData phaseUpdate;
                phaseUpdate.AddConditionType(CONDITION_AURA);
                phaseUpdate.AddQuestUpdate(38723);
                player->GetPhaseMgr().NotifyConditionChanged(phaseUpdate);
            });
        }

        // inside the Vault of the Betrayer, by a near teleport: no loading screen
        static void MoveInside(Player* player)
        {
            if (player->GetDistance2d(4084.27f, -298.11f) > 10.0f)
                player->NearTeleportTo(4084.27f, -298.11f, -282.07f, 3.118031f);
        }

        // moved a moment before the scene ends, so the demon hunter already stands in the room when it fades out
        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* player = GetTarget() ? GetTarget()->ToPlayer() : nullptr;
            int32 const duration = GetAura()->GetDuration();
            // a scene with no duration of its own: the length set in spell_scene must also be the maximum, or the aura never ends
            if (duration > 0)
                GetAura()->SetMaxDuration(duration);
            // the demon hunter is not part of the scene: an invisible model for its length
            if (player)
            {
                player->SetDisplayId(11686);
                RefreshPhases(player);
                // the seal of the door is lost from sight once the quest is taken: the scene shows a copy for this player alone
                if (GameObject* seal = player->SummonGameObject(2601302, 4219.97f, -297.86f, -264.86f, 0.0f, 0.0f, 0.7071068f, 0.0f, 0.7071068f, 22, player->GetGUID()))
                    seal->SetGoState(GO_STATE_ACTIVE);
            }
            if (!player || duration <= 0)
                return;

            // a single near teleport over the whole way shows a loading screen: two shorter ones, halfway along the corridor
            // 2 s before the end, then inside 1 s before
            player->AddDelayedEvent(uint64(std::max(0, duration - 2000)), [player]() -> void
            {
                if (player->HasAura(187864) && player->GetDistance2d(4084.27f, -298.11f) > 10.0f)
                    player->NearTeleportTo(4160.0f, -298.0f, -281.4f, 3.118031f);
            });
            player->AddDelayedEvent(uint64(std::max(0, duration - 1000)), [player]() -> void
            {
                if (player->HasAura(187864))
                    MoveInside(player);
            });
        }

        // the scene left before its end: moved at once
        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* player = GetTarget() ? GetTarget()->ToPlayer() : nullptr;
            if (!player)
                return;

            player->RestoreDisplayId();
            MoveInside(player);
            player->KilledMonsterCredit(99303);

            // the door of the Vault of the Betrayer opens now the scene is over
            RefreshPhases(player);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_q38723_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ACTIVATE_SCENE, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_q38723_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ACTIVATE_SCENE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_q38723_AuraScript();
    }
};

// 199375 - Demon Link: Crusher and Sledge share their wounds between them, never with the demon hunter fighting them
class spell_vault_demon_link : public SpellScriptLoader
{
public:
    spell_vault_demon_link() : SpellScriptLoader("spell_vault_demon_link") { }

    // an aura put on every unit around the caster (target 7): the players are taken out of the list
    class spell_vault_demon_link_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_vault_demon_link_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target) { return !target->ToUnit() || target->ToUnit()->IsControlledByPlayer(); });
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_vault_demon_link_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_vault_demon_link_SpellScript();
    }

};

// 199645 - Face Kick: Crusher's kick threw the demon hunter off the platform; a shorter throw keeps the fight on it
class spell_vault_face_kick : public SpellScriptLoader
{
public:
    spell_vault_face_kick() : SpellScriptLoader("spell_vault_face_kick") { }

    class spell_vault_face_kick_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_vault_face_kick_SpellScript);

        void HandleKnockBack(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (caster && target)
                target->KnockbackFrom(caster->GetPositionX(), caster->GetPositionY(), 6.0f, 4.0f);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_vault_face_kick_SpellScript::HandleKnockBack, EFFECT_1, SPELL_EFFECT_KNOCK_BACK);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_vault_face_kick_SpellScript();
    }
};

// 201023 - the pulse of the Seeping Inquisitor Eye: 12 m around it in the data, it struck from afar and stacked with every eye;
// only who stands within 2 m of the eye is hit
class spell_vault_seeping_pulse : public SpellScriptLoader
{
public:
    spell_vault_seeping_pulse() : SpellScriptLoader("spell_vault_seeping_pulse") { }

    class spell_vault_seeping_pulse_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_vault_seeping_pulse_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Unit* caster = GetCaster();
            targets.remove_if([caster](WorldObject* target) { return !caster || caster->GetExactDist2d(target) > 2.0f; });
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_vault_seeping_pulse_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_vault_seeping_pulse_SpellScript();
    }
};

// 201025 - Explosive Shadows, 201038 - Mind Spike: Kethrazor's blows hit softer, about half as hard
class spell_vault_kethrazor_softened : public SpellScriptLoader
{
public:
    spell_vault_kethrazor_softened() : SpellScriptLoader("spell_vault_kethrazor_softened") { }

    class spell_vault_kethrazor_softened_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_vault_kethrazor_softened_SpellScript);

        void Soften()
        {
            // Mind Spike: -30 % then -20 % (56 %); Explosive Shadows: -30 % twice (49 %)
            int32 const damage = GetHitDamage();
            SetHitDamage(GetSpellInfo()->Id == 201038 ? damage * 56 / 100 : damage * 49 / 100);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_vault_kethrazor_softened_SpellScript::Soften);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_vault_kethrazor_softened_SpellScript();
    }
};

// 96681 - Ash'golm of Frozen in Time: it had no script. Its two attacks of the dungeon that need nothing of the dungeon's
// instance: Fissure, a line of eruptions from the middle of the room to its target, and Lava Wreath, a ring of lava around
// each enemy it strikes. Kept within 25 m of its place; the countermeasures freeze it (phases of zone 7814, 107 and 108)
struct npc_vault_ashgolm : public ScriptedAI
{
    npc_vault_ashgolm(Creature* creature) : ScriptedAI(creature) { }

    enum
    {
        SPELL_LAVA_WREATH       = 192631,
        SPELL_LAVA_WREATH_VIS   = 193354,
        SPELL_LAVA_WREATH_TRIG  = 193353,
        SPELL_FISSURE           = 192522,
        SPELL_FISSURE_TRIG      = 193355,
        EVENT_FISSURE           = 1,
        EVENT_LAVA_WREATH,
    };

    EventMap events;

    Position Centre() const
    {
        return { 4239.14f, -451.332f, 105.947f, 0.0f };
    }

    // the demon hunter does not fight it, it only freezes it: out of the demon hunter's reach
    uint32 _holdTimer = 1000;

    void Reset() override
    {
        events.Reset();
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
        // its template also kept every NPC off it: Kayn and the freed Illidari could not fight it
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
    }

    // Kayn (102393) and the freed Illidari keep at it; their blows deal nothing, so a token threat holds both sides
    void HoldContainers()
    {
        for (uint32 entry : { 102393u, 96656u })
        {
            std::list<Creature*> fighters;
            me->GetCreatureListWithEntryInGrid(fighters, entry, 35.0f);
            for (Creature* fighter : fighters)
            {
                if (!fighter->IsAlive() || !fighter->AI() || !fighter->IsValidAttackTarget(me))
                    continue;
                if (fighter->getThreatManager().getThreat(me) < 1.0f)
                    fighter->AddThreat(me, 1.0f);
                if (me->getThreatManager().getThreat(fighter) < 1.0f)
                    me->AddThreat(fighter, 1.0f);
                if (!fighter->getVictim())
                    fighter->AI()->AttackStart(me);
                if (!me->getVictim())
                    AttackStart(fighter);
            }
        }
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_FISSURE, 6000);
        events.ScheduleEvent(EVENT_LAVA_WREATH, 12000);
    }

    // Kayn and the freed Illidari only hold it until the countermeasures are on: neither side wounds the other
    // its lava and blows are set for the dungeon groups: the demon hunter, alone, takes 60 % less of them
    void DamageDealt(Unit* victim, uint32& damage, DamageEffectType /*damageType*/) override
    {
        if (!victim->IsControlledByPlayer())
            damage = 0;
        else
            damage = damage * 4 / 10;
    }

    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*damageType*/) override
    {
        if (!attacker || !attacker->IsControlledByPlayer())
            damage = 0;
    }

    void SpellHitTarget(Unit* target, SpellInfo const* spell) override
    {
        if (spell->Id == SPELL_LAVA_WREATH)
        {
            DoCast(target, SPELL_LAVA_WREATH_VIS, true);
            for (uint8 i = 0; i < 6; ++i)
            {
                Position const pos = target->GetNearPosition(5.0f, i * 1.2f);
                me->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_LAVA_WREATH_TRIG, true);
            }
        }
        else if (spell->Id == SPELL_FISSURE)
        {
            Position const centre = Centre();
            float const angle = centre.GetRelativeAngle(target);
            for (uint8 i = 0; i < 5; ++i)
            {
                Position pos;
                centre.SimplePosXYRelocationByAngle(pos, i * 6.0f, angle);
                me->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_FISSURE_TRIG, true);
            }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (_holdTimer <= diff)
        {
            _holdTimer = 1000;
            HoldContainers();
        }
        else
            _holdTimer -= diff;

        if (!UpdateVictim())
            return;

        if (me->GetDistance(me->GetHomePosition()) > 25.0f)
        {
            EnterEvadeMode();
            return;
        }

        events.Update(diff);
        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        switch (events.ExecuteEvent())
        {
            case EVENT_FISSURE:
                if (Unit* victim = me->getVictim())
                    DoCast(victim, SPELL_FISSURE);
                events.ScheduleEvent(EVENT_FISSURE, 15000);
                return;
            case EVENT_LAVA_WREATH:
                DoCast(SPELL_LAVA_WREATH);
                events.ScheduleEvent(EVENT_LAVA_WREATH, 25000);
                return;
            default:
                break;
        }

        DoMeleeAttackIfReady();
    }
};

// 96997 - Kethrazor, rare inquisitor of the Vault: no script at all, he only fought in melee
struct npc_vault_kethrazor : public ScriptedAI
{
    npc_vault_kethrazor(Creature* creature) : ScriptedAI(creature) { }

    enum
    {
        SPELL_EXPLOSIVE_EYES    = 200999,   // throws an Explosive Inquisitor Eye at the target's place
        SPELL_SEEPING_EYES      = 201033,   // summons a Seeping Inquisitor Eye nearby
        SPELL_MIND_SPIKE        = 201038,
        SAY_AGGRO               = 1,
        SAY_DEATH               = 2,
        EVENT_MIND_SPIKE        = 1,
        EVENT_SEEPING_EYES,
        EVENT_EXPLOSIVE_EYES,
    };

    EventMap events;

    void Reset() override
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) override
    {
        Talk(SAY_AGGRO);
        events.ScheduleEvent(EVENT_MIND_SPIKE, 3000);
        events.ScheduleEvent(EVENT_SEEPING_EYES, 7000);
    }

    void JustDied(Unit* /*killer*/) override
    {
        Talk(SAY_DEATH);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);
        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_MIND_SPIKE:
                    DoCastVictim(SPELL_MIND_SPIKE);
                    events.ScheduleEvent(EVENT_MIND_SPIKE, 3000);
                    return;
                // both kinds of eyes together, every 9 seconds; Mind Spike resumes once the summoning cast is over
                case EVENT_SEEPING_EYES:
                    // the explosive eye appears about 30 m from the demon hunter, time enough to see it come: summoned there
                    // directly, the missile spell picked its own landing place next to the target. A spot in sight and on the
                    // same floor is looked for around the target, else 20 m towards Kethrazor
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true))
                    {
                        float const start = frand(0.0f, 2.0f * float(M_PI));
                        bool found = false;
                        float x = 0.0f, y = 0.0f, z = 0.0f;
                        for (uint8 i = 0; i < 12 && !found; ++i)
                        {
                            float const angle = start + float(i) * float(M_PI) / 6.0f;
                            x = target->GetPositionX() + 30.0f * std::cos(angle);
                            y = target->GetPositionY() + 30.0f * std::sin(angle);
                            z = target->GetPositionZ() + 2.0f;
                            target->UpdateGroundPositionZ(x, y, z);
                            found = target->IsWithinLOS(x, y, z + 1.0f) && std::fabs(z - target->GetPositionZ()) < 3.0f;
                        }
                        if (!found)
                        {
                            float const back = target->GetAngle(me);
                            x = target->GetPositionX() + 20.0f * std::cos(back);
                            y = target->GetPositionY() + 20.0f * std::sin(back);
                            z = target->GetPositionZ() + 2.0f;
                            target->UpdateGroundPositionZ(x, y, z);
                        }
                        me->SummonCreature(101901, x, y, z, target->GetAngle(x, y) + float(M_PI), TEMPSUMMON_TIMED_DESPAWN, 20000);
                    }
                    DoCast(me, SPELL_SEEPING_EYES);
                    events.ScheduleEvent(EVENT_SEEPING_EYES, 9000);
                    events.RescheduleEvent(EVENT_MIND_SPIKE, 2500);
                    return;
                default:
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// 101901 - Explosive Inquisitor Eye: fixes on a demon hunter, drifts to it and bursts in shadow on reaching it
struct npc_vault_explosive_eye : public ScriptedAI
{
    npc_vault_explosive_eye(Creature* creature) : ScriptedAI(creature) { }

    enum
    {
        SPELL_FIXATE            = 201015,
        SPELL_EXPLOSIVE_SHADOWS = 201025,
    };

    ObjectGuid _target;
    uint32 _checkTimer = 500;
    bool _burst = false;

    void IsSummonedBy(Unit* summoner) override
    {
        me->SetReactState(REACT_PASSIVE);
        Player* target = nullptr;
        if (Unit* victim = summoner->getVictim())
            target = victim->ToPlayer();
        if (!target)
            target = me->SelectNearestPlayer(40.0f);
        if (!target)
        {
            me->DespawnOrUnsummon(1000);
            return;
        }
        _target = target->GetGUID();
        // the fixate aura is put on the demon hunter itself, which shows its purple mark: cast, it looks for enemies
        // in a small area around the eye and missed the target it follows
        me->AddAura(SPELL_FIXATE, target, nullptr, 0, 20000, 20000);   // as long as the eye lives
        me->GetMotionMaster()->MoveFollow(target, 0.0f, 0.0f);
        me->DespawnOrUnsummon(20000);
    }

    // its mark and beam on the demon hunter go with it
    void JustDied(Unit* /*killer*/) override
    {
        if (Player* target = ObjectAccessor::GetPlayer(*me, _target))
            target->RemoveAurasDueToSpell(SPELL_FIXATE, me->GetGUID());
    }

    void UpdateAI(uint32 diff) override
    {
        if (_burst || !_target)
            return;
        if (_checkTimer > diff)
        {
            _checkTimer -= diff;
            return;
        }
        _checkTimer = 250;

        Player* target = ObjectAccessor::GetPlayer(*me, _target);
        if (!target || !target->IsAlive())
        {
            me->DespawnOrUnsummon(500);
            return;
        }
        if (me->GetDistance(target) < 2.0f)
        {
            _burst = true;
            target->RemoveAurasDueToSpell(SPELL_FIXATE, me->GetGUID());
            DoCast(me, SPELL_EXPLOSIVE_SHADOWS, true);
            me->DespawnOrUnsummon(500);
        }
    }
};

// 101907 - Seeping Inquisitor Eye: stands where it is summoned and pulses shadow around it for its 40 seconds
struct npc_vault_seeping_eye : public ScriptedAI
{
    npc_vault_seeping_eye(Creature* creature) : ScriptedAI(creature) { }

    enum { SPELL_SEEPING_SHADOWS = 201023 };

    uint32 _pulseTimer = 2000;
    bool _lifeSet = false;

    // rooted from the start, whatever the way it was summoned (the summoning spell may not call IsSummonedBy),
    // and gone after 20 seconds
    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
        me->SetControlled(true, UNIT_STATE_ROOT);
        if (!_lifeSet)
        {
            _lifeSet = true;
            me->DespawnOrUnsummon(20000);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (_pulseTimer > diff)
        {
            _pulseTimer -= diff;
            return;
        }
        _pulseTimer = 2000;
        DoCast(me, SPELL_SEEPING_SHADOWS, true);
    }
};

// 92984, 92985 - Kayn and Altruis of the Vault of the Betrayer: without a script they had a passive AI that ignored
// every order to fight Crusher and Sledge
struct npc_vault_betrayer_ally : public ScriptedAI
{
    npc_vault_betrayer_ally(Creature* creature) : ScriptedAI(creature) { }

    // every fight of the Vault's allies is scenery: no one is wounded
    void DamageDealt(Unit* victim, uint32& damage, DamageEffectType /*damageType*/) override
    {
        if (!victim->IsControlledByPlayer())
            damage = 0;
    }

    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*damageType*/) override
    {
        if (!attacker || !attacker->IsControlledByPlayer())
            damage = 0;
    }

    void UpdateAI(uint32 /*diff*/) override
    {
        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

// 97225 Wrathguard Legate, 97333 Magolith: a fighting AI; on the scenery floors, a fight without wounds (VaultSceneryDemon)
struct npc_vault_scenery_demon : public ScriptedAI
{
    npc_vault_scenery_demon(Creature* creature) : ScriptedAI(creature) { }

    uint32 _holdTimer = 1000;
    uint32 _spellTimer = 12000;

    void Reset() override
    {
        _spellTimer = 12000;
    }

    void DamageDealt(Unit* victim, uint32& damage, DamageEffectType /*damageType*/) override
    {
        if (VaultSceneryDemon(me) && !victim->IsControlledByPlayer())
            damage = 0;
    }

    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*damageType*/) override
    {
        if (VaultSceneryDemon(me) && (!attacker || !attacker->IsControlledByPlayer()))
            damage = 0;
    }

    void UpdateAI(uint32 diff) override
    {
        if (VaultSceneryDemon(me))
        {
            if (_holdTimer <= diff)
            {
                _holdTimer = 1000;
                if (me->GetDistance(me->GetHomePosition()) > 20.0f)
                    EnterEvadeMode();
                else
                    VaultHoldAllies(me);
            }
            else
                _holdTimer -= diff;
        }

        if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
            return;

        // the Wrathguard Legate's own strike (its former SmartAI)
        if (me->GetEntry() == 97225)
        {
            if (_spellTimer <= diff)
            {
                _spellTimer = 12000;
                DoCastVictim(201077);
            }
            else
                _spellTimer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

// 97265 Kayn Sunfury, 97267 Altruis the Sufferer: in the Court of the Wardens they fight each other, friends as they are,
// with blows that land on nothing, until the Pool of Judgment reminds them (their copies of phase 4902)
struct npc_vault_court_duelist : public ScriptedAI
{
    npc_vault_court_duelist(Creature* creature) : ScriptedAI(creature) { }

    uint32 _strikeTimer = 1000;

    void UpdateAI(uint32 diff) override
    {
        if (!me->GetPhases().count(4902))
            return;
        if (_strikeTimer > diff)
        {
            _strikeTimer -= diff;
            return;
        }
        _strikeTimer = urand(1100, 1700);

        Creature* foe = me->FindNearestCreature(me->GetEntry() == 97265 ? 97267 : 97265, 8.0f);
        if (!foe)
            return;
        me->SetSheath(SHEATH_STATE_MELEE);
        me->SetFacingToObject(foe);
        me->HandleEmoteCommand(urand(0, 1) ? EMOTE_ONESHOT_ATTACK1H : EMOTE_ONESHOT_ATTACK2HTIGHT);
    }
};

//! 92990, 97632
class npc_q38723 : public CreatureScript
{
public:
    npc_q38723() : CreatureScript("npc_q38723") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q38723AI(creature);
    }

    struct npc_q38723AI : public ScriptedAI
    {

        npc_q38723AI(Creature* creature) : ScriptedAI(creature)
        {
            creature->SetCorpseDelay(30);
            creature->SetRespawnDelay(15);
        }

        EventMap events;
        enum data
        {
            QUEST = 38723,
            SPELL_CREDIT = 210461,
            SPELL_CREDIT2 = 195327,
        };

        bool _playerEngaged = false;
        uint32 _allyTimer = 1000;

        // Kayn and Altruis already fight them when the demon hunter comes in: brought down to a third, they hold there
        // until the demon hunter strikes
        void Reset() override
        {
            events.Reset();
            _playerEngaged = false;
            me->SetHealth(me->CountPctFromMaxHealth(33));

            if (me->getVictim() || me->IsInEvadeMode())
                return;

            std::list<Unit*> list;
            me->GetAttackableUnitListInRange(list, 70.0f);
            for (auto enemy : list)
            {
                if (enemy->ToPlayer())
                    continue;
                me->AI()->AttackStart(enemy);
                break;
            }
        }
        
        // each is held by its own: Kayn faces Sledge, Altruis faces Crusher. Their blows deal nothing, so they raise no
        // threat either: a token one is kept on both sides, or the fight would drop at once
        void EngageAllies()
        {
            uint32 const allyEntry = me->GetEntry() == 92990 ? 92984 : 92985;
            std::list<Creature*> allies;
            me->GetCreatureListWithEntryInGrid(allies, allyEntry, 45.0f);
            for (Creature* ally : allies)
            {
                if (!ally->IsAlive() || !ally->AI())
                    continue;
                // they stand in the vault immune to NPCs, which drops every fight at once
                if (ally->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC))
                    ally->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                if (!ally->IsValidAttackTarget(me))
                    continue;
                if (ally->getThreatManager().getThreat(me) < 1.0f)
                    ally->AddThreat(me, 1.0f);
                // their blows deal nothing but hold the brute's attention at a fixed level, about 2 % of its health: the demon
                // hunter takes it over by outdoing them, after a few blows
                float const held = float(me->GetMaxHealth()) / 50.0f;
                float const current = me->getThreatManager().getThreat(ally);
                if (current < held)
                    me->AddThreat(ally, held - current);
                if (!ally->getVictim())
                    ally->AI()->AttackStart(me);
                if (!me->getVictim())
                    AttackStart(ally);
            }
        }

        void DamageDealt(Unit* victim, uint32& damage, DamageEffectType /*damageType*/) override
        {
            if (!victim->IsControlledByPlayer())
                damage = 0;
        }

        void EnterCombat(Unit* who) override
        {
            if (me->GetEntry() == 92990)
                events.RescheduleEvent(EVENT_1, 20000); // 199602
            if (me->GetEntry() == 97632)
                events.RescheduleEvent(EVENT_2, 16000); // 199645
            events.RescheduleEvent(EVENT_3, 42000); // 199556
        }

        void DamageTaken(Unit* attacker, uint32 &damage, DamageEffectType dmgType) override
        {
            if (!attacker || !attacker->IsControlledByPlayer())
                damage = 0;
            else if (!_playerEngaged)
            {
                // the taunts start with the demon hunter's first blow
                _playerEngaged = true;
                if (me->GetEntry() == 92990)
                    events.RescheduleEvent(EVENT_4, 5000);
            }

            // Demon Link: they share their wounds, the other one follows the same share of health
            if (!damage)
                return;
            if (Creature* oAdd = GetOtherAdd())
            {
                if (damage >= me->GetHealth())
                    oAdd->Kill(oAdd, true);
                else
                    oAdd->SetHealth(std::max<uint64>(1, uint64(double(oAdd->GetMaxHealth()) * double(me->GetHealth() - damage) / double(me->GetMaxHealth()))));
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            std::list<Player*> lList;
            me->GetPlayerListInGrid(lList, 200.0f);
            for (auto player : lList)
            {
                if (player->GetQuestStatus(QUEST) == QUEST_STATUS_INCOMPLETE)
                {
                    me->CastSpell(player, SPELL_CREDIT, true);
                    player->KilledMonsterCredit(106241);

                    
                    player->CastSpell(player, SPELL_CREDIT2, true);
                }
            }
        }
        
        void UpdateAI(uint32 diff) override
        {
            if (_allyTimer <= diff)
            {
                _allyTimer = 1000;
                // a third of their health until the demon hunter strikes
                if (!_playerEngaged && me->GetHealthPct() > 34.0f)
                    me->SetHealth(me->CountPctFromMaxHealth(33));
                EngageAllies();
            }
            else
                _allyTimer -= diff;

            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);
            
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(199602);
                        events.RescheduleEvent(EVENT_1, 20000); // 199602
                        break;
                    case EVENT_2:
                        DoCast(199645);
                        events.RescheduleEvent(EVENT_2, 16000); // 199645
                        break;
                    case EVENT_3:
                        DoCast(199556);
                        events.RescheduleEvent(EVENT_3, 42000); // 199556
                        break;
                    case EVENT_4:
                    {
                        std::list<Player*> playerList;
                        me->GetPlayerListInGrid(playerList, 30.0f);
                        for (auto plr : playerList)
                        {
                            Conversation* conversation = new Conversation;
                            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), 528, plr, NULL, *plr))
                                delete conversation;
                        }
                        events.RescheduleEvent(EVENT_5, 11000);   // once Kayn has answered
                    }
                    break;
                    case EVENT_5:
                    {
                        std::list<Player*> playerList;
                        me->GetPlayerListInGrid(playerList, 30.0f);
                        for (auto plr : playerList)
                        {
                            Conversation* conversation = new Conversation;
                            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), 529, plr, NULL, *plr))
                                delete conversation;
                        }
                        events.RescheduleEvent(EVENT_6, 17000);   // once the previous exchange is over
                    }
                    break;
                    case EVENT_6:
                    {
                        std::list<Player*> playerList;
                        me->GetPlayerListInGrid(playerList, 30.0f);
                        for (auto plr : playerList)
                        {
                            Conversation* conversation = new Conversation;
                            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), 530, plr, NULL, *plr))
                                delete conversation;
                        }
                        // the last exchange is said once
                    }
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
        
        Creature* GetOtherAdd()
        {

            // the other brute: Sledge looks for Crusher and Crusher for Sledge
            if (Creature* oAdd = me->FindNearestCreature((me->GetEntry() == 92990 ? 97632 : 92990), 100.0f, true))
            {
                if (oAdd->IsAlive())
                    return oAdd;
            }
            
            return NULL;
        }

    };
};

/*
[1] byte70: False
[1] [0] Waypoint: X: 4436.342 Y: -285.1404 Z: -245.9517
[1] [1] Waypoint: X: 4436.776 Y: -284.2395 Z: -245.9517
[1] [2] Waypoint: X: 4446.447 Y: -295.8958 Z: -235.3393
[1] [3] Waypoint: X: 4456.686 Y: -311.0035 Z: -207.6675
[1] [4] Waypoint: X: 4460.651 Y: -321.5399 Z: -187.7354
[1] [5] Waypoint: X: 4454.374 Y: -328.5417 Z: -165.5679
[1] [6] Waypoint: X: 4447.924 Y: -327.4219 Z: -153.9652
[1] [7] Waypoint: X: 4448.483 Y: -319.5451 Z: -144.8872
[1] [8] Waypoint: X: 4454.757 Y: -318.3906 Z: -127.1212
[1] [9] Waypoint: X: 4455.591 Y: -325.2778 Z: -107.7209
[1] [10] Waypoint: X: 4449.187 Y: -328.9375 Z: -85.20784
[1] [11] Waypoint: X: 4444.792 Y: -325.2083 Z: -66.88081
[1] [12] Waypoint: X: 4446.588 Y: -317.9913 Z: -27.77394
[1] [13] Waypoint: X: 4451.853 Y: -316.2778 Z: -6.795706
[1] [14] Waypoint: X: 4454.854 Y: -320.9028 Z: 38.56396
[1] [15] Waypoint: X: 4454.283 Y: -324.1094 Z: 68.72004
[1] [16] Waypoint: X: 4451.352 Y: -326.1875 Z: 91.97843
[1] [17] Waypoint: X: 4451.143 Y: -327.8229 Z: 136.2271
[1] [18] Waypoint: X: 4451.238 Y: -351.0729 Z: 129.3609
[1] [19] Waypoint: X: 4451.238 Y: -351.0729 Z: 129.3609
[1] RecID: 4372
[1] InitialRawFacing: 1.121713
*/
class npc_q39682 : public CreatureScript
{
public:
    npc_q39682() : CreatureScript("npc_q39682") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q39682AI(creature);
    }

    // the felbat flies the demon hunter up the shaft along the points of its old escort path: walked by the escort
    // generator it never left the ground, and the demon hunter got no control back on landing
    struct npc_q39682AI : public ScriptedAI
    {
        npc_q39682AI(Creature* creature) : ScriptedAI(creature) { }

        enum { POINT_LANDED = 1 };

        ObjectGuid _rider;
        bool _flying = false;

        void OnCharmed(bool /*apply*/) override { }

        void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply) override
        {
            if (!apply || who->GetTypeId() != TYPEID_PLAYER || _flying)
                return;

            _flying = true;
            _rider = who->GetGUID();
            me->SetCanFly(true);
            me->SetDisableGravity(true);

            static G3D::Vector3 const path[] =
            {
                { 0.0f, 0.0f, 0.0f },   // replaced by the felbat's own position
                { 4436.8f, -284.2f, -246.0f }, { 4446.5f, -295.9f, -235.3f }, { 4456.7f, -311.0f, -207.7f }, { 4460.6f, -321.5f, -187.7f },
                { 4454.4f, -328.5f, -165.6f }, { 4447.9f, -327.4f, -154.0f }, { 4448.5f, -319.5f, -144.9f }, { 4454.8f, -318.4f, -127.1f },
                { 4455.6f, -325.3f, -107.7f }, { 4449.2f, -328.9f, -85.2f }, { 4444.8f, -325.2f, -66.9f }, { 4446.6f, -318.0f, -27.8f },
                { 4451.9f, -316.3f, -6.8f }, { 4454.9f, -320.9f, 38.6f }, { 4454.3f, -324.1f, 68.7f }, { 4451.4f, -326.2f, 92.0f },
                { 4451.1f, -327.8f, 136.2f }, { 4451.2f, -351.1f, 129.4f },
            };
            me->AddDelayedEvent(500, [this]() -> void
            {
                // the demon hunter rides, it does not steer
                if (Player* player = ObjectAccessor::GetPlayer(*me, _rider))
                    player->SetClientControl(me, false);
                me->GetMotionMaster()->MoveSmoothFlyPath(POINT_LANDED, path, std::extent<decltype(path)>::value);
            });
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type != EFFECT_MOTION_TYPE || id != POINT_LANDED)
                return;

            if (Player* player = ObjectAccessor::GetPlayer(*me, _rider))
            {
                WorldLocation loc;
                loc.m_mapId = 1468;
                loc.m_positionX = 4450.89f;
                loc.m_positionY = -431.51f;
                loc.m_positionZ = 119.26f;
                loc.SetOrientation(player->GetOrientation());
                player->SetHomebind(loc, 7866);
                player->SendBindPointUpdate();

                player->KilledMonsterCredit(96659);
                player->ExitVehicle();
                player->SetClientControl(player, true);
            }
            me->DespawnOrUnsummon(1000);
        }

        void UpdateAI(uint32 /*diff*/) override { }
    };
};

//Q: 39685
class npc_q39685 : public CreatureScript
{
public:
    npc_q39685() : CreatureScript("npc_q39685") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q39685AI(creature);
    }

    enum data
    {
        SPELL_ID = 0,
        SPELL_AURA = 195189,
    };
    struct npc_q39685AI : public ScriptedAI
    {
        npc_q39685AI(Creature* creature) : ScriptedAI(creature), cooldown(0)
        {
        }

        uint32 cooldown;

        void Reset() override
        {
            cooldown = 0;
            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            me->SetUInt32Value(UNIT_FIELD_MOUNT_DISPLAY_ID, 51861);
            me->RemoveAurasDueToSpell(SPELL_AURA);
            me->CastSpell(me, SPELL_AURA, true);
            //me->AddAura(SPELL_AURA, me);
            
        }

        void OnSpellClick(Unit* Clicker) override
        {
            if (cooldown)
                return;

            Clicker->ToPlayer()->KilledMonsterCredit(me->GetEntry());

            // as in the dungeon: the countermeasure wakes its Freeze Statue (99710, above its Frost Sentry) with a frost beam,
            // and the statue breathes frost over the room (the breath only harms the dungeon's Ash'golm, it is a sight here)
            if (Creature* statue = me->FindNearestCreature(99710, 80.0f))
            {
                me->CastSpell(statue, 195213, true);
                ObjectGuid const countermeasure = me->GetGUID();
                statue->AddDelayedEvent(300, [statue, countermeasure]() -> void
                {
                    // the beam holds once the countermeasure is on
                    if (Aura* beam = statue->GetAura(195213, countermeasure))
                    {
                        beam->SetMaxDuration(-1);
                        beam->SetDuration(-1);
                    }
                });
                statue->AddDelayedEvent(1500, [statue]() -> void
                {
                    // the breath rises towards the vault (its model attachment is tilted by a hotfix)
                    statue->CastSpell(statue, 195227, true);
                });
            }

            // the last countermeasure freezes Ash'golm: the same frost breaths rise all around it, for the demon hunter alone
            if (Clicker->ToPlayer()->GetQuestStatus(39685) == QUEST_STATUS_COMPLETE)
            {
                ObjectGuid const playerGuid = Clicker->GetGUID();
                me->AddDelayedEvent(1500, [this, playerGuid]() -> void
                {
                    Player* player = ObjectAccessor::GetPlayer(*me, playerGuid);
                    if (!player)
                        return;
                    for (uint8 i = 0; i < 8; ++i)
                    {
                        float const angle = i * float(M_PI) / 4.0f;
                        float const x = 4238.65f + 6.0f * std::cos(angle);
                        float const y = -452.602f + 6.0f * std::sin(angle);
                        if (TempSummon* breath = player->SummonCreature(22515, x, y, 105.947f, angle, TEMPSUMMON_TIMED_DESPAWN, 8000, playerGuid))
                            breath->AddDelayedEvent(500, [breath]() -> void { breath->CastSpell(breath, 195227, true); });
                    }
                });
            }

            cooldown = 5000;
            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        }

        void EnterCombat(Unit* /*who*/) override
        {
            return;
        }

        void UpdateAI(uint32 diff) override
        {
            if (cooldown)
            {
                if (cooldown <= diff)
                {
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    cooldown = 0;
                }
                else
                    cooldown -= diff;
            }
        }
    };
};

//! 96682
class npc_q39683 : public CreatureScript
{
public:
    npc_q39683() : CreatureScript("npc_q39683") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q39683AI(creature);
    }

    //! Scripted_NoMovementAI HACK!
    struct npc_q39683AI : public ScriptedAI
    {
        EventMap events;
        ObjectGuid playerGuid;
        int8 lowStep;
        bool phase2;
        
        npc_q39683AI(Creature* creature) : ScriptedAI(creature), phase2(false)
        {
            lowStep = 1;
            creature->SetCorpseDelay(30);
            creature->SetRespawnDelay(15);
            me->SetHealth(me->CountPctFromMaxHealth(21));
            me->setRegeneratingHealth(false);
        }

        enum data
        {
            REWARD_SPELL = 210486,
            REWARD_SPELL2 = 195440,
            CREDIT = 106254,                   
        };

        bool _playerEngaged = false;
        uint32 _foeTimer = 1000;

        // it fights the freed Illidari of its room until the demon hunter comes: neither side wounds the other
        void HoldFoes()
        {
            for (uint32 entry : { 96672u, 96656u })
            {
                std::list<Creature*> foes;
                me->GetCreatureListWithEntryInGrid(foes, entry, 20.0f);
                for (Creature* foe : foes)
                {
                    if (!foe->IsAlive() || !foe->AI() || !foe->IsValidAttackTarget(me))
                        continue;
                    if (foe->getThreatManager().getThreat(me) < 1.0f)
                        foe->AddThreat(me, 1.0f);
                    if (me->getThreatManager().getThreat(foe) < 1.0f)
                        me->AddThreat(foe, 1.0f);
                    if (!foe->getVictim())
                        foe->AI()->AttackStart(me);
                    if (!me->getVictim())
                        AttackStart(foe);
                }
            }
        }

        void DamageDealt(Unit* victim, uint32& damage, DamageEffectType /*damageType*/) override
        {
            if (!victim->IsControlledByPlayer())
                damage = 0;
        }

        void EnterCombat(Unit* victim) override
        {
            if (Player* player = victim->ToPlayer())
                EngagePlayer(player);
        }

        void EngagePlayer(Player* player)
        {
            if (_playerEngaged)
                return;
            _playerEngaged = true;

            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, player->GetGUID());

            lowStep = 1;

            float health_pct = 100.0f;
            if (player->GetQuestStatus(39684) == QUEST_STATUS_COMPLETE || player->GetQuestStatus(39684) ==  QUEST_STATUS_REWARDED)
            {
                ++lowStep;
                health_pct -= 40.0f;
            }
            if (player->GetQuestStatus(39685) == QUEST_STATUS_COMPLETE || player->GetQuestStatus(39685) == QUEST_STATUS_COMPLETE)
            {
                ++lowStep;
                health_pct -= 40.0f;
            }

         //   me->SetHealth(float(me->GetMaxHealth() / 100.0f)*health_pct);
            me->SetHealth(me->CountPctFromMaxHealth(21));
            events.RescheduleEvent(EVENT_1, 20000); // 199758
            events.RescheduleEvent(EVENT_2, 15000); // 199836
        }
        
        void Reset() override
        {
            phase2 = false;
            _playerEngaged = false;
            events.Reset();
            me->SetHealth(me->CountPctFromMaxHealth(21));
            me->setRegeneratingHealth(false);
        }

        //! HACK!!! ANTIL FINISH EVENT
        void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType dmgType) override
        {
            if (!attacker || !attacker->IsControlledByPlayer())
            {
                damage = 0;
                return;
            }
            if (Player* player = attacker->GetCharmerOrOwnerPlayerOrPlayerItself())
                EngagePlayer(player);

            damage *= lowStep;
            if (me->HealthBelowPct(5) && !phase2)
            {
                phase2 = true;
                DoCast(199828);
                Talk(2);
            }
        }

        void JustDied(Unit* killer) override
        {
            Player *player = killer->ToPlayer();
            if (!player)
                return;

            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_3, player->GetGUID());
            player->CastSpell(player, REWARD_SPELL, true);
            player->CastSpell(player, REWARD_SPELL2, true);
            player->KilledMonsterCredit(CREDIT);
        }
        void UpdateAI(uint32 diff) override
        {
            if (_foeTimer <= diff)
            {
                _foeTimer = 1000;
                HoldFoes();
            }
            else
                _foeTimer -= diff;

            UpdateVictim();

            events.Update(diff);
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(199758);
                        events.RescheduleEvent(EVENT_1, 40000); // 199758
                        break;
                    case EVENT_2:
                        DoCast(199836);
                        events.RescheduleEvent(EVENT_2, 31000); // 199836
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

//! 96675
class npc_dh_questgiver_96675 : public CreatureScript
{
public:
    npc_dh_questgiver_96675() : CreatureScript("npc_dh_questgiver_96675") {}
    enum data
    {
        QUEST_03 = 39686,
        GO_ELEVATOR = 244644,
        CREDIT = 96814,
    };
    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_03) // The Lesson of the Burning Scroll
        {
            WorldLocation loc;
            loc.m_mapId = 1468;
            loc.m_positionX = 4317.87f;
            loc.m_positionY = -451.61f;
            loc.m_positionZ = 259.36f;
            loc.SetOrientation(player->GetOrientation());
            player->SetHomebind(loc, 7866);
            player->SendBindPointUpdate();

            creature->GetMap()->LoadGrid(4450.857f, -451.7679f); //voice

            std::list<GameObject*> lList;
            creature->GetGameObjectListWithEntryInGrid(lList, GO_ELEVATOR, 500.0f);
            if (lList.empty())
            {
                creature->MonsterSay("SCRIPT::npc_dh_questgiver_96675 no elevator", LANG_UNIVERSAL, player->GetGUID());
                return true;
            }
            //creature->MonsterSay("SCRIPT::npc_dh_questgiver_96675 OK", LANG_UNIVERSAL, player->GetGUID());

            GameObject* elevator = *lList.begin();
            player->AddToExtraLook(elevator->GetGUID());

            UpdateData transData(player->GetMapId());
            elevator->BuildCreateUpdateBlockForPlayer(&transData, player);
            player->KilledMonsterCredit(CREDIT);
            sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_0, player->GetGUID());
        }

        return true;
    }
};

//! 244455
class go_q39687 : public GameObjectScript
{
public:
    go_q39687() : GameObjectScript("go_q39687") { }

    struct go_q39687AI : public GameObjectAI
    {
        go_q39687AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 39687,
            CREDIT = 100166,
        };

        // a goober is used through GameObject::Use, which calls GossipUse, never GossipHello with isUse:
        // touching the Pool plays the memories of the past (movie 478) and completes the quest
        bool GossipUse(Player* player) override
        {
            if (player->GetQuestStatus(QUEST) != QUEST_STATUS_INCOMPLETE)
                return true;

            player->CastSpell(player, 226867, true);
            player->KilledMonsterCredit(CREDIT);
            return true;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_q39687AI(go);
    }
};

//! 96675
class npc_dh_questgiver_97644 : public CreatureScript
{
public:
    npc_dh_questgiver_97644() : CreatureScript("npc_dh_questgiver_97644") {}
    
    enum data
    {
        QUEST_02 = 40373,
        CAST_SPELL = 196650,
    };
    
    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_02) // The Lesson of the Burning Scroll
        {
            player->CastSpell(player, CAST_SPELL, true);
        }

        // The Hunt for Freedom (Alliance 39688, Horde 39694): Kor'vas and the chosen leader run on to Bastillax's chamber,
        // where their own copies wait; the ones of the Court are lost from sight for this demon hunter (phases 4913 to 4915)
        if (quest->GetQuestId() == 39688 || quest->GetQuestId() == 39694)
        {
            ObjectGuid const playerGuid = player->GetGUID();
            bool const kayn = VaultChoseKayn(player);
            struct Runner { uint32 entry; Position from; VaultRoad const* road; };
            Runner const runners[2] =
            {
                { 99968, VaultCourtKorvas, &VaultRoadKorvasToChamber },
                { kayn ? 97273u : 97274u, kayn ? VaultCourtKayn : VaultCourtAltruis, kayn ? &VaultRoadKaynToChamber : &VaultRoadAltruisToChamber },
            };
            for (Runner const& runner : runners)
            {
                TempSummon* summon = player->SummonCreature(runner.entry, runner.from, TEMPSUMMON_TIMED_DESPAWN, 120000, 0, playerGuid);
                if (!summon)
                    continue;
                if (runner.entry == 99968)
                {
                    // "Let's keep moving. The others are in the chamber just ahead."
                    summon->Talk(99645, CHAT_MSG_MONSTER_SAY, 40.0f, player);
                    summon->PlayDirectSound(57361, player);
                }
                VaultRoad const* road = runner.road;
                summon->AddDelayedEvent(runner.entry == 99968 ? 3000 : 1500, [summon, road]() -> void
                {
                    VaultFollowRoad(summon, *road, false, Seconds(0));
                });
            }
        }

        return true;
    }

    // "Will it be Kayn or Altruis?": the choice comes back for the demon hunter who closed it without choosing
    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 /*action*/) override
    {
        player->CLOSE_GOSSIP_MENU();
        if (player->GetQuestStatus(QUEST_02) == QUEST_STATUS_INCOMPLETE)
            player->CastSpell(player, CAST_SPELL, true);
        return true;
    }
    
    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_dh_questgiver_97644AI(creature);
    }

    //! Scripted_NoMovementAI HACK!
    struct npc_dh_questgiver_97644AI : public Scripted_NoMovementAI
    {
        npc_dh_questgiver_97644AI(Creature* creature) : Scripted_NoMovementAI(creature) {}

        GuidSet m_player_for_event;

        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || who->IsOnVehicle())
                return;

            GuidSet::iterator itr = m_player_for_event.find(who->GetGUID());
            if (itr != m_player_for_event.end())
                return;

            if (!me->IsWithinDistInMap(who, 10.0f))
                return;
            
            Conversation* conversation = new Conversation;
            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), 536, who, NULL, *who))
                delete conversation;
            m_player_for_event.insert(who->GetGUID());
        }

    };
};

//! 96783
class npc_q39694 : public CreatureScript
{
public:
    npc_q39694() : CreatureScript("npc_q39694") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q39694AI(creature);
    }

    struct npc_q39694AI : public ScriptedAI
    {
        SummonList summons;
        EventMap events;
        ObjectGuid playerGuid;
        npc_q39694AI(Creature* creature) : ScriptedAI(creature), summons(me)
        {
            creature->SetCorpseDelay(30);
            creature->SetRespawnDelay(15);
        }

        enum data
        {
            REWARD_SPELL = 210500,
            REWARD_SPELL2 = 195450,
            CREDIT = 106255,
            CREDIT2 = 113812,
            
            SPELL_1 = 200027,  // 45
            SPELL_2 = 200007,  // 5 + 35
            SPELL_3 = 200353,  // 16 + 50  say + summon
            SPELL_4 = 200002,  // 23  say
            
        };
        
        void Reset() override
        {
            summons.DespawnAll();
            events.Reset();
        }

        void EnterCombat(Unit* victim) override
        {
            Talk(0);
            events.RescheduleEvent(EVENT_1, 45000);
            events.RescheduleEvent(EVENT_2, 5000);
            events.RescheduleEvent(EVENT_3, 15000);
            events.RescheduleEvent(EVENT_4, 23000);
        }
        
        void JustSummoned(Creature* summon) override
        { 
            summons.Summon(summon);
        }

        // the Illidari of the chamber gather outside, where the quest is handed in: each runs to the place of its own copy there
        void GatherOutside()
        {
            struct Gathering { uint32 entry; VaultRoad const* road; };
            static Gathering const gatherings[] =
            {
                { 97273, &VaultRoadLeaderOut },     // Kayn Sunfury
                { 97274, &VaultRoadLeaderOut },     // Altruis the Sufferer
                { 99962, &VaultRoadLyanaOut },      // Lyana Darksorrow
                { 99963, &VaultRoadCyanaOut },      // Cyana Nightglaive
                { 99964, &VaultRoadJaceOut },       // Jace Darkweaver
                { 99965, &VaultRoadMariusOut },     // Marius Felbane
                { 99966, &VaultRoadAshaOut },       // Asha Ravensong
                { 99967, &VaultRoadFalaraOut },     // Falara Nightsong
                { 99968, &VaultRoadKorvasOut },     // Kor'vas Bloodthorn
                { 99969, &VaultRoadAllariOut },     // Allari the Souleater
                { 100065, &VaultRoadCassielOut },   // Cassiel Nightthorn
            };
            uint32 delay = 1500;
            for (Gathering const& gathering : gatherings)
            {
                std::list<Creature*> illidari;
                me->GetCreatureListWithEntryInGrid(illidari, gathering.entry, 80.0f);
                for (Creature* illidan : illidari)
                {
                    if (!illidan->IsAlive() || illidan->isSummon())
                        continue;
                    // one after the other, so that they do not run into each other
                    VaultRoad const* road = gathering.road;
                    illidan->AddDelayedEvent(delay, [illidan, road]() -> void
                    {
                        illidan->CombatStop(true);
                        VaultFollowRoad(illidan, *road, false, Seconds(120));
                    });
                    delay += 600;
                }
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            Talk(2);

            // his demons fall with him
            for (ObjectGuid const& guid : summons)
                if (Creature* add = ObjectAccessor::GetCreature(*me, guid))
                    if (add->IsAlive())
                        add->Kill(add, false);
            for (uint32 entry : { 92776u, 92782u, 97225u, 97228u, 101505u, 101506u, 101507u })
            {
                std::list<Creature*> demons;
                me->GetCreatureListWithEntryInGrid(demons, entry, 80.0f);
                for (Creature* demon : demons)
                    if (demon->IsAlive())
                    {
                        // none comes back while the Illidari leave the chamber
                        demon->SetRespawnDelay(5 * MINUTE);
                        demon->Kill(demon, false);
                    }
            }

            GatherOutside();

            // the demon hunters around share the victory, whoever struck the last blow (the Illidari fight too)

            std::list<Player*> playerList;
            me->GetPlayerListInGrid(playerList, 60.0f);
            for (auto plr : playerList)
            {
                plr->CastSpell(plr, REWARD_SPELL, true);
                plr->CastSpell(plr, REWARD_SPELL2, true);
                plr->KilledMonsterCredit(CREDIT);
                plr->KilledMonsterCredit(CREDIT2);
            }
        }
        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;    

            events.Update(diff);
            
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_1);
                        events.RescheduleEvent(EVENT_1, 45000);
                        break;
                    case EVENT_2:
                        DoCast(SPELL_2);
                        events.RescheduleEvent(EVENT_2, 35000);
                        break;
                    case EVENT_3:
                    {
                        DoCast(SPELL_3);
                        Talk(1);
                        for (uint8 i = 0; i < 4; i++)
                        {
                            if (Creature* add = me->SummonCreature(101505, 4221.51f + irand(-3, 3), -627.92f + irand(-3, 3), 255.12f, 3.10f))
                            {
                                add->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                                DoZoneInCombat(add, 50.0f);
                            }
                        }
                        for (uint8 i = 0; i < 4; i++)
                        {
                            if (Creature* add = me->SummonCreature(101505, 4146.97f + irand(-3, 3), -626.29f + irand(-3, 3), 255.12f, 6.27f))
                            {
                                add->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                                DoZoneInCombat(add, 50.0f);
                            }
                        }
                        events.RescheduleEvent(EVENT_3, 50000);
                    }
                        break;
                    case EVENT_4:
                        DoCast(SPELL_4);
                        events.RescheduleEvent(EVENT_4, 23000);
                        break;
                }

            }
            DoMeleeAttackIfReady();
        }
    };
};

class npc_92718 : public CreatureScript
{
public:
    npc_92718() : CreatureScript("npc_92718") { }
    
    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        // Our Last Hope: the new warglaives wait on the rack beside her
        if (quest->GetQuestId() == 38669)
            sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_0, player->GetGUID());

        // Stop Gul'dan!: the scene at the door, after which the demon hunter stands inside the Vault of the Betrayer (spell_q38723)
        if (quest->GetQuestId() == 38723)
            player->CastSpell(player, 187864, true);

        if (quest->GetQuestId() == 38672)
        {
            if (Creature* tar = player->SummonCreature(creature->GetEntry(), creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ(), creature->GetOrientation()))
            {
                tar->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                sCreatureTextMgr->SendChat(tar, TEXT_GENERIC_1, player->GetGUID());
                // her line said, the ward of the cell room opens for her (for this player only) and she runs out
                ObjectGuid const viewer = player->GetGUID();
                tar->AddDelayedEvent(3500, [tar, viewer]() -> void
                {
                    VaultWardOpenFor(tar, viewer, 7000);
                    tar->SetWalk(false);
                    tar->GetMotionMaster()->MovePoint(0, 4325.57f, -562.81f, -281.74f);
                });
                tar->DespawnOrUnsummon(10000);
            }
        }
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_92718AI(creature);
    }

    struct npc_92718AI : public ScriptedAI
    {
        npc_92718AI(Creature* creature) : ScriptedAI(creature) {}

        GuidSet m_player_for_event;

        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetTypeId() != TYPEID_PLAYER || who->IsOnVehicle())
                return;

            // the cell room: in case the cinematic did not strip the warglaives (a character already in the Vault).
            // The Maiev of Illidan's chamber plays the arrival conversation
            if (me->GetDistance2d(4235.6f, -293.0f) > 30.0f)
            {
                if (me->IsWithinDistInMap(who, 40.0f))
                    VaultStripWarglaives(who->ToPlayer());
                return;
            }

            // once per character, kept by a hidden tracking quest of the Vault: the map reloads after a logout or a restart
            enum { QUEST_MAIEV_ARRIVAL_SEEN = 33071 };
            Player* player = who->ToPlayer();
            if (player->GetQuestRewardStatus(QUEST_MAIEV_ARRIVAL_SEEN) || !m_player_for_event.insert(player->GetGUID()).second)
                return;

            if (!me->IsWithinDistInMap(who, 10.0f))
            {
                m_player_for_event.erase(player->GetGUID());
                return;
            }

            player->CastSpell(player, 192605, true);
            player->CompleteQuest(QUEST_MAIEV_ARRIVAL_SEEN);

            // Altruis answers once Kayn has spoken
            if (Creature* altruis = me->FindNearestCreature(92986, 15.0f))
            {
                ObjectGuid const playerGuid = player->GetGUID();
                altruis->AddDelayedEvent(12000, [altruis, playerGuid]() -> void { sCreatureTextMgr->SendChat(altruis, TEXT_GENERIC_0, playerGuid); });
            }
        }

    };
};

class spell_199760 : public SpellScriptLoader
{
    public:
        spell_199760() : SpellScriptLoader("spell_199760") { }

        class spell_199760_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_199760_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                PreventHitDefaultEffect(EFFECT_0);

                if (!GetCaster())
                    return;

                Position pos = GetCaster()->GetRandomNearPosition(20.0f);
                WorldLocation* dest = const_cast<WorldLocation*>(GetExplTargetDest());
                dest->Relocate(pos);
            }

            void Register() override
            {
                OnEffectLaunch += SpellEffectFn(spell_199760_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_199760_SpellScript();
        }
};

class npc_102391 : public CreatureScript
{
public:
    npc_102391() : CreatureScript("npc_102391") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_102391AI(creature);
    }

    struct npc_102391AI : public npc_escortAI
    {
        npc_102391AI(Creature* creature) : npc_escortAI(creature) {}

        GuidSet m_player_for_event;
        ObjectGuid leftAdd;
        ObjectGuid rightAdd;
        ObjectGuid bossGUID = ObjectGuid::Create<HighGuid::Creature>(me->GetMapId(), 96680, 365929);

        void IsSummonedBy(Unit* summoner) override
        {
            me->RemoveAura(202212);
            if (summoner->GetTypeId() != TYPEID_PLAYER)
                return;

            Start(false, true, summoner->GetGUID());
            if (TempSummon* add = me->SummonCreature(96656, 4445.42f, -679.495f, 117.316f, 5.73285f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 28500))
            {
                add->AddPlayerInPersonnalVisibilityList(summoner->GetGUID());
                leftAdd = add->GetGUID();
            }
            if (TempSummon* add = me->SummonCreature(96656, 4453.46f, -680.844f, 117.284f, 4.0283f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 28500))
            {
                add->AddPlayerInPersonnalVisibilityList(summoner->GetGUID());
                rightAdd = add->GetGUID();
            }
        }

        void WaypointReached(uint32 waypointId) override
        {
            switch (waypointId)
            {
            case 3:
                Talk(TEXT_GENERIC_1);
                break;
            }
        }

        void WaypointStart(uint32 waypointId) override
        {
            switch (waypointId)
            {
            case 1:
                if (Creature* boss = me->GetCreature(*me, bossGUID))
                    boss->CastSpell(boss, 202220, true);
                break;
            case 4:
                if (Creature* left = me->GetCreature(*me, leftAdd))
                    left->GetMotionMaster()->MoveFollow(me, 0.5f, 90.0f);
                if (Creature* right = me->GetCreature(*me, rightAdd))
                    right->GetMotionMaster()->MoveFollow(me, 0.5f, 270.0f);
                break;
            }
        }

        uint32 _lookTimer = 1000;
        uint32 _strikeTimer = 1500;

        // held in Glazer's beam, Altruis is never sighted: he looks for the demon hunters himself
        void GreetDemonHunters()
        {
            std::list<Player*> players;
            me->GetPlayerListInGrid(players, 50.0f);
            for (Player* player : players)
            {
                if (player->IsOnVehicle() || player->GetQuestStatus(39684) != QUEST_STATUS_INCOMPLETE)
                    continue;
                if (!m_player_for_event.insert(player->GetGUID()).second)
                    continue;
                Talk(TEXT_GENERIC_0, player->GetGUID());
            }
        }

        // the two freed Illidari strike at Glazer without fighting it: Glazer must keep its gaze out of combat
        void FreedIllidariStrike()
        {
            Creature* glazer = me->GetMap()->GetCreature(ObjectGuid::Create<HighGuid::Creature>(me->GetMapId(), 96680, 365928));
            if (!glazer)
                return;
            std::list<Creature*> illidari;
            glazer->GetCreatureListWithEntryInGrid(illidari, 96656, 12.0f);
            for (Creature* illidari_ : illidari)
            {
                if (illidari_->isSummon() || !illidari_->IsAlive() || illidari_->isInCombat())
                    continue;
                illidari_->SetSheath(SHEATH_STATE_MELEE);
                illidari_->SetFacingToObject(glazer);
                illidari_->HandleEmoteCommand(urand(0, 1) ? EMOTE_ONESHOT_ATTACK1H : EMOTE_ONESHOT_ATTACK2HTIGHT);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            npc_escortAI::UpdateAI(diff);

            // only the static spawn should do this, the summons are for the escape sequence
            if (me->isSummon())
                return;

            if (_lookTimer <= diff)
            {
                _lookTimer = 1000;
                GreetDemonHunters();
            }
            else
                _lookTimer -= diff;

            if (_strikeTimer <= diff)
            {
                _strikeTimer = urand(1200, 1800);
                FreedIllidariStrike();
            }
            else
                _strikeTimer -= diff;
        }

    };
};

class spell_196460 : public SpellScriptLoader
{
public:
    spell_196460() : SpellScriptLoader("spell_196460") { }

    class spell_196460_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_196460_SpellScript);

        // the demon hunter's Vault of Mirrors: the gaze falls over the whole mirror floor (a square turned by 45 degrees
        // around 4451.3, -675.3), never at a Reflective Mirror, which must stay usable
        static bool OnMirrorFloor(float x, float y)
        {
            float const dx = x - 4451.34f;
            float const dy = y + 675.25f;
            return std::fabs(dx + dy) * float(M_SQRT1_2) < 26.0f && std::fabs(dx - dy) * float(M_SQRT1_2) < 26.0f;
        }

        static bool AwayFromMirrors(float x, float y)
        {
            static float const mirrors[4][2] = { { 4442.66f, -699.927f }, { 4468.48f, -691.247f }, { 4424.84f, -665.623f }, { 4468.37f, -656.859f } };
            for (auto const& mirror : mirrors)
                if (std::hypot(x - mirror[0], y - mirror[1]) < 8.0f)
                    return false;
            return true;
        }

        void SpreadOverTheVault()
        {
            float x = 0.0f, y = 0.0f;
            bool found = false;
            for (uint8 tries = 0; tries < 20 && !found; ++tries)
            {
                float const u = frand(-24.0f, 24.0f);
                float const v = frand(-24.0f, 24.0f);
                x = 4451.34f + (u + v) * float(M_SQRT1_2);
                y = -675.25f + (u - v) * float(M_SQRT1_2);
                found = AwayFromMirrors(x, y);
            }
            if (!found)
                return;

            float const angle = frand(0.0f, 2.0f * float(M_PI));
            uint32 const missiles = urand(1, 3);
            for (uint32 i = 0; i < missiles; ++i)
            {
                if (!OnMirrorFloor(x, y) || !AwayFromMirrors(x, y))
                    break;
                GetCaster()->CastSpellDelay(Position(x, y, 117.2f, 0.0f), 196504, true, i * 1000);
                x += 9.0f * std::cos(angle);
                y += 9.0f * std::sin(angle);
            }
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster())
                return;

            if (GetCaster()->GetMapId() == 1468)
            {
                SpreadOverTheVault();
                return;
            }

            uint32 missiles = urand(1, 3);
            float directionX = frand(0.0f, 6.0f);
            float directionY = 6.0f - directionX;
            if (urand(0, 1))
                directionX *= -1.0f;
            if (urand(0, 1))
                directionY *= -1.0f;

            Position pos = GetCaster()->GetRandomNearPosition(25.0f);
            for (uint32 i = 0; i < missiles; i++)
            {
                GetCaster()->CastSpellDelay(pos, 196504, true, i * 1000);
                pos.m_positionX += directionX;
                pos.m_positionY += directionY;
            }
        }

        void Register() override
        {
            OnEffectLaunch += SpellEffectFn(spell_196460_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_196460_SpellScript();
    }
};

class spell_196462 : public SpellScriptLoader
{
public:
    spell_196462() : SpellScriptLoader("spell_196462") { }

    class spell_196462_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_196462_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster())
                return;

            Position pos = GetCaster()->GetPosition();
            pos.SetOrientation(frand(0.0f, 2.0f) * static_cast<float>(M_PI));
            GetCaster()->CastSpell(pos, 194853, true);
        }

        void Register() override
        {
            OnEffectLaunch += SpellEffectFn(spell_196462_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_196462_SpellScript();
    }
};

// 103658 103655
// Breaking Out (38672): the cells of Kayn and Altruis. A broken cell frees its prisoner, who picks his warglaives up
// on his rack and goes to stand before the ward; when both are there they talk it over, which ends the quest
class npc_q38672 : public CreatureScript
{
public:
    npc_q38672() : CreatureScript("npc_q38672") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_q38672AI(creature);
    }

    //! Scripted_NoMovementAI HACK!
    struct npc_q38672AI : public Scripted_NoMovementAI
    {
        npc_q38672AI(Creature* creature) : Scripted_NoMovementAI(creature) {}

        void OnSpellClick(Unit* clicker) override
        {
            Player* player = clicker->ToPlayer();
            if (!player || player->GetQuestStatus(38672) != QUEST_STATUS_INCOMPLETE)
                return;

            bool const kayn = me->GetEntry() == 103658;     // 103655 is Altruis's cell
            uint32 const freed = kayn ? 92848 : 92849;
            if (player->GetQuestObjectiveData(38672, freed))
                return;

            player->KilledMonsterCredit(freed);

            Position const out = kayn ? Position(4341.70f, -589.69f, -281.40f, 3.26f) : Position(4309.16f, -589.73f, -281.40f, 6.09f);
            if (Creature* prisoner = player->SummonCreature(kayn ? 99631 : 99632, out, TEMPSUMMON_MANUAL_DESPAWN, 0, 0, player->GetGUID()))
                prisoner->AddPlayerInPersonnalVisibilityList(player->GetGUID());

            // the cell breaks for this player: its copy plays the destruction and stays broken (Destroy then Destroyed, kit 976),
            // the cell and its collision are gone for him
            if (Creature* cell = player->SummonCreature(me->GetEntry(), me->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0, 0, player->GetGUID()))
            {
                cell->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                cell->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                cell->AddDelayedEvent(100, [cell]() -> void { cell->PlayOneShotAnimKit(976); });
                cell->DespawnOrUnsummon(3500);
            }
            VaultHideFor(me, player);
            if (GameObject* wall = me->FindNearestGameObject(247411, 10.0f))
                VaultHideFor(wall, player);
        }
    };
};

// 99631 Kayn, 99632 Altruis, just out of their cells (one copy per player)
struct npc_vault_freed_illidari : public ScriptedAI
{
    npc_vault_freed_illidari(Creature* creature) : ScriptedAI(creature) { }

    enum data
    {
        NPC_KAYN                = 99631,
        NPC_ALTRUIS             = 99632,
        GO_KAYN_WARGLAIVES      = 244943,
        GO_ALTRUIS_WARGLAIVES   = 244944,
        SAY_FREED               = 0,    // both
        SAY_ALTRUIS_THANKS      = 1,
        SAY_KAYN_WHY_MAIEV      = 1,
        SAY_ALTRUIS_FOOL        = 2,
        SAY_KAYN_RELEASE_ALLIES = 2,
        SAY_ALTRUIS_ANSWERS     = 3,
        TO_RACK                 = 1,    // the two legs of their way out
        TO_WARD                 = 2,
    };

    // their way out of the cell (paths 10267122 and 10267123 without their first point, under their feet: walking it
    // turned them about face first), to the rack, then before the ward
    void Walk(uint32 leg)
    {
        static G3D::Vector3 const altruis[2][4] =
        {
            { { 4311.4f, -590.3f, -281.4f }, { 4314.1f, -590.2f, -281.5f }, { 4316.1f, -590.4f, -281.9f }, { 4318.7f, -588.3f, -281.9f } },
            { { 4319.9f, -586.2f, -281.9f }, { 4321.1f, -583.5f, -281.9f }, { 4322.6f, -583.4f, -281.9f }, { 4322.6f, -583.4f, -281.9f } },
        };
        static G3D::Vector3 const kayn[2][4] =
        {
            { { 4338.7f, -590.5f, -281.5f }, { 4335.6f, -590.8f, -281.9f }, { 4333.4f, -590.1f, -281.9f }, { 4333.6f, -588.4f, -281.9f } },
            { { 4332.3f, -586.1f, -281.9f }, { 4331.4f, -584.6f, -281.9f }, { 4330.5f, -583.2f, -281.9f }, { 4328.4f, -583.5f, -281.9f } },
        };
        G3D::Vector3 const* points = (IsKayn() ? kayn : altruis)[leg - 1];
        size_t const count = (leg == TO_WARD && !IsKayn()) ? 3 : 4;
        std::vector<G3D::Vector3> path(1, G3D::Vector3(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()));
        path.insert(path.end(), points, points + count);
        me->SetWalk(false);
        me->GetMotionMaster()->MoveSmoothPath(leg, path.data(), path.size(), false);
    }

    bool IsKayn() const { return me->GetEntry() == NPC_KAYN; }

    void IsSummonedBy(Unit* summoner) override
    {
        Player* player = summoner->ToPlayer();
        if (!player)
        {
            me->DespawnOrUnsummon();
            return;
        }

        _player = player->GetGUID();
        me->SetReactState(REACT_PASSIVE);
        me->LoadEquipment(0, true);     // their warglaives wait on the racks
        me->AddDelayedEvent(1500, [this]() -> void { Talk(SAY_FREED, _player); });
        me->AddDelayedEvent(3500, [this]() -> void { Walk(TO_RACK); });
        me->DespawnOrUnsummon(10 * MINUTE * IN_MILLISECONDS);
    }

    void SetGUID(ObjectGuid const& guid, int32 id) override
    {
        if (id == VAULT_RUN_TO_HALL)
            _waiting = guid;
    }

    void MovementInform(uint32 type, uint32 leg) override
    {
        // in the next hall: the one waiting there shows up again in his place, this one is gone
        if (type == POINT_MOTION_TYPE && leg == VAULT_RUN_TO_HALL)
        {
            if (Player* player = ObjectAccessor::GetPlayer(*me, _player))
            {
                if (Creature* waiting = ObjectAccessor::GetCreature(*me, _waiting))
                    waiting->RemoveFromHideList(player->GetGUID());
                player->UpdateObjectVisibility();
            }
            me->DespawnOrUnsummon();
            return;
        }

        if (type != EFFECT_MOTION_TYPE)
            return;

        Player* player = ObjectAccessor::GetPlayer(*me, _player);
        if (leg == TO_RACK)
        {
            if (GameObject* rack = me->FindNearestGameObject(244582, 5.0f))
                me->SetFacingToObject(rack);
            me->LoadEquipment(1, true);
            me->HandleEmoteCommand(EMOTE_ONESHOT_LOOT);
            if (GameObject* glaives = me->FindNearestGameObject(IsKayn() ? GO_KAYN_WARGLAIVES : GO_ALTRUIS_WARGLAIVES, 5.0f))
                if (player)
                    VaultHideFor(glaives, player);
            if (player)
                player->KilledMonsterCredit(IsKayn() ? 112276 : 112277);
            if (!IsKayn())
                me->AddDelayedEvent(1000, [this]() -> void { Talk(SAY_ALTRUIS_THANKS, _player); });
            me->AddDelayedEvent(2000, [this]() -> void { Walk(TO_WARD); });
            return;
        }

        if (leg != TO_WARD)
            return;

        // before the ward of the cell room
        me->SetFacingTo(me->GetAngle(4326.0f, -577.5f));
        _arrived = true;
        if (Creature* other = Partner())
            if (npc_vault_freed_illidari* otherAI = dynamic_cast<npc_vault_freed_illidari*>(other->AI()))
                if (otherAI->_arrived)
                    (IsKayn() ? this : otherAI)->Reunion(IsKayn() ? other : me);
    }

    Creature* Partner() const
    {
        std::list<Creature*> list;
        me->GetCreatureListWithEntryInGrid(list, IsKayn() ? NPC_ALTRUIS : NPC_KAYN, 40.0f);
        for (Creature* creature : list)
            if (TempSummon* summon = creature->ToTempSummon())
                if (summon->GetSummonerGUID() == _player)
                    return creature;
        return nullptr;
    }

    // run by Kayn: why would Maiev free them, Altruis does not trust her; then the quest is done
    void Reunion(Creature* altruis)
    {
        if (_talked)
            return;
        _talked = true;
        ObjectGuid const altruisGuid = altruis->GetGUID();
        me->AddDelayedEvent(1000, [this]() -> void { Talk(SAY_KAYN_WHY_MAIEV, _player); });
        me->AddDelayedEvent(5500, [this, altruisGuid]() -> void
        {
            if (Creature* altruis = ObjectAccessor::GetCreature(*me, altruisGuid))
                sCreatureTextMgr->SendChat(altruis, SAY_ALTRUIS_FOOL, _player);
        });
        me->AddDelayedEvent(10500, [this]() -> void
        {
            if (Player* player = ObjectAccessor::GetPlayer(*me, _player))
            {
                player->KilledMonsterCredit(99326);
                player->KilledMonsterCredit(112287);
            }
        });
    }

    ObjectGuid _player;
    ObjectGuid _waiting;
    bool _arrived = false;
    bool _talked = false;
};

// the ward of the cell room, seen by one player only, closed and swinging open a moment later
static GameObject* VaultSwingWard(Player* player)
{
    GameObject* ward = player->SummonGameObject(244925, 4325.98f, -578.06f, -281.81f, 4.69f, 0.0f, 0.0f, 0.0f, 0.0f, 0, player->GetGUID());
    if (!ward)
        return nullptr;

    ward->SetGoState(GO_STATE_READY);
    ObjectGuid const wardGuid = ward->GetGUID();
    player->AddDelayedEvent(400, [player, wardGuid]() -> void
    {
        if (GameObject* ward = ObjectAccessor::GetGameObject(*player, wardGuid))
            ward->SetGoState(GO_STATE_ACTIVE);
    });
    return ward;
}

// the ward of the cell room opened for one player: the closed barrier hidden, the ward swinging open in its place for a while
static void VaultWardOpenFor(WorldObject* from, ObjectGuid const& viewer, uint32 duration)
{
    Player* player = ObjectAccessor::GetPlayer(*from, viewer);
    if (!player)
        return;

    GameObject* barrier = from->FindNearestGameObject(244927, 60.0f);
    if (barrier)
    {
        barrier->AddToHideList(viewer);
        barrier->DestroyForPlayer(player);
    }
    // the ward, where and as the one of the rewarded quest stands, swings open, closes again behind her, and gives way to the barrier
    ObjectGuid wardGuid;
    if (GameObject* ward = VaultSwingWard(player))
        wardGuid = ward->GetGUID();
    player->AddDelayedEvent(duration > 2000 ? duration - 2000 : 0, [player, wardGuid]() -> void
    {
        if (GameObject* ward = ObjectAccessor::GetGameObject(*player, wardGuid))
            ward->SetGoState(GO_STATE_READY);
    });

    ObjectGuid const barrierGuid = barrier ? barrier->GetGUID() : ObjectGuid::Empty;
    player->AddDelayedEvent(duration, [player, barrierGuid, wardGuid]() -> void
    {
        if (GameObject* ward = ObjectAccessor::GetGameObject(*player, wardGuid))
        {
            ward->SetRespawnTime(0);
            ward->Delete();
        }
        if (GameObject* barrier = ObjectAccessor::GetGameObject(*player, barrierGuid))
            barrier->RemoveFromHideList(player->GetGUID());
        player->UpdateObjectVisibility();
    });
}

// the lever: the ward opens (the rewarded quest changes the phase), Kayn and Altruis have their last words and run
// to the next hall where they wait with the next quests (the bonus objective of the Vault starts there)
class go_244923 : public GameObjectScript
{
public:
    go_244923() : GameObjectScript("go_244923") { }

    bool OnQuestReward(Player* player, GameObject* go, Quest const* quest, uint32) override
    {
        if (quest->GetQuestId() != 38672)
            return true;

        // the ward swings open: the barrier goes with the phase of the rewarded quest, the ward of that phase, already open,
        // is replaced for this session by one that opens before him
        if (GameObject* openWard = go->FindNearestGameObject(244925, 30.0f))
            if (openWard->GetDBTableGUIDLow())     // the static one, not one swung open for someone
            {
                openWard->AddToHideList(player->GetGUID());
                openWard->DestroyForPlayer(player);
            }
        if (GameObject* barrier = go->FindNearestGameObject(244927, 30.0f))
        {
            barrier->AddToHideList(player->GetGUID());
            barrier->DestroyForPlayer(player);
        }
        VaultSwingWard(player);

        struct Runner { uint32 freed; uint32 waiting; uint8 say; Position start; Position hall; uint32 delay; };
        Runner const runners[] =
        {
            { 99631, 92980, 2, { 4328.41f, -583.50f, -281.92f, 3.29f }, { 4330.08f, -548.28f, -281.75f, 1.68f }, 500 },  // Kayn
            { 99632, 92986, 3, { 4322.64f, -583.44f, -281.92f, 0.02f }, { 4321.75f, -548.86f, -281.75f, 1.68f }, 4000 }, // Altruis
        };
        for (Runner const& runner : runners)
        {
            // the one freed by this player, or a new one when he is not there any more
            Creature* demonHunter = nullptr;
            std::list<Creature*> list;
            go->GetCreatureListWithEntryInGrid(list, runner.freed, 60.0f);
            for (Creature* creature : list)
                if (TempSummon* summon = creature->ToTempSummon())
                    if (summon->GetSummonerGUID() == player->GetGUID())
                        demonHunter = creature;
            if (!demonHunter)
            {
                demonHunter = player->SummonCreature(runner.freed, runner.start, TEMPSUMMON_MANUAL_DESPAWN, 0, 0, player->GetGUID());
                if (!demonHunter)
                    continue;
                demonHunter->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                demonHunter->LoadEquipment(1, true);
            }

            // the Kayn or Altruis of the next hall, who gives the next quests, is hidden while this one runs to his place
            Creature* waiting = nullptr;
            std::list<Creature*> waitingList;
            go->GetCreatureListWithEntryInGrid(waitingList, runner.waiting, 80.0f);
            for (Creature* creature : waitingList)
                if (!creature->isSummon() && creature->GetExactDist2d(&runner.hall) < 3.0f)
                    waiting = creature;
            if (waiting)
            {
                waiting->AddToHideList(player->GetGUID());
                waiting->DestroyForPlayer(player);
            }

            ObjectGuid const viewer = player->GetGUID();
            ObjectGuid const waitingGuid = waiting ? waiting->GetGUID() : ObjectGuid::Empty;
            uint8 const say = runner.say;
            Position const hall = runner.hall;
            demonHunter->GetMotionMaster()->Clear(false);
            demonHunter->AddDelayedEvent(runner.delay, [demonHunter, viewer, say]() -> void
            {
                sCreatureTextMgr->SendChat(demonHunter, say, viewer);
            });
            demonHunter->AddDelayedEvent(runner.delay + 3500, [demonHunter, hall]() -> void
            {
                demonHunter->SetWalk(false);
                demonHunter->GetMotionMaster()->MovePoint(VAULT_RUN_TO_HALL, hall);
            });
            // on his place he hands over to the one of the hall (npc_vault_freed_illidari::MovementInform)
            if (demonHunter->AI())
                demonHunter->AI()->SetGUID(waitingGuid, VAULT_RUN_TO_HALL);
            demonHunter->DespawnOrUnsummon(runner.delay + 30000);
        }
        return true;
    }
};

static void RunOutOfTheVault(Creature* runner, float x, float y)
{
    float z = -278.0f;
    runner->UpdateGroundPositionZ(x, y, z);
    PathGenerator path(runner);
    path.CalculatePath(x, y, z);
    Movement::PointsArray const road = VaultKeepOffWalls(runner, path.GetPath(), 1.0f);
    if (road.empty())
    {
        runner->DespawnOrUnsummon(500);
        return;
    }

    float length = 0.0f;
    G3D::Vector3 last(runner->GetPositionX(), runner->GetPositionY(), runner->GetPositionZ());
    for (G3D::Vector3 const& point : road)
    {
        length += (point - last).length();
        last = point;
    }
    runner->GetMotionMaster()->MoveSmoothPath(1, road.data(), road.size(), false);
    runner->DespawnOrUnsummon(uint32(length / std::max(1.0f, runner->GetSpeed(MOVE_RUN)) * IN_MILLISECONDS) + 500);
}

class npc_96665 : public CreatureScript
{
public:
    npc_96665() : CreatureScript("npc_96665") {}
        
    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        switch(quest->GetQuestId()) 
        {
            // Kayn and Altruis run out of the Vault of the Betrayer ahead of the demon hunter and are gone a little past the
            // door; the ones standing there leave this player's phase (zone 7814, entry 104)
            case 39682:
                if (Creature* targ = player->SummonCreature(creature->GetEntry(), creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ(), creature->GetOrientation()))
                {
                    targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                    sCreatureTextMgr->SendChat(targ, TEXT_GENERIC_0, player->GetGUID());
                    targ->AddDelayedEvent(1000, [targ]() -> void { RunOutOfTheVault(targ, 4252.0f, -302.0f); });
                }
                if (Creature* targ = player->SummonCreature(92986, 4078.70f, -288.80f, -281.45f, 3.16f))
                {
                    targ->AddPlayerInPersonnalVisibilityList(player->GetGUID());
                    targ->AddDelayedEvent(1000, [targ]() -> void { RunOutOfTheVault(targ, 4248.0f, -294.0f); });
                }
                break;
            
            default:
                break;
        }

        return true;
    }
    
    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_96665AI(creature);
    }
    struct npc_96665AI : public ScriptedAI
    {
        npc_96665AI(Creature* creature) : ScriptedAI(creature) 
        {
            check1 = false;
            check2 = false;
            timer = 0;
        }
        
        bool check1;
        bool check2;
        uint32 timer;
        
        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetTypeId() == TYPEID_PLAYER)
            {
                // Altruis answers Kayn's glance, then Kayn answers him once his long line is read (French timing)
                check1 = true;
                timer = 2500;
            }
            else 
                me->DespawnOrUnsummon();
        }
        
        void UpdateAI(uint32 diff) override
        {
            if (check1)
                if (timer <= diff)
                {
                    if (Creature* t = me->FindNearestCreature(92986, 50.0f, true))
                        t->AI()->Talk(2);
                    check1 = false;
                    timer = 8000;
                    check2 = true;
                } else timer -= diff;
                
            if (check2)
                if (timer <= diff)
                {
                    Talk(2);
                    check2 = false;
                } else timer -= diff;
        }

    };
};

class npc_96666 : public CreatureScript
{
public:
    npc_96666() : CreatureScript("npc_96666") {}
            
    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_96666AI(creature);
    }
    struct npc_96666AI : public ScriptedAI
    {
        npc_96666AI(Creature* creature) : ScriptedAI(creature) {}

        // the Khadgar who came for each demon hunter: he is the only one, and comes again after a reconnection
        std::map<ObjectGuid, ObjectGuid> _khadgarFor;

        // Kayn or Altruis, outside the Vault: once the Hunt for Freedom is done, Khadgar comes for the demon hunter, for him alone,
        // until The Illidari Depart is done
        void MoveInLineOfSight(Unit* who) override
        {
            Player* player = who->ToPlayer();
            if (!player || !me->IsWithinDistInMap(who, 10.0f))
                return;

            auto done = [player](uint32 questId) -> bool
            {
                QuestStatus const status = player->GetQuestStatus(questId);
                return status == QUEST_STATUS_COMPLETE || status == QUEST_STATUS_REWARDED;
            };
            if ((!done(39688) && !done(39694)) || player->GetQuestRewardStatus(39689) || player->GetQuestRewardStatus(39690))
                return;

            auto itr = _khadgarFor.find(player->GetGUID());
            if (itr != _khadgarFor.end())
                if (Creature* khadgar = ObjectAccessor::GetCreature(*me, itr->second))
                    if (khadgar->IsInWorld())
                        return;

            // he comes as a raven from the sky behind him, lands, then takes back his own shape (his SmartAI, 6 s after coming)
            if (Creature* targ = player->SummonCreature(97978, 4143.3f, -885.7f, 301.2f, 1.0f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID(), NULL))
            {
                targ->SetDisplayId(68476);
                targ->SetCanFly(true);
                targ->SetDisableGravity(true);
                targ->GetMotionMaster()->MoveSmoothFlyPath(0, Position(4154.08f, -868.86f, 291.69f, 1.0f), 7.0f);
                VaultWhenArrived(targ, [targ]() -> void
                {
                    targ->SetCanFly(false);
                    targ->SetDisableGravity(false);
                    targ->SetFacingTo(1.0f);
                });
                _khadgarFor[player->GetGUID()] = targ->GetGUID();
            }
        }
    };
    
};



// 241553 - Illidari Battle Blades: Our Last Hope's rack. Used during the quest it puts straight in the bags the Fel-Etched
// Glaives not owned yet: no loot window to close by mistake, no personal loot lock that would hide it for good. It can only be
// used while a hidden objective of the quest asks for it, which also outlines it
struct go_vault_illidari_battle_blades : public GameObjectAI
{
    go_vault_illidari_battle_blades(GameObject* go) : GameObjectAI(go) { }

    // the object has a lock: opening it calls GossipUse, never GossipHello
    bool GossipUse(Player* player) override
    {
        return GossipHello(player, true);
    }

    bool GossipHello(Player* player, bool /*isUse*/) override
    {
        if (player->GetQuestStatus(38669) != QUEST_STATUS_INCOMPLETE)
            return true;

        static uint32 const glaives[2][2] = { { 128956, 97574 }, { 132243, 98369 } };   // main hand, off hand, and their credits
        for (auto const& glaive : glaives)
        {
            if (!player->HasItemCount(glaive[0], 1, true) && !player->AddItem(glaive[0], 1))
                continue;
            player->KilledMonsterCredit(glaive[1]);
        }
        // the hidden objective that lights the rack up for the quest: done, it goes dark
        player->KillCreditGO(go->GetEntry(), go->GetGUID());
        return true;
    }
};

// 99501 - Legion Portal of the Vault: every 10 to 15 seconds a Fel Shocktrooper or a Savage Felstalker comes out of it and
// runs at the nearest Freed Illidari or Drelanim Whisperwind, so that the invasion never stops while someone is around
struct npc_vault_legion_portal : public ScriptedAI
{
    npc_vault_legion_portal(Creature* creature) : ScriptedAI(creature), _summons(creature) { }

    uint32 _timer = urand(2000, 8000);
    uint32 _checkTimer = 2000;
    SummonList _summons;
    GuidList _returned;     // demons placed in the hall that came back through this portal, watched like its own
    // last place and count of checks spent there, for each demon on its way
    std::map<ObjectGuid, std::pair<Position, uint8>> _progress;

    void JustSummoned(Creature* summon) override
    {
        _summons.Summon(summon);
    }

    void SummonedCreatureDespawn(Creature* summon) override
    {
        _summons.Despawn(summon);
        _progress.erase(summon->GetGUID());
    }

    // the same portal stands once per phase set: only the first of them sends demons
    bool Leads() const
    {
        std::list<Creature*> portals;
        me->GetCreatureListWithEntryInGrid(portals, me->GetEntry(), 3.0f);
        for (Creature* portal : portals)
            if (portal != me && portal->GetDBTableGUIDLow() && portal->GetDBTableGUIDLow() < me->GetDBTableGUIDLow())
                return false;
        return true;
    }

    // the nearest Illidari reachable on foot without a detour: a road much longer than the straight line goes over the scenery
    Creature* PickFoe(WorldObject const* walker, Position const& from, Creature const* skip = nullptr) const
    {
        Creature* target = nullptr;
        float best = 200.0f;
        for (uint32 entry : { 96656u, 96847u })     // Freed Illidari, Drelanim Whisperwind
        {
            std::list<Creature*> list;
            me->GetCreatureListWithEntryInGrid(list, entry, 80.0f);
            for (Creature* illidari : list)
            {
                if (illidari == skip || !illidari->IsAlive() || std::fabs(illidari->GetPositionZ() - from.GetPositionZ()) > 12.0f)
                    continue;
                PathGenerator path(walker);
                path.CalculatePath(from, illidari->GetPosition());
                // a post standing just off the walkable ground (Drelanim's) only gives a path that stops short of it
                PathType const type = path.GetPathType();
                bool const nearEnough = (type & PATHFIND_INCOMPLETE) && !(type & (PATHFIND_NOPATH | PATHFIND_SHORTCUT | PATHFIND_NOT_USING_PATH | PATHFIND_FARFROMPOLY_START))
                    && !path.GetPath().empty() && illidari->GetExactDist(path.GetPath().back().x, path.GetPath().back().y, path.GetPath().back().z) < 5.0f;
                if (type != PATHFIND_NORMAL && !nearEnough)
                    continue;
                float const length = path.GetTotalLength();
                if (length > from.GetExactDist(illidari) * 1.4f + 4.0f)
                    continue;
                // the ones already beset weigh more, so that every post gets its share of the fight
                // Drelanim, the leader of the hall, draws the demons of the portals around her though others stand nearer
                float const cost = length + 12.0f * float(illidari->getAttackers()->size()) - (entry == 96847 ? 30.0f : 0.0f);
                if (cost >= best)
                    continue;
                best = cost;
                target = illidari;
            }
        }
        return target;
    }

    void SendAt(Creature* demon, Creature* foe)
    {
        demon->SetWalk(false);
        demon->DeleteThreatList();
        demon->AddThreat(foe, 100.0f);
        demon->AI()->AttackStart(foe);
        _progress[demon->GetGUID()] = { demon->GetPosition(), 0 };
    }

    // a demon keeps the foe it has; one left without a foe is sent at the nearest reachable one,
    // and one stuck away from its foe for a long while, or finding none, goes back through the portal
    void WatchDemons()
    {
        _returned.remove_if([this](ObjectGuid const& guid)
        {
            Creature* demon = ObjectAccessor::GetCreature(*me, guid);
            if (demon && demon->IsAlive())
                return false;
            _progress.erase(guid);
            return true;
        });

        GuidList watched(_summons);
        watched.insert(watched.end(), _returned.begin(), _returned.end());
        for (ObjectGuid const& guid : watched)
        {
            Creature* demon = ObjectAccessor::GetCreature(*me, guid);
            if (!demon || !demon->IsAlive() || !demon->AI())
                continue;
            Unit* victim = demon->getVictim();
            auto& progress = _progress[guid];
            if (victim && victim->IsAlive())
            {
                if (demon->IsWithinMeleeRange(victim) || demon->GetExactDist2d(&progress.first) > 1.5f)
                    progress = { demon->GetPosition(), 0 };
                else if (++progress.second >= 10)
                    demon->DespawnOrUnsummon(500);
                continue;
            }
            if (++progress.second < 2)
                continue;
            if (Creature* foe = PickFoe(demon, demon->GetPosition()))
                SendAt(demon, foe);
            else if (progress.second >= 4)
                demon->DespawnOrUnsummon(500);
        }
    }

    // anchored, an Illidari cannot reach a foe busy further away: it turns to whoever stands within reach, its own attackers first
    static void StrikeWithinReach(Creature* illidari)
    {
        Unit* victim = illidari->getVictim();
        if (victim && victim->IsAlive() && illidari->IsWithinMeleeRange(victim))
            return;

        Unit* foe = nullptr;
        for (Unit* attacker : *illidari->getAttackers())
            if (attacker->IsAlive() && !attacker->IsPlayer() && illidari->IsWithinMeleeRange(attacker))
            {
                foe = attacker;
                break;
            }
        if (!foe)
        {
            std::list<Unit*> around;
            illidari->GetAttackableUnitListInRange(around, 6.0f);
            for (Unit* unit : around)
                if (unit->IsAlive() && !unit->IsPlayer() && illidari->IsWithinMeleeRange(unit) && illidari->IsValidAttackTarget(unit))
                {
                    foe = unit;
                    break;
                }
        }
        if (!foe)
            return;

        if (victim)
            illidari->getThreatManager().modifyThreatPercent(victim, -100);
        illidari->AddThreat(foe, 100.0f);
        illidari->AI()->AttackStart(foe);
    }

    // the Illidari hold the posts they were placed at: anchored there, they strike whatever comes at them and the demons walk up to them,
    // where chasing demon after demon gathered them all in one spot; one already drawn away goes back first
    void HoldPosts()
    {
        for (uint32 entry : { 96656u, 96847u })     // Freed Illidari, Drelanim Whisperwind
        {
            std::list<Creature*> list;
            me->GetCreatureListWithEntryInGrid(list, entry, 60.0f);
            for (Creature* illidari : list)
            {
                if (!illidari->IsAlive() || illidari->isSummon() || illidari->IsInEvadeMode() || !illidari->AI())
                    continue;
                if (illidari->HasUnitState(UNIT_STATE_ROOT))
                {
                    StrikeWithinReach(illidari);
                    continue;
                }
                float const away = illidari->GetExactDist2d(illidari->GetHomePosition());
                if (away < 3.0f)
                    illidari->SetControlled(true, UNIT_STATE_ROOT);
                else if (illidari->isInCombat() && away > 10.0f)
                    illidari->AI()->EnterEvadeMode();
            }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (me->GetMapId() != 1468)
            return;

        if (_checkTimer > diff)
            _checkTimer -= diff;
        else
        {
            _checkTimer = 2000;
            WatchDemons();
            if (Leads())
                HoldPosts();
        }

        if (_timer > diff)
        {
            _timer -= diff;
            return;
        }
        _timer = urand(3000, 6000);

        if (!Leads() || _summons.size() >= 6 || !me->SelectNearestPlayer(100.0f))
            return;

        Position exit;
        Creature* foe = Exit(exit);
        if (!foe)
            return;
        Creature* demon = me->SummonCreature(urand(0, 1) ? 92776 : 92782, exit, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
        if (demon && demon->AI())
            SendAt(demon, foe);
    }

    // demons step out of the ring of crystals on the side of the hall, where they find a clear road to an Illidari
    Creature* Exit(Position& exit) const
    {
        float const toHall = me->GetAngle(4322.0f, -490.0f);
        for (float offset : { 0.0f, 0.5f, -0.5f, 1.0f, -1.0f })
        {
            float const angle = toHall + offset;
            float x = me->GetPositionX() + 5.0f * std::cos(angle);
            float y = me->GetPositionY() + 5.0f * std::sin(angle);
            float z = me->GetPositionZ() + 2.0f;
            me->UpdateGroundPositionZ(x, y, z);
            if (std::fabs(z - me->GetPositionZ()) > 2.5f || !me->IsWithinLOS(x, y, z + 1.0f))
                continue;
            exit.Relocate(x, y, z, angle);
            if (Creature* foe = PickFoe(me, exit))
                return foe;
        }
        return nullptr;
    }

    // a demon placed in the hall comes back to life through the portal rather than on its spot, so it looks like a new one
    void SetGUID(ObjectGuid const& guid, int32 id) override
    {
        if (id != VAULT_PORTAL_SENDS_BACK)
            return;
        Creature* demon = ObjectAccessor::GetCreature(*me, guid);
        Position exit;
        Creature* foe = demon ? Exit(exit) : nullptr;
        if (!foe || !demon->AI())
            return;
        demon->GetMap()->CreatureRelocation(demon, exit.GetPositionX(), exit.GetPositionY(), exit.GetPositionZ(), exit.GetOrientation(), true);
        SendAt(demon, foe);
        _returned.push_back(demon->GetGUID());
    }
};

// 196661 (Kayn), 196662 (Altruis): the follower choice of A New Direction. The one left aside says his piece and walks
// away towards the next chamber; his copy of the Court is lost from sight for this demon hunter (phases 4913 and 4914)
class spell_vault_follower_choice : public SpellScriptLoader
{
public:
    spell_vault_follower_choice() : SpellScriptLoader("spell_vault_follower_choice") { }

    class spell_vault_follower_choice_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_vault_follower_choice_SpellScript);

        void HandleAfterCast()
        {
            Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
            if (!player)
                return;

            // the choice is a tracking quest set as rewarded without a word to the client: the phases are told here
            player->AddDelayedEvent(100, [player]() -> void
            {
                PhaseUpdateData phaseUpdate;
                phaseUpdate.AddQuestUpdate(40374);
                phaseUpdate.AddQuestUpdate(40375);
                player->GetPhaseMgr().NotifyConditionChanged(phaseUpdate);
            });

            bool const kayn = GetSpellInfo()->Id == 196661;
            // Altruis: "There is no convincing you, I see. Very well, I will take my leave..."; Kayn: "You will trust the traitor?..."
            TempSummon* leaver = player->SummonCreature(kayn ? 97267 : 97265, kayn ? VaultCourtAltruis : VaultCourtKayn,
                TEMPSUMMON_TIMED_DESPAWN, 20000, 0, player->GetGUID());
            if (!leaver)
                return;
            sCreatureTextMgr->SendChat(leaver, kayn ? 3 : 0, player->GetGUID());
            // a few steps on the way to the next chamber, then he is gone
            VaultRoad const* road = kayn ? &VaultRoadAltruisToChamber : &VaultRoadKaynToChamber;
            leaver->AddDelayedEvent(2500, [leaver, road]() -> void
            {
                VaultFollowRoad(leaver, *road, true, Seconds(0), 8);
            });
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_vault_follower_choice_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_vault_follower_choice_SpellScript();
    }
};

// 97273 Kayn, 97274 Altruis, 99968 Kor'vas, 100065 Cassiel in Bastillax's chamber: they fall on Bastillax as soon as he fights
// a demon hunter, and nothing wounds them
struct npc_vault_bastillax_ally : public ScriptedAI
{
    npc_vault_bastillax_ally(Creature* creature) : ScriptedAI(creature) { }

    uint32 _lookTimer = 1000;

    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*damageType*/) override
    {
        if (!attacker || !attacker->IsControlledByPlayer())
            damage = 0;
    }

    void UpdateAI(uint32 diff) override
    {
        if (_lookTimer <= diff)
        {
            _lookTimer = 1000;
            if (!me->getVictim() && !me->isSummon())
                if (Creature* bastillax = me->FindNearestCreature(96783, 70.0f))
                    if (bastillax->isInCombat() && bastillax->getVictim() && bastillax->getVictim()->IsControlledByPlayer())
                    {
                        me->AddThreat(bastillax, 1.0f);
                        AttackStart(bastillax);
                    }
        }
        else
            _lookTimer -= diff;

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

// 97296 Archmage Khadgar, waiting in Stormwind and Orgrimmar for the demon hunters he brought from the Vault: once The Call
// of War is taken he turns into a raven and flies away, for this demon hunter alone (his copy is lost from sight, phases 4922 and 4923)
class npc_dh_khadgar_call_of_war : public CreatureScript
{
public:
    npc_dh_khadgar_call_of_war() : CreatureScript("npc_dh_khadgar_call_of_war") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() != 39691 && quest->GetQuestId() != 39692)
            return false;

        Position const from = creature->GetPosition();
        TempSummon* raven = player->SummonCreature(creature->GetEntry(), from, TEMPSUMMON_TIMED_DESPAWN, 20000, 0, player->GetGUID());
        if (!raven)
            return false;
        raven->SetDisplayId(68476);
        raven->SetCanFly(true);
        raven->SetDisableGravity(true);
        // Stormwind: towards the Keep; Orgrimmar: up over the city, towards Dalaran
        Position const to = quest->GetQuestId() == 39691 ? Position(-8511.6f, 421.9f, 135.0f, 0.0f)
            : Position(from.GetPositionX() + 30.0f, from.GetPositionY(), from.GetPositionZ() + 35.0f, 0.0f);
        raven->AddDelayedEvent(1000, [raven, to]() -> void
        {
            raven->GetMotionMaster()->MoveSmoothFlyPath(1, to, 9.0f);
            VaultVanishOnArrival(raven, Seconds(0));
        });
        return false;
    }
};

// Entering the Tomb of the Penitent after breaking out, Bastillax taunts the demon hunter in a talking head; he does it again
// when the demon hunter steps off the elevator onto the top of the Vault (Up to the Top)
class player_vault_bastillax_taunt : public PlayerScript
{
public:
    player_vault_bastillax_taunt() : PlayerScript("player_vault_bastillax_taunt") { }

    enum
    {
        AREA_TOMB_OF_THE_PENITENT   = 7871,
        CONVERSATION_BASTILLAX      = 1181,
        CONVERSATION_BASTILLAX_TOP  = 1184,
        QUEST_HUNT_FOR_FREEDOM_A    = 39688,
        QUEST_HUNT_FOR_FREEDOM_H    = 39694,
        QUEST_UP_TO_THE_TOP         = 39686,
        QUEST_BREAKING_OUT          = 38672,
        QUEST_FEL_INFUSION          = 38689,
        QUEST_RISE_OF_THE_ILLIDARI  = 38690,
    };

    GuidSet _taunted;
    GuidSet _tauntedAtTheTop;
    GuidSet _cheered;

    // coming near Bastillax's chamber, the chosen leader calls the Illidari: "We've done it, Illidari! FREEDOM IS OURS!"
    void CheerAtTheChamber(Player* player)
    {
        if (player->GetPositionZ() < 245.0f || player->GetPositionZ() > 270.0f || player->GetDistance2d(4184.5f, -615.0f) > 45.0f)
            return;
        if (player->GetQuestStatus(QUEST_HUNT_FOR_FREEDOM_A) != QUEST_STATUS_INCOMPLETE && player->GetQuestStatus(QUEST_HUNT_FOR_FREEDOM_H) != QUEST_STATUS_INCOMPLETE)
            return;
        if (!_cheered.insert(player->GetGUID()).second)
            return;
        bool const kayn = player->GetQuestRewardStatus(40374);
        if (Creature* leader = player->FindNearestCreature(kayn ? 97273 : 97274, 80.0f))
            sCreatureTextMgr->SendChat(leader, kayn ? 3 : 4, player->GetGUID());
    }

    // Bastillax is an actor known by his model alone, which the client draws where the conversation stands:
    // set deep under the floor, only his talking head is seen
    static void Taunt(Player* player, uint32 conversationId)
    {
        Position const hidden = { player->GetPositionX(), player->GetPositionY(), player->GetPositionZ() - 60.0f, player->GetOrientation() };
        Conversation* conversation = new Conversation;
        if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), conversationId, player, nullptr, hidden))
            delete conversation;
    }

    void OnUpdate(Player* player, uint32 /*diff*/) override
    {
        if (player->GetMapId() != 1468)
            return;

        CheerAtTheChamber(player);

        float const fromElevator = player->GetDistance2d(4450.86f, -451.77f);
        if (player->GetPositionZ() > 245.0f && fromElevator > 12.0f && !player->IsOnVehicle())
        {
            // only on stepping off the elevator: farther on, Kor'vas, Kayn and Altruis speak in the Court
            if (fromElevator > 40.0f)
                return;

            QuestStatus const status = player->GetQuestStatus(QUEST_UP_TO_THE_TOP);
            if ((status == QUEST_STATUS_INCOMPLETE || status == QUEST_STATUS_COMPLETE) && _tauntedAtTheTop.insert(player->GetGUID()).second)
                Taunt(player, CONVERSATION_BASTILLAX_TOP);
            return;
        }

        if (player->GetCurrentAreaID() != AREA_TOMB_OF_THE_PENITENT)
            return;
        if (!player->GetQuestRewardStatus(QUEST_BREAKING_OUT)
            || (player->GetQuestRewardStatus(QUEST_FEL_INFUSION) && player->GetQuestRewardStatus(QUEST_RISE_OF_THE_ILLIDARI)))
            return;
        if (!_taunted.insert(player->GetGUID()).second)
            return;

        Taunt(player, CONVERSATION_BASTILLAX);
    }
};

void AddSC_warden_prison()
{
    new go_q38690();
    new npc_q38689();
    new spell_q38723();
    new npc_q38723();
    new spell_vault_demon_link();
    new spell_vault_face_kick();
    new spell_vault_kethrazor_softened();
    new spell_vault_seeping_pulse();
    RegisterCreatureAI(npc_vault_betrayer_ally);
    RegisterCreatureAI(npc_vault_kethrazor);
    RegisterCreatureAI(npc_vault_ashgolm);
    RegisterCreatureAI(npc_vault_scenery_demon);
    RegisterCreatureAI(npc_vault_court_duelist);
    RegisterCreatureAI(npc_vault_bastillax_ally);
    new spell_vault_follower_choice();
    new npc_dh_khadgar_call_of_war();
    RegisterCreatureAI(npc_vault_explosive_eye);
    RegisterCreatureAI(npc_vault_seeping_eye);
    new npc_q39682();
    new npc_q39685();
    new npc_q39683();
    new npc_dh_questgiver_96675();
    new go_q39687();
    new npc_dh_questgiver_97644();
    new npc_q39694();
    new npc_92718();
    new spell_199760();
    new npc_102391();
    new spell_196460();
    new spell_196462();
    new npc_q38672();
    RegisterCreatureAI(npc_vault_freed_illidari);
    new player_vault_stripped_of_warglaives();
    new player_vault_breaking_out_reset();
    RegisterGameObjectAI(go_vault_illidari_battle_blades);
    RegisterCreatureAI(npc_vault_legion_portal);
    new player_vault_bastillax_taunt();
    new go_244923();
    new npc_96665();
    new npc_96666();
}
