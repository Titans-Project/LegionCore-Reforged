-- Moving holidays: the client reads year 31 as every year, so the 2031 date showed a second start each year; dates stop at 2030
UPDATE `holidays` SET `Date12` = 0, `Date13` = 0, `Date14` = 0, `Date15` = 0, `Date16` = 0 WHERE `ID` IN (181,201,321,327,404,423);
