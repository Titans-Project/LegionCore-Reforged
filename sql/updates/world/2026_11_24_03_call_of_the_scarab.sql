-- Call of the Scarab: the holiday of Silithus, its quests, world quests, loot and bosses

DELETE FROM `outdoorpvp_template` WHERE `TypeId` = 5;
INSERT INTO `outdoorpvp_template` (`TypeId`, `ScriptName`, `ZoneList`, `MapList`, `comment`) VALUES
(5, 'outdoorpvp_silithus', '1377 5695', '1', 'Silithus: Call of the Scarab');

UPDATE `game_event` SET `start_time` = `end_time` WHERE `eventEntry` IN (76, 77) AND `start_time` IS NULL;
UPDATE `game_event` SET `length` = 527040 WHERE `eventEntry` IN (76, 77);

UPDATE `quest_template` SET `LogTitle` = 'Sandworm Meat' WHERE `ID` = 45637;
UPDATE `quest_template` SET `LogTitle` = 'Crunchy Spider Legs' WHERE `ID` = 45638;
UPDATE `quest_template` SET `LogTitle` = 'Chilled Meat' WHERE `ID` = 45639;
UPDATE `quest_template` SET `LogTitle` = 'Crocolisk Tails' WHERE `ID` = 45640;
UPDATE `quest_template` SET `LogTitle` = 'Raw Tiger Steaks' WHERE `ID` = 45641;
UPDATE `quest_template` SET `LogTitle` = 'Raw Clefthoof Meat' WHERE `ID` = 45642;
UPDATE `quest_template` SET `LogTitle` = 'Lean Shanks' WHERE `ID` = 45643;
UPDATE `quest_template` SET `LogTitle` = 'The Ruins of Ahn\'Qiraj: Ossirian' WHERE `ID` = 45651;
UPDATE `quest_template` SET `LogTitle` = 'Ahn\'Qiraj: C\'thun' WHERE `ID` = 45654;
UPDATE `quest_template` SET `LogTitle` = 'The Colossus of Ashi' WHERE `ID` = 45655;
UPDATE `quest_template` SET `LogTitle` = 'Lesser Wind Stones' WHERE `ID` = 45656;
UPDATE `quest_template` SET `LogTitle` = 'Silithyst' WHERE `ID` = 45657;
UPDATE `quest_template` SET `LogTitle` = 'The Colossus of Regal' WHERE `ID` = 45674;
UPDATE `quest_template` SET `LogTitle` = 'The Colossus of Zora' WHERE `ID` = 45675;
UPDATE `quest_template` SET `LogTitle` = 'Sandworm Meat' WHERE `ID` = 45729;
UPDATE `quest_template` SET `LogTitle` = 'Crunchy Spider Legs' WHERE `ID` = 45730;
UPDATE `quest_template` SET `LogTitle` = 'Chilled Meat' WHERE `ID` = 45731;
UPDATE `quest_template` SET `LogTitle` = 'Crocolisk Tails' WHERE `ID` = 45732;
UPDATE `quest_template` SET `LogTitle` = 'Raw Tiger Steaks' WHERE `ID` = 45733;
UPDATE `quest_template` SET `LogTitle` = 'Raw Clefthoof Meat' WHERE `ID` = 45734;
UPDATE `quest_template` SET `LogTitle` = 'Lean Shanks' WHERE `ID` = 45735;
UPDATE `quest_template` SET `LogTitle` = 'The Ruins of Ahn\'Qiraj: Ossirian' WHERE `ID` = 45736;
UPDATE `quest_template` SET `LogTitle` = 'Ahn\'Qiraj: C\'thun' WHERE `ID` = 45737;
UPDATE `quest_template` SET `LogTitle` = 'The Colossus of Ashi' WHERE `ID` = 45738;
UPDATE `quest_template` SET `LogTitle` = 'Lesser Wind Stones' WHERE `ID` = 45739;
UPDATE `quest_template` SET `LogTitle` = 'Silithyst' WHERE `ID` = 45740;
UPDATE `quest_template` SET `LogTitle` = 'The Colossus of Regal' WHERE `ID` = 45741;
UPDATE `quest_template` SET `LogTitle` = 'The Colossus of Zora' WHERE `ID` = 45742;
UPDATE `quest_template` SET `LogTitle` = 'Cinder of Cynders' WHERE `ID` = 45747;
UPDATE `quest_template` SET `LogTitle` = 'Pearl of Fathoms' WHERE `ID` = 45748;
UPDATE `quest_template` SET `LogTitle` = 'Salt of Shards' WHERE `ID` = 45749;
UPDATE `quest_template` SET `LogTitle` = 'Song of Zephyrs' WHERE `ID` = 45750;
UPDATE `quest_template` SET `LogTitle` = 'Coreforged Sigil of Skaldrenox' WHERE `ID` = 45751;
UPDATE `quest_template` SET `LogTitle` = 'Glimmering Sigil of Whirlaxis' WHERE `ID` = 45752;
UPDATE `quest_template` SET `LogTitle` = 'Diamondine Sigil of Kazum' WHERE `ID` = 45753;
UPDATE `quest_template` SET `LogTitle` = 'Tidal Sigil of Skwol' WHERE `ID` = 45754;
UPDATE `quest_template` SET `LogTitle` = 'Cinder of Cynders' WHERE `ID` = 45755;
UPDATE `quest_template` SET `LogTitle` = 'Pearl of Fathoms' WHERE `ID` = 45756;
UPDATE `quest_template` SET `LogTitle` = 'Salt of Shards' WHERE `ID` = 45757;
UPDATE `quest_template` SET `LogTitle` = 'Song of Zephyrs' WHERE `ID` = 45758;
UPDATE `quest_template` SET `LogTitle` = 'Coreforged Sigil of Skaldrenox' WHERE `ID` = 45759;
UPDATE `quest_template` SET `LogTitle` = 'Glimmering Sigil of Whirlaxis' WHERE `ID` = 45760;
UPDATE `quest_template` SET `LogTitle` = 'Diamondine Sigil of Kazum' WHERE `ID` = 45761;
UPDATE `quest_template` SET `LogTitle` = 'Tidal Sigil of Skwol' WHERE `ID` = 45762;
UPDATE `quest_template` SET `LogTitle` = 'Call of the Scarab' WHERE `ID` = 45785;
UPDATE `quest_template` SET `LogTitle` = 'Call of the Scarab' WHERE `ID` = 45787;

UPDATE `quest_template_locale` SET `LogTitle` = 'Chair de ver des sables' WHERE `ID` = 45637 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Pattes d’araignée croustillantes' WHERE `ID` = 45638 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Viande glacée' WHERE `ID` = 45639 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Queues de crocilisque' WHERE `ID` = 45640 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Steaks de tigre crus' WHERE `ID` = 45641 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Viande de sabot-fourchu crue' WHERE `ID` = 45642 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Jarrets dégraissés' WHERE `ID` = 45643 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Ahn’Qiraj : C’Thun' WHERE `ID` = 45654 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Chair de ver des sables' WHERE `ID` = 45729 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Pattes d’araignée croustillantes' WHERE `ID` = 45730 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Viande glacée' WHERE `ID` = 45731 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Queues de crocilisque' WHERE `ID` = 45732 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Steaks de tigre crus' WHERE `ID` = 45733 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Viande de sabot-fourchu crue' WHERE `ID` = 45734 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Jarrets dégraissés' WHERE `ID` = 45735 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Ahn’Qiraj : C’Thun' WHERE `ID` = 45737 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Cachet diamantin de Kazum', `LogDescription` = 'Apportez le Cachet diamantin de Kazum au Seigneur de guerre Gorchuk en Silithus.', `QuestDescription` = 'Les cachets du Haut conseil abyssal sont composés d’essence distillée de l’élément de leur porteur. Celui-ci a été gravé dans la pierre, puis transformé en diamant parfait.' WHERE `ID` = 45753 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Chant des zéphirs', `LogDescription` = 'Apportez le Chant des zéphirs au Grand maréchal Tombeneige en Silithus.', `QuestDescription` = 'Une essence tourbillonnante se fond avec les pieds du duc.' WHERE `ID` = 45758 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Cachet magmatique de Skaldrenox', `LogDescription` = 'Apportez le cachet magmatique de Skaldrenox au Grand maréchal Tombeneige en Silithus.', `QuestDescription` = 'Les cachets du Haut conseil abyssal sont composés d’essence distillée de l’élément de leur porteur. Celui-ci a été forgé dans les flammes.' WHERE `ID` = 45759 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Cachet houleux de Skwol', `LogDescription` = 'Apportez le cachet houleux de Skwol au Grand maréchal Tombeneige en Silithus.', `QuestDescription` = 'Les cachets du Haut conseil abyssal sont composés d’essence distillée de l’élément de leur porteur. Celui-ci a été taillé dans le corail et lissé par les marées.' WHERE `ID` = 45762 AND `locale` = 'frFR';

UPDATE `gameobject` SET `PhaseId` = '9491' WHERE `guid` BETWEEN 25662612 AND 25662619;

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 6543 AND `SourceEntry` = 0 AND `ConditionTypeOrReference` = 2 AND `ConditionValue1` = 143867;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 6543, 0, 0, 0, 2, 0, 143867, 1, 0, 0, 0, '', 'Greater Wind Stone - the medallion as well as the ring');

UPDATE `creature_template` SET `ScriptName` = 'npc_wind_stone_templar' WHERE `entry` IN (15209, 15211, 15212, 15307);
DELETE FROM `creature_loot_template` WHERE `entry` IN (15209, 15211, 15212, 15307) AND `item` = 143865;

UPDATE `creature_loot_template` SET `QuestRequired` = 0, `Chance` = 12.5 WHERE `entry` IN (117662, 117664, 117665, 117666, 117667, 117672) AND `item` = 143865;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 1 AND `SourceEntry` BETWEEN 143872 AND 143887 AND `ConditionTypeOrReference` = 6;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(1, 11880, 143880, 0, 0, 6, 0, 469, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Alliance'),
(1, 11880, 143883, 0, 0, 6, 0, 469, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Alliance'),
(1, 11880, 143885, 0, 0, 6, 0, 469, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Alliance'),
(1, 11882, 143880, 0, 0, 6, 0, 469, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Alliance'),
(1, 11882, 143887, 0, 0, 6, 0, 469, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Alliance'),
(1, 11883, 143883, 0, 0, 6, 0, 469, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Alliance'),
(1, 117662, 143872, 0, 0, 6, 0, 67, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Horde'),
(1, 117662, 143880, 0, 0, 6, 0, 469, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Alliance'),
(1, 117663, 143873, 0, 0, 6, 0, 67, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Horde'),
(1, 117663, 143881, 0, 0, 6, 0, 469, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Alliance'),
(1, 117664, 143874, 0, 0, 6, 0, 67, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Horde'),
(1, 117664, 143882, 0, 0, 6, 0, 469, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Alliance'),
(1, 117665, 143875, 0, 0, 6, 0, 67, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Horde'),
(1, 117665, 143883, 0, 0, 6, 0, 469, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Alliance'),
(1, 117666, 143876, 0, 0, 6, 0, 67, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Horde'),
(1, 117666, 143884, 0, 0, 6, 0, 469, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Alliance'),
(1, 117667, 143877, 0, 0, 6, 0, 67, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Horde'),
(1, 117667, 143885, 0, 0, 6, 0, 469, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Alliance'),
(1, 117670, 143878, 0, 0, 6, 0, 67, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Horde'),
(1, 117670, 143886, 0, 0, 6, 0, 469, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Alliance'),
(1, 117672, 143879, 0, 0, 6, 0, 67, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Horde'),
(1, 117672, 143887, 0, 0, 6, 0, 469, 0, 0, 0, 0, '', 'Call of the Scarab - the quest item of the Alliance');

DELETE FROM `creature_questender` WHERE `quest` = 45787 AND `id` = 117433;
UPDATE `creature` SET `spawntimesecs` = 600 WHERE `guid` IN (146728592, 146728593, 146728594);

UPDATE `creature_template` SET `lootid` = `entry` WHERE `entry` IN (117489, 117490, 117491);
DELETE FROM `creature_loot_template` WHERE `Entry` IN (117489, 117490, 117491);
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Currency`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Shared`, `Comment`) VALUES
(117489, 1973, 0, 0, 4.19, 0, 1, 1, 1, 1, 0, ''),
(117489, 11992, 0, 0, 1.20, 0, 1, 1, 1, 1, 0, ''),
(117489, 12058, 0, 0, 4.19, 0, 1, 1, 1, 1, 0, ''),
(117489, 13013, 0, 0, 4.19, 0, 1, 1, 1, 1, 0, ''),
(117489, 13116, 0, 0, 4.19, 0, 1, 1, 1, 1, 0, ''),
(117489, 13133, 0, 0, 4.19, 0, 1, 1, 1, 1, 0, ''),
(117489, 14329, 0, 0, 1.20, 0, 1, 1, 1, 1, 0, ''),
(117489, 14332, 0, 0, 4.19, 0, 1, 1, 1, 1, 0, ''),
(117489, 14333, 0, 0, 4.19, 0, 1, 1, 1, 1, 0, ''),
(117489, 14334, 0, 0, 4.19, 0, 1, 1, 1, 1, 0, ''),
(117489, 14680, 0, 0, 4.19, 0, 1, 1, 1, 1, 0, ''),
(117489, 14686, 0, 0, 8.38, 0, 1, 1, 1, 1, 0, ''),
(117489, 14862, 0, 0, 4.19, 0, 1, 1, 1, 1, 0, ''),
(117489, 15195, 0, 0, 4.19, 0, 1, 1, 1, 1, 0, ''),
(117489, 15221, 0, 0, 1.20, 0, 1, 1, 1, 1, 0, ''),
(117489, 15240, 0, 0, 4.19, 0, 1, 1, 1, 1, 0, ''),
(117489, 15258, 0, 0, 4.19, 0, 1, 1, 1, 1, 0, ''),
(117489, 15283, 0, 0, 6.59, 0, 1, 1, 1, 1, 0, ''),
(117489, 15289, 0, 0, 4.19, 0, 1, 1, 1, 1, 0, ''),
(117489, 15438, 0, 0, 4.19, 0, 1, 1, 1, 1, 0, ''),
(117489, 15675, 0, 0, 5.99, 0, 1, 1, 1, 1, 0, ''),
(117489, 15682, 0, 0, 4.19, 0, 1, 1, 1, 1, 0, ''),
(117489, 15686, 0, 0, 8.38, 0, 1, 1, 1, 1, 0, ''),
(117490, 2564, 0, 0, 18.11, 0, 1, 1, 1, 1, 0, ''),
(117490, 12017, 0, 0, 5.76, 0, 1, 1, 1, 1, 0, ''),
(117490, 12058, 0, 0, 2.88, 0, 1, 1, 1, 1, 0, ''),
(117490, 13036, 0, 0, 2.88, 0, 1, 1, 1, 1, 0, ''),
(117490, 13125, 0, 0, 2.88, 0, 1, 1, 1, 1, 0, ''),
(117490, 14322, 0, 0, 2.88, 0, 1, 1, 1, 1, 0, ''),
(117490, 14332, 0, 0, 18.11, 0, 1, 1, 1, 1, 0, ''),
(117490, 14335, 0, 0, 2.88, 0, 1, 1, 1, 1, 0, ''),
(117490, 14680, 0, 0, 9.05, 0, 1, 1, 1, 1, 0, ''),
(117490, 14816, 0, 0, 2.88, 0, 1, 1, 1, 1, 0, ''),
(117490, 14817, 0, 0, 2.88, 0, 1, 1, 1, 1, 0, ''),
(117490, 14978, 0, 0, 2.88, 0, 1, 1, 1, 1, 0, ''),
(117490, 15247, 0, 0, 2.88, 0, 1, 1, 1, 1, 0, ''),
(117490, 15289, 0, 0, 5.76, 0, 1, 1, 1, 1, 0, ''),
(117490, 15435, 0, 0, 2.88, 0, 1, 1, 1, 1, 0, ''),
(117490, 15438, 0, 0, 1.65, 0, 1, 1, 1, 1, 0, ''),
(117490, 15439, 0, 0, 7.00, 0, 1, 1, 1, 1, 0, ''),
(117490, 15678, 0, 0, 2.88, 0, 1, 1, 1, 1, 0, ''),
(117490, 15684, 0, 0, 2.88, 0, 1, 1, 1, 1, 0, ''),
(117491, 13040, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 13120, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 13133, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 14329, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 14333, 0, 0, 6.97, 0, 1, 1, 1, 1, 0, ''),
(117491, 14334, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 14460, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 14680, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 14685, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 14687, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 14811, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 14814, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 14816, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 14817, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 14931, 0, 0, 6.97, 0, 1, 1, 1, 1, 0, ''),
(117491, 14948, 0, 0, 1.00, 0, 1, 1, 1, 1, 0, ''),
(117491, 14979, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 14981, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 15195, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 15247, 0, 0, 3.98, 0, 1, 1, 1, 1, 0, ''),
(117491, 15257, 0, 0, 0.50, 0, 1, 1, 1, 1, 0, ''),
(117491, 15258, 0, 0, 14.43, 0, 1, 1, 1, 1, 0, ''),
(117491, 15273, 0, 0, 0.50, 0, 1, 1, 1, 1, 0, ''),
(117491, 15283, 0, 0, 1.00, 0, 1, 1, 1, 1, 0, ''),
(117491, 15440, 0, 0, 1.00, 0, 1, 1, 1, 1, 0, ''),
(117491, 15667, 0, 0, 1.00, 0, 1, 1, 1, 1, 0, ''),
(117491, 15680, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 15687, 0, 0, 3.48, 0, 1, 1, 1, 1, 0, ''),
(117491, 56204, 0, 0, 10.88, 0, 1, 0, 1, 1, 0, '');

UPDATE `creature_template` SET `HealthScalingExpansion` = 6 WHERE `entry` IN (6491, 11698, 11721, 11722, 11723, 11724, 11725, 11726, 11727, 11728, 11729, 11730, 11731, 11732, 11733, 11734, 11735, 11736, 11737, 11738, 11739, 11740, 11741, 11744, 11745, 11746, 11747, 11803, 11804, 11805, 11880, 11881, 11882, 11883, 12178, 12179, 12199, 12956, 13220, 14455, 14471, 14472, 14473, 14474, 14475, 14476, 14477, 14478, 14479, 14536, 15169, 15170, 15171, 15174, 15175, 15176, 15179, 15180, 15183, 15189, 15190, 15191, 15194, 15196, 15200, 15201, 15202, 15209, 15211, 15212, 15213, 15215, 15270, 15282, 15293, 15306, 15307, 15308, 15419, 15440, 15441, 15442, 15443, 15444, 15498, 15499, 15500, 15540, 15541, 15542, 15545, 15570, 15599, 15609, 15610, 15611, 15612, 15613, 15615, 15616, 15617, 15722, 15760, 15801, 15903, 16091, 16543, 17068, 17070, 17079, 17080, 17081, 17082, 17765, 17766, 25962, 25994, 26215, 50587, 50588, 50737, 117662, 117663, 117664, 117665, 117666, 117667, 117670, 117672);
UPDATE `creature_template` SET `minlevel` = 110, `maxlevel` = 110 WHERE `entry` IN (15209, 15211, 15212, 15307, 117662, 117663, 117664, 117665, 117666, 117667, 117670, 117672);

DELETE FROM `gameobject` WHERE `guid` = 25926055;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(25926055, 180718, 1, 1377, 0, 1, 1, '9491', -8069.05, 1641.72, 27.03, 4.7473, 0, 0, 0.694657, -0.719341, 120, 255, 1);

DELETE FROM `game_event_worldquest` WHERE `eventEntry` = 78;
INSERT INTO `game_event_worldquest` (`eventEntry`, `quest`, `VariableID`) VALUES
(78, 45651, 12955), (78, 45654, 12956), (78, 45655, 12957), (78, 45656, 12958), (78, 45657, 12958), (78, 45674, 12976), (78, 45675, 12977),
(78, 45736, 12955), (78, 45737, 12956), (78, 45738, 12957), (78, 45739, 12958), (78, 45740, 12958), (78, 45741, 12976), (78, 45742, 12977);
UPDATE `world_quest_template` SET `CurrencyID_A` = 1325, `CurrencyID_H` = 1324, `CurrencyMin` = 150, `CurrencyMax` = 150 WHERE `ZoneID` = 1377 AND `QuestInfoID` IN (109, 111, 137);
UPDATE `world_quest_template` SET `CurrencyID_A` = 1325, `CurrencyID_H` = 1324, `CurrencyMin` = 100, `CurrencyMax` = 100 WHERE `ZoneID` = 1377 AND `QuestInfoID` = 113;
UPDATE `world_quest_update` SET `Timer` = 259200 WHERE `EventID` = 78 AND `QuestID` IN (45651, 45654, 45655, 45656, 45657, 45674, 45675, 45736, 45737, 45738, 45739, 45740, 45741, 45742);
