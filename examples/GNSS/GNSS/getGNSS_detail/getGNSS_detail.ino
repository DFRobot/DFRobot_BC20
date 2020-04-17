/*!
 *@file getGNSS_detail.ino
 *
 *@After clicking upload, you can see all the complete satellite information on the serial monitor
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

void Display_Location_Information() {

  /*
     UTC time of the anchor point
  */
  Serial.print("Time:\t\t");
  Serial.print(sCLK.Year);
  Serial.print("/");
  Serial.print(sCLK.Month);
  Serial.print("/");
  Serial.print(sCLK.Day);
  Serial.print("  ");
  Serial.print(sCLK.Hour);
  Serial.print(":");
  Serial.print(sCLK.Minute);
  Serial.print(":");
  Serial.println(sCLK.Second);

  Serial.print("Latitude:\t");
  Serial.print(sGGNS.LatitudeVal, 6);
  Serial.print(" deg ");
  Serial.println(sGGNS.LatitudeDir);
  Serial.print("Longitude:\t");
  Serial.print(sGGNS.LongitudeVal, 6);
  Serial.print(" deg ");
  Serial.println(sGGNS.LongitudeDir);
  Serial.print("Altitude:\t");
  Serial.print(sGGNS.Altitude, 1);
  Serial.println(" m");
  Serial.print("Speed:\t\t");
  Serial.print(sGGNS.Speed);
  Serial.println(" km/h");
  Serial.print("Heading:\t");
  Serial.print(sGGNS.Heading);
  Serial.println(" deg");
  Serial.print("Status:\t\t");
  Serial.println(sGGNS.FixStatus);
  Serial.print("PDOP:\t\t");
  Serial.println(sGGNS.PDOP);
  Serial.print("HDOP:\t\t");
  Serial.println(sGGNS.HDOP);
  Serial.print("VDOP:\t\t");
  Serial.println(sGGNS.VDOP);
  Serial.println();
}

void Display_Satellite_Information() {
  Serial.print(sSAT.NUM);
  Serial.println(" in view.");
  Serial.print(sSAT2.USE);
  Serial.println(" in used.");
  Serial.print("PRN\t");
  Serial.print("Elev(deg)\t");
  Serial.print("Azim(deg)\t");
  Serial.print("SNR(dBHz)\t");
  Serial.print("SYS\t");
  Serial.println("Used");
  for (uint8_t i = 0; i < sSAT.NUM; i++) {
    Serial.print(sSAT2.data[i].PRN);
    Serial.print("\t");
    Serial.print(sSAT2.data[i].Elev);
    Serial.print("\t\t");
    Serial.print(sSAT2.data[i].Azim);
    Serial.print("\t\t");
    Serial.print(sSAT2.data[i].SNR);
    Serial.print("\t\t");
    Serial.print(sSAT2.data[i].SYS);
    Serial.print("\t");
    Serial.println(sSAT2.data[i].Status);
  }
  Serial.println("");
}

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

  /**
     Is used to obtain the specified satellite information, and the parameter is used to specify the type of information to be obtained.
     The parameter is selected as follows:
  */
  myBC20.getQGNSSRD();
  Display_Location_Information();
  Display_Satellite_Information();
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