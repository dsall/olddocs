#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int mot = 7;
int led = 6;

void setup()
{
  pinMode(mot, INPUT); 
  pinMode(led, OUTPUT);
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
}


void loop()
{
  int motion = digitalRead(mot);

  if (motion == HIGH){
    lcd.setCursor(0,0);
    lcd.print("Motion Detected");
    lcd.setCursor(0,1);
    lcd.print("Kenneth s");
    digitalWrite(led, HIGH);
  }

}
