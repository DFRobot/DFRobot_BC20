#ifndef __DFROBOT_BC20AT__
#define __DFROBOT_BC20AT__
#define     CIMI_INFO                 0
#define     AT_OK                     "OK\r\n"

/*Product Identification Information获取产品信息*/
#define     GET_PII                   "ATI"

#define     SET_ECHO_0                "ATE0"
#define     SET_ECHO_1                "ATE1"

#define     READ_BAUD                 "IPR?"
#define     CHECK_BAUD                "IPR=?"
#define     SET_BAUD                  "IPR="


#define     CHECK_BC20                ""
#define     CHECK_NB_CARD             "CIMI"
#define     CHECK_IMEI_NUM            "CGSN=1"

/*Manufacturer Identification获取厂商信息*/
#define     TEST_MI                   "CGMI=?"
#define     GET_MI                    "CGMI"

/*Model Identification获取模组信息*/
#define     TEST_MM                   "CGMM=?"
#define     GET_MM                    "CGMM"

/*Manufacturer Identification获取厂商版本*/
#define     TEST_MR                   "CGMR=?"
#define     GET_MR                    "CGMR"

/*Signal Quality获取信号强度和信道误码率*/
#define     TEST_SQ                   "CSQ=?"
#define     GET_SQ                    "CSQ"

/*Extended Signal Quality获取扩展信号质量*/
#define     TEST_ESQ                  "CESQ=?"
#define     GET_ESQ                   "CESQ"

/*Product Serial Number获取产品序列号*/
#define     TEST_GSN                  "CGSN=?"
#define     GET_GSN                   "CGSN"

/*EPS Network Registration Status EPS网络注册状态*/
#define     TEST_EREG                 "CEREG=?"
#define     GET_EREG                  "CEREG?"
#define     SET_EREG                  "CEREG"

/*Signalling Connection Status*/
#define     TEST_SCON                 "CSCON=?"
#define     GET_SCON                  "CSCON?"
#define     SET_SCON                  "CSCON"

/*Operator Selection*/
#define     TEST_OPS                  "COPS=?"
#define     GET_OPS                   "COPS?"
#define     SET_OPS                   "COPS"

/*PS Attach or Detach*/
#define     TEST_GATT                 "CGATT=?"
#define     GET_GATT                  "CGATT?"
#define     SET_GATT                  "CGATT"

/*Define a PDP Context*/
#define     TEST_GDCONT               "CGDCONT=?"
#define     GET_GDCONT                "CGDCONT?"
#define     SET_GDCONT                "CGDCONT"

/*PDP Context Activation/Deactivation*/
#define     TEST_GACT                 "CGACT=?"
#define     GET_GACT                  "CGACT?"
#define     SET_GACT                  "CGACT"

/*Show PDP Addresses*/
#define     TEST_GPADDR               "CGPADDR=?"
#define     GET_GPADDR                "CGPADDR?"
#define     SET_GPADDR                "CGPADDR"

/*Request International Mobile Subscriber Identity*/
#define     TEST_IMI                  "CIMI=?"
#define     GET_IMI                   "CIMI"

/*Set UE Functionality*/
#define     TEST_FUN                  "CFUN=?"
#define     GET_FUN                   "CFUN?"
#define     SET_FUN                   "CFUN"

/*Report Mobile Termination Error*/
#define     TEST_MEE                  "CMEE=?"
#define     GET_MEE                   "CMEE?"
#define     SET_MEE                   "CMEE"

/*Return Current Date and Time*/
#define     TEST_CLK                  "CCLK=?"
#define     GET_CLK                   "CCLK?"

/*Query Power Supply Voltage*/
#define     TEST_BC                   "CBC=?"
#define     GET_BC                    "CBC"

/*Power Saving Mode Setting*/
#define     TEST_PSMS                 "CPSMS=?"
#define     GET_PSMS                  "CPSMS?"
#define     SET_PSMS                  "CPSMS"

/*eDRX Setting*/
#define     TEST_EDRXS                "CEDRXS=?"
#define     GET_EDRXS                 "CEDRXS?"
#define     SET_EDRXS                 "CEDRXS"

/*eDRX Read Dynamic Parameters*/
#define     TEST_EDRXRDP              "CEDRXRDP=?"
#define     GET_EDRXRDP               "CEDRXRDP"

/*Time Zone Reporting*/
#define     TEST_TZR                  "CTZR=?"
#define     GET_TZR                   "CTZR?"
#define     SET_TZR                   "CTZR"

/*Initial PDP Context Activation*/
#define     TEST_IPCA                  "CIPCA=?"
#define     GET_IPCA                   "CIPCA?"
#define     SET_IPCA                   "CIPCA"

/*CloT Optimization Configuration*/
#define     TEST_CIOTOPT               "CCIOTOPT=?"
#define     GET_CIOTOPT                "CCIOTOPT?"
#define     SET_CIOTOPT                "CCIOTOPT"

/*APN Rate Control*/
#define     TEST_GAPNRC                "CGAPNRC=?"
#define     SET_GAPNRC                 "CGAPNRC"

/*Extended Error Report*/
#define     TEST_EER                   "CEER=?"
#define     GET_EER                    "CEER"

/*Activate/Deactivate a PDN Context*/
#define     SET_QGACT                  "QGACT"

/*Get and Set Mobile Operation Band*/
#define     TEST_QBAND                 "QBAND=?"
#define     GET_QBAND                  "QBAND?"
#define     SET_QBAND                  "QBAND"

/*Automatically Reset*/
#define     TEST_QRST                  "QRST=?"
#define     SET_QRST                   "QRST"

/*Set Scrambling Algorithm*/
#define     TEST_QSPCHSC               "QSPCHSC=?"
#define     GET_QSPCHSC                "QSPCHSC?"
#define     SET_QSPCHSC                "QSPCHSC"

/*Lock NB-IoT Frequency*/
#define     TEST_QLOCKF                "QLOCKF=?"
#define     GET_QLOCKF                 "QLOCKF?"
#define     SET_QLOCKF                 "QLOCKF"

/*Clear NB-IoT Stored EARFCN List*/
#define     GET_QCSEARFCN              "QCSEARFCN"

/*Set Default PSD Connection Settings*/
#define     TEST_QCGDEFCONT            "QCGDEFCONT=?"
#define     GET_QCGDEFCONT             "QCGDEFCONT?"
#define     SET_QCGDEFCONT             "QCGDEFCONT"

/*NB-IoT Release Assistance Indication*/
#define     TEST_QNBIOTRAI            "QNBIOTRAI=?"
#define     GET_QNBIOTRAI             "QNBIOTRAI?"
#define     SET_QNBIOTRAI             "QNBIOTRAI"

/*Enable/Disable NB-IoT Related Event Report*/
#define     TEST_QNBIOTEVENT          "QNBIOTEVENT=?"
#define     GET_QNBIOTEVENT           "QNBIOTEVENT?"
#define     SET_QNBIOTEVENT           "QNBIOTEVENT"

/*Enable/Disable Deep Sleep Wakeup Indication*/
#define     TEST_QATWAKEUP            "QATWAKEUP=?"
#define     GET_QATWAKEUP             "QATWAKEUP?"
#define     SET_QATWAKEUP             "QATWAKEUP"

/*Engineering Mode*/
#define     TEST_QENG                 "QENG=?"
#define     GET_QENG                  "QENG"

/*USIM Card Identification*/
#define     GET_QCCID                 "QCCID"

/*Power off the Module*/
#define     GET_QPOWD                 "QPOWD=?"
#define     SET_QPOWD                 "QPOWD"

/*Configure Sleep Mode*/
#define     TEST_QSCLK                "QSCLK=?"
#define     GET_QSCLK                 "QSCLK?"
#define     SET_QSCLK                 "QSCLK"

/*Upgrade Firmware When the Delta Firmware Package is Stored on HTTP(S) Server*/
#define     TEST_QFOTADL              "QFOTADL=?"
#define     SET_QFOTADL               "QFOTADL"

/*GNSS 模块电源控制*/
#define     TEST_QGNSSC               "QGNSSC=?"
#define     GET_QGNSSC                "QGNSSC?"
#define     SET_QGNSSC                "QGNSSC"

/*获取 GNSS 信息*/
#define     TEST_QGNSSRD              "QGNSSRD=?"
#define     GET_QGNSSRD               "QGNSSRD?"
#define     SET_QGNSSRD               "QGNSSRD"
#define     NMEA_RMC                  "\"NMEA/RMC\""
#define     NMEA_VTG                  "\"NMEA/VTG\""
#define     NMEA_GGA                  "\"NMEA/GGA\""
#define     NMEA_GSA                  "\"NMEA/GSA\""
#define     NMEA_GSV                  "\"NMEA/GSV\""
#define     NMEA_GLL                  "\"NMEA/GLL\""

/*Configure Optional Parameters of MQTT*/
#define     TEST_QMTCFG               "QMTCFG=?"
#define     GET_QMTCFG                "QMTCFG"

/*Open a Network for MQTT Client*/
#define     TEST_QMTOPEN              "QMTOPEN=?"
#define     GET_QMTOPEN               "QMTOPEN?"
#define     SET_QMTOPEN               "QMTOPEN"

/*Close a Network for MQTT Client*/
#define     TEST_QMTCLOSE             "QMTCLOSE=?"
#define     SET_QMTCLOSE               "QMTCLOSE"

/*Connect a Client to MQTT Server*/
#define     TEST_QMTCONN              "QMTCONN=?"
#define     GET_QMTCONN               "QMTCONN?"
#define     SET_QMTCONN               "QMTCONN"

/*Disconnect a Client from MQTT Server*/
#define     TEST_QMTDISC              "QMTDISC=?"
#define     SET_QMTDISC               "QMTDISC"

/*Subscribe to Topics*/
#define     TEST_QMTSUB               "QMTSUB=?"
#define     SET_QMTSUB                "QMTSUB"

/*Unsubscribe from Topics*/
#define     TEST_QMTUNS               "QMTUNS=?"
#define     SET_QMTUNS                "QMTUNS"

/*Publish Messages*/
#define     TEST_QMTPUB               "QMTPUB=?"
#define     SET_QMTPUB                "QMTPUB"
#endif