-- Spawn 152851 (entry 50457, Twilight Highlands) had MovementType 46 -> assert in FactorySelector::SelectMovementGenerator on grid load
UPDATE `creature` SET `MovementType` = 0 WHERE `guid` = 152851 AND `MovementType` = 46;
