-- Holidays: dates, hours and lengths of the in-game calendar in the realm's local time; Winter Veil, Love is in the Air and Pilgrim's Bounty no longer stop after 2020-2021
UPDATE `game_event` SET `end_time` = '2035-07-05 19:00:00' WHERE `eventEntry` IN (2, 8, 26, 52);

UPDATE `game_event` SET `start_time` = CONCAT(DATE(`start_time`), ' 10:00:00') WHERE `eventEntry` IN (1, 2, 5, 7, 8, 10, 11, 12, 24, 26, 50, 51, 52);
UPDATE `game_event` SET `start_time` = CONCAT(DATE(`start_time`), ' 00:00:00') WHERE `eventEntry` IN (69, 70, 78, 87, 299, 301, 302, 303, 305, 306, 307);

UPDATE `game_event` SET `length` = 72 * 60 WHERE `eventEntry` = 307;
UPDATE `game_event` SET `length` = 18 * 60 WHERE `eventEntry` = 5;
UPDATE `game_event` SET `length` = 404 * 60 WHERE `eventEntry` = 2;
UPDATE `game_event` SET `length` = 384 * 60 WHERE `eventEntry` = 24;
UPDATE `game_event` SET `length` = 120 * 60 WHERE `eventEntry` IN (302, 303);
UPDATE `game_event` SET `length` = 48 * 60 WHERE `eventEntry` = 51;
UPDATE `game_event` SET `length` = 337 * 60 WHERE `eventEntry` = 12;
UPDATE `game_event` SET `length` = 188 * 60 WHERE `eventEntry` = 52;
