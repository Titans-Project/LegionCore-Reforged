////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////

#include "CalendarAnnouncements.h"
#include "Common.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "World.h"

#include <algorithm>
#include <ctime>
#include <sstream>
#include <string>
#include <vector>

// Calendar announcements - raid wings, dungeons, zones opening - are described in a single world
// table: a name, a date, and Enabled set to 1. The client, however, only knows how to read
// holidays: three hotfix tables plus one announcement per record in `hotfix_data`. This file does
// the translation, once per startup.
//
// Why through the database rather than by patching the stores in memory: C++ can only *amend* a
// record the client's own .db2 already holds. These holidays are not in it, so they have to be
// *created*, and only a hotfix table can do that.
namespace
{
    // Hashes read from the header of each .db2, at offset 20. A made-up value would have the
    // announcement dropped without a word: the core would send it under a table the client does
    // not know.
    uint32 const HASH_HOLIDAYS     = 1972272076;
    uint32 const HASH_NAMES        = 1419319386;
    uint32 const HASH_DESCRIPTIONS =  690463935;

    // Range reserved for the announcements produced here. It is wiped and rewritten on every
    // startup, so no hand-written row may live in it.
    uint32 const HOTFIX_ID_FIRST = 9010000;
    uint32 const HOTFIX_ID_LAST  = 9012999;

    uint32 const BUILD = 26972;

    // The file holds sixteen dates but only ten durations and ten calendar flags, and a date
    // without its flag does not show.
    uint32 const MAX_OCCURRENCES = 10;

    // The client's packed date, the one ByteBuffer::AppendPackedTime writes.
    uint32 PackDate(tm const& t)
    {
        return uint32((t.tm_year - 100) << 24 | t.tm_mon << 20 | (t.tm_mday - 1) << 14
                    | t.tm_wday << 11 | t.tm_hour << 6 | t.tm_min);
    }

    std::string Escape(std::string const& s)
    {
        std::string out;
        out.reserve(s.size() + 8);
        for (char c : s)
        {
            if (c == '\'' || c == '\\')
                out.push_back('\\');
            out.push_back(c);
        }
        return out;
    }
}

void CalendarAnnouncements::Publish()
{
    // The server's locale decides what a player reads. English goes into the base table, which is
    // the universal fallback; the configured locale goes into the locale table, where the client
    // looks first. An English realm therefore writes only once.
    LocaleConstant locale = LocaleConstant(sConfigMgr->GetIntDefault("DBC.Locale", 0));
    if (locale >= MAX_LOCALES || locale == LOCALE_none)
        locale = LOCALE_enUS;
    bool const translate = locale != LOCALE_enUS;

    QueryResult result = WorldDatabase.Query(
        "SELECT HolidayID, HolidayNameID, HolidayDescriptionID, NameLocalized, DescriptionLocalized, "
        "UNIX_TIMESTAMP(StartDate), DurationDays, Texture1, Texture2, Texture3, Enabled, GameEventID, "
        "Name, Description, RepeatDays, UNIX_TIMESTAMP(EndDate), RepeatMonths "
        "FROM custom_calendar_event ORDER BY HolidayID");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Calendar: no announcement to publish.");
        return;
    }

    std::vector<uint32> allHolidays, allNames, allDescriptions;
    std::ostringstream vHolidays, vNames, vNamesLoc, vDescs, vDescsLoc, vHotfix;
    uint32 published = 0;
    uint32 translated = 0;
    uint32 undated = 0;

    do
    {
        Field* f = result->Fetch();
        uint32 holidayId   = f[0].GetUInt32();
        uint32 nameId      = f[1].GetUInt32();
        uint32 descId      = f[2].GetUInt32();
        std::string name   = f[3].GetString();
        std::string desc   = f[4].GetString();
        bool hasStart      = !f[5].IsNull();
        time_t start       = hasStart ? time_t(f[5].GetUInt64()) : time_t(0);
        uint32 days        = f[6].GetUInt32();
        uint32 tex1        = f[7].GetUInt32();
        uint32 tex2        = f[8].GetUInt32();
        uint32 tex3        = f[9].GetUInt32();
        bool enabled       = f[10].GetUInt8() != 0;
        uint32 gameEvent   = f[11].GetUInt32();
        std::string nameEN = f[12].GetString();
        std::string descEN = f[13].GetString();
        uint32 repeat      = f[14].GetUInt32();
        bool hasEnd        = !f[15].IsNull();
        time_t end         = hasEnd ? time_t(f[15].GetUInt64()) : time_t(0);
        uint32 repeatMonths = f[16].GetUInt32();

        // A game event can follow the announcement's dates: `game_event.holiday` names a holiday,
        // and GameEventMgr reads its dates to know when to fire. The link is set while the
        // announcement is enabled and cleared otherwise, so the event is never left bound to a
        // holiday we just emptied.
        //
        // Direct: `game_event` is read later in this same startup, a deferred write would land
        // too late.
        if (gameEvent)
            WorldDatabase.DirectPExecute("UPDATE game_event SET holiday = %u WHERE eventEntry = %u",
                enabled ? holidayId : 0, gameEvent);

        // Everything is wiped, disabled rows included: an announcement taken down has to leave the
        // calendar rather than linger with its old date.
        allHolidays.push_back(holidayId);
        allNames.push_back(nameId);
        allDescriptions.push_back(descId);

        if (!enabled)
            continue;

        if (!hasStart)
        {
            TC_LOG_WARN("server.loading", "Calendar: announcement %u (%s) is enabled with no date, skipped.",
                holidayId, name.c_str());
            ++undated;
            continue;
        }

        // Three ways to state a length, in this order: an end date, a number of days, or nothing
        // at all - the announcement then only marks its opening day. The table reads in days, the
        // client's file counts in hours.
        uint32 duration = days * 24;
        if (hasEnd)
        {
            if (days)
                TC_LOG_WARN("server.loading", "Calendar: announcement %u has both an end date and a "
                    "duration; the end date wins, set DurationDays to 0.", holidayId);

            if (end > start)
                duration = uint32((end - start) / HOUR);
            else
            {
                TC_LOG_WARN("server.loading", "Calendar: announcement %u ends before it starts; "
                    "its duration is ignored.", holidayId);
                duration = 0;
            }
        }
        else if (repeat && !days)
            TC_LOG_WARN("server.loading", "Calendar: announcement %u repeats every %u days with no "
                "duration; each occurrence will only mark its opening day.", holidayId, repeat);

        // A holiday carries several dates: that is how Blizzard's own come back. We lay down as
        // many as the file accepts, `RepeatDays` apart. At zero, a single one.
        uint32 dates[MAX_OCCURRENCES] = { };
        uint32 occurrences = 0;
        for (uint32 k = 0; k < (repeat || repeatMonths ? MAX_OCCURRENCES : 1u); ++k)
        {
            // counted in calendar days (or months: `RepeatMonths`), so that a date past a daylight saving
            // change keeps its hour
            tm lt;
            localtime_r(&start, &lt);
            if (repeatMonths)
                lt.tm_mon += int(k * repeatMonths);
            else
                lt.tm_mday += int(k * repeat);
            lt.tm_isdst = -1;
            mktime(&lt);
            dates[k] = PackDate(lt);
            ++occurrences;
        }

        if (published)
        {
            vHolidays << ",";
            vNames << ",";
            vDescs << ",";
            vHotfix << ",";
        }

        // `CalendarFlags1 = 3` is what makes a date visible: read off every Blizzard holiday that
        // shows up. At zero the client accepts the record and then has nothing to draw. `Flags = 13`
        // and `CalendarFilterType = 0` come from the PvP Brawls, the closest thing to a one-off
        // announcement. Durations and calendar flags are not one per date but the phases of each
        // occurrence, one after the other: only the first is set, as in Blizzard's timewalking
        // holidays, or every date would come back as many times as there are durations.
        vHolidays << "(" << holidayId;
        for (uint32 k = 0; k < MAX_OCCURRENCES; ++k)
            vHolidays << "," << dates[k];                            // Date1..Date10
        for (uint32 k = 0; k < MAX_OCCURRENCES; ++k)
            vHolidays << "," << (k ? 0 : duration);                   // Duration1..Duration10
        vHolidays << ",0,0";                                         // Region, Looping
        for (uint32 k = 0; k < MAX_OCCURRENCES; ++k)
            vHolidays << "," << (k ? 0 : 3);                          // CalendarFlags1..10
        vHolidays << ",0,0,13," << nameId << "," << descId << ","
                  << tex1 << "," << tex2 << "," << tex3 << "," << BUILD << ")";

        // English is the base; failing that, the translated wording, so a row with no English
        // version still shows something rather than nothing.
        std::string const& baseName = nameEN.empty()  ? name : nameEN;
        std::string const& baseDesc = descEN.empty() ? desc : descEN;

        vNames << "(" << nameId  << ",'" << Escape(baseName) << "'," << BUILD << ")";
        vDescs << "(" << descId << ",'" << Escape(baseDesc) << "'," << BUILD << ")";

        if (translate)
        {
            if (translated)
            {
                vNamesLoc << ",";
                vDescsLoc << ",";
            }
            vNamesLoc << "(" << nameId  << ",'" << localeNames[locale] << "','" << Escape(name) << "'," << BUILD << ")";
            vDescsLoc << "(" << descId << ",'" << localeNames[locale] << "','" << Escape(desc) << "'," << BUILD << ")";
            ++translated;
        }

        uint32 base = HOTFIX_ID_FIRST + published * 3;
        vHotfix << "(" << base     << "," << HASH_HOLIDAYS     << "," << holidayId << ",0,0),"
                << "(" << base + 1 << "," << HASH_NAMES        << "," << nameId    << ",0,0),"
                << "(" << base + 2 << "," << HASH_DESCRIPTIONS << "," << descId    << ",0,0)";

        ++published;
    }
    while (result->NextRow());

    auto join = [](std::vector<uint32> const& v)
    {
        std::ostringstream s;
        for (size_t i = 0; i < v.size(); ++i)
            s << (i ? "," : "") << v[i];
        return s.str();
    };

    std::string const idsHolidays = join(allHolidays);
    std::string const idsNames    = join(allNames);
    std::string const idsDescs    = join(allDescriptions);

    HotfixDatabaseTransaction trans = HotfixDatabase.BeginTransaction();
    trans->PAppend("DELETE FROM holidays WHERE ID IN (%s)", idsHolidays.c_str());
    trans->PAppend("DELETE FROM holiday_names WHERE ID IN (%s)", idsNames.c_str());
    trans->PAppend("DELETE FROM holiday_names_locale WHERE ID IN (%s)", idsNames.c_str());
    trans->PAppend("DELETE FROM holiday_descriptions WHERE ID IN (%s)", idsDescs.c_str());
    trans->PAppend("DELETE FROM holiday_descriptions_locale WHERE ID IN (%s)", idsDescs.c_str());
    trans->PAppend("DELETE FROM hotfix_data WHERE Id BETWEEN %u AND %u", HOTFIX_ID_FIRST, HOTFIX_ID_LAST);

    if (published)
    {
        trans->PAppend("INSERT INTO holidays (ID,"
            "Date1,Date2,Date3,Date4,Date5,Date6,Date7,Date8,Date9,Date10,"
            "Duration1,Duration2,Duration3,Duration4,Duration5,Duration6,Duration7,Duration8,Duration9,Duration10,"
            "Region,Looping,"
            "CalendarFlags1,CalendarFlags2,CalendarFlags3,CalendarFlags4,CalendarFlags5,"
            "CalendarFlags6,CalendarFlags7,CalendarFlags8,CalendarFlags9,CalendarFlags10,"
            "Priority,CalendarFilterType,Flags,HolidayNameID,HolidayDescriptionID,"
            "TextureFileDataID1,TextureFileDataID2,TextureFileDataID3,VerifiedBuild) VALUES %s",
            vHolidays.str().c_str());
        trans->PAppend("INSERT INTO holiday_names (ID,Name,VerifiedBuild) VALUES %s",
            vNames.str().c_str());
        if (translated)
            trans->PAppend("INSERT INTO holiday_names_locale (ID,locale,Name_lang,VerifiedBuild) VALUES %s",
                vNamesLoc.str().c_str());
        trans->PAppend("INSERT INTO holiday_descriptions (ID,Description,VerifiedBuild) VALUES %s",
            vDescs.str().c_str());
        if (translated)
            trans->PAppend("INSERT INTO holiday_descriptions_locale (ID,locale,Description_lang,VerifiedBuild) VALUES %s",
                vDescsLoc.str().c_str());
        trans->PAppend("INSERT INTO hotfix_data (Id,TableHash,RecordID,Timestamp,Deleted) VALUES %s",
            vHotfix.str().c_str());
    }

    HotfixDatabase.DirectCommitTransaction(trans);

    // The client asks for nothing until this counter moves, and it does not know our announcements
    // before asking. Both rows of `version` have to follow.
    WorldDatabase.DirectExecute("UPDATE version SET hotfix_cache_id = hotfix_cache_id + 1");

    TC_LOG_INFO("server.loading", ">> Calendar: %u announcement(s) published in %s%s%s.", published,
        localeNames[locale], translated ? " (English as fallback)" : "",
        undated ? ", some enabled rows have no date" : "");
}

time_t GreatGnomereganRun::GetCurrentStart()
{
    uint32 const months = std::max<uint32>(sWorld->getIntConfig(CONFIG_GNOMEREGAN_RUN_MONTHS), 1);
    uint32 const days = std::max<uint32>(sWorld->getIntConfig(CONFIG_GNOMEREGAN_RUN_DAYS), 1);
    time_t const now = time(nullptr);

    // from last year's 14th of October, step by step up to the occurrence not yet over
    tm t;
    localtime_r(&now, &t);
    tm first = { };
    first.tm_year = t.tm_year - 1;
    first.tm_mon = 9;
    first.tm_mday = 14;
    first.tm_isdst = -1;
    time_t start = mktime(&first);
    while (start + time_t(days) * DAY <= now)
        start = GetNextStart(start);
    return start;
}

time_t GreatGnomereganRun::GetNextStart(time_t start)
{
    tm t;
    localtime_r(&start, &t);
    t.tm_mon += int(std::max<uint32>(sWorld->getIntConfig(CONFIG_GNOMEREGAN_RUN_MONTHS), 1));
    t.tm_isdst = -1;
    return mktime(&t);
}

void GreatGnomereganRun::PrepareCalendar()
{
    // Blizzard's dates are already the client's own: the realm's entry only shows when they differ
    uint32 const months = sWorld->getIntConfig(CONFIG_GNOMEREGAN_RUN_MONTHS);
    uint32 const days = sWorld->getIntConfig(CONFIG_GNOMEREGAN_RUN_DAYS);
    bool const custom = months != 12 || days != 1;
    WorldDatabase.DirectPExecute("UPDATE custom_calendar_event SET StartDate = FROM_UNIXTIME(%u), EndDate = NULL, DurationDays = %u, "
        "RepeatDays = 0, RepeatMonths = %u, Enabled = %u WHERE HolidayID = %u", uint32(GetCurrentStart()), days, months, uint32(custom), HOLIDAY_ID);
}

void DarkmoonFaire::PrepareCalendar()
{
    // outside the range Publish wipes and rewrites
    uint32 const HOTFIX_ID = 9100223;

    time_t const now = time(nullptr);
    tm today;
    localtime_r(&now, &today);

    std::ostringstream dates;
    for (int k = 0; k < 16; ++k)
    {
        tm day = {};
        day.tm_year = today.tm_year;
        day.tm_mon = today.tm_mon + k;
        day.tm_mday = 1;
        day.tm_isdst = -1;
        mktime(&day);
        // the Friday before the first Sunday of the month
        day.tm_mday += (7 - day.tm_wday) % 7 - 2;
        day.tm_isdst = -1;
        mktime(&day);
        dates << ',' << PackDate(day);
    }

    HotfixDatabaseTransaction trans = HotfixDatabase.BeginTransaction();
    trans->PAppend("DELETE FROM holidays WHERE ID = %u", HOLIDAY_ID);
    trans->PAppend("INSERT INTO holidays (ID,"
        "Date1,Date2,Date3,Date4,Date5,Date6,Date7,Date8,Date9,Date10,Date11,Date12,Date13,Date14,Date15,Date16,"
        "Duration1,Duration2,Duration3,Duration4,Duration5,Duration6,Duration7,Duration8,Duration9,Duration10,"
        "Region,Looping,"
        "CalendarFlags1,CalendarFlags2,CalendarFlags3,CalendarFlags4,CalendarFlags5,"
        "CalendarFlags6,CalendarFlags7,CalendarFlags8,CalendarFlags9,CalendarFlags10,"
        "Priority,CalendarFilterType,Flags,HolidayNameID,HolidayDescriptionID,"
        "TextureFileDataID1,TextureFileDataID2,TextureFileDataID3,VerifiedBuild) VALUES "
        "(%u%s,48,168,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,1,1,1,1,201,235448,235447,235446,%u)",
        HOLIDAY_ID, dates.str().c_str(), BUILD);
    trans->PAppend("DELETE FROM hotfix_data WHERE Id = %u", HOTFIX_ID);
    trans->PAppend("INSERT INTO hotfix_data (Id,TableHash,RecordID,Timestamp,Deleted) VALUES (%u,%u,%u,0,0)",
        HOTFIX_ID, HASH_HOLIDAYS, HOLIDAY_ID);
    HotfixDatabase.DirectCommitTransaction(trans);
}
