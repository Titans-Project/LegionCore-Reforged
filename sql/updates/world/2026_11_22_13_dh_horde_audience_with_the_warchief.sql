-- Horde demon hunters: Khadgar gives Audience with the Warchief, their Fate of the Horde, instead of the unused Horde The Call of War
DELETE FROM `creature_queststarter` WHERE `id` = 97296 AND `quest` IN (39692,40976);
INSERT INTO `creature_queststarter` (`id`,`quest`) VALUES (97296,40976);
UPDATE `phase_definitions` SET `comment` = 'Khadgar until Audience with the Warchief is taken' WHERE `zoneId` = 1637 AND `entry` = 1251;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 1637 AND `SourceEntry` = 1251;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,1637,1251,0,0,14,0,40976,0,0,0,0,'','Audience with the Warchief not taken');
