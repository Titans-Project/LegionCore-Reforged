-- Don't Play With That In The House: the collector's gift comes back every Winter Veil, its achievement no longer names 2017
DELETE FROM `achievement` WHERE `ID` = 11913;
INSERT INTO `achievement` (`Title`,`Description`,`Reward`,`Flags`,`InstanceID`,`Supercedes`,`Category`,`UiOrder`,`SharesCriteria`,`Faction`,`Points`,`MinimumCriteria`,`ID`,`IconFileID`,`CriteriaTree`,`VerifiedBuild`) VALUES
('Don''t Play With That In The House','Proud owner of a collector''s gift.',NULL,2048,-1,0,15274,17,0,-1,0,0,11913,133202,59896,-1);
DELETE FROM `achievement_locale` WHERE `ID` = 11913 AND `locale` = 'frFR';
INSERT INTO `achievement_locale` (`ID`,`locale`,`Title_lang`,`Description_lang`,`Reward_lang`,`VerifiedBuild`) VALUES
(11913,'frFR','Ne convient pas à la vraie guerre','Être l’heureux propriétaire d’un cadeau de collection.',NULL,-1);
DELETE FROM `hotfix_data` WHERE `Id` = 9100232;
INSERT INTO `hotfix_data` (`Id`,`TableHash`,`RecordID`,`Timestamp`,`Deleted`) VALUES
(9100232, 3538824359, 11913, 0, 0);
