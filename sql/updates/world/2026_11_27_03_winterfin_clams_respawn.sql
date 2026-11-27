-- Winterfin Clams come back after 5 minutes (TDB 7.3.5), not at once
UPDATE `gameobject` SET `spawntimesecs` = 300 WHERE `id` = 187367 AND `spawntimesecs` = 1;
