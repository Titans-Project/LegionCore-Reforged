-- Sunstrider Isle: Delios Silverblade spawned, SmartAI of the starting zone NPCs
DELETE FROM `creature` WHERE `guid` = 146941536;
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`PhaseId`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`MovementType`) VALUES
(146941536,43010,530,6455,3431,1,1,'',10381.3,-6422.5,38.6156,2.68781,300,0);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (15281,15371,15494,20244);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 15281 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(15281,0,0,1,'',1,0,100,0,0,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Lanthan Perilon - Out of Combat - Disable Combat Movement'),
(15281,0,1,0,'',61,0,100,0,0,0,0,0,0,22,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Lanthan Perilon - Out of Combat - Set Event Phase 0'),
(15281,0,2,3,'',4,0,100,0,0,0,0,0,0,11,20811,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Lanthan Perilon - On Aggro - Cast ''Fireball'''),
(15281,0,3,0,'',61,0,100,0,0,0,0,0,0,23,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Lanthan Perilon - On Aggro - Increment Phase By 1'),
(15281,0,4,0,'',9,0,100,0,0,40,3400,4800,0,11,20811,0,0,0,0,0,2,0,0,0,0,0,0,0,0,'Lanthan Perilon - Within 0-40 Range - Cast ''Fireball'''),
(15281,0,5,6,'',3,0,100,0,0,7,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Lanthan Perilon - Between 0-7% Mana - Enable Combat Movement'),
(15281,0,6,0,'',61,0,100,0,0,0,0,0,0,23,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Lanthan Perilon - Between 0-7% Mana - Increment Phase By 1'),
(15281,0,7,0,'',9,0,100,1,35,80,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Lanthan Perilon - Within 35-80 Range - Enable Combat Movement'),
(15281,0,8,0,'',9,0,100,1,5,15,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Lanthan Perilon - Within 5-15 Range - Disable Combat Movement'),
(15281,0,9,0,'',9,0,100,1,0,5,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Lanthan Perilon - Within 0-5 Range - Enable Combat Movement'),
(15281,0,10,0,'',3,0,100,0,15,100,100,100,0,23,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,'Lanthan Perilon - Between 15-100% Mana - Decrement Phase By 1'),
(15281,0,11,0,'',2,0,100,1,0,15,0,0,0,22,3,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Lanthan Perilon - Between 0-15% Health - Set Event Phase 3'),
(15281,0,12,13,'',2,0,100,1,0,15,0,0,0,21,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Lanthan Perilon - Between 0-15% Health - Enable Combat Movement (No Repeat)'),
(15281,0,13,0,'',61,0,100,1,0,0,0,0,0,25,1,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Lanthan Perilon - Between 0-15% Health - Flee For Assist (No Repeat)');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 15371 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(15371,0,0,0,'',22,0,30,0,78,1000,1000,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Sunstrider Guardian - Received Emote 78 - Say Line 0'),
(15371,0,1,0,'',22,0,30,0,77,1000,1000,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Sunstrider Guardian - Received Emote 77 - Say Line 1');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 15494 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(15494,0,0,0,'',1,0,100,1,0,0,0,0,0,11,11939,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Yasmine Teli/Larien - On Just Summoned - Summon Imp');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 20244 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(20244,0,0,0,'',1,0,100,0,10000,10000,25000,60000,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Nova - Out Of Combat - Say Line'),
(20244,0,1,0,'',1,0,100,0,11000,11000,25000,60000,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Nova - Out Of Combat - Say Line');
DELETE FROM `creature_text` WHERE `CreatureID` = 15371 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 15371 AND `GroupID` = 1;
DELETE FROM `creature_text` WHERE `CreatureID` = 20244 AND `GroupID` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 20244 AND `GroupID` = 1;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(15371,0,0,'Move along, $C!',12,0,100,66,0,0,10992,'Sunstrider Guardian'),
(15371,0,1,'Off with you, $n!',12,0,100,66,0,0,10993,'Sunstrider Guardian'),
(15371,1,0,'Do not push it citizen!',12,0,100,1,0,0,10994,'Sunstrider Guardian'),
(20244,0,0,'%s picks up a sea shell.',16,10,100,0,0,0,17922,'Nova'),
(20244,0,1,'%s holds a sea shell up to her ear.',16,10,100,0,0,0,17923,'Nova'),
(20244,0,2,'%s shakes the dirt loose from the shell.',16,10,100,0,0,0,17924,'Nova'),
(20244,1,0,'I think I can see the Sunwell from here!',12,10,100,1,0,0,17918,'Nova'),
(20244,1,1,'Can you really hear the ocean from one of these shells?',12,10,100,1,0,0,17919,'Nova'),
(20244,1,2,'Oooh! Look, a shiny one!',12,10,100,1,0,0,17920,'Nova'),
(20244,1,3,'Jane will love this one!',12,10,100,1,0,0,17921,'Nova');
