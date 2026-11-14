-- Data errors reported at worldserver startup

-- Fixes data rows reported as invalid by the world server at startup.

-- Movement overrides and SmartAI scripts of creature spawns that no longer exist
DELETE m FROM `creature_movement_override` m LEFT JOIN `creature` c ON c.`guid` = m.`SpawnId` WHERE c.`guid` IS NULL;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (-327636, -327635, -327468, -327457, -324195, -324189);

-- Orphan rows: text without creature, reward of an achievement absent from 7.3.5, graveyard of a missing zone
DELETE FROM `creature_text` WHERE `CreatureID` = 16777215;
DELETE FROM `achievement_reward` WHERE `entry` = 12461;
DELETE FROM `game_graveyard_zone` WHERE `ghost_zone` = 12003;

-- Gameobject spawns identical to another spawn
DELETE FROM `gameobject` WHERE `guid` IN (25810081, 25810097);

-- Loot id and reward spell that do not exist
UPDATE `creature_template` SET `lootid` = 0 WHERE `entry` = 36868 AND `lootid` = 36868;
UPDATE `quest_template` SET `RewardSpell` = 0 WHERE `ID` IN (36807, 36809) AND `RewardSpell` = 204071;

-- Daily tracking quests are repeatable, garrison invasion quest 50320 is completed by spell 262290
INSERT INTO `quest_template_addon` (`ID`, `SpecialFlags`) VALUES (50260, 1), (50261, 1), (50262, 1), (50263, 1), (50264, 1), (50320, 2)
    ON DUPLICATE KEY UPDATE `SpecialFlags` = `SpecialFlags` | VALUES(`SpecialFlags`);

-- "Only in 10 yards": caster within 10 yards (target 1, distance 10, lower or equal)
UPDATE `conditions` SET `ConditionValue1` = 1, `ConditionValue2` = 10, `ConditionValue3` = 4
    WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` IN (181555, 214176) AND `ConditionTypeOrReference` = 35 AND `ConditionValue1` = 10 AND `ConditionValue2` = 4;

-- Subzones checked as zones
UPDATE `conditions` SET `ConditionTypeOrReference` = 23
    WHERE `SourceTypeOrReferenceId` = 22 AND `SourceEntry` IN (123984, 124246, 123991) AND `ConditionTypeOrReference` = 4 AND `ConditionValue1` IN (6457, 5287);

-- Unused values on quest conditions
UPDATE `conditions` SET `ConditionValue2` = 0 WHERE `ConditionTypeOrReference` = 9 AND `ConditionValue1` = 42833 AND `ConditionValue2` = 109241;
UPDATE `conditions` SET `ConditionValue2` = 0, `ConditionValue3` = 0 WHERE `ConditionTypeOrReference` = 9 AND `ConditionValue1` = 33837 AND `ConditionValue2` = 82283;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 5287 AND `SourceEntry` = 1 AND `ConditionTypeOrReference` = 14 AND `ConditionValue1` = 40847 AND `ConditionValue3` = 1;

-- Deprecated difficulty flags on a timed action list
UPDATE `smart_scripts` SET `event_flags` = 0 WHERE `entryorguid` = 53693 AND `source_type` = 9 AND `id` = 0 AND `event_flags` = 30;

-- Service NPCs of the original Russian server (experience rate, arena, quest repair...), without name, model nor script
DELETE ca FROM `creature_addon` ca JOIN `creature` c ON c.`guid` = ca.`guid` WHERE c.`id` IN (220010, 220011, 230000, 230001, 230002, 230003, 230004, 230005, 230006, 230007);
DELETE FROM `creature` WHERE `id` IN (220010, 220011, 230000, 230001, 230002, 230003, 230004, 230005, 230006, 230007);
DELETE FROM `creature_template_addon` WHERE `entry` IN (220010, 220011, 230000, 230001, 230002, 230003, 230004, 230005, 230006, 230007);
DELETE FROM `creature_equip_template` WHERE `CreatureID` IN (220010, 220011, 230000, 230001, 230002, 230003, 230004, 230005, 230006, 230007);
DELETE FROM `npc_vendor` WHERE `entry` IN (220010, 220011, 230000, 230001, 230002, 230003, 230004, 230005, 230006, 230007);
DELETE FROM `creature_text` WHERE `CreatureID` IN (220010, 220011, 230000, 230001, 230002, 230003, 230004, 230005, 230006, 230007);
DELETE FROM `creature_text_locale` WHERE `CreatureID` IN (220010, 220011, 230000, 230001, 230002, 230003, 230004, 230005, 230006, 230007);
DELETE FROM `creature_template_wdb` WHERE `Entry` IN (220010, 220011, 230000, 230001, 230002, 230003, 230004, 230005, 230006, 230007);
DELETE FROM `creature_template` WHERE `entry` IN (220010, 220011, 230000, 230001, 230002, 230003, 230004, 230005, 230006, 230007);

-- Zone auras of the original server bound to quests and an event that no longer exist (ghost flight, custom sky)
DELETE FROM `spell_area` WHERE `spell` = 55164 AND `quest_start` IN (60023, 60024);
DELETE FROM `spell_area` WHERE `spell` = 305164 AND `quest_start` IN (60021, 60022);

-- Conditions on missing quests, wrong source groups or a source id their type does not allow (all already ignored)
DELETE FROM `conditions` WHERE `ConditionTypeOrReference` = 41 AND `ConditionValue1` IN (108260, 108261);
DELETE FROM `conditions` WHERE `ConditionValue1` >= 4294900000 AND `ConditionTypeOrReference` IN (8, 9, 14, 28, 41);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 29 AND `SourceGroup` = 0 AND `SourceEntry` = 1502;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 31 AND `SourceGroup` = 1 AND `SourceEntry` = 35277;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 7814 AND `SourceEntry` = 3 AND `SourceId` = 1;

-- Frostfire Ridge phase 3331: not "For the Horde!" rewarded, and scene 594 seen or "Of Wolves and Warriors" rewarded
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 6720 AND `SourceEntry` = 4;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,6720,4,0,0,8,0,34364,0,0,1,0,'','Draenor. FrostFireRidge. not rew 34364'),
(23,6720,4,0,0,40,0,594,0,0,0,0,'','Draenor. FrostFireRidge. scene comp. 594'),
(23,6720,4,0,1,8,0,34364,0,0,1,0,'','Draenor. FrostFireRidge. not rew 34364'),
(23,6720,4,0,1,8,0,34402,0,0,0,0,'','Draenor. FrostFireRidge. or rew 34402');

-- Crown Chemical Co. dailies have no giver in 7.3.5
DELETE FROM `pool_quest` WHERE `pool_entry` = 348;
DELETE FROM `pool_template` WHERE `entry` = 348;

-- Loot row pointing to a currency that does not exist
DELETE FROM `gameobject_loot_template` WHERE `Entry` = 40870 AND `Item` = 0 AND `Currency` = 138623;

-- Waypoint script linked to no waypoint
DELETE FROM `waypoint_scripts` WHERE `id` = 920104;

-- Master Mathias Shaw (Legion) display without model data
INSERT IGNORE INTO `creature_model_info` (`DisplayID`,`BoundingRadius`,`CombatReach`,`DisplayID_Other_Gender`,`hostileId`) VALUES (83274, 0.3672, 1.8, 0, 0);

-- Scripts written in the core but never bound
UPDATE `gameobject_template` SET `ScriptName` = 'go_wod_q34392_plunger' WHERE `entry` = 231985;
DELETE FROM `spell_script_names` WHERE `spell_id` = 170995 AND `ScriptName` = 'spell_warl_cripple';
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES (170995, 'spell_warl_cripple');

-- Pillar of Frost no longer grants an incapacitate immunity in 7.3.5
DELETE FROM `spell_script_names` WHERE `spell_id` = 51271 AND `ScriptName` = 'spell_dk_pillar_of_frost';

-- Garrison work orders spawned by hand: the buildings spawn their own
DELETE FROM `gameobject` WHERE `guid` IN (25329436, 105501, 105507, 105513, 105551, 106900, 130641);

-- Speed variants of these mounts no longer exist in 7.3.5: the mount spell itself scales with the terrain
DELETE FROM `spell_script_names` WHERE `spell_id` IN (47977, 48025, 52408, 71342, 72286, 75614, 75973) AND `ScriptName` IN ('spell_magic_broom', 'spell_headless_horseman_mount', 'spell_gen_seaforium_blast', 'spell_big_love_rocket', 'spell_invincible', 'spell_celestial_steed', 'spell_x53_touring_rocket');

-- Order Archives (artifact knowledge rework) is open to every race
UPDATE `quest_template` SET `AllowableRaces` = 18446744073709551615 WHERE `ID` = 316999 AND `AllowableRaces` = 0;

-- Removes the quest links left on the deleted Russian server NPCs (their quests keep their official givers)
DELETE FROM `creature_queststarter` WHERE `id` IN (220010, 220011, 230000, 230001, 230002, 230003, 230004, 230005, 230006, 230007);
DELETE FROM `creature_questender` WHERE `id` IN (220010, 220011, 230000, 230001, 230002, 230003, 230004, 230005, 230006, 230007);
