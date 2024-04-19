#include "datetime.h"
#include <iomanip>
#include <sstream>
#include <iostream>

namespace datetime {

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second) {
    this->year = year;
    this->month = month;
    this->day = day;
    this->hour = hour;
    this->minute = minute;
    this->second = second;
}

DateTime::DateTime(const ponystring& datetimeStr) {
    std::istringstream ss(datetimeStr.str());
    char discard;
    ss >> year >> discard >> month >> discard >> day >> hour >> discard >> minute >> discard >> second;
}

ponystring DateTime::toString() {
    std::ostringstream oss;
    oss << std::setfill('0');
    oss << std::setw(4) << year << "-" << std::setw(2) << month << "-" << std::setw(2) << day << " ";
    oss << std::setw(2) << hour << ":" << std::setw(2) << minute << ":" << std::setw(2) << second;
    return ponystring(oss.str());
}

DateTime now() {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    return DateTime(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
}

DateTime today() {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    return DateTime(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, 0, 0, 0);
}

} // 
