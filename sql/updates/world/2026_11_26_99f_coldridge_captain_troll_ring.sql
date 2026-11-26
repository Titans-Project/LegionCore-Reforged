-- Coldridge Valley: ring of 7 Rockjaw Invaders (37070) around Captain Joren Ironstock, keep only 84061 (the one Joren faces). 27 spawns of 37070 remain for quest 24469 (needs 6).
DELETE FROM `creature` WHERE `id`=37070 AND `guid` in (84051,84058,84059,84060,84062,84063);
