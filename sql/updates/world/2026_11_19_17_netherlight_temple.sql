-- The Netherlight Temple (40938): Follow Velen to the portal counts, promotion and exit from the ceremony scene, spec ritual bars, ritual target, Life Grip
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 202457 AND `spell_effect` = 202945 AND `type` = 0;
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 202835 AND `spell_effect` IN (202749, 216416, 216419) AND `type` = 0;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `hastype`, `hastalent`, `comment`) VALUES
(202835, 216416, 0, 8, 256, 'Priest formation: Discipline ritual bar'),
(202835, 216419, 0, 8, 257, 'Priest formation: Holy ritual bar'),
(202835, 202749, 0, 8, 258, 'Priest formation: Shadow ritual bar');

INSERT INTO `spell_scene_event` (`MiscValue`, `Event`, `trigerSpell`, `MonsterCredit`, `comment`) VALUES
(1191, 'CeremonyComplete', 202945, 0, 'Priest formation: promotion credit'),
(1191, 'PortPlayer', 202491, 0, 'Priest formation: leave the ritual')
ON DUPLICATE KEY UPDATE `trigerSpell` = VALUES(`trigerSpell`), `MonsterCredit` = VALUES(`MonsterCredit`);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` IN (202747, 216409, 216418);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 202747, 0, 0, 31, 0, 3, 102528, 0, 0, 0, '', 'Priest formation: ritual target'),
(13, 1, 216409, 0, 0, 31, 0, 3, 102528, 0, 0, 0, '', 'Priest formation: ritual target'),
(13, 1, 216418, 0, 0, 31, 0, 3, 102528, 0, 0, 0, '', 'Priest formation: ritual target');

INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `MovementType`)
SELECT 102528, 1512, 7834, 7834, 1, 1, '', 1417.67712, 1343.95483, 184.22673, 3.13932, 300, 0
FROM DUAL WHERE NOT EXISTS (SELECT 1 FROM `creature` WHERE `id` = 102528 AND `map` = 1512);
UPDATE `quest_objectives` SET `Amount` = 1 WHERE `ID` = 281419 AND `QuestID` = 40938;
DELETE FROM `spell_script_names` WHERE `spell_id` = 202324 AND `ScriptName` = 'spell_pri_netherlight_portal_follow_velen';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (202324, 'spell_pri_netherlight_portal_follow_velen');
DELETE FROM `spell_target_position` WHERE `id` = 202982;
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(202982, 1512, 1384.08679, 1344.21350, 176.737564, 0.202962);
INSERT INTO `spell_scene_event` (`MiscValue`, `Event`, `trigerSpell`, `MonsterCredit`, `comment`) VALUES
(1191, 'LifeGrip', 202982, 0, 'Priest formation: Life Grip')
ON DUPLICATE KEY UPDATE `trigerSpell` = VALUES(`trigerSpell`), `MonsterCredit` = VALUES(`MonsterCredit`);
