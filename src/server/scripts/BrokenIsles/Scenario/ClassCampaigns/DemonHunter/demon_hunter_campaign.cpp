////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

/*
    Demon Hunter order hall campaign.
    Securing the Way (42872), at the Illidari Redoubt of Dalaran, and the Redoubt once it is done.
    The Fel Hammer (map 1519): the chained prisoners of the training ground, the Illidari of the decks, Spoils of Victory
    (41062) on the lower command center, and the doubles of Kayn and Matron Mother Malevolence.
    The Empowered Nether Crucible: The Hunter's Gaze (41066) and Immortal Soul (42510).
    The Fel Tome of the arena under the Fel Hammer (order advancement Twisting Nether): one demon a day.
    Campaign scenarios in the client data: The Invasion of Niskara (scenario 1145, map 1489); Confrontation at the Black Temple (scenario 1131, map 1621).
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "GameObjectAI.h"
#include "Group.h"
#include "SpellScript.h"
#include <vector>

namespace SecuringTheWay
{
    enum
    {
        QUEST_SECURING_THE_WAY          = 42872,
        NPC_CREDIT_SOULS_UNLEASHED      = 109230,
        NPC_CAPTURED_WRATHGUARD         = 109247,
        NPC_IMPRISONED_IMP              = 109252,
        NPC_SOUL_FONT                   = 109273,
    };

    uint32 const Executioners[] = { 109344, 109309, 109308, 109314 }; // Kor'vas, Asha, Belath, Allari

    // where the prisoners' souls gather, before the gateway (Wowhead, Dalaran floor 10, at 97.2 68.2 and 97.8 69.0)
    Position const SoulFonts[] =
    {
        { -957.6f, 4073.1f, 650.5f, 0.65f },
        { -961.9f, 4068.2f, 650.5f, 0.65f },
    };

    bool SoulsRemoved(Player* player)
    {
        return player->GetQuestStatus(QUEST_SECURING_THE_WAY) == QUEST_STATUS_INCOMPLETE
            && player->GetReqKillOrCastCurrentCount(QUEST_SECURING_THE_WAY, NPC_CREDIT_SOULS_UNLEASHED) > 0;
    }
}

// 217377 - Sentence to Death: the Illidari of the Redoubt execute the prisoners, whose souls gather in two Soul Fonts by
// the gateway, there for the hunter alone to click and rip the gateway open (217719)
class spell_dh_sentence_to_death : public SpellScript
{
    PrepareSpellScript(spell_dh_sentence_to_death);

    void HandleAfterCast()
    {
        using namespace SecuringTheWay;

        Player* hunter = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        if (!hunter)
            return;

        std::vector<Creature*> executioners;
        for (uint32 entry : Executioners)
            if (Creature* executioner = hunter->FindNearestCreature(entry, 60.0f))
                executioners.push_back(executioner);

        std::list<Creature*> prisoners;
        hunter->GetCreatureListWithEntryInGrid(prisoners, NPC_CAPTURED_WRATHGUARD, 60.0f);
        std::list<Creature*> imps;
        hunter->GetCreatureListWithEntryInGrid(imps, NPC_IMPRISONED_IMP, 60.0f);
        prisoners.splice(prisoners.end(), imps);

        uint32 delay = 300;
        for (Creature* prisoner : prisoners)
        {
            if (!prisoner->IsAlive())
                continue;

            // each prisoner by the Illidari standing nearest to him
            Creature* executioner = nullptr;
            for (Creature* candidate : executioners)
                if (!executioner || prisoner->GetExactDist(candidate) < prisoner->GetExactDist(executioner))
                    executioner = candidate;

            ObjectGuid const prisonerGuid = prisoner->GetGUID();
            ObjectGuid const executionerGuid = executioner ? executioner->GetGUID() : ObjectGuid::Empty;
            prisoner->AddDelayedEvent(delay, [prisoner, prisonerGuid, executionerGuid]() -> void
            {
                if (!prisoner->IsAlive())
                    return;

                Creature* executioner = executionerGuid ? ObjectAccessor::GetCreature(*prisoner, executionerGuid) : nullptr;
                if (executioner && executioner->IsAlive())
                {
                    executioner->SetFacingToObject(prisoner);
                    executioner->HandleEmoteCommand(EMOTE_ONESHOT_ATTACK1H);
                    executioner->Kill(prisoner);
                }
                else
                    prisoner->Kill(prisoner);
            });
            delay += 400;
        }

        for (Position const& pos : SoulFonts)
            hunter->SummonCreature(NPC_SOUL_FONT, pos, TEMPSUMMON_TIMED_DESPAWN, 5 * MINUTE * IN_MILLISECONDS, 0, hunter->GetGUID());
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_dh_sentence_to_death::HandleAfterCast);
    }
};

// 217723 - the gateway ripped open throws the hunter back. Cast by him on himself, its push has no direction of its own
// (a random one): it goes away from the gateway. The Soul Fonts go too
class spell_dh_gateway_ripped_open_knockback : public SpellScript
{
    PrepareSpellScript(spell_dh_gateway_ripped_open_knockback);

    static constexpr uint32 GO_ILLIDARI_GATEWAY = 251579;

    void HandleKnockBack(SpellEffIndex effIndex)
    {
        Unit* target = GetHitUnit();
        GameObject* gateway = target ? target->FindNearestGameObject(GO_ILLIDARI_GATEWAY, 60.0f) : nullptr;
        if (!gateway)
            return;

        PreventHitDefaultEffect(effIndex);

        // the souls are spent on the gateway: the Soul Fonts that held them, his own, are gone with it
        std::list<Creature*> fonts;
        target->GetCreatureListWithEntryInGrid(fonts, SecuringTheWay::NPC_SOUL_FONT, 60.0f);
        for (Creature* font : fonts)
            if (TempSummon* summon = font->ToTempSummon())
                if (summon->GetSummonerGUID() == target->GetGUID())
                    font->DespawnOrUnsummon(500);

        SpellEffectInfo const* effect = GetSpellInfo()->GetEffect(effIndex);
        // as Spell::EffectKnockBack: MiscValue the speed away, the effect's value the speed up, both in tenths
        target->KnockbackFrom(gateway->GetPositionX(), gateway->GetPositionY(), float(effect->MiscValue) * 0.1f, float(GetEffectValue()) * 0.1f);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dh_gateway_ripped_open_knockback::HandleKnockBack, EFFECT_0, SPELL_EFFECT_KNOCK_BACK);
    }
};

// The Illidari Redoubt after Securing the Way: the leaders have gone through the gateway to the Fel Hammer, the prisoners
// were executed for their souls; only the Enforcers keep the Redoubt
struct npc_illidari_redoubt_securing_the_way : public ScriptedAI
{
    npc_illidari_redoubt_securing_the_way(Creature* creature) : ScriptedAI(creature) { }

    bool IsHiddenFor(Player const* player) const override
    {
        using namespace SecuringTheWay;

        if (player->GetQuestRewardStatus(QUEST_SECURING_THE_WAY))
            return true;

        uint32 const entry = me->GetEntry();
        if (entry != NPC_CAPTURED_WRATHGUARD && entry != NPC_IMPRISONED_IMP)
            return false;

        QuestStatus const status = player->GetQuestStatus(QUEST_SECURING_THE_WAY);
        return status == QUEST_STATUS_COMPLETE
            || (status == QUEST_STATUS_INCOMPLETE && const_cast<Player*>(player)->GetReqKillOrCastCurrentCount(QUEST_SECURING_THE_WAY, NPC_CREDIT_SOULS_UNLEASHED) > 0);
    }
};

// Jace Darkweaver (109196), who sends the hunter to Securing the Way, then leaves the Redoubt like the others
class npc_jace_darkweaver_securing_the_way : public CreatureScript
{
public:
    npc_jace_darkweaver_securing_the_way() : CreatureScript("npc_jace_darkweaver_securing_the_way") { }

    static constexpr uint32 SPELL_SECURING_THE_WAY_ACCEPTED = 217376;

    bool OnQuestAccept(Player* player, Creature* /*creature*/, Quest const* quest) override
    {
        if (quest->GetQuestId() == SecuringTheWay::QUEST_SECURING_THE_WAY)
            player->CastSpell(player, SPELL_SECURING_THE_WAY_ACCEPTED, false);
        return false;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_illidari_redoubt_securing_the_way(creature);
    }
};

// Illidari Gateway (251579), the frame of the gateway: ripped open only once the prisoners' souls are out of them
struct go_illidari_gateway_securing_the_way : public GameObjectAI
{
    go_illidari_gateway_securing_the_way(GameObject* go) : GameObjectAI(go) { }

    bool GossipUse(Player* player) override
    {
        return !SecuringTheWay::SoulsRemoved(player);
    }
};

namespace FelHammerPrisoners
{
    uint32 constexpr NPC_CHAIN_BUNNY        = 35845;
    uint32 constexpr SPELL_DEMON_CHAIN      = 226174; // channelled: the chain is its beam, its targets are the five prisoners
    uint32 constexpr SPELL_FEIGN_DEATH_STUN = 113368;
    float  constexpr CHAIN_SEARCH_RANGE     = 25.0f;

    // Imprisoned Forgefiend, Eradicator, Executioner, Centurion, Weaver
    uint32 constexpr Prisoners[] = { 113636, 113647, 113673, 113674, 113676 };
}

// Fel Hammer training ground: the demons the Illidari train on. Each wears its shackles (template addon) and is held by the
// Demon Chain of the bunnies hanging above it; like a training dummy, it takes no damage and never fights back.
struct npc_fel_hammer_chained_prisoner : public ScriptedAI
{
    npc_fel_hammer_chained_prisoner(Creature* creature) : ScriptedAI(creature) { }

    uint32 chainTimer = 0;

    void Reset() override
    {
        me->AddUnitTypeMask(UNIT_MASK_TRAINING_DUMMY);
        if (!me->HasAura(FelHammerPrisoners::SPELL_FEIGN_DEATH_STUN))
            me->AddAura(FelHammerPrisoners::SPELL_FEIGN_DEATH_STUN, me);
        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
        me->SetReactState(REACT_PASSIVE);
        chainTimer = 1 * IN_MILLISECONDS;
    }

    void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType /*dmgType*/) override
    {
        damage = 0;
    }

    void UpdateAI(uint32 diff) override
    {
        if (chainTimer > diff)
        {
            chainTimer -= diff;
            return;
        }
        // checked again later: the bunnies of a neighbouring grid may load after the prisoner
        chainTimer = 10 * IN_MILLISECONDS;

        std::list<Creature*> bunnies;
        me->GetCreatureListWithEntryInGrid(bunnies, FelHammerPrisoners::NPC_CHAIN_BUNNY, FelHammerPrisoners::CHAIN_SEARCH_RANGE);
        for (Creature* bunny : bunnies)
            if (HeldBy(bunny) && !me->HasAura(FelHammerPrisoners::SPELL_DEMON_CHAIN, bunny->GetGUID()))
                bunny->CastSpell(me, FelHammerPrisoners::SPELL_DEMON_CHAIN, true);
    }

    // a bunny holds the prisoner nearest to it on the floor plan; it hangs well above them all
    bool HeldBy(Creature* bunny) const
    {
        float const mine = bunny->GetExactDist2d(me);
        std::list<Creature*> prisoners;
        for (uint32 entry : FelHammerPrisoners::Prisoners)
            bunny->GetCreatureListWithEntryInGridAppend(prisoners, entry, mine + 20.0f);

        for (Creature* other : prisoners)
            if (other != me && bunny->GetExactDist2d(other) < mine)
                return false;
        return true;
    }
};

namespace FelHammerIllidari
{
    enum class Pastime : uint8
    {
        Chat,
        Craft,
        Sleep,
        Spar,
    };

    struct Role
    {
        uint64 Spawn;
        Pastime Kind;
        uint64 Partner;
    };

    // spawns of the Illidari Enforcers (99601-99604) and Adepts (113228) on the decks
    Role constexpr DeckRoles[] =
    {
        { 384647, Pastime::Chat,  384658 }, { 384658, Pastime::Chat,  384647 },
        { 384607, Pastime::Chat,  384602 }, { 384602, Pastime::Chat,  384607 },
        { 384565, Pastime::Chat,  384564 }, { 384564, Pastime::Chat,  384565 },
        { 384623, Pastime::Chat,  384613 }, { 384613, Pastime::Chat,  384623 },
        { 384614, Pastime::Craft, 0 },
        { 384665, Pastime::Sleep, 0 },
        { 384582, Pastime::Spar,  384586 }, { 384586, Pastime::Spar,  384582 },
        { 384585, Pastime::Spar,  384575 }, { 384575, Pastime::Spar,  384585 },
    };

    uint32 const ChatEmotes[] = { EMOTE_ONESHOT_TALK, EMOTE_ONESHOT_TALK, EMOTE_ONESHOT_EXCLAMATION, EMOTE_ONESHOT_QUESTION,
                                  EMOTE_ONESHOT_LAUGH, EMOTE_ONESHOT_POINT, EMOTE_ONESHOT_YES, EMOTE_ONESHOT_NO };
    uint32 const SparEmotes[] = { EMOTE_ONESHOT_ATTACK1H, EMOTE_ONESHOT_ATTACK_OFF, EMOTE_ONESHOT_ATTACK2HTIGHT, EMOTE_ONESHOT_PARRY_UNARMED };
}

namespace SpoilsOfVictory
{
    enum
    {
        QUEST_SPOILS_OF_VICTORY         = 41062,
        NPC_GAARDOUN_TO_TALK_TO         = 98650,
        NPC_GAARDOUN_GUIDE              = 103047,
        NPC_CREDIT_GAARDOUN             = 103025,
        SPELL_SUMMON_GAARDOUN_GUIDE     = 203676,
        POINT_STEP_ASIDE                = 1,
    };

    // the two Ashtongue Warriors barring the lower command center, and where they stand once they let the hunter in:
    // the places of the two warriors (384581, 384583) shown when the quest is no longer in progress
    struct DoorGuard
    {
        uint64 Spawn;
        uint64 PlaceOf;
        Position Aside;
    };

    DoorGuard const DoorGuards[] =
    {
        { 384537, 384581, { 1597.2f, 1418.6f, 213.9f, 4.73f } },
        { 384538, 384583, { 1597.3f, 1408.6f, 213.9f, 1.66f } },
    };
}

// Battlelord Gaardoun (98650), Spoils of Victory: talked to, he sends his own double to lead the hunter down to the lower
// command center, and steps out of the hunter's sight
class npc_gaardoun_spoils_of_victory : public CreatureScript
{
public:
    npc_gaardoun_spoils_of_victory() : CreatureScript("npc_gaardoun_spoils_of_victory") { }

    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 /*action*/) override
    {
        player->PlayerTalkClass->SendCloseGossip();
        player->CastSpell(player, SpoilsOfVictory::SPELL_SUMMON_GAARDOUN_GUIDE, false);
        return true;
    }

    struct npc_gaardoun_spoils_of_victoryAI : public ScriptedAI
    {
        npc_gaardoun_spoils_of_victoryAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 creditTimer = 3 * IN_MILLISECONDS;

        void UpdateAI(uint32 diff) override
        {
            if (creditTimer > diff)
            {
                creditTimer -= diff;
                return;
            }
            creditTimer = 3 * IN_MILLISECONDS;

            std::list<Player*> players;
            me->GetPlayerListInGrid(players, 20.0f);
            for (Player* player : players)
                player->KilledMonsterCredit(SpoilsOfVictory::NPC_CREDIT_GAARDOUN);
        }

        bool IsHiddenFor(Player const* player) const override
        {
            using namespace SpoilsOfVictory;
            return player->GetQuestStatus(QUEST_SPOILS_OF_VICTORY) == QUEST_STATUS_INCOMPLETE
                && const_cast<Player*>(player)->GetReqKillOrCastCurrentCount(QUEST_SPOILS_OF_VICTORY, NPC_GAARDOUN_TO_TALK_TO) > 0;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_gaardoun_spoils_of_victoryAI(creature);
    }
};

// Ashtongue Warriors (98655) of the lower command center: the two at its door step aside to their places at the back when
// Gaardoun's double comes; the two who stand there in the end are not shown before
struct npc_fel_hammer_ashtongue_warrior : public ScriptedAI
{
    npc_fel_hammer_ashtongue_warrior(Creature* creature) : ScriptedAI(creature) { }

    SpoilsOfVictory::DoorGuard const* guard = nullptr;
    bool placeOfGuard = false;
    bool aside = false;
    uint32 checkTimer = 500;

    void InitializeAI() override
    {
        for (SpoilsOfVictory::DoorGuard const& candidate : SpoilsOfVictory::DoorGuards)
        {
            if (candidate.Spawn == me->GetDBTableGUIDLow())
                guard = &candidate;
            if (candidate.PlaceOf == me->GetDBTableGUIDLow())
                placeOfGuard = true;
        }
        ScriptedAI::InitializeAI();
    }

    bool IsHiddenFor(Player const* player) const override
    {
        return placeOfGuard && player->GetQuestStatus(SpoilsOfVictory::QUEST_SPOILS_OF_VICTORY) == QUEST_STATUS_INCOMPLETE;
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (guard && type == POINT_MOTION_TYPE && id == SpoilsOfVictory::POINT_STEP_ASIDE)
            me->SetFacingTo(guard->Aside.GetOrientation());
    }

    void UpdateAI(uint32 diff) override
    {
        if (UpdateVictim())
        {
            DoMeleeAttackIfReady();
            return;
        }

        if (!guard)
            return;

        if (checkTimer > diff)
        {
            checkTimer -= diff;
            return;
        }
        checkTimer = 500;

        bool const leading = SomeoneLedNearby();
        if (!aside && leading && me->FindNearestCreature(SpoilsOfVictory::NPC_GAARDOUN_GUIDE, 8.0f))
        {
            aside = true;
            me->SetWalk(true);
            me->GetMotionMaster()->MovePoint(SpoilsOfVictory::POINT_STEP_ASIDE, guard->Aside);
        }
        // nobody is being led any more (quest turned to complete, abandoned, hunter gone): back to the door
        else if (aside && !leading)
        {
            aside = false;
            me->GetMotionMaster()->MoveTargetedHome();
        }
    }

    // a hunter near the door who talked to Gaardoun and is still to follow him
    bool SomeoneLedNearby() const
    {
        using namespace SpoilsOfVictory;

        std::list<Player*> players;
        me->GetPlayerListInGrid(players, 40.0f);
        for (Player* player : players)
            if (player->GetQuestStatus(QUEST_SPOILS_OF_VICTORY) == QUEST_STATUS_INCOMPLETE
                && player->GetReqKillOrCastCurrentCount(QUEST_SPOILS_OF_VICTORY, NPC_GAARDOUN_TO_TALK_TO) > 0)
                return true;
        return false;
    }
};

// Fel Hammer: what the Illidari of the decks do between two battles. A spawn without a role keeps its usual behaviour.
struct npc_fel_hammer_illidari : public ScriptedAI
{
    npc_fel_hammer_illidari(Creature* creature) : ScriptedAI(creature) { }

    FelHammerIllidari::Role const* role = nullptr;
    uint32 pastimeTimer = 0;
    bool facingPartner = false;

    void Reset() override
    {
        using namespace FelHammerIllidari;

        role = nullptr;
        for (Role const& candidate : DeckRoles)
            if (candidate.Spawn == me->GetDBTableGUIDLow())
                role = &candidate;
        if (!role)
            return;

        facingPartner = false;
        // partners take turns: the lower spawn speaks or strikes first
        pastimeTimer = role->Spawn < role->Partner ? urand(1000, 2000) : urand(3000, 4000);
        switch (role->Kind)
        {
            case Pastime::Craft:
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_USE_STANDING);
                break;
            case Pastime::Sleep:
                me->SetStandState(UNIT_STAND_STATE_SLEEP);
                break;
            case Pastime::Spar:
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_READY1H);
                break;
            default:
                break;
        }
    }

    void EnterCombat(Unit* /*victim*/) override
    {
        if (!role)
            return;
        me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
        me->SetStandState(UNIT_STAND_STATE_STAND);
    }

    void UpdateAI(uint32 diff) override
    {
        using namespace FelHammerIllidari;

        if (UpdateVictim())
        {
            DoMeleeAttackIfReady();
            return;
        }

        if (!role || (role->Kind != Pastime::Chat && role->Kind != Pastime::Spar))
            return;

        if (pastimeTimer > diff)
        {
            pastimeTimer -= diff;
            return;
        }

        if (!facingPartner)
            if (Creature* partner = FindPartner())
            {
                me->SetFacingToObject(partner);
                facingPartner = true;
            }

        if (role->Kind == Pastime::Chat)
        {
            me->HandleEmoteCommand(Trinity::Containers::SelectRandomContainerElement(ChatEmotes));
            pastimeTimer = urand(5000, 9000);
        }
        else
        {
            me->HandleEmoteCommand(Trinity::Containers::SelectRandomContainerElement(SparEmotes));
            pastimeTimer = urand(1500, 2500);
        }
    }

    Creature* FindPartner() const
    {
        std::list<Creature*> around;
        me->GetCreatureListInGrid(around, 8.0f);
        for (Creature* creature : around)
            if (creature->GetDBTableGUIDLow() == role->Partner)
                return creature;
        return nullptr;
    }
};

namespace FelHammerDoubles
{
    enum
    {
        QUEST_THE_BLOOD_OF_DEMONS       = 37447,
        QUEST_LEADER_OF_THE_ILLIDARI    = 42522,
        QUEST_LIVIN_ON_THE_LEDGE        = 46333,
        QUEST_TO_FEL_AND_BACK           = 46334,
        QUEST_MATRON_GATEWAY            = 42131,
        QUEST_MATRON_CREDIT             = 42921,
        SPELL_MATRON_GATEWAY            = 197337,
        NPC_MATRON_CREDIT               = 108077,
        TEXT_MATRON_GATEWAY_OPEN        = 2,
    };

    bool Taken(Player const* player, uint32 questId)
    {
        QuestStatus const status = player->GetQuestStatus(questId);
        return status == QUEST_STATUS_INCOMPLETE || status == QUEST_STATUS_COMPLETE || status == QUEST_STATUS_FAILED;
    }
}

// Kayn Sunfury (107724), lower command center: there from The Blood of Demons until Leader of the Illidari is taken; the rest
// of the campaign shows the Kayn of the upper deck
struct npc_kayn_sunfury_lower_command_center : public ScriptedAI
{
    npc_kayn_sunfury_lower_command_center(Creature* creature) : ScriptedAI(creature) { }

    bool IsHiddenFor(Player const* player) const override
    {
        using namespace FelHammerDoubles;

        if (Taken(player, QUEST_THE_BLOOD_OF_DEMONS))
            return false;
        return !player->GetQuestRewardStatus(QUEST_THE_BLOOD_OF_DEMONS) || Taken(player, QUEST_LEADER_OF_THE_ILLIDARI)
            || player->GetQuestRewardStatus(QUEST_LEADER_OF_THE_ILLIDARI);
    }
};

// Matron Mother Malevolence (98632): steps aside while her double (108784) stands at the same place for Livin' on the Ledge
// and To Fel and Back
class npc_matron_mother_malevolence_fel_hammer : public CreatureScript
{
public:
    npc_matron_mother_malevolence_fel_hammer() : CreatureScript("npc_matron_mother_malevolence_fel_hammer") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        using namespace FelHammerDoubles;

        switch (quest->GetQuestId())
        {
            case QUEST_MATRON_GATEWAY:
                player->CastSpell(player, SPELL_MATRON_GATEWAY, false);
                break;
            case QUEST_MATRON_CREDIT:
                player->KilledMonsterCredit(NPC_MATRON_CREDIT);
                creature->AI()->Talk(TEXT_MATRON_GATEWAY_OPEN, player->GetGUID());
                break;
            default:
                break;
        }
        return false;
    }

    struct npc_matron_mother_malevolence_fel_hammerAI : public ScriptedAI
    {
        npc_matron_mother_malevolence_fel_hammerAI(Creature* creature) : ScriptedAI(creature) { }

        bool IsHiddenFor(Player const* player) const override
        {
            using namespace FelHammerDoubles;

            return player->GetQuestStatus(QUEST_LIVIN_ON_THE_LEDGE) == QUEST_STATUS_COMPLETE
                || (player->GetQuestRewardStatus(QUEST_LIVIN_ON_THE_LEDGE) && !player->GetQuestRewardStatus(QUEST_TO_FEL_AND_BACK));
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_matron_mother_malevolence_fel_hammerAI(creature);
    }
};

namespace NetherCrucible
{
    enum
    {
        SPELL_GAZING_TELEPORT           = 203802, // The Hunter's Gaze: to the vision, before the Tomb of Sargeras (Broken Shore)
        SPELL_GAZING                    = 203790, // the vision itself: the camera flies to the Tomb, Allari speaks
        SPELL_GAZING_RETURN             = 203803, // credit, back to the crucible, the first aura removed (scene completed)
        MAP_BROKEN_ISLES                = 1220,
        SPELL_IMMORTAL_SOUL_KAYN        = 214243, // Immortal Soul: Illidan's soul speaks from the crucible
        SPELL_IMMORTAL_SOUL_ALTRUIS     = 214276,
        QUEST_SPOILS_OF_VICTORY_ALTRUIS = 41070,  // the hunter sided with Altruis
    };
}

// Empowered Nether Crucible (250677): its gossip option, shown during The Hunter's Gaze only, sends the hunter to the vision
struct go_empowered_nether_crucible : public GameObjectAI
{
    go_empowered_nether_crucible(GameObject* go) : GameObjectAI(go) { }

    bool GossipSelect(Player* player, uint32 /*sender*/, uint32 /*action*/) override
    {
        player->PlayerTalkClass->SendCloseGossip();
        player->CastSpell(player, NetherCrucible::SPELL_GAZING_TELEPORT, false);
        return true;
    }
};

// the vision plays once the hunter has arrived before the Tomb (his loading screen is over, in the map's thread); its end
// casts Gazing (203803) through spell_scene_event, which brings him back
class player_dh_hunters_gaze_vision : public PlayerScript
{
public:
    player_dh_hunters_gaze_vision() : PlayerScript("player_dh_hunters_gaze_vision") { }

    void OnLoadingScreenEnd(Player* player) override
    {
        player->AddDelayedEvent(0, [player]() -> void
        {
            using namespace NetherCrucible;

            if (!player->IsInWorld() || player->GetMapId() != MAP_BROKEN_ISLES)
                return;
            Aura* gazing = player->GetAura(SPELL_GAZING_TELEPORT);
            if (!gazing || player->HasAura(SPELL_GAZING))
                return;
            // the loading screen ate into the 30 seconds: they count from the vision on
            gazing->SetDuration(35 * IN_MILLISECONDS);
            player->CastSpell(player, SPELL_GAZING, true);
        });
    }
};

// 203802 - Gazing: if the vision never ends (client gone silent), the hunter is not left before the Tomb
class spell_dh_gazing_teleport : public AuraScript
{
    PrepareAuraScript(spell_dh_gazing_teleport);

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        using namespace NetherCrucible;

        Unit* target = GetTarget();
        if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE && target->GetMapId() == MAP_BROKEN_ISLES)
            target->CastSpell(target, SPELL_GAZING_RETURN, true);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_dh_gazing_teleport::HandleRemove, EFFECT_1, SPELL_AURA_HOVER, AURA_EFFECT_HANDLE_REAL);
    }
};

// Blood Offering (213918), Immortal Soul: the offering made at the crucible calls Illidan's soul, beside the leader the
// hunter chose
class spell_dh_blood_offering : public SpellScript
{
    PrepareSpellScript(spell_dh_blood_offering);

    void HandleAfterCast()
    {
        if (Player* player = GetCaster()->ToPlayer())
            player->CastSpell(player, player->GetQuestRewardStatus(NetherCrucible::QUEST_SPOILS_OF_VICTORY_ALTRUIS)
                ? NetherCrucible::SPELL_IMMORTAL_SOUL_ALTRUIS : NetherCrucible::SPELL_IMMORTAL_SOUL_KAYN, true);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_dh_blood_offering::HandleAfterCast);
    }
};

namespace TwistingNether
{
    // server-side tracking quest (no client record): a demon of the Fel Tome slain today; cleared by the daily reset
    uint32 constexpr QUEST_DEMON_SLAIN_TODAY = 900001;
    float  constexpr SHARE_RANGE             = 100.0f;

    // the twenty demons the Fel Tome may call
    uint32 constexpr Demons[] = { 112374, 112375, 112376, 112377, 112378, 112379, 112380, 112381, 112384, 112385,
                                  112387, 112388, 112389, 112391, 112394, 112395, 112396, 112397, 112398, 112399 };

    bool IsDemon(uint32 entry)
    {
        for (uint32 demon : Demons)
            if (demon == entry)
                return true;
        return false;
    }

    void MarkSlain(Player* player)
    {
        player->SetDailyQuestStatus(QUEST_DEMON_SLAIN_TODAY);
        player->UpdateObjectVisibility();
    }
}

// Fel Tome (114816), the arena under the Fel Hammer (order advancement Twisting Nether): one demon a day, so the tome
// stays out of sight for a hunter who slew one today
struct npc_fel_tome_twisting_nether : public ScriptedAI
{
    npc_fel_tome_twisting_nether(Creature* creature) : ScriptedAI(creature) { }

    bool IsHiddenFor(Player const* player) const override
    {
        return player->IsQuestDailyRewarded(TwistingNether::QUEST_DEMON_SLAIN_TODAY);
    }
};

class player_twisting_nether_demon_slain : public PlayerScript
{
public:
    player_twisting_nether_demon_slain() : PlayerScript("player_twisting_nether_demon_slain") { }

    void OnCreatureKill(Player* killer, Creature* killed) override
    {
        if (!TwistingNether::IsDemon(killed->GetEntry()))
            return;

        Group* group = killer->GetGroup();
        if (!group)
        {
            TwistingNether::MarkSlain(killer);
            return;
        }

        group->ForEachMemberInMap(killed->GetMap(), [killed](Player* member)
        {
            if (member->IsWithinDistInMap(killed, TwistingNether::SHARE_RANGE))
                TwistingNether::MarkSlain(member);
        });
    }
};

void AddSC_demon_hunter_campaign()
{
    RegisterSpellScript(spell_dh_sentence_to_death);
    RegisterSpellScript(spell_dh_gateway_ripped_open_knockback);
    RegisterGameObjectAI(go_illidari_gateway_securing_the_way);
    RegisterCreatureAI(npc_illidari_redoubt_securing_the_way);
    new npc_jace_darkweaver_securing_the_way();
    RegisterCreatureAI(npc_fel_hammer_chained_prisoner);
    RegisterCreatureAI(npc_fel_hammer_illidari);
    new npc_gaardoun_spoils_of_victory();
    RegisterCreatureAI(npc_fel_hammer_ashtongue_warrior);
    RegisterCreatureAI(npc_kayn_sunfury_lower_command_center);
    new npc_matron_mother_malevolence_fel_hammer();
    RegisterGameObjectAI(go_empowered_nether_crucible);
    new player_dh_hunters_gaze_vision();
    RegisterAuraScript(spell_dh_gazing_teleport);
    RegisterSpellScript(spell_dh_blood_offering);
    RegisterCreatureAI(npc_fel_tome_twisting_nether);
    new player_twisting_nether_demon_slain();
}
