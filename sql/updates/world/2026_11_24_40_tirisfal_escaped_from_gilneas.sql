-- Tirisfal Glades, 24992 "Escaped From Gilneas" (Horde): fighting a Decrepit 1547 or Cursed Darkhound 1548 brings a
-- Worgen Infiltrator 38949, which speaks and gives the (auto-accept) quest.
-- Here the hound had a 10% chance to cast 73150 on itself at 40-70% health, vanished, and the worgen gave the quest only
-- after its 8 s line to any player within 15 yards, whether or not that player had already done it.
-- As in the Cata 4.3.4 TDB and Cata Classic 4.4.2 TDB (identical): 5 s after aggro the attacker casts 73150 (client
-- DB2 + Wowhead spell=73150: summons 38949 for 15 s), the worgen talks and adds the quest to its summoner at once.
-- Wayback quest=24992 (2013-01-18): players confirm both hounds, and that the hound must be left alive for a while and
-- not be pulled by a pet. Ravenous Darkhound 1549 has no such event in either TDB.
-- Offer conditions: the TDBs require the quest not taken / complete / rewarded (= quest state none); AddQuest checks
-- nothing, so Horde and level 5 (quest_template) are added, and the same state check guards the worgen's add.

-- hounds
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (1548,1549) AND `id` IN (1,2) AND (`event_type`=2 OR `event_type`=61);
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (1547,1548) AND `id`=2;
DELETE FROM `smart_scripts` WHERE `source_type`=9 AND `entryorguid` IN (154700,154800);
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=38949;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(1547,0,2,0,4,0,100,0,0,0,0,0,80,154700,2,0,0,0,0,1,0,0,0,0,0,0,0,'Decrepit Darkhound - On Aggro - Run Script (Quest 24992)'),
(1548,0,2,0,4,0,100,0,0,0,0,0,80,154800,2,0,0,0,0,1,0,0,0,0,0,0,0,'Cursed Darkhound - On Aggro - Run Script (Quest 24992)'),
(154700,9,0,0,0,0,100,0,5000,5000,0,0,85,73150,0,0,0,0,0,7,0,0,0,0,0,0,0,'Decrepit Darkhound - Script - Invoker Cast Summon Worgen Infiltrator'),
(154800,9,0,0,0,0,100,0,5000,5000,0,0,85,73150,0,0,0,0,0,7,0,0,0,0,0,0,0,'Cursed Darkhound - Script - Invoker Cast Summon Worgen Infiltrator'),
(38949,0,0,0,54,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Worgen Infiltrator - Just Summoned - Say Line 0'),
(38949,0,1,0,54,0,100,0,0,0,0,0,7,24992,1,0,0,0,0,7,0,0,0,0,0,0,0,'Worgen Infiltrator - Just Summoned - Add Quest 24992');

-- SourceGroup = event id + 1
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceGroup`=3 AND `SourceEntry` IN (1547,1548) AND `SourceId`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceGroup`=2 AND `SourceEntry`=38949 AND `SourceId`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(22,3,1547,0,0,14,0,24992,0,0,0,0,'','Decrepit Darkhound - quest 24992 not taken, complete or rewarded'),
(22,3,1547,0,0,6,0,67,0,0,0,0,'','Decrepit Darkhound - Horde player'),
(22,3,1547,0,0,27,0,5,3,0,0,0,'','Decrepit Darkhound - player level 5+'),
(22,3,1548,0,0,14,0,24992,0,0,0,0,'','Cursed Darkhound - quest 24992 not taken, complete or rewarded'),
(22,3,1548,0,0,6,0,67,0,0,0,0,'','Cursed Darkhound - Horde player'),
(22,3,1548,0,0,27,0,5,3,0,0,0,'','Cursed Darkhound - player level 5+'),
(22,2,38949,0,0,14,0,24992,0,0,0,0,'','Worgen Infiltrator - quest 24992 not taken, complete or rewarded');
