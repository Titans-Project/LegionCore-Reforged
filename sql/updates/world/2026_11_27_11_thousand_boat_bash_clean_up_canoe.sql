-- Thousand Boat Bash: the Clean-Up Canoe rows a wide loop on the surface of the lake instead of sinking as it wandered
UPDATE `creature` SET `position_x` = -5707.0, `position_y` = -4115.0, `position_z` = 0.25, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 146728312 AND `id` = 118114;
UPDATE `creature_template_movement` SET `Ground` = 0, `Swim` = 0, `Flight` = 1 WHERE `CreatureId` = 118114;
DELETE FROM `waypoint_data` WHERE `id` = 146728312;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`delay_chance`,`move_type`,`speed`,`action`,`action_chance`,`wpguid`) VALUES
(146728312,1,-5707.0,-4115.0,0.25,2.197,0,0,0,0,0,0,0),
(146728312,2,-5804.0,-3981.0,0.25,1.260,0,0,0,0,0,0,0),
(146728312,3,-5733.0,-3760.0,0.25,2.631,0,0,0,0,0,0,0),
(146728312,4,-5901.0,-3666.0,0.25,2.833,0,0,0,0,0,0,0),
(146728312,5,-6067.0,-3613.0,0.25,3.268,0,0,0,0,0,0,0),
(146728312,6,-6256.0,-3637.0,0.25,3.794,0,0,0,0,0,0,0),
(146728312,7,-6375.0,-3728.0,0.25,4.450,0,0,0,0,0,0,0),
(146728312,8,-6426.0,-3918.0,0.25,5.302,0,0,0,0,0,0,0),
(146728312,9,-6345.0,-4039.0,0.25,5.706,0,0,0,0,0,0,0),
(146728312,10,-6219.0,-4121.0,0.25,0.376,0,0,0,0,0,0,0),
(146728312,11,-5935.0,-4009.0,0.25,5.848,0,0,0,0,0,0,0);
DELETE FROM `creature_addon` WHERE `guid` = 146728312;
INSERT INTO `creature_addon` (`guid`,`path_id`) VALUES (146728312,146728312);
