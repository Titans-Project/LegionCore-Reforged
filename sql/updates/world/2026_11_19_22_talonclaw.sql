-- Talonclaw: weapon claim credit, Dorro's farewell and portal to Dalaran, Dakarr's death only in his lair, spear carrier model and chest loot admission.
-- The claim of the spear is an objective of its own (Amount 1 as in the client data); Return to Dalaran no longer comes from Dakarr's death
UPDATE `quest_objectives` SET `Amount` = 1
WHERE `ID` = 280794 AND `QuestID` = 40385 AND `ObjectID` = 100112 AND `Amount` = 0;

UPDATE `smart_scripts` AS `summon`
JOIN `smart_scripts` AS `credit` ON `credit`.`entryorguid` = `summon`.`entryorguid` AND `credit`.`source_type` = `summon`.`source_type` AND `credit`.`id` = 4
SET `summon`.`link` = 0
WHERE `summon`.`entryorguid` = 99480 AND `summon`.`source_type` = 0 AND `summon`.`id` = 3 AND `summon`.`link` = 4
  AND `credit`.`event_type` = 6 AND `credit`.`action_type` = 33 AND `credit`.`action_param1` = 103798;

DELETE FROM `smart_scripts`
WHERE `entryorguid` = 99480 AND `source_type` = 0 AND `id` = 4
  AND `event_type` = 6 AND `action_type` = 33 AND `action_param1` = 103798;

-- The final death only counts while the scenario is on the lair step
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 2 AND `SourceEntry` = 99480 AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 2, 99480, 0, 0, 44, 0, 973, 11, 0, 0, 0, '', 'Talonclaw: final Dakarr death only in the lair step');

-- Spear chest: the script finishes the pickup, Dorro is summoned once the scene is over
UPDATE `gameobject_template` SET `ScriptName` = 'go_talonclaw_spear' WHERE `entry` = 248834 AND `ScriptName` = '';

UPDATE `smart_scripts` SET `link` = 0
WHERE `entryorguid` = 248834 AND `source_type` = 1 AND `id` = 1 AND `link` = 3 AND `action_type` = 205 AND `action_param2` = 47689;

DELETE FROM `smart_scripts`
WHERE `entryorguid` = 248834 AND `source_type` = 1
  AND ((`id` = 2 AND `event_type` = 64 AND `action_type` = 85 AND `action_param1` = 205589)
    OR (`id` = 3 AND `event_type` = 61 AND `action_type` = 12 AND `action_param1` = 100005));

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 3 AND `SourceEntry` = 248834 AND `SourceId` = 1;

UPDATE `creature_template` SET `ScriptName` = 'npc_talonclaw_dorro' WHERE `entry` = 100005 AND `AIName` = 'SmartAI' AND `ScriptName` = '';

DELETE FROM `creature_text` WHERE `CreatureID` = 100005 AND `GroupID` = 1;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextID`, `MinTimer`, `MaxTimer`, `SpellID`, `comment`) VALUES
(100005, 1, 0, 'Hero, I sense your destiny lies with the ancient night elf who started you on this journey. Before I depart, I can send you back to the city in the sky. May Talonclaw serve you well.', 12, 0, 100, 0, 0, 0, 106397, 0, 0, 0, 'Dorro farewell');

-- Portal to Dalaran offered by Dorro; the script casts the return teleport
DELETE FROM `gameobject_template` WHERE `entry` = 9000575;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `size`, `ScriptName`) VALUES
(9000575, 10, 8111, 'Portal to Dalaran', 'interact', 1, 'go_talonclaw_return_portal');

-- The spear carrier takes its invisible model (the visual comes from its state kit) instead of one of its two displays at random
UPDATE `creature_template` AS `actor`
JOIN `creature_template_wdb` AS `native` ON `native`.`Entry` = `actor`.`entry`
SET `actor`.`flags_extra` = `actor`.`flags_extra` | 128
WHERE `actor`.`entry` = 100310 AND `actor`.`flags_extra` = 0 AND `actor`.`AIName` = '' AND `actor`.`ScriptName` = ''
  AND `native`.`Displayid1` = 33225 AND `native`.`Displayid2` = 66609 AND `native`.`Displayid3` = 0 AND `native`.`Displayid4` = 0;

-- Only a hunter on the claim step of the scenario can loot the spear
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 4 AND `SourceGroup` = 248834 AND `SourceEntry` = 128808;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(4, 248834, 128808, 0, 0, 15, 0, 4, 0, 0, 0, 0, '', 'Talonclaw: hunter, scenario step 12, quest taken'),
(4, 248834, 128808, 0, 0, 22, 0, 1511, 0, 0, 0, 0, '', 'Talonclaw: hunter, scenario step 12, quest taken'),
(4, 248834, 128808, 0, 0, 44, 0, 973, 12, 0, 0, 0, '', 'Talonclaw: hunter, scenario step 12, quest taken'),
(4, 248834, 128808, 0, 0, 9, 0, 40385, 0, 0, 0, 0, '', 'Talonclaw: hunter, scenario step 12, quest taken'),
(4, 248834, 128808, 0, 1, 15, 0, 4, 0, 0, 0, 0, '', 'Talonclaw: hunter, scenario step 13, quest taken'),
(4, 248834, 128808, 0, 1, 22, 0, 1511, 0, 0, 0, 0, '', 'Talonclaw: hunter, scenario step 13, quest taken'),
(4, 248834, 128808, 0, 1, 44, 0, 973, 13, 0, 0, 0, '', 'Talonclaw: hunter, scenario step 13, quest taken'),
(4, 248834, 128808, 0, 1, 9, 0, 40385, 0, 0, 0, 0, '', 'Talonclaw: hunter, scenario step 13, quest taken'),
(4, 248834, 128808, 0, 2, 15, 0, 4, 0, 0, 0, 0, '', 'Talonclaw: hunter, scenario step 12, quest complete'),
(4, 248834, 128808, 0, 2, 22, 0, 1511, 0, 0, 0, 0, '', 'Talonclaw: hunter, scenario step 12, quest complete'),
(4, 248834, 128808, 0, 2, 44, 0, 973, 12, 0, 0, 0, '', 'Talonclaw: hunter, scenario step 12, quest complete'),
(4, 248834, 128808, 0, 2, 28, 0, 40385, 0, 0, 0, 0, '', 'Talonclaw: hunter, scenario step 12, quest complete'),
(4, 248834, 128808, 0, 3, 15, 0, 4, 0, 0, 0, 0, '', 'Talonclaw: hunter, scenario step 13, quest complete'),
(4, 248834, 128808, 0, 3, 22, 0, 1511, 0, 0, 0, 0, '', 'Talonclaw: hunter, scenario step 13, quest complete'),
(4, 248834, 128808, 0, 3, 44, 0, 973, 13, 0, 0, 0, '', 'Talonclaw: hunter, scenario step 13, quest complete'),
(4, 248834, 128808, 0, 3, 28, 0, 40385, 0, 0, 0, 0, '', 'Talonclaw: hunter, scenario step 13, quest complete');
