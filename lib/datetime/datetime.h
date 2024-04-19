#ifndef DATETIME_H
#define DATETIME_H

#include "ponystring.h"

namespace datetime {

class DateTime {
private:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

public:
    DateTime(int year, int month, int day, int hour, int minute, int second);
    DateTime(const ponystring& datetimeStr);
    ponystring toString();
};

DateTime now();
DateTime today();

} //

#endif /* DATETIME_H */
