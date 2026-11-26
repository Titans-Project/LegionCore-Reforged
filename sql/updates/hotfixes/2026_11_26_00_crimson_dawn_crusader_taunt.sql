-- Scarlet Crusaders of How To Win Friends And Influence Enemies: the taunt rewritten since Legion gets its French text
DELETE FROM `broadcast_text_locale` WHERE `ID` = 29146 AND `locale` = 'frFR';
INSERT INTO `broadcast_text_locale` (`ID`,`locale`,`Text_lang`,`Text1_lang`,`VerifiedBuild`) VALUES
(29146,'frFR','Tu frappes comme une fillette. Franchement. C''est tout ce que tu sais faire ?','Tu frappes comme une fillette. Franchement. C''est tout ce que tu sais faire ?',26124);
