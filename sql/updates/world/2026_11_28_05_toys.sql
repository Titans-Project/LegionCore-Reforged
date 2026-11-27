-- Toys: the hitching post gets its French name, Vargoth answers when spoken to, the adopted puppies run around, the star chart floats above its stand
DELETE FROM `locales_gameobject` WHERE `entry` = 208428;
INSERT INTO `locales_gameobject` (`entry`,`name_loc2`) VALUES (208428,'Barre d''attache de la compagnie');
UPDATE `creature_template` SET `gossip_menu_id` = 8024 WHERE `entry` = 19644;
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 118191;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 118191 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(118191,0,0,0,'',54,0,100,0,0,0,0,0,0,89,6,0,0,0,0,0,1,0,0,0,0,0,0,0,0,'Adopted Puppy - Just summoned - Play around');
DELETE FROM `creature_template_movement` WHERE `CreatureId` = 84785;
INSERT INTO `creature_template_movement` (`CreatureId`,`Ground`,`Swim`,`Flight`,`Rooted`,`Random`) VALUES (84785,2,1,0,0,0);
