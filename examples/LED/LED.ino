/*!
 *@file LED.ino
 *@
 *@After uploading you can see the LED lights of the NB module flashing in
 *@red - blue - green - yellow - purple - blue - white order
 *
 *@copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@licence     The MIT License (MIT)
 *@author      [PengKaixing](kaixing.peng@dfrobot.com)
 *@version  V1.0
 *@date   2020-3-19
 *@get from https://www.dfrobot.com
 *@get from https://www.dfrobot.com.cn
*/
#include "DFRobot_BC20_Gravity.h"

/*7 colors are available*/
#define  RED 0
#define  BLUE 1
#define  GREEN 2
#define  YELLOW 3
#define  PURPLE 4
#define  CYAN 5
#define  WHITE 6

/*Communication by IIC*/
#define USE_IIC

/*Communication by HardwareSerial*/
//#define USE_HSERIAL

/*Communication by SoftwareSerial*/
//#define USE_SSERIAL


/******************IIC******************/
#ifdef USE_IIC
/*
   For general controllers. Communicate by IIC
   Connect Instructions
      Controller     |    Module(BC20)
          SDA        |       D/T
          SCL        |       C/R
          GND        |       GND
       5V or 3V3     |       VCC

   IIC address(A0,A1)
     0x30:(A0=0,A1=0)
     0x31:(A0=0,A1=1)
     0x32:(A0=1,A1=0)
     0x33:(A0=1,A1=1) default
*/
DFRobot_BC20_IIC myBC20(0x33);

/******************HardwareSerial******************/
#elif defined(USE_HSERIAL)
/*
   For MEGA2560/ESP32 HardwareSerial
   Connect Instructions
   esp32      |               MEGA Series    |    Module(BC20)
   IO16       |               D17(RX)        |       D/T
   IO17       |               D16(TX)        |       C/R
   GND        |               GND            |       GND
   5V(USB) or 3V3(battery)  | 5V or 3V3      |       VCC
*/
#if defined(ARDUINO_ESP32_DEV)
HardwareSerial Serial2(2);
DFRobot_BC20_Serial myBC20(&Serial2);//ESP32HardwareSerial
#else
DFRobot_BC20_Serial myBC20(&Serial2);//others
#endif

/******************SoftwareSerial******************/
#elif defined(USE_SSERIAL)
/*
    For Arduino Series SoftwareSerial
    Connect Instructions
        UNO     |    Module(BC20)
      PIN_RXD   |       D/T
      PIN_TXD   |       C/R
        GND     |       GND
     5V or 3V3  |       VCC
*/
#define PIN_TXD   3
#define PIN_RXD   4
SoftwareSerial ss(PIN_RXD,PIN_TXD);
DFRobot_BC20_SW_Serial myBC20(&ss);
#endif

void setup() {
  Serial.begin(115200);
  
  /*Initialize BC20*/
  Serial.print("Starting the BC20.Please wait. . . ");
  while (!myBC20.powerOn()) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("BC20 started successfully !");
}

void loop() {
  
  myBC20.changeColor(RED);    //Change the color
  //Blink three times
  for (int i = 0; i < 3; i++) {
    myBC20.LED_ON();          //Turn on LED
    delay(500);
    myBC20.LED_OFF();         //Turn off LED
    delay(500);
  }

  myBC20.changeColor(BLUE);
  for (int i = 0; i < 3; i++) {
    myBC20.LED_ON();
    delay(500);
    myBC20.LED_OFF();
    delay(500);
  }

  myBC20.changeColor(GREEN);
  for (int i = 0; i < 3; i++) {
    myBC20.LED_ON();
    delay(500);
    myBC20.LED_OFF();
    delay(500);
  }

  myBC20.changeColor(YELLOW);
  for (int i = 0; i < 3; i++) {
    myBC20.LED_ON();
    delay(500);
    myBC20.LED_OFF();
    delay(500);
  }

  myBC20.changeColor(PURPLE);
  for (int i = 0; i < 3; i++) {
    myBC20.LED_ON();
    delay(500);
    myBC20.LED_OFF();
    delay(500);
  }

  myBC20.changeColor(CYAN);
  for (int i = 0; i < 3; i++) {
    myBC20.LED_ON();
    delay(500);
    myBC20.LED_OFF();
    delay(500);
  }

  myBC20.changeColor(WHITE);
  for (int i = 0; i < 3; i++) {
    myBC20.LED_ON();
    delay(500);
    myBC20.LED_OFF();
    delay(500);
  }
}
