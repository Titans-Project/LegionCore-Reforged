-- Hatching of the Hippogryphs: Lorthalium drops her Spectral Feather, which extends the hatchling to five days
UPDATE `creature_template` SET `lootid` = 115537 WHERE `entry` = 115537;
DELETE FROM `spell_script_names` WHERE `spell_id` = 229709;
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES (229709,'spell_extend_hatchling_duration');
