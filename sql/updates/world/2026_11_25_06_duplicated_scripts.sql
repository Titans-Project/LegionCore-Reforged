-- Old script tables: every row was stored up to 278 times, so each command ran as many times
DROP TEMPORARY TABLE IF EXISTS `tmp_quest_end_scripts`;
CREATE TEMPORARY TABLE `tmp_quest_end_scripts` AS SELECT DISTINCT * FROM `quest_end_scripts`;
DELETE FROM `quest_end_scripts`;
INSERT INTO `quest_end_scripts` SELECT * FROM `tmp_quest_end_scripts`;
DROP TEMPORARY TABLE `tmp_quest_end_scripts`;
DROP TEMPORARY TABLE IF EXISTS `tmp_quest_start_scripts`;
CREATE TEMPORARY TABLE `tmp_quest_start_scripts` AS SELECT DISTINCT * FROM `quest_start_scripts`;
DELETE FROM `quest_start_scripts`;
INSERT INTO `quest_start_scripts` SELECT * FROM `tmp_quest_start_scripts`;
DROP TEMPORARY TABLE `tmp_quest_start_scripts`;
DROP TEMPORARY TABLE IF EXISTS `tmp_spell_scripts`;
CREATE TEMPORARY TABLE `tmp_spell_scripts` AS SELECT DISTINCT * FROM `spell_scripts`;
DELETE FROM `spell_scripts`;
INSERT INTO `spell_scripts` SELECT * FROM `tmp_spell_scripts`;
DROP TEMPORARY TABLE `tmp_spell_scripts`;
