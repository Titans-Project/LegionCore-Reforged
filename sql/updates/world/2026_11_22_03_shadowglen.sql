-- Shadowglen: SmartAI of the starting zone NPCs
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (1986,1994,2077,2079,2081,2150,2852,2853,3514,3515,3594,12160,34575,34757,34761,49478,49479);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1986 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1986,0,0,0,'',0,0,100,0,3000,7000,3000,7000,0,11,6751,32,0,0,0,0,2,0,0,0,0,0,0,0,0,'Webwood Spider - In Combat - Cast ''Weak Poison''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1994 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1994,0,0,0,'',0,0,100,0,4000,9000,4000,9000,0,11,11918,32,0,0,0,0,2,0,0,0,0,0,0,0,0,'Githyiss the Vile - In Combat - Cast ''Poison''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2077 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2077,0,0,0,'',1,0,70,0,2000,4000,7000,9000,0,5,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Melithar Forteramure - OOC - Play emote Talk'),
(2077,0,1,0,'',19,0,100,0,3118,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Melithar Forteramure - on quest accepted - Say text1'),
(2077,0,2,0,'',19,0,100,0,3117,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Melithar Forteramure - on quest accepted - Say text1'),
(2077,0,3,0,'',19,0,100,0,26841,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Melithar Forteramure - on quest accepted - Say text1'),
(2077,0,4,0,'',19,0,100,0,3119,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Melithar Forteramure - on quest accepted - Say text1'),
(2077,0,5,0,'',19,0,100,0,3116,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Melithar Forteramure - on quest accepted - Say text1'),
(2077,0,6,0,'',19,0,100,0,3120,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Melithar Forteramure - on quest accepted - Say text1'),
(2077,0,7,0,'',19,0,100,0,31168,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Melithar Forteramure - on quest accepted - Say text1');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2079 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2079,0,0,0,'',20,0,100,0,28713,0,0,0,0,80,207900,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Ilthalaine - on quest rewarded - action list'),
(2079,0,1,0,'',19,0,100,0,3118,0,0,0,0,1,1,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Ilthalaine - on quest accepted - Say text2'),
(2079,0,2,0,'',19,0,100,0,3117,0,0,0,0,1,1,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Ilthalaine - on quest accepted - Say text2'),
(2079,0,3,0,'',19,0,100,0,26841,0,0,0,0,1,1,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Ilthalaine - on quest accepted - Say text2'),
(2079,0,4,0,'',19,0,100,0,3119,0,0,0,0,1,1,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Ilthalaine - on quest accepted - Say text2'),
(2079,0,5,0,'',19,0,100,0,3116,0,0,0,0,1,1,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Ilthalaine - on quest accepted - Say text2'),
(2079,0,6,0,'',19,0,100,0,3120,0,0,0,0,1,1,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Ilthalaine - on quest accepted - Say text2');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2081 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2081,0,0,0,'',19,0,100,0,13945,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Sentinel Kyra Starsong - on quest Accepted - Say text');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2150 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2150,0,0,0,'',20,0,100,0,489,0,0,0,0,80,215000,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Zenn Foulhoof - On Quest ''Seek Redemption!'' Finished - Run Script (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2852 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2852,0,0,1,'',38,0,100,0,1,1,0,0,0,12,2853,3,7000,0,0,0,1,0,0,0,0,0,0,0,0,'Enslaved Druid of the Talon - On Data set  - Summon unit'),
(2852,0,1,0,'',61,0,100,0,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Enslaved Druid of the Talon - On Data set  - Despawn');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2853 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2853,0,0,0,'',54,0,100,0,0,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,10342.3,1201.52,1472.23,3.2734,'Freed Druid of the Talon - On just summoned  - move to pos');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3514 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(3514,0,0,0,'',10,0,100,0,1,10,20000,20000,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Tenaron Stormgrip - Ooc los  - Say line 1'),
(3514,0,1,0,'',19,0,100,0,28731,0,0,0,0,80,351400,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Tenaron Stormgrip - On quest accepted  - Action list');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3515 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(3515,0,0,0,'',20,0,100,0,28731,0,0,0,0,45,1,1,0,0,0,0,19,34575,20,0,0,0,0,0,0,'Corithras Moonrage - on quest rewarded - Set Data');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3594 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(3594,0,0,0,'',20,0,100,0,3118,0,0,0,0,5,1,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Frahun Shadewhisper - On Quest ''Encrypted Sigil'' Finished - Play Emote 1');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 12160 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(12160,0,0,0,'',25,0,100,1,0,0,0,0,0,11,18950,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Shadowglen Sentinel - On Respawn - Cast ''Invisibility and Stealth Detection'' (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 34575 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(34575,0,0,0,'',25,0,100,0,0,0,0,0,0,47,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Moonwell Bunny - On reset  - Set invisible'),
(34575,0,1,0,'',10,0,100,0,1,10,119000,119000,0,11,65656,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Doranel Amberleaf - ooc los - Cast Summon Shade of the Kaldorei');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 34757 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(34757,0,0,0,'',10,0,100,0,1,25,25000,25000,0,80,3475700,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Doranel Amberleaf - ooc los - action list');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 34761 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(34761,0,0,1,'',62,0,100,0,10564,0,0,0,0,11,66159,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Sentinel Kyra Starsong - on gossip select - Cast Forcecast Summon Nightsaber'),
(34761,0,1,0,'',61,0,100,0,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Sentinel Kyra Starsong - on gossip select - Close gossip');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 49478 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(49478,0,0,0,'',38,0,100,0,1,1,0,0,0,80,4947800,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Dentaria Silverglade - On data set - action list');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 49479 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(49479,0,0,0,'',20,0,100,0,28724,0,0,0,0,80,4947900,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Dentaria Silverglade - on quest rewarded - action list'),
(49479,0,1,0,'',19,0,100,0,28725,0,0,0,0,1,1,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Dentaria Silverglade - on quest rewarded - Say text 2'),
(49479,0,2,0,'',19,0,100,0,28729,0,0,0,0,1,2,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Dentaria Silverglade - on quest rewarded - Say text 3'),
(49479,0,3,0,'',19,0,100,0,28730,0,0,0,0,1,3,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Dentaria Silverglade - on quest rewarded - Say text 4');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 207900 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(207900,9,0,0,'',0,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Ilthalaine - Action list - Say text 1'),
(207900,9,1,0,'',0,0,100,0,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Ilthalaine - Action list - store target'),
(207900,9,2,0,'',0,0,100,0,6000,6000,0,0,0,1,0,0,0,0,0,0,19,49477,10,0,0,0,0,0,0,'Ilthalaine - Action list - Say text 1'),
(207900,9,3,0,'',0,0,100,0,1000,1000,0,0,0,100,1,0,0,0,0,0,19,49478,10,0,0,0,0,0,0,'Ilthalaine - Action list - Send target'),
(207900,9,4,0,'',0,0,100,0,6000,6000,0,0,0,45,1,1,0,0,0,0,19,49478,10,0,0,0,0,0,0,'Ilthalaine - Action list - Set data');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 215000 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(215000,9,0,0,'',0,0,100,0,2000,2000,0,0,0,11,12244,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Zenn Foulhoof - On Script - Cast ''Poof'''),
(215000,9,1,0,'',0,0,100,0,0,0,0,0,0,11,3329,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Zenn Foulhoof - On Script - Cast ''Creature - Frog Form'''),
(215000,9,2,0,'',0,0,100,0,1000,1000,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Zenn Foulhoof - On Script - Say Line 0');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 351400 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(351400,9,0,0,'',0,0,100,0,0,0,0,0,0,1,1,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Tenaron Stormgrip - Action list  - Say text 2'),
(351400,9,1,0,'',0,0,100,0,0,0,0,0,0,11,65453,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Tenaron Stormgrip - Action list  - Cast Wind''s Embrace');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3475700 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(3475700,9,0,0,'',0,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Doranel Amberleaf - action list - Say text 1'),
(3475700,9,1,0,'',0,0,100,0,4000,4000,0,0,0,1,0,0,0,0,0,0,19,34756,15,0,0,0,0,0,0,'Doranel Amberleaf - action list - Say text 1'),
(3475700,9,2,0,'',0,0,100,0,2000,2000,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,10456,833.091,1380.94,2.79533,'Doranel Amberleaf - action list - Set Orientation'),
(3475700,9,3,0,'',0,0,100,0,5000,5000,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,10456,832.972,1381.02,5.3058,'Doranel Amberleaf - action list - Set Orientation');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4947800 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4947800,9,0,0,'',0,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,12,1,0,0,0,0,0,0,0,'Dentaria Silverglade - Action list - Say text 1'),
(4947800,9,1,0,'',0,0,100,0,6000,6000,0,0,0,1,1,0,0,0,0,0,12,1,0,0,0,0,0,0,0,'Dentaria Silverglade - Action list - Say text 2');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4947900 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4947900,9,0,0,'',0,0,100,0,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Dentaria Silverglade - on quest rewarded - Close gossip'),
(4947900,9,1,0,'',0,0,100,0,0,0,0,0,0,83,2,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Dentaria Silverglade - on quest rewarded - Remove npc flag'),
(4947900,9,2,0,'',0,0,100,0,0,0,0,0,0,11,87071,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Dentaria Silverglade - on quest rewarded - Cast Alchemy'),
(4947900,9,3,0,'',0,0,100,0,3000,3000,0,0,0,11,92388,0,0,0,0,0,19,8584,15,0,0,0,0,0,0,'Dentaria Silverglade - on quest rewarded - Cast Curing Ivveron'),
(4947900,9,4,0,'',0,0,100,0,2000,2000,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Dentaria Silverglade - on quest rewarded - Say text 1'),
(4947900,9,5,0,'',0,0,100,0,0,0,0,0,0,82,2,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Dentaria Silverglade - on quest rewarded - Add npc flag');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 1 AND `SourceEntry` = 34757 AND `SourceId` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 2 AND `SourceEntry` = 34575 AND `SourceId` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 1 AND `SourceEntry` = 3514 AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(22,1,34757,0,0,31,0,4,0,0,0,0,'','event require a player'),
(22,2,34575,0,0,9,0,28730,0,0,0,0,'','event require quest 28730 taken'),
(22,1,3514,0,0,31,0,4,0,0,0,0,'','event require a player');
DELETE FROM `creature_text` WHERE `CreatureID` = 2077 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 2079 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 2079 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 2081 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 2150 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 34756 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 34757 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 3514 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 3514 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 49477 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 49478 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 49478 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 49479 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 49479 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 49479 AND `GroupID` = 2;
DELETE FROM `creature_text` WHERE `CreatureID` = 49479 AND `GroupID` = 3;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(2077,0,0,'You''ll find the trainer inside of Aldrassil, $n.',12,0,100,0,0,0,49664,'Melithar Staghelm'),
(2079,0,0,'I''m sorry, Huntress. None of our new $cs are ready. They still have much to learn.',12,0,100,0,0,0,49681,'Ilthalaine'),
(2079,1,0,'You''ll find the trainer inside of Aldrassil, $n.',12,0,100,0,0,0,49664,'Ilthalaine'),
(2081,0,0,'Some of our huntresses have already made their way to Ban''ethil Barrow Den. They will find you when you get inside. Go ahead and speak to Citarre to secure a ride there.',12,0,100,0,0,0,34694,'Sentinel Kyra Starsong'),
(2150,0,0,'Ribbit! No!! This cannot...ribbit...be! You have duped me with...ribbit...your foul trickery! Ribbit!',12,0,100,1,0,0,886,'Zenn Foulhoof'),
(3514,0,0,'Greetings, $c.',12,0,100,2,0,0,13306,'Tenaron Stormgrip'),
(3514,1,0,'Take with you the blessing of the wind''s embrace. May it make your descent from Aldrassil more... direct.',12,0,100,1,0,0,34602,'Tenaron Stormgrip'),
(34756,0,0,'Shh! Someone''s here.',12,0,100,0,0,0,35023,'Moriana Dawnlight'),
(34757,0,0,'Don''t get me wrong... I''m grateful to have Shando Stormrage back, but this is all really suspicious. What happened to Fandral?',12,0,100,0,0,0,35024,'Doranel Amberleaf'),
(49477,0,0,'I fear time may not be on our side, Ilthalaine. The battle may carry to Teldrassil itself if we do not act soon.',12,0,100,0,0,0,49682,'Huntress Sandrya Moonfall'),
(49478,0,0,'We will leave you to your training, Ilthalaine. This is your area of expertise.',12,0,100,0,0,0,49683,'Dentaria Silverglade'),
(49478,1,0,'Study well, $n. We may need you fighting by our side before the next moon fills.',12,0,100,0,0,0,49684,'Dentaria Silverglade'),
(49479,0,0,'Iverron''s poison is cured, but it will take some time for him to recover.',12,0,100,0,0,0,49668,'Dentaria Silverglade'),
(49479,1,0,'Shadowthread Cave lies to the north. Be careful, it''s dangerous there of late.',12,0,100,0,0,0,49665,'Dentaria Silverglade'),
(49479,2,0,'The moonwell is to the northeast, on the other side of the pool and up the hill.',12,0,100,0,0,0,49666,'Dentaria Silverglade'),
(49479,3,0,'The ramp up to Aldrassil is just in sight over there. Circle around and find Tenaron up top.',12,0,100,0,0,0,49667,'Dentaria Silverglade');
