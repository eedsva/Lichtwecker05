/*
 * Lichtwecker rev 0.0
 * Alarm Clock + LCD
 *
 * rev 0.2 print alarmtime
 * rev 0.1 add LEDs
 */
#include <SoftPWM.h> 
#include <Time.h>
#include <TimeAlarms.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int fadetime = 500;
int dim = 0;      // startvalue for leds -> dark
int dimmax = 248; // max brightness led 
int inc = 8;      // increase of LED
boolean light = false; // light switch bool

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2); 
  setTime(21,55,15,10,7,13); // set time to Saturday 20:33:00am 30.09.2013

  // create the alarms 
  Alarm.alarmRepeat(6,25,0, MorningAlarm);  // 20:34 every day
  Alarm.timerRepeat(1, lcdDisplay);      // display time every sec
  
  // lcd alarm
  lcd.setCursor(0,0);
  lcd.print("Alarm 06:25"); 
  lcd.setCursor(0,1);
  lcd.print("Time ");
  
  SoftPWMBegin();  
  SoftPWMSet(19,0);
  SoftPWMSet(18,0);
  SoftPWMSet(17,0);
  SoftPWMSet(16,0);
  SoftPWMSetFadeTime(19, fadetime, fadetime);
  SoftPWMSetFadeTime(18, fadetime, fadetime);
  SoftPWMSetFadeTime(17, fadetime, fadetime);
  SoftPWMSetFadeTime(16, fadetime, fadetime); 
}

void  loop(){  
  digitalClockDisplay();
  Alarm.delay(1000); // wait one second between clock display
  if (light){
     for(int x = 0; x < 31; x++){ 
        increaseLight();
        Alarm.delay(60000); //1min every step
     }
     light = false;
  }else{
     Alarm.delay(600000); // wait 10min
     lightOff();
  }
}

// functions to be called when an alarm triggers:
void MorningAlarm(){
  Serial.println("Alarm: - turn lights on");  
  light = true;
}

void increaseLight(){
    // 30min sonnenaufgang
    // 255/30 = 8.5 ~ 1min/8
    if (dim < dimmax){
      dim = dim + inc;
    }    
    Serial.println(dim);
    SoftPWMSet(19, dim);
    SoftPWMSet(18, dim);
    SoftPWMSet(17, dim);
    SoftPWMSet(16, dim); 
}

void lightOff(){
    SoftPWMSet(19, 0);
    SoftPWMSet(18, 0);
    SoftPWMSet(17, 0);
    SoftPWMSet(16, 0);
}

void lcdDisplay(){
  lcd.setCursor(6,1);
  lcd.print(hour());
  printLcdDigits(minute());
  printLcdDigits(second());
  lcd.print(" ");
}
void printLcdDigits(int digits)
{
  lcd.print(":");
  if(digits < 10)
    lcd.print('0');
  lcd.print(digits);
}
void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}

void printDigits(int digits)
{
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

