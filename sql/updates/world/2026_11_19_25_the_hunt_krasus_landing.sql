-- Krasus' Landing around The Hunt: one Kor'vas for the hunter, and the Illidari stay until he leaves for the Fel Hammer
UPDATE `creature_template` SET `ScriptName` = 'npc_korvas_bloodthorn_krasus_landing' WHERE `entry` = 99260;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 7502 AND `SourceEntry` = 6 AND `ConditionTypeOrReference` = 8 AND `ConditionValue1` IN (39247, 41863, 42872);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,7502,6,0,0,8,0,42872,0,0,1,0,'','Activate Phase - If Player Not Reward Quest'),
(23,7502,6,0,1,8,0,42872,0,0,1,0,'','Activate Phase - If Player Not Reward Quest');
