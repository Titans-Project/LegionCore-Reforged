-- Talador, Dying Wish (33973): the healed Ahm takes the place of the dying one instead of appearing beside him
DELETE FROM `spell_target_position` WHERE `id` = 171734;
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(171734, 1116, 3397.08, 2440.46, 113.453, 2.3872);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 77031;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 77031 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(77031, 0, 0, 0, 20, 0, 100, 0, 33973, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ahm - Dying Wish rewarded - Make way for the healed Ahm');
UPDATE `creature` SET `spawntimesecs` = 60 WHERE `guid` = 328466;
