#include <Arduino.h>
#include <PCF8575.h>
#include <SPI.h>
#include <Wire.h>
#include <DS1307RTC.h>

#define SYNC_INTERVAL_MIN 10


#define SECOND_MILLIS (1000UL)
#define MINUTE_MILLIS (60UL*SECOND_MILLIS)
#define HOUR_MILLIS (60UL*MINUTE_MILLIS)
#define DAY_MILLIS (24UL*HOUR_MILLIS)


void sendDigits(PCF8575 expander, byte low_val, byte high_val);
void rotateAllDigits();
void disableAllDigits();
void displayCurrentTime();
void syncTime();