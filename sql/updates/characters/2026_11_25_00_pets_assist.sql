-- Pets saved on Passive because every new pet started there move to Assist
UPDATE `character_pet` SET `Reactstate` = 3 WHERE `Reactstate` = 0;
