-- Glowcap Festival: the festival mushrooms come back after 2 minutes like Sporeggar's glowcaps, and the Pungent Truffle below exalted is the reputation-free one
UPDATE `gameobject` SET `spawntimesecs` = 120 WHERE `id` IN (267734,267735,267736,267737,267738) AND `guid` IN (SELECT `guid` FROM `game_event_gameobject` WHERE `eventEntry` = 301);
DELETE FROM `gameobject_loot_template` WHERE `Entry` = 267735 AND `Item` = 144263;
INSERT INTO `gameobject_loot_template` (`Entry`,`Item`,`Reference`,`Chance`,`QuestRequired`,`LootMode`,`GroupId`,`MinCount`,`MaxCount`,`Comment`) VALUES
(267735,144263,0,100,0,1,0,1,1,'Smelly truffle - Pungent Truffle below exalted');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 4 AND `SourceGroup` = 267735 AND `SourceEntry` IN (144263,144264);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(4,267735,144263,0,0,5,0,970,128,0,1,0,'','Pungent Truffle - below exalted with Sporeggar'),
(4,267735,144264,0,0,5,0,970,128,0,0,0,'','Pungent Truffle - exalted with Sporeggar');
