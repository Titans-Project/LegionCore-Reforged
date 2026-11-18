-- Player conditions lighting the six pre-patch invasion markers of the world map: world state 19701-19706 equal to 1
DELETE FROM `world_state_expression` WHERE `ID` BETWEEN 24590 AND 24595;
INSERT INTO `world_state_expression` (`ID`, `Expression`) VALUES
(24590, '0102F54C0000000101010000000000'),
(24591, '0102F64C0000000101010000000000'),
(24592, '0102F74C0000000101010000000000'),
(24593, '0102F84C0000000101010000000000'),
(24594, '0102F94C0000000101010000000000'),
(24595, '0102FA4C0000000101010000000000');

DELETE FROM `player_condition` WHERE `ID` BETWEEN 65001 AND 65006;
INSERT INTO `player_condition` (`ID`, `FailureDescription`, `Flags`, `Gender`, `NativeGender`, `WorldStateExpressionID`, `MinExpansionLevel`, `MaxExpansionLevel`, `MinExpansionTier`, `MaxExpansionTier`, `ChrSpecializationIndex`, `ChrSpecializationRole`, `PowerType`) VALUES
(65001, '', 1, 3, 3, 24590, -1, -1, -1, -1, 255, 255, 255),
(65002, '', 1, 3, 3, 24591, -1, -1, -1, -1, 255, 255, 255),
(65003, '', 1, 3, 3, 24592, -1, -1, -1, -1, 255, 255, 255),
(65004, '', 1, 3, 3, 24593, -1, -1, -1, -1, 255, 255, 255),
(65005, '', 1, 3, 3, 24594, -1, -1, -1, -1, 255, 255, 255),
(65006, '', 1, 3, 3, 24595, -1, -1, -1, -1, 255, 255, 255);

DELETE FROM `hotfix_data` WHERE `Id` BETWEEN 9100205 AND 9100216;
INSERT INTO `hotfix_data` (`Id`, `TableHash`, `RecordID`, `Timestamp`, `Deleted`) VALUES
(9100205, 492039028, 24590, 0, 0),
(9100206, 3108775943, 65001, 0, 0),
(9100207, 492039028, 24591, 0, 0),
(9100208, 3108775943, 65002, 0, 0),
(9100209, 492039028, 24592, 0, 0),
(9100210, 3108775943, 65003, 0, 0),
(9100211, 492039028, 24593, 0, 0),
(9100212, 3108775943, 65004, 0, 0),
(9100213, 492039028, 24594, 0, 0),
(9100214, 3108775943, 65005, 0, 0),
(9100215, 492039028, 24595, 0, 0),
(9100216, 3108775943, 65006, 0, 0);

UPDATE `world_legion`.`version` SET `hotfix_cache_id` = 571;
