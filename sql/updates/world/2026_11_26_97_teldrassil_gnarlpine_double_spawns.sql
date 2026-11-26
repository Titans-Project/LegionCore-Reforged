-- Teldrassil, Gnarlpine camp: second spawns standing 0.6-4.5 yd from a twin of the same creature
-- (2006 Gnarlpine Ursa: 77877; 2008 Gnarlpine Warrior: the rest). MoP 5.4.8 has no such pairs.
DELETE FROM `creature` WHERE `guid` IN (77877,77878,77879,77880,77881,77882,77883,77886,77887) AND `id` IN (2006,2008) AND `map`=1;
