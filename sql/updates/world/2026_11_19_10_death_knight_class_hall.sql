-- Death Knight: scenario entrances, artifact pickup loot and equip, speakers of the acquisition fights, Tier 21 bonuses; the artifact chests give their own loot.

-- Dungeon finder entrances of Rescue Koltira, The Fourth Horseman and The Fleshripper's Harvest.
DELETE FROM `lfg_entrances` WHERE `dungeonId` IN (1059, 1295, 1317);
INSERT INTO `lfg_entrances` (`dungeonId`, `name`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(1059, 'The Fleshripper''s Harvest', 240.9, 1953.44, -55.29, 0.384845),
(1295, 'Rescue Koltira', 1675.92, 727.864, 77.6129, 2.893),
(1317, 'The Fourth Horseman', 2407.95, -5160.34, 82.1714, 4.351);

-- The Maw of the Damned and Blades of the Fallen Prince chests give their artifact through the loot they already own.
UPDATE `gameobject_template` SET `Data1` = `entry` WHERE `entry` IN (247566, 247386) AND `Data1` = 0;

DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId` = 4 AND `SourceGroup` IN (247566, 247386))
   OR (`SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 1 AND `SourceEntry` IN (247566, 247386) AND `SourceId` = 1);

INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(4, 247566, 128402, 0, 0, 2, 0, 128402, 1, 1, 1, 0, '', 'Blood artifact chest loot'),
(4, 247566, 128402, 0, 0, 15, 0, 32, 0, 0, 0, 0, '', 'Blood artifact chest loot'),
(4, 247566, 128402, 0, 0, 9, 0, 40740, 0, 0, 0, 0, '', 'Blood artifact chest loot'),
(4, 247566, 128402, 0, 0, 22, 0, 1489, 0, 0, 0, 0, '', 'Blood artifact chest loot'),
(4, 247566, 128402, 0, 0, 44, 0, 940, 9, 0, 0, 0, '', 'Blood artifact chest loot'),
(4, 247566, 128402, 0, 0, 8, 0, 40740, 0, 0, 1, 0, '', 'Blood artifact chest loot'),
(4, 247566, 128402, 0, 0, 36, 0, 0, 0, 0, 0, 0, '', 'Blood artifact chest loot'),
(4, 247566, 128402, 0, 1, 2, 0, 128402, 1, 1, 1, 0, '', 'Blood artifact chest loot'),
(4, 247566, 128402, 0, 1, 15, 0, 32, 0, 0, 0, 0, '', 'Blood artifact chest loot'),
(4, 247566, 128402, 0, 1, 28, 0, 40740, 0, 0, 0, 0, '', 'Blood artifact chest loot'),
(4, 247566, 128402, 0, 1, 22, 0, 1489, 0, 0, 0, 0, '', 'Blood artifact chest loot'),
(4, 247566, 128402, 0, 1, 44, 0, 940, 9, 0, 0, 0, '', 'Blood artifact chest loot'),
(4, 247566, 128402, 0, 1, 8, 0, 40740, 0, 0, 1, 0, '', 'Blood artifact chest loot'),
(4, 247566, 128402, 0, 1, 36, 0, 0, 0, 0, 0, 0, '', 'Blood artifact chest loot'),
(4, 247386, 128292, 0, 0, 2, 0, 128292, 1, 1, 1, 0, '', 'Frost artifact chest loot'),
(4, 247386, 128292, 0, 0, 15, 0, 32, 0, 0, 0, 0, '', 'Frost artifact chest loot'),
(4, 247386, 128292, 0, 0, 9, 0, 38990, 0, 0, 0, 0, '', 'Frost artifact chest loot'),
(4, 247386, 128292, 0, 0, 22, 0, 1480, 0, 0, 0, 0, '', 'Frost artifact chest loot'),
(4, 247386, 128292, 0, 0, 44, 0, 901, 6, 0, 0, 0, '', 'Frost artifact chest loot'),
(4, 247386, 128292, 0, 0, 8, 0, 38990, 0, 0, 1, 0, '', 'Frost artifact chest loot'),
(4, 247386, 128292, 0, 0, 36, 0, 0, 0, 0, 0, 0, '', 'Frost artifact chest loot'),
(4, 247386, 128292, 0, 1, 2, 0, 128292, 1, 1, 1, 0, '', 'Frost artifact chest loot'),
(4, 247386, 128292, 0, 1, 15, 0, 32, 0, 0, 0, 0, '', 'Frost artifact chest loot'),
(4, 247386, 128292, 0, 1, 28, 0, 38990, 0, 0, 0, 0, '', 'Frost artifact chest loot'),
(4, 247386, 128292, 0, 1, 22, 0, 1480, 0, 0, 0, 0, '', 'Frost artifact chest loot'),
(4, 247386, 128292, 0, 1, 44, 0, 901, 6, 0, 0, 0, '', 'Frost artifact chest loot'),
(4, 247386, 128292, 0, 1, 8, 0, 38990, 0, 0, 1, 0, '', 'Frost artifact chest loot'),
(4, 247386, 128292, 0, 1, 36, 0, 0, 0, 0, 0, 0, '', 'Frost artifact chest loot'),
(22, 1, 247566, 1, 0, 2, 0, 128402, 1, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 0, 15, 0, 32, 0, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 0, 9, 0, 40740, 0, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 0, 22, 0, 1489, 0, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 0, 44, 0, 940, 9, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 0, 8, 0, 40740, 0, 0, 1, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 0, 36, 0, 0, 0, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 1, 2, 0, 128402, 1, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 1, 15, 0, 32, 0, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 1, 28, 0, 40740, 0, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 1, 22, 0, 1489, 0, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 1, 44, 0, 940, 9, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 1, 8, 0, 40740, 0, 0, 1, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 1, 36, 0, 0, 0, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 2, 2, 0, 128402, 1, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 2, 15, 0, 32, 0, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 2, 9, 0, 40740, 0, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 2, 22, 0, 1489, 0, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 2, 44, 0, 940, 10, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 2, 8, 0, 40740, 0, 0, 1, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 2, 36, 0, 0, 0, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 3, 2, 0, 128402, 1, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 3, 15, 0, 32, 0, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 3, 28, 0, 40740, 0, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 3, 22, 0, 1489, 0, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 3, 44, 0, 940, 10, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 3, 8, 0, 40740, 0, 0, 1, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247566, 1, 3, 36, 0, 0, 0, 0, 0, 0, '', 'Blood artifact pickup scene'),
(22, 1, 247386, 1, 0, 2, 0, 128292, 1, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 0, 15, 0, 32, 0, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 0, 9, 0, 38990, 0, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 0, 22, 0, 1480, 0, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 0, 44, 0, 901, 6, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 0, 8, 0, 38990, 0, 0, 1, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 0, 36, 0, 0, 0, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 1, 2, 0, 128292, 1, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 1, 15, 0, 32, 0, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 1, 28, 0, 38990, 0, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 1, 22, 0, 1480, 0, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 1, 44, 0, 901, 6, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 1, 8, 0, 38990, 0, 0, 1, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 1, 36, 0, 0, 0, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 2, 2, 0, 128292, 1, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 2, 15, 0, 32, 0, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 2, 9, 0, 38990, 0, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 2, 22, 0, 1480, 0, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 2, 44, 0, 901, 7, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 2, 8, 0, 38990, 0, 0, 1, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 2, 36, 0, 0, 0, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 3, 2, 0, 128292, 1, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 3, 15, 0, 32, 0, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 3, 28, 0, 38990, 0, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 3, 22, 0, 1480, 0, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 3, 44, 0, 901, 7, 0, 0, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 3, 8, 0, 38990, 0, 0, 1, 0, '', 'Frost artifact pickup scene'),
(22, 1, 247386, 1, 3, 36, 0, 0, 0, 0, 0, 0, '', 'Frost artifact pickup scene');

-- The chest scene of the Maw of the Damned has no second smart action to link to.
UPDATE `smart_scripts` SET `link` = 0 WHERE `entryorguid` = 247566 AND `source_type` = 1 AND `id` = 0 AND `link` = 1;

-- The pickup scenes equip the looted artifact when they complete.
UPDATE `spell_scene` SET `ScriptName` = 'scene_dk_blood_equipment' WHERE `MiscValue` = 1168;
UPDATE `spell_scene` SET `ScriptName` = 'scene_dk_frost_equipment' WHERE `MiscValue` = 1071;
DELETE FROM `spell_scene_event` WHERE `MiscValue` IN (1168, 1071) AND `Event` = 'complete';
INSERT INTO `spell_scene_event` (`MiscValue`, `Event`, `trigerSpell`, `MonsterCredit`, `comment`) VALUES
(1168, 'complete', 200134, 0, 'Blood scene completion equips owned artifact'),
(1071, 'complete', 197092, 0, 'Frost scene completion equips owned artifact');

-- The guard-clear check measures around the dying sentry, not around its killer.
UPDATE `conditions` SET `ConditionTarget` = 1 WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 4 AND `SourceEntry` = 102244 AND `SourceId` = 0
  AND `ElseGroup` = 0 AND `ConditionTypeOrReference` = 29 AND `ConditionTarget` = 0;

-- Fleshripper and the three Frost souls speak themselves when a pet or guardian is their victim.
UPDATE `smart_scripts` SET `target_type` = 1 WHERE `source_type` = 0 AND `entryorguid` = 101778 AND `id` IN (0, 1) AND `action_type` = 1 AND `target_type` IN (2, 7);
UPDATE `smart_scripts` SET `target_type` = 1 WHERE `source_type` = 0 AND `action_type` = 1 AND `target_type` IN (2, 7)
  AND ((`entryorguid` = 93948 AND `id` IN (2, 3, 4)) OR (`entryorguid` IN (93950, 93951) AND `id` IN (1, 2)));

-- Restore the scripted effects of the Death Knight Tier 21 bonuses.
DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_dk_t21_unholy_2p', 'spell_dk_t21_unholy_4p', 'spell_dk_t21_blood_4p');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(251871, 'spell_dk_t21_unholy_2p'),
(251872, 'spell_dk_t21_unholy_4p'),
(49028, 'spell_dk_t21_blood_4p');

UPDATE `gameobject_template` SET `Data1` = `entry` WHERE `entry` IN (246699) AND `Data1` = 0;
