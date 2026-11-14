-- Supreme Lord Kazzak (Tanaan Jungle world boss): 15 minutes to respawn, as observed on the official realms, instead of three minutes
-- Players of the zone hold quest phases and no longer the default one: he is placed in all of them so that he is seen whatever the quests done
UPDATE `creature` SET `spawntimesecs` = 900, `phaseMask` = 65535, `PhaseId` = '2002 2004 2006 2008 2010 2012 2014 2016 2018 2020 2022 2024 2026 2028 2030 2032 2034 2036 2038 2040 2042 2044 2046 6546' WHERE `id` = 94015 AND `guid` = 297197;
