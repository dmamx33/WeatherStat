/**
 * @Author: Daniel Murrieta M.Sc. Student FH Aachen - CIDESI <daniel>
 * @Date:   2020-05-02T23:44:59+02:00
 * @Email:  daniel.murrieta-alvarez@alumni.fh-aachen.de
 * @Filename: RoboMowerTime.cpp
 * @Last modified by:   daniel
 * @Last modified time: 2021-01-10T23:12:11+01:00
 * @License: CC by-sa
 */

 #include <Arduino.h>
//#include <TimeLib.h>
//#include <DS1307RTC.h>
// #include "RTClib.h"
 #include "WeatherStat_RTC.h"

// //CON BIBLIOTECA DE1307 NO SE USA MAS
// void Read_RTC(uint8_t* objeto_fecha)
// {
//         RTC_DS1307 RTC;
//         DateTime now = RTC.now();
//
// //         tmElements_t tm;
// //         RTC.read(tm);
//         objeto_fecha[SECONDS]= now.second();
//         objeto_fecha[MINUTES]= now.minute();
//         objeto_fecha[HOURS]= now.hour();
// //         objeto_fecha[WEEKDAY]= tm.Wday;
//         objeto_fecha[DAY]= now.day();
//         objeto_fecha[MONTH]= now.month();
//         objeto_fecha[YEAR]= now.year()-208;
// // //tm.Hour =
// //
// }
//
// void Transfer_to_the_right(uint8_t* Source,uint8_t* Receipt)
// {
//
//         for (uint8_t i = 0; i < 6; i++) {
//                 Receipt[i]=Source[i];
//         }
//         return;
// }
// ////////////////////////////////////////////////////////////////////
// boolean Check_Timer(uint8_t*timer){
//
//         TIMEDATE_OBJECT current;
//
//         Read_RTC(current);
//
//         if(current[MONTH]==timer[MONTH]) {
//                 if(current[DAY]==timer[DAY]) {
//                         if (timer[HOURS]<=current[HOURS]) {
//                                 if (timer[MINUTES]<=current[MINUTES]) {
//                                         return true;
//                                 } else return false;
//                         } else return false;
//                 }
//                 else return false;
//         }
//         else return false;
//
//         return false;
// }
// ////////////////////////////////////////////////////////////////////
// void Clear_Timer(uint8_t*timer){
//   for (uint8_t i = 0; i < 6; i++) {
//           timer[i]=0;
//   }
//   return;
// }
// //CON BIBLIOTECA DE1307 NO SE USA MAS
// // void Read_RTC(uint8_t* objeto_fecha)
// // {
// //         tmElements_t tm;
// //         RTC.read(tm);
// //         objeto_fecha[SECONDS]= tm.Second;
// //         objeto_fecha[MINUTES]= tm.Minute;
// //         objeto_fecha[HOURS]= tm.Hour;
// //         objeto_fecha[WEEKDAY]= tm.Wday;
// //         objeto_fecha[DAY]= tm.Day;
// //         objeto_fecha[MONTH]= tm.Month;
// //         objeto_fecha[YEAR]= tm.Year-30;
// // //tm.Hour =
// //
// // }
