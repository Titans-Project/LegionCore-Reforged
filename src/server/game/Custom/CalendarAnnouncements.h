////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#ifndef CalendarAnnouncements_h__
#define CalendarAnnouncements_h__

#include "Define.h"
#include <ctime>

// Copies `world.custom_calendar_event` into the three hotfix tables the client reads to fill its
// calendar.
//
// Call this **before** the DB2 stores load: they read their hotfixes at that moment, and anything
// written afterwards would not be seen until the next startup.
namespace CalendarAnnouncements
{
    void Publish();
}

// The Great Gnomeregan Run (holiday 696): from midnight on the 14th of October, every
// Custom.GnomereganRun.Months months, for Custom.GnomereganRun.Days days. Blizzard's own is once a year for one
// day, the date of the client's calendar; a fixed period in minutes would drift with the leap years.
namespace GreatGnomereganRun
{
    uint32 const HOLIDAY_ID = 696;
    // the running occurrence, or the next one
    time_t GetCurrentStart();
    time_t GetNextStart(time_t start);
    // its calendar entry, when the realm departs from Blizzard's dates; to run before Publish
    void PrepareCalendar();
}

// The Darkmoon Faire (holiday 479): a week from the first Sunday of each month. The client's own
// sixteen dates ended in 2018; the next sixteen months are written at each startup, in Blizzard's
// layout (a hidden two-day setup from the Friday, then the week of the faire).
namespace DarkmoonFaire
{
    uint32 const HOLIDAY_ID = 479;
    // to run before the DB2 stores load
    void PrepareCalendar();
}

#endif // CalendarAnnouncements_h__
