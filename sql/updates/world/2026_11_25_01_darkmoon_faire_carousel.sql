-- Darkmoon Faire: one ride ticket book per level range, the carousel takes a ride ticket and gives WHEE!
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 25 AND `SourceGroup` = 68402 AND `SourceEntry` BETWEEN 92788 AND 92794;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(25, 68402, 92788, 0, 0, 27, 0, 19, 4, 0, 0, 0, '', 'Kae Ti - Ride Ticket Book - levels 1 to 19'),
(25, 68402, 92789, 0, 0, 27, 0, 20, 3, 0, 0, 0, '', 'Kae Ti - Ride Ticket Book - levels 20 to 39'),
(25, 68402, 92789, 0, 0, 27, 0, 39, 4, 0, 0, 0, '', 'Kae Ti - Ride Ticket Book - levels 20 to 39'),
(25, 68402, 92790, 0, 0, 27, 0, 40, 3, 0, 0, 0, '', 'Kae Ti - Ride Ticket Book - levels 40 to 59'),
(25, 68402, 92790, 0, 0, 27, 0, 59, 4, 0, 0, 0, '', 'Kae Ti - Ride Ticket Book - levels 40 to 59'),
(25, 68402, 92791, 0, 0, 27, 0, 60, 3, 0, 0, 0, '', 'Kae Ti - Ride Ticket Book - levels 60 to 69'),
(25, 68402, 92791, 0, 0, 27, 0, 69, 4, 0, 0, 0, '', 'Kae Ti - Ride Ticket Book - levels 60 to 69'),
(25, 68402, 92792, 0, 0, 27, 0, 70, 3, 0, 0, 0, '', 'Kae Ti - Ride Ticket Book - levels 70 to 79'),
(25, 68402, 92792, 0, 0, 27, 0, 79, 4, 0, 0, 0, '', 'Kae Ti - Ride Ticket Book - levels 70 to 79'),
(25, 68402, 92793, 0, 0, 27, 0, 80, 3, 0, 0, 0, '', 'Kae Ti - Ride Ticket Book - levels 80 to 89'),
(25, 68402, 92793, 0, 0, 27, 0, 89, 4, 0, 0, 0, '', 'Kae Ti - Ride Ticket Book - levels 80 to 89'),
(25, 68402, 92794, 0, 0, 27, 0, 90, 3, 0, 0, 0, '', 'Kae Ti - Ride Ticket Book - levels 90 and above');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` IN (68231, 68232, 68233, 68238, 68239) AND `SourceEntry` IN (75648, 134815);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 68231, 75648, 0, 0, 2, 0, 81055, 1, 0, 0, 0, '', 'Carousel Dragon - needs a Darkmoon Ride Ticket'),
(18, 68231, 134815, 0, 0, 2, 0, 81055, 1, 0, 0, 0, '', 'Carousel Dragon - needs a Darkmoon Ride Ticket'),
(18, 68232, 75648, 0, 0, 2, 0, 81055, 1, 0, 0, 0, '', 'Carousel Gryphon - needs a Darkmoon Ride Ticket'),
(18, 68232, 134815, 0, 0, 2, 0, 81055, 1, 0, 0, 0, '', 'Carousel Gryphon - needs a Darkmoon Ride Ticket'),
(18, 68233, 75648, 0, 0, 2, 0, 81055, 1, 0, 0, 0, '', 'Carousel Murloc - needs a Darkmoon Ride Ticket'),
(18, 68233, 134815, 0, 0, 2, 0, 81055, 1, 0, 0, 0, '', 'Carousel Murloc - needs a Darkmoon Ride Ticket'),
(18, 68238, 75648, 0, 0, 2, 0, 81055, 1, 0, 0, 0, '', 'Carousel Rocket - needs a Darkmoon Ride Ticket'),
(18, 68238, 134815, 0, 0, 2, 0, 81055, 1, 0, 0, 0, '', 'Carousel Rocket - needs a Darkmoon Ride Ticket'),
(18, 68239, 75648, 0, 0, 2, 0, 81055, 1, 0, 0, 0, '', 'Carousel Wyvern - needs a Darkmoon Ride Ticket'),
(18, 68239, 134815, 0, 0, 2, 0, 81055, 1, 0, 0, 0, '', 'Carousel Wyvern - needs a Darkmoon Ride Ticket');

DELETE FROM `spell_script_names` WHERE `spell_id` = 134815;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(134815, 'spell_gen_darkmoon_carousel_whee');
