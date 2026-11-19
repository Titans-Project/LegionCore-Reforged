-- The Netherlight Temple (40938): priests already on the quest keep "Follow Velen to the portal" done (it used to ask for nothing)
INSERT INTO `character_queststatus_objectives` (`guid`, `account`, `quest`, `objective`, `data`)
 SELECT `guid`, `account`, `quest`, 0, 1 FROM `character_queststatus` WHERE `quest` = 40938
 ON DUPLICATE KEY UPDATE `data` = 1;
