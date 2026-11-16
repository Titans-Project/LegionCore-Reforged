-- Mardum: the Legion ship over the battlefield gets its fel engine, carried by an invisible bunny at its centre.
DELETE FROM `creature_addon` WHERE `guid` = 600000;
DELETE FROM `creature_template` WHERE `entry` = 600104;
DELETE FROM `creature_template_wdb` WHERE `Entry` = 600104;
DELETE FROM `creature_template_addon` WHERE `entry` = 600104;
DELETE FROM `creature_template_movement` WHERE `CreatureId` = 600104;
CREATE TEMPORARY TABLE `tmp_engine` SELECT * FROM `creature_template` WHERE `entry` = 23837;
UPDATE `tmp_engine` SET `entry` = 600104, `scale` = 1.6667, `MaxVisible` = 1, `VisibilityDistance` = 533, `AIName` = '', `ScriptName` = '';
INSERT INTO `creature_template` SELECT * FROM `tmp_engine`;
DROP TEMPORARY TABLE `tmp_engine`;
CREATE TEMPORARY TABLE `tmp_engine` SELECT * FROM `creature_template_wdb` WHERE `Entry` = 23837;
UPDATE `tmp_engine` SET `Entry` = 600104, `Name1` = 'Legion Ship Fel Engine (Mardum)';
INSERT INTO `creature_template_wdb` SELECT * FROM `tmp_engine`;
DROP TEMPORARY TABLE `tmp_engine`;
CREATE TEMPORARY TABLE `tmp_engine` SELECT * FROM `creature_template_movement` WHERE `CreatureId` = 23837;
UPDATE `tmp_engine` SET `CreatureId` = 600104;
INSERT INTO `creature_template_movement` SELECT * FROM `tmp_engine`;
DROP TEMPORARY TABLE `tmp_engine`;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (600104, 0, 0, 0, 1, 0, '225713');
DELETE FROM `creature` WHERE `guid` = 600036;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`, `isActive`) VALUES
(600036, 600104, 1481, 7705, 7705, 1, 1, '5837 5324 5116 5115 5114', 11686, 1319.10, 3026.08, 253.58, 3.97, 120, 0, 0, 1);
DELETE FROM `creature_movement_override` WHERE `SpawnId` = 600036;
INSERT INTO `creature_movement_override` (`SpawnId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Random`) VALUES (600036, 0, 0, 1, 1, 0);
