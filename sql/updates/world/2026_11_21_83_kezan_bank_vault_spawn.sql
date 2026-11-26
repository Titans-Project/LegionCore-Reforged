-- Kezan: "The Great Bank Heist" 14122. The First Bank of Kezan Vault (35486,
-- vehicle with spellclick 67476 and npc_hack_bank_controller) had no spawn, so
-- the quest could not be done. Spawns from MoP 5.4.8 world DB (bank, area 4770);
-- phaseMask 7 like the bank teller 35120 standing next to it.
DELETE FROM `creature` WHERE `guid` BETWEEN 15187370 AND 15187372;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`) VALUES
(15187370, 35486, 648, 4737, 4770, 1, 7, -8370.23, 1728.82, 40.0281, 2.47827, 300),
(15187371, 35486, 648, 4737, 4770, 1, 7, -8371.88, 1729.45, 40.021,  2.40881, 300),
(15187372, 35486, 648, 4737, 4770, 1, 7, -8372.41, 1721.09, 39.9397, 3.56568, 300);
