-- New Tinkertown: Muffinus Chromebrew spawned, SmartAI of the starting zone NPCs
DELETE FROM `creature` WHERE `guid` = 146941535;
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`PhaseId`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`MovementType`) VALUES
(146941535,103614,0,6457,133,1,1,'',-5117.27,430.642,402.991,3.33525,120,0);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (1132,1260,13076,42184,42221,42222,42223,42226,42286,42290,42324,42328,42329,42396,42489,42505,42739,42773,42894,42945,43230,43325,45966,46025,46208,46230,46267,46274,47250,47323,47349,47350,47351,47359,63238,103618);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1132 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1132,0,0,0,'',0,0,100,0,5000,7000,24000,28000,0,11,3150,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Timber - In Combat - Cast ''Rabies''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1260 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1260,0,0,1,'',2,0,100,1,0,30,0,0,0,11,18501,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Great Father Arctikus - Between 0-30% Health - Cast ''Enrage'' (No Repeat)'),
(1260,0,1,0,'',61,0,100,1,0,30,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Great Father Arctikus - Between 0-30% Health - Say Line 0 (No Repeat)'),
(1260,0,2,0,'',4,0,100,1,0,0,0,0,0,11,77808,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Great Father Arctikus - On Aggro - Cast ''Battle Shout'' (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 13076 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(13076,0,0,0,'',0,0,100,0,0,0,2300,3900,0,11,95826,64,0,0,0,0,2,0,0,0,0,0,0,0,0,'Dun Morogh Mountaineer - In Combat - Cast ''Shoot''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42184 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42184,0,0,0,'',0,0,100,0,5000,8000,19000,25000,0,11,84150,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Toxic Sludge - In Combat - Cast ''Toxic Residue''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42221 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42221,0,0,0,'',4,0,100,0,0,0,0,0,0,28,77831,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rockjaw Bonepicker - On Aggro - Cast ''Bone Toss'''),
(42221,0,1,0,'',0,0,100,0,0,0,3000,4000,0,11,82625,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Rockjaw Bonepicker - Combat - Cast ''Bone Toss''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42222 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42222,0,0,0,'',0,0,100,0,3500,4500,12000,13000,0,11,69851,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Rockjaw Marauder - In Combat - Cast ''Club''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42223 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42223,0,0,0,'',0,0,100,0,4500,6500,18000,19000,0,11,84148,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Irradiated Technician - In Combat - Cast ''Irradiation Gun''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42226 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42226,0,0,0,'',9,0,100,0,0,5,18000,22000,0,11,84146,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Crushcog Battle Suit - Within 0-5 Range - Cast ''Razor Edge''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42286 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42286,0,0,0,'',4,0,100,1,0,0,0,0,0,11,32720,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Young Snow Leopard - On Aggro - Cast ''Sprint'' (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42290 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42290,0,0,1,'',1,0,10,0,30000,600000,120000,600000,0,4,1018,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Winter Wolf - Out of Combat - Play Sound 1018'),
(42290,0,1,0,'',61,0,100,0,30000,600000,120000,600000,0,5,393,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Winter Wolf - Out of Combat - Play Emote 393'),
(42290,0,2,0,'',0,0,100,0,6000,7000,18000,20000,0,11,32919,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Winter Wolf - In Combat - Cast ''Snarl''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42324 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42324,0,0,0,'',11,0,100,0,0,0,0,0,0,53,0,42324,1,0,0,0,1,0,0,0,0,0,0,0,0,'Drill Sergeant Steamcrank - On Spawn - Start WP'),
(42324,0,1,0,'',64,0,100,0,0,0,0,0,0,54,30000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Drill Sergeant Steamcrank - On Gossip Hello - Pause WP'),
(42324,0,2,3,'',40,0,100,0,2,42324,0,0,0,54,6000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Drill Sergeant Steamcrank - On Reached WP2 - Pause WP'),
(42324,0,3,0,'',61,0,100,0,0,0,0,0,0,80,4232400,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Drill Sergeant Steamcrank - On Reached WP2 - Run Script'),
(42324,0,4,5,'',40,0,100,0,3,42324,0,0,0,54,9000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Drill Sergeant Steamcrank - On Reached WP3 - Pause WP'),
(42324,0,5,0,'',61,0,100,0,0,0,0,0,0,80,4232400,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Drill Sergeant Steamcrank - On Reached WP3 - Run Script'),
(42324,0,6,7,'',40,0,100,0,5,42324,0,0,0,54,7000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Drill Sergeant Steamcrank - On Reached WP5 - Pause WP'),
(42324,0,7,0,'',61,0,100,0,0,0,0,0,0,80,4232400,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Drill Sergeant Steamcrank - On Reached WP5 - Run Script'),
(42324,0,8,9,'',40,0,100,0,6,42324,0,0,0,54,8000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Drill Sergeant Steamcrank - On Reached WP6 - Pause WP'),
(42324,0,9,0,'',61,0,100,0,0,0,0,0,0,80,4232400,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Drill Sergeant Steamcrank - On Reached WP6 - Run Script');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42328 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42328,0,0,0,'',8,0,100,0,42880,0,0,0,0,5,33,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Practice Dummy - On Spellhit - Cast ''Wound Impact'''),
(42328,0,1,0,'',8,0,100,0,78959,0,0,0,0,5,33,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Practice Dummy - On Spellhit - Cast ''Wound Impact'''),
(42328,0,2,0,'',8,0,100,0,44079,0,0,0,0,11,78961,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Practice Dummy - On Spellhit - Cast ''Wound Impact Critical''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42329 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42329,0,0,0,'',1,0,100,0,1000,5000,5000,5000,0,87,4232900,4232901,4232902,0,0,0,1,0,0,0,0,0,0,0,0,'Gnomeregan Trainee - OOC - Run Random Script');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42396 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42396,0,0,0,'',1,0,100,0,10000,10000,75000,75000,0,87,4239600,4239601,4239602,0,0,0,1,0,0,0,0,0,0,0,0,'Nevin Twistwrench - OOC - Run Random Script'''),
(42396,0,1,0,'',19,0,100,0,26197,0,0,0,0,85,88340,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Nevin Twistwrench - On Quest Accept - Invoker Cast ''Summon Mage Guide'''),
(42396,0,2,0,'',19,0,100,0,26199,0,0,0,0,85,88347,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Nevin Twistwrench - On Quest Accept - Invoker Cast ''Summon Priest Guide'''),
(42396,0,3,0,'',19,0,100,0,26202,0,0,0,0,85,88303,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Nevin Twistwrench - On Quest Accept - Invoker Cast ''Summon Warlock Guide'''),
(42396,0,4,0,'',19,0,100,0,26203,0,0,0,0,85,88326,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Nevin Twistwrench - On Quest Accept - Invoker Cast ''Summon Warrior Guide'''),
(42396,0,5,0,'',19,0,100,0,26206,0,0,0,0,85,88328,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Nevin Twistwrench - On Quest Accept - Invoker Cast ''Summon Rogue Guide'''),
(42396,0,6,0,'',19,0,100,0,41217,0,0,0,0,85,205137,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Nevin Twistwrench - On Quest Accept - Invoker Cast ''Summon Hunter Guide''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42489 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42489,0,0,0,'',1,0,100,0,30000,30000,300000,300000,0,80,4248900,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Captain Tread Sparknozzle - OOC - Run Script');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42505 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42505,0,0,0,'',25,0,100,1,0,0,0,0,0,11,16245,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Image of Razlo Crushcog - On Reset - Cast Freeze Anim'''),
(42505,0,1,0,'',38,0,100,0,1,1,10000,10000,0,53,0,42505,0,0,0,0,1,0,0,0,0,0,0,0,0,'Image of Razlo Crushcog - On Data Set - Load Path'''),
(42505,0,2,0,'',40,0,100,0,2,42505,0,0,0,80,4250500,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Image of Razlo Crushcog - On Reached WP2 - Run Script');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42739 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42739,0,0,1,'',38,0,100,0,1,1,10000,10000,0,11,76841,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Frostmane Hold Target - On Data Set - Cast ''Cosmetic - Explosion'''),
(42739,0,1,2,'',61,0,100,0,0,0,0,0,0,11,66772,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Frostmane Hold Target - On Data Set - Cast ''Earthquake Camera Shake'''),
(42739,0,2,3,'',61,0,100,0,0,0,0,0,0,11,79695,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Frostmane Hold Target - On Data Set - Cast ''Trigger Powder Keg Explosions'''),
(42739,0,3,0,'',61,0,100,0,0,0,0,0,0,80,4273900,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Frostmane Hold Target - On Data Set - Cast ''Trigger Powder Keg Explosions''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42773 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42773,0,0,1,'',2,0,100,1,0,30,0,0,0,11,8599,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Boss Bruggor - Between 0-30% Health - Cast ''Enrage'' (No Repeat)'),
(42773,0,1,0,'',61,0,100,1,0,30,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Boss Bruggor - Between 0-30% Health - Say Line 0 (No Repeat)'),
(42773,0,2,0,'',0,0,100,0,5000,7000,18000,22000,0,11,69851,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Boss Bruggor - In Combat - Cast ''Club''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42894 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42894,0,0,0,'',1,0,100,0,3000,5000,3000,5000,0,11,80054,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Trogg Tunnel - OOC - Cast ''Boulder Impact''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42945 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42945,0,0,0,'',25,0,100,1,0,0,0,0,0,11,29266,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'GS-9x Prototype - On Reset - Cast ''Permanent Feign Death'''),
(42945,0,1,0,'',38,0,100,0,1,1,10000,10000,0,80,4294500,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'GS-9x Prototype - On Data Set - Run Script'),
(42945,0,2,3,'',40,0,100,0,8,42945,0,0,0,11,46419,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Image of Razlo Crushcog - On Reached WP8 - Cast ''Cosmetic - Explosion'''),
(42945,0,3,0,'',61,0,100,0,0,0,0,0,0,41,1000,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Image of Razlo Crushcog - On Reached WP8 - Despawn');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 43230 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(43230,0,0,0,'',0,0,100,0,4500,6500,18000,19000,0,11,84148,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Crushcog Technician - In Combat - Cast ''Irradiation Gun''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 43325 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(43325,0,0,0,'',4,0,100,0,0,0,0,0,0,28,77831,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rockjaw Fungus-Flinger - Aggro - Remove Aura ''Trogg Sleep'''),
(43325,0,1,0,'',0,0,100,0,0,0,2300,3900,0,11,80944,64,0,0,0,0,2,0,0,0,0,0,0,0,0,'Rockjaw Fungus-Flinger - In Combat - Cast ''Fling Fungus'''),
(43325,0,2,0,'',9,0,100,0,10,20,17000,22000,0,11,87347,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Rockjaw Fungus-Flinger - Within 10-20 Range - Cast ''Poisonous Mushroom''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 45966 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(45966,0,0,1,'',19,0,100,0,28167,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Nevin Twistwrench - On Quest Accept - Say 0'),
(45966,0,1,0,'',61,0,100,0,0,0,0,0,0,85,80653,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Nevin Twistwrench - On Quest Accept - Invoker Cast ''Irradiated'''),
(45966,0,2,0,'',1,0,100,0,1000,1000,60000,60000,0,12,46449,1,58000,0,0,0,8,0,0,0,0,-4958.17,827.382,285.898,2.72271,'Nevin Twistwrench - OOC - Spawn ''S.A.F.E. Operative''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 46025 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(46025,0,0,1,'',54,0,100,0,0,0,0,0,0,11,7791,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Officer - On Summon - Cast ''Teleport'''),
(46025,0,1,0,'',61,0,100,0,0,0,0,0,0,80,4602500,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Officer - On Summon - Run Script'),
(46025,0,2,0,'',40,0,100,0,10,42396,0,0,0,41,2000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Officer - On Reached WP10 - Despawn');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 46208 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(46208,0,0,1,'',11,0,100,0,0,0,0,0,0,3,0,27669,0,0,0,0,1,0,0,0,0,0,0,0,0,'Clean Cannon X-2  - On Spawn - Set Model'),
(46208,0,1,0,'',61,0,100,0,0,0,0,0,0,66,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Clean Cannon X-2  - On Spawn - Set Orientaion self');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 46230 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(46230,0,0,0,'',25,0,100,0,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - On Reset - Set Phase 1'),
(46230,0,1,0,'',38,1,100,0,1,2,0,0,0,80,4623000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - On Data Set 1 2 - Run Script'),
(46230,0,2,0,'',38,0,100,0,1,1,0,0,0,1,0,15000,15000,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - On Data Set 1 1 - Sat 0'),
(46230,0,3,4,'',54,0,100,0,0,0,0,0,0,11,7791,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - On Summon - Cast ''Teleport'''),
(46230,0,4,0,'',61,0,100,0,0,0,0,0,0,80,4623001,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - On Summon - Run Script'),
(46230,0,5,0,'',40,0,100,0,10,42396,0,0,0,41,2000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - On Reached WP10 - Despawn');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 46267 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(46267,0,0,1,'',54,0,100,0,0,0,0,0,0,11,7791,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rescued Survivor - On Summon - Cast ''Teleport'''),
(46267,0,1,0,'',61,0,100,0,0,0,0,0,0,80,4626700,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rescued Survivor - On Summon - Run Script'),
(46267,0,2,0,'',40,0,100,0,5,46267,0,0,0,80,4626701,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rescued Survivor - On Reached WP5 - Run Script');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 46274 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(46274,0,0,0,'',19,0,100,0,27635,0,0,0,0,85,80653,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Gaffer Coilspring <S.A.F.E.> - On Quest Accept - Invoker Cast ''Irradiated''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 47250 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(47250,0,0,1,'',19,0,100,0,28171,0,0,0,0,85,80653,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Carvo Blastbolt <S.A.F.E.> - On Quest Accept - Invoker Cast ''Irradiated'''),
(47250,0,1,2,'',19,0,100,0,28169,0,0,0,0,85,80653,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Carvo Blastbolt <S.A.F.E.> - On Quest Accept - Invoker Cast ''Irradiated'''),
(47250,0,2,0,'',61,0,100,0,0,0,0,0,0,85,88988,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Carvo Blastbolt <S.A.F.E.> - On Quest Accept - Invoker Cast ''Summon Loading Room Guide''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 47323 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(47323,0,0,0,'',1,0,100,1,1000,1000,1000,1000,0,66,0,0,0,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Turn to Summoner ''No Repeat'''),
(47323,0,1,0,'',1,0,100,1,3000,3000,3000,3000,0,1,0,0,1,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Say 0 ''No Repeat'''),
(47323,0,2,0,'',1,0,100,1,5000,5000,5000,5000,0,53,0,47323,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Load path ''No Repeat'''),
(47323,0,3,4,'',40,0,100,0,5,47323,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,3.59538,'S.A.F.E. Guide - On Reached WP5 - Turn to'),
(47323,0,4,5,'',61,0,100,0,0,0,0,0,0,1,1,0,1,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - On Reached WP5 - Say 1'),
(47323,0,5,0,'',61,0,100,0,0,0,0,0,0,41,6000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - On Reached WP5 - Despawn in 6 sec');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 47349 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(47349,0,0,0,'',1,0,100,1,1000,1000,1000,1000,0,66,0,0,0,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Turn to Summoner ''No Repeat'''),
(47349,0,1,0,'',1,0,100,1,3000,3000,3000,3000,0,1,0,0,1,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Say 0 ''No Repeat'''),
(47349,0,2,0,'',1,0,100,1,5000,5000,5000,5000,0,53,0,47349,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Load path ''No Repeat'''),
(47349,0,3,4,'',40,0,100,0,3,47349,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,5.13127,'S.A.F.E. Guide - On Reached WP3 - Turn to'),
(47349,0,4,5,'',61,0,100,0,0,0,0,0,0,1,1,0,1,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - On Reached WP3 - Say 1'),
(47349,0,5,0,'',61,0,100,0,0,0,0,0,0,41,6000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - On Reached WP3 - Despawn in 6 sec');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 47350 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(47350,0,0,0,'',1,0,100,1,1000,1000,1000,1000,0,66,0,0,0,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Turn to Summoner ''No Repeat'''),
(47350,0,1,0,'',1,0,100,1,3000,3000,3000,3000,0,1,0,0,1,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Say 0 ''No Repeat'''),
(47350,0,2,0,'',1,0,100,1,5000,5000,5000,5000,0,53,0,47350,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Load path ''No Repeat'''),
(47350,0,3,4,'',40,0,100,0,7,47350,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0.767945,'S.A.F.E. Guide - On Reached WP7 - Turn to'),
(47350,0,4,5,'',61,0,100,0,0,0,0,0,0,1,1,0,1,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - On Reached WP7 - Say 1'),
(47350,0,5,0,'',61,0,100,0,0,0,0,0,0,41,6000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - On Reached WP7 - Despawn in 6 sec');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 47351 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(47351,0,0,0,'',1,0,100,1,1000,1000,1000,1000,0,66,0,0,0,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Turn to Summoner ''No Repeat'''),
(47351,0,1,0,'',1,0,100,1,3000,3000,3000,3000,0,1,0,0,1,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Say 0 ''No Repeat'''),
(47351,0,2,0,'',1,0,100,1,5000,5000,5000,5000,0,53,0,47351,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Load path ''No Repeat'''),
(47351,0,3,4,'',40,0,100,0,9,47351,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,5.53269,'S.A.F.E. Guide - On Reached WP9 - Turn to'),
(47351,0,4,5,'',61,0,100,0,0,0,0,0,0,1,1,0,1,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - On Reached WP9 - Say 1'),
(47351,0,5,0,'',61,0,100,0,0,0,0,0,0,41,6000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - On Reached WP9 - Despawn in 6 sec');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 47359 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(47359,0,0,0,'',1,0,100,1,1000,1000,1000,1000,0,66,0,0,0,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Turn to Summoner ''No Repeat'''),
(47359,0,1,0,'',1,0,100,1,3000,3000,3000,3000,0,1,0,0,1,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Say 0 ''No Repeat'''),
(47359,0,2,0,'',1,0,100,1,5000,5000,5000,5000,0,53,0,47359,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Load path ''No Repeat'''),
(47359,0,3,4,'',40,0,100,0,8,47359,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,3.59538,'S.A.F.E. Guide - On Reached WP8 - Turn to'),
(47359,0,4,5,'',61,0,100,0,0,0,0,0,0,1,1,0,1,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - On Reached WP8 - Say 1'),
(47359,0,5,0,'',61,0,100,0,0,0,0,0,0,41,6000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - On Reached WP8 - Despawn in 6 sec');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 63238 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(63238,0,0,0,'',1,0,100,0,4000,5000,4000,5000,0,10,507,508,509,0,0,0,1,0,0,0,0,0,0,0,0,'Xi, Friend to the Small - OOC - Random Emote');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 103618 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(103618,0,0,0,'',1,0,100,1,1000,1000,1000,1000,0,66,0,0,0,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Turn to Summoner ''No Repeat'''),
(103618,0,1,0,'',1,0,100,1,3000,3000,3000,3000,0,1,0,0,1,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Say 0 ''No Repeat'''),
(103618,0,2,0,'',1,0,100,1,5000,5000,5000,5000,0,53,0,103618,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - OOC - Load path ''No Repeat'''),
(103618,0,3,4,'',40,0,100,0,8,103618,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,3.59538,'S.A.F.E. Guide - On Reached WP13 - Turn to'),
(103618,0,4,5,'',61,0,100,0,0,0,0,0,0,1,1,0,1,0,0,0,23,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - On Reached WP13 - Say 1'),
(103618,0,5,0,'',61,0,100,0,0,0,0,0,0,41,6000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Guide - On Reached WP13 - Despawn in 6 sec');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4232400 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4232400,9,0,0,'',0,0,100,0,0,0,0,0,0,66,0,0,0,0,0,0,19,42329,5,0,0,0,0,0,0,'Drill Sergeant Steamcrank - Script - Turn to closest Trainee'),
(4232400,9,1,0,'',0,0,100,0,500,500,0,0,0,10,5,25,273,0,0,0,1,0,0,0,0,0,0,0,0,'Drill Sergeant Steamcrank - Script - Play random emote');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4232900 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4232900,9,0,0,'',0,0,100,0,0,0,0,0,0,11,42880,0,0,0,0,0,9,42328,0,2,0,0,0,0,0,'Gnomeregan Trainee - OOC - Cast ''Cosmetic - Combat Attack 1H''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4232901 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4232901,9,0,0,'',0,0,100,0,0,0,0,0,0,11,44079,0,0,0,0,0,9,42328,0,2,0,0,0,0,0,'Gnomeregan Trainee - OOC - Cast ''Cosmetic - Combat Special Attack 1H''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4232902 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4232902,9,0,0,'',0,0,100,0,0,0,0,0,0,11,78959,0,0,0,0,0,9,42328,0,2,0,0,0,0,0,'Gnomeregan Trainee - OOC - Cast ''Cosmetic - Combat Attack 1H (Thrust)''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4239600 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4239600,9,0,0,'',0,0,100,0,0,0,0,0,0,12,45847,1,60000,0,0,0,8,0,0,0,0,-5201.76,478.174,388.543,5.09636,'Nevin Twistwrench - Script - Spawn ''S.A.F.E. Operative''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4239601 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4239601,9,0,0,'',0,0,100,0,0,0,0,0,0,12,46025,1,60000,0,0,0,8,0,0,0,0,-5201.76,478.174,388.543,5.09636,'Nevin Twistwrench - Script - Spawn ''S.A.F.E. Officer''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4239602 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4239602,9,0,0,'',0,0,100,0,0,0,0,0,0,12,46230,1,60000,0,0,0,8,0,0,0,0,-5201.76,478.174,388.543,5.09636,'Nevin Twistwrench - Script - Spawn ''S.A.F.E. Technician''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4248900 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4248900,9,0,0,'',0,0,100,0,1000,1000,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Captain Tread Sparknozzle - Script - Say 0'),
(4248900,9,1,0,'',0,0,100,0,7000,7000,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Captain Tread Sparknozzle - Script - Say 1'),
(4248900,9,2,0,'',0,0,100,0,10000,10000,0,0,0,1,0,0,0,0,0,0,19,42491,10,0,0,0,0,0,0,'Captain Tread Sparknozzle - Script - Hinkles Fastblast Say 0'),
(4248900,9,3,0,'',0,0,100,0,0,0,0,0,0,45,1,1,0,0,0,0,19,42505,10,0,0,0,0,0,0,'Captain Tread Sparknozzle - Script - Set Data'),
(4248900,9,4,0,'',0,0,100,0,8000,8000,0,0,0,1,1,0,0,0,0,0,19,42491,10,0,0,0,0,0,0,'Captain Tread Sparknozzle - Script - Hinkles Fastblast Say 1'),
(4248900,9,5,0,'',0,0,100,0,11000,11000,0,0,0,1,0,0,0,0,0,0,19,42366,10,0,0,0,0,0,0,'Captain Tread Sparknozzle - Script - Kelsey Steelspark Say 0'),
(4248900,9,6,0,'',0,0,100,0,9000,9000,0,0,0,1,0,0,0,0,0,0,19,42490,10,0,0,0,0,0,0,'Captain Tread Sparknozzle - Script - Elgin Clickspring Say 0'),
(4248900,9,7,0,'',0,0,100,0,0,0,0,0,0,12,43132,1,20000,0,0,0,8,0,0,0,0,-5138.01,498.262,396.42,1.50098,'Captain Tread Sparknozzle - Script - Spawn 43132'),
(4248900,9,8,0,'',0,0,100,0,0,0,0,0,0,12,43132,1,20000,0,0,0,8,0,0,0,0,-5138.44,498.372,396.421,1.41372,'Captain Tread Sparknozzle - Script - Spawn 43132'),
(4248900,9,9,0,'',0,0,100,0,0,0,0,0,0,12,43132,1,20000,0,0,0,8,0,0,0,0,-5138.9,498.429,396.436,1.41372,'Captain Tread Sparknozzle - Script - Spawn 43132'),
(4248900,9,10,0,'',0,0,100,0,4000,4000,0,0,0,12,43131,1,16000,0,0,0,8,0,0,0,0,-5139.73,499.203,396.42,6.19592,'Captain Tread Sparknozzle - Script - Spawn 43131'),
(4248900,9,11,0,'',0,0,100,0,0,0,0,0,0,12,43131,1,16000,0,0,0,8,0,0,0,0,-5139.75,499.571,396.456,6.19592,'Captain Tread Sparknozzle - Script - Spawn 43131'),
(4248900,9,12,0,'',0,0,100,0,0,0,0,0,0,12,43131,1,16000,0,0,0,8,0,0,0,0,-5139.76,499.977,396.443,6.19592,'Captain Tread Sparknozzle - Script - Spawn 43131'),
(4248900,9,13,0,'',0,0,100,0,3000,3000,0,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Captain Tread Sparknozzle - Script - Say 2'),
(4248900,9,14,0,'',0,0,100,0,0,0,0,0,0,5,4,0,0,0,0,0,11,43132,10,0,0,0,0,0,0,'Captain Tread Sparknozzle - Script - Image of Dwarf  Mountaineer oneshotcheer'),
(4248900,9,15,0,'',0,0,100,0,8000,8000,0,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Captain Tread Sparknozzle - Script - Say 3'),
(4248900,9,16,0,'',0,0,100,0,0,0,0,0,0,5,4,0,0,0,0,0,11,43131,10,0,0,0,0,0,0,'Captain Tread Sparknozzle - Script - Image of Gnomeregan Infantry oneshotcheer');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4250500 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4250500,9,0,0,'',0,0,100,0,0,0,0,0,0,12,43133,1,42000,0,0,0,8,0,0,0,0,-5137.05,499.769,396.457,3.10669,'Image of Razlo Crushcog - Script - Spawn 43133'),
(4250500,9,1,0,'',0,0,100,0,0,0,0,0,0,12,43133,1,42000,0,0,0,8,0,0,0,0,-5138.56,500.873,396.469,4.34587,'Image of Razlo Crushcog - Script - Spawn 43133'),
(4250500,9,2,0,'',0,0,100,0,0,0,0,0,0,3,0,35960,0,0,0,0,11,43133,5,0,0,0,0,0,0,'Image of Razlo Crushcog - Script - Image of Mechano-Tank Set Model'),
(4250500,9,3,0,'',0,0,100,0,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,3.9968,'Image of Razlo Crushcog - Script - Turn to'),
(4250500,9,4,0,'',0,0,100,0,30000,30000,0,0,0,28,16245,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Image of Razlo Crushcog - Script - Remove aura Freeze Anim'),
(4250500,9,5,0,'',0,0,100,0,0,0,0,0,0,5,5,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Image of Razlo Crushcog - Script - Emote OneShotExclamation'),
(4250500,9,6,0,'',0,0,100,0,8000,8000,0,0,0,41,0,8,0,0,0,0,1,0,0,0,0,0,0,0,0,'Image of Razlo Crushcog - Script - Despawn');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4273900 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4273900,9,0,0,'',0,0,100,0,3000,3000,0,0,0,11,63360,0,0,0,0,0,8,0,0,0,0,-5523.76,698.339,375.854,2.11185,'Frostmane Hold Target - Script - Cast ''Dusty Explosion'''),
(4273900,9,1,0,'',0,0,100,0,6000,6000,0,0,0,32,0,0,0,0,0,0,20,204047,10,0,0,0,0,0,0,'Frostmane Hold Target - Script - Reset Go');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4294500 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4294500,9,0,0,'',0,0,100,0,0,0,0,0,0,28,29266,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'GS-9x Prototype - Script - Remove aura'),
(4294500,9,1,0,'',0,0,100,0,0,0,0,0,0,19,536870912,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'GS-9x Prototype - Script - Remove Unit flag'),
(4294500,9,2,0,'',0,0,100,0,0,0,0,0,0,19,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,'GS-9x Prototype - Script - Remove Unit flag2'),
(4294500,9,3,0,'',0,0,100,0,2000,2000,0,0,0,4,12182,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'GS-9x Prototype - Script - Play Sound'),
(4294500,9,4,0,'',0,0,100,0,1000,1000,0,0,0,53,1,42945,0,0,0,0,1,0,0,0,0,0,0,0,0,'GS-9x Prototype - Script - Load Path');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4602500 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4602500,9,0,0,'',0,0,100,0,0,0,0,0,0,59,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Officer - Script - Turn run off'),
(4602500,9,1,0,'',0,0,100,0,0,0,0,0,0,71,0,0,0,61392,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Officer - Script - Equip Weapon'),
(4602500,9,2,0,'',0,0,100,0,2000,2000,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-5201.01,476.32,388.294,0,'S.A.F.E. Officer - Script - Move to'),
(4602500,9,3,0,'',0,0,100,0,1000,1000,0,0,0,5,66,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Officer - Script - Play Emote OneShotSalute'),
(4602500,9,4,0,'',0,0,100,0,3000,3000,0,0,0,53,0,42396,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Officer - Script - Load path');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4623000 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4623000,9,0,0,'',0,0,100,0,0,0,0,0,0,22,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - Script - Set Phase 0'),
(4623000,9,1,0,'',0,0,100,0,2000,2000,0,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - Script - Set Emotstate'),
(4623000,9,2,0,'',0,0,100,0,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0.296706,'S.A.F.E. Technician - Script - Turn to'),
(4623000,9,3,0,'',0,0,100,0,2000,2000,0,0,0,5,5,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - Script - Play Emote ''OneShotExclamation'''),
(4623000,9,4,0,'',0,0,100,0,0,0,0,0,0,59,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - Script - Turn run on'),
(4623000,9,5,0,'',0,0,100,0,1500,1500,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-5178.17,696.811,288.085,0,'S.A.F.E. Technician - Script - Move to'),
(4623000,9,6,0,'',0,0,100,0,1500,1500,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,1.50098,'S.A.F.E. Technician - Script - Turn to'),
(4623000,9,7,0,'',0,0,100,0,1500,1500,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - Script - Say 1'),
(4623000,9,8,0,'',0,0,100,0,2500,2500,0,0,0,17,69,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - Script - Set Emotstate'),
(4623000,9,9,0,'',0,0,100,0,5000,5000,0,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - Script - Set Emotstate'),
(4623000,9,10,0,'',0,0,100,0,0,0,0,0,0,59,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - Script - Turn run off'),
(4623000,9,11,0,'',0,0,100,0,1500,1500,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-5184.88,699.385,288.085,0,'S.A.F.E. Technician - Script - Move to'),
(4623000,9,12,0,'',0,0,100,0,3000,3000,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,4.06662,'S.A.F.E. Technician - Script - Turn to'),
(4623000,9,13,0,'',0,0,100,0,500,500,0,0,0,17,233,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - Script - Set Emotstate'),
(4623000,9,14,0,'',0,0,100,0,3000,3000,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - Script - Set Phase 1');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4623001 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4623001,9,0,0,'',0,0,100,0,0,0,0,0,0,59,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - Script - Turn run off'),
(4623001,9,1,0,'',0,0,100,0,0,0,0,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - Script - Set Emotestate'),
(4623001,9,2,0,'',0,0,100,0,2000,2000,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-5201.01,476.32,388.294,0,'S.A.F.E. Technician - Script - Move to'),
(4623001,9,3,0,'',0,0,100,0,1000,1000,0,0,0,5,66,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - Script - Play Emote OneShotSalute'),
(4623001,9,4,0,'',0,0,100,0,3000,3000,0,0,0,53,0,42396,0,0,0,0,1,0,0,0,0,0,0,0,0,'S.A.F.E. Technician - Script - Load path');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4626700 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4626700,9,0,0,'',0,0,100,0,1000,1000,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rescued Survivor - Script - Say 0'),
(4626700,9,1,0,'',0,0,100,0,0,0,0,0,0,59,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rescued Survivor - Script - Turn run off'),
(4626700,9,2,0,'',0,0,100,0,5000,5000,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-5163.99,759.596,285.474,0,'Rescued Survivor - Script - Move to'),
(4626700,9,3,0,'',0,0,100,0,10000,10000,0,0,0,53,0,46267,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rescued Survivor - Script - Load path');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4626701 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(4626701,9,0,0,'',0,0,100,0,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0,4.38078,'Rescued Survivor - script - Turn to'),
(4626701,9,1,0,'',0,0,100,0,0,0,0,0,0,90,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Rescued Survivor - script - sit');
DELETE FROM `creature_text` WHERE `CreatureID` = 103618 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 103618 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 1260 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 42366 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 42489 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 42489 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 42489 AND `GroupID` = 2;
DELETE FROM `creature_text` WHERE `CreatureID` = 42489 AND `GroupID` = 3;
DELETE FROM `creature_text` WHERE `CreatureID` = 42490 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 42491 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 42491 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 42773 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 45847 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 45847 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 45966 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 46025 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 46230 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 46230 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 46267 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 46449 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 47323 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 47323 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 47349 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 47349 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 47350 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 47350 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 47351 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 47351 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 47359 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 47359 AND `GroupID` = 1;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(1260,0,0,'%s becomes enraged!',16,0,100,0,0,0,10677,'combat Enrage'),
(42366,0,0,'We''re going to have a hard time handling this one alone. I sent a message to the dwarven capital of Ironforge asking for their support.',12,0,100,1,0,0,43111,'Kelsey Steelspark to Captain Tread Sparknozzle'),
(42489,0,0,'We can''t afford to let our enemy, Thermaplugg, continue to tighten his grip on the rest of Gnomeregan.',12,0,100,1,0,0,43103,'Captain Tread Sparknozzle'),
(42489,1,0,'He has ordered his crony, Razlo Crushcog, to keep us on the defensive. Hinkles, what''s the latest intelligence on Razlo and his men?',12,0,100,1,0,0,43104,'Captain Tread Sparknozzle'),
(42489,2,0,'We have to be in position to strike against Crushcog when the mountaineers arrive.',12,0,100,1,0,0,43118,'Captain Tread Sparknozzle to Captain Tread Sparknozzle'),
(42489,3,0,'With Crushcog defeated, Thermaplugg is sure to be quaking in his mechano-tank, and rightly so. You''re next, Thermaplugg. You''re next!',12,0,100,1,0,0,43119,'Captain Tread Sparknozzle to Captain Tread Sparknozzle'),
(42490,0,0,'A messenger reached us from Brewnall Village this morning. The dwarves are sending a squad of mountaineers to help us.',12,0,100,1,0,0,43117,'Elgin Clickspring to Captain Tread Sparknozzle'),
(42491,0,0,'It''s been hard for our scouts to get information on what Crushcog is planning.',12,0,100,1,0,0,43105,'Hinkles Fastblast to Captain Tread Sparknozzle'),
(42491,1,0,'What we do know is that Crushcog is using an advanced mechano-tank prototype and his irradiated gnomes are trying to repair more at the old arsenal.',12,0,100,1,0,0,43110,'Hinkles Fastblast to Captain Tread Sparknozzle'),
(42773,0,0,'%s becomes enraged!',16,0,100,0,0,0,10677,'combat Enrage'),
(45847,0,0,'You''re doing well... you''re going to be fine.',12,0,100,1,0,0,46503,'S.A.F.E. Operative'),
(45847,1,0,'Our men have secured the walkway. Focus on helping those in the main room.',12,0,100,1,0,0,47758,'S.A.F.E. Operative'),
(45966,0,0,'We have to break through those mindless leper gnomes if we want to get out of here alive!',12,0,100,25,0,0,46382,'Nevin Twistwrench'),
(46025,0,0,'This path only leads deeper into the city. Keep going south toward the Loading Room.',12,0,100,5,0,0,47191,'S.A.F.E. Officer'),
(46230,0,0,'Ugh! Not this again! I''m asking for a new station next expedition...',12,0,100,1,0,0,46342,'S.A.F.E. Technician'),
(46230,1,0,'No, not the Sanitron... it was my pride and joy!',12,0,100,6,0,0,46341,'S.A.F.E. Technician'),
(46267,0,0,'Thank the Light! I''ve made it.',12,0,100,1,0,0,46477,'Rescued Survivor'),
(46267,0,1,'It''s a relief to be surrounded by normal gnomes again!',12,0,100,1,0,0,46478,'Rescued Survivor'),
(46267,0,2,'I thought I was goner!',12,0,100,1,0,0,46479,'Rescued Survivor'),
(46267,0,3,'You... you''re not going to try to eat me, are you?',12,0,100,1,0,0,46480,'Rescued Survivor'),
(46267,0,4,'I never would''ve made it on my own...',12,0,100,1,0,0,46481,'Rescued Survivor'),
(46267,0,5,'Thank you for getting me out of there!',12,0,100,1,0,0,46482,'Rescued Survivor'),
(46267,0,6,'My family... did you find my family?',12,0,100,1,0,0,46483,'Rescued Survivor'),
(46449,1,0,'We''re going to get you some help.',12,0,100,0,0,0,46502,'S.A.F.E. Operative'),
(47323,0,0,'Follow me, $n, and I''ll introduce you to your trainer, Alamar Grimm.',12,0,100,1,0,0,47493,'S.A.F.E. Guide'),
(47323,1,0,'Alamar, I''d  like to introduce you to $n, a recent escapee from Gnomeregan.',12,0,100,1,0,0,47494,'S.A.F.E. Guide'),
(47349,0,0,'Follow me, $n, and I''ll introduce you to your trainer, Drill Sergeant Steamcrank.',12,0,100,1,0,0,47516,'S.A.F.E. Guide'),
(47349,1,0,'Sergeant, allow me to introduce $n, who helped us rescue survivors in Gnomeregan.',12,0,100,1,0,0,47517,'S.A.F.E. Guide'),
(47350,0,0,'Follow me, $n, and I''ll introduce you to your trainer, Kelsey Steelspark.',12,0,100,1,0,0,47519,'S.A.F.E. Guide'),
(47350,1,0,'Kelsey, allow me to introduce $n, who helped us rescue survivors in Gnomeregan.',12,0,100,1,0,0,47520,'S.A.F.E. Guide'),
(47351,0,0,'Follow me, $n, and I''ll introduce you to your trainer, Bipsi Frostflinger.',12,0,100,1,0,0,47531,'S.A.F.E. Guide'),
(47351,1,0,'Bipsi, this is $n, one of the most recent survivors to emerge from Gnomeregan.',12,0,100,1,0,0,47532,'S.A.F.E. Guide'),
(47359,0,0,'Follow me, $n, and I''ll introduce you to your trainer, "Doc" Cogspin.',12,0,100,1,0,0,47533,'S.A.F.E. Guide'),
(47359,1,0,'Doc, this is $n, who helped Nevin''s team get a number of survivors out of Gnomeregan.',12,0,100,1,0,0,47534,'S.A.F.E. Guide'),
(103618,0,0,'Follow me, $n, and I''ll introduce you to your trainer, Muffinus Chromebrew.',12,0,100,1,0,0,106215,'S.A.F.E. Guide'),
(103618,1,0,'Muffinus, allow me to introduce $n, who helped us rescue survivors in Gnomeregan.',12,0,100,1,0,0,106216,'S.A.F.E. Guide');
DELETE FROM `waypoints` WHERE `entry` = 103618;
DELETE FROM `waypoints` WHERE `entry` = 42324;
DELETE FROM `waypoints` WHERE `entry` = 42396;
DELETE FROM `waypoints` WHERE `entry` = 42505;
DELETE FROM `waypoints` WHERE `entry` = 42945;
DELETE FROM `waypoints` WHERE `entry` = 46267;
DELETE FROM `waypoints` WHERE `entry` = 47323;
DELETE FROM `waypoints` WHERE `entry` = 47349;
DELETE FROM `waypoints` WHERE `entry` = 47350;
DELETE FROM `waypoints` WHERE `entry` = 47351;
DELETE FROM `waypoints` WHERE `entry` = 47359;
INSERT INTO `waypoints` (`entry`,`pointid`,`position_x`,`position_y`,`position_z`,`point_comment`) VALUES
(42324,1,-5173.22,454.167,390.576,'Drill Sergeant Steamcrank'),
(42324,2,-5173.53,453.476,390.586,'Drill Sergeant Steamcrank'),
(42324,3,-5158.39,456.637,392.405,'Drill Sergeant Steamcrank'),
(42324,4,-5162.46,455.573,392.245,'Drill Sergeant Steamcrank'),
(42324,5,-5163.07,454.227,392.314,'Drill Sergeant Steamcrank'),
(42324,6,-5167.78,453.701,391.813,'Drill Sergeant Steamcrank'),
(42396,1,-5196.02,474.892,388.488,'S.A.F.E. Officer / Technician Summoned by Nevin Twistwrench'),
(42396,2,-5186.08,473.747,388.437,'S.A.F.E. Officer / Technician Summoned by Nevin Twistwrench'),
(42396,3,-5181.28,479.153,388.312,'S.A.F.E. Officer / Technician Summoned by Nevin Twistwrench'),
(42396,4,-5180.78,488.089,388.178,'S.A.F.E. Officer / Technician Summoned by Nevin Twistwrench'),
(42396,5,-5185.95,495.59,388.062,'S.A.F.E. Officer / Technician Summoned by Nevin Twistwrench'),
(42396,6,-5190.03,506.799,387.883,'S.A.F.E. Officer / Technician Summoned by Nevin Twistwrench'),
(42396,7,-5192.92,514.186,387.758,'S.A.F.E. Officer / Technician Summoned by Nevin Twistwrench'),
(42396,8,-5198.44,512.016,388.28,'S.A.F.E. Officer / Technician Summoned by Nevin Twistwrench'),
(42396,9,-5205.1,505.281,388.28,'S.A.F.E. Officer / Technician Summoned by Nevin Twistwrench'),
(42396,10,-5205.77,501.943,388.28,'S.A.F.E. Officer / Technician Summoned by Nevin Twistwrench'),
(42505,1,-5140.15,498.619,396.504,'Image of Razlo Crushcog'),
(42505,2,-5137.77,500.366,395.884,'Image of Razlo Crushcog'),
(42945,1,-5076.74,457.986,410.258,'GS-9x Prototype'),
(42945,2,-5079.74,458.736,409.008,'GS-9x Prototype'),
(42945,3,-5081.49,459.236,408.008,'GS-9x Prototype'),
(42945,4,-5083.49,459.736,407.258,'GS-9x Prototype'),
(42945,5,-5086.99,460.736,406.508,'GS-9x Prototype'),
(42945,6,-5089.99,461.486,405.758,'GS-9x Prototype'),
(42945,7,-5093.74,462.486,405.008,'GS-9x Prototype'),
(42945,8,-5097.02,463.358,404.222,'GS-9x Prototype'),
(46267,1,-5163.99,759.596,285.474,'Rescued Survivor'),
(46267,2,-5157.86,764.885,286.772,'Rescued Survivor'),
(46267,3,-5156.11,767.95,287.397,'Rescued Survivor'),
(46267,4,-5158.27,773.444,287.397,'Rescued Survivor'),
(46267,5,-5159.82,776.925,287.397,'Rescued Survivor'),
(47323,1,-5187.03,474.24,388.437,'S.A.F.E. Guide'),
(47323,2,-5181.46,480.069,388.312,'S.A.F.E. Guide'),
(47323,3,-5181.73,488.365,388.145,'S.A.F.E. Guide'),
(47323,4,-5186.75,500.146,387.883,'S.A.F.E. Guide'),
(47323,5,-5192.54,508.486,387.883,'S.A.F.E. Guide'),
(47349,1,-5180.05,470.845,388.437,'S.A.F.E. Guide'),
(47349,2,-5172.42,465.8,389.373,'S.A.F.E. Guide'),
(47349,3,-5167.26,460.304,390.895,'S.A.F.E. Guide'),
(47350,1,-5176.65,480.795,388.437,'S.A.F.E. Guide'),
(47350,2,-5161.17,476.915,390.419,'S.A.F.E. Guide'),
(47350,3,-5153.21,478.215,391.297,'S.A.F.E. Guide'),
(47350,4,-5146.08,487.271,394.612,'S.A.F.E. Guide'),
(47350,5,-5142.51,493.896,395.669,'S.A.F.E. Guide'),
(47350,6,-5145.51,500.418,395.594,'S.A.F.E. Guide'),
(47350,7,-5141.99,502.071,395.594,'S.A.F.E. Guide'),
(47351,1,-5177.57,476.611,388.437,'S.A.F.E. Guide'),
(47351,2,-5164.87,478.951,389.975,'S.A.F.E. Guide'),
(47351,3,-5155.42,470.168,390.669,'S.A.F.E. Guide'),
(47351,4,-5146.19,459.63,392.389,'S.A.F.E. Guide'),
(47351,5,-5130.46,450.373,394.945,'S.A.F.E. Guide'),
(47351,6,-5116.67,453.446,399.057,'S.A.F.E. Guide'),
(47351,7,-5105.84,458.625,402.52,'S.A.F.E. Guide'),
(47351,8,-5093.27,457.524,405.407,'S.A.F.E. Guide'),
(47351,9,-5087.58,448.462,409.144,'S.A.F.E. Guide'),
(47359,1,-5175.7,479.142,388.437,'S.A.F.E. Guide'),
(47359,2,-5169.51,475.753,389.226,'S.A.F.E. Guide'),
(47359,3,-5151.5,465.866,391.306,'S.A.F.E. Guide'),
(47359,4,-5139.41,453.912,393.889,'S.A.F.E. Guide'),
(47359,5,-5122.75,450.625,396.895,'S.A.F.E. Guide'),
(47359,6,-5104.41,459.804,402.837,'S.A.F.E. Guide'),
(47359,7,-5094.97,465.049,404.179,'S.A.F.E. Guide'),
(47359,8,-5077.84,475.616,401.925,'S.A.F.E. Guide'),
(103618,1,-5196.8,475.03,388.543,'S.A.F.E. Guide'),
(103618,2,-5179.24,473.552,388.437,'S.A.F.E. Guide'),
(103618,3,-5168.28,480.726,389.43,'S.A.F.E. Guide'),
(103618,4,-5154.67,469.188,390.806,'S.A.F.E. Guide'),
(103618,5,-5141.15,455.132,393.461,'S.A.F.E. Guide'),
(103618,6,-5128.71,449.24,395.292,'S.A.F.E. Guide'),
(103618,7,-5114.34,453.953,399.671,'S.A.F.E. Guide'),
(103618,8,-5107.07,447.585,400.632,'S.A.F.E. Guide'),
(103618,9,-5107.3,437.724,400.802,'S.A.F.E. Guide'),
(103618,10,-5106.89,427.568,402.726,'S.A.F.E. Guide'),
(103618,11,-5112.9,425.344,402.878,'S.A.F.E. Guide'),
(103618,12,-5115.12,430.734,402.883,'S.A.F.E. Guide'),
(103618,13,-5115.12,430.734,402.883,'S.A.F.E. Guide');
