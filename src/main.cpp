#include <main.h>

PCF8575 hourPCF(0x24);
PCF8575 minPCF(0x22);
PCF8575 secPCF(0x20);

RTC_DS1307 rtc;

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  Serial.print("Connecting to RTC...");
  if (rtc.begin())
  {
    Serial.println("done.");
  }
  {
    Serial.println("failed.\nPlease check your wiring.");
  }
  if (!rtc.isrunning())
    Serial.println("Clock is not running!");

  Serial.println("Connecting to PCFs...");
  if (!hourPCF.isConnected())
    Serial.println("Hours PCF is NOT connected!");
  if (!minPCF.isConnected())
    Serial.println("Minutes PCF is NOT connected!");
  if (!secPCF.isConnected())
    Serial.println("Seconds PCF is NOT connected!");
  
  Serial.print("Rotating all digits...");
  hourPCF.selectNone();
  minPCF.selectNone();
  secPCF.selectNone();
  delay(1000);
  rotateAllDigits();
  Serial.println("done.");
  disableAllDigits();
}

void loop()
{
}

void sendDigits(PCF8575 expander, byte low_val, byte high_val)
{
  low_val = low_val < 31 ? low_val : 31;
  high_val = high_val < 31 ? high_val : 31;
  expander.write16((uint16_t)(low_val << 4) | high_val);
}

void rotateAllDigits()
{
  for (byte i = 0; i <= 9; i++)
  {
    sendDigits(hourPCF, i, i);
    delay(1000);
  }
}

void disableAllDigits()
{
  sendDigits(hourPCF,0x1F,0x1F);
  sendDigits(minPCF,0x1F,0x1F);
  sendDigits(secPCF,0x1F,0x1F);
}