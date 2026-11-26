-- 13946 Nature's Reprisal: a Fel Rock mob hit by Nature's Fury (65455, Ireroot Seeds) shrinks and pops, as in the MoP 5.4.8 world DB
-- (Wowhead 2011 comment: "Such a satisfying pop sound"). Before, the action list 200200 only gave the credit and killed it with no visual.
-- 65462 Explode Demon Shrink Countdown (every 2 s 65457, scale -20%), 65467 Explode Demon Trigger Countdown (65456 instakill after 5 s).
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND ((`entryorguid` IN (2002, 2004, 2005) AND `id` = 1) OR (`entryorguid` = 2003 AND `id` = 14));
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (2002, 2003, 2004, 2005) AND `id` BETWEEN 20 AND 23;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2002, 0, 20, 21, 8,  0, 100, 0, 65455, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7,  0, 0, 0, 0, 0, 0, 0, 0, 'Rascal Sprite - On Spell Hit Nature''s Fury - Store Target List'),
(2002, 0, 21, 22, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65462, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Rascal Sprite - Linked - Cast Explode Demon Shrink Countdown'),
(2002, 0, 22, 23, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 34440, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 0, 'Rascal Sprite - Linked - Quest Credit'),
(2002, 0, 23, 0,  61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65467, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Rascal Sprite - Linked - Cast Explode Demon Trigger Countdown'),
(2003, 0, 20, 21, 8,  0, 100, 0, 65455, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7,  0, 0, 0, 0, 0, 0, 0, 0, 'Shadow Sprite - On Spell Hit Nature''s Fury - Store Target List'),
(2003, 0, 21, 22, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65462, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Shadow Sprite - Linked - Cast Explode Demon Shrink Countdown'),
(2003, 0, 22, 23, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 34440, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadow Sprite - Linked - Quest Credit'),
(2003, 0, 23, 0,  61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65467, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Shadow Sprite - Linked - Cast Explode Demon Trigger Countdown'),
(2004, 0, 20, 21, 8,  0, 100, 0, 65455, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7,  0, 0, 0, 0, 0, 0, 0, 0, 'Dark Sprite - On Spell Hit Nature''s Fury - Store Target List'),
(2004, 0, 21, 22, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65462, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Dark Sprite - Linked - Cast Explode Demon Shrink Countdown'),
(2004, 0, 22, 23, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 34440, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 0, 'Dark Sprite - Linked - Quest Credit'),
(2004, 0, 23, 0,  61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65467, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Dark Sprite - Linked - Cast Explode Demon Trigger Countdown'),
(2005, 0, 20, 21, 8,  0, 100, 0, 65455, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7,  0, 0, 0, 0, 0, 0, 0, 0, 'Vicious Grell - On Spell Hit Nature''s Fury - Store Target List'),
(2005, 0, 21, 22, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65462, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Vicious Grell - Linked - Cast Explode Demon Shrink Countdown'),
(2005, 0, 22, 23, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 34440, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 0, 'Vicious Grell - Linked - Quest Credit'),
(2005, 0, 23, 0,  61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 65467, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Vicious Grell - Linked - Cast Explode Demon Trigger Countdown');
