-- Silithus: world map of the Wound, wasps of Hive'Zora, population of the past and level ranges

UPDATE `phase_definitions` SET `VisibleMapID` = 1206, `UiWorldMapAreaID` = 0 WHERE `PreloadMapID` = 1817;

DELETE FROM `creature_template_movement` WHERE `CreatureId` = 11727;
INSERT INTO `creature_template_movement` (`CreatureId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Random`, `InteractionPauseTimer`) VALUES (11727, 1, 0, 1, 0, 0, NULL);
UPDATE `creature` SET `position_x` = -7077.00, `position_y` = 1752.33, `position_z` = 1.86, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165068;
UPDATE `creature` SET `position_x` = -7077.00, `position_y` = 1714.33, `position_z` = 5.53, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165069;
UPDATE `creature` SET `position_x` = -7025.08, `position_y` = 1647.52, `position_z` = 17.10, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165070;
UPDATE `creature` SET `position_x` = -7689.00, `position_y` = 1656.33, `position_z` = 8.36, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165071;
UPDATE `creature` SET `position_x` = -7677.00, `position_y` = 1690.33, `position_z` = 22.37, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165072;
UPDATE `creature` SET `position_x` = -7679.83, `position_y` = 1575.21, `position_z` = 13.61, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165073;
UPDATE `creature` SET `position_x` = -7708.36, `position_y` = 1497.34, `position_z` = 23.12, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165074;
UPDATE `creature` SET `position_x` = -7587.43, `position_y` = 1628.91, `position_z` = 21.61, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165075;
UPDATE `creature` SET `position_x` = -7595.00, `position_y` = 1716.33, `position_z` = -30.48, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165076;
UPDATE `creature` SET `position_x` = -7577.00, `position_y` = 1780.33, `position_z` = -35.47, `MovementType` = 1, `spawndist` = 4.0 WHERE `guid` = 165077;
UPDATE `creature` SET `position_x` = -7625.00, `position_y` = 1710.33, `position_z` = -29.40, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165078;
UPDATE `creature` SET `position_x` = -7625.00, `position_y` = 1796.33, `position_z` = -31.77, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165079;
UPDATE `creature` SET `position_x` = -7571.00, `position_y` = 1766.33, `position_z` = -38.55, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165080;
UPDATE `creature` SET `position_x` = -7149.00, `position_y` = 1562.33, `position_z` = -4.05, `MovementType` = 1, `spawndist` = 4.0 WHERE `guid` = 165081;
UPDATE `creature` SET `position_x` = -7109.57, `position_y` = 1537.27, `position_z` = 12.18, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165082;
UPDATE `creature` SET `position_x` = -7101.00, `position_y` = 1538.33, `position_z` = 4.55, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165083;
UPDATE `creature` SET `position_x` = -7151.00, `position_y` = 1588.33, `position_z` = -4.25, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165084;
UPDATE `creature` SET `position_x` = -7088.58, `position_y` = 1486.38, `position_z` = 21.46, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165085;
UPDATE `creature` SET `position_x` = -7131.00, `position_y` = 1518.33, `position_z` = 4.46, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165086;
UPDATE `creature` SET `position_x` = -7145.00, `position_y` = 1468.33, `position_z` = 6.08, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165087;
UPDATE `creature` SET `position_x` = -7437.00, `position_y` = 1790.33, `position_z` = -96.57, `MovementType` = 1, `spawndist` = 3.0 WHERE `guid` = 165088;
UPDATE `creature` SET `position_x` = -7397.00, `position_y` = 1758.33, `position_z` = -86.02, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165089;
UPDATE `creature` SET `position_x` = -7459.00, `position_y` = 1824.33, `position_z` = -45.13, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165090;
UPDATE `creature` SET `position_x` = -7425.00, `position_y` = 1824.33, `position_z` = -98.17, `MovementType` = 1, `spawndist` = 3.0 WHERE `guid` = 165091;
UPDATE `creature` SET `position_x` = -7405.00, `position_y` = 1756.33, `position_z` = -86.88, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165092;
UPDATE `creature` SET `position_x` = -7381.00, `position_y` = 1556.33, `position_z` = -77.94, `MovementType` = 1, `spawndist` = 4.0 WHERE `guid` = 165093;
UPDATE `creature` SET `position_x` = -7367.00, `position_y` = 1586.33, `position_z` = -13.86, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165094;
UPDATE `creature` SET `position_x` = -7469.00, `position_y` = 1674.33, `position_z` = -29.62, `MovementType` = 1, `spawndist` = 2.5 WHERE `guid` = 165095;
UPDATE `creature` SET `position_x` = -7391.00, `position_y` = 1580.33, `position_z` = -75.91, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165096;
UPDATE `creature` SET `position_x` = -7343.00, `position_y` = 1632.33, `position_z` = -75.26, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165097;
UPDATE `creature` SET `position_x` = -7371.00, `position_y` = 1730.33, `position_z` = -86.13, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165098;
UPDATE `creature` SET `position_x` = -7371.00, `position_y` = 1690.33, `position_z` = -85.16, `MovementType` = 1, `spawndist` = 5.0 WHERE `guid` = 165099;
UPDATE `creature` SET `position_x` = -7087.00, `position_y` = 2042.33, `position_z` = 116.03, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165100;
UPDATE `creature` SET `position_x` = -7331.00, `position_y` = 1830.33, `position_z` = -84.45, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165101;
UPDATE `creature` SET `position_x` = -7339.00, `position_y` = 1848.33, `position_z` = -81.06, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165102;
UPDATE `creature` SET `position_x` = -7295.00, `position_y` = 1852.33, `position_z` = -83.69, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165103;
UPDATE `creature` SET `position_x` = -7303.00, `position_y` = 1842.33, `position_z` = -79.87, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165104;
UPDATE `creature` SET `position_x` = -7385.00, `position_y` = 1376.33, `position_z` = -79.46, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165105;
UPDATE `creature` SET `position_x` = -7423.00, `position_y` = 1512.33, `position_z` = -18.46, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165106;
UPDATE `creature` SET `position_x` = -7373.00, `position_y` = 1266.33, `position_z` = -77.52, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165107;
UPDATE `creature` SET `position_x` = -7427.00, `position_y` = 1484.33, `position_z` = -58.08, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165108;
UPDATE `creature` SET `position_x` = -7393.00, `position_y` = 1470.33, `position_z` = -39.38, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165109;
UPDATE `creature` SET `position_x` = -7439.00, `position_y` = 1256.33, `position_z` = -43.95, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165110;
UPDATE `creature` SET `position_x` = -7413.00, `position_y` = 1272.33, `position_z` = -84.09, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165111;
UPDATE `creature` SET `position_x` = -7443.00, `position_y` = 1506.33, `position_z` = -3.93, `MovementType` = 1, `spawndist` = 2.5 WHERE `guid` = 165112;
UPDATE `creature` SET `position_x` = -7501.00, `position_y` = 1416.33, `position_z` = -84.04, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165113;
UPDATE `creature` SET `position_x` = -7439.00, `position_y` = 1408.33, `position_z` = -84.63, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165114;
UPDATE `creature` SET `position_x` = -7409.00, `position_y` = 1312.33, `position_z` = -83.82, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165115;
UPDATE `creature` SET `position_x` = -7415.00, `position_y` = 1220.33, `position_z` = -70.25, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165116;
UPDATE `creature` SET `position_x` = -7399.00, `position_y` = 1218.33, `position_z` = -72.74, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165117;
UPDATE `creature` SET `position_x` = -7409.00, `position_y` = 1222.33, `position_z` = -46.58, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165118;
UPDATE `creature` SET `position_x` = -7417.00, `position_y` = 1252.33, `position_z` = -50.94, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165119;
UPDATE `creature` SET `position_x` = -7167.00, `position_y` = 1796.33, `position_z` = -72.74, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165120;
UPDATE `creature` SET `position_x` = -7081.00, `position_y` = 1844.33, `position_z` = 1.63, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165121;
UPDATE `creature` SET `position_x` = -7171.00, `position_y` = 1784.33, `position_z` = -74.89, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165122;
UPDATE `creature` SET `position_x` = -7394.88, `position_y` = 1461.26, `position_z` = 28.64, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165123;
UPDATE `creature` SET `position_x` = -7423.00, `position_y` = 1528.33, `position_z` = 0.74, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165124;
UPDATE `creature` SET `position_x` = -7351.00, `position_y` = 1436.33, `position_z` = 7.48, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165125;
UPDATE `creature` SET `position_x` = -7251.00, `position_y` = 1810.33, `position_z` = -31.38, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165126;
UPDATE `creature` SET `position_x` = -7391.00, `position_y` = 1798.33, `position_z` = -32.43, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165127;
UPDATE `creature` SET `position_x` = -7337.00, `position_y` = 1826.33, `position_z` = -6.81, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165128;
UPDATE `creature` SET `position_x` = -7275.00, `position_y` = 1854.33, `position_z` = 4.46, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165129;
UPDATE `creature` SET `position_x` = -7393.33, `position_y` = 1839.88, `position_z` = -12.64, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165130;
UPDATE `creature` SET `position_x` = -7111.00, `position_y` = 1836.33, `position_z` = 12.64, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165131;
UPDATE `creature` SET `position_x` = -7166.63, `position_y` = 1814.49, `position_z` = 41.12, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165132;
UPDATE `creature` SET `position_x` = -7099.00, `position_y` = 1786.33, `position_z` = -1.84, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165133;
UPDATE `creature` SET `position_x` = -7387.00, `position_y` = 1178.33, `position_z` = 5.30, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165134;
UPDATE `creature` SET `position_x` = -7386.43, `position_y` = 1397.74, `position_z` = 14.40, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165135;
UPDATE `creature` SET `position_x` = -7371.00, `position_y` = 1246.33, `position_z` = 1.60, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165136;
UPDATE `creature` SET `position_x` = -7377.00, `position_y` = 1372.33, `position_z` = 5.53, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165137;
UPDATE `creature` SET `position_x` = -7261.00, `position_y` = 1680.33, `position_z` = -69.44, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165138;
UPDATE `creature` SET `position_x` = -7411.00, `position_y` = 1708.33, `position_z` = -34.19, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165139;
UPDATE `creature` SET `position_x` = -7341.00, `position_y` = 1648.33, `position_z` = -31.16, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165140;
UPDATE `creature` SET `position_x` = -7410.28, `position_y` = 1650.92, `position_z` = -7.22, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165141;
UPDATE `creature` SET `position_x` = -7307.00, `position_y` = 1828.33, `position_z` = -85.35, `MovementType` = 1, `spawndist` = 5.0 WHERE `guid` = 165142;
UPDATE `creature` SET `position_x` = -7243.00, `position_y` = 1854.33, `position_z` = -81.69, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165143;
UPDATE `creature` SET `position_x` = -7233.00, `position_y` = 1762.33, `position_z` = -62.72, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165144;
UPDATE `creature` SET `position_x` = -7231.00, `position_y` = 1836.33, `position_z` = -75.75, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165145;
UPDATE `creature` SET `position_x` = -7231.00, `position_y` = 1812.33, `position_z` = -67.29, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165146;
UPDATE `creature` SET `position_x` = -7577.00, `position_y` = 1682.33, `position_z` = -31.28, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165147;
UPDATE `creature` SET `position_x` = -7591.00, `position_y` = 1698.33, `position_z` = -35.11, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165148;
UPDATE `creature` SET `position_x` = -7525.00, `position_y` = 1730.33, `position_z` = -20.04, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165149;
UPDATE `creature` SET `position_x` = -7158.34, `position_y` = 1416.00, `position_z` = 15.52, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165150;
UPDATE `creature` SET `position_x` = -7195.32, `position_y` = 1307.82, `position_z` = 27.57, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165151;
UPDATE `creature` SET `position_x` = -7511.00, `position_y` = 1820.33, `position_z` = -5.85, `MovementType` = 1, `spawndist` = 3.0 WHERE `guid` = 165152;
UPDATE `creature` SET `position_x` = -7497.00, `position_y` = 1790.33, `position_z` = -29.92, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165153;
UPDATE `creature` SET `position_x` = -7507.00, `position_y` = 1904.33, `position_z` = -53.08, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165154;
UPDATE `creature` SET `position_x` = -7473.00, `position_y` = 1866.33, `position_z` = -55.22, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165155;
UPDATE `creature` SET `position_x` = -7519.00, `position_y` = 1934.33, `position_z` = -13.25, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165156;
UPDATE `creature` SET `position_x` = -7481.00, `position_y` = 1992.33, `position_z` = 79.09, `MovementType` = 1, `spawndist` = 3.0 WHERE `guid` = 165157;
UPDATE `creature` SET `position_x` = -7559.00, `position_y` = 1942.33, `position_z` = -55.85, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165158;
UPDATE `creature` SET `position_x` = -7645.00, `position_y` = 1984.33, `position_z` = 129.96, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165159;
UPDATE `creature` SET `position_x` = -7575.00, `position_y` = 1580.33, `position_z` = -64.37, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165160;
UPDATE `creature` SET `position_x` = -7539.00, `position_y` = 1360.33, `position_z` = -87.94, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165161;
UPDATE `creature` SET `position_x` = -7593.00, `position_y` = 1570.33, `position_z` = -12.01, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165162;
UPDATE `creature` SET `position_x` = -7541.00, `position_y` = 1474.33, `position_z` = -73.11, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165163;
UPDATE `creature` SET `position_x` = -7575.00, `position_y` = 1848.33, `position_z` = -48.24, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165164;
UPDATE `creature` SET `position_x` = -7557.00, `position_y` = 1890.33, `position_z` = -59.43, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165165;
UPDATE `creature` SET `position_x` = -7545.00, `position_y` = 1876.33, `position_z` = -27.80, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165166;
UPDATE `creature` SET `position_x` = -7549.00, `position_y` = 1914.33, `position_z` = -57.50, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165167;
UPDATE `creature` SET `position_x` = -7311.00, `position_y` = 1594.33, `position_z` = -69.65, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165168;
UPDATE `creature` SET `position_x` = -7311.00, `position_y` = 1640.33, `position_z` = -48.08, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165169;
UPDATE `creature` SET `position_x` = -7323.00, `position_y` = 1750.33, `position_z` = -53.46, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165170;
UPDATE `creature` SET `position_x` = -7281.00, `position_y` = 1640.33, `position_z` = -73.04, `MovementType` = 1, `spawndist` = 2.5 WHERE `guid` = 165171;
UPDATE `creature` SET `position_x` = -7059.00, `position_y` = 1812.33, `position_z` = -13.68, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165172;
UPDATE `creature` SET `position_x` = -7079.00, `position_y` = 1804.33, `position_z` = -13.92, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165173;
UPDATE `creature` SET `position_x` = -7273.00, `position_y` = 1612.33, `position_z` = -73.92, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165174;
UPDATE `creature` SET `position_x` = -7648.95, `position_y` = 1397.74, `position_z` = 18.65, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165916;
UPDATE `creature` SET `position_x` = -7571.00, `position_y` = 1706.33, `position_z` = -34.97, `MovementType` = 1, `spawndist` = 3.0 WHERE `guid` = 165923;
UPDATE `creature` SET `position_x` = -7201.00, `position_y` = 1806.33, `position_z` = -29.97, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165925;
UPDATE `creature` SET `position_x` = -7300.78, `position_y` = 1230.21, `position_z` = 24.31, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 165946;
UPDATE `creature` SET `position_x` = -7565.00, `position_y` = 1928.33, `position_z` = -56.82, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 165964;
UPDATE `creature` SET `position_x` = -7441.00, `position_y` = 1268.33, `position_z` = 4.39, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 165967;
UPDATE `creature` SET `position_x` = -7545.00, `position_y` = 1624.33, `position_z` = -1.47, `MovementType` = 1, `spawndist` = 3.0 WHERE `guid` = 166004;
UPDATE `creature` SET `position_x` = -7453.00, `position_y` = 1370.33, `position_z` = -77.72, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166022;
UPDATE `creature` SET `position_x` = -7347.00, `position_y` = 1710.33, `position_z` = -82.80, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 166032;
UPDATE `creature` SET `position_x` = -7311.00, `position_y` = 1360.33, `position_z` = -0.38, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166126;
UPDATE `creature` SET `position_x` = -7443.00, `position_y` = 1684.33, `position_z` = -86.85, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166133;
UPDATE `creature` SET `position_x` = -7625.00, `position_y` = 1542.33, `position_z` = 8.00, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 166145;
UPDATE `creature` SET `position_x` = -7605.00, `position_y` = 1686.33, `position_z` = -27.62, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166155;
UPDATE `creature` SET `position_x` = -7491.00, `position_y` = 1766.33, `position_z` = -33.47, `MovementType` = 1, `spawndist` = 4.0 WHERE `guid` = 166160;
UPDATE `creature` SET `position_x` = -7253.00, `position_y` = 1734.33, `position_z` = -59.78, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166163;
UPDATE `creature` SET `position_x` = -7217.00, `position_y` = 1486.33, `position_z` = 0.89, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166177;
UPDATE `creature` SET `position_x` = -7559.00, `position_y` = 1914.33, `position_z` = -58.89, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 166185;
UPDATE `creature` SET `position_x` = -7501.00, `position_y` = 1836.33, `position_z` = -4.65, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166243;
UPDATE `creature` SET `position_x` = -7247.00, `position_y` = 1558.33, `position_z` = -75.58, `MovementType` = 1, `spawndist` = 4.0 WHERE `guid` = 166270;
UPDATE `creature` SET `position_x` = -7329.00, `position_y` = 1632.33, `position_z` = -70.82, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 166277;
UPDATE `creature` SET `position_x` = -7561.00, `position_y` = 1380.33, `position_z` = 3.89, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 166280;
UPDATE `creature` SET `position_x` = -7611.00, `position_y` = 1650.33, `position_z` = 12.89, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166282;
UPDATE `creature` SET `position_x` = -7266.44, `position_y` = 1506.53, `position_z` = 21.42, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 166328;
UPDATE `creature` SET `position_x` = -7468.45, `position_y` = 1278.20, `position_z` = 20.57, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 166345;
UPDATE `creature` SET `position_x` = -7540.75, `position_y` = 1544.67, `position_z` = 15.55, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 166352;
UPDATE `creature` SET `position_x` = -7051.00, `position_y` = 1856.33, `position_z` = -13.30, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166363;
UPDATE `creature` SET `position_x` = -7413.00, `position_y` = 1796.33, `position_z` = -96.05, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166368;
UPDATE `creature` SET `position_x` = -7339.00, `position_y` = 1296.33, `position_z` = 7.23, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166378;
UPDATE `creature` SET `position_x` = -7537.00, `position_y` = 1734.33, `position_z` = 8.29, `MovementType` = 1, `spawndist` = 5.0 WHERE `guid` = 166387;
UPDATE `creature` SET `position_x` = -7241.00, `position_y` = 1844.33, `position_z` = 1.33, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166413;
UPDATE `creature` SET `position_x` = -7517.00, `position_y` = 1378.33, `position_z` = 5.17, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166446;
UPDATE `creature` SET `position_x` = -7273.00, `position_y` = 1554.33, `position_z` = -73.41, `MovementType` = 1, `spawndist` = 2.5 WHERE `guid` = 166472;
UPDATE `creature` SET `position_x` = -7399.00, `position_y` = 1728.33, `position_z` = -89.78, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166477;
UPDATE `creature` SET `position_x` = -7495.00, `position_y` = 1454.33, `position_z` = 7.90, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166479;
UPDATE `creature` SET `position_x` = -7533.00, `position_y` = 1364.33, `position_z` = -84.84, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 166480;
UPDATE `creature` SET `position_x` = -7395.00, `position_y` = 1678.33, `position_z` = -86.45, `MovementType` = 1, `spawndist` = 5.0 WHERE `guid` = 166489;
UPDATE `creature` SET `position_x` = -7381.00, `position_y` = 1456.33, `position_z` = 6.70, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 166534;
UPDATE `creature` SET `position_x` = -7497.00, `position_y` = 1956.33, `position_z` = -34.13, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166544;
UPDATE `creature` SET `position_x` = -7594.95, `position_y` = 1570.67, `position_z` = 26.33, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 166552;
UPDATE `creature` SET `position_x` = -7435.00, `position_y` = 1810.33, `position_z` = -42.61, `MovementType` = 1, `spawndist` = 2.5 WHERE `guid` = 166564;
UPDATE `creature` SET `position_x` = -7491.00, `position_y` = 1874.33, `position_z` = -56.62, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166593;
UPDATE `creature` SET `position_x` = -7473.00, `position_y` = 1296.33, `position_z` = -69.17, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166594;
UPDATE `creature` SET `position_x` = -7307.00, `position_y` = 1742.33, `position_z` = -52.51, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166599;
UPDATE `creature` SET `position_x` = -7448.43, `position_y` = 1420.13, `position_z` = 19.95, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 166667;
UPDATE `creature` SET `position_x` = -7525.00, `position_y` = 1658.33, `position_z` = -41.61, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166669;
UPDATE `creature` SET `position_x` = -7409.00, `position_y` = 1560.33, `position_z` = -32.62, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166684;
UPDATE `creature` SET `position_x` = -7407.00, `position_y` = 1892.33, `position_z` = -6.16, `MovementType` = 1, `spawndist` = 2.5 WHERE `guid` = 166693;
UPDATE `creature` SET `position_x` = -7367.00, `position_y` = 1210.33, `position_z` = 2.01, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166727;
UPDATE `creature` SET `position_x` = -7407.00, `position_y` = 1360.33, `position_z` = 4.34, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 166743;
UPDATE `creature` SET `position_x` = -7397.00, `position_y` = 1192.33, `position_z` = -75.12, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166745;
UPDATE `creature` SET `position_x` = -7529.00, `position_y` = 1578.33, `position_z` = -14.19, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166748;
UPDATE `creature` SET `position_x` = -7413.00, `position_y` = 1780.33, `position_z` = -96.03, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166764;
UPDATE `creature` SET `position_x` = -7379.00, `position_y` = 1748.33, `position_z` = -34.64, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 166779;
UPDATE `creature` SET `position_x` = -7481.00, `position_y` = 1928.33, `position_z` = -52.58, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166796;
UPDATE `creature` SET `position_x` = -7437.00, `position_y` = 1300.33, `position_z` = -85.17, `MovementType` = 1, `spawndist` = 2.5 WHERE `guid` = 166797;
UPDATE `creature` SET `position_x` = -7257.00, `position_y` = 1752.33, `position_z` = -49.31, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166805;
UPDATE `creature` SET `position_x` = -7603.54, `position_y` = 1851.39, `position_z` = 116.18, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 166813;
UPDATE `creature` SET `position_x` = -7533.00, `position_y` = 1336.33, `position_z` = 4.26, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166883;
UPDATE `creature` SET `position_x` = -7375.00, `position_y` = 1652.33, `position_z` = -81.89, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 166893;
UPDATE `creature` SET `position_x` = -7359.00, `position_y` = 1778.33, `position_z` = -83.61, `MovementType` = 1, `spawndist` = 5.0 WHERE `guid` = 166899;
UPDATE `creature` SET `position_x` = -7551.00, `position_y` = 1590.33, `position_z` = -62.58, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 166919;
UPDATE `creature` SET `position_x` = -7307.00, `position_y` = 1796.33, `position_z` = -25.29, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 166921;
UPDATE `creature` SET `position_x` = -7347.00, `position_y` = 1130.33, `position_z` = 3.24, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 166944;
UPDATE `creature` SET `position_x` = -7504.81, `position_y` = 1797.54, `position_z` = 11.16, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 166992;
UPDATE `creature` SET `position_x` = -7533.00, `position_y` = 1944.33, `position_z` = -49.75, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 166999;
UPDATE `creature` SET `position_x` = -7535.00, `position_y` = 1862.33, `position_z` = -55.60, `MovementType` = 1, `spawndist` = 6.0 WHERE `guid` = 167008;
UPDATE `creature` SET `position_x` = -7465.00, `position_y` = 1316.33, `position_z` = -79.82, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 167010;
UPDATE `creature` SET `position_x` = -7329.00, `position_y` = 1654.33, `position_z` = -72.65, `MovementType` = 1, `spawndist` = 3.0 WHERE `guid` = 167017;
UPDATE `creature` SET `position_x` = -7565.00, `position_y` = 1564.33, `position_z` = -64.86, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 167045;
UPDATE `creature` SET `position_x` = -7395.00, `position_y` = 1370.33, `position_z` = -76.31, `MovementType` = 2, `spawndist` = 0 WHERE `guid` = 167083;
UPDATE `creature` SET `position_x` = -7413.00, `position_y` = 1682.33, `position_z` = -87.97, `MovementType` = 1, `spawndist` = 4.0 WHERE `guid` = 167091;
UPDATE `creature` SET `position_x` = -7345.00, `position_y` = 1784.33, `position_z` = -80.85, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 167098;
UPDATE `creature` SET `position_x` = -7417.00, `position_y` = 1324.33, `position_z` = 3.12, `MovementType` = 0, `spawndist` = 0.0 WHERE `guid` = 167102;
DELETE FROM `creature_movement_override` WHERE `SpawnId` IN (165068, 165069, 165070, 165071, 165072, 165073, 165074, 165075, 165076, 165077, 165078, 165079, 165080, 165081, 165082, 165083, 165084, 165085, 165086, 165087, 165088, 165089, 165090, 165091, 165092, 165093, 165094, 165095, 165096, 165097, 165098, 165099, 165100, 165101, 165102, 165103, 165104, 165105, 165106, 165107, 165108, 165109, 165110, 165111, 165112, 165113, 165114, 165115, 165116, 165117, 165118, 165119, 165120, 165121, 165122, 165123, 165124, 165125, 165126, 165127, 165128, 165129, 165130, 165131, 165132, 165133, 165134, 165135, 165136, 165137, 165138, 165139, 165140, 165141, 165142, 165143, 165144, 165145, 165146, 165147, 165148, 165149, 165150, 165151, 165152, 165153, 165154, 165155, 165156, 165157, 165158, 165159, 165160, 165161, 165162, 165163, 165164, 165165, 165166, 165167, 165168, 165169, 165170, 165171, 165172, 165173, 165174, 165916, 165923, 165925, 165946, 165964, 165967, 166004, 166022, 166032, 166126, 166133, 166145, 166155, 166160, 166163, 166177, 166185, 166243, 166270, 166277, 166280, 166282, 166328, 166345, 166352, 166363, 166368, 166378, 166387, 166413, 166446, 166472, 166477, 166479, 166480, 166489, 166534, 166544, 166552, 166564, 166593, 166594, 166599, 166667, 166669, 166684, 166693, 166727, 166743, 166745, 166748, 166764, 166779, 166796, 166797, 166805, 166813, 166883, 166893, 166899, 166919, 166921, 166944, 166992, 166999, 167008, 167010, 167017, 167045, 167083, 167091, 167098, 167102);
DELETE FROM `creature_addon` WHERE `guid` IN (165068, 165069, 165070, 165071, 165072, 165073, 165074, 165075, 165076, 165077, 165078, 165079, 165080, 165081, 165082, 165083, 165084, 165085, 165086, 165087, 165088, 165089, 165090, 165091, 165092, 165093, 165094, 165095, 165096, 165097, 165098, 165099, 165100, 165101, 165102, 165103, 165104, 165105, 165106, 165107, 165108, 165109, 165110, 165111, 165112, 165113, 165114, 165115, 165116, 165117, 165118, 165119, 165120, 165121, 165122, 165123, 165124, 165125, 165126, 165127, 165128, 165129, 165130, 165131, 165132, 165133, 165134, 165135, 165136, 165137, 165138, 165139, 165140, 165141, 165142, 165143, 165144, 165145, 165146, 165147, 165148, 165149, 165150, 165151, 165152, 165153, 165154, 165155, 165156, 165157, 165158, 165159, 165160, 165161, 165162, 165163, 165164, 165165, 165166, 165167, 165168, 165169, 165170, 165171, 165172, 165173, 165174, 165916, 165923, 165925, 165946, 165964, 165967, 166004, 166022, 166032, 166126, 166133, 166145, 166155, 166160, 166163, 166177, 166185, 166243, 166270, 166277, 166280, 166282, 166328, 166345, 166352, 166363, 166368, 166378, 166387, 166413, 166446, 166472, 166477, 166479, 166480, 166489, 166534, 166544, 166552, 166564, 166593, 166594, 166599, 166667, 166669, 166684, 166693, 166727, 166743, 166745, 166748, 166764, 166779, 166796, 166797, 166805, 166813, 166883, 166893, 166899, 166919, 166921, 166944, 166992, 166999, 167008, 167010, 167017, 167045, 167083, 167091, 167098, 167102);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(165068, 0, 0, 50331648, 1, 0, ''),
(165069, 0, 0, 50331648, 1, 0, ''),
(165070, 1500165070, 0, 50331648, 1, 0, ''),
(165071, 0, 0, 50331648, 1, 0, ''),
(165072, 0, 0, 50331648, 1, 0, ''),
(165073, 1500165073, 0, 50331648, 1, 0, ''),
(165074, 1500165074, 0, 50331648, 1, 0, ''),
(165075, 1500165075, 0, 50331648, 1, 0, ''),
(165076, 1500165076, 0, 50331648, 1, 0, ''),
(165077, 0, 0, 50331648, 1, 0, ''),
(165078, 0, 0, 50331648, 1, 0, ''),
(165079, 0, 0, 50331648, 1, 0, ''),
(165080, 0, 0, 50331648, 1, 0, ''),
(165081, 0, 0, 50331648, 1, 0, ''),
(165082, 1500165082, 0, 50331648, 1, 0, ''),
(165083, 0, 0, 50331648, 1, 0, ''),
(165084, 0, 0, 50331648, 1, 0, ''),
(165085, 1500165085, 0, 50331648, 1, 0, ''),
(165086, 0, 0, 50331648, 1, 0, ''),
(165087, 0, 0, 50331648, 1, 0, ''),
(165088, 0, 0, 50331648, 1, 0, ''),
(165089, 1500165089, 0, 50331648, 1, 0, ''),
(165090, 0, 0, 50331648, 1, 0, ''),
(165091, 0, 0, 50331648, 1, 0, ''),
(165092, 0, 0, 50331648, 1, 0, ''),
(165093, 0, 0, 50331648, 1, 0, ''),
(165094, 0, 0, 50331648, 1, 0, ''),
(165095, 0, 0, 50331648, 1, 0, ''),
(165096, 0, 0, 50331648, 1, 0, ''),
(165097, 0, 0, 50331648, 1, 0, ''),
(165098, 0, 0, 50331648, 1, 0, ''),
(165099, 0, 0, 50331648, 1, 0, ''),
(165100, 0, 0, 50331648, 1, 0, ''),
(165101, 0, 0, 50331648, 1, 0, ''),
(165102, 1500165102, 0, 50331648, 1, 0, ''),
(165103, 0, 0, 50331648, 1, 0, ''),
(165104, 1500165104, 0, 50331648, 1, 0, ''),
(165105, 0, 0, 50331648, 1, 0, ''),
(165106, 0, 0, 50331648, 1, 0, ''),
(165107, 1500165107, 0, 50331648, 1, 0, ''),
(165108, 0, 0, 50331648, 1, 0, ''),
(165109, 0, 0, 50331648, 1, 0, ''),
(165110, 0, 0, 50331648, 1, 0, ''),
(165111, 0, 0, 50331648, 1, 0, ''),
(165112, 0, 0, 50331648, 1, 0, ''),
(165113, 0, 0, 50331648, 1, 0, ''),
(165114, 0, 0, 50331648, 1, 0, ''),
(165115, 0, 0, 50331648, 1, 0, ''),
(165116, 1500165116, 0, 50331648, 1, 0, ''),
(165117, 1500165117, 0, 50331648, 1, 0, ''),
(165118, 0, 0, 50331648, 1, 0, ''),
(165119, 0, 0, 50331648, 1, 0, ''),
(165120, 1500165120, 0, 50331648, 1, 0, ''),
(165121, 0, 0, 50331648, 1, 0, ''),
(165122, 0, 0, 50331648, 1, 0, ''),
(165123, 1500165123, 0, 50331648, 1, 0, ''),
(165124, 0, 0, 50331648, 1, 0, ''),
(165125, 0, 0, 50331648, 1, 0, ''),
(165126, 0, 0, 50331648, 1, 0, ''),
(165127, 0, 0, 50331648, 1, 0, ''),
(165128, 0, 0, 50331648, 1, 0, ''),
(165129, 0, 0, 50331648, 1, 0, ''),
(165130, 1500165130, 0, 50331648, 1, 0, ''),
(165131, 0, 0, 50331648, 1, 0, ''),
(165132, 1500165132, 0, 50331648, 1, 0, ''),
(165133, 0, 0, 50331648, 1, 0, ''),
(165134, 0, 0, 50331648, 1, 0, ''),
(165135, 1500165135, 0, 50331648, 1, 0, ''),
(165136, 0, 0, 50331648, 1, 0, ''),
(165137, 0, 0, 50331648, 1, 0, ''),
(165138, 0, 0, 50331648, 1, 0, ''),
(165139, 0, 0, 50331648, 1, 0, ''),
(165140, 0, 0, 50331648, 1, 0, ''),
(165141, 1500165141, 0, 50331648, 1, 0, ''),
(165142, 0, 0, 50331648, 1, 0, ''),
(165143, 0, 0, 50331648, 1, 0, ''),
(165144, 0, 0, 50331648, 1, 0, ''),
(165145, 0, 0, 50331648, 1, 0, ''),
(165146, 0, 0, 50331648, 1, 0, ''),
(165147, 0, 0, 50331648, 1, 0, ''),
(165148, 0, 0, 50331648, 1, 0, ''),
(165149, 0, 0, 50331648, 1, 0, ''),
(165150, 1500165150, 0, 50331648, 1, 0, ''),
(165151, 1500165151, 0, 50331648, 1, 0, ''),
(165152, 0, 0, 50331648, 1, 0, ''),
(165153, 0, 0, 50331648, 1, 0, ''),
(165154, 1500165154, 0, 50331648, 1, 0, ''),
(165155, 0, 0, 50331648, 1, 0, ''),
(165156, 0, 0, 50331648, 1, 0, ''),
(165157, 0, 0, 50331648, 1, 0, ''),
(165158, 0, 0, 50331648, 1, 0, ''),
(165159, 0, 0, 50331648, 1, 0, ''),
(165160, 0, 0, 50331648, 1, 0, ''),
(165161, 0, 0, 50331648, 1, 0, ''),
(165162, 0, 0, 50331648, 1, 0, ''),
(165163, 0, 0, 50331648, 1, 0, ''),
(165164, 0, 0, 50331648, 1, 0, ''),
(165165, 0, 0, 50331648, 1, 0, ''),
(165166, 0, 0, 50331648, 1, 0, ''),
(165167, 1500165167, 0, 50331648, 1, 0, ''),
(165168, 1500165168, 0, 50331648, 1, 0, ''),
(165169, 0, 0, 50331648, 1, 0, ''),
(165170, 0, 0, 50331648, 1, 0, ''),
(165171, 0, 0, 50331648, 1, 0, ''),
(165172, 0, 0, 50331648, 1, 0, ''),
(165173, 0, 0, 50331648, 1, 0, ''),
(165174, 0, 0, 50331648, 1, 0, ''),
(165916, 1500165916, 0, 50331648, 1, 0, ''),
(165923, 0, 0, 50331648, 1, 0, ''),
(165925, 0, 0, 50331648, 1, 0, ''),
(165946, 1500165946, 0, 50331648, 1, 0, ''),
(165964, 0, 0, 50331648, 1, 0, ''),
(165967, 0, 0, 50331648, 1, 0, ''),
(166004, 0, 0, 50331648, 1, 0, ''),
(166022, 0, 0, 50331648, 1, 0, ''),
(166032, 1500166032, 0, 50331648, 1, 0, ''),
(166126, 0, 0, 50331648, 1, 0, ''),
(166133, 0, 0, 50331648, 1, 0, ''),
(166145, 0, 0, 50331648, 1, 0, ''),
(166155, 0, 0, 50331648, 1, 0, ''),
(166160, 0, 0, 50331648, 1, 0, ''),
(166163, 0, 0, 50331648, 1, 0, ''),
(166177, 0, 0, 50331648, 1, 0, ''),
(166185, 0, 0, 50331648, 1, 0, ''),
(166243, 0, 0, 50331648, 1, 0, ''),
(166270, 0, 0, 50331648, 1, 0, ''),
(166277, 1500166277, 0, 50331648, 1, 0, ''),
(166280, 0, 0, 50331648, 1, 0, ''),
(166282, 0, 0, 50331648, 1, 0, ''),
(166328, 1500166328, 0, 50331648, 1, 0, ''),
(166345, 1500166345, 0, 50331648, 1, 0, ''),
(166352, 1500166352, 0, 50331648, 1, 0, ''),
(166363, 0, 0, 50331648, 1, 0, ''),
(166368, 0, 0, 50331648, 1, 0, ''),
(166378, 0, 0, 50331648, 1, 0, ''),
(166387, 0, 0, 50331648, 1, 0, ''),
(166413, 0, 0, 50331648, 1, 0, ''),
(166446, 0, 0, 50331648, 1, 0, ''),
(166472, 0, 0, 50331648, 1, 0, ''),
(166477, 0, 0, 50331648, 1, 0, ''),
(166479, 0, 0, 50331648, 1, 0, ''),
(166480, 1500166480, 0, 50331648, 1, 0, ''),
(166489, 0, 0, 50331648, 1, 0, ''),
(166534, 0, 0, 50331648, 1, 0, ''),
(166544, 0, 0, 50331648, 1, 0, ''),
(166552, 1500166552, 0, 50331648, 1, 0, ''),
(166564, 0, 0, 50331648, 1, 0, ''),
(166593, 0, 0, 50331648, 1, 0, ''),
(166594, 0, 0, 50331648, 1, 0, ''),
(166599, 0, 0, 50331648, 1, 0, ''),
(166667, 1500166667, 0, 50331648, 1, 0, ''),
(166669, 0, 0, 50331648, 1, 0, ''),
(166684, 0, 0, 50331648, 1, 0, ''),
(166693, 0, 0, 50331648, 1, 0, ''),
(166727, 0, 0, 50331648, 1, 0, ''),
(166743, 0, 0, 50331648, 1, 0, ''),
(166745, 0, 0, 50331648, 1, 0, ''),
(166748, 0, 0, 50331648, 1, 0, ''),
(166764, 0, 0, 50331648, 1, 0, ''),
(166779, 0, 0, 50331648, 1, 0, ''),
(166796, 0, 0, 50331648, 1, 0, ''),
(166797, 0, 0, 50331648, 1, 0, ''),
(166805, 0, 0, 50331648, 1, 0, ''),
(166813, 1500166813, 0, 50331648, 1, 0, ''),
(166883, 0, 0, 50331648, 1, 0, ''),
(166893, 1500166893, 0, 50331648, 1, 0, ''),
(166899, 0, 0, 50331648, 1, 0, ''),
(166919, 1500166919, 0, 50331648, 1, 0, ''),
(166921, 0, 0, 50331648, 1, 0, ''),
(166944, 0, 0, 50331648, 1, 0, ''),
(166992, 1500166992, 0, 50331648, 1, 0, ''),
(166999, 1500166999, 0, 50331648, 1, 0, ''),
(167008, 0, 0, 50331648, 1, 0, ''),
(167010, 0, 0, 50331648, 1, 0, ''),
(167017, 0, 0, 50331648, 1, 0, ''),
(167045, 0, 0, 50331648, 1, 0, ''),
(167083, 1500167083, 0, 50331648, 1, 0, ''),
(167091, 0, 0, 50331648, 1, 0, ''),
(167098, 0, 0, 50331648, 1, 0, ''),
(167102, 0, 0, 50331648, 1, 0, '');
DELETE FROM `waypoint_data` WHERE `id` BETWEEN 1500165068 AND 1500167102;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `delay_chance`, `move_type`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES
(1500165070, 1, -7025.08, 1647.52, 17.10, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165070, 2, -7083.06, 1686.46, 16.69, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165070, 3, -7143.09, 1708.07, 8.70, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165070, 4, -7180.80, 1693.11, -6.99, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165070, 5, -7154.98, 1657.59, 6.25, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165070, 6, -7142.95, 1641.41, 8.55, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165070, 7, -7098.54, 1636.06, 19.83, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165070, 8, -7022.87, 1625.45, 11.24, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165073, 1, -7679.83, 1575.21, 13.61, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165073, 2, -7731.70, 1580.93, 22.43, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165073, 3, -7746.58, 1630.44, 30.30, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165073, 4, -7721.09, 1686.09, 37.54, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165073, 5, -7737.51, 1699.52, 33.61, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165073, 6, -7727.25, 1729.51, 36.14, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165073, 7, -7716.36, 1691.32, 29.08, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165073, 8, -7699.50, 1634.89, 17.07, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165074, 1, -7708.36, 1497.34, 23.12, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165074, 2, -7739.80, 1476.09, 15.23, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165074, 3, -7726.99, 1424.91, 12.59, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165074, 4, -7681.61, 1361.49, 14.72, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165074, 5, -7652.26, 1346.55, 11.21, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165074, 6, -7669.51, 1398.36, 15.48, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165074, 7, -7694.49, 1468.13, 19.89, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165075, 1, -7587.43, 1628.91, 21.61, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165075, 2, -7596.35, 1664.90, 23.62, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165075, 3, -7551.25, 1698.45, 29.08, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165075, 4, -7531.58, 1765.25, 29.40, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165075, 5, -7484.77, 1791.58, 6.42, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165075, 6, -7497.19, 1749.49, 14.52, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165075, 7, -7533.20, 1705.31, 20.15, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165075, 8, -7554.35, 1675.60, 21.69, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165076, 1, -7595.00, 1716.33, -30.48, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165076, 2, -7581.00, 1702.33, -29.54, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165076, 3, -7587.00, 1688.33, -28.94, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165076, 4, -7615.00, 1698.33, -29.47, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165076, 5, -7609.00, 1712.33, -31.52, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165076, 6, -7605.00, 1696.33, -26.94, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165082, 1, -7109.57, 1537.27, 12.18, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165082, 2, -7067.50, 1475.40, 28.55, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165082, 3, -7048.99, 1410.77, 26.23, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165082, 4, -7065.66, 1373.53, 15.70, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165082, 5, -7105.91, 1388.46, 14.10, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165082, 6, -7106.01, 1432.15, 29.85, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165082, 7, -7107.38, 1480.49, 15.32, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165085, 1, -7088.58, 1486.38, 21.46, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165085, 2, -7065.29, 1435.47, 19.58, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165085, 3, -7046.77, 1467.91, 25.42, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165085, 4, -7073.02, 1508.36, 30.23, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165085, 5, -7088.32, 1567.30, 21.27, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165085, 6, -7097.40, 1586.97, 15.22, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165085, 7, -7162.34, 1630.41, 22.54, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165085, 8, -7146.54, 1597.14, 23.18, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165085, 9, -7115.80, 1538.53, 20.93, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165089, 1, -7397.00, 1758.33, -86.02, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165089, 2, -7387.00, 1736.33, -83.02, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165089, 3, -7399.00, 1710.33, -84.86, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165089, 4, -7409.00, 1692.33, -87.48, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165089, 5, -7423.00, 1684.33, -84.72, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165089, 6, -7433.00, 1696.33, -77.99, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165089, 7, -7413.00, 1708.33, -84.68, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165089, 8, -7403.00, 1734.33, -86.59, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165089, 9, -7413.00, 1740.33, -81.89, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165089, 10, -7413.00, 1752.33, -79.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165102, 1, -7339.00, 1848.33, -81.06, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165102, 2, -7351.00, 1842.33, -78.25, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165102, 3, -7351.00, 1858.33, -76.59, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165102, 4, -7333.00, 1868.33, -77.76, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165102, 5, -7321.00, 1872.33, -80.23, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165104, 1, -7303.00, 1842.33, -79.87, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165104, 2, -7317.00, 1822.33, -81.69, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165104, 3, -7305.00, 1822.33, -81.93, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165104, 4, -7287.00, 1828.33, -78.33, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165104, 5, -7287.00, 1858.33, -83.36, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165104, 6, -7297.00, 1876.33, -82.18, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165104, 7, -7307.00, 1882.33, -76.62, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165104, 8, -7299.00, 1854.33, -79.45, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165107, 1, -7373.00, 1266.33, -77.52, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165107, 2, -7399.00, 1264.33, -82.48, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165107, 3, -7413.00, 1288.33, -80.43, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165107, 4, -7387.00, 1284.33, -79.88, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165107, 5, -7377.00, 1294.33, -76.41, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165107, 6, -7361.00, 1284.33, -79.95, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165116, 1, -7415.00, 1220.33, -70.25, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165116, 2, -7429.00, 1216.33, -70.33, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165116, 3, -7429.00, 1246.33, -80.88, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165116, 4, -7419.00, 1268.33, -81.67, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165116, 5, -7413.00, 1288.33, -80.43, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165116, 6, -7387.00, 1284.33, -79.88, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165116, 7, -7373.00, 1266.33, -77.52, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165116, 8, -7387.00, 1248.33, -80.31, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165116, 9, -7407.00, 1248.33, -79.90, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165116, 10, -7419.00, 1248.33, -77.41, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165117, 1, -7399.00, 1218.33, -72.74, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165117, 2, -7389.00, 1228.33, -75.72, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165117, 3, -7411.00, 1230.33, -71.26, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165117, 4, -7429.00, 1216.33, -70.33, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165117, 5, -7421.00, 1208.33, -69.85, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165117, 6, -7411.00, 1202.33, -69.28, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165120, 1, -7167.00, 1796.33, -72.74, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165120, 2, -7151.00, 1808.33, -72.23, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165120, 3, -7143.00, 1796.33, -73.08, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165120, 4, -7143.00, 1776.33, -74.88, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165120, 5, -7153.00, 1762.33, -72.24, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165120, 6, -7153.00, 1788.33, -72.54, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165123, 1, -7394.88, 1461.26, 28.64, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165123, 2, -7459.88, 1472.35, 25.71, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165123, 3, -7532.45, 1448.37, 17.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165123, 4, -7548.33, 1374.33, 21.08, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165123, 5, -7560.61, 1329.39, 24.41, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165123, 6, -7539.56, 1346.34, 24.66, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165123, 7, -7485.52, 1386.28, 24.30, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165123, 8, -7444.60, 1419.71, 25.47, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165130, 1, -7393.33, 1839.88, -12.64, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165130, 2, -7418.20, 1823.75, -12.00, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165130, 3, -7448.73, 1801.29, -9.76, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165130, 4, -7409.82, 1752.62, -12.70, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165130, 5, -7372.82, 1731.43, -14.51, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165130, 6, -7345.46, 1755.59, -1.40, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165130, 7, -7398.17, 1809.71, -9.16, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165132, 1, -7166.63, 1814.49, 41.12, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165132, 2, -7144.89, 1817.43, 35.33, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165132, 3, -7184.21, 1839.66, 54.52, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165132, 4, -7230.55, 1884.33, 74.27, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165132, 5, -7267.99, 1914.25, 56.60, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165132, 6, -7293.64, 1899.93, 44.45, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165132, 7, -7270.77, 1860.40, 25.62, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165132, 8, -7232.57, 1838.40, 17.16, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165135, 1, -7386.43, 1397.74, 14.40, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165135, 2, -7397.85, 1350.96, 15.50, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165135, 3, -7411.82, 1291.67, 21.10, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165135, 4, -7420.83, 1242.85, 24.61, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165135, 5, -7445.37, 1243.35, 23.43, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165135, 6, -7461.70, 1255.61, 27.61, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165135, 7, -7465.48, 1330.67, 29.46, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165135, 8, -7450.51, 1341.47, 22.53, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165135, 9, -7416.31, 1392.72, 18.62, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165141, 1, -7410.28, 1650.92, -7.22, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165141, 2, -7440.47, 1638.71, -16.56, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165141, 3, -7435.48, 1573.58, -12.52, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165141, 4, -7372.49, 1547.55, 12.46, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165141, 5, -7340.55, 1580.33, 14.77, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165141, 6, -7373.82, 1583.65, 0.69, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165141, 7, -7375.40, 1618.32, -10.53, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165141, 8, -7359.53, 1629.10, -13.06, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165150, 1, -7158.34, 1416.00, 15.52, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165150, 2, -7172.09, 1391.36, 20.47, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165150, 3, -7196.23, 1334.98, 20.97, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165150, 4, -7186.37, 1292.35, 26.49, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165150, 5, -7157.78, 1295.88, 13.82, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165150, 6, -7155.56, 1368.75, 30.40, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165151, 1, -7195.32, 1307.82, 27.57, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165151, 2, -7207.55, 1254.69, 29.94, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165151, 3, -7250.94, 1233.86, 14.25, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165151, 4, -7297.72, 1241.55, 13.39, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165151, 5, -7367.83, 1278.97, 10.95, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165151, 6, -7337.28, 1285.37, 12.70, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165151, 7, -7268.33, 1301.66, 23.29, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165154, 1, -7507.00, 1904.33, -53.08, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165154, 2, -7497.00, 1924.33, -50.06, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165154, 3, -7489.00, 1912.33, -50.15, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165154, 4, -7499.00, 1896.33, -52.87, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165154, 5, -7497.00, 1882.33, -54.24, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165154, 6, -7497.00, 1870.33, -54.11, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165154, 7, -7517.00, 1880.33, -51.31, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165154, 8, -7533.00, 1896.33, -50.88, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165167, 1, -7549.00, 1914.33, -57.50, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165167, 2, -7557.00, 1938.33, -52.60, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165167, 3, -7547.00, 1962.33, -54.07, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165167, 4, -7539.00, 1954.33, -49.84, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165167, 5, -7539.00, 1936.33, -53.50, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165168, 1, -7311.00, 1594.33, -69.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165168, 2, -7319.00, 1602.33, -73.96, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165168, 3, -7295.00, 1618.33, -70.61, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165168, 4, -7289.00, 1634.33, -69.18, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165168, 5, -7301.00, 1652.33, -63.36, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165168, 6, -7323.00, 1652.33, -70.01, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165168, 7, -7337.00, 1640.33, -70.68, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165168, 8, -7331.00, 1620.33, -71.33, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165168, 9, -7345.00, 1604.33, -70.37, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165168, 10, -7323.00, 1592.33, -71.67, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165916, 1, -7648.95, 1397.74, 18.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165916, 2, -7655.47, 1378.03, 15.18, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165916, 3, -7686.44, 1365.04, 16.89, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165916, 4, -7691.74, 1344.42, 23.08, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165916, 5, -7651.81, 1306.11, 22.62, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165916, 6, -7589.95, 1285.05, 26.09, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165916, 7, -7512.38, 1282.63, 22.96, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165916, 8, -7560.61, 1329.39, 24.41, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165916, 9, -7603.45, 1370.81, 16.70, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165946, 1, -7300.78, 1230.21, 24.31, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165946, 2, -7319.83, 1212.59, 23.06, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165946, 3, -7373.03, 1203.15, 24.70, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165946, 4, -7434.47, 1190.53, 13.42, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165946, 5, -7468.31, 1205.66, 13.56, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165946, 6, -7438.90, 1209.20, 13.84, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500165946, 7, -7378.33, 1219.22, 17.83, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166032, 1, -7347.00, 1710.33, -82.80, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166032, 2, -7337.00, 1720.33, -87.03, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166032, 3, -7321.00, 1722.33, -83.16, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166032, 4, -7337.00, 1736.33, -82.26, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166032, 5, -7355.00, 1718.33, -85.68, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166032, 6, -7369.00, 1706.33, -84.13, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166032, 7, -7393.00, 1722.33, -86.07, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166032, 8, -7413.00, 1708.33, -84.68, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166032, 9, -7395.00, 1700.33, -84.72, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166032, 10, -7373.00, 1696.33, -79.72, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166277, 1, -7329.00, 1632.33, -70.82, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166277, 2, -7337.00, 1640.33, -70.68, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166277, 3, -7321.00, 1664.33, -70.63, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166277, 4, -7305.00, 1666.33, -63.19, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166277, 5, -7301.00, 1652.33, -63.36, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166277, 6, -7323.00, 1652.33, -70.01, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166328, 1, -7266.44, 1506.53, 21.42, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166328, 2, -7215.44, 1518.19, 24.69, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166328, 3, -7191.13, 1515.03, 21.38, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166328, 4, -7189.34, 1436.78, 20.04, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166328, 5, -7248.28, 1410.69, 12.02, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166328, 6, -7273.73, 1338.59, 21.24, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166328, 7, -7290.07, 1407.52, 14.96, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166328, 8, -7294.58, 1440.33, 14.54, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166328, 9, -7290.69, 1470.47, 18.04, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166345, 1, -7468.45, 1278.20, 20.57, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166345, 2, -7450.69, 1208.33, 22.97, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166345, 3, -7470.29, 1188.11, 28.84, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166345, 4, -7507.56, 1211.51, 22.98, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166345, 5, -7531.66, 1217.71, 26.48, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166345, 6, -7568.41, 1266.81, 27.40, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166345, 7, -7600.77, 1320.42, 14.82, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166345, 8, -7548.52, 1299.05, 23.83, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166345, 9, -7500.31, 1280.52, 22.47, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166352, 1, -7540.75, 1544.67, 15.55, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166352, 2, -7470.31, 1511.59, 19.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166352, 3, -7502.37, 1504.60, 26.95, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166352, 4, -7545.23, 1487.43, 26.73, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166352, 5, -7606.33, 1467.61, 23.75, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166352, 6, -7627.41, 1459.55, 23.90, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166352, 7, -7628.27, 1491.35, 14.77, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166352, 8, -7596.62, 1515.09, 12.67, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166480, 1, -7533.00, 1364.33, -84.84, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166480, 2, -7549.00, 1368.33, -83.44, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166480, 3, -7565.00, 1374.33, -79.40, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166480, 4, -7565.00, 1392.33, -83.98, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166480, 5, -7537.00, 1396.33, -86.39, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166480, 6, -7525.00, 1394.33, -87.31, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166480, 7, -7533.00, 1386.33, -88.19, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166552, 1, -7594.95, 1570.67, 26.33, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166552, 2, -7636.94, 1573.38, 19.91, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166552, 3, -7676.30, 1556.51, 32.04, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166552, 4, -7705.95, 1534.43, 17.24, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166552, 5, -7723.81, 1504.51, 12.43, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166552, 6, -7729.94, 1467.85, 27.15, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166552, 7, -7685.75, 1489.27, 29.76, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166552, 8, -7638.61, 1512.87, 28.75, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166552, 9, -7580.68, 1547.96, 17.71, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166667, 1, -7448.43, 1420.13, 19.95, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166667, 2, -7501.96, 1394.83, 20.93, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166667, 3, -7472.55, 1387.32, 11.26, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166667, 4, -7431.50, 1389.48, 20.52, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166667, 5, -7376.85, 1407.13, 16.44, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166667, 6, -7352.19, 1426.49, 14.65, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166667, 7, -7329.45, 1490.66, 6.16, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166667, 8, -7395.13, 1455.07, 17.84, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166813, 1, -7603.54, 1851.39, 116.18, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166813, 2, -7598.77, 1797.43, 139.79, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166813, 3, -7644.24, 1812.34, 134.76, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166813, 4, -7655.73, 1754.52, 123.08, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166813, 5, -7686.24, 1767.05, 114.13, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166813, 6, -7676.94, 1799.61, 101.49, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166813, 7, -7645.91, 1830.72, 118.34, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166893, 1, -7375.00, 1652.33, -81.89, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166893, 2, -7379.00, 1666.33, -84.67, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166893, 3, -7389.00, 1692.33, -85.63, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166893, 4, -7369.00, 1686.33, -83.23, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166893, 5, -7361.00, 1678.33, -80.95, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166893, 6, -7363.00, 1658.33, -86.72, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166919, 1, -7551.00, 1590.33, -62.58, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166919, 2, -7557.00, 1600.33, -63.01, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166919, 3, -7539.00, 1592.33, -64.17, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166919, 4, -7543.00, 1578.33, -62.27, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166919, 5, -7549.00, 1566.33, -60.06, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166919, 6, -7563.00, 1572.33, -60.12, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166992, 1, -7504.81, 1797.54, 11.16, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166992, 2, -7526.78, 1809.46, 17.26, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166992, 3, -7532.41, 1877.92, 44.28, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166992, 4, -7535.67, 1904.93, 39.39, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166992, 5, -7488.14, 1913.95, 42.48, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166992, 6, -7501.79, 1849.18, 19.57, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166999, 1, -7533.00, 1944.33, -49.75, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166999, 2, -7535.00, 1926.33, -52.64, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166999, 3, -7519.00, 1910.33, -50.55, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166999, 4, -7513.00, 1896.33, -49.29, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166999, 5, -7497.00, 1882.33, -54.24, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166999, 6, -7517.00, 1880.33, -51.31, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166999, 7, -7533.00, 1896.33, -50.88, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166999, 8, -7549.00, 1914.33, -57.50, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166999, 9, -7557.00, 1938.33, -52.60, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500166999, 10, -7547.00, 1962.33, -54.07, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500167083, 1, -7395.00, 1370.33, -76.31, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500167083, 2, -7417.00, 1372.33, -77.87, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500167083, 3, -7401.00, 1380.33, -82.09, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500167083, 4, -7385.00, 1392.33, -81.13, 0, 0, 0, 0, 0, 0, 100, 0, 0),
(1500167083, 5, -7375.00, 1386.33, -80.77, 0, 0, 0, 0, 0, 0, 100, 0, 0);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 11727 AND `source_type` = 0 AND `id` IN (1, 2);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(11727, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hive''Zora Wasp - On Aggro - Land');

DELETE FROM `creature` WHERE `guid` IN (164634, 164636, 164643, 164645, 164646, 164651, 164652, 164653, 164663, 164715, 164716, 164727, 164729, 164730, 164731, 164732, 164734, 164735, 164736, 164737, 164738, 164739, 164741, 164742, 164743, 164744, 164745, 164747, 164748, 164751, 164753, 164754, 164755, 164756, 164759, 164760, 164762, 164763, 164764, 164767, 164770, 164772, 164773, 164774, 164775, 164776, 164777, 164779, 164780, 164782, 164783, 164784, 164786, 164787, 164788, 164789, 164790, 164791, 164794, 164795, 164796, 164797, 164798, 164799, 164800, 164802, 164803, 164804, 164805, 164806, 164807, 164808, 164809, 164810, 164812, 164813, 164815, 164816, 164820, 164861, 164862, 164863, 164864, 164865, 164866, 164867, 164870, 164872, 164874, 164875, 164876, 164877, 164879, 164880, 164881, 164882, 164883, 164885, 164886, 164887, 164888, 164889, 164890, 164891, 164892, 164893, 164894, 164895, 164897, 164898, 164899, 164900, 164903, 164904, 164913, 164914, 164915, 164917, 164918, 164919, 164920, 164921, 164922, 164923, 164924, 164925, 164926, 164927, 164928, 164929, 164962, 164963, 164964, 164965, 164969, 164970, 164971, 164973, 164974, 164975, 164978, 164982, 164983, 164984, 164985, 164986, 164987, 164988, 164989, 164990, 164991, 164992, 164993, 164994, 164995, 164996, 164997, 164998, 164999, 165000, 165002, 165003, 165005, 165006, 165007, 165009, 165010, 165011, 165012, 165013, 165014, 165015, 165016, 165017, 165018, 165019, 165020, 165021, 165022, 165023, 165025, 165027, 165028, 165029, 165030, 165031, 165032, 165034, 165037, 165039, 165040, 165042, 165044, 165045, 165046, 165047, 165048, 165049, 165052, 165053, 165055, 165057, 165058, 165077, 165078, 165081, 165083, 165086, 165088, 165090, 165091, 165092, 165093, 165094, 165095, 165096, 165097, 165098, 165099, 165101, 165103, 165105, 165106, 165108, 165109, 165110, 165111, 165112, 165114, 165115, 165118, 165119, 165121, 165122, 165124, 165126, 165128, 165129, 165133, 165134, 165136, 165137, 165139, 165140, 165142, 165143, 165144, 165145, 165146, 165147, 165148, 165149, 165152, 165153, 165155, 165156, 165158, 165160, 165161, 165162, 165164, 165165, 165166, 165169, 165170, 165171, 165172, 165175, 165176, 165177, 165180, 165181, 165182, 165183, 165184, 165186, 165187, 165188, 165189, 165190, 165192, 165193, 165194, 165196, 165197, 165198, 165199, 165201, 165203, 165204, 165205, 165206, 165207, 165208, 165209, 165210, 165215, 165217, 165218, 165219, 165220, 165221, 165222, 165223, 165224, 165226, 165227, 165228, 165229, 165230, 165231, 165236, 165237, 165238, 165239, 165240, 165241, 165243, 165248, 165251, 165262, 165263, 165265, 165268, 165269, 165270, 165271, 165272, 165274, 165275, 165276, 165277, 165278, 165279, 165280, 165281, 165282, 165283, 165284, 165286, 165287, 165291, 165292, 165293, 165295, 165296, 165297, 165298, 165299, 165300, 165301, 165302, 165303, 165304, 165305, 165306, 165307, 165308, 165309, 165310, 165311, 165312, 165313, 165314, 165315, 165316, 165317, 165318, 165321, 165322, 165323, 165328, 165331, 165332, 165333, 165334, 165335, 165336, 165337, 165338, 165339, 165340, 165342, 165343, 165344, 165345, 165350, 165351, 165352, 165353, 165355, 165357, 165358, 165359, 165360, 165361, 165362, 165363, 165364, 165365, 165366, 165367, 165369, 165370, 165371, 165372, 165373, 165374, 165378, 165380, 165381, 165384, 165385, 165386, 165388, 165390, 165391, 165392, 165393, 165394, 165396, 165397, 165398, 165399, 165400, 165401, 165402, 165403, 165404, 165405, 165406, 165407, 165408, 165409, 165410, 165411, 165412, 165413, 165414, 165415, 165416, 165417, 165418, 165419, 165420, 165421, 165422, 165423, 165424, 165425, 165426, 165427, 165429, 165430, 165435, 165436, 165437, 165438, 165439, 165440, 165441, 165442, 165443, 165444, 165445, 165447, 165448, 165450, 165451, 165452, 165454, 165455, 165457, 165458, 165459, 165460, 165461, 165462, 165463, 165465, 165466, 165467, 165468, 165469, 165470, 165472, 165473, 165474, 165475, 165477, 165478, 165479, 165480, 165481, 165482, 165483, 165484, 165486, 165487, 165488, 165489, 165490, 165491, 165493, 165494, 165495, 165497, 165498, 165499, 165500, 165501, 165502, 165503, 165504, 165505, 165506, 165508, 165509, 165510, 165511, 165512, 165515, 165518, 165519, 165520, 165521, 165522, 165523, 165524, 165525, 165526, 165527, 165528, 165529, 165530, 165531, 165532, 165533, 165535, 165536, 165537, 165538, 165540, 165541, 165542, 165544, 165545, 165546, 165547, 165548, 165549, 165550, 165551, 165552, 165553, 165554, 165555, 165556, 165557, 165559, 165560, 165561, 165562, 165563, 165565, 165566, 165567, 165568, 165569, 165570, 165571, 165573, 165574, 165576, 165578, 165579, 165580, 165584, 165585, 165586, 165587, 165588, 165589, 165590, 165591, 165592, 165593, 165594, 165595, 165596, 165598, 165599, 165600, 165601, 165602, 165604, 165605, 165607, 165608, 165610, 165611, 165612, 165614, 165615, 165616, 165617, 165618, 165619, 165620, 165622, 165623, 165624, 165625, 165626, 165627, 165628, 165630, 165631, 165632, 165633, 165634, 165635, 165637, 165638, 165639, 165640, 165641, 165642, 165643, 165644, 165646, 165647, 165648, 165649, 165650, 165651, 165652, 165653, 165654, 165655, 165656, 165657, 165658, 165660, 165661, 165662, 165663, 165665, 165666, 165667, 165668, 165669, 165671, 165672, 165673, 165674, 165675, 165676, 165677, 165678, 165679, 165681, 165682, 165683, 165684, 165685, 165687, 165688, 165689, 165690, 165691, 165693, 165694, 165695, 165696, 165697, 165698, 165699, 165700, 165702, 165703, 165704, 165705, 165706, 165707, 165708, 165709, 165710, 165711, 165713, 165714, 165716, 165717, 165718, 165719, 165720, 165721, 165722, 165723, 165725, 165726, 165727, 165728, 165729, 165730, 165731, 165732, 165733, 165734, 165735, 165736, 165737, 165738, 165740, 165742, 165743, 165745, 165746, 165747, 165748, 165749, 165750, 165751, 165752, 165753, 165755, 165756, 165757, 165758, 165759, 165760, 165761, 165762, 165763, 165764, 165765, 165768, 165769, 165772, 165773, 165774, 165775, 165776, 165777, 165778, 165780, 165781, 165782, 165783, 165784, 165785, 165786, 165788, 165789, 165792, 165793, 165795, 165796, 165797, 165799, 165801, 165802, 165803, 165804, 165805, 165806, 165807, 165808, 165809, 165812, 165813, 165814, 165815, 165816, 165817, 165818, 165821, 165822, 165825, 165826, 165827, 165828, 165829, 165830, 165832, 165835, 165836, 165837, 165838, 165839, 165840, 165841, 165842, 165843, 165844, 165845, 165846, 165847, 165848, 165849, 165850, 165851, 165852, 165853, 165854, 165856, 165859, 165860, 165861, 165864, 165865, 165866, 165867, 165868, 165869, 165870, 165871, 165872, 165875, 165876, 165877, 165878, 165879, 165880, 165881, 165882, 165883, 165884, 165885, 165887, 165917, 165918, 165923, 165931, 165935, 165941, 165958, 165964, 165966, 165967, 165972, 165973, 165976, 165991, 166002, 166004, 166010, 166014, 166015, 166023, 166035, 166036, 166043, 166045, 166050, 166054, 166056, 166061, 166062, 166075, 166077, 166080, 166098, 166102, 166109, 166110, 166116, 166123, 166132, 166133, 166135, 166143, 166144, 166150, 166155, 166160, 166163, 166165, 166183, 166185, 166191, 166192, 166194, 166215, 166216, 166229, 166230, 166238, 166244, 166245, 166260, 166270, 166271, 166273, 166280, 166282, 166285, 166301, 166312, 166317, 166333, 166338, 166339, 166340, 166368, 166372, 166406, 166430, 166433, 166443, 166446, 166447, 166449, 166460, 166469, 166477, 166478, 166481, 166482, 166489, 166490, 166496, 166505, 166512, 166514, 166520, 166534, 166544, 166550, 166555, 166557, 166565, 166566, 166570, 166581, 166584, 166593, 166594, 166608, 166628, 166629, 166633, 166636, 166637, 166640, 166641, 166642, 166644, 166663, 166666, 166671, 166681, 166696, 166699, 166723, 166727, 166728, 166743, 166745, 166748, 166764, 166774, 166778, 166779, 166797, 166807, 166808, 166818, 166819, 166822, 166829, 166845, 166868, 166883, 166894, 166899, 166901, 166928, 166947, 166953, 167017, 167045, 167091, 169932, 169937, 169942, 169953, 169959, 169968, 169970, 169972, 169976, 169983, 169984, 169989, 169992, 169994, 170007, 170008, 170013, 170014, 170017, 170019, 170024, 170036, 170039, 170046, 170068, 170077, 170078, 170079, 170255, 170256);
DELETE FROM `creature_addon` WHERE `guid` IN (164634, 164636, 164643, 164645, 164646, 164651, 164652, 164653, 164663, 164715, 164716, 164727, 164729, 164730, 164731, 164732, 164734, 164735, 164736, 164737, 164738, 164739, 164741, 164742, 164743, 164744, 164745, 164747, 164748, 164751, 164753, 164754, 164755, 164756, 164759, 164760, 164762, 164763, 164764, 164767, 164770, 164772, 164773, 164774, 164775, 164776, 164777, 164779, 164780, 164782, 164783, 164784, 164786, 164787, 164788, 164789, 164790, 164791, 164794, 164795, 164796, 164797, 164798, 164799, 164800, 164802, 164803, 164804, 164805, 164806, 164807, 164808, 164809, 164810, 164812, 164813, 164815, 164816, 164820, 164861, 164862, 164863, 164864, 164865, 164866, 164867, 164870, 164872, 164874, 164875, 164876, 164877, 164879, 164880, 164881, 164882, 164883, 164885, 164886, 164887, 164888, 164889, 164890, 164891, 164892, 164893, 164894, 164895, 164897, 164898, 164899, 164900, 164903, 164904, 164913, 164914, 164915, 164917, 164918, 164919, 164920, 164921, 164922, 164923, 164924, 164925, 164926, 164927, 164928, 164929, 164962, 164963, 164964, 164965, 164969, 164970, 164971, 164973, 164974, 164975, 164978, 164982, 164983, 164984, 164985, 164986, 164987, 164988, 164989, 164990, 164991, 164992, 164993, 164994, 164995, 164996, 164997, 164998, 164999, 165000, 165002, 165003, 165005, 165006, 165007, 165009, 165010, 165011, 165012, 165013, 165014, 165015, 165016, 165017, 165018, 165019, 165020, 165021, 165022, 165023, 165025, 165027, 165028, 165029, 165030, 165031, 165032, 165034, 165037, 165039, 165040, 165042, 165044, 165045, 165046, 165047, 165048, 165049, 165052, 165053, 165055, 165057, 165058, 165077, 165078, 165081, 165083, 165086, 165088, 165090, 165091, 165092, 165093, 165094, 165095, 165096, 165097, 165098, 165099, 165101, 165103, 165105, 165106, 165108, 165109, 165110, 165111, 165112, 165114, 165115, 165118, 165119, 165121, 165122, 165124, 165126, 165128, 165129, 165133, 165134, 165136, 165137, 165139, 165140, 165142, 165143, 165144, 165145, 165146, 165147, 165148, 165149, 165152, 165153, 165155, 165156, 165158, 165160, 165161, 165162, 165164, 165165, 165166, 165169, 165170, 165171, 165172, 165175, 165176, 165177, 165180, 165181, 165182, 165183, 165184, 165186, 165187, 165188, 165189, 165190, 165192, 165193, 165194, 165196, 165197, 165198, 165199, 165201, 165203, 165204, 165205, 165206, 165207, 165208, 165209, 165210, 165215, 165217, 165218, 165219, 165220, 165221, 165222, 165223, 165224, 165226, 165227, 165228, 165229, 165230, 165231, 165236, 165237, 165238, 165239, 165240, 165241, 165243, 165248, 165251, 165262, 165263, 165265, 165268, 165269, 165270, 165271, 165272, 165274, 165275, 165276, 165277, 165278, 165279, 165280, 165281, 165282, 165283, 165284, 165286, 165287, 165291, 165292, 165293, 165295, 165296, 165297, 165298, 165299, 165300, 165301, 165302, 165303, 165304, 165305, 165306, 165307, 165308, 165309, 165310, 165311, 165312, 165313, 165314, 165315, 165316, 165317, 165318, 165321, 165322, 165323, 165328, 165331, 165332, 165333, 165334, 165335, 165336, 165337, 165338, 165339, 165340, 165342, 165343, 165344, 165345, 165350, 165351, 165352, 165353, 165355, 165357, 165358, 165359, 165360, 165361, 165362, 165363, 165364, 165365, 165366, 165367, 165369, 165370, 165371, 165372, 165373, 165374, 165378, 165380, 165381, 165384, 165385, 165386, 165388, 165390, 165391, 165392, 165393, 165394, 165396, 165397, 165398, 165399, 165400, 165401, 165402, 165403, 165404, 165405, 165406, 165407, 165408, 165409, 165410, 165411, 165412, 165413, 165414, 165415, 165416, 165417, 165418, 165419, 165420, 165421, 165422, 165423, 165424, 165425, 165426, 165427, 165429, 165430, 165435, 165436, 165437, 165438, 165439, 165440, 165441, 165442, 165443, 165444, 165445, 165447, 165448, 165450, 165451, 165452, 165454, 165455, 165457, 165458, 165459, 165460, 165461, 165462, 165463, 165465, 165466, 165467, 165468, 165469, 165470, 165472, 165473, 165474, 165475, 165477, 165478, 165479, 165480, 165481, 165482, 165483, 165484, 165486, 165487, 165488, 165489, 165490, 165491, 165493, 165494, 165495, 165497, 165498, 165499, 165500, 165501, 165502, 165503, 165504, 165505, 165506, 165508, 165509, 165510, 165511, 165512, 165515, 165518, 165519, 165520, 165521, 165522, 165523, 165524, 165525, 165526, 165527, 165528, 165529, 165530, 165531, 165532, 165533, 165535, 165536, 165537, 165538, 165540, 165541, 165542, 165544, 165545, 165546, 165547, 165548, 165549, 165550, 165551, 165552, 165553, 165554, 165555, 165556, 165557, 165559, 165560, 165561, 165562, 165563, 165565, 165566, 165567, 165568, 165569, 165570, 165571, 165573, 165574, 165576, 165578, 165579, 165580, 165584, 165585, 165586, 165587, 165588, 165589, 165590, 165591, 165592, 165593, 165594, 165595, 165596, 165598, 165599, 165600, 165601, 165602, 165604, 165605, 165607, 165608, 165610, 165611, 165612, 165614, 165615, 165616, 165617, 165618, 165619, 165620, 165622, 165623, 165624, 165625, 165626, 165627, 165628, 165630, 165631, 165632, 165633, 165634, 165635, 165637, 165638, 165639, 165640, 165641, 165642, 165643, 165644, 165646, 165647, 165648, 165649, 165650, 165651, 165652, 165653, 165654, 165655, 165656, 165657, 165658, 165660, 165661, 165662, 165663, 165665, 165666, 165667, 165668, 165669, 165671, 165672, 165673, 165674, 165675, 165676, 165677, 165678, 165679, 165681, 165682, 165683, 165684, 165685, 165687, 165688, 165689, 165690, 165691, 165693, 165694, 165695, 165696, 165697, 165698, 165699, 165700, 165702, 165703, 165704, 165705, 165706, 165707, 165708, 165709, 165710, 165711, 165713, 165714, 165716, 165717, 165718, 165719, 165720, 165721, 165722, 165723, 165725, 165726, 165727, 165728, 165729, 165730, 165731, 165732, 165733, 165734, 165735, 165736, 165737, 165738, 165740, 165742, 165743, 165745, 165746, 165747, 165748, 165749, 165750, 165751, 165752, 165753, 165755, 165756, 165757, 165758, 165759, 165760, 165761, 165762, 165763, 165764, 165765, 165768, 165769, 165772, 165773, 165774, 165775, 165776, 165777, 165778, 165780, 165781, 165782, 165783, 165784, 165785, 165786, 165788, 165789, 165792, 165793, 165795, 165796, 165797, 165799, 165801, 165802, 165803, 165804, 165805, 165806, 165807, 165808, 165809, 165812, 165813, 165814, 165815, 165816, 165817, 165818, 165821, 165822, 165825, 165826, 165827, 165828, 165829, 165830, 165832, 165835, 165836, 165837, 165838, 165839, 165840, 165841, 165842, 165843, 165844, 165845, 165846, 165847, 165848, 165849, 165850, 165851, 165852, 165853, 165854, 165856, 165859, 165860, 165861, 165864, 165865, 165866, 165867, 165868, 165869, 165870, 165871, 165872, 165875, 165876, 165877, 165878, 165879, 165880, 165881, 165882, 165883, 165884, 165885, 165887, 165917, 165918, 165923, 165931, 165935, 165941, 165958, 165964, 165966, 165967, 165972, 165973, 165976, 165991, 166002, 166004, 166010, 166014, 166015, 166023, 166035, 166036, 166043, 166045, 166050, 166054, 166056, 166061, 166062, 166075, 166077, 166080, 166098, 166102, 166109, 166110, 166116, 166123, 166132, 166133, 166135, 166143, 166144, 166150, 166155, 166160, 166163, 166165, 166183, 166185, 166191, 166192, 166194, 166215, 166216, 166229, 166230, 166238, 166244, 166245, 166260, 166270, 166271, 166273, 166280, 166282, 166285, 166301, 166312, 166317, 166333, 166338, 166339, 166340, 166368, 166372, 166406, 166430, 166433, 166443, 166446, 166447, 166449, 166460, 166469, 166477, 166478, 166481, 166482, 166489, 166490, 166496, 166505, 166512, 166514, 166520, 166534, 166544, 166550, 166555, 166557, 166565, 166566, 166570, 166581, 166584, 166593, 166594, 166608, 166628, 166629, 166633, 166636, 166637, 166640, 166641, 166642, 166644, 166663, 166666, 166671, 166681, 166696, 166699, 166723, 166727, 166728, 166743, 166745, 166748, 166764, 166774, 166778, 166779, 166797, 166807, 166808, 166818, 166819, 166822, 166829, 166845, 166868, 166883, 166894, 166899, 166901, 166928, 166947, 166953, 167017, 167045, 167091, 169932, 169937, 169942, 169953, 169959, 169968, 169970, 169972, 169976, 169983, 169984, 169989, 169992, 169994, 170007, 170008, 170013, 170014, 170017, 170019, 170024, 170036, 170039, 170046, 170068, 170077, 170078, 170079, 170255, 170256);
DELETE FROM `creature_movement_override` WHERE `SpawnId` IN (164634, 164636, 164643, 164645, 164646, 164651, 164652, 164653, 164663, 164715, 164716, 164727, 164729, 164730, 164731, 164732, 164734, 164735, 164736, 164737, 164738, 164739, 164741, 164742, 164743, 164744, 164745, 164747, 164748, 164751, 164753, 164754, 164755, 164756, 164759, 164760, 164762, 164763, 164764, 164767, 164770, 164772, 164773, 164774, 164775, 164776, 164777, 164779, 164780, 164782, 164783, 164784, 164786, 164787, 164788, 164789, 164790, 164791, 164794, 164795, 164796, 164797, 164798, 164799, 164800, 164802, 164803, 164804, 164805, 164806, 164807, 164808, 164809, 164810, 164812, 164813, 164815, 164816, 164820, 164861, 164862, 164863, 164864, 164865, 164866, 164867, 164870, 164872, 164874, 164875, 164876, 164877, 164879, 164880, 164881, 164882, 164883, 164885, 164886, 164887, 164888, 164889, 164890, 164891, 164892, 164893, 164894, 164895, 164897, 164898, 164899, 164900, 164903, 164904, 164913, 164914, 164915, 164917, 164918, 164919, 164920, 164921, 164922, 164923, 164924, 164925, 164926, 164927, 164928, 164929, 164962, 164963, 164964, 164965, 164969, 164970, 164971, 164973, 164974, 164975, 164978, 164982, 164983, 164984, 164985, 164986, 164987, 164988, 164989, 164990, 164991, 164992, 164993, 164994, 164995, 164996, 164997, 164998, 164999, 165000, 165002, 165003, 165005, 165006, 165007, 165009, 165010, 165011, 165012, 165013, 165014, 165015, 165016, 165017, 165018, 165019, 165020, 165021, 165022, 165023, 165025, 165027, 165028, 165029, 165030, 165031, 165032, 165034, 165037, 165039, 165040, 165042, 165044, 165045, 165046, 165047, 165048, 165049, 165052, 165053, 165055, 165057, 165058, 165077, 165078, 165081, 165083, 165086, 165088, 165090, 165091, 165092, 165093, 165094, 165095, 165096, 165097, 165098, 165099, 165101, 165103, 165105, 165106, 165108, 165109, 165110, 165111, 165112, 165114, 165115, 165118, 165119, 165121, 165122, 165124, 165126, 165128, 165129, 165133, 165134, 165136, 165137, 165139, 165140, 165142, 165143, 165144, 165145, 165146, 165147, 165148, 165149, 165152, 165153, 165155, 165156, 165158, 165160, 165161, 165162, 165164, 165165, 165166, 165169, 165170, 165171, 165172, 165175, 165176, 165177, 165180, 165181, 165182, 165183, 165184, 165186, 165187, 165188, 165189, 165190, 165192, 165193, 165194, 165196, 165197, 165198, 165199, 165201, 165203, 165204, 165205, 165206, 165207, 165208, 165209, 165210, 165215, 165217, 165218, 165219, 165220, 165221, 165222, 165223, 165224, 165226, 165227, 165228, 165229, 165230, 165231, 165236, 165237, 165238, 165239, 165240, 165241, 165243, 165248, 165251, 165262, 165263, 165265, 165268, 165269, 165270, 165271, 165272, 165274, 165275, 165276, 165277, 165278, 165279, 165280, 165281, 165282, 165283, 165284, 165286, 165287, 165291, 165292, 165293, 165295, 165296, 165297, 165298, 165299, 165300, 165301, 165302, 165303, 165304, 165305, 165306, 165307, 165308, 165309, 165310, 165311, 165312, 165313, 165314, 165315, 165316, 165317, 165318, 165321, 165322, 165323, 165328, 165331, 165332, 165333, 165334, 165335, 165336, 165337, 165338, 165339, 165340, 165342, 165343, 165344, 165345, 165350, 165351, 165352, 165353, 165355, 165357, 165358, 165359, 165360, 165361, 165362, 165363, 165364, 165365, 165366, 165367, 165369, 165370, 165371, 165372, 165373, 165374, 165378, 165380, 165381, 165384, 165385, 165386, 165388, 165390, 165391, 165392, 165393, 165394, 165396, 165397, 165398, 165399, 165400, 165401, 165402, 165403, 165404, 165405, 165406, 165407, 165408, 165409, 165410, 165411, 165412, 165413, 165414, 165415, 165416, 165417, 165418, 165419, 165420, 165421, 165422, 165423, 165424, 165425, 165426, 165427, 165429, 165430, 165435, 165436, 165437, 165438, 165439, 165440, 165441, 165442, 165443, 165444, 165445, 165447, 165448, 165450, 165451, 165452, 165454, 165455, 165457, 165458, 165459, 165460, 165461, 165462, 165463, 165465, 165466, 165467, 165468, 165469, 165470, 165472, 165473, 165474, 165475, 165477, 165478, 165479, 165480, 165481, 165482, 165483, 165484, 165486, 165487, 165488, 165489, 165490, 165491, 165493, 165494, 165495, 165497, 165498, 165499, 165500, 165501, 165502, 165503, 165504, 165505, 165506, 165508, 165509, 165510, 165511, 165512, 165515, 165518, 165519, 165520, 165521, 165522, 165523, 165524, 165525, 165526, 165527, 165528, 165529, 165530, 165531, 165532, 165533, 165535, 165536, 165537, 165538, 165540, 165541, 165542, 165544, 165545, 165546, 165547, 165548, 165549, 165550, 165551, 165552, 165553, 165554, 165555, 165556, 165557, 165559, 165560, 165561, 165562, 165563, 165565, 165566, 165567, 165568, 165569, 165570, 165571, 165573, 165574, 165576, 165578, 165579, 165580, 165584, 165585, 165586, 165587, 165588, 165589, 165590, 165591, 165592, 165593, 165594, 165595, 165596, 165598, 165599, 165600, 165601, 165602, 165604, 165605, 165607, 165608, 165610, 165611, 165612, 165614, 165615, 165616, 165617, 165618, 165619, 165620, 165622, 165623, 165624, 165625, 165626, 165627, 165628, 165630, 165631, 165632, 165633, 165634, 165635, 165637, 165638, 165639, 165640, 165641, 165642, 165643, 165644, 165646, 165647, 165648, 165649, 165650, 165651, 165652, 165653, 165654, 165655, 165656, 165657, 165658, 165660, 165661, 165662, 165663, 165665, 165666, 165667, 165668, 165669, 165671, 165672, 165673, 165674, 165675, 165676, 165677, 165678, 165679, 165681, 165682, 165683, 165684, 165685, 165687, 165688, 165689, 165690, 165691, 165693, 165694, 165695, 165696, 165697, 165698, 165699, 165700, 165702, 165703, 165704, 165705, 165706, 165707, 165708, 165709, 165710, 165711, 165713, 165714, 165716, 165717, 165718, 165719, 165720, 165721, 165722, 165723, 165725, 165726, 165727, 165728, 165729, 165730, 165731, 165732, 165733, 165734, 165735, 165736, 165737, 165738, 165740, 165742, 165743, 165745, 165746, 165747, 165748, 165749, 165750, 165751, 165752, 165753, 165755, 165756, 165757, 165758, 165759, 165760, 165761, 165762, 165763, 165764, 165765, 165768, 165769, 165772, 165773, 165774, 165775, 165776, 165777, 165778, 165780, 165781, 165782, 165783, 165784, 165785, 165786, 165788, 165789, 165792, 165793, 165795, 165796, 165797, 165799, 165801, 165802, 165803, 165804, 165805, 165806, 165807, 165808, 165809, 165812, 165813, 165814, 165815, 165816, 165817, 165818, 165821, 165822, 165825, 165826, 165827, 165828, 165829, 165830, 165832, 165835, 165836, 165837, 165838, 165839, 165840, 165841, 165842, 165843, 165844, 165845, 165846, 165847, 165848, 165849, 165850, 165851, 165852, 165853, 165854, 165856, 165859, 165860, 165861, 165864, 165865, 165866, 165867, 165868, 165869, 165870, 165871, 165872, 165875, 165876, 165877, 165878, 165879, 165880, 165881, 165882, 165883, 165884, 165885, 165887, 165917, 165918, 165923, 165931, 165935, 165941, 165958, 165964, 165966, 165967, 165972, 165973, 165976, 165991, 166002, 166004, 166010, 166014, 166015, 166023, 166035, 166036, 166043, 166045, 166050, 166054, 166056, 166061, 166062, 166075, 166077, 166080, 166098, 166102, 166109, 166110, 166116, 166123, 166132, 166133, 166135, 166143, 166144, 166150, 166155, 166160, 166163, 166165, 166183, 166185, 166191, 166192, 166194, 166215, 166216, 166229, 166230, 166238, 166244, 166245, 166260, 166270, 166271, 166273, 166280, 166282, 166285, 166301, 166312, 166317, 166333, 166338, 166339, 166340, 166368, 166372, 166406, 166430, 166433, 166443, 166446, 166447, 166449, 166460, 166469, 166477, 166478, 166481, 166482, 166489, 166490, 166496, 166505, 166512, 166514, 166520, 166534, 166544, 166550, 166555, 166557, 166565, 166566, 166570, 166581, 166584, 166593, 166594, 166608, 166628, 166629, 166633, 166636, 166637, 166640, 166641, 166642, 166644, 166663, 166666, 166671, 166681, 166696, 166699, 166723, 166727, 166728, 166743, 166745, 166748, 166764, 166774, 166778, 166779, 166797, 166807, 166808, 166818, 166819, 166822, 166829, 166845, 166868, 166883, 166894, 166899, 166901, 166928, 166947, 166953, 167017, 167045, 167091, 169932, 169937, 169942, 169953, 169959, 169968, 169970, 169972, 169976, 169983, 169984, 169989, 169992, 169994, 170007, 170008, 170013, 170014, 170017, 170019, 170024, 170036, 170039, 170046, 170068, 170077, 170078, 170079, 170255, 170256);

DELETE FROM `creature_template_scaling` WHERE `Entry` IN (11723, 14455, 14471, 14472, 14474, 14477, 14478);
INSERT INTO `creature_template_scaling` (`Entry`, `LevelScalingMin`, `LevelScalingMax`, `LevelScalingDeltaMin`, `LevelScalingDeltaMax`, `LevelScalingDuration`, `VerifiedBuild`) VALUES
(11723, 40, 60, 0, 0, 0, 0),
(14455, 40, 60, 0, 0, 0, 0),
(14471, 40, 60, 2, 2, 0, 0),
(14472, 40, 60, 2, 2, 0, 0),
(14474, 40, 60, 2, 2, 0, 0),
(14477, 40, 60, 2, 2, 0, 0),
(14478, 40, 60, 2, 2, 0, 0);
