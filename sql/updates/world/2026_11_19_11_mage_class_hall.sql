-- Mage class hall and artifact scenarios: Lyandra's fight, artifact pickups, Cinderstorm bonus; the artifact chests give their own loot.

-- Lyandra Sunstrider: the 80% and 20% casts were stored as combat timers and fired at once, the phase's Awaken is cast
-- while she is stunned by the phase itself (triggered), and two links pointed at rows that do not exist
UPDATE `smart_scripts` SET `event_type` = 2 WHERE `entryorguid` = 99615 AND `source_type` = 0 AND `id` IN (15, 16) AND `event_type` = 0;
UPDATE `smart_scripts` SET `action_param2` = 2 WHERE `entryorguid` = 99615 AND `source_type` = 0 AND `id` = 19 AND `action_type` = 11 AND `action_param1` = 195739 AND `action_param2` = 64;
UPDATE `smart_scripts` SET `link` = 0 WHERE `entryorguid` = 99615 AND `source_type` = 0 AND `id` = 21 AND `link` = 22;
UPDATE `smart_scripts` SET `link` = 0 WHERE `entryorguid` = 99615 AND `source_type` = 0 AND `id` = 31 AND `link` = 32;

-- Deaths that give scenario credit, a conversation or Nexus energy to the killer: a pet or guardian kill hands it to the owner
UPDATE `creature_template` SET `ScriptName` = 'npc_fire_artifact_lyandra' WHERE `entry` = 99615 AND `ScriptName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_mage_artifact_kill_credit' WHERE `entry` IN (98760, 106569, 106708, 108168) AND `ScriptName` = '';

-- Felo'melorn: the chest holds the weapon but its loot id was never set, and the scene does not equip it when it ends
UPDATE `gameobject_template` SET `Data1` = 247494 WHERE `entry` = 247494 AND `Data1` = 0;

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 4 AND `SourceGroup` = 247494 AND `SourceEntry` = 128820;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(4, 247494, 128820, 0, 0, 2, 0, 128820, 1, 1, 1, 0, '', 'Felo''melorn chest: not already owned'),
(4, 247494, 128820, 0, 0, 9, 0, 11997, 0, 0, 0, 0, '', 'Felo''melorn chest: quest taken'),
(4, 247494, 128820, 0, 0, 15, 0, 128, 0, 0, 0, 0, '', 'Felo''melorn chest: mage'),
(4, 247494, 128820, 0, 0, 22, 0, 1480, 0, 0, 0, 0, '', 'Felo''melorn chest: scenario map');

INSERT INTO `spell_scene_event` (`MiscValue`, `Event`, `trigerSpell`, `MonsterCredit`, `comment`) VALUES
(1125, 'complete', 196020, 0, 'Felo''melorn: equip the weapon when the scene ends')
ON DUPLICATE KEY UPDATE `trigerSpell` = VALUES(`trigerSpell`), `MonsterCredit` = VALUES(`MonsterCredit`), `comment` = VALUES(`comment`);

-- Aluneth: same missing loot id on the Nexus chest
UPDATE `gameobject_template` SET `Data1` = 249814 WHERE `entry` = 249814 AND `Data1` = 0;

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 4 AND `SourceGroup` = 249814 AND `SourceEntry` = 127857;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(4, 249814, 127857, 0, 0, 2, 0, 127857, 1, 1, 1, 0, '', 'Aluneth chest: not already owned'),
(4, 249814, 127857, 0, 0, 9, 0, 42011, 0, 0, 0, 0, '', 'Aluneth chest: quest taken'),
(4, 249814, 127857, 0, 0, 15, 0, 128, 0, 0, 0, 0, '', 'Aluneth chest: mage'),
(4, 249814, 127857, 0, 0, 22, 0, 1583, 0, 0, 0, 0, '', 'Aluneth chest: scenario map');

-- Cinderstorm: cinders hit harder on a target carrying the caster's Ignite
INSERT IGNORE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(198928, 'spell_mage_cinderstorm_damage');

UPDATE `gameobject_template` SET `Data1` = `entry` WHERE `entry` IN (251049) AND `Data1` = 0;
