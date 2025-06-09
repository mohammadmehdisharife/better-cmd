#include <windows.h>
#include <stdio.h>
#include "../libs/ansi_colors.h"

int main()
{
    ULONGLONG uptime_ms = GetTickCount64();

    ULONGLONG total_seconds = uptime_ms / 1000;
    ULONGLONG milliseconds = uptime_ms % 1000;

    const unsigned int seconds_per_minute = 60;
    const unsigned int seconds_per_hour = 3600;
    const unsigned int seconds_per_day = 86400;           // 24 * 3600
    const unsigned int seconds_per_month = 2592000;       // 30 * 86400
    const unsigned int seconds_per_year = 31536000;       // 365 * 86400

    unsigned int years = (unsigned int)(total_seconds / seconds_per_year);
    total_seconds %= seconds_per_year;

    unsigned int months = (unsigned int)(total_seconds / seconds_per_month);
    total_seconds %= seconds_per_month;

    unsigned int days = (unsigned int)(total_seconds / seconds_per_day);
    total_seconds %= seconds_per_day;

    unsigned int hours = (unsigned int)(total_seconds / seconds_per_hour);
    total_seconds %= seconds_per_hour;

    unsigned int minutes = (unsigned int)(total_seconds / seconds_per_minute);

    unsigned int seconds = (unsigned int)(total_seconds % seconds_per_minute);

    printf(ANSI_BOLD_YELLOW "Year        : " ANSI_BOLD_WHITE "%u\n" ANSI_RESET, years);
    printf(ANSI_BOLD_YELLOW "Month       : " ANSI_BOLD_WHITE "%u\n" ANSI_RESET, months);
    printf(ANSI_BOLD_YELLOW "Day         : " ANSI_BOLD_WHITE "%u\n" ANSI_RESET, days);
    printf(ANSI_BOLD_YELLOW "Hour        : " ANSI_BOLD_WHITE "%u\n" ANSI_RESET, hours);
    printf(ANSI_BOLD_YELLOW "Minute      : " ANSI_BOLD_WHITE "%u\n" ANSI_RESET, minutes);
    printf(ANSI_BOLD_YELLOW "Second      : " ANSI_BOLD_WHITE "%u\n" ANSI_RESET, seconds);
    printf(ANSI_BOLD_YELLOW "Millisecond : " ANSI_BOLD_WHITE "%llu\n" ANSI_RESET, milliseconds);

    return 0;
}
