#include <Arduino.h>
#include <PCF8575.h>
#include <SPI.h>
#include <Wire.h>
#include <DS1307RTC.h>
//#include <RTClib.h>

#define AUTO_SYNC_TICKS 10


void sendDigits(PCF8575 expander, byte low_val, byte high_val);
void rotateAllDigits();
void disableAllDigits();
void displayCurrentTime();
