-- Shadowglen: Dirania Silvershine sits to talk with Melithar Staghelm, Dentaria Silverglade and Huntress Sandrya Moonfall ride a short way near Ilthalaine
UPDATE `creature_template_addon` SET `emote` = 426 WHERE `entry` = 8583;
DELETE FROM `creature_addon` WHERE `guid` IN (77379,190931);
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(77379,773790,6448,0,1,0,''),
(190931,1909310,9991,0,0,0,'');
UPDATE `creature` SET `MovementType` = 2, `spawndist` = 0 WHERE `guid` IN (77379,190931);
DELETE FROM `waypoint_data` WHERE `id` IN (773790,1909310);
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`delay_chance`,`move_type`,`speed`,`action`,`action_chance`,`entry`,`wpguid`) VALUES
(773790,1,10317.0,827.27,1326.46,2.44346,8000,0,0,0,0,100,0,0),
(773790,2,10321.2,831.6,1326.55,0,0,0,0,0,0,100,0,0),
(773790,3,10325.4,834.3,1326.6,0.6,6000,0,0,0,0,100,0,0),
(773790,4,10321.2,831.6,1326.55,0,0,0,0,0,0,100,0,0),
(1909310,1,10315.3,824.56,1326.42,2.25817,10000,0,0,0,0,100,0,0),
(1909310,2,10312.3,821.4,1326.8,0,0,0,0,0,0,100,0,0),
(1909310,3,10309.8,817.2,1327.3,4.1,6000,0,0,0,0,100,0,0),
(1909310,4,10312.3,821.4,1326.8,0,0,0,0,0,0,100,0,0);
