-- Vault of the Wardens: display of the fel prison model, stood up as the seal of the door of the Vault of the Betrayer
DELETE FROM `gameobject_display_info` WHERE `ID`=47595;
INSERT INTO `gameobject_display_info` (`ID`,`FileDataID`,`GeoBoxMinX`,`GeoBoxMinY`,`GeoBoxMinZ`,`GeoBoxMaxX`,`GeoBoxMaxY`,`GeoBoxMaxZ`,`OverrideLootEffectScale`,`OverrideNameScale`,`ObjectEffectPackageID`,`VerifiedBuild`) VALUES
(47595,1122543,-5,-5,-5,5,5,5,0,0,0,26365);
DELETE FROM `hotfix_data` WHERE `TableHash`=1829768651 AND `RecordID`=47595;
INSERT INTO `hotfix_data` (`Id`,`TableHash`,`RecordID`,`Timestamp`,`Deleted`) VALUES (9100194,1829768651,47595,0,0);

-- Vault of the Wardens: the frost breath of the Freeze Statues (spells 195227 and 198229) rises towards the vault
DELETE FROM `spell_visual_kit_model_attach` WHERE `ID`=312103;
INSERT INTO `spell_visual_kit_model_attach` (`Offset1`,`Offset2`,`Offset3`,`OffsetVariation1`,`OffsetVariation2`,`OffsetVariation3`,`ID`,`SpellVisualEffectNameID`,`AttachmentID`,`Flags`,`PositionerID`,`Yaw`,`Pitch`,`Roll`,`YawVariation`,`PitchVariation`,`RollVariation`,`Scale`,`ScaleVariation`,`StartAnimID`,`AnimID`,`EndAnimID`,`AnimKitID`,`LowDefModelAttachID`,`StartDelay`,`SpellVisualKitID`,`VerifiedBuild`) VALUES
(0,0,0,0,0,0,312103,1344,17,0,0,0,1.5708,0,0,0,0,2,0,65535,65535,65535,0,0,0,60422,26365);
DELETE FROM `hotfix_data` WHERE `Id`=9100196 OR (`TableHash`=4033975491 AND `RecordID`=312103);
INSERT INTO `hotfix_data` (`Id`,`TableHash`,`RecordID`,`Timestamp`,`Deleted`) VALUES (9100196,4033975491,312103,0,0);
