#ifndef _SET_H_
#define _SET_H_
#include "./../public/globals.h"
#include <stdio.h>
#include <math.h>

void print();


char prnbuf[512];
typedef union
{
    vu8 DATA;
    struct
    {
        volatile unsigned b0:1;
        volatile unsigned b1:1;
        volatile unsigned b2:1;
        volatile unsigned b3:1;
        volatile unsigned b4:1;
        volatile unsigned b5:1;
        volatile unsigned b6:1;
        volatile unsigned b7:1;
    } Bit;
} SFR8;
typedef struct
{

    struct
    {
        uint8 print : 1;       // print ticker
        uint8 dblprint : 1;    // print duplicate
        uint8 imgprint : 1;    // print barcode
        uint8 BarSingle : 1;   // print barcode
        uint8 BarTotal : 1;    // print barcode
        uint8 oblpay : 1;      // obligatory payment
        uint8 proce : 1;       // % enable
        uint8 cod : 1;         // print code
    } f;
    uint8 BarSingle[17];     // barcode format ASCIIZ strings
    uint8 BarTotal[17];      // barcode format ASCIIZ strings
    uint8 com1baud;         // rs baudrate 1200-115200
    uint8 lan;               // lan number 0..99
    uint16 autooff;          // KYK word->INT16U
    uint8 procM;             // default %-
    uint8 chekmedje;         // in ms
    uint8 dark;              // darkness 1,2,3,4,5 normal 3
    uint8 valute[4];

    struct
    {
        uint8 printallrep : 1;  // print all reports when CLOSE Z
        uint8 prntax : 1;       // print tax enable or disable
        uint8 backlight : 1;    // 0: Don't Use Backlight, 1: Use Backlight
        uint8 SavePLU : 1;      // 0: Don't Save PLU, 1: Save PLU
        //INT8U useTrace : 1;     // 0: Don't use, 1: use
        uint8 allowForceSale : 1;     // 0: Don't use, 1: use
    } f1;
    uint8 traceType;
    uint16 MarketNo;
    uint16 ShopNo;
    uint16 scaleNo;
    uint32 com2baud;    //com2 rs baudrate 1200-115200
    uint8 receiptNo;    //receipt format no
    uint8 engFontNo;
    uint8 chnFontNo;
} SALE_SET_STRUCT;
 SALE_SET_STRUCT settings;

//int PRT_Driver.c  line37
#define FLASH_SCALE_BITMAP_SIZE         (256*48)
#include <sys/ioctl.h>


#define MAX_CHAR 32
#define mes_on "是"
#define  mes_off "否"
#define pricedot 2

byte nopaper;




#endif
