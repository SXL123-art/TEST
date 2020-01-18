#include "print_manager.h"
#include "string.h"
extern "C"
{
#include "print.h"
#include "set.h"

}

print_manager::print_manager()
{

}

int print_manager::init()
{
    Open_Dev_Prt_ct500();
    // if((settings.dark == 0) || (settings.dark > 5)) settings.dark = 3;
    setDarkness(3);//dark 12345 normal :3
    settings.chnFontNo = 5;//◊÷∫≈…Ë÷√
    settings.engFontNo = 5;
    font_type(FontNO);    //type= FontNO,FontBOLD,FontWIDE,FontHIGH,FontBIG,FontUNDERLINE,FontSTRIKETHROUGH
    return 0;
}

void print_manager::print_starts()
{
    print_start();
    MakeInitPattern();
    prt_line = 0;
}

void print_manager::print_string(char * str)
{
    sprintf(prnbuf, "\n%s",str);
    print();
}

void print_manager::print_string_center(char * str)
{
    int strLen = strlen(str);
    int wLen = 33;

    if (wLen < strLen) strLen = wLen;
    int sPos = (wLen - strLen)/2;
    memset(prnbuf, 0x20, sizeof(prnbuf));
    prnbuf[0]='\n';
    sprintf(&prnbuf[sPos], "%s", str);
    print();
}

void print_manager::print_string_bigsize(char * str)
{

    font_type(FontBIG);
    print_str((uint8 *)str, strlen(str));
    MakeFontPattern(prt_line++);
    font_type(FontNO);
}

void print_manager::print_push()
{
    paperpush();
}

void SetCodeType(unsigned char x)
{
    switch(x)//KYK barcode test
    {
        case 'a': //EAN13
            BARCode_Config(BARTYP_EAN13,NUM_DOWN,Middle,2,50);
            break;
        case 'b': //UPCA
            BARCode_Config(BARTYP_UPCA ,NUM_DOWN,Middle,2,50);
            break;
        case 'c': //EAN8
            BARCode_Config(BARTYP_EAN8 ,NUM_DOWN,Middle,2,50);
            break;
        case 'd': //CODE128-C
            Code128Type = CODE128_STARTC;
            BARCode_Config(BARTYP_CODE128,NUM_DOWN,Middle,2,50);
            break;
        case 'e': //CODE128-B
            Code128Type = CODE128_STARTB;
            BARCode_Config(BARTYP_CODE128,NUM_DOWN,Middle,2,50);
            break;
        case 'f': //CODE128-A
            Code128Type = CODE128_STARTA;
            BARCode_Config(BARTYP_CODE128,NUM_DOWN,Middle,2,50);
            break;
        case 'g': //EAN128
            Code128Type = EAN128;
            BARCode_Config(BARTYP_EAN128,NUM_DOWN,Middle,2,50);
            break;
        default: break;
    }
}
void print_manager::print_barcode(char * s,long kg, long pr, long to)
{
    unsigned char BarBuf[26];
    unsigned char max[8] = {1,12,11,7,26,14,14,26};  // KYK, No,EAN13,UPCA,EAN8,CODE128-C,CODE128-B,CODE128-A,EAN128
    dword digit9[11] = {0,9,99,999,9999,99999,999999,9999999,99999999,999999999,2000000000}; // new v201
    unsigned char i;
    unsigned char j;
    unsigned char k;
    unsigned char x;
    unsigned char Barcode_type;
    long num;

    i = 0;
    j = 0;
    Barcode_type = 1;

    memset(BarBuf, 0x00, sizeof(BarBuf));

    x = s[i];

    //SetCodeType('a'); //Default:EAN13
    if((x >= 'a') && (x <= 'g'))
    {
        i = 1;
        Barcode_type = x & 0x0f;
        SetCodeType(x);           //Set the Barcode type;
    }
    else BARCode_Init();           //Default:EAN13

    while(1)
    {
        x = s[i++];
#ifdef USE_JINAN_BASED_CHONGQING
        if(i >= 22) break;
#else
	if(i >= 16) break;
#endif
        if(x == 0) break;        //if no char
        if((x >= '0') && (x <= '9'))
        {
            BarBuf[j++] = x;
            goto l3;                //---
        }

        x = s[i++];
#ifdef USE_JINAN_BASED_CHONGQING
        if((x == 0) || (i >= 22)) return ;
#else
	if((x == 0) || (i >= 16)) return ;
#endif
        x -= '0';
        if((x > 9) || (x == 0)) return ;
        if((dword)num > digit9[x]) return ;
        for(k = 0; k < x; k++)
        {
            BarBuf[j+x-k-1] = (unsigned char)(num%10)+'0';
            num=num/10;
        }
        j = j + x;
l3:
        if(j > max[Barcode_type]) return ;
    }
    prt_line ++;
    BarCode_Print(BarBuf);			//start print barcode

    prt_mode=1;
    prt_flag=1;

    MakeBarPattern(4);
	prt_line += 4;
}

void print_manager::print_tireta()
{
    prt_line++;
    tireta();
}

void print_manager::save_qrcode(char * str)
{
   	Save_QEImage(str);
}

void print_manager::print_qrcode(char * str)
{
    prt_line++;
    Print_QRCode((unsigned char *)str);
}

void print_manager::Print_img(void)
{  
	Print_Img();
}

void print_manager::print_line()
{
    printline();
}

print_manager::~print_manager()
{
    //dtor
}

long print_manager::print_noPaper()
{
    return getNoPaper();
}


long print_manager::print_temp()
{
    return getTemp();
}

int print_manager::TestPrint(char * str)
{
//    font_type(FontBIG);
//    //char strs[300] ={0x00};
//    //sprintf(strs, "%c%c%c%s%c%c%c", 0x1b,0x57,0x31, str, 0x1b,0x57,0x30);
//    print_str((uint8 *)str, strlen(str));
//    MakeFontPattern(prt_line++);
//    font_type(FontNO);
//    return 0;
}

