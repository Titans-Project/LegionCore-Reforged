-- Dun Morogh, Rejold Barleybrew / Marleth Barleybrew chain 317 -> 318 -> 319 -> 320 was removed in Cataclysm.
-- 317 "Stocking Jetsteam" is already disabled ("Deprecated quest"). 318 "Evershine", 319 "A Favor for Evershine" and
-- 320 "Return to Bellowfiz" stayed offered here; 319 needs Elder Crag Boar 1127, which has no spawn in this DB,
-- the Cata 4.3.4 TDB or the MoP 5.4.8 DB, so it could never be completed.
-- Sources: Wowhead quest=319 "marked obsolete by Blizzard"; Cata Classic 4.4.2 TDB has no creature_queststarter
-- row for 317-320 (only enders), only 315 "The Perfect Stout" is still given by 1374.
-- Reversible: DELETE FROM `disables` WHERE `sourceType`=1 AND `entry` IN (318,319,320);
DELETE FROM `disables` WHERE `sourceType`=1 AND `entry` IN (318,319,320);
INSERT INTO `disables` (`sourceType`,`entry`,`flags`,`params_0`,`params_1`,`comment`) VALUES
(1,318,0,'','','Removed in Cataclysm (Cata Classic 4.4.2: no starter): Evershine'),
(1,319,0,'','','Removed in Cataclysm (Wowhead obsolete, Cata Classic 4.4.2: no starter): A Favor for Evershine'),
(1,320,0,'','','Removed in Cataclysm (Cata Classic 4.4.2: no starter): Return to Bellowfiz');
