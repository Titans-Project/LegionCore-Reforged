-- Empowered Nether Crucible: The Hunter's Gaze follows Spoils of Victory, its vision is seen before the Tomb of Sargeras and
-- speaks; the Blood Offering calls Illidan
UPDATE `quest_template_addon` SET `PrevQuestID` = 41062 WHERE `ID` = 41066;
UPDATE `quest_template_addon` SET `PrevQuestID` = 41066 WHERE `ID` = 41067;
UPDATE `gameobject_template` SET `AIName` = '', `ScriptName` = 'go_empowered_nether_crucible' WHERE `entry` = 250677;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 250677 AND `source_type` = 1;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 19337 AND `SourceEntry` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 19337, 0, 0, 0, 9, 0, 41066, 0, 0, 0, 0, '', 'Empowered Nether Crucible: the vision during The Hunter''s Gaze only');
DELETE FROM `spell_scene_event` WHERE `MiscValue` = 1203;
INSERT INTO `spell_scene_event` (`MiscValue`, `Event`, `trigerSpell`, `MonsterCredit`, `comment`) VALUES
(1203, 'CastTalkingHead', 203799, 0, 'Hunter''s Gaze: Allari speaks'),
(1203, 'complete', 203803, 0, 'Hunter''s Gaze: back, credit');
DELETE FROM `spell_script_names` WHERE `spell_id` IN (213918, 203802);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(213918, 'spell_dh_blood_offering'),
(203802, 'spell_dh_gazing_teleport');
UPDATE `spell_scene` SET `PlaybackFlags` = 16 WHERE `MiscValue` = 1203;
