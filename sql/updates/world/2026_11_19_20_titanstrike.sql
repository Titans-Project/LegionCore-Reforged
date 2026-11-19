-- Titanstrike scenario: wave and workshop trackers, spirit winds floor, rune anchors, Mimiron lines and fusion hand-off.
UPDATE `creature_template` SET `ScriptName` = 'npc_titanstrike_prustaga' WHERE `entry` = 104949 AND `ScriptName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_titanstrike_xr947' WHERE `entry` = 105016 AND `ScriptName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_titanstrike_foe' WHERE `entry` IN (106335, 106341, 106344, 106352, 106354) AND `ScriptName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_titanstrike_fusion_mimiron' WHERE `entry` = 106558 AND `ScriptName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_titanstrike_fusion_core' WHERE `entry` IN (106613, 106925) AND `ScriptName` = '';

-- The ritual lightning checks Prustaga within 20 yards, but the highest anchors hang 22 yards above her
UPDATE `conditions` SET `ConditionValue2` = 30
WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 1 AND `SourceEntry` = 64367 AND `SourceId` = 0 AND `ElseGroup` = 0
  AND `ConditionTypeOrReference` = 29 AND `ConditionValue1` = 104949 AND `ConditionValue2` = 20;

-- Rune room: the three runes would pick the invisible alternative display half of the time, and their anchors fall to the floor
UPDATE `creature` SET `modelid` = CASE `id` WHEN 106322 THEN 69560 WHEN 106323 THEN 69561 ELSE 69562 END
WHERE `map` = 1495 AND `PhaseId` = '6985' AND `id` IN (106322, 106323, 106324) AND `modelid` IN (0, 11686);

INSERT INTO `creature_movement_override` (`SpawnId`, `Ground`, `Flight`)
SELECT `guid`, 0, 1 FROM `creature`
WHERE `map` = 1495 AND `PhaseId` = '6985'
  AND (`id` IN (106322, 106323, 106324)
    OR (`id` = 64367 AND `position_x` BETWEEN 4870 AND 4895 AND `position_y` BETWEEN 300 AND 330))
ON DUPLICATE KEY UPDATE `Ground` = VALUES(`Ground`), `Flight` = VALUES(`Flight`);

-- XR9-47 becomes functional when its tracker reports the last construct dead (the native timer looked 30 yards around the relay)
UPDATE `smart_scripts` SET `event_type` = 38, `event_param1` = 2, `event_param2` = 2, `event_param3` = 0, `event_param4` = 0,
    `comment` = 'XR9-47 - all constructs defeated - walk to the console'
WHERE `entryorguid` = 105016 AND `source_type` = 0 AND `id` = 2 AND `event_type` = 60 AND `action_type` = 53;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 3 AND `SourceEntry` = 105016
  AND `ConditionTypeOrReference` = 29 AND `ConditionValue1` IN (106352, 106354);

-- Mimiron's lines get a conversation of their own, started once his hologram exists; the first one keeps only XR9-47 and Grif
INSERT IGNORE INTO `conversation_data` (`entry`, `id`, `idx`, `textId`, `unk1`, `unk2`)
SELECT 9002055, `id`, `idx` - 2, `textId` - 14150, `unk1`, `unk2` FROM `conversation_data` WHERE `entry` = 2055 AND `idx` >= 2 AND `textId` >= 14150;
INSERT IGNORE INTO `conversation_creature` (`entry`, `id`, `creatureId`, `creatureGuid`, `unk1`, `unk2`, `duration`)
SELECT 9002055, `id`, IF(`id` = 2, 106376, `creatureId`), `creatureGuid`, `unk1`, `unk2`, IF(`id` = 2, 50000, 0)
FROM `conversation_creature` WHERE `entry` = 2055;
DELETE `src` FROM `conversation_data` `src`
JOIN `conversation_data` `moved` ON `moved`.`entry` = 9002055 AND `moved`.`id` = `src`.`id`
WHERE `src`.`entry` = 2055 AND `src`.`idx` >= 2;
UPDATE `conversation_creature` SET `duration` = IF(`id` = 0, 14150, 0) WHERE `entry` = 2055;

-- Spirit winds: the two invisible emitters of the tomb corridor keep the floor aura; the area trigger script applies the push in the Tomb Raider step
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`)
SELECT `guid`, 0, 0, 0, 0, 0, '210880' FROM `creature`
WHERE `id` = 64367 AND `map` = 1495 AND `PhaseId` = '6985' AND `position_x` BETWEEN 4825 AND 4850 AND `position_y` BETWEEN 285 AND 335
ON DUPLICATE KEY UPDATE `auras` = VALUES(`auras`);

DELETE FROM `areatrigger_scripts` WHERE `entry` = 11360;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES (11360, 'at_titanstrike_spirit_winds');
DELETE FROM `areatrigger_actions` WHERE `entry` = 6837 AND `customEntry` = 11360;
INSERT INTO `areatrigger_actions` (`entry`, `customEntry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `comment`) VALUES
(6837, 11360, 0, 1, 15, 8, 210880, 'Spirit Winds - track players for the script');

-- Mimiron's fusion: the panels hand off to him only when both cores took their charge
UPDATE `conditions` SET `ConditionTypeOrReference` = 32, `ConditionTarget` = 1, `ConditionValue1` = 32, `ConditionValue2` = 0,
    `NegativeCondition` = 0, `ScriptName` = 'condition_titanstrike_fusion_ready'
WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 2 AND `SourceEntry` IN (249765, 249773) AND `SourceId` = 1 AND `ElseGroup` = 0
  AND `ConditionTypeOrReference` = 29 AND `ConditionValue1` IN (106613, 106925) AND `NegativeCondition` = 1;
-- The core's despawn event links to an event that does not exist
UPDATE `smart_scripts` SET `link` = 0 WHERE `entryorguid` IN (106613, 106925) AND `source_type` = 0 AND `id` = 1 AND `link` = 2;
