'''file getGNSS.py
   After clicking upload, you can see all the complete satellite information on the serial monitor

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

def Display_Location_Information():
#UTC time of the anchor point
  print("Time:\t\t"),
  print(BC20.sCLK.Year),
  print("/"),
  print(BC20.sCLK.Month),
  print("/"),
  print(BC20.sCLK.Day),
  print("  "),
  print(BC20.sCLK.Hour),
  print(":"),
  print(BC20.sCLK.Minute),
  print(":"),
  print(BC20.sCLK.Second)

  print("Latitude:\t"),
  print('%.6f' % (BC20.sGGNS.LatitudeVal)),
  print(" deg "),
  print(BC20.sGGNS.LatitudeDir) 
  print("Longitude:\t"),
  print('%.6f' % (BC20.sGGNS.LongitudeVal)),
  print(" deg "),
  print(BC20.sGGNS.LongitudeDir)
  print("Altitude:\t"),
  print('%.2f' % (BC20.sGGNS.Altitude)),
  print(" m")
  print("Speed:\t\t"),
  print('%.3f' % (BC20.sGGNS.Speed)),
  print(" km/h")
  print("Heading:\t"),
  print(BC20.sGGNS.Heading),
  print(" deg")
  print("Status:\t\t"),
  print(BC20.sGGNS.FixStatus)
  print("PDOP:\t\t"),
  print(BC20.sGGNS.PDOP)
  print("HDOP:\t\t"),
  print(BC20.sGGNS.HDOP)
  print("VDOP:\t\t"),
  print(BC20.sGGNS.VDOP)
  print("")
  
def Display_Satellite_Information():
  print(BC20.sSAT.NUM),
  print(" in view.") 
  print(BC20.sSAT.USE),
  print(" in used.")
  print("PRN\t"),
  print("Elev(deg)\t"),
  print("Azim(deg)\t"),
  print("SNR(dBHz)\t"),
  print("SYS\t"),
  print("Used")
  for i in range(BC20.sSAT.NUM):
    print(BC20.sSAT.data[i].PRN),
    print("\t"),
    print(BC20.sSAT.data[i].Elev),
    print("\t\t"),
    print(BC20.sSAT.data[i].Azim),
    print("\t\t"),
    print(BC20.sSAT.data[i].SNR),
    print("\t\t"),
    print(BC20.sSAT.data[i].SYS),
    print("\t"),
    print(BC20.sSAT.data[i].Status)
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
#Is used to obtain the specified satellite information, and the parameter is used to specify the type of information to be obtained.
#The parameter is selected as follows:
  BC20.getQGNSSRD("")
  Display_Location_Information()
  Display_Satellite_Information()
  BC20.ledON()
  time.sleep(0.5)
  BC20.ledOFF()
  time.sleep(3)  