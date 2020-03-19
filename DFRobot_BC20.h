#ifndef __DFROBOT_BC20__
#define __DFROBOT_BC20__
#define OPENDBG    1
#include "Arduino.h"
#include "DFRobot_BC20AT.h"
#include "DFRobot_queue.h"
#ifndef ARDUINO_ESP32_DEV
#include "SoftwareSerial.h"
#endif
#include "Wire.h"
#include "EEPROM.h"
#define DBG(sth)   Serial.print("DBG:");Serial.print(__LINE__);Serial.print("--------");Serial.println(sth)
#define ON   1
#define OFF  0
#define SN      0
#define IMEI    1
#define IMEISV  2
#define SVN     3
extern char *ret;
extern uint8_t Flag;
typedef struct
{
    uint8_t *IMEI_NUM;
}sSIMCard_t;
extern sSIMCard_t sSIMCard;

typedef struct
{
    uint8_t dataNum;
}sRecData_t;
extern sRecData_t sRecData;

typedef struct
{
    uint8_t *company;
    uint8_t *deviceType;
    uint8_t *Revision;
}sPII_t;
extern sPII_t sPII;

typedef struct
{
    uint8_t *company;
    uint8_t *deviceType;
    uint8_t *Revision;
}sMI_t;
extern sMI_t sMI;

typedef struct
{
    uint8_t rssi;
    uint8_t ber;
}sSQ_t;
extern sSQ_t sSQ;

typedef struct
{
    uint8_t rxlev;/*Received signal strength level*/
    uint8_t ber;/*Channel bit error rate (in percent)*/
    uint8_t rscp;/*received signal code power */
    uint8_t ecno;/* Ec/No*/
    uint8_t rsrq;/*Reference signal received quality*/
    uint8_t rsrp;/*Reference signal received power*/
}sESQ_t;
extern sESQ_t sESQ;

typedef struct
{
    String sn;
    String imei;
    char* imeisv;
    char* svn;
}sGSN_t;
extern sGSN_t sGSN;

typedef struct
{
    //char* LatitudeVal;
	char *LatitudeVal(){
	int addr=0;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0);
	}
	char*a=ret; 	
	return a;
	};	
    //char* LatitudeDir;
	char *LatitudeDir(){
	int addr=0+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11);
	}
	char*a=ret; 	
	return a;
	};		
    //char* LongitudeVal;
	char *LongitudeVal(){
	int addr=0+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11);
	}
	char*a=ret; 	
	return a;
	};		
    //char* LongitudeDir;
	char *LongitudeDir(){
	int addr=0+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11);
	}
	char*a=ret; 	
	return a;
	};		
    //char* UTC_Time;
	char *UTC_Time(){
	int addr=0+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};		
    //char* DataStatus;
	char *DataStatus(){
	int addr=0+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};		
    //char* PositionMode;
	char *PositionMode(){
	int addr=0+11+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};		
}sGLL_t;
extern sGLL_t sGLL;

typedef struct
{
    //char* UTC_Time;
	char *UTC_Time(){
	int addr=0;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0);
	}
	char*a=ret; 	
	return a;
	};		
    //char* LatitudeVal;
	char *LatitudeVal(){
	int addr=0+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11);
	}
	char*a=ret; 	
	return a;
	};		
    //char* LatitudeDir;
	char *LatitudeDir(){
	int addr=0+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11);
	}
	char*a=ret; 	
	return a;
	};		
    //char* LongitudeVal;
	char *LongitudeVal(){
	int addr=0+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11);
	}
	char*a=ret; 	
	return a;
	};		
    //char* LongitudeDir;
	char *LongitudeDir(){
	int addr=0+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};		
    //char* Status;
	char *Status(){
	int addr=0+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};		
    //char* StatelliteNum;//Number of satellites
	char *StatelliteNum(){
	int addr=0+11+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};		
    //char* HDOP;//Horizontal precision factor
	char *HDOP(){
	int addr=0+11+11+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};		
    //char* Altitude;//altitude 
	char *Altitude(){
	int addr=0+11+11+11+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};		
    //char* GeoID_Separation;//Earth horizontal distance
	char *GeoID_Separation(){
	int addr=0+11+11+11+11+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};		
    //char* DGPS_Age;//Differential revision time
	char *DGPS_Age(){
	int addr=0+11+11+11+11+11+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};		
    //char* DGPS_StationID;//Differential reference base station ID number
	char *DGPS_StationID(){
	int addr=0+11+11+11+11+11+11+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11+11+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};		
}sGGA_t;
extern sGGA_t sGGA;
struct sGSAD
{
    //String Mode;	
    uint8_t NUM;
    char *Mode(){
        int addr=NUM*72;     
        if(ret != NULL){
            free(ret);
            ret = NULL;
        }
        ret = (char *)malloc(5);
        memset(ret,'\0',5);
        for(int i = 0; i < 4; i++){
			ret[i] =(char)EEPROM.read(addr+i);
        }
        char*a=ret; 		
        return a;
    };
    //String FixStatus;
    char *FixStatus(){
        int addr=NUM*72+4;     
        if(ret != NULL){
            free(ret);
            ret = NULL;
        }
        ret = (char *)malloc(5);
        memset(ret,'\0',5);
        for(int i = 0; i < 4; i++){
			ret[i] =(char)EEPROM.read(addr+i);
        }
        char*a=ret;
		if(strcmp(a,"1")==0){
			return "No fix";
		}else if(strcmp(a,"2")==0){
			return "2D fix";
		}else if(strcmp(a,"3")==0){
			return "3D fix";
		}
        
    };	
    //String Statellite_CH[12];
    char *Statellite_CH(int j){
        int addr=NUM*72+4+4+j*4;     
        if(ret != NULL){
            free(ret);
            ret = NULL;
        }
        ret = (char *)malloc(5);
        memset(ret,'\0',5);
        for(int i = 0; i < 4; i++){
			ret[i] =(char)EEPROM.read(addr+i);
        }
        char*a=ret; 		
        return a;
    };	
	/*
    String Statellite_CH1;
    String Statellite_CH2;
    String Statellite_CH3;
    String Statellite_CH4;
    String Statellite_CH5;
    String Statellite_CH6;
    String Statellite_CH7;
    String Statellite_CH8;
    String Statellite_CH9;
    String Statellite_CH10;
    String Statellite_CH11;
    String Statellite_CH12;*/
    //String PDOP;//Position precision factor
    char *PDOP(){
        int addr=NUM*72+4+4+4*12;     
        if(ret != NULL){
            free(ret);
            ret = NULL;
        }
        ret = (char *)malloc(5);
        memset(ret,'\0',5);
        for(int i = 0; i < 4; i++){
			ret[i] =(char)EEPROM.read(addr+i);
        }
        char*a=ret; 		
        return a;
    };	
    //String HDOP;//Horizontal precision factor
	char *HDOP(){
	int addr=NUM*72+4+4+12*4+4;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(5);
	memset(ret,'\0',5);
	for(int i = 0; i < 4; i++){
		ret[i] =(char)EEPROM.read(addr+i);
	}
	char*a=ret; 		
	return a;
	};
    //String VDOP;//Vertical precision factor
	char *VDOP(){
	int addr=NUM*72+4+4+12*4+4+4;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(5);
	memset(ret,'\0',5);
	for(int i = 0; i < 4; i++){
		ret[i] =(char)EEPROM.read(addr+i);
	}
	char*a=ret; 		
	return a;
	};	
    //String GNSS_SystemID;//1-GPS,2-GLONASS,3-Galileo,4-BeiDou
	char *GNSS_SystemID(){
	int addr=NUM*72+4+4+12*4+4+4+4;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(5);
	memset(ret,'\0',5);
	for(int i = 0; i < 1; i++){
		ret[i] =(char)EEPROM.read(addr+i);
	}
	char*a=ret; 
    if(strcmp(a,"1"))
	{
		return("GPS");
	}else if(strcmp(a,"4"))
	{
		return("BeiDou");
	}
	return a;
	};	
};

typedef struct
{
    uint8_t NUM;
    sGSAD data[2];
}sGSA_t;
extern sGSA_t sGSA;

typedef struct
{
    //char* UTC_Time;
	char *UTC_Time(){
	int addr=0;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0);
	}
	char*a=ret; 	
	return a;
	};	
    //char* DataStatus;
	char *DataStatus(){
	int addr=0+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11);
	}
	char*a=ret; 	
	return a;
	};	
    //char* LatitudeVal;
	char *LatitudeVal(){
	int addr=0+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11);
	}
	char*a=ret; 	
	return a;
	};	
    //char* LatitudeDir;
	char *LatitudeDir(){
	int addr=0+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11);
	}
	char*a=ret; 	
	return a;
	};	
    //char* LongitudeVal;
	char *LongitudeVal(){
	int addr=0+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};	
    //char* LongitudeDir;
	char *LongitudeDir(){
	int addr=0+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};	
    //char* GroundSpeed;
	char *GroundSpeed(){
	int addr=0+11+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};	
    //char* GroundHeading;
	char *GroundHeading(){
	int addr=0+11+11+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};	
    //char* UTC_Date;
	char *UTC_Date(){
	int addr=0+11+11+11+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};	
    //char* MagDeclination;//magnetic declination
	char *MagDeclination(){
	int addr=0+11+11+11+11+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};	
    //char* MagDeclinationDir;//Direction of magnetic decrement
	char *MagDeclinationDir(){
	int addr=0+11+11+11+11+11+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};	
    //char* PositioningMode;//Location model
	char *PositioningMode(){
	int addr=0+11+11+11+11+11+11+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11+11+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};	
    //char* NaviStatus;//Navigational state
	char *NaviStatus(){
	int addr=0+11+11+11+11+11+11+11+11+11+11+11+11;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(12);
	memset(ret,'\0',12);
	for(int i = 0; i < 11; i++){
		ret[i] =(char)EEPROM.read(i+0+11+11+11+11+11+11+11+11+11+11+11+11);
	}
	char*a=ret; 	
	return a;
	};	
}sRMC_t;
extern sRMC_t sRMC;

typedef struct
{
    //char* GroundCourse_True;
	char *GroundCourse_True(){
	int addr=0;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(6);
	memset(ret,'\0',6);
	for(int i = 0; i < 5; i++){
		ret[i] =(char)EEPROM.read(i+0);
	}
	char*a=ret; 	
	return a;
	};	
    //char* GroundCourse_Mag;
	char *GroundCourse_Mag(){
	int addr=0+5;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(6);
	memset(ret,'\0',6);
	for(int i = 0; i < 5; i++){
		ret[i] =(char)EEPROM.read(i+5);
	}
	char*a=ret; 		
	return a;
	};	
    //char* GroundCourse_Knots;//Ground speed
	char *GroundCourse_Knots(){
	int addr=0+5+5;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(6);
	memset(ret,'\0',6);
	for(int i = 0; i < 5; i++){
		ret[i] =(char)EEPROM.read(i+5+5);
	}
	char*a=ret; 		
	return a;
	};	
    //char* GroundCourse_Kmh;//Speed over the ground
	char *GroundCourse_Kmh(){
	int addr=0+5+5+5;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(6);
	memset(ret,'\0',6);
	for(int i = 0; i < 5; i++){
		ret[i] =(char)EEPROM.read(i+5+5+5);
	}
	char*a=ret; 		
	return a;
	};	
    //char* PositioningMode;
	char *PositioningMode(){
	int addr=0+5+5+5+5;     
	if(ret != NULL){
		free(ret);
		ret = NULL;
	}
	ret = (char *)malloc(6);
	memset(ret,'\0',6);
	for(int i = 0; i < 5; i++){
		ret[i] =(char)EEPROM.read(i+5+5+5+5);
	}
	char*a=ret; 		
	return a;
	};	
}sVTG_t;
extern sVTG_t sVTG;

struct sGSV
{
    uint8_t NUM;
	//String PRN;
    char *PRN(){
        int addr=NUM*21;     
        if(ret != NULL){
          free(ret);
          ret = NULL;
        }
        ret = (char *)malloc(4);
        memset(ret,'\0',4);
        for(int i = 0; i < 3; i++){
        ret[i] =(char)EEPROM.read(addr+i);
        }
        char*a=ret;      
        return a;
      };
    //String Elev;
    char *Elev(){
        int addr=(this->NUM)*21+3;     
        if(ret != NULL){
          free(ret);
          ret = NULL;
        }
        ret = (char *)malloc(4);
        memset(ret,'\0',4);
        for(int i = 0; i < 3; i++){
        ret[i] =(char)EEPROM.read(addr+i);
        }
        char*a=ret;      
        return a;
      }; 	
    //String Azim;
    char *Azim(){
        int addr=(this->NUM)*21+3+3;     
        if(ret != NULL){
          free(ret);
          ret = NULL;
        }
        ret = (char *)malloc(7);
        memset(ret,'\0',4);
        for(int i = 0; i < 3; i++){
        ret[i] =(char)EEPROM.read(addr+i);
        }
        char*a=ret;      
        return a;
      }; 	
    //String SNR;
    char *SNR(){
        int addr=(this->NUM)*21+3+3+3;     
        if(ret != NULL){
          free(ret);
          ret = NULL;
        }
        ret = (char *)malloc(4);
        memset(ret,'\0',4);
        for(int i = 0; i < 3; i++){
        ret[i] =(char)EEPROM.read(addr+i);
        }
        char*a=ret;      
        return a;
      }; 	
    //String Status;
    char *Status(){
        int addr=(this->NUM)*21+3+3+3+3;     
        if(ret != NULL){
          free(ret);
          ret = NULL;
        }
        ret = (char *)malloc(4);
        memset(ret,'\0',4);
        for(int i = 0; i < 3; i++){
        ret[i] =(char)EEPROM.read(addr+i);
        }
        char*a=ret;      
        return a;
      };	  
    //String SYS;
    char *SYS(){
        int addr=(this->NUM)*21+3+3+3+3+3;     
        if(ret != NULL){
          free(ret);
          ret = NULL;
        }
        ret = (char *)malloc(7);
        memset(ret,'\0',7);
        for(int i = 0; i < 6; i++){
        ret[i] =(char)EEPROM.read(addr+i);
        }
        char*a=ret;      
        return a;
      };	
};

typedef struct
{
	uint8_t NUM;
    uint8_t USE;
    sGSV data[40];
}sSAT_t;
extern sSAT_t sSAT;

typedef struct
{
    uint16_t Year;
    uint8_t  Month;
    uint8_t  Day;
    int  Hour;
    uint8_t  Minute;
    uint8_t  Second;
	char*  Mode;
    int TimeEquation;
}sCLK_t;
extern sCLK_t sCLK;


typedef struct
{
    uint8_t NUM;
	float LatitudeVal;
	float LongitudeVal;
	float Altitude;
	uint8_t StatelliteNum;
	float HDOP;
	char*LatitudeDir(){
		return (sGGA.LatitudeDir());
	}    
	char*LongitudeDir(){
		return (sGGA.LongitudeDir());
	}
	char*Status(){
		if(strcmp(sGGA.Status(),"2")){
			return "DGPS";
		}else if(strcmp(sGGA.Status(),"1")){
			return "GNSS";
		}else{
			return "Not fixed";
		}
	}	
}sGGNS_t2;
extern sGGNS_t2 sGGNS2;

struct sGSV2
{
    char* PRN;
    char* Elev;
    char* Azim;
    char* SNR;
    char* Status;
    char* SYS;
};

typedef struct
{
    uint8_t NUM;
    uint8_t USE=0;
    sGSV2 data[40];
}sSAT_t2;
extern sSAT_t2 sSAT2;

typedef struct
{
    uint8_t NUM;	
    float LatitudeVal;
    String LatitudeDir;
    float LongitudeVal;
    String LongitudeDir;
    float Altitude;
    float Speed;
    float Heading;
    String FixStatus;
    String Statellite_CH[2][12];
    String PDOP;
    String HDOP;
    String VDOP;
    uint8_t StatelliteNum;
}sGGNS_t;
extern sGGNS_t sGGNS;

typedef struct
{
    uint8_t ActiveTime;
    String ActiveUint;
    uint8_t TAUTime;
    String TAUUint;
}sCEREG_t;
extern sCEREG_t sCEREG;

typedef enum {
    eTAUunit_10M,  //10 minutes
    eTAUunit_1H,   //1 hour
    eTAUunit_10H,  //10 hour
    eTAUunit_2S,   //2 seconds
    eTAUunit_30S,  //30 seconds
    eTAUunit_1M,   //1 minute
    eTAUunit_320H, //320 hours
    eTAUunit_stop  // stop
} eTAUunit_t;

typedef enum {
    eAcTunit_2S,         //2 seconds
    eAcTunit_1M,         //1 minute
    eAcTunit_decihours,   //decihours
    eAcTunit_stop        //stop
} eAcTunit_t;

typedef enum {
    eeDRXValue_20_48,    //20.48 seconds
    eeDRXValue_40_96,    //40.96 seconds
    eeDRXValue_81_92,    //81.92 seconds
    eeDRXValue_163_84,   //163.84 seconds
    eeDRXValue_327_68,   //327.68 seconds
    eeDRXValue_655_36,   //655.36 seconds
    eeDRXValue_1310_72,  //1310.72 seconds
    eeDRXValue_2621_44,  //2621.44 seconds
    eeDRXValue_5242_88,  //5242.88 seconds
    eeDRXValue_10485_76  //10485.76 seconds
} eeDRXValue_t;

typedef enum {
    eNWProvidedValue_20_48,    //20.48 seconds
    eNWProvidedValue_40_96,    //40.96 seconds
    eNWProvidedValue_81_92,    //81.92 seconds
    eNWProvidedValue_163_84,   //163.84 seconds
    eNWProvidedValue_327_68,   //327.68 seconds
    eNWProvidedValue_655_36,   //655.36 seconds
    eNWProvidedValue_1310_72,  //1310.72 seconds
    eNWProvidedValue_2621_44,  //2621.44 seconds
    eNWProvidedValue_5242_88,  //5242.88 seconds
    eNWProvidedValue_10485_76  //10485.76 seconds
} eNWProvidedValue_t;

typedef enum {
    ePTWValue_2_56,            //2.56  seconds
    ePTWValue_5_12,            //5.12  seconds
    ePTWValue_7_68,            //7.68  seconds
    ePTWValue_10_24,           //10.24 seconds
    ePTWValue_12_8,            //12.8  seconds
    ePTWValue_15_36,           //15.36 seconds
    ePTWValue_17_92,           //17.92 seconds
    ePTWValue_20_48,           //20.48 seconds
    ePTWValue_23_04,           //23.04 seconds
    ePTWValue_25_6,            //25.6  seconds
    ePTWValue_28_16,           //28.16 seconds
    ePTWValue_30_72,           //30.72 seconds
    ePTWValue_33_28,           //33.28 seconds
    ePTWValue_35_84,           //35.84 seconds
    ePTWValue_38_4,            //38.4  seconds
    ePTWValue_40_96            //40.96 seconds
} ePTWValue_t;

typedef enum {
    ePSM_OFF,
    ePSM_ON,
    ePSM_OFF_ResetParam
}ePSM_t;

typedef enum {
    eSleepMode_Disable,
    eSleepMode_DeepSleep,
    eSleepMode_Light
}eSleepMode_t;

class DFRobot_BC20{
  public:
    DFRobot_BC20(void);
    ~DFRobot_BC20(void);
    char* Echo_OFF = SET_ECHO_0;
    char* Echo_ON = SET_ECHO_1;
    bool mqttConneced = false;
    bool powerOn(void);
    bool checkBC20(void);
    bool checkStmStauts(void);
    void clearGPS();
    bool checkNBCard(void);
    void getGPS(void);
    void getPII(void);
    void getMI(void);
    void getMM(void);
    void getMR(void);
    void getSQ(void);
    void getESQ(void);
    void getGSN(uint8_t cmd=0);
    void getEREG(void);
    bool setEREG(uint8_t cmd=4);
    void getSCON(void);
    bool setSCON(uint8_t cmd);
    void getOPS(void);
    bool setOPS(uint8_t cmd);
    uint8_t getGATT(void);
    bool setGATT(uint8_t state);
    void getGDCONT(void);
    bool setGDCONT(uint8_t state);
    void getGACT(void);
    bool setGACT(uint8_t state);
    void getGPADDR(void);
    bool setGPADDR(uint8_t state);
    uint8_t *getIMI(void);
    uint8_t getFUN(void);
    bool setFUN(uint8_t fun);
    uint8_t getMEE(void);
    bool setMEE(uint8_t fun);
    String getCLK(void);
    void getBC(void);
    void getPSMS(void);
    bool setPSMS(uint8_t mode);
    void getEDRXS(void);
    bool setEDRXS(uint8_t mode);
    void getEDRXRDP(void);
    uint8_t getTZR(void);
    bool setTZR(uint8_t onoff);
    void getIPCA(void);
    bool setIPCA(uint8_t n);
    void getCIOTOPT(void);
    bool setCIOTOPT(uint8_t n);
    bool setGAPNRC(uint8_t cid);
    String getEER(void);
    bool setQGACT(uint8_t op);
    uint8_t getQBAND(void);
    bool setQBAND(uint8_t band);
    bool rest(uint8_t mode = 1);
    uint8_t getQSPCHSC(void);
    bool setQSPCHSC(uint8_t mode);
    void getQLOCKF(void);
    bool setQLOCKF(uint8_t mode);
    uint8_t getQCSEARFCN(void);
    void getQCGDEFCONT(void);
    bool setQCGDEFCONT(uint8_t type);
    uint8_t getQNBIOTRAI(void);
    bool setQNBIOTRAI(uint8_t rai);
    void getQNBIOTEVENT(void);
    bool setQNBIOTEVENT(uint8_t enable);
    uint8_t getQATWAKEUP(void);
    bool setQATWAKEUP(uint8_t enable);
    String getQENG(uint8_t mode);
    String getQCCID(void);
    uint8_t getQPOWD(void);
    bool setQPOWD(uint8_t op);
    uint8_t getQSCLK(void);
    bool setQSCLK(uint8_t mode);
    bool setQFOTADL(String url);
    uint8_t getQGNSSC(void);
    bool setQGNSSC(uint8_t mode);
    uint8_t getQGNSSRD(void);
	uint8_t getQGNSSRD2(void);
    bool getQGNSSRD(char* sth);

    bool setCmdEcho(String str);
    bool saveConfig(void);
    bool setBaudrate(unsigned long baud);
    unsigned long readBaudrate(void);
    
	bool setAliyunserver(char*ProductKey,char* IPAddress,char* port,char connectID = '0');
    bool connect_Aliyun(char* ProductKey,char* DeviceName,char* DeviceSecret);
    void configMQTT(void);
    bool setMQTTVersion(uint8_t version);
    void getMQTTParameter(void);
    bool openMQTT(char connectID, char* Server, char* Port);
    bool readMQTT(void);
    bool checkMQTT(uint8_t connectID);
    bool closeMQTT(uint8_t connectID);
    bool setServer(char* IPAddress,char* port,char connectID = '0');
    bool connected();
    bool connectServer(char connectID, char* clientID, char* UserName, char* PassWord);
    bool connect(char * clientID, char * username,char * password,char connectID ='0');
    bool getQMTCONN(void);
    bool disConnectServer(String connectID);
    bool subTopic(char connectID, char msgID, char* topic, char qos);
    bool unSubTopic(char connectID, char msgID, char* topic);
    bool publish(char* topic,char* msg);
    bool pubMessage(char connectID,char msgID, char qos, char retain, char* topic, char* msg);
    void setCallback(void (*call)(char*, uint8_t*, unsigned int));
    void loop();
    bool configSleepMode(eSleepMode_t mode);
    bool BC20Wakeup();
    void setTAUTime(uint8_t time, eTAUunit_t unit);
    void setActiveTime(uint8_t time, eAcTunit_t unit);
    bool setPSMMode(ePSM_t status);
    bool seteDRX(uint8_t mode);// OFF = 0 // ON = 1
    bool seteDRX(uint8_t mode,uint8_t ActType, uint8_t Requested);
    bool seteDRX(uint8_t mode,uint8_t ActType, uint8_t Requested, uint8_t NW);
    bool seteDRX(uint8_t mode,uint8_t ActType, uint8_t Requested, uint8_t NW,uint8_t PTW);
    bool setNBEventReport(bool mode);
    void setPSMCallback(void (*call)(void));
	uint8_t getNum(char* str);
	char*removeSthString(char* sth, char* str);
	String removeSthString(String sth, String str);
	char* GetSthfrontString(char* sth, char* str);
	String GetSthfrontString(String sth, String str);	
	bool CheckRecData(char* str);
	void flushBC20Serial(void);
	void getRecData(uint8_t INFO);
	uint8_t getTailHandle();
	void getRecDataforNum(uint8_t num, uint8_t* &buf);
	void getRecDataforNum_NoCheck(uint8_t num, uint8_t* &buf);
	uint8_t getIntforString(String CMD,String basic,uint8_t n);
	void DBGPrintRecData(void);
	void getSatelliteInformation(uint8_t start, uint8_t num, char* str, char* sys);
	char color='R';
	void LED_ON();
	void LED_OFF();
	void changeColor(uint8_t newColor);
	void controlLED(char * chr);
	void controlLED(String str);
	void LEDFlash(String Color);
	bool stmLowpower();
	bool stmWakeup(uint8_t Awake_Pin);
	
	virtual void sendATCMD(char* str) =0;
	virtual void sendATCMD(String str) =0;
	virtual void sendATCMD(uint8_t num) =0;
	virtual void sendATCMD(String str,uint8_t num) =0;
	virtual void sendATCMD(String str,String cmd) =0;
	virtual String readData() =0;
	virtual void receviceATCMD(uint32_t timeout) =0;
	virtual void sendATCMDBychar(char str) =0;
	virtual bool available(void)=0;
  private:
    void (*callback)(char*, uint8_t*, unsigned int);
    void (*PSMcallback)(void);
    String _TAUValue = "";
    String _AcTValue = "";
};
class DFRobot_BC20_Serial:public DFRobot_BC20{
	public:
	    DFRobot_BC20_Serial(HardwareSerial*SSerial1);
		~DFRobot_BC20_Serial(void){};
		void sendATCMD(char* str);
		void sendATCMD(String str);
		void sendATCMD(uint8_t num);
		void sendATCMD(String str,uint8_t num);
		void sendATCMD(String str,String cmd);
		String readData();
		void receviceATCMD(uint32_t timeout);
		void sendATCMDBychar(char str);
		bool available(void);
	private:	
		HardwareSerial*SSerial;
};
class DFRobot_BC20_IIC:public DFRobot_BC20{
	public:
	    DFRobot_BC20_IIC(uint8_t addr);
		~DFRobot_BC20_IIC(void){};
		void sendATCMD(char* str);
		void sendATCMD(String str);
		void sendATCMD(uint8_t num);
		void sendATCMD(String str,uint8_t num);
		void sendATCMD(String str,String cmd);
		String readData();
		void receviceATCMD(uint32_t timeout);
		void sendATCMDBychar(char str);
		bool available(void);
	private:
		int IIC_addr;
};
#ifndef ARDUINO_ESP32_DEV
class DFRobot_BC20_SW_Serial:public DFRobot_BC20{
	public:
	    DFRobot_BC20_SW_Serial(SoftwareSerial* ss);
		~DFRobot_BC20_SW_Serial(void){};
		void sendATCMD(char* str);
		void sendATCMD(String str);
		void sendATCMD(uint8_t num);
		void sendATCMD(String str,uint8_t num);
		void sendATCMD(String str,String cmd);
		String readData();
		void receviceATCMD(uint32_t timeout);
		void sendATCMDBychar(char str);
		bool available(void);
	private:
		SoftwareSerial* SSerial;
};
#endif
#endif
