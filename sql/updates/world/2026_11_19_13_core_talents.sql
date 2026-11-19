-- Mage Fire: Hot Streak consumes Heating Up whichever way it was gained (Controlled Burn).
DELETE FROM `spell_script_names` WHERE `spell_id` = 48108 AND `ScriptName` = 'spell_mage_hot_streak';
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES (48108,'spell_mage_hot_streak');
