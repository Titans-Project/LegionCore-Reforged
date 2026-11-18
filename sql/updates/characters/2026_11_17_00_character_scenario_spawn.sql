-- Placed creatures a character killed and objects used in a scenario, so that a resumed scenario keeps its finished steps as they were left
CREATE TABLE IF NOT EXISTS `character_scenario_spawn` (
  `guid` BIGINT UNSIGNED NOT NULL,
  `scenario` INT UNSIGNED NOT NULL,
  `type` TINYINT UNSIGNED NOT NULL COMMENT '0 creature killed, 1 gameobject used',
  `spawn` BIGINT UNSIGNED NOT NULL,
  `step` TINYINT UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `scenario`, `type`, `spawn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
