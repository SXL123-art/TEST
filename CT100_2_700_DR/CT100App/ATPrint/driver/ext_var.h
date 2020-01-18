


#ifndef __EXT_VAR_H__
#define __EXT_VAR_H__



//*************************PRT*****************************
#define PRT_DEF
#ifdef PRT_DEF
#define PRT_EXT
#else
#define PRT_EXT extern
#endif

//PRT_EXT unsigned char prt_flag;              						//打印标志,准备要打印,则先置1  ; 打印开始后清0
//unsigned char print_oneline[MAX_PRINT_BYTEWIDTH];  //要打印1行的内容
PRT_EXT unsigned char motor_step;
PRT_EXT unsigned int tph_prt_line;                     // 打印缓冲区行计数  //即打印了多少行了  //这个值应该小于= tph_prt_height
PRT_EXT unsigned int tph_prt_addr;                    // 待打印缓冲区在RAM中的地址
PRT_EXT unsigned char tph_prt_height;                   // 打印缓冲区的高度
PRT_EXT unsigned char print_oneline[48];				// 将要打印的一点行的内容
PRT_EXT unsigned char prt_flag;				// 1:打印  0:停止打印

PRT_EXT unsigned char prt_buf[1000];                    //the fonts or CMD will be printed shoud write to this buffer
PRT_EXT unsigned int prt_write;              //prt_buf write pointer
PRT_EXT unsigned int prt_read;               //prt_buf read pointer
PRT_EXT volatile unsigned short ch_exist;               //prt_buf[]里有数据

PRT_EXT unsigned short draw_x_pos;                                  // The position in prt_img_buffer        //一行的第几个位置  //一行共48个位置
PRT_EXT unsigned char eng_font_width;                               // The width of the English character width//英文字符宽度		//=8或16
#ifdef COUNTRY_CHINA
PRT_EXT unsigned char cn_font_width;                                // The width of the Chinese character width//中方字符宽度
#endif
PRT_EXT unsigned char prt_state;
PRT_EXT unsigned char prt_mode;                         //=1 正常打印模式 ; =0 测试模式

PRT_EXT unsigned char state;
PRT_EXT unsigned char * line_prt_dat ;

PRT_EXT unsigned char darkness;
PRT_EXT unsigned short strobe_time;
PRT_EXT unsigned long LogoPointer;

#undef PRT_DEF

//------------------------end------------------------------
//*************************IMG*****************************
#ifdef IMG_DEF
#define IMG_EXT
#else
#define IMG_EXT extern
#endif

IMG_EXT struct IMG_BUF
{

    unsigned char* prt_addr;          //打印地址
    unsigned char* write_addr;        //写地址
    unsigned short height;            //高度				//PRINT_CTRL.height=1时，IMG_BUF的高度为32
    unsigned short prt_line;          //打印行数

} IMG_BUF;                  //结构体

IMG_EXT struct IMG_BUF img_buf0;
IMG_EXT struct IMG_BUF img_buf1;
IMG_EXT struct IMG_BUF* prt_img_buffer;

IMG_EXT unsigned char space_line;
IMG_EXT unsigned char prt_ctrl_typeface;  //打印类型
IMG_EXT unsigned char prt_ctrl_width;     //打印宽度
IMG_EXT unsigned char prt_ctrl_height;    //打印高度

IMG_EXT unsigned char BUF0[0x900*2];//KYK
IMG_EXT unsigned char BUF1[0x900*2];//KYK

IMG_EXT unsigned char TraceabilityCode[24];

#undef IMG_DEF
//------------------------end------------------------------

//*************************Bar*****************************
/*-----bar print val----------------------------------------------------------*/
#ifndef BAR_EXTDEF
#include  "bar_print.h"
extern BAR_STRUCT	Bar_tab;
extern unsigned char BarFrame[13];
extern unsigned char BarNums[13];
extern unsigned char Bar_PrtDat[48];            //存放将要打印数据的缓冲区
extern unsigned char bar_picture[13];           //用来存放编码后得到的
//0-1序列的数组，用于图形化输出
//unsigned char bar_dat[13];                    //存放转换成数值型的条码
extern unsigned char nums,lines_num;            //lines_num:判断一行数据是否打印结束
extern unsigned char Bar_PrintOver_Flag;        //条码打印结束标志　＝１：条码打印结束
// =0 :条码打印中
extern unsigned char BarDoubProcess_Flag;	//此变量的功能：防止相应的函数重复执行，
//在条码打印结束以后，此标识变量必须复位
//bit7 6 5 4 3 2
//bit1 :判断条码数据是否已经经过编码处理
//bit0 : 判断是否已进行条码图形转换，若转换则bit0置位
extern unsigned short Code128Type;//code128码的编码方式标志，可以是参数
#endif
#undef BAR_EXTDEF
extern unsigned char Code128char[38];
extern unsigned short Code128Type;//code128码的编码方式标志，可以是参数
extern unsigned char left_len,rignt_len;//定义EAN8,EAN13或UPCA左右条码的长度
extern unsigned char barcode_len; //定义EAN8,EAN13或UPCA左右条码的长度
extern const unsigned char EAN_EncodingTable[3][10];
extern const unsigned char EAN_parityTable[10][6];
extern const unsigned char EAN_numberTable[10][7];//0 - 9 点阵字库
extern const unsigned short Code128Tab[107];//code128码编码表
extern const  unsigned char EngFont[];//英文字库
extern unsigned int  print_lines;//计数已经打印的行数
/*----------------------------------------------------------------------*/
#endif
