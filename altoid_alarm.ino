/*  Alarm clock that plays Wham's Wake Me Up Before You Go Go when it goes off.
 *   Uses trinket pro, trinket power backpack, seven segment 4 digit display 
 *   with backpack, a piezo speaker, 3 push buttons, and a real time clock.
 *      
 *   
 *   Wiring
 *   seven segment backpack
 *   + to +5v
 *   - to gnd
 *   d to A4
 *   c to A5
 *   
 *   passive buzzer
 *   + to digital 12
 *   other pin to gnd
 *   
 *   real time clock
 *   gnd to gnd
 *   vcc to +5v
 *   sda to A4
 *   scl to A5
 *   
 *   buttons
 *   one side to ground
 *   one side to digital pin 3, 4, 5
 *  
 */   



//piazo buzzer library.
#include "pitches.h"
//real time clock libraries.
#include <Wire.h>
#include <DS3231.h>
//seven segment backpack libraries.
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

//notes in the melody for the piazo buzzer.
int melody[] = {NOTE_G5,NOTE_A5,NOTE_C6,NOTE_D6,NOTE_E6,NOTE_F6,NOTE_G6,NOTE_A6, NOTE_AS6};

//rtc (real time clock) initialization.
DS3231 clock;
RTCDateTime dt;

//ssb (seven segment backpack) initialization.
Adafruit_7segment matrix = Adafruit_7segment();

//variables for the alarm time and a flag if the alarm is on or off.
int alarmH = 12;
int alarmM = 00;
int alarmFlag = 1;

void setup() {
  //start the serial output for everything.
  Serial.begin(9600);

  //initialize rtc.
  clock.begin();
  //set sketch compiling time
  //uncomment to set time on rtc, otherwise it'll mess up the time 
  //clock.setDateTime(__DATE__, __TIME__);

  //initialize the ssb.
  matrix.begin(0x71);
  //put the colon between the hour and minutes on the ssb.
  matrix.drawColon(true);
  //dim the display.
  matrix.setBrightness(1);

  //setup the buttons as inputs using the internal pullup resistors.
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}
 
void loop() {
  //read the button inputs as variables.
  int button1 = digitalRead(3);
  int button2 = digitalRead(4);
  int button3 = digitalRead(5);

  //if button1 is pressed, play the song.
  //checks to see if the alarm is turned on or off.
  //if it's on need to turn the alarm back on after the song,
  //since the song turns it off.
  if(button1 == 0){
    if(alarmFlag == 1){
      buzzer();
      alarmFlag = 1;    
    }

    else{
      buzzer();
    }
  }

  //if button2 is pressed, toggle the alarm on or off.
  if(button2 == 0){
    if(alarmFlag == 0){
      alarmFlag = 1;
    }

    else{
      alarmFlag = 0;
    }

    delay(250);
  }

  //if button3 is held down and button1 is pressed, increment the hours of the alarm time.
  //if button3 is held down and button2 is pressed, increment the minutes of the alarm time.
  //otherwise if button3 is held down display the alarm time.
  if(button3 == 0){
    if(button2 == 0){
      if(alarmM == 59){
        alarmM = 0;
      }

      else{
        alarmM ++;
      }
    }

    else if(button1 == 0){
      if(alarmH == 23){
        alarmH = 0;
      }

      else{
        alarmH ++;
      }
    }
    
    alarmDisplay();
    delay(250);
  }

  //if no buttons are pressed, show the time.
  else{
    timeDisplay();
  }
}

void timeDisplay(){
   //pull the time from the rtc and show it on the ssb.
  dt = clock.getDateTime();
  
  //set variables for the hours and minutes.
  int h = dt.hour;
  int m = dt.minute;

  //stop any digit blinking if it's blinking.
  matrix.blinkRate(0);

  if(alarmFlag == 0){
    //if the alarm is off, just display the time.
    //separate the hours and minutes by individual digits.
    matrix.writeDigitNum(0, (h / 10));
    matrix.writeDigitNum(1, (h % 10));
    matrix.writeDigitNum(3, (m / 10));
    matrix.writeDigitNum(4, (m % 10));
    //display the time on the ssb.
    matrix.writeDisplay();
  }

  else{
    //if the alarm is turned on, put a dot at the end of the minutes.
    //separate the hours and minutes by individual digits.
    matrix.writeDigitNum(0, (h / 10));
    matrix.writeDigitNum(1, (h % 10));
    matrix.writeDigitNum(3, (m / 10));
    matrix.writeDigitNum(4, (m % 10), true);
    //display the time on the ssb.
    matrix.writeDisplay();

    //since the alarm is on, check to see if it's time to turn on the alarm.
    if(h == alarmH){
      if(m == alarmM){
        buzzer();
      }
    }
  }
}

void alarmDisplay(){
  //show the alarm time.

  //blink the digits to show it's "programming".
  matrix.blinkRate(2);
  
  //separate the hours and minutes by individual digits.
  matrix.writeDigitNum(0, (alarmH / 10));
  matrix.writeDigitNum(1, (alarmH % 10));
  matrix.writeDigitNum(3, (alarmM / 10));
  matrix.writeDigitNum(4, (alarmM % 10));
  //display the time on the ssb.
  matrix.writeDisplay();
}

void buzzer(){
  //piezo buzzer song.
  //start by turning the alarm off once it plays.
  //then plays tones.
  //code is messy, but works, so i'm not going to fuss with it right now.
  alarmFlag = 0;

  //Wake Me Up Before You Go Go.
  //26
  Serial.println("26");
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //b5 8b
  tone(12, melody[8], 400);
  delay(150);

  //27
  Serial.println("27");
  //quarter rest +1 beat
  delay(750);
  //e5 8
  tone(12, melody[4], 250);
  delay(300);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //b5 8b
  tone(12, melody[8], 400);
  delay(150);
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);

  //28
  Serial.println("28");
  //eighth rest
  delay(250);
  //c5 8
  tone(12, melody[2], 250);
  delay(300);
  //eighth rest
  delay(250);
  //c5 8
  tone(12, melody[2], 250);
  delay(300);
  //d5 8b
  tone(12, melody[3], 400);
  delay(150);
  //e5 8b
  tone(12, melody[4], 400);
  delay(300);
  //f5 8
  tone(12, melody[5], 400);
  delay(150);

  //29
  Serial.println("29");
  //f5 8b
  tone(12, melody[5], 400);
  delay(150);
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);
  //d5 8b
  tone(12, melody[3], 400);
  delay(150);
  //c5 8b
  tone(12, melody[2], 400);
  delay(150);
  //e5 4
  tone(12, melody[4], 500);
  delay(300);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);

  //30
  Serial.println("30");
  //eighth rest
  delay(250);
  //c5 8
  tone(12, melody[2], 250);
  delay(300);
  //quarter rest
  delay(250);
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //b5 8b
  tone(12, melody[8], 400);
  delay(150);

  //31
  Serial.println("31");
  //quarter rest +1 beat
  delay(750);
  //e5 8
  tone(12, melody[4], 250);
  delay(300);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //b5 8b
  tone(12, melody[8], 400);
  delay(150);
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);

  //32
  Serial.println("32");
  //eighth rest
  delay(250);
  //c5 8
  tone(12, melody[2], 250);
  delay(300);
  //eighth rest
  delay(250);
  //c5 8
  tone(12, melody[2], 250);
  delay(300);
  //d5 8b
  tone(12, melody[3], 400);
  delay(150);
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);
  //d5 8b
  tone(12, melody[3], 400);
  delay(150);
  //f5 8b
  tone(12, melody[5], 400);
  delay(150);

  //33
  Serial.println("33");
  //f5 8b
  tone(12, melody[5], 400);
  delay(150);
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);
  //c5 8b
  tone(12, melody[2], 400);
  delay(150);
  //a4 8b
  tone(12, melody[1], 400);
  delay(300);
  //c5 8b(2)
  tone(12, melody[2], 400);
  delay(150);
  //c5 8b(2)
  tone(12, melody[2], 400);
  delay(150);
  //c5 8b(2)
  tone(12, melody[2], 400);
  delay(150);
  //a4 8b(2)
  tone(12, melody[1], 400);
  delay(150);

  //34
  Serial.println("34");
  //g4 8
  tone(12, melody[0], 250);
  delay(300);
  //eighth rest
  delay(250);
  //quarter rest
  delay(500);
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //b5 8b
  tone(12, melody[8], 400);
  delay(150);

  //35
  Serial.println("35");
  //quarter rest +1 beat
  delay(500);
  //e5 8
  tone(12, melody[4], 250);
  delay(300);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //b5 8b
  tone(12, melody[8], 400);
  delay(150);
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);

  //36
  Serial.println("36");
  //eighth rest
  delay(250);
  //c5 8
  tone(12, melody[2], 250);
  delay(300);
  //eighth rest
  delay(250);
  //c5 8
  tone(12, melody[2], 250);
  delay(300);
  //d5 8b
  tone(12, melody[3], 400);
  delay(150);
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);
  //f5 8b
  tone(12, melody[5], 400);
  delay(150);

  //37
  Serial.println("37");
  //f5 8b
  tone(12, melody[5], 400);
  delay(150);
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);
  //d5 8b
  tone(12, melody[3], 400);
  delay(150);
  //c5 8b
  tone(12, melody[2], 400);
  delay(150);
  //e5 8
  tone(12, melody[4], 400);
  delay(300);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);

  //38
  Serial.println("38");
  //eighth rest
  delay(250);
  //c5 8
  tone(12, melody[2], 250);
  delay(300);
  //quarter rest
  delay(250);
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //b5 8b
  tone(12, melody[8], 400);
  delay(150);

  //39
  Serial.println("39");
  //quarter rest
  delay(500);
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);
  //a5 8b
  tone(12, melody[7], 400);
  delay(150);
  //a5 8
  tone(12, melody[7], 250);
  delay(300);
  //b5 8b
  tone(12, melody[8], 400);
  delay(150);
  //e5 8b
  tone(12, melody[4], 400);
  delay(150);

  //40
  Serial.println("40");
  //eighth rest
  delay(250);
  //c5 8b
  tone(12, melody[2], 400);
  delay(150);
  //a4 8b
  tone(12, melody[1], 400);
  delay(150);
  //c5 8b
  tone(12, melody[2], 400);
  delay(150);
  //half rest
  delay(500);
  delay(150);

  //41
  Serial.println("41");
  //e5 8
  tone(12, melody[4], 250);
  delay(300);
  //c5 8b
  tone(12, melody[2], 400);
  delay(150);
  //d5 8b
  tone(12, melody[3], 400);
  delay(300);
  //d5 8b(2)
  tone(12, melody[3], 400);
  delay(150);
  //e5 8b(2)
  tone(12, melody[4], 400);
  delay(150);
  //d5 8b(2)
  tone(12, melody[3], 400);
  delay(150);
  //d5 8b(2)
  tone(12, melody[3], 400);
  delay(150);

  //42
  Serial.println("42");
  //d5 8b
  tone(12, melody[3], 400);
  delay(150);
  //c5 8b
  tone(12, melody[2], 400);
  delay(300);
  //c5 2 +1 beat
  tone(12, melody[2], 1250);
  delay(150);
}

