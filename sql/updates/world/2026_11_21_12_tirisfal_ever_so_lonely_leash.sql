-- Tirisfal Glades, 24974 "Ever So Lonely": Murloc Leash 52059 casts 73108 on a weakened Vile Fin murloc, which turns
-- into a captured murloc that follows the player back to Sedrick Calston 38925.
-- Here only Vile Fin Puddlejumper 1543 reacted, at any health. Like the Cata 4.3.4 / Cata Classic 4.4.2 TDB:
--  * 73108 only on Vile Fin Puddlejumper 1543 or Vile Fin Minor Oracle 1544 below 30% health
--    (Wowhead quest=24974: "weaken it first before it can be leashed");
--  * Vile Fin Minor Oracle 1544 is leashed too (Cata SAI 1544 SpellHit 73108 -> 73434 "Summon Minor Oracle: Force Cast";
--    Wowhead quest=24974 lists Captured Vile Fin Minor Oracle 39078), and its captured form 39078 is credited at Sedrick.
-- The Oracle rows copy this DB's own 1543 / 38923 / 38925 rows.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=17 AND `SourceEntry`=73108;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(17,0,73108,0,0,31,1,3,1543,0,0,0,'','Murloc Leash - Vile Fin Puddlejumper'),
(17,0,73108,0,0,38,1,30,2,0,0,0,'','Murloc Leash - target below 30% health'),
(17,0,73108,0,1,31,1,3,1544,0,0,0,'','Murloc Leash - Vile Fin Minor Oracle'),
(17,0,73108,0,1,38,1,30,2,0,0,0,'','Murloc Leash - target below 30% health');

-- Vile Fin Minor Oracle: leashed -> the player summons the captured Oracle, kill credit 38923, despawn
DELETE FROM `smart_scripts` WHERE `entryorguid`=1544 AND `source_type`=0 AND `id` IN (3,4,5);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1544,0,3,4,8,0,100,0,73108,0,0,0,11,73434,0,0,0,0,0,7,0,0,0,0,0,0,0,'Vile Fin Minor Oracle - On SpellHit Murloc Leash - Summon Minor Oracle: Force Cast'),
(1544,0,4,5,61,0,100,0,0,0,0,0,33,38923,0,0,0,0,0,7,0,0,0,0,0,0,0,'Vile Fin Minor Oracle - Link - Kill Credit Vile Fin captured'),
(1544,0,5,0,61,0,100,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vile Fin Minor Oracle - Link - Despawn');

-- Captured Vile Fin Minor Oracle: follows the player; at Sedrick (data 1 1) credits its owner 38887, despawns
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=39078;
DELETE FROM `smart_scripts` WHERE `entryorguid`=39078 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(39078,0,0,1,38,0,100,0,1,1,0,0,33,38887,0,0,0,0,0,23,0,0,0,0,0,0,0,'Captured Vile Fin Minor Oracle - On Data Set - Kill Credit Owner Vile Fin returned'),
(39078,0,1,0,61,0,100,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Captured Vile Fin Minor Oracle - Link - Despawn'),
(39078,0,3,0,54,0,100,0,0,0,0,0,29,3,0,38925,0,0,0,18,20,0,0,0,0,0,0,'Captured Vile Fin Minor Oracle - Just Summoned - Follow Player');

-- Sedrick Calston: also tell a captured Oracle beside him
DELETE FROM `smart_scripts` WHERE `entryorguid`=38925 AND `source_type`=0 AND `id`=1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(38925,0,1,0,1,0,100,0,3000,3000,3000,3000,45,1,1,0,0,0,0,11,39078,5,0,0,0,0,0,'Sedrick Calston - OOC 3s - Set Data 1 1 Captured Vile Fin Minor Oracle (Quest 24974)');
