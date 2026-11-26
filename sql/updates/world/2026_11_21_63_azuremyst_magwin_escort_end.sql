-- Azuremyst Isle 9528 "A Cry for Help", Magwin 17312 escort (SmartAI), after an in-game test:
-- 1. She runs instead of walking (the walk was slow).
-- 2. "You can thank $n ..." named nobody: the text targeted Magwin herself; it targets the
--    closest player now.
-- 3. The pier where Cowlen 17311 stands is missing from the navmesh (mmaps tile 05304252 has only
--    water polygons at z 0.25 around him), so the last points 27-28 were walked through the water
--    under the pier, Cowlen (10 yd signal range) never answered, and she stayed there without the
--    hug and the despawn. The end scene now starts at point 26, the last point on the ground at the
--    foot of the pier, the path stops there, and the Magwin <-> Cowlen signals reach 40 yd.
UPDATE `smart_scripts` SET `action_param1` = 1 WHERE `entryorguid` = 17312 AND `source_type` = 0 AND `id` = 1;
UPDATE `smart_scripts` SET `target_type` = 21, `target_param1` = 30 WHERE `entryorguid` = 17312 AND `source_type` = 0 AND `id` = 7;
UPDATE `smart_scripts` SET `event_param1` = 26, `link` = 12 WHERE `entryorguid` = 17312 AND `source_type` = 0 AND `id` = 5;
UPDATE `smart_scripts` SET `target_param2` = 40 WHERE `entryorguid` = 17312 AND `source_type` = 0 AND `id` = 8;
UPDATE `smart_scripts` SET `target_param2` = 40 WHERE `entryorguid` = 17311 AND `source_type` = 0 AND `id` = 3;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 17312 AND `source_type` = 0 AND `id` = 12;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(17312,0,12,0,61,0,100,0,0,0,0,0,55,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Magwin - On waypoint 26 - Stop path');
