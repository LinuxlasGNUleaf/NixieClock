#include <main.h>

PCF8575 hourPCF(0x24);
PCF8575 minPCF(0x22);
PCF8575 secPCF(0x20);

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  hourPCF.selectNone();
  minPCF.selectNone();
  secPCF.selectNone();

  delay(1000);
  rotateAllDigits();
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