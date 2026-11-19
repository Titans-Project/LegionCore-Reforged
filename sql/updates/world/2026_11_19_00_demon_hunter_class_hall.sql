-- Demon Hunter spells: soul fragment mechanics of Havoc and Vengeance, Demon Spikes, Razor Spikes, Shear, Spirit Bomb and the Tier 21 Havoc bonus move to scripts; dungeon finder entrance of the Mardum scenario.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (178963,202644,203720,203783,203794,209400,210042,228532) AND `ScriptName` IN ('spell_dh_consume_soul','spell_dh_consume_soul_vengeance','spell_dh_demon_spikes','spell_dh_razor_spikes','spell_dh_shear_fragment');
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(178963,'spell_dh_consume_soul'),
(228532,'spell_dh_consume_soul'),
(202644,'spell_dh_consume_soul'),
(203794,'spell_dh_consume_soul_vengeance'),
(210042,'spell_dh_consume_soul_vengeance'),
(203720,'spell_dh_demon_spikes'),
(209400,'spell_dh_razor_spikes'),
(203783,'spell_dh_shear_fragment');
-- Demon Spikes buff is applied by the script, Demonic Infusion now casts Demon Spikes itself
DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=203720 AND `spell_effect`=203819 AND `type`=0;
UPDATE `spell_linked_spell` SET `spell_effect`=203720 WHERE `spell_trigger`=236189 AND `spell_effect`=203819 AND `type`=6;
-- Razor Spikes slow, Defensive Spikes parry and Shear fragments are handled by the scripts
DELETE FROM `spell_trigger` WHERE `spell_id`=209400 AND `spell_trigger`=210003 AND `option`=0;
DELETE FROM `spell_trigger` WHERE `spell_id`=212829 AND `spell_trigger`=212871 AND `option`=3;
DELETE FROM `spell_trigger` WHERE `spell_id` IN (203783,212827) AND `spell_trigger`=209980 AND `option`=20;
-- Spirit Bomb applies Frailty itself before the explosion
DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=247455 AND `spell_effect`=247456 AND `type`=0;
-- Betrayer's Fury (Havoc Tier 21 4P) is granted by the Eye Beam script when the channel runs out
DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=198013 AND `spell_effect`=252165 AND `type`=8;

DELETE FROM `lfg_entrances` WHERE `dungeonId` = 1094;
INSERT INTO `lfg_entrances` (`dungeonId`, `name`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES (1094, 'Mardum Scenario', 1470.44, 1411.41, 243.73, 0.0300197);
