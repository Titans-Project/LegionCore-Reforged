-- Warlock class hall: Dark Whispers, Ulthalesh, Dreadscar claim, Calydus artifact choice and An Eye for a Scepter scenario; the artifact chests give their own loot.

-- Dark Whispers: skull handoff, ritual beams and Mephistroth's approach
INSERT INTO `creature_template_movement` (`CreatureId`, `Ground`, `Flight`) VALUES (106693, 0, 1)
ON DUPLICATE KEY UPDATE `Ground` = VALUES(`Ground`), `Flight` = VALUES(`Flight`);

DELETE FROM `spell_script_names` WHERE (`spell_id` = 211703 AND `ScriptName` = 'spell_dark_whispers_ride_skull')
   OR (`spell_id` = 211704 AND `ScriptName` = 'spell_dark_whispers_despawn_skull');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(211703, 'spell_dark_whispers_ride_skull'),
(211704, 'spell_dark_whispers_despawn_skull');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 211703 AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 211703, 0, 0, 31, 0, 3, 106693, 'Dark Whispers - Reverse ride effect 0 selects only Thal''kiel');

UPDATE `smart_scripts` SET `event_type` = 1, `event_flags` = 0, `event_param1` = 500, `event_param2` = 500, `event_param3` = 1000, `event_param4` = 1000,
 `comment` = 'Dark Whispers - OOC - Maintain ritual channel'
WHERE `entryorguid` = 106698 AND `source_type` = 0 AND `id` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 106698 AND `source_type` = 0 AND `id` = 4;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `event_type`, `event_chance`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `target_type`, `comment`) VALUES
(106698, 0, 4, 4, 100, 92, 1, 212497, 1, 1, 'Dark Whispers - Aggro - Interrupt own ritual channel');

UPDATE `smart_scripts` SET `action_param2` = 16500 WHERE `entryorguid` = 106692 AND `source_type` = 0 AND `id` = 1;
UPDATE `smart_scripts` SET `action_param2` = 7750 WHERE `entryorguid` = 106692 AND `source_type` = 0 AND `id` = 2;
UPDATE `smart_scripts` SET `action_param2` = 7250, `link` = 0 WHERE `entryorguid` = 106692 AND `source_type` = 0 AND `id` = 4;
UPDATE `smart_scripts` SET `action_type` = 22, `action_param1` = 1, `action_param2` = 0, `target_type` = 1, `target_param1` = 0, `target_param2` = 0
WHERE `entryorguid` = 106692 AND `source_type` = 0 AND `id` = 3;
UPDATE `smart_scripts` SET `action_type` = 11, `action_param1` = 211703, `action_param2` = 2
WHERE `entryorguid` = 106692 AND `source_type` = 0 AND `id` = 11;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 106693 AND `source_type` = 0 AND `id` = 1;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 106692 AND `source_type` = 0 AND `id` IN (5, 7, 8, 9, 10, 12, 13, 14, 15);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `target_type`, `target_param1`, `target_param2`, `comment`) VALUES
(106692, 0, 12, 14, 27, 0, 100, 0, 0, 0, 0, 28, 211683, 0, 7, 0, 0, 'Dark Whispers - Passenger boarded - Remove binding after actual hand pickup'),
(106692, 0, 13, 0, 1, 1, 100, 500, 500, 500, 500, 11, 211703, 2, 19, 106693, 30, 'Dark Whispers - Pickup phase - Retry missing or failed boarding'),
(106692, 0, 14, 15, 61, 0, 100, 0, 0, 0, 0, 22, 2, 0, 1, 0, 0, 'Dark Whispers - Passenger boarded - End pickup retries'),
(106692, 0, 15, 0, 61, 0, 100, 0, 0, 0, 0, 28, 212497, 0, 19, 106693, 30, 'Dark Whispers - Passenger boarded - End the doomweavers ritual beams');

UPDATE `smart_scripts` SET `link` = 0, `action_type` = 80, `action_param1` = 10669200, `action_param2` = 0, `action_param3` = 2
WHERE `entryorguid` = 106692 AND `source_type` = 0 AND `id` = 6;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 10669200 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `event_type`, `event_chance`, `event_param1`, `event_param2`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `target_type`, `target_param1`, `comment`) VALUES
(10669200, 9, 0, 0, 100, 0, 0, 11, 211701, 2, 0, 1, 0, 'Dark Whispers - Summon Overfiend - Native visual and 2000ms event tick'),
(10669200, 9, 1, 0, 100, 2300, 2300, 205, 92, 50917, 0, 18, 100, 'Dark Whispers - After summon tick - Credit players'),
(10669200, 9, 2, 0, 100, 0, 0, 11, 211704, 2, 0, 1, 0, 'Dark Whispers - Departure - Despawn skull passenger'),
(10669200, 9, 3, 0, 100, 0, 0, 11, 211705, 2, 0, 1, 0, 'Dark Whispers - Departure - Native teleport visual'),
(10669200, 9, 4, 0, 100, 200, 200, 11, 211706, 2, 0, 1, 0, 'Dark Whispers - Departure - Native invisible transform after flash'),
(10669200, 9, 5, 0, 100, 2200, 2200, 41, 0, 0, 0, 1, 0, 'Dark Whispers - Departure - Allow effects before despawn');

UPDATE `smart_scripts` SET `event_type` = 1, `event_flags` = 0, `event_param1` = 500, `event_param2` = 500, `event_param3` = 1000, `event_param4` = 1000,
 `comment` = 'Dark Whispers - OOC - Retry attack on nearby player'
WHERE `entryorguid` = 106644 AND `source_type` = 0 AND `id` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 106644 AND `source_type` = 0 AND `id` = 1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `event_type`, `event_chance`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `target_type`, `target_param1`, `comment`) VALUES
(106644, 0, 1, 1, 100, 500, 500, 1000, 1000, 205, 92, 50917, 18, 50, 'Dark Whispers - OOC - Recover missed introduction credit on player return');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` IN (1, 2) AND `SourceEntry` = 106644 AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(22, 1, 106644, 0, 0, 44, 1, 1097, 1, 'Dark Whispers - Attack retry only during the Overfiend stage'),
(22, 2, 106644, 0, 0, 44, 1, 1097, 0, 'Dark Whispers - Missed introduction credit only at the initial step');

UPDATE `creature_text` SET `Type` = 14
WHERE `CreatureID` = 106692 AND `GroupID` = 0 AND `ID` = 0 AND `BroadcastTextID` = 110315;

UPDATE `smart_scripts` SET `link` = 5
WHERE `entryorguid` = 106698 AND `source_type` = 0 AND `id` = 3 AND `action_type` = 12 AND `action_param1` = 106692;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 106698 AND `source_type` = 0 AND `id` = 5;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `event_type`, `event_chance`, `action_type`, `action_param1`, `target_type`, `target_param1`, `target_param2`, `comment`) VALUES
(106698, 0, 5, 61, 100, 28, 211683, 19, 106693, 50, 'Dark Whispers - Last doomweaver death - Remove skull binding before approach');

-- Dark Whispers: both leaders and the doomweavers are judged from the dying actor, and pet kills still credit the owner
UPDATE `conditions` SET `ConditionTarget` = 1
WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 0 AND `ConditionTypeOrReference` = 29 AND `NegativeCondition` = 1
 AND ((`SourceEntry` = 106825 AND `SourceGroup` = 4 AND `ConditionValue1` = 106827)
   OR (`SourceEntry` = 106827 AND `SourceGroup` = 3 AND `ConditionValue1` = 106825)
   OR (`SourceEntry` = 106698 AND `SourceGroup` = 4 AND `ConditionValue1` = 106698));
UPDATE `smart_scripts` SET `target_type` = 18, `target_param1` = 100, `target_param2` = 0, `target_param3` = 0,
 `comment` = 'Dark Whispers - Last leader death - Credit nearby players including pet owners'
WHERE `source_type` = 0 AND ((`entryorguid` = 106825 AND `id` = 3) OR (`entryorguid` = 106827 AND `id` = 2))
 AND `action_type` = 205 AND `action_param1` = 92 AND `action_param2` = 50959;
UPDATE `smart_scripts` SET `action_type` = 11, `action_param2` = 18, `target_type` = 18, `target_param1` = 100, `target_param2` = 0, `target_param3` = 0,
 `comment` = 'Dark Whispers - Last leader death - Player stage5 conversation independent of pet killer'
WHERE `source_type` = 0 AND ((`entryorguid` = 106825 AND `id` = 5) OR (`entryorguid` = 106827 AND `id` = 4))
 AND `action_type` IN (85, 11) AND `action_param1` = 212550;

-- Dark Whispers: the barrier event is admitted for players only
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceEntry` = 99739 AND `SourceId` = 0
    AND `SourceGroup` = 2 AND `Comment` = 'Dark Whispers - barrier requires Player invoker';
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(22, 2, 99739, 0, 0, 31, 0, 4, 0, 'Dark Whispers - barrier requires Player invoker');

-- Dark Whispers: Mephistroth finale cannot be skipped by a heavy hit and only runs in stage 5
UPDATE `smart_scripts` SET `link` = 0 WHERE `entryorguid` = 106652 AND `source_type` = 0 AND `id` = 0 AND `action_type` = 1;
UPDATE `smart_scripts` SET `event_type` = 0, `event_param1` = 0, `event_param2` = 0, `event_param3` = 1000, `event_param4` = 1000,
    `target_type` = 21, `target_param1` = 100, `target_param2` = 0, `comment` = 'Dark Whispers - combat ritual credit to nearby player'
WHERE `entryorguid` = 106652 AND `source_type` = 0 AND `id` = 2 AND `action_type` = 205 AND `action_param2` = 50961;
UPDATE `smart_scripts` SET `link` = 0, `action_type` = 11, `action_param1` = 152821, `action_param2` = 0, `target_type` = 1,
    `comment` = 'Dark Whispers - optional low-health retreat visual'
WHERE `entryorguid` = 106652 AND `source_type` = 0 AND `id` = 3 AND `event_type` = 2;
UPDATE `smart_scripts` SET `event_type` = 2, `event_param1` = 0, `event_param2` = 8, `event_param3` = 0, `event_param4` = 0,
    `action_type` = 1, `action_param1` = 1, `action_param2` = 0, `target_type` = 21, `target_param1` = 100, `target_param2` = 0,
    `comment` = 'Dark Whispers - independent low-health defeat dialogue'
WHERE `entryorguid` = 106652 AND `source_type` = 0 AND `id` = 4 AND `link` = 5;
UPDATE `smart_scripts` SET `target_type` = 21, `target_param1` = 100, `target_param2` = 0
WHERE `entryorguid` = 106652 AND `source_type` = 0 AND `id` = 5 AND `action_type` = 205 AND `action_param2` = 50962;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 106652 AND `source_type` = 0 AND `id` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `event_type`, `event_chance`, `event_flags`, `action_type`, `action_param1`, `action_param2`, `target_type`, `comment`) VALUES
(106652, 0, 9, 4, 100, 0, 42, 0, 5, 1, 'Dark Whispers - Aggro - Cannot drop below 5% before the defeat dialogue');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceEntry` = 106652 AND `SourceId` = 0
    AND `SourceGroup` IN (3, 4, 5) AND `Comment` = 'Dark Whispers - finale only in stage 5';
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(22, 3, 106652, 0, 0, 44, 1, 1097, 4, 'Dark Whispers - finale only in stage 5'),
(22, 4, 106652, 0, 0, 44, 1, 1097, 4, 'Dark Whispers - finale only in stage 5'),
(22, 5, 106652, 0, 0, 44, 1, 1097, 4, 'Dark Whispers - finale only in stage 5');

-- Ritual Reagents: a single blood sample
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 19586 AND `SourceEntry` IN (0, 1)
 AND `SourceId` = 0 AND `ElseGroup` = 0 AND `ConditionTypeOrReference` = 2 AND `ConditionValue1` = 136986;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 1 AND `SourceEntry` = 106581 AND `SourceId` = 0
 AND `ElseGroup` = 0 AND ((`ConditionTypeOrReference` = 2 AND `ConditionValue1` = 136986)
 OR (`ConditionTypeOrReference` = 9 AND `ConditionValue1` = 42128));
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES
(15, 19586, 0, 0, 0, 2, 0, 136986, 1, 1, 1, 'Ritual Reagents - Hide collection when sample already owned'),
(15, 19586, 1, 0, 0, 2, 0, 136986, 1, 1, 1, 'Ritual Reagents - Hide collection when sample already owned'),
(22, 1, 106581, 0, 0, 9, 0, 42128, 0, 0, 0, 'Ritual Reagents - Grant sample only during quest'),
(22, 1, 106581, 0, 0, 2, 0, 136986, 1, 1, 1, 'Ritual Reagents - Grant at most one sample');

-- Ulthalesh: Ariden's defeat dialogue no longer depends on the optional cast, the chest waits for its own step, the compass drops its quest item
UPDATE `gameobject_template` SET `Data1` = 245794
WHERE `entry` = 245794 AND `type` = 3 AND `Data1` IN (0, 245794) AND `ScriptName` = '';
UPDATE `smart_scripts` SET `link` = 0 WHERE `entryorguid` = 100344 AND `source_type` = 0 AND `id` = 10 AND `action_type` = 11 AND `action_param1` = 199764;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 100344 AND `source_type` = 0 AND `id` = 90 AND `comment` = 'Ulthalesh - independent low-health finale';
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `Difficulties`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
SELECT `entryorguid`, `source_type`, 90, 12, `Difficulties`, 2, `event_phase_mask`, `event_chance`, `event_flags`, 0, 1, 0, 0, 0, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, 'Ulthalesh - independent low-health finale'
FROM `smart_scripts` WHERE `entryorguid` = 100344 AND `source_type` = 0 AND `id` = 11 AND `action_type` = 1 AND `action_param1` = 3;
UPDATE `smart_scripts` SET `link` = 0 WHERE `entryorguid` = 100344 AND `source_type` = 0 AND `id` = 12 AND `action_type` = 21;
UPDATE `smart_scripts` SET `link` = 15 WHERE `entryorguid` = 100344 AND `source_type` = 0 AND `id` = 14 AND `action_type` = 205 AND `action_param2` = 47989;
UPDATE `smart_scripts` SET `link` = 16 WHERE `entryorguid` = 100344 AND `source_type` = 0 AND `id` = 15 AND `action_type` = 50 AND `action_param1` = 246021;
UPDATE `smart_scripts` SET `event_type` = 37 WHERE `entryorguid` = 100344 AND `source_type` = 0 AND `id` = 9 AND `action_type` = 42 AND `action_param1` = 1 AND `event_type` IN (37, 60);

-- Dreadscar Rift: scouting map stays on the floor, Ritssyn's greeting, return to Dreadscar and Jagganoth's pet kill
UPDATE `creature_template_movement` SET `Ground` = 0, `Swim` = 0, `Flight` = 1, `Rooted` = 1
WHERE `CreatureId` = 101979;

UPDATE `creature_text`
SET `Text` = 'A moment of your time, warlock. The Council of the Black Harvest requests your assistance.',
    `BroadcastTextID` = 106111, `Sound` = 71324
WHERE `CreatureID` = 103506 AND `GroupID` = 0 AND `ID` = 0 AND `BroadcastTextID` = 105637;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Sound`, `BroadcastTextID`, `comment`)
SELECT 103506, 1, 0, 'Join us at the Circle of Wills in Dalaran''s underbelly, warlock. My portal will take you there directly. We will be waiting.',
 12, 0, 100, 63919, 104179, 'Ritssyn - The Sixth accepted (native BroadcastText)'
WHERE NOT EXISTS (SELECT 1 FROM `creature_text` WHERE `CreatureID` = 103506 AND `GroupID` = 1);

UPDATE `quest_template_addon` SET `PrevQuestID` = 40729 WHERE `ID` = 40731 AND `PrevQuestID` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 19 AND `SourceGroup` = 0
 AND `SourceEntry` = 42125 AND `SourceId` = 0 AND `ConditionTypeOrReference` = 8
 AND `ConditionTarget` = 0 AND `NegativeCondition` = 0 AND `ConditionValue2` = 0 AND `ConditionValue3` = 0
 AND ((`ElseGroup` = 0 AND `ConditionValue1` = 42125) OR (`ElseGroup` = 1 AND `ConditionValue1` = 40712)
 OR (`ElseGroup` = 2 AND `ConditionValue1` = 43254));
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 19 AND `SourceGroup` = 0 AND `SourceEntry` = 40731 AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES
(19, 0, 40731, 0, 0, 8, 0, 40712, 0, 0, 0, 'Dreadscar - Affliction artifact returned'),
(19, 0, 40731, 0, 1, 8, 0, 41156, 0, 0, 0, 'Dreadscar - alternate Affliction return'),
(19, 0, 40731, 0, 2, 8, 0, 42125, 0, 0, 0, 'Dreadscar - Demonology artifact returned'),
(19, 0, 40731, 0, 3, 8, 0, 43254, 0, 0, 0, 'Dreadscar - Destruction artifact returned');

UPDATE `smart_scripts` SET `target_type` = 18, `target_param1` = 100,
 `comment` = 'Jagganoth death - scenario credit to nearby players, including pet kills'
WHERE `entryorguid` = 101468 AND `source_type` = 0 AND `id` = 4 AND `event_type` = 6
 AND `action_type` = 205 AND `action_param1` = 92 AND `action_param2` = 48186 AND `target_type` = 7;
UPDATE `smart_scripts` SET `target_type` = 18, `target_param1` = 100
WHERE `entryorguid` = 101468 AND `source_type` = 0 AND `id` = 7 AND `event_type` = 61
 AND `action_type` = 11 AND `action_param1` = 200125 AND `target_type` = 7;

DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId` = 18 AND `SourceGroup` IN (101468, 101470) AND `SourceEntry` = 200095)
 OR (`SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 1 AND `SourceEntry` = 246148 AND `SourceId` = 1)
 OR (`SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 3 AND `SourceEntry` = 101470 AND `SourceId` = 0)
 OR (`SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 19326 AND `SourceEntry` = 2)
 OR (`SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 6 AND `SourceEntry` = 101097 AND `SourceId` = 0);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES
(18, 101468, 200095, 0, 0, 9, 0, 40731, 0, 0, 0, 'Jagganoth heart - quest active'),
(18, 101468, 200095, 0, 0, 44, 0, 998, 1, 0, 0, 'Jagganoth heart - after defeat'),
(18, 101468, 200095, 0, 0, 36, 1, 0, 0, 0, 1, 'Jagganoth heart - corpse only'),
(18, 101470, 200095, 0, 0, 9, 0, 40731, 0, 0, 0, 'Heart - quest active'),
(18, 101470, 200095, 0, 0, 44, 0, 998, 1, 0, 0, 'Heart - collection step'),
(22, 1, 246148, 1, 0, 9, 0, 40731, 0, 0, 0, 'Altar - quest active'),
(22, 1, 246148, 1, 0, 44, 0, 998, 2, 0, 0, 'Altar - final scenario step'),
(22, 1, 246148, 1, 0, 2, 0, 132880, 1, 0, 0, 'Altar - must carry the heart'),
(15, 19326, 2, 0, 0, 9, 0, 40731, 0, 0, 0, 'Dreadscar return - active quest'),
(15, 19326, 2, 0, 0, 22, 0, 1502, 0, 0, 0, 'Dreadscar return - Underbelly'),
(15, 19326, 2, 0, 1, 9, 0, 40731, 0, 0, 0, 'Dreadscar return - active quest'),
(15, 19326, 2, 0, 1, 22, 0, 1220, 0, 0, 0, 'Dreadscar return - Dalaran'),
(22, 6, 101097, 0, 0, 9, 0, 40731, 0, 0, 0, 'Dreadscar return handler - active quest'),
(22, 6, 101097, 0, 0, 22, 0, 1502, 0, 0, 0, 'Dreadscar return handler - Underbelly'),
(22, 6, 101097, 0, 1, 9, 0, 40731, 0, 0, 0, 'Dreadscar return handler - active quest'),
(22, 6, 101097, 0, 1, 22, 0, 1220, 0, 0, 0, 'Dreadscar return handler - Dalaran'),
(22, 3, 101470, 0, 0, 9, 0, 40731, 0, 0, 0, 'Heart recovery - active quest'),
(22, 3, 101470, 0, 0, 44, 0, 998, 1, 0, 0, 'Heart recovery - boss already defeated'),
(22, 3, 101470, 0, 0, 2, 0, 132880, 1, 0, 0, 'Heart recovery - item already owned');

DELETE FROM `gossip_menu_option` WHERE `MenuID` = 19326 AND `OptionID` = 2;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionNpc`, `OptionText`, `OptionBroadcastTextID`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(19326, 2, 0, 'Return to Dreadscar Rift.', 0, 0, 0, 0, 0, '', 0, 0);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 101097 AND `source_type` = 0 AND `id` = 5;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_chance`, `event_param1`, `event_param2`, `action_type`, `action_param1`, `target_type`, `comment`) VALUES
(101097, 0, 5, 3, 62, 100, 19326, 2, 204, 998, 7, 'Calydus - return to unfinished Dreadscar scenario');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 101470 AND `source_type` = 0 AND `id` = 2;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `event_type`, `event_chance`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `target_type`, `comment`) VALUES
(101470, 0, 2, 10, 100, 1, 30, 1000, 1000, 205, 36, 132880, 1, 7, 'Heart - recover existing inventory progress after re-entry');

-- Calydus: completed artifact routes are no longer offered, on the gossip, the choice itself and the map breadcrumb
DELETE FROM `conditions`
 WHERE `Comment` LIKE 'Warlock artifact choice - %'
 AND ((`SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 19326 AND `SourceEntry` = 0 AND `SourceId` = 0)
   OR (`SourceTypeOrReferenceId` = 32 AND `SourceGroup` = 0 AND `SourceEntry` = 245 AND `SourceId` = 0)
   OR (`SourceTypeOrReferenceId` = 33 AND `SourceGroup` = 0 AND `SourceEntry` IN (523, 524, 525) AND `SourceId` = 0));
DELETE FROM `conditions`
 WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 19326 AND `SourceEntry` = 0
 AND `SourceId` = 0 AND `ElseGroup` = 0 AND `ConditionTypeOrReference` = 8 AND `ConditionTarget` = 0
 AND `ConditionValue1` = 40684 AND `ConditionValue2` = 0 AND `ConditionValue3` = 0 AND `NegativeCondition` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`) VALUES
(15, 19326, 0, 0, 0, 8, 0, 40684, 0, 0, 0, 'Warlock artifact choice - Calydus introduction rewarded'),
(32, 0, 245, 0, 0, 15, 0, 256, 0, 0, 0, 'Warlock artifact choice - Warlock only'),
(15, 19326, 0, 0, 0, 8, 0, 40712, 0, 0, 1, 'Warlock artifact choice - route 523 not rewarded'),
(32, 0, 245, 0, 0, 8, 0, 40712, 0, 0, 1, 'Warlock artifact choice - route 523 not rewarded'),
(33, 0, 523, 0, 0, 8, 0, 40712, 0, 0, 1, 'Warlock artifact choice - route 523 not rewarded'),
(15, 19326, 0, 0, 0, 28, 0, 40712, 0, 0, 1, 'Warlock artifact choice - route 523 not awaiting turn-in'),
(32, 0, 245, 0, 0, 28, 0, 40712, 0, 0, 1, 'Warlock artifact choice - route 523 not awaiting turn-in'),
(33, 0, 523, 0, 0, 28, 0, 40712, 0, 0, 1, 'Warlock artifact choice - route 523 not awaiting turn-in'),
(15, 19326, 0, 0, 0, 8, 0, 41156, 0, 0, 1, 'Warlock artifact choice - route 523 not rewarded'),
(32, 0, 245, 0, 0, 8, 0, 41156, 0, 0, 1, 'Warlock artifact choice - route 523 not rewarded'),
(33, 0, 523, 0, 0, 8, 0, 41156, 0, 0, 1, 'Warlock artifact choice - route 523 not rewarded'),
(15, 19326, 0, 0, 0, 28, 0, 41156, 0, 0, 1, 'Warlock artifact choice - route 523 not awaiting turn-in'),
(32, 0, 245, 0, 0, 28, 0, 41156, 0, 0, 1, 'Warlock artifact choice - route 523 not awaiting turn-in'),
(33, 0, 523, 0, 0, 28, 0, 41156, 0, 0, 1, 'Warlock artifact choice - route 523 not awaiting turn-in'),
(15, 19326, 0, 0, 0, 8, 0, 40623, 0, 0, 1, 'Warlock artifact choice - route 523 acquisition not rewarded'),
(32, 0, 245, 0, 0, 8, 0, 40623, 0, 0, 1, 'Warlock artifact choice - route 523 acquisition not rewarded'),
(33, 0, 523, 0, 0, 8, 0, 40623, 0, 0, 1, 'Warlock artifact choice - route 523 acquisition not rewarded'),
(15, 19326, 0, 0, 0, 28, 0, 40623, 0, 0, 1, 'Warlock artifact choice - route 523 acquisition not awaiting turn-in'),
(32, 0, 245, 0, 0, 28, 0, 40623, 0, 0, 1, 'Warlock artifact choice - route 523 acquisition not awaiting turn-in'),
(33, 0, 523, 0, 0, 28, 0, 40623, 0, 0, 1, 'Warlock artifact choice - route 523 acquisition not awaiting turn-in'),
(15, 19326, 0, 0, 0, 8, 0, 41155, 0, 0, 1, 'Warlock artifact choice - route 523 acquisition not rewarded'),
(32, 0, 245, 0, 0, 8, 0, 41155, 0, 0, 1, 'Warlock artifact choice - route 523 acquisition not rewarded'),
(33, 0, 523, 0, 0, 8, 0, 41155, 0, 0, 1, 'Warlock artifact choice - route 523 acquisition not rewarded'),
(15, 19326, 0, 0, 0, 28, 0, 41155, 0, 0, 1, 'Warlock artifact choice - route 523 acquisition not awaiting turn-in'),
(32, 0, 245, 0, 0, 28, 0, 41155, 0, 0, 1, 'Warlock artifact choice - route 523 acquisition not awaiting turn-in'),
(33, 0, 523, 0, 0, 28, 0, 41155, 0, 0, 1, 'Warlock artifact choice - route 523 acquisition not awaiting turn-in'),
(15, 19326, 0, 0, 1, 8, 0, 40684, 0, 0, 0, 'Warlock artifact choice - Calydus introduction rewarded'),
(32, 0, 245, 0, 1, 15, 0, 256, 0, 0, 0, 'Warlock artifact choice - Warlock only'),
(15, 19326, 0, 0, 1, 8, 0, 42125, 0, 0, 1, 'Warlock artifact choice - route 524 not rewarded'),
(32, 0, 245, 0, 1, 8, 0, 42125, 0, 0, 1, 'Warlock artifact choice - route 524 not rewarded'),
(33, 0, 524, 0, 0, 8, 0, 42125, 0, 0, 1, 'Warlock artifact choice - route 524 not rewarded'),
(15, 19326, 0, 0, 1, 28, 0, 42125, 0, 0, 1, 'Warlock artifact choice - route 524 not awaiting turn-in'),
(32, 0, 245, 0, 1, 28, 0, 42125, 0, 0, 1, 'Warlock artifact choice - route 524 not awaiting turn-in'),
(33, 0, 524, 0, 0, 28, 0, 42125, 0, 0, 1, 'Warlock artifact choice - route 524 not awaiting turn-in'),
(15, 19326, 0, 0, 2, 8, 0, 40684, 0, 0, 0, 'Warlock artifact choice - Calydus introduction rewarded'),
(32, 0, 245, 0, 2, 15, 0, 256, 0, 0, 0, 'Warlock artifact choice - Warlock only'),
(15, 19326, 0, 0, 2, 8, 0, 43254, 0, 0, 1, 'Warlock artifact choice - route 525 not rewarded'),
(32, 0, 245, 0, 2, 8, 0, 43254, 0, 0, 1, 'Warlock artifact choice - route 525 not rewarded'),
(33, 0, 525, 0, 0, 8, 0, 43254, 0, 0, 1, 'Warlock artifact choice - route 525 not rewarded'),
(15, 19326, 0, 0, 2, 28, 0, 43254, 0, 0, 1, 'Warlock artifact choice - route 525 not awaiting turn-in'),
(32, 0, 245, 0, 2, 28, 0, 43254, 0, 0, 1, 'Warlock artifact choice - route 525 not awaiting turn-in'),
(33, 0, 525, 0, 0, 28, 0, 43254, 0, 0, 1, 'Warlock artifact choice - route 525 not awaiting turn-in');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 19 AND `SourceGroup` = 0
 AND `SourceEntry` = 44089 AND `SourceId` = 0 AND `Comment` LIKE 'Warlock artifact map - %';
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`)
SELECT 19, 0, 44089, 0, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`,
 CONCAT('Warlock artifact map - ', SUBSTRING(`Comment`, LENGTH('Warlock artifact choice - ') + 1))
FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 32 AND `SourceGroup` = 0 AND `SourceEntry` = 245
 AND `SourceId` = 0 AND `Comment` LIKE 'Warlock artifact choice - %';

INSERT IGNORE INTO `creature_queststarter` (`id`, `quest`) VALUES (101097, 44089);
UPDATE `quest_template_addon` SET `PrevQuestID` = 40684 WHERE `ID` = 43984 AND `PrevQuestID` = 0;
UPDATE `quest_template_addon` SET `PrevQuestID` = 43984 WHERE `ID` = 44089 AND `PrevQuestID` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 19 AND `SourceGroup` = 0
 AND `SourceEntry` = 43984 AND `SourceId` = 0 AND `Comment` LIKE 'Warlock arsenal breadcrumb - %';
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `Comment`)
SELECT 19, 0, 43984, 0, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`,
 CONCAT('Warlock arsenal breadcrumb - ', SUBSTRING(`Comment`, LENGTH('Warlock artifact choice - ') + 1))
FROM `conditions` WHERE `SourceTypeOrReferenceId` = 32 AND `SourceGroup` = 0 AND `SourceEntry` = 245
 AND `SourceId` = 0 AND `Comment` LIKE 'Warlock artifact choice - %';
DELETE FROM `smart_scripts` WHERE `entryorguid` = 101097 AND `source_type` = 0 AND `id` IN (30, 31)
 AND `comment` LIKE 'Warlock arsenal breadcrumb - %';
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `event_type`, `event_param1`, `action_type`, `action_param1`, `target_type`, `comment`) VALUES
(101097, 0, 30, 19, 43984, 85, 199411, 7, 'Warlock arsenal breadcrumb - second artifact accept opens choice'),
(101097, 0, 31, 19, 44089, 85, 199411, 7, 'Warlock arsenal breadcrumb - final artifact accept opens choice');

-- An Eye for a Scepter: scenario conditions inspect the base actor, credits no longer wait on optional conversations
SET @scepter_hold_entrance := (SELECT `guid` FROM `creature` WHERE `id` = 110021 AND `map` = 1630 LIMIT 1);

UPDATE `conditions` SET `ConditionTarget` = 1
WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 0 AND `SourceEntry` IN (109955, 109956, 110021)
  AND `ConditionTypeOrReference` = 44 AND `ConditionValue1` = 1155;

UPDATE `smart_scripts` SET `link` = 0, `event_flags` = 0
WHERE `source_type` = 13 AND `entryorguid` IN (601, 602) AND `id` = 0;

UPDATE `smart_scripts` SET `action_type` = 205, `action_param1` = 92, `action_param2` = 52741,
 `comment` = 'Scepter - D-Block arrival credit before optional conversation'
WHERE `source_type` = 0 AND `entryorguid` = 109956 AND `id` = 0;
UPDATE `smart_scripts` SET `action_type` = 11, `action_param1` = 218995, `action_param2` = 2,
 `comment` = 'Scepter - arrival conversation (independent of credit)'
WHERE `source_type` = 0 AND `entryorguid` = 109956 AND `id` = 1;
UPDATE `smart_scripts` SET `event_flags` = 1 WHERE `source_type` = 0 AND `entryorguid` = 109956 AND `id` IN (2, 9, 18);
UPDATE `smart_scripts` SET `target_type` = 21, `target_param1` = 60, `target_param2` = 0
WHERE `source_type` = 0 AND `entryorguid` = 109956 AND `id` IN (2, 10);
UPDATE `smart_scripts` SET `event_flags` = 1 WHERE `source_type` = 0 AND `entryorguid` = 109955 AND `id` = 5;
UPDATE `smart_scripts` SET `id` = 18 WHERE `source_type` = 0 AND `entryorguid` = 109955 AND `id` = 6 AND `event_param1` = 3;
UPDATE `smart_scripts` SET `target_type` = 21, `target_param1` = 60, `target_param2` = 0
WHERE `source_type` = 0 AND `entryorguid` = 109955 AND `id` = 12;

UPDATE `smart_scripts` SET `link` = 0, `action_type` = 53, `action_param1` = 1, `action_param2` = 10995601,
 `target_type` = 1, `target_param1` = 0, `target_param2` = 0,
 `comment` = 'Scepter - Armond defeated - approach the manifest'
WHERE `source_type` = 0 AND `entryorguid` = 109956 AND `id` = 14;
UPDATE `smart_scripts` SET `event_type` = 38, `event_flags` = 1, `event_param1` = 7, `event_param2` = 7,
 `action_type` = 1, `action_param1` = 5, `action_param2` = 10000, `link` = 0,
 `comment` = 'Scepter - manifest clicked - read then leave for Baradin Hold'
WHERE `source_type` = 0 AND `entryorguid` = 109956 AND `id` = 15;
UPDATE `smart_scripts` SET `link` = 2, `target_param2` = 60 WHERE `source_type` = 0 AND `entryorguid` = 110323 AND `id` = 1;
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI', `flags` = `flags` | 16 WHERE `entry` = 251879;
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` = 251699;

UPDATE `creature_template` SET `unit_flags` = `unit_flags` | 770 WHERE `entry` IN (109981, 110323);
UPDATE `smart_scripts` SET `link` = 31 WHERE `source_type` = 0 AND `entryorguid` = 109956 AND `id` = 13;
UPDATE `smart_scripts` SET `link` = 27 WHERE `source_type` = 0 AND `entryorguid` = 109956 AND `id` = 26;
UPDATE `smart_scripts` SET `target_param1` = 100 WHERE `source_type` = 0 AND `entryorguid` = 109956 AND `id` IN (25, 26);
UPDATE `smart_scripts` SET `action_type` = 12, `action_param1` = 110800, `action_param2` = 8,
 `target_type` = 8, `target_param1` = 0, `target_param2` = 0, `target_x` = -129.828, `target_y` = -4529.59, `target_z` = 170.275, `target_o` = 1.59769,
 `comment` = 'Scepter - create return portal before optional conversation'
WHERE `source_type` = 0 AND `entryorguid` = 109956 AND `id` = 27;
UPDATE `smart_scripts` SET `action_type` = 85, `action_param1` = 219706, `action_param2` = 2,
 `target_type` = 21, `target_param1` = 60, `target_param2` = 0, `target_x` = 0, `target_y` = 0, `target_z` = 0, `target_o` = 0,
 `comment` = 'Scepter - optional Eye retrieval conversation'
WHERE `source_type` = 0 AND `entryorguid` = 109956 AND `id` = 28;
UPDATE `smart_scripts` SET `link` = 0, `target_param2` = 100 WHERE `source_type` = 0 AND `entryorguid` = 109981 AND `id` = 2;
UPDATE `smart_scripts` SET `event_type` = 38, `event_param1` = 1, `event_param2` = 1, `target_type` = 21, `target_param1` = 60 WHERE `source_type` = 0 AND `entryorguid` = 109964 AND `id` = 2;
UPDATE `smart_scripts` SET `target_type` = 21, `target_param1` = 60 WHERE `source_type` = 0 AND `entryorguid` = 109964 AND `id` = 4;

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 13 AND `SourceEntry` IN (601, 602, 60) AND `SourceGroup` IN (1, 3);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 0 AND `SourceEntry` = 109956 AND `SourceGroup` IN (15, 16, 30, 31, 33);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 0 AND `SourceEntry` = 109981 AND `SourceGroup` IN (3, 4);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 1 AND `SourceEntry` = 251879 AND `SourceGroup` IN (1, 3);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 0 AND `SourceEntry` = 110030 AND `SourceGroup` = 2;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 0 AND `SourceEntry` = 109964 AND `SourceGroup` IN (3, 10);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 110021 AND `source_type` = 0 AND `id` IN (1, 2, 3);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 0 AND `SourceEntry` = 110021 AND `SourceGroup` IN (2, 4);
DELETE FROM `smart_scripts` WHERE `entryorguid` = -@scepter_hold_entrance AND `source_type` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 0 AND `SourceEntry` = -@scepter_hold_entrance;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 109956 AND `source_type` = 0 AND `id` IN (29, 30, 31, 32);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 109964 AND `source_type` = 0 AND `id` = 9;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 109981 AND `source_type` = 0 AND `id` IN (3, 5);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 110030 AND `source_type` = 0 AND `id` = 1;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 110323 AND `source_type` = 0 AND `id` = 2;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 251699 AND `source_type` = 1 AND `id` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 251879 AND `source_type` = 1 AND `id` IN (0, 1, 2);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 60 AND `source_type` = 13 AND `id` = 2;

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `comment`) VALUES
(60, 13, 2, 0, 91, 0, 100, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 109964, 40, 0, 'Scepter - leave Tyranis to his fate when exiting his room'),
(251699, 1, 0, 0, 64, 0, 100, 0, 0, 0, 0, 0, 204, 1155, 0, 0, 0, 0, 0, 7, 0, 0, 0, 'Scepter - select correct scenario before native portal teleport'),
(109956, 0, 29, 0, 10, 0, 100, 0, 1, 15, 1000, 1000, 45, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 'Scepter - D-Block arrival recovery near entrance guide'),
(109956, 0, 30, 0, 10, 0, 100, 0, 1, 10, 1000, 1000, 45, 3, 3, 0, 0, 0, 0, 1, 0, 0, 0, 'Scepter - start entrance dialogue after arrival'),
(109956, 0, 31, 0, 61, 0, 100, 0, 0, 0, 0, 0, 19, 770, 0, 0, 0, 0, 0, 19, 110323, 60, 0, 'Scepter - allow combat with Armond after guide reaches him'),
(109956, 0, 32, 0, 40, 0, 100, 0, 20, 10995603, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 109981, 60, 0, 'Scepter - guide in final room - activate Occultharon'),
(109981, 0, 3, 5, 38, 0, 100, 0, 1, 1, 0, 0, 19, 770, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'Scepter - remove encounter protection'),
(109981, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 60, 0, 0, 'Scepter - begin final fight with nearby player'),
(110323, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 20, 251879, 30, 0, 'Scepter - unlock manifest only after Armond dies'),
(251879, 1, 0, 1, 38, 0, 100, 0, 1, 1, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'Scepter - manifest unlocked'),
(251879, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 106, 16, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'Scepter - make manifest selectable'),
(251879, 1, 2, 0, 64, 1, 100, 0, 0, 0, 0, 0, 45, 7, 7, 0, 0, 0, 0, 19, 109956, 40, 0, 'Scepter - manifest click tells present guide to read'),
(110030, 0, 1, 0, 8, 0, 100, 0, 219203, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 109964, 40, 0, 'Scepter - chain hook clicked - release Tyranis'),
(109964, 0, 9, 0, 8, 0, 100, 0, 219203, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 'Scepter - prisoner clicked - release Tyranis');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `comment`)
SELECT -@scepter_hold_entrance, 0, 1, 2, 10, 0, 100, 0, 1, 3, 1000, 1000, 205, 92, 52900, 0, 0, 0, 0, 7, 0, 0, 0, 'Scepter - enter Baradin Hold' FROM DUAL WHERE @scepter_hold_entrance IS NOT NULL
UNION ALL
SELECT -@scepter_hold_entrance, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 85, 219338, 2, 0, 0, 0, 0, 7, 0, 0, 0, 'Scepter - native Hold interior teleport' FROM DUAL WHERE @scepter_hold_entrance IS NOT NULL
UNION ALL
SELECT -@scepter_hold_entrance, 0, 3, 0, 10, 0, 100, 0, 1, 3, 1000, 1000, 85, 219338, 2, 0, 0, 0, 0, 7, 0, 0, 0, 'Scepter - retry Hold entrance teleport after failed first cast' FROM DUAL WHERE @scepter_hold_entrance IS NOT NULL;

INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `NegativeCondition`, `Comment`) VALUES
(22, 1, 601, 13, 0, 44, 1, 1155, 0, 0, 'Scepter - base object scenario step 0'),
(22, 1, 602, 13, 0, 44, 1, 1155, 1, 0, 'Scepter - base object scenario step 1'),
(22, 1, 60, 13, 0, 44, 1, 1155, 2, 0, 'Scepter - base object scenario step 2'),
(22, 30, 109956, 0, 0, 44, 1, 1155, 0, 0, 'Scepter - base object scenario step 0'),
(22, 31, 109956, 0, 0, 44, 1, 1155, 1, 0, 'Scepter - base object scenario step 1'),
(22, 16, 109956, 0, 0, 44, 1, 1155, 7, 0, 'Scepter - base object scenario step 7'),
(22, 15, 109956, 0, 0, 44, 1, 1155, 7, 0, 'Scepter - base object scenario step 7'),
(22, 33, 109956, 0, 0, 44, 1, 1155, 9, 0, 'Scepter - base object scenario step 9'),
(22, 4, 109981, 0, 0, 44, 1, 1155, 9, 0, 'Scepter - base object scenario step 9'),
(22, 3, 109981, 0, 0, 44, 1, 1155, 9, 0, 'Scepter - base object scenario step 9'),
(22, 1, 251879, 1, 0, 44, 1, 1155, 7, 0, 'Scepter - base object scenario step 7'),
(22, 3, 251879, 1, 0, 44, 1, 1155, 7, 0, 'Scepter - base object scenario step 7'),
(22, 2, 110030, 0, 0, 44, 1, 1155, 3, 0, 'Scepter - base object scenario step 3'),
(22, 3, 109964, 0, 0, 44, 1, 1155, 3, 0, 'Scepter - base object scenario step 3'),
(22, 10, 109964, 0, 0, 44, 1, 1155, 3, 0, 'Scepter - base object scenario step 3'),
(22, 3, 60, 13, 0, 44, 1, 1155, 3, 0, 'Scepter - undecided Tyranis stage'),
(22, 3, 60, 13, 0, 29, 0, 109964, 25, 1, 'Scepter - player leaving the room, not walking towards Tyranis'),
(22, 30, 109956, 0, 0, 29, 1, 109955, 20, 0, 'Scepter - entrance guide near Nagaz'),
(22, 31, 109956, 0, 0, 29, 1, 109955, 20, 0, 'Scepter - entrance guide near Nagaz');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `NegativeCondition`, `Comment`)
SELECT 22, 2, -@scepter_hold_entrance, 0, 0, 44, 1, 1155, 8, 0, 'Scepter - Hold entrance only at step 8' FROM DUAL WHERE @scepter_hold_entrance IS NOT NULL
UNION ALL
SELECT 22, 4, -@scepter_hold_entrance, 0, 0, 44, 1, 1155, 9, 0, 'Scepter - Hold entrance teleport recovery' FROM DUAL WHERE @scepter_hold_entrance IS NOT NULL;

-- Seed of Corruption: Sow the Seeds limits the explosion targets
DELETE FROM `spell_script_names` WHERE `spell_id` = 27243 AND `ScriptName` = 'spell_warl_sow_the_seeds';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (27243, 'spell_warl_sow_the_seeds');

UPDATE `gameobject_template` SET `Data1` = `entry` WHERE `entry` IN (246021, 249821, 252054) AND `Data1` = 0;
