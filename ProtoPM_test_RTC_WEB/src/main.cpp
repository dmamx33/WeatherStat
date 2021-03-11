/**
 * @Author: Daniel Murrieta M.Sc. Student FH Aachen - CIDESI <daniel>
 * @Date:   2021-01-03T00:56:38+01:00
 * @Email:  daniel.murrieta-alvarez@alumni.fh-aachen.de
 * @Filename: main.cpp
 * @Last modified by:   daniel
 * @Last modified time: 2021-03-11T21:09:34+01:00
 * @License: CC by-sa
 */
/////////////################# headers #####################////////////////
#include <Arduino.h>
//#include <RTClib.h>
#include <Wire.h>
// #include <Adafruit_Sensor.h>
// #include "Adafruit_BME680.h"
#include "bsec.h"
#include <SPIFFS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
//#include "esp_task_wdt.h"
//#include "soc/timer_group_struct.h"
//#include "soc/timer_group_reg.h"
//#include "WeatherStat_RTC.h"
//#include <DS3231.h>
uint8_t bsec_config_iaq[] = {
#include "config/generic_33v_3s_4d/bsec_iaq.txt"
};
/////////////################# directives #####################////////////////
#define Number_susc_sens 11
/////////////################# variables #####################////////////////
//char daysOfTheWeek[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
const byte led_gpio = 32; // the PWM pin the LED is attached to
int PWMchannel = 0;
int i = 0;
char flag=0;
boolean flag_inter_loop=false;
const char * intro = "Time[ms],r_t[°C],p[hPa],r_hum[%],gas[Ohm],IAQ,IAQacc,temp[°C],h[%],S_IAQ,CO2_equ,bre_VOC,Gas%";
String output;
String tempe_web;
String iaq_web;
String press_web;
/////////////################# functions #####################////////////////
void loop2(void *parameter);
void loop1(void *parameter);
void feedTheDog(void);
void hw_wdt_disable(void);
void checkIaqSensorStatus(void);
/////////////################# handlers definition #####################///////
/////////////################# Web Specific #####################///////
const char *ssid = "FRITZ!Box 6591 Cable SW";         // replace with your SSID
const char *password = "62407078731195560963"; // replace with your Password
AsyncWebServer server(80);
/////////////################# Arduino sh%& #####################///////////////
TaskHandle_t Task2;
TaskHandle_t Task1;
//RTC_DS3231 rtc;
Bsec iaqSensor;
/////////////################# SETUP #####################////////////////
void setup() {
        Serial.begin(115200);
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
///SPIFSSS begin and check
        if(!SPIFFS.begin()) {
                Serial.println("An Error has occurred while mounting SPIFFS");
                return;
        }
        else Serial.println("SPIFFS Mounted correctly");
/////PWM Settings
        ledcAttachPin(led_gpio, PWMchannel); // assign a led pins to a channel
        ledcSetup(PWMchannel, 10000, 10); // 12 kHz PWM, 8-bit resolution
////// Wifi Configs
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED)
        {
                delay(1000);
                Serial.print("Connecting to WiFi..");
                Serial.print(".");
        }
        Serial.print("\nConnected to the WiFi network: ");
        Serial.println(WiFi.SSID());
        Serial.print("IP address:");
        Serial.print(WiFi.localIP());
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
        iaqSensor.setState(bsec_config_iaq);
        iaqSensor.updateSubscription(sensorList, Number_susc_sens, BSEC_SAMPLE_RATE_LP);
        checkIaqSensorStatus();
        //Serial.println("------### Entrando en loops");
/////Enabling Multicore program execution
        xTaskCreatePinnedToCore(loop1,"Task_1",10000,NULL,1,&Task1,0);
        //delay(500);
        xTaskCreatePinnedToCore(loop2,"Task_2",10000,NULL,1,&Task2,1);
        delay(500);
        //hw_wdt_disable();

/////Handlers
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
                request->send(SPIFFS, "/index.html");
        });
        // send style.css file from SPIFFS
        server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
                request->send(SPIFFS, "/style.css", "text/css");
        });
        // send script.js file from SPIFFS
        server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
                request->send(SPIFFS, "/script.js", "text/javascript");
        });
        server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
                request->send_P(200, "text/plain", tempe_web.c_str());
        });
        // send BME680 humidity
        server.on("/iaq", HTTP_GET, [](AsyncWebServerRequest *request) {
                request->send_P(200, "text/plain", iaq_web.c_str());
        });
        // server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request) {
        //         request->send_P(200, "text/plain", press_web.c_str());
        // });
/////Initialize Server
        server.begin(); // begin server at port 80
}
/////////////################# LOOP Executed in Core 0 #####################////////////////
void loop2(void *parameter) {

        while(1) {
                yield();
                //Serial.print("---### LOOP2");
                //Serial.println(i);//
                unsigned long time_trigger = millis();
                if (iaqSensor.run()) {   // If new data is available
                        output = String(time_trigger);
                        output += ", " + String(iaqSensor.rawTemperature);
                        press_web = String(iaqSensor.pressure/100);
                        output += ", " + press_web;
                        output += ", " + String(iaqSensor.rawHumidity);
                        output += ", " + String(iaqSensor.gasResistance);
                        iaq_web = String(iaqSensor.iaq);
                        output += ", " + iaq_web;
                        output += ", " + String(iaqSensor.iaqAccuracy);
                        tempe_web = String(iaqSensor.temperature);
                        output += ", " + tempe_web;
                        output += ", " + String(iaqSensor.humidity);
                        output += ", " + String(iaqSensor.staticIaq);
                        output += ", " + String(iaqSensor.co2Equivalent);
                        output += ", " + String(iaqSensor.breathVocEquivalent);
                        output += ", " + String(iaqSensor.gasPercentage);
                        Serial.println(' ');
                        Serial.println(intro);
                        Serial.println(output);
                } else {
                        checkIaqSensorStatus();
                }

                if(flag_inter_loop) {
                        //Serial.println("Loop2 says: Core "+String(xPortGetCoreID()));
                        flag_inter_loop=false;
                        //vTaskDelay(10);///funktioniert nicht
                }
        }


}
/////////////################# LOOP Executed in Core 1 #####################////////////////
void loop1(void *parameter) {
        //Serial.print("---------### LOOP1");
        //Serial.println(i);//
        while(1) {
                yield();
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
                        //Serial.println("Loop1 says: Core "+String(xPortGetCoreID()));
                        flag_inter_loop=true;
                }
        }
        //Serial.println("####Saliendo loop()");//
}
/////////////################# funciton  #####################////////////////
void loop(){
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
                        //for (;;);
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
