-- Hunter: talent and artifact spell scripts, Spitting Cobra, introduction and Trueshot Lodge interactions, Titanstrike introduction order and props, Thas'dorah objective slots; the artifact chests give their own loot.
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_hun_chimaera_shot', 'spell_hun_posthaste', 'spell_hun_farstrider', 'spell_hun_killer_cobra',
    'spell_hun_healing_shell', 'spell_hun_call_of_the_hunter', 'spell_hun_critical_focus', 'spell_hun_rapid_killing',
    'spell_hun_lock_and_load', 'spell_hun_true_aim', 'spell_hun_bullseye', 'spell_hun_feet_of_wind',
    'spell_hun_bombardment', 'spell_hun_marking_targets', 'spell_hun_legacy_of_the_windrunners',
    'spell_hun_animal_instincts', 'spell_hun_snake_hunter', 'spell_hun_throwing_axes', 'spell_hun_mortal_wounds',
    'spell_hun_way_of_the_moknathal', 'spell_hun_bird_of_prey', 'spell_hun_aspect_of_the_skylord',
    'spell_hun_eagles_bite_trait', 'spell_hun_echoes_of_ohnara', 'spell_hun_talon_strike',
    'spell_hun_fury_of_the_eagle', 'spell_hun_fury_of_the_eagle_damage', 'spell_hun_freezing_trap_expert',
    'spell_hun_tar_trap_aura', 'spell_hun_fluffy_go', 'spell_hun_t21_beast_mastery_4p');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(53209,  'spell_hun_chimaera_shot'),
(781,    'spell_hun_posthaste'),
(190925, 'spell_hun_posthaste'),
(199523, 'spell_hun_farstrider'),
(199532, 'spell_hun_killer_cobra'),
(190503, 'spell_hun_healing_shell'),
(191048, 'spell_hun_call_of_the_hunter'),
(191328, 'spell_hun_critical_focus'),
(191339, 'spell_hun_rapid_killing'),
(194595, 'spell_hun_lock_and_load'),
(199527, 'spell_hun_true_aim'),
(204089, 'spell_hun_bullseye'),
(238088, 'spell_hun_feet_of_wind'),
(35110,  'spell_hun_bombardment'),
(223138, 'spell_hun_marking_targets'),
(190852, 'spell_hun_legacy_of_the_windrunners'),
(204315, 'spell_hun_animal_instincts'),
(201078, 'spell_hun_snake_hunter'),
(200163, 'spell_hun_throwing_axes'),
(201075, 'spell_hun_mortal_wounds'),
(201082, 'spell_hun_way_of_the_moknathal'),
(224764, 'spell_hun_bird_of_prey'),
(203755, 'spell_hun_aspect_of_the_skylord'),
(203757, 'spell_hun_eagles_bite_trait'),
(238125, 'spell_hun_echoes_of_ohnara'),
(203563, 'spell_hun_talon_strike'),
(203415, 'spell_hun_fury_of_the_eagle'),
(203413, 'spell_hun_fury_of_the_eagle_damage'),
(3355,   'spell_hun_freezing_trap_expert'),
(135299, 'spell_hun_tar_trap_aura'),
(203669, 'spell_hun_fluffy_go'),
(251756, 'spell_hun_t21_beast_mastery_4p');

-- The scripts above replace these generic links and triggers (one axe only, Mongoose Bite cast on the hunter, unfiltered Animal Instincts, Frost-only Chimaera Shot)
DELETE FROM `spell_linked_spell` WHERE (`spell_trigger` = 200163 AND `spell_effect` = 200167)
   OR (`spell_trigger` = 201078 AND `spell_effect` = 190928)
   OR (`spell_trigger` = 201091 AND `spell_effect` = 190928)
   OR (`spell_trigger` = 232646 AND `spell_effect` IN (190925, 202800))
   OR (`spell_trigger` = 53209 AND `spell_effect` = 171454 AND `type` = 1);
DELETE FROM `spell_trigger` WHERE (`spell_id` = 204315 AND `spell_trigger` = 232646)
   OR (`spell_id` = 203749 AND `spell_trigger` = 109304)
   OR (`spell_id` = 203754 AND `spell_trigger` = 190925);

-- Sentinel area trigger (custom entry 14691) and the Spitting Cobra guardian AI
DELETE FROM `areatrigger_scripts` WHERE `entry` = 14691;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES (14691, 'areatrigger_hun_sentinel');

UPDATE `creature_template` SET `ScriptName` = 'npc_hun_spitting_cobra' WHERE `entry` = 104493 AND `AIName` = '' AND `ScriptName` = '';
UPDATE `pet_stats` SET `ap` = -1.4, `ap_type` = 2, `spd` = 1.4, `school_mask` = 127, `maxspdorap` = 1, `type` = 1, `haste` = 1 WHERE `entry` = 104493;

-- Hunter's Call: the credit action links to a node that does not exist; close the gossip after it
INSERT IGNORE INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_chance`, `action_type`, `target_type`, `comment`)
SELECT s.`entryorguid`, 0, 1, 0, 61, 100, 72, 7, 'Hunter introduction - after weapon information credit - close gossip'
FROM `smart_scripts` s
WHERE s.`source_type` = 0 AND s.`id` = 0 AND s.`link` = 1 AND s.`event_type` = 62 AND s.`action_type` = 33 AND s.`target_type` = 7
  AND ((s.`entryorguid` = 100810 AND s.`action_param1` = 104297)
    OR (s.`entryorguid` = 99986 AND s.`action_param1` = 104298)
    OR (s.`entryorguid` = 100171 AND s.`action_param1` = 104299));

-- Krasus' Landing hippogryph: only the active Call of the Marksman can click it; its teleport node is terminal
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 102177 AND `SourceEntry` = 201568;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 102177, 201568, 0, 0, 9, 0, 40392, 0, 0, 0, 0, '', 'Hunter marksman hippogryph - active Call of the Marksman only');
UPDATE `smart_scripts` SET `link` = 0 WHERE `entryorguid` = 102177 AND `source_type` = 0 AND `id` = 1 AND `link` = 2 AND `action_type` = 62;

-- Visage of Ohn'ahra: only the active Oath of Service can click it
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 102694 AND `SourceEntry` = 203156;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 102694, 203156, 0, 0, 9, 0, 40955, 0, 0, 0, 0, '', 'Hunter oath statue - active Oath of Service only');

-- Titanstrike introduction: Grif starts his introduction before the optional conversation, Prustaga gives the meeting credit before Thorim's Bolt
UPDATE `smart_scripts` SET `link` = 3, `action_type` = 80, `action_param1` = 104904, `action_param2` = 2, `target_type` = 1,
    `comment` = 'Grif - readiness - start native introduction before optional conversation'
WHERE `entryorguid` = 104904 AND `source_type` = 0 AND `id` = 0 AND `link` = 2 AND `action_type` = 11 AND `action_param1` = 210494;
UPDATE `smart_scripts` SET `link` = 0, `action_type` = 11, `action_param1` = 210494, `action_param2` = 0, `target_type` = 7,
    `comment` = 'Grif - introduction started - optional readiness conversation'
WHERE `entryorguid` = 104904 AND `source_type` = 0 AND `id` = 2 AND `link` = 3 AND `action_type` = 80 AND `action_param1` = 104904;
UPDATE `smart_scripts` SET `link` = 2
WHERE `entryorguid` = 104904 AND `source_type` = 0 AND `id` = 4 AND `link` = 0 AND `action_type` = 81;

UPDATE `smart_scripts` SET `link` = 3, `action_type` = 205, `action_param1` = 92, `action_param2` = 49879, `target_type` = 21, `target_param1` = 50,
    `comment` = 'Prustaga - actual introduction waypoint - meet credit'
WHERE `entryorguid` = 104949 AND `source_type` = 0 AND `id` = 1 AND `link` = 2 AND `event_type` = 40 AND `action_type` = 11 AND `action_param1` = 187207;
UPDATE `smart_scripts` SET `link` = 0, `action_type` = 11, `action_param1` = 187207, `action_param2` = 0, `target_type` = 19, `target_param1` = 106302,
    `comment` = 'Prustaga - introduction completed - Thorims Bolt'
WHERE `entryorguid` = 104949 AND `source_type` = 0 AND `id` = 2 AND `link` = 3 AND `action_type` = 205 AND `action_param1` = 92 AND `action_param2` = 49879;
UPDATE `smart_scripts` SET `link` = 2
WHERE `entryorguid` = 104949 AND `source_type` = 0 AND `id` = 4 AND `link` = 0 AND `action_type` = 45;

-- Titanstrike props use the invisible anchor display (its kit attaches the rifle) and stay at their summon height
UPDATE `creature_template_wdb` SET `Displayid1` = 69591, `Displayid2` = 0
WHERE `Entry` IN (106428, 106745) AND `Displayid1` = 42428 AND `Displayid2` = 69591 AND `Displayid3` = 0 AND `Displayid4` = 0;
INSERT INTO `creature_template_movement` (`CreatureId`, `Ground`, `Flight`) VALUES (106428, 0, 1), (106745, 0, 1)
ON DUPLICATE KEY UPDATE `Ground` = VALUES(`Ground`), `Flight` = VALUES(`Flight`);

-- Titanstrike finale, aftermath and restoration conversations: the first actor lifetime ended before their last lines
UPDATE `conversation_actor` SET `duration` = 15000 WHERE `entry` = 2147 AND `id` = 0 AND `duration` = 3550;
UPDATE `conversation_actor` SET `duration` = 50000 WHERE `entry` = 2167 AND `id` = 0 AND `duration` = 9400;
UPDATE `conversation_actor` SET `duration` = 55000 WHERE `entry` = 2173 AND `id` = 0 AND `duration` = 3700;

-- Call of the Marksman: Thas'dorah objectives shared storage index 4 and left index 0 unused
UPDATE `quest_objectives` SET `StorageIndex` = CASE `ID` WHEN 281309 THEN 0 WHEN 281310 THEN 1 WHEN 281311 THEN 2 WHEN 281574 THEN 3 WHEN 281980 THEN 5 END
WHERE `QuestID` = 40419 AND (`ID`, `StorageIndex`) IN ((281309, 1), (281310, 2), (281311, 3), (281574, 5), (281980, 4));

UPDATE `gameobject_template` SET `Data1` = `entry` WHERE `entry` IN (248834, 249812) AND `Data1` = 0;
