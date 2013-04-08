/* 
 This sketch prints "Hello World!" to the LCD and shows the
 time. The time has to be initialized by sending a string in
 HHMM format.
 
 This program is based on the LiquidCrystal Library Tutorial
 and my qlock.js implementation.
 @see http://arduino.cc/en/Tutorial/LiquidCrystal
 @see https://github.com/f00f/qlock.js
 */

// include the library code:
#include <LiquidCrystal.h>

#define SIZE_OF_TIME_MSG 4

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

unsigned int base_hours = 0;
unsigned int base_minutes = 0;
unsigned int base_seconds = 0;
unsigned long base_millis = 0;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, qlock!");
  
  Serial.begin(9600);
  Serial.print("To init clock, send time in HHMM format.\n");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);

  if (0 == base_millis)
  {
    // print the number of seconds since reset:
    lcd.print(millis()/1000);
    lcd.print(" sec since reset");
  }
  else
  {
    // compute time
    unsigned int offset_s = int( (millis() - base_millis) / 1000 );
    unsigned int cur_s = (base_seconds + offset_s);

    // overflow
    unsigned int offset_m = cur_s / 60;
    unsigned int cur_m = (base_minutes + offset_m);

    // overflow
    unsigned int offset_h = cur_m / 60;
    unsigned int cur_h = (base_hours + offset_h);

    cur_s = cur_s % 60;
    cur_m = cur_m % 60;
    cur_h = cur_h % 24;

    // generate time string
    String time_string = getTimeString(cur_h, cur_m, cur_s);
    lcd.print(time_string);
  }
}

void serialEvent() {
  char time_buf[4];

  unsigned long m = millis(); // remember time of the event

  Serial.print("Got serial event.\n");
  if (Serial.available() != SIZE_OF_TIME_MSG)
  {
    //Serial.print("E_MSG_TOO_SHORT\n");
    return;
  }

  unsigned int nBytes = Serial.readBytes(time_buf, SIZE_OF_TIME_MSG);
  if (nBytes != SIZE_OF_TIME_MSG)
  {
    Serial.print("E_WRONG_TIME_FORMAT\n");
    return;
  }
  Serial.print(String(time_buf) + String("\n"));
  base_millis = m;
  base_hours = digitToInt(time_buf[0]) * 10 + digitToInt(time_buf[1]);
  base_minutes = digitToInt(time_buf[2]) * 10 + digitToInt(time_buf[3]);

  // erase 2nd line
  lcd.setCursor(0, 1);
  lcd.print("                ");//16x' '
}

char digitToInt(char digit)
{
  return digit - '0';
}

String getTimeString(unsigned int h, unsigned int m, unsigned int s)
{
  String time_str = "";
  if (h < 10) { time_str += "0"; }
  time_str += h;
  time_str += ":";
  if (m < 10) { time_str += "0"; }
  time_str += m;
  time_str += ":";
  if (s < 10) { time_str += "0"; }
  time_str += s;
  return time_str;
}
