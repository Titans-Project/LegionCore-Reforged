-- Ordos and his Ancient Flames: their SmartAI becomes the C++ script of world_boss_ordos.cpp
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'boss_ordos' WHERE `entry` = 72057;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_ordos_ancient_flame' WHERE `entry` = 72059;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (72057, 72059) AND `source_type` = 0;
