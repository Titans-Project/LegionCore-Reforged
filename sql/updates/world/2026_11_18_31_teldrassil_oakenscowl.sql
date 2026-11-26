-- Portions Copyright (C) 2026 DjusikW
-- 2499 Oakenscowl (Teldrassil): the quest item 8136 drops only from Oakenscowl (2166), which had no spawn.
-- Spawn from the Cataclysm 4.3.4 TDB (same spot in the WoD world DB), wandering 10 yd. GUIDs 15130000+.
DELETE FROM `creature` WHERE `guid`=15130000;
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`PhaseId`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`MovementType`) VALUES
(15130000,2166,1,141,141,1,1,'',0,0,10469.4,1451.67,1327.55,0.128997,300,10,1);
