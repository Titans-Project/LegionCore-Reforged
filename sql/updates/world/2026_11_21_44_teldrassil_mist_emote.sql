-- 938 Mist: npc_mist says EMOTE_AT_HOME (group 1) as Mist herself, but the emote (BroadcastText 1340) sat on
-- Sentinel Arynia Cloudsbreak (3519) as a say. Cata 4.3.4 TDB: 1340 is Mist's (3568) text emote.
UPDATE `creature_text` SET `CreatureID` = 3568, `Type` = 16, `comment` = 'Mist - EMOTE_AT_HOME'
WHERE `CreatureID` = 3519 AND `GroupID` = 1 AND `ID` = 0 AND `BroadcastTextId` = 1340;
