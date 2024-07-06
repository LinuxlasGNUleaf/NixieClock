#include <main.h>

PCF8575 hourPCF(0x24);
PCF8575 minPCF(0x22);
PCF8575 secPCF(0x20);

RTC_DS1307 rtc;
DateTime current_ts;
TimeSpan second_ts(1);


int ticks = 0;
unsigned long last_millis;
unsigned long current_millis;

void setup()
{
  // begin serial and wire connection
  Serial.begin(115200);
  Wire.begin();

  // check RTC connection
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
  delay(1000);
  rotateAllDigits();
  Serial.println("done.");
  disableAllDigits();

  // read RTC and prepare variables
  current_ts = rtc.now();
  last_millis = 0;
}

void loop()
{
  current_millis = millis();
  if (last_millis + 1000 <= current_millis)
  {
    last_millis = current_millis;
    ticks++;
    
    // display current DateTime
    displayCurrentTime(current_ts);

    if (ticks >= AUTO_SYNC_TICKS)
    {
      ticks = 0;
      Serial.println("Syncing time from RTC...");
      char res[100];
      current_ts.toString(res);
      Serial.println(res);
      current_ts = rtc.now();
      current_ts.toString(res);
      Serial.println(res);
    }

    // add second to DateTime 
    current_ts = current_ts + second_ts;
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
    delay(1000);
  }
}

void disableAllDigits()
{
  sendDigits(hourPCF, 0x1F, 0x1F);
  sendDigits(minPCF, 0x1F, 0x1F);
  sendDigits(secPCF, 0x1F, 0x1F);
}

void displayCurrentTime(DateTime current)
{
  int hours = current.hour();
  int mins = current.minute();
  int secs = current.second();
  sendDigits(hourPCF, hours % 10, hours / 10);
  sendDigits(minPCF, mins % 10, mins / 10);
  sendDigits(secPCF, secs % 10, secs / 10);
}