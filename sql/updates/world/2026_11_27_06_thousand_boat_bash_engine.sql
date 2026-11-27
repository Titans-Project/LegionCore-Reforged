-- Thousand Boat Bash: the Thousand Needles engine is loaded, so the festival hands out its boats
DELETE FROM `outdoorpvp_template` WHERE `TypeId` = 13;
INSERT INTO `outdoorpvp_template` (`TypeId`,`ScriptName`,`ZoneList`,`MapList`,`comment`) VALUES (13,'outdoorpvp_thousandneedles','400','1','Thousand Needles: Thousand Boat Bash');
