-- Demon hunter artifact chain from Dalaran: Call of the Illidari, the weapon choice, Making Arrangements, By Any Means and The Hunt
UPDATE `quest_template_addon` SET `PrevQuestID` = 39261 WHERE `ID` = 40814;
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 99247 AND `AIName` = '';
DELETE FROM `smart_scripts` WHERE `entryorguid` = 99247 AND `source_type` = 0 AND `id` IN (0,1);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_chance`,`event_param1`,`action_type`,`action_param1`,`action_param2`,`target_type`,`comment`) VALUES
(99247,0,0,0,19,100,40814,85,201092,0,7,'Quest Accept - Invoker Cast Spell (weapon choice)'),
(99247,0,1,0,62,100,19214,85,201092,0,7,'Gossip Select - Invoker Cast Spell (weapon choice)');

-- The weapon choice can be reopened from Kayn or Altruis while the choice quest is still in progress
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 19214 AND `SourceEntry` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(15,19214,0,0,0,9,0,40814,0,0,0,0,'','Weapon choice gossip: The Power to Survive (Kayn) in progress'),
(15,19214,0,0,1,9,0,40816,0,0,0,0,'','Weapon choice gossip: The Power to Survive (Altruis) in progress');

-- Making Arrangements: each mentor gives the version of his path (Kayn 40819, Altruis 41120), Altruis waits in front of the Violet Hold next to Kayn and both lead to By Any Means
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 19 AND `SourceGroup` = 0 AND `SourceEntry` IN (40819, 41120);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(19,0,40819,0,0,8,0,40814,0,0,0,0,'','Making Arrangements (Kayn): The Power to Survive (Kayn) rewarded'),
(19,0,41120,0,0,8,0,40816,0,0,0,0,'','Making Arrangements (Altruis): The Power to Survive (Altruis) rewarded');
DELETE FROM `creature_queststarter` WHERE `quest` IN (40819, 41120) AND `id` = 99254;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (99254, 41120);
DELETE FROM `disables` WHERE `sourceType` = 1 AND `entry` = 41120;
UPDATE `quest_template_addon` SET `NextQuestID` = 39051 WHERE `ID` = 41120;
DELETE FROM `creature` WHERE `guid` = 9101001;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`) VALUES
(9101001, 103156, 1220, 7502, 7592, 1, 1, '6218', -950.95, 4331.02, 740.35, 3.87571, 180);

DELETE FROM `disables` WHERE `sourceType` = 1 AND `entry` IN (41121,41119,40247,40249);
INSERT INTO `disables` (`sourceType`,`entry`,`flags`,`params_0`,`params_1`,`comment`) VALUES
(1,41121,0,'','','Demon hunter artifact quest variant without any giver: By Any Means'),
(1,41119,0,'','','Demon hunter artifact quest variant without any giver: The Hunt'),
(1,40247,0,'','','Demon hunter artifact quest variant without any giver: Asking a Favor'),
(1,40249,0,'','','Demon hunter artifact quest variant without any giver: Vengeance Will Be Ours');

UPDATE `quest_offer_reward` SET `RewardText` = 'Ah, $n, it''s good to see you on your feet! The members of the Council of Six are recuperating from their ordeal.$B$BThe intensity of the teleportation spell can be disorienting. It seems you lost consciousness for a time, but at least you didn''t awaken to find yourself stuck in a wall!$B$BDalaran has been relocated to the Broken Isles. From here, we will spearhead the effort to acquire the Pillars of Creation and drive the Legion from Azeroth once and for all!' WHERE `ID` = 44663;

-- Making Arrangements: the mentor says "See you there." when the quest is accepted (Kayn's line had been mixed into another text group, Altruis waited for his old quest and a missing group)
UPDATE `creature_text` SET `GroupID` = 4 WHERE `CreatureID` = 99247 AND `BroadcastTextId` = 105832;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 99247 AND `source_type` = 0 AND `id` = 2;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`action_type`,`action_param1`,`target_type`,`comment`) VALUES
(99247,0,2,0,19,0,100,0,40819,1,4,7,'Quest Accept - Talk (See you there)');
UPDATE `smart_scripts` SET `event_param1` = 41120, `action_param1` = 2, `comment` = 'Quest Accept - Talk (See you there)' WHERE `entryorguid` = 99254 AND `source_type` = 0 AND `id` = 2;

-- Krasus' Landing: the knife of the Illidari table lies on the battle map instead of floating above it
UPDATE `gameobject` SET `position_z` = 747.50 WHERE `guid` = 128002 AND `id` = 246349;

-- By Any Means, Warden Alturas in front of the Violet Hold: every answer opens the hold (one left the player stuck), Kayn speaks his two lines to the warden, each line waits for the end of the previous voice (the longest of English and French); each answer runs the whole scene in one action list (an action list started from another one does not run)
DELETE FROM `creature_text_locale` WHERE `Locale` = 'frFR' AND ((`CreatureID` = 99247 AND `GroupID` = 4) OR (`CreatureID` = 99254 AND `GroupID` = 2) OR (`CreatureID` = 108572 AND `GroupID` = 3) OR (`CreatureID` = 94902 AND `GroupID` = 0));
DELETE FROM `smart_scripts` WHERE `entryorguid` = 96313 AND `source_type` = 0 AND `id` BETWEEN 0 AND 7;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (9631300, 9631301) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`target_type`,`target_param1`,`target_param2`,`comment`) VALUES
(96313,0,0,1,62,0,100,0,19303,0,0,0,72,0,0,0,7,0,0,'Gossip no time to explain - Close gossip'),
(96313,0,1,0,61,0,100,0,0,0,0,0,80,9631300,2,0,1,0,0,'Link - Warden refuses, Kayn explains'),
(96313,0,2,4,62,0,100,0,19304,0,0,0,72,0,0,0,7,0,0,'Gossip see for yourself - Close gossip'),
(96313,0,3,4,62,0,100,0,19304,1,0,0,72,0,0,0,7,0,0,'Gossip what do you have to lose - Close gossip'),
(96313,0,4,0,61,0,100,0,0,0,0,0,80,9631301,2,0,1,0,0,'Link - Warden gives way'),
(9631300,9,0,0,0,0,100,0,0,0,0,0,81,0,0,0,1,0,0,'Warden - Remove gossip during the talk'),
(9631300,9,1,0,0,0,100,0,0,0,0,0,1,0,0,0,7,0,0,'Warden - Say no time to waste'),
(9631300,9,2,0,0,0,100,0,4800,4800,0,0,1,0,0,0,19,94902,30,'Kayn - Say a few moments with the eredar'),
(9631300,9,3,0,0,0,100,0,9100,9100,0,0,1,1,0,0,7,0,0,'Warden - Say Khadgar trusts you'),
(9631300,9,4,0,0,0,100,0,7300,7300,0,0,1,2,0,0,7,0,0,'Warden - Say is that clear'),
(9631300,9,5,0,0,0,100,0,7200,7200,0,0,1,1,0,0,19,94902,30,'Kayn - Say crystal, warden'),
(9631300,9,6,0,0,0,100,0,2900,2900,0,0,33,96313,0,0,7,0,0,'Warden - Credit convince the warden'),
(9631300,9,7,0,0,0,100,0,0,0,0,0,85,188163,0,0,7,0,0,'Player - Teleport into the Violet Hold'),
(9631300,9,8,0,0,0,100,0,0,0,0,0,81,1,0,0,1,0,0,'Warden - Gossip back'),
(9631301,9,0,0,0,0,100,0,0,0,0,0,81,0,0,0,1,0,0,'Warden - Remove gossip during the talk'),
(9631301,9,1,0,0,0,100,0,0,0,0,0,1,1,0,0,7,0,0,'Warden - Say Khadgar trusts you'),
(9631301,9,2,0,0,0,100,0,7300,7300,0,0,1,2,0,0,7,0,0,'Warden - Say is that clear'),
(9631301,9,3,0,0,0,100,0,7200,7200,0,0,1,1,0,0,19,94902,30,'Kayn - Say crystal, warden'),
(9631301,9,4,0,0,0,100,0,2900,2900,0,0,33,96313,0,0,7,0,0,'Warden - Credit convince the warden'),
(9631301,9,5,0,0,0,100,0,0,0,0,0,85,188163,0,0,7,0,0,'Player - Teleport into the Violet Hold'),
(9631301,9,6,0,0,0,100,0,0,0,0,0,81,1,0,0,1,0,0,'Warden - Gossip back');

-- The Hunt: at Krasus' Landing, Kayn sends the hunter off with Lyana and Illysana (his line was only kept three times in another text group)
DELETE FROM `creature_text` WHERE `CreatureID` = 99247 AND `GroupID` = 3;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextId`,`comment`) VALUES
(99247,3,0,'Darksorrow and Ravencrest will have the honor of accompanying you into Suramar, $p. A small force will have the best chance of flying to the hold undetected.',12,0,100,0,0,0,96561,'Kayn Sunfury - The Hunt');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 99247 AND `source_type` = 0 AND `id` = 3;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`target_type`,`target_param1`,`target_param2`,`comment`) VALUES
(99247,0,3,0,10,0,100,0,1,20,120000,120000,1,3,0,0,7,0,0,'Player in range on The Hunt - Talk');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 4 AND `SourceEntry` = 99247;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(22,4,99247,0,0,9,0,39247,0,0,0,0,'','Kayn sends off the hunt: The Hunt in progress');

-- Violet Hold entrance: the scene of By Any Means had its own guards always shown over the usual ones (phase 6261 is always on in Dalaran); the scene gets its own phase from Making Arrangements until The Hunt is turned in, the usual guards and Lieutenant Sinclari step aside meanwhile
DELETE FROM `phase_definitions` WHERE `zoneId` = 7502 AND `entry` IN (1203, 1204);
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `PreloadMapID`, `VisibleMapID`, `UiWorldMapAreaID`, `flags`, `comment`) VALUES
(7502, 1203, 0, 6218, 0, 0, 0, 16, 'Violet Hold entrance: By Any Means scene'),
(7502, 1204, 0, 6219, 0, 0, 0, 16, 'Violet Hold entrance: usual guards');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 7502 AND `SourceEntry` IN (1203, 1204);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,7502,1203,0,0,9,0,40819,0,0,0,0,'','Violet Hold quest phase: Making Arrangements 40819 in progress'),
(23,7502,1203,0,0,8,0,39247,0,0,1,0,'','Violet Hold quest phase: The Hunt not turned in yet'),
(23,7502,1203,0,1,28,0,40819,0,0,0,0,'','Violet Hold quest phase: Making Arrangements 40819 complete'),
(23,7502,1203,0,1,8,0,39247,0,0,1,0,'','Violet Hold quest phase: The Hunt not turned in yet'),
(23,7502,1203,0,2,8,0,40819,0,0,0,0,'','Violet Hold quest phase: Making Arrangements 40819 rewarded'),
(23,7502,1203,0,2,8,0,39247,0,0,1,0,'','Violet Hold quest phase: The Hunt not turned in yet'),
(23,7502,1203,0,3,9,0,41120,0,0,0,0,'','Violet Hold quest phase: Making Arrangements 41120 in progress'),
(23,7502,1203,0,3,8,0,39247,0,0,1,0,'','Violet Hold quest phase: The Hunt not turned in yet'),
(23,7502,1203,0,4,28,0,41120,0,0,0,0,'','Violet Hold quest phase: Making Arrangements 41120 complete'),
(23,7502,1203,0,4,8,0,39247,0,0,1,0,'','Violet Hold quest phase: The Hunt not turned in yet'),
(23,7502,1203,0,5,8,0,41120,0,0,0,0,'','Violet Hold quest phase: Making Arrangements 41120 rewarded'),
(23,7502,1203,0,5,8,0,39247,0,0,1,0,'','Violet Hold quest phase: The Hunt not turned in yet'),
(23,7502,1204,0,0,14,0,40819,0,0,0,0,'','Violet Hold usual guards: Making Arrangements (Kayn) never taken'),
(23,7502,1204,0,0,14,0,41120,0,0,0,0,'','Violet Hold usual guards: Making Arrangements (Altruis) never taken'),
(23,7502,1204,0,1,8,0,39247,0,0,0,0,'','Violet Hold usual guards: The Hunt turned in');
UPDATE `creature` SET `PhaseId` = '6218' WHERE `guid` IN (366276, 366277, 366279, 366280, 9101001);
UPDATE `creature` SET `PhaseId` = '6219' WHERE `guid` IN (272554, 272556, 272557, 272559);
-- the guards standing where the scene has nobody stay through the change of phase (272541 was doubled by 366278 at the same spot)
UPDATE `creature` SET `PhaseId` = '6218 6219' WHERE `guid` IN (272541, 272551, 272560);
DELETE FROM `creature` WHERE `guid` = 366278;
DELETE FROM `creature_addon` WHERE `guid` = 366278;

-- By Any Means, inside the Violet Hold: Warden Alturas leads the way to Taldath's cell at a brisker walk
UPDATE `creature_template` SET `speed_walk` = 1.6 WHERE `entry` = 96313;

-- By Any Means, inside the Violet Hold: every line waits for the end of the previous voice; a moment after Taldath's grip takes the hunter, his Shadow Nova throws him down to the ground floor and lets go, then Taldath teleports in front of him in fel flames and fights with his Felstomp and Fel Slagblast; Kayn's lines are cast by the hunter so that their portrait shows; the interrogation starts at 21% health (80% of the quarter he is released at) and runs line after line; the warden and Taldath stay passive while they walk and leap (a fight started on the way cancelled their movement)
UPDATE `creature` SET `MovementType` = 0 WHERE `guid` IN (376588, 376590, 376595);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 94731 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (9473100, 9473101) AND `source_type` = 9;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 96313 AND `source_type` = 0 AND `id` BETWEEN 8 AND 30;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`target_type`,`target_param1`,`target_param2`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(96313,0,8,0,60,0,100,1,0,0,0,0,81,0,0,0,1,0,0,0,0,0,0,'Update - Remove gossip'),
(96313,0,9,11,52,0,100,1,3,96313,0,0,53,1,96313,0,21,60,0,0,0,0,0,'Barrier line said - Run to the cell'),
(96313,0,10,12,10,0,100,1,1,30,0,0,1,3,8400,0,21,30,0,0,0,0,0,'Hunter in sight - Say lower the barrier'),
(96313,0,11,22,61,0,100,1,0,0,0,0,1,4,6000,0,1,0,0,0,0,0,0,'Link - Say follow me'),
(96313,0,22,0,61,0,100,1,0,0,0,0,8,0,0,0,1,0,0,0,0,0,0,'Link - Passive on the way (a prisoner in sight made him evade and turn back)'),
(96313,0,12,0,61,0,100,1,0,0,0,0,45,1,1,0,11,96308,30,0,0,0,0,'Link - The protector lowers the barrier'),
(96313,0,13,0,40,0,100,1,4,96313,0,0,86,201103,2,17,17,0,100,0,0,0,0,'Waypoint 4 - Kayn stands guard'),
(96313,0,14,0,40,0,100,1,10,96313,0,0,45,1,1,0,11,94731,50,0,0,0,0,'Waypoint 10 - Taldath sees the hunter'),
(96313,0,15,0,40,0,100,1,10,96313,0,0,11,190972,0,0,1,0,0,0,0,0,0,'Waypoint 10 - In front of the cell'),
(96313,0,16,0,38,0,100,1,3,3,0,0,1,5,8400,0,1,0,0,0,0,0,0,'Taldath has spoken - Say meet Taldath'),
(96313,0,17,21,52,0,100,1,5,96313,0,0,1,6,3800,0,1,0,0,0,0,0,0,'Taldath introduced - Say you were never here'),
(96313,0,18,19,52,0,100,1,6,96313,0,0,45,2,2,0,11,94731,50,0,0,0,0,'Last line said - Release Taldath'),
(96313,0,19,20,61,0,100,1,0,0,0,0,11,195453,0,0,1,0,0,0,0,0,0,'Link - Cast'),
(96313,0,20,0,61,0,100,1,0,0,0,0,41,1500,0,0,1,0,0,0,0,0,0,'Link - Despawn'),
(96313,0,21,0,61,0,100,1,0,0,0,0,9,0,0,0,15,246993,30,0,0,0,0,'Link - Open the cell');

UPDATE `smart_scripts` SET `action_param4` = 0, `action_param5` = 100 WHERE `action_type` = 86 AND `entryorguid` = 96313 AND `source_type` = 0 AND `id` = 13;
-- Taldath runs on his own script (npc_taldath_by_any_means): grip, throw down, teleport, fight and interrogation in order
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_taldath_by_any_means' WHERE `entry` = 94731;
-- Kayn's portraits stay up until his voice ends
UPDATE `conversation_actor` SET `duration` = 6000 WHERE `entry` IN (1093, 1096) AND `id` = 0;
UPDATE `conversation_actor` SET `duration` = 3500 WHERE `entry` = 1097 AND `id` = 0;

-- By Any Means, inside the Violet Hold: the hold keeps its cells, prisoners and guards for demon hunters after abandoning the quest; only the scene waits for the quest
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 7777 AND ((`SourceEntry` = 3 AND `ElseGroup` = 3) OR (`SourceEntry` = 4 AND `ElseGroup` = 9));
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` IN (1, 11) AND `SourceEntry` IN (94902, 96313) AND `ConditionTypeOrReference` = 9;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,7777,3,0,3,15,0,2048,0,0,0,0,'','Violet Hold copy: demon hunter prisoners and guards'),
(23,7777,4,0,9,15,0,2048,0,0,0,0,'','Violet Hold copy: cells and barriers for demon hunters'),
(22,11,96313,0,0,9,0,39051,0,0,0,0,'','Warden Alturas starts the scene: By Any Means in progress'),
(22,1,94902,0,0,9,0,39051,0,0,0,0,'','Kayn takes his place: By Any Means in progress');

-- By Any Means: the quest's copy of the Violet Hold had no way out; an invisible trigger at the entrance door, always present, takes back to Dalaran in front of the warden (also after abandoning the quest inside); it is not flagged as a trigger, which would keep it from noticing players
DELETE FROM `creature_template` WHERE `entry` = 600110;
DELETE FROM `creature_template_wdb` WHERE `Entry` = 600110;
DELETE FROM `creature_template_movement` WHERE `CreatureId` = 600110;
CREATE TEMPORARY TABLE `tmp_exit` SELECT * FROM `creature_template` WHERE `entry` = 23837;
UPDATE `tmp_exit` SET `entry` = 600110, `AIName` = 'SmartAI', `ScriptName` = '', `flags_extra` = 2;
INSERT INTO `creature_template` SELECT * FROM `tmp_exit`;
DROP TEMPORARY TABLE `tmp_exit`;
CREATE TEMPORARY TABLE `tmp_exit` SELECT * FROM `creature_template_wdb` WHERE `Entry` = 23837;
UPDATE `tmp_exit` SET `Entry` = 600110, `Name1` = 'Violet Hold Exit (By Any Means)';
INSERT INTO `creature_template_wdb` SELECT * FROM `tmp_exit`;
DROP TEMPORARY TABLE `tmp_exit`;
CREATE TEMPORARY TABLE `tmp_exit` SELECT * FROM `creature_template_movement` WHERE `CreatureId` = 23837;
UPDATE `tmp_exit` SET `CreatureId` = 600110;
INSERT INTO `creature_template_movement` SELECT * FROM `tmp_exit`;
DROP TEMPORARY TABLE `tmp_exit`;
DELETE FROM `creature` WHERE `guid` = 9101002;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`) VALUES
(9101002, 600110, 1494, 7777, 7777, 1, 1, '5014', 1270.0, -262.6, 44.53, 0, 300);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 600110 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`target_type`,`target_param1`,`target_param2`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(600110,0,0,0,10,0,100,0,1,5,2000,2000,62,1220,0,0,7,0,0,-961.5,4323.5,740.35,0.85,'Violet Hold exit - Back to Dalaran');

-- By Any Means: the quest is only taken outside the Violet Hold, the copy of the hold cannot be left by Kayn's side otherwise
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 19 AND `SourceEntry` = 39051 AND `ConditionTypeOrReference` = 22;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(19,0,39051,0,0,22,0,1494,0,0,1,0,'','By Any Means: not taken inside the Violet Hold copy');

-- By Any Means: the guards evacuating the hold leave by the entrance door instead of piling up in front of it
DELETE FROM `smart_scripts` WHERE `entryorguid` = 96312 AND `source_type` = 0 AND `id` = 2;
UPDATE `smart_scripts` SET `link` = 2 WHERE `entryorguid` = 96312 AND `source_type` = 0 AND `id` = 1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`target_type`,`target_param1`,`target_param2`,`comment`) VALUES
(96312,0,2,0,61,0,100,0,0,0,0,0,41,500,0,0,1,0,0,'Link - Leave the hold');

-- By Any Means: Warden Alturas checks that he himself stands in the hold; his events without a player (end of a line, spawn) failed the check and he never set off
UPDATE `conditions` SET `ConditionTarget` = 1 WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` IN (9, 10) AND `SourceEntry` = 96313 AND `ConditionTypeOrReference` = 4 AND `ConditionValue1` = 7777;

-- By Any Means: Taldath has about 5 million health and is released at a quarter of it
UPDATE `creature_template_wdb` SET `HpMulti` = 315 WHERE `Entry` = 94731;

-- By Any Means: Taldath's Shadow Nova throws only players, down to the ground floor of the hold
DELETE FROM `spell_target_position` WHERE `id` = 191144;
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(191144, 1494, 1361.5, -263.5, 38.67, 0);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 191028;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,1,191028,0,0,32,0,16,0,0,0,0,'','Shadow Nova: players only');

-- By Any Means: an arcane barrier (a straight Arcane Wall, 16 yards wide) closes the hall in front of the Kirin Tor Protector; during the warden's line the protector channels, the barrier opens, he leaves and only then the guards evacuate; the warden then runs to the cell
DELETE FROM `gameobject_template` WHERE `entry` = 600111;
CREATE TEMPORARY TABLE `tmp_barrier` SELECT * FROM `gameobject_template` WHERE `entry` = 243884;
UPDATE `tmp_barrier` SET `entry` = 600111, `name` = 'Violet Hold Barrier (By Any Means)', `size` = 0.36;
INSERT INTO `gameobject_template` SELECT * FROM `tmp_barrier`;
DROP TEMPORARY TABLE `tmp_barrier`;
DELETE FROM `gameobject` WHERE `guid` = 9101003;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(9101003, 600111, 1494, 7777, 7777, 4096, 1, '5020', 1319.00, -262.25, 44.1, 0, 0, 0, 0, 1, 7200, 255, 1);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 96308 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 9630800 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`target_type`,`target_param1`,`target_param2`,`comment`) VALUES
(96308,0,0,0,38,0,100,1,1,1,0,0,80,9630800,2,0,1,0,0,'Data set 1 - Lower the barrier');
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`target_type`,`target_param1`,`target_param2`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(9630800,9,0,0,0,0,100,0,1500,1500,0,0,66,0,0,0,8,0,0,0,0,0,0,'Protector - Turns from the entrance to the barrier'),
(9630800,9,1,0,0,0,100,0,1500,1500,0,0,11,190953,0,0,1,0,0,0,0,0,0,'Protector - Arcane Channel'),
(9630800,9,2,0,0,0,100,0,3000,3000,0,0,9,0,0,0,15,600111,12,0,0,0,0,'Protector - The barrier opens'),
(9630800,9,3,0,0,0,100,0,0,0,0,0,45,1,1,0,11,96312,15,0,0,0,0,'Protector - The guards evacuate'),
(9630800,9,4,0,0,0,100,0,0,0,0,0,41,1000,0,0,1,0,0,0,0,0,0,'Protector - Leave');
-- the protector watches the entrance until the warden calls on him
UPDATE `creature` SET `orientation` = 3.14159 WHERE `guid` = 376584;
UPDATE `creature_template` SET `speed_run` = 1.2 WHERE `entry` = 96313;

-- By Any Means: Warden Alturas' 4th and 5th points stood off the walkable way to the stairs, so he went around and turned back; they
-- now lie on it, at the foot of the stairs and on the steps
UPDATE `waypoints` SET `position_x` = 1384.6, `position_y` = -256.4, `position_z` = 43.99 WHERE `entry` = 96313 AND `pointid` = 4;
UPDATE `waypoints` SET `position_x` = 1385.5, `position_y` = -252.6, `position_z` = 46.53 WHERE `entry` = 96313 AND `pointid` = 5;

-- The Hunt: the fel bat takes off from Krasus' Landing before the teleport to Felsoul Hold, then flies over the hold with two escorts and ends with the dive onto the camp (npc_illidari_fel_bat_the_hunt)
UPDATE `smart_scripts` SET `link` = 0 WHERE `entryorguid` = 94321 AND `source_type` = 0 AND `id` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 94321 AND `source_type` = 0 AND `id` = 1;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 94324 AND `source_type` IN (0, 9);
DELETE FROM `waypoints` WHERE `entry` = 94324;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_illidari_fel_bat_the_hunt' WHERE `entry` = 94324;
DELETE FROM `spell_script_names` WHERE `spell_id` = 186733;
-- the click summons the bat at the one clicked; without its target entry the spell failed
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 186671;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 186671, 0, 0, 31, 0, 3, 94321, 0, 0, 0, '', 'Illidari Fel Bat summoned at the clicked bat');
-- the scenario's bat comes from the script on arrival, no longer from an area aura
DELETE FROM `spell_area` WHERE `spell` = 186671 AND `area` = 7768;
-- the hold's ground triggers act once per hunter (they started their talking heads again at each pass); the arrival one no longer summons a second bat
UPDATE `eventobject_template` SET `Flags` = `Flags` | 1 WHERE `entry` BETWEEN 110 AND 115;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 110 AND `source_type` = 13;
-- the Felsoul swarmers and doomlords hover where they fire at the bats; their random move brought them down to the ground
UPDATE `creature` SET `MovementType` = 0, `spawndist` = 0 WHERE `map` = 1498 AND `id` IN (101763, 101764);
-- Kayn's talking heads around The Hunt: on turning in By Any Means (1098, it had no lines) and on accepting The Hunt (1099, it ran at the turn-in, without its actor); cast by the hunter, so that the portrait shows
DELETE FROM `conversation_data` WHERE `entry` IN (1098, 1099);
INSERT INTO `conversation_data` (`entry`, `id`, `idx`, `textId`, `unk1`, `unk2`) VALUES
(1098, 2692, 0, 0, 119, 0),
(1098, 2693, 1, 0, 119, 0),
(1099, 2694, 0, 0, 119, 0);
DELETE FROM `conversation_actor` WHERE `entry` IN (1098, 1099);
INSERT INTO `conversation_actor` (`entry`, `id`, `actorId`, `creatureId`, `displayId`, `unk1`, `unk2`, `unk3`, `duration`) VALUES
(1098, 0, 50925, 96498, 61698, 0, 1, 0, 6500),
(1098, 1, 50925, 96498, 61698, 0, 1, 0, 11000),
(1099, 0, 50925, 96498, 61698, 0, 1, 0, 8500);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 94902 AND `source_type` = 0 AND `id` IN (1, 2);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `target_type`, `comment`) VALUES
(94902, 0, 1, 0, 20, 0, 100, 0, 39051, 0, 0, 0, 85, 201107, 0, 0, 7, 'Kayn - On By Any Means rewarded - Hunter casts his talking head (Twinblades)'),
(94902, 0, 2, 0, 19, 0, 100, 0, 39247, 0, 0, 0, 85, 201108, 0, 0, 7, 'Kayn - On The Hunt accepted - Hunter casts his talking head (Krasus'' Landing)');
-- four Felsoul swarmers circle the hold on wide free loops (500 to 800 yards, at least 35 yards above the ground); the doomlords and the two swarmers closest to the flight keep hovering where they fire
UPDATE `creature` SET `MovementType` = 2 WHERE `guid` IN (376299, 376300, 376301, 376304);
DELETE FROM `creature_addon` WHERE `guid` IN (376299, 376300, 376301, 376304);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(376299, 376299, 0, 33554432, 0, 0, ''),
(376300, 376300, 0, 33554432, 0, 0, ''),
(376301, 376301, 0, 33554432, 0, 0, ''),
(376304, 376304, 0, 33554432, 0, 0, '');
DELETE FROM `waypoint_data` WHERE `id` IN (376299, 376300, 376301, 376304);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(376299, 1, 936.30, 4882.30, 131.30, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376299, 2, 974.20, 4836.54, 138.19, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376299, 3, 1027.23, 4866.71, 144.55, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376299, 4, 1032.60, 4929.18, 133.87, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376299, 5, 1040.73, 5003.20, 128.04, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376299, 6, 954.17, 5016.22, 149.39, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376299, 7, 897.06, 5015.04, 149.75, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376299, 8, 874.00, 4948.92, 145.34, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376299, 9, 831.01, 4907.99, 129.15, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376300, 1, 1004.60, 5014.30, 131.00, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376300, 2, 932.07, 4945.33, 138.64, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376300, 3, 985.21, 4868.54, 148.17, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376300, 4, 1060.87, 4910.71, 147.72, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376300, 5, 1168.44, 4941.56, 131.55, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376300, 6, 1145.35, 5027.26, 140.89, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376300, 7, 1160.01, 5080.75, 132.83, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376300, 8, 1055.00, 5123.65, 139.01, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376300, 9, 981.38, 5088.57, 124.89, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376301, 1, 970.30, 4989.10, 135.70, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376301, 2, 874.74, 4920.67, 131.95, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376301, 3, 976.05, 4890.93, 145.68, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376301, 4, 1033.63, 4880.77, 149.78, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376301, 5, 1125.27, 4951.81, 133.21, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376301, 6, 1072.88, 5044.26, 136.07, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376301, 7, 1003.45, 5050.11, 142.09, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376304, 1, 1094.60, 5064.20, 139.40, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376304, 2, 1146.06, 5022.23, 126.00, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376304, 3, 1222.57, 5004.23, 144.52, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376304, 4, 1202.67, 5099.58, 135.72, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376304, 5, 1167.97, 5219.59, 144.16, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376304, 6, 1081.56, 5192.59, 131.07, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376304, 7, 1016.04, 5148.88, 145.44, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376304, 8, 990.74, 5079.98, 134.50, 0, 0, 0, 1, 0, 0, 100, 0, 0);
-- Felsoul Hold: the wards' French name; the swarmers, doomlords and the bunnies carrying the wards' spinning visual stay in the air (with "can fly" alone they fell while standing still)
INSERT INTO `locales_gameobject` (`entry`, `name_loc2`) VALUES
(243428, 'Protection d’énergie gangrenée'),
(243472, 'Protection d’énergie gangrenée'),
(243473, 'Protection d’énergie gangrenée')
ON DUPLICATE KEY UPDATE `name_loc2` = VALUES(`name_loc2`);
UPDATE `creature_template_movement` SET `Flight` = 1 WHERE `CreatureId` IN (101763, 101764);
-- Felsoul Hold: the Nueux and the Dreadflame swarmers only fly (they fired at the bats too, too many shots), the doomlords near the camp stay in the air
UPDATE `creature_template_movement` SET `Flight` = 1 WHERE `CreatureId` IN (101770, 106647);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 101763 AND `source_type` = 0;
UPDATE `creature_template` SET `AIName` = '' WHERE `entry` = 101763;
-- the three Felsoul doomlords patrol wide loops across the flight and stop to fire at the bats that come near (npc_felsoul_doomlord_the_hunt)
DELETE FROM `smart_scripts` WHERE `entryorguid` = 101764 AND `source_type` = 0;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_felsoul_doomlord_the_hunt' WHERE `entry` = 101764;
UPDATE `creature` SET `MovementType` = 2, `spawndist` = 0 WHERE `guid` IN (376307, 376309, 376310);
DELETE FROM `creature_addon` WHERE `guid` IN (376307, 376309, 376310);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(376307, 376307, 0, 50331648, 0, 0, ''),
(376309, 376309, 0, 50331648, 0, 0, ''),
(376310, 376310, 0, 50331648, 0, 0, '');
DELETE FROM `waypoint_data` WHERE `id` IN (376307, 376309, 376310);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(376307, 1, 1099.80, 5009.20, 126.80, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376307, 2, 1082.67, 5091.08, 130.86, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376307, 3, 978.28, 5053.98, 140.41, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376307, 4, 924.01, 5013.85, 133.01, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376307, 5, 920.08, 4958.59, 138.07, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376307, 6, 1020.04, 4926.91, 142.68, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376307, 7, 1109.59, 4954.56, 146.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376309, 1, 1106.70, 5044.10, 135.40, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376309, 2, 1183.30, 5005.62, 130.59, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376309, 3, 1182.12, 5137.11, 137.17, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376309, 4, 1140.16, 5209.81, 129.42, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376309, 5, 1024.29, 5152.14, 139.99, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376309, 6, 1015.79, 5032.92, 130.65, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376310, 1, 1155.90, 5061.10, 143.40, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376310, 2, 1145.04, 4984.62, 138.68, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376310, 3, 1226.40, 4987.96, 140.62, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376310, 4, 1290.42, 5041.82, 145.69, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376310, 5, 1258.48, 5169.74, 146.48, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(376310, 6, 1123.31, 5107.68, 141.85, 0, 0, 0, 1, 0, 0, 100, 0, 0);
-- the ward bunnies are linked by their beam (189204): the middle one (ward 243428) from the west one and to the east one; these two spawns keep the bunny's own lines
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (-376290, -376297) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `target_type`, `target_param1`, `target_param2`, `comment`) VALUES
(-376290, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 41, 0, 0, 0, 1, 0, 0, 'Universal Bunny - On data 1 1 - Despawn'),
(-376297, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 41, 0, 0, 0, 1, 0, 0, 'Universal Bunny - On data 1 1 - Despawn'),
(-376290, 0, 1, 0, 60, 0, 100, 0, 10000, 10000, 10000, 10000, 11, 205726, 0, 0, 21, 60, 0, 'Universal Bunny - Every 10 s - Cast ward blast'),
(-376297, 0, 1, 0, 60, 0, 100, 0, 10000, 10000, 10000, 10000, 11, 205726, 0, 0, 21, 60, 0, 'Universal Bunny - Every 10 s - Cast ward blast'),
(-376290, 0, 2, 0, 60, 0, 100, 0, 2000, 2000, 15000, 15000, 11, 189204, 34, 0, 19, 600112, 300, 'Ward Bunny - Every 15 s - Beam to the next ward bunny, unless it has it'),
(-376297, 0, 2, 0, 60, 0, 100, 0, 2000, 2000, 15000, 15000, 11, 189204, 34, 0, 19, 600112, 300, 'Ward Bunny - Every 15 s - Beam to the next ward bunny, unless it has it');
-- the three ward bunnies get their own entry (a copy of Universal Bunny 101906, which map 1470 also uses): seen from 400 yards, so that both ends of a beam show, which keeps their grids loaded too
DELETE FROM `creature_template` WHERE `entry` = 600112;
DELETE FROM `creature_template_wdb` WHERE `Entry` = 600112;
DELETE FROM `creature_template_addon` WHERE `entry` = 600112;
DELETE FROM `creature_template_movement` WHERE `CreatureId` = 600112;
CREATE TEMPORARY TABLE `tmp_ward` SELECT * FROM `creature_template` WHERE `entry` = 101906;
UPDATE `tmp_ward` SET `entry` = 600112, `minlevel` = 110, `maxlevel` = 110, `AIName` = 'SmartAI', `ScriptName` = '', `VisibilityDistance` = 1000;
INSERT INTO `creature_template` SELECT * FROM `tmp_ward`;
DROP TEMPORARY TABLE `tmp_ward`;
CREATE TEMPORARY TABLE `tmp_ward` SELECT * FROM `creature_template_wdb` WHERE `Entry` = 101906;
UPDATE `tmp_ward` SET `Entry` = 600112, `Name1` = 'Felsoul Ward Bunny';
INSERT INTO `creature_template_wdb` SELECT * FROM `tmp_ward`;
DROP TEMPORARY TABLE `tmp_ward`;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (600112, 0, 0, 0, 0, 0, '200975');
INSERT INTO `creature_template_movement` (`CreatureId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Random`) VALUES (600112, 1, 1, 1, 0, 0);
UPDATE `creature_template_movement` SET `Flight` = 2 WHERE `CreatureId` = 101906;
UPDATE `creature` SET `id` = 600112 WHERE `guid` IN (376290, 376294, 376297);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 600112 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `target_type`, `target_param1`, `target_param2`, `comment`) VALUES
(600112, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 41, 0, 0, 0, 1, 0, 0, 'Ward Bunny - On data 1 1 - Despawn'),
(600112, 0, 1, 0, 60, 0, 100, 0, 10000, 10000, 10000, 10000, 11, 205726, 0, 0, 21, 60, 0, 'Ward Bunny - Every 10 s - Cast ward blast');
UPDATE `smart_scripts` SET `target_param1` = 600112 WHERE `entryorguid` IN (243428, 243472, 243473) AND `source_type` = 1 AND `id` = 2 AND `target_type` = 19;
UPDATE `conditions` SET `ConditionValue2` = 600112, `Comment` = 'Ward Beam - Only the Felsoul ward bunnies' WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 189204 AND `ConditionTypeOrReference` = 31;
-- the Felsoul doomlords hover with the flying animation tier (3); with the hover tier (2) they stood as on the ground when they stopped
UPDATE `creature_template_addon` SET `bytes1` = 50331648 WHERE `entry` IN (101764, 101770);
-- Felsoul Hold, step 3: the ward bunnies run on a script (npc_felsoul_ward_bunny: beam to the next ward, blast, and a destroyed ward is removed for good)
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_felsoul_ward_bunny' WHERE `entry` = 600112;
DELETE FROM `smart_scripts` WHERE (`entryorguid` = 600112 OR `entryorguid` IN (-376290, -376297)) AND `source_type` = 0;
-- the Fist of the Deceiver gets its Fel Crash and three times its health (npc_fist_of_the_deceiver)
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_fist_of_the_deceiver' WHERE `entry` = 95429;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 95429 AND `source_type` = 0;
UPDATE `creature_template_wdb` SET `HpMulti` = 4.5 WHERE `Entry` = 95429;
-- the ground triggers of the berserkers and the crusher (souls the step needs) reach 30 yards, they were easy to walk past
UPDATE `eventobject_template` SET `radius` = 30 WHERE `entry` IN (113, 114);
-- everything in Felsoul Hold is seen from as far as the core allows (533 yards, the default range of instances is much shorter)
DELETE FROM `world_visible_distance` WHERE `type` = 0 AND `id` = 1498;
INSERT INTO `world_visible_distance` (`type`, `id`, `distance`, `comment`) VALUES (0, 1498, 533, 'Felsoul Hold, The Hunt scenario');
-- the Obliterators no longer fire on their own at the nearest bat, which could be the hunter's: the bat's script makes them shoot down the escorts and miss the hunter
DELETE FROM `smart_scripts` WHERE `entryorguid` = 101695 AND `source_type` = 0 AND `id` = 0;
-- Felsoul Hold, step 3: fifteen more demons on the ground between the wards, so that their souls fill the step without relying on the elites
DELETE FROM `creature` WHERE `guid` BETWEEN 9101010 AND 9101024;
CREATE TEMPORARY TABLE `tmp_souls` SELECT * FROM `creature` WHERE `guid` = 376385;
UPDATE `tmp_souls` SET `guid` = 9101010, `id` = 95313, `position_x` = 985.0, `position_y` = 4925.0, `position_z` = 36.8, `orientation` = 4.04;
INSERT INTO `creature` SELECT * FROM `tmp_souls`;
UPDATE `tmp_souls` SET `guid` = 9101011, `id` = 95329, `position_x` = 1000.0, `position_y` = 4905.0, `position_z` = 36.4, `orientation` = 3.07;
INSERT INTO `creature` SELECT * FROM `tmp_souls`;
UPDATE `tmp_souls` SET `guid` = 9101012, `id` = 95314, `position_x` = 1012.0, `position_y` = 4940.0, `position_z` = 36.5, `orientation` = 3.82;
INSERT INTO `creature` SELECT * FROM `tmp_souls`;
UPDATE `tmp_souls` SET `guid` = 9101013, `id` = 95313, `position_x` = 945.0, `position_y` = 4900.0, `position_z` = 33.4, `orientation` = 0.27;
INSERT INTO `creature` SELECT * FROM `tmp_souls`;
UPDATE `tmp_souls` SET `guid` = 9101014, `id` = 95329, `position_x` = 992.0, `position_y` = 4882.0, `position_z` = 33.4, `orientation` = 2.28;
INSERT INTO `creature` SELECT * FROM `tmp_souls`;
UPDATE `tmp_souls` SET `guid` = 9101015, `id` = 95314, `position_x` = 958.0, `position_y` = 4936.0, `position_z` = 34.4, `orientation` = 5.13;
INSERT INTO `creature` SELECT * FROM `tmp_souls`;
UPDATE `tmp_souls` SET `guid` = 9101016, `id` = 95329, `position_x` = 1000.0, `position_y` = 4965.0, `position_z` = 37.2, `orientation` = 4.24;
INSERT INTO `creature` SELECT * FROM `tmp_souls`;
UPDATE `tmp_souls` SET `guid` = 9101017, `id` = 95314, `position_x` = 1015.0, `position_y` = 4990.0, `position_z` = 35.6, `orientation` = 1.19;
INSERT INTO `creature` SELECT * FROM `tmp_souls`;
UPDATE `tmp_souls` SET `guid` = 9101018, `id` = 95313, `position_x` = 1030.0, `position_y` = 5030.0, `position_z` = 49.2, `orientation` = 1.18;
INSERT INTO `creature` SELECT * FROM `tmp_souls`;
UPDATE `tmp_souls` SET `guid` = 9101019, `id` = 95329, `position_x` = 1040.0, `position_y` = 5060.0, `position_z` = 59.1, `orientation` = 1.12;
INSERT INTO `creature` SELECT * FROM `tmp_souls`;
UPDATE `tmp_souls` SET `guid` = 9101020, `id` = 95313, `position_x` = 1190.0, `position_y` = 5025.0, `position_z` = 57.3, `orientation` = 5.62;
INSERT INTO `creature` SELECT * FROM `tmp_souls`;
UPDATE `tmp_souls` SET `guid` = 9101021, `id` = 95329, `position_x` = 1232.0, `position_y` = 5030.0, `position_z` = 59.1, `orientation` = 4.19;
INSERT INTO `creature` SELECT * FROM `tmp_souls`;
UPDATE `tmp_souls` SET `guid` = 9101022, `id` = 95314, `position_x` = 1175.0, `position_y` = 5050.0, `position_z` = 56.8, `orientation` = 5.45;
INSERT INTO `creature` SELECT * FROM `tmp_souls`;
UPDATE `tmp_souls` SET `guid` = 9101023, `id` = 95313, `position_x` = 1160.0, `position_y` = 5085.0, `position_z` = 51.8, `orientation` = 5.32;
INSERT INTO `creature` SELECT * FROM `tmp_souls`;
UPDATE `tmp_souls` SET `guid` = 9101024, `id` = 95329, `position_x` = 1205.0, `position_y` = 4970.0, `position_z` = 59.3, `orientation` = 1.24;
INSERT INTO `creature` SELECT * FROM `tmp_souls`;
DROP TEMPORARY TABLE `tmp_souls`;
-- the Felsoul Crusher runs on a script (npc_felsoul_crusher): its arrival hides it from the start, and its souls are counted
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_felsoul_crusher' WHERE `entry` = 95452;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 95452 AND `source_type` = 0;
-- Felsoul Hold's second scenario is the demonology warlock's Dark Whispers (Thal'kiel, the Skull of the Man'ari), not the affliction one
UPDATE `phase_definitions` SET `comment` = 'Demonology Warlock Scenario (Dark Whispers)' WHERE `zoneId` = 7637 AND `entry` = 3;
-- Varedis Felsoul runs on a script (npc_varedis_felsoul_the_hunt): his intro said its first talking head twice and played its scene on him, his health lines repeated
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_varedis_felsoul_the_hunt' WHERE `entry` = 94836;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 94836 AND `source_type` IN (0, 9);
-- Varedis has twice his health
UPDATE `creature_template_wdb` SET `HpMulti` = 8 WHERE `Entry` = 94836;
-- and hits harder: 6 000 to 7 000 per blow, about 10 000 for his Demon's Bite
UPDATE `creature_template` SET `dmg_multiplier` = 2.9 WHERE `entry` = 94836;
-- the Invisible Stalker an infernal turns into while it falls (62142) keeps only its invisible stalker model, which carries the falling comet: the other one was a visible elemental
UPDATE `creature_template_wdb` SET `Displayid1` = 38615, `Displayid2` = 0 WHERE `Entry` = 62142;
-- the end of Varedis (npc_varedis_felsoul_the_hunt): his two lines as he drops the Twinblades, then Kil'jaeden calling him back to Argus; the actors' durations are 5 s short of each conversation, which the core adds
DELETE FROM `conversation_data` WHERE `entry` IN (600001, 600002, 600003);
INSERT INTO `conversation_data` (`entry`, `id`, `idx`, `textId`, `unk1`, `unk2`) VALUES
(600001, 6068, 0, 0, 617, 0),
(600001, 6065, 1, 4500, 617, 0),
(600002, 6063, 0, 0, 819, 0);
DELETE FROM `conversation_actor` WHERE `entry` IN (600001, 600002, 600003);
INSERT INTO `conversation_actor` (`entry`, `id`, `actorId`, `creatureId`, `displayId`, `unk1`, `unk2`, `unk3`, `duration`) VALUES
(600001, 0, 50997, 94836, 67368, 0, 1, 0, 4300),
(600002, 0, 53978, 109422, 71057, 0, 1, 0, 1900);
-- the Twinblades he drops are the object; the creature that lets them be taken keeps its invisible model, not the beholder
UPDATE `creature_template_wdb` SET `Displayid1` = 71275, `Displayid2` = 0 WHERE `Entry` = 109978;
UPDATE `creature_text` SET `Text` = 'Gladly, my master!', `comment` = 'Varedis Felsoul - leaving through the portal' WHERE `CreatureID` = 94836 AND `GroupID` = 8;
-- the way back (npc_illidari_fel_bat_hunt_return, npc_illidari_fel_bat_hunt_mount): Kayn's talking head (201034), the bat comes down by the bridge, flies off toward Dalaran and lands on Krasus' Landing
DELETE FROM `conversation_data` WHERE `entry` = 1089;
INSERT INTO `conversation_data` (`entry`, `id`, `idx`, `textId`, `unk1`, `unk2`) VALUES
(1089, 2678, 0, 0, 119, 0);
DELETE FROM `conversation_actor` WHERE `entry` = 1089;
INSERT INTO `conversation_actor` (`entry`, `id`, `actorId`, `creatureId`, `displayId`, `unk1`, `unk2`, `unk3`, `duration`) VALUES
(1089, 0, 50925, 96498, 61698, 0, 1, 0, 6000);
-- the Twinblades are taken at once, without a loot window (go_twinblades_of_the_deceiver_the_hunt)
DELETE FROM `smart_scripts` WHERE `entryorguid` = 251878 AND `source_type` = 1;
UPDATE `gameobject_template` SET `AIName` = '', `ScriptName` = 'go_twinblades_of_the_deceiver_the_hunt' WHERE `entry` = 251878;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 101902 AND `source_type` = 0;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_illidari_fel_bat_hunt_return' WHERE `entry` = 101902;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_illidari_fel_bat_hunt_mount' WHERE `entry` = 101904;
-- The Hunt: the Fel Winds of Fel Commander Igrius roam and throw back, they no longer chase and strike the hunter
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_fel_winds_igrius' WHERE `entry` = 98470;
-- Violet Hold entrance: Kayn leaves the scene as soon as The Hunt is taken (another Kayn sets off toward Krasus' Landing), the rest of it stays until the quest is turned in
DELETE FROM `phase_definitions` WHERE `zoneId` = 7502 AND `entry` = 1205;
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `PreloadMapID`, `VisibleMapID`, `UiWorldMapAreaID`, `flags`, `comment`) VALUES
(7502, 1205, 0, 6220, 0, 0, 0, 16, 'Violet Hold entrance: Kayn of the By Any Means scene');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 7502 AND `SourceEntry` = 1205;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,7502,1205,0,0,9,0,40819,0,0,0,0,'','Violet Hold Kayn: Making Arrangements 40819 in progress'),
(23,7502,1205,0,0,14,0,39247,0,0,0,0,'','Violet Hold Kayn: The Hunt not taken yet'),
(23,7502,1205,0,1,28,0,40819,0,0,0,0,'','Violet Hold Kayn: Making Arrangements 40819 complete'),
(23,7502,1205,0,1,14,0,39247,0,0,0,0,'','Violet Hold Kayn: The Hunt not taken yet'),
(23,7502,1205,0,2,8,0,40819,0,0,0,0,'','Violet Hold Kayn: Making Arrangements 40819 rewarded'),
(23,7502,1205,0,2,14,0,39247,0,0,0,0,'','Violet Hold Kayn: The Hunt not taken yet'),
(23,7502,1205,0,3,9,0,41120,0,0,0,0,'','Violet Hold Kayn: Making Arrangements 41120 in progress'),
(23,7502,1205,0,3,14,0,39247,0,0,0,0,'','Violet Hold Kayn: The Hunt not taken yet'),
(23,7502,1205,0,4,28,0,41120,0,0,0,0,'','Violet Hold Kayn: Making Arrangements 41120 complete'),
(23,7502,1205,0,4,14,0,39247,0,0,0,0,'','Violet Hold Kayn: The Hunt not taken yet'),
(23,7502,1205,0,5,8,0,41120,0,0,0,0,'','Violet Hold Kayn: Making Arrangements 41120 rewarded'),
(23,7502,1205,0,5,14,0,39247,0,0,0,0,'','Violet Hold Kayn: The Hunt not taken yet');
UPDATE `creature` SET `PhaseId` = '6220' WHERE `guid` = 366279;
-- Krasus' Landing: the fel bat to Felsoul Hold waits only while The Hunt is under way, not once it is done and the hunter is back
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 7502 AND `SourceEntry` IN (151, 152);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,7502,151,0,0,9,0,41863,0,0,0,0,'','Krasus'' Landing fel bat: The Hunt 41863 under way'),
(23,7502,152,0,0,9,0,39247,0,0,0,0,'','Krasus'' Landing fel bat: The Hunt 39247 under way');
