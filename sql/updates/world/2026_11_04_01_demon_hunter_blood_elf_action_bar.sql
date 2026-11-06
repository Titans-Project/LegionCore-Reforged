-- Blood elf demon hunters start with the same action bar as the night elf ones: Fel Rush, Demon's Bite, Chaos Strike, racial and Spectral Sight
DELETE FROM `playercreateinfo_action` WHERE `race` = 10 AND `class` = 12;
INSERT INTO `playercreateinfo_action` (`race`, `class`, `button`, `action`, `type`) VALUES
(10, 12, 0, 195072, 0),
(10, 12, 1, 162243, 0),
(10, 12, 2, 162794, 0),
(10, 12, 9, 202719, 0),
(10, 12, 10, 188501, 0);
