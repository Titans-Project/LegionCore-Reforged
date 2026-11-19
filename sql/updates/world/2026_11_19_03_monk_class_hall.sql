-- Monk: Elusive Brawler, Special Delivery, Tier 21 bonuses, Black Ox Statue, Niuzao, Fu Zan Yigthregor field, Li Li kite click conditions; the artifact chests give their own loot.
DELETE FROM `spell_script_names` WHERE (`spell_id` = 195630 AND `ScriptName` = 'spell_monk_elusive_brawler')
   OR (`spell_id` = 196734 AND `ScriptName` = 'spell_monk_special_delivery')
   OR (`spell_id` = 251830 AND `ScriptName` = 'spell_monk_t21_brewmaster_4p')
   OR (`spell_id` = 100784 AND `ScriptName` = 'spell_monk_t21_windwalker_4p');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(195630, 'spell_monk_elusive_brawler'),
(196734, 'spell_monk_special_delivery'),
(251830, 'spell_monk_t21_brewmaster_4p'),
(100784, 'spell_monk_t21_windwalker_4p');

DELETE FROM `spell_proc_event` WHERE `entry` = 251830;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`, `effectmask`) VALUES
(251830, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 7);

UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_monk_black_ox_statue' WHERE `entry` = 61146;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_monk_niuzao' WHERE `entry` = 73967;

-- Fu Zan: Yigthregor leaves no Felfire Bomb field behind
UPDATE `creature_template` SET `ScriptName` = 'npc_fuzan_yigthregor' WHERE `entry` = 109820;
DELETE FROM `spell_script_names` WHERE `spell_id` = 217787 AND `ScriptName` = 'spell_fuzan_felfire_bomb';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (217787, 'spell_fuzan_felfire_bomb');

-- Li Li's kite: only a monk on the class hall quest is carried off by a click
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 1 AND `SourceEntry` = 102531 AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 102531, 0, 0, 9, 0, 40633, 0, 0, 0, 0, '', 'Smart - Player Has Quest'),
(22, 1, 102531, 0, 0, 15, 0, 512, 0, 0, 0, 0, '', 'Smart - Player Is Monk');

UPDATE `gameobject_template` SET `Data1` = `entry` WHERE `entry` IN (248086, 251605) AND `Data1` = 0;
