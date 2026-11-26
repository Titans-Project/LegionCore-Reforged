-- Lost Isles: Sassy Hardwrench (38928) - ender of 24958 "Volcanoth!" and giver of 25023 "Old Friends".
-- Our only spawn stood at Thrall's camp in phaseMask 1, which nobody on the isles has once
-- 73065 (phaseMask 16384, from 24958 complete) is applied, so she was invisible when needed.
-- MoP 5.4.8 DB (guid 375293) has her at the volcano next to Volcanoth (38855, phase 24576);
-- the Flying Bomber (38918) she summons on 25023 starts its path there (script_waypoint 38918 point 1: 1151,1115,129).
UPDATE `creature` SET `areaId`=4911, `phaseMask`=16384, `position_x`=1163.57, `position_y`=1094.07, `position_z`=123.481, `orientation`=2.11185
WHERE `guid`=190227 AND `id`=38928;
