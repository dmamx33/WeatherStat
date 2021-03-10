/**
 * @Author: Daniel Murrieta M.Sc. Student FH Aachen - CIDESI <daniel>
 * @Date:   2020-05-02T23:38:27+02:00
 * @Email:  daniel.murrieta-alvarez@alumni.fh-aachen.de
 * @Filename: RoboMowerTime.h
 * @Last modified by:   daniel
 * @Last modified time: 2021-01-03T21:30:27+01:00
 * @License: CC by-sa
 */


#include <Arduino.h>
#ifndef WeatherStat_RTC_h
#define WeatherStat_RTC_h

// #define SECONDS 0
// #define MINUTES 1
// #define HOURS 2
// #define DAY 3
// #define MONTH 4
// #define YEAR 5
//const uint8_t DaysPerMonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};
char daysOfTheWeek[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};

// January - 31 days
// February - 28 days in a common year and 29 days in leap years
// March - 31 days
// April - 30 days
// May - 31 days
// June - 30 days
// July - 31 days
// August - 31 days
// September - 30 days
// October - 31 days
// November - 30 days
// December - 31 days
typedef uint8_t TIMEDATE_OBJECT[6];
typedef uint8_t* TIMEDATE_OBJPTR;

// void Read_RTC(uint8_t* objeto_fecha);
// boolean Check_Timer(uint8_t* );
// void Transfer_to_the_right(uint8_t* Source,uint8_t* Receipt);
// void Clear_Timer(uint8_t*timer);

#endif
