-- Azuremyst Isle 9528 "A Cry for Help": Magwin 17312 had both AIName SmartAI and ScriptName
-- npc_magwin. The C++ escort wins and has no script_waypoint rows ("starts with 0 waypoints" in
-- DBErrors.log), so the escort never moved and the quest could not be completed. The SmartAI
-- escort (path 17312, 28 points, quest credit at Cowlen 17311) takes over.
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = 17312;

-- Texts as in the C++ script / Cata 4.3.4 TDB: SAY_START on accept, SAY_AGGRO on aggro
-- (was group 0 every 5-10 s in combat), and EMOTE_HUG (group 5), which the SmartAI waits for
-- before despawning but had no creature_text row.
UPDATE `smart_scripts` SET `link` = 11 WHERE `entryorguid` = 17312 AND `source_type` = 0 AND `id` = 1;
UPDATE `smart_scripts` SET `event_type` = 4, `event_param1` = 0, `event_param2` = 0, `event_param3` = 0, `event_param4` = 0, `action_param1` = 1, `comment` = 'Magwin - On aggro - Say text'
WHERE `entryorguid` = 17312 AND `source_type` = 0 AND `id` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 17312 AND `source_type` = 0 AND `id` = 11;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(17312,0,11,0,61,0,100,0,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Magwin - On quest accept - Say text');

DELETE FROM `creature_text` WHERE `CreatureID` = 17312 AND `GroupID` = 5;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(17312,5,0,'%s hugs her father.',16,7,100,0,0,0,13747,'Magwin - EMOTE_HUG');
