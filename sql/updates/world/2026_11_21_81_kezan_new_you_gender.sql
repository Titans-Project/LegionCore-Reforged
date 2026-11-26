-- Kezan: "The New You" 14109/14110 had their gender conditions swapped.
-- 14109 follows 26712 (male "Off to the Bank") and is turned in to the girlfriend
-- ("I can't wait to not have to work anymore!"); 14110 follows 26711 (female) and
-- the boyfriend ("I can't wait to be a kept man!"). Sources: Wowhead Series boxes
-- (26712 -> 14109, 26711 -> 14110), Cata 4.3.4 TDB RewardNextQuest (same pairing).
-- CONDITION_GENDER: 0 = male, 1 = female.
UPDATE `conditions` SET `ConditionValue1` = 0
 WHERE `SourceTypeOrReferenceId` = 19 AND `SourceEntry` = 14109 AND `ConditionTypeOrReference` = 20;
UPDATE `conditions` SET `ConditionValue1` = 1
 WHERE `SourceTypeOrReferenceId` = 19 AND `SourceEntry` = 14110 AND `ConditionTypeOrReference` = 20;
