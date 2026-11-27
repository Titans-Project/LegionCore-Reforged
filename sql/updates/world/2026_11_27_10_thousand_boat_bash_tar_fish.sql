-- Thousand Boat Bash: two Tar-Covered Fish no longer float above the water, and the doubled fish and tar pool go
UPDATE `gameobject` SET `position_z` = 0 WHERE `guid` IN (25658877,25658879) AND `id` = 267596;
DELETE FROM `gameobject` WHERE `guid` IN (25658887,25658890,25658969,25658892);
DELETE FROM `game_event_gameobject` WHERE `guid` IN (25658887,25658890,25658969,25658892);
