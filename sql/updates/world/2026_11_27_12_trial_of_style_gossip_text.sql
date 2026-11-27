-- Trial of Style: the transmogrifiers' entry option reads properly in English and French
UPDATE `gossip_menu_option` SET `OptionText` = 'I want to take part in the Trial of Style!' WHERE `MenuID` = 21043 AND `OptionID` = 2;
UPDATE `gossip_menu_option_locale` SET `OptionText` = 'Je veux participer à l’Épreuve du style !' WHERE `MenuID` = 21043 AND `OptionID` = 2 AND `Locale` = 'frFR';
