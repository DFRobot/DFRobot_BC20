/*!
 *@file Subscribe_Topic.ino
 * 
 *@You can subscribe to the EASY-IOT topic by filling in the information about the EASY-IOT platform
 *
 *@By using the EASY-IOT platform to publish messages, you can see the corresponding topic messages
 *@on the serial monitor
 *
 *@copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@licence     The MIT License (MIT)
 *@author      [PengKaixing](kaixing.peng@dfrobot.com)
 *@version  V1.0
 *@date  2020-3-19
 *@get from https://www.dfrobot.com
 *@get from https://www.dfrobot.com.cn
*/
#include "DFRobot_BC20.h"

/*7 colors are available*/
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
char* subTopic = "your_subTopic";

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
   Each time the module receive a message, this function will be called.
   It will print the message and corresponding topic.
*/
void callback(char * topic, uint8_t * payload, unsigned int len) {
  Serial.print("Recevice [Topic:");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < len; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  //Control onboard RGB LED according to received message.
  char* receivedData =(char*)payload;
  if (strcmp(receivedData,"OFF")== 0) {
    myBC20.LED_OFF();
    //    Serial.println("LED is OFF.");
  }
  else if (strcmp(receivedData,"RED")== 0) {
    myBC20.changeColor(RED);
    myBC20.LED_ON();
    //    Serial.println("LED is red.");
  }
  else if (strcmp(receivedData,"GREEN")== 0) {
    myBC20.changeColor(GREEN);
    myBC20.LED_ON();
    //      Serial.println("LED is green.");
  }
  else if (strcmp(receivedData,"BLUE")== 0) {
    myBC20.changeColor(BLUE);
    myBC20.LED_ON();
    //      Serial.println("LED is blue.");
  }
  else if (strcmp(receivedData,"YELLOW")== 0) {
    myBC20.changeColor(YELLOW);
    myBC20.LED_ON();
    //      Serial.println("LED is yellow.");
  }
  else if (strcmp(receivedData,"PURPLE")== 0) {
    myBC20.changeColor(PURPLE);
    myBC20.LED_ON();
    //      Serial.println("LED is purple.");
  }
  else if (strcmp(receivedData,"CYAN")== 0) {
    myBC20.changeColor(CYAN);
    myBC20.LED_ON();
    //      Serial.println("LED is cyan.");
  }
  else if (strcmp(receivedData,"WHITE")== 0) {
    myBC20.changeColor(WHITE);
    myBC20.LED_ON();
    //      Serial.println("LED is white.");
  }
  else {
    ;
  }
}

void ConnectCloud() {
  Serial.print("Attempting MQTT connection...");
  while (!myBC20.connected()) {
    Serial.print(".");
    if (myBC20.connect(Client_ID, Iot_id, Iot_pwd)) {
      Serial.println("\nConnect Server OK");
    } else {
      /**
         Used to detect the connection between the device and the server
      */
      if (myBC20.getQMTCONN())
        break;
    }
  }

  while (!myBC20.subTopic('0', '1', subTopic, '0')) {
    Serial.print(".");
  }
  Serial.print("\nTopic:");
  Serial.print(subTopic);
  Serial.println(" subscribed!");
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
  Serial.println("started successfully!");

  /*Check whether SIM card is inserted*/
  Serial.print("Checking SIM card ...");
  myBC20.changeColor(GREEN);
  while (!myBC20.checkNBCard()) {
    Serial.print(".");
    myBC20.LED_ON();
    delay(500);
    myBC20.LED_OFF();
    delay(500);
  }
  Serial.println("OK!");

  //Disable sleep mode
  myBC20.configSleepMode(eSleepMode_Disable);
  //Disable PSM
  myBC20.setPSMMode(ePSM_OFF);

  /**
     The module will automatically attempt to connect to the network (mobile station).
     Check whether it is connected to the network.
  */
  Serial.print("Connecting network ...");
  myBC20.changeColor(BLUE);
  while (myBC20.getGATT() == 0) {
    Serial.print(".");
    myBC20.LED_ON();
    delay(500);
    myBC20.LED_OFF();
    delay(500);
  }
  Serial.println("connected!");

  //Set callback function
  myBC20.setCallback(callback);

  Serial.println("Connecting to DFRobot Easy-IoT");

  //Configure IoT Server
  myBC20.setServer(EasyIot_SERVER, PORT);
  Serial.println("Server is available!");

  //Conect to DFRobot Easy-IoT
  ConnectCloud();

}

void loop() {
  myBC20.loop();
}
