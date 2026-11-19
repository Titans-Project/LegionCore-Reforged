-- Druid artifact acquisitions: weapon chests give their weapon, Fangs of Ashamane scene end equips it, Ursoc's Lair wave and Malithar script links, Earthwarden proc, Tier 21 scripts; the artifact chests give their own loot.

-- Weapon chests of Fangs of Ashamane, Claws of Ursoc and G'Hanir: loot the weapon on the final scenario step, start the scene only for its owner
UPDATE `gameobject_template` SET `Data1` = `entry` WHERE `entry` IN (250386, 248853, 248499);

DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId` = 4 AND `SourceGroup` IN (250386, 248853, 248499))
   OR (`SourceTypeOrReferenceId` = 22 AND `SourceEntry` IN (250386, 248853, 248499) AND `SourceId` = 1);

INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`)
SELECT scope.`kind`, IF(scope.`kind` = 4, weapon.`go`, scope.`grp`), IF(scope.`kind` = 4, weapon.`item`, weapon.`go`), IF(scope.`kind` = 4, 0, 1), variant.`grp`,
    IF(rule.`kind` = 0, variant.`quest_state`, rule.`kind`), 0,
    CASE rule.`kind` WHEN 2 THEN weapon.`item` WHEN 15 THEN 1024 WHEN 22 THEN weapon.`map` WHEN 44 THEN weapon.`scenario`
        WHEN 0 THEN weapon.`quest` WHEN 8 THEN weapon.`quest` ELSE 0 END,
    IF(rule.`kind` = 44, weapon.`stage` + variant.`next_stage`, IF(rule.`kind` = 2, 1, 0)),
    IF(rule.`kind` = 2 AND scope.`kind` = 4, 1, 0),
    IF(rule.`kind` = 2 AND scope.`kind` = 4, 1, rule.`negative`), 0, '', CONCAT(weapon.`label`, ': weapon chest and its final scenario step')
FROM (SELECT 250386 AS `go`, 128860 AS `item`, 1612 AS `map`, 1108 AS `scenario`, 6 AS `stage`, 42430 AS `quest`, 'Fangs of Ashamane' AS `label`
      UNION ALL SELECT 248853, 128821, 1536, 990, 7, 40647, 'Claws of Ursoc'
      UNION ALL SELECT 248499, 128306, 1599, 1061, 5, 41689, 'G''Hanir') weapon
CROSS JOIN (SELECT 4 AS `kind`, 0 AS `grp` UNION ALL SELECT 22, 1 UNION ALL SELECT 22, 3) scope
CROSS JOIN (SELECT 0 AS `grp`, 0 AS `next_stage`, 9 AS `quest_state` UNION ALL SELECT 1, 0, 28
            UNION ALL SELECT 2, 1, 9 UNION ALL SELECT 3, 1, 28) variant
CROSS JOIN (SELECT 2 AS `kind`, 0 AS `negative` UNION ALL SELECT 15, 0 UNION ALL SELECT 22, 0 UNION ALL SELECT 44, 0
            UNION ALL SELECT 0, 0 UNION ALL SELECT 8, 1 UNION ALL SELECT 36, 0) rule
WHERE scope.`kind` = 22 OR variant.`next_stage` = 0;

-- Fangs of Ashamane: equip the weapon when its scene ends
DELETE FROM `spell_scene_event` WHERE `MiscValue` = 1282 AND `Event` = 'complete';
INSERT INTO `spell_scene_event` (`MiscValue`, `Event`, `trigerSpell`, `MonsterCredit`, `comment`) VALUES
(1282, 'complete', 214715, 0, 'Feral artifact scene end equips the Fangs');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 214715;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`)
SELECT 17, 0, 214715, 0, variant.`grp`, IF(rule.`kind` = 0, variant.`quest_state`, rule.`kind`), 0,
    CASE rule.`kind` WHEN 2 THEN 128860 WHEN 15 THEN 1024 WHEN 22 THEN 1612 WHEN 44 THEN 1108 ELSE IF(rule.`kind` IN (0, 8), 42430, 0) END,
    IF(rule.`kind` = 44, variant.`stage`, IF(rule.`kind` = 2, 1, 0)), 0, rule.`negative`, 0, '', 'Fangs of Ashamane: equip the owned weapon on the final scenario steps'
FROM (SELECT 1 AS `grp`, 6 AS `stage`, 9 AS `quest_state` UNION ALL SELECT 2, 6, 28 UNION ALL SELECT 3, 7, 9 UNION ALL SELECT 4, 7, 28) variant
CROSS JOIN (SELECT 2 AS `kind`, 0 AS `negative` UNION ALL SELECT 15, 0 UNION ALL SELECT 22, 0 UNION ALL SELECT 44, 0
            UNION ALL SELECT 0, 0 UNION ALL SELECT 8, 1 UNION ALL SELECT 36, 0) rule;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 214715, 0, 0, 22, 0, 1612, 0, 0, 1, 0, '', 'Fangs of Ashamane: the equip spell stays usable outside the scenario map');

-- Malithar: the claw visual cleanup follows his speech
UPDATE `smart_scripts` SET `event_type` = 61 WHERE `entryorguid` = 101390 AND `source_type` = 0 AND `id` = 10 AND `event_type` = 0;

-- Ursoc's Lair waves: independent death events, last-living checks anchored on the dying creature
UPDATE `smart_scripts` SET `link` = 0 WHERE `source_type` = 0
    AND ((`entryorguid` = 101388 AND `id` IN (6, 7, 8)) OR (`entryorguid` = 101389 AND `id` IN (0, 1)) OR (`entryorguid` = 101384 AND `id` = 0 AND `link` = 1));

UPDATE `conditions` SET `ConditionTarget` = 1
WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 0 AND `ElseGroup` = 0 AND `ConditionTypeOrReference` = 29 AND `ConditionTarget` = 0
  AND `ConditionValue2` = 100 AND `ConditionValue3` = 0 AND `NegativeCondition` = 1
  AND ((`SourceEntry` = 101384 AND `SourceGroup` IN (4, 5, 6)) OR (`SourceEntry` = 101388 AND `SourceGroup` IN (7, 8, 9, 10)) OR (`SourceEntry` = 101389 AND `SourceGroup` IN (2, 3)));

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 0 AND `ElseGroup` = 0 AND `ConditionTypeOrReference` IN (29, 32)
  AND ((`SourceEntry` = 101388 AND `SourceGroup` IN (7, 8, 9, 10) AND `ConditionValue1` IN (101388, 24))
    OR (`SourceEntry` = 101384 AND `SourceGroup` IN (4, 5, 6) AND `ConditionValue1` = 24)
    OR (`SourceEntry` = 101389 AND `SourceGroup` IN (2, 3) AND `ConditionValue1` = 24));
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`)
SELECT 22, wave.`grp`, wave.`entry`, 0, 0, 29, 1, 101388, 100, 0, 1, 0, '', 'Smart - If Near No Creature'
FROM (SELECT 7 AS `grp`, 101388 AS `entry` UNION ALL SELECT 8, 101388 UNION ALL SELECT 9, 101388 UNION ALL SELECT 10, 101388) wave
UNION ALL
SELECT 22, wave.`grp`, wave.`entry`, 0, 0, 32, 0, 24, 0, 0, 0, 0, '', 'Smart - Killer is a unit'
FROM (SELECT 4 AS `grp`, 101384 AS `entry` UNION ALL SELECT 5, 101384 UNION ALL SELECT 6, 101384
      UNION ALL SELECT 7, 101388 UNION ALL SELECT 8, 101388 UNION ALL SELECT 9, 101388 UNION ALL SELECT 10, 101388
      UNION ALL SELECT 2, 101389 UNION ALL SELECT 3, 101389) wave;

-- Earthwarden: only direct Guardian Thrash damage grants a charge
DELETE FROM `spell_proc_check` WHERE `entry` IN (203974, 203975) AND `checkspell` = 77758 AND `effectmask` = 1;
INSERT INTO `spell_proc_check` (`entry`, `checkspell`, `effectmask`, `comment`) VALUES
(203974, 77758, 1, 'Earthwarden: only direct Guardian Thrash damage grants a charge');

-- Druid Tier 21 bonus scripts
DELETE FROM `spell_script_names` WHERE (`spell_id` = 251789 AND `ScriptName` = 'spell_dru_t21_feral_2p')
   OR (`spell_id` = 33917 AND `ScriptName` = 'spell_dru_t21_guardian_2p')
   OR (`spell_id` = 22812 AND `ScriptName` = 'spell_dru_t21_guardian_4p');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(251789, 'spell_dru_t21_feral_2p'),
(33917, 'spell_dru_t21_guardian_2p'),
(22812, 'spell_dru_t21_guardian_4p');

UPDATE `gameobject_template` SET `Data1` = `entry` WHERE `entry` IN (246437) AND `Data1` = 0;
