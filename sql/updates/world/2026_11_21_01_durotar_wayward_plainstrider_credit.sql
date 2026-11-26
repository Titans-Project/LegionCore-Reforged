-- 25194 "Unbidden Visitors": Wayward Plainstrider 39337 gives its credit and flees at 1-30% health (MoP 5.4.8 world DB),
-- not 0-15%: a hit that took it from above 15% straight to death skipped the window and gave no credit.
UPDATE `smart_scripts` SET `event_param1`=1, `event_param2`=30 WHERE `entryorguid`=39337 AND `source_type`=0 AND `id`=2 AND `event_type`=2;
