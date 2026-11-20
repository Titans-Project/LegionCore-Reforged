-- Order halls: the acolytes added by the repack no longer offer the campaign quests; Odyn offers his two who had no one else
DELETE FROM `creature_queststarter` WHERE `id` BETWEEN 400001 AND 400012;
DELETE FROM `creature_queststarter` WHERE `id` = 96469 AND `quest` IN (43586, 44667);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(96469, 43586),
(96469, 44667);
