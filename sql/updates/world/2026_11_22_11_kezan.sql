-- Kezan: Gasbot Control Panel spawned and Gasbot scripted, SmartAI of the starting zone NPCs
DELETE FROM `gameobject` WHERE `guid` = 25926054;
INSERT INTO `gameobject` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`PhaseId`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(25926054,201736,648,4737,4765,1,8,'',-8422.51,1325.68,101.916,4.67748,0,0,0.719341,-0.694658,120,255,1);
UPDATE `creature_template` SET `ScriptName` = 'npc_kezan_gasbot' WHERE `entry` = 37598;
DELETE FROM `spell_script_names` WHERE `spell_id` = 70253;
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES (70253,'spell_kezan_gasbot_master');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (34668,34693,34872,35053,35054,35063,35075,35120,35185,35201,35207,35209,35210,35294,35609,35623,37054,37056,37057,37500,37602,37708,37709,37710,37761,37762,37804,48494,48719,48721,48806,48949,49218,51411);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 34668 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(34668,0,0,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Sassy Hardwrench - On Gossip Hello - Play Random Emote (1, 3, 5, 6)'),
(34668,0,1,0,'',19,0,100,0,14138,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Sassy Hardwrench - On Quest '''' Taken - Say Line 0'),
(34668,0,2,0,'',20,0,100,0,25473,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Sassy Hardwrench - On Quest '''' Finished - Say Line 1'),
(34668,0,3,0,'',19,0,100,0,14070,0,0,0,0,1,2,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Sassy Hardwrench - On Quest '''' Taken - Say Line 2'),
(34668,0,4,0,'',20,0,100,0,14070,0,0,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Sassy Hardwrench - On Quest '''' Finished - Play Emote 4'),
(34668,0,5,0,'',19,0,100,0,14115,0,0,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Sassy Hardwrench - On Quest '''' Taken - Say Line 3'),
(34668,0,6,0,'',20,0,100,0,14115,0,0,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Sassy Hardwrench - On Quest '''' Finished - Play Emote 4'),
(34668,0,7,0,'',19,0,100,0,14116,0,0,0,0,1,4,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Sassy Hardwrench - On Quest '''' Taken - Say Line 4'),
(34668,0,8,0,'',19,0,100,0,14122,0,0,0,0,1,5,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Sassy Hardwrench - On Quest '''' Taken - Say Line 5'),
(34668,0,11,0,'',20,0,100,0,14122,0,0,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Sassy Hardwrench - On Quest '''' Finished - Play Emote 4'),
(34668,0,12,0,'',20,0,100,0,14125,0,0,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Sassy Hardwrench - On Quest '''' Finished - Play Emote 4'),
(34668,0,14,0,'',20,0,100,0,24520,0,0,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Sassy Hardwrench - On Quest '''' Finished - Play Emote 4'),
(34668,0,15,0,'',19,0,100,0,14125,0,0,0,0,1,6,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Sassy Hardwrench - On Quest '''' Taken - Say Line 6'),
(34668,0,16,0,'',19,0,100,0,14126,0,0,0,0,1,7,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Sassy Hardwrench - On Quest '''' Taken - Say Line 7');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 34693 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(34693,0,0,0,'',60,0,100,0,1000,1000,500000,500000,0,11,1784,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Slinky Sharpshiv - On Update - Cast ''Stealth'''),
(34693,0,1,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Slinky Sharpshiv - On Gossip Hello - Play Random Emote (1, 3, 5, 6)'),
(34693,0,2,0,'',20,0,100,0,14123,0,0,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Slinky Sharpshiv - On Quest '''' Finished - Play Emote 4'),
(34693,0,3,0,'',19,0,100,0,14115,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Slinky Sharpshiv - On Quest '''' Taken - Say Line 0'),
(34693,0,4,0,'',60,0,100,0,5000,5000,510000,510000,0,28,1784,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Slinky Sharpshiv - On Update - Remove Aura ''Stealth'''),
(34693,0,5,0,'',19,0,100,0,14010,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Slinky Sharpshiv - On Quest '''' Taken - Say Line 1'),
(34693,0,6,0,'',20,0,100,0,14010,0,0,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Slinky Sharpshiv - On Quest '''' Finished - Play Emote 4');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 34872 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(34872,0,0,0,'',20,0,100,0,14138,0,0,0,0,1,0,3000,0,0,0,0,1,0,0,0,0,0,0,0,0,'Foreman Dampwick - On Quest '''' Finished - Say Line 0'),
(34872,0,1,2,'',52,0,100,0,0,34872,0,0,0,11,87226,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Foreman Dampwick - On Text 0 Over - Cast ''Cosmetic - Rocket Barrage (Goblin Racial)'''),
(34872,0,2,0,'',61,0,100,0,0,0,0,0,0,1,1,3000,0,0,0,0,1,0,0,0,0,0,0,0,0,'Foreman Dampwick - On Text 0 Over - Say Line 1'),
(34872,0,3,0,'',52,0,100,0,1,34872,0,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Foreman Dampwick - On Text 1 Over - Say Line 2'),
(34872,0,4,0,'',19,0,100,0,25473,0,0,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Foreman Dampwick - On Quest '''' Taken - Say Line 3'),
(34872,0,5,0,'',19,0,100,0,14124,0,0,0,0,1,4,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Foreman Dampwick - On Quest '''' Taken - Say Line 4'),
(34872,0,6,0,'',1,0,100,0,45000,245000,45000,245000,0,1,5,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Foreman Dampwick - Out of Combat - Say Line 5'),
(34872,0,7,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Foreman Dampwick - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 35053 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(35053,0,0,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Candy Cane - On Gossip Hello - Play Random Emote (1, 3, 5, 6)'),
(35053,0,1,0,'',19,0,100,0,26712,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Candy Cane - On Quest '''' Taken - Say Line 0'),
(35053,0,2,0,'',19,0,100,0,14113,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Candy Cane - On Quest '''' Taken - Say Line 1');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 35054 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(35054,0,0,0,'',19,0,100,0,26711,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Chip Endale - On Quest '''' Taken - Say Line 0'),
(35054,0,1,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Chip Endale - On Gossip Hello - Play Random Emote (1, 3, 5, 6)'),
(35054,0,2,0,'',20,0,100,0,14110,0,0,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Chip Endale - On Quest '''' Finished - Play Emote 4'),
(35054,0,3,0,'',19,0,100,0,14153,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Chip Endale - On Quest '''' Taken - Say Line 1');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 35063 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(35063,0,0,0,'',60,0,100,0,1000,5000,1000,25000,0,89,20,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Kezan Citizen - On Update - Start Random Movement');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 35075 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(35075,0,0,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Kezan Citizen - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 35120 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(35120,0,0,0,'',20,0,100,0,26712,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'FBoK Bank Teller - On Quest '''' Finished - Say Line 0'),
(35120,0,1,0,'',20,0,100,0,26711,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'FBoK Bank Teller - On Quest '''' Finished - Say Line 0'),
(35120,0,2,0,'',1,0,100,0,15000,60000,15000,60000,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'FBoK Bank Teller - Out of Combat - Say Line 1'),
(35120,0,3,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'FBoK Bank Teller - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 35185 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(35185,0,0,0,'',60,0,100,0,1000,5000,5000,25000,0,89,24,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Kezan Partygoer - On Update - Start Random Movement'),
(35185,0,1,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Kezan Partygoer - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 35201 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(35201,0,0,0,'',60,0,100,0,1000,9000,10000,40000,0,5,18,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Kezan Partygoer - On Update - Play Emote 18'),
(35201,0,1,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Kezan Partygoer - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 35207 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(35207,0,0,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Ace - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 35209 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(35209,0,0,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Gobber - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 35210 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(35210,0,0,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Izzy - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 35294 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(35294,0,0,0,'',4,0,80,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rebellious Troll - On Aggro - Say Line 0 (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 35609 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(35609,0,0,0,'',1,0,100,1,500,1000,0,0,0,11,76136,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Villa Mook - Out of Combat - Cast ''Acidic Sweat'' (No Repeat)'),
(35609,0,1,0,'',4,0,100,1,0,0,0,0,0,11,32064,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Villa Mook - On Aggro - Cast ''Battle Shout'' (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 35623 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(35623,0,0,0,'',1,0,100,1,500,1000,0,0,0,11,76136,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Villa Mook - Out of Combat - Cast ''Acidic Sweat'' (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 37054 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(37054,0,0,0,'',1,0,100,1,1000,1000,1000,1000,0,11,51733,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin Supermodel - Out of Combat - Cast ''Shadow Channelling'' (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 37056 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(37056,0,0,0,'',1,0,100,1,1000,1000,1000,1000,0,11,51733,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin Supermodel - Out of Combat - Cast ''Shadow Channelling'' (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 37057 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(37057,0,0,0,'',1,0,100,1,1000,1000,1000,1000,0,11,51733,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Goblin Supermodel - Out of Combat - Cast ''Shadow Channelling'' (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 37500 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(37500,0,0,0,'',60,0,100,0,15000,60000,30000,350000,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Vinny Slapchop - On Update - Say Line 0'),
(37500,0,1,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Vinny Slapchop - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 37602 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(37602,0,0,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Claims Adjuster - On Gossip Hello - Play Random Emote (1, 3, 5, 6)'),
(37602,0,1,0,'',20,0,100,0,14125,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Claims Adjuster - On Quest '''' Finished - Say Line 0');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 37708 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(37708,0,0,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Izzy - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 37709 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(37709,0,0,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Ace - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 37710 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(37710,0,0,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Gobber - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 37761 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(37761,0,0,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Sally Salvager Sandscrew - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 37762 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(37762,0,0,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Brett Coins McQuid - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 37804 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(37804,0,0,0,'',1,0,100,0,30000,120000,30000,120000,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Kaja''Cola Balloon - Out of Combat - Say Line 0');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 48494 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(48494,0,0,0,'',60,0,100,0,5000,5000,360000,360000,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 0'),
(48494,0,1,0,'',60,0,100,0,25000,25000,380000,380000,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 1'),
(48494,0,2,0,'',60,0,100,0,50000,50000,405000,405000,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 2'),
(48494,0,3,0,'',60,0,100,0,60000,60000,415000,415000,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 3'),
(48494,0,4,0,'',60,0,100,0,75000,75000,430000,430000,0,1,4,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 4'),
(48494,0,5,0,'',60,0,100,0,85000,85000,440000,440000,0,1,5,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 5'),
(48494,0,6,0,'',60,0,100,0,90000,90000,445000,445000,0,1,6,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 6'),
(48494,0,7,0,'',60,0,100,0,110000,110000,465000,465000,0,1,7,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 7'),
(48494,0,8,0,'',60,0,100,0,115000,115000,470000,470000,0,1,8,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 8'),
(48494,0,9,0,'',60,0,100,0,120000,120000,475000,475000,0,1,9,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 9'),
(48494,0,10,0,'',60,0,100,0,125000,125000,480000,480000,0,1,10,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 10'),
(48494,0,11,0,'',60,0,100,0,135000,135000,490000,490000,0,1,11,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 11'),
(48494,0,12,0,'',60,0,100,0,140000,140000,495000,495000,0,1,12,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 12'),
(48494,0,13,0,'',60,0,100,0,145000,145000,500000,500000,0,1,13,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 13'),
(48494,0,14,0,'',60,0,100,0,165000,165000,520000,520000,0,1,14,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 14'),
(48494,0,15,0,'',60,0,100,0,185000,185000,540000,540000,0,1,15,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 15'),
(48494,0,16,0,'',60,0,100,0,195000,195000,550000,550000,0,1,16,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 16'),
(48494,0,17,0,'',60,0,100,0,200000,200000,555000,555000,0,1,17,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 17'),
(48494,0,18,0,'',60,0,100,0,210000,210000,565000,565000,0,1,18,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 18'),
(48494,0,19,0,'',60,0,100,0,230000,230000,585000,585000,0,1,19,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 19'),
(48494,0,20,0,'',60,0,100,0,235000,235000,590000,590000,0,1,20,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 20'),
(48494,0,21,0,'',60,0,100,0,270000,270000,625000,625000,0,1,21,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Update - Say Line 21'),
(48494,0,22,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Hobart Grapplehammer - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 48719 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(48719,0,0,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'KTC Waiter - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 48721 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(48721,0,0,0,'',1,0,100,0,500,500,500,500,0,12,48340,7,0,0,0,0,1,0,0,0,0,0,0,0,0,'KTC Waitress - Out of Combat - Summon Creature ''Drink Tray'''),
(48721,0,1,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'KTC Waitress - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 48806 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(48806,0,0,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'KTC Waitress - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 48949 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(48949,0,0,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Kezan Socialite - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 49218 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(49218,0,0,0,'',1,0,100,1,500,1000,0,0,0,11,76136,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Villa Mook - Out of Combat - Cast ''Acidic Sweat'' (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 51411 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(51411,0,0,0,'',64,0,100,0,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,0,'Neill Penny - On Gossip Hello - Play Random Emote (1, 3, 5, 6)');
DELETE FROM `creature_text` WHERE `CreatureID` = 34668 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 34668 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 34668 AND `GroupID` = 2;
DELETE FROM `creature_text` WHERE `CreatureID` = 34668 AND `GroupID` = 3;
DELETE FROM `creature_text` WHERE `CreatureID` = 34668 AND `GroupID` = 4;
DELETE FROM `creature_text` WHERE `CreatureID` = 34668 AND `GroupID` = 5;
DELETE FROM `creature_text` WHERE `CreatureID` = 34668 AND `GroupID` = 6;
DELETE FROM `creature_text` WHERE `CreatureID` = 34668 AND `GroupID` = 7;
DELETE FROM `creature_text` WHERE `CreatureID` = 34693 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 34693 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 34697 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 34872 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 34872 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 34872 AND `GroupID` = 2;
DELETE FROM `creature_text` WHERE `CreatureID` = 34872 AND `GroupID` = 3;
DELETE FROM `creature_text` WHERE `CreatureID` = 34872 AND `GroupID` = 4;
DELETE FROM `creature_text` WHERE `CreatureID` = 34872 AND `GroupID` = 5;
DELETE FROM `creature_text` WHERE `CreatureID` = 35053 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 35053 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 35053 AND `GroupID` = 2;
DELETE FROM `creature_text` WHERE `CreatureID` = 35054 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 35054 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 35120 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 35120 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 35294 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 37500 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 37602 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 37804 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 37804 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 37804 AND `GroupID` = 2;
DELETE FROM `creature_text` WHERE `CreatureID` = 37804 AND `GroupID` = 4;
DELETE FROM `creature_text` WHERE `CreatureID` = 37804 AND `GroupID` = 5;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 10;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 11;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 12;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 13;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 14;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 15;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 16;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 17;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 18;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 19;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 2;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 20;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 21;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 4;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 5;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 6;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 7;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 8;
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` = 9;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(34668,0,0,'Haha! that should be a blast!',12,0,100,25,0,0,43954,'say Text'),
(34668,1,0,'He''ll get me more later? How are we supposed to get you promoted to Trade Prince without our number one product on the shelves?',12,0,100,1,0,0,49018,'say Text'),
(34668,2,0,'Ace,Izzy,Gobber,you three make sure you tag a long with $N and help him take care of those dead beats!',12,0,100,1,0,0,49021,'say Text'),
(34668,3,0,'Go get ''em,boss!',12,0,100,1,0,0,37318,'say Text'),
(34668,4,0,'You better get upstairs pronto, boss. Trade Prince Gallywix is waiting for you.',12,0,100,1,0,0,49015,'say Text'),
(34668,5,0,'$N, you have to hurry and break your moolah out of the bank!',12,0,100,1,0,0,49016,'say Text'),
(34668,6,0,'I can''t stand to watch. So many good memories. So much profit.',12,0,100,1,0,0,49071,'say Text'),
(34668,7,0,'The Trade Prince''s yacht is about to set sail. Everyone, get to the dock! Hand over the keys, boss. I''m driving. Just let me know when you''re ready to go.',12,0,100,1,0,0,40278,'say Text'),
(34693,0,0,'I''ll address the CAP so that it activates automatically when you''re at the villa. Do just well warning avoid pigs!',12,0,100,1,0,0,0,'say Text'),
(34693,1,0,'All what you have to do is let me show you how was a Visca cooperation. Then, head to one of the models to lead training on which Bamm is going to pull, start a pernicious attack and finish it with a Visca cooperation.',12,0,100,1,0,0,0,'say Text'),
(34697,0,0,'Insert your coins inside the Warrior NX-01, who''s learn you. Then use the technique on one of the models to lead training.',12,0,100,0,0,0,0,'say Text'),
(34872,0,0,'It is always pleasant to receive a gift from the hand of the second in person. I''m going to the baller immediately!',12,0,100,1,0,0,0,'say Text'),
(34872,1,0,'OUCH!',14,0,100,34,0,0,0,'say Text'),
(34872,2,0,'Okay, okay! I understand the message!',12,0,100,1,0,0,0,'say Text'),
(34872,3,0,'Chief, tell Chipie I''ll kick him if  Kaja''Cola  will get more later then that production has resumed.',12,0,100,1,0,0,0,'say Text'),
(34872,4,0,'I can''t to believe the nerve of those trolls. Rebelling after all that I''ve done for them!',12,0,100,1,0,0,0,'say Text'),
(34872,5,0,'Useless! You are simply useless! I fear that we are all obligate to throw you into the Maelstrom!',14,0,50,22,0,0,0,'say Text'),
(34872,5,1,'You want us go bash bad trolls?',12,0,50,0,0,0,0,'say Text'),
(35053,0,0,'Make sure you pull plenty of moolah out of the bank. I don''t want you buying any of that cheap clothing you usually like to wear. This party''s important!',12,0,100,1,0,0,49023,'say Text'),
(35053,1,0,'You''re the most handsome boyfriend a girl could hope for. Knock''em dead, honey!',12,0,100,1,0,0,49012,'say Text'),
(35053,2,0,'You are dresed Impress! Use your new powers below to make your party guests happy!',41,0,100,0,0,0,35444,'say Text'),
(35054,0,0,'Make sure you pull plenty of moolah out of the bank. I don''t want you buying any of that cheap clothing you usually like to wear. This party''s important!',12,0,100,1,0,0,49023,'say Text'),
(35054,1,0,'Look at you! I''m the luckiest boyfriend on the island!!!',42,0,100,1,0,0,49013,'say Text'),
(35120,0,0,'You people cool your heels! $N is here. He''s gonna be the next Trade Prince!',12,0,100,1,0,0,49024,'say Text'),
(35120,1,0,'Next!',12,0,100,1,0,0,24357,'say Text'),
(35294,0,0,'I not gonna take it anymore,mon!',12,0,50,0,0,0,49041,'on Aggro Text'),
(35294,0,1,'Kezan for da trolls!',12,0,50,0,0,0,49042,'on Aggro Text'),
(35294,0,2,'Ya got some nerve,mon!',12,0,50,0,0,0,49039,'on Aggro Text'),
(35294,0,3,'Maybe I take dem bombs from ya an'' we see who go boom!',12,0,50,0,0,0,49044,'on Aggro Text'),
(37500,0,0,'Volcano rocks - new low price! You''re gonna love my rocks.',14,0,50,1,0,0,37100,'say Text'),
(37500,0,1,'Volcano rocks for sale! Hot off the mountain! You want ''em, I got ''em!',14,0,50,1,0,0,37099,'say Text'),
(37500,0,2,'Take home a piece of the Cataclysm! Volcano rocks for sale - hot, Hot, HOT!',14,0,50,1,0,0,37102,'say Text'),
(37500,0,3,'Get your volcano rocks here! ...Great for cats!',14,0,50,1,0,0,37101,'say Text'),
(37500,0,4,'Volcano rocks. Million and one uses. Lava, java, sauna, banana....',14,0,50,1,0,0,37104,'say Text'),
(37500,0,5,'Papers to weigh? Doors to stop? Windows to break? bam, Bam, BAM! Volcano rocks.',14,0,50,1,0,0,37103,'say Text'),
(37500,0,6,'Volcano rocks! Limited time offer! Papers of authenticity and everything!',14,0,50,1,0,0,37106,'say Text'),
(37602,0,0,'Still an unfortunate accident of fire-fireworks-lit flammable-gas - Lectriquea?',14,0,100,0,0,0,0,'say Text'),
(37804,0,0,'This party lacks a bit of brilliant',12,0,100,10,0,0,0,'say Text'),
(37804,1,0,'Thanks !',12,0,100,17,0,0,0,'say Text'),
(37804,2,0,'Passport delicious! And these are just the out-of A "work?',12,0,100,7,0,0,0,'say Text'),
(37804,4,0,'This white Pa wine and sparkling is delicious. Where have you found?',12,0,50,17,0,0,0,'say Text'),
(37804,4,1,'Thank you. I was soon offcourse. Yum yum!',12,0,50,7,0,0,0,'say Text'),
(37804,5,0,'I need a bucket!',12,0,50,1,0,0,0,'say Text'),
(37804,5,1,'I am hungry! But where is the food good blood?',14,0,50,6,0,0,0,'say Text'),
(37804,5,2,'If only I had someone with whom dance.',12,0,50,5,0,0,0,'say Text'),
(37804,5,3,'I love Fireworks! It is necessary that its repeats in the eardrum and it we have blouisse the view.',12,0,50,5,0,0,0,'say Text'),
(37804,5,4,'I love Fireworks!',12,0,50,5,0,0,0,'say Text'),
(37804,5,5,'Ah the love! Made go to your buttock Wouhaya Boniface!',12,0,50,21,0,0,0,'say Text'),
(48494,1,0,'There''s a great deal that you do not understand, my dear Greely. That is why you are the assistant and I... I am Hobart Grapplehammer!',12,0,100,1,0,0,49103,'say Text'),
(48494,2,0,'Excellent. Now back away from the Ultra Concave Fissionable Testing Platform. Slowly, if you please.',12,0,100,1,0,0,49107,'say Text'),
(48494,4,0,'I am going to end the horrible problem of food storage cutting into the profits of the Bilgewater Cartel!',12,0,100,1,0,0,49110,'say Text'),
(48494,5,0,'I''ll soon be revered as the greatest goblin tinker of all times! Besides, that negligence case involving the creation of the gilgoblins will never stick.',12,0,100,1,0,0,49111,'say Text'),
(48494,6,0,'Flip the switch!',12,0,100,1,0,0,49112,'say Text'),
(48494,7,0,'Quite right, Greely. You passed today''s pop quiz.',12,0,100,1,0,0,49115,'say Text'),
(48494,8,0,'I am now flipping the switch!',12,0,100,1,0,0,49116,'say Text'),
(48494,9,0,'Greely, can you feel it? My moment of success is at hand!',12,0,100,1,0,0,49117,'say Text'),
(48494,10,0,'Grocery store owners everywhere will owe me a percentage of their profits!',12,0,100,1,0,0,49118,'say Text'),
(48494,11,0,'I''m flipping the second switch!',12,0,100,1,0,0,49119,'say Text'),
(48494,12,0,'The micro mechachicken begins to grow and Greely starts sneaking away from the machine.',16,0,100,0,0,0,0,'say Text'),
(48494,13,0,'Wait... something''s wrong!',12,0,100,0,0,0,49120,'say Text'),
(48494,14,0,'I know... I know! Where''s the override? The override?!',14,0,100,0,0,0,49122,'say Text'),
(48494,15,0,'Here goes nothing, I always loved you, Greely!',14,0,100,0,0,0,49124,'say Text'),
(48494,16,0,'Flipping the override switch now!',14,0,100,0,0,0,49125,'say Text'),
(48494,17,0,'The micro mechachicken quickly shrinks back to normal.',16,0,100,0,0,0,0,'say Text'),
(48494,18,0,'Are we still alive?',12,0,100,0,0,0,49127,'say Text'),
(48494,19,0,'Well of course we are, my dear Greely. And look there. The Micro Mechachicken''s stabilization appears to be a complete success!',12,0,100,1,0,0,49129,'say Text'),
(48494,20,0,'The micro mechachicken explodes.',16,0,100,0,0,0,0,'say Text'),
(48494,21,0,'I haven''t the foggiest notion what you''re talking about, Miss Greely.',12,0,100,1,0,0,49133,'say Text');
DELETE FROM `creature_text` WHERE `CreatureID` = 48494 AND `GroupID` IN (0,3);
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(48494,0,0,'Prepare the Micro Mechachicken!',14,0,100,1,0,0,49101,'say Text'),
(48494,3,0,'Thank you. Now behold as I stabilize the highly unstable miniaturization process.',12,0,100,1,0,0,49108,'say Text');
