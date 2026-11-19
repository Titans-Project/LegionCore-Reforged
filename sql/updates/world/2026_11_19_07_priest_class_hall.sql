-- Prêtre : butin des coffres de Xal'atath, T'uure et Lumière vengeresse, équipement en fin de scène, entrée de T'uure, messager de Dalaran, introduction de Marteau-Fracasse.
UPDATE `smart_scripts` SET `event_type` = 61, `event_param1` = 0 WHERE `entryorguid` = 247690 AND `source_type` = 1 AND `id` = 1 AND `event_type` = 70;
UPDATE `gameobject_template` SET `Data1` = 247690 WHERE `entry` = 247690 AND `Data1` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 4 AND `SourceGroup` = 247690 AND `SourceEntry` = 128827;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(4, 247690, 128827, 0, 0, 15, 0, 16, 0, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 0, 22, 0, 1539, 0, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 0, 44, 0, 991, 8, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 0, 9, 0, 40710, 0, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 0, 8, 0, 40710, 0, 0, 1, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 0, 36, 0, 0, 0, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 1, 15, 0, 16, 0, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 1, 22, 0, 1539, 0, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 1, 44, 0, 991, 8, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 1, 28, 0, 40710, 0, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 1, 8, 0, 40710, 0, 0, 1, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 1, 36, 0, 0, 0, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 2, 15, 0, 16, 0, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 2, 22, 0, 1539, 0, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 2, 44, 0, 991, 9, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 2, 9, 0, 40710, 0, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 2, 8, 0, 40710, 0, 0, 1, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 2, 36, 0, 0, 0, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 3, 15, 0, 16, 0, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 3, 22, 0, 1539, 0, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 3, 44, 0, 991, 9, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 3, 28, 0, 40710, 0, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 3, 8, 0, 40710, 0, 0, 1, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages'),
(4, 247690, 128827, 0, 3, 36, 0, 0, 0, 0, 0, 0, '', 'Xalatath: Priest on Blade in Twilight, final stages');
UPDATE `spell_scene` SET `ScriptName` = 'scene_xalatath_pickup' WHERE `MiscValue` = 1187 AND `SceneScriptPackageID` = 1553 AND (`ScriptName` IS NULL OR `ScriptName` = '');
UPDATE `gameobject_template` SET `Data1` = 249705 WHERE `entry` = 249705 AND `Data1` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 4 AND `SourceGroup` = 249705 AND `SourceEntry` = 128825;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(4, 249705, 128825, 0, 0, 15, 0, 16, 0, 0, 0, 0, '', 'Tuure: Priest on the final step without the staff'),
(4, 249705, 128825, 0, 0, 22, 0, 1489, 0, 0, 0, 0, '', 'Tuure: Priest on the final step without the staff'),
(4, 249705, 128825, 0, 0, 44, 0, 1085, 6, 0, 0, 0, '', 'Tuure: Priest on the final step without the staff'),
(4, 249705, 128825, 0, 0, 9, 0, 41993, 0, 0, 0, 0, '', 'Tuure: Priest on the final step without the staff'),
(4, 249705, 128825, 0, 0, 8, 0, 41993, 0, 0, 1, 0, '', 'Tuure: Priest on the final step without the staff'),
(4, 249705, 128825, 0, 0, 36, 0, 0, 0, 0, 0, 0, '', 'Tuure: Priest on the final step without the staff'),
(4, 249705, 128825, 0, 0, 2, 0, 128825, 1, 1, 1, 0, '', 'Tuure: Priest on the final step without the staff'),
(4, 249705, 128825, 0, 1, 15, 0, 16, 0, 0, 0, 0, '', 'Tuure: Priest on the final step without the staff'),
(4, 249705, 128825, 0, 1, 22, 0, 1489, 0, 0, 0, 0, '', 'Tuure: Priest on the final step without the staff'),
(4, 249705, 128825, 0, 1, 44, 0, 1085, 6, 0, 0, 0, '', 'Tuure: Priest on the final step without the staff'),
(4, 249705, 128825, 0, 1, 28, 0, 41993, 0, 0, 0, 0, '', 'Tuure: Priest on the final step without the staff'),
(4, 249705, 128825, 0, 1, 8, 0, 41993, 0, 0, 1, 0, '', 'Tuure: Priest on the final step without the staff'),
(4, 249705, 128825, 0, 1, 36, 0, 0, 0, 0, 0, 0, '', 'Tuure: Priest on the final step without the staff'),
(4, 249705, 128825, 0, 1, 2, 0, 128825, 1, 1, 1, 0, '', 'Tuure: Priest on the final step without the staff');
UPDATE `spell_scene` SET `ScriptName` = 'scene_tuure_pickup' WHERE `MiscValue` = 1265 AND `SceneScriptPackageID` = 1565 AND (`ScriptName` IS NULL OR `ScriptName` = '');
INSERT IGNORE INTO `lfg_entrances` (`dungeonId`, `name`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES (1243, NULL, -43.1440964, 1160.33679, -74.353569, 0.483893174);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 105697 AND `source_type` = 0 AND `id` = 0 AND `action_type` = 33;
UPDATE `gameobject_template` SET `Data1` = 248572 WHERE `entry` = 248572 AND `Data1` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 4 AND `SourceGroup` = 248572 AND `SourceEntry` = 128868;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(4, 248572, 128868, 0, 0, 15, 0, 16, 0, 0, 0, 0, '', 'Lights Wrath: Priest on the final steps without the staff'),
(4, 248572, 128868, 0, 0, 22, 0, 1583, 0, 0, 0, 0, '', 'Lights Wrath: Priest on the final steps without the staff'),
(4, 248572, 128868, 0, 0, 44, 0, 1065, 6, 0, 0, 0, '', 'Lights Wrath: Priest on the final steps without the staff'),
(4, 248572, 128868, 0, 0, 9, 0, 41631, 0, 0, 0, 0, '', 'Lights Wrath: Priest on the final steps without the staff'),
(4, 248572, 128868, 0, 0, 8, 0, 41631, 0, 0, 1, 0, '', 'Lights Wrath: Priest on the final steps without the staff'),
(4, 248572, 128868, 0, 0, 36, 0, 0, 0, 0, 0, 0, '', 'Lights Wrath: Priest on the final steps without the staff'),
(4, 248572, 128868, 0, 0, 2, 0, 128868, 1, 1, 1, 0, '', 'Lights Wrath: Priest on the final steps without the staff'),
(4, 248572, 128868, 0, 1, 15, 0, 16, 0, 0, 0, 0, '', 'Lights Wrath: Priest on the final steps without the staff'),
(4, 248572, 128868, 0, 1, 22, 0, 1583, 0, 0, 0, 0, '', 'Lights Wrath: Priest on the final steps without the staff'),
(4, 248572, 128868, 0, 1, 44, 0, 1065, 6, 0, 0, 0, '', 'Lights Wrath: Priest on the final steps without the staff'),
(4, 248572, 128868, 0, 1, 28, 0, 41631, 0, 0, 0, 0, '', 'Lights Wrath: Priest on the final steps without the staff'),
(4, 248572, 128868, 0, 1, 8, 0, 41631, 0, 0, 1, 0, '', 'Lights Wrath: Priest on the final steps without the staff'),
(4, 248572, 128868, 0, 1, 36, 0, 0, 0, 0, 0, 0, '', 'Lights Wrath: Priest on the final steps without the staff'),
(4, 248572, 128868, 0, 1, 2, 0, 128868, 1, 1, 1, 0, '', 'Lights Wrath: Priest on the final steps without the staff');
INSERT IGNORE INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_chance`, `action_type`, `action_param1`, `target_type`, `comment`)
SELECT 101344, 0, 1, 0, 61, 100, 41, 5000, 1, 'Priestly Matters - acceptance link - despawn after dialogue' FROM DUAL
WHERE EXISTS (SELECT 1 FROM (SELECT * FROM `smart_scripts`) AS `s` WHERE `s`.`entryorguid` = 101344 AND `s`.`source_type` = 0 AND `s`.`id` = 0 AND `s`.`link` = 1);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` IN (1, 2) AND `SourceEntry` = 101430 AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 101430, 0, 0, 31, 0, 4, 0, 0, 0, 0, '', 'Slaghammer intro: player invoker'),
(22, 1, 101430, 0, 0, 9, 0, 40710, 0, 0, 0, 0, '', 'Slaghammer intro: active Blade in Twilight'),
(22, 1, 101430, 0, 0, 36, 0, 0, 0, 0, 0, 0, '', 'Slaghammer intro: live player'),
(22, 1, 101430, 0, 0, 44, 1, 991, 1, 0, 0, 0, '', 'Slaghammer intro: first area stage'),
(22, 2, 101430, 0, 0, 31, 0, 4, 0, 0, 0, 0, '', 'Slaghammer conversation: player invoker'),
(22, 2, 101430, 0, 0, 9, 0, 40710, 0, 0, 0, 0, '', 'Slaghammer conversation: active Blade in Twilight'),
(22, 2, 101430, 0, 0, 36, 0, 0, 0, 0, 0, 0, '', 'Slaghammer conversation: live player'),
(22, 2, 101430, 0, 0, 44, 1, 991, 1, 0, 0, 0, '', 'Slaghammer conversation: first area stage'),
(22, 2, 101430, 0, 1, 31, 0, 4, 0, 0, 0, 0, '', 'Slaghammer conversation: player invoker after credit'),
(22, 2, 101430, 0, 1, 9, 0, 40710, 0, 0, 0, 0, '', 'Slaghammer conversation: active quest after credit'),
(22, 2, 101430, 0, 1, 36, 0, 0, 0, 0, 0, 0, '', 'Slaghammer conversation: live player after credit'),
(22, 2, 101430, 0, 1, 44, 1, 991, 2, 0, 0, 0, '', 'Slaghammer conversation: stage after credit');
INSERT IGNORE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (234746, 'spell_pri_void_bolt_extension'), (162452, 'spell_pri_shadowy_insight');
UPDATE `creature_template` SET `ScriptName` = 'npc_pri_void_tendril' WHERE `entry` = 98167 AND `AIName` = '' AND `ScriptName` = '';
