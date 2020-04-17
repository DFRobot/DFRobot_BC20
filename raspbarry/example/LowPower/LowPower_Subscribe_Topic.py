'''file LowPower_Subscribe_Topic.py
   After filling in the relevant information and clicking upload, the 
   NB module will enter low power consumption. You can issue a message 
   through the cloud to wake up the NB module, but this message will not 
   be displayed. After waking up, you can normally communicate with the 
   device from the cloud.You can modify the code to select whether the 
   NB module will enter low power again, just remember to wake it up
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

sleepFlag=False
timeStamp=0
SLEEP_INTERVAL=10000

#Configure device certificate information
Iot_id = "zaMf62jZg"
Client_ID  = "1"
Iot_pwd    = "z-MB62jWRz"

#Configure the domain name and port number
EasyIot_SERVER = "182.254.130.180"
PORT = "1883"

#Set the Topic you need to publish to
subTopic = "QjREoXEZg"

#Each time the module receive a message, this function will be called.
#It will print the message and corresponding topic.
def callback():
  global sleepFlag
  global timeStamp
  print("Recevice [Topic:"+BC20.topic+"] ")
  receivedData=BC20.payload
  print(receivedData)
  timeStamp=int(round(time.time() * 1000))
  sleepFlag = True   

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
  while BC20.subTopic('0', '1', subTopic, '0') == False:
    print(".")
  print("\nTopic:"+subTopic+" subscribed!")

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

#The module will automatically attempt to connect to the network (mobile station).
#Check whether it is connected to the network.
BC20.changeColor(BLUE)
while BC20.getGATT()==0 :
  print(".");
  BC20.ledON()
  time.sleep(0.5)
  BC20.ledOFF()
  time.sleep(0.5)   
print("Network is connected!\r\n")

#Set callback function
BC20.setCallback(callback)

print("Connecting to DFRobot Easy-IoT")

#Configure IoT Server
BC20.setServer(EasyIot_SERVER, PORT)
print("Server is available!")

#Enable Deep Sleep Mode
BC20.configSleepMode(1)
BC20.setPSMMode(1)

#Conect to DFRobot Easy-IoT
ConnectCloud()


if BC20.stmLowpower() == False:
  print("Module enters Sleep Mode!")

BC20.setCallback(callback)

while True:
#Note:
#1.The first message sent from the cloud is to wake up the module.
#This meassage will be THROWN AWAY.

#2.The second message and the following messages will be received and printed
#before the module enters sleep module.

#3.When the module enters sleep mode again, another wakeup message
#is needed before sending the valid messages.

#4.Interval between entering each sleep mode is determined
#by SLEEP_INTERVAL.
  BC20.loop()
  if sleepFlag == True:
    if (int(round(time.time() * 1000))) - timeStamp  >SLEEP_INTERVAL:
      sleepFlag=False
      if BC20.stmLowpower() == False:
        print("Module enters Sleep Mode!")
  
  
  
  