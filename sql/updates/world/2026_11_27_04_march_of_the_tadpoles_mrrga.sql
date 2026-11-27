-- March of the Tadpoles: Mrrga, the orca of The Ways of the World, swims in the Westrift during the festival
DELETE FROM `creature` WHERE `guid` = 9200101;
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`PhaseId`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`MovementType`) VALUES
(9200101,118662,571,3537,4099,1,1,'',4419.4,6187.2,-10,0,300,15,1);
DELETE FROM `game_event_creature` WHERE `guid` = 9200101;
INSERT INTO `game_event_creature` (`eventEntry`,`guid`) VALUES (305,9200101);
