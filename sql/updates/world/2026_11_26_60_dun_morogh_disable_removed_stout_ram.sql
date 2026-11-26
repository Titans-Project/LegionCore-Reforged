-- Dun Morogh: 413 "Shimmer Stout", 414 "Stout to Kadrell" and 13637 "Ram Delivery" have no giver and were removed.
-- 413/414: Cata 4.3.4 and Cata Classic 4.4.2 TDB `disables` "Deprecated quest"; current Wowhead "marked obsolete by Blizzard";
-- Wowhead comment (quest=414, 2010-12-18, confirmed 2011-07-07): "Removed in Cataclysm (replaced by 26131)".
-- 13637: current Wowhead "This quest is no longer available within the game"; no starter in Cata 4.3.4, Cata Classic 4.4.2 or MoP 5.4.8.
-- 315 "The Perfect Stout" pointed to 413 (2026_11_24_70), which 1374 does not give: back to no follow-up.
DELETE FROM `disables` WHERE `sourceType`=1 AND `entry` IN (413,414,13637);
INSERT INTO `disables` (`sourceType`,`entry`,`flags`,`params_0`,`params_1`,`comment`) VALUES
(1,413,0,'','','Removed in Cataclysm (Cata/Cata Classic disables, Wowhead obsolete): Shimmer Stout'),
(1,414,0,'','','Removed in Cataclysm (Cata/Cata Classic disables, Wowhead obsolete): Stout to Kadrell'),
(1,13637,0,'','','No longer available (Wowhead), no starter in Cata/Cata Classic/MoP: Ram Delivery');
UPDATE `quest_template` SET `RewardNextQuest`=0 WHERE `ID`=315 AND `RewardNextQuest`=413;
