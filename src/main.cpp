#include <Arduino.h>

#include "PCF8575.h"

//  adjust addresses if needed
PCF8575 A(0x24);
PCF8575 B(0x22);
PCF8575 C(0x20);

PCF8575 PCF[3] = { A, B, C };


void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("PCF8575_LIB_VERSION:\t");
  Serial.println(PCF8575_LIB_VERSION);

  Wire.begin();

  PCF[2].write(0, HIGH);
  PCF[2].write(1, LOW);
  PCF[2].write(2, LOW);
  PCF[2].write(3, LOW);
  PCF[2].write(4, LOW);
  PCF[2].write(5, LOW);
  PCF[2].write(6, LOW);
  PCF[2].write(7, LOW);

  PCF[0].write(0, LOW);
  PCF[0].write(1, LOW);
  PCF[0].write(2, LOW);
  PCF[0].write(3, LOW);
  PCF[0].write(4, LOW);
  PCF[0].write(5, LOW);
  PCF[0].write(6, LOW);
  PCF[0].write(7, LOW);

  PCF[1].write(0, LOW);
  PCF[1].write(1, LOW);
  PCF[1].write(2, LOW);
  PCF[1].write(3, LOW);
  PCF[1].write(4, LOW);
  PCF[1].write(5, LOW);
  PCF[1].write(6, LOW);
  PCF[1].write(7, LOW);  

}


void loop()
{
  
  PCF[0].write(2, HIGH);
  delay(100);
  PCF[0].write(2, LOW);
  PCF[0].write(6, HIGH);
  delay(100);
  PCF[0].write(6, LOW);

  PCF[1].write(2, HIGH);
  delay(100);
  PCF[1].write(2, LOW);
  PCF[1].write(6, HIGH);
  delay(100);
  PCF[1].write(6, LOW);

  PCF[2].write(2, HIGH);
  delay(100);
  PCF[2].write(2, LOW);
  PCF[2].write(6, HIGH);
  delay(100);
  PCF[2].write(6, LOW);


}