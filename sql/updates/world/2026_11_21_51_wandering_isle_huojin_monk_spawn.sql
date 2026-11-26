-- The Wandering Isle: "Balanced Perspective" 29784. Its only quest starter,
-- Huojin Monk 65558 (mob_huojin_monk), had no spawn, so the quest could not be
-- taken. Spawn next to Ji Firepaw at the rope dock from the MoP 5.4.8 world DB
-- (guid 563690, area 5946, phaseMask 1); Wowhead (Wayback 2013-07-20) lists
-- Huojin Monk as the starter. The script walks him off and despawns him on
-- accept, so respawn is 60 s instead of 300 s to not block other players.
DELETE FROM `creature` WHERE `guid` = 15195700;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`) VALUES
(15195700, 65558, 860, 5736, 5946, 1, 1, 1134.48, 4154.32, 190.599, 3.95156, 60);
