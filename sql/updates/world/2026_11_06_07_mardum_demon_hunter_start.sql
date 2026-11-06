-- Mardum, the demon hunter starting zone, part 1 of the introduction: invasion, ships, phases, patrols and flights, quests, Beliash and Caza, the Soul Engine and its road, the Illidari Outpost, the Fel Hammer and Tyranna

-- Mardum, "The Invasion Begins": the arrival scene plays once, the Illidari offer the quest when it is over, then charge toward the battlefield after their lines
UPDATE `quest_template_addon` SET `PrevQuestID` = 40076 WHERE `ID` = 40077;
UPDATE `spell_area` SET `quest_end` = 40076, `quest_end_status` = 1 WHERE `spell` = 193525 AND `area` = 7705 AND `quest_end` IN (40076, 40077);
UPDATE `creature` SET `MovementType` = 0 WHERE `guid` IN (367297, 367298);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 7705 AND `SourceEntry` = 3 AND `ElseGroup` = 1 AND `ConditionTypeOrReference` = 8 AND `ConditionValue1` = 40076;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(23, 7705, 3, 0, 1, 8, 0, 40076, 0, 0, 0, 0, '', 'Legion. Illidari group once Illidan has spoken (40076 done)');
DELETE FROM `waypoint_data` WHERE `id` BETWEEN 10267101 AND 10267112;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267107, 1, 1179.57, 3202.61, 51.43, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 2, 1176.50, 3199.50, 50.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 3, 1173.50, 3196.50, 50.44, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 4, 1169.50, 3195.50, 49.16, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 5, 1166.50, 3191.50, 47.68, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 6, 1163.50, 3188.50, 47.58, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 7, 1161.50, 3184.50, 47.22, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 8, 1162.50, 3180.50, 45.05, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 9, 1166.50, 3178.50, 42.99, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 10, 1170.50, 3178.50, 41.99, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 11, 1174.50, 3176.50, 39.94, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 12, 1175.50, 3172.50, 37.03, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 13, 1171.50, 3170.50, 34.51, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 14, 1167.50, 3170.50, 34.10, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 15, 1163.50, 3170.50, 30.99, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 16, 1159.50, 3170.50, 29.09, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 17, 1155.50, 3170.50, 32.47, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 18, 1152.50, 3173.50, 34.56, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 19, 1149.50, 3176.50, 34.80, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 20, 1145.50, 3176.50, 34.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 21, 1141.50, 3176.50, 34.40, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 22, 1137.50, 3177.50, 33.73, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 23, 1133.50, 3177.50, 32.13, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 24, 1130.50, 3174.50, 29.62, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 25, 1126.50, 3173.50, 27.81, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267107, 26, 1123.50, 3173.50, 26.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 1, 1177.00, 3203.07, 51.45, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 2, 1174.50, 3200.50, 51.48, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 3, 1171.50, 3197.50, 50.44, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 4, 1167.50, 3194.50, 49.16, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 5, 1165.50, 3190.50, 47.68, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 6, 1162.50, 3187.50, 47.39, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 7, 1161.50, 3183.50, 47.22, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 8, 1164.50, 3179.50, 44.45, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 9, 1168.50, 3178.50, 42.49, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 10, 1172.50, 3178.50, 41.49, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 11, 1175.50, 3175.50, 39.08, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 12, 1175.50, 3171.50, 36.25, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 13, 1171.50, 3170.50, 34.51, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 14, 1167.50, 3170.50, 34.10, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 15, 1163.50, 3170.50, 30.99, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 16, 1159.50, 3170.50, 29.09, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 17, 1155.50, 3170.50, 32.47, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 18, 1152.50, 3173.50, 34.56, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 19, 1149.50, 3176.50, 34.80, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 20, 1145.50, 3176.50, 34.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 21, 1141.50, 3176.50, 34.40, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 22, 1137.50, 3177.50, 33.73, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 23, 1133.50, 3177.50, 32.13, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 24, 1130.50, 3174.50, 29.62, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 25, 1126.50, 3173.50, 27.81, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 26, 1122.50, 3173.50, 26.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267109, 27, 1121.50, 3173.50, 26.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 1, 1182.36, 3202.91, 51.52, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 2, 1179.50, 3199.50, 50.75, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 3, 1176.50, 3196.50, 50.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 4, 1172.50, 3196.50, 50.44, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 5, 1168.50, 3195.50, 49.16, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 6, 1166.50, 3191.50, 47.68, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 7, 1163.50, 3188.50, 47.58, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 8, 1162.50, 3184.50, 47.38, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 9, 1162.50, 3180.50, 45.05, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 10, 1166.50, 3178.50, 42.99, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 11, 1170.50, 3178.50, 41.99, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 12, 1174.50, 3176.50, 39.94, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 13, 1175.50, 3172.50, 37.03, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 14, 1171.50, 3170.50, 34.51, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 15, 1167.50, 3170.50, 34.10, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 16, 1163.50, 3170.50, 30.99, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 17, 1159.50, 3170.50, 29.09, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 18, 1155.50, 3170.50, 32.47, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 19, 1152.50, 3173.50, 34.56, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 20, 1149.50, 3176.50, 34.80, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 21, 1145.50, 3176.50, 34.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 22, 1141.50, 3176.50, 34.40, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 23, 1137.50, 3177.50, 33.73, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 24, 1133.50, 3177.50, 32.13, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 25, 1130.50, 3174.50, 29.62, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267108, 26, 1126.50, 3173.50, 27.81, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 1, 1171.49, 3203.69, 51.31, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 2, 1168.50, 3200.50, 51.11, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 3, 1165.50, 3197.50, 49.93, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 4, 1162.50, 3194.50, 48.76, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 5, 1161.50, 3190.50, 47.14, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 6, 1161.50, 3186.50, 47.20, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 7, 1161.50, 3182.50, 46.54, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 8, 1163.50, 3178.50, 43.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 9, 1167.50, 3178.50, 42.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 10, 1171.50, 3178.50, 41.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 11, 1175.50, 3175.50, 39.08, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 12, 1175.50, 3171.50, 36.25, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 13, 1171.50, 3170.50, 34.51, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 14, 1167.50, 3170.50, 34.10, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 15, 1163.50, 3170.50, 30.99, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 16, 1159.50, 3170.50, 29.09, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 17, 1155.50, 3170.50, 32.47, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 18, 1152.50, 3173.50, 34.56, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 19, 1149.50, 3176.50, 34.80, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 20, 1145.50, 3176.50, 34.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 21, 1141.50, 3176.50, 34.40, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 22, 1137.50, 3177.50, 33.73, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 23, 1133.50, 3177.50, 32.13, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 24, 1130.50, 3174.50, 29.62, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 25, 1126.50, 3173.50, 27.81, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 26, 1122.50, 3173.50, 26.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267112, 27, 1118.50, 3173.50, 27.88, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 1, 1170.74, 3204.71, 51.63, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 2, 1167.50, 3201.50, 51.06, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 3, 1164.50, 3198.50, 49.73, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 4, 1161.50, 3195.50, 48.35, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 5, 1161.50, 3191.50, 47.12, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 6, 1161.50, 3187.50, 47.18, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 7, 1161.50, 3183.50, 47.22, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 8, 1163.50, 3179.50, 44.70, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 9, 1167.50, 3178.50, 42.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 10, 1171.50, 3178.50, 41.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 11, 1175.50, 3175.50, 39.08, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 12, 1175.50, 3171.50, 36.25, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 13, 1171.50, 3170.50, 34.51, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 14, 1167.50, 3170.50, 34.10, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 15, 1163.50, 3170.50, 30.99, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 16, 1159.50, 3171.50, 35.92, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 17, 1155.50, 3171.50, 33.67, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 18, 1151.50, 3174.50, 34.65, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 19, 1147.50, 3176.50, 34.70, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 20, 1143.50, 3176.50, 34.50, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 21, 1139.50, 3176.50, 34.23, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 22, 1135.50, 3177.50, 32.93, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 23, 1131.50, 3175.50, 30.73, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 24, 1127.50, 3174.50, 27.86, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 25, 1123.50, 3173.50, 26.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 26, 1119.50, 3173.50, 28.27, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267111, 27, 1118.50, 3173.50, 27.88, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 1, 1172.92, 3207.82, 52.48, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 2, 1169.50, 3204.50, 52.25, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 3, 1166.50, 3201.50, 50.77, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 4, 1163.50, 3198.50, 49.45, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 5, 1161.50, 3194.50, 48.35, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 6, 1161.50, 3190.50, 47.14, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 7, 1161.50, 3186.50, 47.20, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 8, 1161.50, 3182.50, 46.54, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 9, 1163.50, 3178.50, 43.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 10, 1167.50, 3178.50, 42.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 11, 1171.50, 3178.50, 41.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 12, 1175.50, 3175.50, 39.08, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 13, 1175.50, 3171.50, 36.25, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 14, 1171.50, 3170.50, 34.51, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 15, 1167.50, 3170.50, 34.10, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 16, 1163.50, 3170.50, 30.99, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 17, 1159.50, 3170.50, 29.09, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 18, 1155.50, 3170.50, 32.47, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 19, 1152.50, 3173.50, 34.56, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 20, 1149.50, 3176.50, 34.80, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 21, 1145.50, 3176.50, 34.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 22, 1141.50, 3176.50, 34.40, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 23, 1137.50, 3177.50, 33.73, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 24, 1133.50, 3177.50, 32.13, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 25, 1130.50, 3174.50, 29.62, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 26, 1126.50, 3173.50, 27.81, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267110, 27, 1122.50, 3173.50, 26.74, 0, 0, 0, 1, 0, 0, 100, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` IN (367295, 367297, 367298);
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES (367295, 1, 333), (367297, 1, 333), (367298, 1, 333);
-- the departure paths stop at the stream's bank; the run goes on, after a leap, from the far bank (path + 100)
DELETE FROM `waypoint_data` WHERE `id` BETWEEN 10267207 AND 10267212;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) SELECT 10267207, `point` - 17, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid` FROM `waypoint_data` WHERE `id` = 10267107 AND `point` >= 18;
DELETE FROM `waypoint_data` WHERE `id` = 10267107 AND `point` > 14;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) SELECT 10267208, `point` - 18, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid` FROM `waypoint_data` WHERE `id` = 10267108 AND `point` >= 19;
DELETE FROM `waypoint_data` WHERE `id` = 10267108 AND `point` > 15;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) SELECT 10267209, `point` - 17, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid` FROM `waypoint_data` WHERE `id` = 10267109 AND `point` >= 18;
DELETE FROM `waypoint_data` WHERE `id` = 10267109 AND `point` > 14;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) SELECT 10267210, `point` - 18, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid` FROM `waypoint_data` WHERE `id` = 10267110 AND `point` >= 19;
DELETE FROM `waypoint_data` WHERE `id` = 10267110 AND `point` > 15;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) SELECT 10267211, `point` - 17, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid` FROM `waypoint_data` WHERE `id` = 10267111 AND `point` >= 18;
DELETE FROM `waypoint_data` WHERE `id` = 10267111 AND `point` > 14;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) SELECT 10267212, `point` - 17, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid` FROM `waypoint_data` WHERE `id` = 10267112 AND `point` >= 18;
DELETE FROM `waypoint_data` WHERE `id` = 10267112 AND `point` > 14;
-- the banner to plant is Blizzard's Legion banner (250560); once planted, the same banner stays in its place
UPDATE `gameobject_template` SET `ScriptName` = 'go_q40077' WHERE `entry` = 250560;
UPDATE `gameobject_template` SET `ScriptName` = '' WHERE `entry` = 244898;
UPDATE `gameobject` SET `id` = 250560, `PhaseId` = '5310' WHERE `guid` = 131210;
DELETE FROM `gameobject` WHERE `guid` = 600200;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(600200, 250560, 1481, 7705, 7741, 1, 1, '5305', 1002.7, 2955.81, -10.5557, 1.55481, 0, 0, 0, 1, 180, 255, 0);
UPDATE `quest_objectives` SET `ObjectID` = 250560 WHERE `ID` = 280292;
DELETE FROM `locales_gameobject` WHERE `entry` IN (244898, 250560);
INSERT INTO `locales_gameobject` (`entry`, `name_loc2`, `castbarcaption_loc2`) VALUES (250560, 'Bannière de la Légion', 'Remplacement');
-- phase 7018 shows the banner to plant until it is planted, and the phase of the planted
-- banner (7001) waits for the new objective object
UPDATE `conditions` SET `ConditionValue2` = 250560 WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 7705 AND `SourceEntry` = 4 AND `ConditionTypeOrReference` = 41 AND `ConditionValue2` = 244898;
DELETE FROM `phase_definitions` WHERE `zoneId` = 7705 AND `entry` = 26;
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `PreloadMapID`, `VisibleMapID`, `UiWorldMapAreaID`, `flags`, `comment`) VALUES (7705, 26, 0, '7018', 0, 0, 0, 16, 'Legion. Legion banner not planted yet');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 7705 AND `SourceEntry` = 26;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(23, 7705, 26, 0, 1, 14, 0, 40378, 0, 0, 0, 0, '', 'Legion banner: area not cleared yet'),
(23, 7705, 26, 0, 1, 41, 0, 40077, 250560, 0, 1, 0, '', 'Legion banner: not planted yet');
UPDATE `gameobject` SET `PhaseId` = '7018' WHERE `guid` = 131210;
UPDATE `gameobject` SET `PhaseId` = '7001 5305' WHERE `guid` = 600200;
-- Kayn's gossip about Mardum opens its page, which leads back to the first one
UPDATE `gossip_menu_option` SET `ActionMenuID` = 18993 WHERE `MenuID` = 18864 AND `OptionID` = 0;
UPDATE `gossip_menu_option` SET `ActionMenuID` = 18864 WHERE `MenuID` = 18993 AND `OptionID` = 0;

-- Mardum: the Legion Command Center and a Legion ship hang high over the battlefield, seen from afar; the ship's cannon fires at the ground ahead, where big fel bursts break out, harmlessly
UPDATE `creature_template` SET `MaxVisible` = 1, `VisibilityDistance` = 0 WHERE `entry` = 98191;
DELETE FROM `creature_movement_override` WHERE `SpawnId` = 367279;
INSERT INTO `creature_movement_override` (`SpawnId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Random`) VALUES (367279, 0, 0, 1, 1, 0);
UPDATE `creature_template` SET `ScriptName` = 'npc_mardum_legion_ship', `MaxVisible` = 1, `VisibilityDistance` = 533 WHERE `entry` = 104842;
DELETE FROM `creature` WHERE `guid` IN (600000, 600001);
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`, `isActive`) VALUES
(600000, 104842, 1481, 7705, 7705, 1, 1, '5837 5324 5116 5115 5114', 65683, 1319.10, 3026.08, 253.58, 3.97, 120, 0, 0, 1);
DELETE FROM `creature_movement_override` WHERE `SpawnId` IN (600000, 600001);
INSERT INTO `creature_movement_override` (`SpawnId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Random`) VALUES (600000, 0, 0, 1, 1, 0), (600001, 0, 0, 1, 1, 0);
-- the ground spot: a copy of the invisible ELM General Purpose Bunny, seen from afar and five times its size
DELETE FROM `creature_template` WHERE `entry` = 600100;
DELETE FROM `creature_template_wdb` WHERE `Entry` = 600100;
DELETE FROM `creature_template_addon` WHERE `entry` = 600100;
DELETE FROM `creature_template_movement` WHERE `CreatureId` = 600100;
CREATE TEMPORARY TABLE `tmp_burst` SELECT * FROM `creature_template` WHERE `entry` = 23837;
UPDATE `tmp_burst` SET `entry` = 600100, `scale` = 10.2, `MaxVisible` = 1, `VisibilityDistance` = 533, `AIName` = '', `ScriptName` = '';
INSERT INTO `creature_template` SELECT * FROM `tmp_burst`;
DROP TEMPORARY TABLE `tmp_burst`;
CREATE TEMPORARY TABLE `tmp_burst` SELECT * FROM `creature_template_wdb` WHERE `Entry` = 23837;
UPDATE `tmp_burst` SET `Entry` = 600100, `Name1` = 'Mardum Fel Burst';
INSERT INTO `creature_template_wdb` SELECT * FROM `tmp_burst`;
DROP TEMPORARY TABLE `tmp_burst`;
CREATE TEMPORARY TABLE `tmp_burst` SELECT * FROM `creature_template_addon` WHERE `entry` = 23837;
UPDATE `tmp_burst` SET `entry` = 600100;
INSERT INTO `creature_template_addon` SELECT * FROM `tmp_burst`;
DROP TEMPORARY TABLE `tmp_burst`;
CREATE TEMPORARY TABLE `tmp_burst` SELECT * FROM `creature_template_movement` WHERE `CreatureId` = 23837;
UPDATE `tmp_burst` SET `CreatureId` = 600100;
INSERT INTO `creature_template_movement` SELECT * FROM `tmp_burst`;
DROP TEMPORARY TABLE `tmp_burst`;
-- the ship's cannon: the same invisible bunny eight times its size, so that its fel shot is seen from afar
DELETE FROM `creature_template` WHERE `entry` = 600101;
DELETE FROM `creature_template_wdb` WHERE `Entry` = 600101;
DELETE FROM `creature_template_addon` WHERE `entry` = 600101;
DELETE FROM `creature_template_movement` WHERE `CreatureId` = 600101;
CREATE TEMPORARY TABLE `tmp_burst` SELECT * FROM `creature_template` WHERE `entry` = 600100;
UPDATE `tmp_burst` SET `entry` = 600101, `scale` = 5;
INSERT INTO `creature_template` SELECT * FROM `tmp_burst`;
DROP TEMPORARY TABLE `tmp_burst`;
CREATE TEMPORARY TABLE `tmp_burst` SELECT * FROM `creature_template_wdb` WHERE `Entry` = 600100;
UPDATE `tmp_burst` SET `Entry` = 600101, `Name1` = 'Mardum Legion Ship Cannon';
INSERT INTO `creature_template_wdb` SELECT * FROM `tmp_burst`;
DROP TEMPORARY TABLE `tmp_burst`;
CREATE TEMPORARY TABLE `tmp_burst` SELECT * FROM `creature_template_addon` WHERE `entry` = 600100;
UPDATE `tmp_burst` SET `entry` = 600101;
INSERT INTO `creature_template_addon` SELECT * FROM `tmp_burst`;
DROP TEMPORARY TABLE `tmp_burst`;
CREATE TEMPORARY TABLE `tmp_burst` SELECT * FROM `creature_template_movement` WHERE `CreatureId` = 600100;
UPDATE `tmp_burst` SET `CreatureId` = 600101;
INSERT INTO `creature_template_movement` SELECT * FROM `tmp_burst`;
DROP TEMPORARY TABLE `tmp_burst`;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`, `isActive`) VALUES
(600001, 600101, 1481, 7705, 7705, 1, 1, '5837 5324 5116 5115 5114', 1179.79, 2850.19, 250.53, 2.84, 120, 0, 0, 1);
DELETE FROM `creature` WHERE `guid` BETWEEN 600002 AND 600019;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`, `isActive`) VALUES
(600002, 600100, 1481, 7705, 7705, 1, 1, '5837 5324 5116 5115 5114', 1399.00, 2705.00, 78.00, 0, 120, 0, 0, 1),
(600003, 600100, 1481, 7705, 7705, 1, 1, '5837 5324 5116 5115 5114', 1474.00, 2830.00, 72.40, 0, 120, 0, 0, 1),
(600004, 600100, 1481, 7705, 7705, 1, 1, '5837 5324 5116 5115 5114', 1389.00, 2780.00, 50.20, 0, 120, 0, 0, 1),
(600005, 600100, 1481, 7705, 7705, 1, 1, '5837 5324 5116 5115 5114', 1459.00, 2765.00, 72.80, 0, 120, 0, 0, 1),
(600006, 600100, 1481, 7705, 7705, 1, 1, '5837 5324 5116 5115 5114', 1110.00, 2878.00, 6.59, 0, 120, 0, 0, 1),
(600007, 600100, 1481, 7705, 7705, 1, 1, '5837 5324 5116 5115 5114', 1188.00, 2992.00, -46.96, 0, 120, 0, 0, 1),
(600008, 600100, 1481, 7705, 7705, 1, 1, '5837 5324 5116 5115 5114', 1364.00, 2760.00, 40.90, 0, 120, 0, 0, 1),
(600009, 600100, 1481, 7705, 7705, 1, 1, '5837 5324 5116 5115 5114', 1429.00, 2655.00, 94.50, 0, 120, 0, 0, 1),
(600010, 600100, 1481, 7705, 7705, 1, 1, '5837 5324 5116 5115 5114', 1514.00, 2750.00, 82.40, 0, 120, 0, 0, 1),
(600011, 600100, 1481, 7705, 7705, 1, 1, '5837 5324 5116 5115 5114', 1504.00, 2715.00, 91.20, 0, 120, 0, 0, 1),
(600012, 600100, 1481, 7705, 7705, 1, 1, '5837 5324 5116 5115 5114', 1092.00, 2932.00, 2.39, 0, 120, 0, 0, 1),
(600013, 600100, 1481, 7705, 7705, 1, 1, '5837 5324 5116 5115 5114', 1494.00, 2805.00, 73.60, 0, 120, 0, 0, 1);
-- the explosion markers: the same invisible bunny on every ground spot, smaller, for the shot's explosion
DELETE FROM `creature` WHERE `guid` BETWEEN 600020 AND 600031;
DELETE FROM `creature_template` WHERE `entry` = 600102;
DELETE FROM `creature_template_wdb` WHERE `Entry` = 600102;
DELETE FROM `creature_template_addon` WHERE `entry` = 600102;
DELETE FROM `creature_template_movement` WHERE `CreatureId` = 600102;
CREATE TEMPORARY TABLE `tmp_burst` SELECT * FROM `creature_template` WHERE `entry` = 600100;
UPDATE `tmp_burst` SET `entry` = 600102, `scale` = 1.2;
INSERT INTO `creature_template` SELECT * FROM `tmp_burst`;
DROP TEMPORARY TABLE `tmp_burst`;
CREATE TEMPORARY TABLE `tmp_burst` SELECT * FROM `creature_template_wdb` WHERE `Entry` = 600100;
UPDATE `tmp_burst` SET `Entry` = 600102, `Name1` = 'Mardum Fel Burst Explosion';
INSERT INTO `creature_template_wdb` SELECT * FROM `tmp_burst`;
DROP TEMPORARY TABLE `tmp_burst`;
CREATE TEMPORARY TABLE `tmp_burst` SELECT * FROM `creature_template_addon` WHERE `entry` = 600100;
UPDATE `tmp_burst` SET `entry` = 600102;
INSERT INTO `creature_template_addon` SELECT * FROM `tmp_burst`;
DROP TEMPORARY TABLE `tmp_burst`;
CREATE TEMPORARY TABLE `tmp_burst` SELECT * FROM `creature_template_movement` WHERE `CreatureId` = 600100;
UPDATE `tmp_burst` SET `CreatureId` = 600102;
INSERT INTO `creature_template_movement` SELECT * FROM `tmp_burst`;
DROP TEMPORARY TABLE `tmp_burst`;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`, `isActive`)
SELECT `guid` + 18, 600102, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`, `isActive` FROM `creature` WHERE `guid` BETWEEN 600002 AND 600013;
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_mardum_legion_ship_impact';

-- Mardum: Dread Felbats fly long irregular circuits above the battlefield
DELETE FROM `waypoint_data` WHERE `id` BETWEEN 10267300 AND 10267399;
DELETE FROM `creature_addon` WHERE `guid` IN (367280, 367281, 367282, 367401, 367418, 367419, 367420, 367580, 367592, 367595, 367596, 367597, 367611, 367789, 367821, 367828);
DELETE FROM `creature_movement_override` WHERE `SpawnId` IN (367280, 367281, 367282, 367401, 367418, 367419, 367420, 367580, 367592, 367595, 367596, 367597, 367611, 367789, 367821, 367828);
UPDATE `creature` SET `position_x` = 1137.22, `position_y` = 3094.03, `position_z` = 37.41, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367280;
UPDATE `creature` SET `position_x` = 1071.06, `position_y` = 3053.57, `position_z` = 39.90, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367281;
UPDATE `creature` SET `position_x` = 1025.18, `position_y` = 3101.70, `position_z` = 18.17, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367282;
UPDATE `creature` SET `position_x` = 1010.76, `position_y` = 2898.79, `position_z` = 19.63, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367401;
UPDATE `creature` SET `position_x` = 1020.30, `position_y` = 2756.14, `position_z` = 34.61, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367418;
UPDATE `creature` SET `position_x` = 1066.63, `position_y` = 2835.54, `position_z` = 28.37, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367419;
UPDATE `creature` SET `position_x` = 1064.39, `position_y` = 2782.32, `position_z` = 20.88, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367420;
UPDATE `creature` SET `position_x` = 788.19, `position_y` = 2480.04, `position_z` = -50.74, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367580;
UPDATE `creature` SET `position_x` = 830.64, `position_y` = 2326.77, `position_z` = -37.60, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367592;
UPDATE `creature` SET `position_x` = 838.46, `position_y` = 2221.95, `position_z` = -68.80, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367595;
UPDATE `creature` SET `position_x` = 861.06, `position_y` = 2546.13, `position_z` = -35.79, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367596;
UPDATE `creature` SET `position_x` = 945.44, `position_y` = 2569.20, `position_z` = -46.66, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367597;
UPDATE `creature` SET `position_x` = 734.44, `position_y` = 2359.09, `position_z` = -46.82, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367611;
UPDATE `creature` SET `position_x` = 1468.55, `position_y` = 2551.23, `position_z` = 54.95, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367789;
UPDATE `creature` SET `position_x` = 1652.37, `position_y` = 2496.72, `position_z` = 106.37, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367821;
UPDATE `creature` SET `position_x` = 1508.51, `position_y` = 2541.38, `position_z` = 69.23, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367828;
INSERT INTO `creature_movement_override` (`SpawnId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Random`) VALUES
(367280, 0, 0, 1, 0, 0),
(367281, 0, 0, 1, 0, 0),
(367282, 0, 0, 1, 0, 0),
(367401, 0, 0, 1, 0, 0),
(367418, 0, 0, 1, 0, 0),
(367419, 0, 0, 1, 0, 0),
(367420, 0, 0, 1, 0, 0),
(367580, 0, 0, 1, 0, 0),
(367592, 0, 0, 1, 0, 0),
(367595, 0, 0, 1, 0, 0),
(367596, 0, 0, 1, 0, 0),
(367597, 0, 0, 1, 0, 0),
(367611, 0, 0, 1, 0, 0),
(367789, 0, 0, 1, 0, 0),
(367821, 0, 0, 1, 0, 0),
(367828, 0, 0, 1, 0, 0);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(367280, 10267300, 0, 50331648, 1, 0, ''),
(367281, 10267301, 0, 50331648, 1, 0, ''),
(367282, 10267302, 0, 50331648, 1, 0, ''),
(367401, 10267303, 0, 50331648, 1, 0, ''),
(367418, 10267304, 0, 50331648, 1, 0, ''),
(367419, 10267305, 0, 50331648, 1, 0, ''),
(367420, 10267306, 0, 50331648, 1, 0, ''),
(367580, 10267307, 0, 50331648, 1, 0, ''),
(367592, 10267308, 0, 50331648, 1, 0, ''),
(367595, 10267309, 0, 50331648, 1, 0, ''),
(367596, 10267310, 0, 50331648, 1, 0, ''),
(367597, 10267311, 0, 50331648, 1, 0, ''),
(367611, 10267312, 0, 50331648, 1, 0, ''),
(367789, 10267313, 0, 50331648, 1, 0, ''),
(367821, 10267314, 0, 50331648, 1, 0, ''),
(367828, 10267315, 0, 50331648, 1, 0, '');
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267300, 1, 1137.22, 3094.03, 37.41, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267300, 2, 972.07, 3109.36, 21.17, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267300, 3, 917.38, 3090.97, 8.71, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267300, 4, 848.98, 3061.35, -0.06, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267300, 5, 908.64, 2923.28, -8.32, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267300, 6, 943.71, 2959.26, 1.38, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267300, 7, 1020.53, 3009.84, 12.47, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267301, 1, 1071.06, 3053.57, 39.90, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267301, 2, 1164.07, 3122.03, 27.04, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267301, 3, 1248.28, 3243.43, 74.90, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267301, 4, 1220.79, 3302.37, 100.58, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267301, 5, 1180.59, 3293.27, 85.62, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267301, 6, 1150.93, 3257.48, 75.57, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267301, 7, 1122.58, 3221.09, 63.49, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267301, 8, 1092.24, 3144.78, 45.35, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267302, 1, 1025.18, 3101.70, 18.17, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267302, 2, 962.68, 3124.23, 12.66, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267302, 3, 937.44, 3096.86, 6.56, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267302, 4, 1029.69, 2973.46, 21.93, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267302, 5, 1058.52, 2928.81, 27.75, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267302, 6, 1125.26, 2887.92, 12.47, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267302, 7, 1094.35, 2842.19, 24.86, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267302, 8, 1064.58, 2981.23, 29.96, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267303, 1, 1010.76, 2898.79, 19.63, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267303, 2, 1080.14, 2955.93, 18.44, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267303, 3, 1127.45, 2988.78, 9.95, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267303, 4, 1186.55, 3140.64, 29.38, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267303, 5, 1064.30, 3184.93, 68.81, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267303, 6, 1018.67, 3152.13, 61.07, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267303, 7, 1070.28, 3138.76, 48.41, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267303, 8, 1043.08, 3059.43, 15.92, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267303, 9, 1024.44, 2938.97, 20.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267304, 1, 1020.30, 2756.14, 34.61, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267304, 2, 974.59, 2727.94, 48.65, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267304, 3, 907.87, 2752.50, 24.52, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267304, 4, 849.28, 2867.17, -4.85, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267304, 5, 871.90, 2934.30, -5.30, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267304, 6, 955.76, 2811.63, 28.58, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267305, 1, 1066.63, 2835.54, 28.37, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267305, 2, 1107.15, 2850.95, 25.62, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267305, 3, 1097.68, 2908.09, 25.12, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267305, 4, 1082.43, 2972.58, 30.44, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267305, 5, 1045.99, 3000.53, 26.86, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267305, 6, 978.31, 3058.05, -0.37, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267305, 7, 998.64, 3000.53, 6.95, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267305, 8, 1010.76, 2898.79, 19.63, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267306, 1, 1064.39, 2782.32, 20.88, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267306, 2, 979.33, 2763.44, 39.26, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267306, 3, 895.47, 2826.49, 16.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267306, 4, 854.38, 2867.11, -5.49, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267306, 5, 881.37, 2932.86, -8.53, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267306, 6, 981.41, 2868.50, 12.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267306, 7, 1007.84, 2832.50, 24.87, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267306, 8, 1021.56, 2796.59, 23.61, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267307, 1, 788.19, 2480.04, -50.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267307, 2, 740.21, 2542.78, -48.70, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267307, 3, 720.02, 2625.67, -43.72, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267307, 4, 782.63, 2700.41, -8.73, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267307, 5, 719.02, 2790.54, -42.22, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267307, 6, 733.46, 2830.14, -47.89, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267307, 7, 793.76, 2869.48, -27.11, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267307, 8, 777.63, 2702.06, -17.63, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267307, 9, 773.37, 2626.40, -18.23, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267307, 10, 788.45, 2533.73, -44.55, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267308, 1, 830.64, 2326.77, -37.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267308, 2, 770.88, 2335.17, -30.67, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267308, 3, 709.64, 2362.71, -47.50, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267308, 4, 666.33, 2506.39, -47.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267308, 5, 760.44, 2609.09, -32.54, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267308, 6, 782.51, 2476.10, -42.58, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267308, 7, 795.26, 2376.66, -38.16, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267309, 1, 838.46, 2221.95, -68.80, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267309, 2, 875.35, 2218.49, -62.49, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267309, 3, 966.14, 2202.37, -50.28, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267309, 4, 986.65, 2173.92, -36.47, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267309, 5, 968.61, 2143.81, -24.23, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267309, 6, 916.49, 2147.23, -21.50, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267309, 7, 776.78, 2109.46, -75.08, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267310, 1, 861.06, 2546.13, -35.79, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267310, 2, 783.25, 2502.43, -47.87, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267310, 3, 669.10, 2554.58, -60.58, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267310, 4, 633.04, 2561.17, -62.21, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267310, 5, 566.07, 2482.93, -43.93, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267310, 6, 614.70, 2437.57, -53.18, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267310, 7, 729.53, 2419.12, -46.99, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267310, 8, 838.69, 2543.47, -47.12, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267310, 9, 855.47, 2510.17, -41.31, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267311, 1, 945.44, 2569.20, -46.66, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267311, 2, 1019.03, 2600.17, -35.17, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267311, 3, 1043.61, 2540.44, -16.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267311, 4, 938.68, 2443.35, -58.46, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267311, 5, 866.97, 2433.36, -39.45, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267311, 6, 781.62, 2505.33, -50.19, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267311, 7, 828.19, 2511.37, -44.32, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267311, 8, 926.53, 2538.78, -48.62, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267312, 1, 734.44, 2359.09, -46.82, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267312, 2, 761.90, 2293.56, -54.04, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267312, 3, 719.58, 2246.17, -46.72, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267312, 4, 601.72, 2153.69, -52.70, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267312, 5, 632.25, 2111.52, -66.59, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267312, 6, 667.45, 2104.76, -69.27, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267312, 7, 712.53, 2103.35, -68.42, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267312, 8, 812.01, 2105.68, -75.35, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267312, 9, 821.96, 2271.77, -59.39, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267312, 10, 767.25, 2338.51, -34.30, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267313, 1, 1468.55, 2551.23, 54.95, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267313, 2, 1558.90, 2469.89, 73.29, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267313, 3, 1460.00, 2387.11, 79.67, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267313, 4, 1475.11, 2346.79, 95.77, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267313, 5, 1445.99, 2297.52, 104.46, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267313, 6, 1436.26, 2259.61, 117.78, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267313, 7, 1376.02, 2239.59, 100.41, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267313, 8, 1379.57, 2291.59, 90.84, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267313, 9, 1433.29, 2436.86, 69.58, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267314, 1, 1652.37, 2496.72, 106.37, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267314, 2, 1615.35, 2462.26, 119.64, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267314, 3, 1580.73, 2417.18, 102.30, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267314, 4, 1529.25, 2473.14, 76.03, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267314, 5, 1455.64, 2527.64, 56.51, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267314, 6, 1479.63, 2578.14, 54.77, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267314, 7, 1471.71, 2629.51, 53.47, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267314, 8, 1512.49, 2587.26, 75.37, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267314, 9, 1568.45, 2563.47, 80.96, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267314, 10, 1659.02, 2542.44, 102.63, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267315, 1, 1508.51, 2541.38, 69.23, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267315, 2, 1548.41, 2561.52, 64.02, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267315, 3, 1548.50, 2650.97, 43.42, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267315, 4, 1554.69, 2697.53, 46.47, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267315, 5, 1548.67, 2753.17, 40.56, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267315, 6, 1573.88, 2789.01, 24.29, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267315, 7, 1585.89, 2751.37, 27.26, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267315, 8, 1593.32, 2699.01, 39.00, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267315, 9, 1548.28, 2614.51, 61.04, 0, 0, 0, 1, 0, 0, 100, 0, 0);

-- Mardum's demons, Illidari and critters alive until the Legion banner is planted and dead after: each version only in its own phase
UPDATE `creature` SET `PhaseId` = '5094' WHERE `guid` IN (367502, 367503, 367509, 367510, 367512, 367517, 367519, 367520, 367521, 367522, 367524, 367525, 367526, 367527, 367528, 367536, 367537, 367538, 367539, 367540, 367541, 367542, 367545, 367546, 367547, 367549);
UPDATE `creature` SET `PhaseId` = '5305' WHERE `guid` IN (367285, 367286, 367287, 367290, 367294, 367431, 367432, 367433, 367435, 367436, 367439, 367440, 367441, 367442, 367443, 367444, 367445, 367449, 367450, 367451, 367454, 367455, 367456, 367457, 367458, 367459, 367460, 367461, 367462, 367463, 367464, 367466, 367469, 367472, 367473, 367475, 367476, 367477, 367478, 367480, 367484, 367488, 367490, 367499, 367500);
UPDATE `creature` SET `PhaseId` = '5310' WHERE `guid` IN (367303, 367305, 367306, 367307, 367308, 367309, 367310, 367311, 367312, 367314, 367315, 367317, 367318, 367320, 367321, 367322, 367323, 367327, 367329, 367330, 367331, 367333, 367334, 367335, 367338, 367339, 367340, 367341, 367342, 367343, 367344, 367345, 367346, 367347, 367348, 367350, 367351, 367354, 367357, 367358, 367359, 367360, 367361, 367362, 367364, 367366, 367368, 367369, 367370, 367374, 367375, 367377, 367379, 367380, 367381, 367382, 367383, 367386, 367387, 367388, 367392, 367396, 367397, 367398, 367399, 367400, 367402, 367403, 367404, 367405, 367406, 367407, 367408, 367409, 367410, 367411, 367412, 367413, 367414, 367415, 367416, 367417, 367426, 367428, 367430);
UPDATE `creature` SET `PhaseId` = '5310' WHERE `guid` IN (367304, 367313, 367316, 367319, 367324, 367325, 367326, 367328, 367332, 367336, 367337, 367349, 367352, 367353, 367355, 367356, 367363, 367365, 367367, 367371, 367372, 367373, 367376, 367378, 367384, 367385, 367389, 367390, 367391, 367393, 367394, 367395, 367401, 367418, 367419, 367420, 367421, 367422, 367423, 367424, 367425, 367427, 367429);
UPDATE `creature` SET `PhaseId` = '5305' WHERE `guid` IN (367434, 367437, 367438, 367446, 367447, 367452, 367453, 367465, 367467, 367468, 367470, 367471, 367474, 367479, 367481, 367482, 367483, 367485);
UPDATE `gameobject` SET `PhaseId` = '5310' WHERE `guid` IN (131211, 131212);
UPDATE `gameobject` SET `PhaseId` = '5305' WHERE `guid` = 131209;
-- the dead felguard butcher replaces the living one once quest 39495 is taken
DELETE FROM `phase_definitions` WHERE `zoneId` = 7705 AND `entry` = 27;
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `PreloadMapID`, `VisibleMapID`, `UiWorldMapAreaID`, `flags`, `comment`) VALUES (7705, 27, 0, '7019', 0, 0, 0, 16, 'Legion. Felguard butcher dead once 39495 is taken');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 7705 AND `SourceEntry` = 27;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(23, 7705, 27, 0, 1, 14, 0, 39495, 0, 0, 1, 0, '', 'Felguard butcher dead once 39495 is taken');
DELETE FROM `creature` WHERE `guid` = 368136;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `AiID`, `MovementID`, `MeleeID`, `isActive`, `skipClone`, `personal_size`, `isTeemingSpawn`, `unit_flags3`) VALUES
(368136, 97598, 1481, 7705, 7754, 1, 1, '7019', 0, 0, 1204.77, 1676.11, 92.7669, 5.43331, 180, 0, 0, 0, 0, 0, 0, 0, 536904448, 32, 0, 0, 0, 0, 0, 0, 0, 0);
-- the battlefield is cleared when the Legion banner is planted, behind the scene
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 7705 AND `SourceEntry` IN (18, 19);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(23, 7705, 18, 0, 1, 41, 0, 40077, 250560, 1, 1, 0, '', 'Battlefield alive: Legion banner not planted'),
(23, 7705, 18, 0, 1, 8, 0, 40077, 0, 0, 1, 0, '', 'Battlefield alive: 40077 not rewarded'),
(23, 7705, 19, 0, 1, 41, 0, 40077, 250560, 1, 0, 0, '', 'Battlefield cleared: Legion banner planted'),
(23, 7705, 19, 0, 2, 8, 0, 40077, 0, 0, 0, 0, '', 'Battlefield cleared: 40077 rewarded');
UPDATE `phase_definitions` SET `comment` = 'Legion. Battlefield alive until the Legion banner is planted' WHERE `zoneId` = 7705 AND `entry` = 18;
UPDATE `phase_definitions` SET `comment` = 'Legion. Battlefield cleared once the Legion banner is planted' WHERE `zoneId` = 7705 AND `entry` = 19;

-- Mardum: demons of the battlefield patrol along the roads, clear of the fel pools, of the road ends and of the Legion banner's platform, with points wherever the ground dips
DELETE FROM `waypoint_data` WHERE `id` BETWEEN 10267400 AND 10267409;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267400, 1, 996.00, 3038.00, -11.96, 0, 5000, 0, 0, 0, 0, 100, 0, 0),
(10267400, 2, 993.50, 3038.00, -12.61, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 3, 986.00, 3038.00, -12.49, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 4, 982.00, 3038.00, -13.19, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 5, 980.29, 3036.86, -13.72, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 6, 979.86, 3036.57, -14.40, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 7, 978.14, 3035.43, -14.52, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 8, 977.71, 3035.14, -15.20, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 9, 970.00, 3030.00, -15.38, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 10, 964.26, 3024.26, -15.58, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 11, 963.90, 3023.90, -16.75, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 12, 956.00, 3016.00, -17.60, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 13, 944.00, 3004.00, -18.35, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 14, 938.00, 3000.00, -18.73, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 15, 930.00, 2996.00, -19.47, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 16, 926.00, 2996.00, -19.77, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 17, 920.00, 2990.00, -19.70, 0, 4000, 0, 0, 0, 0, 100, 0, 0),
(10267400, 18, 926.00, 2996.00, -19.77, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 19, 930.00, 2996.00, -19.47, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 20, 938.00, 3000.00, -18.73, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 21, 944.00, 3004.00, -18.35, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 22, 956.00, 3016.00, -17.60, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 23, 963.90, 3023.90, -16.75, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 24, 964.26, 3024.26, -15.58, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 25, 970.00, 3030.00, -15.38, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 26, 977.71, 3035.14, -15.20, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 27, 978.14, 3035.43, -14.52, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 28, 979.86, 3036.57, -14.40, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 29, 980.29, 3036.86, -13.72, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 30, 982.00, 3038.00, -13.19, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 31, 986.00, 3038.00, -12.49, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267400, 32, 993.50, 3038.00, -12.61, 0, 0, 0, 0, 0, 0, 100, 0, 0);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267401, 1, 997.03, 3053.00, -9.27, 0, 5000, 0, 0, 0, 0, 100, 0, 0),
(10267401, 2, 997.65, 3053.80, -9.95, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 3, 1000.11, 3056.98, -8.35, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 4, 1005.34, 3063.76, -6.78, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 5, 1007.18, 3066.15, -5.41, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 6, 1011.49, 3071.72, -4.98, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 7, 1014.57, 3075.71, -3.58, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 8, 1014.87, 3076.10, -2.85, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 9, 1015.80, 3077.30, -2.85, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 10, 1016.41, 3078.10, -1.33, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 11, 1019.49, 3082.08, -1.08, 0, 4000, 0, 0, 0, 0, 100, 0, 0),
(10267401, 12, 1016.41, 3078.10, -1.33, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 13, 1015.80, 3077.30, -2.85, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 14, 1014.87, 3076.10, -2.85, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 15, 1014.57, 3075.71, -3.58, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 16, 1011.49, 3071.72, -4.98, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 17, 1007.18, 3066.15, -5.41, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 18, 1005.34, 3063.76, -6.78, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 19, 1000.11, 3056.98, -8.35, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267401, 20, 997.65, 3053.80, -9.95, 0, 0, 0, 0, 0, 0, 100, 0, 0);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267402, 1, 1003.00, 3034.00, -11.81, 0, 5000, 0, 0, 0, 0, 100, 0, 0),
(10267402, 2, 1003.00, 3029.98, -12.42, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267402, 3, 1003.00, 3006.36, -13.07, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267402, 4, 1003.00, 2984.25, -12.00, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267402, 5, 1003.00, 2973.70, -10.68, 0, 4000, 0, 0, 0, 0, 100, 0, 0),
(10267402, 6, 1003.00, 2984.25, -12.00, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267402, 7, 1003.00, 3006.36, -13.07, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267402, 8, 1003.00, 3029.98, -12.42, 0, 0, 0, 0, 0, 0, 100, 0, 0);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267403, 1, 972.00, 2894.00, -4.74, 0, 5000, 0, 0, 0, 0, 100, 0, 0),
(10267403, 2, 964.00, 2896.00, -6.74, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 3, 958.00, 2900.00, -8.94, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 4, 957.52, 2900.19, -9.57, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 5, 954.19, 2901.52, -10.19, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 6, 953.71, 2901.71, -10.82, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 7, 950.38, 2903.05, -11.57, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 8, 949.90, 2903.24, -12.19, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 9, 948.00, 2904.00, -12.19, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 10, 947.50, 2904.12, -12.82, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 11, 940.00, 2906.00, -14.69, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 12, 939.57, 2905.73, -15.32, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 13, 931.78, 2900.86, -16.75, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 14, 930.49, 2900.05, -16.75, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 15, 930.05, 2899.78, -16.10, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 16, 927.89, 2898.43, -16.85, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 17, 927.03, 2897.89, -16.20, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 18, 924.00, 2896.00, -16.49, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 19, 923.50, 2895.83, -15.86, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 20, 922.00, 2895.33, -15.86, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 21, 921.50, 2895.17, -16.56, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 22, 918.00, 2894.00, -17.26, 0, 4000, 0, 0, 0, 0, 100, 0, 0),
(10267403, 23, 921.50, 2895.17, -16.56, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 24, 922.00, 2895.33, -15.86, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 25, 923.50, 2895.83, -15.86, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 26, 924.00, 2896.00, -16.49, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 27, 927.03, 2897.89, -16.20, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 28, 927.89, 2898.43, -16.85, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 29, 930.05, 2899.78, -16.10, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 30, 930.49, 2900.05, -16.75, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 31, 931.78, 2900.86, -16.75, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 32, 939.57, 2905.73, -15.32, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 33, 940.00, 2906.00, -14.69, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 34, 947.50, 2904.12, -12.82, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 35, 948.00, 2904.00, -12.19, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 36, 949.90, 2903.24, -12.19, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 37, 950.38, 2903.05, -11.57, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 38, 951.81, 2902.48, -11.57, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 39, 954.19, 2901.52, -10.19, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 40, 957.52, 2900.19, -9.57, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 41, 958.00, 2900.00, -8.94, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267403, 42, 964.00, 2896.00, -6.74, 0, 0, 0, 0, 0, 0, 100, 0, 0);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267404, 1, 1108.00, 3026.00, -14.94, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 2, 1100.00, 3028.00, -13.79, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 3, 1092.00, 3028.00, -12.59, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 4, 1090.00, 3030.00, -12.08, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 5, 1076.00, 3028.00, -10.79, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 6, 1074.12, 3029.88, -10.59, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 7, 1073.75, 3030.25, -9.92, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 8, 1070.00, 3034.00, -9.85, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 9, 1064.00, 3034.00, -10.00, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 10, 1058.00, 3042.00, -9.17, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 11, 1056.00, 3050.00, -7.96, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 12, 1056.00, 3060.00, -5.41, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 13, 1061.69, 3065.69, -5.86, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 14, 1066.31, 3070.31, -5.08, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 15, 1072.00, 3076.00, -5.31, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 16, 1079.50, 3076.00, -6.29, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 17, 1080.00, 3076.00, -6.87, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 18, 1085.55, 3076.94, -8.05, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 19, 1086.05, 3077.02, -8.64, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 20, 1093.62, 3078.30, -9.40, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 21, 1095.64, 3078.65, -8.95, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 22, 1096.14, 3078.73, -7.95, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 23, 1097.65, 3078.99, -7.95, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 24, 1098.16, 3079.07, -6.95, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 25, 1099.67, 3079.33, -6.95, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 26, 1100.18, 3079.41, -5.95, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 27, 1101.69, 3079.67, -5.95, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 28, 1103.80, 3081.03, -5.81, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 29, 1104.22, 3081.30, -7.62, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 30, 1105.90, 3082.39, -8.09, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 31, 1106.32, 3082.66, -9.91, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 32, 1109.69, 3084.84, -10.57, 0, 3000, 0, 0, 0, 0, 100, 0, 0),
(10267404, 33, 1110.18, 3084.97, -9.93, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 34, 1111.66, 3085.35, -9.93, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 35, 1112.15, 3085.47, -9.12, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 36, 1114.12, 3085.98, -9.06, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 37, 1114.61, 3086.10, -9.98, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 38, 1119.54, 3087.37, -9.31, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 39, 1120.03, 3087.49, -9.97, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 40, 1122.00, 3088.00, -10.10, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 41, 1126.00, 3092.00, -10.62, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 42, 1144.00, 3092.00, -10.44, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 43, 1147.75, 3088.25, -9.62, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 44, 1148.12, 3087.88, -10.76, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 45, 1150.00, 3086.00, -11.23, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 46, 1154.00, 3086.00, -11.47, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 47, 1158.00, 3082.00, -12.48, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 48, 1160.00, 3070.00, -13.79, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 49, 1160.00, 3062.00, -14.39, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 50, 1146.00, 3050.00, -14.59, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 51, 1144.18, 3048.18, -14.40, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 52, 1143.82, 3047.82, -13.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 53, 1142.00, 3046.00, -13.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 54, 1141.51, 3045.84, -12.83, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 55, 1140.05, 3045.35, -12.83, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 56, 1136.16, 3044.05, -13.33, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 57, 1135.68, 3043.89, -14.38, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 58, 1130.32, 3042.11, -14.88, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 59, 1129.84, 3041.95, -15.81, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 60, 1124.00, 3040.00, -15.47, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 61, 1112.00, 3028.00, -15.49, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267404, 62, 1111.50, 3027.75, -14.90, 0, 0, 0, 0, 0, 0, 100, 0, 0);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267406, 1, 934.00, 2936.00, -17.00, 0, 5000, 0, 0, 0, 0, 100, 0, 0),
(10267406, 2, 930.00, 2932.00, -17.00, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267406, 3, 930.00, 2920.00, -16.57, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267406, 4, 924.29, 2914.29, -17.47, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267406, 5, 923.93, 2913.93, -18.07, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267406, 6, 917.86, 2907.86, -19.43, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267406, 7, 910.00, 2900.00, -19.89, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267406, 8, 900.00, 2900.00, -20.77, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267406, 9, 896.00, 2904.00, -20.97, 0, 4000, 0, 0, 0, 0, 100, 0, 0),
(10267406, 10, 900.00, 2900.00, -20.77, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267406, 11, 910.00, 2900.00, -19.89, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267406, 12, 917.86, 2907.86, -19.43, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267406, 13, 923.93, 2913.93, -18.07, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267406, 14, 924.29, 2914.29, -17.47, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267406, 15, 930.00, 2920.00, -16.57, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267406, 16, 930.00, 2932.00, -17.00, 0, 0, 0, 0, 0, 0, 100, 0, 0);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267407, 1, 890.00, 2942.00, -23.88, 0, 5000, 0, 0, 0, 0, 100, 0, 0),
(10267407, 2, 894.00, 2946.00, -23.69, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 3, 910.00, 2946.00, -20.68, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 4, 916.00, 2942.00, -19.19, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 5, 920.00, 2948.00, -19.02, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 6, 924.00, 2954.00, -18.90, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 7, 924.00, 2964.00, -19.73, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 8, 927.64, 2967.64, -19.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 9, 928.00, 2968.00, -18.82, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 10, 929.82, 2969.82, -18.82, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 11, 930.18, 2970.18, -17.94, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 12, 932.00, 2972.00, -17.62, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 13, 936.00, 2972.00, -17.62, 0, 4000, 0, 0, 0, 0, 100, 0, 0),
(10267407, 14, 932.00, 2972.00, -17.62, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 15, 930.18, 2970.18, -17.94, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 16, 929.82, 2969.82, -18.82, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 17, 928.00, 2968.00, -18.82, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 18, 927.64, 2967.64, -19.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 19, 924.00, 2964.00, -19.73, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 20, 924.00, 2954.00, -18.90, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 21, 920.00, 2948.00, -19.02, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 22, 916.00, 2942.00, -19.19, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 23, 910.00, 2946.00, -20.68, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267407, 24, 894.00, 2946.00, -23.69, 0, 0, 0, 0, 0, 0, 100, 0, 0);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267408, 1, 988.00, 2890.00, -0.95, 0, 5000, 0, 0, 0, 0, 100, 0, 0),
(10267408, 2, 988.55, 2891.93, -0.95, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267408, 3, 988.69, 2892.41, -1.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267408, 4, 989.10, 2893.86, -1.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267408, 5, 989.79, 2896.28, -2.80, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267408, 6, 990.21, 2897.72, -2.60, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267408, 7, 990.90, 2900.14, -3.70, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267408, 8, 992.00, 2904.00, -4.23, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267408, 9, 992.00, 2920.00, -7.38, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267408, 10, 996.00, 2926.00, -8.05, 0, 4000, 0, 0, 0, 0, 100, 0, 0),
(10267408, 11, 992.00, 2920.00, -7.38, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267408, 12, 992.00, 2904.00, -4.23, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267408, 13, 990.90, 2900.14, -3.70, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267408, 14, 990.21, 2897.72, -2.60, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267408, 15, 989.79, 2896.28, -2.80, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267408, 16, 989.10, 2893.86, -1.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267408, 17, 988.69, 2892.41, -1.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267408, 18, 988.55, 2891.93, -0.95, 0, 0, 0, 0, 0, 0, 100, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` IN (367426, 367342, 367354, 367413, 367318, 367380, 367383, 367410, 367339, 367320);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(367426, 10267400, 0, 0, 1, 0, ''),
(367342, 10267401, 0, 0, 1, 0, ''),
(367354, 10267402, 0, 0, 1, 0, ''),
(367413, 10267403, 0, 0, 1, 0, ''),
(367318, 10267404, 0, 0, 1, 0, ''),
(367380, 10267406, 0, 0, 1, 0, ''),
(367383, 10267407, 0, 0, 1, 0, ''),
(367410, 10267408, 0, 0, 1, 0, '');
UPDATE `creature` SET `position_x` = 996.00, `position_y` = 3038.00, `position_z` = -11.96, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367426;
UPDATE `creature` SET `position_x` = 997.03, `position_y` = 3053.00, `position_z` = -9.27, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367342;
UPDATE `creature` SET `position_x` = 1003.00, `position_y` = 3034.00, `position_z` = -11.81, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367354;
UPDATE `creature` SET `position_x` = 972.00, `position_y` = 2894.00, `position_z` = -4.74, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367413;
UPDATE `creature` SET `position_x` = 1108.00, `position_y` = 3026.00, `position_z` = -14.94, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367318;
UPDATE `creature` SET `position_x` = 934.00, `position_y` = 2936.00, `position_z` = -17.00, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367380;
UPDATE `creature` SET `position_x` = 890.00, `position_y` = 2942.00, `position_z` = -23.88, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367383;
UPDATE `creature` SET `position_x` = 988.00, `position_y` = 2890.00, `position_z` = -0.95, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367410;
-- these brutes stand still
UPDATE `creature` SET `MovementType` = 0, `spawndist` = 0 WHERE `guid` IN (367339, 367320);
-- the brute walks among the fighting Illidari without being drawn into their fights
UPDATE `creature` SET `unit_flags` = 16896 WHERE `guid` = 367342;

-- Mardum: corpses spawn really dead, crystals keep their look, the objects of the zone get their French names, gateway activators stay once used, mounts are allowed once the invasion has begun, communicators and portals are destroyed when used, the colossal infernal is spawned once, Jace's and Sevis' gossip options open their pages
UPDATE `creature_template` SET `RegenHealth` = 0 WHERE `entry` IN (97594, 97712, 98618, 98622, 98621, 97592, 98611, 97603, 97604, 97602, 97598, 97597, 97601, 108409, 99735, 97770, 97771, 97772, 97769);
UPDATE `creature` SET `curhealth` = 0 WHERE `id` IN (97594, 97712, 98618, 98622, 98621, 97592, 98611, 97603, 97604, 97602, 97598, 97597, 97601, 108409, 99735, 97770, 97771, 97772, 97769);
-- the empowered fel crystal shows the fel demon crystal (59610), not the infernal (169)
UPDATE `creature_template_wdb` SET `Displayid1` = 59610, `Displayid2` = 0 WHERE `Entry` = 101704;
INSERT INTO `locales_gameobject` (`entry`, `name_loc2`) VALUES
(241751, 'Activateur de porte de la Légion'),
(241756, 'Activateur de porte de la Légion'),
(241757, 'Activateur de porte de la Légion')
ON DUPLICATE KEY UPDATE `name_loc2` = VALUES(`name_loc2`);
-- the gateway activators stay in place once used: their phase ended with their own credit, so they vanished at the click
UPDATE `gameobject` SET `PhaseId` = '5837' WHERE `guid` IN (131213, 131200, 131186);
UPDATE `locales_gameobject` SET `castbarcaption_loc2` = 'Activation' WHERE `entry` IN (241751, 241756, 241757);
-- mounts are disabled in Mardum until The Invasion Begins is turned in, not from then on (quest_end_status lists the states where the aura applies)
UPDATE `spell_area` SET `quest_end_status` = 43 WHERE `spell` = 179665 AND `area` = 7705 AND `quest_end` = 40077;
-- the Legion communicators and portals of the Mardum assault are destroyed when used, then come back after their respawn time
UPDATE `gameobject_template` SET `flags` = `flags` & ~0x20 WHERE `entry` IN (243873, 244439, 244440, 244441);
INSERT INTO `locales_gameobject` (`entry`, `castbarcaption_loc2`) VALUES
(243873, 'Destruction'),
(244439, 'Destruction'),
(244440, 'Destruction'),
(244441, 'Destruction')
ON DUPLICATE KEY UPDATE `castbarcaption_loc2` = VALUES(`castbarcaption_loc2`);
UPDATE `locales_gameobject` SET `name_loc2` = 'Portail de la Légion' WHERE `entry` = 243873;
INSERT INTO `locales_gameobject` (`entry`, `name_loc2`) VALUES
(242987, 'Cage du geôlier'),
(242989, 'Cage du geôlier'),
(242990, 'Cage du geôlier'),
(244916, 'Cage du geôlier')
ON DUPLICATE KEY UPDATE `name_loc2` = VALUES(`name_loc2`);
UPDATE `gameobject` SET `spawntimesecs` = 60 WHERE `id` IN (244439, 244440, 244441);
-- the colossal infernal of the Inquisitor's chasm was spawned three times at the same spot
DELETE FROM `creature_addon` WHERE `guid` IN (367685, 367686);
DELETE FROM `creature` WHERE `guid` IN (367685, 367686);
-- Jace's two gossip options open their pages
UPDATE `gossip_menu_option` SET `ActionMenuID` = 19109 WHERE `MenuID` = 18438 AND `OptionID` = 0;
UPDATE `gossip_menu_option` SET `ActionMenuID` = 19131 WHERE `MenuID` = 18438 AND `OptionID` = 1;
UPDATE `gossip_menu_option` SET `ActionMenuID` = 19016 WHERE `MenuID` = 19133 AND `OptionID` = 1;
-- French names and cast bars of the Mardum objects
INSERT INTO `locales_gameobject` (`entry`, `name_loc2`, `castbarcaption_loc2`) VALUES
(243335, 'Creuset du Néant', 'Accomplissement du rituel'),
(244466, 'Gangreportail', ''),
(243965, 'Bannière illidari', 'Mise en place'),
(243967, 'Bannière illidari', 'Mise en place'),
(243968, 'Bannière illidari', 'Mise en place'),
(244601, 'Stabilisateur de flèche', 'Désactivation'),
(244604, 'Stabilisateur de flèche', 'Désactivation'),
(245995, 'Stabilisateur de flèche', 'Désactivation'),
(244638, 'Puits des âmes', 'Ouverture'),
(245112, 'Tome des secrets gangrenés', 'Choix'),
(245728, 'Clé sargerite', 'Activation'),
(246485, 'Flèche de malheur', ''),
(246486, 'Flèche de malheur', ''),
(246566, 'Flèche de malheur', ''),
(246567, 'Flèche de malheur', ''),
(245045, 'Éboulement', ''),
(246296, 'Toile d''araignée', ''),
(259043, 'Bannière de la Légion', '')
ON DUPLICATE KEY UPDATE `name_loc2` = VALUES(`name_loc2`), `castbarcaption_loc2` = VALUES(`castbarcaption_loc2`);
UPDATE `locales_gameobject` SET `castbarcaption_loc2` = 'Ouverture' WHERE `entry` IN (244689, 244691, 244692, 244694, 244699, 244700, 244701, 245316, 245345, 245996, 246147, 246249, 246309);
INSERT INTO `locales_gameobject` (`entry`, `name_loc2`) VALUES (246401, 'Portail de la Légion') ON DUPLICATE KEY UPDATE `name_loc2` = VALUES(`name_loc2`);
-- the destroyed fel spreaders come back after a minute
UPDATE `creature` SET `spawntimesecs` = 60 WHERE `id` = 97142;
-- the Meeting With the Queen scene fades to black on completion while nothing follows it: the screen stayed black
UPDATE `spell_scene` SET `PlaybackFlags` = `PlaybackFlags` & ~0x2 WHERE `MiscValue` = 1061;
UPDATE `gameobject` SET `spawntimesecs` = 120 WHERE `id` = 243873;
-- the eredar summoners turn on whoever destroys their portal
DELETE FROM `creature_text` WHERE `CreatureID` = 102714 AND `GroupID` = 1;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextID`, `MinTimer`, `MaxTimer`, `SpellID`, `comment`) VALUES
(102714, 1, 0, 'Illidari traitor! You will pay for this portal!', 14, 0, 50, 0, 0, 0, 0, 0, 0, 0, 'Eredar Summoner - portal destroyed'),
(102714, 1, 1, 'The portal has fallen! Crush this Illidari!', 14, 0, 50, 0, 0, 0, 0, 0, 0, 0, 'Eredar Summoner - portal destroyed');
DELETE FROM `creature_text_locale` WHERE `CreatureID` = 102714 AND `GroupID` = 1;
INSERT INTO `creature_text_locale` (`CreatureID`, `GroupID`, `ID`, `Locale`, `Text`) VALUES
(102714, 1, 0, 'frFR', 'Traître illidari ! Tu paieras pour ce portail !'),
(102714, 1, 1, 'frFR', 'Le portail est tombé ! Écrasez cet illidari !');
-- the eredar summoners run on a script: their combat line is kept for the portal they defend, not for the one already lost
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_mardum_eredar_summoner' WHERE `entry` = 102714;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 102714 AND `source_type` = 0;

-- Mardum: Anguish Jailers patrol back and forth along the roads of their camp, following the ground; the others stand still
DELETE FROM `waypoint_data` WHERE `id` BETWEEN 10267410 AND 10267414;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267410, 1, 730.00, 2636.00, -60.97, 0, 5000, 0, 0, 0, 0, 100, 0, 0),
(10267410, 2, 731.75, 2639.50, -60.69, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267410, 3, 732.00, 2640.00, -59.97, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267410, 4, 732.00, 2656.00, -58.55, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267410, 5, 731.70, 2657.50, -59.06, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267410, 6, 730.00, 2666.00, -58.27, 0, 4000, 0, 0, 0, 0, 100, 0, 0),
(10267410, 7, 731.70, 2657.50, -59.06, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267410, 8, 732.00, 2656.00, -58.55, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267410, 9, 732.00, 2640.00, -59.97, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267410, 10, 731.75, 2639.50, -60.69, 0, 0, 0, 0, 0, 0, 100, 0, 0);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267411, 1, 730.00, 2606.00, -65.43, 0, 5000, 0, 0, 0, 0, 100, 0, 0),
(10267411, 2, 730.00, 2598.00, -66.93, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267411, 3, 731.18, 2595.65, -67.78, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267411, 4, 734.00, 2590.00, -68.13, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267411, 5, 738.00, 2582.00, -69.13, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267411, 6, 740.00, 2568.00, -70.09, 0, 4000, 0, 0, 0, 0, 100, 0, 0),
(10267411, 7, 738.00, 2582.00, -69.13, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267411, 8, 734.00, 2590.00, -68.13, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267411, 9, 731.18, 2595.65, -67.78, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267411, 10, 730.00, 2598.00, -66.93, 0, 0, 0, 0, 0, 0, 100, 0, 0);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267412, 1, 770.00, 2558.00, -70.27, 0, 5000, 0, 0, 0, 0, 100, 0, 0),
(10267412, 2, 766.25, 2554.25, -70.29, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267412, 3, 765.88, 2553.88, -69.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267412, 4, 764.00, 2552.00, -69.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267412, 5, 763.53, 2551.76, -69.13, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267412, 6, 756.00, 2548.00, -68.78, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267412, 7, 746.00, 2538.00, -68.83, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267412, 8, 746.00, 2518.00, -67.81, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267412, 9, 746.00, 2510.00, -67.45, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267412, 10, 736.00, 2500.00, -67.24, 0, 4000, 0, 0, 0, 0, 100, 0, 0),
(10267412, 11, 746.00, 2510.00, -67.45, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267412, 12, 746.00, 2518.00, -67.81, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267412, 13, 746.00, 2538.00, -68.83, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267412, 14, 756.00, 2548.00, -68.78, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267412, 15, 763.53, 2551.76, -69.13, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267412, 16, 764.00, 2552.00, -69.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267412, 17, 765.88, 2553.88, -69.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267412, 18, 766.25, 2554.25, -70.29, 0, 0, 0, 0, 0, 0, 100, 0, 0);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267413, 1, 760.00, 2444.00, -61.57, 0, 5000, 0, 0, 0, 0, 100, 0, 0),
(10267413, 2, 754.00, 2442.00, -62.03, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267413, 3, 748.00, 2442.00, -62.48, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267413, 4, 747.50, 2442.00, -63.13, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267413, 5, 745.50, 2442.00, -63.43, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267413, 6, 740.00, 2442.00, -63.21, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267413, 7, 734.00, 2442.00, -64.01, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267413, 8, 718.00, 2458.00, -65.94, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267413, 9, 718.00, 2471.50, -66.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267413, 10, 718.00, 2480.00, -68.03, 0, 4000, 0, 0, 0, 0, 100, 0, 0),
(10267413, 11, 718.00, 2471.50, -66.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267413, 12, 718.00, 2458.00, -65.94, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267413, 13, 734.00, 2442.00, -64.01, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267413, 14, 740.00, 2442.00, -63.21, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267413, 15, 745.50, 2442.00, -63.43, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267413, 16, 747.50, 2442.00, -63.13, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267413, 17, 748.00, 2442.00, -62.48, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267413, 18, 754.00, 2442.00, -62.03, 0, 0, 0, 0, 0, 0, 100, 0, 0);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267414, 1, 866.00, 2486.00, -59.42, 0, 5000, 0, 0, 0, 0, 100, 0, 0),
(10267414, 2, 866.00, 2482.00, -60.27, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267414, 3, 874.00, 2476.00, -61.93, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267414, 4, 882.00, 2472.00, -63.59, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267414, 5, 888.00, 2472.00, -64.34, 0, 4000, 0, 0, 0, 0, 100, 0, 0),
(10267414, 6, 882.00, 2472.00, -63.59, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267414, 7, 874.00, 2476.00, -61.93, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267414, 8, 866.00, 2482.00, -60.27, 0, 0, 0, 0, 0, 0, 100, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` IN (367616, 367568, 367572, 367620, 367630);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(367616, 10267410, 0, 0, 1, 0, ''),
(367568, 10267411, 0, 0, 1, 0, ''),
(367572, 10267412, 0, 0, 1, 0, ''),
(367620, 10267413, 0, 0, 1, 0, ''),
(367630, 10267414, 0, 0, 1, 0, '');
UPDATE `creature` SET `position_x` = 730.00, `position_y` = 2636.00, `position_z` = -60.97, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367616;
UPDATE `creature` SET `position_x` = 730.00, `position_y` = 2606.00, `position_z` = -65.43, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367568;
UPDATE `creature` SET `position_x` = 770.00, `position_y` = 2558.00, `position_z` = -70.27, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367572;
UPDATE `creature` SET `position_x` = 760.00, `position_y` = 2444.00, `position_z` = -61.57, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367620;
UPDATE `creature` SET `position_x` = 866.00, `position_y` = 2486.00, `position_z` = -59.42, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367630;
UPDATE `creature` SET `MovementType` = 0, `spawndist` = 0 WHERE `guid` IN (367573, 367594, 367604, 367609, 367610, 367612, 367615, 367706);
-- the Mo'arg Brute on the walkway walks down its ramp toward Jace and back
DELETE FROM `waypoint_data` WHERE `id` = 10267415;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267415, 1, 1149.40, 2557.00, -18.50, 0, 5000, 0, 0, 0, 0, 100, 0, 0),
(10267415, 2, 1145.40, 2557.00, -19.34, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267415, 3, 1143.40, 2555.00, -19.56, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267415, 4, 1139.40, 2555.00, -20.54, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267415, 5, 1137.40, 2553.00, -20.74, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267415, 6, 1131.40, 2553.00, -22.24, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267415, 7, 1117.40, 2553.00, -24.09, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267415, 8, 1109.40, 2561.00, -26.97, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267415, 9, 1099.40, 2561.00, -28.60, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267415, 10, 1095.40, 2561.00, -29.45, 0, 4000, 0, 0, 0, 0, 100, 0, 0),
(10267415, 11, 1099.40, 2561.00, -28.60, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267415, 12, 1109.40, 2561.00, -26.97, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267415, 13, 1117.40, 2553.00, -24.09, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267415, 14, 1131.40, 2553.00, -22.24, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267415, 15, 1137.40, 2553.00, -20.74, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267415, 16, 1139.40, 2555.00, -20.54, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267415, 17, 1143.40, 2555.00, -19.56, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267415, 18, 1145.40, 2557.00, -19.34, 0, 0, 0, 0, 0, 0, 100, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` = 367735;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (367735, 10267415, 0, 0, 1, 0, '');
UPDATE `creature` SET `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367735;

-- Mardum: the Doomguard Eradicators in the air fly circuits over their area, those on the ground stand still, away from the roads where they can; a doubled one is removed
DELETE FROM `creature_addon` WHERE `guid` = 367779;
DELETE FROM `creature` WHERE `guid` = 367779;
DELETE FROM `waypoint_data` WHERE `id` BETWEEN 10267320 AND 10267323;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267320, 1, 1031.23, 2548.50, -18.52, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267320, 2, 1070.93, 2576.58, -13.87, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267320, 3, 1150.99, 2544.80, -0.06, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267320, 4, 1223.30, 2592.94, 17.38, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267320, 5, 1241.52, 2623.75, 19.66, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267320, 6, 1304.70, 2629.81, 33.10, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267320, 7, 1387.35, 2598.70, 40.40, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267320, 8, 1278.25, 2565.67, 28.18, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267320, 9, 1155.07, 2554.59, 3.14, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267321, 1, 1237.62, 2423.60, -2.24, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267321, 2, 1183.32, 2291.30, 13.35, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267321, 3, 1188.25, 2227.65, 30.15, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267321, 4, 1235.90, 2238.87, 39.02, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267321, 5, 1266.23, 2352.47, 37.59, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267321, 6, 1352.98, 2342.49, 67.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267321, 7, 1248.11, 2457.77, 11.61, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267322, 1, 1212.39, 2537.24, 9.88, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267322, 2, 1332.09, 2576.22, 33.46, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267322, 3, 1425.11, 2596.73, 38.40, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267322, 4, 1495.89, 2659.25, 45.77, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267322, 5, 1539.48, 2653.81, 41.39, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267322, 6, 1570.81, 2633.51, 43.21, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267322, 7, 1547.42, 2543.33, 70.59, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267322, 8, 1415.78, 2478.93, 50.77, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267322, 9, 1375.46, 2488.18, 38.80, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267322, 10, 1323.01, 2480.28, 55.36, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267323, 1, 1124.96, 2730.54, -59.13, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267323, 2, 1139.20, 2659.07, -76.62, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267323, 3, 1049.36, 2653.72, -54.16, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267323, 4, 1004.06, 2595.73, -46.59, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267323, 5, 922.34, 2554.72, -37.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267323, 6, 946.47, 2495.46, -45.48, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267323, 7, 983.59, 2543.38, -29.54, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267323, 8, 1064.97, 2620.37, -35.90, 0, 0, 0, 1, 0, 0, 100, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` IN (367743, 367745, 367747, 367736);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(367743, 10267320, 0, 50331648, 1, 0, ''),
(367745, 10267321, 0, 50331648, 1, 0, ''),
(367747, 10267322, 0, 50331648, 1, 0, ''),
(367736, 10267323, 0, 50331648, 1, 0, '');
DELETE FROM `creature_movement_override` WHERE `SpawnId` IN (367743, 367745, 367747, 367736);
INSERT INTO `creature_movement_override` (`SpawnId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Random`) VALUES
(367743, 0, 0, 1, 0, 0),
(367745, 0, 0, 1, 0, 0),
(367747, 0, 0, 1, 0, 0),
(367736, 0, 0, 1, 0, 0);
UPDATE `creature` SET `position_x` = 1031.23, `position_y` = 2548.50, `position_z` = -18.52, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367743;
UPDATE `creature` SET `position_x` = 1237.62, `position_y` = 2423.60, `position_z` = -2.24, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367745;
UPDATE `creature` SET `position_x` = 1212.39, `position_y` = 2537.24, `position_z` = 9.88, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367747;
UPDATE `creature` SET `position_x` = 1124.96, `position_y` = 2730.54, `position_z` = -59.13, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367736;
UPDATE `creature` SET `position_x` = 1396.64, `position_y` = 2538.78, `position_z` = 26.83 WHERE `guid` = 367771;
UPDATE `creature` SET `position_x` = 1408.83, `position_y` = 2557.19, `position_z` = 31.97 WHERE `guid` = 367776;
UPDATE `creature` SET `position_x` = 1464.94, `position_y` = 2673.80, `position_z` = 38.91 WHERE `guid` = 367792;
UPDATE `creature` SET `position_x` = 1499.78, `position_y` = 2596.26, `position_z` = 36.49 WHERE `guid` = 367795;
UPDATE `creature` SET `position_x` = 1526.27, `position_y` = 2627.62, `position_z` = 29.32 WHERE `guid` = 367813;
UPDATE `creature` SET `position_x` = 1571.05, `position_y` = 2581.30, `position_z` = 45.35 WHERE `guid` = 367815;
UPDATE `creature` SET `MovementType` = 0, `spawndist` = 0 WHERE `id` = 94654 AND `guid` NOT IN (367743, 367745, 367747, 367736);
-- the eradicators that hovered in place fly short loops around their spot
DELETE FROM `waypoint_data` WHERE `id` BETWEEN 10267324 AND 10267330;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267324, 1, 1056.60, 2584.24, -19.63, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267324, 2, 1092.02, 2547.39, -13.79, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267324, 3, 1108.51, 2568.03, -5.23, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267324, 4, 1135.69, 2616.44, -27.24, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267324, 5, 1113.56, 2631.73, -33.12, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267324, 6, 1077.76, 2627.88, -44.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267324, 7, 1044.46, 2621.38, -32.67, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267325, 1, 1182.23, 2541.10, -32.97, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267325, 2, 1144.69, 2526.09, -27.18, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267325, 3, 1112.38, 2462.39, -47.38, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267325, 4, 1161.74, 2440.70, -28.24, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267325, 5, 1199.00, 2422.26, -32.19, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267325, 6, 1213.39, 2481.83, -6.21, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267325, 7, 1201.20, 2513.57, -20.55, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267326, 1, 1092.46, 2477.25, -56.63, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267326, 2, 1042.46, 2446.21, -69.56, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267326, 3, 1013.87, 2438.03, -78.62, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267326, 4, 938.66, 2453.33, -55.69, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267326, 5, 910.97, 2479.63, -46.97, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267326, 6, 997.39, 2481.58, -50.81, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267326, 7, 1052.55, 2476.21, -50.78, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267327, 1, 1109.37, 2726.16, -48.92, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267327, 2, 1088.98, 2676.38, -66.95, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267327, 3, 1142.52, 2608.32, -76.48, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267327, 4, 1189.74, 2628.76, -36.33, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267327, 5, 1202.01, 2688.70, -68.00, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267327, 6, 1175.97, 2722.19, -65.18, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267328, 1, 1421.68, 2514.89, 70.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267328, 2, 1352.07, 2507.01, 45.82, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267328, 3, 1331.49, 2445.39, 63.26, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267328, 4, 1358.37, 2403.77, 63.05, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267328, 5, 1385.42, 2383.12, 66.17, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267328, 6, 1379.13, 2438.06, 76.34, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267329, 1, 1510.82, 2644.79, 123.86, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267329, 2, 1508.20, 2693.70, 108.69, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267329, 3, 1484.63, 2750.02, 103.28, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267329, 4, 1452.59, 2761.15, 94.67, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267329, 5, 1397.79, 2742.89, 87.22, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267329, 6, 1451.06, 2698.25, 114.60, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267330, 1, 1338.30, 2282.32, 101.82, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267330, 2, 1391.23, 2259.61, 91.48, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267330, 3, 1450.54, 2289.77, 106.38, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267330, 4, 1425.25, 2339.12, 87.83, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267330, 5, 1400.92, 2345.54, 82.47, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10267330, 6, 1357.43, 2332.90, 95.14, 0, 0, 0, 1, 0, 0, 100, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` IN (367733, 367740, 367741, 367742, 367771, 367813, 367921);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(367733, 10267324, 0, 50331648, 1, 0, ''),
(367740, 10267325, 0, 50331648, 1, 0, ''),
(367741, 10267326, 0, 50331648, 1, 0, ''),
(367742, 10267327, 0, 50331648, 1, 0, ''),
(367771, 10267328, 0, 50331648, 1, 0, ''),
(367813, 10267329, 0, 50331648, 1, 0, ''),
(367921, 10267330, 0, 50331648, 1, 0, '');
DELETE FROM `creature_movement_override` WHERE `SpawnId` IN (367733, 367740, 367741, 367742, 367771, 367813, 367921);
INSERT INTO `creature_movement_override` (`SpawnId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Random`) VALUES
(367733, 0, 0, 1, 0, 0),
(367740, 0, 0, 1, 0, 0),
(367741, 0, 0, 1, 0, 0),
(367742, 0, 0, 1, 0, 0),
(367771, 0, 0, 1, 0, 0),
(367813, 0, 0, 1, 0, 0),
(367921, 0, 0, 1, 0, 0);
UPDATE `creature` SET `position_x` = 1056.60, `position_y` = 2584.24, `position_z` = -19.63, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367733;
UPDATE `creature` SET `position_x` = 1182.23, `position_y` = 2541.10, `position_z` = -32.97, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367740;
UPDATE `creature` SET `position_x` = 1092.46, `position_y` = 2477.25, `position_z` = -56.63, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367741;
UPDATE `creature` SET `position_x` = 1109.37, `position_y` = 2726.16, `position_z` = -48.92, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367742;
UPDATE `creature` SET `position_x` = 1421.68, `position_y` = 2514.89, `position_z` = 70.60, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367771;
UPDATE `creature` SET `position_x` = 1510.82, `position_y` = 2644.79, `position_z` = 123.86, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367813;
UPDATE `creature` SET `position_x` = 1338.30, `position_y` = 2282.32, `position_z` = 101.82, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367921;

-- Mardum: an Eredar Summoner and a Felguard Sentry with its felstalker patrol back and forth along the roads, following the ground
DELETE FROM `waypoint_data` WHERE `id` IN (10267416, 10267417);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267416, 1, 1320.00, 2566.00, 16.62, 0, 5000, 0, 0, 0, 0, 100, 0, 0),
(10267416, 2, 1332.00, 2568.00, 17.77, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267416, 3, 1340.00, 2572.00, 17.89, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267416, 4, 1346.00, 2578.00, 18.07, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267416, 5, 1352.00, 2578.00, 18.66, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267416, 6, 1368.00, 2588.00, 20.33, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267416, 7, 1380.00, 2590.00, 21.78, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267416, 8, 1398.00, 2590.00, 24.04, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267416, 9, 1403.82, 2595.82, 24.56, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267416, 10, 1406.00, 2598.00, 25.36, 0, 4000, 0, 0, 0, 0, 100, 0, 0),
(10267416, 11, 1403.82, 2595.82, 24.56, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267416, 12, 1398.00, 2590.00, 24.04, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267416, 13, 1380.00, 2590.00, 21.78, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267416, 14, 1368.00, 2588.00, 20.33, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267416, 15, 1352.00, 2578.00, 18.66, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267416, 16, 1346.00, 2578.00, 18.07, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267416, 17, 1340.00, 2572.00, 17.89, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267416, 18, 1332.00, 2568.00, 17.77, 0, 0, 0, 0, 0, 0, 100, 0, 0);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10267417, 1, 1452.00, 2408.00, 56.88, 0, 5000, 0, 0, 0, 0, 100, 0, 0),
(10267417, 2, 1451.06, 2406.12, 56.64, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 3, 1448.00, 2400.00, 57.60, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 4, 1444.70, 2395.60, 58.62, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 5, 1442.00, 2392.00, 58.22, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 6, 1428.00, 2378.00, 58.27, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 7, 1418.00, 2376.00, 57.57, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 8, 1411.50, 2376.00, 55.99, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 9, 1398.00, 2376.00, 54.23, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 10, 1378.00, 2376.00, 49.69, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 11, 1373.50, 2375.25, 48.00, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 12, 1372.00, 2375.00, 48.00, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 13, 1371.50, 2374.92, 47.38, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 14, 1370.00, 2374.67, 47.38, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 15, 1369.50, 2374.58, 46.75, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 16, 1368.00, 2374.33, 46.75, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 17, 1367.50, 2374.25, 46.13, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 18, 1366.00, 2374.00, 46.13, 0, 4000, 0, 0, 0, 0, 100, 0, 0),
(10267417, 19, 1367.50, 2374.25, 46.13, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 20, 1368.00, 2374.33, 46.75, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 21, 1369.50, 2374.58, 46.75, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 22, 1370.00, 2374.67, 47.38, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 23, 1371.50, 2374.92, 47.38, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 24, 1372.00, 2375.00, 48.00, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 25, 1373.50, 2375.25, 48.00, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 26, 1378.00, 2376.00, 49.69, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 27, 1398.00, 2376.00, 54.23, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 28, 1411.50, 2376.00, 55.99, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 29, 1418.00, 2376.00, 57.57, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 30, 1428.00, 2378.00, 58.27, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 31, 1442.00, 2392.00, 58.22, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 32, 1444.70, 2395.60, 58.62, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 33, 1448.00, 2400.00, 57.60, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(10267417, 34, 1451.06, 2406.12, 56.64, 0, 0, 0, 0, 0, 0, 100, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` IN (367758, 367888);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(367758, 10267416, 0, 0, 1, 0, ''),
(367888, 10267417, 0, 0, 1, 0, '');
UPDATE `creature` SET `position_x` = 1320.00, `position_y` = 2566.00, `position_z` = 16.62, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367758;
UPDATE `creature` SET `position_x` = 1452.00, `position_y` = 2408.00, `position_z` = 56.88, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 367888;
-- the felstalker walks at the sentry's left side
UPDATE `creature` SET `MovementType` = 0, `spawndist` = 0 WHERE `guid` = 367811;
DELETE FROM `creature_formations` WHERE `memberGUID` IN (367888, 367811);
INSERT INTO `creature_formations` (`leaderGUID`, `memberGUID`, `dist`, `angle`, `groupAI`) VALUES
(367888, 367888, 0, 0, 3),
(367888, 367811, 3, 90, 3);

-- Mardum, Before We're Overrun: Beliash's fight, Tyranna, the Spires of Woe and their fire, the Spire Stabilizers
DELETE FROM `creature` WHERE `guid` BETWEEN 367856 AND 367867 AND `id` IN (97624, 97629, 97634);
DELETE FROM `creature_addon` WHERE `guid` BETWEEN 367856 AND 367867;

DELETE FROM `areatrigger_actions` WHERE `entry` = 4920 AND `customEntry` = 0;
INSERT INTO `areatrigger_actions` (`entry`, `customEntry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `comment`) VALUES
(4920, 0, 0, 1, 0, 8, 192603, 'Spire of Woe fire patch - burn on enter'),
(4920, 0, 1, 2, 1, 8, 192603, 'Spire of Woe fire patch - end the burn on leave');

UPDATE `gameobject_template` SET `flags` = 32, `ScriptName` = 'go_mardum_spire_stabilizer' WHERE `entry` IN (244601, 244604);
UPDATE `gameobject` SET `state` = 1 WHERE `guid` IN (131184, 131185);

UPDATE `creature_template_addon` SET `auras` = '' WHERE `entry` IN (95048, 97629, 97634);
UPDATE `creature_template` SET `unit_flags` = `unit_flags` | 0x100 WHERE `entry` = 95048;
UPDATE `creature_text` SET `GroupID` = 0 WHERE `CreatureID` = 95048 AND `GroupID` = 3;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_mardum_spire_of_woe_fire' WHERE `entry` IN (97629, 97634);
UPDATE `creature_template_wdb` SET `Displayid1` = 11686, `Displayid2` = 0, `Displayid3` = 0, `Displayid4` = 0 WHERE `Entry` IN (97624, 97629, 97634);
UPDATE `creature` SET `position_x` = 1571.06, `position_y` = 2724.83, `position_z` = 45.44 WHERE `guid` = 367850 AND `id` = 97624;
UPDATE `creature` SET `position_x` = 1622.58, `position_y` = 2689.24, `position_z` = 45.09 WHERE `guid` = 367846 AND `id` = 97624;
DELETE FROM `creature` WHERE `guid` IN (367849, 367852) AND `id` = 97624;
DELETE FROM `creature_addon` WHERE `guid` IN (367849, 367852);
DELETE FROM `creature_template_movement` WHERE `CreatureId` = 97624;
INSERT INTO `creature_template_movement` (`CreatureId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Random`) VALUES (97624, 0, 0, 1, 1, 0);
UPDATE `creature_template_movement` SET `Flight` = 0 WHERE `CreatureId` = 93221;

-- Mardum, Illidari Outpost: a wounded demon hunter lies on the ground, Jace offers his option until he has been spoken to, the Coilskar Myrmidons no longer play dead, and the cave party gives way to its march once Hidden No More is taken
DELETE FROM `creature_addon` WHERE `guid` = 368017;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (368017, 0, 0, 7, 1, 0, '');
UPDATE `conditions` SET `ConditionValue2` = 96436 WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 19175 AND `SourceEntry` = 0 AND `ConditionTypeOrReference` = 41 AND `ConditionValue1` = 39262;
UPDATE `creature_template` SET `unit_flags2` = `unit_flags2` & ~0x1 WHERE `entry` = 96502;
UPDATE `creature` SET `PhaseId` = '5086' WHERE `guid` IN (368102, 368103, 368104, 368105, 368106, 368108, 368111, 368112, 368113, 368115, 368117);

-- Mardum: Doom Slayers and demon hunters standing close fight to the death, the demon hunters winning
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_mardum_sparring_foes' WHERE `entry` IN (93716, 96930);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 96930 AND `source_type` = 0;
UPDATE `creature_template_addon` SET `auras` = '42459' WHERE `entry` = 96930;

-- Mardum, Fel Lord Caza: his Fel Infusion and his thrown axe burn their surroundings every second
DELETE FROM `areatrigger_data` WHERE `entry` IN (5315, 6058);
INSERT INTO `areatrigger_data` (`entry`, `spellId`, `customEntry`, `updateDelay`) VALUES
(5315, 197180, 10037, 1000),
(6058, 204763, 10745, 1000);
UPDATE `areatrigger_actions` SET `spellId` = 196989 WHERE `entry` = 6058 AND `spellId` = 204762;
UPDATE `creature_template` SET `unit_flags` = 33554434 WHERE `entry` = 100062;

-- Mardum: the Coilskar Harpooners fight the demons around them
UPDATE `creature_template` SET `unit_flags` = `unit_flags` & ~0x200 WHERE `entry` = 96561;

-- Mardum, Soul Engine: patrols, flights, a soulmaster casting at an altar, eating hounds, healable demon hunters and Volroth's lines
DELETE FROM `waypoint_data` WHERE `id` = 368656;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(368656, 0, 1708.00, 1679.00, 67.56, 0, 3000, 0, 0, 0, 0, 100, 96279, 0),
(368656, 1, 1709.00, 1672.00, 66.96, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 2, 1713.00, 1666.00, 66.86, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 3, 1718.00, 1661.00, 67.02, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 4, 1722.00, 1655.00, 66.99, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 5, 1723.00, 1648.00, 66.36, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 6, 1724.58, 1643.26, 64.29, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 7, 1725.00, 1642.00, 64.29, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 8, 1725.71, 1641.29, 63.48, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 9, 1727.83, 1639.17, 63.01, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 10, 1728.54, 1638.46, 63.88, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 11, 1729.24, 1637.76, 63.48, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 12, 1730.00, 1637.00, 64.35, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 13, 1731.41, 1635.59, 63.98, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 14, 1732.12, 1634.88, 64.65, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 15, 1735.00, 1632.00, 64.95, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 16, 1740.00, 1627.00, 65.38, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 17, 1745.00, 1622.00, 65.46, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 18, 1752.00, 1622.00, 66.71, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 19, 1759.00, 1622.00, 68.06, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 20, 1766.00, 1623.00, 69.86, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 21, 1769.79, 1621.74, 70.21, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 22, 1772.00, 1621.00, 71.11, 0, 3000, 0, 0, 0, 0, 100, 96279, 0),
(368656, 23, 1769.79, 1621.74, 70.21, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 24, 1766.00, 1623.00, 69.86, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 25, 1759.00, 1622.00, 68.06, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 26, 1752.00, 1622.00, 66.71, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 27, 1745.00, 1622.00, 65.46, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 28, 1740.00, 1627.00, 65.38, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 29, 1735.00, 1632.00, 64.95, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 30, 1732.12, 1634.88, 64.65, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 31, 1731.41, 1635.59, 63.98, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 32, 1730.00, 1637.00, 64.35, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 33, 1729.24, 1637.76, 63.48, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 34, 1728.54, 1638.46, 63.88, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 35, 1727.83, 1639.17, 63.01, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 36, 1725.71, 1641.29, 63.48, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 37, 1725.00, 1642.00, 64.29, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 38, 1724.58, 1643.26, 64.29, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 39, 1723.00, 1648.00, 66.36, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 40, 1722.00, 1655.00, 66.99, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 41, 1718.00, 1661.00, 67.02, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 42, 1713.00, 1666.00, 66.86, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368656, 43, 1709.00, 1672.00, 66.96, 0, 0, 0, 0, 0, 0, 100, 96279, 0);
UPDATE `creature` SET `position_x` = 1723.00, `position_y` = 1648.00, `position_z` = 66.36 WHERE `guid` = 368656;
UPDATE `creature` SET `MovementType` = 2, `spawndist` = 0, `unit_flags` = 32848 | 0x200 WHERE `guid` = 368656;
DELETE FROM `waypoint_data` WHERE `id` = 368595;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(368595, 0, 1876.00, 1623.00, 93.13, 0, 3000, 0, 0, 0, 0, 100, 96279, 0),
(368595, 1, 1869.00, 1623.00, 92.39, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 2, 1862.00, 1623.00, 91.56, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 3, 1855.00, 1619.50, 90.15, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 4, 1848.00, 1619.00, 88.79, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 5, 1841.00, 1619.00, 86.99, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 6, 1834.00, 1619.00, 85.69, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 7, 1827.00, 1619.00, 83.76, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 8, 1820.00, 1619.00, 82.34, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 9, 1813.00, 1619.00, 80.97, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 10, 1810.00, 1619.00, 81.03, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 11, 1809.00, 1619.00, 79.86, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 12, 1806.00, 1619.00, 78.86, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 13, 1799.00, 1619.00, 76.96, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 14, 1792.00, 1618.00, 75.53, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 15, 1785.00, 1618.00, 73.91, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 16, 1778.00, 1619.00, 72.56, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 17, 1772.00, 1621.00, 71.11, 0, 3000, 0, 0, 0, 0, 100, 96279, 0),
(368595, 18, 1778.00, 1619.00, 72.56, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 19, 1785.00, 1618.00, 73.91, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 20, 1792.00, 1618.00, 75.53, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 21, 1799.00, 1619.00, 76.96, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 22, 1806.00, 1619.00, 78.86, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 23, 1809.00, 1619.00, 79.86, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 24, 1810.00, 1619.00, 81.03, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 25, 1813.00, 1619.00, 80.97, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 26, 1820.00, 1619.00, 82.34, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 27, 1827.00, 1619.00, 83.76, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 28, 1834.00, 1619.00, 85.69, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 29, 1841.00, 1619.00, 86.99, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 30, 1848.00, 1619.00, 88.79, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 31, 1855.00, 1619.50, 90.15, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 32, 1862.00, 1623.00, 91.56, 0, 0, 0, 0, 0, 0, 100, 96279, 0),
(368595, 33, 1869.00, 1623.00, 92.39, 0, 0, 0, 0, 0, 0, 100, 96279, 0);
UPDATE `creature` SET `MovementType` = 2, `spawndist` = 0, `unit_flags` = 32848 | 0x200 WHERE `guid` = 368595;
DELETE FROM `waypoint_data` WHERE `id` IN (368560, 368562);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(368560, 0, 1859.38, 1550.14, 95.30, 0, 3000, 0, 0, 0, 0, 100, 102724, 0),
(368560, 1, 1856.42, 1544.92, 94.43, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 2, 1855.86, 1543.93, 93.70, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 3, 1854.14, 1540.32, 93.63, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 4, 1853.71, 1539.42, 92.90, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 5, 1852.78, 1537.47, 92.83, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 6, 1849.83, 1530.78, 91.43, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 7, 1846.18, 1524.31, 91.08, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 8, 1838.89, 1521.74, 88.40, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 9, 1831.85, 1519.58, 86.21, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 10, 1830.16, 1518.51, 86.21, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 11, 1825.64, 1515.66, 84.70, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 12, 1822.46, 1511.80, 84.71, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 13, 1821.83, 1511.03, 83.85, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 14, 1821.01, 1510.03, 83.85, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 15, 1819.90, 1508.37, 83.32, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 16, 1818.78, 1506.71, 84.34, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 17, 1816.94, 1503.96, 83.10, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 18, 1813.06, 1497.88, 82.35, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 19, 1809.38, 1491.81, 81.68, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 20, 1805.69, 1485.53, 81.21, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 21, 1804.20, 1482.93, 81.38, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 22, 1801.95, 1478.99, 80.60, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 23, 1800.35, 1476.18, 80.73, 0, 3000, 0, 0, 0, 0, 100, 102724, 0),
(368560, 24, 1801.95, 1478.99, 80.60, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 25, 1804.20, 1482.93, 81.38, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 26, 1805.69, 1485.53, 81.21, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 27, 1809.38, 1491.81, 81.68, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 28, 1813.06, 1497.88, 82.35, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 29, 1816.94, 1503.96, 83.10, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 30, 1818.78, 1506.71, 84.34, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 31, 1819.90, 1508.37, 83.32, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 32, 1821.01, 1510.03, 83.85, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 33, 1821.83, 1511.03, 83.85, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 34, 1822.46, 1511.80, 84.71, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 35, 1825.64, 1515.66, 84.70, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 36, 1830.16, 1518.51, 86.21, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 37, 1831.85, 1519.58, 86.21, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 38, 1838.89, 1521.74, 88.40, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 39, 1846.18, 1524.31, 91.08, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 40, 1849.83, 1530.78, 91.43, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 41, 1852.78, 1537.47, 92.83, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 42, 1853.71, 1539.42, 92.90, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 43, 1854.14, 1540.32, 93.63, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 44, 1855.86, 1543.93, 93.70, 0, 0, 0, 0, 0, 0, 100, 102724, 0),
(368560, 45, 1856.42, 1544.92, 94.43, 0, 0, 0, 0, 0, 0, 100, 102724, 0);
UPDATE `creature` SET `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 368560;
UPDATE `creature` SET `MovementType` = 0, `spawndist` = 0 WHERE `guid` = 368562;
DELETE FROM `creature_formations` WHERE `leaderGUID` = 368560 OR `memberGUID` IN (368560, 368562);
INSERT INTO `creature_formations` (`leaderGUID`, `memberGUID`, `dist`, `angle`, `groupAI`) VALUES
(368560, 368560, 0, 0, 2),
(368560, 368562, 3, 90, 2);
DELETE FROM `creature_addon` WHERE `guid` IN (368656, 368595, 368560, 368562, 368616, 368586, 368579);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(368656, 368656, 0, 0, 1, 0, '198294'),
(368595, 368595, 0, 0, 1, 0, '198294'),
(368560, 368560, 0, 0, 1, 0, '151597'),
(368562, 0, 0, 0, 1, 0, '151597'),
(368616, 0, 0, 0, 1, 7, '151597'),
(368586, 0, 0, 0, 1, 7, '151597'),
(368579, 0, 0, 0, 1, 7, '151597');
DELETE FROM `waypoint_data` WHERE `id` BETWEEN 10267118 AND 10267120;
DELETE FROM `creature` WHERE `guid` = 600032;
UPDATE `creature` SET `orientation` = 0.94 WHERE `guid` = 368639;
DELETE FROM `creature_addon` WHERE `guid` = 368639;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(368639, 0, 0, 0, 1, 468, '151597 188560');
UPDATE `creature` SET `modelid` = 11686 WHERE `guid` = 368556;
UPDATE `gameobject_template` SET `data1` = 38727 WHERE `entry` IN (243965, 243967, 243968);
UPDATE `gameobject_template` SET `SpellStateVisualID` = 37794 WHERE `entry` IN (243965, 243967, 243968);
DELETE FROM `creature_addon` WHERE `guid` IN (368548, 368559, 368621, 368655, 146929243);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(368548, 0, 0, 0, 1, 468, '151597 188560'),
(368559, 0, 0, 0, 1, 468, '151597 188560'),
(368621, 0, 0, 0, 1, 468, '151597 188560'),
(368655, 0, 0, 0, 1, 468, '151597 188560'),
(146929243, 0, 0, 0, 1, 468, '151597 188560');
UPDATE `creature` SET `unit_flags` = 32768 WHERE `guid` IN (368559, 146929243);
DELETE FROM `waypoint_data` WHERE `id` IN (368605, 368606);
DELETE FROM `creature` WHERE `guid` IN (368605, 368606);
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 97014;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_mardum_injured_demon_hunter' WHERE `entry` IN (100543, 100545, 100548, 100549);
UPDATE `creature_template_addon` SET `bytes1` = 7 WHERE `entry` IN (100543, 100545, 100548, 100549);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 97370 AND `source_type` = 0 AND `id` IN (4, 5);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `Difficulties`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(97370, 0, 4, 0, '', 4, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'General Volroth - On Aggro - Say Line 0'),
(97370, 0, 5, 0, '', 0, 0, 100, 1, 15000, 15000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'General Volroth - In Combat - Say Line 1 once');

-- Mardum: the Legion Razorwings fly short irregular circuits around their spot
DELETE FROM `waypoint_data` WHERE `id` BETWEEN 10276300 AND 10276399 OR `id` IN (368061, 368081, 368082, 368083, 368084, 368377, 368424, 368475, 368476, 368566, 368571, 368652);
DELETE FROM `creature_addon` WHERE `guid` IN (368061, 368081, 368082, 368083, 368084, 368377, 368424, 368475, 368476, 368566, 368571, 368652);
UPDATE `creature` SET `position_x` = 1680.90, `position_y` = 1514.55, `position_z` = 58.37, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 368061;
UPDATE `creature` SET `position_x` = 1480.30, `position_y` = 1624.56, `position_z` = 16.52, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 368081;
UPDATE `creature` SET `position_x` = 1580.08, `position_y` = 1718.35, `position_z` = 14.77, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 368082;
UPDATE `creature` SET `position_x` = 1513.24, `position_y` = 1772.65, `position_z` = 33.69, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 368083;
UPDATE `creature` SET `position_x` = 1539.11, `position_y` = 1709.38, `position_z` = 28.01, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 368084;
UPDATE `creature` SET `position_x` = 1463.77, `position_y` = 1098.93, `position_z` = 130.15, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 368377;
UPDATE `creature` SET `position_x` = 1444.56, `position_y` = 1243.88, `position_z` = 133.10, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 368424;
UPDATE `creature` SET `position_x` = 1577.57, `position_y` = 1005.69, `position_z` = 99.33, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 368475;
UPDATE `creature` SET `position_x` = 1509.74, `position_y` = 995.15, `position_z` = 102.23, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 368476;
UPDATE `creature` SET `position_x` = 1945.32, `position_y` = 1554.62, `position_z` = 122.46, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 368566;
UPDATE `creature` SET `position_x` = 1739.96, `position_y` = 1541.48, `position_z` = 94.47, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 368571;
UPDATE `creature` SET `position_x` = 1848.98, `position_y` = 1632.33, `position_z` = 121.26, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 368652;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(368061, 10276300, 0, 50331648, 1, 0, '198294'),
(368081, 10276301, 0, 50331648, 1, 0, '198294'),
(368082, 10276302, 0, 50331648, 1, 0, '198294'),
(368083, 10276303, 0, 50331648, 1, 0, '198294'),
(368084, 10276304, 0, 50331648, 1, 0, '198294'),
(368377, 10276305, 0, 50331648, 1, 0, '198294'),
(368424, 10276306, 0, 50331648, 1, 0, '198294'),
(368475, 10276307, 0, 50331648, 1, 0, '198294'),
(368476, 10276308, 0, 50331648, 1, 0, '198294'),
(368566, 10276309, 0, 50331648, 1, 0, '198294'),
(368571, 10276310, 0, 50331648, 1, 0, '198294'),
(368652, 10276311, 0, 50331648, 1, 0, '198294');
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(10276300, 1, 1680.90, 1514.55, 58.37, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276300, 2, 1642.04, 1498.63, 56.18, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276300, 3, 1635.74, 1465.28, 54.11, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276300, 4, 1636.80, 1441.51, 55.23, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276300, 5, 1662.19, 1446.75, 48.83, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276300, 6, 1696.26, 1437.91, 48.19, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276300, 7, 1669.84, 1477.15, 51.05, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276301, 1, 1480.30, 1624.56, 16.52, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276301, 2, 1534.67, 1648.13, 13.96, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276301, 3, 1568.35, 1663.56, 12.28, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276301, 4, 1540.89, 1689.35, 17.53, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276301, 5, 1517.89, 1727.00, 15.23, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276301, 6, 1506.28, 1684.37, 15.54, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276301, 7, 1485.52, 1670.05, 16.92, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276302, 1, 1580.08, 1718.35, 14.77, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276302, 2, 1556.77, 1720.85, 8.48, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276302, 3, 1555.06, 1683.70, 10.05, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276302, 4, 1551.78, 1634.39, 5.49, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276302, 5, 1604.18, 1650.98, 8.32, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276302, 6, 1591.69, 1693.28, 7.76, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276303, 1, 1513.24, 1772.65, 33.69, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276303, 2, 1528.69, 1747.87, 31.90, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276303, 3, 1562.33, 1785.62, 32.61, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276303, 4, 1547.67, 1841.44, 25.16, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276303, 5, 1516.95, 1801.45, 29.27, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276304, 1, 1539.11, 1709.38, 28.01, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276304, 2, 1517.07, 1660.12, 21.82, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276304, 3, 1583.72, 1655.06, 25.67, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276304, 4, 1574.24, 1717.59, 27.73, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276304, 5, 1561.04, 1754.87, 18.67, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276305, 1, 1463.77, 1098.93, 130.15, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276305, 2, 1440.28, 1097.82, 124.16, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276305, 3, 1413.72, 1063.29, 123.79, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276305, 4, 1410.74, 1024.37, 123.13, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276305, 5, 1455.23, 1027.36, 119.30, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276305, 6, 1500.93, 1023.16, 121.18, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276305, 7, 1470.16, 1057.79, 130.55, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276306, 1, 1444.56, 1243.88, 133.10, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276306, 2, 1444.63, 1188.27, 133.33, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276306, 3, 1464.84, 1132.78, 126.24, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276306, 4, 1486.03, 1191.03, 135.11, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276306, 5, 1485.71, 1224.37, 132.28, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276307, 1, 1577.57, 1005.69, 99.33, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276307, 2, 1567.06, 1041.26, 111.89, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276307, 3, 1532.91, 1055.92, 100.03, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276307, 4, 1496.65, 1063.92, 107.13, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276307, 5, 1527.30, 1005.69, 111.42, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276307, 6, 1563.37, 985.35, 101.05, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276308, 1, 1509.74, 995.15, 102.23, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276308, 2, 1499.40, 1039.97, 104.63, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276308, 3, 1459.10, 1006.88, 113.10, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276308, 4, 1442.44, 987.87, 109.51, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276308, 5, 1499.72, 941.65, 109.02, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276309, 1, 1945.32, 1554.62, 122.46, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276309, 2, 1895.73, 1637.92, 121.24, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276309, 3, 1891.44, 1682.19, 116.89, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276309, 4, 1855.61, 1721.78, 123.39, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276309, 5, 1811.56, 1682.91, 119.69, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276309, 6, 1726.10, 1692.91, 91.17, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276309, 7, 1763.38, 1594.90, 93.78, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276309, 8, 1818.79, 1542.08, 114.15, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276309, 9, 1867.16, 1539.59, 130.15, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276310, 1, 1739.96, 1541.48, 94.47, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276310, 2, 1813.29, 1546.12, 108.90, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276310, 3, 1846.21, 1475.98, 135.80, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276310, 4, 1913.16, 1519.48, 138.43, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276310, 5, 1887.85, 1608.33, 120.03, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276310, 6, 1894.55, 1653.63, 120.64, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276310, 7, 1821.38, 1707.89, 120.28, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276310, 8, 1763.28, 1703.49, 118.22, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276310, 9, 1746.47, 1636.46, 99.54, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276311, 1, 1848.98, 1632.33, 121.26, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276311, 2, 1807.73, 1677.62, 112.70, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276311, 3, 1738.58, 1668.02, 102.74, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276311, 4, 1676.95, 1666.52, 82.37, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276311, 5, 1720.30, 1588.84, 86.86, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276311, 6, 1766.28, 1581.40, 102.65, 0, 0, 0, 1, 0, 0, 100, 0, 0),
(10276311, 7, 1855.06, 1530.94, 126.78, 0, 0, 0, 1, 0, 0, 100, 0, 0);

-- Mardum: Into the Foul Creche, Stop the Bombardment and Their Numbers Are Legion all open after Hidden No More, side by side
UPDATE `quest_template_addon` SET `PrevQuestID` = 39495, `NextQuestID` = 0, `ExclusiveGroup` = 0 WHERE `ID` IN (38725, 38727, 38819);
-- the broken bridge to the Soul Engine gives the bridge credit of Their Numbers Are Legion and starts Tyranna and Cyana's exchange
UPDATE `creature_template` SET `ScriptName` = 'conversation_announcer', `unit_flags` = 33554434 WHERE `entry` = 97400;
-- Stop the Bombardment is a quest to do, not one completed on acceptance
UPDATE `quest_template` SET `QuestType` = 2 WHERE `ID` = 38727;
DELETE FROM `creature` WHERE `guid` = 600034;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES (600034, 97400, 1481, 7705, 7713, 1, 1, '5837 5324 5203 5056', 1553.36, 1809.27, 4.44, 0, 120, 0, 0);
-- Tyranna and Cyana speak as actors: as creatures to find near the player, the conversation was dropped, Tyranna being far away
DELETE FROM `conversation_creature` WHERE `entry` = 540;
DELETE FROM `conversation_actor` WHERE `entry` = 540;
INSERT INTO `conversation_actor` (`entry`, `id`, `actorId`, `creatureId`, `displayId`, `unk1`, `unk2`, `unk3`, `duration`) VALUES
(540, 0, 49935, 93802, 65935, 0, 1, 0, 3900),
(540, 1, 49936, 97297, 63986, 0, 1, 0, 3900);

-- Mardum, Doom Fortress: crowded creepers and fel weavers thinned out, a doubled demon hunter removed, live spiders no longer lying as dead
DELETE FROM `creature_addon` WHERE `guid` IN (368251,368261,368301,368327,368340,368341,368347,368348,368349,368350,368351,368353,368354,368355,368359,368361,368362,368363,368364,368365,368303,368344,368356,368357,368358,368360,368366,368367,368375);
DELETE FROM `creature` WHERE `guid` IN (368251,368261,368301,368327,368340,368341,368347,368348,368349,368350,368351,368353,368354,368355,368359,368361,368362,368363,368364,368365,368303,368344,368356,368357,368358,368360,368366,368367,368375);
UPDATE `creature_template` SET `unit_flags2` = `unit_flags2` & ~0x1 WHERE `entry` IN (97706, 100243);
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_mardum_doom_fortress_stabilizer' WHERE `entry` = 101947;

-- Mardum, road to the Soul Engine: three walkers who keep out of the fights, the burning Spires of Woe, the devastators seen from afar
DELETE FROM `waypoint_data` WHERE `id` = 368393;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(368393, 0, 1466.00, 1110.00, 70.16, 0, 3000, 0, 0, 0, 0, 100, 96402, 0),
(368393, 1, 1467.00, 1117.00, 70.38, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 2, 1464.00, 1123.00, 71.22, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 3, 1458.00, 1127.00, 72.82, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 4, 1451.00, 1129.00, 74.67, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 5, 1445.00, 1131.00, 76.12, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 6, 1439.00, 1127.00, 77.08, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 7, 1432.00, 1124.00, 78.19, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 8, 1426.00, 1121.00, 78.75, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 9, 1424.08, 1120.45, 79.16, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 10, 1423.12, 1120.18, 80.03, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 11, 1421.19, 1119.63, 79.40, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 12, 1419.00, 1119.00, 79.56, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 13, 1412.00, 1119.00, 80.39, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 14, 1407.84, 1116.23, 81.30, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 15, 1406.00, 1115.00, 81.00, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 16, 1401.84, 1112.23, 82.03, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 17, 1400.00, 1111.00, 81.63, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 18, 1393.00, 1111.00, 83.23, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 19, 1386.00, 1111.00, 84.43, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 20, 1380.00, 1108.00, 85.63, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 21, 1377.00, 1104.00, 86.38, 0, 3000, 0, 0, 0, 0, 100, 96402, 0),
(368393, 22, 1380.00, 1108.00, 85.63, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 23, 1386.00, 1111.00, 84.43, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 24, 1393.00, 1111.00, 83.23, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 25, 1400.00, 1111.00, 81.63, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 26, 1401.84, 1112.23, 82.03, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 27, 1406.00, 1115.00, 81.00, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 28, 1407.84, 1116.23, 81.30, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 29, 1412.00, 1119.00, 80.39, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 30, 1419.00, 1119.00, 79.56, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 31, 1421.19, 1119.63, 79.40, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 32, 1423.12, 1120.18, 80.03, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 33, 1424.08, 1120.45, 79.16, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 34, 1426.00, 1121.00, 78.75, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 35, 1432.00, 1124.00, 78.19, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 36, 1439.00, 1127.00, 77.08, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 37, 1445.00, 1131.00, 76.12, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 38, 1451.00, 1129.00, 74.67, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 39, 1458.00, 1127.00, 72.82, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 40, 1464.00, 1123.00, 71.22, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 41, 1467.00, 1117.00, 70.38, 0, 0, 0, 0, 0, 0, 100, 96402, 0);
UPDATE `creature` SET `position_x` = 1466.00, `position_y` = 1110.00, `position_z` = 70.16, `MovementType` = 2, `spawndist` = 0, `unit_flags` = 16384 | 0x200 WHERE `guid` = 368393;
DELETE FROM `creature_addon` WHERE `guid` = 368393;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (368393, 368393, 0, 0, 1, 0, '198294');
DELETE FROM `waypoint_data` WHERE `id` = 368440;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(368440, 0, 1505.00, 1168.00, 72.46, 0, 3000, 0, 0, 0, 0, 100, 96504, 0),
(368440, 1, 1500.00, 1162.00, 72.43, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 2, 1495.00, 1157.00, 72.21, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 3, 1489.00, 1154.00, 72.39, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 4, 1482.00, 1151.00, 72.26, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 5, 1477.00, 1146.00, 72.63, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 6, 1473.93, 1143.44, 71.10, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 7, 1471.00, 1141.00, 70.90, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 8, 1470.00, 1134.00, 70.90, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 9, 1469.72, 1132.02, 71.58, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 10, 1469.00, 1127.00, 70.79, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 11, 1467.00, 1120.00, 70.53, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 12, 1463.00, 1115.00, 70.61, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 13, 1460.09, 1115.73, 71.09, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 14, 1459.00, 1116.00, 71.85, 0, 3000, 0, 0, 0, 0, 100, 96504, 0),
(368440, 15, 1460.09, 1115.73, 71.09, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 16, 1463.00, 1115.00, 70.61, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 17, 1467.00, 1120.00, 70.53, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 18, 1469.00, 1127.00, 70.79, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 19, 1469.72, 1132.02, 71.58, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 20, 1470.00, 1134.00, 70.90, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 21, 1471.00, 1141.00, 70.90, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 22, 1473.93, 1143.44, 71.10, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 23, 1477.00, 1146.00, 72.63, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 24, 1482.00, 1151.00, 72.26, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 25, 1489.00, 1154.00, 72.39, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 26, 1495.00, 1157.00, 72.21, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 27, 1500.00, 1162.00, 72.43, 0, 0, 0, 0, 0, 0, 100, 96504, 0);
UPDATE `creature` SET `position_x` = 1505.00, `position_y` = 1168.00, `position_z` = 72.46, `MovementType` = 2, `spawndist` = 0, `unit_flags` = 32768 | 0x200 WHERE `guid` = 368440;
DELETE FROM `creature_addon` WHERE `guid` = 368440;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (368440, 368440, 0, 0, 1, 0, '');
DELETE FROM `waypoint_data` WHERE `id` = 368462;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(368462, 0, 1618.00, 1190.00, 87.28, 0, 3000, 0, 0, 0, 0, 100, 96402, 0),
(368462, 1, 1617.11, 1189.55, 86.49, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 2, 1612.00, 1187.00, 85.52, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 3, 1605.00, 1187.00, 84.90, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 4, 1598.00, 1188.00, 84.16, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 5, 1591.00, 1188.00, 82.76, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 6, 1584.00, 1188.00, 81.71, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 7, 1578.00, 1193.00, 80.27, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 8, 1573.00, 1199.00, 78.44, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 9, 1566.00, 1199.00, 77.24, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 10, 1560.00, 1202.00, 75.74, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 11, 1553.00, 1202.00, 74.76, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 12, 1546.00, 1203.00, 73.61, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 13, 1539.00, 1205.00, 72.43, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 14, 1532.00, 1205.00, 72.10, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 15, 1528.00, 1205.00, 72.15, 0, 3000, 0, 0, 0, 0, 100, 96402, 0),
(368462, 16, 1532.00, 1205.00, 72.10, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 17, 1539.00, 1205.00, 72.43, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 18, 1546.00, 1203.00, 73.61, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 19, 1553.00, 1202.00, 74.76, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 20, 1560.00, 1202.00, 75.74, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 21, 1566.00, 1199.00, 77.24, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 22, 1573.00, 1199.00, 78.44, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 23, 1578.00, 1193.00, 80.27, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 24, 1584.00, 1188.00, 81.71, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 25, 1591.00, 1188.00, 82.76, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 26, 1598.00, 1188.00, 84.16, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 27, 1605.00, 1187.00, 84.90, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 28, 1612.00, 1187.00, 85.52, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 29, 1617.11, 1189.55, 86.49, 0, 0, 0, 0, 0, 0, 100, 96402, 0);
UPDATE `creature` SET `position_x` = 1618.00, `position_y` = 1190.00, `position_z` = 87.28, `MovementType` = 2, `spawndist` = 0, `unit_flags` = 16384 | 0x200 WHERE `guid` = 368462;
DELETE FROM `creature_addon` WHERE `guid` = 368462;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (368462, 368462, 0, 0, 1, 0, '198294');
UPDATE `gameobject_template` SET `flags` = 32, `ScriptName` = 'go_mardum_spire_stabilizer' WHERE `entry` = 245995;
UPDATE `gameobject` SET `state` = 1 WHERE `id` = 245995;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_mardum_spire_of_woe_fire' WHERE `entry` IN (101318, 101321);
UPDATE `creature_template_addon` SET `auras` = '' WHERE `entry` IN (101318, 101321);
UPDATE `creature_template_wdb` SET `Displayid1` = 11686, `Displayid2` = 0, `Displayid3` = 0, `Displayid4` = 0 WHERE `Entry` IN (101318, 101320, 101321);
DELETE FROM `creature_template_movement` WHERE `CreatureId` = 101320;
INSERT INTO `creature_template_movement` (`CreatureId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Random`) VALUES (101320, 0, 0, 1, 1, 0);
DELETE FROM `creature` WHERE `guid` IN (368456, 600033);
DELETE FROM `creature_addon` WHERE `guid` = 368456;
UPDATE `creature` SET `position_x` = 1566.50, `position_y` = 1174.11, `position_z` = 98.00 WHERE `guid` = 368455;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES (600033, 101320, 1481, 7705, 7820, 1, 1, '5837 5324 5203 5117 5116 5114 5113 5056', 1440.99, 1011.00, 88.70, 0, 120, 0, 0);
UPDATE `creature_template` SET `MaxVisible` = 1, `VisibilityDistance` = 533 WHERE `entry` IN (93762, 96731, 96732);

-- Mardum, road to the Soul Engine: three walkers who keep out of the fights, the burning Spires of Woe, the devastators seen from afar
DELETE FROM `waypoint_data` WHERE `id` = 368393;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(368393, 0, 1466.00, 1110.00, 70.16, 0, 3000, 0, 0, 0, 0, 100, 96402, 0),
(368393, 1, 1467.00, 1117.00, 70.38, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 2, 1464.00, 1123.00, 71.22, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 3, 1458.00, 1127.00, 72.82, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 4, 1451.00, 1129.00, 74.67, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 5, 1445.00, 1131.00, 76.12, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 6, 1439.00, 1127.00, 77.08, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 7, 1432.00, 1124.00, 78.19, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 8, 1426.00, 1121.00, 78.75, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 9, 1424.08, 1120.45, 79.16, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 10, 1423.12, 1120.18, 80.03, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 11, 1421.19, 1119.63, 79.40, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 12, 1419.00, 1119.00, 79.56, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 13, 1412.00, 1119.00, 80.39, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 14, 1407.84, 1116.23, 81.30, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 15, 1406.00, 1115.00, 81.00, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 16, 1401.84, 1112.23, 82.03, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 17, 1400.00, 1111.00, 81.63, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 18, 1393.00, 1111.00, 83.23, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 19, 1386.00, 1111.00, 84.43, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 20, 1380.00, 1108.00, 85.63, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 21, 1377.00, 1104.00, 86.38, 0, 3000, 0, 0, 0, 0, 100, 96402, 0),
(368393, 22, 1380.00, 1108.00, 85.63, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 23, 1386.00, 1111.00, 84.43, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 24, 1393.00, 1111.00, 83.23, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 25, 1400.00, 1111.00, 81.63, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 26, 1401.84, 1112.23, 82.03, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 27, 1406.00, 1115.00, 81.00, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 28, 1407.84, 1116.23, 81.30, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 29, 1412.00, 1119.00, 80.39, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 30, 1419.00, 1119.00, 79.56, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 31, 1421.19, 1119.63, 79.40, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 32, 1423.12, 1120.18, 80.03, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 33, 1424.08, 1120.45, 79.16, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 34, 1426.00, 1121.00, 78.75, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 35, 1432.00, 1124.00, 78.19, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 36, 1439.00, 1127.00, 77.08, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 37, 1445.00, 1131.00, 76.12, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 38, 1451.00, 1129.00, 74.67, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 39, 1458.00, 1127.00, 72.82, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 40, 1464.00, 1123.00, 71.22, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368393, 41, 1467.00, 1117.00, 70.38, 0, 0, 0, 0, 0, 0, 100, 96402, 0);
UPDATE `creature` SET `position_x` = 1466.00, `position_y` = 1110.00, `position_z` = 70.16, `MovementType` = 2, `spawndist` = 0, `unit_flags` = 16384 | 0x200 WHERE `guid` = 368393;
DELETE FROM `creature_addon` WHERE `guid` = 368393;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (368393, 368393, 0, 0, 1, 0, '198294');
DELETE FROM `waypoint_data` WHERE `id` = 368440;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(368440, 0, 1505.00, 1168.00, 72.46, 0, 3000, 0, 0, 0, 0, 100, 96504, 0),
(368440, 1, 1500.00, 1162.00, 72.43, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 2, 1495.00, 1157.00, 72.21, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 3, 1489.00, 1154.00, 72.39, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 4, 1482.00, 1151.00, 72.26, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 5, 1477.00, 1146.00, 72.63, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 6, 1473.93, 1143.44, 71.10, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 7, 1471.00, 1141.00, 70.90, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 8, 1470.00, 1134.00, 70.90, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 9, 1469.72, 1132.02, 71.58, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 10, 1469.00, 1127.00, 70.79, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 11, 1467.00, 1120.00, 70.53, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 12, 1463.00, 1115.00, 70.61, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 13, 1460.09, 1115.73, 71.09, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 14, 1459.00, 1116.00, 71.85, 0, 3000, 0, 0, 0, 0, 100, 96504, 0),
(368440, 15, 1460.09, 1115.73, 71.09, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 16, 1463.00, 1115.00, 70.61, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 17, 1467.00, 1120.00, 70.53, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 18, 1469.00, 1127.00, 70.79, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 19, 1469.72, 1132.02, 71.58, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 20, 1470.00, 1134.00, 70.90, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 21, 1471.00, 1141.00, 70.90, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 22, 1473.93, 1143.44, 71.10, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 23, 1477.00, 1146.00, 72.63, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 24, 1482.00, 1151.00, 72.26, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 25, 1489.00, 1154.00, 72.39, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 26, 1495.00, 1157.00, 72.21, 0, 0, 0, 0, 0, 0, 100, 96504, 0),
(368440, 27, 1500.00, 1162.00, 72.43, 0, 0, 0, 0, 0, 0, 100, 96504, 0);
UPDATE `creature` SET `position_x` = 1505.00, `position_y` = 1168.00, `position_z` = 72.46, `MovementType` = 2, `spawndist` = 0, `unit_flags` = 32768 | 0x200 WHERE `guid` = 368440;
DELETE FROM `creature_addon` WHERE `guid` = 368440;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (368440, 368440, 0, 0, 1, 0, '');
DELETE FROM `waypoint_data` WHERE `id` = 368462;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(368462, 0, 1618.00, 1190.00, 87.28, 0, 3000, 0, 0, 0, 0, 100, 96402, 0),
(368462, 1, 1617.11, 1189.55, 86.49, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 2, 1612.00, 1187.00, 85.52, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 3, 1605.00, 1187.00, 84.90, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 4, 1598.00, 1188.00, 84.16, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 5, 1591.00, 1188.00, 82.76, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 6, 1584.00, 1188.00, 81.71, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 7, 1578.00, 1193.00, 80.27, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 8, 1573.00, 1199.00, 78.44, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 9, 1566.00, 1199.00, 77.24, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 10, 1560.00, 1202.00, 75.74, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 11, 1553.00, 1202.00, 74.76, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 12, 1546.00, 1203.00, 73.61, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 13, 1539.00, 1205.00, 72.43, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 14, 1532.00, 1205.00, 72.10, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 15, 1528.00, 1205.00, 72.15, 0, 3000, 0, 0, 0, 0, 100, 96402, 0),
(368462, 16, 1532.00, 1205.00, 72.10, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 17, 1539.00, 1205.00, 72.43, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 18, 1546.00, 1203.00, 73.61, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 19, 1553.00, 1202.00, 74.76, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 20, 1560.00, 1202.00, 75.74, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 21, 1566.00, 1199.00, 77.24, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 22, 1573.00, 1199.00, 78.44, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 23, 1578.00, 1193.00, 80.27, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 24, 1584.00, 1188.00, 81.71, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 25, 1591.00, 1188.00, 82.76, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 26, 1598.00, 1188.00, 84.16, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 27, 1605.00, 1187.00, 84.90, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 28, 1612.00, 1187.00, 85.52, 0, 0, 0, 0, 0, 0, 100, 96402, 0),
(368462, 29, 1617.11, 1189.55, 86.49, 0, 0, 0, 0, 0, 0, 100, 96402, 0);
UPDATE `creature` SET `position_x` = 1618.00, `position_y` = 1190.00, `position_z` = 87.28, `MovementType` = 2, `spawndist` = 0, `unit_flags` = 16384 | 0x200 WHERE `guid` = 368462;
DELETE FROM `creature_addon` WHERE `guid` = 368462;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (368462, 368462, 0, 0, 1, 0, '198294');
UPDATE `gameobject_template` SET `flags` = 32, `ScriptName` = 'go_mardum_spire_stabilizer' WHERE `entry` = 245995;
UPDATE `gameobject` SET `state` = 1 WHERE `id` = 245995;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_mardum_spire_of_woe_fire' WHERE `entry` IN (101318, 101321);
UPDATE `creature_template_addon` SET `auras` = '' WHERE `entry` IN (101318, 101321);
UPDATE `creature_template_wdb` SET `Displayid1` = 11686, `Displayid2` = 0, `Displayid3` = 0, `Displayid4` = 0 WHERE `Entry` IN (101318, 101320, 101321);
DELETE FROM `creature_template_movement` WHERE `CreatureId` = 101320;
INSERT INTO `creature_template_movement` (`CreatureId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Random`) VALUES (101320, 0, 0, 1, 1, 0);
DELETE FROM `creature` WHERE `guid` IN (368456, 600033);
DELETE FROM `creature_addon` WHERE `guid` = 368456;
UPDATE `creature` SET `position_x` = 1566.50, `position_y` = 1174.11, `position_z` = 100.00 WHERE `guid` = 368455;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES (600033, 101320, 1481, 7705, 7820, 1, 1, '5837 5324 5203 5117 5116 5114 5113 5056', 1440.99, 1011.00, 90.70, 0, 120, 0, 0);
UPDATE `creature_template` SET `MaxVisible` = 1, `VisibilityDistance` = 533 WHERE `entry` IN (93762, 96731, 96732);

-- the second line of the bridge exchange is Cyana's: actor 1 (the lowest byte of unk2 is the line's actor)
UPDATE `conversation_data` SET `unk2` = 4128769 WHERE `entry` = 540 AND `idx` = 1;

-- Mardum, Illidari outpost: two demon hunters asleep without their mounts, two walking up and down the camp
DELETE FROM `creature_addon` WHERE `guid` IN (368010, 368021, 368032, 146929818);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(368010, 0, 0, 3, 1, 0, ''),
(368021, 0, 0, 3, 1, 0, ''),
(368032, 368032, 65445, 0, 1, 0, ''),
(146929818, 146929818, 65445, 0, 1, 0, '');
DELETE FROM `waypoint_data` WHERE `id` = 368032;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(368032, 0, 1456.00, 1724.00, 47.81, 0, 3000, 0, 0, 0, 0, 100, 97600, 0),
(368032, 1, 1453.88, 1721.88, 49.18, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 2, 1451.00, 1719.00, 49.21, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 3, 1444.00, 1719.00, 50.09, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 4, 1438.00, 1724.00, 51.52, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 5, 1431.00, 1726.00, 52.89, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 6, 1426.00, 1731.00, 53.66, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 7, 1426.00, 1738.00, 54.19, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 8, 1425.00, 1745.00, 55.30, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 9, 1427.00, 1752.00, 55.41, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 10, 1432.00, 1757.00, 54.83, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 11, 1434.83, 1759.83, 54.83, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 12, 1437.00, 1762.00, 55.86, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 13, 1440.54, 1765.54, 56.82, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 14, 1442.00, 1767.00, 55.51, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 15, 1447.00, 1772.00, 55.01, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 16, 1450.54, 1775.54, 54.69, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 17, 1452.00, 1777.00, 55.15, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 18, 1457.00, 1782.00, 55.78, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 19, 1462.00, 1787.00, 55.46, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 20, 1464.83, 1789.83, 54.55, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 21, 1465.54, 1790.54, 53.66, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 22, 1467.00, 1792.00, 53.09, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 23, 1472.00, 1797.00, 52.44, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 24, 1476.24, 1801.24, 52.29, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 25, 1477.00, 1802.00, 52.74, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 26, 1479.00, 1805.00, 53.17, 0, 3000, 0, 0, 0, 0, 100, 97600, 0),
(368032, 27, 1477.00, 1802.00, 52.74, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 28, 1476.24, 1801.24, 52.29, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 29, 1472.00, 1797.00, 52.44, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 30, 1467.00, 1792.00, 53.09, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 31, 1465.54, 1790.54, 53.66, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 32, 1464.83, 1789.83, 54.55, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 33, 1462.00, 1787.00, 55.46, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 34, 1457.00, 1782.00, 55.78, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 35, 1452.00, 1777.00, 55.15, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 36, 1450.54, 1775.54, 54.69, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 37, 1447.00, 1772.00, 55.01, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 38, 1442.00, 1767.00, 55.51, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 39, 1440.54, 1765.54, 56.82, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 40, 1437.00, 1762.00, 55.86, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 41, 1434.83, 1759.83, 54.83, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 42, 1432.00, 1757.00, 54.83, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 43, 1427.00, 1752.00, 55.41, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 44, 1425.00, 1745.00, 55.30, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 45, 1426.00, 1738.00, 54.19, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 46, 1426.00, 1731.00, 53.66, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 47, 1431.00, 1726.00, 52.89, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 48, 1438.00, 1724.00, 51.52, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 49, 1444.00, 1719.00, 50.09, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 50, 1451.00, 1719.00, 49.21, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(368032, 51, 1453.88, 1721.88, 49.18, 0, 0, 0, 0, 0, 0, 100, 97600, 0);
UPDATE `creature` SET `position_x` = 1456.00, `position_y` = 1724.00, `position_z` = 47.81, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 368032;
DELETE FROM `waypoint_data` WHERE `id` = 146929818;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(146929818, 0, 1448.00, 1785.00, 54.99, 0, 3000, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 1, 1448.00, 1778.00, 54.92, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 2, 1446.00, 1771.00, 54.78, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 3, 1441.00, 1766.00, 56.11, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 4, 1439.59, 1764.59, 56.79, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 5, 1438.88, 1763.88, 56.01, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 6, 1436.00, 1761.00, 55.30, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 7, 1431.00, 1756.00, 54.94, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 8, 1426.00, 1751.00, 55.46, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 9, 1425.00, 1744.00, 55.30, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 10, 1425.00, 1737.00, 54.21, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 11, 1425.00, 1730.00, 53.90, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 12, 1423.00, 1723.00, 54.46, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 13, 1419.00, 1717.00, 55.48, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 14, 1418.00, 1710.00, 55.11, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 15, 1416.00, 1703.00, 54.97, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 16, 1416.00, 1702.00, 54.97, 0, 3000, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 17, 1416.00, 1703.00, 54.97, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 18, 1418.00, 1710.00, 55.11, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 19, 1419.00, 1717.00, 55.48, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 20, 1423.00, 1723.00, 54.46, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 21, 1425.00, 1730.00, 53.90, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 22, 1425.00, 1737.00, 54.21, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 23, 1425.00, 1744.00, 55.30, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 24, 1426.00, 1751.00, 55.46, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 25, 1431.00, 1756.00, 54.94, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 26, 1436.00, 1761.00, 55.30, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 27, 1438.88, 1763.88, 56.01, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 28, 1439.59, 1764.59, 56.79, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 29, 1441.00, 1766.00, 56.11, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 30, 1446.00, 1771.00, 54.78, 0, 0, 0, 0, 0, 0, 100, 97600, 0),
(146929818, 31, 1448.00, 1778.00, 54.92, 0, 0, 0, 0, 0, 0, 100, 97600, 0);
UPDATE `creature` SET `position_x` = 1448.00, `position_y` = 1785.00, `position_z` = 54.99, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 146929818;
UPDATE `creature_template` SET `dmg_multiplier` = 0.5 WHERE `entry` = 101317;
UPDATE `creature` SET `PhaseId` = '5837 5324 5116 5115 5114 5056 4884 4883 4881' WHERE `guid` = 146927508;

-- Mardum: Tyranna's conversations with her own lines and portrait camera, and conversation 569 paced so that each line is read before the next
DELETE FROM `conversation_creature` WHERE `entry` IN (510, 527, 540, 541);
DELETE FROM `conversation_actor` WHERE `entry` IN (510, 527, 540, 541);
INSERT INTO `conversation_actor` (`entry`, `id`, `actorId`, `creatureId`, `displayId`, `unk1`, `unk2`, `unk3`, `duration`) VALUES
(510, 0, 49935, 93802, 65935, 0, 1, 0, 4500),
(527, 0, 49935, 93802, 65935, 0, 1, 0, 5500),
(540, 0, 49935, 93802, 65935, 0, 1, 0, 8000),
(541, 0, 49935, 93802, 65935, 0, 1, 0, 5000);
DELETE FROM `conversation_data` WHERE `entry` IN (510, 527, 540, 541);
INSERT INTO `conversation_data` (`entry`, `id`, `idx`, `textId`, `unk1`, `unk2`) VALUES
(510, 1354, 0, 0, 262, 0),
(527, 1375, 0, 0, 262, 0),
(540, 1409, 0, 0, 262, 0),
(540, 1410, 1, 4500, 262, 0),
(541, 1410, 0, 0, 262, 0);
UPDATE `conversation_actor` SET `duration` = 10500 WHERE `entry` = 569;
UPDATE `conversation_data` SET `textId` = 0 WHERE `entry` = 569 AND `id` = 1476;
UPDATE `conversation_data` SET `textId` = 6500 WHERE `entry` = 569 AND `id` = 1477;
UPDATE `conversation_data` SET `textId` = 13500 WHERE `entry` = 569 AND `id` = 1478;
UPDATE `conversation_data` SET `textId` = 18500 WHERE `entry` = 569 AND `id` = 1479;
UPDATE `conversation_data` SET `textId` = 24000 WHERE `entry` = 569 AND `id` = 1480;
UPDATE `conversation_data` SET `textId` = 29500 WHERE `entry` = 569 AND `id` = 1481;
UPDATE `conversation_data` SET `textId` = 32500 WHERE `entry` = 569 AND `id` = 1736;
UPDATE `conversation_data` SET `textId` = 35500 WHERE `entry` = 569 AND `id` = 1737;
UPDATE `conversation_data` SET `textId` = 41500 WHERE `entry` = 569 AND `id` = 1738;
UPDATE `conversation_data` SET `textId` = 48000 WHERE `entry` = 569 AND `id` = 1739;
UPDATE `conversation_data` SET `textId` = 52000 WHERE `entry` = 569 AND `id` = 1828;

-- Mardum: the specialization quest matches the chosen specialization, and the flight to the Fel Hammer waits for the work before it
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` IN (19, 20) AND `SourceEntry` IN (39515, 39516, 39663);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(19, 0, 39663, 0, 0, 8, 0, 38727, 0, 0, 0, 0, '', 'On Felbat Wings needs quest 38727 rewarded'),
(19, 0, 39663, 0, 0, 8, 0, 38819, 0, 0, 0, 0, '', 'On Felbat Wings needs quest 38819 rewarded'),
(19, 0, 39663, 0, 0, 8, 0, 39515, 0, 0, 0, 0, '', 'On Felbat Wings needs quest 39515 rewarded'),
(19, 0, 39663, 0, 1, 8, 0, 38727, 0, 0, 0, 0, '', 'On Felbat Wings needs quest 38727 rewarded'),
(19, 0, 39663, 0, 1, 8, 0, 38819, 0, 0, 0, 0, '', 'On Felbat Wings needs quest 38819 rewarded'),
(19, 0, 39663, 0, 1, 8, 0, 39516, 0, 0, 0, 0, '', 'On Felbat Wings needs quest 39516 rewarded'),
(19, 0, 39515, 0, 0, 55, 0, 581, 0, 0, 0, 0, '', 'Vengeance Will Be Mine! for the Vengeance specialization'),
(19, 0, 39516, 0, 0, 55, 0, 577, 0, 0, 0, 0, '', 'Cry Havoc for the Havoc specialization'),
(20, 0, 39663, 0, 0, 8, 0, 38727, 0, 0, 0, 0, '', 'On Felbat Wings needs quest 38727 rewarded'),
(20, 0, 39663, 0, 0, 8, 0, 38819, 0, 0, 0, 0, '', 'On Felbat Wings needs quest 38819 rewarded'),
(20, 0, 39663, 0, 0, 8, 0, 39515, 0, 0, 0, 0, '', 'On Felbat Wings needs quest 39515 rewarded'),
(20, 0, 39663, 0, 1, 8, 0, 38727, 0, 0, 0, 0, '', 'On Felbat Wings needs quest 38727 rewarded'),
(20, 0, 39663, 0, 1, 8, 0, 38819, 0, 0, 0, 0, '', 'On Felbat Wings needs quest 38819 rewarded'),
(20, 0, 39663, 0, 1, 8, 0, 39516, 0, 0, 0, 0, '', 'On Felbat Wings needs quest 39516 rewarded'),
(20, 0, 39515, 0, 0, 55, 0, 581, 0, 0, 0, 0, '', 'Vengeance Will Be Mine! for the Vengeance specialization'),
(20, 0, 39516, 0, 0, 55, 0, 577, 0, 0, 0, 0, '', 'Cry Havoc for the Havoc specialization');

-- Mardum: Mannethrel Darkstar bursts with the power he is taught, and Kayn sees him go
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_mardum_mannethrel_darkstar' WHERE `entry` = 96652;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 96652 AND `source_type` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 93127 AND `GroupID` = 6;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextID`, `MinTimer`, `MaxTimer`, `SpellID`, `comment`) VALUES
(93127, 6, 0, 'No... Mannethrel.', 12, 0, 100, 0, 0, 55262, 100936, 0, 0, 0, 'Kayn Sunfury to Player');

-- Mardum: each captain taught the specialization says what the power does to them, roars and burns with fel
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (96655, 96420, 99045) AND `source_type` = 0 AND `id` IN (3, 4, 5);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `Difficulties`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(96655, 0, 3, 0, '', 62, 0, 100, 0, 18935, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Teaching - say the power felt'),
(96655, 0, 4, 0, '', 62, 0, 100, 0, 18935, 0, 0, 0, 0, 5, 53, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Teaching - battle roar'),
(96655, 0, 5, 0, '', 62, 0, 100, 0, 18935, 0, 0, 0, 0, 11, 178740, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Teaching - wrapped in fel flames'),
(96420, 0, 3, 0, '', 62, 0, 100, 0, 18936, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Teaching - say the power felt'),
(96420, 0, 4, 0, '', 62, 0, 100, 0, 18936, 0, 0, 0, 0, 5, 53, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Teaching - battle roar'),
(96420, 0, 5, 0, '', 62, 0, 100, 0, 18936, 0, 0, 0, 0, 11, 178740, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Teaching - wrapped in fel flames'),
(99045, 0, 3, 0, '', 62, 0, 100, 0, 18937, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Teaching - say the power felt'),
(99045, 0, 4, 0, '', 62, 0, 100, 0, 18937, 0, 0, 0, 0, 5, 53, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Teaching - battle roar'),
(99045, 0, 5, 0, '', 62, 0, 100, 0, 18937, 0, 0, 0, 0, 11, 178740, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Teaching - wrapped in fel flames');

-- Mardum: the spiders of the Fel Hammer laid out as corpses are alive, they fight and can be fought
UPDATE `creature` SET `unit_flags` = 0, `dynamicflags` = 0 WHERE `guid` IN (368712, 368755, 368759, 368875, 368879, 368851, 368858, 368891);

-- Mardum: the Aranasi Maidens of the Fel Hammer are alive and channel fel into the motes over the vault
UPDATE `creature_template` SET `unit_flags` = 32768, `AIName` = 'SmartAI' WHERE `entry` = 100244;
UPDATE `creature_template_addon` SET `auras` = '' WHERE `entry` = 100244;
UPDATE `creature` SET `unit_flags` = 0, `dynamicflags` = 0 WHERE `id` = 100244;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 100244 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `Difficulties`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(100244, 0, 0, 0, '', 1, 0, 100, 0, 1000, 2000, 20000, 20000, 0, 11, 195898, 0, 0, 0, 0, 0, 19, 24021, 50, 0, 0, 0, 0, 0, 0, 'Aranasi Maiden - out of combat - Fel Channelling on the nearest mote');
DELETE FROM `smart_scripts` WHERE `entryorguid` = -368767 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `Difficulties`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-368767, 0, 0, 0, '', 25, 0, 100, 0, 0, 0, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Aranasi Maiden on the ledge - no aura, no channel');

-- Mardum: no broodling waits in the Fel Hammer, Tyranna's fight brings them down the threads from the vault
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_mardum_skittering_broodling' WHERE `entry` IN (100333, 100334);
DELETE FROM `creature_addon` WHERE `guid` IN (SELECT `guid` FROM `creature` WHERE `id` IN (100333, 100334));
DELETE FROM `creature` WHERE `id` IN (100333, 100334);

-- Mardum: Kayn, Jace, Allari and Kor'vas are not spawned at Tyranna's feet, they come in and charge when she appears
DELETE FROM `creature` WHERE `guid` IN (368849, 368974, 368975, 368976);
DELETE FROM `creature_addon` WHERE `guid` IN (368849, 368974, 368975, 368976);
DELETE FROM `vehicle_template_accessory` WHERE `EntryOrAura` = 93802 AND `accessory_entry` = 97959;
