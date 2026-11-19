-- Rogue: the artifact chests of the three acquisition scenarios give their own loot; Restless Blades, True Bearing, Alacrity, Marked for Death, Surge of Toxins and Tier 21 Subtlety move to C++ scripts.
DELETE FROM `spell_trigger` WHERE `spell_id` IN (79096, 193359);

DELETE FROM `spell_script_names` WHERE (`spell_id`, `ScriptName`) IN (
(79096, 'spell_rog_restless_blades'),
(193359, 'spell_rog_restless_blades'),
(193539, 'spell_rog_alacrity'),
(137619, 'spell_rog_marked_for_death'),
(192424, 'spell_rog_surge_of_toxins'),
(251785, 'spell_rog_t21_subtlety_2p'),
(257945, 'spell_rog_t21_shadow_gestures'));

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(79096, 'spell_rog_restless_blades'),
(193359, 'spell_rog_restless_blades'),
(193539, 'spell_rog_alacrity'),
(137619, 'spell_rog_marked_for_death'),
(192424, 'spell_rog_surge_of_toxins'),
(251785, 'spell_rog_t21_subtlety_2p'),
(257945, 'spell_rog_t21_shadow_gestures');

UPDATE `gameobject_template` SET `Data1` = `entry` WHERE `entry` IN (249347, 251107, 254087) AND `Data1` = 0;
