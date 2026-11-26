-- Kezan 14071 "Rolling with my Homies": Izzy 34890 still not picked up after 2026_11_26_90.
-- The car cast 49416/49417/60922 on itself, but every one of them has spell_area rows, and a spell
-- with spell_area rows can be cast only inside one of its areas (SpellInfo::CheckLocation):
-- 60922 (Izzy, area 4765) failed with INCORRECT_AREA where the car is boarded.
-- Add the auras directly (no cast, no area check); stack 0 = add once, never refresh.
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=34840 AND `id` IN (9,10,11);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(34840,0,9,0,27,0,100,0,0,0,0,0,75,49416,0,0,0,0,0,1,0,0,0,0,0,0,0,'Hot Rod - On Passenger Boarded - Add Aura Generic Quest Invisibility Detection 1 (Ace)'),
(34840,0,10,0,27,0,100,0,0,0,0,0,75,49417,0,0,0,0,0,1,0,0,0,0,0,0,0,'Hot Rod - On Passenger Boarded - Add Aura Generic Quest Invisibility Detection 2 (Gobber)'),
(34840,0,11,0,27,0,100,0,0,0,0,0,75,60922,0,0,0,0,0,1,0,0,0,0,0,0,0,'Hot Rod - On Passenger Boarded - Add Aura Generic Quest Invisibility Detection 3 (Izzy)');
