#include <main.h>

PCF8575 hourPCF(0x24);
PCF8575 minPCF(0x22);
PCF8575 secPCF(0x20);

unsigned long last_millis = 0;
unsigned long current_millis = 0;
unsigned long last_sync_millis = 0;

byte last_hours = 0;
long millis_offset = 0;

bool cycle_flag = false;

TimeElements tm;

void setup()
{
  // begin serial and wire connection
  Serial.begin(9600);
  Wire.begin(0x50);

  // check RTC connection
  Serial.print("Connecting to RTC...");
  while (!RTC.read(tm))
  {
    Serial.print('.');
    delay(200);
  }
  Serial.println("done.");

  // checking whether all PCFs are connected
  Serial.println("Connecting to PCFs...");
  if (!hourPCF.isConnected())
    Serial.println("Hours PCF is NOT connected!");
  if (!minPCF.isConnected())
    Serial.println("Minutes PCF is NOT connected!");
  if (!secPCF.isConnected())
    Serial.println("Seconds PCF is NOT connected!");

  disableAllDigits();
  delay(500);

  // read RTC and prepare variables
  syncTime();
  last_millis = 0;
}

void loop()
{
  current_millis = millis();
  if (last_millis + 1000 <= current_millis)
  {
    if (tm.Hour != last_hours)
    {
      rotateAllDigits();
      last_hours = tm.Hour;
    }
    last_millis = millis();
    displayCurrentTime();
  }

  if (last_sync_millis + SYNC_INTERVAL_MIN * MINUTE_MILLIS <= current_millis)
  {
    syncTime();
    last_sync_millis = millis();
  }

  if (cycle_flag)
  {
    rotateAllDigits();
    cycle_flag = false;
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
  Serial.print("Rotating all digits...");
  for (int j = 0; j < 3; j++)
  {
    for (byte i = 0; i <= 9; i++)
    {
      sendDigits(hourPCF, i, i);
      sendDigits(minPCF, i, i);
      sendDigits(secPCF, i, i);
      delay(200);
    }
  }
  Serial.println("done.");
}

void disableAllDigits()
{
  sendDigits(hourPCF, 0x1F, 0x1F);
  sendDigits(minPCF, 0x1F, 0x1F);
  sendDigits(secPCF, 0x1F, 0x1F);
}

void displayCurrentTime()
{
  unsigned long tod_millis = (millis() + millis_offset) % DAY_MILLIS;

  int hours = tod_millis / HOUR_MILLIS;
  if (hours != last_hours)
  {
    last_hours = hours;
    cycle_flag = true;
  }
  int mins = (tod_millis - (hours * HOUR_MILLIS)) / MINUTE_MILLIS;
  int secs = (tod_millis % MINUTE_MILLIS) / SECOND_MILLIS;
  sendDigits(hourPCF, hours % 10, hours / 10);
  sendDigits(minPCF, mins % 10, mins / 10);
  sendDigits(secPCF, secs % 10, secs / 10);
}

void syncTime()
{
  TimeElements tm;
  RTC.read(tm);

  // calculate offset from millis()
  millis_offset = (tm.Hour * HOUR_MILLIS + tm.Minute * MINUTE_MILLIS + tm.Second * SECOND_MILLIS) - millis();
  Serial.println((tm.Hour * HOUR_MILLIS + tm.Minute * MINUTE_MILLIS + tm.Second * SECOND_MILLIS));
  Serial.println(millis());
  Serial.println(millis_offset);
}