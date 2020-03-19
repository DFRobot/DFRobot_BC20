#include "DFRobot_BC20.h"
#include "DFRobot_queue.h"
#include "hardwareSerial.h"

#define POWERKEY    15  // io 15
#define SETKEY      13
#define PSW_EINT    5   // io 5
char *ret = NULL;
sSIMCard_t sSIMCard;
sRecData_t sRecData;
sPII_t sPII;
sMI_t sMI;
sSQ_t sSQ;
sESQ_t sESQ;
sGSN_t sGSN;
sGLL_t sGLL;
sGGA_t sGGA;
sGSA_t sGSA;
sRMC_t sRMC;
sVTG_t sVTG;
sCLK_t sCLK;
sGGNS_t sGGNS;
sGGNS_t2 sGGNS2;
sSAT_t sSAT;
sSAT_t2 sSAT2;
sCEREG_t sCEREG;
char* ret1=NULL;
int num_q=0;
uint8_t Flag=0;

uint8_t DFRobot_BC20::getNum(char* str){
    if(strcmp(str,NMEA_RMC)==0){
        return 0;
    }else if(strcmp(str,NMEA_VTG)==0){
        return 1;
    }else if(strcmp(str,NMEA_GGA)==0){
        return 2;
    }else if(strcmp(str,NMEA_GSA)==0){
        return 3;
    }else if(strcmp(str,NMEA_GSV)==0){
        return 4;
    }else if(strcmp(str,NMEA_GLL)==0){
        return 5;
    }else{
        return 255;
    }
}

int indexOf(char *str1,char *str2)  
{  
    char *p=str1;  
    int i=0;  
    p=strstr(str1,str2);  
    if(p==NULL)  
        return -1;  
    else{  
        while(str1!=p)  
        {  
            str1++;  
            i++;  
        }  
    }  
    return i;  
} 

void substring(char *dest,char *src,int start,int end)  
{  
    int i=start;  
    if(start>strlen(src))return;  
    if(end>strlen(src))  
        end=strlen(src);  
    while(i<end)  
    {     
        dest[i-start]=src[i];  
        i++;  
    }  
    dest[i-start]='\0';  
    return;  
} 


String DFRobot_BC20::removeSthString(String sth, String str){
    String ret = str;
    int tempInt = 0;
    tempInt = ret.indexOf(sth);
    if(tempInt != -1){
        ret = ret.substring(tempInt+sth.length(),ret.length());
    }
    return ret;
}

char* DFRobot_BC20::removeSthString(char* sth, char* str){
    char* ret = str;
    int tempInt = 0;
    tempInt =indexOf(ret,sth);
    if(tempInt != -1){
    substring(ret,ret,tempInt+strlen(sth),strlen(ret));
    }
    return ret;
}

String DFRobot_BC20::GetSthfrontString(String sth, String str){/*Returns the string preceding the given string*/
    String ret = str;
    String temp = sth;
    uint8_t tempInt = 0;
    tempInt = ret.indexOf(temp);
    if(tempInt != -1){
        ret = ret.substring(0,tempInt);
    }
    return ret;
}

char* DFRobot_BC20::GetSthfrontString(char* sth, char* str){/*Returns the string preceding the given string*/  
	if(ret1!=NULL)
	{
		free(ret1);
		ret1=NULL;
	}
	ret1=(char*)malloc(strlen(str)+1);
	ret1[strlen(str)]='\0';
	memcpy(ret1,str,strlen(str));
    char* temp = sth;
    uint8_t tempInt = 0;
    tempInt = indexOf(ret1,temp);
    if(tempInt != -1){
    substring(ret1,ret1,0,tempInt);
    }
    return ret1;
}

bool DFRobot_BC20::CheckRecData(char* str){/*Check if there is a corresponding string in the data received in the queue*/
    bool ret = false;
    static struct sQueueData *p = NULL;
    p = cuappDequeue();
    while(p != NULL){
       if(strncmp((const char* )p->data,str,strlen(str)) == 0){
			ret = true;				
       }
       free(p);
       p = cuappDequeue();
       
    }
    return ret;
}

void DFRobot_BC20::flushBC20Serial(void){/*Clear the BC20 serial port of unprocessed data and receive queue data*/
    receviceATCMD(1);
    CheckRecData("");
}

void DFRobot_BC20::getRecData(uint8_t INFO){/*Gets information about a parameter from the returned data.For example, the CIMI number of the SIM card*/
    static struct sQueueData *p;
	static struct sQueueData *q;
    do{
        p = cuappDequeue();
        if(strncmp((const char* )p->data,"\r\n",2) == 0){
            free(p);
			p=NULL;
            break;
        }
		q=cuappDequeue();
		if(p==NULL)
		{
			free(p);
			p=NULL;
			break;
		}
        free(p);
		p=NULL;		
    }while(p != NULL);
    switch(INFO){
        case CIMI_INFO:
            if(sSIMCard.IMEI_NUM == NULL){
                sSIMCard.IMEI_NUM = (uint8_t *)malloc(15+1);
            }else{
                free(sSIMCard.IMEI_NUM);
				sSIMCard.IMEI_NUM = NULL;
                sSIMCard.IMEI_NUM = (uint8_t *)malloc(15+1);
            }
            memset((sSIMCard.IMEI_NUM),'\0',16);
            memcpy((sSIMCard.IMEI_NUM),q->data, 15);
			free(q);
			q=NULL;
            break;
        default:
			free(q);
			q=NULL;		
            break;
    }
    flushBC20Serial();
}
uint8_t DFRobot_BC20::getTailHandle(){
    static struct sQueueData *p = NULL;
    p = getQueueTail();
    if(p != NULL){
        return p->handle;
    }
    return 0;
}
void DFRobot_BC20::getRecDataforNum(uint8_t num, uint8_t* &buf){/*Gets the number of received data*/
    static struct sQueueData *p = NULL;
    p = getQueueTail();
    if(strncmp((const char* )p->data,"OK\r\n",4) == 0){
        p = getQueueHead();
        if(p == NULL){
            return;
        }
        for(uint8_t i = 1; i < num; i++){
            p = p->next;
            if(p == NULL){
                return;
            }
        }
        if(p != NULL){
            if(buf != NULL){
                free(buf);
            }
            buf = (uint8_t *)malloc((p->len) +1);
			if(buf==NULL)
			{
				free(buf);
				return;
			}
            memset(buf,'\0',(p->len)+1);
            memcpy(buf,p->data,p->len);
        }
    } 
}

void DFRobot_BC20::getRecDataforNum_NoCheck(uint8_t num, uint8_t* &buf){/*Gets the number of received data*/
    static struct sQueueData *p = NULL;
    p = getQueueHead();
    if(p == NULL){
        return;
    }
	if((p->len)<3)
	{
		p=p->next;
        if(p == NULL){
            return;
        }		
	}	
    for(uint8_t i = 1; i < num; i++){
        p = p->next;
        if(p == NULL){
            return;
        }
    }
	if(buf != NULL){
		free(buf);
		buf = NULL;
	}	
    if(p != NULL){
        buf = (uint8_t *)malloc((p->len) +1);
        if(buf == NULL){
            free(buf);
            buf = NULL;
        }
        memset(buf,'\0',(p->len)+1);
        memcpy(buf,p->data,p->len);
    }
}

uint8_t DFRobot_BC20::getIntforString(String CMD,String basic,uint8_t n){/*Send the command and get the number specified in the returned command*/
     uint8_t *data=NULL;
    String tempStr;
	int a=0;
    flushBC20Serial();
    sendATCMD(CMD);
    receviceATCMD(3000);
    getRecDataforNum(2,data);
    tempStr  = (char *)data;
    if(data != NULL){
        free(data);
		data=NULL;
    }
    if( n == 1){
        tempStr  = removeSthString("+"+basic+": ",tempStr);
		a=GetSthfrontString("\r\n",tempStr).toInt();
		flushBC20Serial();
        return a;
    }else{
		flushBC20Serial();
        return 0; 
    }
}

void DFRobot_BC20::DBGPrintRecData(void){
    static struct sQueueData *p = NULL;
    uint8_t tempNum = 0;
}


DFRobot_BC20::DFRobot_BC20(void){
    pinMode(PSW_EINT,OUTPUT);
    pinMode(POWERKEY,OUTPUT);
    digitalWrite(POWERKEY, LOW);
}

DFRobot_BC20::~DFRobot_BC20(void){
    
}

bool DFRobot_BC20::powerOn(void){
	if(available())
	{
		readData();
	}
    flushBC20Serial();
    if(checkBC20()){
        return true;
    }
    sendATCMD("powerOn");
	delay(300);
	sendATCMD("");
    if(checkBC20()){
        return true;
    }
	return false;
}

bool DFRobot_BC20::checkBC20(void){
    sendATCMD(CHECK_BC20);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

bool DFRobot_BC20::checkStmStauts(void){
    flushBC20Serial();
    sendATCMD("STMSTATUS");
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

bool DFRobot_BC20::checkNBCard(void){
    bool ret = false;
    sendATCMD(CHECK_NB_CARD);
    receviceATCMD(5000);
    ret = CheckRecData(AT_OK);
    if(!ret){
        rest();
    }
    return ret;
}

bool DFRobot_BC20::setCmdEcho(String str){/*Sets whether the AT command is echoed*/
    sendATCMD(str);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

bool DFRobot_BC20 :: setBaudrate(unsigned long baud){/*Set the baud rate of UART for BC20*/
    sendATCMD("AT+IPR=");
    sendATCMD(baud);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

unsigned long DFRobot_BC20 :: readBaudrate(){/*Read the UART baud rate of BC20*/
    unsigned long ret=0;
    sendATCMD(READ_BAUD);
    receviceATCMD(300);
    uint8_t *baud=NULL;
    baud = (uint8_t *)malloc(1);
    getRecDataforNum(2,baud);
    uint8_t i = 0,k = 0;
    while((baud[i] != '\r') && (i < 15)){
        i++;
    }
    for(uint8_t j = i-1; j > 0; j--){
        if((baud[j] > '9') || (baud[j] < '0')){
            break;
        }
        ret = ret + ((baud[j] - '0')*(pow(10,k)) );
        k++;
        
    }
    free(baud);
	baud=NULL;
    return ret;
}

bool DFRobot_BC20 :: saveConfig(){/*Power loss saves the current configuration*/
    sendATCMD("AT&W0");
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

void DFRobot_BC20 :: getPII(void){
    //sendATCMD(GET_PII);
/* 			Wire.beginTransmission(0x33);
			Wire.write(0x00);
			Wire.write("ATI",3);
			Wire.write(0x0D);
			Wire.write(0x0A);
			Wire.endTransmission(); */	
/*     receviceATCMD(300);
    getRecDataforNum(2,sPII.company);
    getRecDataforNum(3,sPII.deviceType);
    getRecDataforNum(4,sPII.Revision);
    flushBC20Serial(); */
}

void DFRobot_BC20 :: getMI(void){
    sendATCMD(GET_MI);
    receviceATCMD(300);
    getRecDataforNum(2,sMI.company);
    getRecDataforNum(3,sMI.deviceType);
    getRecDataforNum(4,sMI.Revision);
    flushBC20Serial();
}

void DFRobot_BC20 :: getMM(void){
    sendATCMD(GET_MM);
    receviceATCMD(300);
    getRecDataforNum(2,sPII.deviceType);
    flushBC20Serial();
}

void DFRobot_BC20 :: getMR(void){
    sendATCMD(GET_MR);
    receviceATCMD(300);
    getRecDataforNum(2,sPII.Revision);
    flushBC20Serial();
}

void DFRobot_BC20 :: getSQ(void){
    uint8_t *data=NULL;
    int tempInt = 0;
    String tempStr="";
    flushBC20Serial();
    sendATCMD(GET_SQ);
    receviceATCMD(300);
    getRecDataforNum(2,data);
    tempStr  = (char *)data;
    if(data != NULL){
        free(data);
		data=NULL;
    }else{
        return;
    }
    tempStr  = removeSthString("+CSQ: ",tempStr);
    sSQ.rssi = GetSthfrontString(",",tempStr).toInt();
    tempStr  = removeSthString(",",tempStr);
    sSQ.ber  = GetSthfrontString("\r\n",tempStr).toInt();
    flushBC20Serial();
}

void DFRobot_BC20 :: getESQ(void){
    uint8_t *data=NULL;
    int tempInt = 0;
    String tempStr="";
    sendATCMD(GET_ESQ);
    receviceATCMD(300);
    getRecDataforNum(2,data);
    tempStr  = (char *)data;
    if(data != NULL){
        free(data);
		data=NULL;
    }
    tempStr  = removeSthString("+CESQ: ",tempStr);

    sESQ.rxlev = GetSthfrontString(",",tempStr).toInt();
    tempStr  = removeSthString(",",tempStr);

    sESQ.ber = GetSthfrontString(",",tempStr).toInt();
    tempStr  = removeSthString(",",tempStr);

    sESQ.rscp = GetSthfrontString(",",tempStr).toInt();
    tempStr  = removeSthString(",",tempStr);

    sESQ.ecno = GetSthfrontString(",",tempStr).toInt();
    tempStr  = removeSthString(",",tempStr);

    sESQ.rsrq = GetSthfrontString(",",tempStr).toInt();
    tempStr  = removeSthString(",",tempStr);

    sESQ.rsrp = GetSthfrontString(",",tempStr).toInt();
    tempStr  = removeSthString(",",tempStr);
	
    flushBC20Serial();
}

void DFRobot_BC20 :: clearGPS(){
	if(ret!=NULL)
	{
	  free(ret);
	  ret=NULL;       
	}	
}

void DFRobot_BC20 :: getGSN(uint8_t cmd){
	if(available())
	{
		readData();
	}
    uint8_t *data=NULL;
    int tempInt = 0;
    String tempStr="";
    flushBC20Serial();
    sendATCMD(GET_GSN,cmd);
    receviceATCMD(300);
    getRecDataforNum(2,data);
    tempStr  = (char *)data;
    if(data != NULL){
        free(data);
		data=NULL;
    }
    if(cmd){
		tempStr  = removeSthString("+CGSN: ",tempStr);
		tempStr = GetSthfrontString("\r\n",tempStr);
		sGSN.imei=tempStr;			
    }else{
        sGSN.sn = tempStr;
	}	
}

static uint8_t ConvertUint8(String num, uint8_t bit){
    uint8_t ret = 0;
    String temp = "";
    for(uint8_t i = bit; i > 0; i--){
        temp = num.substring(i-1,i);
        if(temp.toInt() == 1){
            ret |= 1<<(bit-i);
        }else{
        }
    }
    return ret;
}

void DFRobot_BC20 :: getEREG(void){/*待完善*/
    uint8_t *data=NULL;
    String tempStr="";
    String ActTime="";
    String PerTau="";
    flushBC20Serial();
    sendATCMD(GET_EREG);
    receviceATCMD(300);
    getRecDataforNum(2,data);
    tempStr  = (char *)data;
    if(data != NULL){
        free(data);
		data=NULL;
    }
    tempStr  = removeSthString("+CEREG: ",tempStr);
    uint8_t temp  = GetSthfrontString(",",tempStr).toInt();
    if(temp == 4){
        tempStr  = removeSthString(",",tempStr);/*4*/
        tempStr  = removeSthString(",",tempStr);/*1*/
        tempStr  = removeSthString(",",tempStr);/*"5A10"*/
        tempStr  = removeSthString(",",tempStr);/*"0CFD33A0"*/
        tempStr  = removeSthString(",",tempStr);/*9*/
        tempStr  = removeSthString(",",tempStr);/* */
        tempStr  = removeSthString(",",tempStr);/* */
        ActTime = tempStr;
        ActTime = removeSthString("\"",ActTime);
        ActTime = GetSthfrontString("\"",ActTime);
        sCEREG.ActiveTime = ConvertUint8(ActTime.substring(3,ActTime.length()),5);
        temp = ConvertUint8(ActTime.substring(0,3),3);
        if(temp == 0){
            sCEREG.ActiveUint = "2 Seconds";
        }else if(temp == 0x01){
            sCEREG.ActiveUint = "1 Minute";
        }else if(temp == 0x02){
            sCEREG.ActiveUint = "Decihours";
        }else{
            sCEREG.ActiveUint = "Deactivated";
        }
        tempStr  = removeSthString(",",tempStr);/*"11100000"*/
        PerTau = tempStr;
        PerTau = removeSthString("\"",PerTau);
        PerTau = GetSthfrontString("\"",PerTau);
        sCEREG.TAUTime = ConvertUint8(PerTau.substring(3,ActTime.length()),5);
        temp = ConvertUint8(PerTau.substring(0,3),3);
        switch(temp){
            case 0x00:
                sCEREG.TAUUint = "10 Minutes";
                break;
            case 0x01:
                sCEREG.TAUUint = "1 Hour";
                break;
            case 0x02:
                sCEREG.TAUUint = "10 Hour";
                break;
            case 0x03:
                sCEREG.TAUUint = "2 Seconds";
                break;
            case 0x04:
                sCEREG.TAUUint = "30 Seconds";
                break;
            case 0x05:
                sCEREG.TAUUint = "1 Minute";
                break;
            case 0x06:
                sCEREG.TAUUint = "320 Hours";
                break;
            case 0x07:
                sCEREG.TAUUint = "Deactivated";
                break;
            default:
                break;
        }
    }
}

bool DFRobot_BC20 :: setEREG(uint8_t cmd){/*待完善*/
    flushBC20Serial();
    sendATCMD(SET_EREG,cmd);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

void DFRobot_BC20 :: getSCON(void){
    flushBC20Serial();
    sendATCMD(GET_SCON);
    receviceATCMD(300);
}

/*!
 * @param cmd
 *     0 Disable unsolicited result code
 *     1 Enable unsolicited result code: “+CSCON:<mode>”
 * return true or false
 */
bool DFRobot_BC20 :: setSCON(uint8_t cmd){
    flushBC20Serial();
    sendATCMD(SET_SCON,cmd);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

void DFRobot_BC20 :: getOPS(void){/*待定*/
    flushBC20Serial();
    sendATCMD(GET_OPS);
    receviceATCMD(630000);
}

bool DFRobot_BC20 :: setOPS(uint8_t cmd){/*待定*/
    flushBC20Serial();
    sendATCMD(SET_OPS,cmd);
    receviceATCMD(630000);
    return CheckRecData(AT_OK);
}

/*!
 * @return Indicates the state of PDP context activation.
 *     0 Detached
 *     1 Attached
 */
uint8_t DFRobot_BC20 :: getGATT(void){
	if(available())
	{
		readData();
	}
    return getIntforString(GET_GATT,SET_GATT,1);
}

/*!
 * @param state
 *     0 Detached
 *     1 Attached
 * return true or false
 */
bool DFRobot_BC20 :: setGATT(uint8_t state){
    flushBC20Serial();
    sendATCMD(SET_GATT,state);
    receviceATCMD(75000);
    return CheckRecData(AT_OK);
}

void DFRobot_BC20 :: getGDCONT(void){/*待定*/
    flushBC20Serial();
    sendATCMD(GET_GDCONT);
    receviceATCMD(300);
}

bool DFRobot_BC20 :: setGDCONT(uint8_t state){/*待定*/
    flushBC20Serial();
    sendATCMD(SET_GDCONT,state);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

void DFRobot_BC20 :: getGACT(void){/*待定*/
    flushBC20Serial();
    sendATCMD(GET_GACT);
    receviceATCMD(150000);
}

bool DFRobot_BC20 :: setGACT(uint8_t state){/*待定*/
    flushBC20Serial();
    sendATCMD(SET_GACT,state);
    receviceATCMD(150000);
    return CheckRecData(AT_OK);
}

void DFRobot_BC20 :: getGPADDR(void){/*待定*/
    flushBC20Serial();
    sendATCMD(GET_GPADDR);
    receviceATCMD(300);
}

bool DFRobot_BC20 :: setGPADDR(uint8_t state){
    return CheckRecData(AT_OK);
}

uint8_t * DFRobot_BC20 :: getIMI(void){
	if(available())
	{
		readData();
	}
	uint8_t* data;
    flushBC20Serial();
    sendATCMD(GET_IMI);
    receviceATCMD(300);
    getRecData(CIMI_INFO);
    return (uint8_t *)sSIMCard.IMEI_NUM;
}

/*!
 * @return
 *     0 Minimum functionality
 *     1 Full functionality
 *     4 Disable the UE from both transmitting and receiving RF signals
 */
uint8_t DFRobot_BC20 :: getFUN(void){
    return getIntforString(GET_FUN,SET_FUN,1);
}

/*!
 * @param fun
 *     0 Minimum functionality
 *     1 Full functionality
 *     4 Disable the UE from both transmitting and receiving RF signals
 * @return true or false
 */
bool DFRobot_BC20 :: setFUN(uint8_t fun){
    flushBC20Serial();
    sendATCMD(SET_FUN,fun);
    receviceATCMD(15000);
    return CheckRecData(AT_OK);
}

/*!
 * @return
 *     0 Disable result code
 *     1 Enable result code and use numeric values
 *     2 Enable result code and use verbose values
 */
uint8_t DFRobot_BC20 :: getMEE(void){
    uint8_t *data=NULL;
    String tempStr="";
    flushBC20Serial();
    sendATCMD(GET_MEE);
    receviceATCMD(300);
    getRecDataforNum(2,data);
    tempStr  = (char *)data;
    if(data != NULL){
        free(data);
		data=NULL;
    }
    tempStr  = removeSthString("+CMEE: ",tempStr);
    return GetSthfrontString("\r\n",tempStr).toInt();
}

/*!
 * @param n
 *     0 Disable result code
 *     1 Enable result code and use numeric values
 *     2 Enable result code and use verbose values
 * @return true or false
 */
bool DFRobot_BC20 :: setMEE(uint8_t n){
    flushBC20Serial();
    sendATCMD(SET_MEE,n);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

/*!
 * @return time
 * The format is "yy/MM/dd,hh:mm:ss±zz",
 */
String DFRobot_BC20 :: getCLK(void){
    uint8_t *data=NULL;
    String tempStr="";
    String retStr = "";
    flushBC20Serial();
    sendATCMD(GET_CLK);
    receviceATCMD(300);
    getRecDataforNum(2,data);
    tempStr  = (char *)data;
    if(data != NULL){
        free(data);
		data=NULL;
    }
    tempStr  = removeSthString("+CCLK: ",tempStr);
    retStr = tempStr;
    sCLK.Year = GetSthfrontString("/",tempStr).toInt();
    tempStr  = removeSthString("/",tempStr);
    sCLK.Month = GetSthfrontString("/",tempStr).toInt();
    tempStr  = removeSthString("/",tempStr);
    sCLK.Day = GetSthfrontString(",",tempStr).toInt();
    tempStr  = removeSthString(",",tempStr);
    sCLK.Hour = GetSthfrontString(":",tempStr).toInt();
    tempStr  = removeSthString(":",tempStr);
    sCLK.Minute = GetSthfrontString(":",tempStr).toInt();
    tempStr  = removeSthString(":",tempStr);
    sCLK.Second = GetSthfrontString("GMT",tempStr).toInt();
    tempStr  = removeSthString("GMT",tempStr);
    sCLK.TimeEquation = tempStr.toInt();
    sCLK.Hour = sCLK.Hour+sCLK.TimeEquation;
    tempStr = GetSthfrontString("\r\n",tempStr);
    return retStr;
}

void DFRobot_BC20 :: getBC(void){/*待定*/
    flushBC20Serial();
    sendATCMD(GET_BC);
    receviceATCMD(300);
}

void DFRobot_BC20 :: getPSMS(void){/*待定*/
    flushBC20Serial();
    sendATCMD(GET_PSMS);
    receviceATCMD(300);
}

bool DFRobot_BC20 :: setPSMS(uint8_t mode){/*待定*/
    return CheckRecData(AT_OK);
}

void DFRobot_BC20 :: getEDRXS(void){/*待定*/
    flushBC20Serial();
    sendATCMD(GET_EDRXS);
    receviceATCMD(300);
}

bool DFRobot_BC20 :: setEDRXS(uint8_t mode){/*待定*/
    return CheckRecData(AT_OK);
}

void DFRobot_BC20 :: getEDRXRDP(void){/*待定*/
    flushBC20Serial();
    sendATCMD(GET_EDRXRDP);
    receviceATCMD(300);
}

/*!
 * @return
 *     0 Disable time zone change event reporting
 *     1 Enable time zone change event reporting by unsolicited result code “+CTZV: <tz>”.
 */
uint8_t DFRobot_BC20 :: getTZR(void){
    uint8_t *data=NULL;
    String tempStr="";
    flushBC20Serial();
    sendATCMD(GET_TZR);
    receviceATCMD(300);
    getRecDataforNum(2,data);
    tempStr  = (char *)data;
    if(data != NULL){
        free(data);
		data=NULL;
    }
    tempStr  = removeSthString("+CTZR: ",tempStr);
    return GetSthfrontString("\r\n",tempStr).toInt();
}

/*!
 * @param onoff
 *     0 Disable time zone change event reporting
 *     1 Enable time zone change event reporting by unsolicited result code “+CTZV: <tz>”.
 * @return true or false.
 */
bool DFRobot_BC20 :: setTZR(uint8_t onoff){
    flushBC20Serial();
    sendATCMD(SET_TZR,onoff);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

void DFRobot_BC20 :: getIPCA(void){/*待定*/
    flushBC20Serial();
    sendATCMD(GET_IPCA);
    receviceATCMD(300);
}

bool DFRobot_BC20 :: setIPCA(uint8_t n){/*待定*/
    return CheckRecData(AT_OK);
}

void DFRobot_BC20 :: getCIOTOPT(void){/*待定*/
    flushBC20Serial();
    sendATCMD(GET_CIOTOPT);
    receviceATCMD(300);
}

bool DFRobot_BC20 :: setCIOTOPT(uint8_t n){/*待定*/
    return CheckRecData(AT_OK);
}

bool DFRobot_BC20 :: setGAPNRC(uint8_t n){/*待定*/
    return CheckRecData(AT_OK);
}

String DFRobot_BC20 :: getEER(void){
    uint8_t *data=NULL;
    String tempStr = "";
    flushBC20Serial();
    sendATCMD(GET_EER);
    receviceATCMD(300);
    getRecDataforNum(2,data);
    tempStr = (char *)data;
    if(data != NULL){
        free(data);
		data=NULL;
    }
    tempStr = removeSthString("+CEER: ",tempStr);
    return tempStr;
}

bool DFRobot_BC20 :: setQGACT(uint8_t op){/*待定*/
    return CheckRecData(AT_OK);
}

/*!
 * @return Operating Band.
 * Integer value indicating the band(s) being used.
 * Valid values: 1, 2, 3, 5, 8, 12, 13, 17, 18, 19, 20, 25, 26, 28, 66
 */
uint8_t DFRobot_BC20 :: getQBAND(void){
    uint8_t *data=NULL;
    String tempStr = "";
    flushBC20Serial();
    sendATCMD(GET_QBAND);
    receviceATCMD(300);
    getRecDataforNum(2,data);
    tempStr = (char *)data;
    if(data != NULL){
        free(data);
		data=NULL;
    }
    tempStr = removeSthString("+QBAND: ",tempStr);
    return GetSthfrontString("\r\n",tempStr).toInt();
}

bool DFRobot_BC20 :: setQBAND(uint8_t n){/*待定*/
    return CheckRecData(AT_OK);
}
/*!
 * @return true or false.
 */
bool DFRobot_BC20 :: rest(uint8_t mode){
    flushBC20Serial();
    sendATCMD(SET_QRST,mode);
    receviceATCMD(300);
    delay(1000);
    return true;
    //return CheckRecData(AT_OK);
}
/*!
 * @return Integer
 *     0 Old algorithm
 *     1 New algorithm
 */
uint8_t DFRobot_BC20 :: getQSPCHSC(void){
    uint8_t *data=NULL;
    String tempStr = "";
    flushBC20Serial();
    sendATCMD(GET_QSPCHSC);
    receviceATCMD(300);
    getRecDataforNum(2,data);
    tempStr = (char *)data;
    if(data != NULL){
        free(data);
		data=NULL;
    }
    tempStr = removeSthString("+QSPCHSC: ",tempStr);
    return GetSthfrontString("\r\n",tempStr).toInt();
}
/*!
 * @param mode
 *     0 Old algorithm
 *     1 New algorithm
 * @return true or false.
 */
bool DFRobot_BC20 :: setQSPCHSC(uint8_t mode){
    flushBC20Serial();
    sendATCMD(SET_QSPCHSC,mode);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

void DFRobot_BC20 :: getQLOCKF(void){/*待定*/

}

bool DFRobot_BC20 :: setQLOCKF(uint8_t mode){/*待定*/
    return CheckRecData(AT_OK);
}

/*!
 * @return Integer value indicating the EARFCN has been cleared
 *     0 Successful
 *     2 Failure
 *     3 Not found
 */
uint8_t DFRobot_BC20 :: getQCSEARFCN(void){
    uint8_t *data=NULL;
    String tempStr = "";
    flushBC20Serial();
    sendATCMD(GET_QCSEARFCN);
    receviceATCMD(300);
    getRecDataforNum(2,data);
    tempStr = (char *)data;
    if(data != NULL){
        free(data);
		data=NULL;
    }
    tempStr = removeSthString("+QCSEARFCN: ",tempStr);
    return GetSthfrontString("\r\n",tempStr).toInt();
}

void DFRobot_BC20 :: getQCGDEFCONT(void){/*待定*/
    flushBC20Serial();
    sendATCMD(GET_QCGDEFCONT);
    receviceATCMD(300);
}

bool DFRobot_BC20 :: setQCGDEFCONT(uint8_t type){/*待定*/
    return CheckRecData(AT_OK);
}

/*!
 * @return
 *     0 No information available (or none of the other options apply)
 *     1 TE will send only 1 UL packet and no DL packets expected
 *     2 TE will send only 1 UL packet and only 1 DL packet expected
 */
uint8_t DFRobot_BC20 :: getQNBIOTRAI(void){
    uint8_t *data=NULL;
    String tempStr = "";
    flushBC20Serial();
    sendATCMD(GET_QNBIOTRAI);
    receviceATCMD(300);
    getRecDataforNum(2,data);
    tempStr = (char *)data;
    if(data != NULL){
        free(data);
		data=NULL;
    }
    tempStr = removeSthString("+QNBIOTRAI: ",tempStr);
    return GetSthfrontString("\r\n",tempStr).toInt();
}

/*!
 * @param rai
 *     0 No information available (or none of the other options apply)
 *     1 TE will send only 1 UL packet and no DL packets expected
 *     2 TE will send only 1 UL packet and only 1 DL packet expected
 * @return true or false.
 */
bool DFRobot_BC20 :: setQNBIOTRAI(uint8_t rai){
    flushBC20Serial();
    sendATCMD(SET_QNBIOTRAI,rai);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

void DFRobot_BC20 :: getQNBIOTEVENT(void){/*待定*/
    flushBC20Serial();
    sendATCMD(GET_QNBIOTEVENT);
    receviceATCMD(300);
}

bool DFRobot_BC20 :: setQNBIOTEVENT(uint8_t enable){/*待定*/
    return CheckRecData(AT_OK);
}

/*!
 * @return
 *     0 Disable indication on this channel when modem wakes up from deep sleep
 *     1 Enable indication on this channel when modem wakes up from deep sleep
 */
uint8_t DFRobot_BC20 :: getQATWAKEUP(void){
    uint8_t *data=NULL;
    String tempStr = "";
    flushBC20Serial();
    sendATCMD(GET_QATWAKEUP);
    receviceATCMD(300);
    getRecDataforNum(2,data);
    tempStr = (char *)data;
    if(data != NULL){
        free(data);
		data=NULL;
    }
    tempStr = removeSthString("+QATWAKEUP: ",tempStr);
    return GetSthfrontString("\r\n",tempStr).toInt();
}

/*!
 * @param enable
 *     0 Disable indication on this channel when modem wakes up from deep sleep
 *     1 Enable indication on this channel when modem wakes up from deep sleep
 * @return true or false.
 */
bool DFRobot_BC20 :: setQATWAKEUP(uint8_t enable){
    flushBC20Serial();
    sendATCMD(SET_QATWAKEUP,enable);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

/*!
 * @param mode
 *     0 Display radio information for serving and neighbor cells
 *     1 Display data transfer information only if modem in RRC-CONNECTED state
 * @return String.
 */
String DFRobot_BC20 :: getQENG(uint8_t mode){
    uint8_t *data0=NULL,*data1=NULL;
    String tempStr0 = "",tempStr1 = "";
    flushBC20Serial();
    sendATCMD(GET_QENG,mode);
    receviceATCMD(300);
    getRecDataforNum(2,data0);
    tempStr0 = (char *)data0;
    tempStr0 = removeSthString("+QENG: ",tempStr0);
    if(data0 != NULL){
        free(data0);
		data0=NULL;
    }
    getRecDataforNum(3,data1);
    tempStr1 = (char *)data1;
    tempStr1 = removeSthString("+QENG: ",tempStr1);
    if(data1 != NULL){
        free(data1);
		data1=NULL;
    }
    return tempStr0+tempStr1;
}

/*!
 * @return USIM card identification number
 */
String DFRobot_BC20 :: getQCCID(void){
    uint8_t *data=NULL;
    String CCID = "";
    flushBC20Serial();
    sendATCMD(GET_QCCID);
    receviceATCMD(300);
    getRecDataforNum(2,data);
    CCID = removeSthString("+QCCID: ",(char *)data);
    if(data != NULL){
        free(data);
		data=NULL;
    }
    return CCID;
}

/*!
 * @return
 *     0 Power off the module
 */
uint8_t DFRobot_BC20 :: getQPOWD(void){
    uint8_t *data=NULL;
    String tempStr="";
    flushBC20Serial();
    sendATCMD(GET_QPOWD);
    receviceATCMD(1000);
    getRecDataforNum(2,data);
    tempStr  = (char *)data;
    if(data != NULL){
        free(data);
		data=NULL;
    }
    tempStr  = removeSthString("+QPOWD: (",tempStr);
    return GetSthfrontString(")",tempStr).toInt();
}

/*!
 * @param op
 *     0 Power off the module.
 * @return true or false.
 */
bool DFRobot_BC20 :: setQPOWD(uint8_t op){
    flushBC20Serial();
    sendATCMD(SET_QPOWD,op);
    receviceATCMD(1000);
    return CheckRecData(AT_OK);
}

/*!
 * @return
 *     0 Disable Sleep Mode
 *     1 Enable light sleep and deep sleep, wakeup by PSM_EINT (Falling Edge)
 *     2 Enable light sleep only, wakeup by Main UART
 */
uint8_t DFRobot_BC20 :: getQSCLK(void){
    uint8_t *data=NULL;
    String tempStr="";
    flushBC20Serial();
    sendATCMD(GET_QSCLK);
    receviceATCMD(300);
    getRecDataforNum(2,data);
    tempStr  = (char *)data;
    if(data != NULL){
        free(data);
		data=NULL;
    }
    tempStr  = removeSthString("+QSCLK: ",tempStr);
    
    return GetSthfrontString("\r\n",tempStr).toInt();
}

/*!
 * @param mode
 *     0 Disable Sleep Mode.
 *     1 Enable light sleep and deep sleep, wakeup by PSM_EINT (Falling Edge).
 *     2 Enable light sleep only, wakeup by Main UART.
 * @return true or false.
 */
bool DFRobot_BC20 :: setQSCLK(uint8_t mode){
    flushBC20Serial();
    sendATCMD(SET_QSCLK,mode);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

bool DFRobot_BC20 :: setQFOTADL(String url){
    flushBC20Serial();
    sendATCMD((char*)("QFOTADL=\"http://download3.dfrobot.com.cn/nbtest/BC20NAR01A09.bin\""));
    return CheckRecData(AT_OK);
}
uint8_t DFRobot_BC20 :: getQGNSSC(void){
    return getIntforString(GET_QGNSSC,SET_QGNSSC,1);
}

bool DFRobot_BC20 :: setQGNSSC(uint8_t mode){
    flushBC20Serial();
    sendATCMD(SET_QGNSSC,mode);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

static float Longitude_conversion(String str){
    float temp = 0;
    String tempStr = str;
    if(str.length() > 0){
        if((strcmp(str.c_str(),sRMC.LatitudeVal())==0)||(strcmp(str.c_str(),sGGA.LatitudeVal())==0)){
            temp = tempStr.substring(0,2).toInt();
            tempStr = tempStr.substring(2,tempStr.length());
            temp += (tempStr.substring(0,2).toInt())*10.0/60.0/10.0;
            tempStr = tempStr.substring(3,tempStr.length());
            temp += (tempStr.substring(0,4).toInt())*1.0/60.0/10000.0;
        }else if((strcmp(str.c_str(),sRMC.LongitudeVal())==0)||(strcmp(str.c_str(),sGGA.LongitudeVal())==0)){
            temp = tempStr.substring(0,3).toInt();
            tempStr = tempStr.substring(3,tempStr.length());
            temp += (tempStr.substring(0,2).toInt())*10.0/60.0/10.0;
            tempStr = tempStr.substring(3,tempStr.length());
            temp += (tempStr.substring(0,4).toInt())*1.0/60.0/10000.0;
        }else{
            
        }
    }
    return temp;
}

uint8_t DFRobot_BC20 :: getQGNSSRD(void){
	for(int i=0;i<40;i++){
		if(sSAT2.data[i].PRN !=NULL)
		{
			free(sSAT2.data[i].PRN);
			sSAT2.data[i].PRN=NULL;
		}
		if(sSAT2.data[i].Elev !=NULL)
		{
			free(sSAT2.data[i].Elev);
			sSAT2.data[i].Elev=NULL;
		}
		if(sSAT2.data[i].Azim !=NULL)
		{
			free(sSAT2.data[i].Azim);
			sSAT2.data[i].Azim=NULL;
		}
		if(sSAT2.data[i].SNR !=NULL)
		{
			free(sSAT2.data[i].SNR);
			sSAT2.data[i].SNR=NULL;
		}
		if(sSAT2.data[i].Status !=NULL)
		{
			free(sSAT2.data[i].Status);
			sSAT2.data[i].Status=NULL;
		}
		if(sSAT2.data[i].SYS !=NULL)
		{
			free(sSAT2.data[i].SYS);
			sSAT2.data[i].SYS=NULL;
		}		
	}
	uint8_t size; 
    getQGNSSRD(NMEA_RMC);
    sGGNS.LatitudeVal = Longitude_conversion(sRMC.LatitudeVal());
    sGGNS.LatitudeDir = sRMC.LatitudeDir();
    sGGNS.LongitudeVal = Longitude_conversion(sRMC.LongitudeVal());
    sGGNS.LongitudeDir = sRMC.LongitudeDir();
    getQGNSSRD(NMEA_VTG);
    sGGNS.Speed = atof(sVTG.GroundCourse_Kmh());
    sGGNS.Heading = atof(sVTG.GroundCourse_True());	
    getQGNSSRD(NMEA_GGA);
	sGGNS.Altitude=atof(sGGA.Altitude());
	getQGNSSRD(NMEA_GSA);
	  for (uint8_t i = 0; i < 2; i++) {
		for (uint8_t j = 0; j < 12; j++) {
		  sGGNS.Statellite_CH[i][j]=sGSA.data[i].Statellite_CH(j);
		  delay(1);
		}
	  }
    sGGNS.PDOP = sGSA.data[0].PDOP();
    sGGNS.HDOP = sGSA.data[0].HDOP();
    sGGNS.VDOP = sGSA.data[0].VDOP();
    sGGNS.FixStatus = sGSA.data[0].FixStatus();
    getQGNSSRD(NMEA_GSV);
	sSAT2.USE=0;
	for(int i=0;i<sSAT.NUM;i++)
	{
  		//sSAT2.data[i].PRN=sSAT.data[i].PRN();
		size=strlen(sSAT.data[i].PRN());
		sSAT2.data[i].PRN=(char*)malloc(size+1);
		if(sSAT2.data[i].PRN==NULL){
			free(sSAT2.data[i].PRN);
			sSAT2.data[i].PRN=NULL;
			return 0;
		}
		memset(sSAT2.data[i].PRN,'\0',size+1);
		memcpy(sSAT2.data[i].PRN,sSAT.data[i].PRN(),size);
		
		
 		//sSAT2.data[i].Elev=sSAT.data[i].Elev();
		size=strlen(sSAT.data[i].Elev());
		sSAT2.data[i].Elev=(char*)malloc(size+1);
		if(sSAT2.data[i].Elev==NULL){
			free(sSAT2.data[i].Elev);
			sSAT2.data[i].Elev=NULL;
			return 0;
		}
		memset(sSAT2.data[i].Elev,'\0',size+1);
		memcpy(sSAT2.data[i].Elev,sSAT.data[i].Elev(),size);

		
		//sSAT2.data[i].Azim=sSAT.data[i].Azim();
		size=strlen(sSAT.data[i].Azim());
		sSAT2.data[i].Azim=(char*)malloc(size+1);
		if(sSAT2.data[i].Azim==NULL){
			free(sSAT2.data[i].Azim);
			sSAT2.data[i].Azim=NULL;
			return 0;
		}
		memset(sSAT2.data[i].Azim,'\0',size+1);
		memcpy(sSAT2.data[i].Azim,sSAT.data[i].Azim(),size);

		
		//sSAT2.data[i].SNR=sSAT.data[i].SNR();
		size=strlen(sSAT.data[i].SNR());
		sSAT2.data[i].SNR=(char*)malloc(size+1);
		if(sSAT2.data[i].SNR==NULL){
			free(sSAT2.data[i].SNR);
			sSAT2.data[i].SNR=NULL;
			return 0;
		}
		memset(sSAT2.data[i].SNR,'\0',size+1);
		memcpy(sSAT2.data[i].SNR,sSAT.data[i].SNR(),size);

		
		//sSAT2.data[i].SYS=sSAT.data[i].SYS();
		size=strlen(sSAT.data[i].SYS());
		sSAT2.data[i].SYS=(char*)malloc(size+1);
		if(sSAT2.data[i].SYS==NULL){
			free(sSAT2.data[i].SYS);
			sSAT2.data[i].SYS=NULL;
			return 0;
		}
		memset(sSAT2.data[i].SYS,'\0',size+1);
		memcpy(sSAT2.data[i].SYS,sSAT.data[i].SYS(),size);

		
		//sSAT2.data[i].Status=sSAT.data[i].Status();
		size=strlen(sSAT.data[i].Status());
		sSAT2.data[i].Status=(char*)malloc(size+1);
		if(sSAT2.data[i].Status==NULL){
			free(sSAT2.data[i].Status);
			sSAT2.data[i].Status=NULL;
			return 0;
		}
		memset(sSAT2.data[i].Status,'\0',size+1);
		memcpy(sSAT2.data[i].Status,sSAT.data[i].Status(),size);

		
		if(strcmp(sSAT2.data[i].Status,"Y")==0)
			(sSAT2.USE)++;
		if(ret!=NULL)
		{
			free(ret);
			ret=NULL;
		}
	}	
    return 1;
}

uint8_t DFRobot_BC20 :: getQGNSSRD2(void){
	char* timeStr;
	char tempdata[5]={'0'};
    getQGNSSRD(NMEA_GGA);
	sGGNS2.LatitudeVal = Longitude_conversion(sGGA.LatitudeVal());
	sGGNS2.LongitudeVal = Longitude_conversion(sGGA.LongitudeVal());
	sGGNS2.Altitude=atof(sGGA.Altitude());
	sGGNS2.StatelliteNum=atoi(sGGA.StatelliteNum());
	sGGNS2.HDOP=atof(sGGA.HDOP());
	
	timeStr = sGGA.UTC_Time();
	substring(tempdata,timeStr,0,2);
	sCLK.Hour=atoi(tempdata);
	substring(timeStr,timeStr,2,strlen(timeStr));
	substring(tempdata,timeStr,0,2);
	sCLK.Minute=atoi(tempdata);				
	substring(timeStr,timeStr,2,strlen(timeStr));
	substring(tempdata,timeStr,0,2);
	sCLK.Second=atoi(tempdata);	
    return 1;
}

static void CheckSatelliteUse(uint8_t num){
    uint8_t temp = 0;
	char* tempdata;
	char*d;
	int addr;
	if(strcmp(sSAT.data[num].SYS(),"GPS")==0){	
        temp = 0;
	}else if(strcmp(sSAT.data[num].SYS(),"BeiDou")==0){			
        temp = 1;
    }else{
        
    }		
    for(uint8_t i =0; i < 2; i++){
		for(uint8_t j =0; j < 12; j++){
			if(strcmp(sSAT.data[num].PRN(),(sGGNS.Statellite_CH[i][j]).c_str())==0){
				sSAT.data[num].NUM=num;
				addr=(sSAT.data[num].NUM)*21+3+3+3+3;
				tempdata="Y";
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }            
				return;
			}
		}
    }
	sSAT.data[num].NUM=num;
	addr=(sSAT.data[num].NUM)*21+3+3+3+3;
	tempdata="N";
	d=tempdata;
	for(int j=0;j<strlen(d);j++){
	  EEPROM.write(addr+j,d[j]);
	  }  	
}
void DFRobot_BC20::getSatelliteInformation(uint8_t start, uint8_t num, char* str, char* sys){//Given a satellite data, which satellite does it start with, how many satellites do you have   
	char* tempStr = str;
	int addr;
	char*d;
	char* tempdata;
    for(uint8_t i = 0; i < num;i++){
        if(strlen(GetSthfrontString(",",tempStr)) == 0){
			sSAT.data[i+start].NUM=i+start;
			addr=(sSAT.data[i+start].NUM)*21;
			d="N/A";
		    for(int j=0;j<strlen(d);j++){
			  EEPROM.write(addr+j,d[j]);
			  }	
        }else{
			sSAT.data[i+start].NUM=i+start;
			addr=(sSAT.data[i+start].NUM)*21;
			tempdata=GetSthfrontString(",",tempStr);
			d=tempdata;
		    for(int j=0;j<strlen(d);j++){
			  EEPROM.write(addr+j,d[j]);
			  }				
            //sSAT.data[i+start].PRN = GetSthfrontString(",",tempStr);//Satellite PRN code number i+start
        }
        tempStr = removeSthString(",",tempStr);
        if(strlen(GetSthfrontString(",",tempStr))== 0){
			sSAT.data[i+start].NUM=i+start;
			addr=(sSAT.data[i+start].NUM)*21+3;
			d="N/A";
		    for(int j=0;j<strlen(d);j++){
			  EEPROM.write(addr+j,d[j]);
			  }				
            //sSAT.data[i+start].Elev = "N/A";
        }else{
			sSAT.data[i+start].NUM=i+start;
			addr=(sSAT.data[i+start].NUM)*21+3;
			tempdata=GetSthfrontString(",",tempStr);
			d=tempdata;
		    for(int j=0;j<strlen(d);j++){
			  EEPROM.write(addr+j,d[j]);
			  }				
            //sSAT.data[i+start].Elev = GetSthfrontString(",",tempStr);//The satellite elevation Angle i+start
        }		
        tempStr = removeSthString(",",tempStr);
        if(strlen(GetSthfrontString(",",tempStr)) == 0){
			sSAT.data[i+start].NUM=i+start;
			addr=(sSAT.data[i+start].NUM)*21+3+3;
			d="N/A";
		    for(int j=0;j<strlen(d);j++){
			  EEPROM.write(addr+j,d[j]);
			  }				
            //sSAT.data[i+start].Azim = "N/A";
        }else{
			sSAT.data[i+start].NUM=i+start;
			addr=(sSAT.data[i+start].NUM)*21+3+3;
			tempdata=GetSthfrontString(",",tempStr);
			d=tempdata;
		    for(int j=0;j<strlen(d);j++){
			  EEPROM.write(addr+j,d[j]);
			  }			  
            //sSAT.data[i+start].Azim = GetSthfrontString(",",tempStr);//Azimuth of satellite i+start
        }
        tempStr = removeSthString(",",tempStr);
        if((strlen(GetSthfrontString(",",tempStr))== 0)||(strlen(GetSthfrontString(",",tempStr))>2)){
			sSAT.data[i+start].NUM=i+start;
			addr=(sSAT.data[i+start].NUM)*21+3+3+3;
			d="N/A";
		    for(int j=0;j<strlen(d);j++){
			  EEPROM.write(addr+j,d[j]);
			  }			
            //sSAT.data[i+start].SNR = "N/A";
        }else{
			sSAT.data[i+start].NUM=i+start;
			addr=(sSAT.data[i+start].NUM)*21+3+3+3;
			tempdata=GetSthfrontString(",",tempStr);
			d=tempdata;
		    for(int j=0;j<strlen(d);j++){
			  EEPROM.write(addr+j,d[j]);
			  }				
            //sSAT.data[i+start].SNR = GetSthfrontString(",",tempStr);//SNR i+start
        }	
		sSAT.data[i+start].NUM=i+start;
		addr=(sSAT.data[i+start].NUM)*21+3+3+3+3+3;
		tempdata=sys;
		d=tempdata;
		for(int j=0;j<strlen(d);j++){
		  EEPROM.write(addr+j,d[j]);
		  }			
        //sSAT.data[i+start].SYS = sys;
        tempStr = removeSthString(",",tempStr);
        CheckSatelliteUse(i+start);
    }
}

static void CheckUse(){//Calculate the amount in use
    sSAT.USE = 0x00;
    for(uint8_t i =0; i < sGSA.NUM;i++){
        for(uint8_t j =0; j < 12; j++){
            //if((sGSA.data[i].Statellite_CH(j).length()> 0) &&(sGSA.data[i].Statellite_CH(j).equals("N/A"))){
			if((strlen(sGSA.data[i].Statellite_CH(j))> 0) &&(strcmp(sGSA.data[i].Statellite_CH(j),"N/A")==0)){
                sSAT.USE++;
            }
        }
    }
}

bool DFRobot_BC20 :: getQGNSSRD(char* cmd){
#ifdef ARDUINO_ESP32_DEV	
	EEPROM.begin(1024);
#endif	
    for(int i=0;i<1024;i++)
    EEPROM.write(i,'\0');
	if(available())
	{
		readData();
	}
    uint8_t *data=NULL;
	char* timeStr;
    uint8_t NumSAT=0;
	uint8_t NumSEN=0;
	uint8_t StartNum=0;
	uint8_t lastTotSEN=0;
	uint8_t TotSEN = 0;
    uint8_t readnum = 2;
	char* tempdata=NULL;
	char*d=NULL;
	char* t_addr=NULL;
	int addr;
    char* tempStr=NULL;	
    flushBC20Serial();
    sendATCMD(SET_QGNSSRD,cmd);
    receviceATCMD(5000); 	
    if(strcmp(cmd,NMEA_GSV)==0){
       StartNum = 0x00;
        lastTotSEN = 0x00;
        sSAT.NUM = 0x00;
        for(uint8_t i = 0; i < 10; i++){
            getRecDataforNum_NoCheck(readnum+i,data);
			if(strlen((char*)data)<5)
			{
				if(ret1!=NULL)
				{
					free(ret1);
					ret1=NULL;				
				}			
				free(data);
				data=NULL;
				return false;
			}				
			t_addr =(char*)malloc(strlen((char*)data)+1);
			if(t_addr==NULL)
			{
                free(data);
                data = NULL;				
				free(t_addr);
				return false;
			}
			memset(t_addr,'\0',strlen((char*)data)+1);
			memcpy(t_addr,(char*)data,strlen((char*)data));
			tempStr = t_addr;			
            if(data != NULL){
                free(data);
                data = NULL;
            }else{
				if(ret1!=NULL)
				{
					free(ret1);
					ret1=NULL;				
				}				
				free(t_addr);
				t_addr=NULL;
				free(data);
                data = NULL;
                return false;
            } 		
            if((readnum + i) == 2){
                tempStr  = removeSthString("+QGNSSRD: ",tempStr);
            }
 			char* tempSYS;
            if(indexOf(tempStr,"$GPGSV,") != -1){
                tempSYS = "GPS";
            }else if(indexOf(tempStr,"$GBGSV,") != -1){
                tempSYS = "BeiDou";
            }else{
                tempSYS = "N/A";
            } 			
            tempStr = removeSthString(",",tempStr);			
            //TotSEN = atoi(GetSthfrontString(",",tempStr));//语句的总数目
			tempdata=GetSthfrontString(",",tempStr);
			TotSEN = atoi(tempdata);			
            tempStr = removeSthString(",",tempStr);				
            //NumSEN = atoi(GetSthfrontString(",",tempStr));//当前语句序号
			tempdata=GetSthfrontString(",",tempStr);
			NumSEN = atoi(tempdata); 			
			tempStr = removeSthString(",",tempStr);
            //NumSAT = atoi(GetSthfrontString(",",tempStr));//可见卫星数
			tempdata=GetSthfrontString(",",tempStr);
			NumSAT = atoi(tempdata);			
            tempStr = removeSthString(",",tempStr); 			       
			if(TotSEN > 0){
                if(NumSEN == 1){
                    sSAT.NUM += NumSAT;
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_MEGA2560)
					if(sSAT.NUM>11)
					sSAT.NUM=11;
					if(Flag==1)
					{
						sSAT.NUM=6;
					}	
#else 
	          if(sSAT.NUM>17)    
			  sSAT.NUM=17;
#endif					
                }
                if(NumSAT /(NumSEN*4) > 0){
                    getSatelliteInformation(StartNum,4,tempStr,tempSYS);
                    StartNum += 4;
                }else{
                    getSatelliteInformation(StartNum,(NumSAT-((NumSEN-1)*4)),tempStr,tempSYS);
                    StartNum += (NumSAT-((NumSEN-1)*4));
                }
            }
			if(ret1!=NULL)
			{
				free(ret1);
				ret1=NULL;				
			}			
			free(t_addr);
			t_addr=NULL;			
		}
        return true;
    }else{		
        getRecDataforNum(2,data);		
		t_addr =(char*)malloc(strlen((char*)data)+1);
		memset(t_addr,'\0',strlen((char*)data)+1);
		memcpy(t_addr,(char*)data,strlen((char*)data));        
		if(data != NULL){			
            free(data);
            data = NULL;
		}
		tempStr=t_addr;
		if(strlen(tempStr)<30)
		{
			free(tempStr);
			tempStr=NULL;
			return 0;
		}
		tempStr  = removeSthString("+QGNSSRD:",tempStr);
		switch(getNum(cmd)){			
            case 0://NMEA_RMC
                tempStr = removeSthString("$GNRMC,",tempStr);
                //sRMC.UTC_Time = GetSthfrontString(",",tempStr);
				addr=0;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }				
                tempStr = removeSthString(",",tempStr);
                //sRMC.DataStatus = GetSthfrontString(",",tempStr);
				addr=0+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }				
                tempStr = removeSthString(",",tempStr);
                //sRMC.LatitudeVal = GetSthfrontString(",",tempStr);
				addr=0+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }				
                tempStr = removeSthString(",",tempStr);
                //sRMC.LatitudeDir = GetSthfrontString(",",tempStr);
				addr=0+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }				
                tempStr = removeSthString(",",tempStr);
                //sRMC.LongitudeVal = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }				
                tempStr = removeSthString(",",tempStr);
                //sRMC.LongitudeDir = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }				
                tempStr = removeSthString(",",tempStr);
                //sRMC.GroundSpeed = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }				
                tempStr = removeSthString(",",tempStr);
                //sRMC.GroundHeading = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }				
                tempStr = removeSthString(",",tempStr);
                //sRMC.UTC_Date = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }				
                tempStr = removeSthString(",",tempStr);
                //sRMC.MagDeclination = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11+11+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }				
                tempStr = removeSthString(",",tempStr);
                //sRMC.MagDeclinationDir = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11+11+11+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }				
                tempStr = removeSthString(",",tempStr);
                //sRMC.PositioningMode = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11+11+11+11+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }				
                tempStr = removeSthString(",",tempStr);
                //sRMC.NaviStatus = GetSthfrontString("*",tempStr);
				addr=0+11+11+11+11+11+11+11+11+11+11+11+11;
				tempdata=GetSthfrontString("*",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }	
								  
                timeStr = sRMC.UTC_Time();
				substring(tempdata,timeStr,0,2);
				sCLK.Hour=atoi(tempdata);				
				substring(timeStr,timeStr,2,strlen(timeStr));
				substring(tempdata,timeStr,0,2);
				sCLK.Minute=atoi(tempdata);				
				substring(timeStr,timeStr,2,strlen(timeStr));
				substring(tempdata,timeStr,0,2);
				sCLK.Second=atoi(tempdata);
				timeStr = sRMC.UTC_Date();
				substring(tempdata,timeStr,0,2);
				sCLK.Day=atoi(tempdata);				
				substring(timeStr,timeStr,2,strlen(timeStr));
				substring(tempdata,timeStr,0,2);
				sCLK.Month=atoi(tempdata);				
				substring(timeStr,timeStr,2,strlen(timeStr));
				substring(tempdata,timeStr,0,2);
				sCLK.Year=atoi(tempdata)+2000;
				//sCLK.Mode = "UTC+0";				
                break;				
            case 1:/*NMEA_VTG*/
                tempStr = removeSthString("$GNVTG,",tempStr);
                //sVTG.GroundCourse_True = GetSthfrontString(",",tempStr);						
				addr=0;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  } 				
                tempStr = removeSthString("T,",tempStr);               
                //sVTG.GroundCourse_Mag = GetSthfrontString(",",tempStr);
				addr=0+5;
				tempdata=GetSthfrontString(",",tempStr);
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  } 				
                tempStr = removeSthString("M,",tempStr);
                //sVTG.GroundCourse_Knots = GetSthfrontString(",",tempStr);
				addr=0+5+5;
				tempdata=GetSthfrontString(",",tempStr);
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  } 				
                tempStr = removeSthString("N,",tempStr);
                //sVTG.GroundCourse_Kmh = GetSthfrontString(",",tempStr);
				addr=0+5+5+5;
				tempdata=GetSthfrontString(",",tempStr);
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  } 				
                tempStr = removeSthString("K,",tempStr);
                //sVTG.PositioningMode = GetSthfrontString("*",tempStr);
				addr=0+5+5+5+5;
				tempdata=GetSthfrontString("*",tempStr);
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  } 				
                break;
            case 2:/*NMEA_GGA*//*待定*/			
                tempStr = removeSthString("$GNGGA,",tempStr);				
                //sGGA.UTC_Time= GetSthfrontString(",",tempStr);
				addr=0;
				tempdata=GetSthfrontString(",",tempStr);			
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }					
                tempStr = removeSthString(",",tempStr);
                //sGGA.LatitudeVal = GetSthfrontString(",",tempStr);
				addr=0+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }					
                tempStr = removeSthString(",",tempStr);
                //sGGA.LatitudeDir = GetSthfrontString(",",tempStr);
				addr=0+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }					
                tempStr = removeSthString(",",tempStr);
                //sGGA.LongitudeVal = GetSthfrontString(",",tempStr);
				addr=0+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }					
                tempStr = removeSthString(",",tempStr);
                //sGGA.LongitudeDir = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }					
                tempStr = removeSthString(",",tempStr);
                //sGGA.Status = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }					
                tempStr = removeSthString(",",tempStr);
                //sGGA.StatelliteNum = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }					
                tempStr = removeSthString(",",tempStr);
                //sGGA.HDOP = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }					
                tempStr = removeSthString(",",tempStr);
                //sGGA.Altitude = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }					
                tempStr = removeSthString("M,",tempStr);
                //sGGA.GeoID_Separation = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11+11+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }					
                tempStr = removeSthString("M,",tempStr);
                //sGGA.DGPS_Age = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11+11+11+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }					
                tempStr = removeSthString(",",tempStr);
                //sGGA.DGPS_StationID = GetSthfrontString("*",tempStr);
				addr=0+11+11+11+11+11+11+11+11+11+11+11;
				tempdata=GetSthfrontString("*",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }	
				//EEPROM.commit();	
                break; 
            case 3:/*NMEA_GSA*/			
                sGSA.NUM = 0;				
                for(int i = 0; i < 2; i++){					
                    if(indexOf(tempStr,"$GNGSA") != -1){						
                        sGSA.NUM = i+1;
                        tempStr = removeSthString(",",tempStr);
						sGSA.data[i].NUM=i;
						addr=(sGSA.data[i].NUM)*72;
						tempdata=GetSthfrontString(",",tempStr);						
						d=tempdata;
						for(int j=0;j<strlen(d);j++){
						  EEPROM.write(addr+j,d[j]);
						  } 						  
                        //sGSA.data[i].Mode = GetSthfrontString(",",tempStr);
                        tempStr = removeSthString(",",tempStr);							
						addr=(sGSA.data[i].NUM)*72+4;
						tempdata=GetSthfrontString(",",tempStr);
						d=tempdata;
						for(int j=0;j<strlen(d);j++){
						  EEPROM.write(addr+j,d[j]);
						  } 						
                        //sGSA.data[i].FixStatus = GetSthfrontString(",",tempStr);
                        tempStr = removeSthString(",",tempStr);
                        for(int j = 0; j < 12; j++){
						addr=(sGSA.data[i].NUM)*72+4+4+j*4;
						tempdata=GetSthfrontString(",",tempStr);
						d=tempdata;
						for(int k=0;k<strlen(d);k++){
						  EEPROM.write(addr+k,d[k]);
						  } 								
                            //sGSA.data[i].Statellite_CH[j] = GetSthfrontString(",",tempStr);
                            tempStr = removeSthString(",",tempStr);
                        }						
						addr=(sGSA.data[i].NUM)*72+4+4+4*12;
						tempdata=GetSthfrontString(",",tempStr);
						d=tempdata;
						for(int j=0;j<strlen(d);j++){
						  EEPROM.write(addr+j,d[j]);
						  }						
                        //sGSA.data[i].PDOP = GetSthfrontString(",",tempStr);
                        tempStr = removeSthString(",",tempStr);
						addr=(sGSA.data[i].NUM)*72+4+4+4*12+4;
						tempdata=GetSthfrontString(",",tempStr);
						d=tempdata;
						for(int j=0;j<strlen(d);j++){
						  EEPROM.write(addr+j,d[j]);
						  }							
                        //sGSA.data[i].HDOP = GetSthfrontString(",",tempStr);
                        tempStr = removeSthString(",",tempStr);
						addr=(sGSA.data[i].NUM)*72+4+4+4*12+4+4;
						tempdata=GetSthfrontString(",",tempStr);
						d=tempdata;
						for(int j=0;j<strlen(d);j++){
						  EEPROM.write(addr+j,d[j]);
						  }							
                        //sGSA.data[i].VDOP = GetSthfrontString("*",tempStr);
                        tempStr = removeSthString(",",tempStr);
						addr=(sGSA.data[i].NUM)*72+4+4+4*12+4+4+4;
						tempdata=GetSthfrontString("*",tempStr);
						d=tempdata;
						EEPROM.write(addr,d[0]);
                        //sGSA.data[i].GNSS_SystemID = GetSthfrontString("*",tempStr);
                        for(int j = 0; j < 12; j++){
                            if(strlen(sGSA.data[i].Statellite_CH(j)) == 0){
								addr=(sGSA.data[i].NUM)*72+4+4+j*4;
								d="N/A";
								for(int j=0;j<strlen(d);j++){
								  EEPROM.write(addr+j,d[j]);
								  }								
                                //sGSA.data[i].Statellite_CH(j) = "N/A";
                            }
                        }						
                    }					
					free(t_addr);
					t_addr=NULL;				
 				    getRecDataforNum(3+i,data);						
					if(data != NULL){
						t_addr =(char*)malloc(strlen((char*)data)+1);						
						if(t_addr==NULL)
						{
							free(t_addr);
							return false;
						}
						memset(t_addr,'\0',strlen((char*)data)+1);
						memcpy(t_addr,data,strlen((char*)data));
						tempStr=t_addr;						
						free(data);
						data = NULL;
					}else{
						return false;
					}
                }				
                CheckUse();
				if(t_addr!=NULL)
				{
					free(t_addr);
					return false;
				}				
                break;
            case 4://NMEA_GSV
            case 5:/* NMEA_GLL */			
                tempStr = removeSthString("$GNGLL,",tempStr);
                //sGLL.LatitudeVal = GetSthfrontString(",",tempStr);
				addr=0;				
				tempdata=GetSthfrontString(",",tempStr);					
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }					  
                tempStr = removeSthString(",",tempStr);				
                //sGLL.LatitudeDir = GetSthfrontString(",",tempStr);
				addr=0+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }				
                tempStr = removeSthString(",",tempStr);				
                //sGLL.LongitudeVal = GetSthfrontString(",",tempStr);
				addr=0+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }							
                tempStr = removeSthString(",",tempStr);
                //sGLL.LongitudeDir = GetSthfrontString(",",tempStr);
				addr=0+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }							
                tempStr = removeSthString(",",tempStr);				
                //sGLL.UTC_Time = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }								
                tempStr = removeSthString(",",tempStr);
                //sGLL.DataStatus = GetSthfrontString(",",tempStr);
				addr=0+11+11+11+11+11;
				tempdata=GetSthfrontString(",",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }							
                tempStr = removeSthString(",",tempStr);
                //sGLL.PositionMode = GetSthfrontString("*",tempStr);
				addr=0+11+11+11+11+11+11;
				tempdata=GetSthfrontString("*",tempStr);				
				d=tempdata;
				for(int j=0;j<strlen(d);j++){
				  EEPROM.write(addr+j,d[j]);
				  }					  
                break;
            default:
                break;				
        }		
	free(t_addr);
	t_addr=NULL;
    }
	flushBC20Serial();
    return 1;
}


void DFRobot_BC20 :: getGPS(void){
    sendATCMD("Hello World!");
}


bool DFRobot_BC20 ::setAliyunserver(char*ProductKey,char* IPAddress,char* port,char connectID){
	char*IPaddress=(char*)malloc(strlen(ProductKey)+strlen(IPAddress)+1);
	if(IPaddress==NULL){
		free(IPaddress);
		IPaddress=NULL;
		return false;
	}
	memset(IPaddress,'\0',strlen(ProductKey)+strlen(IPAddress)+1);
	memcpy(IPaddress,ProductKey,strlen(ProductKey));
	memcpy(IPaddress+strlen(ProductKey),IPAddress,strlen(IPAddress));
    while(false==openMQTT(connectID,IPaddress,port)){
        closeMQTT(0);
        delay(1000);
    }
	free(IPaddress);
	IPaddress=NULL;
    return true;	
}

bool DFRobot_BC20 ::connect_Aliyun(char* ProductKey,char* DeviceName,char* DeviceSecret){
	uint8_t* data=NULL;
	uint8_t timeout=0;
	String tempStr;
	uint8_t ProductKey_len=strlen(ProductKey);
	uint8_t DeviceName_len=strlen(DeviceName);
	uint8_t DeviceSecret_len=strlen(DeviceSecret);
	char* tempdata=(char*)malloc(13+ProductKey_len+DeviceName_len+DeviceSecret_len+15);
	if(tempdata==NULL){
		free(tempdata);
		tempdata=NULL;
		return false;
	}
	memset(tempdata,'\0',6+ProductKey_len+DeviceName_len+DeviceSecret_len+15);
	memcpy(tempdata,GET_QMTCFG,6);
	memcpy(tempdata+6,"=\"",2);
	memcpy(tempdata+8,"ALIAUTH",7);
	memcpy(tempdata+8+7,"\",0,\"",5);
	memcpy(tempdata+8+7+5,ProductKey,ProductKey_len);
	ProductKey=NULL;
	memcpy(tempdata+8+7+5+ProductKey_len,"\",\"",3);
	memcpy(tempdata+8+7+5+ProductKey_len+3,DeviceName,DeviceName_len);
	memcpy(tempdata+8+7+5+ProductKey_len+3+DeviceName_len,"\",\"",3);
	memcpy(tempdata+8+7+5+ProductKey_len+3+DeviceName_len+3,DeviceSecret,DeviceSecret_len);
	DeviceSecret=NULL;
	memcpy(tempdata+8+7+5+ProductKey_len+3+DeviceName_len+3+DeviceSecret_len,"\"",1);
	flushBC20Serial();
	sendATCMD((char*)tempdata);
	free(tempdata);
	tempdata=NULL;
	receviceATCMD(300);
	if(!CheckRecData(AT_OK))
	{
		return false;
	}
	tempdata=(char*)malloc(13+DeviceName_len);
	if(tempdata==NULL){
		free(tempdata);
		tempdata=NULL;
		return false;
	}
	memset(tempdata,'\0',13+DeviceName_len);
	memcpy(tempdata,SET_QMTCONN,7);
	memcpy(tempdata+7,"=0,\"",4);
	memcpy(tempdata+11,DeviceName,DeviceName_len);
	DeviceSecret=NULL;
	memcpy(tempdata+11+DeviceName_len,"\"",1);
	flushBC20Serial();
	sendATCMD((char*)tempdata);
	free(tempdata);
	tempdata=NULL;
	receviceATCMD(5000);
   if(CheckRecData(AT_OK)){
		while(1)
		{
			receviceATCMD(300);
			timeout++;
			if(timeout>250)
			{
				return false;
			}			
			getRecDataforNum_NoCheck(1,data);
			tempStr  = (char *)data;
			if(tempStr.length()>0){
				if(data != NULL){
					free(data);
					data = NULL;
				}
				if(tempStr.equals("+QMTCONN: 0,0,0\r\n")){
					this->mqttConneced = true;
					delay(1000);
					return true;
				}
			}
		}
    } 
    return false;
}



void DFRobot_BC20 :: configMQTT(void){
    flushBC20Serial();
}

bool DFRobot_BC20::setMQTTVersion(uint8_t version){
    flushBC20Serial();
    sendATCMD((String)GET_QMTCFG+"="+"\"version\",0,"+version);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

void DFRobot_BC20 :: getMQTTParameter(void){
    flushBC20Serial();
    sendATCMD(TEST_QMTCFG);
    receviceATCMD(300);
    
}

bool DFRobot_BC20 :: openMQTT(char connectID, char* Server, char* Port){
    uint8_t *data=NULL;
	char * tempChar=NULL;
    String tempStr="";
    uint8_t temptime = 0;
	uint8_t ServerLen = strlen(Server);
	uint8_t ALLlen=strlen(SET_QMTOPEN)+4+ServerLen+2+strlen(Port);
	tempChar = (char*)malloc(ALLlen+1);
	if(tempChar == NULL){
        free(tempChar);
        return 0;
    }
 	memset(tempChar,'\0',ALLlen+1);
	memcpy(tempChar,SET_QMTOPEN,strlen(SET_QMTOPEN));
 	memcpy(tempChar+strlen(SET_QMTOPEN),"=",1);
	tempChar[strlen(SET_QMTOPEN)+1] = connectID;
	memcpy(tempChar+strlen(SET_QMTOPEN)+2,",\"",2);
	memcpy(tempChar+strlen(SET_QMTOPEN)+4,Server,ServerLen);
	memcpy(tempChar+strlen(SET_QMTOPEN)+4+ServerLen,"\",",2);
	memcpy(tempChar+strlen(SET_QMTOPEN)+4+ServerLen+2,Port,strlen(Port)); 
    flushBC20Serial();
	sendATCMD(tempChar);
	free(tempChar);
	tempChar=NULL;
	receviceATCMD(5000);	 
    if(CheckRecData(AT_OK)){
         while(1){
			receviceATCMD(100);
            if(temptime > 250){
                return false;
            }
            temptime++;
            getRecDataforNum_NoCheck(1,data);			
            CheckRecData("");
            tempStr  = (char *)data;
            if(data != NULL){
                free(data);
                data = NULL;
            }
            if(tempStr.equals("+QMTOPEN: 0,0\r\n")){
                return true;
            }
        } 
    }
    return false; 
}

bool DFRobot_BC20 :: readMQTT(void){
    flushBC20Serial();
    sendATCMD(GET_QMTOPEN);
    receviceATCMD(75000);
    return CheckRecData(AT_OK);
}

bool DFRobot_BC20 :: checkMQTT(uint8_t connectID){
    uint8_t *data=NULL;
    String tempStr="";
    flushBC20Serial();
    sendATCMD(GET_QMTOPEN);
    receviceATCMD(75000);
    getRecDataforNum(2,data);
    tempStr  = (char *)data;
    if(data != NULL){
        free(data);
        data = NULL;
    }
    if(tempStr.length() < 5){
        receviceATCMD(75000);
        getRecDataforNum(2,data);
        tempStr  = (char *)data;
        tempStr  = removeSthString("+QMTOPEN: ",tempStr);
        
    }
    return false;
}

bool DFRobot_BC20 :: closeMQTT(uint8_t connectID){
    flushBC20Serial();
    sendATCMD(SET_QMTCLOSE,connectID);
    receviceATCMD(500);
    return CheckRecData(AT_OK);
}
bool DFRobot_BC20::connected(){
    return this->mqttConneced;
}
bool DFRobot_BC20 :: connectServer(char connectID, char* clientID, char* UserName, char* PassWord){	
  	uint8_t * data=NULL;
	uint8_t timeout=0;
    String tempStr;
	char * tempChar=NULL;
	uint8_t clientID_len=strlen(clientID);
	uint8_t UserName_len=strlen(UserName);
	uint8_t PassWord_len=strlen(PassWord);  
	tempChar = (char*)malloc(strlen(SET_QMTCONN)+1+1+2+clientID_len+2+1+UserName_len-1+2+1+PassWord_len+2);
	if(tempChar == NULL){
        free(tempChar);
        return false;
    }
	memset(tempChar,'\0',strlen(SET_QMTCONN)+1+1+2+clientID_len+2+1+UserName_len-1+2+1+PassWord_len+2);
	memcpy(tempChar,SET_QMTCONN,strlen(SET_QMTCONN));
	memcpy(tempChar+strlen(SET_QMTOPEN),"=",1);
	tempChar[strlen(SET_QMTOPEN)+1] = connectID;
 	memcpy(tempChar+strlen(SET_QMTCONN)+1+1,",\"",2);
	memcpy(tempChar+strlen(SET_QMTCONN)+1+1+2,clientID,clientID_len);
	memcpy(tempChar+strlen(SET_QMTCONN)+1+1+2+clientID_len,"\",",2);
	memcpy(tempChar+strlen(SET_QMTCONN)+1+1+2+clientID_len+2,"\"",1);
	memcpy(tempChar+strlen(SET_QMTCONN)+1+1+2+clientID_len+2+1,UserName,UserName_len);
	memcpy(tempChar+strlen(SET_QMTCONN)+1+1+2+clientID_len+2+1+UserName_len,"\",",2);
	memcpy(tempChar+strlen(SET_QMTCONN)+1+1+2+clientID_len+2+1+UserName_len+2,"\"",1);
	memcpy(tempChar+strlen(SET_QMTCONN)+1+1+2+clientID_len+2+1+UserName_len+2+1,PassWord,PassWord_len);
	memcpy(tempChar+strlen(SET_QMTCONN)+1+1+2+clientID_len+2+1+UserName_len+2+1+PassWord_len,"\"",1); 
	flushBC20Serial();
	sendATCMD(tempChar);
	free(tempChar);
	tempChar=NULL;
    receviceATCMD(5000);
   if(CheckRecData(AT_OK)){
		while(1)
		{
			receviceATCMD(300);
			timeout++;
			if(timeout>250)
			{
				return false;
			}			
			getRecDataforNum_NoCheck(1,data);
			CheckRecData("");			
			tempStr  = (char *)data;
			if(tempStr.length()>0)
			if(data != NULL){
				free(data);
				data = NULL;
			}
			if(tempStr.equals("+QMTCONN: 0,0,0\r\n")){
				this->mqttConneced = true;
				delay(1000);
				return true;
			}
		}
    } 
    return false;
}

bool DFRobot_BC20 :: connect(char * clientID, char * username,char * password,char connectID){
	return connectServer(connectID,clientID,username,password);
}

bool DFRobot_BC20 :: setServer(char* IPAddress, char* port, char connectID){
    while(false==openMQTT(connectID,IPAddress,port)){
        closeMQTT(0);
        delay(1000);
    }
    return true;
}

bool DFRobot_BC20 :: getQMTCONN(){
	uint8_t* data=NULL;
	String tempstr;
    flushBC20Serial();
    sendATCMD(GET_QMTCONN);
    receviceATCMD(5000);
 	getRecDataforNum(2,data);
	if(data!=NULL)
	{
		tempstr=(char*)data;		
		free(data);
		data=NULL;
		return 0;
	}
	if(tempstr.equals("+QMTCONN: 0,3\r\n")){
		return true;
	}else{
		return false;
	} 		
    //return CheckRecData(AT_OK);
}

bool DFRobot_BC20 :: disConnectServer(String connectID){
    flushBC20Serial();
    sendATCMD(SET_QMTDISC,connectID);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

bool DFRobot_BC20 :: subTopic(char connectID, char msgID, char* topic, char qos){
	char* tempdata=NULL;
	uint8_t* data=NULL;
	String tempStr;
	uint8_t timeout=0;
	uint8_t topic_len=strlen(topic);
	uint8_t cmd_len=strlen(SET_QMTSUB);
	uint8_t ALL_len=cmd_len+1+1+1+1+2+topic_len+2+1;
	tempdata=(char*)malloc(ALL_len+1);
	if(tempdata==NULL)
	{
		free(tempdata);
		return false;
	}
	memset(tempdata,'\0',ALL_len+1);	
	memcpy(tempdata,SET_QMTSUB,cmd_len);
	memcpy(tempdata+cmd_len,"=",1);
	tempdata[cmd_len+1] = connectID;
	memcpy(tempdata+cmd_len+1+1,",",1);
	tempdata[cmd_len+1+1+1] = msgID;
	memcpy(tempdata+cmd_len+1+1+1+1,",\"",2);
	memcpy(tempdata+cmd_len+1+1+1+1+2,topic,topic_len);
	memcpy(tempdata+cmd_len+1+1+1+1+2+topic_len,"\",",2);
	tempdata[cmd_len+1+1+1+1+2+topic_len+2] = qos;	
    flushBC20Serial();
	sendATCMD((char*)tempdata);
	free(tempdata);
	tempdata=NULL;
    receviceATCMD(10000);
    if(CheckRecData(AT_OK)){
		while(1)
		{
			receviceATCMD(300);
			timeout++;
			if(timeout>250)
			{
				return false;
			}			
			getRecDataforNum_NoCheck(1,data);
			CheckRecData("");			
			tempStr  = (char *)data;
			if(tempStr.length()>0)
			if(data != NULL){
				free(data);
				data = NULL;
			}
			if((tempStr.equals("+QMTSUB: 0,1,0,1\r\n"))||(tempStr.equals("+QMTSUB: 0,1,0,0\r\n"))){
				this->mqttConneced = true;
				delay(1000);
				return true;
			}
		}
    }
	return false;
}

bool DFRobot_BC20 :: unSubTopic(char connectID, char msgID, char* topic){
	char* tempdata;
	uint8_t topic_len=strlen(topic);
	uint8_t cmd_len=strlen(SET_QMTUNS);
	uint8_t ALL_len=cmd_len+1+1+1+1+2+topic_len+2;
	tempdata=(char*)malloc(ALL_len);
	if(tempdata==NULL)
	{
		free(tempdata);
		return 0;
	}	
	memset(tempdata,'\0',ALL_len+1);
	memcpy(tempdata,SET_QMTUNS,cmd_len);
	memcpy(tempdata+cmd_len,"=",1);
	tempdata[cmd_len+1] = connectID;
	memcpy(tempdata+cmd_len+1+1,",",1);
	tempdata[cmd_len+1+1+1] = msgID;
	memcpy(tempdata+cmd_len+1+1+1+1,",\"",2);
	memcpy(tempdata+cmd_len+1+1+1+1+2,topic,topic_len);
	memcpy(tempdata+cmd_len+1+1+1+1+2+topic_len,"\"",2);
    flushBC20Serial();
    sendATCMD((char*)tempdata);
	free(tempdata);	
    receviceATCMD(5000);
    return CheckRecData(AT_OK);
}
bool DFRobot_BC20::publish(char* topic,char* msg){
		return pubMessage('0','1','1','0',topic,msg);
		
}
bool DFRobot_BC20 :: pubMessage(char connectID, char msgID, char qos, char retain, char* topic, char* msg){		
	char * tempChar=NULL;
	uint8_t* data;
	String tempStr;
	uint8_t timeout;
	uint8_t topic_len=strlen(topic);
	uint8_t msg_len=strlen(msg);
	uint8_t cmd_len=strlen(SET_QMTPUB);
 	tempChar=(char*)malloc(cmd_len+1+1+1+1+1+1+1+1+2+topic_len+3+msg_len+2);
	if(tempChar == NULL){
      free(tempChar);
      return 1;
    }
	memset(tempChar,'\0',cmd_len+1+1+1+1+1+1+1+1+2+topic_len+3+msg_len+2);
	memcpy(tempChar,SET_QMTPUB,cmd_len);
 	memcpy(tempChar+cmd_len,"=",1);
	tempChar[cmd_len+1] = connectID;
 	memcpy(tempChar+cmd_len+1+1,",",1);
	tempChar[cmd_len+1+1+1] = msgID;
	memcpy(tempChar+cmd_len+1+1+1+1,",",1);
	tempChar[cmd_len+1+1+1+1+1] = qos;
	memcpy(tempChar+cmd_len+1+1+1+1+1+1,",",1);
	tempChar[cmd_len+1+1+1+1+1+1+1] = retain;
	memcpy(tempChar+cmd_len+1+1+1+1+1+1+1+1,",\"",2);
	memcpy(tempChar+cmd_len+1+1+1+1+1+1+1+1+2,topic,topic_len);
	memcpy(tempChar+cmd_len+1+1+1+1+1+1+1+1+2+topic_len,"\",\"",3);
	memcpy(tempChar+cmd_len+1+1+1+1+1+1+1+1+2+topic_len+3,msg,msg_len);
	memcpy(tempChar+cmd_len+1+1+1+1+1+1+1+1+2+topic_len+3+msg_len,"\"",1);   
	flushBC20Serial();
    sendATCMD((char*)tempChar);
	free(tempChar);
	tempChar=NULL;
    receviceATCMD(5000);
    if(CheckRecData(AT_OK)){
         while(1){
			receviceATCMD(100);
            if(timeout > 250){
				flushBC20Serial();
                return false;
            }
            timeout++;
            getRecDataforNum_NoCheck(1,data);
            CheckRecData("");
            tempStr  = (char *)data;
            if(data != NULL){
                free(data);
                data = NULL;
            }
            if(tempStr.equals("+QMTPUB: 0,1,0\r\n")){
				flushBC20Serial();
                return true;
            }
        } 
    }
    return false; 
}

void DFRobot_BC20 :: setCallback(void (*call)(char*, uint8_t*, unsigned int)){
    this->callback = call;
}

void DFRobot_BC20 :: loop(){
	String tempdata;
	char* tempStr=NULL;
	if(available()){
		tempdata=readData();
		tempStr=(char*)malloc(80);
		if(tempStr==NULL)
		{
			if(ret!=NULL)
			{
				free(ret);
				ret=NULL;
			}			
			free(tempStr);
			return;
		}
		memset(tempStr,'\0',80);
		memcpy(tempStr,tempdata.c_str(),tempdata.length());
	if(indexOf(tempStr,"+QMTRECV: ") != -1){
 		tempStr = removeSthString("+QMTRECV: ",tempStr);
		char* Topic;
		char*a;
		Topic=(char*)malloc(60);
		if(Topic==NULL)
		{
			if(ret!=NULL)
			{
				free(ret);
				ret=NULL;
			}			
			free(tempStr);
			tempStr=NULL;
			free(Topic);
			Topic=NULL;			
			return;			
		}
		memset(Topic,'\0',60);
		char* RecData;
		tempStr = removeSthString(",",tempStr);
		tempStr = removeSthString(",\"",tempStr);
		a = GetSthfrontString("\",",tempStr);
		memcpy(Topic,a,strlen(a));
		tempStr = removeSthString("\",\"",tempStr);
		RecData = GetSthfrontString("\"\r\n",tempStr);
		if(callback!=NULL){
			callback((char *)(Topic),(uint8_t *)(RecData),strlen(RecData));
		}
		free(Topic);
		Topic=NULL;
		if(ret!=NULL)
		{
			free(ret);
			ret=NULL;
		}
		free(tempStr);
		tempStr=NULL;		
	}else{
		if(ret!=NULL)
		{
			free(ret);
			ret=NULL;
		}			
			free(tempStr);
			return;			
		} 

	}	
}

bool DFRobot_BC20::configSleepMode(eSleepMode_t mode){
    return setQSCLK(mode);
}

bool DFRobot_BC20::BC20Wakeup(){
    flushBC20Serial();
	char* str="WakeUp";
    sendATCMD(str);
    delay(300);
    return(checkBC20());
}

static String ConvertString(uint8_t num, uint8_t bit){
    String ret = "";
    for(int i = bit; i > 0; i--){
        if(num & (0x01<<i-1)){
            ret += '1';
        }else{
            ret += '0';
        }
    }
    return ret;
}


void DFRobot_BC20::setTAUTime(uint8_t time, eTAUunit_t unit){
    this->_TAUValue = ConvertString(unit,3) + ConvertString(time,5);
}

void DFRobot_BC20::setActiveTime(uint8_t time, eAcTunit_t unit){
    this->_AcTValue = ConvertString(unit,3) + ConvertString(time,5);
}

bool DFRobot_BC20::setPSMMode(ePSM_t status){
	char s;
	switch(status)
	{
		case 0:
			s='0';
			break;
		case 1:
			s='1';
			break;
		case 2:
			s='2';
			break;	
		default:
			break;
	}
	//String strdata;
	//strdata=(String)SET_PSMS+"="+status+",,,\""+"10000100"+"\",\""+"00000101"+"\"";
	char* strdata=NULL;
	strdata=(char*)malloc(32);
	if(strdata==NULL)
	{
		free(strdata);
		strdata=NULL;
		return false;
	}
	memset(strdata,'\0',32);
	memcpy(strdata,SET_PSMS,5);
	strdata[5]='=';
	strdata[6]=s;
	//memcpy(strdata+6,(char)status,1);
	memcpy(strdata+7,",,,\"",4);
	memcpy(strdata+11,"10000010",8);
	//memcpy(strdata+11,"01000011",8);
	memcpy(strdata+19,"\",\"",3);
	//memcpy(strdata+22,"01000011",8);
	memcpy(strdata+22,"00010000",8);
	memcpy(strdata+30,"\"",1);
    flushBC20Serial();
	sendATCMD(strdata);
	free(strdata);
	strdata=NULL;
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

bool DFRobot_BC20::seteDRX(uint8_t mode){
    String val = "";
	String data;
	data=(String)SET_EDRXS+"=";
    flushBC20Serial();
    sendATCMD(data,mode);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

bool DFRobot_BC20::seteDRX(uint8_t mode,uint8_t ActType, uint8_t Requested){
    String val = "";
	String tempdata;
    switch(Requested){
        case eeDRXValue_20_48:
            val = "0010";
            break;
        case eeDRXValue_40_96:
            val = "0011";
            break;
        case eeDRXValue_81_92:
            val = "0101";
            break;
        case eeDRXValue_163_84:
            val = "1001";
            break;
        case eeDRXValue_327_68:
            val = "1010";
            break;
        case eeDRXValue_655_36:
            val = "1011";
            break;
        case eeDRXValue_1310_72:
            val = "1100";
            break;
        case eeDRXValue_2621_44:
            val = "1101";
            break;
        case eeDRXValue_5242_88:
            val = "1110";
            break;
        case eeDRXValue_10485_76:
            val = "1111";
            break;
        default:
            val = "0101";
            break;
    }
	tempdata=(String)SET_EDRXS+"="+mode+","+ActType+",\""+val+"\"";
    flushBC20Serial();
    sendATCMD(tempdata);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

bool DFRobot_BC20::seteDRX(uint8_t mode,uint8_t ActType, uint8_t Requested, uint8_t NW){
    String val = "";
    String nw = "";
	String tempdata;
    switch(Requested){
        case eeDRXValue_20_48:
            val = "0010";
            break;
        case eeDRXValue_40_96:
            val = "0011";
            break;
        case eeDRXValue_81_92:
            val = "0101";
            break;
        case eeDRXValue_163_84:
            val = "1001";
            break;
        case eeDRXValue_327_68:
            val = "1010";
            break;
        case eeDRXValue_655_36:
            val = "1011";
            break;
        case eeDRXValue_1310_72:
            val = "1100";
            break;
        case eeDRXValue_2621_44:
            val = "1101";
            break;
        case eeDRXValue_5242_88:
            val = "1110";
            break;
        case eeDRXValue_10485_76:
            val = "1111";
            break;
        default:
            val = "0101";
            break;
    }
    switch(NW){
        case eNWProvidedValue_20_48:
            nw = "0010";
            break;
        case eNWProvidedValue_40_96:
            nw = "0011";
            break;
        case eNWProvidedValue_81_92:
            nw = "0101";
            break;
        case eNWProvidedValue_163_84:
            nw = "1001";
            break;
        case eNWProvidedValue_327_68:
            nw = "1010";
            break;
        case eNWProvidedValue_655_36:
            nw = "1011";
            break;
        case eNWProvidedValue_1310_72:
            nw = "1100";
            break;
        case eNWProvidedValue_2621_44:
            nw = "1101";
            break;
        case eNWProvidedValue_5242_88:
            nw = "1110";
            break;
        case eNWProvidedValue_10485_76:
            nw = "1111";
            break;
        default:
            nw = "0101";
            break;
    }
	tempdata=(String)SET_EDRXS+"="+mode+","+ActType+",\""+val+"\""+",\""+nw+"\"";
    flushBC20Serial();
    sendATCMD(tempdata);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

bool DFRobot_BC20::seteDRX(uint8_t mode,uint8_t ActType, uint8_t Requested, uint8_t NW, uint8_t PTW){
    String val = "";
    String nw = "";
    String ptw = "";
	String tempdata;
    switch(Requested){
        case eeDRXValue_20_48:
            val = "0010";
            break;
        case eeDRXValue_40_96:
            val = "0011";
            break;
        case eeDRXValue_81_92:
            val = "0101";
            break;
        case eeDRXValue_163_84:
            val = "1001";
            break;
        case eeDRXValue_327_68:
            val = "1010";
            break;
        case eeDRXValue_655_36:
            val = "1011";
            break;
        case eeDRXValue_1310_72:
            val = "1100";
            break;
        case eeDRXValue_2621_44:
            val = "1101";
            break;
        case eeDRXValue_5242_88:
            val = "1110";
            break;
        case eeDRXValue_10485_76:
            val = "1111";
            break;
        default:
            val = "0101";
            break;
    }
    switch(NW){
        case eNWProvidedValue_20_48:
            nw = "0010";
            break;
        case eNWProvidedValue_40_96:
            nw = "0011";
            break;
        case eNWProvidedValue_81_92:
            nw = "0101";
            break;
        case eNWProvidedValue_163_84:
            nw = "1001";
            break;
        case eNWProvidedValue_327_68:
            nw = "1010";
            break;
        case eNWProvidedValue_655_36:
            nw = "1011";
            break;
        case eNWProvidedValue_1310_72:
            nw = "1100";
            break;
        case eNWProvidedValue_2621_44:
            nw = "1101";
            break;
        case eNWProvidedValue_5242_88:
            nw = "1110";
            break;
        case eNWProvidedValue_10485_76:
            nw = "1111";
            break;
        default:
            nw = "0101";
            break;
    }
    switch(PTW){
        case ePTWValue_2_56:
            ptw = "0000";
            break;
        case ePTWValue_5_12:
            ptw = "0001";
            break;
        case ePTWValue_7_68:
            ptw = "0010";
            break;
        case ePTWValue_10_24:
            ptw = "0011";
            break;
        case ePTWValue_12_8:
            ptw = "0100";
            break;
        case ePTWValue_15_36:
            ptw = "0101";
            break;
        case ePTWValue_17_92:
            ptw = "0110";
            break;
        case ePTWValue_20_48:
            ptw = "0111";
            break;
        case ePTWValue_23_04:
            ptw = "1000";
            break;
        case ePTWValue_25_6:
            ptw = "1001";
            break;
        case ePTWValue_28_16:
            ptw = "1010";
            break;
        case ePTWValue_30_72:
            ptw = "1011";
            break;
        case ePTWValue_33_28:
            ptw = "1100";
            break;
        case ePTWValue_35_84:
            ptw = "1101";
            break;
        case ePTWValue_38_4:
            ptw = "1110";
            break;
        case ePTWValue_40_96:
            ptw = "1111";
            break;
        default:
            ptw = "0101";
            break;
    }
	tempdata=(String)SET_EDRXS+"="+mode+","+ActType+",\""+val+"\""+",\""+nw+"\""+",\""+ptw+"\"";
    flushBC20Serial();
    sendATCMD(tempdata);
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

bool DFRobot_BC20::setNBEventReport(bool mode){
    if(mode){
        sendATCMD("AT+QNBIOTEVENT=1,1");	
    }else{
        sendATCMD("AT+QNBIOTEVENT=0,1");
    }
    receviceATCMD(300);
    return CheckRecData(AT_OK);
}

void DFRobot_BC20::setPSMCallback(void (*call)(void)){
    this->PSMcallback = call;
}

/* void DFRobot_BC20::controlLED(String str)
{
	sendATCMD(str);
} */

void DFRobot_BC20::controlLED(char * str)
{
	sendATCMD((char *)str);
}

void DFRobot_BC20::LED_ON()
{
	uint8_t size=0;
	char color=this->color;
	size=1;
	LED_OFF();
	char* str=NULL;
	str=(char*)malloc(8+size);
	if(str==NULL)
	{
		free(str);
		str=NULL;
		return;
	}
	memset(str,'\0',8+size);
	memcpy(str,"LED_",4);
	str[4]=this->color;
	memcpy(str+4+size,"_ON",3);
	controlLED((char*)str);
	free(str);
	str=NULL;	
}
void DFRobot_BC20::LED_OFF()
{
	char* str;
	str="LED_W_OFF";
	controlLED((char*)str);	
}
void DFRobot_BC20::changeColor(uint8_t newColor)
{
	switch(newColor)
	{
		case 0:
			this->color='R';
			break;
		case 1:
			this->color='B';
			break;
		case 2:
			this->color='G';
			break;
		case 3:
			this->color='Y';
			break;
		case 4:
			this->color='P';
			break;
		case 5:
			this->color='C';
			break;
		case 6:
			this->color='W';
			break;
		default:
			break;
	}
	return;	
}
/* void DFRobot_BC20::LEDFlash(String Color)
{
	String str1;
	String str2;
	str1="LED_"+Color+"_ON";
	str2="LED_"+Color+"_OFF";
	controlLED(str1);
	delay(10);
	controlLED(str2);
} */

bool DFRobot_BC20::stmLowpower()
{
	controlLED("LED_G_ON");
	delay(10);
	controlLED("LED_G_OFF");
	delay(10);
	sendATCMD("DSLEEP");
	delay(10);
	return(checkBC20());
}
bool DFRobot_BC20::stmWakeup(uint8_t Awake_Pin)
{
	pinMode(Awake_Pin,OUTPUT);
	digitalWrite(Awake_Pin,LOW);
	delay(100);
	digitalWrite(Awake_Pin,HIGH);
	delay(100);
	digitalWrite(Awake_Pin,LOW);
	return(checkStmStauts());
}
DFRobot_BC20_Serial::DFRobot_BC20_Serial(HardwareSerial*SSerial1)
:SSerial(SSerial1)
{
	SSerial->begin(9600);
}

void DFRobot_BC20_Serial::sendATCMD(String str){
    if(str.length() > 0){
        SSerial->println("AT+"+str);
    }else{
        SSerial->println("AT"+str);
    }
	delay(500);
}

void DFRobot_BC20_Serial::sendATCMD(char* str){
    if(strlen(str) > 0){
        SSerial->print("AT+");
		SSerial->println(str);
    }else{
        SSerial->println("AT");
    }
	delay(500);
}


void DFRobot_BC20_Serial::sendATCMDBychar(char str){
	 SSerial->print(str);
}

void DFRobot_BC20_Serial::sendATCMD(String str,uint8_t num){
    SSerial->print("AT+"+str+"=");
    SSerial->println(num);
	delay(500);
}

void DFRobot_BC20_Serial::sendATCMD(String str,String cmd){
    SSerial->print("AT+"+str+"=");
    SSerial->println(cmd);
}

void DFRobot_BC20_Serial::sendATCMD(uint8_t num){
    SSerial->println(num);
	delay(500);
}

bool DFRobot_BC20_Serial::available(void)
{
	if(SSerial->available()> 0)
		return true;
	else
		return false;
}

String DFRobot_BC20_Serial::readData()
{
	String tempData="";
    uint32_t nowtime = millis();
    while(millis() - nowtime < 1000){
        while(SSerial->available() > 0){
            tempData += (char)SSerial->read();
			delay(1);
        }
    }
	return tempData;
}

void DFRobot_BC20_Serial::receviceATCMD(uint32_t timeout){//Receive the command data returned by BC20
    String tempData="";
    int tempInt;
    uint8_t tempNum = 0;
    uint32_t nowtime = millis();
    while(millis() - nowtime < timeout){
        while(SSerial->available() > 0)
		{
            tempData += (char)(SSerial->read());
        }
        if((tempData.indexOf("OK\r\n") != -1)){
            break;
        }
    }
    if(tempData.length() > 0)
	{	
        do{
            tempInt = tempData.indexOf("\r\n");
            if(tempInt != -1){
                cuappEnqueue((uint8_t *)((tempData.substring(0,tempInt+2)).c_str()),tempInt+2,tempNum);
                tempData = tempData.substring(tempInt + 2, tempData.length());
                tempNum ++;
            }else{
                if(tempData.length() > 0){//Displays the data returned by BC20
                }
            }
        }while(tempInt >= 0);
        sRecData.dataNum = tempNum-1;
    }
}

DFRobot_BC20_IIC::DFRobot_BC20_IIC(uint8_t addr)
:IIC_addr(addr)
{
	Wire.begin();
}
void DFRobot_BC20_IIC::sendATCMD(char* str){
	int count=0;
	uint8_t str_len=strlen(str);
	uint8_t* tempdata;
	uint8_t IIC_len=30;
	tempdata=(uint8_t*)malloc(str_len+3);
	if(tempdata==NULL)
	{
		free(tempdata);
		return;
	}
    if(strlen(str)> 0){
		memcpy(tempdata,"AT+",3);
		memcpy(tempdata+3,str,str_len);	
		str_len+=3;
		if(str_len>IIC_len){
			while(str_len>IIC_len){
				Wire.beginTransmission(IIC_addr);
				Wire.write(0x00);
				Wire.write(tempdata+IIC_len*count,IIC_len);
				Wire.endTransmission();
				count++;
				str_len-=IIC_len;				
			}				
			Wire.beginTransmission(IIC_addr);
			Wire.write(0x00);			
			Wire.write(tempdata+IIC_len*count,str_len);
			Wire.write(0x0D);
			Wire.write(0x0A);
			Wire.endTransmission();
		}else{
			Wire.beginTransmission(IIC_addr);
			Wire.write(0x00);
			Wire.write(tempdata,str_len);				
			Wire.write(0x0D);
			Wire.write(0x0A);
			Wire.endTransmission();		
		}		
    }else{
		Wire.beginTransmission(IIC_addr);
		Wire.write(0x00);
		Wire.write((uint8_t*)"AT",2);
		Wire.write(0x0D);
		Wire.write(0x0A);
		Wire.endTransmission();	
    }
	free(tempdata);
	tempdata=NULL;
	delay(500);		
} 
void DFRobot_BC20_IIC::sendATCMD(String str){
	String temp;
    if(str.length() > 0){
		temp="AT+"+str;			
		uint8_t data[temp.length()+1];
		temp.toCharArray((char*)data,temp.length()+1);
		if(temp.length()>30)
		{
			Wire.beginTransmission(IIC_addr);
			Wire.write(0x00);
			Wire.write(data,30);
			Wire.endTransmission(); 				
			Wire.beginTransmission(IIC_addr);
			Wire.write(0x00);	
			Wire.write(data+30,temp.length()-30);
			Wire.write(0x0D);
			Wire.write(0x0A);
			Wire.endTransmission();				
		}else{
			Wire.beginTransmission(IIC_addr);
			Wire.write(0x00);
			Wire.write(data,temp.length());			
			Wire.write(0x0D);
			Wire.write(0x0A);
			Wire.endTransmission(); 		
		}
    }else{
		temp="AT"+str;
		uint8_t data[temp.length()+1];
		temp.toCharArray((char*)data,temp.length()+1);
		Wire.beginTransmission(IIC_addr);
		Wire.write(0x00);
		Wire.write(data,temp.length());
		Wire.write(0x0D);
		Wire.write(0x0A);
		Wire.endTransmission();		
    } 
	delay(500);
}

void DFRobot_BC20_IIC::sendATCMDBychar(char str){
	Wire.beginTransmission(IIC_addr);
	Wire.write(0x00);
	Wire.write(str);
	Wire.endTransmission();
	delay(500);
}

void DFRobot_BC20_IIC::sendATCMD(String str,uint8_t num){
	str="AT+"+str+"=";
	uint8_t data[str.length()+1];
	str.toCharArray((char*)data,str.length()+1);
	Wire.beginTransmission(IIC_addr);
	Wire.write(0x00);
    Wire.write(data,str.length());
    Wire.write(num+0x30);
	Wire.write(0x0D);
	Wire.write(0x0A);
	Wire.endTransmission();	
	delay(500);
}

void DFRobot_BC20_IIC::sendATCMD(String str,String cmd){
	str="AT+"+str+"="+cmd;
	uint8_t data[str.length()+1];
	data[str.length()]='\0';
	str.toCharArray((char*)data,str.length()+1);
	Wire.beginTransmission(IIC_addr);
	Wire.write(0x00);    
	Wire.write(data,str.length());
	Wire.write(0x0D);
	Wire.write(0x0A);
	Wire.endTransmission();	
	delay(500);
}

void DFRobot_BC20_IIC::sendATCMD(uint8_t num){
	Wire.beginTransmission(IIC_addr);
	Wire.write(0x00);    
	Wire.write(num+0x30);
	Wire.write(0x0D);
	Wire.write(0x0A);
	Wire.endTransmission();
	delay(500);
}

bool DFRobot_BC20_IIC::available(void)
{
	uint8_t len = 0;
	Wire.beginTransmission(IIC_addr);
	Wire.write(0x00);
	Wire.endTransmission();
	Wire.requestFrom(IIC_addr,1);
	len = Wire.read();	
	if(len > 0){
		return true;
	}
	return false;
}
String DFRobot_BC20_IIC::readData()
{
	String tempData;
	int a_addr=1;
	int num=0;
	int len = 0;
    int tempInt;
    uint8_t tempNum = 0;
	uint32_t nowtime = millis();
	int IIC_Len = 32;
    while(millis() - nowtime < 5000){
		Wire.beginTransmission(IIC_addr);
		Wire.write(0x00);
		Wire.endTransmission();		
		Wire.requestFrom(IIC_addr,1);
		len = Wire.read();			
 		if(len==255)
			return tempData; 
		if(len > 0){
			Wire.beginTransmission(IIC_addr);
			Wire.write(0x01);
			Wire.endTransmission();			
			while(len!=0){
				if(len > IIC_Len){		
					Wire.requestFrom(IIC_addr,IIC_Len);
					for(int i=0;i<IIC_Len;i++){
						tempData+=(char)Wire.read();

					}
					len -= IIC_Len;	
					if(a_addr > IIC_Len){
						a_addr = IIC_Len;
					}				
				}else{
					Wire.requestFrom(IIC_addr,len);
					for(int i=0;i<len;i++){
						tempData+=(char)Wire.read();
					}
					len=0;
				}				
			}				
		if((tempData.indexOf("OK\r\n") != -1)){
			break;
        }
		}
    }	
	return tempData;
}

void DFRobot_BC20_IIC::receviceATCMD(uint32_t timeout){//Receive the command data returned by BC20
	String tempData;
	int a_addr=1;
	int num=0;
	uint8_t flag=0;
	int len = 0;
    int tempInt;
    uint8_t tempNum = 0;
	uint32_t nowtime = millis();
	int  IIC_Len = 32;
    while(millis() - nowtime < timeout){
		Wire.beginTransmission(IIC_addr);
		Wire.write(0x00);
		Wire.endTransmission();		
		Wire.requestFrom(IIC_addr,1);
		len = Wire.read();
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_LEONARDO)	|| defined(ARDUINO_AVR_MEGA2560)
		if(len>0)
		{
			num++;			
		}
		if(((num>6)&&(len==0))||(flag==1))
		{
			sRecData.dataNum = tempNum-1;
			return;
		}
#else 
		if(flag==1){
			sRecData.dataNum = tempNum-1;
			return;
		}
#endif		
 		if(len==255)
			return; 
		if(len > 0){
			Wire.beginTransmission(IIC_addr);
			Wire.write(0x01);
			Wire.endTransmission();			
			while(len!=0){
				if(len > IIC_Len){		
					Wire.requestFrom(IIC_addr,IIC_Len);
					for(int i=0;i<IIC_Len;i++){
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_LEONARDO)	|| defined(ARDUINO_AVR_MEGA2560)		
						if(num<7){
							tempData+=(char)Wire.read();
						}else{
							Wire.read();
						}	
#else 
						tempData+=(char)Wire.read();
#endif						
					}
					len -= IIC_Len;	
					if(a_addr > IIC_Len){
						a_addr = IIC_Len;
					}				
				}else{
					Wire.requestFrom(IIC_addr,len);
					for(int i=0;i<len;i++){
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_LEONARDO)	|| defined(ARDUINO_AVR_MEGA2560)					
						if(num<7){
							tempData+=(char)Wire.read();
						}else{
							Wire.read();
						}	
#else 
						tempData+=(char)Wire.read();
#endif
					}
					len=0;
				}
				if((tempData.indexOf("\r\n") != -1))
				{
					break;
				}
				
			}
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_LEONARDO)	|| defined(ARDUINO_AVR_MEGA2560)			
if((num>5)&&(flag==0))
{
	tempData="OK\r\n";
}
#endif	
		if((tempData.indexOf("\r\n") != -1)){
			if(tempData.length() > 0)
			{
				do{
				tempInt = tempData.indexOf("\r\n");
				if(tempInt != -1){
					cuappEnqueue((uint8_t *)((tempData.substring(0,tempInt+2)).c_str()),tempInt+2,tempNum);						
					tempNum ++;
					if((tempData.indexOf("OK\r\n") != -1))					
						flag=1;											
						tempData="";											
				}else{
					if(tempData.length() > 0){
						
					}
				}
				}while(tempInt >= 0);						 
			}
		}			
		}		
    }
}
#ifndef ARDUINO_ESP32_DEV
DFRobot_BC20_SW_Serial::DFRobot_BC20_SW_Serial(SoftwareSerial* ss)
:SSerial(ss)
{
	SSerial->begin(9600);
}

void DFRobot_BC20_SW_Serial::sendATCMD(char* str){
	
	if(strlen(str) > 0){
        SSerial->print("AT+");
		SSerial->println(str);
    }else{
        SSerial->println("AT");
    }
	delay(500);
}

void DFRobot_BC20_SW_Serial::sendATCMD(String str){
	
    if(str.length() > 0){
        SSerial->println("AT+"+str);
    }else{
        SSerial->println("AT");
    }
	delay(500);
}

void DFRobot_BC20_SW_Serial::sendATCMDBychar(char str){
	 SSerial->print(str);
}

void DFRobot_BC20_SW_Serial::sendATCMD(String str,uint8_t num){
    SSerial->print("AT+"+str+"=");
    SSerial->println(num);
	delay(500);
}

void DFRobot_BC20_SW_Serial::sendATCMD(String str,String cmd){
    SSerial->print("AT+"+str+"=");
    SSerial->println(cmd);
	//delay(100);
}

void DFRobot_BC20_SW_Serial::sendATCMD(uint8_t num){
    SSerial->println(num);
	delay(500);
}

bool DFRobot_BC20_SW_Serial::available(void)
{
	if(SSerial->available()> 0){
		return true;
	}
	else
		return false;
}

String DFRobot_BC20_SW_Serial::readData()
{
	String tempData="";
    uint32_t nowtime = millis();
    while(millis() - nowtime < 1000){
        while(SSerial->available() > 0){
			tempData += (char)SSerial->read();		
        }
		if((tempData.indexOf("OK") != -1)){
		break;
	}
	}
	return tempData;
}

void DFRobot_BC20_SW_Serial::receviceATCMD(uint32_t timeout){	
    String tempData="";
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_MEGA2560)
	Flag=1;
	uint8_t counter=5;
#else
	uint8_t counter=100;
#endif
    int tempInt;
    uint8_t tempNum = 0;
	uint8_t num=0;
	uint8_t flag=0;
    uint32_t nowtime = millis();
	uint32_t len = 0;
    while(millis() - nowtime < timeout){
        while(SSerial->available() > 0){
			if(num<counter){
				tempData +=(char)(SSerial->read());
				len++;
				nowtime = millis();
			}else{
				SSerial->read();
				break;
			}
			if((tempData.indexOf("\r\n") != -1))
			{
				flag=1;					
				num++;
				break;
			}				
        }	
			if(num==counter)
			{
				tempData="OK\r\n";
			}		
		if((tempData.length() > 0)&&(flag==1)){	
			tempInt = tempData.indexOf("\r\n");
			cuappEnqueue((uint8_t *)((tempData.substring(0,tempInt+2)).c_str()),tempInt+2,tempNum);
			if((tempData.indexOf("OK\r\n") != -1))
			{
				tempData="";
				flag=0;		
				break;
			}
			tempData="";
			flag=0;		
			tempNum ++;
		}		      
    }
	sRecData.dataNum = tempNum-1;
}
#endif