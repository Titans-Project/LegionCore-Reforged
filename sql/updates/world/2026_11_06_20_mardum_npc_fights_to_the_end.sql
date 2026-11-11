-- Mardum: the ambient fights go to the end, the No NPC Damage Below 45-90% aura (151597) is taken off the creatures of Mardum
UPDATE `creature_template_addon` SET `auras` = TRIM(REGEXP_REPLACE(CONCAT(' ', `auras`, ' '), ' 151597 ', ' ')) WHERE `entry` IN (SELECT `id` FROM `creature` WHERE `map` = 1481) AND CONCAT(' ', `auras`, ' ') LIKE '% 151597 %';
UPDATE `creature_addon` SET `auras` = TRIM(REGEXP_REPLACE(CONCAT(' ', `auras`, ' '), ' 151597 ', ' ')) WHERE `guid` IN (SELECT `guid` FROM `creature` WHERE `map` = 1481) AND CONCAT(' ', `auras`, ' ') LIKE '% 151597 %';
