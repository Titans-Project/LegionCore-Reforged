-- Coldridge Valley: SmartAI of the starting zone NPCs, Milo and his gyro-copter hidden until Follow that Gyro-Copter! is accepted
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (704,705,706,836,946,1125,1243,6782,10610,10611,10803,10804,10805,37115,37507,41478,42221,42222,42286);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 704 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(704,0,0,1,'',1,0,10,0,30000,600000,120000,600000,0,4,1018,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Ragged Timber Wolf - Out of Combat - Play Sound 1018'),
(704,0,1,0,'',61,0,100,0,30000,600000,120000,600000,0,5,393,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Ragged Timber Wolf - Out of Combat - Play Emote 393');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 705 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(705,0,0,1,'',1,0,10,0,30000,600000,120000,600000,0,4,1018,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Ragged Young Wolf - Out of Combat - Play Sound 1018'),
(705,0,1,0,'',61,0,100,0,30000,600000,120000,600000,0,5,393,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Ragged Young Wolf - Out of Combat - Play Emote 393');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 706 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(706,0,0,0,'',4,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Frostmane Troll Whelp - On Aggro - Say 0 (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 836 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(836,0,0,0,'',20,0,100,0,3364,0,0,0,0,80,83600,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Durnan Furcutter - On Quest ''Scalding Mornbrew Delivery'' Finished - Run Script');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 946 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(946,0,0,0,'',4,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Frostmane Novice - On Aggro - Say 0 (No Repeat)'),
(946,0,1,0,'',0,0,100,0,0,0,3000,4000,0,11,6949,64,0,0,0,0,2,0,0,0,0,0,0,0,0,'Frostmane Novice - In Combat - Cast ''Weak Frostbolt''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1125 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1125,0,0,0,'',4,0,100,1,0,0,0,0,0,11,6268,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Crag Boar - On Aggro - Cast ''Rushing Charge'' (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1243 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1243,0,0,1,'',20,0,100,0,5541,0,0,0,0,45,1,0,0,0,0,0,19,10610,20,0,0,0,0,0,0,'Hegnar Rumbleshot - On Quest ''Ammo for Rumbleshot'' Finished - Set Data 1 0'),
(1243,0,1,2,'',61,0,100,0,5541,0,0,0,0,45,1,0,0,0,0,0,19,10611,20,0,0,0,0,0,0,'Hegnar Rumbleshot - On Quest ''Ammo for Rumbleshot'' Finished - Set Data 1 0'),
(1243,0,2,3,'',61,0,100,0,5541,0,0,0,0,45,1,0,0,0,0,0,19,10804,20,0,0,0,0,0,0,'Hegnar Rumbleshot - On Quest ''Ammo for Rumbleshot'' Finished - Set Data 1 0'),
(1243,0,3,4,'',61,0,100,0,5541,0,0,0,0,45,1,0,0,0,0,0,19,10805,20,0,0,0,0,0,0,'Hegnar Rumbleshot - On Quest ''Ammo for Rumbleshot'' Finished - Set Data 1 0'),
(1243,0,4,0,'',61,0,100,0,5541,0,0,0,0,45,1,0,0,0,0,0,19,10803,20,0,0,0,0,0,0,'Hegnar Rumbleshot - On Quest ''Ammo for Rumbleshot'' Finished - Set Data 1 0'),
(1243,0,5,0,'',38,0,100,0,2,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,3.68522,'Hegnar Rumbleshot - On Data Set 2 0 - Set Orientation 3,68522'),
(1243,0,6,0,'',38,0,100,0,3,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0.0349066,'Hegnar Rumbleshot - On Data Set 3 0 - Set Orientation 0.03490658');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 6782 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(6782,0,0,0,'',20,0,100,0,24490,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Hands Springsprocket - On Quest Reward - Say 0'),
(6782,0,1,0,'',19,0,100,0,24491,0,0,0,0,11,70047,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Hands Springsprocket - On Quest Accept - Cast ''Follow That Gyro-Copter - Quest Start''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 10610 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(10610,0,0,1,'',38,0,100,0,1,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Data Set 1 0 - Set Active On'),
(10610,0,1,2,'',61,0,100,0,1,0,0,0,0,53,0,10610,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Data Set 1 0 - Start Waypoint'),
(10610,0,2,0,'',61,0,100,0,1,0,0,0,0,4,6598,0,0,0,0,0,21,50,0,0,0,0,0,0,0,'Angus - On Data Set 1 0 - Play Sound 6598'),
(10610,0,3,4,'',40,0,100,0,4,10610,0,0,0,54,250000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Waypoint 4 Reached - Pause Waypoint'),
(10610,0,4,0,'',61,0,100,0,4,10610,0,0,0,80,1061000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Waypoint 4 Reached - Run Script'),
(10610,0,5,0,'',38,0,100,0,2,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-6009.25,-202.65,406.983,2,'Shorty - On Data Set 2 0 - Move To Position'),
(10610,0,6,0,'',38,0,100,0,3,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,1.58825,'Shorty - On Data Set 3 0 - Set Orientation 1.58825');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 10611 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(10611,0,0,1,'',38,0,100,0,1,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-6031.59,-228.182,414.404,2,'Shorty - On Data Set 1 0 - Move To Position'),
(10611,0,1,0,'',61,0,100,0,1,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Shorty - On Data Set 1 0 - Set Active On'),
(10611,0,2,0,'',38,0,100,0,2,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,2.18492,'Shorty - On Data Set 2 0 - Set Orientation 2,18492'),
(10611,0,3,0,'',38,0,100,0,3,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,2.84489,'Shorty - On Data Set 3 0 - Set Orientation 2.844887'),
(10611,0,4,0,'',38,0,100,0,4,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,4.54215,'Shorty - On Data Set 4 0 - Set Orientation 4.54215'),
(10611,0,5,0,'',38,0,100,0,5,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,2.84489,'Shorty - On Data Set 4 0 - Set Orientation 2.844887'),
(10611,0,6,0,'',38,0,100,0,6,0,0,0,0,11,18655,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Shorty - On Data Set 6 0 - Cast ''Mortar Animate'''),
(10611,0,7,0,'',38,0,100,0,7,0,0,0,0,11,16786,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Shorty - On Data Set 7 0 - Cast ''Mortar Shot'''),
(10611,0,8,0,'',38,0,100,0,8,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,4.54215,'Shorty - On Data Set 8 0 - Set Orientation 4.54215'),
(10611,0,9,0,'',38,0,100,0,9,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,4.54215,'Shorty - On Data Set 9 0 - Set Orientation 4.54215'),
(10611,0,10,0,'',38,0,100,0,10,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,2.84489,'Shorty - On Data Set 10 0 - Set Orientation 2.844887'),
(10611,0,11,0,'',38,0,100,0,11,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-6007.46,-202.158,407.448,2,'Shorty - On Data Set 11 0 - Move To Position'),
(10611,0,12,0,'',38,0,100,0,12,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,2.47837,'Shorty - On Data Set 12 0 - Set Orientation 2.478368');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 10803 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(10803,0,0,1,'',38,0,100,0,1,0,0,0,0,53,0,10803,0,0,0,0,1,0,0,0,0,0,0,0,0,'Wheeler - On Data Set 1 0 - Start Waypoint'),
(10803,0,1,0,'',61,0,100,0,1,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Wheeler - On Data Set 1 0 - Set Active On'),
(10803,0,2,0,'',38,0,100,0,2,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,2.44346,'Wheeler - On Data Set 2 0 - Set Orientation 2,443461'),
(10803,0,3,0,'',38,0,100,0,3,0,0,0,0,11,16767,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Wheeler - On Data Set 3 0 - Cast ''Shoot'''),
(10803,0,4,0,'',38,0,100,0,4,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,4.00565,'Wheeler - On Data Set 4 0 - Set Orientation 4.00565'),
(10803,0,5,0,'',38,0,100,0,5,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,2.44346,'Wheeler - On Data Set 5 0 - Set Orientation 2.443461'),
(10803,0,6,0,'',38,0,100,0,6,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,4.00565,'Wheeler - On Data Set 6 0 - Set Orientation 4.00565'),
(10803,0,7,0,'',38,0,100,0,7,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,2.44346,'Wheeler - On Data Set 7 0 - Set Orientation 2.443461'),
(10803,0,8,0,'',38,0,100,0,8,0,0,0,0,11,16775,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Wheeler - On Data Set 8 0 - Cast ''Shoot'''),
(10803,0,9,0,'',38,0,100,0,9,0,0,0,0,11,16778,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Wheeler - On Data Set 9 0 - Cast ''Shoot'''),
(10803,0,10,0,'',38,0,100,0,10,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,4.00565,'Wheeler - On Data Set 10 0 - Set Orientation 4.00565'),
(10803,0,11,0,'',38,0,100,0,11,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,2.44346,'Wheeler - On Data Set 11 0 - Set Orientation 4.00565'),
(10803,0,12,0,'',38,0,100,0,12,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,4.00565,'Wheeler - On Data Set 12 0 - Set Orientation 4.00565'),
(10803,0,13,0,'',38,0,100,0,13,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,4.77803,'Wheeler - On Data Set 13 0 - Set Orientation 4.77803'),
(10803,0,14,0,'',38,0,100,0,14,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,3.01942,'Wheeler - On Data Set 14 0 - Set Orientation 3.01942'),
(10803,0,15,0,'',38,0,100,0,15,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-6008.53,-199.145,406.687,4.37218,'Wheeler - On Data Set 15 0 - Move To Position'),
(10803,0,16,0,'',38,0,100,0,16,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,4.37218,'Wheeler - On Data Set 16 0 - Set Orientation 4.372177');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 10804 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(10804,0,0,1,'',38,0,100,0,1,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-6034.14,-223.333,413.4,1,'Rifleman Middlecamp - On Data Set 1 0 - Move To Position'),
(10804,0,1,0,'',61,0,100,0,1,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rifleman Middlecamp - On Data Set 1 0 - Set Active On'),
(10804,0,2,0,'',38,0,100,0,2,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,1.88496,'Rifleman Middlecamp - On Data Set 2 0 - Set Orientation 1,884956'),
(10804,0,3,0,'',38,0,100,0,3,0,0,0,0,11,16768,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rifleman Middlecamp - On Data Set 3 0 - Cast ''Shoot'''),
(10804,0,4,0,'',38,0,100,0,4,0,0,0,0,11,16772,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rifleman Middlecamp - On Data Set 4 0 - Cast ''Shoot'''),
(10804,0,5,0,'',38,0,100,0,5,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0.810652,'Rifleman Middlecamp - On Data Set 5 0 - Set Orientation 0.810652'),
(10804,0,6,0,'',38,0,100,0,6,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,1.88496,'Rifleman Middlecamp - On Data Set 6 0 - Set Orientation 1.884956'),
(10804,0,7,0,'',38,0,100,0,7,0,0,0,0,11,16777,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rifleman Middlecamp - On Data Set 7 0 - Cast ''Shoot'''),
(10804,0,8,0,'',38,0,100,0,8,0,0,0,0,11,16780,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rifleman Middlecamp - On Data Set 8 0 - Cast ''Shoot'''),
(10804,0,9,0,'',38,0,100,0,9,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0.810652,'Rifleman Middlecamp - On Data Set 9 0 - Set Orientation 0.810652'),
(10804,0,10,0,'',38,0,100,0,10,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,1.88496,'Rifleman Middlecamp - On Data Set 10 0 - Set Orientation 1.884956'),
(10804,0,11,0,'',38,0,100,0,11,0,0,0,0,11,16572,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rifleman Middlecamp - On Data Set 11 0 - Cast ''Shoot'''),
(10804,0,12,0,'',38,0,100,0,12,0,0,0,0,11,16776,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rifleman Middlecamp - On Data Set 12 0 - Cast ''Shoot'''),
(10804,0,13,0,'',38,0,100,0,13,0,0,0,0,11,16779,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rifleman Middlecamp - On Data Set 13 0 - Cast ''Shoot'''),
(10804,0,14,0,'',38,0,100,0,14,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0.810652,'Rifleman Middlecamp - On Data Set 14 0 - Set Orientation 0.810652'),
(10804,0,15,0,'',38,0,100,0,15,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,5.13852,'Rifleman Middlecamp - On Data Set 15 0 - Set Orientation 5.13852'),
(10804,0,16,0,'',38,0,100,0,16,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,2.9147,'Rifleman Middlecamp - On Data Set 16 0 - Set Orientation 2.9147'),
(10804,0,17,1,'',38,0,100,0,17,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-6010.67,-199.434,406.792,5.53269,'Rifleman Middlecamp - On Data Set 17 0 - Move To Position'),
(10804,0,18,0,'',38,0,100,0,18,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,5.53269,'Rifleman Middlecamp - On Data Set 18 0 - Set Orientation 5.532694');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 10805 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(10805,0,0,1,'',38,0,100,0,1,0,0,0,0,53,0,10805,0,0,0,0,1,0,0,0,0,0,0,0,0,'Klemmy - On Data Set 1 0 - Start Waypoint'),
(10805,0,1,0,'',61,0,100,0,1,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Klemmy - On Data Set 1 0 - Set Active On'),
(10805,0,2,0,'',38,0,100,0,2,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,5.59936,'Spotter Klemmy - On Data Set 2 0 - Set Orientation 5,59936'),
(10805,0,3,0,'',38,0,100,0,3,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-6077.43,-213.257,424.05,5.59936,'Spotter Klemmy - On Data Set 3 0 - Move To Position'),
(10805,0,4,0,'',38,0,100,0,4,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-6065.9,-202.298,424.369,5,'Spotter Klemmy - On Data Set 4 0 - Move To Position'),
(10805,0,5,0,'',38,0,100,0,5,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,5.59936,'Spotter Klemmy - On Data Set 5 0 - Set Orientation 5.59936'),
(10805,0,6,0,'',38,0,100,0,6,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,4.0985,'Spotter Klemmy - On Data Set 6 0 - Set Orientation 4.0985'),
(10805,0,7,0,'',38,0,100,0,7,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,4.0985,'Spotter Klemmy - On Data Set 7 0 - Set Orientation 4.0985'),
(10805,0,8,0,'',38,0,100,0,8,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-6077.43,-213.257,424.05,5.59936,'Spotter Klemmy - On Data Set 8 0 - Move To Position'),
(10805,0,9,0,'',38,0,100,0,9,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,2.84489,'Spotter Klemmy - On Data Set 9 0 - Set Orientation 2.844887'),
(10805,0,10,0,'',38,0,100,0,10,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-6071.88,-212.831,422.929,5,'Spotter Klemmy - On Data Set 10 0 - Move To Position'),
(10805,0,11,0,'',38,0,100,0,11,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,6.04228,'Spotter Klemmy - On Data Set 11 0 - Set Orientation 6.04228'),
(10805,0,12,0,'',38,0,100,0,12,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-6065.9,-202.298,424.369,5,'Spotter Klemmy - On Data Set 12 0 - Move To Position'),
(10805,0,13,0,'',38,0,100,0,13,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,4.0985,'Spotter Klemmy - On Data Set 13 0 - Set Orientation 4.0985'),
(10805,0,14,0,'',38,0,100,0,14,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-6064.07,-209.851,421.98,6.10275,'Spotter Klemmy - On Data Set 14 0 - Move To Position'),
(10805,0,15,0,'',38,0,100,0,15,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-6061.97,-218.956,421.085,5.92054,'Spotter Klemmy - On Data Set 15 0 - Move To Position'),
(10805,0,16,0,'',38,0,100,0,16,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-6040.96,-221.929,414.921,5.92054,'Spotter Klemmy - On Data Set 16 0 - Move To Position'),
(10805,0,17,0,'',38,0,100,0,17,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-6007.3,-200.348,406.777,3.28222,'Spotter Klemmy - On Data Set 17 0 - Move To Position'),
(10805,0,18,0,'',38,0,100,0,18,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,3.28222,'Spotter Klemmy - On Data Set 18 0 - Set Orientation 3.282219');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 37115 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(37115,0,0,0,'',1,0,100,0,30000,30000,120000,120000,0,11,78159,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Teo Hammerstorm - OOC - Cast ''Windfury Totem'''),
(37115,0,1,0,'',1,0,100,0,60000,60000,120000,120000,0,11,79429,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Teo Hammerstorm - OOC - Cast ''Elemental Resistance Totem'''),
(37115,0,2,0,'',1,0,100,0,90000,90000,120000,120000,0,11,79431,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Teo Hammerstorm - OOC - Cast ''Flametongue Totem''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 37507 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(37507,0,0,0,'',4,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Frostmane Blade - On Aggro - Say 0 (No Repeat)'),
(37507,0,1,0,'',9,0,100,0,0,5,20000,30000,0,11,70133,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Frostmane Blade - Range 5 yards - Cast ''Poisoned Blade''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 41478 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(41478,0,0,1,'',1,0,10,0,30000,600000,120000,600000,0,4,1018,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Snow Tracker Wolf - Out of Combat - Play Sound 1018'),
(41478,0,1,0,'',61,0,100,0,30000,600000,120000,600000,0,5,393,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Snow Tracker Wolf - Out of Combat - Play Emote 393'),
(41478,0,2,0,'',0,0,100,0,9000,10000,20000,25000,0,11,32918,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Snow Tracker Wolf - In Combat - Cast ''Chilling Howl''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42221 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42221,0,0,0,'',4,0,100,0,0,0,0,0,0,28,77831,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rockjaw Bonepicker - On Aggro - Cast ''Bone Toss'''),
(42221,0,1,0,'',0,0,100,0,0,0,3000,4000,0,11,82625,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Rockjaw Bonepicker - Combat - Cast ''Bone Toss''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42222 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42222,0,0,0,'',0,0,100,0,3500,4500,12000,13000,0,11,69851,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Rockjaw Marauder - In Combat - Cast ''Club''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42286 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42286,0,0,0,'',4,0,100,1,0,0,0,0,0,11,32720,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Young Snow Leopard - On Aggro - Cast ''Sprint'' (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 83600 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(83600,9,0,0,'',0,0,100,0,2000,2000,0,0,0,1,0,2000,0,0,0,0,7,0,0,0,0,0,0,0,0,'Durnan Furcutter - On Script - Say Line 0'),
(83600,9,1,0,'',0,0,100,0,0,0,0,0,0,5,7,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Durnan Furcutter - On Script - Play Emote 7'),
(83600,9,2,0,'',0,0,100,0,4000,4000,0,0,0,1,1,12000,0,0,0,0,7,0,0,0,0,0,0,0,0,'Durnan Furcutter - On Script - Say Line 0'),
(83600,9,3,0,'',0,0,100,0,0,0,0,0,0,5,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Durnan Furcutter - On Script - Play Emote 0');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1061000 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1061000,9,0,0,'',0,0,100,0,0,0,0,0,0,45,2,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'On Script - Set Data 2 0'),
(1061000,9,1,0,'',0,0,100,0,0,0,0,0,0,45,2,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'On Script - Set Data 2 0'),
(1061000,9,2,0,'',0,0,100,0,0,0,0,0,0,45,2,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'On Script - Set Data 2 0'),
(1061000,9,3,0,'',0,0,100,0,0,0,0,0,0,45,2,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'On Script - Set Data 2 0'),
(1061000,9,4,0,'',0,0,100,0,0,0,0,0,0,45,2,0,0,0,0,0,19,1243,100,0,0,0,0,0,0,'On Script - Set Data 2 0'),
(1061000,9,5,0,'',0,0,100,0,5000,5000,0,0,0,1,0,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Say Line 0'),
(1061000,9,6,0,'',0,0,100,0,3000,3000,0,0,0,45,3,0,0,0,0,0,19,10804,50,0,0,0,0,0,0,'Angus - On Script - Set Data 3 0'),
(1061000,9,7,0,'',0,0,100,0,2000,2000,0,0,0,1,1,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Say Line 1'),
(1061000,9,8,0,'',0,0,100,0,4000,4000,0,0,0,1,2,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Say Line 2'),
(1061000,9,9,0,'',0,0,100,0,2000,2000,0,0,0,45,4,0,0,0,0,0,19,10804,50,0,0,0,0,0,0,'Angus - On Script - Set Data 4 0'),
(1061000,9,10,0,'',0,0,100,0,2000,2000,0,0,0,1,3,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Say Line 3'),
(1061000,9,11,0,'',0,0,100,0,2000,2000,0,0,0,45,5,0,0,0,0,0,19,10804,50,0,0,0,0,0,0,'Angus - On Script - Set Data 5 0'),
(1061000,9,12,0,'',0,0,100,0,1000,1000,0,0,0,5,5,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 5'),
(1061000,9,13,0,'',0,0,100,0,1000,1000,0,0,0,5,11,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 11'),
(1061000,9,14,0,'',0,0,100,0,1000,1000,0,0,0,5,274,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 11'),
(1061000,9,15,0,'',0,0,100,0,0,0,0,0,0,5,274,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Script - Play Emote 11'),
(1061000,9,16,0,'',0,0,100,0,1000,1000,0,0,0,45,6,0,0,0,0,0,19,10804,50,0,0,0,0,0,0,'Angus - On Script - Set Data 6 0'),
(1061000,9,17,0,'',0,0,100,0,1000,1000,0,0,0,1,0,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'Angus - On Script - Say Line 0'),
(1061000,9,18,0,'',0,0,100,0,4000,4000,0,0,0,45,3,0,0,0,0,0,19,10803,50,0,0,0,0,0,0,'Angus - On Script - Set Data 3 0'),
(1061000,9,19,0,'',0,0,100,0,2000,2000,0,0,0,1,1,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'Angus - On Script - Say Line 1'),
(1061000,9,20,0,'',0,0,100,0,2000,2000,0,0,0,45,4,0,0,0,0,0,19,10803,50,0,0,0,0,0,0,'Angus - On Script - Set Data 4 0'),
(1061000,9,21,0,'',0,0,100,0,1000,1000,0,0,0,5,25,0,0,0,0,0,19,10803,50,0,0,0,0,0,0,'Angus - On Script - Play Emote 25'),
(1061000,9,22,0,'',0,0,100,0,0,0,0,0,0,4,6249,0,0,0,0,0,21,100,0,0,0,0,0,0,0,'Angus - On Script - Play Sound 6249'),
(1061000,9,23,0,'',0,0,100,0,2000,2000,0,0,0,5,21,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Script - Play Emote 21'),
(1061000,9,24,0,'',0,0,100,0,1000,1000,0,0,0,5,21,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 21'),
(1061000,9,25,0,'',0,0,100,0,0,0,0,0,0,5,23,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 23'),
(1061000,9,26,0,'',0,0,100,0,2000,2000,0,0,0,45,5,0,0,0,0,0,19,10803,50,0,0,0,0,0,0,'Angus - On Script - Set Data 5 0 Wheeler'),
(1061000,9,27,0,'',0,0,100,0,3000,3000,0,0,0,1,4,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Say Line 4'),
(1061000,9,28,0,'',0,0,100,0,2000,2000,0,0,0,45,7,0,0,0,0,0,19,10804,50,0,0,0,0,0,0,'Angus - On Script - Set Data 5 0 Middlecamp'),
(1061000,9,29,0,'',0,0,100,0,2000,2000,0,0,0,1,5,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Say Line 5 Middlecamp'),
(1061000,9,30,0,'',0,0,100,0,2000,2000,0,0,0,5,20,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 20 Middlecamp'),
(1061000,9,31,0,'',0,0,100,0,0,0,0,0,0,45,6,0,0,0,0,0,19,10803,50,0,0,0,0,0,0,'Angus - On Script - Set Data 6 0 Wheeler'),
(1061000,9,32,0,'',0,0,100,0,1000,1000,0,0,0,5,6,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 6 Wheeler'),
(1061000,9,33,0,'',0,0,100,0,0,0,0,0,0,4,6248,0,0,0,0,0,21,100,0,0,0,0,0,0,0,'Angus - On Script - Play Sound 6248'),
(1061000,9,34,0,'',0,0,100,0,3000,3000,0,0,0,45,7,0,0,0,0,0,19,10803,50,0,0,0,0,0,0,'Angus - On Script - Set Data 6 0 Wheeler'),
(1061000,9,35,0,'',0,0,100,0,0,0,0,0,0,1,6,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Say Line 6 Middlecamp'),
(1061000,9,36,0,'',0,0,100,0,1000,1000,0,0,0,45,8,0,0,0,0,0,19,10804,50,0,0,0,0,0,0,'Angus - On Script - Set Data 8 0 Middlecamp'),
(1061000,9,38,0,'',0,0,100,0,0,0,0,0,0,1,7,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Say Line 7 Middlecamp'),
(1061000,9,40,0,'',0,0,100,0,0,0,0,0,0,1,2,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'Angus - On Script - Say Line 2 Wheeler'),
(1061000,9,41,0,'',0,0,100,0,2000,2000,0,0,0,45,8,0,0,0,0,0,19,10803,50,0,0,0,0,0,0,'Angus - On Script - Set Data 8 0 Wheeler'),
(1061000,9,42,0,'',0,0,100,0,2000,2000,0,0,0,1,3,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'Angus - On Script - Say Line 3 Wheeler'),
(1061000,9,43,0,'',0,0,100,0,2000,2000,0,0,0,45,9,0,0,0,0,0,19,10803,50,0,0,0,0,0,0,'Angus - On Script - Set Data 9 0 Wheeler'),
(1061000,9,44,0,'',0,0,100,0,2000,2000,0,0,0,1,4,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'Angus - On Script - Say Line 4 Wheeler'),
(1061000,9,45,0,'',0,0,100,0,1000,1000,0,0,0,45,10,0,0,0,0,0,19,10803,50,0,0,0,0,0,0,'Angus - On Script - Set Data 10 0 Wheeler'),
(1061000,9,46,0,'',0,0,100,0,1000,1000,0,0,0,5,5,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 5 Wheeler'),
(1061000,9,47,0,'',0,0,100,0,0,0,0,0,0,45,9,0,0,0,0,0,19,10804,50,0,0,0,0,0,0,'Angus - On Script - Set Data 9 0 Middlecamp'),
(1061000,9,48,0,'',0,0,100,0,2000,2000,0,0,0,5,25,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 25 Middlecamp'),
(1061000,9,59,0,'',0,0,100,0,2000,2000,0,0,0,5,11,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 11 Middlecamp'),
(1061000,9,60,0,'',0,0,100,0,3000,3000,0,0,0,45,10,0,0,0,0,0,19,10803,50,0,0,0,0,0,0,'Angus - On Script - Set Data 10 0 Middlecamp'),
(1061000,9,61,0,'',0,0,100,0,0,0,0,0,0,45,11,0,0,0,0,0,19,10803,50,0,0,0,0,0,0,'Angus - On Script - Set Data 11 0 Wheeler'),
(1061000,9,62,0,'',0,0,100,0,2000,2000,0,0,0,1,8,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Say Line 8 Middlecamp'),
(1061000,9,63,0,'',0,0,100,0,0,0,0,0,0,4,6250,0,0,0,0,0,21,100,0,0,0,0,0,0,0,'Angus - On Script - Play Sound 6250'),
(1061000,9,64,0,'',0,0,100,0,0,0,0,0,0,45,11,0,0,0,0,0,19,10804,50,0,0,0,0,0,0,'Angus - On Script - Set Data 11 0 Middlecamp'),
(1061000,9,65,0,'',0,0,100,0,3000,3000,0,0,0,1,9,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Say Line 9 Middlecamp'),
(1061000,9,66,0,'',0,0,100,0,0,0,0,0,0,4,6250,0,0,0,0,0,21,100,0,0,0,0,0,0,0,'Angus - On Script - Play Sound 6250'),
(1061000,9,67,0,'',0,0,100,0,0,0,0,0,0,45,12,0,0,0,0,0,19,10804,50,0,0,0,0,0,0,'Angus - On Script - Set Data 12 0 Middlecamp'),
(1061000,9,68,0,'',0,0,100,0,3000,3000,0,0,0,1,10,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Say Line 10 Middlecamp'),
(1061000,9,69,0,'',0,0,100,0,0,0,0,0,0,4,6250,0,0,0,0,0,21,100,0,0,0,0,0,0,0,'Angus - On Script - Play Sound 6250'),
(1061000,9,70,0,'',0,0,100,0,0,0,0,0,0,45,13,0,0,0,0,0,19,10804,50,0,0,0,0,0,0,'Angus - On Script - Set Data 13 0 Middlecamp'),
(1061000,9,71,0,'',0,0,100,0,0,0,0,0,0,5,71,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 71 Middlecamp'),
(1061000,9,72,0,'',0,0,100,0,0,0,0,0,0,4,6246,0,0,0,0,0,21,100,0,0,0,0,0,0,0,'Angus - On Script - Play Sound 6246'),
(1061000,9,73,0,'',0,0,100,0,3000,3000,0,0,0,45,14,0,0,0,0,0,19,10804,50,0,0,0,0,0,0,'Angus - On Script - Set Data 14 0 Middlecamp'),
(1061000,9,74,0,'',0,0,100,0,1000,1000,0,0,0,45,12,0,0,0,0,0,19,10803,50,0,0,0,0,0,0,'Angus - On Script - Set Data 12 0 Wheeler'),
(1061000,9,75,0,'',0,0,100,0,1000,1000,0,0,0,5,2,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 2 Middlecamp'),
(1061000,9,76,0,'',0,0,100,0,0,0,0,0,0,5,21,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 21 Wheeler'),
(1061000,9,77,0,'',0,0,100,0,4000,4000,0,0,0,45,15,0,0,0,0,0,19,10804,50,0,0,0,0,0,0,'Angus - On Script - Set Data 15 0 Middlecamp'),
(1061000,9,78,0,'',0,0,100,0,0,0,0,0,0,45,13,0,0,0,0,0,19,10803,50,0,0,0,0,0,0,'Angus - On Script - Set Data 13 0 Wheeler'),
(1061000,9,79,0,'',0,0,100,0,0,0,0,0,0,45,3,0,0,0,0,0,19,10611,50,0,0,0,0,0,0,'Angus - On Script - Set Data 3 0 Shorty'),
(1061000,9,80,0,'',0,0,100,0,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,2.84489,'Angus - On Script - Set Orientation 2,844887'),
(1061000,9,81,0,'',0,0,100,0,2000,2000,0,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Script - Play Emote 4 Wheeler'),
(1061000,9,82,0,'',0,0,100,0,0,0,0,0,0,5,4,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 4 Shorty'),
(1061000,9,83,0,'',0,0,100,0,0,0,0,0,0,4,6256,0,0,0,0,0,21,100,0,0,0,0,0,0,0,'Angus - On Script - Play Sound 6256'),
(1061000,9,84,0,'',0,0,100,0,0,0,0,0,0,45,3,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 3 0 Klemmy'),
(1061000,9,85,0,'',0,0,100,0,10000,10000,0,0,0,11,18634,2,0,0,0,0,8,0,0,0,0,0,0,0,0,'Angus - On Script - Summon Creature ''Mortar Team Target Dummy'''),
(1061000,9,86,0,'',0,0,100,0,3000,3000,0,0,0,45,4,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 4 0 Klemmy'),
(1061000,9,87,0,'',0,0,100,0,3000,3000,0,0,0,45,5,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 5 0 Klemmy'),
(1061000,9,88,0,'',0,0,100,0,2000,2000,0,0,0,5,3,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 3 Klemmy'),
(1061000,9,89,0,'',0,0,100,0,0,0,0,0,0,45,4,0,0,0,0,0,19,10611,50,0,0,0,0,0,0,'Angus - On Script - Set Data 4 0 Shorty'),
(1061000,9,90,0,'',0,0,100,0,1000,1000,0,0,0,45,6,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 6 0 Klemmy'),
(1061000,9,91,0,'',0,0,100,0,1000,1000,0,0,0,5,16,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 16 Shorty'),
(1061000,9,92,0,'',0,0,100,0,1000,1000,0,0,0,5,69,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 69 Shorty'),
(1061000,9,93,0,'',0,0,100,0,1000,1000,0,0,0,5,0,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 0 Shorty'),
(1061000,9,94,0,'',0,0,100,0,2000,2000,0,0,0,45,5,0,0,0,0,0,19,10611,50,0,0,0,0,0,0,'Angus - On Script - Set Data 5 0 Shorty'),
(1061000,9,95,0,'',0,0,100,0,1000,1000,0,0,0,45,16,0,0,0,0,0,19,10804,50,0,0,0,0,0,0,'Angus - On Script - Set Data 16 0 Middlecamp'),
(1061000,9,96,0,'',0,0,100,0,0,0,0,0,0,45,14,0,0,0,0,0,19,10803,50,0,0,0,0,0,0,'Angus - On Script - Set Data 14 0 Wheeler'),
(1061000,9,97,0,'',0,0,100,0,0,0,0,0,0,5,25,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Script - Play Emote 25'),
(1061000,9,98,0,'',0,0,100,0,1000,1000,0,0,0,45,6,0,0,0,0,0,19,10611,50,0,0,0,0,0,0,'Angus - On Script - Set Data 6 0 Shorty'),
(1061000,9,99,0,'',0,0,100,0,2000,2000,0,0,0,4,7316,0,0,0,0,0,21,100,0,0,0,0,0,0,0,'Angus - On Script - Play Sound 7316'),
(1061000,9,100,0,'',0,0,100,0,2000,2000,0,0,0,45,7,0,0,0,0,0,19,10611,50,0,0,0,0,0,0,'Angus - On Script - Set Data 7 0 Shorty'),
(1061000,9,101,0,'',0,0,100,0,0,0,0,0,0,51,0,0,0,0,0,0,19,11875,200,0,0,0,0,0,0,'On Script - Kill Target'),
(1061000,9,102,0,'',0,0,100,0,3000,3000,0,0,0,5,17,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 17 Shorty'),
(1061000,9,103,0,'',0,0,100,0,3000,3000,0,0,0,45,8,0,0,0,0,0,19,10611,50,0,0,0,0,0,0,'Angus - On Script - Set Data 8 0 Shorty'),
(1061000,9,104,0,'',0,0,100,0,0,0,0,0,0,45,8,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 8 0 Klemmy'),
(1061000,9,105,0,'',0,0,100,0,0,0,0,0,0,5,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Script - Play Emote 1'),
(1061000,9,106,0,'',0,0,100,0,0,0,0,0,0,4,6252,0,0,0,0,0,21,100,0,0,0,0,0,0,0,'Angus - On Script - Play Sound 6252'),
(1061000,9,107,0,'',0,0,100,0,1000,1000,0,0,0,5,4,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 4 Shorty'),
(1061000,9,108,0,'',0,0,100,0,3000,3000,0,0,0,45,9,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Set Data 8 0 Shorty'),
(1061000,9,109,0,'',0,0,100,0,5000,5000,0,0,0,11,18634,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Script - Summon Creature ''Mortar Team Target Dummy'''),
(1061000,9,110,0,'',0,0,100,0,2000,2000,0,0,0,45,10,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 10 0 Klemmy'),
(1061000,9,111,0,'',0,0,100,0,2000,2000,0,0,0,45,11,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 11 0 Klemmy'),
(1061000,9,112,0,'',0,0,100,0,2000,2000,0,0,0,5,66,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 66 Klemmy'),
(1061000,9,113,0,'',0,0,100,0,3000,3000,0,0,0,45,9,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Set Data 9 0 Shorty'),
(1061000,9,114,0,'',0,0,100,0,2000,2000,0,0,0,45,12,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 12 0 Klemmy'),
(1061000,9,115,0,'',0,0,100,0,0,0,0,0,0,5,25,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 8 Shorty'),
(1061000,9,116,0,'',0,0,100,0,2000,2000,0,0,0,4,6254,0,0,0,0,0,21,100,0,0,0,0,0,0,0,'Angus - On Script - Play Sound 6254'),
(1061000,9,117,0,'',0,0,100,0,1000,1000,0,0,0,45,13,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 13 0 Klemmy'),
(1061000,9,118,0,'',0,0,100,0,3000,3000,0,0,0,17,0,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 0 Shorty'),
(1061000,9,119,0,'',0,0,100,0,1000,1000,0,0,0,45,10,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Set Data 10 0 Shorty'),
(1061000,9,120,0,'',0,0,100,0,1000,1000,0,0,0,5,25,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 25 Shorty'),
(1061000,9,121,0,'',0,0,100,0,0,0,0,0,0,4,6255,0,0,0,0,0,21,100,0,0,0,0,0,0,0,'Angus - On Script - Play Sound 6255'),
(1061000,9,122,0,'',0,0,100,0,1000,1000,0,0,0,45,6,0,0,0,0,0,19,10611,50,0,0,0,0,0,0,'Angus - On Script - Set Data 6 0 Shorty'),
(1061000,9,123,0,'',0,0,100,0,1000,1000,0,0,0,4,7316,0,0,0,0,0,21,100,0,0,0,0,0,0,0,'Angus - On Script - Play Sound 7316'),
(1061000,9,124,0,'',0,0,100,0,1000,1000,0,0,0,45,7,0,0,0,0,0,19,10611,50,0,0,0,0,0,0,'Angus - On Script - Set Data 7 0 Shorty'),
(1061000,9,125,0,'',0,0,100,0,0,0,0,0,0,51,0,0,0,0,0,0,19,11875,200,0,0,0,0,0,0,'On Script - Kill Target'),
(1061000,9,126,0,'',0,0,100,0,1000,1000,0,0,0,45,9,0,0,0,0,0,19,10611,50,0,0,0,0,0,0,'Angus - On Script - Set Data 9 0 Shorty'),
(1061000,9,127,0,'',0,0,100,0,2000,2000,0,0,0,5,25,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 25 Shorty'),
(1061000,9,128,0,'',0,0,100,0,0,0,0,0,0,4,6251,0,0,0,0,0,21,100,0,0,0,0,0,0,0,'Angus - On Script - Play Sound 6251'),
(1061000,9,129,0,'',0,0,100,0,1000,1000,0,0,0,45,15,0,0,0,0,0,19,10804,50,0,0,0,0,0,0,'Angus - On Script - Set Data 15 0 Middlecamp'),
(1061000,9,130,0,'',0,0,100,0,0,0,0,0,0,45,8,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 8 0 Klemmy'),
(1061000,9,131,0,'',0,0,100,0,1000,1000,0,0,0,45,13,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'Angus - On Script - Set Data 13 0 Wheeler'),
(1061000,9,132,0,'',0,0,100,0,1000,1000,0,0,0,45,10,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Set Data 10 0 Shorty'),
(1061000,9,133,0,'',0,0,100,0,0,0,0,0,0,5,4,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 4 Wheeler'),
(1061000,9,134,0,'',0,0,100,0,7000,7000,0,0,0,11,18634,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Script - Summon Creature ''Mortar Team Target Dummy'''),
(1061000,9,135,0,'',0,0,100,0,1000,1000,0,0,0,45,10,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 10 0 Klemmy'),
(1061000,9,136,0,'',0,0,100,0,1000,1000,0,0,0,45,11,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 11 0 Klemmy'),
(1061000,9,137,0,'',0,0,100,0,2000,2000,0,0,0,45,6,0,0,0,0,0,19,10611,50,0,0,0,0,0,0,'Angus - On Script - Set Data 6 0 Shorty'),
(1061000,9,138,0,'',0,0,100,0,0,0,0,0,0,5,25,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 25 Klemmy'),
(1061000,9,139,0,'',0,0,100,0,1000,1000,0,0,0,5,11,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 11 Klemmy'),
(1061000,9,140,0,'',0,0,100,0,0,0,0,0,0,5,15,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 15 Shorty'),
(1061000,9,141,0,'',0,0,100,0,0,0,0,0,0,5,5,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Script - Play Emote 5'),
(1061000,9,142,0,'',0,0,100,0,0,0,0,0,0,4,6258,0,0,0,0,0,21,100,0,0,0,0,0,0,0,'Angus - On Script - Play Sound 6258'),
(1061000,9,143,0,'',0,0,100,0,2000,2000,0,0,0,4,7316,0,0,0,0,0,21,100,0,0,0,0,0,0,0,'Angus - On Script - Play Sound 7316'),
(1061000,9,144,0,'',0,0,100,0,1000,1000,0,0,0,45,16,0,0,0,0,0,19,10804,50,0,0,0,0,0,0,'Angus - On Script - Set Data 16 0 Middlecamp'),
(1061000,9,145,0,'',0,0,100,0,1000,1000,0,0,0,45,14,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'Angus - On Script - Set Data 14 0 Wheeler'),
(1061000,9,146,0,'',0,0,100,0,0,0,0,0,0,45,14,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 14 0 Klemmy'),
(1061000,9,147,0,'',0,0,100,0,1000,1000,0,0,0,45,7,0,0,0,0,0,19,10611,50,0,0,0,0,0,0,'Angus - On Script - Set Data 6 0 Shorty'),
(1061000,9,148,0,'',0,0,100,0,0,0,0,0,0,51,0,0,0,0,0,0,19,11875,200,0,0,0,0,0,0,'On Script - Kill Target'),
(1061000,9,149,0,'',0,0,100,0,1000,1000,0,0,0,5,10,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Script - Play Emote 10'),
(1061000,9,150,0,'',0,0,100,0,0,0,0,0,0,5,10,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 10 Shorty'),
(1061000,9,151,0,'',0,0,100,0,2000,2000,0,0,0,45,15,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 15 0 Klemmy'),
(1061000,9,152,0,'',0,0,100,0,3000,3000,0,0,0,45,16,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 16 0 Klemmy'),
(1061000,9,153,0,'',0,0,100,0,6000,6000,0,0,0,5,14,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 14 Klemmy'),
(1061000,9,154,0,'',0,0,100,0,0,0,0,0,0,5,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Script - Play Emote 0'),
(1061000,9,155,0,'',0,0,100,0,0,0,0,0,0,5,0,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 0 Shorty'),
(1061000,9,156,0,'',0,0,100,0,0,0,0,0,0,5,11,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Script - Play Emote 11'),
(1061000,9,157,0,'',0,0,100,0,0,0,0,0,0,5,11,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 11 Shorty'),
(1061000,9,158,0,'',0,0,100,0,1000,1000,0,0,0,5,11,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 11 Middlecamp'),
(1061000,9,159,0,'',0,0,100,0,0,0,0,0,0,5,11,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 11 Wheeler'),
(1061000,9,160,0,'',0,0,100,0,0,0,0,0,0,5,11,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 11 Klemmy'),
(1061000,9,161,0,'',0,0,100,0,1000,1000,0,0,0,5,11,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Script - Play Emote 11'),
(1061000,9,162,0,'',0,0,100,0,0,0,0,0,0,5,11,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 11 Shorty'),
(1061000,9,163,0,'',0,0,100,0,1000,1000,0,0,0,5,11,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 11 Middlecamp'),
(1061000,9,164,0,'',0,0,100,0,0,0,0,0,0,5,11,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'Angus - On Script - Play Emote 11 Wheeler'),
(1061000,9,165,0,'',0,0,100,0,1000,1000,0,0,0,45,17,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 17 0 Klemmy'),
(1061000,9,166,0,'',0,0,100,0,0,0,0,0,0,45,11,0,0,0,0,0,19,10611,50,0,0,0,0,0,0,'Angus - On Script - Set Data 11 0 Shorty'),
(1061000,9,167,0,'',0,0,100,0,0,0,0,0,0,45,2,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Script - Set Data 2 0 Angus'),
(1061000,9,168,0,'',0,0,100,0,0,0,0,0,0,45,17,0,0,0,0,0,19,10804,50,0,0,0,0,0,0,'Angus - On Script - Set Data 17 0 Middlecamp'),
(1061000,9,169,0,'',0,0,100,0,0,0,0,0,0,45,15,0,0,0,0,0,19,10803,50,0,0,0,0,0,0,'Angus - On Script - Set Data 15 0 Wheeler'),
(1061000,9,170,0,'',0,0,100,0,12000,12000,0,0,0,45,18,0,0,0,0,0,19,10805,100,0,0,0,0,0,0,'Angus - On Script - Set Data 18 0 Klemmy'),
(1061000,9,171,0,'',0,0,100,0,4000,4000,0,0,0,45,16,0,0,0,0,0,19,10803,100,0,0,0,0,0,0,'Angus - On Script - Set Data 16 0 Wheeler'),
(1061000,9,172,0,'',0,0,100,0,2000,2000,0,0,0,45,3,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Angus - On Script - Set Data 3 0 Angus'),
(1061000,9,173,0,'',0,0,100,0,1000,1000,0,0,0,45,18,0,0,0,0,0,19,10804,100,0,0,0,0,0,0,'Angus - On Script - Set Data 18 0 Middlecamp'),
(1061000,9,174,0,'',0,0,100,0,1000,1000,0,0,0,45,3,0,0,0,0,0,19,1243,100,0,0,0,0,0,0,'Angus - On Script - Set Data 3 0 Hegnar'),
(1061000,9,175,0,'',0,0,100,0,0,0,0,0,0,45,12,0,0,0,0,0,19,10611,100,0,0,0,0,0,0,'Angus - On Script - Set Data 12 0 Shorty');
DELETE FROM `creature_text` WHERE `CreatureID` = 10803 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 10803 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 10803 AND `GroupID` = 2;
DELETE FROM `creature_text` WHERE `CreatureID` = 10803 AND `GroupID` = 3;
DELETE FROM `creature_text` WHERE `CreatureID` = 10803 AND `GroupID` = 4;
DELETE FROM `creature_text` WHERE `CreatureID` = 10804 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 10804 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 10804 AND `GroupID` = 10;
DELETE FROM `creature_text` WHERE `CreatureID` = 10804 AND `GroupID` = 2;
DELETE FROM `creature_text` WHERE `CreatureID` = 10804 AND `GroupID` = 3;
DELETE FROM `creature_text` WHERE `CreatureID` = 10804 AND `GroupID` = 4;
DELETE FROM `creature_text` WHERE `CreatureID` = 10804 AND `GroupID` = 5;
DELETE FROM `creature_text` WHERE `CreatureID` = 10804 AND `GroupID` = 6;
DELETE FROM `creature_text` WHERE `CreatureID` = 10804 AND `GroupID` = 7;
DELETE FROM `creature_text` WHERE `CreatureID` = 10804 AND `GroupID` = 8;
DELETE FROM `creature_text` WHERE `CreatureID` = 10804 AND `GroupID` = 9;
DELETE FROM `creature_text` WHERE `CreatureID` = 37507 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 6782 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 706 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 836 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 836 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 946 AND `GroupID` = 0;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(706,0,0,'You be dead soon!',12,0,100,0,0,0,10434,'Frostmane Troll Whelp'),
(706,0,1,'Killing you be easy.',12,0,100,0,0,0,1909,'Frostmane Troll Whelp'),
(706,0,2,'I gonna make you into mojo!',12,0,100,0,0,0,10435,'Frostmane Troll Whelp'),
(706,0,3,'My weapon be thirsty!',12,0,100,0,0,0,1908,'Frostmane Troll Whelp - On Aggro'),
(836,0,1,'There''s nothing like some scalding mornbrew on a chilly Dun Morogh day to get things started right!',12,0,100,1,0,0,4377,'Durnan Furcutter after quest'),
(836,1,1,'Oooooo hot hot hot!  If that won''t put spring in your step, I don''t know what will!',12,0,100,400,0,0,4378,'Durnan Furcutter after quest'),
(946,0,0,'You be dead soon!',12,14,100,0,0,0,10434,'Frostmane Novice'),
(946,0,1,'Killing you be easy.',12,14,100,0,0,0,1909,'Frostmane Novice'),
(946,0,2,'I gonna make you into mojo!',12,14,100,0,0,0,10435,'Frostmane Novice'),
(946,0,3,'My weapon be thirsty!',12,14,100,0,0,0,1908,'Frostmane Novice'),
(6782,0,0,'Alright, so you''re just going to head through this tunnel and whaaaaa....?',12,0,100,397,0,0,37003,'Hands Springsprocket'),
(10803,0,0,'%s casually points at the orc target...',16,0,100,25,0,0,6118,'Wheeler'),
(10803,1,0,'%s hits!',16,0,100,0,0,0,6142,'Wheeler'),
(10803,2,0,'%s quickly shoots the middle target dead center...',16,0,100,0,0,0,6124,'Wheeler'),
(10803,3,0,'%s then takes aim at the bottles...',16,0,100,0,0,0,6122,'Wheeler'),
(10803,4,0,'%s only hits the side of the box!',16,0,100,0,0,0,6123,'Wheeler'),
(10804,0,0,'%s picks the orc target...',16,0,100,25,0,0,6107,'Middlecamp'),
(10804,1,0,'%s misses...',16,0,100,6,0,0,6140,'Middlecamp'),
(10804,2,0,'%s takes a more careful aim...',16,0,100,6,0,6247,6106,'Middlecamp'),
(10804,3,0,'%s misses again...',16,0,100,6,0,0,6141,'Middlecamp'),
(10804,4,0,'%s takes aim at the bottles...',16,0,100,0,0,0,6120,'Middlecamp'),
(10804,5,0,'%s misses...',16,0,100,15,0,0,6140,'Middlecamp'),
(10804,6,0,'%s takes aim at the bottles again...',16,0,100,0,0,0,6121,'Middlecamp'),
(10804,7,0,'%s misses again...',16,0,100,18,0,0,6141,'Middlecamp'),
(10804,8,0,'%s quickly shoots the orc dummy...',16,0,100,0,0,0,6137,'Middlecamp'),
(10804,9,0,'%s turns to the paper target...',16,0,100,0,0,0,6136,'Middlecamp'),
(10804,10,0,'%s shoots from the hip at the bottles...',16,0,100,0,0,0,6138,'Middlecamp'),
(37507,0,0,'You be dead soon!',12,0,100,0,0,0,10434,'Frostmane Blade'),
(37507,0,1,'Killing you be easy.',12,0,100,0,0,0,1909,'Frostmane Blade'),
(37507,0,2,'I gonna make you into mojo!',12,0,100,0,0,0,10435,'Frostmane Blade'),
(37507,0,3,'My weapon be thirsty!',12,0,100,0,0,0,1908,'Frostmane Blade');
DELETE FROM `waypoints` WHERE `entry` = 10610;
DELETE FROM `waypoints` WHERE `entry` = 10803;
DELETE FROM `waypoints` WHERE `entry` = 10805;
INSERT INTO `waypoints` (`entry`,`pointid`,`position_x`,`position_y`,`position_z`,`point_comment`) VALUES
(10610,1,-6009.25,-202.65,406.983,'Angus'),
(10610,2,-6009.25,-202.65,406.983,'Angus'),
(10610,3,-6009.25,-202.65,406.983,'Angus'),
(10610,4,-6031.16,-230.264,414.361,'Angus'),
(10803,1,-6011.87,-199.241,406.834,'Wheeler'),
(10803,2,-6031.93,-221.088,413.153,'Wheeler'),
(10805,1,-6011.17,-197.021,406.765,'Klemmy'),
(10805,2,-6015.51,-184.362,407.712,'Klemmy'),
(10805,3,-6042.37,-191.419,421.046,'Klemmy'),
(10805,4,-6065.18,-201.938,424.419,'Klemmy');
DELETE FROM `creature_template_addon` WHERE `entry` IN (37113,37169);
INSERT INTO `creature_template_addon` (`entry`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(37113,0,0,0,1,0,'70045'),
(37169,0,0,0,1,0,'70045');
DELETE FROM `spell_area` WHERE `spell` = 70044 AND `area` = 132;
INSERT INTO `spell_area` (`spell`,`area`,`quest_start`,`quest_end`,`aura_spell`,`racemask`,`classmask`,`active_event`,`gender`,`autocast`,`quest_start_status`,`quest_end_status`) VALUES
(70044,132,24491,24492,0,0,0,0,2,1,74,43);
