'''file getGLL.py
 *
 *@After clicking upload, you can see the satellite GLL protocol in the serial port
 *
 *@copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@licence     The MIT License (MIT)
 *@author      [PengKaixing](kaixing.peng@dfrobot.com)
 *@version  V1.0
 *@date   2020-4-13
 *@get from https://www.dfrobot.com
 *@get from https://www.dfrobot.com.cn'''

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

print("Starting the BC20.Please wait...")
BC20.changeColor(RED)
while BC20.powerOn()==False:
  print(".")
  BC20.ledON()
  time.sleep(0.5)
  BC20.ledOFF()
  time.sleep(0.5)
print("BC20 started successfully!\r\n") 

BC20.configSleepMode(0)

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
  BC20.getQGNSSRD("\"NMEA/GLL\"")
  
#UTC time, format: hhmmss.ss, ex. 162436.54 => 16:24:36.54
#h - hours
#m - minutes
#s - seconds
  print("UTC Time: "),
  print(BC20.sGLL.UTC_Time) 

#Latitude, format: ddmm.mmmmm, ex. 3150.7820 => 31deg 50.7820min
#d - degrees
#m - minutes
  print("Latitude: "),
  print(BC20.sGLL.LatitudeVal), 
  
#Latitude north or south
#N - North
#S - South
  print(BC20.sGLL.LatitudeDir)
  
#Longitude, format: dddmm.mmmmm, ex. 12135.6794 = 121deg 35.6794min
#d - degrees
#m - minutes
  print("Longitude: "),
  print(BC20.sGLL.LongitudeVal),

#Longitude wast or west
#E - East
#W - West
  print(BC20.sGLL.LongitudeDir)
  
#DataStatus
#V - Invalid
#A - Valid

  print("Data Status: "),
  print(BC20.sGLL.DataStatus)

#Positioning Mode
#N - No fix
#A - Autonomous GPS fix
#D - Differential GPS fix

  print("Positioning Mode: "),
  print(BC20.sGLL.PositionMode)
  print("")

  BC20.ledON()
  time.sleep(0.5)
  BC20.ledOFF()
  time.sleep(3)  
  
  