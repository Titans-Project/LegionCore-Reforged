-- Fel Tome of the Fel Hammer arena: one demon a day, kept by a server-side daily tracking quest
DELETE FROM `quest_template` WHERE `ID` = 900001;
DROP TEMPORARY TABLE IF EXISTS `tmp_quest_900001`;
CREATE TEMPORARY TABLE `tmp_quest_900001` SELECT * FROM `quest_template` WHERE `ID` = 44638;
UPDATE `tmp_quest_900001` SET `ID` = 900001, `Flags` = 0x1400, `FlagsEx` = 0, `LogTitle` = 'Tracking Quest: Fel Hammer - Twisting Nether demon slain today', `LogDescription` = '', `QuestDescription` = '';
INSERT INTO `quest_template` SELECT * FROM `tmp_quest_900001`;
DROP TEMPORARY TABLE `tmp_quest_900001`;
UPDATE `creature_template` SET `ScriptName` = 'npc_fel_tome_twisting_nether' WHERE `entry` = 114816;
