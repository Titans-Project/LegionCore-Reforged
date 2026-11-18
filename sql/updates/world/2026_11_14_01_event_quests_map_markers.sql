-- Ties the Great Gnomeregan Run and Mage Tower challenge quests to their event, so the map only shows them while it runs
DELETE FROM `game_event_seasonal_questrelation` WHERE `questId` IN (47709, 44925, 45416, 45526, 45627, 46035, 46065, 46127);
INSERT INTO `game_event_seasonal_questrelation` (`questId`, `eventEntry`) VALUES
(47709, 304),
(44925, 400), (45416, 400), (45526, 400), (45627, 400), (46035, 400), (46065, 400), (46127, 400);
