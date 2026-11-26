-- Mulgore: 24861 "Last Rites, First Rites" could not be completed.
-- Water Pitcher 50465 casts 71898 (effect 0 summons GO 202138, effect 1 dummy on nearby 38438),
-- but 38438 "Mulgore Offering Credit Bunny" had no SmartAI and the spell had no target condition,
-- so the objective "Offering Placed" (38438) was never credited.
-- Condition as Cata 4.3.4 TDB (13,2,71898 -> 38438); credit on the dummy hit, as other Mulgore
-- item-credit bunnies (36727 Feed of Evil).
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=71898;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,2,71898,0,0,31,0,3,38438,0,0,0,'','Funeral Offering - Target Mulgore Offering Credit Bunny');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=38438;
DELETE FROM `smart_scripts` WHERE `entryorguid`=38438 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`Difficulties`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_param4`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(38438,0,0,0,'',8,0,100,0,71898,0,0,0,0,33,38438,0,0,0,0,0,7,0,0,0,0,0,0,0,0,'Mulgore Offering Credit Bunny - On Spellhit Funeral Offering - Kill Credit');
