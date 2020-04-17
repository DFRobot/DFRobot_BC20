/*!
 *@file getGLL.ino
 *
 *@After clicking upload, you can see the satellite GLL protocol in the serial port
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

/* 7 colors are available */
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
  myBC20.LED_OFF();

  /*Initialize BC20*/
  Serial.print("Starting the BC20.Please wait. . . ");
  myBC20.changeColor(RED);
  while (!myBC20.powerOn()) {
    Serial.print(".");
    myBC20.LED_ON();
    delay(500);
    myBC20.LED_OFF();
    delay(500);
  }
  Serial.println("BC20 started successfully!");

  /* Disable sleep mode */
  myBC20.configSleepMode(eSleepMode_Disable);

  /*Power up GNSS*/
  Serial.print("Turning on GNSS ... ");
  myBC20.setQGNSSC(ON);
  myBC20.changeColor(YELLOW);
  if (myBC20.getQGNSSC() == OFF) {
    Serial.print(".");
    myBC20.LED_ON();
    delay(500);
    myBC20.LED_OFF();
    delay(500);
  }
  Serial.println("GNSS is ON.");
  myBC20.changeColor(CYAN);
}

void loop() {

  myBC20.getQGNSSRD(NMEA_GLL);

  /*
     UTC time, format: hhmmss.ss, ex. 162436.54 => 16:24:36.54
     h - hours
     m - minutes
     s - seconds
  */
  Serial.print("UTC Time: ");
  Serial.println(sGLL.UTC_Time());

  /*
     Latitude, format: ddmm.mmmmm, ex. 3150.7820 => 31deg 50.7820min
     d - degrees
     m - minutes
  */
  Serial.print("Latitude: ");
  Serial.print(sGLL.LatitudeVal());

  /*
     Latitude north or south
     N - North
     S - South
  */
  Serial.print(" ");
  Serial.println(sGLL.LatitudeDir());

  /*
     Longitude, format: dddmm.mmmmm, ex. 12135.6794 = 121deg 35.6794min
     d - degrees
     m - minutes
  */
  Serial.print("Longitude: ");
  Serial.print(sGLL.LongitudeVal());
  Serial.print(" ");

  /*
     Longitude wast or west
     E - East
     W - West
  */
  Serial.println(sGLL.LongitudeDir());

  /*
     DataStatus
     V - Invalid
     A - Valid
  */
  Serial.print("Data Status: ");
  Serial.println(sGLL.DataStatus());

  /*
     Positioning Mode
     N - No fix
     A - Autonomous GPS fix
     D - Differential GPS fix
  */
  Serial.print("Positioning Mode: ");
  Serial.println(sGLL.PositionMode());
  Serial.println();
  Serial.println();
  myBC20.clearGPS();

  myBC20.LED_ON();
  delay(500);
  myBC20.LED_OFF();
#ifndef ARDUINO_ESP32_DEV  
  delay(500);
#else 
  delay(5000);
#endif
}
