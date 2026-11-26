-- 24981 A Thorn in our Side: Urgent Scarlet Memorandum (52077) dropped at 9.5-18.8% from the Missionaries, Zealots and
-- Captain Perrine, also for players without the quest. Cata 4.3.4 TDB and Cata Classic 4.4.2 TDB: 75% / 75% / 100%, quest only.
UPDATE `creature_loot_template` SET `Chance` = 75, `QuestRequired` = 1 WHERE `Item` = 52077 AND `Entry` IN (1536, 1537);
UPDATE `creature_loot_template` SET `Chance` = 100, `QuestRequired` = 1 WHERE `Item` = 52077 AND `Entry` = 1662;
