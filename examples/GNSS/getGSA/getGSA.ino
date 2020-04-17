/*!
 *@file getGSA.ino
 *
 *@After clicking upload, you can see the satellite GSA protocol in the serial port
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

  myBC20.getQGNSSRD(NMEA_GSA);

  Serial.print("Mode\t\t");
  for (uint8_t i = 0; i < sGSA.NUM; i++) {
    /*
       Selection of 2D or 3D fix
       'M'=Manual, forced to switch 2D/3D mode
       'A'=Allowed to automatically switch 2D/3D mode
    */
    Serial.print(sGSA.data[i].Mode());
    Serial.print("\t\t");
  }
  Serial.println();

  Serial.print("Fix Status\t");
  for (uint8_t i = 0; i < sGSA.NUM; i++) {
    Serial.print(sGSA.data[i].FixStatus());
    Serial.print("\t\t");
  }
  Serial.println();

  Serial.print("System ID\t");
  for (uint8_t i = 0; i < sGSA.NUM; i++) {
    Serial.print(sGSA.data[i].GNSS_SystemID());
    Serial.print("\t\t");
  }
  Serial.println();

  /*
     A list of satellite being used on each channel
  */
  for (uint8_t i = 0; i < 12; i++) {
    Serial.print("CH");
    Serial.print(i + 1);
    Serial.print("\t\t");
    for (uint8_t j = 0; j < sGSA.NUM; j++) {
      Serial.print(sGSA.data[j].Statellite_CH(i));
      Serial.print("\t\t");
    }
    Serial.println();
  }
  /*
     HDOP: Horizontal Dilution Of Precision
  */
  Serial.print("PDOP\t\t");
  for (uint8_t i = 0; i < sGSA.NUM; i++) {
    Serial.print(sGSA.data[i].PDOP());
    Serial.print("\t\t");
  }
  Serial.println();

  /*
     VDOP: Vertical Dilution Of Precision
  */
  Serial.print("HDOP\t\t");
  for (uint8_t i = 0; i < sGSA.NUM; i++) {
    Serial.print(sGSA.data[i].HDOP());
    Serial.print("\t\t");
  }
  Serial.println();

  Serial.print("VDOP\t\t");
  for (uint8_t i = 0; i < sGSA.NUM; i++) {
    Serial.print(sGSA.data[i].VDOP());
    Serial.print("\t\t");
  }
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