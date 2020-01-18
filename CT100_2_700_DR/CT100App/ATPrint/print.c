///*****************************************************************************
/*
|*  Copyright       :   (c) 2010 CAS
|*  Filename        :   print.c
|*  Version         :   0.1
|*  Programmer(s)   :   Keum Young-Kwang (KYK)
|*  Created         :   2010/11/01
|*  Modified        :
|*  Description     :   CT100 printer function
*/
//****************************************************************************/

#include "./../public/globals.h"
#include "print.h"
#include <fcntl.h>
#include "set.h"
#include "string.h"
#include "stdlib.h"
#include <unistd.h>
#define MAX_TEST_ITEM 18
uint32 power10(int n)
{
    switch (n)
    {
    case 0:
        return 1L;
    case 1:
        return 10L;
    case 2:
        return 100L;
    case 3:
        return 1000L;
    case 4:
        return 10000L;
    case 5:
        return 100000L;
    case 6:
        return 1000000L;
    case 7:
        return 10000000L;
    case 8:
        return 100000000L;
    case 9:
        return 1000000000L;
    default :
        return 1000000L;
    }
}

char ft_name[MAX_TEST_ITEM][20] =
{
    "GRAPHIC LCD",      // 0
    "SOUND",            // 1
    "7-SEG LCD",        // 2
    "WLAN",             // 3
    "LAN",              // 4
    "COM1",             // 5
    "COM2",             // 6
    "COM3",             // 7
    "USB KEYBOARD1",    // 8
    "USB KEYBOARD2",    // 9
    "KEYPAD",           // 10
    "CASHDRAWER",       // 11
    "A/D ZERO",         // 12
    "A/D FULL",         // 13
    "BAT A/D",          // 14
    "AC CHECK",         // 15
    "PRINTER",          // 16
    "RTC"               // 17
};
char ft_result_str[3][10] = {"N/A", "PASS", "FAIL"};
int dottype = 1;
VERSION_STRUCT fw_version =
{
    0,		//Class
    100,	//Model Num     //CL5000JR = 5010, CT100 = 100
    1,		//type Bench    //1 = Bench, 2 = Pole, 3 = Hanging, 4 = Self	//modify when request
    0,		//Scale ID		//modify when request
    1,		//Main Version x.x
    02,		//Sub Version     .x
    0x0100,	//Data Version  //high byte = FLASH STRUCT VERSION, low byte = EEPROM STRUCT VERSION
    0,		//Country no (not used)
    //0,//MEM_TYPE,
    //0,//BARCODE_SET,
    {
        COUNTRY_CODE0,	//CountryCode[0]
        COUNTRY_CODE1	//CountryCode[1]
    }
};

STR_STRUCT StrStruct;
BAR_FIELD_STRUCT BarStruct;
BMP_STRUCT BitmapStruct;

#ifdef USE_CT100G
struct driver_ver prt_ver;
char patternBuf[TPH_LINE_BYTES*TPH_DOTS_PER_LINE*PRT_MAX_LENGTH];
char patternChessBuf[TPH_LINE_BYTES*TPH_DOTS_PER_LINE*PRT_MAX_LENGTH];
char patternFeedBuf[TPH_LINE_BYTES*TPH_DOTS_PER_LINE*PRT_MAX_LENGTH];//TODO: modify feed function


void paperpush()
{
#ifndef USE_CT100G
    sprintf((char*)prnbuf, "\n\n\n\n"); //test
    print();
#else
    sprintf((char*)prnbuf, "\n"); //test
    print();
    sprintf((char*)prnbuf, "\n"); //test
    print();
    sprintf((char*)prnbuf, "\n"); //test
    print();
    sprintf((char*)prnbuf, "\n"); //test
    print();
#endif
}
const unsigned char test_buf[60] =
{
    0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
    0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
    0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
    0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
    0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
    0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
};
//********************************************************************

void digit_to_string(uint8 *cbuf, long value, uint8 dot, uint8 leng)
{
    char typeDp1[10][6] = {"%0ld", "%1ld", "%2ld", "%3ld", "%4ld", "%5ld", "%6ld", "%7ld", "%8ld", "%9ld"};
    char typeDp2[10][6] = {"%00ld", "%01ld", "%02ld", "%03ld", "%04ld", "%05ld", "%06ld", "%07ld", "%08ld", "%09ld"};
    uint8 Q[10], R[10];
    uint32 sub, quo, rest;
    uint8 dotChar;
    uint8 tmp;
    uint8 sign;
    int32 lvalue;

    sign = PLUS;
    lvalue = value;

    if (value < 0)
    {
        sign = MINUS;
        lvalue = labs(value);
    }

    if(dot)
    {
        if(dottype) dotChar = ',';
        else dotChar = '.';

        sub = power10((int)dot);
        quo = lvalue / sub;
        rest = lvalue % sub;

        sprintf((char *)Q, typeDp1[leng-dot], quo);
        sprintf((char *)R, typeDp2[dot], rest);
        sprintf((char *)cbuf, "%s%c%s", Q, dotChar, R);
        if (sign == MINUS)
        {
            sprintf((char *)cbuf, "%s%c%s", Q, dotChar, R);
            tmp = strlen((char const *)Q);

            if (quo < 10)      tmp -= 3;
            if (quo >= 10     && quo < 100)     tmp -= 4;
            if (quo >= 100    && quo < 1000)    tmp -= 5;
            if (quo >= 1000   && quo < 10000)   tmp -= 6;
            if (quo >= 10000  && quo < 100000)  tmp -= 7;
            if (quo >= 100000 && quo < 1000000) tmp -= 8;
            cbuf[tmp] = '-';
        }
    }
    else
    {
        sprintf((char *)cbuf, typeDp1[leng], lvalue);
        tmp = strlen((char const *)cbuf);
        if (sign == MINUS)
        {
            if (lvalue >= 0      && lvalue < 10)      tmp -= 2;
            if (lvalue >= 10     && lvalue < 100)     tmp -= 3;
            if (lvalue >= 100    && lvalue < 1000)    tmp -= 4;
            if (lvalue >= 1000   && lvalue < 10000)   tmp -= 5;
            if (lvalue >= 10000  && lvalue < 100000)  tmp -= 5;
            if (lvalue >= 100000 && lvalue < 1000000) tmp -= 5;
            cbuf[tmp] = '-';
        }
    }
}

void MakeChessPattern()
{
    uint32 bufPtr;
    int line_counter;
    int buf[1];

    buf[0] = 0;

    bufPtr = 0;
    for (line_counter = 0; line_counter < 8*500; line_counter++)
    {
        //chess pattern
        if ((line_counter % 16) < 8)
        {
            memcpy(&patternChessBuf[bufPtr], &test_buf[0], 48);
        }
        else
        {
            memcpy(&patternChessBuf[bufPtr], &test_buf[1], 48);
        }
        bufPtr += 48;
    }
}
//********************************************************************
void MakeInitPattern(void)
{
    uint32 bufPtr;
    int line_counter;

    bufPtr = 0;
    for (line_counter = 0; line_counter < (TPH_DOTS_PER_LINE * PRT_MAX_LENGTH); line_counter++)
    {
        memset(&patternBuf[bufPtr], 0, TPH_LINE_BYTES);
        memset(&patternChessBuf[bufPtr], 0, TPH_LINE_BYTES);
        memset(&patternFeedBuf[bufPtr], 0, TPH_LINE_BYTES); //TODO: modify feed function
        bufPtr += TPH_LINE_BYTES;
    }
}
//********************************************************************
#if 0
#define FONT_24x24_WIDTH_BYTE   24  //8byte * 3byte
#define FONT_16x16_WIDTH_BYTE   16  //8byte * 2byte
#define TPH_WIDTH_BYTE      48  //2inch=384 dot
#define FONT_TYPE_NORMAL    1
#define FONT_TYPE_DOUBLE    2
#define FONT_TYPE_BARCODE   3
void MakePrtPattern(INT16U s_line, INT8U mode, INT8U barHeight)
{
    //barHeight: 1 = 0.125mm, 8 = 1 mm
    INT32U bufPtr;
    INT16U line_counter;
    INT8U e_line;
    INT8U tmpStr[48*2];

    memset(tmpStr, 0x00, sizeof(tmpStr));

    bufPtr = 0;
    e_line = 24;
    if (mode == FONT_TYPE_DOUBLE)
    {
        e_line *= 2;
        prt_line++;
    }
    switch(mode)
    {
    case FONT_TYPE_NORMAL: //normal, bold,
    case FONT_TYPE_DOUBLE:
        for (line_counter = 0; line_counter < e_line; line_counter++)//font size: 24x24
        {
            memcpy(&patternBuf[(24*TPH_WIDTH_BYTE)*s_line + bufPtr], &prt_img_buffer->prt_addr[bufPtr], TPH_WIDTH_BYTE);
            memset(&prt_img_buffer->prt_addr[bufPtr], 0, TPH_WIDTH_BYTE);
            bufPtr += TPH_WIDTH_BYTE;
        }
        /*
        for (line_counter = 0; line_counter < e_line; line_counter++)//font size: 24x24
        {
            if (line_counter >= 8*3)
            {
                memcpy(&patternBuf[((8*3+3)*48)*s_line + bufPtr], tmpStr, 48*height);
            }
            else
            {
                memcpy(&patternBuf[((8*3+3)*48)*s_line + bufPtr], &prt_img_buffer->prt_addr[bufPtr], 48*height);
            }
        }
        */
        break;
    case FONT_TYPE_BARCODE:
        for (line_counter = 0; line_counter < e_line*barHeight; line_counter++)//font size: 24x24
        {
            //memcpy(&patternBarBuf[bufPtr], &Bar_PrtDat, 48);
            memcpy(&patternBuf[(24*TPH_WIDTH_BYTE)*(s_line-1) + bufPtr], &Bar_PrtDat, TPH_WIDTH_BYTE);
            bufPtr += TPH_WIDTH_BYTE;
        }
        break;
        //case DATA_TYPE_IMAGE:
        //    break;

    default:
        break;
    }
}
#endif
//********************************************************************
void MakePattern(uint16 s_line)
{
    uint32 bufPtr;
    int line_counter;
    //INT8U tmpStr[48];

    bufPtr = 0;

    for (line_counter = 0; line_counter < TPH_DOTS_PER_LINE; line_counter++)//font size: 24x24
    {
        memcpy(&patternBuf[(TPH_DOTS_PER_LINE*TPH_LINE_BYTES)*s_line + bufPtr], &prt_img_buffer->prt_addr[bufPtr], TPH_LINE_BYTES);
        memset(&prt_img_buffer->prt_addr[bufPtr], 0, TPH_LINE_BYTES);
        bufPtr += TPH_LINE_BYTES;
    }
    memset(prnbuf, 0, sizeof(prnbuf));
}
//********************************************************************
void MakeFontPattern(uint16 s_line)
{
    uint32 bufPtr;
    int line_counter;
    uint8 height;

    bufPtr = 0;
    height = 2; //font height dobule

    for (line_counter = 0; line_counter < TPH_DOTS_PER_LINE*height; line_counter++)//font size: 24x24
    {
        memcpy(&patternBuf[(TPH_DOTS_PER_LINE*TPH_LINE_BYTES)*s_line + bufPtr], &(prt_img_buffer->prt_addr[bufPtr]), TPH_LINE_BYTES);
        memset(&prt_img_buffer->prt_addr[bufPtr], 0, TPH_LINE_BYTES);
        bufPtr += TPH_LINE_BYTES;
    }
    prt_line++;
    memset(prnbuf, 0, sizeof(prnbuf));
}
//********************************************************************
void MakeFeedPattern(void)
{
    uint32 bufPtr;
    int line_counter;
    int feed_line_max;

    bufPtr = 0;
    feed_line_max = 1;

    for (line_counter = 0; line_counter < TPH_DOTS_PER_LINE*feed_line_max; line_counter++)//font size: 24x24
    {
        memcpy(&patternFeedBuf[(TPH_DOTS_PER_LINE*TPH_LINE_BYTES) + bufPtr], &(prt_img_buffer->prt_addr[bufPtr]), TPH_LINE_BYTES);
        memset(&prt_img_buffer->prt_addr[bufPtr], 0, TPH_LINE_BYTES);

        bufPtr += TPH_LINE_BYTES;
    }
}
//********************************************************************
void MakeBarPattern(uint16 s_line)
{
    uint32 bufPtr;
    int line_counter;

    bufPtr = 0;
    for (line_counter = 0; line_counter < TPH_DOTS_PER_LINE*s_line; line_counter++)//font size: 24x24
    {
        //memcpy(&patternBarBuf[bufPtr], &Bar_PrtDat, 48);
        memcpy(&patternBuf[(TPH_DOTS_PER_LINE*TPH_LINE_BYTES)*(prt_line-1) + bufPtr], &Bar_PrtDat, TPH_LINE_BYTES);
        bufPtr += TPH_LINE_BYTES;
    }
}
//********************************************************************
void MakeImgPattern(uint16 s_line)
{
    uint32 bufPtr;
    int line_counter;

    bufPtr = 0;
    for (line_counter = 0; line_counter < TPH_DOTS_PER_LINE*s_line; line_counter++)//font size: 24x24
    {
        memcpy(&patternBuf[(TPH_DOTS_PER_LINE*TPH_LINE_BYTES)*(prt_line) + bufPtr], &print_oneline, TPH_LINE_BYTES);
        bufPtr += TPH_LINE_BYTES;
    }
}
//********************************************************************
void MOTOR_CommandList( void )
{
    printf( "\n" );
    printf( "# [0] Motor Stop \n" );
    printf( "# [1~9] Print Chess 1,2,5,10,20,50,100,300,500(mm)\n" );
    printf( "# [s] Check Sensor \n" );
    printf( "# [t] Read temp sensor \n" );
    printf( "# [p] Read paper sensor \n" );
    printf( "# [a] Set Intensity 0 \n" );
    printf( "# [b] Set Intensity 10 \n" );
    printf( "# [c] Set Intensity 20 \n" );
    printf( "# [j] Set Speed 75 \n" );
    printf( "# [k] Set Speed 60 \n" );
    printf( "# [l] Set Speed 40 \n" );
    printf( "# [n] Read Printer Status(0,1,2) \n" );
    printf( "# [q] Quit\n" );
    printf( "-----------------------------------\n" );
}
//********************************************************************
unsigned long TestLength[10] = //0, 1, 2, 5, 10, 20, 50, 100, 300, 500(mm)
{
    0*48*8*10, 48*8*1, 48*8*2, 48*8*5, 1*48*8*10,
    2*48*8*10, 5*48*8*10, 10*48*8*10, 30*48*8*10, 50*48*8*10
};
static int prt_ct500_fd;
void Open_Dev_Prt_ct500()
{
    prt_ct500_fd = open("/dev/ct100gc_printer", O_WRONLY|O_NDELAY );
    if(prt_ct500_fd < 0)
    {
        printf( "Printer Drive Open Fail\n");
        exit(1);
    }
    printer_init();
    printf( "Printer Process Starting\n" );
}
void Close_Dev_Prt_ct500(void)
{
    close(prt_ct500_fd);
}
//********************************************************************
uint16 prt_line = 0;
char* get_ver_dev_printer(void)
{
    ioctl(prt_ct500_fd, CMD_PRT_DRV_RD_VER, &prt_ver);
    return &prt_ver.str[0];
}
void print_start()
{
    /*
        write(prt_ct500_fd, patternBuf, (48*8*3+3)*prt_line);//space height = 3 dot
        fsync(prt_ct500_fd);//TODO : check needing this func.
        ioctl(prt_ct500_fd, 0, (48*8*3+3)*prt_line);
    */
    write(prt_ct500_fd, patternBuf, TPH_LINE_BYTES*TPH_DOTS_PER_LINE*prt_line);
    fsync(prt_ct500_fd);//TODO : check needing this func.
    ioctl(prt_ct500_fd, 0, TPH_LINE_BYTES*TPH_DOTS_PER_LINE*prt_line);
}
void print_start_null()
{
    write(prt_ct500_fd, patternBuf, TPH_LINE_BYTES*TPH_DOTS_PER_LINE*prt_line);
}
void print_feed_start()
{
    write(prt_ct500_fd, patternFeedBuf, TPH_LINE_BYTES*TPH_DOTS_PER_LINE);
    fsync(prt_ct500_fd);//TODO : check needing this func.
    ioctl(prt_ct500_fd, 0, TPH_LINE_BYTES*TPH_DOTS_PER_LINE);
}

//********************************************************************
/*uint16 getBatAd(void)
{
    long tmp32;

    //wait until Checking sensor complete
    //TODO : need timeout
    ioctl(prt_ct500_fd, CMD_PRT_DRV_START_SENSOR, 10);//start sensor data
    tmp32 = ioctl(prt_ct500_fd, CMD_PRT_DRV_RD_BATT_VOLT, 0); //battery volt

    return (uint16)tmp32;
}*/
//********************************************************************
bit checkPaper(void)
{
#ifndef USE_CT100G
    INT8U tmp = 0;
    nopaper = 0;

    if (settings.set3.UseNoPaper)
    {
        tmp = NO_PAPER;
        if (tmp)
        {
            nopaper = 1;
        }
    }
    return (uint16)nopaper;
#else
    long ret_value;

    //wait until Checking sensor complete
    //TODO : need timeout
    ret_value = -1;
    int key = 0;
    if (key == 0)
    {
        ioctl(prt_ct500_fd, CMD_PRT_DRV_START_SENSOR, 10);//start sensor data
        while (ret_value == -1) ret_value = ioctl(prt_ct500_fd, CMD_PRT_DRV_RD_TPH_PAPER, 0); //paper
//measurement data: no paper < 10 cnt
//printf("read paper sensor adc: %d\n", (unsigned int)ret_value);
    }
    return ret_value; //TODO: check data type
#endif //#ifndef USE_CT100G
}
//********************************************************************
unsigned short GetTphAD(void)
{
    long ret_value;

    //wait until Checking sensor complete
    //TODO : need timeout
    ret_value = -1;
    int key =0;
    if (key == 0) //test
    {
        ioctl(prt_ct500_fd, CMD_PRT_DRV_START_SENSOR, 10);//start sensor data
        while (ret_value == -1) ret_value = ioctl(prt_ct500_fd, CMD_PRT_DRV_RD_TPH_TEMP, 0); //tph temperature
//TODO: make temperature convert table
//printf("read temp adc: %d\n", (unsigned int)ret_value);
    }
    return ret_value;
}
//********************************************************************
unsigned SpeedTimerTable[5] = //75, 70, 60, 50, 40 (mm/s)
{
    //9164, 9802, 11450, 13736, 17164
    75, 70, 60, 50, 40
};
void setPrintSpeed(uint8 speed)
{
    long ret_value;

    //TODO : need timeout
    ret_value = -1;
    int key =0;
    if (key == 0)
    {
        ioctl(prt_ct500_fd, CMD_PRT_DRV_MOD_SPEED, speed);
//printf("Set Speed: %d\n", (unsigned int)SpeedTimerTable[speed]);
    }
    //return ret_value;
}
//********************************************************************
#define SPEED_75        0
#define SPEED_70        1
#define SPEED_60        2
#define SPEED_50        3
#define SPEED_40        4
#define DARK_LEVEL_1    0
#define DARK_LEVEL_2    1
#define DARK_LEVEL_3    2
#define DARK_LEVEL_4    3
#define DARK_LEVEL_5    4
void setDarkness(uint8 darkness)
{
    uint8 dark;
    uint8 speed;
    //if (key == _KEY_ADD)
    {

        switch(darkness)
        {
        case 1: //light
            dark = DARK_LEVEL_1;
            speed = SPEED_75;
            break;
        case 2:
            dark = DARK_LEVEL_2;
            speed = SPEED_75;
            break;
        case 3:
            dark = DARK_LEVEL_2;
            speed = SPEED_70;
            break;
        case 4:
            dark = DARK_LEVEL_3;
            speed = SPEED_60;
            break;
        case 5: //thicker
            dark = DARK_LEVEL_4;
            speed = SPEED_50;
            break;
        default:
            break;
        }
        ioctl(prt_ct500_fd, CMD_PRT_DRV_MOD_INTENSITY, dark);
        ioctl(prt_ct500_fd, CMD_PRT_DRV_MOD_SPEED, speed);

//printf("Set Intensity: %d\n", (unsigned int)dark);
//printf("Set Speed: %d\n", (unsigned int)SpeedTimerTable[speed]);
    }
}
//********************************************************************
int linux_print_test()
{
//	int     dev;
    int     i;
    char	key_control;
    char	buf[2];
    long ret_value;

    MakeInitPattern();
    MOTOR_CommandList();
    while(1)
    {
        i = read(4, buf, 1);//test
        if (i == 0)
        {
            key_control = 0x00;
        }
        else
        {
            key_control = buf[0];
        }

        if (key_control == 'q')
        {
            break;
        }
        else if (key_control == '0')
        {
            write(prt_ct500_fd, patternBuf, 0);
            fsync(prt_ct500_fd);//TODO : check needing this func.
            ioctl(prt_ct500_fd, 0, (unsigned long)key_control-'0');
        }
        else if (key_control >= '1' && key_control <= '9')
        {
            //test
           // print_receipt();
            //print_barcode(); //test
            //print_image();
            /*
            printbar("a4W562D2C19",1550,15,33);	//EAN13
            write(prt_ct500_fd, Bar_PrtDat, 48*8*3*5);//patternBuf
            fsync(prt_ct500_fd);//TODO : check needing this func.
            ioctl(prt_ct500_fd, 0, 48*8*3*5);
            */

        }
        else if (key_control == 's')
        {
            ioctl(prt_ct500_fd, 1, 10);
        }
        else if (key_control == 't')
        {
            //TODO : why does not work number 2 (app)
            //ret_value = ioctl(dev, 2, 0);

            ret_value = -1;
            //wait until Checking sensor complete
            //TODO : need timeout
            while (ret_value == -1) ret_value = ioctl(prt_ct500_fd, 4, 0);
            printf("read temp adc: %d\n", (unsigned int)ret_value);
        }
        else if (key_control == 'p')
        {
            ret_value = -1;
            //wait until Checking sensor complete
            //TODO : need timeout
            while (ret_value == -1) ret_value = ioctl(prt_ct500_fd, 3, 0);
            printf("read paper adc: %d\n", (unsigned int)ret_value);
        }
        else if (key_control == 'a')
        {
            ioctl(prt_ct500_fd, 5, 0);
            printf("Set Intensity: %d\n", (unsigned int)0);
        }
        else if (key_control == 'b')
        {
            ioctl(prt_ct500_fd, 5, 10);
            printf("Set Intensity: %d\n", (unsigned int)10);
        }
        else if (key_control == 'c')
        {
            ioctl(prt_ct500_fd, 5, 20);
            printf("Set Intensity: %d\n", (unsigned int)20);
        }
        else if (key_control == 'j')
        {
            ioctl(prt_ct500_fd, 6, 0);
            printf("Set Speed: %d\n", (unsigned int)0);
        }
        else if (key_control == 'k')
        {
            ioctl(prt_ct500_fd, 6, 2);
            printf("Set Speed: %d\n", (unsigned int)2);
        }
        else if (key_control == 'l')
        {
            ioctl(prt_ct500_fd, 6, 4);
            printf("Set Speed: %d\n", (unsigned int)4);
        }
        else if (key_control == 'n')
        {
            ret_value = ioctl(prt_ct500_fd, 7, 0);
            printf("read Printer Status num: %d\n", (unsigned int)ret_value);
        }
    }

    //close(dev);
    printf( "Printer Process Ending\n" );

    return 0;
}
#endif
/*******************************************************************************
* Function Name  :
* Description    : Print Traceability Code
* Input          : Traceability Code NUM
* Output         : None
* Return         : None
*******************************************************************************/

//********************************************************************

 void print(void)
{
    print_str((uint8 *)prnbuf, strlen(prnbuf));
    MakePattern(prt_line++);
}

//********************************************************************
void tireta()
{
    sprintf(prnbuf, "\n");
    print();
}
//********************************************************************
void printline()
{
#if defined(USE_CHN_GB2312_24x24) || defined(USE_ENG_12x24)
      sprintf(prnbuf, "********************************\n");
    //sprintf(prnbuf, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
#elif defined(USE_CHN_GB2312_16x16) || defined(USE_ENG_8x16)
      sprintf(prnbuf, "************************************************\n");
    //sprintf(prnbuf, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
#endif
    print();
}
//********************************************************************
char * pindex;
void printstr(byte* s)
{
    pindex = prnbuf;
   // paddsx((char *)s, MAX_CHAR);
    print();
}
//********************************************************************
byte bool2char(bit b)
{
    return b ? 'Y' : 'N';
}
//********************************************************************
void printstrbool(const char *x, bit b)
{

#ifdef COUNTRY_CHINA
#ifdef USE_CHN_GB2312_24x24
    sprintf(prnbuf, "%-28s%4s\n", x, b ? mes_on : mes_off);
#elif defined(USE_CHN_GB2312_16x16)
    sprintf(prnbuf, "%-38s%4s\n", x, b ? mes_on : mes_off);
#endif
#else
    sprintf(prnbuf, "%-28s%4s\n", x, b ? mes_on : mes_off);
#endif

    print();
}
//********************************************************************
void PrintStrNum(byte *x, long l)
{
    uint8 str[16];

    digit_to_string(str, l, pricedot, TOTAL_DIGIT);
#ifdef COUNTRY_CHINA
#ifdef USE_CHN_GB2312_24x24
    sprintf(prnbuf, "%-8s%23s\n", x, str);
#elif defined(USE_CHN_GB2312_16x16)
    sprintf(prnbuf, "%-6s%35s\n", x, str);
#endif
#else
    sprintf(prnbuf, "%-6s%26s\n", x, str);
#endif
    print();
}
//********************************************************************
void PrintStrNumZ(byte *x, long l)
{
    if (l != 0) PrintStrNum(x, l);
}
//********************************************************************

void printsettings(){
printf("settings\n");
}
//********************************************************************
void test_chinese(void)
{
    sprintf((char*)prnbuf,"abcd     ABCDEF                 XYZ1234567890%c%c%c%c%c%c%c%c%c%c      123   467",
            0xC9,0xCC,0xC6,0xB7,0xCF,0xFA,0xCA,0xDB,0xB1,0xA8);
    print();
}
//********************************************************************

//********************************************************************
//void test_rfid_read_card(INT32U addr, INT16U size, INT8U *str)

//********************************************************************

void print_chess()
{
    MakeChessPattern();

    write(prt_ct500_fd, patternChessBuf, TestLength[6]); //50mm
    fsync(prt_ct500_fd);//TODO : check needing this func.
    ioctl(prt_ct500_fd, 0, TestLength[6]);

    MakeInitPattern();
}
//********************************************************************
void test_print(void)
{
    //byte i,j;
    checkPaper();
    if(nopaper)
    {
     printf("do beeperror%d\n",__LINE__);
        return;
    }
#ifdef TEST_CHESS
    prt_mode=0; //1 正常打印模式 ;0 测试打印模式
    prt_flag=1;
    sprintf((char*)&prt_buf[prt_write],"Printing chess...\n");
    prt_write=(unsigned int) strlen((char*)prt_buf);
    start_prt();
    //ch_exist=1;
#else
    printline();
    //image
   // if (settings.f.imgprint) ImgPrt();
    // version
    font_type(FontNO);    //type= FontNO,FontBOLD,FontWIDE,FontHIGH,FontBIG,FontUNDERLINE,FontSTRIKETHROUGH
    sprintf((char *)prnbuf, "TEST PRINT wjxbelieve");
    print();
    tireta();

    // clock

    // settings

    // izbutvane
   paperpush();
   // if (settings.f.imgprint) ImgPrt();

    // version
    //---------------------------
    //PrtTraceCode("1234567890"); for test
    //---------------------------
    font_type(FontNO);    //type= FontNO,FontBOLD,FontWIDE,FontHIGH,FontBIG,FontUNDERLINE,FontSTRIKETHROUGH
    sprintf((char *)prnbuf,"model: CL5000R %c%cV%d.%d\n", fw_version.CountryCode[0], fw_version.CountryCode[1],
            fw_version.MainVersion, fw_version.SubVersion);
    print();
    tireta();



   // KYK
        // chargen
     /*   const char mes_st2[] = "字表";
        sprintf((char *)prnbuf,"%s\n", mes_st2);
        print();
        int i;int j;
        for(j = 32; j != 0; j += MAX_CHAR)
        {
            for(i = 0; i != MAX_CHAR; i++) prnbuf[i] = i + j;
            if(j == 96)
            {
                prnbuf[MAX_CHAR]=127;
                prnbuf[MAX_CHAR-1]='~';
            }
            print();
        }
        tireta();
*/
    /*
        // powerfail
        pabs(prnbuf);
        pprintf("%24s%1a%6Bms", mes_st3, pfailtime);
        print();
    */
    /*
        // voltage
        if(en220)
        {
            pabs(prnbuf);
            pprintf("%24s%1a%7.1BV", mes_st4, getvolt()+7);
            print();
        }
    */
    /*
        // temperature
        pabs(prnbuf);
        pprintf("%24s%1a%6b'C", mes_st5, gettemp());
           //sprintf((char*)prnbuf, "\n\n\n\n\n\n"); //distance tph and receipt cutter
#ifndef USE_CT100G
    sprintf((char*)prnbuf, "\n\n\n\n"); //test
    print();
#else
    sprintf((char*)prnbuf, "\n"); //test
    print();
    sprintf((char*)prnbuf, "\n"); //test
    print();
    sprintf((char*)prnbuf, "\n"); //test
    print();
    sprintf((char*)prnbuf, "\n"); //test
    print();
#endif
    */
    // settings
    printsettings();

    // izbutvane
        //sprintf((char*)prnbuf, "\n\n\n\n\n\n"); //distance tph and receipt cutter
#ifndef USE_CT100G
    sprintf((char*)prnbuf, "\n\n\n\n"); //test
    print();
#else
    sprintf((char*)prnbuf, "\n"); //test
    print();
    sprintf((char*)prnbuf, "\n"); //test
    print();
    sprintf((char*)prnbuf, "\n"); //test
    print();
    sprintf((char*)prnbuf, "\n"); //test
    print();
#endif

    print_start();
    MakeInitPattern();
    prt_line = 0;
#endif//USE_PRINT_JIG
}
//********************************************************************
void test_print_chess(void)
{
    //byte i,j;
    checkPaper();
    if(nopaper)
    {
        printf("do beep error %d\n",__LINE__);
                return;
    }
    print_chess(); //test chess
    printf("after print chess\n");
}
//********************************************************************
void print_str(uint8 *src, uint32 size)
{
    char  count = 0;

    do
    {
        prt_write = 0;
        count = (size>512)?512:size;
        memcpy((char*)&prt_buf[prt_write],src,count);
        size -= count;
        src += count;
        prt_buf[count] = '\0';
        prt_write = (unsigned int) strlen((char*)prt_buf);
        start_prt();
    }
    while(size>0);
}

//读温度传感器
long getTemp(void)
{

	long tmp32;
    //wait until Checking sensor complete
    //TODO : need timeout
    ioctl(prt_ct500_fd, CMD_PRT_DRV_START_SENSOR, 10);//start sensor data
    tmp32 = ioctl(prt_ct500_fd, CMD_PRT_DRV_RD_TPH_TEMP, &tmp32); //battery volt
	//printf("getBatAd from prt_ct500_fd");
    return tmp32;

}

//NO PAPER
long getNoPaper(void)
{

	long tmp32;
    //wait until Checking sensor complete
    //TODO : need timeout
    ioctl(prt_ct500_fd, CMD_PRT_DRV_START_SENSOR, 10);//start sensor data
    tmp32 = ioctl(prt_ct500_fd, CMD_PRT_DRV_RD_TPH_PAPER, &tmp32); //battery volt
	//printf("getBatAd from prt_ct500_fd");
    return tmp32;

}

//********************************************************************

