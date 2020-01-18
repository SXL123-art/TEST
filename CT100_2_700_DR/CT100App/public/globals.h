#ifndef _GLOBALS_H
#define _GLOBALS_H

#define USE_CT100G
#define __no_init

// <Printer JIG Setting> ------------------
// use only test mode
//#define USE_PRINT_JIG
//#define TEST_CHESS

// <Debug Setting> ------------------
//#define USE_DEBUG

//USE_DEBUG_CONSOLE : if you use ttySAC0 as a debug console. Main serial port will be disabled
//if you want use UART0 as a main serial port, you should undefine this.
#define USE_DEBUG_CONSOLE

// <Country Setting> ----------------------
//#define COUNTRY_EUROPE
#define COUNTRY_CHINA
///////////////////////////////////////////

#ifdef COUNTRY_CHINA
//(1) FONT --------------------------------
//#define USE_ENG_8x16
//#define USE_CHN_GB2312_16x16
#define USE_ENG_12x24
#define USE_CHN_GB2312_24x24
//#define USE_CHN_SONGTI_FONT
//(2) MAIN PCB TYPE -----------------------
//#define USE_MAIN_4_LAYER
//#define USE_MAIN_2_LAYER
#define USE_ENET
//(3) KEYPAD ------------------------------
//#define USE_OLD_KEYPAD
//#define USE_NEW_KEYPAD
#define USE_CHN_STD_MKT_KEYPAD
// #define USE_CHN_STD_MKT_CART_KEY
//(4) PROTOCOL TYPE -----------------------
//#define USE_POSCALE_PROTOCOL
#define USE_CT100_PROTOCOL
///////////////////////////////////////////
//(5) AREA
//#define USE_SHANGHAI
//#define USE_QINGDAO
#define USE_CHONGQING

#ifdef USE_SHANGHAI
  //#define USE_UART2
  //#define USE_RFID
  //#define USE_GPRS
#endif

#ifdef USE_QINGDAO
  //#define USE_UART2
  //#define USE_RFID
  //#define USE_GPRS
#endif

#ifdef USE_CHONGQING
  #define USE_JINAN_BASED_CHONGQING
  #define USE_UART2
  #define USE_RFID
  //#define USE_RFID_SH //: It is different CT100G's rfid. when testing, we have to do commented out USE_RFID.
  #define USE_GPRS
  #define USE_UART3
  #define USE_CHINA_PINYIN
  #define USE_3G_USB // HUAWEI E261 tested
#endif
///////////////////////////////////////////
//(6) OPTION
#define USE_BITMAP
#define USE_GROUP_NAME
#define USE_TRACE
#define USE_HEAD_TAIL
#define USE_RECEIPT_FORM
#define USE_PARAMETER
//#define USE_PARAMETER_M //menu
#define USE_SHANGHAI_DLL
#ifdef USE_SHANGHAI_DLL
  #define USE_BOOTH
  #define USE_MEAT_TRACE
  #define USE_VEGE_TRACE
  #define USE_LOG_UPLOAD
  #define USE_VEGE_STOCK_CLEAR
#endif

//#define USE_REALTIME_UPLOAD
#define USE_USB_HOST
#define USE_SMPS
#define USE_EEPROM_TRACE
#define USE_FLASH_CLEAR_SPEED_UP
//#define USE_WEAR_LEVELING
#define USE_ORIGIN
#endif

#ifdef COUNTRY_EUROPE
//(1) FONT --------------------------------
//#define USE_ENG_8x16
//#define USE_CHN_GB2312_16x16
//#define USE_CHN_GB2312_24x24
//#define USE_ENG_12x24
//(2) MAIN PCB TYPE -----------------------
#define USE_MAIN_4_LAYER
//#define USE_MAIN_2_LAYER
//#define USE_ENET
//(3) KEYPAD ------------------------------
//#define USE_OLD_KEYPAD
#define USE_NEW_KEYPAD
//#define USE_CHN_STD_MKT_KEYPAD
//(4) PROTOCOL TYPE -----------------------
//#define USE_POSCALE_PROTOCOL
#define USE_CT100_PROTOCOL
///////////////////////////////////////////
#endif

#define ROMDATA	const
#define HUGEDATA

//HYP 20060412
//#define USE_NEARDATA
//_near is 1kbyte in default DS
#ifdef USE_NEARDATA
#define NEARDATA	_near
#else
#define NEARDATA	HUGEDATA
#endif//#ifdef USE_NEARDATA

#define PLUS    1
#define MINUS   0

#define ON		1
#define OFF		0
#define IDLE	2

#define SCALE   1
#define PC      2

#define STABLE_ALLOW_TIME   50
#define SUPER_PASSWORD      13579
#define CAL_PASSWORD        83419

#define FW_MAIN_CHECKSUM_START           0
#define FW_MAIN_CHECKSUM_END          0x200000 //2048 Kb
#define FW_BOOT_VERSION_ROM_ADDR      0x21fff0
#define MAIN_VERSION        0
#define BOOT_VERSION        1

#ifdef USE_CT100G
//#include "91x_type.h"
//#include "91x_rtc.h"

#define _PACKED_DEF //__packed
#else
#define _PACKED_DEF __packed
#endif

//#define NULL 0

#ifndef true
#define true  1
#endif

#ifndef false
#define false 0
#endif


//#ifndef NULL
//#define NULL  0
//#endif

//#ifndef null
//#define null  0
//#endif

typedef unsigned char   	uint8;                                      /* Unsigned  8 bit quantity     */
typedef signed   char   	int8;                                       /* Signed    8 bit quantity     */
typedef unsigned short  	uint16;                                     /* Unsigned  16 bit quantity    */
typedef signed   short  	int16;                                      /* Signed    16 bit quantity    */
typedef unsigned long    	uint32;                                     /* Unsigned  32 bit quantity    */
typedef signed   long    	int32;                                      /* Signed    32 bit quantity    */


typedef unsigned char bit;
typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long dword;

typedef volatile unsigned long   vu32;
typedef volatile unsigned short  vu16;
typedef volatile unsigned char   vu8;

typedef volatile signed long     vs32;
typedef volatile signed short    vs16;
typedef volatile signed char     vs8;

typedef enum { RESET = 0, SET   = !RESET } FlagStatus, ITStatus;

typedef enum { DISABLE = 0, ENABLE  = !DISABLE} FunctionalState;

typedef enum { ERROR = 0, SUCCESS  = !ERROR} ErrorStatus;


struct driver_ver {
        char str[10];
}__attribute__((packed));


#pragma pack(1)
typedef struct {
        uint8 Class;
        uint16 ModelNum;//ex) CT100 = 100, CL5000J = 5010
        uint8 Type;//1 = Bench, 2 = Pole, 3 = Hanging, 4 = Self, 9 = Printer(?)
        uint16 ScaleId;
        uint16 MainVersion;//1
        uint16 SubVersion;//0
        uint16 DataVersion;//0 //high byte = zFLASH STRUCT VERSION, low byte = EEPROM STRUCT VERSION
        uint16 CountryNum;//not used
        uint8 CountryCode[2];
} VERSION_STRUCT;
#pragma pack()

typedef struct {
        char *Buf;
        uint16 Size;
        uint16 InPtr;
        uint16 OutPtr;
} RING_BUF;

extern VERSION_STRUCT fw_version;// ROMDATA delete

//DEFINE NATION OPTION/////////////////////
#ifdef USE_CT100G
#define COUNTRY_CODE0		'C'
#define COUNTRY_CODE1		'N'
#else
#if (defined(COUNTRY_KOREA))
#define COUNTRY_CODE0		'K'
#define COUNTRY_CODE1		'R'
#endif

#if (defined(COUNTRY_EUROPE))
#define COUNTRY_CODE0		'E'
#define COUNTRY_CODE1		'U'
#endif

#if (defined(COUNTRY_CHINA))
#ifdef USE_SHANGHAI
  #define COUNTRY_CODE0		'S'
  #define COUNTRY_CODE1		'H'
#elif defined (USE_QINGDAO)
  #define COUNTRY_CODE0		'Q'
  #define COUNTRY_CODE1		'D'
#elif defined (USE_CHONGQING)
  #define COUNTRY_CODE0		'C'
  #define COUNTRY_CODE1		'Q'
#else //china un version
  #define COUNTRY_CODE0		'C'
  #define COUNTRY_CODE1		'N'
#endif
#endif
#endif //#ifdef USE_CT100G

#ifdef USE_POSCALE_PROTOCOL
#define POSCALE_MAX_ART     1800
#endif

#define MODEL_CT100GR	0
#define MODEL_CT100GC	1


#define CELL_CHARGED_LEVEL                  1380		//14.00V//
#define CELL_RECHARGING_LEVEL               1290 //in the future modify 1360		//13.60V//
#define CELL_LOW_LEVEL                      1160        //11.6V//
#define CELL_OFF_LEVEL                      1120        //11.20V//

//#define kilo 1000 //28


#define PLU_POS  			0
#define TAREW_POS       	17
#define NETW_POS     		25
#define UNITP_POS 			33
#define TOTALP_POS 			43
#define SHIFT_POS           44
#define POWERMODE_POS       45
#define BATLOW_POS          46

#define PLU_DIGIT           12
#define TARE_DIGIT          5
#define WEIGHT_DIGIT        5
#define UNIT_DIGIT          6
#define TOTAL_DIGIT         7

#define QT_PLU_DIGIT        30
#define QT_OUT_FILE		"/root/Application/QRBMP/qrpay.bmp"

#define SALE_KEY_VALID 1
#define MAXLEN_PRICE  7
// 键盘类型开关
#define SMALL_KEY 		0
#define MIDDLE_KEY 		1

#define KEY_TYPE 		MIDDLE_KEY


#endif // GLOBALS_H
