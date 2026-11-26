-- Portions Copyright (C) 2026 DjusikW <https://github.com/DjusikW>
-- Quests 24503 / 28414 "Fourth and Goal" (Kezan, goblin start): the kick target 37203 between the KTC HQ
-- smokestacks only carried the "Explosives Marker" arrow (130856), so every player in Kezan saw a red arrow
-- bouncing over the smokestacks from level 1 on. In Cataclysm (TDB 4.3.4) the target also has
-- 70086 "Fourth and Goal: Cat's Mark Aura & Invis" (invisibility type 15), and npc_bilgewater_buccaneer_2
-- in kezan.cpp was written to let the player detect type 15 during the kick. The target is now invisible
-- again and players see it (with the arrow) only while 24503 or 28414 is incomplete, through the generic
-- type 15 detection 94568 (nothing else in Kezan uses invisibility type 15). Quest credit comes from
-- KilledMonsterCredit in the script and does not depend on seeing the target.

UPDATE `creature_template_addon` SET `auras` = '70086 130856' WHERE `entry` = 37203;

DELETE FROM `spell_area` WHERE `spell` = 94568 AND `area` = 4737 AND `quest_start` IN (24503, 28414);
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `classmask`, `active_event`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(94568, 4737, 24503, 0, 0, 0, 0, 0, 2, 1, 8, 11),
(94568, 4737, 28414, 0, 0, 0, 0, 0, 2, 1, 8, 11);
