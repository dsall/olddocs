#include <Arduino.h>
#include <U8g2lib.h>  // Including the display library
#include <DS3231.h>  // Including the clock library

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R2, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
DS3231  rtc(A4, A5);   // Connecting the clock to A4&A5

int dis = 7;   // Screen showing that system disarmed; Input from De-10
int initi = 6; // Screeen asking to Arm or Disarm; Input from De-10
int arm = 5;   // Screen Showing System Armed; Input from De-10
int mot= 4;    // Screen Showing Morion detected or not; Input from De-10
int pin = 3;   // Screen Showing PIN input; Input from De-10
int pincu = 2; // Input from DE-10 for correctness and uncorrectness of PIN
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

void setup() {
  // Declaring Inputs and Outputs
  pinMode(dis, INPUT);   
  pinMode(initi, INPUT); 
  pinMode(arm, INPUT);
  pinMode(mot, INPUT);
  pinMode(pin, INPUT);
  pinMode(pincu, INPUT);
  
  // Declaring the Outputs
  pinMode(buz, OUTPUT); // buzzer
  pinMode(blue, OUTPUT); // buzzer
  pinMode(red, OUTPUT); // buzzer
  pinMode(green, OUTPUT); // buzzer

  rtc.begin();  // Intializing the clock 
  u8g2.begin(); // Initializing the lcd
  
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
      if (stateprev != statecur)
      {
      tone(buz, 1000);
      delay(500);
      noTone(buz);
      }
}
void initialization_screen(){
      stateprev = statecur;
      u8g2.setFont(u8g2_font_6x13_tf); // choose a suitable font
      u8g2.drawStr(0,20,"Press # to Arm");  // write something to the internal memory
      u8g2.drawStr(0,35,"Press * to Disarm");  // write something to the internal memory
      time_screen();
      u8g2.sendBuffer();          // transfer internal memory to the display  
      blue_led();
      red_led();
      statecur = 2;
      if (stateprev != statecur)
      {
      tone(buz, 1000);
      delay(500);
      noTone(buz);
      }
      
}
void armed_screen(){
      stateprev = statecur;
      u8g2.setFont(u8g2_font_6x13_tf); // choose a suitable font
      u8g2.drawStr(0,20,"Alarm system is armed");  // write something to the internal memory
      time_screen();
      u8g2.sendBuffer();          // transfer internal memory to the display  
      green_led();
      statecur = 3;
      if (stateprev != statecur)
      {
      tone(buz, 1000);
      delay(500);
      noTone(buz);
      }
}
void motion_screen(){
      stateprev = statecur;
      u8g2.setFont(u8g2_font_6x13_tf); // choose a suitable font
      u8g2.drawStr(0,20,"Motion detected");  // write something to the internal memory
      u8g2.drawStr(0,35,"Back Door Open");  // write something to the internal memory
      time_screen();
      u8g2.sendBuffer();          // transfer internal memory to the display  
      blue_led();
      delay(500);
      digitalWrite(blue, LOW); 
      statecur = 4;
      if (stateprev != statecur)
      {
      tone(buz, 1000);
      delay(500);
      noTone(buz);
      }
}
}
void pin_screen(){
      stateprev = statecur;
      int password = digitalRead(pincu);
      
      if (password == HIGH){
      u8g2.setFont(u8g2_font_6x13_tf); // choose a suitable font
      u8g2.drawStr(0,20,"Please Input your PIN");  // write something to the internal memory
      u8g2.drawStr(0,35,"The PIN is correct");  // write something to the internal memory
      time_screen();
      u8g2.sendBuffer();          // transfer internal memory to the display   
      green_led();
      delay(500);
      digitalWrite(green, LOW); 
      }
      else if (password == LOW){
      u8g2.setFont(u8g2_font_6x13_tf); // choose a suitable font
      u8g2.drawStr(0,20,"Please Input your PIN");  // write something to the internal memory
      u8g2.drawStr(0,35,"The PIN is incorrect");  // write something to the internal memory
      time_screen();
      u8g2.sendBuffer();          // transfer internal memory to the display   
      red_led();
      delay(500);
      digitalWrite(red, LOW); 
      }
      statecur = 5;
      if (stateprev != statecur)
      {
      tone(buz, 1000);
      delay(500);
      noTone(buz);
      }
}
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

void state_st(){
  while (state != laststate){
    tone(buz, 1000);
    laststate = !state;  
    }
  
}

void loop(){
  int disarmed  = digitalRead(dis);   // Screen showing that system disarmed; Input from De-10
  int initialization = digitalRead(initi); // Screeen asking to Arm or Disarm; Input from De-10
  int armed = digitalRead(arm) ;   // Screen Showing System Armed; Input from De-10
  int motion = digitalRead(mot);    // Screen Showing Morion detected or not; Input from De-10
  int pinstate = digitalRead(pin);   // Screen Showing PIN input; Input from De-10
  
  if (disarmed == HIGH) {
  u8g2.clearBuffer();          // clear the internal memory
  disarmed_screen();
  state == disarmed;
  state_st();
  }
  else if (initialization == HIGH){
  u8g2.clearBuffer();          // clear the internal memory
  initialization_screen();
  state == initialization;
  state_st();
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
  pin_screen();
  }
  else {
  u8g2.clearBuffer();          // clear the internal memory
  error_screen();
  }
  
}


