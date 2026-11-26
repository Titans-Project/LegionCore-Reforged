-- Portions Copyright (C) 2026 DjusikW
-- 24741 Trading Up (The Lost Isles): Raptor Egg (gameobject 201974, loot 50239) had no spawn, so the
-- quest could not be done. 5 eggs from the WoD world DB (the Cataclysm 4.3.4 TDB has 5 too, other spots),
-- in the phases of the quest giver 38122 (14336 | 16384 | 32768 | 65536). GUIDs 15130000+.
DELETE FROM `gameobject` WHERE `guid` BETWEEN 15130000 AND 15130004;
INSERT INTO `gameobject` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`PhaseId`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(15130000,201974,648,4720,4873,1,129024,'',799.529,2140.91,78.6159,-2.44926,0,0,-0.94068,0.339296,300,100,1),
(15130001,201974,648,4720,4873,1,129024,'',769.227,2231.16,64.4487,-3.03866,0,0,-0.998676,0.0514359,300,100,1),
(15130002,201974,648,4720,4873,1,129024,'',717.139,2275.15,61.6101,-2.53604,0,0,-0.954513,0.298168,300,100,1),
(15130003,201974,648,4720,4873,1,129024,'',820.748,2233.19,57.5727,1.82158,0,0,0.789989,0.61312,300,100,1),
(15130004,201974,648,4720,4873,1,129024,'',688.741,2228.18,66.8018,-1.67736,0,0,-0.743763,0.668443,300,100,1);
