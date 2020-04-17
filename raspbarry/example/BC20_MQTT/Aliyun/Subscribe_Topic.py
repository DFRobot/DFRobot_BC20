'''file Subscribe_Topic.py
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
 *@date  2020-4-10
 *@get from https://www.dfrobot.com
 *@get from https://www.dfrobot.com.cn'''


# -*- coding: utf-8 -*
import time
import sys
sys.path.append("../../..")
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

#Configure device certificate information
ProductKey = "your_ProductKey"
ClientId = "your_ClientId"
DeviceName = "your_DeviceName"
DeviceSecret = "your_DeviceSecret"

#Configure the domain name and port number
ALIYUN_SERVER =".iot-as-mqtt.cn-shanghai.aliyuncs.com"
PORT = "1883"

#Product identifier that needs to be operated

Identifier = "your_Identifier"

#TOPIC that need to be published
subTopic = "your_subTopic"

'''Each time the module receive a message, this function will be called.
   It will print the message and corresponding topic.'''
def callback():
  print("Recevice [Topic:"+BC20.topic+"] ")
  receivedData=BC20.payload
  print(receivedData)

def ConnectCloud():
  print("Attempting MQTT connection...")
  BC20.changeColor(YELLOW)
  while BC20.connected() == False:
    print(".")
    BC20.ledON()
    time.sleep(0.5)
    BC20.ledOFF()
    time.sleep(0.5)    
    if BC20.connect_Aliyun(ProductKey,DeviceName,DeviceSecret):  
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

#Print IMEI, ICCID and IMSI  
BC20.getGSN(1)
print("BC20 IMEI:")
print(BC20.imei)
print("\r\n")
print("SIM card ICCID:")
print(BC20.getQCCID())
print("SIM card IMSI: ")
print(BC20.getIMI())

'''The module will automatically attempt to connect to the network (mobile station).
   Check whether it is connected to the network.'''
print("Connecting network ...")
BC20.changeColor(BLUE)
while BC20.getGATT()==0 :
  print(".");
  BC20.ledON()
  time.sleep(0.5)
  BC20.ledOFF()
  time.sleep(0.5)   
print("Network is connected!\r\n")

print("Connecting to ALIYUN_IoT")
#Configure IoT Server
while BC20.setAliyunserver(ProductKey,ALIYUN_SERVER,PORT) == False:
  print("Connection to server failed!")
  time.sleep(1)

print("Server is available!")
ConnectCloud()
BC20.setCallback(callback)

while True:
  BC20.loop()        