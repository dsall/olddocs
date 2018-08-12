#include <DS3231.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DS3231  rtc(A4, A5);

void setup()
{

  rtc.begin();
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
}

void loop()
{
  lcd.setCursor(12,3);
  lcd.print(rtc.getTimeStr());
  delay (1000);
}
