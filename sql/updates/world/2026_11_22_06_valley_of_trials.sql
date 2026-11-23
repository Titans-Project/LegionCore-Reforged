-- Valley of Trials: SmartAI of the starting zone NPCs
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (3143,3158,3287,3446,3882,7166,20102);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3143 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(3143,0,0,0,'',1,0,100,0,1000,1000,5000,10000,0,5,7,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Gornek - Out of Combat - Play Emote 7');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3158 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(3158,0,0,0,'',1,0,100,0,2000,2000,30000,30000,0,80,315800,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Duokna - Out of Combat - Run Script');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3287 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(3287,0,0,1,'',20,0,100,0,790,0,0,0,0,91,7,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hana zua - On Quest reward Sarkoth  - Set Bytes 1'),
(3287,0,1,2,'',61,0,100,0,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Hana zua - On Quest reward Sarkoth  - Store Targetlist'),
(3287,0,2,3,'',61,0,100,0,0,0,0,0,0,66,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Hana zua - On Quest reward Sarkoth  - Face Player'),
(3287,0,3,0,'',61,0,100,0,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hana zua - On Quest reward Sarkoth  - Set Phase 1'),
(3287,0,4,5,'',1,1,100,0,1000,1000,1000,1000,0,90,8,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hana zua - OOC (Phase 1)  - Set Bytes 1'),
(3287,0,5,6,'',61,0,100,0,0,0,0,0,0,1,0,60000,0,0,0,0,12,1,0,0,0,0,0,0,0,'Hana zua - OOC (Phase 1)  - Set Say Line 0'),
(3287,0,6,0,'',61,0,100,0,0,0,0,0,0,22,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hana zua - OOC (Phase 1)  - Set Phase 0'),
(3287,0,7,8,'',52,0,100,0,0,3287,0,0,0,91,8,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hana zua - On Text Over Line 0  - Set Bytes 1'),
(3287,0,8,0,'',61,0,100,0,0,0,0,0,0,90,7,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hana zua - On Text Over Line 0  - Set Bytes 1');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3446 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(3446,0,0,0,'',20,0,100,0,865,0,0,0,0,80,344600,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Mebok Mizzyrix - On Quest ''Raptor Horns'' Finished - Run Script'),
(3446,0,1,0,'',19,0,100,0,1221,0,0,0,0,80,344601,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Mebok Mizzyrix - On Quest ''Blueleaf Tubers'' Taken - Run Script');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3882 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(3882,0,0,0,'',1,0,100,0,1000,1000,21000,21000,0,80,388200,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Zlagk - Out of Combat - Run Script');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 7166 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(7166,0,0,1,'',62,0,100,0,524,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Wrenixs Gizmotronic Apparatus - On Gossip Select - Close gossip'),
(7166,0,1,0,'',61,0,100,0,0,0,0,0,0,11,9977,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Wrenixs Gizmotronic Apparatus - Cast Conjure E.C.A.C'),
(7166,0,2,3,'',62,0,100,0,524,1,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Wrenixs Gizmotronic Apparatus - On Gossip Select - Close Gossip'),
(7166,0,3,0,'',61,0,100,0,0,0,0,0,0,11,9949,1,0,0,0,0,7,0,0,0,0,0,0,0,0,'Wrenixs Gizmotronic Apparatus - Cast Thieves Tool Rack Conjure');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 20102 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(20102,0,0,0,'',1,0,100,0,3000,15000,45000,90000,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin Commoner - OOC - Say'),
(20102,0,6,0,'',1,0,100,0,3000,15000,45000,90000,0,1,6,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin Commoner - OOC - Say'),
(20102,0,8,0,'',1,0,100,0,3000,15000,45000,90000,0,1,8,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin Commoner - OOC - Say'),
(20102,0,10,0,'',64,0,100,0,0,0,0,0,0,98,8064,9964,0,0,0,0,7,0,0,0,0,0,0,0,0,'Goblin Commoner - On Gossip Hello - Send Gossip Menu'),
(20102,0,11,0,'',64,0,100,0,0,0,0,0,0,98,8939,11960,0,0,0,0,7,0,0,0,0,0,0,0,0,'Goblin Commoner - On Gossip Hello - Send Gossip'),
(20102,0,12,0,'',64,0,100,0,0,0,0,0,0,98,10513,14544,0,0,0,0,7,0,0,0,0,0,0,0,0,'Goblin Commoner - On Gossip Hello - Send Gossip'),
(20102,0,13,0,'',11,0,100,0,0,0,0,0,0,87,2010200,2010201,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin  commoner - On respawn - random action list'),
(20102,0,14,0,'',11,0,100,0,0,0,0,0,0,87,2010202,2010203,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin  commoner - On respawn - random action list'),
(20102,0,15,0,'',11,0,100,0,0,0,0,0,0,87,2010204,2010205,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin  commoner - On respawn - random action list'),
(20102,0,16,0,'',11,0,100,0,0,0,0,0,0,80,2010206,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin commoner - On respawn - random action list'),
(20102,0,17,0,'',11,0,100,0,0,0,0,0,0,87,2010207,2010208,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin  commoner - On respawn - random action list'),
(20102,0,18,0,'',11,0,100,0,0,0,0,0,0,87,2010209,2010210,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin  commoner - On respawn - random action list'),
(20102,0,19,0,'',11,0,100,0,0,0,0,0,0,3,25872,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin  commoner - On respawn - Morph');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 315800 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(315800,9,0,0,'',0,0,100,0,0,0,0,0,0,69,1,0,0,0,0,0,8,0,0,0,0,-563.079,-4217.33,42.8817,5.00283,'On Script - Move To Position'),
(315800,9,1,0,'',0,0,100,0,3000,3000,0,0,0,17,10,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'On Script - Set Emote State 10'),
(315800,9,2,0,'',0,0,100,0,2000,2000,0,0,0,1,0,0,0,0,0,0,19,9796,20,0,0,0,0,0,0,'On Script - Say Line 0'),
(315800,9,3,0,'',0,0,100,0,8000,8000,0,0,0,69,2,0,0,0,0,0,8,0,0,0,0,-565.393,-4213.93,41.661,0,'On Script - Move To Position'),
(315800,9,4,0,'',0,0,100,0,0,0,0,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'On Script - Set Emote State 0'),
(315800,9,5,0,'',0,0,100,0,3000,3000,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,5.18363,'On Script - Set Orientation 5,183630');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 344600 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(344600,9,0,0,'',0,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Mebok Mizzyrix - On Script - Say Line 0'),
(344600,9,1,0,'',0,0,100,0,2000,2000,0,0,0,11,17550,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Mebok Mizzyrix - On Script - Cast ''Purification'''),
(344600,9,2,0,'',0,0,100,0,4000,4000,0,0,0,5,34,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Mebok Mizzyrix - On Script - Play Emote 34'),
(344600,9,3,0,'',0,0,100,0,4000,4000,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Mebok Mizzyrix - On Script - Say Line 1');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 344601 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(344601,9,0,0,'',0,0,100,0,0,0,0,0,0,1,2,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Mebok Mizzyrix - On Script - Say Line 2'),
(344601,9,1,0,'',0,0,100,0,2000,2000,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-929.023,-3696.98,7.97395,1.7352,'Mebok Mizzyrix - On Script - Move To Position'),
(344601,9,2,0,'',0,0,100,0,3000,3000,0,0,0,5,25,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Mebok Mizzyrix - On Script - Play Emote 25'),
(344601,9,3,0,'',0,0,100,0,3000,3000,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-928.778,-3697.25,7.99705,6.17741,'Mebok Mizzyrix - On Script - Move To Position'),
(344601,9,4,0,'',0,0,100,0,1000,1000,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-928.982,-3697.23,8.06642,3.08923,'Mebok Mizzyrix - On Script - Move To Position');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 388200 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(388200,9,0,0,'',0,0,100,0,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0.577578,'On Script - Set Orientation 0.577578'),
(388200,9,1,0,'',0,0,100,0,1000,1000,0,0,0,17,234,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'On Script - Set Emote State 234'),
(388200,9,2,0,'',0,0,100,0,9000,9000,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,3.30292,'On Script - Set Orientation 3,302916'),
(388200,9,3,0,'',0,0,100,0,0,0,0,0,0,17,69,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'On Script - Set Emote State 69');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2010200 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2010200,9,0,0,'',0,0,100,0,0,0,0,0,0,11,34845,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin  commoner - random action list - cast ');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2010201 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2010201,9,0,0,'',0,0,100,0,0,0,0,0,0,11,34849,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin  commoner - random action list - cast');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2010202 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2010202,9,0,0,'',0,0,100,0,0,0,0,0,0,11,43631,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin  commoner - random action list - cast ');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2010203 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2010203,9,0,0,'',0,0,100,0,0,0,0,0,0,11,43630,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin  commoner - random action list - cast');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2010204 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2010204,9,0,0,'',0,0,100,0,0,0,0,0,0,11,34848,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin  commoner - random action list - cast ');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2010205 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2010205,9,0,0,'',0,0,100,0,0,0,0,0,0,11,34851,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin  commoner - random action list - cast');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2010206 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2010206,9,0,0,'',0,0,100,0,0,0,0,0,0,11,43911,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin commoner - random action list - cast '),
(2010206,9,1,0,'',0,0,100,0,0,0,0,0,0,71,0,0,13861,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin commoner - random action list - Equip');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2010207 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2010207,9,0,0,'',0,0,100,0,0,0,0,0,0,3,0,29203,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin  commoner - random action list - Morph ');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2010208 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2010208,9,0,0,'',0,0,100,0,0,0,0,0,0,3,0,29204,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin  commoner - random action list - Morph');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2010209 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2010209,9,0,0,'',0,0,100,0,0,0,0,0,0,3,0,25036,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin  commoner - random action list - Morph ');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2010210 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2010210,9,0,0,'',0,0,100,0,0,0,0,0,0,3,0,25047,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin  commoner - random action list - Morph');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 1 AND `SourceEntry` = 20102 AND `SourceId` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 7 AND `SourceEntry` = 20102 AND `SourceId` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 9 AND `SourceEntry` = 20102 AND `SourceId` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 11 AND `SourceEntry` = 20102 AND `SourceId` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 12 AND `SourceEntry` = 20102 AND `SourceId` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 13 AND `SourceEntry` = 20102 AND `SourceId` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 14 AND `SourceEntry` = 20102 AND `SourceId` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 15 AND `SourceEntry` = 20102 AND `SourceId` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 16 AND `SourceEntry` = 20102 AND `SourceId` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 17 AND `SourceEntry` = 20102 AND `SourceId` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 18 AND `SourceEntry` = 20102 AND `SourceId` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 19 AND `SourceEntry` = 20102 AND `SourceId` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 20 AND `SourceEntry` = 20102 AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(22,1,20102,0,0,12,1,7,0,0,0,0,'','Commoner - Luna Festival must be active'),
(22,7,20102,0,0,12,1,12,0,0,0,0,'','Commoner - Hallows End must be active'),
(22,9,20102,0,0,12,1,2,0,0,0,0,'','Commoner - Winterveil must be active'),
(22,11,20102,0,0,12,1,7,0,0,0,0,'','Commoner - Luna Festival must be active'),
(22,12,20102,0,0,12,1,12,0,0,0,0,'','Commoner - Hallows End must be active'),
(22,13,20102,0,0,12,1,51,0,0,0,0,'','Commoner - DOTD must be active'),
(22,14,20102,0,0,12,1,2,0,0,0,0,'','Commoner - Winter Veil must be active'),
(22,15,20102,0,0,12,1,12,0,0,0,0,'','Commoner - Hallow''s End must be active'),
(22,16,20102,0,0,12,1,7,0,0,0,0,'','Commoner - Lunar Festival must be active'),
(22,17,20102,0,0,12,1,24,0,0,0,0,'','Commoner - Brew Fest must be active'),
(22,18,20102,0,0,12,1,51,0,0,0,0,'','Commoner - DOTD must be active'),
(22,19,20102,0,0,12,1,50,0,0,0,0,'','Commoner - Pirates Day must be active'),
(22,20,20102,0,0,12,1,1,0,0,0,0,'','Commoner - Midsummer Fire Festival must be active');
DELETE FROM `creature_text` WHERE `CreatureID` = 20102 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 20102 AND `GroupID` = 6;
DELETE FROM `creature_text` WHERE `CreatureID` = 20102 AND `GroupID` = 8;
DELETE FROM `creature_text` WHERE `CreatureID` = 3287 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 3446 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 3446 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 3446 AND `GroupID` = 2;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(3287,0,0,'Go swiftly, $n. My fate is in your hands.',12,1,100,0,0,0,1341,'Hana zua'),
(3446,0,0,'Now let''s try it...',12,0,100,7,0,0,2078,'Mebok Mizzyrix on Quest 865 rewarded'),
(3446,1,0,'Ugh!  That''s terrible!',12,0,100,33,0,0,2079,'Mebok Mizzyrix on Quest 865 rewarded'),
(3446,2,0,'Hey $n!  Don''t forget to grab a crate, a manual and a command stick!',12,0,100,0,0,0,1788,'Mebok Mizzyrix on Quest 1221 taken'),
(20102,0,0,'During the Lunar Festival, we should take time to seek out our elders and consider their wisdom.',12,0,100,0,0,0,16466,'Goblin Commoner to Goblin Commoner'),
(20102,0,1,'Legend says that the great beast Omen sleeps in the waters of Lake Elune''ara in Moonglade, only to appear once a year. But that''s just a legend.',12,0,100,0,0,0,16465,'Goblin Commoner to Goblin Commoner'),
(20102,0,2,'Have you seen the fireworks? This is the best time of year to buy them.',12,0,100,0,0,0,16467,'Goblin Commoner to Goblin Commoner'),
(20102,0,3,'The druids of Nighthaven are holding a great celebration in Moonglade for the Lunar Festival.',12,0,100,0,0,0,16468,'Goblin Commoner to Goblin Commoner'),
(20102,6,0,'We should go trick or treating later.',12,0,100,0,0,0,23286,'Goblin Commoner to Goblin Commoner'),
(20102,6,1,'There MUST be a way to make more money off of this holiday.',12,0,100,0,0,0,23299,'Goblin Commoner to Goblin Commoner'),
(20102,6,2,'The innkeepers are mad to be giving away treats for free.',12,0,100,0,0,0,23300,'Goblin Commoner to Goblin Commoner'),
(20102,6,3,'For the Alliance! Wait... the Horde! Wait... which was I again?',12,0,100,4,0,149,23364,'Goblin Commoner to Goblin Commoner'),
(20102,8,0,'Where are all those wonderful winter hats coming from?',12,0,100,0,0,0,16503,'Goblin Commoner to Goblin Commoner'),
(20102,8,1,'Ah, the Winter Wondervolt. What will we think of next?',12,0,100,0,0,0,16501,'Goblin Commoner to Goblin Commoner'),
(20102,8,2,'The goblins of Smokywood Pastures always have such wonderful treats for sale this time of year.',12,0,100,0,0,0,16463,'Goblin Commoner to Goblin Commoner'),
(20102,8,3,'Ah, the Winter Wondervolt. What will we think of next?',12,0,100,274,0,0,16501,'Goblin Commoner to Goblin Commoner');
