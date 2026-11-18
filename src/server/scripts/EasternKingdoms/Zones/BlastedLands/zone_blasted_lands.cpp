#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectAccessor.h"
#include "Player.h"

// Iron Demolisher NPC 82273 — Blasted Lands (pre-patch Iron Horde Incursion 6.0.2)
// 5 instances (GUIDs 254972/254990/254991/254992/254993), model 53699 (WoD cannon)
// Behaviour:
//   - Fires spell 171950 "Cannon Blast" at a fixed ground position every 6-10s (per GUID)
//   - On SpellClick (quest 35746 interaction): 2s fuse -> explosion -> despawn
//   - During fuse: clicker is made immune to NPC spells (prevents explosion damage)
//   - Respawns after 20s (spawntimesecs in creature table)

enum eDemolisher
{
    SPELL_CANNON_BLAST       = 171950,
    EVENT_CANNON_FIRE        = 1,
    EVENT_DEMOLISHER_DESTROY = 2,
    EVENT_REMOVE_IMMUNITY    = 3,
};

struct IronDemolisherTargetPos
{
    uint64 guid;
    float x, y, z;
};

static const IronDemolisherTargetPos ironDemolisherTargets[] =
{
    { 254972, -11412.9f, -3504.01f,  8.24f },
    { 254990, -11549.2f, -3593.77f, 14.75f },
    { 254991, -11285.2f, -3599.18f, 18.11f },
    { 254992, -11464.3f, -3623.18f, 14.26f },
    { 254993, -11367.7f, -3599.06f, 11.32f },
};

// 82273
class npc_iron_demolisher : public CreatureScript
{
public:
    npc_iron_demolisher() : CreatureScript("npc_iron_demolisher") { }

    struct npc_iron_demolisherAI : public ScriptedAI
    {
        npc_iron_demolisherAI(Creature* creature) : ScriptedAI(creature)
        {
            _targetX = 0.f;
            _targetY = 0.f;
            _targetZ = 0.f;
            _destroyed = false;
            _pendingDestroy = false;

            uint64 spawnGuid = me->GetDBTableGUIDLow();
            for (auto& entry : ironDemolisherTargets)
            {
                if (entry.guid == spawnGuid)
                {
                    _targetX = entry.x;
                    _targetY = entry.y;
                    _targetZ = entry.z;
                    break;
                }
            }
        }

        EventMap  _events;
        float     _targetX, _targetY, _targetZ;
        bool      _destroyed;
        bool      _pendingDestroy;
        ObjectGuid _clickerGuid;

        void Reset() override
        {
            _events.Reset();
            _destroyed     = false;
            _pendingDestroy = false;
            _clickerGuid   = ObjectGuid::Empty;
            me->SetReactState(REACT_PASSIVE);
            _events.ScheduleEvent(EVENT_CANNON_FIRE, urand(6000, 10000));
        }

        void OnSpellClick(Unit* clicker) override
        {
            if (_destroyed || _pendingDestroy)
                return;

            // Dismount if needed
            if (Player* player = clicker->ToPlayer())
                if (player->IsMounted())
                    player->Dismount();

            _pendingDestroy = true;

            // Make the player immune to NPC spells for the whole sequence (fuse + explosion)
            _clickerGuid = clicker->GetGUID();
            clicker->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);

            _events.ScheduleEvent(EVENT_DEMOLISHER_DESTROY, 2000);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CANNON_FIRE:
                        if (!_destroyed && _targetX != 0.f)
                            me->CastSpell(_targetX, _targetY, _targetZ, SPELL_CANNON_BLAST, true);
                        _events.ScheduleEvent(EVENT_CANNON_FIRE, urand(6000, 10000));
                        break;
                    case EVENT_DEMOLISHER_DESTROY:
                        _destroyed = true;
                        me->CastSpell(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 1.0f,
                                      SPELL_CANNON_BLAST, true);
                        // Remove the immunity 1s after the explosion (DespawnOrUnsummon gives the NPC 2s)
                        _events.ScheduleEvent(EVENT_REMOVE_IMMUNITY, 1000);
                        me->DespawnOrUnsummon(2000);
                        break;
                    case EVENT_REMOVE_IMMUNITY:
                        if (Unit* clicker = ObjectAccessor::GetUnit(*me, _clickerGuid))
                            clicker->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_iron_demolisherAI(creature);
    }
};

void AddSC_blasted_lands()
{
    new npc_iron_demolisher();
}
