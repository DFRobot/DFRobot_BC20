# -*- coding: utf-8 -*
import serial
import time
import ctypes
import smbus
import RPi.GPIO as GPIO

from enum import Enum

ser = serial.Serial("/dev/ttyAMA0", 9600)

class Node:
  def __init__(self):
    self.handle=0
    self.datalen=0
    self.data="" 
    self.next=None
    
front=Node()
rear=Node()
front=None
rear=None
color='W' 

class sGSV(ctypes.Structure):
  _fields_= [('PRN',ctypes.c_char_p),
             ('Elev',ctypes.c_char_p),
             ('Azim',ctypes.c_char_p),
             ('SNR',ctypes.c_char_p),
             ('Status',ctypes.c_char_p),
             ('SYS',ctypes.c_char_p)]

class sSAT_t(ctypes.Structure):
  _fields_=[('NUM',ctypes.c_int),
            ('USE',ctypes.c_int),
            ('data',sGSV*40)]  

class sGSAD(ctypes.Structure):
  _fields_=[('Mode',ctypes.c_char_p),
            ('FixStatus',ctypes.c_char_p),
            ('Statellite_CH',(ctypes.c_char_p)*12),     
            ('PDOP',ctypes.c_char_p),
            ('HDOP',ctypes.c_char_p), 
            ('VDOP',ctypes.c_char_p),
            ('GNSS_SystemID',ctypes.c_char_p)]

            
class sGSA_t(ctypes.Structure):
  _fields_=[('NUM',ctypes.c_int),
            ('data',sGSAD*2)]          

class sRMC_t(ctypes.Structure):
  _fields_=[('UTC_Time',ctypes.c_char_p),
            ('DataStatus',ctypes.c_char_p),
            ('LatitudeVal',ctypes.c_char_p),
            ('LatitudeDir',ctypes.c_char_p),
            ('LongitudeVal',ctypes.c_char_p),
            ('LongitudeDir',ctypes.c_char_p),
            ('GroundSpeed',ctypes.c_char_p),
            ('GroundHeading',ctypes.c_char_p),
            ('UTC_Date',ctypes.c_char_p),
            ('MagDeclination',ctypes.c_char_p),
            ('MagDeclinationDir',ctypes.c_char_p),
            ('PositioningMode',ctypes.c_char_p),
            ('NaviStatus',ctypes.c_char_p)]
            
class sCLK_t(ctypes.Structure):
  _fields_=[('Year',ctypes.c_int),
            ('Month',ctypes.c_int),
            ('Day',ctypes.c_int),
            ('Hour',ctypes.c_int),
            ('Minute',ctypes.c_int),
            ('Second',ctypes.c_int),
            ('TimeEquation',ctypes.c_int),
            ('Mode',ctypes.c_char_p)]

class sVTG_t(ctypes.Structure):
  _fields_=[('GroundCourse_True',ctypes.c_char_p),
            ('GroundCourse_Mag',ctypes.c_char_p),
            ('GroundCourse_Knots',ctypes.c_char_p),
            ('GroundCourse_Kmh',ctypes.c_char_p),
            ('PositioningMode',ctypes.c_char_p)]

class sGGA_t(ctypes.Structure):
  _fields_=[('UTC_Time',ctypes.c_char_p),
            ('LatitudeVal',ctypes.c_char_p),
            ('LatitudeDir',ctypes.c_char_p),
            ('LongitudeVal',ctypes.c_char_p),
            ('LongitudeDir',ctypes.c_char_p),
            ('Status',ctypes.c_char_p),
            ('StatelliteNum',ctypes.c_char_p),
            ('HDOP',ctypes.c_char_p),
            ('Altitude',ctypes.c_char_p),
            ('GeoID_Separation',ctypes.c_char_p),
            ('DGPS_Age',ctypes.c_char_p),
            ('DGPS_StationID',ctypes.c_char_p)]
 
class sGLL_t(ctypes.Structure):
  _fields_=[('LatitudeVal',ctypes.c_char_p),
           ('LatitudeDir',ctypes.c_char_p),
           ('LongitudeVal',ctypes.c_char_p),
           ('LongitudeDir',ctypes.c_char_p),
           ('UTC_Time',ctypes.c_char_p),
           ('DataStatus',ctypes.c_char_p),
           ('PositionMode',ctypes.c_char_p)]

class sGGNS_t(ctypes.Structure):
  _fields_=[('NUM',ctypes.c_int),
            ('LatitudeVal',ctypes.c_float),
            ('LatitudeDir',ctypes.c_char_p),
            ('LongitudeVal',ctypes.c_float),
            ('LongitudeDir',ctypes.c_char_p),
            ('Altitude',ctypes.c_float),
            ('Speed',ctypes.c_float),
            ('Heading',ctypes.c_float),
            ('Statellite_CH',((ctypes.c_char_p)*12)*2),
            ('NUM',ctypes.c_char_p),
            ('PDOP',ctypes.c_char_p),
            ('HDOP',ctypes.c_char_p),
            ('VDOP',ctypes.c_char_p),
            ('StatelliteNum',ctypes.c_int)]                      

class DFRobot_BC20(object):

  def __init__(self): 
    #e_Mode 
    self.eSleepMode_Disable="0"
    self.eSleepMode_DeepSleep="1"
    self.eSleepMode_Light="2" 
    #sGSN_t
    self.sn=""
    self.imei=""
    self.imeisv="" 
    self.svn=""
    #sSIMCard
    self.IMEI_NUM=""
    #sCLK_t
    self.Year=0
    self.Month=0
    self.Day=0
    self.Hour=0
    self.Minute=0
    self.Second=0
    self.Mode=""
    self.TimeEquation=0
    #sSQ
    self.rssi=0
    #mqtt_Connec
    self.mqttConneced=0
    #callback_data
    self.topic=""
    self.payload=""
    self.callback=None
    #sSAT
    self.sSAT=sSAT_t()
    #sGSA
    self.sGSA=sGSA_t()
    #sRMC
    self.sRMC=sRMC_t()
    #sCLK
    self.sCLK=sCLK_t()
    #sVTG
    self.sVTG=sVTG_t()
    #sGGA
    self.sGGA=sGGA_t()
    #sGLL
    self.sGLL=sGLL_t()
    #sGGNS
    self.sGGNS=sGGNS_t()
    
    self._TAUValue = "10000010"
    self._AcTValue = "01000011"
         
  def enqueue(self,handle,data):
    global front
    global rear
    new_data=Node()  
    new_data.datalen=len(data)
    new_data.handle=handle  
    new_data.data = data
    if rear==None:      
      front = new_data
    else:
      rear.next = new_data    
    rear = new_data         
    rear.next = None    

  def dequeue(self):   
    global front
    global rear   
    if front == None:
      return
    else:
      node=front
      front = front.next    
      return node

  def flushQueue(self):
    global front
    global rear  
    if(rear!=None):  
      front=None    
      rear=None             

  def getRecDataforNum(self,num):
    global front
    global rear
    if rear == None:
      return
    else:
      if rear.data=="OK\r\n" :
        p=front
        for i in range(num-1): 
          if p==None:
            return  
          p=p.next    
        return p
  
  def getRecDataforNum_NoCheck(self,num):
    global front
    global rear
    if rear == None:
      return
    else:
      p=front
      for i in range(num-1): 
        if p==None:
          return  
        p=p.next      
      return p            
        
  def getRecData(self,INFO):
    Data=self.getRecDataforNum(2)
    if INFO==0:
      if Data !=None:
        self.IMEI_NUM=Data.data      

  def getIntforString(self,CMD,basic,n):
    self.flushQueue()
    self.sendCmd(CMD)
    self.receviceATCMD(3000)
    Data=self.getRecDataforNum(2)
    if Data != None:
      if n==1: 
        tempStr=Data.data
        tempStr = self.removeSthString("+"+basic+":",tempStr);
        tempStr = self.GetSthfrontString("\r\n",tempStr)
        if len(tempStr)>15:
          return 0
        tempStr = int(tempStr) 
        return tempStr
      else :
        return 0
    else:
      return 0    
  
  def changeColor(self,newcolor):
    global color
    color=newcolor
  
  def ledON(self):
    global color
    self.flushQueue()
    self.sendCmd("LED_"+color+"_ON")
    
  def ledOFF(self):
    self.flushQueue()
    self.sendCmd("LED_W_OFF")    
  
  def checkRecdata(self,str):
    p=self.dequeue()
    while p!=None:
      if p.data==str:
        return True  
      p=self.dequeue()  
    return False
    
  def checkBC20(self):
    self.flushQueue()
    self.sendCmd("")
    self.receviceATCMD(2000)
    return self.checkRecdata("OK\r\n")

  def powerOn(self):
    if self.checkBC20():
      return True
    self.flushQueue()  
    self.sendCmd("powerOn")
    time.sleep(0.3)
    if self.checkBC20():
      return True
    return False
    
  def configSleepMode(self,mode):
    self.flushQueue()
    self.sendCmd("QSCLK",str(mode))
    self.receviceATCMD(1000)
    return self.checkRecdata("OK\r\n")
  
  def rest(self,mode=1):
    self.flushQueue()
    self.sendCmd("QRST",str(mode))
    self.receviceATCMD(300)
    time.sleep(1000)
    return True
    
  def checkNBCard(self):
    self.flushQueue()
    self.sendCmd("CIMI")  
    self.receviceATCMD(300)
    ret=self.checkRecdata("OK\r\n")
    if ret==False:
      self.rest()
    return ret 

  def removeSthString(self,sth,std):  
    if std == None:
      return  
    ret=std
    tempInt=ret.find(sth)
    if tempInt != -1:
      ret=ret[tempInt+len(sth):len(ret)]
    return ret
    
  def GetSthfrontString(self,sth,std):
    if std == None:
      return   
    ret=std
    temp=sth
    tempInt=ret.find(temp)  
    if tempInt != -1:
      ret=ret[0:tempInt]
    return  ret    
        
  def getGSN(self,cmd):
    self.flushQueue()
    self.sendCmd("CGSN",str(cmd))
    self.receviceATCMD(300)
    Data=self.getRecDataforNum(2)
    if Data!=None:
      tempStr=Data.data
      tempStr  = self.removeSthString("+CGSN: ",tempStr)
      tempStr = self.GetSthfrontString("\r\n",tempStr)
      self.imei=tempStr

  def getQCCID(self):
    self.flushQueue()
    self.sendCmd("QCCID")
    self.receviceATCMD(300)
    Data=self.getRecDataforNum(2)
    if Data!=None:
      tempStr=Data.data    
      CCID=self.removeSthString("+QCCID: ",tempStr)
      return CCID

  def getIMI(self):
    self.flushQueue()
    self.sendCmd("CIMI")    
    self.receviceATCMD(300)
    self.getRecData(0)
    return self.IMEI_NUM
    
  def getGATT(self):
    return (self.getIntforString("CGATT?","CGATT",1))
    
  def getCLK(self):
    self.flushQueue()
    self.sendCmd("CCLK?")   
    self.receviceATCMD(300)
    Data=self.getRecDataforNum(2)
    if Data != None:
      tempStr = Data.data
      if tempStr.find("+CCLK: ")==-1:
        return
      tempStr = self.removeSthString("+CCLK: ",tempStr)
      self.Year =int(self.GetSthfrontString("/",tempStr))
      tempStr  = self.removeSthString("/",tempStr)
      self.Month =int(self.GetSthfrontString("/",tempStr))
      tempStr  = self.removeSthString("/",tempStr)
      self.Day =int(self.GetSthfrontString(",",tempStr))
      tempStr  = self.removeSthString(",",tempStr)
      self.Hour =int(self.GetSthfrontString(":",tempStr))
      tempStr  = self.removeSthString(":",tempStr)
      self.Minute =int(self.GetSthfrontString(":",tempStr))
      tempStr  = self.removeSthString(":",tempStr)
      self.Second =int(self.GetSthfrontString("GMT",tempStr))
      tempStr  = self.removeSthString("GMT+",tempStr)
      tempStr = self.GetSthfrontString("\r",tempStr)
      self.TimeEquation=int(tempStr)
      self.Hour=self.Hour+self.TimeEquation
      tempStr = self.GetSthfrontString("\r\n",tempStr)
    
  def getSQ(self):
    self.flushQueue()
    self.sendCmd("CSQ")    
    self.receviceATCMD(300)
    Data=self.getRecDataforNum(2)
    if Data!=None:
      tempStr  =Data.data
      if tempStr.find("+CSQ: ")==-1:
        return
      tempStr  =self.removeSthString("+CSQ: ",tempStr)
      self.rssi = int(self.GetSthfrontString(",",tempStr))
      tempStr  = self.removeSthString(",",tempStr)
      self.ber  = int(self.GetSthfrontString("\r\n",tempStr))

  def openMQTT(self,connectID,Server,Port):
    temptime=0
    tempStr ="QMTOPEN"+"="+connectID+",\""+Server+"\","+Port
    self.flushQueue()
    self.sendCmd(tempStr)   
    self.receviceATCMD(1000)
    if self.checkRecdata("OK\r\n"):
      while True:
        self.flushQueue()
        if temptime > 250:
          return False
        self.receviceATCMD(300)
        temptime+=1
        Data=self.getRecDataforNum_NoCheck(2)
        if Data!=None:
          tempStr=Data.data
          if tempStr=="+QMTOPEN: 0,0\r\n":
            return True
    return False       

  def closeMQTT(self,ConnectID):
     self.flushQueue()
     self.sendCmd("QMTCLOSE",str(ConnectID))
     self.receviceATCMD(500)
     

  def setServer(self,IPAddress,port,connectID="0"):
    while self.openMQTT(connectID,IPAddress,port)==False :
      self.closeMQTT(0)
      time.sleep(1)
 
  def publish(self,topic,msg):  
    self.pubMessage('0','1','1','0',topic,msg)      
 
  def pubMessage(self,connectID,msgID,qos,retain,topic,msg):
    self.flushQueue()
    tempStr="QMTPUB"+"="+connectID+","+msgID+","+qos+","+retain+","+"\""+topic+"\","+"\""+msg+"\""
    self.sendCmd(tempStr)
    self.receviceATCMD(1000)

  def connected(self):
    return self.mqttConneced

  def connectServer(self,clientID,username,password,connectID="0"):
    temptime=0
    tempStr="QMTCONN"+"="+connectID+",\""+clientID+"\""+",\""+username+"\""+",\""+password+"\""
    self.flushQueue()
    self.sendCmd(tempStr)
    self.receviceATCMD(1000)
    if self.checkRecdata("OK\r\n"):
      while True:
        self.flushQueue()
        if temptime > 250:
          return False
        self.receviceATCMD(300)
        temptime+=1
        Data=self.getRecDataforNum_NoCheck(2)
        if Data!=None:
          tempStr=Data.data
          if tempStr=="+QMTCONN: 0,0,0\r\n":
            self.mqttConneced=True
            return True
    return False 

  def getQMTCONN(self):
    self.flushQueue()
    self.sendCmd("QMTCONN?")
    self.receviceATCMD(5000)
    Data=self.getRecDataforNum(2)
    if Data != None:
      tempStr=Data.data
      if tempStr=="+QMTCONN: 0,3\r\n":
        return True
      else:
        return False
    return False

  def subTopic(self,connectID,msgID,topic,qos):
    tempStr="QMTSUB"+"="+connectID+","+msgID+",\""+topic+"\","+qos
    self.flushQueue()
    self.sendCmd(tempStr)    
    self.receviceATCMD(3000)
    return self.checkRecdata("OK\r\n")

  def setCallback(self,call):
    self.callback=call
    

  def loop(self):
    if self.available():
      tempStr=self.readData(1000)
      if tempStr.find("+QMTRECV: ") !=-1 :
        tempStr = self.removeSthString("+QMTRECV: ",tempStr) 
        tempStr = self.removeSthString(",",tempStr)    
        tempStr = self.removeSthString(",\"",tempStr)
        self.topic=self.GetSthfrontString("\",",tempStr)
        tempStr = self.removeSthString("\",\"",tempStr)
        self.payload=self.GetSthfrontString("\"\r\n",tempStr)
        if self.callback != None:
          self.callback()
      sleep_time = 0.1
      time.sleep(sleep_time)
    else:
      sleep_time=10
      time.sleep(sleep_time)  

  def setAliyunserver(self,ProductKey,IPAddress,port,connectID="0"):
    IPaddress=ProductKey+IPAddress
    while self.openMQTT(connectID,IPaddress,port) == False:
      self.closeMQTT(0)
      time.sleep(1)
      
  def connect_Aliyun(self,ProductKey,DeviceName,DeviceSecret):  
    temptime=0    
    tempStr="QMTCFG"+"=\"ALIAUTH\",0,\""+ProductKey+"\",\""+DeviceName+"\",\""+DeviceSecret+"\""
    self.flushQueue()
    self.sendCmd(tempStr)
    self.receviceATCMD(300)
    if self.checkRecdata("OK\r\n") == False:
      return False
    tempStr="QMTCONN=0,\""+DeviceName+"\""
    self.flushQueue()
    self.sendCmd(tempStr)
    self.receviceATCMD(300)
    if self.checkRecdata("OK\r\n"):
      while True:
        self.flushQueue()
        if temptime > 250:
          return False
        self.receviceATCMD(300)
        temptime+=1
        Data=self.getRecDataforNum_NoCheck(2)
        if Data!=None:
          tempStr=Data.data
          if tempStr.find("ERROR") != -1:
            return False 
          if tempStr=="+QMTCONN: 0,0,0\r\n":
            self.mqttConneced=True
            return True
    return False  
    
  def setQGNSSC(self,mode):
    self.flushQueue()
    self.sendCmd("QGNSSC",str(mode))
    self.receviceATCMD(300)
    return self.checkRecdata("OK\r\n") 
    
  def getQGNSSC(self):
    return self.getIntforString("QGNSSC?","QGNSSC",1)    

  def setPSMMode(self,status):
    self.flushQueue()
    tempStr="CPSMS"+"="+str(status)+",,,\""+self._TAUValue+"\",\""+self._AcTValue+"\""
    self.receviceATCMD(300)
    return self.checkRecdata("OK\r\n") 

  def configSleepMode(self,mode):
    self.setQSCLK(mode)

  def setQSCLK(self,mode):
    self.flushQueue()
    self.sendCmd("QSCLK",str(mode))
    self.receviceATCMD(300)
    return self.checkRecdata("OK\r\n")

  def stmLowpower(self):
    self.changeColor('B')
    self.ledON()
    time.sleep(0.01)
    self.ledOFF()
    time.sleep(0.01)     
    self.sendCmd("DSLEEP")
    time.sleep(0.01)
    return self.checkBC20()

  def stmWakeup(self,Awake_Pin):
    try:  
      GPIO.setmode(GPIO.BOARD)
      GPIO.setup(Awake_Pin, GPIO.OUT)    
      GPIO.output(Awake_Pin, GPIO.LOW)
    except:
      print("")
    time.sleep(0.1)
    GPIO.output(Awake_Pin, GPIO.HIGH)
    time.sleep(0.1)
    GPIO.output(Awake_Pin, GPIO.LOW)
    return self.checkStmStauts()
    
  def BC20Wakeup(self):
    self.flushQueue()
    self.sendCmd("WakeUp")
    time.sleep(0.3)
    return self.checkBC20()    
    

  def checkStmStauts(self):
    self.flushQueue()
    self.sendCmd("STMSTATUS")
    self.receviceATCMD(300)
    return self.checkRecdata("OK\r\n")

  def GetSatelliteInformation(self,start,num,_str,sys):
    tempStr=_str
    for i in range(num):
      if len(self.GetSthfrontString(",",tempStr))==0:
        self.sSAT.data[i+start].PRN = "N/A"
      else:
        self.sSAT.data[i+start].PRN = self.GetSthfrontString(",",tempStr)  #ÎÀÐÇPRNÂë±àºÅi+start 
      tempStr =self.removeSthString(",",tempStr)  
      if len(self.GetSthfrontString(",",tempStr)) == 0:
        self.sSAT.data[i+start].Elev = "N/A"
      else:
        self.sSAT.data[i+start].Elev = self.GetSthfrontString(",",tempStr) #ÎÀÐÇÑö½Çi+start
      tempStr = self.removeSthString(",",tempStr) 
      if len(self.GetSthfrontString(",",tempStr)) == 0: 
        self.sSAT.data[i+start].Azim = "N/A"
      else:
        self.sSAT.data[i+start].Azim = self.GetSthfrontString(",",tempStr) #ÎÀÐÇ·½Î»½Çi+start   
      tempStr = self.removeSthString(",",tempStr)
      if len(self.GetSthfrontString(",",tempStr)) == 0:
        self.sSAT.data[i+start].SNR = "N/A"
      else:  
        self.sSAT.data[i+start].SNR = self.GetSthfrontString(",",tempStr) #ÐÅÔë±Èi+start
      self.sSAT.data[i+start].SYS = sys
      tempStr = self.removeSthString(",",tempStr)
      self.CheckSatelliteUse(i+start)
        
  def CheckSatelliteUse(self,num):
    if self.sSAT.data[num].SYS == "GPS":
      temp = 0
    elif self.sSAT.data[num].SYS == "BeiDou":
      temp = 1   
    for i in range(12):
      if self.sSAT.data[num].PRN == self.sGSA.data[temp].Statellite_CH[i]:
        self.sSAT.data[num].Status = "Y" 
        return     
    self.sSAT.data[num].Status = "N"

  def CheckUse(self):
    self.sSAT.USE = 0
    for i in range(self.sGSA.NUM):
      for j in range(12):
        if len(self.sGSA.data[i].Statellite_CH[j]) > 0 and (self.sGSA.data[i].Statellite_CH[j] !="N/A"):
          self.sSAT.USE +=1
        
  def getQGNSSRD(self,cmd):
    if len(cmd)>0:
      readnum = 2
      self.flushQueue()
      self.sendCmd("QGNSSRD",cmd)
      self.receviceATCMD(5000)
      if cmd == "\"NMEA/GSV\"":
        StartNum = 0x00
        lastTotSEN = 0x00
        self.sSAT.NUM = 0x00
        for i in range(15):
          Data=self.getRecDataforNum_NoCheck(readnum+i)
          if Data != None:
            tempStr=Data.data
            if (tempStr.find("ERROR") != -1) or len(tempStr)<8:
              break 
            if tempStr.find("+QGNSSRD:") != -1:
              tempStr  = self.removeSthString("+QGNSSRD: ",tempStr)
            if tempStr.find("$GPGSV,") != -1:
              tempSYS = "GPS"
            elif tempStr.find("$GBGSV,") != -1:
              tempSYS = "BeiDou"
            else:
              tempSYS = "N/A"    
            tempStr = self.removeSthString(",",tempStr)
            TotSEN=int(self.GetSthfrontString(",",tempStr)) # Óï¾äµÄ×ÜÊýÄ¿
            tempStr = self.removeSthString(",",tempStr)
            NumSEN=int(self.GetSthfrontString(",",tempStr)) # µ±Ç°Óï¾äÐòºÅ
            tempStr = self.removeSthString(",",tempStr)
            NumSAT=int(self.GetSthfrontString(",",tempStr)) # ¿É¼ûÎÀÐÇÊý
            tempStr = self.removeSthString(",",tempStr)
            if TotSEN > 0:
              if NumSEN == 1:
                self.sSAT.NUM += NumSAT
                if self.sSAT.NUM>20:
                  self.sSAT.NUM=19
              if NumSAT /(NumSEN*4) > 0:
                self.GetSatelliteInformation(StartNum,4,tempStr,tempSYS) 
                StartNum += 4 
              else :
                self.GetSatelliteInformation(StartNum,(NumSAT-((NumSEN-1)*4)),tempStr,tempSYS)
                StartNum += (NumSAT-((NumSEN-1)*4))
      else:
        Data=self.getRecDataforNum(2)
        if Data!=None :
          tempStr  = Data.data
          if len(tempStr) < 10:
            return
        else:
          return 
        tempStr = self.removeSthString("+QGNSSRD: ",tempStr)
        if len(tempStr)<10:
          return
        if cmd== "\"NMEA/RMC\"":
          tempStr = self.removeSthString("$GNRMC,",tempStr)
          self.sRMC.UTC_Time = self.GetSthfrontString(",",tempStr)
          tempStr = self.removeSthString(",",tempStr)
          self.sRMC.DataStatus = self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sRMC.LatitudeVal = self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sRMC.LatitudeDir= self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sRMC.LongitudeVal=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sRMC.LongitudeDir=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sRMC.GroundSpeed=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sRMC.GroundHeading=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sRMC.UTC_Date=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sRMC.MagDeclination=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sRMC.MagDeclinationDir=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)        
          self.sRMC.PositioningMode=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr) 
          self.sRMC.NaviStatus=self.GetSthfrontString("*",tempStr) 
          if self.sRMC.UTC_Time != None:
            timeStr = self.sRMC.UTC_Time
            self.sCLK.Hour = int(timeStr[0:2])      
            timeStr=timeStr[2:len(timeStr)]
            self.sCLK.Minute = int(timeStr[0:2])
            timeStr = timeStr[2:len(timeStr)]
            self.sCLK.Second = int(timeStr[0:2])
          if self.sRMC.UTC_Date != None:
            timeStr = self.sRMC.UTC_Date
            self.sCLK.Day = int(timeStr[0:2])
            timeStr = timeStr[2:len(timeStr)]
            self.sCLK.Month =int(timeStr[0:2])
            timeStr = timeStr[2:len(timeStr)]
            self.sCLK.Year = int(timeStr[0:2]) + 2000
            self.sCLK.Mode = "UTC+0"
        elif cmd == "\"NMEA/VTG\"":
          tempStr = self.removeSthString("$GNVTG,",tempStr) 
          self.sVTG.GroundCourse_True = self.GetSthfrontString(",",tempStr)
          tempStr = self.removeSthString("T,",tempStr)  
          self.sVTG.GroundCourse_Mag =self.GetSthfrontString(",",tempStr)
          tempStr = self.removeSthString("M,",tempStr)
          self.sVTG.GroundCourse_Knots = self.GetSthfrontString(",",tempStr)
          tempStr = self.removeSthString("N,",tempStr)
          self.sVTG.GroundCourse_Kmh=self.GetSthfrontString(",",tempStr)
          tempStr = self.removeSthString("K,",tempStr)
          self.sVTG.PositioningMode = self.GetSthfrontString("*",tempStr)
        elif cmd == "\"NMEA/GGA\"":
          tempStr = self.removeSthString("$GNGGA,",tempStr)
          self.sGGA.UTC_Time=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sGGA.LatitudeVal=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sGGA.LatitudeDir=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sGGA.LongitudeVal=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sGGA.LongitudeDir=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sGGA.Status=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sGGA.StatelliteNum=self.GetSthfrontString(",",tempStr)  
          tempStr =self.removeSthString(",",tempStr)
          self.sGGA.HDOP=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sGGA.Altitude=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString("M,",tempStr)
          self.sGGA.GeoID_Separation=self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString("M,",tempStr)
          self.sGGA.DGPS_Age = self.GetSthfrontString(",",tempStr)
          tempStr =self.removeSthString(",",tempStr)
          self.sGGA.DGPS_StationID = self.GetSthfrontString("*",tempStr)                                                              
        elif cmd == "\"NMEA/GSA\"":
          self.sGSA.NUM = 0
          for i in range(4):
            if tempStr.find("$GNGSA,") != -1 :
              self.sGSA.NUM = i+1
              tempStr = self.removeSthString("$GNGSA,",tempStr)
              self.sGSA.data[i].Mode = self.GetSthfrontString(",",tempStr)
              tempStr =self.removeSthString(",",tempStr)
              self.sGSA.data[i].FixStatus = self.GetSthfrontString(",",tempStr)
              tempStr =self.removeSthString(",",tempStr)
              for j in range(12):
                self.sGSA.data[i].Statellite_CH[j] = self.GetSthfrontString(",",tempStr)
                tempStr=self.removeSthString(",",tempStr)
              self.sGSA.data[i].PDOP = self.GetSthfrontString(",",tempStr)
              tempStr=self.removeSthString(",",tempStr)
              self.sGSA.data[i].HDOP = self.GetSthfrontString(",",tempStr)
              tempStr=self.removeSthString(",",tempStr)
              self.sGSA.data[i].VDOP = self.GetSthfrontString(",",tempStr)
              tempStr=self.removeSthString(",",tempStr)
              self.sGSA.data[i].GNSS_SystemID = self.GetSthfrontString("*",tempStr)
              for j in range(12):
                if len(self.sGSA.data[i].Statellite_CH[j]) == 0:
                  self.sGSA.data[i].Statellite_CH[j] = "N/A"
              Data=self.getRecDataforNum(3+i) 
              if Data != None : 
                tempStr  =Data.data
              else :
                return             
          self.CheckUse()
        elif cmd == "\"NMEA/GLL\"":
          tempStr=self.removeSthString("$GNGLL,",tempStr)
          self.sGLL.LatitudeVal=self.GetSthfrontString(",",tempStr)
          tempStr=self.removeSthString(",",tempStr)
          self.sGLL.LatitudeDir=self.GetSthfrontString(",",tempStr)
          tempStr=self.removeSthString(",",tempStr)
          self.sGLL.LongitudeVal=self.GetSthfrontString(",",tempStr)
          tempStr=self.removeSthString(",",tempStr)
          self.sGLL.LongitudeDir=self.GetSthfrontString(",",tempStr)
          tempStr=self.removeSthString(",",tempStr)
          self.sGLL.UTC_Time=self.GetSthfrontString(",",tempStr)
          tempStr=self.removeSthString(",",tempStr)
          self.sGLL.DataStatus=self.GetSthfrontString(",",tempStr)
          tempStr=self.removeSthString(",",tempStr)
          self.sGLL.PositionMode=self.GetSthfrontString("*",tempStr)                                        
        else:
          print("error")
    else :
      self.getQGNSSRD("\"NMEA/RMC\"")
      self.sGGNS.LatitudeVal = self.Longitude_conversion(self.sRMC.LatitudeVal)
      self.sGGNS.LatitudeDir = self.sRMC.LatitudeDir
      self.sGGNS.LongitudeVal =self.Longitude_conversion(self.sRMC.LongitudeVal)
      self.sGGNS.LongitudeDir=self.sRMC.LongitudeDir
    
      self.getQGNSSRD("\"NMEA/VTG\"")
      if self.sVTG.GroundCourse_Kmh !=None:
        self.sGGNS.Speed = float(self.sVTG.GroundCourse_Kmh)
      '''if self.sVTG.GroundCourse_True != None :
        self.sGGNS.Heading =float(self.sVTG.GroundCourse_True)'''
  
      self.getQGNSSRD("\"NMEA/GGA\"")
      if self.sGGA.Altitude !=None:
        self.sGGNS.Altitude = float(self.sGGA.Altitude)
  
      self.getQGNSSRD("\"NMEA/GSA\"")
      self.sGGNS.PDOP = self.sGSA.data[0].PDOP 
      self.sGGNS.HDOP = self.sGSA.data[0].HDOP 
      self.sGGNS.VDOP = self.sGSA.data[0].VDOP 
      if self.sGSA.data[0].FixStatus == "2":
        self.sGGNS.FixStatus = "2D fixed"
      elif self.sGSA.data[0].FixStatus == "3" :
        self.sGGNS.FixStatus = "3D fixed" 
      else:
        self.sGGNS.FixStatus = "No fix" 
      self.sSAT.USE=0   
      self.getQGNSSRD("\"NMEA/GSV\"")
      for i in range(self.sSAT.NUM):
        if self.sSAT.data[i].Status=="Y":
          self.sSAT.USE+=1;
    
  def Longitude_conversion(self,strdata):
    tempStr = strdata
    temp=0
    if tempStr !=None :
      if tempStr==self.sRMC.LatitudeVal:
        temp = int(tempStr[0:2])  
        tempStr = tempStr[2:len(tempStr)]
        temp += int(tempStr[0:2])*10.0/60.0/10.0
        tempStr = tempStr[3:len(tempStr)]
        temp += int(tempStr[0:4])*1.0/60.0/10000.0
      elif tempStr== self.sRMC.LongitudeVal:
        temp = int(tempStr[0:3])
        tempStr = tempStr[3:len(tempStr)]
        temp += int(tempStr[0:2])*10.0/60.0/10.0
        tempStr = tempStr[3:len(tempStr)]
        temp += int(tempStr[0:4])*1.0/60.0/10000.0
      else:
        print("ERROR")
    return float(temp)  

class DFRobot_BC20_Serial(DFRobot_BC20):
  def __init__(self):
    if ser.isOpen() is False:
      ser.Open()
    super(DFRobot_BC20_Serial, self).__init__()  
        
  def available(self):
    if ser.inWaiting() !=0:
      return True
    return False
  
  def sendCmd(self,CMD,arg=""):
    if arg=="":
      if(len(CMD)>0) is True:
        ser.write("AT+"+CMD+"\r\n")
      else:
        ser.write("AT\r\n")
    else:
        ser.write("AT+"+CMD+"="+arg+"\r\n")
    time.sleep(0.5)    
  
  def readData(self,timeout):
    recv=""
    ltime=int(round(time.time() * 1000))
    while(int(round(time.time() * 1000))-ltime)<=timeout:
      count = ser.inWaiting()
      if count != 0:
        recv += ser.read(count)
        if recv.find("OK\r\n")!= -1:
          break   
    return  recv        
     
  def receviceATCMD(self,timeout):
    ID=0
    recv=""
    ltime=int(round(time.time() * 1000))
    while((int(round(time.time() * 1000))-ltime)<=timeout):
      count = ser.inWaiting()
      if count != 0:
        recv += ser.read(count)
        if recv.find("OK\r\n")!= -1:
          break
    if len(recv)>0:     
      tempInt=recv.find("\r\n")
      while (tempInt >= 0):
        if tempInt != -1:
          self.enqueue(ID,recv[0:tempInt+2])
          recv=recv[tempInt+2:len(recv)]
          ID +=1
        tempInt=recv.find("\r\n")  
      
class DFRobot_BC20_IIC(DFRobot_BC20): 
  def __init__(self,addr):
    self._addr = addr
    self.i2c = smbus.SMBus(1)
    super(DFRobot_BC20_IIC, self).__init__()  

  def BC20_get_regs(self,reg,lenth):
    try:  
      rslt=[]
      for i in range(lenth):
        rslt.append(0)
      '''if lenth<30:
        rslt = self.i2c.read_i2c_block_data(self._addr,reg,lenth)    
      else:'''
      for i in range(lenth):  
        rslt[i] = self.i2c.read_byte_data(self._addr,reg+i)   
      return rslt
    except:
      return [0]         
    
  def BC20_set_regs(self,reg,data):
    try:
      if data != None:
        data=[ord(c) for c in data]
        if len(data)>31:
          for i in range(len(data)):  
            self.i2c.write_byte_data(self._addr,reg,data[i])
        else :
          self.i2c.write_i2c_block_data(self._addr,reg,data)     
    except:
      return []  
      
  def available(self):
    recv=self.BC20_get_regs(0x00,1)
    if recv[0]>0:
      return True
    return False

  def sendCmd(self,CMD,arg=""): 
    if arg=="":
      if(len(CMD)>0) is True:
        temp="AT+"+CMD+"\r\n"
        self.BC20_set_regs(0x00,temp)
      else:
        temp="AT\r\n"
        self.BC20_set_regs(0x00,temp)
    else:
      temp="AT+"+CMD+"="+arg+"\r\n"
      self.BC20_set_regs(0x00,temp)  

  def readData(self,timeout):
    recv=""
    ltime=int(round(time.time() * 1000))
    while((int(round(time.time() * 1000))-ltime)<=timeout):
      count = self.BC20_get_regs(0x00,1)
      if count[0] != 0:
        Recv = self.BC20_get_regs(0x01,count[0])
        recv += str(bytearray(Recv))           
        if recv.find("OK\r\n")!= -1:
          break
    return  recv 

  def receviceATCMD(self,timeout):
    ID=0
    recv=""
    ltime=int(round(time.time() * 1000))
    while((int(round(time.time() * 1000))-ltime)<=timeout):
      count = self.BC20_get_regs(0x00,1)
      if count[0] != 0: 
        Recv = self.BC20_get_regs(0x01,count[0])
        recv += str(bytearray(Recv))         
        if recv.find("OK\r\n")!= -1:
          break
    if len(recv)>0:
      if recv.find("+IP:") != -1:
        return  
      tempInt=recv.find("\r\n")
      while (tempInt >= 0):
        if tempInt != -1:
          self.enqueue(ID,recv[0:tempInt+2])
          recv=recv[tempInt+2:len(recv)]
          ID +=1
        tempInt=recv.find("\r\n")      
       
             