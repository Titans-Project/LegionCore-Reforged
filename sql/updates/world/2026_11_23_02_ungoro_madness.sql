-- Un'Goro Madness: the micro-holiday of 17-19 March, three of the ten elites up at a time, with their summons and the Dino Mojo they drop
DELETE FROM `game_event` WHERE `eventEntry` = 308;
INSERT INTO `game_event` (`eventEntry`, `start_time`, `end_time`, `occurence`, `length`, `holiday`, `description`, `world_event`) VALUES
(308, '2026-03-17 00:00:00', '2035-07-05 19:00:00', 525600, 4320, 644, 'Un''Goro Madness', 0);

DELETE FROM `creature_template` WHERE `entry` IN (118267, 118268, 118269, 118270, 118271, 118272, 118273, 118274, 118277, 118279, 118810, 118812, 118929, 118956, 119041, 119042);
INSERT INTO `creature_template` (`entry`, `minlevel`, `maxlevel`, `HealthScalingExpansion`, `exp`, `faction`, `speed_walk`, `speed_run`, `scale`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `lootid`, `VehicleId`, `AIName`, `MovementType`, `RegenHealth`, `WorldEffects`, `PassiveSpells`, `ScriptName`) VALUES
(118267, 110, 110, 0, 6, 14, 1, 1.14286, 1, 5.5, 2000, 2000, 1, 0, 0, 0, 118267, 0, '', 1, 1, '', '', 'npc_tyrantus'),
(118268, 110, 110, 0, 6, 14, 1, 1.14286, 1, 5.5, 2000, 2000, 1, 0, 0, 0, 118268, 0, '', 1, 1, '', '', 'npc_giantfin'),
(118269, 110, 110, 0, 6, 14, 1, 1.14286, 1, 5.5, 2000, 2000, 1, 0, 0, 0, 118269, 0, '', 1, 1, '', '', 'npc_queen_zavra'),
(118270, 110, 110, 0, 6, 14, 1, 1.14286, 1, 5.5, 2000, 2000, 1, 0, 0, 0, 118270, 0, '', 0, 1, '', '', 'npc_sunkeeper_croesus'),
(118271, 110, 110, 0, 6, 14, 1, 1.14286, 1, 5.5, 2000, 2000, 1, 0, 0, 0, 118271, 0, '', 1, 1, '', '', 'npc_clutchmother_zavas'),
(118272, 110, 110, 0, 6, 14, 1, 1.14286, 1, 5.5, 2000, 2000, 1, 0, 0, 0, 118272, 0, '', 0, 1, '', '', 'npc_sherazin'),
(118273, 110, 110, 0, 6, 14, 1, 1.14286, 1, 5.5, 2000, 2000, 1, 0, 0, 0, 118273, 0, '', 1, 1, '', '', 'npc_akaridal'),
(118274, 110, 110, 0, 6, 14, 1, 1.14286, 1, 5.5, 2000, 2000, 1, 0, 0, 0, 118274, 0, '', 1, 1, '', '', 'npc_tar_tyrant'),
(118277, 110, 110, 0, 6, 14, 1, 1.14286, 1, 5.5, 2000, 2000, 1, 0, 0, 0, 118277, 0, '', 0, 1, '', '', 'npc_skrox'),
(118279, 110, 110, 0, 6, 14, 1, 1.14286, 1, 5.5, 2000, 2000, 1, 0, 0, 0, 118279, 0, '', 1, 1, '', '', 'npc_dadanga'),
(118810, 110, 110, 0, 6, 14, 1, 1, 1, 1, 2000, 2000, 1, 0, 67127296, 1, 0, 5221, '', 0, 1, '', '', 'npc_water_bubble'),
(118812, 110, 110, 0, 6, 14, 1, 1.14286, 1, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, '', 0, 1, '', '', 'npc_water_bubble_murloc'),
(118929, 110, 110, 0, 6, 14, 1, 1.14286, 1, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, '', 0, 1, '', '', 'npc_ravenous_larva'),
(118956, 110, 110, 0, 6, 14, 1, 1.14286, 1, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, '', 0, 1, '', '', 'npc_crystal_vine'),
(119041, 110, 110, 0, 6, 14, 1, 1.14286, 1, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, '', 0, 1, '', '', 'npc_reanimated_sludge'),
(119042, 110, 110, 0, 6, 14, 1, 1.14286, 1, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, '', 0, 1, '', '', 'npc_tar_pit');

DELETE FROM `creature_template_wdb` WHERE `Entry` IN (118267, 118268, 118269, 118270, 118271, 118272, 118273, 118274, 118277, 118279, 118810, 118812, 118929, 118956, 119041, 119042);
INSERT INTO `creature_template_wdb` (`Entry`, `Name1`, `Title`, `CursorName`, `TypeFlags`, `Type`, `Family`, `Classification`, `Displayid1`, `Displayid2`, `Displayid3`, `Displayid4`, `HpMulti`, `PowerMulti`, `MovementInfoID`, `RequiredExpansion`, `VerifiedBuild`) VALUES
(118267, 'Tyrantus', '', '', 2097224, 1, 0, 1, 47813, 0, 0, 0, 300, 1, 220, 0, 25549),
(118268, 'Giantfin', '', '', 2097224, 7, 0, 1, 74930, 0, 0, 0, 300, 1, 220, 0, 25549),
(118269, 'Queen Zavra', '', '', 2097224, 1, 0, 1, 74931, 0, 0, 0, 300, 1, 220, 0, 25549),
(118270, 'Sunkeeper Croesus', '', '', 270532680, 7, 0, 1, 75319, 0, 0, 0, 300, 1, 220, 0, 25549),
(118271, 'Clutchmother Zavas', '', '', 2097224, 1, 0, 1, 76906, 0, 0, 0, 300, 1, 220, 0, 25549),
(118272, 'Sherazin', 'Corpse Flower', '', 2097224, 4, 0, 1, 75388, 0, 0, 0, 300, 1, 220, 0, 25549),
(118273, 'Akaridal', 'Skumblade Dinomancer', '', 2097224, 7, 0, 1, 51230, 0, 0, 0, 300, 1, 220, 0, 25549),
(118274, 'Tar Tyrant', '', '', 2097224, 4, 0, 1, 74728, 0, 0, 0, 300, 1, 220, 0, 25549),
(118277, 'Skrox', '', '', 2097224, 1, 0, 1, 74724, 0, 0, 0, 300, 1, 220, 0, 25549),
(118279, 'Dadanga', '', '', 2097224, 6, 0, 1, 74725, 0, 0, 0, 300, 1, 220, 0, 25549),
(118810, 'Water Bubble', '', 'vehichlecursor', 2359368, 4, 0, 1, 74912, 0, 0, 0, 10, 1, 0, 0, 25549),
(118812, 'Murloc', '', '', 270532680, 7, 0, 1, 441, 346, 983, 1762, 10, 1, 0, 0, 25549),
(118929, 'Ravenous Larva', '', '', 2097224, 1, 0, 1, 7900, 0, 0, 0, 200, 1, 0, 0, 25549),
(118956, 'Crystal Vine', '', '', 2097224, 4, 0, 1, 74975, 0, 0, 0, 200, 1, 0, 0, 25549),
(119041, 'Reanimated Sludge', '', '', 2097224, 4, 0, 1, 57553, 0, 0, 0, 50, 1, 888, 0, 25549),
(119042, 'Tar Pit', '', '', 2097224, 4, 0, 1, 75033, 0, 0, 0, 50, 1, 220, 0, 25549);

DELETE FROM `creature_text` WHERE `CreatureID` IN (118269, 118272, 118274);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextID`, `MinTimer`, `MaxTimer`, `SpellID`, `comment`) VALUES
(118269, 0, 0, '|TInterface\\Icons\\ABILITY_HUNTER_PET_RAPTOR.BLP:20|t%s |cFFFF0000|Hspell:235886|h[calls]|h|r to her pack!', 41, 0, 100, 0, 0, 0, 127091, 0, 0, 0, 'Queen Zavra - Call Pack'),
(118272, 0, 0, '|TInterface\\Icons\\SPELL_SHADOW_SOULLEECH_1.BLP:20|t%s begins |cFFFF0000|Hspell:236133|h[Blooming]|h|r !', 41, 0, 100, 0, 0, 0, 127285, 0, 0, 0, 'Sherazin - Bloom'),
(118274, 0, 0, '|TInterface\\Icons\\INV_Misc_Food_Legion_gooOil_multi.blp:20|tThe tar pits start |cFFFF0000|Hspell:236333|h[Erupting]|h|r !', 41, 0, 100, 0, 0, 0, 127361, 0, 0, 0, 'Tar Tyrant - Eruption');

DELETE FROM `creature` WHERE `guid` BETWEEN 9200001 AND 9200010;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `AiID`, `MovementID`, `MeleeID`, `isActive`, `skipClone`, `personal_size`, `isTeemingSpawn`, `unit_flags3`) VALUES
(9200001, 118267, 1, 490, 490, 1, 1, '', 0, 0, -7884.62, -1085.72, -270.49, 0, 300, 20, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(9200002, 118268, 1, 490, 490, 1, 1, '', 0, 0, -7420.34, -846.93, -269.26, 0, 300, 15, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(9200003, 118269, 1, 490, 490, 1, 1, '', 0, 0, -7408.48, -1926.89, -270.17, 0, 300, 15, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(9200004, 118270, 1, 490, 490, 1, 1, '', 0, 0, -8183.54, -1110.12, -219.80, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(9200005, 118271, 1, 490, 490, 1, 1, '', 0, 0, -7845.33, -1330.45, -264.40, 0, 300, 25, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(9200006, 118272, 1, 490, 490, 1, 1, '', 0, 0, -6779.39, -1844.09, -272.06, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(9200007, 118273, 1, 490, 490, 1, 1, '', 0, 0, -7200.00, -380.00, -215.12, 0, 300, 20, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(9200008, 118274, 1, 490, 490, 1, 1, '', 0, 0, -6495.61, -1447.09, -271.09, 0, 300, 20, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(9200009, 118277, 1, 490, 490, 1, 1, '', 0, 0, -6917.46, -755.93, -272.07, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(9200010, 118279, 1, 490, 490, 1, 1, '', 0, 0, -6969.76, -1505.76, -272.87, 0, 300, 20, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `pool_template` WHERE `entry` = 700075;
INSERT INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES
(700075, 3, 'Un''Goro Madness - three elites at a time');
DELETE FROM `pool_creature` WHERE `guid` BETWEEN 9200001 AND 9200010;
INSERT INTO `pool_creature` (`guid`, `pool_entry`, `chance`, `description`) VALUES
(9200001, 700075, 0, 'Un''Goro Madness - Tyrantus'),
(9200002, 700075, 0, 'Un''Goro Madness - Giantfin'),
(9200003, 700075, 0, 'Un''Goro Madness - Queen Zavra'),
(9200004, 700075, 0, 'Un''Goro Madness - Sunkeeper Croesus'),
(9200005, 700075, 0, 'Un''Goro Madness - Clutchmother Zavas'),
(9200006, 700075, 0, 'Un''Goro Madness - Sherazin'),
(9200007, 700075, 0, 'Un''Goro Madness - Akaridal'),
(9200008, 700075, 0, 'Un''Goro Madness - Tar Tyrant'),
(9200009, 700075, 0, 'Un''Goro Madness - Skrox'),
(9200010, 700075, 0, 'Un''Goro Madness - Dadanga');
DELETE FROM `game_event_pool` WHERE `pool_entry` = 700075;
INSERT INTO `game_event_pool` (`eventEntry`, `pool_entry`) VALUES
(308, 700075);
DELETE FROM `spell_script_names` WHERE `spell_id` IN (234781, 235757, 236133, 234756, 235990, 235659, 236358, 235656);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(234781, 'spell_dino_mojo'),
(235757, 'spell_adaptation'),
(236133, 'spell_bloom'),
(234756, 'spell_dino_mojo_bonus'),
(235990, 'spell_fertile_toxin'),
(235659, 'spell_adaptation_caustic_blood'),
(236358, 'spell_tar_strike'),
(235656, 'spell_adaptation_heavy_footed');
