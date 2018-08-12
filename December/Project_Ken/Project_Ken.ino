#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <SPI.h>
#include <MFRC522.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DS3231  rtc(A4, A5);
constexpr uint8_t RST_PIN = 5;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 53;     // Configurable, see typical pin layout above

int motion_sensor = 11;
int green = 3;
int blue = 2;
int red = 1;
int buzzer = 9;
int flame_sensor=8; // Flame sendor
int lightsensor = A0; 
int pushbutton;
boolean on=false;
int buttonState; 
int cardread;

#define analogPin  A1 //the thermistor attach to 
#define beta 4090 //the beta of the thermistor
#define resistance 10 //the value of the pull-down resistor
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 


void setup()
{
  pinMode(motion_sensor, INPUT); 
  pinMode(flame_sensor, INPUT);
  pinMode(lightsensor, INPUT);
 
  pinMode(cardread, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(buzzer, OUTPUT);
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  rtc.begin(); 
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
}

void temperature(){
  long a =1023 - analogRead(analogPin);
  int tempC = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
  lcd.setCursor(17,0);
  lcd.print(tempC);
  lcd.setCursor(19,0);
  lcd.print("F");
  
}
void card(){
  if (  rfid.PICC_IsNewCardPresent()){
  digitalWrite(cardread, HIGH); 
}
else {
  digitalWrite(cardread, LOW);
}
delay(10);
pushbutton == digitalRead(cardread);

}
void flamedt(){
  int flame = digitalRead(flame_sensor);
  if (flame == HIGH){
    lcd.setCursor(0,1);
    lcd.print("Fire detected in the");
    lcd.setCursor(0,2);
    lcd.print("store");
    tone(buzzer, 900);
  }
}
void motiondt(){
  int motion = digitalRead(motion_sensor);
  
  if (motion == HIGH){
    lcd.setCursor(0,1);
    lcd.print("Motion Detected");
    digitalWrite(3,HIGH);
    tone(buzzer, 900);
    }
  }

void clockr(){
  lcd.setCursor(0,0);
  lcd.print(rtc.getTimeStr());
}
void state(){
  card();
   buttonState = digitalRead(pushbutton);
  if (buttonState == HIGH) {    
    if(on==true){
      on=false;
    }  else{
      on=true;
    }
  }
   if (on == true) {  
    lcd.clear();  
    digitalWrite(2, HIGH);  
    noTone(buzzer);
    lcd.setCursor(0,1);
    lcd.print("Alarm Reset");
   int buttonstate = 0;
  }
  else if (on == false){
  }
   delay(300);
}


void loop(){
  
  temperature();
  clockr();
  flamedt();
  motiondt();
  state();
}


