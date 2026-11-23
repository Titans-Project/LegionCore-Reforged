-- Auction House Dance Party: midnight start as the calendar, the auctioneers and guards of the two main auction houses leave for the day, Happy Feet is scripted and the second light of Stormwind turns above the floor
UPDATE `game_event` SET `start_time` = '2026-07-22 00:00:00' WHERE `eventEntry` = 300;

DELETE FROM `game_event` WHERE `eventEntry` = 313;

DELETE FROM `game_event_creature` WHERE `eventEntry` = -300;
INSERT INTO `game_event_creature` (`guid`, `eventEntry`) VALUES
(264505, -300), (264624, -300), (264625, -300), (264626, -300), (264627, -300), (264628, -300), (264629, -300),
(350745, -300), (350748, -300), (350749, -300),
(351154, -300), (351161, -300), (351165, -300), (351166, -300), (351167, -300), (351172, -300),
(265260, -300), (265261, -300), (265271, -300), (266090, -300);

DELETE FROM `game_event_gameobject` WHERE `eventEntry` = -300;
INSERT INTO `game_event_gameobject` (`guid`, `eventEntry`) VALUES
(25646391, -300), (25646392, -300);

DELETE FROM `spell_script_names` WHERE `spell_id` = 243138;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (243138, 'spell_happy_feet');

UPDATE `creature` SET `position_x` = -8828, `position_y` = 656.3 WHERE `guid` = 14208148;
