-- Night elf Touch of Elune picks its day or night stat aura
DELETE FROM `spell_script_names` WHERE `spell_id` = 154748 AND `ScriptName` = 'spell_gen_touch_of_elune';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (154748, 'spell_gen_touch_of_elune');
