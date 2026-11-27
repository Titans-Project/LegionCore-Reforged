-- Holidays: commoners only offer a holiday invitation during that holiday, the Desolace Flame Warden only stands by his bonfire during Midsummer
DELETE FROM `creature_queststarter` WHERE `quest` IN (11356,11970,14022);
DELETE FROM `game_event_creature_quest` WHERE `quest` IN (11356,11970,14022);
INSERT INTO `game_event_creature_quest` (`eventEntry`,`id`,`quest`) VALUES
(12,18927,11356),(12,19148,11356),(12,19172,11356),(12,19173,11356),(12,20102,11356),
(1,18927,11970),(1,19148,11970),(1,19171,11970),(1,19172,11970),(1,19173,11970),(1,20102,11970),
(26,18927,14022),(26,19148,14022),(26,19171,14022),(26,19172,14022),(26,19173,14022),(26,20102,14022);
DELETE FROM `game_event_seasonal_questrelation` WHERE `questId` IN (11356,11357,11970,11971,14022,14036);
INSERT INTO `game_event_seasonal_questrelation` (`questId`,`eventEntry`) VALUES
(11356,12),(11357,12),(11970,1),(11971,1),(14022,26),(14036,26);
DELETE FROM `game_event_creature` WHERE `guid` = 173163;
INSERT INTO `game_event_creature` (`eventEntry`,`guid`) VALUES (1,173163);
