-- Illidari Redoubt: leaders gone and prisoners executed after Securing the Way and the gateway frame not clickable
UPDATE `creature_template` SET `ScriptName` = 'npc_illidari_redoubt_securing_the_way' WHERE `entry` IN (109247, 109252, 109295, 109308, 109309, 109314, 109340, 109344);
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_jace_darkweaver_securing_the_way' WHERE `entry` = 109196;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 109196 AND `source_type` = 0;
UPDATE `gameobject_template` SET `flags` = `flags` | 0x10 WHERE `entry` = 251579;
