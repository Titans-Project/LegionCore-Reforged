-- Deadwind Pass: Legion ship over Karazhan and Kirin Tor camp quests

-- Deadwind Pass: the Legion ship over Karazhan shows its own model to every player, its fel engine carried by an invisible bunny at its centre.
UPDATE `creature` SET `modelid` = 67380 WHERE `guid` = 349787 AND `id` = 113507;
DELETE FROM `creature_addon` WHERE `guid` = 349787;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (349787, 0, 0, 50331648, 0, 0, '');
DELETE FROM `creature_template` WHERE `entry` = 600103;
DELETE FROM `creature_template_wdb` WHERE `Entry` = 600103;
DELETE FROM `creature_template_addon` WHERE `entry` = 600103;
DELETE FROM `creature_template_movement` WHERE `CreatureId` = 600103;
CREATE TEMPORARY TABLE `tmp_engine` SELECT * FROM `creature_template` WHERE `entry` = 23837;
UPDATE `tmp_engine` SET `entry` = 600103, `scale` = 0.8333, `MaxVisible` = 0, `VisibilityDistance` = 0, `AIName` = '', `ScriptName` = '';
INSERT INTO `creature_template` SELECT * FROM `tmp_engine`;
DROP TEMPORARY TABLE `tmp_engine`;
CREATE TEMPORARY TABLE `tmp_engine` SELECT * FROM `creature_template_wdb` WHERE `Entry` = 23837;
UPDATE `tmp_engine` SET `Entry` = 600103, `Name1` = 'Legion Ship Fel Engine (Deadwind)';
INSERT INTO `creature_template_wdb` SELECT * FROM `tmp_engine`;
DROP TEMPORARY TABLE `tmp_engine`;
CREATE TEMPORARY TABLE `tmp_engine` SELECT * FROM `creature_template_movement` WHERE `CreatureId` = 23837;
UPDATE `tmp_engine` SET `CreatureId` = 600103;
INSERT INTO `creature_template_movement` SELECT * FROM `tmp_engine`;
DROP TEMPORARY TABLE `tmp_engine`;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (600103, 0, 0, 0, 1, 0, '225713');
DELETE FROM `creature` WHERE `guid` = 600035;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`, `isActive`) VALUES
(600035, 600103, 0, 41, 2558, 1, 1, '7552', 11686, -10658.1, -1839.28, 997.658, 0.494883, 120, 0, 0, 0);
DELETE FROM `creature_movement_override` WHERE `SpawnId` = 600035;
INSERT INTO `creature_movement_override` (`SpawnId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Random`) VALUES (600035, 0, 0, 1, 1, 0);

-- Return to Karazhan (Deadwind Pass): quest order, Karlain for both factions, objective credits, missing quest creatures
-- quest order of 7.1: Return to Karazhan opens Finite Numbers, Holding the Lines and Corruption Runs Deep;
-- those three open Reclaiming the Ramparts and Thought Collection; then Demon in Disguise, The Power of Corruption, the dungeon
UPDATE `quest_template_addon` SET `PrevQuestID`=44556 WHERE `ID` IN (44557,44683,44684);
UPDATE `quest_template_addon` SET `PrevQuestID`=44686 WHERE `ID`=44764;
UPDATE `quest_template_addon` SET `PrevQuestID`=44764 WHERE `ID`=44733;
UPDATE `quest_template_addon` SET `PrevQuestID`=44733 WHERE `ID` IN (44734,44735);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=19 AND `SourceEntry` IN (44685,44686);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(19,0,44685,0,0,8,0,44557,0,0,0,0,'','Reclaiming the Ramparts: Finite Numbers rewarded'),
(19,0,44685,0,0,8,0,44683,0,0,0,0,'','Reclaiming the Ramparts: Holding the Lines rewarded'),
(19,0,44685,0,0,8,0,44684,0,0,0,0,'','Reclaiming the Ramparts: Corruption Runs Deep rewarded'),
(19,0,44686,0,0,8,0,44557,0,0,0,0,'','Thought Collection: Finite Numbers rewarded'),
(19,0,44686,0,0,8,0,44683,0,0,0,0,'','Thought Collection: Holding the Lines rewarded'),
(19,0,44686,0,0,8,0,44684,0,0,0,0,'','Thought Collection: Corruption Runs Deep rewarded');

-- Archmage Karlain gives three links of a chain open to every race: friendly to the Horde as well
UPDATE `creature_template` SET `faction`=35 WHERE `entry`=114631;

-- spells aimed at nearby creatures of one entry
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry` IN (228271,229170,228208,231458,229466);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,1,228271,0,0,31,0,3,114641,0,0,0,'','Collecting Sample: Soil Sample'),
(13,1,228271,0,1,31,0,3,114645,0,0,0,'','Collecting Sample: Water Sample'),
(13,1,229170,0,0,31,0,3,115219,0,0,0,'','Planting Ward: Southern Ward'),
(13,1,229170,0,1,31,0,3,115227,0,0,0,'','Planting Ward: Northern Ward'),
(13,1,229170,0,2,31,0,3,115228,0,0,0,'','Planting Ward: Eastern Ward'),
(13,1,228208,0,0,31,0,3,115027,0,0,0,'','Eastern Ley Line: Fel Spreader'),
(13,1,231458,0,0,31,0,3,115037,0,0,0,'','Western Ley Line: Fel Spreader'),
(13,1,229466,0,0,31,0,3,115414,0,0,0,'','Legion Portal disabled: Legion Portal');

-- Corruption Runs Deep: the vial filled near a sample; Reclaiming the Ramparts: a ward planted on its spot
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (114641,114645,115219,115227,115228,115414,114822,111345);
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (114641,114645,115219,115227,115228,115414,114822,-373191);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(114641,0,0,0,'',8,0,100,0,228271,0,0,0,0,33,114641,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Soil Sample - On Spellhit Collecting Sample - Quest Credit'),
(114645,0,0,0,'',8,0,100,0,228271,0,0,0,0,33,114645,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Water Sample - On Spellhit Collecting Sample - Quest Credit'),
(115219,0,0,0,'',8,0,100,0,229170,0,0,0,0,33,114780,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Southern Ward - On Spellhit Planting Ward - Quest Credit'),
(115227,0,0,0,'',8,0,100,0,229170,0,0,0,0,33,114778,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Northern Ward - On Spellhit Planting Ward - Quest Credit'),
(115228,0,0,0,'',8,0,100,0,229170,0,0,0,0,33,114779,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Eastern Ward - On Spellhit Planting Ward - Quest Credit'),
-- Finite Numbers: a closed portal is gone until it reopens, so one portal counts once
(115414,0,0,0,'',8,0,100,0,229466,0,0,0,0,41,1000,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Legion Portal - On Spellhit Legion Portal disabled - Despawn'),
-- Thought Collection: Rezzle answers
(114822,0,0,1,'',62,0,100,0,20535,0,0,0,0,11,228682,2,0,0,0,0,7,0,0,0,0,0,0,0,0,'Captured Wyrmtongue - On Gossip Select - Cast Demon Questioned'),
(114822,0,1,0,'',61,0,100,0,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Captured Wyrmtongue - Linked - Close Gossip'),
-- The Power of Corruption: the disguised player stepping on the plate before the fel curtain gains access
(-373191,0,0,0,'',10,0,100,0,1,3,1000,1000,0,11,229348,2,0,0,0,0,7,0,0,0,0,0,0,0,0,'Chamber access plate - Disguised player in range - Cast Gained Access');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceEntry`=-373191;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=20535 AND `SourceEntry`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(22,1,-373191,0,0,1,0,229332,0,0,0,0,'','Chamber access plate: player wears the Wyrmtongue Disguise'),
(22,1,-373191,0,0,9,0,44733,0,0,0,0,'','Chamber access plate: player is on The Power of Corruption'),
(15,20535,0,0,0,9,0,44686,0,0,0,0,'','Captured Wyrmtongue: questioning only on Thought Collection');
UPDATE `creature_template` SET `npcflag`=1 WHERE `entry`=114822;

-- the import left the three Legion Portals under the generic template; the fourth Portal-Keeper had lost its portal
UPDATE `creature` SET `id`=115414, `spawntimesecs`=30 WHERE `guid` IN (373184,373185,373206) AND `id` IN (111345,115414);
DELETE FROM `creature` WHERE `guid` BETWEEN 146941000 AND 146941006;
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`PhaseId`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`npcflag2`,`unit_flags`,`dynamicflags`,`AiID`,`MovementID`,`MeleeID`,`isActive`,`skipClone`,`personal_size`,`isTeemingSpawn`,`unit_flags3`) VALUES
(146941000,115414,0,41,2562,1,1,'',0,0,-11304.7,-2200.98,22.79,0.785457,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
-- the two Fel Spreaders on the ley lines, the eastern one in the southern cellar, the western one under the Disturbed Energy
(146941001,115027,0,41,2562,1,1,'',0,0,-11194,-2196,20.5,0,180,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
(146941002,115037,0,41,2562,1,1,'',0,0,-11182,-1926,-14,0,180,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
-- the soil to sample in the southern cellar
(146941003,114641,0,41,2562,1,1,'',0,0,-11088,-2207,17.8,0,180,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
(146941004,114641,0,41,2562,1,1,'',0,0,-11100,-2194,15.45,0,180,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
(146941005,114641,0,41,2562,1,1,'',0,0,-11076,-2221,16.05,0,180,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
-- Elux'ara Darkscorn beside Thar'zul in the hidden chamber
(146941006,115179,0,41,2562,1,1,'',0,0,-10919,-1874,-15.4,3.0,180,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

-- Deadwind Widows: they hang from an invisible stalker under branches and rock arches, the raiders of the ogre camps fight the ogres for show
UPDATE `creature_template` SET `ScriptName` = 'npc_deadwind_widow' WHERE `entry` = 51983;
UPDATE `creature_template` SET `faction` = 1925, `ScriptName` = 'npc_deadwind_widow_raider' WHERE `entry` = 51987;
DELETE FROM `creature_template_movement` WHERE `CreatureId` = 51984;
INSERT INTO `creature_template_movement` (`CreatureId`, `Ground`, `Swim`, `Flight`, `Rooted`) VALUES (51984, 1, 0, 1, 1);

-- Deadwind Pass: the Legion invasion sky over the whole zone, sent by the server with the screen effect of the invasion sky aura
DELETE FROM `spell_area` WHERE `spell` = 305164 AND `area` = 41;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `classmask`, `active_event`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(305164, 41, 0, 0, 0, 0, 0, 0, 2, 1, 64, 11);
