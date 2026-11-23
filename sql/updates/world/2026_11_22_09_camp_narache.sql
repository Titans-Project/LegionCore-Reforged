-- Camp Narache: SmartAI of the starting zone NPCs
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (3054);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3054 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(3054,0,1,0,'',20,0,100,0,771,0,0,0,0,80,305400,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Zarlman Two-Moons - On Quest ''Rite of Vision'' Finished - Run Script');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 305400 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(305400,9,0,0,'',0,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Zarlman Two-Moons - On Script - Say Line 0'),
(305400,9,1,0,'',0,0,100,0,1000,1000,0,0,0,11,5026,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Zarlman Two-Moons - On Script - Cast ''Create Water of the Seers'''),
(305400,9,2,0,'',0,0,100,0,7000,7000,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Zarlman Two-Moons - On Script - Say Line 1');
DELETE FROM `creature_text` WHERE `CreatureID` = 3054 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 3054 AND `GroupID` = 1;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(3054,0,0,'%s begins chanting as he mixes the well stones and ambercorn before the Tribal Fire.',16,0,100,0,0,0,1011,'Zarlman Two-Moons on Quest 771 finished 1'),
(3054,1,0,'The Water of the Seers is ready for your consumption, $n.',12,1,100,0,0,0,1012,'Zarlman Two-Moons on Quest 771 finished 2');
