-- Quest progress moved to the objective slots changed by world 2026_11_18_04; a quest that loses a counter is incomplete again
START TRANSACTION;
CREATE TEMPORARY TABLE `qo_remap` (`quest` INT UNSIGNED NOT NULL, `old_slot` TINYINT NOT NULL, `new_slot` TINYINT NULL, PRIMARY KEY (`quest`, `old_slot`));
INSERT INTO `qo_remap` VALUES (36822,0,NULL),(36822,1,1),(36822,2,2),(36822,3,3),(36822,5,5),(36822,6,6),(36822,7,7),(37585,0,NULL),(37585,1,1),(37585,2,2),(37585,3,3),(37585,5,5),(37585,6,6),(37585,7,7),(38692,0,NULL),(38720,0,0),(38720,1,1),(38720,2,NULL),(38720,3,3),(38720,4,4),(38720,5,5),(38720,6,6),(40608,1,NULL),(40608,2,NULL),(41087,0,NULL),(41087,1,1),(41087,2,4),(41087,3,5),(41087,4,2),(41626,0,NULL),(41626,1,NULL),(41626,2,2),(41926,0,NULL),(41926,1,NULL),(41926,2,NULL),(41992,0,NULL),(41992,1,NULL),(41992,2,NULL),(41992,3,3),(42006,0,NULL),(42006,1,NULL),(42006,2,2),(42377,0,0),(42377,1,1),(42377,2,NULL),(42377,3,6),(42377,4,8),(42377,5,9),(42377,6,NULL),(42476,0,NULL),(42476,1,0),(42476,2,1),(42476,3,2),(42476,4,3),(42477,0,NULL),(42477,1,8),(42477,2,NULL),(42521,0,4),(42521,1,2),(42521,2,1),(42521,3,NULL),(42920,0,0),(42920,1,4),(42920,2,NULL),(43100,0,NULL),(43100,1,0),(43100,2,2),(43100,3,3),(43100,4,4),(43100,5,5),(43100,6,6);
CREATE TEMPORARY TABLE `qo_saved` AS SELECT o.`guid`, o.`account`, o.`quest`, o.`objective`, o.`data` FROM `character_queststatus_objectives` o
 JOIN `qo_remap` m ON m.`quest` = o.`quest` AND m.`old_slot` = o.`objective`
 WHERE NOT EXISTS (SELECT 1 FROM `character_queststatus_rewarded` r WHERE r.`guid` = o.`guid` AND r.`quest` = o.`quest`);
DELETE o FROM `character_queststatus_objectives` o JOIN `qo_saved` s ON s.`guid` = o.`guid` AND s.`quest` = o.`quest` AND s.`objective` = o.`objective`;
INSERT INTO `character_queststatus_objectives` (`guid`, `account`, `quest`, `objective`, `data`)
 SELECT s.`guid`, s.`account`, s.`quest`, m.`new_slot`, s.`data` FROM `qo_saved` s JOIN `qo_remap` m ON m.`quest` = s.`quest` AND m.`old_slot` = s.`objective`
 WHERE m.`new_slot` IS NOT NULL
 ON DUPLICATE KEY UPDATE `data` = GREATEST(`character_queststatus_objectives`.`data`, VALUES(`data`));
UPDATE `character_queststatus` s SET s.`status` = 3
 WHERE s.`status` = 1 AND s.`quest` IN (SELECT DISTINCT `quest` FROM `qo_remap` WHERE `new_slot` IS NULL OR `new_slot` <> `old_slot`);
DROP TEMPORARY TABLE `qo_saved`;
DROP TEMPORARY TABLE `qo_remap`;
COMMIT;
