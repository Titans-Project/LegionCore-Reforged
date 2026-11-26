-- Dun Morogh 26364 "Down with Crushcog!": the battle at Iceflow Lake had no script. Only the gossip credit of
-- High Tinker Mekkatorque (42849) existed; Razlo Crushcog (42839) had no AI, so "Razlo Crushcog defeated" (42860)
-- was never given. Ported from the MoP 5.4.8 DB (world_04_03_2023): Mekkatorque's gossip starts the event, he and
-- Mountaineer Stonegrind (42852) walk to Razlo, the Crushcog Technicians (43230) wake the four Crushcog's Guardians
-- (42294), when all four are dead Razlo becomes attackable, and his death gives 42860 to the players stored at the
-- gossip. Wowhead (cata/quest=26364): "Speak to High Tinker Mekkatorque ... when you are ready to begin the battle".
-- Changes against the MoP rows:
--  * the guardian-death counter (event 77 / action 63) is not implemented in this core: Razlo counts the deaths in
--    event phases instead (phase 3 + 4th death = remove UNIT_FLAG 768);
--  * action 203 (MoP combat stop) is SET_KD here: replaced by EVADE (24);
--  * WP_START gets reactState 2 (here param6 sets the react state) and no escort quest;
--  * the Legion objective "Talk to High Tinker Mekkatorque" (42849) keeps its credit on the gossip.
-- creature_text: MoP texts, BroadcastText ids from hotfixes broadcast_text.
-- Spawn guids mapped by position: MoP 168763/168765/168766/168767 -> 84392/84407/84393/84394 (guardians),
-- MoP 168788/168789/168790/168791 -> 84397/84398/84399/84400 (technicians).

-- templates (MoP 5.4.8 unit_flags)
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (42839,42852,43230,42294);
UPDATE `creature_template` SET `unit_flags`=768 WHERE `entry` IN (42839,43230);
UPDATE `creature_template` SET `unit_flags`=33555200 WHERE `entry`=42294;

DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (42849,42839,42852,43230,-84392,-84393,-84394,-84407,-84397,-84398,-84399,-84400);
DELETE FROM `smart_scripts` WHERE `source_type`=9 AND `entryorguid` IN (4284900,4284901,4284902,4284903,4284904,4283900,4283901,4229400,4229401,4229402,4229403);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
-- High Tinker Mekkatorque
(42849,0,0,0,11,0,100,0,0,0,0,0,8,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Respawn - Set React State Aggressive'),
(42849,0,1,0,0,0,100,0,0,0,3000,3000,49,0,0,0,0,0,0,19,42294,5,0,0,0,0,0,'High Tinker Mekkatorque - In Combat - Attack Crushcog''s Guardian'),
(42849,0,2,3,62,0,100,0,11662,0,0,0,33,42849,0,0,0,0,0,7,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Gossip Select - Credit Talk to High Tinker Mekkatorque'),
(42849,0,3,4,61,0,100,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,'High Tinker Mekkatorque - Linked - Store Target List'),
(42849,0,4,5,61,0,100,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'High Tinker Mekkatorque - Linked - Close Gossip'),
(42849,0,5,0,61,0,100,0,0,0,0,0,80,4284900,2,0,0,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - Linked - Run Script'),
(42849,0,6,7,58,0,100,0,3,42849,0,0,45,0,1,0,0,0,0,11,42839,15,0,0,0,0,0,'High Tinker Mekkatorque - On Waypoint Ended - Set Data 0 1 Razlo Crushcog'),
(42849,0,7,0,61,0,100,0,0,0,0,0,101,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - Linked - Set Home'),
(42849,0,8,9,38,0,100,0,0,1,0,0,49,0,0,0,0,0,0,11,42839,10,0,0,0,0,0,'High Tinker Mekkatorque - On Data Set 0 1 - Attack Razlo Crushcog'),
(42849,0,9,0,61,0,100,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - Linked - Set Phase 1'),
(42849,0,10,0,38,1,100,0,0,3,0,0,87,4284901,4284902,4284903,4284904,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Data Set 0 3 - Run Random Script'),
(42849,0,11,12,38,0,100,0,0,2,0,0,24,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Data Set 0 2 - Evade'),
(42849,0,12,13,61,0,100,0,0,0,0,0,1,3,5000,0,0,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - Linked - Say Line 3'),
(42849,0,13,0,61,0,100,0,0,0,0,0,22,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - Linked - Reset Phase'),
(42849,0,14,15,52,0,100,0,3,42849,0,0,1,4,25000,0,0,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Text Over 3 - Say Line 4'),
(42849,0,15,0,61,0,100,0,0,0,0,0,33,42860,0,0,0,0,0,12,1,0,0,0,0,0,0,'High Tinker Mekkatorque - Linked - Credit Razlo Crushcog defeated'),
(42849,0,16,0,52,0,100,0,4,42849,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Text Over 4 - Despawn'),
(4284900,9,0,0,0,0,100,0,0,0,0,0,83,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Script - Remove NpcFlag Gossip'),
(4284900,9,1,0,0,0,100,0,2000,2000,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Script - Say Line 0'),
(4284900,9,2,0,0,0,100,0,13000,13000,0,0,1,1,0,0,0,0,0,12,1,0,0,0,0,0,0,'High Tinker Mekkatorque - On Script - Say Line 1'),
(4284900,9,3,0,0,0,100,0,11000,11000,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Script - Say Line 2'),
(4284900,9,4,0,0,0,100,0,4000,4000,0,0,45,0,1,0,0,0,0,11,42852,5,0,0,0,0,0,'High Tinker Mekkatorque - On Script - Set Data 0 1 Mountaineer Stonegrind'),
(4284900,9,5,0,0,0,100,0,3000,3000,0,0,53,0,42849,0,0,0,2,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Script - Waypoint Start'),
(4284901,9,0,0,0,0,100,0,0,0,0,0,1,5,0,0,0,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Script - Say Line 5'),
(4284901,9,1,0,0,0,100,0,0,0,0,0,11,80110,0,0,0,0,0,2,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Script - Cast Mekkatorque-Missiles'),
(4284902,9,0,0,0,0,100,0,0,0,0,0,1,6,0,0,0,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Script - Say Line 6'),
(4284902,9,1,0,0,0,100,0,0,0,0,0,11,22742,0,0,0,0,0,2,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Script - Cast Super Shrink Ray'),
(4284903,9,0,0,0,0,100,0,0,0,0,0,1,7,0,0,0,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Script - Say Line 7'),
(4284903,9,1,0,0,0,100,0,0,0,0,0,11,80148,0,0,0,0,0,2,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Script - Cast Mekkatorque-Ray'),
(4284904,9,0,0,0,0,100,0,0,0,0,0,1,8,0,0,0,0,0,1,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Script - Say Line 8'),
(4284904,9,1,0,0,0,100,0,0,0,0,0,11,80098,0,0,0,0,0,2,0,0,0,0,0,0,0,'High Tinker Mekkatorque - On Script - Cast Mekkatorque-Cannon'),
-- Razlo Crushcog
(42839,0,0,0,11,0,100,0,0,0,0,0,8,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Razlo Crushcog - On Respawn - Set React State Defensive'),
(42839,0,1,0,0,0,100,0,5000,5000,15000,15000,87,4283900,4283901,0,0,0,0,1,0,0,0,0,0,0,0,'Razlo Crushcog - In Combat - Run Random Script'),
(42839,0,2,0,38,0,100,0,0,1,0,0,1,0,8000,0,0,0,0,1,0,0,0,0,0,0,0,'Razlo Crushcog - On Data Set 0 1 - Say Line 0'),
(42839,0,3,0,52,0,100,0,0,42839,0,0,1,1,5000,0,0,0,0,1,0,0,0,0,0,0,0,'Razlo Crushcog - On Text Over 0 - Say Line 1'),
(42839,0,4,0,52,0,100,0,1,42839,0,0,45,0,1,0,0,0,0,11,43230,5,0,0,0,0,0,'Razlo Crushcog - On Text Over 1 - Set Data 0 1 Crushcog Technicians'),
(42839,0,5,0,38,0,100,0,0,2,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Razlo Crushcog - On Data Set 0 2 - Say Line 2'),
(42839,0,6,7,38,4,100,0,0,3,0,0,19,768,0,0,0,0,0,1,0,0,0,0,0,0,0,'Razlo Crushcog - On Data Set 0 3 in Phase 3 (4th guardian dead) - Remove UnitFlag 768'),
(42839,0,7,8,61,0,100,0,0,0,0,0,45,0,1,0,0,0,0,11,42849,10,0,0,0,0,0,'Razlo Crushcog - Linked - Set Data 0 1 High Tinker Mekkatorque'),
(42839,0,8,0,61,0,100,0,0,0,0,0,45,0,2,0,0,0,0,11,42852,10,0,0,0,0,0,'Razlo Crushcog - Linked - Set Data 0 2 Mountaineer Stonegrind'),
(42839,0,9,0,38,0,100,0,0,3,0,0,23,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Razlo Crushcog - On Data Set 0 3 - Increment Phase (guardian death count)'),
(42839,0,10,11,6,0,100,0,0,0,0,0,45,0,2,0,0,0,0,11,42849,10,0,0,0,0,0,'Razlo Crushcog - On Death - Set Data 0 2 High Tinker Mekkatorque'),
(42839,0,11,12,61,0,100,0,0,0,0,0,45,0,3,0,0,0,0,11,42852,10,0,0,0,0,0,'Razlo Crushcog - Linked - Set Data 0 3 Mountaineer Stonegrind'),
(42839,0,12,0,61,0,100,0,0,0,0,0,41,3000,0,0,0,0,0,1,0,0,0,0,0,0,0,'Razlo Crushcog - Linked - Despawn'),
(4283900,9,0,0,0,0,100,0,0,0,0,0,12,43230,6,5000,0,1,0,8,0,0,0,-5267.87,105.625,392.128,0,'Razlo Crushcog - On Script - Summon Crushcog Technician'),
(4283901,9,0,0,0,0,100,0,0,0,0,0,12,43230,6,5000,0,1,0,8,0,0,0,-5270.25,129.416,392.886,0,'Razlo Crushcog - On Script - Summon Crushcog Technician'),
-- Mountaineer Stonegrind
(42852,0,0,0,11,0,100,0,0,0,0,0,8,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mountaineer Stonegrind - On Respawn - Set React State Aggressive'),
(42852,0,1,0,0,0,100,0,0,0,3000,3000,49,0,0,0,0,0,0,19,42294,5,0,0,0,0,0,'Mountaineer Stonegrind - In Combat - Attack Crushcog''s Guardian'),
(42852,0,2,3,38,0,100,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mountaineer Stonegrind - On Data Set 0 1 - Say Line 0'),
(42852,0,3,0,61,0,100,0,0,0,0,0,53,0,42852,0,0,0,2,1,0,0,0,0,0,0,0,'Mountaineer Stonegrind - Linked - Waypoint Start'),
(42852,0,4,0,58,0,100,0,2,42852,0,0,101,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mountaineer Stonegrind - On Waypoint Ended - Set Home'),
(42852,0,5,0,38,0,100,0,0,2,0,0,49,0,0,0,0,0,0,11,42839,10,0,0,0,0,0,'Mountaineer Stonegrind - On Data Set 0 2 - Attack Razlo Crushcog'),
(42852,0,6,7,38,0,100,0,0,3,0,0,24,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mountaineer Stonegrind - On Data Set 0 3 - Evade'),
(42852,0,7,0,61,0,100,0,0,0,0,0,1,1,30000,0,0,0,0,1,0,0,0,0,0,0,0,'Mountaineer Stonegrind - Linked - Say Line 1'),
(42852,0,8,0,52,0,100,0,1,42852,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mountaineer Stonegrind - On Text Over 1 - Despawn'),
-- Crushcog Technician: summoned ones fight, the four standing ones wake a guardian each
(43230,0,0,1,54,0,100,0,0,0,0,0,19,768,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog Technician - On Summoned - Remove UnitFlag 768'),
(43230,0,1,0,61,0,100,0,0,0,0,0,49,0,0,0,0,0,0,21,30,0,0,0,0,0,0,'Crushcog Technician - Linked - Attack Closest Player'),
(-84397,0,0,1,38,0,100,0,0,1,0,0,97,10,10,0,0,0,0,8,0,0,0,-5254.76,107.405,394.973,0,'Crushcog Technician - On Data 0 1 - Jump To Pos'),
(-84397,0,1,2,61,0,100,0,0,0,0,0,45,0,1,0,0,0,0,10,84394,42294,0,0,0,0,0,'Crushcog Technician - Linked - Set Data 0 1 Crushcog''s Guardian'),
(-84397,0,2,0,61,0,100,0,0,0,0,0,41,2000,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog Technician - Linked - Despawn'),
(-84398,0,0,1,38,0,100,0,0,1,0,0,97,10,10,0,0,0,0,8,0,0,0,-5243.31,104.566,394.584,0,'Crushcog Technician - On Data 0 1 - Jump To Pos'),
(-84398,0,1,2,61,0,100,0,0,0,0,0,45,0,1,0,0,0,0,10,84407,42294,0,0,0,0,0,'Crushcog Technician - Linked - Set Data 0 1 Crushcog''s Guardian'),
(-84398,0,2,0,61,0,100,0,0,0,0,0,41,2000,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog Technician - Linked - Despawn'),
(-84399,0,0,1,38,0,100,0,0,1,0,0,97,10,10,0,0,0,0,8,0,0,0,-5238.75,129.153,397.304,0,'Crushcog Technician - On Data 0 1 - Jump To Pos'),
(-84399,0,1,2,61,0,100,0,0,0,0,0,45,0,1,0,0,0,0,10,84393,42294,0,0,0,0,0,'Crushcog Technician - Linked - Set Data 0 1 Crushcog''s Guardian'),
(-84399,0,2,0,61,0,100,0,0,0,0,0,41,2000,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog Technician - Linked - Despawn'),
(-84400,0,0,1,38,0,100,0,0,1,0,0,97,10,10,0,0,0,0,8,0,0,0,-5251.92,131.445,396.917,0,'Crushcog Technician - On Data 0 1 - Jump To Pos'),
(-84400,0,1,2,61,0,100,0,0,0,0,0,45,0,1,0,0,0,0,10,84392,42294,0,0,0,0,0,'Crushcog Technician - Linked - Set Data 0 1 Crushcog''s Guardian'),
(-84400,0,2,0,61,0,100,0,0,0,0,0,41,2000,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog Technician - Linked - Despawn'),
-- Crushcog's Guardian (frozen until a technician wakes it; each death counts for Razlo)
(-84392,0,0,0,38,0,100,0,0,1,0,0,80,4229400,2,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Data Set 0 1 - Run Script'),
(-84392,0,1,0,6,0,100,0,0,0,0,0,45,0,3,0,0,0,0,11,42839,15,0,0,0,0,0,'Crushcog''s Guardian - On Death - Set Data 0 3 Razlo Crushcog'),
(-84393,0,0,0,38,0,100,0,0,1,0,0,80,4229401,2,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Data Set 0 1 - Run Script'),
(-84393,0,1,0,6,0,100,0,0,0,0,0,45,0,3,0,0,0,0,11,42839,15,0,0,0,0,0,'Crushcog''s Guardian - On Death - Set Data 0 3 Razlo Crushcog'),
(-84407,0,0,0,38,0,100,0,0,1,0,0,80,4229402,2,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Data Set 0 1 - Run Script'),
(-84407,0,1,0,6,0,100,0,0,0,0,0,45,0,3,0,0,0,0,11,42839,15,0,0,0,0,0,'Crushcog''s Guardian - On Death - Set Data 0 3 Razlo Crushcog'),
(-84394,0,0,0,38,0,100,0,0,1,0,0,80,4229403,2,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Data Set 0 1 - Run Script'),
(-84394,0,1,0,6,0,100,0,0,0,0,0,45,0,3,0,0,0,0,11,42839,15,0,0,0,0,0,'Crushcog''s Guardian - On Death - Set Data 0 3 Razlo Crushcog'),
(4229400,9,0,0,0,0,100,0,0,0,0,0,59,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Set Run'),
(4229400,9,1,0,0,0,100,0,2000,2000,0,0,28,16245,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Remove Aura Freeze Anim'),
(4229400,9,2,0,0,0,100,0,3000,3000,0,0,69,0,0,0,0,0,0,8,0,0,0,-5252.98,128.316,394.184,0,'Crushcog''s Guardian - On Script - Move To Pos'),
(4229400,9,3,0,0,0,100,0,2000,2000,0,0,19,33554432,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Remove UnitFlag Not Selectable'),
(4229400,9,4,0,0,0,100,0,3000,3000,0,0,19,768,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Remove UnitFlag 768'),
(4229400,9,5,0,0,0,100,0,0,0,0,0,45,0,2,0,0,0,0,11,42839,10,0,0,0,0,0,'Crushcog''s Guardian - On Script - Set Data 0 2 Razlo Crushcog'),
(4229400,9,6,0,0,0,100,0,0,0,0,0,8,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Set React State Aggressive'),
(4229400,9,7,0,0,0,100,0,0,0,0,0,49,0,0,0,0,0,0,11,42852,10,0,0,0,0,0,'Crushcog''s Guardian - On Script - Attack Mountaineer Stonegrind'),
(4229401,9,0,0,0,0,100,0,0,0,0,0,59,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Set Run'),
(4229401,9,1,0,0,0,100,0,2000,2000,0,0,28,16245,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Remove Aura Freeze Anim'),
(4229401,9,2,0,0,0,100,0,3000,3000,0,0,69,0,0,0,0,0,0,8,0,0,0,-5252.07,123.959,393.777,0,'Crushcog''s Guardian - On Script - Move To Pos'),
(4229401,9,3,0,0,0,100,0,2000,2000,0,0,19,33554432,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Remove UnitFlag Not Selectable'),
(4229401,9,4,0,0,0,100,0,3000,3000,0,0,19,768,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Remove UnitFlag 768'),
(4229401,9,5,0,0,0,100,0,0,0,0,0,8,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Set React State Aggressive'),
(4229401,9,6,0,0,0,100,0,0,0,0,0,49,0,0,0,0,0,0,11,42852,10,0,0,0,0,0,'Crushcog''s Guardian - On Script - Attack Mountaineer Stonegrind'),
(4229402,9,0,0,0,0,100,0,0,0,0,0,59,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Set Run'),
(4229402,9,1,0,0,0,100,0,2000,2000,0,0,28,16245,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Remove Aura Freeze Anim'),
(4229402,9,2,0,0,0,100,0,3000,3000,0,0,69,0,0,0,0,0,0,8,0,0,0,-5253.38,115.766,393.962,0,'Crushcog''s Guardian - On Script - Move To Pos'),
(4229402,9,3,0,0,0,100,0,2000,2000,0,0,19,33554432,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Remove UnitFlag Not Selectable'),
(4229402,9,4,0,0,0,100,0,3000,3000,0,0,19,768,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Remove UnitFlag 768'),
(4229402,9,5,0,0,0,100,0,0,0,0,0,8,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Set React State Aggressive'),
(4229402,9,6,0,0,0,100,0,0,0,0,0,49,0,0,0,0,0,0,11,42849,10,0,0,0,0,0,'Crushcog''s Guardian - On Script - Attack High Tinker Mekkatorque'),
(4229403,9,0,0,0,0,100,0,0,0,0,0,59,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Set Run'),
(4229403,9,1,0,0,0,100,0,2000,2000,0,0,28,16245,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Remove Aura Freeze Anim'),
(4229403,9,2,0,0,0,100,0,3000,3000,0,0,69,0,0,0,0,0,0,8,0,0,0,-5255.27,111.534,393.167,0,'Crushcog''s Guardian - On Script - Move To Pos'),
(4229403,9,3,0,0,0,100,0,2000,2000,0,0,19,33554432,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Remove UnitFlag Not Selectable'),
(4229403,9,4,0,0,0,100,0,3000,3000,0,0,19,768,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Remove UnitFlag 768'),
(4229403,9,5,0,0,0,100,0,0,0,0,0,8,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Crushcog''s Guardian - On Script - Set React State Aggressive'),
(4229403,9,6,0,0,0,100,0,0,0,0,0,49,0,0,0,0,0,0,11,42849,10,0,0,0,0,0,'Crushcog''s Guardian - On Script - Attack High Tinker Mekkatorque');

DELETE FROM `waypoints` WHERE `entry` IN (42849,42852);
INSERT INTO `waypoints` (`entry`,`pointid`,`position_x`,`position_y`,`position_z`,`point_comment`) VALUES
(42849,1,-5292.57,132.09,386.318,'High Tinker Mekkatorque'),
(42849,2,-5280.62,124.816,390.736,'High Tinker Mekkatorque'),
(42849,3,-5261.72,119.5,393.865,'High Tinker Mekkatorque'),
(42852,1,-5278.12,129.457,391.236,'Mountaineer Stonegrind'),
(42852,2,-5260.35,124.175,393.962,'Mountaineer Stonegrind');

DELETE FROM `creature_text` WHERE `CreatureID` IN (42849,42839,42852);
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextID`,`comment`) VALUES
(42849,0,0,'Mekgineer Thermaplugg refuses to acknowledge that his defeat is imminent! He has sent Razlo Crushcog to prevent us from rebuilding our beloved Gnomeregan!',12,0,100,5,0,20890,42758,'High Tinker Mekkatorque'),
(42849,1,0,'But $n has thwarted his plans at every turn, and the dwarves of Ironforge stand with us!',12,0,100,25,0,20891,42759,'High Tinker Mekkatorque'),
(42849,2,0,'Let''s send him crawling back to his master in defeat!',12,0,100,5,0,20892,42760,'High Tinker Mekkatorque'),
(42849,3,0,'We''ve done it! We''re victorious!',12,0,100,5,0,20893,42763,'High Tinker Mekkatorque'),
(42849,4,0,'With Crushcog defeated, Thermaplugg is sure to be quaking in his mechano-tank, and rightly so. You''re next Thermaplugg. You''re next!',12,0,100,5,0,20894,42765,'High Tinker Mekkatorque'),
(42849,5,0,'Mekkatorque-Missiles!',14,7,100,0,0,17562,42833,'High Tinker Mekkatorque'),
(42849,6,0,'Mekkatorque-Ray!',14,7,100,0,0,17563,42832,'High Tinker Mekkatorque'),
(42849,7,0,'Mekkatorque-Beam!',14,7,100,0,0,17560,42834,'High Tinker Mekkatorque'),
(42849,8,0,'Mekkatorque-Cannon!',14,7,100,0,0,17561,42829,'High Tinker Mekkatorque'),
(42839,0,0,'You! How did you escape detection by my sentry-bots?',12,0,100,0,0,21243,42761,'Razlo Crushcog'),
(42839,1,0,'No matter! My guardians and I will make short work of you. To arms, men!',12,0,100,0,0,21244,42762,'Razlo Crushcog'),
(42839,2,0,'You will never defeat the true sons of Gnomeregan!',12,0,100,0,0,21245,42781,'Razlo Crushcog'),
(42852,0,0,'Aye, let''s teach this addle-brained gnome a lesson!',12,0,100,25,0,0,42767,'Mountaineer Stonegrind'),
(42852,1,0,'That''ll teach you to mess with the might of Ironforge and Gnomeregan!',12,0,100,25,0,0,42766,'Mountaineer Stonegrind');
