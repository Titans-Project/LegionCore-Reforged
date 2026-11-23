-- Wandering Isle: the Wind Stone calls the Living Air, SmartAI of the starting zone NPCs
DELETE FROM `spell_script_names` WHERE `spell_id` = 106299;
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES (106299,'spell_wandering_isle_wind_stone');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (57752,57753,57873);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 57752 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(57752,0,0,0,'',1,0,100,0,5000,5000,5000,5000,0,10,509,543,511,507,0,0,1,0,0,0,0,0,0,0,0,'Quiet Lam - Out of Combat - Play Random Emote (509, 543, 511, 507)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 57753 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(57753,0,0,0,'',1,0,100,0,4000,4000,5000,5000,0,10,509,543,511,507,0,0,1,0,0,0,0,0,0,0,0,'Ironfist Zhou - Out of Combat - Play Random Emote (509, 543, 511, 507)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 57873 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(57873,0,0,0,'',25,0,100,0,0,0,0,0,0,8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Training Target - On Reset - Set Reactstate Passive'),
(57873,0,1,0,'',6,0,100,0,0,0,0,0,0,45,2,1,0,0,0,0,11,53565,3,0,0,0,0,0,0,'Training Target - On Just Died - Set Data 2 1'),
(57873,0,2,0,'',6,0,100,0,0,0,0,0,0,45,2,1,0,0,0,0,11,65469,3,0,0,0,0,0,0,'Training Target - On Just Died - Set Data 2 1'),
(57873,0,3,0,'',25,0,100,0,0,0,0,0,0,45,2,2,0,0,0,0,11,53565,3,0,0,0,0,0,0,'Training Target - On Reset - Set Data 2 2'),
(57873,0,4,0,'',25,0,100,0,0,0,0,0,0,45,2,2,0,0,0,0,11,65469,3,0,0,0,0,0,0,'Training Target - On Reset - Set Data 2 2');
DELETE FROM `creature_text` WHERE `CreatureID` = 53565 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 53565 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 65469 AND `GroupID` = 0;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(53565,0,0,'Such skill for a new trainee.',12,0,100,511,0,0,56431,'Aspiring Trainee to Training Target'),
(53565,0,1,'That target didn''t stand a chance.',12,0,100,511,0,0,56433,'Aspiring Trainee to Training Target'),
(53565,0,2,'One day you will have to teach me your secrets.',12,0,100,509,0,0,56432,'Aspiring Trainee to Training Target'),
(53565,0,3,'You are bound for a great things, trainee.',12,0,100,508,0,0,56435,'Aspiring Trainee to Training Target'),
(53565,1,0,'My money''s on Lam. Did you see him take down Chu during training last week?',12,0,100,1,0,0,56420,'Aspiring Trainee to 0'),
(65469,0,0,'Such skill for a new trainee.',12,0,100,511,0,0,56431,'Aspiring Trainee to Training Target'),
(65469,0,1,'That target didn''t stand a chance.',12,0,100,509,0,0,56433,'Aspiring Trainee to Training Target'),
(65469,0,2,'One day you will have to teach me your secrets.',12,0,100,509,0,0,56432,'Aspiring Trainee to Training Target'),
(65469,0,3,'You are bound for a great things, trainee.',12,0,100,508,0,0,56435,'Aspiring Trainee to Training Target');
