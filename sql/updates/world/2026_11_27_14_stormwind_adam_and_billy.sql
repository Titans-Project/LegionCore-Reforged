-- Adam and Billy stay where they play instead of wandering into the canal
UPDATE `creature` SET `MovementType` = 0, `spawndist` = 0 WHERE `guid` IN (265238,265241) AND `id` IN (1366,1367);
