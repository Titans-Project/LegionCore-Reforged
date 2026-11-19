-- Frost artifact souls and Fleshripper reach the player who owns a pet or guardian invoker (target 130).
UPDATE `smart_scripts` SET `target_type` = 130 WHERE `source_type` = 0 AND `action_type` = 206 AND `target_type` = 2
  AND ((`entryorguid` = 93948 AND `id` = 1) OR (`entryorguid` IN (93950, 93951) AND `id` = 0));
UPDATE `smart_scripts` SET `target_type` = 130 WHERE `source_type` = 0 AND `action_type` = 1 AND `target_type` = 1
  AND ((`entryorguid` = 101778 AND `id` IN (0, 1)) OR (`entryorguid` = 93948 AND `id` IN (2, 3, 4)) OR (`entryorguid` IN (93950, 93951) AND `id` IN (1, 2)));
