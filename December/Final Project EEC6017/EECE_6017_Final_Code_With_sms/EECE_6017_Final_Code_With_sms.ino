#include <Arduino.h>
#include <U8g2lib.h>  // Including the display library
#include <DS3231.h>  // Including the clock library
#include "SIM900.h"
#include <SoftwareSerial.h>
#include "sms.h"

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R2, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
DS3231  rtc(A4, A5);   // Connecting the clock to A4&A5

int dis = 7;   // Screen showing that system disarmed; Input from De-10
int initi = 6; // Screeen asking to Arm or Disarm; Input from De-10
int arm = 5;   // Screen Showing System Armed; Input from De-10
int mot= 4;    // Screen Showing Morion detected or not; Input from De-10
int pin = 3;   // Screen Showing PIN input; Input from De-10
int pinco = 2; // Input from DE-10 for correctness and uncorrectness of PIN
int pinin = 1; // Input from DE-10 for correctness and uncorrectness of PIN
int buz = 12;   // Output for the buzzer 
int blue = 9; // Blue color on the led
int red = 10; // Red color on the led 
int green = 11; // Green color on the led
int statecur = 0;
int stateprev = 0;

boolean on=false; // boolean
int screen;
int state;
int lastpinstate;  // to be able to save the pinstate 
int laststate;

SMSGSM sms;
// creating buffers for the sms shield

int numdata;
char smsbuffer[160];
char n[20];

// text buffers
void setup() {
  // Declaring Inputs and Outputs
  pinMode(dis, INPUT);   
  pinMode(initi, INPUT); 
  pinMode(arm, INPUT);
  pinMode(mot, INPUT);
  pinMode(pin, INPUT);
  pinMode(pinco, INPUT);
  pinMode(pinin, INPUT);
  
  // Declaring the Outputs
  pinMode(buz, OUTPUT); // buzzer
  pinMode(blue, OUTPUT); // buzzer
  pinMode(red, OUTPUT); // buzzer
  pinMode(green, OUTPUT); // buzzer

  rtc.begin();  // Intializing the clock 
  u8g2.begin(); // Initializing the lcd
  gsm.begin(2400); // Initializing the GSM Shield
  
}

void green_led(){
  digitalWrite(green, HIGH); 
  digitalWrite(red, LOW); 
  digitalWrite(blue, LOW); 
}
void red_led(){
  digitalWrite(red, HIGH); 
  digitalWrite(green, LOW); 
  digitalWrite(blue, LOW); 
  
}
void blue_led(){
  digitalWrite(blue, HIGH); 
  digitalWrite(red, LOW); 
  digitalWrite(green, LOW); 
  
}
void time_screen(){  // This is a function to set-up the clock screen 
      u8g2.setCursor(32,64);
      u8g2.setFont(u8g2_font_6x13_tf); 
      u8g2.print(rtc.getTimeStr());
      u8g2.sendBuffer();   
}

void disarmed_screen(){  // This is a function for the disarmed state with the display output as well as the sound and 
      stateprev = statecur;
      u8g2.setFont(u8g2_font_6x13_tf); // choose a suitable font
      u8g2.drawStr(0,20,"The Alarm is Disarmed");  // write something to the internal memory
      time_screen();
      u8g2.sendBuffer();          // transfer internal memory to the display 
      red_led();
      statecur = 1;
      buzzer();
}
void initialization_screen(){
      stateprev = statecur;
      u8g2.setFont(u8g2_font_6x13_tf); // choose a suitable font
      u8g2.drawStr(0,20,"Press KEY1 to Arm");  // write something to the internal memory
      u8g2.drawStr(0,35,"Press * to change PIN");  // write something to the internal memory
      time_screen();
      u8g2.sendBuffer();          // transfer internal memory to the display  
      blue_led();
      red_led();
      statecur = 2;
      buzzer();     
}
void armed_screen(){ 
      stateprev = statecur;
      u8g2.setFont(u8g2_font_6x13_tf); // choose a suitable font
      u8g2.drawStr(0,20,"Alarm system is armed");  // write something to the internal memory
      time_screen();
      u8g2.sendBuffer();          // transfer internal memory to the display  
      green_led();
      statecur = 3;
      sms.SendSMS("5134490428", "Alarm System Armed");
      buzzer();
}
void motion_screen(){
      stateprev = statecur;
      u8g2.setFont(u8g2_font_6x13_tf); // choose a suitable font
      u8g2.drawStr(0,20,"Motion detected");  // write something to the internal memory
      time_screen();
      u8g2.sendBuffer();          // transfer internal memory to the display  
      sms.SendSMS("5134490428", "Motion has been detected at");
      blue_led();
      tone(buz, 700);
      delay(500);
      digitalWrite(blue, LOW); 
      noTone(buz);   
}
void buzzer(){
      if (stateprev != statecur)
      {
      tone(buz, 1000);
      delay(500);
      noTone(buz);
      }
}
void correct(){
      stateprev = statecur;
      u8g2.setFont(u8g2_font_6x13_tf); // choose a suitable font
      u8g2.drawStr(0,20,"PIN is correct");  // write something to the internal memory
      time_screen();
      u8g2.sendBuffer();          // transfer internal memory to the display  
      green_led();
      statecur = 7;
      buzzer();
}
void incorrect(){
      stateprev = statecur;
      u8g2.setFont(u8g2_font_6x13_tf); // choose a suitable font
      u8g2.drawStr(0,20,"incorrect PIN entered");  // write something to the internal memory
      time_screen();
      u8g2.sendBuffer();          // transfer internal memory to the display  
      red_led();
      statecur = 8;
      buzzer();
}

void enter_pin(){
      stateprev = statecur;
      u8g2.setFont(u8g2_font_6x13_tf); // choose a suitable font
      u8g2.drawStr(0,20,"Please Enter New PIN");  // write something to the internal memory
      time_screen();
      u8g2.sendBuffer();          // transfer internal memory to the display 
      red_led();
      statecur = 6;
      buzzer();
      sms.SendSMS("5134490428", "New PIN entered in your alarm system");
}

void error_screen(){     
      u8g2.setFont(u8g2_font_6x13_tf); // choose a suitable font
      u8g2.drawStr(0,20,"Error detected ");  // write something to the internal memory
      time_screen();
      u8g2.sendBuffer();          // transfer internal memory to the display   
      tone(buz, 700);
      red_led();
      delay(500);  // buzz for 500 ms
      digitalWrite(red, LOW); 
      noTone(buz); 
}
void pin_crr(){
  int pin_co = digitalRead(pinco);   // Screen Showing PIN input; Input from De-10
  int pin_in = digitalRead(pinin);   // Screen Showing PIN input; Input from De-10
  
  if (pin_co == HIGH){
  u8g2.clearBuffer();          // clear the internal memory
  correct();
  }
  else if (pin_in == HIGH){
  u8g2.clearBuffer();          // clear the internal memory
  incorrect();
  sms.SendSMS("5134490428", "An incorrect PIN has been entered in your alarm system");
  }
}

void loop(){
  int disarmed  = digitalRead(dis);   // Screen showing that system disarmed; Input from De-10
  int initialization = digitalRead(initi); // Screeen asking to Arm or Disarm; Input from De-10
  int armed = digitalRead(arm) ;   // Screen Showing System Armed; Input from De-10
  int motion = digitalRead(mot);    // Screen Showing Morion detected or not; Input from De-10
  int pinstate = digitalRead(pin);   // Screen Showing PIN input; Input from De-10
  int pin_co = digitalRead(pinco);   // Screen Showing PIN input; Input from De-10
  int pin_in = digitalRead(pinin);   // Screen Showing PIN input; Input from De-10
  pin_crr();
  if (disarmed == HIGH) {
  u8g2.clearBuffer();          // clear the internal memory
  disarmed_screen();
  sms.SendSMS("5134490428", "Your Alarm System is Disarmed");
  }
  else if (initialization == HIGH){
  u8g2.clearBuffer();          // clear the internal memory
  initialization_screen();
  
  }
  else if (armed == HIGH){
  u8g2.clearBuffer();          // clear the internal memory
  armed_screen();
  
  }
  else if (motion == HIGH){
  u8g2.clearBuffer();          // clear the internal memory
  motion_screen();
  }
  else if (pinstate == HIGH){
  u8g2.clearBuffer();          // clear the internal memory
  enter_pin();
  }

  else {
  sms.SendSMS("5134490428", "Error detected in your alarm system");
  u8g2.clearBuffer();          // clear the internal memory
  error_screen();
  }
  
}


