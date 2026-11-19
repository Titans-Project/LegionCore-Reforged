-- Securing the Way: the gateway stays shut until the prisoners are executed, then the Soul Fonts rip it open
UPDATE `gameobject` SET `state` = 1 WHERE `guid` = 110183 AND `id` = 251579;
UPDATE `gameobject_template` SET `ScriptName` = 'go_illidari_gateway_securing_the_way' WHERE `entry` = 251579;
DELETE FROM `spell_script_names` WHERE `spell_id` IN (217377, 217723);
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES (217377,'spell_dh_sentence_to_death'),(217723,'spell_dh_gateway_ripped_open_knockback');
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 109273;
INSERT INTO `npc_spellclick_spells` (`npc_entry`,`spell_id`,`cast_flags`,`user_type`,`add_npc_flag`) VALUES (109273,217719,1,0,0);
DELETE FROM `creature_template_addon` WHERE `entry` = 109247;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (109247, 0, 0, 0, 1, 0, '191057');
