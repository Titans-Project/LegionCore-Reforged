-- Thousand Boat Bash: the festival world quests in French
UPDATE `quest_template_locale` SET `LogTitle` = 'Promenade en bateau' WHERE `ID` = 45806 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Pêche aux déchets' WHERE `ID` = 45807 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Bataille de ballons de plage' WHERE `ID` = 45808 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Grog, rhum et bière' WHERE `ID` = 45809 AND `locale` = 'frFR';
UPDATE `quest_template_locale` SET `LogTitle` = 'Les gobelins ont volé nos bombes !' WHERE `ID` = 45811 AND `locale` = 'frFR';
DELETE FROM `quest_objectives_locale` WHERE `ID` IN (288591,288600,288641,288642,288643,288644) AND `locale` = 'frFR';
INSERT INTO `quest_objectives_locale` (`ID`,`locale`,`QuestId`,`StorageIndex`,`Description`,`VerifiedBuild`) VALUES
(288591,'frFR',45807,0,'Déchets repêchés dans l''eau',1),
(288600,'frFR',45808,0,'Faire rebondir le ballon de plage 10 fois d''affilée',1),
(288641,'frFR',45806,0,'Faire votre rapport à Féplouf et Pozzik',1),
(288642,'frFR',45806,1,'Faire votre rapport à Tu-Luak sur la rive nord du lac',1),
(288643,'frFR',45806,2,'Faire votre rapport à Theldurin l''Égaré sur la rive est du lac',1),
(288644,'frFR',45806,3,'Faire votre rapport à Ilthine Solchant sur la rive sud du lac',1);
