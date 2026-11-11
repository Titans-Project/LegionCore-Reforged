-- Demon hunter intro, after the Vault: the teleport of Illidari, We Are Leaving! lands before Khadgar, who gives The Call of War
-- in Stormwind and in Orgrimmar and leaves as a raven; in Stormwind Keep, Varian's coffin and the leaders of the Alliance mourn him
DELETE FROM `spell_target_position` WHERE `id` IN (192757,192758);
INSERT INTO `spell_target_position` (`id`,`target_map`,`target_position_x`,`target_position_y`,`target_position_z`,`target_orientation`) VALUES
(192757,0,-8537.71,457.35,104.95,2.111),
(192758,1,1463.21,-4421.19,25.56,1.65894);

DELETE FROM `creature_queststarter` WHERE `id`=97296 AND `quest`=39692;
INSERT INTO `creature_queststarter` (`id`,`quest`) VALUES (97296,39692);
UPDATE `creature_template` SET `ScriptName`='npc_dh_khadgar_call_of_war' WHERE `entry`=97296;
UPDATE `creature` SET `PhaseId`='4922' WHERE `guid`=1000000;
UPDATE `creature` SET `PhaseId`='4923' WHERE `guid`=1000002;

DELETE FROM `phase_definitions` WHERE (`zoneId`=1519 AND `entry` IN (1251,1252)) OR (`zoneId`=1637 AND `entry`=1251);
INSERT INTO `phase_definitions` (`zoneId`,`entry`,`phasemask`,`phaseId`,`PreloadMapID`,`VisibleMapID`,`UiWorldMapAreaID`,`flags`,`comment`) VALUES
(1519,1251,0,'4920',0,0,0,16,'Varian''s coffin and the mourners while The Call of War is taken'),
(1519,1252,0,'4922',0,0,0,16,'Khadgar until The Call of War is taken'),
(1637,1251,0,'4923',0,0,0,16,'Khadgar until The Call of War is taken');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND ((`SourceGroup`=1519 AND `SourceEntry` IN (1251,1252)) OR (`SourceGroup`=1637 AND `SourceEntry`=1251));
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,1519,1251,0,0,9,0,39691,0,0,0,0,'','The Call of War taken'),
(23,1519,1251,0,1,28,0,39691,0,0,0,0,'','The Call of War completed'),
(23,1519,1252,0,0,14,0,39691,0,0,0,0,'','The Call of War not taken'),
(23,1637,1251,0,0,14,0,39692,0,0,0,0,'','The Call of War not taken');

UPDATE `gameobject` SET `PhaseId`=CONCAT(`PhaseId`,' 4920') WHERE `guid`=131764 AND CONCAT(' ',`PhaseId`,' ') NOT LIKE '% 4920 %';
UPDATE `creature` SET `PhaseId`=CONCAT(`PhaseId`,' 4920') WHERE `map`=0 AND `id` IN (100978,100454,100457,100458,100979,100428,100460,100449,100754) AND CONCAT(' ',`PhaseId`,' ') LIKE '% 5552 %'
    AND `position_x` BETWEEN -8420 AND -8340 AND `position_y` BETWEEN 200 AND 290 AND `position_z` BETWEEN 145 AND 170 AND CONCAT(' ',`PhaseId`,' ') NOT LIKE '% 4920 %';
