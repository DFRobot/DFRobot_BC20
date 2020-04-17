'''file Publish_Topic.py
   Messages from the device to the cloud are sent by filling in the relevant information of the ALIYUN platform

   If the Message is sent successfully, you can see "Message is sent." in the serial port, and you can see the 
   Message you sent on the ALIYUN platform

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
     
while True:
  BC20.changeColor(RED)
  for i in range(3):
    BC20.ledON()
    time.sleep(0.5)
    BC20.ledOFF()
    time.sleep(0.5)
  
  BC20.changeColor(GREEN)
  for i in range(3):
    BC20.ledON()
    time.sleep(0.5)
    BC20.ledOFF()
    time.sleep(0.5)
   
  BC20.changeColor(BLUE)
  for i in range(3):
    BC20.ledON()
    time.sleep(0.5)
    BC20.ledOFF()
    time.sleep(0.5)

  BC20.changeColor(YELLOW)
  for i in range(3):
    BC20.ledON()
    time.sleep(0.5)
    BC20.ledOFF()
    time.sleep(0.5)
              
  BC20.changeColor(PURPLE)
  for i in range(3):
    BC20.ledON()
    time.sleep(0.5)
    BC20.ledOFF()
    time.sleep(0.5)
   
  BC20.changeColor(CYAN)
  for i in range(3):
    BC20.ledON()
    time.sleep(0.5)
    BC20.ledOFF()
    time.sleep(0.5)  
    
  BC20.changeColor(WHITE)
  for i in range(3):
    BC20.ledON()
    time.sleep(0.5)
    BC20.ledOFF()
    time.sleep(0.5)             