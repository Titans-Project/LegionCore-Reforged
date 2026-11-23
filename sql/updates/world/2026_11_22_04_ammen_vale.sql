-- Ammen Vale: SmartAI of the starting zone NPCs, the caged mutation of Botanist Taerix spawned
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (16514,16521,16534,17073);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 16514 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(16514,0,0,0,'',20,0,100,0,9294,0,0,0,0,1,0,5000,0,0,0,0,1,0,0,0,0,0,0,0,0,'Botanist Taerix - On Quest Reward - Say Line 1'),
(16514,0,1,2,'',52,0,100,0,0,16514,0,0,0,1,0,5000,0,0,0,0,19,20227,0,0,0,0,0,0,0,'Botanist Taerix - On Text Over - Say Line 1 (Apprentice Tedon)'),
(16514,0,2,3,'',61,0,100,0,0,0,0,0,0,9,0,0,0,0,0,0,14,7279,181370,0,0,0,0,0,0,'Botanist Taerix - On Text Over - Activate Volatile mutation cage'),
(16514,0,3,0,'',61,0,100,0,0,0,0,0,0,45,1,1,0,0,0,0,10,146941537,16516,0,0,0,0,0,0,'Botanist Taerix - On Text Over - Set Data on Volatile Mutation'),
(16514,0,4,5,'',52,0,100,0,0,20227,0,0,0,1,1,5000,0,0,0,0,19,20227,0,0,0,0,0,0,0,'Botanist Taerix - On Text Over - Say Line 1 (Apprentice Tedon)'),
(16514,0,5,6,'',61,0,100,0,0,0,0,0,0,45,2,2,0,0,0,0,10,146941537,16516,0,0,0,0,0,0,'Botanist Taerix - On Text Over - Set Data on Volatile Mutation'),
(16514,0,6,0,'',61,0,100,0,0,0,0,0,0,5,133,0,0,0,0,0,19,20227,0,0,0,0,0,0,0,'Botanist Taerix - On Text Over - Play emote STATE_USESTANDING_NOSHEATHE(Apprentice Tedon)'),
(16514,0,7,8,'',52,0,100,0,1,20227,0,0,0,5,30,0,0,0,0,0,19,20227,0,0,0,0,0,0,0,'Botanist Taerix - On Text Over - Play emote STATE_NONE (Apprentice Tedon)'),
(16514,0,8,0,'',61,0,100,0,0,0,0,0,0,1,2,30000,0,0,0,0,19,20227,0,0,0,0,0,0,0,'Botanist Taerix - On Text Over - Say Line 1 (Apprentice Tedon)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 16521 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(16521,0,0,0,'',4,0,30,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Blood Elf Scout - On Aggro - Say Line 0'),
(16521,0,1,0,'',0,0,100,0,4000,9000,30000,30000,0,11,25602,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Blood Elf Scout - In Combat - Cast ''Faerie Fire''');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 16534 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(16534,0,0,0,'',1,0,100,1,1000,1000,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Inoculated Nestlewood Owlkin - Out of Combat - Say Line 0 (Dungeon)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 17073 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(17073,0,0,0,'',1,0,100,0,25000,35000,45000,65000,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Draenei Mourner - Out of Combat - Say Line 0 (No Repeat)');
DELETE FROM `creature_text` WHERE `CreatureID` = 16514 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 16534 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 17073 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 20227 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 20227 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 20227 AND `GroupID` = 2;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(16514,0,0,'Tedon, go ahead and try the neutralizing agent on the mutated creature.',12,0,100,1,0,0,17938,'Botanist Taerix'),
(16534,0,0,'The %s nods appreciatively.',16,0,100,2,0,0,12514,'Inoculated Nestlewood Owlkin'),
(16534,0,1,'The %s looks confused.',16,0,100,6,0,0,12517,'Inoculated Nestlewood Owlkin'),
(16534,0,2,'The %s didn''t like what just happened.',16,0,100,18,0,0,12521,'Inoculated Nestlewood Owlkin'),
(16534,0,3,'The %s seems overjoyed.',16,0,100,4,0,0,12516,'Inoculated Nestlewood Owlkin'),
(16534,0,4,'The %s doesn''t look like it minds the crystal''s effects.',16,0,100,22,0,0,12520,'Inoculated Nestlewood Owlkin'),
(17073,0,0,'<Sobbing> I hate graveyards.  So many dead.  And my sweet Luhanaa....',12,0,100,1,0,0,13413,'Draenei Mourner - Random Say'),
(17073,0,1,'It should have been me!',12,7,100,1,0,0,13419,'Draenei Mourner - Random Say'),
(17073,0,2,'Be at peace, my sweet, sweet, Luhanaa.',12,7,100,1,0,0,13425,'Draenei Mourner - Random Say'),
(17073,0,3,'I''ll never forget you, my love.',12,7,100,1,0,0,13420,'Draenei Mourner - Random Say'),
(17073,0,4,'If I ever find the ones responsible for the crash...!',12,7,100,1,0,0,13423,'Draenei Mourner - Random Say'),
(17073,0,5,'We should never have come along.',12,7,100,1,0,0,13421,'Draenei Mourner - Random Say'),
(17073,0,6,'Why, Luhanaa?  Why?!',12,7,100,6,0,0,13418,'Draenei Mourner - Random Say'),
(17073,0,7,'I miss you so much!',12,7,100,1,0,0,13422,'Draenei Mourner - Random Say'),
(17073,0,8,'You''ll always be here, with me.',12,7,100,1,0,0,13424,'Draenei Mourner - Random Say'),
(17073,0,9,'%s weeps softly.',16,0,100,18,0,0,13414,'Draenei Mourner - Random Say'),
(17073,0,10,'%s bows his head and sighs, clearly exhausted.',16,0,100,2,0,0,13417,'Draenei Mourner - Random Say'),
(17073,0,11,'%s breaks down into huge, wracking sobs.',16,0,100,18,0,0,13416,'Draenei Mourner - Random Say'),
(17073,0,12,'%s stares in silence at the grave marker before him.',16,0,100,0,0,0,13415,'Draenei Mourner - Random Say'),
(20227,0,0,'Looks like it''s time to let you out of this cage.',12,0,100,1,0,0,17925,'Apprentice Tedon'),
(20227,1,0,'Now, to put the neutralizing agent on it...',12,0,100,0,0,0,17934,'Apprentice Tedon'),
(20227,2,0,'Success!',12,0,100,71,0,0,17939,'Apprentice Tedon');

DELETE FROM `creature` WHERE `guid` = 146941537;
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`PhaseId`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`MovementType`) VALUES
(146941537,16516,530,6456,3527,1,1,'',-4059.08,-13711.4,73.5848,5.88176,120,0);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 16516;
DELETE FROM `smart_scripts` WHERE `entryorguid` = -146941537 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(-146941537,0,0,0,'',38,0,100,0,1,1,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-4057.12,-13706.8,73.58,0,'Volatile Mutation - On Data Set 1 1 - Move out of the cage'),
(-146941537,0,1,2,'',38,0,100,0,2,2,0,0,0,11,35068,2,0,0,0,0,1,0,0,0,0,0,0,0,0,'Volatile Mutation - On Data Set 2 2 - Cast Volatile Mutation Transformation'),
(-146941537,0,2,3,'',61,0,100,0,0,0,0,0,0,59,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Volatile Mutation - On Data Set 2 2 - Set Run'),
(-146941537,0,3,4,'',61,0,100,0,0,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,0,-4049.3,-13688.4,73,0,'Volatile Mutation - On Data Set 2 2 - Run away'),
(-146941537,0,4,0,'',61,0,100,0,0,0,0,0,0,41,10000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Volatile Mutation - On Data Set 2 2 - Despawn');
