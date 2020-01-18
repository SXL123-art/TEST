#ifndef _BAR_PRINT_H_
#define _BAR_PRINT_H_


/*-------定义条码操作标志信息---------------------*/
#define BARCODECHAR_FALSE		0   //输入的条码字符不合法
#define BARCODECHAR_TRUE		1   //输入的条码字符合法
#define BAR_OVERFLOW			2   //输入的条码长度超出打印机所能打印的最大长度


/*-------定义条码的类型--------------------------*/
//#define BAR_TYPE_EAN13		1
//#define BAR_TYPE_CODE128		2
//#define BAR_TYPE_EAN128		3
typedef enum {BARTYP_EAN13, BARTYP_EAN128, BARTYP_CODE128,BARTYP_EAN8,BARTYP_UPCA} BARTYP;

/*--------------------定义条码的位置（靠左，居中，靠右）----------------------------------*/
#define Left				1
#define Middle				2
#define Right				3

/*--------定义条码字符的位置（上、下 或者不打印条码字符）-------------*/
#define NONUM				1	//只打印条码图形
#define NUM_UP				2	//13位条码在条码图形的上面
#define NUM_DOWN			3	//13位条码在条码图形的下面


#define EAN13_LEN				13	//条码长度13个字符
#define EAN13_LEFTHAND_LEN		6	//条码左手边6个字符
#define EAN13_RIGHTHAND_LEN		6	//条码右手边6个字符
#define PRT_LINE_LEN			48	//打印机一点行的字节数

#define PRTBAR  				1
#define PRTNUM					2
#define WARN_LINE_EN			1//条码警告线控制
#define WARN_LINE_STOP			0


/*---------ean8--------------------------------*/
#define EAN8_LEN			3
#define EAN8_LEFTHAND_LEN	4
#define EAN8_RIGHTHAND_LEN 	4

/*----------------UPC--------------------------*/
#define UPC_LEN				12
#define UPC_LEFTHAND_LEN	6
#define UPC_RIGHTHAND_LEN	6

//========code128===============================================================
/*---CODE128码的起始码与终止码定义-------------------------*/
#define CODE128_STARTA          0X684 	//code128A的起始码
#define CODE128_STARTB          0X690 	//code128B的起始码
#define CODE128_STARTC          0X69C 	//code128C的起始码
#define EAN128                  0x7ae   //FNC1(EAN128吗)
#define CODE128_STOP            0X18EB  //条码结束标志位

/*--------定义条码的结构-----------*/
typedef struct
{
    unsigned char BAR_Encode;		//条码类型（EAN13，EAN128，CODE128 ...）
    unsigned char TAB_stru[48];		//一点行的条码数据缓存

    unsigned short bar_heigh;		//条码的高度
    unsigned char x_mulriple;		//条码宽度的放大倍数，可以是参数1,2,3）
    unsigned short start_position;	        //条码的起始位置
    unsigned short Numstart_position;	//条码的起始位置
    unsigned char Num_Pos;			//条码字符的位置（NUM_UP ， NUM_DOWN ， NONUM）
    unsigned char BarPosition;              //条码的位置（左侧，右侧，居中）

//	unsigned char y_mulriple;
//	unsigned short coord_x;			//条码打印结束的位置
//	unsigned short coord_y;			//纵坐标

    unsigned char space_flag;		//空格打印完成标志
    unsigned char barpic_flag;		//条码图形打印完成标志
    unsigned char barnum_flag;		//条码字符打印完成标志
} BAR_STRUCT;



extern unsigned char Check_Str(unsigned char *pt);
extern unsigned char  EAN13_CountVarify(unsigned char *pt);
extern void Bar_PictureDatOprate(unsigned char dat_position,short bitt,unsigned char dat);
extern void EAN13_Barprint(void);
extern void EAN13_BarOprate(unsigned char *pt);
extern void EAN13_Print(unsigned char *p,unsigned char BarNum);


extern void Bar_PicturePrt(unsigned char *p,unsigned short heigh );
extern void  Bar_DatPrt(unsigned char updwn);
extern void Bar_SpacePrt();
extern void BARCode_Init(void);
extern void BAR_ZoomXX( unsigned char xbei);
#endif















