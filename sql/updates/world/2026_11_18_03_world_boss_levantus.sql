-- Levantus: her Massive Spout spins her in a random direction (spell_levantus_massive_spout)
DELETE FROM `spell_script_names` WHERE `spell_id` = 217249;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (217249, 'spell_levantus_massive_spout');
