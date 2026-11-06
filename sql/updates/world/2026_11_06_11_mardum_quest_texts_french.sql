-- Mardum: French quest texts still in English (Caza's reward, the captains' request, hidden objectives)
UPDATE `quest_offer_reward_locale` SET `OfferRewardText` = 'Excellent. Vous avez absorbé le pouvoir de ce gangreseigneur et êtes $gdevenu:devenue; plus $gpuissant:puissante; à votre tour.$B$BNous pouvons maintenant nous consacrer pleinement à la destruction de la Légion et de sa reine de la couvée.' WHERE `ID` = 39495 AND `Locale` = 'frFR';
DELETE FROM `quest_request_items_locale` WHERE `ID` = 38813 AND `Locale` = 'frFR';
INSERT INTO `quest_request_items_locale` (`ID`, `Locale`, `CompletionText`, `VerifiedBuild`) VALUES
(38813, 'frFR', 'Sommes-nous prêts à lancer l''assaut ?', 0);
UPDATE `quest_objectives_locale` SET `Description` = 'Belath Aubelame tué' WHERE `ID` = 280164 AND `locale` = 'frFR';
UPDATE `quest_objectives_locale` SET `Description` = 'Déclencheurs du pont' WHERE `ID` = 280101 AND `locale` = 'frFR';
UPDATE `quest_objectives_locale` SET `Description` = 'Communicateur de la Légion 1 - contrôleur' WHERE `ID` = 281333 AND `locale` = 'frFR';
UPDATE `quest_objectives_locale` SET `Description` = 'Communicateur de la Légion 2 - contrôleur' WHERE `ID` = 281334 AND `locale` = 'frFR';
UPDATE `quest_objectives_locale` SET `Description` = 'Communicateur de la Légion 3 - contrôleur' WHERE `ID` = 281335 AND `locale` = 'frFR';
