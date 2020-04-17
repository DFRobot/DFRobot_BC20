/*!
 *@file BC20_LowPower.ino
 *
 *@Connect the ISR pin of the NB module with the WAKEUP_PIN (custom) of the main control 
 *@board through the dupont line and click to start uploading
 *
 *@On the serial monitor, you can see that the NB module is periodically awakened and entered by the PSM
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


/*
   To wake up STM32,
   the IRQ pin on the module should be connected to the WAKEUP_PIN
*/
#define WAKEUP_PIN 7

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

  /*Turn on PSM mod(PSM is ON by default)*/
  myBC20.setPSMMode(ePSM_ON);


  /*
      Deep/Light Sleep Mode.(CPU)

      When in deep sleep mode, BC20 will not respond to any AT commands from the master panel
      Until it is awakened by STM32 (again, BC20 exits the PSM)

      When Light Sleep Mode is entered, BC20 can still receive AT commands from ESP32.
      However, it's power consumption is greater than the the former.

  */
  myBC20.configSleepMode(eSleepMode_DeepSleep);

}

void loop() {

  /*
    The Controller and BC20 is bridged by an STM32(on the module).

    To put the module into low power mode,
    put STM32 into Deep Sleep Mode.
    BC20 will enter lowest power(PSM + Deep Sleep Mode) automatically.

    To wake up the module,
    wake up STM32 first and then BC20.
  */

  //Put STM32 into sleep mode.
  if (!myBC20.stmLowpower()) {
    Serial.println("STM32 enters sleep mode!");
  }

  delay(30000);

  //Wake up STM32.
  Serial.println("Wake up STM32!");
  //A rising edge on IRQ to wake up STM32
  myBC20.stmWakeup(WAKEUP_PIN);

  //wake up BC20 from both PSM and Deep Sleep Mode
  Serial.print("BC20 is waking up ...");
  while (!myBC20.BC20Wakeup()) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("OK");
  delay(10000);
}
