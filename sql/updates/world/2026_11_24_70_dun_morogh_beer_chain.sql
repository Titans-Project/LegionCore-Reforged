-- Dun Morogh 415 "Rejold's New Brew": removed in Cataclysm (Wowhead comments, absent in Cata Classic 4.4.2).
-- In Cata 413 "Shimmer Stout" follows 315 "The Perfect Stout" directly (CC 4.4.2: PrevQuestID 315), 414 follows 413.
DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=415;
INSERT INTO `disables` (`sourceType`,`entry`,`flags`,`params_0`,`params_1`,`comment`) VALUES
(1,415,0,'','','Removed in Cataclysm: Rejold''s New Brew');
UPDATE `quest_template_addon` SET `PrevQuestID`=315 WHERE `ID`=413 AND `PrevQuestID`=415;
UPDATE `quest_template` SET `RewardNextQuest`=413 WHERE `ID`=315 AND `RewardNextQuest`=0;
