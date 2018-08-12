#include <Arduino.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

int potPin = A0;
int lastPotValue;
void setup() {
  pinMode(potPin, INPUT);
  u8g2.begin();
}

void loop() {
  // read potPin and divide by 255 to give 5 possible readings
  int potValue = analogRead(potPin) / 255;
  
  // if something has changed since last value
  if(potValue != lastPotValue)
  {
    // enter switch case
    switch(potValue)
    {
      case 0:
         u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0,10,"unarmed");  // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display 
        break;
      case 1:
                 u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0,10,"armed");  // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display 
        break;
      case 2:
        Serial.println("Moderate");
        break;
      case 3:
        Serial.println("High");
        break;
      case 4:
        Serial.println("Extreme");
        break;
      default:
        Serial.println("error!");
        break;
    }
    lastPotValue = potValue;
  }
}
