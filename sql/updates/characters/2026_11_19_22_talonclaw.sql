-- Talonclaw: Dakarr's death used to credit Return to Dalaran and the spear claim was never saved; a hunter holding the spear gets the claim, one without it goes back to the claim step
START TRANSACTION;
INSERT INTO `character_queststatus_objectives` (`guid`, `account`, `quest`, `objective`, `data`)
SELECT s.`guid`, s.`account`, 40385, 1, 1 FROM `character_queststatus` s
WHERE s.`quest` = 40385 AND s.`status` IN (1, 3)
  AND EXISTS (SELECT 1 FROM `item_instance` i WHERE i.`owner_guid` = s.`guid` AND i.`itemEntry` = 128808)
  AND NOT EXISTS (SELECT 1 FROM `character_queststatus_rewarded` r WHERE r.`guid` = s.`guid` AND r.`quest` = 40385)
ON DUPLICATE KEY UPDATE `data` = GREATEST(`character_queststatus_objectives`.`data`, 1);

DELETE o FROM `character_queststatus_objectives` o
JOIN `character_queststatus` s ON s.`guid` = o.`guid` AND s.`quest` = o.`quest`
WHERE o.`quest` = 40385 AND o.`objective` = 2 AND s.`status` IN (1, 3)
  AND NOT EXISTS (SELECT 1 FROM `item_instance` i WHERE i.`owner_guid` = o.`guid` AND i.`itemEntry` = 128808)
  AND NOT EXISTS (SELECT 1 FROM `character_queststatus_rewarded` r WHERE r.`guid` = o.`guid` AND r.`quest` = 40385);

UPDATE `character_queststatus` s SET s.`status` = 1
WHERE s.`quest` = 40385 AND s.`status` = 3
  AND NOT EXISTS (SELECT 1 FROM `item_instance` i WHERE i.`owner_guid` = s.`guid` AND i.`itemEntry` = 128808)
  AND NOT EXISTS (SELECT 1 FROM `character_queststatus_rewarded` r WHERE r.`guid` = s.`guid` AND r.`quest` = 40385);
COMMIT;
