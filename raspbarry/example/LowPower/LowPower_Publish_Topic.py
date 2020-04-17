'''file LowPower_Publish_Topic.py
   This demo adds low-power content to the regular IOTdemo, where you can see the NB module being awakened 
   in a loop on the serial monitor and then upload the message to the cloud
   copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
   licence     The MIT License (MIT)
   author      [PengKaixing](kaixing.peng@dfrobot.com)
   version  V1.0
   date  2020-4-14
   get from https://www.dfrobot.com
   get from https://www.dfrobot.com.cn'''


# -*- coding: utf-8 -*
import time
import sys
sys.path.append("../..")
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

WAKEUP_PIN = 13

#Configure device certificate information
Iot_id = "zaMf62jZg"
Client_ID  = "1"
Iot_pwd    = "z-MB62jWRz"

#Configure the domain name and port number
EasyIot_SERVER = "182.254.130.180"
PORT = "1883"

#Set the Topic you need to publish to
pubTopic = "QjREoXEZg"

def ConnectCloud():
  print("Attempting MQTT connection...")
  BC20.changeColor(YELLOW)
  while BC20.connected() == False:
    print(".")
    BC20.ledON()
    time.sleep(0.5)
    BC20.ledOFF()
    time.sleep(0.5)    
    if BC20.connectServer(Client_ID, Iot_id, Iot_pwd):  
      print("Connect Server OK")
    else:
#Used to detect the connection between the device and the server
      if BC20.getQMTCONN():
        break 
#Initialize BC20
print("Starting the BC20.Please wait...")
BC20.changeColor(RED)
while BC20.powerOn()==False:
  print(".")
  BC20.ledON()
  time.sleep(0.5)
  BC20.ledOFF()
  time.sleep(0.5)
print("BC20 started successfully!\r\n")  

#Check whether SIM card is inserted
print("Checking SIM card ...")
BC20.changeColor(GREEN)
while BC20.checkNBCard() == False:
  print("Please insert the NB SIM card...")
  BC20.ledON()
  time.sleep(0.5)
  BC20.ledOFF()
  time.sleep(0.5)  
print("SIM card check OK!\r\n") 
  
BC20.getGSN(1)
print("BC20 IMEI:")
print(BC20.imei)
print("\r\n")
print("SIM card ICCID:")
print(BC20.getQCCID())
print("SIM card IMSI: ")
print(BC20.getIMI())

BC20.changeColor(BLUE)
while BC20.getGATT()==0 :
  print(".");
  BC20.ledON()
  time.sleep(0.5)
  BC20.ledOFF()
  time.sleep(0.5)   
print("Network is connected!")

print("Connecting to DFRobot Easy-IoT")
BC20.setServer(EasyIot_SERVER, PORT)
print("Server is available!")

ConnectCloud()

BC20.setPSMMode(1)

BC20.configSleepMode(1)

while True:
  print("send message to cloud...")  
  BC20.publish(pubTopic,"Hello")
  print("Message is sent.") 
#The Controller and BC20(NB-IoT module) is bridged by an STM32.
#To put the module into low power mode,
#simply put STM32 into Deep Sleep Mode.
#BC20 will reaches lowest power(PSM + Deep Sleep Mode) automatically.
#To wake up the module,
#wake up STM32 first and then BC20.  
  if BC20.stmLowpower() == False:
    print("STM32 enter Sleep Mode!")
    
#Wakeup interval    
  time.sleep(6)
  
#A rising edge on IRQ to wake up STM32  
  BC20.stmWakeup(WAKEUP_PIN)   
  print("STM32 exits Sleep Mode!")
#Wake up BC20  
  BC20.changeColor(WHITE)
  while BC20.BC20Wakeup() == False:
    BC20.ledON()
    time.sleep(0.5)
    BC20.ledOFF()
    time.sleep(0.5)
  print("BC20 exits PSM successfully!")        
  
  