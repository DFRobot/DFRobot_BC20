/*!
 *@file LowPower_Publish_Topic.ino
 *
 *@This demo adds low-power content to the regular IOTdemo, where you can see the NB module being awakened 
 *@in a loop on the serial monitor and then upload the message to the cloud
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
#define  RED 0
#define  BLUE 1
#define  GREEN 2
#define  YELLOW 3
#define  PURPLE 4
#define  CYAN 5
#define  WHITE 6

/*Configure device certificate information*/
char* Iot_id = "your_Iot_id";
char* Client_ID  = "your_Client_ID";
char* Iot_pwd    = "your_Iot_pwd";

/*Configure the domain name and port number*/
char* EasyIot_SERVER = "182.254.130.180";
char* PORT = "1883";

/*Set the Topic you need to publish to*/
char* pubTopic = "your_pubTopic";

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
SoftwareSerial ss(PIN_RXD, PIN_TXD);
DFRobot_BC20_SW_Serial myBC20(&ss);
#endif

//Connect to IRQ to wake the module up
#define WAKEUP_PIN 7

void ConnectCloud() {
  Serial.print("Attempting MQTT connection...");
  myBC20.changeColor(YELLOW);
  while (!myBC20.connected()) {
    Serial.print(".");
    myBC20.LED_ON();
    delay(500);
    myBC20.LED_OFF();
    delay(500);

    if (myBC20.connect(Client_ID, Iot_id, Iot_pwd)) {
      Serial.println("server is available");
    } else {

      //Used to detect the connection between the device and the server
      if (myBC20.getQMTCONN())
        break;
    }
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

  /*If you are using a large RAM master, you can turn on 
   *the following function to see the details of the SIM card
   *Print IMEI, ICCID and IMSI*/
//  myBC20.getGSN(IMEI);
//  Serial.print("BC20 IMEI: ");
//  Serial.println(sGSN.imei);
//  Serial.print("SIM card ICCID:");
//  Serial.print(myBC20.getQCCID());
//  Serial.print("SIM card IMSI: ");
//  Serial.println((char *)myBC20.getIMI());

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

  Serial.println("Connecting to DFRobot Easy-IoT");

  //Configure IoT Server
  myBC20.setServer(EasyIot_SERVER, PORT);
  Serial.println("Server is available!");
  
  //Conect to DFRobot Easy-IoT
  ConnectCloud();
  Serial.println("Cloud connect success!");

  //Enable PSM (modem)
  myBC20.setPSMMode(ePSM_ON);

  //Enable Deep Sleep Mode (CPU)
  myBC20.configSleepMode(eSleepMode_DeepSleep);
}

void loop() {
  Serial.println("send message to cloud...");
  myBC20.publish(pubTopic, "Hello");
  Serial.println("Message is sent.");

  /*
    The Controller and BC20(NB-IoT module) is bridged by an STM32.

    To put the module into low power mode,
    simply put STM32 into Deep Sleep Mode.
    BC20 will reaches lowest power(PSM + Deep Sleep Mode) automatically.

    To wake up the module,
    wake up STM32 first and then BC20.
  */
  if (!myBC20.stmLowpower()) {
    Serial.println("STM32 enter Sleep Mode!");
  }

  //Wakeup interval
  delay(60000);

  //A rising edge on IRQ to wake up STM32
  myBC20.stmWakeup(WAKEUP_PIN);
  Serial.println("STM32 exits Sleep Mode!");
  //Wake up BC20
  myBC20.changeColor(WHITE);
  while (!myBC20.BC20Wakeup()) {
    myBC20.LED_ON();
    delay(500);
    myBC20.LED_OFF();
    delay(500);
  }
  Serial.println("BC20 exits PSM successfully!");

}
