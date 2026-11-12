-- Stormwind Keep, demon hunter epilogue: the funeral lasts until In the Blink of an Eye, the throne scene, the disguised demons and the assassins of the attack
-- the funeral: from The Call of War to In the Blink of an Eye
DELETE FROM `phase_definitions` WHERE (`zoneId`=1519 AND `entry` IN (1251,1252)) OR (`zoneId`=1637 AND `entry`=1251);
INSERT INTO `phase_definitions` (`zoneId`,`entry`,`phasemask`,`phaseId`,`PreloadMapID`,`VisibleMapID`,`UiWorldMapAreaID`,`flags`,`comment`) VALUES
(1519,1251,0,'4920',0,0,0,16,'Varian''s coffin and the mourners, from The Call of War to In the Blink of an Eye'),
(1519,1252,0,'4922',0,0,0,16,'Khadgar until The Call of War is taken'),
(1637,1251,0,'4923',0,0,0,16,'Khadgar until The Call of War is taken');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND ((`SourceGroup`=1519 AND `SourceEntry` IN (1251,1252)) OR (`SourceGroup`=1637 AND `SourceEntry`=1251));
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,1519,1251,0,0,14,0,39691,0,0,1,0,'','Funeral: The Call of War taken'),
(23,1519,1251,0,0,14,0,44663,0,0,0,0,'','Funeral: In the Blink of an Eye not taken yet'),
(23,1519,1252,0,0,14,0,39691,0,0,0,0,'','The Call of War not taken'),
(23,1637,1251,0,0,14,0,39692,0,0,0,0,'','The Call of War not taken');

-- Stormwind Keep: the demon hunter's throne scene gets its own phases, so that one Jace and one Anduin only are seen
-- (the throne Jace of the Legion intro and the demon hunter's Jace, the standing Anduin and the one on the throne)
UPDATE `phase_definitions` SET `phaseId`='7047 6821 6690 6666 6481 4070 4064 1231 1139 4924' WHERE `zoneId`=1519 AND `entry`=6;
UPDATE `phase_definitions` SET `phaseId`='7053 6821 6481 5686 5513 4070 4064 1231 1139 7510 4924' WHERE `zoneId`=1519 AND `entry`=7;
DELETE FROM `phase_definitions` WHERE `zoneId`=1519 AND `entry`=13;
INSERT INTO `phase_definitions` (`zoneId`,`entry`,`phasemask`,`phaseId`,`PreloadMapID`,`VisibleMapID`,`UiWorldMapAreaID`,`flags`,`comment`) VALUES
(1519,13,1,'7053 6821 6481 5686 5513 4070 4064 1231 1139 7510 4928',0,0,0,24,'Stormwind. Demon hunter at the throne, from The Call of War to In the Blink of an Eye');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND `SourceGroup`=1519 AND ((`SourceEntry`=7 AND `ElseGroup`=3) OR `SourceEntry` IN (12,13));
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,1519,13,0,0,8,0,44663,0,0,1,0,'','Demon hunter at the throne: In the Blink of an Eye not rewarded'),
(23,1519,13,0,0,14,0,39691,0,0,1,0,'','Demon hunter at the throne: The Call of War taken'),
(23,1519,13,0,0,28,0,44663,0,0,1,0,'','Demon hunter at the throne: In the Blink of an Eye not completed'),
(23,1519,12,0,0,14,0,42978,0,0,0,0,'','Standing Anduin: 42978 not taken'),
(23,1519,12,0,0,14,0,46272,0,0,0,0,'','Standing Anduin: 46272 not taken'),
(23,1519,12,0,0,14,0,39691,0,0,0,0,'','Standing Anduin: not a demon hunter at the throne (The Call of War not taken)'),
(23,1519,12,0,3,14,0,42978,0,0,0,0,'','Standing Anduin: 42978 not taken'),
(23,1519,12,0,3,14,0,46272,0,0,0,0,'','Standing Anduin: 46272 not taken'),
(23,1519,12,0,3,8,0,44663,0,0,0,0,'','Standing Anduin: In the Blink of an Eye rewarded'),
(23,1519,12,0,4,14,0,42978,0,0,0,0,'','Standing Anduin: 42978 not taken'),
(23,1519,12,0,4,14,0,46272,0,0,0,0,'','Standing Anduin: 46272 not taken'),
(23,1519,12,0,4,28,0,44663,0,0,0,0,'','Standing Anduin: In the Blink of an Eye completed'),
(23,1519,12,0,1,8,0,42982,0,0,0,0,'','true if player reward quest'),
(23,1519,12,0,1,8,0,46272,0,0,0,0,'','true if player reward quest'),
(23,1519,12,0,2,8,0,46282,0,0,0,0,'','true if player reward quest');
UPDATE `creature` SET `PhaseId`='4924' WHERE `guid`=344579;
UPDATE `creature` SET `PhaseId`='4928' WHERE `guid`=1000001;

-- Demons Among Them: warning Anduin gives its objective, after which the Felblade Assassins strike the funeral
DELETE FROM `smart_scripts` WHERE `entryorguid`=100429 AND `source_type`=0 AND `id`=7;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`action_type`,`action_param1`,`target_type`,`comment`) VALUES
(100429,0,7,0,62,0,100,0,19075,1,33,111585,7,'At gossip select, Demons Among Them: Anduin warned');

-- Second Sight: five of the Royal Guards at the funeral are demons in disguise; Spectral Sight shows the Felblade Assassins
-- standing in their place, until Anduin is warned and the attack begins (the Jace of the demon hunters refreshes the phases)
DELETE FROM `phase_definitions` WHERE `zoneId`=1519 AND `entry` IN (1253,1254);
INSERT INTO `phase_definitions` (`zoneId`,`entry`,`phasemask`,`phaseId`,`PreloadMapID`,`VisibleMapID`,`UiWorldMapAreaID`,`flags`,`comment`) VALUES
(1519,1253,0,'4929',0,0,0,16,'Second Sight: the disguised demons, seen with Spectral Sight'),
(1519,1254,0,'4930',0,0,0,16,'Second Sight: the same guards, seen without Spectral Sight');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND `SourceGroup`=1519 AND `SourceEntry` IN (1253,1254);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,1519,1253,0,0,14,0,44471,0,0,1,0,'','Demons seen: Second Sight taken'),
(23,1519,1253,0,0,41,0,44463,111585,0,1,0,'','Demons seen: Anduin not warned yet'),
(23,1519,1253,0,0,8,0,44463,0,0,1,0,'','Demons seen: Demons Among Them not rewarded'),
(23,1519,1253,0,0,1,0,188501,2,0,0,0,'','Demons seen: Spectral Sight on (its screen effect)'),
(23,1519,1254,0,0,14,0,44471,0,0,1,0,'','Guards seen: Second Sight taken'),
(23,1519,1254,0,0,41,0,44463,111585,0,1,0,'','Guards seen: Anduin not warned yet'),
(23,1519,1254,0,0,8,0,44463,0,0,1,0,'','Guards seen: Demons Among Them not rewarded'),
(23,1519,1254,0,0,1,0,188501,2,0,1,0,'','Guards seen: Spectral Sight off');
DELETE FROM `creature` WHERE `guid` BETWEEN 146932703 AND 146932712;
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`PhaseId`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`MovementType`,`unit_flags`) VALUES
(146932703,1756,0,1519,6292,1,1,'4930',-8404.01,224.37,155.35,6.219,120,0,0),
(146932704,1756,0,1519,6292,1,1,'4930',-8391.76,229.61,155.43,1.182,120,0,0),
(146932705,1756,0,1519,6292,1,1,'4930',-8377.60,238.82,155.43,2.334,120,0,0),
(146932706,1756,0,1519,6292,1,1,'4930',-8375.19,266.03,155.35,0.613,120,0,0),
(146932707,1756,0,1519,6292,1,1,'4930',-8366.64,250.79,155.43,6.265,120,0,0),
(146932708,100993,0,1519,6292,1,1,'4929',-8404.01,224.37,155.35,6.219,120,0,768),
(146932709,100993,0,1519,6292,1,1,'4929',-8391.76,229.61,155.43,1.182,120,0,768),
(146932710,100993,0,1519,6292,1,1,'4929',-8377.60,238.82,155.43,2.334,120,0,768),
(146932711,100993,0,1519,6292,1,1,'4929',-8375.19,266.03,155.35,0.613,120,0,768),
(146932712,100993,0,1519,6292,1,1,'4929',-8366.64,250.79,155.43,6.265,120,0,768);

-- Stormwind Keep, the attack: the Jace who fights beside the other classes (Demons Among Us) is not seen by the demon hunter,
-- whose own Jace is already there (Demons Among Them gets its own entry, without his phase)
UPDATE `phase_definitions` SET `phaseId`='5552 4934' WHERE `zoneId`=1519 AND `entry`=1250;
DELETE FROM `phase_definitions` WHERE `zoneId`=1519 AND `entry`=1255;
INSERT INTO `phase_definitions` (`zoneId`,`entry`,`phasemask`,`phaseId`,`PreloadMapID`,`VisibleMapID`,`UiWorldMapAreaID`,`flags`,`comment`) VALUES
(1519,1255,0,'5552',0,0,0,16,'demons among them (demon hunter)');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND `SourceGroup`=1519 AND ((`SourceEntry`=1250 AND `ElseGroup` IN (2,4)) OR `SourceEntry`=1255);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,1519,1255,0,0,14,0,44663,0,0,0,0,'','Legion Start Questline - QUEST_NONE In the Blink of an Eye'),
(23,1519,1255,0,0,41,0,44463,111585,0,0,0,'','Legion Start Questline - QUEST_OBJECTIVE_DONE Demons Among Them'),
(23,1519,1255,0,1,8,0,44463,0,0,0,0,'','Legion Start Questline - QUEST_REWARDED Demons Among Them'),
(23,1519,1255,0,1,14,0,44663,0,0,0,0,'','Legion Start Questline - QUEST_NONE In the Blink of an Eye');
UPDATE `creature` SET `PhaseId`='4934' WHERE `guid`=344589;

-- Stormwind Keep, the attack: the Felblade Assassins are there from the warning to the turn-in only, not until In the Blink
-- of an Eye with the mourners (Demons Among Us for the other classes, Demons Among Them for the demon hunter)
DELETE FROM `phase_definitions` WHERE `zoneId`=1519 AND `entry` IN (1256,1257);
INSERT INTO `phase_definitions` (`zoneId`,`entry`,`phasemask`,`phaseId`,`PreloadMapID`,`VisibleMapID`,`UiWorldMapAreaID`,`flags`,`comment`) VALUES
(1519,1256,0,'4935',0,0,0,16,'Felblade Assassins during Demons Among Us'),
(1519,1257,0,'4935',0,0,0,16,'Felblade Assassins during Demons Among Them');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND `SourceGroup`=1519 AND `SourceEntry` IN (1256,1257);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,1519,1256,0,0,41,0,40593,111585,0,0,0,'','Assassins: Jace heard (Demons Among Us)'),
(23,1519,1256,0,0,8,0,40593,0,0,1,0,'','Assassins: Demons Among Us not rewarded'),
(23,1519,1257,0,0,41,0,44463,111585,0,0,0,'','Assassins: Anduin warned (Demons Among Them)'),
(23,1519,1257,0,0,8,0,44463,0,0,1,0,'','Assassins: Demons Among Them not rewarded');
UPDATE `creature` SET `PhaseId`='4935' WHERE `id`=100993 AND `map`=0 AND `PhaseId`='5552';
