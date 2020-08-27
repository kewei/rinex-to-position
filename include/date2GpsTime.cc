#include <vector>
#include <cmath>
#include "date2GpsTime.h"

std::vector<long> date2GpsTime(int year, int month, int day, int hour, int minute, float second)
{
    int start_gps = 1980; // year
    int years_elapse = year - start_gps;
    int year_366 = 0; // leap year with 366 days
    std::vector<long> gpsTime;
    for (int i=start_gps; i<year; i++)
    {
        if (i%4 == 0)
        {
            if (i%100 != 0)
            {
                year_366 = year_366 + 1;
            }
            else if (i%400 == 0)
            {
                year_366 = year_366 + 1;
            }
        }
    }

    int num_days_year_366 = year_366 * 366;
    int num_days_all_year = num_days_year_366 + (years_elapse - year_366) * 365; 
    int num_days_until_last_year = num_days_all_year - 5; // gps time starts from midnight Jan 5-6 1980

    int Feb = 28;
    int days_current_year = 0;
    if (year%4 == 0)
        {
            if (year%100 != 0)
            {
                Feb = 29;
            }
            else if (year%400 == 0)
            {
                Feb = 29;
            }
        }
    switch (month)
    {
    case 1:
        days_current_year = day - 1;
        break;
    case 2:
        days_current_year = 31 + day - 1;
        break;
    case 3:
        days_current_year = 31 + Feb + day - 1;
        break;
    case 4:
        days_current_year = 31 + Feb + 31 + day - 1;
        break;
    case 5:
        days_current_year = 31 + Feb + 31 + 30 + day - 1;
        break;
    case 6:
        days_current_year = 31 + Feb + 31 + 30 + 31 + day - 1;
        break;
    case 7:
        days_current_year = 31 + Feb + 31 + 30 + 31 + 30 + day - 1;
        break;
    case 8:
        days_current_year = 31 + Feb + 31 + 30 + 31 + 30 + 31 + day - 1;
        break;
    case 9:
        days_current_year = 31 + Feb + 31 + 30 + 31 + 30 + 31 + 31 + day - 1;
        break;
    case 10:
        days_current_year = 31 + Feb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + day - 1;
        break;
    case 11:
        days_current_year = 31 + Feb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + day - 1;
        break;
    case 12:
        days_current_year = 31 + Feb + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + day - 1;
        break;
    default:
        break;
    }

    int secGps = ((num_days_until_last_year + days_current_year) % 7) * 24 * 3600 + hour * 3600 + minute * 60 + int(second);
    int week = int(std::floor((num_days_until_last_year + days_current_year)/7));
    gpsTime.push_back(static_cast<long>(secGps));
    gpsTime.push_back(static_cast<long>(week));
    gpsTime.push_back(static_cast<long>(days_current_year));
    return gpsTime;
}