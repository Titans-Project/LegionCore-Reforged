-- Startup data errors: Legion inscription quest items tied to their herb, daily tracking quest repeatable, a talk on a missing quest, the Warsong Wing Commander parachute
UPDATE `conditions` SET `SourceGroup` = 124101 WHERE `SourceTypeOrReferenceId` = 7 AND `SourceGroup` = 0 AND `SourceEntry` = 136909;
UPDATE `conditions` SET `SourceGroup` = 124102 WHERE `SourceTypeOrReferenceId` = 7 AND `SourceGroup` = 0 AND `SourceEntry` = 136912;
UPDATE `conditions` SET `SourceGroup` = 124103 WHERE `SourceTypeOrReferenceId` = 7 AND `SourceGroup` = 0 AND `SourceEntry` = 136915;
UPDATE `conditions` SET `SourceGroup` = 124104 WHERE `SourceTypeOrReferenceId` = 7 AND `SourceGroup` = 0 AND `SourceEntry` = 136916;
UPDATE `conditions` SET `SourceGroup` = 124105 WHERE `SourceTypeOrReferenceId` = 7 AND `SourceGroup` = 0 AND `SourceEntry` = 136917;
UPDATE `conditions` SET `SourceGroup` = 124106 WHERE `SourceTypeOrReferenceId` = 7 AND `SourceGroup` = 0 AND `SourceEntry` = 136918;
INSERT INTO `quest_template_addon` (`ID`,`SpecialFlags`) VALUES (900001,1) ON DUPLICATE KEY UPDATE `SpecialFlags` = `SpecialFlags` | 1;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 117199 AND `source_type` = 0 AND `id` = 8 AND `event_param1` = 45222;
UPDATE `smart_scripts` SET `event_type` = 23, `event_param1` = 46598, `event_param2` = 0, `event_param3` = 30000, `event_param4` = 30000, `comment` = 'Warsong Wing Commander - Ride Vehicle lost - Cast Parachute' WHERE `entryorguid` = 40942 AND `source_type` = 0 AND `id` = 0;
