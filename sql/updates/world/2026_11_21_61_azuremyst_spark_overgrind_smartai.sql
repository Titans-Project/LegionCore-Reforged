-- Azuremyst Isle 9531 "Tree's Company" / 9537 "Show Gnomercy": Engineer "Spark" Overgrind 17243
-- had both AIName SmartAI and ScriptName npc_engineer_spark_overgrind. The C++ AI wins, so the
-- SmartAI scene with Geezle 17318 (set data -> talk -> quest credit -> despawn) never ran and
-- 9531 could not be completed. The SmartAI already covers the 9537 gossip (menu 7426, condition
-- quest 9537 taken -> hostile -> attack); the C++ dynamite cast in combat is added to it.
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = 17243;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 17243 AND `source_type` = 0 AND `id` = 16;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(17243,0,16,0,0,0,100,0,8000,8000,8000,8000,11,7978,0,0,0,0,0,2,0,0,0,0,0,0,0,'Engineer Spark Overgrind - In combat - Cast Throw Dynamite');
