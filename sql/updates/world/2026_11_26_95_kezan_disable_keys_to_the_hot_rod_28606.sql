-- Kezan: 28606 "The Keys to the Hot Rod" (spare keys if 46856 is lost) has no
-- quest giver anywhere: none in Cata 4.3.4 TDB, Cata Classic 4.4.2 TDB or the MoP
-- 5.4.8 world DB; Wowhead (current, /cata and the 2013-09-07 Wayback snapshot)
-- lists no start or end NPC, no description and no comments. Never obtainable
-- on live servers -> disable it, as with the other giverless quests.
DELETE FROM `disables` WHERE `sourceType` = 1 AND `entry` = 28606;
INSERT INTO `disables` (`sourceType`,`entry`,`flags`,`params_0`,`params_1`,`comment`) VALUES
(1,28606,0,'','','Never obtainable (no giver in Cata 4.3.4/4.4.2/MoP 5.4.8, Wowhead + Wayback 2013 no start): The Keys to the Hot Rod');
