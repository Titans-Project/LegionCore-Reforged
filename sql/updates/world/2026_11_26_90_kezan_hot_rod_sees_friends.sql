-- Kezan 14071 "Rolling with my Homies" (goblins): Izzy, Ace and Gobber were never picked up, objectives 34959 /
-- 34957 / 34958 stayed 0. The friends (34890 / 34892 / 34954, npc_roling_friends) stand in quest invisibility
-- 9 / 7 / 8 (auras 66405 / 66403 / 66404) and react to the Hot Rod 34840 in MoveInLineOfSight. The core calls it only
-- when the two can detect each other, and the car's invisibility detection was granted by the C++ script npc_hot_rod,
-- which is disabled (the car runs SmartAI). The car now puts the three detection auras on itself when someone boards:
-- 49416 / 49417 / 60922, Generic Quest Invisibility Detection 1-3, the auras the player gets from spell_area.
DELETE FROM `smart_scripts` WHERE `entryorguid` = 34840 AND `source_type` = 0 AND `id` IN (9, 10, 11);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `action_type`, `action_param1`, `action_param2`, `target_type`, `comment`) VALUES
(34840, 0, 9, 10, 27, 0, 100, 0, 11, 49416, 2, 1, 'Hot Rod - On Passenger Boarded - Cast Generic Quest Invisibility Detection 1 on self'),
(34840, 0, 10, 11, 61, 0, 100, 0, 11, 49417, 2, 1, 'Hot Rod - Linked - Cast Generic Quest Invisibility Detection 2 on self'),
(34840, 0, 11, 0, 61, 0, 100, 0, 11, 60922, 2, 1, 'Hot Rod - Linked - Cast Generic Quest Invisibility Detection 3 on self');
