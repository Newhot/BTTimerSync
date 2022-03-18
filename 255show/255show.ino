#include <Arduino.h>
#include <TM1637Display.h>
#include <SoftwareSerial.h>


/***************************数码管开始**********************************/
// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   2000

const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
  };

TM1637Display display(CLK, DIO);
/***************************数码管结束**********************************/


/***************************蓝牙开始**********************************/
SoftwareSerial mySerial(10, 11); // RX, TX
char val; //存储接收的变量
String fromBT; 
/***************************蓝牙结束**********************************/

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Hello Thomas!");
  
  /***************************蓝牙开始**********************************/
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  /***************************蓝牙结束**********************************/


  display.setBrightness(0x01);
  display.showNumberDecEx(0, 0, true);
}

void loop()
{
//  display.showNumberDecEx(301, 64, true);
//  delay(TEST_DELAY);


  // Done!
//  display.setSegments(SEG_DONE);
//  delay(TEST_DELAY);

  readBT();

  //将调试工具里发的内容转发给蓝牙模块
  if (Serial.available()>0) {
    val = Serial.read();
    mySerial.write(val);
  }

}

/***************************蓝牙开始**********************************/
void readBT()
{
   if (mySerial.available()>0) {
    fromBT = mySerial.readString();
  
    Serial.println(fromBT);

    showInt(fromBT);
  }
}

void showInt(String &sTime)
{
  int theTime = sTime.toInt();
  Serial.println(String(theTime));
  if (theTime == 0)
  {
    // Done!
    display.setSegments(SEG_DONE);
    delay(TEST_DELAY);
  }
  else
  {
    display.showNumberDecEx(theTime, 64, true);
  }
}
/***************************蓝牙结束**********************************/
