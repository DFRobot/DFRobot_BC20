'''file BC20_LowPower.py
   Connect the ISR pin of the NB module with the WAKEUP_PIN (custom) of the main control 
   board through the dupont line and click to start uploading
   On the serial monitor, you can see that the NB module is periodically awakened and entered by the PSM
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

#To wake up STM32,
#the IRQ pin on the module should be connected to the WAKEUP_PIN
WAKEUP_PIN=13

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

#Turn on PSM mod(PSM is ON by default)
BC20.setPSMMode(1)

#Deep/Light Sleep Mode.(CPU)
#When in deep sleep mode, BC20 will not respond to any AT commands from the master panel
#Until it is awakened by STM32 (again, BC20 exits the PSM)
#When Light Sleep Mode is entered, BC20 can still receive AT commands from ESP32.
#However, it's power consumption is greater than the the former.
BC20.configSleepMode(1)

while True:
#The Controller and BC20 is bridged by an STM32(on the module).
#To put the module into low power mode,
#put STM32 into Deep Sleep Mode.
#BC20 will enter lowest power(PSM + Deep Sleep Mode) automatically.
#To wake up the module,
#wake up STM32 first and then BC20.
  if BC20.stmLowpower() == False:
    print("STM32 enters sleep mode!")
  time.sleep(10)
  print("Wake up STM32!")
  BC20.stmWakeup(WAKEUP_PIN)
  
  print("BC20 is waking up ...")
  
  while BC20.BC20Wakeup() == False:
    print("."),
    time.sleep(1) 
  print("OK") 
  time.sleep(10)  



