-- Teldrassil phial chain: 929 -> 7383 -> 933 -> 14005 -> 935 (Wowhead series 7383, 933, 14005, 935, 14039; Cata 4.3.4 TDB 933.PrevQuestID = 7383).
-- Here 933 came before 7383 while 7383 still had RewardNextQuest = 933, so after 933 the quest 7383 could no longer be taken (next chain already done).
UPDATE `quest_template_addon` SET `NextQuestID` = 7383 WHERE `ID` = 929;
UPDATE `quest_template_addon` SET `PrevQuestID` = 929,  `NextQuestID` = 933   WHERE `ID` = 7383;
UPDATE `quest_template_addon` SET `PrevQuestID` = 7383, `NextQuestID` = 14005 WHERE `ID` = 933;
UPDATE `quest_template_addon` SET `PrevQuestID` = 14005 WHERE `ID` = 935;
