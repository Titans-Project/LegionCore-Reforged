-- Stormwind and Orgrimmar guards: drop the Mists of Pandaria lines gone in 7.3.5, the "NEW: Battle Pet Trainer" double and Reforge
DELETE FROM `gossip_menu_option` WHERE `MenuID` IN (435,1951,15768) AND ((`OptionID` = 1 AND `OptionText` = '|cFF0008E8NEW|r: Battle Pet Trainer') OR (`OptionID` = 2 AND `OptionText` = 'Reforge'));
DELETE FROM `gossip_menu_option_locale` WHERE `MenuID` IN (435,1951,15768) AND `OptionID` IN (1,2);
