/*!
 *@file SignalDetector.ino
 *
 *@After clicking upload, you can see the current network signal on the serial monitor.
 *@ Meanwhile, you can judge the signal strength by the flashing of LED light of NB module
 *
 *@Copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@licence     The MIT License (MIT)
 *@author      [Peng kaixing](kaixing.peng@dfrobot.com)
 *@version  V1.0
 *@date   2020-3-19
 *@get from https://www.dfrobot.com
 *@get from https://www.dfrobot.com.cn
*/

#include <DFRobot_BC20_Gravity.h>

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

/*
   The signal strength is indicated by both numerical value in dBm
   and the flashing LED:
   Strong signal - fast flash
   Medium signal - slow flash
   Weak signal - burst flash.
*/
static void NB_Signal_Fun() {

  /*Get the NB-IoT signal strength.*/
  myBC20.getSQ();

  /*Signal quality RSSI<10, WEAK signal strength*/
  myBC20.changeColor(WHITE);
  if (sSQ.rssi < 10 || sSQ.rssi == 99) {
    if (sSQ.rssi == 99) {
      Serial.println("Signal not detectable");
    } else if (sSQ.rssi == 0) {
      Serial.println("Signal Strength: -113 dBm or less");
    } else {
      Serial.print("Signal Strength: ");
      Serial.print((sSQ.rssi - 2) * 2 - 109);
      Serial.println(" dBm Weak");
    }

    //Burst flash
    myBC20.LED_ON();
    delay(100);
    myBC20.LED_OFF();
    delay(900);
  }

  /*Signal quality 10<=RSSI<25, MEDIUM signal strength*/
  else if (sSQ.rssi >= 10  && sSQ.rssi < 25) {
    Serial.print("Signal Strength: ");
    Serial.print((sSQ.rssi - 2) * 2 - 109);
    Serial.println(" dBm Medium");

    //Slow flash
    myBC20.LED_ON();
    delay(500);
    myBC20.LED_OFF();
    delay(500);
  }

  /*Signal quality RSSI>=25, STRONG signal strength*/
  else if (sSQ.rssi >= 25) {
    if (sSQ.rssi < 31) {
      Serial.print("Signal Strength: ");
      Serial.print((sSQ.rssi - 2) * 2 - 109);
      Serial.println(" dBm Strong");
    } else if (sSQ.rssi == 31) {
      Serial.print("Signal Strength: -51 dBm or greater");
    }

    //Fast flash
    for (int i = 0; i < 5 ; i++) {
      myBC20.LED_ON();
      delay(100);
      myBC20.LED_OFF();
      delay(100);
    }
  }
  else {
    ;
  }
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

  /*Check whether SIM card is inserted*/
  Serial.println("Checking SIM card ...");
  myBC20.changeColor(GREEN);
  while (!myBC20.checkNBCard()) {
    Serial.println("Please insert the NB SIM card !");
    myBC20.LED_ON();
    delay(500);
    myBC20.LED_OFF();
    delay(500);
  }
  Serial.println("SIM card check OK!");

  /*Print IMEI, ICCID and IMSI*/
  myBC20.getGSN(IMEI);
  Serial.print("BC20 IMEI: ");
  Serial.println(sGSN.imei);
  Serial.print("SIM card ICCID:");
  Serial.print(myBC20.getQCCID());
  Serial.print("SIM card IMSI: ");
  Serial.println((char *)myBC20.getIMI());

  /**
     The module will automatically attempt to connect to the network (mobile station).
     Check whether it is connected to the network.
  */
  Serial.println("Connecting network ...");
  myBC20.changeColor(BLUE);
  while (myBC20.getGATT() == 0) {
    Serial.print(".");
    myBC20.LED_ON();
    delay(500);
    myBC20.LED_OFF();
    delay(500);
  }
  Serial.println("Network is connected!");
}

void loop() {
  /*Get local NB-IoT signal strength and print*/
  NB_Signal_Fun();
}
