'''file BC20_Serial.py
   Send AT commands to the BC20 module via USB Serial
   copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
   licence     The MIT License (MIT)
   author      [PengKaixing](kaixing.peng@dfrobot.com)
   version  V1.0
   date  2020-3-19
   get from https://www.dfrobot.com
   get from https://www.dfrobot.com.cn'''


# -*- coding: utf-8 -*
import time
import sys
sys.path.append("..")
import DFRobot_BC20

RED = 'R'
BLUE = 'B'
GREEN = 'G'
YELLOW = 'Y'
PURPLE = 'P'
CYAN = 'C'
WHITE = 'W'

'''For general controllers. Communicate by IIC
   Connect Instructions
      Raspberry Pi   |    Module(BC20)
      SDA(GPIO3)     |       D/T
      SCL(GPIO5)     |       C/R
      GND(GPIO6)     |       GND
      VCC(GPIO2)     |       VCC

   IIC address(A0,A1)
     0x30:(A0=0,A1=0)
     0x31:(A0=0,A1=1)
     0x32:(A0=1,A1=0)
     0x33:(A0=1,A1=1) default'''

'''For general controllers. Communicate by Serial
   Connect Instructions
      Raspberry Pi   |    Module(BC20)
      RXD(GPIO10)    |       D/T
      TXD(GPIO8)     |       C/R
      GND(GPIO6)     |       GND
      VCC(GPIO2)     |       VCC'''
          
BC20=DFRobot_BC20.DFRobot_BC20_IIC(0x33)

#BC20=DFRobot_BC20.DFRobot_BC20_Serial()

#Initialize BC20
print("Starting the BC20.Please wait. . . ")
BC20.changeColor(RED)
while BC20.powerOn()==False:
  BC20.ledON()
  time.sleep(0.5)
  BC20.ledOFF()
  time.sleep(0.5)
print("BC20 started successfully!\r\n")  

#Deep Sleep Mode is automatically enable every time upon power up.
#When this mode is entered, 
#BC20 will not respond any AT commands from the controller
#Disable sleep mode to ensure BC20 always responding AT commands
#BC20.configSleepMode(0)

while True:
#Each AT command should begin with "AT" or "at" and end with "Carriage return".
#The commands can be upper-case or lower-case. ex. "AT+CSQ" or "at+csq".
  BC20.sendCmd(raw_input("please input:\r\n"))
  time.sleep(0.5)
  if BC20.available():
    print("readData from BC20:\r\n")
    print(BC20.readData(1000))