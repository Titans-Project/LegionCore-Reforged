-- Warrior class hall: artifact equipped when its scene ends, Strom'kar chest loot, War Machine script, Odyn artifact choice conditions

-- The three acquisition scenes equip the weapon when they complete (Warswords, Scale of the Earth-Warder, Strom'kar)
DELETE FROM `spell_scene_event` WHERE `MiscValue` IN (1215, 1218, 1225) AND `Event` = 'complete';
INSERT INTO `spell_scene_event` (`MiscValue`, `Event`, `trigerSpell`, `MonsterCredit`, `comment`) VALUES
(1215, 'complete', 208562, 0, 'Warrior Protection artifact equipped'),
(1218, 'complete', 194201, 0, 'Warrior Fury artifact equipped'),
(1225, 'complete', 206439, 0, 'Warrior Arms artifact equipped');

-- The Strom'kar chest had no loot id, so the blade was never given
UPDATE `gameobject_template` SET `Data1` = 247877 WHERE `entry` = 247877 AND `Data1` = 0;

-- War Machine: the client proc only marks the dead enemy, the script applies the real buff
DELETE FROM `spell_script_names` WHERE `spell_id` = 215556 AND `ScriptName` = 'spell_warr_war_machine';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (215556, 'spell_warr_war_machine');

-- Odyn offers only the artifacts whose acquisition quest is not finished (needs the choice filter in Player::SendDisplayPlayerChoice)
DELETE FROM `conditions` WHERE `SourceGroup` = 0 AND `SourceId` = 0
    AND ((`SourceTypeOrReferenceId` = 32 AND `SourceEntry` = 236)
      OR (`SourceTypeOrReferenceId` = 33 AND `SourceEntry` IN (491, 492, 493)))
    AND `Comment` LIKE 'Warrior artifact choice - %';
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(32, 0, 236, 0, 0, 15, 0, 1, 0, 0, 0, 0, '', 'Warrior artifact choice - warrior'),
(32, 0, 236, 0, 0, 27, 0, 98, 3, 0, 0, 0, '', 'Warrior artifact choice - level 98'),
(32, 0, 236, 0, 0, 9, 0, 40579, 0, 0, 0, 0, '', 'Warrior artifact choice - first choice quest taken'),
(32, 0, 236, 0, 1, 15, 0, 1, 0, 0, 0, 0, '', 'Warrior artifact choice - warrior'),
(32, 0, 236, 0, 1, 27, 0, 98, 3, 0, 0, 0, '', 'Warrior artifact choice - level 98'),
(32, 0, 236, 0, 1, 28, 0, 40579, 0, 0, 0, 0, '', 'Warrior artifact choice - first choice quest complete'),
(32, 0, 236, 0, 2, 15, 0, 1, 0, 0, 0, 0, '', 'Warrior artifact choice - warrior'),
(32, 0, 236, 0, 2, 27, 0, 98, 3, 0, 0, 0, '', 'Warrior artifact choice - level 98'),
(32, 0, 236, 0, 2, 8, 0, 40579, 0, 0, 0, 0, '', 'Warrior artifact choice - first choice quest rewarded'),
(33, 0, 491, 0, 0, 8, 0, 41105, 0, 0, 1, 0, '', 'Warrior artifact choice - Arms quest not rewarded'),
(33, 0, 491, 0, 0, 28, 0, 41105, 0, 0, 1, 0, '', 'Warrior artifact choice - Arms quest not awaiting turn-in'),
(33, 0, 492, 0, 0, 8, 0, 39191, 0, 0, 1, 0, '', 'Warrior artifact choice - Protection quest not rewarded'),
(33, 0, 492, 0, 0, 28, 0, 39191, 0, 0, 1, 0, '', 'Warrior artifact choice - Protection quest not awaiting turn-in'),
(33, 0, 493, 0, 0, 8, 0, 40043, 0, 0, 1, 0, '', 'Warrior artifact choice - Fury quest not rewarded'),
(33, 0, 493, 0, 0, 28, 0, 40043, 0, 0, 1, 0, '', 'Warrior artifact choice - Fury quest not awaiting turn-in');
