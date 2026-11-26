-- 9066 Swift Discipline: Apprentice Meledor says his line when hit by the Disciplinary Rod (Cata 4.3.4 / Cata Classic 4.4.2 TDB)
DELETE FROM `creature_text` WHERE `CreatureID`=15945 AND `GroupID`=0;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`MinTimer`,`MaxTimer`,`SpellID`,`comment`) VALUES
(15945,0,0,'You can''t do this to me!  We had a deal!',12,0,100,0,0,0,12014,0,0,0,'Apprentice Meledor - Disciplinary Rod');

UPDATE `smart_scripts` SET `link`=1 WHERE `entryorguid`=15945 AND `source_type`=0 AND `id`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=15945 AND `source_type`=0 AND `id`=1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`target_type`,`comment`) VALUES
(15945,0,1,0,61,0,100,0,0,0,0,0,1,0,1,'Apprentice Meledor - On Spellhit Disciplinary Rod - Say Line 0');
