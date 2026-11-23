-- Spell scripts bound to the wrong spell: Echo of Sylvanas' Seeping Shadows and Darnell's summon in Recruitment
DELETE FROM `spell_script_names` WHERE (`spell_id` = 101397 AND `ScriptName` = 'spell_echo_of_sylvanas_seeping_shadows') OR (`spell_id` = 103175 AND `ScriptName` = 'spell_echo_of_sylvanas_seeping_shadows');
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES (103175,'spell_echo_of_sylvanas_seeping_shadows');
DELETE FROM `spell_script_names` WHERE (`spell_id` = 49337 AND `ScriptName` = 'spell_summon_darnel_q26800') OR (`spell_id` = 91938 AND `ScriptName` = 'spell_summon_darnel_q26800');
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES (91938,'spell_summon_darnel_q26800');
