-- Mulgore 14436 "Dwarven Digging": removed in Cataclysm (Wowhead comments), no giver in Cata Classic 4.4.2.
DELETE FROM `disables` WHERE `sourceType`=1 AND `entry`=14436;
INSERT INTO `disables` (`sourceType`,`entry`,`flags`,`params_0`,`params_1`,`comment`) VALUES
(1,14436,0,'','','Removed in Cataclysm: Dwarven Digging');
