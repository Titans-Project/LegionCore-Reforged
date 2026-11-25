-- Script bindings missing at startup: Seaforium Blast and the instance scripts of four Draenor scenarios
DELETE FROM `spell_script_names` WHERE `spell_id` = 52408 AND `ScriptName` = 'spell_gen_seaforium_blast';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(52408, 'spell_gen_seaforium_blast');

DELETE FROM `instance_template` WHERE `map` IN (1203, 1329, 1374, 1402);
INSERT INTO `instance_template` (`map`, `parent`, `script`, `allowMount`, `bonusChance`) VALUES
(1203, 0, 'instance_battle_of_thunder_pass', 1, 0),
(1329, 0, 'instance_grommashar', 1, 0),
(1374, 0, 'instance_trial_of_faith', 1, 0),
(1402, 0, 'instance_iron_approach', 1, 0);
