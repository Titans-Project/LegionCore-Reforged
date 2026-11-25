-- Darkmoon Faire Concert: Power Chords and Wailing Walls, the Death Metal Knight loot once per Faire and the plate cage helm, unclickable graves, two fans sat on their stools
DELETE FROM `creature_loot_template` WHERE `Entry` = 122899 AND `Item` = 151251;
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Currency`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Shared`, `Comment`) VALUES
(122899, 151251, 0, 0, 5, 0, 0, 0, 1, 1, 0, NULL);

UPDATE `quest_template` SET `QuestSortID` = -22 WHERE `ID` = 47767;
UPDATE `quest_template_addon` SET `SpecialFlags` = 0 WHERE `ID` = 47767;
DELETE FROM `game_event_seasonal_questrelation` WHERE `questId` = 47767;
INSERT INTO `game_event_seasonal_questrelation` (`questId`, `eventEntry`) VALUES
(47767, 75);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 1 AND `SourceGroup` = 122899 AND `SourceEntry` IN (151251, 151252, 151253, 151254, 151255, 151265);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(1, 122899, 151251, 0, 0, 8, 0, 47767, 0, 0, 1, 0, '', 'Death Metal Knight - rare loot once per Darkmoon Faire'),
(1, 122899, 151252, 0, 0, 8, 0, 47767, 0, 0, 1, 0, '', 'Death Metal Knight - rare loot once per Darkmoon Faire'),
(1, 122899, 151253, 0, 0, 8, 0, 47767, 0, 0, 1, 0, '', 'Death Metal Knight - rare loot once per Darkmoon Faire'),
(1, 122899, 151254, 0, 0, 8, 0, 47767, 0, 0, 1, 0, '', 'Death Metal Knight - rare loot once per Darkmoon Faire'),
(1, 122899, 151255, 0, 0, 8, 0, 47767, 0, 0, 1, 0, '', 'Death Metal Knight - rare loot once per Darkmoon Faire'),
(1, 122899, 151265, 0, 0, 8, 0, 47767, 0, 0, 1, 0, '', 'Death Metal Knight - rare loot once per Darkmoon Faire');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (245032, 244954);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(245032, 'spell_darkmoon_power_chord'),
(244954, 'spell_darkmoon_wailing_wall');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` IN (245034, 245327);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 3, 245034, 0, 0, 32, 0, 16, 0, 0, 0, 0, '', 'Mind Blown - players only'),
(13, 3, 245327, 0, 0, 32, 0, 16, 0, 0, 0, 0, '', 'Metal Health - players only');

UPDATE `gameobject_template` SET `flags` = `flags` | 16 WHERE `entry` IN (269654, 272825);
UPDATE `creature` SET `position_z` = 8.30845 WHERE `guid` IN (505303, 505310);
