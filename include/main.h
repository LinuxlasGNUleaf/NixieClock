#include <Arduino.h>
#include <PCF8575.h>
#include <Wire.h>
#include <SPI.h>
#include <RTClib.h>

void sendDigits(PCF8575 expander, byte low_val, byte high_val);
void rotateAllDigits();