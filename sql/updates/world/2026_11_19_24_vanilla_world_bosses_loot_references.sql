-- Azuregos and the Emerald Dragons roll each of their loot references on its own, as their TDB 7.3.5 loot expects
UPDATE `creature_loot_template` SET `GroupId` = 0 WHERE `Entry` IN (6109, 14887, 14888, 14889, 14890) AND `Reference` > 0 AND `GroupId` = 1;
