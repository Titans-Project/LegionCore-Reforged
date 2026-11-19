-- Strom'kar scenario: quest objective slots, Execute destination, recoil script, Thoradin at the end of the fight and the way back to Skyhold

-- Two objectives of the Arms quest shared the storage slot 3
UPDATE `quest_objectives` SET `StorageIndex` = CASE `ID` WHEN 281863 THEN 1 WHEN 282354 THEN 2 WHEN 286344 THEN 4 ELSE `StorageIndex` END
WHERE `QuestID` = 41105 AND `ID` IN (281863, 282354, 286344);

-- Execute (206455) jumps to the nearest creature of its destination type: only Zakajz is valid
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 206455;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 206455, 0, 0, 31, 0, 3, 104276, 0, 0, 0, '', 'Execute - Only 104276');

-- Zakajz script (engages the players after the recoil, stays down at one health point) and the recoil of the first attempt to draw the sword
UPDATE `creature_template` SET `ScriptName` = 'npc_stromkar_zakajz' WHERE `entry` = 104276 AND `ScriptName` = '';
DELETE FROM `spell_script_names` WHERE `spell_id` = 206360 AND `ScriptName` = 'spell_stromkar_opening_recoil';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (206360, 'spell_stromkar_opening_recoil');

-- Thoradin was never placed in the scenario: he comes when Zakajz falls to one health point, where his waypoint leads, and only once
DELETE FROM `smart_scripts` WHERE `entryorguid` = 104276 AND `source_type` = 0 AND `id` = 9;
UPDATE `smart_scripts` SET `link` = 9 WHERE `entryorguid` = 104276 AND `source_type` = 0 AND `id` = 6 AND `link` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `Difficulties`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(104276, 0, 9, 0, '', 61, 0, 100, 1, 0, 0, 0, 0, 0, 12, 104307, 8, 0, 0, 0, 0, 8, 0, 0, 0, 0, 1886.91, 2194.33, 37.1166, 4.57, 'Link - Summon Thoradin');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 4 AND `SourceEntry` = 104276 AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 4, 104276, 0, 0, 29, 1, 104307, 100, 0, 1, 0, '', 'Zakajz at one health point - only without Thoradin already there');

-- After his farewell Thoradin gives the way back to Skyhold to the players around him (Odin's Gaze, whose button is 192085), then leaves
DELETE FROM `smart_scripts` WHERE `entryorguid` = 104307 AND `source_type` = 0 AND `id` IN (5, 6);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `Difficulties`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(104307, 0, 5, 6, '', 52, 0, 100, 1, 3, 104307, 0, 0, 0, 75, 215997, 0, 0, 0, 0, 0, 18, 100, 0, 0, 0, 0, 0, 0, 0, 'Text Over - Add Odin''s Gaze'),
(104307, 0, 6, 0, '', 61, 0, 100, 1, 0, 0, 0, 0, 0, 41, 30000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Link - Despawn');

-- Using the button removes the aura, it would stay on the action bar in Skyhold otherwise
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 192085 AND `spell_effect` = -215997;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(192085, -215997, 0, 'Jump to Skyhold - remove Odin''s Gaze');
