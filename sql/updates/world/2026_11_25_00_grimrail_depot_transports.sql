-- Grimrail Depot: the moving scenery of the train ride is created from its templates, not from spawns
DELETE FROM `gameobject` WHERE `guid` IN (25810004, 25810006, 25810007, 25810008, 25810009, 25810010, 25810068) AND `map` = 1208;
