/**
 * @Author: Daniel Murrieta M.Sc. Student FH Aachen - CIDESI <daniel>
 * @Date:   2021-01-03T00:56:38+01:00
 * @Email:  daniel.murrieta-alvarez@alumni.fh-aachen.de
 * @Filename: main.cpp
 * @Last modified by:   daniel
 * @Last modified time: 2021-03-10T02:35:36+01:00
 * @License: CC by-sa
 */
/////////////################# headers #####################////////////////
#include <Arduino.h>
//#include <RTClib.h>
#include <Wire.h>
// #include <Adafruit_Sensor.h>
// #include "Adafruit_BME680.h"
#include "bsec.h"
//#include "esp_task_wdt.h"
//#include "soc/timer_group_struct.h"
//#include "soc/timer_group_reg.h"
//#include "WeatherStat_RTC.h"
//#include <DS3231.h>

/////////////################# directives #####################////////////////
#define Number_susc_sens 11
/////////////################# variables #####################////////////////
//char daysOfTheWeek[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
const byte led_gpio = 32; // the PWM pin the LED is attached to
int PWMchannel = 0;
int i = 0;
char flag=0;
boolean flag_inter_loop=false;
/////////////################# functions #####################////////////////
void loop2(void *parameter);
void feedTheDog(void);
void hw_wdt_disable(void);
void checkIaqSensorStatus(void);
/////////////################# handlers definition #####################///////
/////////////################# Web Specific #####################///////
/////////////################# Arduino sh%& #####################///////////////
TaskHandle_t Task2;
//RTC_DS3231 rtc;
Bsec iaqSensor;
String output;

/////////////################# SETUP #####################////////////////
void setup() {
        Serial.begin(115200);
        Serial.println("---### Entrando en setup");
        Wire.begin();
/////RTC begin and check
        // if (!rtc.begin()) {
        //         Serial.println("Couldn't find RTC");
        //         while (1);
        // }
        // else{
        //         rtc.adjust(DateTime(__DATE__, __TIME__));
        //         Serial.println("RTC Found");
        // }
/////PWM Settings
        ledcAttachPin(led_gpio, PWMchannel); // assign a led pins to a channel
        ledcSetup(PWMchannel, 10000, 10); // 12 kHz PWM, 8-bit resolution
/////Enabling Multicore program execution
        xTaskCreatePinnedToCore(loop2,"Task_2",1000,NULL,1,&Task2,0);
        //hw_wdt_disable();
/////Initialize BME680 Sensor
        iaqSensor.begin(BME680_I2C_ADDR_SECONDARY, Wire);
        output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
        Serial.println(output);
        checkIaqSensorStatus();

        bsec_virtual_sensor_t sensorList[Number_susc_sens] = {
                BSEC_OUTPUT_RAW_TEMPERATURE,
                BSEC_OUTPUT_RAW_PRESSURE,
                BSEC_OUTPUT_RAW_HUMIDITY,
                BSEC_OUTPUT_RAW_GAS,
                BSEC_OUTPUT_IAQ,
                BSEC_OUTPUT_STATIC_IAQ,
                BSEC_OUTPUT_CO2_EQUIVALENT,
                BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
                BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
                BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
                BSEC_OUTPUT_GAS_PERCENTAGE
        };
        iaqSensor.updateSubscription(sensorList, Number_susc_sens, BSEC_SAMPLE_RATE_LP);
        checkIaqSensorStatus();
        Serial.println("------### Entrando en loops");
}
/////////////################# LOOP Executed in Core 0 #####################////////////////
void loop2(void *parameter) {

        while(1) {
                yield();
                Serial.print("---### LOOP2");
                Serial.println(i);//
                if(flag_inter_loop) {
                        //esp_task_wdt_reset();//funktioniert nicht
                        //feedTheDog();
                        //yield();// Anti-Hund
                        Serial.println("Leyendo RTC...");//
                        // DateTime now = rtc.now();
                        // Serial.println("RTC Leido...");//
                        // Serial.print(now.hour(), DEC);
                        // Serial.print(":");
                        // Serial.print(now.minute(), DEC);
                        // Serial.print(":");
                        // Serial.println(now.second(), DEC);
                        // Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
                        // Serial.print(" ");
                        // Serial.print(now.day(), DEC);
                        // Serial.print("-");
                        // Serial.print(now.month(), DEC);
                        // Serial.print("-");
                        // Serial.println(now.year(), DEC);
                        // Serial.print("Temp:");
                        // Serial.println(rtc.getTemperature());
                        Serial.println("Loop2 says: Core "+String(xPortGetCoreID()));
                        flag_inter_loop=false;
                        //vTaskDelay(10);///funktioniert nicht
                }
        }


}
/////////////################# LOOP Executed in Core 1 #####################////////////////
void loop() {
      //Serial.print("---------### LOOP1");
        //Serial.println(i);//
        //yield();
        ledcWrite(PWMchannel, i);
        //delay(100);
        if (!flag) {
                i++;
                delay(2);
        }

        else{
                i--;
                delay(2);
        }

        if(i==1023) {
                flag=1;
                delay(100);
        }
        if(i==0) {
                flag=0;
                ledcWrite(PWMchannel, 0);
                yield();
                delay(800);
                Serial.println("Loop1 says: Core "+String(xPortGetCoreID()));
                flag_inter_loop=true;
        }
        //Serial.println("####Saliendo loop()");//
}
/////////////################# funciton  #####################////////////////
void feedTheDog(){
        // feed dog 0
        // TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE; // write enable
        // TIMERG0.wdt_feed=1;                       // feed dog
        // TIMERG0.wdt_wprotect=0;                   // write protect
        // // feed dog 1
        // TIMERG1.wdt_wprotect=TIMG_WDT_WKEY_VALUE; // write enable
        // TIMERG1.wdt_feed=1;                       // feed dog
        // TIMERG1.wdt_wprotect=0;                   // write protect
}
/////////////################# funciton  #####################////////////////
void hw_wdt_disable(){
        *((volatile uint32_t*) 0x60000900) &= ~(1); // Hardware WDT OFF
}
/////////////################# funciton  #####################////////////////
void checkIaqSensorStatus(void)
{
        if (iaqSensor.status != BSEC_OK) {
                if (iaqSensor.status < BSEC_OK) {
                        output = "BSEC error code : " + String(iaqSensor.status);
                        Serial.println(output);
                        for (;;);
                        //errLeds(); /* Halt in case of failure */
                } else {
                        output = "BSEC warning code : " + String(iaqSensor.status);
                        Serial.println(output);
                }
        }

}
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
/////////////################# funciton  #####################////////////////
