-- Hunter, Thas'dorah scenario: the pickup scene equips the bow, hunter pets no longer swallow credits and conversations, signals wait for their step, dialogue lifetimes, Orestes farewell and exit door, return gossip, loot admission, resume entrance

-- The pickup scene equips the looted bow when it completes
UPDATE `spell_scene` SET `ScriptName` = 'scene_marksmanship_equipment' WHERE `MiscValue` = 1169;
DELETE FROM `spell_scene_event` WHERE `MiscValue` = 1169 AND `Event` = 'complete';
INSERT INTO `spell_scene_event` (`MiscValue`, `Event`, `trigerSpell`, `MonsterCredit`, `comment`) VALUES
(1169, 'complete', 200262, 0, 'Thas''dorah scene completion equips owned artifact');

-- Only a hunter on the quest can take the bow out of the chest
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 4 AND `SourceGroup` = 248419 AND `SourceEntry` = 128826;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(4, 248419, 128826, 0, 0, 15, 0, 4, 0, 0, 0, 0, '', 'Thas''dorah chest: hunter'),
(4, 248419, 128826, 0, 0, 9, 0, 40419, 0, 0, 0, 0, '', 'Thas''dorah chest: quest taken'),
(4, 248419, 128826, 0, 1, 15, 0, 4, 0, 0, 0, 0, '', 'Thas''dorah chest: hunter'),
(4, 248419, 128826, 0, 1, 28, 0, 40419, 0, 0, 0, 0, '', 'Thas''dorah chest: quest complete');

-- Credits and conversations triggered by a death or an aggro go to the player behind the pet or guardian that struck
UPDATE `smart_scripts` SET `target_type` = 130, `target_param1` = 0 WHERE `source_type` = 0 AND `target_type` = 7 AND (
    (`entryorguid` = 100394 AND `id` = 0 AND `action_type` = 206)
 OR (`entryorguid` = 100749 AND `id` = 4 AND `action_type` = 206)
 OR (`entryorguid` = 100836 AND `id` IN (6, 8) AND `action_type` = 206)
 OR (`entryorguid` = 100836 AND `id` = 3 AND `action_type` = 205)
 OR (`entryorguid` = 101269 AND `id` = 10 AND `action_type` = 205));

-- Credits that look for the nearest player cover the whole area, so a lagging player still gets them
UPDATE `smart_scripts` SET `target_param1` = 500 WHERE `target_type` = 21 AND `target_param1` = 50 AND (
    (`entryorguid` = 100397 AND `source_type` = 0 AND `id` IN (8, 49, 51))
 OR (`entryorguid` = 100398 AND `source_type` = 9 AND `id` = 0));

-- One-shot signals are spent on their first tick: each waits for its step (and for the Herald to be near)
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 0 AND (
    (`SourceGroup` = 4 AND `SourceEntry` = 100398)
 OR (`SourceGroup` = 26 AND `SourceEntry` = 100397)
 OR (`SourceGroup` = 1 AND `SourceEntry` IN (100836, 101269)));
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 4, 100398, 0, 0, 44, 1, 972, 2, 0, 0, 0, '', 'Orestes is found only during Search for Your Allies'),
(22, 26, 100397, 0, 0, 44, 1, 972, 4, 0, 0, 0, '', 'Vereesa signals the Herald only during Quiet the Herald'),
(22, 26, 100397, 0, 0, 29, 1, 100836, 50, 0, 0, 0, '', 'Vereesa signals the Herald only once it is near'),
(22, 1, 100836, 0, 0, 44, 1, 972, 4, 0, 0, 0, '', 'Herald challenge belongs to Quiet the Herald'),
(22, 1, 101269, 0, 0, 44, 1, 972, 5, 0, 0, 0, '', 'Qormaladon calls Vereesa only during End the Inquisition');

-- Conversation lifetimes: the first speaker used to vanish before the last lines (farewell, interrogation, Qormaladon)
UPDATE `conversation_creature` SET `duration` = 40000 WHERE `entry` = 1072 AND `id` = 0 AND `duration` = 2650;
UPDATE `conversation_actor` SET `duration` = 27000 WHERE `entry` = 1033 AND `id` = 0 AND `duration` = 9000;
UPDATE `conversation_actor` SET `duration` = 10000 WHERE `entry` = 1556 AND `id` = 0 AND `duration` = 3450;

-- Orestes: credit after the farewell, then the exit door opens (it used to open at Torvis' death, before the step could end), then he leaves
UPDATE `smart_scripts` SET `event_param1` = 40000, `event_param2` = 40000
WHERE `entryorguid` = 100398 AND `source_type` = 9 AND `id` = 0 AND `event_param1` = 10000;
UPDATE `smart_scripts` SET `action_type` = 9, `target_type` = 15, `target_param1` = 249740, `target_param2` = 100,
    `comment` = 'TS - Activate Gameobject'
WHERE `entryorguid` = 100398 AND `source_type` = 9 AND `id` = 1 AND `action_type` = 37;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 100398 AND `source_type` = 9 AND `id` = 2;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_chance`, `event_flags`, `action_type`, `target_type`, `comment`) VALUES
(100398, 9, 2, 0, 0, 100, 1, 37, 1, 'TS - Die');
UPDATE `smart_scripts` SET `link` = 4 WHERE `entryorguid` = 100749 AND `source_type` = 0 AND `id` = 2 AND `link` = 3;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 100749 AND `source_type` = 0 AND `id` = 3 AND `action_type` = 9;

-- Vereesa's way out: every member of the group can use it, and only a hunter who holds the bow (or another class) is offered it
UPDATE `smart_scripts` SET `event_flags` = `event_flags` & ~1
WHERE `entryorguid` = 100397 AND `source_type` = 0 AND `event_type` = 62 AND `event_param1` = 19331 AND `link` = 56;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 19331 AND `SourceEntry` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 19331, 0, 0, 0, 2, 0, 128826, 1, 0, 0, 0, '', 'Vereesa leaves Niskara: Thas''dorah in hand'),
(15, 19331, 0, 0, 1, 15, 0, 4, 0, 0, 1, 0, '', 'Vereesa leaves Niskara: not a hunter');

-- Logging back in inside the scenario returns the player to its entrance
DELETE FROM `lfg_entrances` WHERE `dungeonId` = 1162;
INSERT INTO `lfg_entrances` (`dungeonId`, `name`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
(1162, 'Legacy of the Windrunners', 234.68, 1950.91, -55.89, 0.282743);
