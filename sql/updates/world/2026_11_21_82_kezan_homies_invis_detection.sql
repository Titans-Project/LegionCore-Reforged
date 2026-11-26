-- Kezan: "Rolling with my Homies" 14071. Ace, Gobber and Izzy carry quest
-- invisibility 66403/66404/66405 (types 7/8/9); nothing gave the player the
-- matching detection, so the friends to pick up were never visible.
-- Rows as in Cata 4.3.4 and Cata Classic 4.4.2 TDB spell_area (status INCOMPLETE).
DELETE FROM `spell_area` WHERE `quest_start` = 14071 AND `spell` IN (49416, 49417, 60922);
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `classmask`, `active_event`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(49416, 4767, 14071, 0, 0, 0, 0, 0, 2, 1, 8, 11), -- Generic Quest Invisibility Detection 1 (Ace)
(49417, 4737, 14071, 0, 0, 0, 0, 0, 2, 1, 8, 11), -- Generic Quest Invisibility Detection 2 (Gobber)
(60922, 4765, 14071, 0, 0, 0, 0, 0, 2, 1, 8, 11); -- Generic Quest Invisibility Detection 3 (Izzy)
