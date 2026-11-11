-- Vault of the Wardens: Our Last Hope and the new warglaives on their rack, then Breaking Out: Kayn and Altruis pick up their warglaives, talk before the ward, and wait in the next hall only once it is open, where the bonus objective of the Vault starts; Maiev's conversation of Illidan's chamber (527) and conversation 510 back to their own speakers
DELETE FROM `creature_queststarter` WHERE `quest` IN (38668, 38669);
DELETE FROM `creature_questender` WHERE `quest` IN (38668, 38669);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (92718, 38669);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES (92718, 38669);
UPDATE `quest_template_addon` SET `PrevQuestID` = 0 WHERE `ID` = 38669;
-- the first Our Last Hope (38668), no longer offered, still named 38669 as its next quest, which made it a prerequisite
UPDATE `quest_template_addon` SET `NextQuestID` = 0 WHERE `ID` = 38668;
UPDATE `quest_template_addon` SET `PrevQuestID` = 38669 WHERE `ID` = 38672;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 92718 AND `source_type` = 0 AND `id` = 0;
-- the rack of the Illidari Battle Blades gives the two Fel-Etched Glaives, with no player condition hiding it on the client (its phase shows it) and no personal loot lock
UPDATE `gameobject_template` SET `data1` = 241553, `Data17` = 0, `Data30` = 0, `flags` = `flags` | 0x4, `ScriptName` = 'go_vault_illidari_battle_blades' WHERE `entry` = 241553;
-- a hidden objective of Our Last Hope asks for the rack: outlined and usable only while the quest needs it
DELETE FROM `quest_objectives` WHERE `ID` = 940007;
INSERT INTO `quest_objectives` (`ID`, `QuestID`, `Type`, `StorageIndex`, `ObjectID`, `Amount`, `Flags`, `Flags2`, `TaskStep`, `Description`, `VerifiedBuild`, `Bugged`) VALUES
(940007, 38669, 2, 2, 241553, 1, 8, 0, 0, NULL, 0, 0);
DELETE FROM `locales_gameobject` WHERE `entry` = 241553;
INSERT INTO `locales_gameobject` (`entry`, `name_loc2`, `castbarcaption_loc2`) VALUES (241553, 'Lames de combat illidari', 'Extraction');
-- and stays in sight from the arrival until Our Last Hope is turned in, not only while the quest is in the log
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 7814 AND `SourceEntry` = 101;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(23, 7814, 101, 0, 0, 8, 0, 38669, 0, 0, 1, 0, '', 'Illidari Battle Blades until Our Last Hope is turned in');
UPDATE `quest_template_locale` SET `LogTitle` = 'Notre dernier espoir', `LogDescription` = 'Récupérez vos nouveaux Glaives de guerre.',
  `QuestDescription` = 'Le caveau est tombé aux mains de la Légion ardente.

Cela me fait de la peine de le dire, mais tout semble perdu. Une lueur d''espoir demeure cependant… VOUS. Croyez-moi, je ne voulais vraiment pas en arriver là. Hélas, il semblerait que je n''aie pas d''autre choix.

Aidez-moi, $gchasseur:chasseuse; de démons, et je vous accorderai la liberté.'
  WHERE `ID` = 38669 AND `locale` = 'frFR';
UPDATE `quest_objectives_locale` SET `Description` = 'Glaive de main droite équipé' WHERE `ID` = 278272 AND `locale` = 'frFR';
UPDATE `quest_objectives_locale` SET `Description` = 'Glaive de main gauche équipé' WHERE `ID` = 280322 AND `locale` = 'frFR';
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_vault_freed_illidari' WHERE `entry` IN (99631, 99632);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (99631, 99632) AND `source_type` = 0;
DELETE FROM `creature_text` WHERE `CreatureID` = 99631 AND `GroupID` IN (1, 2);
DELETE FROM `creature_text` WHERE `CreatureID` = 99632 AND `GroupID` IN (2, 3);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextID`, `MinTimer`, `MaxTimer`, `SpellID`, `comment`) VALUES
(99631, 1, 0, 'Why would Maiev free us? There could only be one reason...', 12, 0, 100, 1, 0, 57387, 101987, 0, 0, 0, 'Kayn Sunfury - before the ward'),
(99631, 2, 0, 'The demons aim to destroy our world. We need to release our allies.', 12, 0, 100, 1, 0, 57340, 98738, 0, 0, 0, 'Kayn Sunfury - the ward open'),
(99632, 2, 0, 'Fool! You would trust the Warden? She hates us almost as much as the Legion.', 12, 0, 100, 1, 0, 57389, 101995, 0, 0, 0, 'Altruis the Sufferer - before the ward'),
(99632, 3, 0, 'Time for answers later. There are demons to kill.', 12, 0, 100, 1, 0, 57339, 98737, 0, 0, 0, 'Altruis the Sufferer - the ward open');
UPDATE `creature` SET `PhaseId` = '5976' WHERE `guid` IN (366382, 366398);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 92980 AND `source_type` = 0 AND `id` = 1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `Difficulties`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(92980, 0, 1, 0, '', 10, 0, 100, 0, 1, 25, 1000, 1000, 0, 7, 39742, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Kayn Sunfury - player in the hall - bonus objective Vault of the Wardens');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 2 AND `SourceEntry` = 92980 AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 2, 92980, 0, 0, 8, 0, 38672, 0, 0, 0, 0, '', 'Vault bonus objective once Breaking Out is turned in'),
(22, 2, 92980, 0, 0, 14, 0, 39742, 0, 0, 0, 0, '', 'Vault bonus objective not taken yet');
DELETE FROM `conversation_actor` WHERE `entry` IN (510, 527);
DELETE FROM `conversation_creature` WHERE `entry` IN (510, 527);
INSERT INTO `conversation_creature` (`entry`, `id`, `creatureId`, `creatureGuid`, `unk1`, `unk2`, `duration`) VALUES
(510, 0, 97421, 7946207, 0, 0, 2000),
(527, 0, 92718, 9813038, 0, 0, 2200),
(527, 1, 96665, 1424430, 0, 0, 2200);
DELETE FROM `conversation_data` WHERE `entry` IN (510, 527);
INSERT INTO `conversation_data` (`entry`, `id`, `idx`, `textId`, `unk1`, `unk2`) VALUES
(510, 1353, 0, 0, 4287001008, 0),
(510, 1354, 1, 3250, 4287001008, 3149594624),
(527, 1375, 0, 0, 3216623120, 0),
(527, 1376, 1, 3800, 3216623120, 1);
-- the warglaives turned a quarter round on their racks and set on them, held by the two hooks (a yaw-only rotation is taken from the orientation)
UPDATE `gameobject` SET `position_x` = 4317.79, `position_y` = -587.07, `orientation` = 1.3576, `rotation0` = 0, `rotation1` = 0, `rotation2` = 0, `rotation3` = 1 WHERE `guid` = 131118;
UPDATE `gameobject` SET `position_x` = 4334.39, `position_y` = -587.22, `orientation` = 2.1688, `rotation0` = 0, `rotation1` = 0, `rotation2` = 0, `rotation3` = 1 WHERE `guid` = 131119;
UPDATE `gameobject` SET `position_x` = 4330.88, `position_y` = -605.52, `orientation` = 1.6028, `rotation0` = 0, `rotation1` = 0, `rotation2` = 0, `rotation3` = 1 WHERE `guid` = 131121;

-- Kayn and Altruis wait in the next hall in combat stance
DELETE FROM `creature_addon` WHERE `guid` IN (366382,366398);
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(366382,0,0,0,0,333,''),
(366398,0,0,0,0,333,'');

-- Wrathlord Lekos hits ten times harder with his weapon; his abilities keep their own fixed damage
UPDATE `creature_template` SET `dmg_multiplier`=10 WHERE `entry`=97069;

-- Maiev finishes her line before Kayn answers
UPDATE `conversation_data` SET `textId`=7000 WHERE `entry`=527 AND `id`=1376;
UPDATE `conversation_creature` SET `duration`=7000 WHERE `entry`=527 AND `id`=0;
UPDATE `conversation_creature` SET `duration`=5000 WHERE `entry`=527 AND `id`=1;

-- Vault of the Betrayer: the door stays shut until Stop Gul'dan! is taken, then stands open; one door per stage
DELETE FROM `phase_definitions` WHERE `zoneId`=7814 AND `entry`=102;
INSERT INTO `phase_definitions` (`zoneId`,`entry`,`phasemask`,`phaseId`,`PreloadMapID`,`VisibleMapID`,`UiWorldMapAreaID`,`flags`,`comment`) VALUES
(7814,102,0,'4991',0,0,0,16,'Vault of the Betrayer door shut until 38723 is taken');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND `SourceGroup`=7814 AND `SourceEntry`=102;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,7814,102,0,0,9,0,38723,0,0,1,0,'','Door shut while 38723 not taken'),
(23,7814,102,0,0,28,0,38723,0,0,1,0,'','Door shut while 38723 not completed'),
(23,7814,102,0,0,8,0,38723,0,0,1,0,'','Door shut while 38723 not rewarded');
UPDATE `gameobject` SET `PhaseId`='4991', `state`=1 WHERE `guid`=131059;
UPDATE `gameobject` SET `PhaseId`='5128 5498', `state`=0 WHERE `guid`=130990;
DELETE FROM `gameobject` WHERE `guid`=130995;
DELETE FROM `locales_gameobject` WHERE `entry` IN (245467,253400);
INSERT INTO `locales_gameobject` (`entry`,`name_loc2`) VALUES (245467,'Caveau du Traître'),(253400,'Caveau du Traître');

-- Stop Gul'dan!: the scene is cast when the quest is taken; its area rule could never be met and only blocked the cast
DELETE FROM `spell_area` WHERE `spell`=187864 AND `area`=7814;

-- Altruis says "The Legion is here" with the arrival scene near Maiev, once per character, no longer after every load
DELETE FROM `smart_scripts` WHERE `entryorguid`=92986 AND `source_type`=0 AND `id`=1;

-- Vault of the Betrayer: the sealed door stays through the scene of Stop Gul'dan! and opens once it is over
DELETE FROM `phase_definitions` WHERE `zoneId`=7814 AND `entry`=103;
INSERT INTO `phase_definitions` (`zoneId`,`entry`,`phasemask`,`phaseId`,`PreloadMapID`,`VisibleMapID`,`UiWorldMapAreaID`,`flags`,`comment`) VALUES
(7814,103,0,'4990',0,0,0,16,'Vault of the Betrayer door open once the scene of 38723 is over');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND `SourceGroup`=7814 AND `SourceEntry`=102 AND `ElseGroup`=1;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND `SourceGroup`=7814 AND `SourceEntry`=103;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,7814,102,0,1,9,0,38723,0,0,0,0,'','Door shut while 38723 taken...'),
(23,7814,102,0,1,41,0,38723,99303,0,1,0,'','...and the scene not over'),
(23,7814,103,0,0,9,0,38723,0,0,0,0,'','Door open while 38723 taken...'),
(23,7814,103,0,0,41,0,38723,99303,0,0,0,'','...and the scene over'),
(23,7814,103,0,1,28,0,38723,0,0,0,0,'','Door open once 38723 completed'),
(23,7814,103,0,2,8,0,38723,0,0,0,0,'','Door open once 38723 rewarded');
UPDATE `gameobject` SET `PhaseId`='4990' WHERE `guid`=130990;

-- Kayn and Altruis wait at the door of the Vault of the Betrayer from the end of Stop Gul'dan! until the next quest is taken
DELETE FROM `phase_definitions` WHERE `zoneId`=7814 AND `entry`=104;
INSERT INTO `phase_definitions` (`zoneId`,`entry`,`phasemask`,`phaseId`,`PreloadMapID`,`VisibleMapID`,`UiWorldMapAreaID`,`flags`,`comment`) VALUES
(7814,104,0,'5140',0,0,0,16,'Kayn and Altruis at the door between 38723 done and 39682 taken');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND `SourceGroup`=7814 AND `SourceEntry`=104;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,7814,104,0,0,28,0,38723,0,0,0,0,'','38723 completed...'),
(23,7814,104,0,0,9,0,39682,0,0,1,0,'','...39682 not taken'),
(23,7814,104,0,0,28,0,39682,0,0,1,0,'','...39682 not completed'),
(23,7814,104,0,0,8,0,39682,0,0,1,0,'','...39682 not rewarded'),
(23,7814,104,0,1,8,0,38723,0,0,0,0,'','38723 rewarded...'),
(23,7814,104,0,1,9,0,39682,0,0,1,0,'','...39682 not taken'),
(23,7814,104,0,1,28,0,39682,0,0,1,0,'','...39682 not completed'),
(23,7814,104,0,1,8,0,39682,0,0,1,0,'','...39682 not rewarded');
UPDATE `creature` SET `PhaseId`='5140' WHERE `guid` IN (366541,366542);

-- Maiev, Kayn and Altruis of Illidan's chamber go once Stop Gul'dan! is taken
DELETE FROM `phase_definitions` WHERE `zoneId`=7814 AND `entry`=105;
INSERT INTO `phase_definitions` (`zoneId`,`entry`,`phasemask`,`phaseId`,`PreloadMapID`,`VisibleMapID`,`UiWorldMapAreaID`,`flags`,`comment`) VALUES
(7814,105,0,'5141',0,0,0,16,'Maiev, Kayn and Altruis of Illidan''s chamber until 38723 is taken');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND `SourceGroup`=7814 AND `SourceEntry`=105;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,7814,105,0,0,9,0,38723,0,0,1,0,'','38723 not taken'),
(23,7814,105,0,0,28,0,38723,0,0,1,0,'','38723 not completed'),
(23,7814,105,0,0,8,0,38723,0,0,1,0,'','38723 not rewarded');
UPDATE `creature` SET `PhaseId`='5141' WHERE `guid` IN (366526,366528,366529);

-- Vault of the Betrayer: a fel prison seals the door until the scene of Stop Gul'dan! is over
DELETE FROM `creature_addon` WHERE `guid`=366443;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (366443,0,0,0,0,0,'185153');
UPDATE `creature` SET `PhaseId`='4991' WHERE `guid`=366443;

-- the bunny of the seal hangs where Maiev's beam aims, 9 m above the floor
UPDATE `creature_addon` SET `bytes1`=50331648 WHERE `guid`=366443;

-- Vault of the Betrayer: the fel seal stands upright on the door until the scene of Stop Gul'dan! is over; the bunny no longer carries it
UPDATE `creature_addon` SET `auras`='' WHERE `guid`=366443;
DELETE FROM `gameobject_template` WHERE `entry`=2601302;
INSERT INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`size`) VALUES (2601302,5,47595,'Fel Seal',5);
DELETE FROM `gameobject` WHERE `guid`=25926053;
INSERT INTO `gameobject` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`PhaseId`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(25926053,2601302,1468,7814,7871,1,1,'4991',4219.97,-297.86,-264.86,0,0,0.7071068,0,0.7071068,0,255,0);
UPDATE `version` SET `hotfix_cache_id`=`hotfix_cache_id`+1;

-- the Generic Bunny in front of the door showed as a grey demon in fel flames: the seal is carried by its own object
DELETE FROM `creature_addon` WHERE `guid`=366443;
DELETE FROM `creature` WHERE `guid`=366443;
UPDATE `version` SET `hotfix_cache_id`=`hotfix_cache_id`+1;

-- Crusher carries the sword of his equipment, as Sledge already does
UPDATE `creature` SET `equipment_id`=1 WHERE `guid`=366536;
UPDATE `creature` SET `equipment_id`=0 WHERE `guid`=366539;

-- the scene of Stop Gul'dan! fades to black at its end and waits for the server: it lasts 21 s, the demon hunter is moved
-- inside 1.5 s before and the server closes it
UPDATE `spell_scene` SET `CustomDuration`=21000 WHERE `MiscValue`=1016;

-- Demon Link reaches only Crusher, Sledge and their kind, never a player
DELETE FROM `spell_script_names` WHERE `spell_id`=199375;
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES (199375,'spell_vault_demon_link');

-- the door and its seal stay shut while the scene of Stop Gul'dan! plays (its aura), and open once it is over
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND `SourceGroup`=7814 AND ((`SourceEntry`=102 AND `ElseGroup`=1) OR `SourceEntry`=103);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,7814,102,0,1,1,0,187864,0,0,0,0,'','Door shut while the scene of 38723 plays'),
(23,7814,103,0,0,9,0,38723,0,0,0,0,'','Door open while 38723 taken...'),
(23,7814,103,0,0,1,0,187864,0,0,1,0,'','...and the scene not playing'),
(23,7814,103,0,1,28,0,38723,0,0,0,0,'','Door open once 38723 completed'),
(23,7814,103,0,2,8,0,38723,0,0,0,0,'','Door open once 38723 rewarded');

-- Kayn and Altruis fight Crusher and Sledge once the scene of Stop Gul'dan! is over, until the quest is done
DELETE FROM `phase_definitions` WHERE `zoneId`=7814 AND `entry`=106;
INSERT INTO `phase_definitions` (`zoneId`,`entry`,`phasemask`,`phaseId`,`PreloadMapID`,`VisibleMapID`,`UiWorldMapAreaID`,`flags`,`comment`) VALUES
(7814,106,0,'5015',0,0,0,16,'Kayn and Altruis fighting Crusher and Sledge after the scene of 38723');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND `SourceGroup`=7814 AND `SourceEntry`=106;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,7814,106,0,0,9,0,38723,0,0,0,0,'','38723 taken...'),
(23,7814,106,0,0,1,0,187864,0,0,1,0,'','...and the scene over');
UPDATE `creature` SET `PhaseId`='5015' WHERE `guid` IN (366537,366538);

-- Crusher and Sledge share the phase of Kayn and Altruis, so that they can fight them
UPDATE `creature` SET `PhaseId`='5976 5407 5401 5158 5128 5015' WHERE `guid` IN (366536,366539);

-- Crusher and Sledge only stand in the Vault of the Betrayer once the scene of Stop Gul'dan! is over, with Kayn and Altruis
UPDATE `creature` SET `PhaseId`='5015' WHERE `guid` IN (366536,366539);

-- the Generic Bunny of the Vault of the Betrayer keeps its invisible model, never the imp one
UPDATE `creature` SET `modelid`=11686 WHERE `guid`=366540;

-- Kayn and Altruis of the Vault of the Betrayer fight Crusher and Sledge
UPDATE `creature_template` SET `ScriptName`='npc_vault_betrayer_ally' WHERE `entry` IN (92984,92985);

-- Crusher and Sledge take the faction of the Vault's demons and Kayn and Altruis that of the freed Illidari: they fight each other
UPDATE `creature_template` SET `faction`=954 WHERE `entry` IN (92990,97632);
UPDATE `creature_template` SET `faction`=2804 WHERE `entry` IN (92984,92985);

-- the Generic Bunnies keep only their invisible model, as on retail (the imp model has a probability of 0); the scene of
-- Stop Gul'dan! draws its door bunny with it
UPDATE `creature_template_wdb` SET `Displayid1`=15880, `Displayid2`=0 WHERE `Entry`=59115;
UPDATE `creature_template_wdb` SET `Displayid1`=11686, `Displayid2`=0 WHERE `Entry`=59113;
UPDATE `version` SET `hotfix_cache_id`=`hotfix_cache_id`+1;

-- Sledge wields the greatsword the scene gives him (128520); Crusher fights bare-handed, as in the scene
UPDATE `creature_equip_template` SET `ItemID1`=128520 WHERE `CreatureID`=92990 AND `ID`=1;
UPDATE `creature` SET `equipment_id`=1 WHERE `guid`=366539;
UPDATE `creature` SET `equipment_id`=0 WHERE `guid`=366536;

-- Crusher wields the same greatsword as Sledge
UPDATE `creature_equip_template` SET `ItemID1`=128520 WHERE `CreatureID`=97632 AND `ID`=1;
UPDATE `creature` SET `equipment_id`=1 WHERE `guid`=366536;

-- Face Kick throws the demon hunter a short way only; Crusher and Sledge hold out about half a minute from a third of their health
DELETE FROM `spell_script_names` WHERE `spell_id`=199645;
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES (199645,'spell_vault_face_kick');
UPDATE `creature_template_wdb` SET `HpMulti`=42 WHERE `Entry` IN (92990,97632);

-- Kayn and Altruis bicker during the fight at a pace the French lines can be read at, each exchange after the previous one
UPDATE `conversation_data` SET `textId`=3500 WHERE `entry`=528 AND `id`=1378;
UPDATE `conversation_data` SET `textId`=9500 WHERE `entry`=529 AND `id`=1380;
UPDATE `conversation_data` SET `textId`=6000 WHERE `entry`=530 AND `id`=1382;
UPDATE `conversation_data` SET `textId`=12000 WHERE `entry`=530 AND `id`=1819;
UPDATE `conversation_creature` SET `duration`=2500 WHERE `entry`=528;
UPDATE `conversation_creature` SET `duration`=5500 WHERE `entry`=529;
UPDATE `conversation_creature` SET `duration`=8000 WHERE `entry`=530;

-- Kethrazor casts Mind Spike and summons his inquisitor eyes: the explosive one fixes on the demon hunter and bursts, the seeping one pulses shadow
UPDATE `creature_template` SET `ScriptName`='npc_vault_kethrazor' WHERE `entry`=96997;
UPDATE `creature_template` SET `ScriptName`='npc_vault_explosive_eye' WHERE `entry`=101901;
UPDATE `creature_template` SET `ScriptName`='npc_vault_seeping_eye' WHERE `entry`=101907;

-- Explosive Shadows and Mind Spike of Kethrazor hit 30 % softer
DELETE FROM `spell_script_names` WHERE `spell_id` IN (201025,201038);
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES (201025,'spell_vault_kethrazor_softened'),(201038,'spell_vault_kethrazor_softened');

-- the pulse of the Seeping Inquisitor Eye only strikes within 2 m of it
DELETE FROM `spell_script_names` WHERE `spell_id`=201023;
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES (201023,'spell_vault_seeping_pulse');

-- the Vampiric Felbat waiting for the demon hunter hangs 1 m higher
UPDATE `creature` SET `position_z`=-243.754 WHERE `guid`=366579;

-- Vault of the Wardens, the Wrathguard floor: one demon per freed Illidari and a few roaming ones, the portals send the rest
UPDATE `creature` SET `PhaseId`='5157 5158 5401 5407 5976' WHERE `guid`=366605;
UPDATE `creature` SET `PhaseId`='5157 5158 5401 5407 5976' WHERE `guid`=366621;
UPDATE `creature` SET `PhaseId`='5157 5158 5401 5407 5443 5976' WHERE `guid`=366634;
UPDATE `creature` SET `PhaseId`='5157 5158 5401 5407 5976' WHERE `guid`=366639;
UPDATE `creature` SET `PhaseId`='5157 5158 5401 5407 5443 5976' WHERE `guid`=366678;
UPDATE `creature` SET `PhaseId`='5157 5158 5401 5407 5443 5976' WHERE `guid`=366815;
UPDATE `creature` SET `PhaseId`='5147 5148 5157 5158 5401 5407 5439 5976' WHERE `guid`=366825;
UPDATE `creature` SET `PhaseId`='5157 5158 5401 5407 5976' WHERE `guid`=366635;
DELETE FROM `creature_addon` WHERE `guid` IN (366601,366602,366604,366612,366623,366628,366632,366636,366637,366641,366669,366670,366671,366673,366675,366676,366679,366681,366682,366683,366695,366698,366701,366710,366729,366731,366734,366735,366741,366746,366760,366761,366762,366763,366764,366765,366766,366768,366769,366770,366771,366772,366773,366774,366775,366777,366778,366779,366813,366816,366817,366818,366819,366820,366821,366822,366823,366824,366826,366877);
DELETE FROM `creature` WHERE `guid` IN (366601,366602,366604,366612,366623,366628,366632,366636,366637,366641,366669,366670,366671,366673,366675,366676,366679,366681,366682,366683,366695,366698,366701,366710,366729,366731,366734,366735,366741,366746,366760,366761,366762,366763,366764,366765,366766,366768,366769,366770,366771,366772,366773,366774,366775,366777,366778,366779,366813,366816,366817,366818,366819,366820,366821,366822,366823,366824,366826,366877);

-- the Wrathguard floor: the demons left alone far from any Illidari are taken away, nearly every demon there fights one
DELETE FROM `creature_addon` WHERE `guid` IN (366626,366615,366592);
DELETE FROM `creature` WHERE `guid` IN (366626,366615,366592);

-- Frozen in Time: Ash'golm rages until the three countermeasures are on, then stands frozen for the demon hunter who did it
DELETE FROM `phase_definitions` WHERE `zoneId`=7814 AND `entry` IN (107,108);
INSERT INTO `phase_definitions` (`zoneId`,`entry`,`phasemask`,`phaseId`,`PreloadMapID`,`VisibleMapID`,`UiWorldMapAreaID`,`flags`,`comment`) VALUES
(7814,107,0,'5014',0,0,0,16,'Ash''golm raging until 39685 is completed'),
(7814,108,0,'5585',0,0,0,16,'Ash''golm frozen once 39685 is completed');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND `SourceGroup`=7814 AND `SourceEntry` IN (107,108);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,7814,107,0,0,28,0,39685,0,0,1,0,'','39685 not completed...'),
(23,7814,107,0,0,8,0,39685,0,0,1,0,'','...nor rewarded'),
(23,7814,108,0,0,28,0,39685,0,0,0,0,'','39685 completed'),
(23,7814,108,0,1,8,0,39685,0,0,0,0,'','39685 rewarded');
UPDATE `creature` SET `PhaseId`='5014' WHERE `guid`=366900;
DELETE FROM `creature` WHERE `guid`=146932651;
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`PhaseId`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`MovementType`) VALUES
(146932651,101591,1468,7814,7867,1,1,'5585',4238.65,-452.602,105.947,0,300,0);

-- Frozen in Time: Ash'golm fights with Fissure and Lava Wreath; the Freeze Statues of its room (99710) answer the countermeasures
UPDATE `creature_template` SET `ScriptName`='npc_vault_ashgolm' WHERE `entry`=96681;
DELETE FROM `creature` WHERE `guid` BETWEEN 146932652 AND 146932654;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=1 AND `SourceEntry`=195213 AND `ConditionValue2`=99710;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,1,195213,0,1,31,0,3,99710,0,0,0,'','Activate System - Freeze Statue of the demon hunter''s Vault');

-- Ash'golm of Frozen in Time rages until the countermeasures are on: no frozen aura of its own (the dungeon's Ash'golm starts frozen)
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=96681;

-- Kayn holds Ash'golm with the freed Illidari: their faction and a fighting AI
UPDATE `creature_template` SET `faction`=2804, `ScriptName`='npc_vault_betrayer_ally' WHERE `entry`=102393;

-- Ash'golm shares the phase of the Kayn and the freed Illidari who hold it (5440, Frozen in Time taken), so that they can fight
UPDATE `creature` SET `PhaseId`='5014 5440' WHERE `guid`=366900;

-- Frozen in Time: every Freeze Statue shares the phases of the countermeasures; the frozen Ash'golm has Ash'golm's level;
-- Kayn and the freed Illidari fight it until the quest is done, then stand idle by the frozen one
UPDATE `creature` SET `PhaseId`='5976 5440 5439 5438 5437 5407 5401 5158 5157' WHERE `guid` IN (366713,366715);
UPDATE `creature_template` SET `minlevel`=112, `maxlevel`=112, `exp`=6 WHERE `entry`=101591;
UPDATE `creature` SET `PhaseId`='5014 5440' WHERE `guid` IN (366723,366724,366725);
DELETE FROM `creature` WHERE `guid` BETWEEN 146932655 AND 146932657;
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`PhaseId`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`MovementType`) VALUES
(146932655,96656,1468,7814,7867,1,1,'5585',4250.26,-443.564,106.001,6.23554,300,0),
(146932656,96656,1468,7814,7867,1,1,'5585',4254.13,-459.597,106.001,0.0702364,300,0),
(146932657,102393,1468,7814,7867,1,1,'5585',4255.02,-454.12,106.001,0.00758042,300,0);

-- Frozen in Time: once the three countermeasures are on, Ash'golm stands in the ice of the dungeon's Ash'golm
DELETE FROM `creature_addon` WHERE `guid`=146932651;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (146932651,0,0,0,1,0,'200354');
UPDATE `version` SET `hotfix_cache_id`=`hotfix_cache_id`+1;

-- Vault of the Wardens: every Sentry and Frost Sentry is scenery, named in French and no longer clickable
UPDATE `gameobject_template` SET `flags`=`flags`|16 WHERE `entry` IN (245514,245515,245516,245517,245518,245519,245520,245521,245522,245523,245575,245576,245577);
DELETE FROM `locales_gameobject` WHERE `entry` IN (245514,245515,245516,245517,245518,245519,245520,245521,245522,245523,245575,245576,245577);
INSERT INTO `locales_gameobject` (`entry`,`name_loc2`) VALUES
(245514,'Sentinelle'),
(245515,'Sentinelle'),
(245516,'Sentinelle'),
(245517,'Sentinelle'),
(245518,'Sentinelle'),
(245519,'Sentinelle'),
(245520,'Sentinelle'),
(245521,'Sentinelle'),
(245522,'Sentinelle'),
(245523,'Sentinelle'),
(245575,'Sentinelle de givre'),
(245576,'Sentinelle de givre'),
(245577,'Sentinelle de givre');

-- Vault of Mirrors: the last mirror beams at the wall until Glazer is stunned, then at Glazer (each beam in the phase where it is seen)
UPDATE `creature` SET `PhaseId`='5442' WHERE `guid`=367022;
DELETE FROM `creature` WHERE `guid`=146932658;
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`PhaseId`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`MovementType`) VALUES
(146932658,950008,1468,7814,7868,1,7,'5443',4442.64,-699.868,118.945,2.296,300,0);

-- Forged in Fire: the freed Illidari of the forge (96672) fights its jailer (96682) until the demon hunter comes
UPDATE `creature_template` SET `faction`=2804, `ScriptName`='npc_vault_betrayer_ally' WHERE `entry`=96672;

-- Upper hall of the Vault: every freed Illidari has a foe in front of it, and the demons come back within 30 seconds
DELETE FROM `creature` WHERE `guid` IN (146932659,146932660);
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`PhaseId`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`MovementType`) VALUES
(146932659,92782,1468,7814,7866,1,1,'5976 5407 5401 5158',4401.34,-475.55,119.963,5.41,30,0),
(146932660,92782,1468,7814,7866,1,1,'5976 5407 5401 5158',4379.10,-460.45,126.129,1.016,30,0);
UPDATE `creature` SET `spawntimesecs`=30 WHERE `map`=1468 AND `id` IN (92782,97225,97228) AND `position_x` BETWEEN 4370 AND 4510 AND `position_y` BETWEEN -530 AND -390 AND `position_z` BETWEEN 110 AND 170;

-- The elevator to the top of the Vault only runs for the demon hunter who took Up to the Top (39686)
DELETE FROM `phase_definitions` WHERE `zoneId`=7814 AND `entry`=109;
INSERT INTO `phase_definitions` (`zoneId`,`entry`,`phasemask`,`phaseId`,`PreloadMapID`,`VisibleMapID`,`UiWorldMapAreaID`,`flags`,`comment`) VALUES
(7814,109,0,'4900',0,0,0,16,'Elevator to the top once 39686 is taken');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND `SourceGroup`=7814 AND `SourceEntry`=109;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,7814,109,0,0,9,0,39686,0,0,0,0,'','39686 taken'),
(23,7814,109,0,1,28,0,39686,0,0,0,0,'','39686 completed'),
(23,7814,109,0,2,8,0,39686,0,0,0,0,'','39686 rewarded');
UPDATE `gameobject` SET `PhaseId`='4900' WHERE `guid`=131013;

-- Vault of the Wardens, Magolith's floor and the floor below: the wardens hold Magolith and the Fel Shocktroopers in front of them, a fight without wounds
UPDATE `creature_template` SET `ScriptName`='npc_vault_betrayer_ally' WHERE `entry` IN (96645,100665,100667,97090);
UPDATE `creature_template` SET `ScriptName`='npc_vault_scenery_demon' WHERE `entry`=97333;
UPDATE `creature` SET `unit_flags`=33088 WHERE `guid`=366666;
UPDATE `creature` SET `position_x`=4452.88, `position_y`=-469.54, `position_z`=185.875, `orientation`=4.142 WHERE `guid`=366644;
UPDATE `creature` SET `position_x`=4448.57, `position_y`=-473.39, `position_z`=185.875, `orientation`=0.458 WHERE `guid`=366649;
DELETE FROM `creature` WHERE `guid` BETWEEN 146932661 AND 146932700;
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`PhaseId`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`MovementType`,`unit_flags`) VALUES
(146932661,92776,1468,7814,7866,1,1,'5976 5407 5158',4450.66,-387.27,159.258,0.231,30,0,33024),
(146932662,92776,1468,7814,7866,1,1,'5976 5407 5158',4450.41,-387.86,192.536,1.121,30,0,33024),
(146932663,92776,1468,7814,7866,1,1,'5976 5407 5158',4453.38,-409.19,190.611,1.870,30,0,33024),
(146932664,92776,1468,7814,7866,1,1,'5976 5407 5158',4459.00,-430.31,185.822,5.083,30,0,33024),
(146932665,92776,1468,7814,7866,1,1,'5976 5407 5158',4449.93,-421.45,186.740,4.506,30,0,33024),
(146932666,92776,1468,7814,7866,1,1,'5976 5407 5158',4463.20,-438.21,152.545,1.670,30,0,33024),
(146932667,92776,1468,7814,7866,1,1,'5976 5407 5158',4441.16,-436.97,185.822,1.040,30,0,33024),
(146932668,92776,1468,7814,7866,1,1,'5976 5407 5158',4439.91,-435.10,185.822,0.140,30,0,33024),
(146932669,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4435.08,-442.30,185.822,1.809,30,0,33024),
(146932670,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4433.80,-442.28,185.822,1.309,30,0,33024),
(146932671,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4413.07,-451.76,154.388,0.199,30,0,33024),
(146932672,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4468.79,-442.88,185.822,3.124,30,0,33024),
(146932673,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4429.04,-459.73,185.822,5.573,30,0,33024),
(146932674,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4431.11,-458.83,185.822,4.673,30,0,33024),
(146932675,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4516.31,-451.39,159.256,3.170,30,0,33024),
(146932676,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4516.43,-451.86,159.289,3.625,30,0,33024),
(146932677,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4402.76,-426.66,187.301,3.576,30,0,33024),
(146932678,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4477.81,-453.57,186.786,0.590,30,0,33024),
(146932679,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4516.46,-450.82,159.268,2.611,30,0,33024),
(146932680,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4421.67,-453.31,186.740,1.072,30,0,33024),
(146932681,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4420.35,-451.47,186.740,0.172,30,0,33024),
(146932682,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4408.91,-455.08,188.994,0.819,30,0,33024),
(146932683,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4408.10,-452.97,188.994,6.202,30,0,33024),
(146932684,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4465.64,-457.09,185.822,4.539,30,0,33024),
(146932685,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4467.47,-458.42,185.822,3.639,30,0,33024),
(146932686,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4492.73,-449.09,188.861,3.810,30,0,33024),
(146932687,92776,1468,7814,7866,1,1,'5976 5407 5401 5158 5157',4391.73,-447.45,192.537,3.389,30,0,33024),
(146932688,92776,1468,7814,7866,1,1,'5976 5407 5401 5158 5157',4383.16,-449.10,192.721,3.470,30,0,33024),
(146932689,92776,1468,7814,7866,1,1,'5976 5407 5401 5158 5157',4372.50,-454.58,192.708,1.040,30,0,33024),
(146932690,92776,1468,7814,7866,1,1,'5976 5407 5401 5158 5157',4371.25,-452.70,192.708,0.140,30,0,33024),
(146932691,92776,1468,7814,7866,1,1,'5976 5407 5401 5158 5157',4398.88,-469.59,188.628,4.995,30,0,33024),
(146932692,92776,1468,7814,7866,1,1,'5976 5407 5401 5158 5157',4383.61,-455.99,192.537,0.388,30,0,33024),
(146932693,92776,1468,7814,7866,1,1,'5976 5407 5401 5158 5157',4383.75,-453.73,192.537,5.771,30,0,33024),
(146932694,92776,1468,7814,7866,1,1,'5976 5407 5401 5158 5157',4450.76,-515.49,192.536,2.816,30,0,33024),
(146932695,92776,1468,7814,7866,1,1,'5976 5407 5401 5158 5157',4448.16,-517.25,192.536,1.516,30,0,33024),
(146932696,92776,1468,7814,7866,1,1,'5976 5407 5401 5158 5157',4454.13,-513.77,192.537,4.894,30,0,33024),
(146932697,92776,1468,7814,7866,1,1,'5976 5407 5401 5158 5157',4512.84,-455.04,192.536,0.436,30,0,33024),
(146932698,92776,1468,7814,7866,1,1,'5976 5407 5401 5158 5157',4512.88,-452.78,192.536,5.819,30,0,33024),
(146932699,92776,1468,7814,7866,1,1,'5976 5407 5401 5158 5157',4516.40,-448.17,192.563,6.201,30,0,33024),
(146932700,92776,1468,7814,7866,1,1,'5976 5407 5401 5158',4437.84,-462.22,185.822,2.272,30,0,33024);

-- Top of the Vault: the freed Illidari set there by mistake are taken away (the demons there lie slain)
DELETE FROM `creature_addon` WHERE `guid` BETWEEN 146933210 AND 146933219;
DELETE FROM `creature` WHERE `guid` BETWEEN 146933210 AND 146933219;

-- Harbingers of the Past: the Pool of Judgment named in French
DELETE FROM `locales_gameobject` WHERE `entry`=244455;
INSERT INTO `locales_gameobject` (`entry`,`name_loc2`) VALUES (244455,'Bassin du Jugement');

-- Court of the Wardens: Kayn and Altruis fight each other until the Pool of Judgment reminds them (39687), then stand reconciled
DELETE FROM `phase_definitions` WHERE `zoneId`=7814 AND `entry` IN (110,111);
INSERT INTO `phase_definitions` (`zoneId`,`entry`,`phasemask`,`phaseId`,`PreloadMapID`,`VisibleMapID`,`UiWorldMapAreaID`,`flags`,`comment`) VALUES
(7814,110,0,'4902',0,0,0,16,'Kayn and Altruis fighting until 39687 is completed'),
(7814,111,0,'4913',0,0,0,16,'Kayn of the Court, reconciled (replaced below)');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND `SourceGroup`=7814 AND `SourceEntry` IN (110,111);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,7814,110,0,0,28,0,39687,0,0,1,0,'','39687 not completed...'),
(23,7814,110,0,0,8,0,39687,0,0,1,0,'','...nor rewarded'),
(23,7814,111,0,0,28,0,39687,0,0,0,0,'','39687 completed'),
(23,7814,111,0,1,8,0,39687,0,0,0,0,'','39687 rewarded');
UPDATE `creature` SET `PhaseId`='4902' WHERE `guid` IN (366921,366918);
UPDATE `creature` SET `PhaseId`='4911' WHERE `guid` IN (366927,366928);
DELETE FROM `creature_addon` WHERE `guid`=366929;
DELETE FROM `creature` WHERE `guid`=366929;
UPDATE `creature_template` SET `ScriptName`='npc_vault_court_duelist' WHERE `entry` IN (97265,97267);

-- Court of the Wardens: the lines of Kor’vas, Kayn and Altruis follow one another as their voices end, in English and in French
UPDATE `conversation_data` SET `textId`=0 WHERE `entry`=536 AND `idx`=0;
UPDATE `conversation_data` SET `textId`=3550 WHERE `entry`=536 AND `idx`=1;
UPDATE `conversation_data` SET `textId`=15500 WHERE `entry`=536 AND `idx`=2;
UPDATE `conversation_data` SET `textId`=21940 WHERE `entry`=536 AND `idx`=3;
UPDATE `conversation_creature` SET `duration`=9000 WHERE `entry`=536;

-- Harbingers of the Past (39687), never translated by Blizzard: its French texts
UPDATE `quest_template_locale` SET `LogTitle`='Les hérauts du passé', `LogDescription`='Regardez dans le Bassin du Jugement.', `QuestDescription`='$n, vous devez mettre fin à cette dispute avant que le mal ne soit irréparable.

Il faut rappeler à Kayn et à Altruis les liens qui les unissent.

Le Bassin du Jugement peut refléter les souvenirs du passé. Touchez-le et puisez dans vos propres souvenirs.

Rappelez-leur ce que signifiait combattre sous les ordres du seigneur Illidan. Rappelez-leur pourquoi nous avons choisi la voie du chasseur de démons.', `QuestCompletionLog`='Parlez à Kor’vas Roncesang au Caveau des Gardiennes.' WHERE `ID`=39687 AND `locale`='frFR';
UPDATE `quest_objectives_locale` SET `Description`='Bassin du Jugement activé' WHERE `ID`=279966 AND `locale`='frFR';
UPDATE `quest_request_items_locale` SET `CompletionText`='Le Bassin du Jugement peut nous rappeler notre passé.' WHERE `ID`=39687 AND `Locale`='frFR';

-- Top of the Vault and Court of the Wardens: the slain Savage Felstalkers, Wrathguard Legates and Vault Wardens lie dead on the floor
DELETE FROM `creature_addon` WHERE `guid` IN (SELECT `guid` FROM `creature` WHERE `id` IN (92782,97225,96645) AND `dynamicflags` IN (32,64) AND `map`=1468 AND `position_x` BETWEEN 4240 AND 4560 AND `position_y` BETWEEN -560 AND -340 AND `position_z`>240);
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`)
SELECT `guid`,0,0,0,1,0,'29266' FROM `creature` WHERE `id` IN (92782,97225,96645) AND `dynamicflags` IN (32,64) AND `map`=1468 AND `position_x` BETWEEN 4240 AND 4560 AND `position_y` BETWEEN -560 AND -340 AND `position_z`>240;
-- 0x20 is the Beast Lore information on this client (damage, health, tameable): dead is 0x40
UPDATE `creature` SET `dynamicflags`=64 WHERE `id` IN (92782,97225,96645) AND `dynamicflags`=32 AND `map`=1468 AND `position_x` BETWEEN 4240 AND 4560 AND `position_y` BETWEEN -560 AND -340 AND `position_z`>240;

-- Magolith's floor: the Vault Wardens are alive and hold the demons, not slain
UPDATE `creature` SET `unit_flags`=0, `dynamicflags`=0 WHERE `id`=96645 AND `map`=1468 AND `position_x` BETWEEN 4240 AND 4560 AND `position_y` BETWEEN -560 AND -340 AND `position_z` BETWEEN 170 AND 240;

-- A New Direction (40373): Kor'vas offers the choice between Kayn and Altruis again while it is not made
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=19012 AND `SourceEntry`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(15,19012,0,0,0,9,0,40373,0,0,0,0,'','Will it be Kayn or Altruis? while 40373 is taken');

-- Vault of the Wardens: every line of the conversations starts once the one before has ended, in English and in French
UPDATE `conversation_creature` SET `duration`=3017 WHERE `entry`=528 AND `id`=0;
UPDATE `conversation_data` SET `textId`=12703 WHERE `entry`=530 AND `idx`=2;
UPDATE `conversation_data` SET `textId`=4415 WHERE `entry`=538 AND `idx`=1;
UPDATE `conversation_creature` SET `duration`=3356 WHERE `entry`=538 AND `id`=0;
UPDATE `conversation_data` SET `textId`=6854 WHERE `entry`=833 AND `idx`=1;
UPDATE `conversation_creature` SET `duration`=7503 WHERE `entry`=833 AND `id`=0;
UPDATE `conversation_data` SET `textId`=15399 WHERE `entry`=3443 AND `idx`=2;
UPDATE `conversation_creature` SET `duration`=9899 WHERE `entry`=3443 AND `id`=0;

-- A New Direction (40373): the choice between Kayn and Altruis in French (the client language picks it)
DELETE FROM `playerchoice_locale` WHERE `ChoiceId`=234 AND `locale`='frFR';
INSERT INTO `playerchoice_locale` (`ChoiceId`,`locale`,`Question`,`VerifiedBuild`) VALUES (234,'frFR','Quel sectateur choisirez-vous ?',0);
DELETE FROM `playerchoice_response_locale` WHERE `ChoiceId`=234 AND `locale`='frFR';
INSERT INTO `playerchoice_response_locale` (`ChoiceId`,`ResponseId`,`locale`,`Header`,`Answer`,`Description`,`Confirmation`,`VerifiedBuild`) VALUES
(234,486,'frFR','','Kayn Solfurie','Kayn Solfurie est un soldat loyal et l’un des lieutenants les plus fidèles du seigneur Illidan Hurlorage.

Il est convaincu que la coopération et le sacrifice personnel sont indispensables pour vaincre la Légion.

Honorable et exigeant, il suit les idéaux d’Illidan pour accomplir sa mission, quel qu’en soit le prix.

|cFF7a0000 Ce choix scénaristique n’aura aucune incidence sur la puissance de votre personnage.|r','',0),
(234,487,'frFR','','Altruis le Souffrant','Altruis le Souffrant est un renégat qui n’hésite pas à prendre les choses en main.

S’il poursuit les mêmes buts qu’Illidan, il désapprouve souvent ses méthodes.

Persuadé qu’Illidan ne maîtrise plus ses pouvoirs démoniaques, Altruis veut donner un nouveau chef aux Illidari.

|cFF7a0000 Ce choix scénaristique n’aura aucune incidence sur la puissance de votre personnage.|r','',0);

-- The end of the Vault: after the choice the follower left aside walks away, the chosen one and Kor'vas run on to
-- Bastillax's chamber once The Hunt for Freedom is taken; Kayn or Altruis waits there and outside as chosen
DELETE FROM `phase_definitions` WHERE `zoneId`=7814 AND `entry` BETWEEN 111 AND 115;
INSERT INTO `phase_definitions` (`zoneId`,`entry`,`phasemask`,`phaseId`,`PreloadMapID`,`VisibleMapID`,`UiWorldMapAreaID`,`flags`,`comment`) VALUES
(7814,111,0,'4913',0,0,0,16,'Kayn in the Court until he leaves'),
(7814,112,0,'4914',0,0,0,16,'Altruis in the Court until he leaves'),
(7814,113,0,'4915',0,0,0,16,'Kor''vas in the Court until The Hunt for Freedom is taken'),
(7814,114,0,'4917',0,0,0,16,'Kayn chosen: Bastillax''s chamber and the way out'),
(7814,115,0,'4918',0,0,0,16,'Altruis chosen: Bastillax''s chamber and the way out');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=23 AND `SourceGroup`=7814 AND `SourceEntry` BETWEEN 111 AND 115;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(23,7814,111,0,0,28,0,39687,0,0,0,0,'','Kayn: 39687 completed'),
(23,7814,111,0,0,8,0,40375,0,0,1,0,'','Kayn: not left aside'),
(23,7814,111,0,0,14,0,39688,0,0,0,0,'','Kayn: 39688 not taken'),
(23,7814,111,0,0,14,0,39694,0,0,0,0,'','Kayn: 39694 not taken'),
(23,7814,111,0,1,8,0,39687,0,0,0,0,'','Kayn: 39687 rewarded'),
(23,7814,111,0,1,8,0,40375,0,0,1,0,'','Kayn: not left aside'),
(23,7814,111,0,1,14,0,39688,0,0,0,0,'','Kayn: 39688 not taken'),
(23,7814,111,0,1,14,0,39694,0,0,0,0,'','Kayn: 39694 not taken'),
(23,7814,112,0,0,28,0,39687,0,0,0,0,'','Altruis: 39687 completed'),
(23,7814,112,0,0,8,0,40374,0,0,1,0,'','Altruis: not left aside'),
(23,7814,112,0,0,14,0,39688,0,0,0,0,'','Altruis: 39688 not taken'),
(23,7814,112,0,0,14,0,39694,0,0,0,0,'','Altruis: 39694 not taken'),
(23,7814,112,0,1,8,0,39687,0,0,0,0,'','Altruis: 39687 rewarded'),
(23,7814,112,0,1,8,0,40374,0,0,1,0,'','Altruis: not left aside'),
(23,7814,112,0,1,14,0,39688,0,0,0,0,'','Altruis: 39688 not taken'),
(23,7814,112,0,1,14,0,39694,0,0,0,0,'','Altruis: 39694 not taken'),
(23,7814,113,0,0,14,0,39688,0,0,0,0,'','Kor''vas of the Court: 39688 not taken'),
(23,7814,113,0,0,14,0,39694,0,0,0,0,'','Kor''vas of the Court: 39694 not taken'),
(23,7814,114,0,0,8,0,40374,0,0,0,0,'','Kayn chosen'),
(23,7814,115,0,0,8,0,40375,0,0,0,0,'','Altruis chosen');
UPDATE `creature` SET `PhaseId`='4913' WHERE `guid`=366927;
UPDATE `creature` SET `PhaseId`='4914' WHERE `guid`=366928;
UPDATE `creature` SET `PhaseId`='4915' WHERE `guid`=366930;

-- Bastillax's chamber: seen by the demon hunters who made the choice; the demons stacked twice on the same spot go
DELETE FROM `creature_addon` WHERE `guid` IN (366994,366995,366977,366978,366980,366982,366985,366992,366996,366997,366998);
DELETE FROM `creature` WHERE `guid` IN (366994,366995,366977,366978,366980,366982,366985,366992,366996,366997,366998);
UPDATE `creature` SET `PhaseId`='4917 4918' WHERE `map`=1468 AND `position_x` BETWEEN 4100 AND 4260 AND `position_y` BETWEEN -700 AND -560 AND `position_z` BETWEEN 240 AND 280 AND `PhaseId` IN ('','4917 4918');
UPDATE `creature` SET `PhaseId`='4917' WHERE `guid`=366950;
DELETE FROM `creature` WHERE `guid` IN (146932701,146932702);
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`PhaseId`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`MovementType`) VALUES
(146932701,97274,1468,7814,7864,1,1,'4918',4186.1,-610.0,255.3,4.61132,180,0),
(146932702,96669,1468,7814,7814,1,1,'4918',4161.0,-855.7,291.2,4.2055,180,0);
UPDATE `creature` SET `PhaseId`='4917' WHERE `guid`=367016;
UPDATE `creature_template` SET `ScriptName`='npc_96666' WHERE `entry`=96669;
UPDATE `creature_template` SET `ScriptName`='npc_vault_bastillax_ally' WHERE `entry` IN (97273,97274,99968,100065);
DELETE FROM `spell_script_names` WHERE `spell_id` IN (196661,196662);
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES (196661,'spell_vault_follower_choice'),(196662,'spell_vault_follower_choice');

-- Outside the Vault: Khadgar only comes for the demon hunter (npc_96666); the one standing there was a second Khadgar
DELETE FROM `creature_addon` WHERE `guid`=369016;
DELETE FROM `creature` WHERE `guid`=369016;

-- Outside the Vault: Khadgar lands as a raven, then takes back his own shape and speaks
UPDATE `smart_scripts` SET `event_param1`=6000, `event_param2`=6000 WHERE `entryorguid`=97978 AND `source_type`=0 AND `id`=0;
UPDATE `smart_scripts` SET `event_param1`=7500, `event_param2`=7500 WHERE `entryorguid`=97978 AND `source_type`=0 AND `id`=1;
UPDATE `smart_scripts` SET `event_param1`=11000, `event_param2`=12000 WHERE `entryorguid`=97978 AND `source_type`=0 AND `id`=2;

-- Vault of the Wardens: the Legion Portals keep sending demons at the Illidari
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_vault_legion_portal' WHERE `entry` = 99501;

-- Vault of the Wardens: the demons stacked on the same spots in the portal hall become one each, duplicate portals removed
UPDATE `creature` SET `PhaseId`='5129 5158 5366 5367 5401 5407 5409 5494 5495 5976' WHERE `guid`=366360;
UPDATE `creature` SET `PhaseId`='5129 5158 5309 5366 5367 5401 5407 5409 5494 5495 5964 5965 5966 5976 5982' WHERE `guid`=366361;
UPDATE `creature` SET `PhaseId`='5129 5158 5366 5367 5401 5407 5409 5494 5495 5976 5982' WHERE `guid`=366364;
UPDATE `creature` SET `PhaseId`='5129 5158 5309 5366 5367 5401 5407 5409 5494 5495 5964 5965 5966 5976 5982' WHERE `guid`=366369;
UPDATE `creature` SET `PhaseId`='5129 5158 5309 5367 5401 5407 5409 5494 5495 5964 5965 5966 5976 5982' WHERE `guid`=366381;
UPDATE `creature` SET `PhaseId`='5129 5158 5366 5367 5401 5407 5409 5494 5495 5976 5982' WHERE `guid`=366394;
UPDATE `creature` SET `PhaseId`='5129 5158 5309 5401 5407 5409 5494 5495 5964 5965 5966 5976 5982' WHERE `guid`=366400;
DELETE FROM `creature` WHERE `guid` IN (366362,366365,366368,366379,366380,366385,366386,366387,366388,366389,366390,366392,366395,366396,366397,366402,366403,366405,366410,366429,366433,366434,366436,366437,366438,366440,366450,366451,366453,366454,366455,366457,366459,366460,366462,366466,366467,366468,366470,366471,366472,366473,366474,366475,366476,366477,366485,366486,366487,366489,366490,366491,366492,366494,366496,366497,366498,366499,366501,366504,366505,366506,366507,366508,366509,366510,366511,366513,366514,366515,366516,366464,366465,366478,366488,366495,366502);

-- Vault of the Wardens: the spawns placed, moved or removed in game
DELETE FROM `creature` WHERE `guid` = 146931510;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `AiID`, `MovementID`, `MeleeID`, `isActive`, `skipClone`, `personal_size`, `isTeemingSpawn`, `unit_flags3`) VALUES (146931510, 96656, 1468, 7814, 7872, 1, 1, '5121 5124 5125 5129 5157 5158 5309 5401 5407 5409 5964 5965 5966 5976 5982 7016 7017', 0, 1, 4332.81, -481.719, -288.638, 1.38795, 300, 0, 0, 3589584, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` = 146931510;
DELETE FROM `creature` WHERE `guid` = 146931511;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `AiID`, `MovementID`, `MeleeID`, `isActive`, `skipClone`, `personal_size`, `isTeemingSpawn`, `unit_flags3`) VALUES (146931511, 96656, 1468, 7814, 7814, 1, 65535, '5121 5124 5125 5129 5157 5158 5309 5401 5407 5409 5964 5965 5966 5976 5982 7016 7017', 0, 1, 4332.21, -488.671, -288.788, 2.00056, 300, 0, 0, 3589584, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` = 146931511;
DELETE FROM `creature` WHERE `guid` = 366399;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `AiID`, `MovementID`, `MeleeID`, `isActive`, `skipClone`, `personal_size`, `isTeemingSpawn`, `unit_flags3`) VALUES (366399, 96656, 1468, 7814, 7872, 1, 1, '5982 5966 5409 5407 5401 5309 5158 5129 5125 5124', 0, 0, 4269.99, -496.961, -281.847, 2.85665, 180, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` = 366399;
DELETE FROM `creature` WHERE `guid` = 366409;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `AiID`, `MovementID`, `MeleeID`, `isActive`, `skipClone`, `personal_size`, `isTeemingSpawn`, `unit_flags3`) VALUES (366409, 96656, 1468, 7814, 7872, 1, 1, '5982 5966 5965 5964 5409 5407 5401 5309 5158 5129', 0, 0, 4270.24, -481.774, -281.925, 4.24288, 180, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` = 366409;
DELETE FROM `creature` WHERE `guid` = 146932094;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `AiID`, `MovementID`, `MeleeID`, `isActive`, `skipClone`, `personal_size`, `isTeemingSpawn`, `unit_flags3`) VALUES (146932094, 96656, 1468, 7814, 7872, 1, 1, '5121 5124 5125 5129 5157 5158 5309 5401 5407 5409 5964 5965 5966 5976 5982 7016 7017', 0, 1, 4340.57, -499.352, -288.639, 0.308036, 300, 0, 0, 2385036, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` = 146932094;
DELETE FROM `creature` WHERE `guid` = 146932095;
DELETE FROM `creature_addon` WHERE `guid` = 146932095;
DELETE FROM `creature` WHERE `guid` = 366415;
DELETE FROM `creature_addon` WHERE `guid` = 366415;
DELETE FROM `creature` WHERE `guid` = 146932112;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `AiID`, `MovementID`, `MeleeID`, `isActive`, `skipClone`, `personal_size`, `isTeemingSpawn`, `unit_flags3`) VALUES (146932112, 96656, 1468, 7814, 7872, 1, 65535, '5121 5124 5125 5129 5157 5158 5309 5401 5407 5409 5964 5965 5966 5976 5982 7016 7017', 0, 1, 4346.1, -489.481, -288.585, 5.54916, 300, 0, 0, 3589584, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` = 146932112;
DELETE FROM `creature` WHERE `guid` = 366411;
DELETE FROM `creature_addon` WHERE `guid` = 366411;
DELETE FROM `creature` WHERE `guid` = 366412;
DELETE FROM `creature_addon` WHERE `guid` = 366412;
DELETE FROM `creature` WHERE `guid` = 366393;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `AiID`, `MovementID`, `MeleeID`, `isActive`, `skipClone`, `personal_size`, `isTeemingSpawn`, `unit_flags3`) VALUES (366393, 99451, 1468, 7814, 7872, 1, 1, '5976 5409 5407 5401 5367 5366 5158 5129', 0, 0, 4338.11, -478.848, -288.639, 2.75311, 180, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` = 366393;
DELETE FROM `creature` WHERE `guid` = 366456;
DELETE FROM `creature_addon` WHERE `guid` = 366456;
DELETE FROM `creature` WHERE `guid` = 366406;
DELETE FROM `creature_addon` WHERE `guid` = 366406;
DELETE FROM `creature` WHERE `guid` = 366583;
DELETE FROM `creature_addon` WHERE `guid` = 366583;
DELETE FROM `creature` WHERE `guid` = 366432;
DELETE FROM `creature_addon` WHERE `guid` = 366432;
DELETE FROM `creature` WHERE `guid` = 366572;
DELETE FROM `creature_addon` WHERE `guid` = 366572;
DELETE FROM `creature` WHERE `guid` = 366576;
DELETE FROM `creature_addon` WHERE `guid` = 366576;
DELETE FROM `creature` WHERE `guid` = 366573;
DELETE FROM `creature_addon` WHERE `guid` = 366573;
DELETE FROM `creature` WHERE `guid` = 366578;
DELETE FROM `creature_addon` WHERE `guid` = 366578;
DELETE FROM `creature` WHERE `guid` = 146932650;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `AiID`, `MovementID`, `MeleeID`, `isActive`, `skipClone`, `personal_size`, `isTeemingSpawn`, `unit_flags3`) VALUES (146932650, 96656, 1468, 7814, 7866, 1, 3, '4990 5129 5147 5148 5150 5157 5158 5401 5407 5409 5437 5438 5439 5440 5442 5494 5495 5498 5518 5976 7016 7017', 0, 1, 4429.19, -442.859, 119.185, 0.326298, 300, 0, 0, 3589584, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` = 146932650;
DELETE FROM `creature` WHERE `guid` = 366702;
DELETE FROM `creature_addon` WHERE `guid` = 366702;
DELETE FROM `creature` WHERE `guid` = 366697;
DELETE FROM `creature_addon` WHERE `guid` = 366697;
DELETE FROM `creature` WHERE `guid` = 146933210;
DELETE FROM `creature_addon` WHERE `guid` = 146933210;
DELETE FROM `creature` WHERE `guid` = 146933211;
DELETE FROM `creature_addon` WHERE `guid` = 146933211;
DELETE FROM `creature` WHERE `guid` = 146933212;
DELETE FROM `creature_addon` WHERE `guid` = 146933212;
DELETE FROM `creature` WHERE `guid` = 146933213;
DELETE FROM `creature_addon` WHERE `guid` = 146933213;
DELETE FROM `creature` WHERE `guid` = 146933214;
DELETE FROM `creature_addon` WHERE `guid` = 146933214;
DELETE FROM `creature` WHERE `guid` = 146933215;
DELETE FROM `creature_addon` WHERE `guid` = 146933215;
DELETE FROM `creature` WHERE `guid` = 146933216;
DELETE FROM `creature_addon` WHERE `guid` = 146933216;
DELETE FROM `creature` WHERE `guid` = 146933217;
DELETE FROM `creature_addon` WHERE `guid` = 146933217;
DELETE FROM `creature` WHERE `guid` = 146933218;
DELETE FROM `creature_addon` WHERE `guid` = 146933218;
DELETE FROM `creature` WHERE `guid` = 146933219;
DELETE FROM `creature_addon` WHERE `guid` = 146933219;
DELETE FROM `creature` WHERE `guid` = 366897;
DELETE FROM `creature_addon` WHERE `guid` = 366897;
