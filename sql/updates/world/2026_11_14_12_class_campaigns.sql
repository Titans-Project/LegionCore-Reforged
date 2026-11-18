-- Class campaigns of the Broken Isles: each quest limited to its class, class calls after the arrival in Dalaran and class couriers summoned before their quest

INSERT INTO `quest_template_addon` (`ID`,`AllowableClasses`) VALUES
(38904,1),(39191,1),(39192,1),(39214,1),(39530,1),(39654,1),(40043,1),(40579,1),(40585,1),(41105,1),(42107,1),(42110,1),
(42193,1),(42194,1),(42202,1),(42204,1),(42598,1),(42605,1),(42606,1),(42607,1),(42609,1),(42610,1),(42611,1),(42614,1),
(42616,1),(42617,1),(42618,1),(42619,1),(42650,1),(42651,1),(42814,1),(42815,1),(42918,1),(42974,1),(43090,1),(43425,1),
(43506,1),(43577,1),(43585,1),(43586,1),(43750,1),(43949,1),(44667,1),(44849,1),(44850,1),(44889,1),(45118,1),(45128,1),
(45632,1),(45633,1),(45634,1),(45647,1),(45648,1),(45649,1),(45650,1),(45834,1),(45873,1),(45876,1),(46207,1),(46208,1),
(46267,1)
ON DUPLICATE KEY UPDATE `AllowableClasses` = IF(`AllowableClasses` = 0, VALUES(`AllowableClasses`), `AllowableClasses`);
INSERT INTO `quest_template_addon` (`ID`,`AllowableClasses`) VALUES
(38376,2),(38566,2),(38576,2),(38933,2),(39696,2),(39722,2),(39756,2),(40408,2),(42000,2),(42002,2),(42005,2),(42017,2),
(42120,2),(42231,2),(42377,2),(42770,2),(42771,2),(42772,2),(42773,2),(42774,2),(42811,2),(42812,2),(42846,2),(42847,2),
(42848,2),(42849,2),(42850,2),(42851,2),(42852,2),(42866,2),(42867,2),(42881,2),(42885,2),(42886,2),(42887,2),(42888,2),
(42890,2),(42919,2),(42966,2),(42967,2),(42968,2),(43424,2),(43462,2),(43486,2),(43487,2),(43488,2),(43489,2),(43490,2),
(43491,2),(43492,2),(43493,2),(43534,2),(43535,2),(43540,2),(43541,2),(43697,2),(43698,2),(43699,2),(43700,2),(43701,2),
(43785,2),(43933,2),(43934,2),(44004,2),(44063,2),(44153,2),(44257,2),(44370,2),(45143,2),(45145,2),(45146,2),(45147,2),
(45148,2),(45149,2),(45770,2),(45890,2),(46045,2),(46069,2),(46070,2),(46071,2),(46074,2),(46083,2),(46259,2)
ON DUPLICATE KEY UPDATE `AllowableClasses` = IF(`AllowableClasses` = 0, VALUES(`AllowableClasses`), `AllowableClasses`);
INSERT INTO `quest_template_addon` (`ID`,`AllowableClasses`) VALUES
(39427,4),(40385,4),(40392,4),(40400,4),(40402,4),(40403,4),(40419,4),(40618,4),(40952,4),(40953,4),(40954,4),(40955,4),
(40957,4),(40958,4),(40959,4),(41047,4),(41053,4),(41415,4),(41540,4),(41541,4),(41542,4),(41574,4),(42133,4),(42134,4),
(42158,4),(42185,4),(42384,4),(42385,4),(42386,4),(42387,4),(42388,4),(42389,4),(42390,4),(42391,4),(42392,4),(42393,4),
(42394,4),(42395,4),(42397,4),(42398,4),(42399,4),(42400,4),(42401,4),(42402,4),(42403,4),(42404,4),(42405,4),(42406,4),
(42407,4),(42408,4),(42409,4),(42410,4),(42411,4),(42412,4),(42415,4),(42436,4),(42519,4),(42523,4),(42524,4),(42525,4),
(42526,4),(42654,4),(42655,4),(42656,4),(42657,4),(42658,4),(42659,4),(42689,4),(42691,4),(43182,4),(43335,4),(43423,4),
(44043,4),(44366,4),(44680,4),(45551,4),(45552,4),(45553,4),(45554,4),(45555,4),(45556,4),(45557,4),(46048,4),(46060,4),
(46235,4)
ON DUPLICATE KEY UPDATE `AllowableClasses` = IF(`AllowableClasses` = 0, VALUES(`AllowableClasses`), `AllowableClasses`);
INSERT INTO `quest_template_addon` (`ID`,`AllowableClasses`) VALUES
(37448,8),(37494,8),(37666,8),(40994,8),(40995,8),(40996,8),(40997,8),(41919,8),(41920,8),(41921,8),(41922,8),(41924,8),
(42139,8),(42140,8),(42501,8),(42502,8),(42503,8),(42504,8),(42539,8),(42568,8),(42627,8),(42678,8),(42680,8),(42684,8),
(42730,8),(42800,8),(43013,8),(43014,8),(43015,8),(43249,8),(43250,8),(43251,8),(43252,8),(43253,8),(43261,8),(43262,8),
(43422,8),(43468,8),(43469,8),(43470,8),(43479,8),(43485,8),(43508,8),(43723,8),(43724,8),(43829,8),(43841,8),(43852,8),
(43958,8),(44034,8),(44041,8),(44116,8),(44117,8),(44155,8),(44177,8),(44178,8),(44180,8),(44181,8),(44183,8),(44215,8),
(44375,8),(44758,8),(45073,8),(45571,8),(45573,8),(45576,8),(45628,8),(45629,8),(45833,8),(45835,8),(45836,8),(45848,8),
(46058,8),(46059,8),(46260,8),(46322,8),(46323,8),(46324,8),(46326,8),(46827,8)
ON DUPLICATE KEY UPDATE `AllowableClasses` = IF(`AllowableClasses` = 0, VALUES(`AllowableClasses`), `AllowableClasses`);
INSERT INTO `quest_template_addon` (`ID`,`AllowableClasses`) VALUES
(40706,16),(40710,16),(40938,16),(41015,16),(41017,16),(41019,16),(41625,16),(41626,16),(41627,16),(41628,16),(41629,16),(41630,16),
(41631,16),(41632,16),(41957,16),(41966,16),(41967,16),(41993,16),(42074,16),(42137,16),(42138,16),(43270,16),(43271,16),(43272,16),
(43273,16),(43275,16),(43276,16),(43277,16),(43371,16),(43372,16),(43373,16),(43374,16),(43375,16),(43376,16),(43377,16),(43378,16),
(43379,16),(43380,16),(43381,16),(43382,16),(43383,16),(43385,16),(43386,16),(43387,16),(43388,16),(43389,16),(43390,16),(43391,16),
(43392,16),(43393,16),(43394,16),(43395,16),(43396,16),(43397,16),(43398,16),(43399,16),(43400,16),(43401,16),(43402,16),(43420,16),
(43797,16),(43832,16),(43851,16),(43935,16),(44407,16),(45342,16),(45343,16),(45344,16),(45345,16),(45346,16),(45347,16),(45348,16),
(45349,16),(45350,16),(46034,16),(46145,16)
ON DUPLICATE KEY UPDATE `AllowableClasses` = IF(`AllowableClasses` = 0, VALUES(`AllowableClasses`), `AllowableClasses`);
INSERT INTO `quest_template_addon` (`ID`,`AllowableClasses`) VALUES
(38990,32),(39757,32),(39761,32),(39799,32),(39816,32),(39818,32),(39832,32),(40714,32),(40715,32),(40740,32),(40930,32),(40931,32),
(40932,32),(40933,32),(40934,32),(40935,32),(40986,32),(40987,32),(42449,32),(42484,32),(42533,32),(42534,32),(42535,32),(42536,32),
(42537,32),(42708,32),(42818,32),(42821,32),(42823,32),(42824,32),(42882,32),(43264,32),(43265,32),(43266,32),(43267,32),(43268,32),
(43407,32),(43539,32),(43571,32),(43572,32),(43573,32),(43574,32),(43686,32),(43899,32),(43928,32),(43962,32),(44082,32),(44243,32),
(44244,32),(44245,32),(44246,32),(44247,32),(44248,32),(44282,32),(44286,32),(44401,32),(44690,32),(44775,32),(44783,32),(44787,32),
(45103,32),(45240,32),(45243,32),(45331,32),(45398,32),(45399,32),(46050,32),(46305,32),(46719,32),(46720,32),(46812,32),(46813,32)
ON DUPLICATE KEY UPDATE `AllowableClasses` = IF(`AllowableClasses` = 0, VALUES(`AllowableClasses`), `AllowableClasses`);
INSERT INTO `quest_template_addon` (`ID`,`AllowableClasses`) VALUES
(39771,64),(40224,64),(40225,64),(40276,64),(40341,64),(41335,64),(41510,64),(41740,64),(41741,64),(41742,64),(41743,64),(41744,64),
(41745,64),(41746,64),(41747,64),(41770,64),(41771,64),(41772,64),(41773,64),(41775,64),(41776,64),(41777,64),(41888,64),(41897,64),
(41898,64),(41899,64),(41900,64),(41901,64),(41934,64),(42065,64),(42068,64),(42114,64),(42141,64),(42142,64),(42184,64),(42188,64),
(42197,64),(42198,64),(42200,64),(42208,64),(42383,64),(42932,64),(42933,64),(42935,64),(42936,64),(42937,64),(42977,64),(42983,64),
(42984,64),(42986,64),(42988,64),(42989,64),(42990,64),(42995,64),(42996,64),(42997,64),(43002,64),(43003,64),(43334,64),(43338,64),
(43418,64),(43644,64),(43645,64),(43945,64),(44006,64),(44406,64),(44465,64),(44800,64),(45652,64),(45706,64),(45723,64),(45724,64),
(45725,64),(45763,64),(45765,64),(45767,64),(45769,64),(45883,64),(45971,64),(46057,64),(46258,64)
ON DUPLICATE KEY UPDATE `AllowableClasses` = IF(`AllowableClasses` = 0, VALUES(`AllowableClasses`), `AllowableClasses`);
INSERT INTO `quest_template_addon` (`ID`,`AllowableClasses`) VALUES
(11997,128),(40267,128),(40270,128),(41035,128),(41036,128),(41085,128),(41112,128),(41113,128),(41114,128),(41124,128),(41125,128),(41141,128),
(42001,128),(42006,128),(42007,128),(42008,128),(42009,128),(42010,128),(42011,128),(42126,128),(42127,128),(42149,128),(42166,128),(42171,128),
(42206,128),(42222,128),(42416,128),(42418,128),(42423,128),(42424,128),(42433,128),(42434,128),(42435,128),(42451,128),(42452,128),(42455,128),
(42476,128),(42477,128),(42479,128),(42493,128),(42494,128),(42508,128),(42520,128),(42521,128),(42662,128),(42663,128),(42685,128),(42687,128),
(42696,128),(42702,128),(42703,128),(42704,128),(42705,128),(42706,128),(42707,128),(42734,128),(42914,128),(42917,128),(42940,128),(42954,128),
(42955,128),(42956,128),(42959,128),(43415,128),(43441,128),(44310,128),(44689,128),(44766,128),(44768,128),(44770,128),(45207,128),(45251,128),
(45354,128),(45586,128),(45614,128),(45615,128),(45630,128),(45844,128),(45845,128),(45846,128),(45847,128),(46000,128),(46043,128),(46290,128),
(46335,128),(46338,128),(46339,128),(46345,128),(46351,128),(46722,128),(46723,128),(46724,128)
ON DUPLICATE KEY UPDATE `AllowableClasses` = IF(`AllowableClasses` = 0, VALUES(`AllowableClasses`), `AllowableClasses`);
INSERT INTO `quest_template_addon` (`ID`,`AllowableClasses`) VALUES
(39142,256),(39179,256),(39389,256),(40218,256),(40495,256),(40588,256),(40604,256),(40606,256),(40611,256),(40623,256),(40684,256),(40712,256),
(40729,256),(40731,256),(40821,256),(41155,256),(41156,256),(41748,256),(41750,256),(41751,256),(41752,256),(41753,256),(41754,256),(41755,256),
(41756,256),(41759,256),(41767,256),(41768,256),(41769,256),(41780,256),(41781,256),(41784,256),(41785,256),(41787,256),(41788,256),(41793,256),
(41795,256),(41796,256),(41797,256),(41798,256),(42097,256),(42098,256),(42100,256),(42102,256),(42103,256),(42125,256),(42128,256),(42168,256),
(42601,256),(42602,256),(42603,256),(42608,256),(42660,256),(43100,256),(43153,256),(43254,256),(43414,256),(43984,256),(44089,256),(44682,256),
(45021,256),(45024,256),(45025,256),(45026,256),(45027,256),(45028,256),(45794,256),(46020,256),(46047,256),(46237,256),(46238,256),(46239,256),
(46240,256),(46241,256),(46242,256),(46243,256),(46316,256)
ON DUPLICATE KEY UPDATE `AllowableClasses` = IF(`AllowableClasses` = 0, VALUES(`AllowableClasses`), `AllowableClasses`);
INSERT INTO `quest_template_addon` (`ID`,`AllowableClasses`) VALUES
(32442,512),(40236,512),(40569,512),(40570,512),(40633,512),(40634,512),(40636,512),(40698,512),(40704,512),(40793,512),(40795,512),(41003,512),
(41038,512),(41039,512),(41040,512),(41059,512),(41086,512),(41087,512),(41115,512),(41728,512),(41729,512),(41730,512),(41731,512),(41732,512),
(41733,512),(41734,512),(41735,512),(41736,512),(41737,512),(41738,512),(41739,512),(41849,512),(41850,512),(41851,512),(41852,512),(41853,512),
(41854,512),(41905,512),(41907,512),(41909,512),(41910,512),(41911,512),(41945,512),(41946,512),(42187,512),(42191,512),(42210,512),(42762,512),
(42765,512),(42766,512),(42767,512),(42768,512),(42868,512),(42957,512),(43054,512),(43062,512),(43151,512),(43359,512),(43973,512),(44424,512),
(45404,512),(45440,512),(45442,512),(45449,512),(45459,512),(45545,512),(45574,512),(45771,512),(45790,512),(46341,512),(46342,512),(46343,512),
(46344,512),(46346,512),(46347,512),(46348,512),(46349,512),(46350,512),(46353,512)
ON DUPLICATE KEY UPDATE `AllowableClasses` = IF(`AllowableClasses` = 0, VALUES(`AllowableClasses`), `AllowableClasses`);
INSERT INTO `quest_template_addon` (`ID`,`AllowableClasses`) VALUES
(40644,1024),(40645,1024),(40646,1024),(40647,1024),(40649,1024),(40650,1024),(40651,1024),(40652,1024),(40653,1024),(40783,1024),(40784,1024),(40785,1024),
(40834,1024),(40835,1024),(40837,1024),(40838,1024),(40900,1024),(41106,1024),(41255,1024),(41332,1024),(41422,1024),(41436,1024),(41449,1024),(41468,1024),
(41689,1024),(41690,1024),(41782,1024),(41783,1024),(41790,1024),(41791,1024),(41792,1024),(41918,1024),(42031,1024),(42032,1024),(42033,1024),(42034,1024),
(42035,1024),(42036,1024),(42037,1024),(42038,1024),(42039,1024),(42040,1024),(42041,1024),(42042,1024),(42043,1024),(42044,1024),(42045,1024),(42046,1024),
(42047,1024),(42048,1024),(42049,1024),(42050,1024),(42051,1024),(42053,1024),(42054,1024),(42055,1024),(42056,1024),(42096,1024),(42129,1024),(42365,1024),
(42428,1024),(42430,1024),(42438,1024),(42439,1024),(42440,1024),(42583,1024),(42584,1024),(42585,1024),(42586,1024),(42588,1024),(42719,1024),(43365,1024),
(43368,1024),(43403,1024),(43409,1024),(43980,1024),(43991,1024),(44074,1024),(44075,1024),(44076,1024),(44077,1024),(44431,1024),(44443,1024),(44869,1024),
(44877,1024),(44888,1024),(44921,1024),(45425,1024),(45426,1024),(45498,1024),(45528,1024),(45532,1024),(46044,1024),(46674,1024),(46675,1024),(46676,1024),
(46677,1024),(46924,1024)
ON DUPLICATE KEY UPDATE `AllowableClasses` = IF(`AllowableClasses` = 0, VALUES(`AllowableClasses`), `AllowableClasses`);
INSERT INTO `quest_template_addon` (`ID`,`AllowableClasses`) VALUES
(37447,2048),(39051,2048),(39247,2048),(39261,2048),(39741,2048),(40247,2048),(40249,2048),(40814,2048),(40816,2048),(40819,2048),(41033,2048),(41037,2048),
(41060,2048),(41062,2048),(41064,2048),(41066,2048),(41067,2048),(41069,2048),(41070,2048),(41096,2048),(41099,2048),(41119,2048),(41120,2048),(41121,2048),
(41221,2048),(41803,2048),(41804,2048),(41806,2048),(41807,2048),(41863,2048),(42131,2048),(42132,2048),(42510,2048),(42522,2048),(42593,2048),(42594,2048),
(42634,2048),(42653,2048),(42664,2048),(42665,2048),(42669,2048),(42670,2048),(42671,2048),(42673,2048),(42677,2048),(42679,2048),(42681,2048),(42682,2048),
(42683,2048),(42690,2048),(42695,2048),(42697,2048),(42701,2048),(42731,2048),(42732,2048),(42733,2048),(42735,2048),(42736,2048),(42737,2048),(42738,2048),
(42739,2048),(42749,2048),(42752,2048),(42753,2048),(42754,2048),(42775,2048),(42776,2048),(42777,2048),(42787,2048),(42801,2048),(42802,2048),(42808,2048),
(42809,2048),(42810,2048),(42869,2048),(42872,2048),(42920,2048),(42921,2048),(43184,2048),(43185,2048),(43186,2048),(43412,2048),(44161,2048),(44214,2048),
(44379,2048),(44383,2048),(44694,2048),(45301,2048),(45329,2048),(45330,2048),(45339,2048),(45385,2048),(45391,2048),(45764,2048),(45798,2048),(46159,2048),
(46266,2048),(46725,2048)
ON DUPLICATE KEY UPDATE `AllowableClasses` = IF(`AllowableClasses` = 0, VALUES(`AllowableClasses`), `AllowableClasses`);


DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 19 AND `SourceEntry` IN (12103,38710,39047,39261,39746,40384,40643,40705,40714,40716,40832,41035,41052,42814,42978,42985);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` IN (216443,190886,196908,201208,226409,200023,195356,204860,193978,199277,195286);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 7502 AND `SourceEntry` = 6 AND `ElseGroup` = 1;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(19,0,41052,0,0,8,0,44663,0,0,0,0,'','Class call: In the Blink of an Eye rewarded'),
(19,0,41052,0,1,8,0,44184,0,0,0,0,'','Class call: In the Blink of an Eye (launch event) rewarded'),
(19,0,41052,0,2,27,0,110,3,0,0,0,'','Class call: level 110 (boosted character)'),
(19,0,42814,0,0,8,0,44663,0,0,0,0,'','Class call: In the Blink of an Eye rewarded'),
(19,0,42814,0,1,8,0,44184,0,0,0,0,'','Class call: In the Blink of an Eye (launch event) rewarded'),
(19,0,42814,0,2,27,0,110,3,0,0,0,'','Class call: level 110 (boosted character)'),
(19,0,38710,0,0,8,0,44663,0,0,0,0,'','Class call: In the Blink of an Eye rewarded'),
(19,0,38710,0,1,8,0,44184,0,0,0,0,'','Class call: In the Blink of an Eye (launch event) rewarded'),
(19,0,38710,0,2,27,0,110,3,0,0,0,'','Class call: level 110 (boosted character)'),
(19,0,40384,0,0,8,0,44663,0,0,0,0,'','Class call: In the Blink of an Eye rewarded'),
(19,0,40384,0,1,8,0,44184,0,0,0,0,'','Class call: In the Blink of an Eye (launch event) rewarded'),
(19,0,40384,0,2,27,0,110,3,0,0,0,'','Class call: level 110 (boosted character)'),
(19,0,40832,0,0,8,0,44663,0,0,0,0,'','Class call: In the Blink of an Eye rewarded'),
(19,0,40832,0,1,8,0,44184,0,0,0,0,'','Class call: In the Blink of an Eye (launch event) rewarded'),
(19,0,40832,0,2,27,0,110,3,0,0,0,'','Class call: level 110 (boosted character)'),
(19,0,40705,0,0,8,0,44663,0,0,0,0,'','Class call: In the Blink of an Eye rewarded'),
(19,0,40705,0,1,8,0,44184,0,0,0,0,'','Class call: In the Blink of an Eye (launch event) rewarded'),
(19,0,40705,0,2,27,0,110,3,0,0,0,'','Class call: level 110 (boosted character)'),
(19,0,40714,0,0,8,0,44663,0,0,0,0,'','Class call: In the Blink of an Eye rewarded'),
(19,0,40714,0,1,8,0,44184,0,0,0,0,'','Class call: In the Blink of an Eye (launch event) rewarded'),
(19,0,40714,0,2,27,0,110,3,0,0,0,'','Class call: level 110 (boosted character)'),
(19,0,41035,0,0,8,0,44663,0,0,0,0,'','Class call: In the Blink of an Eye rewarded'),
(19,0,41035,0,1,8,0,44184,0,0,0,0,'','Class call: In the Blink of an Eye (launch event) rewarded'),
(19,0,41035,0,2,27,0,110,3,0,0,0,'','Class call: level 110 (boosted character)'),
(19,0,40716,0,0,8,0,44663,0,0,0,0,'','Class call: In the Blink of an Eye rewarded'),
(19,0,40716,0,1,8,0,44184,0,0,0,0,'','Class call: In the Blink of an Eye (launch event) rewarded'),
(19,0,40716,0,2,27,0,110,3,0,0,0,'','Class call: level 110 (boosted character)'),
(19,0,12103,0,0,8,0,44663,0,0,0,0,'','Class call: In the Blink of an Eye rewarded'),
(19,0,12103,0,1,8,0,44184,0,0,0,0,'','Class call: In the Blink of an Eye (launch event) rewarded'),
(19,0,12103,0,2,27,0,110,3,0,0,0,'','Class call: level 110 (boosted character)'),
(19,0,40643,0,0,8,0,44663,0,0,0,0,'','Class call: In the Blink of an Eye rewarded'),
(19,0,40643,0,1,8,0,44184,0,0,0,0,'','Class call: In the Blink of an Eye (launch event) rewarded'),
(19,0,40643,0,2,27,0,110,3,0,0,0,'','Class call: level 110 (boosted character)'),
(19,0,39746,0,0,8,0,44663,0,0,0,0,'','Class call: In the Blink of an Eye rewarded'),
(19,0,39746,0,1,8,0,44184,0,0,0,0,'','Class call: In the Blink of an Eye (launch event) rewarded'),
(19,0,39746,0,2,27,0,110,3,0,0,0,'','Class call: level 110 (boosted character)'),
(19,0,39047,0,0,8,0,40375,0,0,0,0,'','Call of the Illidari: sided with Altruis'),
(19,0,39047,0,0,8,0,44663,0,0,0,0,'','Call of the Illidari: In the Blink of an Eye rewarded'),
(19,0,39047,0,1,8,0,40375,0,0,0,0,'','Call of the Illidari: sided with Altruis'),
(19,0,39047,0,1,8,0,44184,0,0,0,0,'','Call of the Illidari: In the Blink of an Eye (launch event) rewarded'),
(19,0,39047,0,2,8,0,40375,0,0,0,0,'','Call of the Illidari: sided with Altruis'),
(19,0,39047,0,2,27,0,110,3,0,0,0,'','Call of the Illidari: level 110 (boosted character)'),
(19,0,39261,0,0,8,0,40374,0,0,0,0,'','Call of the Illidari: sided with Kayn'),
(19,0,39261,0,0,8,0,44663,0,0,0,0,'','Call of the Illidari: In the Blink of an Eye rewarded'),
(19,0,39261,0,1,8,0,40374,0,0,0,0,'','Call of the Illidari: sided with Kayn'),
(19,0,39261,0,1,8,0,44184,0,0,0,0,'','Call of the Illidari: In the Blink of an Eye (launch event) rewarded'),
(19,0,39261,0,2,8,0,40374,0,0,0,0,'','Call of the Illidari: sided with Kayn'),
(19,0,39261,0,2,27,0,110,3,0,0,0,'','Call of the Illidari: level 110 (boosted character)'),
(17,0,216443,0,0,8,0,44663,0,0,0,0,'','Class courier: In the Blink of an Eye rewarded'),
(17,0,216443,0,1,8,0,44184,0,0,0,0,'','Class courier: In the Blink of an Eye (launch event) rewarded'),
(17,0,216443,0,2,27,0,110,3,0,0,0,'','Class courier: level 110 (boosted character)'),
(17,0,190886,0,0,8,0,44663,0,0,0,0,'','Class courier: In the Blink of an Eye rewarded'),
(17,0,190886,0,1,8,0,44184,0,0,0,0,'','Class courier: In the Blink of an Eye (launch event) rewarded'),
(17,0,190886,0,2,27,0,110,3,0,0,0,'','Class courier: level 110 (boosted character)'),
(17,0,196908,0,0,8,0,44663,0,0,0,0,'','Class courier: In the Blink of an Eye rewarded'),
(17,0,196908,0,1,8,0,44184,0,0,0,0,'','Class courier: In the Blink of an Eye (launch event) rewarded'),
(17,0,196908,0,2,27,0,110,3,0,0,0,'','Class courier: level 110 (boosted character)'),
(17,0,201208,0,0,8,0,44663,0,0,0,0,'','Class courier: In the Blink of an Eye rewarded'),
(17,0,201208,0,1,8,0,44184,0,0,0,0,'','Class courier: In the Blink of an Eye (launch event) rewarded'),
(17,0,201208,0,2,27,0,110,3,0,0,0,'','Class courier: level 110 (boosted character)'),
(17,0,226409,0,0,8,0,44663,0,0,0,0,'','Class courier: In the Blink of an Eye rewarded'),
(17,0,226409,0,1,8,0,44184,0,0,0,0,'','Class courier: In the Blink of an Eye (launch event) rewarded'),
(17,0,226409,0,2,27,0,110,3,0,0,0,'','Class courier: level 110 (boosted character)'),
(17,0,200023,0,0,8,0,44663,0,0,0,0,'','Class courier: In the Blink of an Eye rewarded'),
(17,0,200023,0,1,8,0,44184,0,0,0,0,'','Class courier: In the Blink of an Eye (launch event) rewarded'),
(17,0,200023,0,2,27,0,110,3,0,0,0,'','Class courier: level 110 (boosted character)'),
(17,0,195356,0,0,8,0,44663,0,0,0,0,'','Class courier: In the Blink of an Eye rewarded'),
(17,0,195356,0,1,8,0,44184,0,0,0,0,'','Class courier: In the Blink of an Eye (launch event) rewarded'),
(17,0,195356,0,2,27,0,110,3,0,0,0,'','Class courier: level 110 (boosted character)'),
(17,0,204860,0,0,8,0,44663,0,0,0,0,'','Class courier: In the Blink of an Eye rewarded'),
(17,0,204860,0,1,8,0,44184,0,0,0,0,'','Class courier: In the Blink of an Eye (launch event) rewarded'),
(17,0,204860,0,2,27,0,110,3,0,0,0,'','Class courier: level 110 (boosted character)'),
(17,0,193978,0,0,8,0,44663,0,0,0,0,'','Class courier: In the Blink of an Eye rewarded'),
(17,0,193978,0,1,8,0,44184,0,0,0,0,'','Class courier: In the Blink of an Eye (launch event) rewarded'),
(17,0,193978,0,2,27,0,110,3,0,0,0,'','Class courier: level 110 (boosted character)'),
(17,0,199277,0,0,8,0,44663,0,0,0,0,'','Class courier: In the Blink of an Eye rewarded'),
(17,0,199277,0,1,8,0,44184,0,0,0,0,'','Class courier: In the Blink of an Eye (launch event) rewarded'),
(17,0,199277,0,2,27,0,110,3,0,0,0,'','Class courier: level 110 (boosted character)'),
(17,0,195286,0,0,8,0,40374,0,0,0,0,'','Kor''vas: sided with Kayn'),
(17,0,195286,0,0,14,0,39261,0,0,0,0,'','Kor''vas: Call of the Illidari not taken'),
(17,0,195286,0,0,8,0,44663,0,0,0,0,'','Kor''vas: In the Blink of an Eye rewarded'),
(17,0,195286,0,1,8,0,40374,0,0,0,0,'','Kor''vas: sided with Kayn'),
(17,0,195286,0,1,14,0,39261,0,0,0,0,'','Kor''vas: Call of the Illidari not taken'),
(17,0,195286,0,1,8,0,44184,0,0,0,0,'','Kor''vas: In the Blink of an Eye (launch event) rewarded'),
(17,0,195286,0,2,8,0,40374,0,0,0,0,'','Kor''vas: sided with Kayn'),
(17,0,195286,0,2,14,0,39261,0,0,0,0,'','Kor''vas: Call of the Illidari not taken'),
(17,0,195286,0,2,27,0,110,3,0,0,0,'','Kor''vas: level 110 (boosted character)'),
(17,0,195286,0,3,8,0,40375,0,0,0,0,'','Kor''vas: sided with Altruis'),
(17,0,195286,0,3,14,0,39047,0,0,0,0,'','Kor''vas: Call of the Illidari not taken'),
(17,0,195286,0,3,8,0,44663,0,0,0,0,'','Kor''vas: In the Blink of an Eye rewarded'),
(17,0,195286,0,4,8,0,40375,0,0,0,0,'','Kor''vas: sided with Altruis'),
(17,0,195286,0,4,14,0,39047,0,0,0,0,'','Kor''vas: Call of the Illidari not taken'),
(17,0,195286,0,4,8,0,44184,0,0,0,0,'','Kor''vas: In the Blink of an Eye (launch event) rewarded'),
(17,0,195286,0,5,8,0,40375,0,0,0,0,'','Kor''vas: sided with Altruis'),
(17,0,195286,0,5,14,0,39047,0,0,0,0,'','Kor''vas: Call of the Illidari not taken'),
(17,0,195286,0,5,27,0,110,3,0,0,0,'','Kor''vas: level 110 (boosted character)'),
(19,0,42978,0,0,17,0,10994,0,0,0,0,'','A Royal Audience: A Glorious Campaign'),
(19,0,42985,0,0,17,0,10994,0,0,0,0,'','A Royal Audience: A Glorious Campaign'),
(23,7502,6,0,1,8,0,39247,0,0,1,0,'','Activate Phase - If Player Not Reward Quest'),
(23,7502,6,0,1,8,0,41863,0,0,1,0,'','Activate Phase - If Player Not Reward Quest'),
(23,7502,6,0,1,14,0,39261,0,0,1,0,'','Activate Phase - If Player Has Quest');

UPDATE `spell_area` SET `quest_end_status` = 33 WHERE `area` IN (7502,7581) AND `quest_end` <> 0 AND `quest_end_status` = 74;
UPDATE `spell_area` SET `quest_end_status` = 41 WHERE `spell` = 210531 AND `area` = 8012 AND `quest_end_status` = 66;

UPDATE `smart_scripts` SET `action_param1` = 1 WHERE `entryorguid` = 99343 AND `source_type` = 0 AND `id` = 1;
UPDATE `smart_scripts` SET `action_param1` = 0 WHERE `entryorguid` = 99343 AND `source_type` = 0 AND `id` = 2;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 99343 AND `source_type` = 0 AND `id` = 6;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_chance`,`event_param1`,`action_type`,`action_param1`,`target_type`,`comment`) VALUES
(99343,0,6,3,19,100,39261,1,0,7,'Quest Accept - Say Text');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 111109 AND `source_type` = 0 AND `id` BETWEEN 1 AND 22;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_chance`,`event_param1`,`action_type`,`action_param1`,`action_param2`,`target_type`,`comment`) VALUES
(111109,0,1,0,20,100,44663,85,216443,2,7,'Quest Reward 44663 - Invoker Cast class courier 216443'),
(111109,0,2,0,20,100,44663,85,190886,2,7,'Quest Reward 44663 - Invoker Cast class courier 190886'),
(111109,0,3,0,20,100,44663,85,196908,2,7,'Quest Reward 44663 - Invoker Cast class courier 196908'),
(111109,0,4,0,20,100,44663,85,201208,2,7,'Quest Reward 44663 - Invoker Cast class courier 201208'),
(111109,0,5,0,20,100,44663,85,226409,2,7,'Quest Reward 44663 - Invoker Cast class courier 226409'),
(111109,0,6,0,20,100,44663,85,200023,2,7,'Quest Reward 44663 - Invoker Cast class courier 200023'),
(111109,0,7,0,20,100,44663,85,195356,2,7,'Quest Reward 44663 - Invoker Cast class courier 195356'),
(111109,0,8,0,20,100,44663,85,204860,2,7,'Quest Reward 44663 - Invoker Cast class courier 204860'),
(111109,0,9,0,20,100,44663,85,193978,2,7,'Quest Reward 44663 - Invoker Cast class courier 193978'),
(111109,0,10,0,20,100,44663,85,199277,2,7,'Quest Reward 44663 - Invoker Cast class courier 199277'),
(111109,0,11,0,20,100,44663,85,195286,2,7,'Quest Reward 44663 - Invoker Cast class courier 195286'),
(111109,0,12,0,20,100,44184,85,216443,2,7,'Quest Reward 44184 - Invoker Cast class courier 216443'),
(111109,0,13,0,20,100,44184,85,190886,2,7,'Quest Reward 44184 - Invoker Cast class courier 190886'),
(111109,0,14,0,20,100,44184,85,196908,2,7,'Quest Reward 44184 - Invoker Cast class courier 196908'),
(111109,0,15,0,20,100,44184,85,201208,2,7,'Quest Reward 44184 - Invoker Cast class courier 201208'),
(111109,0,16,0,20,100,44184,85,226409,2,7,'Quest Reward 44184 - Invoker Cast class courier 226409'),
(111109,0,17,0,20,100,44184,85,200023,2,7,'Quest Reward 44184 - Invoker Cast class courier 200023'),
(111109,0,18,0,20,100,44184,85,195356,2,7,'Quest Reward 44184 - Invoker Cast class courier 195356'),
(111109,0,19,0,20,100,44184,85,204860,2,7,'Quest Reward 44184 - Invoker Cast class courier 204860'),
(111109,0,20,0,20,100,44184,85,193978,2,7,'Quest Reward 44184 - Invoker Cast class courier 193978'),
(111109,0,21,0,20,100,44184,85,199277,2,7,'Quest Reward 44184 - Invoker Cast class courier 199277'),
(111109,0,22,0,20,100,44184,85,195286,2,7,'Quest Reward 44184 - Invoker Cast class courier 195286');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceEntry` = 111109 AND `SourceId` = 0 AND `SourceGroup` BETWEEN 2 AND 23;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(22,2,111109,0,0,15,0,1,0,0,0,0,'','Emissary Auldbridge: class courier 216443 for its class only (In the Blink of an Eye)'),
(22,3,111109,0,0,15,0,2,0,0,0,0,'','Emissary Auldbridge: class courier 190886 for its class only (In the Blink of an Eye)'),
(22,4,111109,0,0,15,0,4,0,0,0,0,'','Emissary Auldbridge: class courier 196908 for its class only (In the Blink of an Eye)'),
(22,5,111109,0,0,15,0,8,0,0,0,0,'','Emissary Auldbridge: class courier 201208 for its class only (In the Blink of an Eye)'),
(22,6,111109,0,0,15,0,16,0,0,0,0,'','Emissary Auldbridge: class courier 226409 for its class only (In the Blink of an Eye)'),
(22,7,111109,0,0,15,0,32,0,0,0,0,'','Emissary Auldbridge: class courier 200023 for its class only (In the Blink of an Eye)'),
(22,8,111109,0,0,15,0,128,0,0,0,0,'','Emissary Auldbridge: class courier 195356 for its class only (In the Blink of an Eye)'),
(22,9,111109,0,0,15,0,256,0,0,0,0,'','Emissary Auldbridge: class courier 204860 for its class only (In the Blink of an Eye)'),
(22,10,111109,0,0,15,0,512,0,0,0,0,'','Emissary Auldbridge: class courier 193978 for its class only (In the Blink of an Eye)'),
(22,11,111109,0,0,15,0,1024,0,0,0,0,'','Emissary Auldbridge: class courier 199277 for its class only (In the Blink of an Eye)'),
(22,12,111109,0,0,15,0,2048,0,0,0,0,'','Emissary Auldbridge: class courier 195286 for its class only (In the Blink of an Eye)'),
(22,13,111109,0,0,15,0,1,0,0,0,0,'','Emissary Auldbridge: class courier 216443 for its class only (In the Blink of an Eye (launch event))'),
(22,14,111109,0,0,15,0,2,0,0,0,0,'','Emissary Auldbridge: class courier 190886 for its class only (In the Blink of an Eye (launch event))'),
(22,15,111109,0,0,15,0,4,0,0,0,0,'','Emissary Auldbridge: class courier 196908 for its class only (In the Blink of an Eye (launch event))'),
(22,16,111109,0,0,15,0,8,0,0,0,0,'','Emissary Auldbridge: class courier 201208 for its class only (In the Blink of an Eye (launch event))'),
(22,17,111109,0,0,15,0,16,0,0,0,0,'','Emissary Auldbridge: class courier 226409 for its class only (In the Blink of an Eye (launch event))'),
(22,18,111109,0,0,15,0,32,0,0,0,0,'','Emissary Auldbridge: class courier 200023 for its class only (In the Blink of an Eye (launch event))'),
(22,19,111109,0,0,15,0,128,0,0,0,0,'','Emissary Auldbridge: class courier 195356 for its class only (In the Blink of an Eye (launch event))'),
(22,20,111109,0,0,15,0,256,0,0,0,0,'','Emissary Auldbridge: class courier 204860 for its class only (In the Blink of an Eye (launch event))'),
(22,21,111109,0,0,15,0,512,0,0,0,0,'','Emissary Auldbridge: class courier 193978 for its class only (In the Blink of an Eye (launch event))'),
(22,22,111109,0,0,15,0,1024,0,0,0,0,'','Emissary Auldbridge: class courier 199277 for its class only (In the Blink of an Eye (launch event))'),
(22,23,111109,0,0,15,0,2048,0,0,0,0,'','Emissary Auldbridge: class courier 195286 for its class only (In the Blink of an Eye (launch event))');
