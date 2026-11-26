-- Fhwoor Smash!: asks for honored with Sporeggar, not exalted (TDB 7.3.5)
UPDATE `quest_template_addon` SET `RequiredMinRepValue` = 9000 WHERE `ID` = 9729 AND `RequiredMinRepFaction` = 970;
