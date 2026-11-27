-- Volunteer Guard Day: Silvermoon City Guardians take volunteers like the other capitals' guards
UPDATE `creature_template` SET `ScriptName` = 'npc_volunteer_guard_day' WHERE `entry` = 16222 AND `ScriptName` = '';
