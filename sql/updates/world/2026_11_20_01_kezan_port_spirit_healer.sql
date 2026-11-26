-- Kezan: the Port of Bilgewater graveyard (ghost appears at -8122.6 1519.7 12.6) had no Spirit Healer; the only one
-- (guid 190430) stands 355 yd away and 80 yd higher. Ghosts that could not reach their corpse were stuck there.
-- New spirit healer in front of the graveyard fence, facing the arriving ghost; other fields as guid 190430.
DELETE FROM `creature` WHERE `guid` = 15130001;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`,
    `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`,
    `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`)
SELECT 15130001, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`,
    -8115.5, 1519.5, 10.3, 3.14, `spawntimesecs`, 0, 0, `curhealth`, `curmana`,
    0, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`
FROM `creature` WHERE `guid` = 190430;
