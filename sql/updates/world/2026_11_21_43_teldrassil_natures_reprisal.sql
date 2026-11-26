-- 13946 Nature's Reprisal: Ireroot Seeds (Nature's Fury 65455) give the grellkin credit (34440) on all four Fel Rock mobs.
-- Dark Sprite (2004) and Vicious Grell (2005) had no spell hit script, and the spell had no target conditions
-- (TDB 735.00, Cata 4.3.4 TDB, MoP 5.4.8: conditions 2002-2005, credit on spell hit).
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 65455;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 65455, 0, 0, 31, 0, 3, 2002, 0, 0, 0, '', 'Nature''s Fury can hit only Rascal Sprite'),
(13, 1, 65455, 0, 1, 31, 0, 3, 2003, 0, 0, 0, '', 'Nature''s Fury can hit only Shadow Sprite'),
(13, 1, 65455, 0, 2, 31, 0, 3, 2004, 0, 0, 0, '', 'Nature''s Fury can hit only Dark Sprite'),
(13, 1, 65455, 0, 3, 31, 0, 3, 2005, 0, 0, 0, '', 'Nature''s Fury can hit only Vicious Grell');

-- same spell hit row as Rascal Sprite (2002 id 1): run action list 200200 (credit 34440, then die)
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (2004, 2005) AND `id` = 1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `Difficulties`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
SELECT e.`entry`, s.`source_type`, 1, s.`link`, s.`Difficulties`, s.`event_type`, s.`event_phase_mask`, s.`event_chance`, s.`event_flags`, s.`event_param1`, s.`event_param2`, s.`event_param3`, s.`event_param4`, s.`event_param5`, s.`action_type`, s.`action_param1`, s.`action_param2`, s.`action_param3`, s.`action_param4`, s.`action_param5`, s.`action_param6`, s.`target_type`, s.`target_param1`, s.`target_param2`, s.`target_param3`, s.`target_param4`, s.`target_x`, s.`target_y`, s.`target_z`, s.`target_o`, e.`comment`
FROM `smart_scripts` s
JOIN (SELECT 2004 AS `entry`, 'Dark Sprite - On Spell Hit Nature''s Fury - Run Action List' AS `comment`
      UNION ALL SELECT 2005, 'Vicious Grell - On Spell Hit Nature''s Fury - Run Action List') e
WHERE s.`entryorguid` = 2002 AND s.`source_type` = 0 AND s.`id` = 1;
