///*****************************************************************************
/*
|*  Copyright       :  (c) 2010 CAS
|*  Filename        :   print.h
|*  Version         :   0.1
|*  Programmer(s)   :   Keum Young-Kwang (KYK)
|*  Created         :   2010/11/01
|*  Modified        :
|*  Description     :   CT100 printer function header
*/
//****************************************************************************/
#ifndef _PRINT_H_
#define _PRINT_H_
#include "driver/PRT_Driver.h"
#include "driver/qrencode.h"
#include "string.h"

#define PRT_TICKET_MSG_1    1
#define PRT_TICKET_MSG_2    2
#define PRT_TICKET_MSG_3    3
#define PRT_TICKET_MSG_4    4
#define PRT_TICKET_MSG_5    5
#define PRT_TICKET_MSG_6    6
#define PRT_TICKET_MSG_7    7
#define PRT_TICKET_MSG_8    8
#define PRT_TICKET_MSG_9    9
#define PRT_TICKET_MSG_10   10
#define PRT_TICKET_MSG_11   11
#define PRT_TICKET_MSG_12   12
#define PRT_BITMAP_IMAGE_1  21
#define PRT_BITMAP_IMAGE_2  22
#define PRT_BITMAP_IMAGE_3  23
#define PRT_BITMAP_IMAGE_4  24
#define PRT_BITMAP_IMAGE_5  25
#define PRT_BARCODE_1       31
#define PRT_BARCODE_2       32
#define PRT_BARCODE_3       33
#define PRT_BARCODE_4       34
#define PRT_BARCODE_5       35
#define PRT_BARCODE_6       36
#define PRT_BARCODE_7       37
#define PRT_BARCODE_8       38
#define PRT_BARCODE_9       39
#define PRT_BARCODE_10      40
#define PRT_CLERK_NAME      41
#define PRT_MARKET_INFO     42
#define PRT_SHOP_INFO       43
#define PRT_SCALE_INFO      44
#define PRT_TRACE_INFO      45
#define PRT_PLU_NAME_1      51
#define PRT_PLU_NAME_2      52
#define PRT_PLU_NAME_3      53
#define PRT_PLU_DATA        54
#define PRT_WEIGHT_UPRICE_TPRICE    61
#define PRT_SUBTOTAL        71
#define PRT_PAY_CASH        72
#define PRT_PAY_CREDIT      73
#define PRT_PAY_CHECK       74
#define PRT_PAY_BONUS_POINT 75
#define PRT_PAY_CHANGE      76
#define PRT_TAX             81
#define PRT_DATETIME        82
#define PRT_TRANSACTION_COUNT   83
#define PRT_REPEAT_LINE_START   91
#define PRT_REPEAT_LINE_END     92
#define PRT_SEPARTOR_1      93
#define PRT_SEPARTOR_2      94
#define PRT_SEPARTOR_3      95
#define PRT_SPACE_LINE      96

#define RECEIPT_FIELD_QTY       100
#define RECEIPT_FIELD_SIZE      24
#define RECEIPT_FIELD_HEADER    2

#define CMD_PRT_DRV_START_PRINT     0
#define CMD_PRT_DRV_START_SENSOR    1
#define CMD_PRT_DRV_RD_TPH_PAPER    3
#define CMD_PRT_DRV_RD_TPH_TEMP     4
#define CMD_PRT_DRV_MOD_INTENSITY   5
#define CMD_PRT_DRV_MOD_SPEED       6
#define CMD_PRT_DRV_RD_STATUS       7
#define CMD_PRT_DRV_RD_BATT_VOLT    8
#define CMD_PRT_DRV_RD_VER	    9

#define CPU_CARD                1
#define M1_CARD                 2
#define RFID_READ_TOTAL         1
#define RFID_READ_CUSTOM_NO     2
#define RFID_COMPARE_CUSTOM_NO 3

//extern uint16 getvolt(void);
extern bit checkPaper(void);
extern unsigned short GetTphAD(void);
extern void setPrintSpeed(uint8 speed);
extern void setDarkness(uint8 darkness);
extern char* get_ver_dev_printer(void);

#define TPH_LINE_BYTES		48
#define TPH_DOTS_PER_LINE	24 //12x24
#define PRT_MAX_LENGTH		1000
extern uint16 prt_line;
extern char patternBuf[TPH_LINE_BYTES*TPH_DOTS_PER_LINE*PRT_MAX_LENGTH];
extern char patternChessBuf[TPH_LINE_BYTES*TPH_DOTS_PER_LINE*PRT_MAX_LENGTH];//TODO: modify chess function
extern char patternFeedBuf[TPH_LINE_BYTES*TPH_DOTS_PER_LINE*PRT_MAX_LENGTH];//TODO: modify feed function
typedef struct {
    uint8 Datatype;
    uint8 Filed_id;
    uint8 Font_id;
    uint8 Typeface;
    uint8 Justification;
    uint8 Magx;
    uint8 Magy;
    uint8 Reserved[17];
} STR_STRUCT;

typedef struct {
    uint8 Datatype;
    uint8 Filed_id;
    uint8 BarType;
    uint8 BarHeight;
    uint8 BarWidthMag;
    uint8 Reserved[19];
} BAR_FIELD_STRUCT;

typedef struct {
    uint8 Datatype;
    uint8 Filed_id;
    uint8 Justification;
    //INT16U Width;
    //INT16U Height;
    uint8 Reserved[21];
} BMP_STRUCT;

extern STR_STRUCT StrStruct;
extern BAR_FIELD_STRUCT BarStruct;
extern BMP_STRUCT BitmapStruct;

extern void Open_Dev_Prt_ct500();
extern void Close_Dev_Prt_ct500(void);
extern byte bool2char(bit b);//return Y or N


extern void startreport(void);
extern void endreport(void);
extern void print_str(uint8 *src, uint32 size);
extern void printline(void);
extern void printstr(byte* s);
extern void printsettings(void);
extern void printartinfo(int i);
extern void printartrep(int i, long atotal);
extern void printstrbool(const char *x, bit b);
extern void PrintStrNum(byte *x, long l);
extern void PrintStrNumZ(byte *x, long l);
extern void PrtTraceCode(unsigned char *p);
extern void tireta(void);
extern void rep_log_content(uint16 i);
extern void rep_vendor(bit p);
extern void test_chinese(void);
extern void test_print(void);
extern void test_print_chess(void);
extern void paperpush();
extern void print_start(void);
extern void print_start_null(void);
extern void print_feed_start(void);//TODO: modify feed function
extern void MakeInitPattern(void);
extern void MakePattern(uint16 s_line);
extern void MakeFeedPattern(void);///TODO: modify feed function
extern void MakeFontPattern(uint16 s_line);
extern void MakeBarPattern(uint16 s_line);
extern void MakeImgPattern(uint16 s_line);
//KYK

//extern long getBatAd(void);
extern long getTemp(void);
extern long getNoPaper(void);

#endif /* _PRINT_H_ */
