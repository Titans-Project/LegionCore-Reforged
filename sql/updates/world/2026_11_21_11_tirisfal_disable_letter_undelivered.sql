-- Tirisfal Glades, 361 "A Letter Undelivered" starts from item 2839 "A Letter to Yvette", which has no source here
-- (no creature / gameobject / item / mail loot) and none in the Cata 4.3.4 TDB, MoP 5.4.8 DB, WoD 6.2.3 DB or
-- AshamaneCore 7.3.5: the quest could never be started by a player.
-- Sources: Wowhead quest=361 "marked obsolete by Blizzard", "cannot be obtained or completed";
-- Wowhead item=2839 "This item's source is no longer available/removed".
-- Reversible: DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=361;
DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=361;
INSERT INTO `disables` (`sourceType`,`entry`,`flags`,`params_0`,`params_1`,`comment`) VALUES
(1,361,0,'','','Removed in Cataclysm (Wowhead obsolete, starter item 2839 has no source): A Letter Undelivered');
