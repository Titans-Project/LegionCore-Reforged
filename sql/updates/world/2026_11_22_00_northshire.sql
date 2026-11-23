-- Northshire: SmartAI of the starting zone NPCs
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (30,43,62,352,466,583,1395,1756,7381,7382,7384,7385,28347,42937,46983,52027,52028,52029,54345);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 30 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(30,0,0,0,'',0,0,80,0,11000,11000,15000,20000,0,11,11918,32,0,0,0,0,2,0,0,0,0,0,0,0,0,'Forest Spider - In Combat - Cast ''Poison'' (Phase 1)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 43 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(43,0,0,0,'',0,0,100,0,4500,4500,18000,28000,0,11,11918,32,0,0,0,0,2,0,0,0,0,0,0,0,0,'Mine Spider - In Combat - Cast ''Poison'' (Phase 1)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 62 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(62,0,0,0,'',1,0,100,0,0,0,10000,10000,0,11,12544,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Gug Fatcandle - OOC - Cast ''Frost Armor'''),
(62,0,1,0,'',0,0,100,0,0,0,3600,4000,0,11,20793,64,0,0,0,0,2,0,0,0,0,0,0,0,0,'Gug Fatcandle - In Combat CMC - Cast ''Fireball''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 352 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(352,0,0,0,'',4,0,100,0,0,0,0,0,0,12,9526,4,60000,0,1,0,1,0,0,0,0,0,0,0,0,'Dungar Longdrink - On Aggro - Summon Creature ''Enraged Gryphon'''),
(352,0,2,0,'',4,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Dungar Longdrink - On Aggro - Say Line 0');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 466 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(466,0,0,0,'',22,0,100,0,78,1000,1000,0,0,5,66,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'General Marcus Jonathan - Received Emote 78 - Play Emote 66 (No Repeat)'),
(466,0,1,0,'',22,0,100,0,101,1000,1000,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'General Marcus Jonathan - Received Emote 101 - Say Line 0 (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 583 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(583,0,0,0,'',4,0,30,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Defias Ambusher - On Aggro - Say Line 0 (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1395 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1395,0,0,0,'',1,0,100,0,0,0,180000,180000,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Ol'' Beasley - Out of Combat - Say Line 0');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 1756 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1756,0,0,0,'',10,0,100,0,1,10,10000,10000,0,87,6800,6801,6802,6803,0,0,1,0,0,0,0,0,0,0,0,'Stormwind Royal Guard - Within 1-10 Range Out of Combat LoS - Run Random Script');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 7381 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(7381,0,0,0,'',23,0,100,0,62002,1,10000,10000,0,11,62000,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Silver Tabby cat - On Has Aura (Stinker in love) - Cast Stinker Periodic'),
(7381,0,1,0,'',23,0,100,0,62002,0,10000,10000,0,24,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Silver Tabby Cat - On Has No Aura (Stinker in love) - Evade');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 7382 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(7382,0,0,0,'',23,0,100,0,62002,1,10000,10000,0,11,62000,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Orange Tabby Cat - On Has Aura (Stinker in love) - Cast Stinker Periodic'),
(7382,0,1,0,'',23,0,100,0,62002,0,10000,10000,0,24,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Orange Tabby Cat - On Has No Aura (Stinker in love) - Evade');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 7384 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(7384,0,0,0,'',23,0,100,0,62002,1,10000,10000,0,11,62000,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Cornish Rex Cat - On Has Aura (Stinker in love) - Cast Stinker Periodic'),
(7384,0,1,0,'',23,0,100,0,62002,0,10000,10000,0,24,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Cornish Rex Cat - On Has No Aura (Stinker in love) - Evade');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 7385 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(7385,0,0,0,'',23,0,100,0,62002,1,10000,10000,0,11,62000,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Bombay Cat - On Has Aura (Stinker in love) - Cast Stinker Periodic'),
(7385,0,1,0,'',23,0,100,0,62002,0,10000,10000,0,24,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Bombay Cat - On Has No Aura (Stinker in love) - Evade');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 28347 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(28347,0,0,0,'',1,0,100,0,0,30000,60000,90000,0,87,2834700,2834701,2834702,2834703,0,0,1,0,0,0,0,0,0,0,0,'Miles Sidney - OOC  - Run Random Script'),
(28347,0,1,0,'',10,0,100,0,1,20,30000,60000,0,1,5,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Miles Sidney - OOC LOS - Say Line 5');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42937 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(42937,0,0,0,'',4,0,25,1,0,0,0,0,0,1,0,0,1,0,0,0,2,0,0,0,0,0,0,0,0,'Blackrock Invader - On Aggro - Say Line 0 (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 46983 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(46983,0,0,0,'',1,0,100,0,1,1,10000,10000,0,11,45425,0,0,0,0,0,10,102199,0,0,0,0,0,0,0,'Benjamin Dignerenard - OOC - cast shot');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 52027 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(52027,0,0,0,'',25,0,100,1,0,0,0,0,0,11,46905,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'On reset (e.g. after reaching home) - Self: Cast spell 46905 on Self // ');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 52028 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(52028,0,0,0,'',25,0,100,1,0,0,0,0,0,11,46905,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'On reset (e.g. after reaching home) - Self: Cast spell 46905 on Self // ');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 52029 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(52029,0,0,0,'',25,0,100,1,0,0,0,0,0,11,46905,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'On reset (e.g. after reaching home) - Self: Cast spell 46905 on Self // ');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 54345 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(54345,0,0,0,'',62,0,100,0,13113,0,0,0,0,11,103413,1,0,0,0,0,7,0,0,0,0,0,0,0,0,'Darkmoon Faire Greeter - On Gossip Option 0 Selected - Cast ''Replace Darkmoon Adventurer''s Guide'''),
(54345,0,1,0,'',62,0,100,0,13113,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Darkmoon Faire Greeter - On Gossip Option 0 Selected - Close Gossip');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 6800 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(6800,9,0,0,'',0,0,100,0,0,0,0,0,0,11,58509,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Stormwind City Guard - On Script - Cast ''''Rotten Apple Toss'''),
(6800,9,1,0,'',0,0,100,0,0,0,0,0,0,1,2,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Stormwind City Guard - On Script - Say Line 2');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 6801 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(6801,9,0,0,'',0,0,100,0,0,0,0,0,0,11,58513,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Stormwind City Guard - On Script - Cast ''Rotten Banana Toss'''),
(6801,9,1,0,'',0,0,100,0,0,0,0,0,0,1,3,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Stormwind City Guard - On Script - Say Line 3');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 6802 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(6802,9,0,0,'',0,0,100,0,0,0,0,0,0,11,58520,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Stormwind City Guard - On Script - Cast ''Spit'''),
(6802,9,1,0,'',0,0,100,0,0,0,0,0,0,1,4,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Stormwind City Guard - On Script - Say Line 4');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 6803 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(6803,9,0,0,'',0,0,100,0,0,0,0,0,0,1,5,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Stormwind City Guard - On Script - Say Line 5');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2834700 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2834700,9,0,0,'',0,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Miles Sidney - Script 1  - Say Line 0'),
(2834700,9,1,0,'',0,0,100,0,5000,5000,0,0,0,1,2,0,0,0,0,0,19,28355,0,0,0,0,0,0,0,'Miles Sidney - Script 1  - Say Line 2 (Wright Williams)'),
(2834700,9,2,0,'',0,0,100,0,5000,5000,0,0,0,1,4,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Miles Sidney - Script 1  - Say Line 4');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2834701 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2834701,9,0,0,'',0,0,100,0,0,0,0,0,0,1,0,0,0,0,0,0,19,28355,0,0,0,0,0,0,0,'Miles Sidney - Script 2  - Say Line 0 (Wright Williams)'),
(2834701,9,1,0,'',0,0,100,0,5000,5000,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Miles Sidney - Script 2  - Say Line 1');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2834702 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2834702,9,0,0,'',0,0,100,0,0,0,0,0,0,1,1,0,0,0,0,0,19,28355,0,0,0,0,0,0,0,'Miles Sidney - Script 3  - Say Line 1 (Wright Williams)'),
(2834702,9,1,0,'',0,0,100,0,5000,5000,0,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Miles Sidney - Script 3  - Say Line 2');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2834703 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(2834703,9,0,0,'',0,0,100,0,0,0,0,0,0,1,3,0,0,0,0,0,19,28355,0,0,0,0,0,0,0,'Miles Sidney - Script 4  - Say Line 3 (Wright Williams)'),
(2834703,9,1,0,'',0,0,100,0,5000,5000,0,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Miles Sidney - Script 4  - Say Line 3');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 1 AND `SourceEntry` = 1756 AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(22,1,1756,0,0,28,0,13188,0,0,0,0,'','Only Run script if quest is completed');
DELETE FROM `creature_text` WHERE `CreatureID` = 1395 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 1756 AND `GroupID` = 5;
DELETE FROM `creature_text` WHERE `CreatureID` = 28347 AND `GroupID` = 3;
DELETE FROM `creature_text` WHERE `CreatureID` = 28347 AND `GroupID` = 4;
DELETE FROM `creature_text` WHERE `CreatureID` = 28347 AND `GroupID` = 5;
DELETE FROM `creature_text` WHERE `CreatureID` = 28355 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 28355 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 28355 AND `GroupID` = 2;
DELETE FROM `creature_text` WHERE `CreatureID` = 466 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 583 AND `GroupID` = 0;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(466,0,0,'Greetings citizen.',12,7,100,0,0,0,0,'General Marcus Jonathan'),
(583,0,0,'The Brotherhood will not tolerate your actions.',12,7,100,0,0,0,1866,'Defias Ambusher - Random Say on Aggro'),
(583,0,1,'Ah, a chance to use this freshly sharpened blade.',12,7,100,0,0,0,1865,'Defias Ambusher - Random Say on Aggro'),
(583,0,2,'Feel the power of the Brotherhood!',12,7,100,0,0,0,1867,'Defias Ambusher - Random Say on Aggro'),
(1395,0,0,'It''s all their fault, stupid Alliance army. Just had to build their towers right behind my farm.',12,7,100,0,0,0,186,'Ol'' Beasley'),
(1395,0,1,'Spare some change for a poor blind man? ...What do you mean I''m not blind? ...I''M NOT BLIND! I CAN SEE!! It''s a miracle!',12,7,100,0,0,0,184,'Ol'' Beasley'),
(1395,0,2,'I will gladly pay you Tuesday for a hamburger today.',12,7,100,0,0,0,182,'Ol'' Beasley'),
(1756,5,0,'Monster!',12,0,100,0,0,0,31662,'Stormwind Royal Guard'),
(1756,5,1,'Murderer!',12,0,100,0,0,0,31679,'Stormwind Royal Guard'),
(1756,5,2,'GET A ROPE!',12,0,100,0,0,0,31669,'Stormwind Royal Guard'),
(1756,5,3,'How dare you set foot in our city!',12,0,100,0,0,0,31684,'Stormwind Royal Guard'),
(1756,5,4,'You disgust me.',12,0,100,0,0,0,31680,'Stormwind Royal Guard'),
(1756,5,5,'Looks like we''re going to have ourselves an execution.',12,0,100,0,0,0,31682,'Stormwind Royal Guard'),
(1756,5,6,'Traitorous dog.',12,0,100,0,0,0,31665,'Stormwind Royal Guard'),
(1756,5,7,'My family was wiped out by the Scourge! MONSTER!',12,0,100,0,0,0,31667,'Stormwind Royal Guard'),
(28347,3,0,'Only put half as much in next time.',12,0,100,0,0,0,27806,'Miles Sidney'),
(28347,4,0,'That''s what you always say. Sometimes I think you''re never going to finish that thing.',12,0,100,0,0,0,27810,'Miles Sidney'),
(28347,5,0,'Greetings.',12,7,100,3,0,0,43337,'Miles Sidney'),
(28347,5,1,'Greetings, $c.',12,7,100,3,0,0,43330,'Miles Sidney'),
(28347,5,2,'Greetings! Please have a look around.',12,7,100,3,0,0,43333,'Miles Sidney'),
(28347,5,3,'Let me know if you need help finding anything, $c.',12,7,100,3,0,0,43335,'Miles Sidney'),
(28347,5,4,'Welcome. May I help you find something?',12,7,100,3,0,0,43336,'Miles Sidney'),
(28355,0,0,'Sir, our customers are complaining that there''s not enough Maiden''s Anguish in our Deadly Poisons.',12,0,100,0,0,0,27801,'Wright Williams'),
(28355,1,0,'Sir, I think we were close with the Lethargy Root in that last poison recipe.',12,0,100,0,0,0,27803,'Wright Williams'),
(28355,2,0,'I''m working on it, I just don''t want to sell it until I''m happy with it. It shouldn''t be long.',12,0,100,0,0,0,27808,'Wright Williams');
DELETE FROM `creature_text` WHERE `CreatureID` = 28355 AND `GroupID` = 3;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(28355,3,0,'Sir, our customers are complaining that there''s too much Deathweed in our Anesthetics.',12,0,100,0,0,0,27802,'Wright Williams');
-- Blackrock Spies: a spyglass or a crouch, never both; Brother Paxton heals the infantry
UPDATE `creature_template_addon` SET `bytes1` = 0, `bytes2` = 1, `auras` = '80676' WHERE `entry` = 49874;
DELETE FROM `creature_addon` WHERE `guid` IN (190882,190883,190884,190885,190890,190894,190896,190899,190905);
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(190882,0,0,0,1,0,'92857'),(190883,0,0,0,1,0,'92857'),(190884,0,0,0,1,0,'92857'),(190885,0,0,0,1,0,'92857'),(190890,0,0,0,1,0,'92857'),
(190894,0,0,0,1,0,'92857'),(190896,0,0,0,1,0,'92857'),(190899,0,0,0,1,0,'92857'),(190905,0,0,0,1,0,'92857');
DELETE FROM `smart_scripts` WHERE (`entryorguid` = 49874 AND `source_type` = 0) OR (`entryorguid` IN (4987400,4987401) AND `source_type` = 9);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(49874,0,0,0,'',4,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Blackrock Spy - On Aggro - Say Line 0 (No Repeat)');
DELETE FROM `creature_text` WHERE `CreatureID` IN (49874,951);
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(49874,0,0,'Orc KILL $r!',12,0,100,0,0,0,0,'Blackrock Spy'),
(49874,0,1,'Blackrock take forest!',12,0,100,0,0,0,0,'Blackrock Spy'),
(49874,0,2,'The grapes were VERY TASTY!',12,0,100,0,0,0,0,'Blackrock Spy'),
(49874,0,3,'Eat you!',12,0,100,0,0,0,0,'Blackrock Spy'),
(951,0,0,'Be healed, $g brother:sister;!',12,0,100,0,0,0,49889,'Brother Paxton'),
(951,0,1,'AND I LAY MY HANDS UPON YOU!',12,0,100,0,0,0,49890,'Brother Paxton'),
(951,0,2,'Let the Holy Light embrace you!',12,0,100,0,0,0,49891,'Brother Paxton'),
(951,0,3,'BY THE LIGHT BE RENEWED!',12,0,100,0,0,0,49892,'Brother Paxton'),
(951,0,4,'FIGHT ON, $G BROTHER:SISTER;!',12,0,100,0,0,0,49893,'Brother Paxton');
UPDATE `creature_template` SET `ScriptName` = 'npc_brother_paxton' WHERE `entry` = 951;
DELETE FROM `creature_text_locale` WHERE `CreatureID` = 49874 AND `GroupID` = 0 AND `ID` = 3 AND `locale` = 'frFR';
INSERT INTO `creature_text_locale` (`CreatureID`,`GroupID`,`ID`,`locale`,`Text`) VALUES (49874,0,3,'frFR','Manger toi !');
-- Stormwind infantry and Blackrock worgs: a real fight scripted in C++ instead of the endless sparring
DELETE FROM `creature_action` WHERE (`entry` = 49869 AND `target` = 49871) OR (`entry` = 49871 AND `target` = 49869);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (49869,49871) AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` BETWEEN 4986900 AND 4986903 AND `source_type` = 9;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_stormwind_infantry' WHERE `entry` = 49869;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_blackrock_battle_worg' WHERE `entry` = 49871;
DELETE FROM `creature_text` WHERE `CreatureID` = 49869;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(49869,0,0,'I could use a heal, brother!',12,0,100,0,0,0,49895,'Stormwind Infantry'),
(49869,0,1,'I need a heal!',12,0,100,0,0,0,49898,'Stormwind Infantry'),
(49869,0,2,'Make yourself useful and heal me, Paxton!',12,0,100,0,0,0,49896,'Stormwind Infantry'),
(49869,0,3,'HELP!',12,0,100,0,0,0,49897,'Stormwind Infantry');
DELETE FROM `creature_text_locale` WHERE `CreatureID` = 49869;
UPDATE `creature_template` SET `faction` = 14 WHERE `entry` = 49871;
DELETE FROM `creature_template_spell` WHERE `entry` = 49874 AND `spell` IN (80676,92857);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 49874;
UPDATE `creature_template` SET `dmg_multiplier` = 3 WHERE `entry` = 49871;
-- Blackrock Spies with a spyglass are male: the female model holds it towards the ground
UPDATE `creature` SET `modelid` = 36652 WHERE `guid` IN (190881,190887,190892,190898,190907);
UPDATE `creature` SET `modelid` = 36653 WHERE `guid` IN (190886,190891,190895,190902,190906);
-- Blackrock Spies: hostile to players, twice the health and the damage, scripted in C++ to leave the soldiers alone
DELETE FROM `smart_scripts` WHERE `entryorguid` = 49874 AND `source_type` = 0;
UPDATE `creature_template` SET `faction` = 14, `dmg_multiplier` = 2, `AIName` = '', `ScriptName` = 'npc_blackrock_spy' WHERE `entry` = 49874;
UPDATE `creature_template_wdb` SET `HpMulti` = 3 WHERE `Entry` = 49874;
-- Extinguishing Hope: the extinguisher is carried on the back while the quest is in the log
DELETE FROM `spell_area` WHERE `spell` = 80209 AND `area` IN (9,59);
INSERT INTO `spell_area` (`spell`,`area`,`quest_start`,`quest_end`,`aura_spell`,`racemask`,`classmask`,`active_event`,`gender`,`autocast`,`quest_start_status`,`quest_end_status`) VALUES
(80209,9,26391,0,0,0,0,0,2,1,10,0),
(80209,59,26391,0,0,0,0,0,2,1,10,0);
-- Goblin Assassins: twice the health, two and a half times the damage
UPDATE `creature_template` SET `dmg_multiplier` = 2.5 WHERE `entry` = 50039;
UPDATE `creature_template_wdb` SET `HpMulti` = 3 WHERE `Entry` = 50039;
-- Blackrock Invaders: two and a half times the damage
UPDATE `creature_template` SET `dmg_multiplier` = 2.5 WHERE `entry` = 42937;
