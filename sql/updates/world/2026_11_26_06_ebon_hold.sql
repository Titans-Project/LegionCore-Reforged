-- Ebon Hold: end of the death knight starting zone, the Light of Dawn battle and an audit of its creatures
DELETE FROM `creature_queststarter` WHERE `quest` = 12779;
INSERT INTO `creature_queststarter` (`id`,`quest`) VALUES (29110,12779);
DELETE FROM `creature_questender` WHERE `id` = 25462 AND `quest` = 12779;
DELETE FROM `creature_template_addon` WHERE `entry` = 28670;
INSERT INTO `creature_template_addon` (`entry`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (28670,0,0,50331648,1,0,'53112');
DELETE FROM `spell_area` WHERE `spell` = 55952 AND `area` = 4298;
INSERT INTO `spell_area` (`spell`,`area`,`quest_start`,`quest_end`,`aura_spell`,`racemask`,`classmask`,`active_event`,`gender`,`autocast`,`quest_start_status`,`quest_end_status`) VALUES
(55952,4298,12706,12757,0,0,0,0,2,1,64,11);
UPDATE `gameobject` SET `phaseMask` = 8 WHERE `guid` IN (29978,29980,29981,29983,29984,29985,29986,29987,29988,29989,29990) AND `map` = 609;
DELETE FROM `creature_template_addon` WHERE `entry` IN (29174,29176,29177,29178,29182);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (29174,29177,29181,29182);
DELETE FROM `creature_template_addon` WHERE `entry` IN (29173,29199,29200,29204);
INSERT INTO `creature_template_addon` (`entry`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(29173,0,25279,0,1,0,''),
(29199,0,25278,0,1,0,'53627'),
(29200,0,25278,0,1,0,'53627'),
(29204,0,25278,0,1,0,'');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (25462,28488,28914,29102,29103,29133);
UPDATE `creature_template` SET `AIName` = '' WHERE `entry` = 32541;
UPDATE `creature_template` SET `faction` = 2084 WHERE `entry` IN (28788,28845);
UPDATE `creature_template` SET `unit_flags` = 32768 WHERE `entry` = 28936;
UPDATE `creature_template` SET `gossip_menu_id` = 9755 WHERE `entry` = 28943;
DELETE FROM `creature_text_locale` WHERE `CreatureID` = 29228 AND `GroupID` = 5 AND `ID` = 0 AND `Locale` = 'frFR';
INSERT INTO `creature_text_locale` (`CreatureID`,`GroupID`,`ID`,`Locale`,`Text`) VALUES
(29228,5,0,'frFR','%s devient l''ombre de son passé et s''avance vers son père.');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2546200 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2546200,9,0,0,0,0,100,0,0,0,0,0,4,14970,1,0,0,0,0,7,0,0,0,0,0,0,0,'The Lich King - On Script - Play Sound 14970'),
(2546200,9,1,0,0,0,100,0,21000,21000,0,0,4,14971,1,0,0,0,0,7,0,0,0,0,0,0,0,'The Lich King - On Script - Play Sound 14971'),
(2546200,9,2,0,0,0,100,0,26000,26000,0,0,4,14972,1,0,0,0,0,7,0,0,0,0,0,0,0,'The Lich King - On Script - Play Sound 14972');
