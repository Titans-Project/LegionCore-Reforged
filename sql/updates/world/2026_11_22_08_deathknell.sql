-- Deathknell: SmartAI of the starting zone NPCs
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (1568,1688,1736,1766,1780,1781,1797,1890,12433,44795,44906,50328,50372,50373,50414,50763);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1568 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1568,0,0,0,'',19,0,100,0,28608,0,0,0,0,85,91576,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Mordo - On Quest Accept - Cast Spell To Summon');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1688 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1688,0,0,0,'',0,0,100,0,1000,1000,15000,15000,0,11,11918,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'1688 - IC - CAST');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1736 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1736,0,0,0,'',0,0,100,0,1000,2000,2000,3000,0,11,95826,66,0,0,0,0,2,0,0,0,0,0,0,0,0,'Deathguard Randolph - In Combat - Cast ''<Spell not found!>'''),
(1736,0,1,0,'',25,0,100,0,0,0,0,0,0,8,2,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Deathguard Randolph - On Reset - Set Reactstate Aggressive');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1766 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1766,0,0,0,'',0,0,100,0,4000,5000,22000,26000,0,11,3150,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Rabid Worg - In Combat - Cast ''Rabies''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1780 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1780,0,0,0,'',0,0,100,0,7000,9000,19000,20000,0,11,13884,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Skitterweb Striker - In Combat - Cast ''Withering Poison'''),
(1780,0,1,0,'',9,0,100,0,0,5,12000,14000,0,11,87081,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Skitterweb Striker - Within 0-5 Range - Cast ''Strike''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1781 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1781,0,0,0,'',0,0,100,0,6000,9000,18000,24000,0,11,82715,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Skitterweb Lurker - In Combat - Cast ''Flee into the Shadows''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1797 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1797,0,0,0,'',0,0,100,0,3000,4500,22000,25000,0,11,3150,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Giant Rabid Bear - In Combat - Cast ''Rabies'''),
(1797,0,1,0,'',2,0,100,1,0,45,0,0,0,11,84868,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Giant Rabid Bear - Between 0-45% Health - Cast ''Hibernate'' (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1890 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1890,0,0,0,'',0,0,100,0,1000,1000,15000,15000,0,11,81219,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'1890 - IC - CAST');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 12433 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(12433,0,0,0,'',11,0,100,1,0,0,0,0,0,11,1784,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Krethis the Shadowspinner - On Respawn - Cast ''Stealth'' (No Repeat)'),
(12433,0,1,0,'',7,0,100,1,0,0,0,0,0,11,1784,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Krethis the Shadowspinner - On Evade - Cast ''Stealth'' (No Repeat)'),
(12433,0,2,0,'',0,0,100,0,5000,6000,17000,20000,0,11,13884,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Krethis the Shadowspinner - In Combat - Cast ''Withering Poison'''),
(12433,0,3,0,'',9,0,100,0,0,30,12000,14500,0,11,12023,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Krethis the Shadowspinner - Within 0-30 Range - Cast ''Web''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 44795 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(44795,0,0,0,'',8,0,100,0,2061,0,0,0,0,33,44175,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Deathguards - On spellHit - CreditKill');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 44906 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(44906,0,0,0,'',4,0,100,1,0,0,0,0,0,11,87084,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Skitterweb Matriarch - On Aggro - Cast ''Summon Spiderlings'' (No Repeat)'),
(44906,0,1,0,'',0,0,100,0,4000,7000,15000,18000,0,11,79607,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Skitterweb Matriarch - In Combat - Cast ''Venom Splash''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 50328 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(50328,0,0,0,'',0,0,100,0,2000,4000,6000,9000,0,11,8281,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Fangor - In Combat - Cast ''Sonic Burst''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 50372 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(50372,0,0,0,'',25,0,100,0,0,0,0,0,0,53,0,50372,1,0,0,0,1,0,0,0,0,0,0,0,0,'Aradne - On Reset - Start Waypoint'),
(50372,0,1,0,'',40,0,100,0,2,50372,0,0,0,80,5037200,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Aradne - On Waypoint 2 Reached - Run Script'),
(50372,0,2,0,'',40,0,100,0,4,50372,0,0,0,80,5037200,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Aradne - On Waypoint 4 Reached - Run Script'),
(50372,0,3,0,'',40,0,100,0,18,50372,0,0,0,80,5037200,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Aradne - On Waypoint 18 Reached - Run Script'),
(50372,0,4,0,'',40,0,100,0,25,50372,0,0,0,80,5037200,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Aradne - On Waypoint 25 Reached - Run Script');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 50373 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(50373,0,0,0,'',38,0,100,0,1,1,0,0,0,11,93447,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Deathknell Grave Target - On Data Set 1 1 - Cast ''<Spell not found!>'''),
(50373,0,1,0,'',25,0,100,0,0,0,0,0,0,47,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Deathknell Grave Target - On Reset - Set Visibility Off');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 50414 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(50414,0,0,0,'',25,0,100,1,0,0,0,0,0,53,0,50414,0,0,0,0,1,0,0,0,0,0,0,0,0,'Risen Recruit - On Reset - Start Waypoint (No Repeat)'),
(50414,0,1,0,'',58,0,100,0,1,50414,0,0,0,80,5041400,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Risen Recruit - On Waypoint 1 Reached - Run Script');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 50763 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(50763,0,0,0,'',0,0,100,0,2000,4000,7000,9000,0,11,745,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Shadowstalker - In Combat - Cast ''Web'''),
(50763,0,1,0,'',0,0,100,0,1000,5000,15000,17000,0,11,744,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Shadowstalker - In Combat - Cast ''Poison''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 5037200 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(5037200,9,0,0,'',0,0,100,0,0,0,0,0,0,54,7000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Aradne - On Script - Pause Waypoint'),
(5037200,9,1,0,'',0,0,100,0,0,0,0,0,0,11,93446,0,0,0,0,0,19,50373,5,0,0,0,0,0,0,'Aradne - On Script - Cast ''Raise Undead'''),
(5037200,9,2,0,'',0,0,100,0,2000,2000,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Aradne - On Script - Say Line 0'),
(5037200,9,3,0,'',0,0,100,0,0,0,0,0,0,45,1,1,0,0,0,0,19,50373,10,0,0,0,0,0,0,'Aradne - On Script - Set Data 1 1');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 5041400 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(5041400,9,0,0,'',0,0,100,0,0,0,0,0,0,5,16,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Risen Recruit - On Script - Play Emote 16'),
(5041400,9,1,0,'',0,0,100,0,0,0,0,0,0,5,69,0,0,0,0,0,19,1568,10,0,0,0,0,0,0,'Risen Recruit - On Script - Play Emote 69'),
(5041400,9,2,0,'',0,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,19,1568,10,0,0,0,0,0,0,'Risen Recruit - On Script - Say Line 0'),
(5041400,9,3,0,'',0,0,100,0,5000,5000,0,0,0,5,26,0,0,0,0,0,19,1568,10,0,0,0,0,0,0,'On Script - Play Emote 26'),
(5041400,9,4,0,'',0,0,100,0,2000,2000,0,0,0,5,25,0,0,0,0,0,19,1568,10,0,0,0,0,0,0,'On Script - Play Emote 25'),
(5041400,9,5,0,'',0,0,100,0,2000,2000,0,0,0,1,1,0,0,0,0,0,19,1568,10,0,0,0,0,0,0,'On Script - Say Line 1'),
(5041400,9,6,0,'',0,0,100,0,0,0,0,0,0,5,26,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'On Script - Play Emote 26'),
(5041400,9,7,0,'',0,0,100,0,2000,2000,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'On Script - Say Line 0'),
(5041400,9,8,0,'',0,0,100,0,2000,2000,0,0,0,5,66,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'On Script - Play Emote 66'),
(5041400,9,9,0,'',0,0,100,0,3000,3000,0,0,0,53,0,504140,0,0,0,0,1,0,0,0,0,0,0,0,0,'On Script - Start Waypoint'),
(5041400,9,10,0,'',0,0,100,0,6000,6000,0,0,0,41,2000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'On Script - Despawn In 2000 ms');
DELETE FROM `creature_text` WHERE `CreatureID` = 1568 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 1568 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 50414 AND `GroupID` = 0;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(1568,0,0,'Let''s see, I just saw a corpse with a jaw that would fit you',12,0,100,0,0,0,50126,'Mordo'),
(1568,1,0,'That should do the job. Come back right away if it falls off again.',12,0,100,0,0,0,50127,'Mordo'),
(50414,0,0,'T-thank you, Under-t-taker.',12,0,100,0,0,0,50128,'Risen Recruit');
DELETE FROM `waypoints` WHERE `entry` = 50372;
DELETE FROM `waypoints` WHERE `entry` = 50414;
DELETE FROM `waypoints` WHERE `entry` = 504140;
INSERT INTO `waypoints` (`entry`,`pointid`,`position_x`,`position_y`,`position_z`,`point_comment`) VALUES
(50372,1,1698.53,1687.82,134.825,'Aradne'),
(50372,2,1696.9,1688.16,135.093,'Aradne'),
(50372,3,1696.82,1680.22,134.648,'Aradne'),
(50372,4,1695.54,1680.51,134.857,'Aradne'),
(50372,5,1694.45,1675.17,134.582,'Aradne'),
(50372,6,1691.49,1672.14,134.725,'Aradne'),
(50372,7,1686.68,1671.13,135.499,'Aradne'),
(50372,8,1683.32,1672.1,136.382,'Aradne'),
(50372,9,1676.21,1673.95,137.504,'Aradne'),
(50372,10,1672.78,1674.63,138.265,'Aradne'),
(50372,11,1671.65,1680.4,139.247,'Aradne'),
(50372,12,1672.95,1685.73,139.639,'Aradne'),
(50372,13,1673.6,1688.58,139.695,'Aradne'),
(50372,14,1675.72,1697.68,140.331,'Aradne'),
(50372,15,1682.36,1695.94,138.045,'Aradne'),
(50372,16,1688.03,1692.28,137.092,'Aradne'),
(50372,17,1686.76,1686.93,137.07,'Aradne'),
(50372,18,1683.31,1686.49,137.777,'Aradne'),
(50372,19,1688.84,1685.97,136.565,'Aradne'),
(50372,20,1701.76,1685.93,134.241,'Aradne'),
(50372,21,1701.76,1685.93,134.241,'Aradne'),
(50372,22,1708.09,1692.04,133.826,'Aradne'),
(50372,23,1707.34,1696.12,133.928,'Aradne'),
(50372,24,1704.29,1699.33,134.399,'Aradne'),
(50372,25,1707.4,1700.54,134.079,'Aradne'),
(50414,1,1691.53,1675.77,135.228,'Risen Recruit'),
(504140,1,1700.93,1655.51,129.171,'Risen Recruit');
