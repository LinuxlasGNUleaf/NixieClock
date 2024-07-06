#include <main.h>

PCF8575 hourPCF(0x24);
PCF8575 minPCF(0x22);
PCF8575 secPCF(0x20);

unsigned long last_millis;
unsigned long current_millis;

TimeElements tm;

void setup()
{
  // begin serial and wire connection
  Serial.begin(9600);
  Wire.begin(0x50);

  // check RTC connection
  Serial.print("Connecting to RTC...");
  delay(500);
  if (RTC.read(tm))
  {
    Serial.println("done.");
  }
  {
    Serial.println("failed.\nPlease check your wiring.");
  }

  // checking whether all PCFs are connected
  Serial.println("Connecting to PCFs...");
  if (!hourPCF.isConnected())
    Serial.println("Hours PCF is NOT connected!");
  if (!minPCF.isConnected())
    Serial.println("Minutes PCF is NOT connected!");
  if (!secPCF.isConnected())
    Serial.println("Seconds PCF is NOT connected!");

  // digit rotation test
  Serial.print("Rotating all digits...");
  hourPCF.selectNone();
  minPCF.selectNone();
  secPCF.selectNone();
  delay(500);
  rotateAllDigits();
  Serial.println("done.");
  disableAllDigits();

  // read RTC and prepare variables
  RTC.read(tm);
  last_millis = 0;
}

void loop()
{
  current_millis = millis();
  if (last_millis + 1000 <= current_millis)
  {
    last_millis = current_millis;
    displayCurrentTime();
  }
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
    sendDigits(minPCF, i, i);
    sendDigits(secPCF, i, i);
    delay(500);
  }
}

void disableAllDigits()
{
  sendDigits(hourPCF, 0x1F, 0x1F);
  sendDigits(minPCF, 0x1F, 0x1F);
  sendDigits(secPCF, 0x1F, 0x1F);
}

void displayCurrentTime()
{
  RTC.read(tm);
  int hours = tm.Hour;
  int mins = tm.Minute;
  int secs = tm.Second;
  sendDigits(hourPCF, hours % 10, hours / 10);
  sendDigits(minPCF, mins % 10, mins / 10);
  sendDigits(secPCF, secs % 10, secs / 10);
}