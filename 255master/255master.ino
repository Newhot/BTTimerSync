#include <arduino-timer.h>
#include <SoftwareSerial.h>


/***************************蓝牙开始**********************************/
SoftwareSerial mySerial(10, 11); // RX, TX
char val; //存储接收的变量
String fromBT; 
String sendStr;
/***************************蓝牙结束**********************************/


/***************************Timer开始**********************************/
auto timer = timer_create_default(); //create a timer with default settings

int theTime = 0;
/***************************Timer结束**********************************/


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  /***************************蓝牙开始**********************************/
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

  connectDevice();
  /***************************蓝牙结束**********************************/
  
  pinMode(2, INPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  timer.every(1000, countDown); //Timer
}

void loop() {
  if (digitalRead(2) == HIGH)
  {
    if (theTime > 0)
    {
      theTime = 0;

      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
    }
    else
    {
      theTime = 25 * 60;

      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);          
    }
    sendTime();

    delay(1000);
  }

  if (theTime == 0)
  {
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
  }


  readBT();

  //将调试工具里发的内容转发给蓝牙模块
  if (Serial.available()>0) {
    val = Serial.read();
    mySerial.write(val);
  }

  timer.tick();//tick the timer
}

/***************************蓝牙开始**********************************/
void readBT()
{
   if (mySerial.available()>0) {
    fromBT = mySerial.readString();
  
    Serial.println(fromBT);
  }
}

void connectDevice()
{
  sendMessage("at+cona0x882583F3A998\r\n");
  Serial.println("Try to connect to the device.");
  delay(1000);
}

void sendTime()
{
  sendStr = String((int)(theTime/60)*100 + theTime%60);
  Serial.println(sendStr);
  sendMessage(sendStr);
}

//通用的发送信息方法
void sendMessage(String msg)
{
  mySerial.print(msg);
  delay(100);
}
/***************************结束开始**********************************/


void countDown(void)
{
  if (theTime > 0)
  {
    theTime--;
    if (theTime%5 == 0)
    {
      sendTime();
    }
  }
  
  Serial.println(theTime);
}
