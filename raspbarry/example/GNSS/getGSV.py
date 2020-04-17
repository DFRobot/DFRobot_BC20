'''file getGSV.py
   After clicking upload, you can see the satellite GSV protocol in the serial port
   copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
   licence     The MIT License (MIT)
   author      [PengKaixing](kaixing.peng@dfrobot.com)
   version  V1.0
   date  2020-4-13
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

def Display_Satellite_Information():
  print(BC20.sSAT.NUM),
  print(" in view.")
  
#Satellite PRN number
  print("PRN\t"),
#Elevation angle, unit in degrees
  print("Elev(deg)\t"),
#Azimuth angle, unit in degrees
  print("Azim(deg)\t"),
#Signal Noise Ratio, unit in dBHz
  print("SNR(dBHz)\t"),
  print("SYS");
  for i in range(BC20.sSAT.NUM):
    print(BC20.sSAT.data[i].PRN),
    print("\t"),
    print(BC20.sSAT.data[i].Elev),
    print("\t\t"),
    print(BC20.sSAT.data[i].Azim),
    print("\t\t"),
    print(BC20.sSAT.data[i].SNR),
    print("\t\t"),
    print(BC20.sSAT.data[i].SYS)  
  print("")     

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

#Disable sleep mode
BC20.configSleepMode(0)

#Power up GNSS
print("Turning on GNSS ... ")
BC20.setQGNSSC(1)
BC20.changeColor(YELLOW)
if BC20.getQGNSSC() == 0:
  print(".")
  BC20.ledON()
  time.sleep(0.5)
  BC20.ledOFF()
  time.sleep(0.5)
print("GNSS is ON.")
BC20.changeColor(CYAN)

while True:
  BC20.getQGNSSRD("\"NMEA/GSV\"")
  Display_Satellite_Information()
  BC20.ledON()
  time.sleep(0.5)
  BC20.ledOFF()
  time.sleep(3)  
  