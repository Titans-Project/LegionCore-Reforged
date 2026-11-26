-- Timewalking: the timewarped pieces of Yor, Eck, Amanitar, Jedoga, Volazj, the Drakkari Colossus and Xin drop in timewalking (loot mode 8), not in normal or heroic
UPDATE `creature_loot_template` SET `LootMode` = 8 WHERE `Entry` = 22930 AND `Item` IN (127231,127420,127421,127422,127423,127424,127425,127426,127427,127428,127429);
UPDATE `creature_loot_template` SET `LootMode` = 8 WHERE `Entry` = 29307 AND `Item` IN (127534);
UPDATE `creature_loot_template` SET `LootMode` = 8 WHERE `Entry` = 29310 AND `Item` IN (127610);
UPDATE `creature_loot_template` SET `LootMode` = 8 WHERE `Entry` = 29311 AND `Item` IN (127628);
UPDATE `creature_loot_template` SET `LootMode` = 8 WHERE `Entry` = 29573 AND `Item` IN (127534);
UPDATE `creature_loot_template` SET `LootMode` = 8 WHERE `Entry` = 29932 AND `Item` IN (127564,127565,127566,127567);
UPDATE `creature_loot_template` SET `LootMode` = 8 WHERE `Entry` = 30258 AND `Item` IN (127613,127614,127615,127617,127635,127643,127644,127645,127646);
UPDATE `creature_loot_template` SET `LootMode` = 8 WHERE `Entry` = 61398 AND `Item` IN (144214);
