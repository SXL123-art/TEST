
#ifndef __EXT_FUN_H__
#define __EXT_FUN_H__


//---------------------------------SPI-------------------------------------------
extern unsigned char spi0_Send_Receive_byte(unsigned char  data);
extern void spi0_initial(void);
extern void spi0_put_str(unsigned char *str);

//-----------------prt--------------------
extern void PrtControl(void);
extern void ChessPrt(void);



//---------------prt_driver.c------------
extern void set_dark(unsigned char dark);
extern void power_printer(void);
extern void shut_prt(void);
extern void StartT2(void);
extern signed char GetTPHTemp(void);//2011-3-22
extern void PrtTraceCode(unsigned char *p);

//-----------------------imgBuf.c---------
extern char get_char(void);
extern void img_buf_init(void);
extern void printer_init(void)	;
extern void prt_memory_ctrl(unsigned char)	;
extern void make_img_eng(unsigned char ch,unsigned char cl,unsigned char language);
extern void make_tracecode(unsigned char num);
extern void start_prt(void);
extern unsigned char *  ASCNUM2BOLDFACE(unsigned char * p);

//-------------------prt_proce.c-----------
extern void revchar_proc(unsigned char ch);
extern void font_type(unsigned char type);

//-------------------BarCode---------------
/*----BAR_PRINT.C-------*/
#include "bar_print.h"
extern void Bar_PrintStop(void);
extern unsigned char Check_Str(unsigned char *pt);
extern void Bar_Print(unsigned char *p,unsigned char BarNum);
extern void BARCode_Config(BARTYP encode,unsigned char numpos,  \
                           unsigned char bar_position,unsigned char ZoomXX,unsigned char BarHeigh);
extern void BarCode_Print(unsigned char *p);

extern void EAN_UPCA_Init();
//EAN13
extern unsigned char  EAN13_CountVarify(unsigned char *pt);
extern void Dat_BitHandle(unsigned char dat,unsigned char j,unsigned char p_x,unsigned char bit_x);
extern void Get_EAN13BarPicture(unsigned char Numpos, unsigned char warn_line);
extern void Get_EanUpcaFrame(unsigned char *pt);

//CODE128
extern void Get_Code128Picture(unsigned short codeABC);
extern void Get_Code128Frame(unsigned char *p, unsigned short codeABC);
extern unsigned char Code128_Messages(unsigned char *p, unsigned short codeABC);

//QRCODE
extern int Save_QEImage(char* szSourceSring);
extern void Print_QRCode(unsigned char *str);
extern void Print_Img(void);


//extern void Print_HanXinCode(unsigned char *str);
//----------------------------PRT_Timer.c---------------------------------------
extern unsigned short GetTphAD(void);

#endif
