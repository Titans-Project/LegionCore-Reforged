-- Vanilla world bosses: Lord Kazzak and Azuregos get their C++ script, Azuregos his line as he draws his foes to him
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'boss_lord_kazzak' WHERE `entry` = 12397;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'boss_azuregos' WHERE `entry` = 6109;
DELETE FROM `creature_text` WHERE `CreatureID` = 6109;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextID`, `comment`) VALUES
(6109, 0, 0, 'Come, little ones. Face me!', 14, 0, 100, 0, 0, 0, 9071, 'Azuregos - Arcane Vacuum');
