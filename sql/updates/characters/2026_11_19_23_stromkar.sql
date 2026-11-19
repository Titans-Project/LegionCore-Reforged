-- Strom'kar (41105): saved progress follows its objectives to their new slots; the slot two objectives shared is dropped
START TRANSACTION;
CREATE TEMPORARY TABLE `qo_remap` (`quest` INT UNSIGNED NOT NULL, `old_slot` TINYINT NOT NULL, `new_slot` TINYINT NULL, PRIMARY KEY (`quest`, `old_slot`));
INSERT INTO `qo_remap` VALUES (41105,0,0),(41105,1,4),(41105,2,1),(41105,3,NULL);
CREATE TEMPORARY TABLE `qo_saved` AS SELECT o.`guid`, o.`account`, o.`quest`, o.`objective`, o.`data` FROM `character_queststatus_objectives` o
 JOIN `qo_remap` m ON m.`quest` = o.`quest` AND m.`old_slot` = o.`objective`;
DELETE o FROM `character_queststatus_objectives` o JOIN `qo_saved` s ON s.`guid` = o.`guid` AND s.`quest` = o.`quest` AND s.`objective` = o.`objective`;
INSERT INTO `character_queststatus_objectives` (`guid`, `account`, `quest`, `objective`, `data`)
 SELECT s.`guid`, s.`account`, s.`quest`, m.`new_slot`, s.`data` FROM `qo_saved` s JOIN `qo_remap` m ON m.`quest` = s.`quest` AND m.`old_slot` = s.`objective`
 WHERE m.`new_slot` IS NOT NULL
 ON DUPLICATE KEY UPDATE `data` = GREATEST(`character_queststatus_objectives`.`data`, VALUES(`data`));
UPDATE `character_queststatus` SET `status` = 3 WHERE `status` = 1 AND `quest` = 41105;
DROP TEMPORARY TABLE `qo_saved`;
DROP TEMPORARY TABLE `qo_remap`;
COMMIT;
