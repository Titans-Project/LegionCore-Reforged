////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "GameObject.h"
#include "grimrail_depot.h"

/// The doors follow the boss states: the entrance closes during the fight, the exit opens once it is done
static DoorData const g_DoorData[] =
{
    { GrimrailDepotGobjects::GameObjectSpikedGate,             GrimrailDepotData::DataRocketspark, DOOR_TYPE_ROOM,    BOUNDARY_NONE },
    { GrimrailDepotGobjects::GameObjectIronWroughtGate,        GrimrailDepotData::DataRocketspark, DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GrimrailDepotGobjects::GameObjectSpikedGateSkylordTovra, GrimrailDepotData::DataSkyLordTovra, DOOR_TYPE_ROOM,   BOUNDARY_NONE },
    { 0,                                                       0,                                  DOOR_TYPE_ROOM,    BOUNDARY_NONE }
};

struct instance_grimrail_depot : public InstanceScript
{
    instance_grimrail_depot(InstanceMap* map) : InstanceScript(map)
    {
        SetHeaders(DataHeader);
        SetBossNumber(GrimrailDepotData::DataMaxBosses);
        LoadDoorData(g_DoorData);
    }

    InstanceScript* m_Instance = this;

    ObjectGuid m_RocketsparkGUID;
    ObjectGuid m_BorkaGUID;
    ObjectGuid m_SkylordTovraGUID;
    ObjectGuid m_SkylordTovraDragonGUID;
    ObjectGuid m_RocketsparkAndBurkaEntrance;
    ObjectGuid m_RocketsparkAndBurkaExit;
    ObjectGuid m_NitrogThunderPowerGUID;
    ObjectGuid m_NitroggTurretGUID;
    ObjectGuid m_NitroggAssaultFlapsGUID;
    ObjectGuid m_NitroggInvisibleDoorGUID;
    ObjectGuid m_SkylordTovraDoorGUID;

    void OnCreatureCreate(Creature* l_Creature) override
    {
        InstanceScript::OnCreatureCreate(l_Creature);

        if (l_Creature)
        {
            switch (l_Creature->GetEntry())
            {
            case GrimrailDepotBosses::BossRocketSpark:
                m_RocketsparkGUID = l_Creature->GetGUID();
                break;
            case GrimrailDepotBosses::BossBorkatheBrute:
                m_BorkaGUID = l_Creature->GetGUID();
                break;
            case GrimrailDepotBosses::BossNitroggThundertower:
                m_NitrogThunderPowerGUID = l_Creature->GetGUID();
                break;
            case GrimrailDepotBosses::BossSkylordTovra:
                m_SkylordTovraGUID = l_Creature->GetGUID();
                break;
            case GrimrailDepotCreatures::CreatureIronTurretNitrogg:
                m_NitroggTurretGUID = l_Creature->GetGUID();
                break;
            case GrimrailDepotCreatures::CreatureSkylordTovraDragon:
                m_SkylordTovraDragonGUID = l_Creature->GetGUID();
                break;
            default:
                break;
            }
        }
    }

    void OnUnitDeath(Unit* p_Unit) override
    {
        Creature* l_Creature = p_Unit ? p_Unit->ToCreature() : nullptr;
        if (!l_Creature)
            return;

        switch (l_Creature->GetEntry())
        {
        case GrimrailDepotBosses::BossSkylordTovra:
        {
            if (l_Creature->GetMap()->IsHeroic())
                DoCompleteAchievement(GrimrailDepotAchivement::AchievementGrimrailDepotHeroic);
            else
                DoCompleteAchievement(GrimrailDepotAchivement::AchievementGrimrailDepotNormal);
            break;
        }
        default:
            break;
        }
    }

    void OnGameObjectCreate(GameObject* p_Go) override
    {
        InstanceScript::OnGameObjectCreate(p_Go);

        switch (p_Go->GetEntry())
        {
        case GrimrailDepotGobjects::GameObjectIronWroughtGate:
            m_RocketsparkAndBurkaExit = p_Go->GetGUID();
            break;
        case GrimrailDepotGobjects::GameObjectSpikedGate:
            m_RocketsparkAndBurkaEntrance = p_Go->GetGUID();
            break;
        case GrimrailDepotGobjects::GameObjectAssaultFlaps:
            m_NitroggAssaultFlapsGUID = p_Go->GetGUID();
            break;
        case GrimrailDepotGobjects::GameObjectInvisibleDoor:
            m_NitroggInvisibleDoorGUID = p_Go->GetGUID();
            break;
        case GrimrailDepotGobjects::GameObjectSpikedGateSkylordTovra:
            m_SkylordTovraDoorGUID = p_Go->GetGUID();
            break;
        default:
            break;
        }
    }

    ObjectGuid GetGuidData(uint32 DataId) const override
    {
        switch (DataId)
        {
        case GrimrailDepotData::DataRocketspark:
            return m_RocketsparkGUID;
        case GrimrailDepotData::DataBorka:
            return m_BorkaGUID;
        case GrimrailDepotData::DataNitroggThundertower:
            return m_NitrogThunderPowerGUID;
        case GrimrailDepotData::DataSkyLordTovra:
            return m_SkylordTovraGUID;
        case GrimrailDepotData::DataNitroggTurret:
            return m_NitroggTurretGUID;
        case GrimrailDepotData::DataAssaultFlaps:
            return m_NitroggAssaultFlapsGUID;
        case GrimrailDepotData::DataInvisibleDoor:
            return m_NitroggInvisibleDoorGUID;
        case GrimrailDepotData::DataSkyLordTovraDragon:
            return m_SkylordTovraDragonGUID;
        case GrimrailDepotData::DataSpikedGateSkylordTovraDoor:
            return m_SkylordTovraDoorGUID;
        case GrimrailDepotData::DataSpikedGateBoss1:
            return m_RocketsparkAndBurkaEntrance;
        case GrimrailDepotData::DataIronWroughtGate:
            return m_RocketsparkAndBurkaExit;
        default:
            break;
        }
        return ObjectGuid::Empty;
    }
};

void AddSC_instance_grimrail_depot()
{
    RegisterInstanceScript(instance_grimrail_depot, 1208);
}
