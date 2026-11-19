-- Shaman: Sharas'dal pickup scene equips the artifact, shared Cloudburst release, T21 bonuses; the Sharas'dal chest gives its own loot.
DELETE FROM `spell_script_names` WHERE (`spell_id` = 209684 AND `ScriptName` = 'aura_sharasdal_acquisition_scene')
    OR (`spell_id` = 157503 AND `ScriptName` = 'spell_sha_cloudburst_heal')
    OR (`spell_id` = 251758 AND `ScriptName` = 'spell_sha_t21_elemental_4p')
    OR (`spell_id` = 251765 AND `ScriptName` = 'spell_sha_t21_restoration_4p');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(209684, 'aura_sharasdal_acquisition_scene'),
(157503, 'spell_sha_cloudburst_heal'),
(251758, 'spell_sha_t21_elemental_4p'),
(251765, 'spell_sha_t21_restoration_4p');

UPDATE `gameobject_template` SET `Data1` = `entry` WHERE `entry` = 249342 AND `Data1` = 0;
