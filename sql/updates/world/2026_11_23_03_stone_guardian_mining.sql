-- Stone Guardian (6560): mining always gives one ore, salt or rock fragment, with the Wowhead rates and the three missing rocks
DELETE FROM `skinning_loot_template` WHERE `Entry` = 6560;
INSERT INTO `skinning_loot_template` (`Entry`, `Item`, `Currency`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
(6560, 3858, 0, 0, 20.8, 0, 1, 1, 1, 2, NULL),
(6560, 8150, 0, 0, 19, 0, 1, 1, 1, 5, NULL),
(6560, 10620, 0, 0, 22, 0, 1, 1, 1, 3, NULL),
(6560, 56046, 0, 0, 0, 0, 1, 1, 1, 3, NULL),
(6560, 56033, 0, 0, 24.5, 0, 1, 0, 1, 1, NULL),
(6560, 56036, 0, 0, 8.5, 0, 1, 0, 1, 1, NULL),
(6560, 62518, 0, 0, 2.9, 0, 1, 0, 1, 1, NULL),
(6560, 56035, 0, 0, 0.8, 0, 1, 0, 1, 1, NULL),
(6560, 62519, 0, 0, 0.8, 0, 1, 0, 1, 1, NULL);
