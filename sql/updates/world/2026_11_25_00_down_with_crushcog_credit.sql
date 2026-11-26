-- 26364 "Down with Crushcog!": action 24 (EVADE) returns from SmartScript::ProcessAction before the link is
-- handled, so the lines after it (victory speech, credit 42860, despawn) never ran. Evade goes last in the chain.
UPDATE `smart_scripts` SET `action_type`=1, `action_param1`=3, `action_param2`=5000, `comment`='High Tinker Mekkatorque - On Data Set 0 2 - Say Line 3' WHERE `entryorguid`=42849 AND `source_type`=0 AND `id`=11;
UPDATE `smart_scripts` SET `action_type`=22, `action_param1`=0, `action_param2`=0, `comment`='High Tinker Mekkatorque - Linked - Reset Phase' WHERE `entryorguid`=42849 AND `source_type`=0 AND `id`=12;
UPDATE `smart_scripts` SET `action_type`=24, `action_param1`=0, `action_param2`=0, `comment`='High Tinker Mekkatorque - Linked - Evade' WHERE `entryorguid`=42849 AND `source_type`=0 AND `id`=13;
UPDATE `smart_scripts` SET `action_type`=1, `action_param1`=1, `action_param2`=30000, `comment`='Mountaineer Stonegrind - On Data Set 0 3 - Say Line 1' WHERE `entryorguid`=42852 AND `source_type`=0 AND `id`=6;
UPDATE `smart_scripts` SET `action_type`=24, `action_param1`=0, `action_param2`=0, `comment`='Mountaineer Stonegrind - Linked - Evade' WHERE `entryorguid`=42852 AND `source_type`=0 AND `id`=7;
-- Mekkatorque's last waypoint is 13.8 yards from Razlo's spawn and both move in the fight: 10 yards is too short.
UPDATE `smart_scripts` SET `target_param2`=40 WHERE `source_type`=0 AND ((`entryorguid`=42839 AND `id` IN (7,8,10,11)) OR (`entryorguid`=42849 AND `id` IN (6,8)) OR (`entryorguid`=42852 AND `id`=5)) AND `target_type`=11;
