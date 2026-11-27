-- Thousand Boat Bash: its five world quests come up with the festival
DELETE FROM `game_event_worldquest` WHERE `eventEntry` = 307;
INSERT INTO `game_event_worldquest` (`eventEntry`,`quest`,`VariableID`) VALUES
(307,45806,13008),
(307,45807,13009),
(307,45809,13010),
(307,45811,13011),
(307,45808,13012);
