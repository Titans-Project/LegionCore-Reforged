-- The Wandering Isle: every Pandaren class chain starts with "Much to Learn" (30039-30045, one per class); "The Lesson of
-- the Iron Bough" (30027, 30033-30038) needs it as PrevQuestID. Master Shang Xi 53566 only took it back, nothing gave
-- it, so a new Pandaren could not start the zone (found by a quest bot). The MoP 5.4.8 world DB has 53566 as starter and
-- ender of all seven; add the starters.
DELETE FROM `playercreateinfo_cast_spell` WHERE `raceMask` = 8388608 AND `spell` BETWEEN 107915 AND 107922;
DELETE FROM `creature_queststarter` WHERE `id` = 53566 AND `quest` BETWEEN 30039 AND 30045;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(53566, 30039), (53566, 30040), (53566, 30041), (53566, 30042), (53566, 30043), (53566, 30044), (53566, 30045);
