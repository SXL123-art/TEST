/*
*===============================================================================
*功能：CODE128条码（A、B、C三种编码方式）及EAN128条码的图形处理
*说明：在放大倍数为2倍的情况下：
				CODE128条码的A,B两种编码方式最多可以编码14个字符
*				C编码方式可以编码28个字符
*				EAN128条码最多可以编码26个字符
* 		在放大倍数为3倍的情况下：
*				CODE128条码的A,B两种编码方式可以编码9个字符
*				CODE128条码的C编码方式最多可以编码18个字符
*				EAN128条码最多可以编码16个字符（放大倍数2x）
*
*如果条码长度范围超过条码的最大允许范围，返回错误信息（BAR_OVERFLOW）,并不打印条码
*===============================================================================
*/

#include <string.h>
#include <stdio.h>
#include "define.h"
#include "ext_var.h"
#include "ext_fun.h"
#include "bar_print.h"

unsigned short 	Code128Frame[19];       //存放編碼后的文件
unsigned char 	Code128char_p[38];      //存放條碼字符在編碼裱中的位置
unsigned char 	Code128char[38];        //存放條碼數據
unsigned short Code128Type = CODE128_STARTC;//code128码的编码方式标志，可以是参数
//CODE128_STARTA,CODE128_STARTB,CODE128_STARTC

/*
--------------------------------------------------------------------------------
*功能：计算出code128码校验码
--------------------------------------------------------------------------------
*/
unsigned char Get_Code128VarifyFlag(unsigned char *pt,unsigned char bar_len)
{
    unsigned char code128_vrfyflg, i;
    unsigned short codesum = 0;

    //code_len = strlen((char const *)pt);

    codesum = *pt;
    //pt++;
    for(i=0; i<bar_len+1; i++)
    {
        codesum += (*pt)*i;
        pt++;
    }
    code128_vrfyflg = codesum % 103;

    return code128_vrfyflg;
}


/*
--------------------------------------------------------------------------------
*功能：获得条码图形
--------------------------------------------------------------------------------
*/
void Get_Code128Picture(unsigned short codeABC)
{
    unsigned char i=0, j, pt_bit, pt;
    unsigned short ptx=0;

    ptx = Bar_tab.start_position;
    while(Code128Frame[i] != 0 )
    {
        for(j=0; j<11; j++)
        {

            pt = ptx / 8;
            pt_bit = 7-(ptx % 8);

            if(Code128Frame[i] & (0x0400 >> j))
            {
                Bar_tab.TAB_stru[pt] |= (0x01 << pt_bit);
            }
            else
            {
                Bar_tab.TAB_stru[pt] &= ((0x01 << pt_bit)^0xff);
            }
            ptx++;

        }//end of  for(j=0; j<11; j++)
        i++;
    }//end of while(Code128Frame[i])

    for(j=0; j<13; j++)
    {
        pt = ptx / 8;
        pt_bit = 7-(ptx % 8);

        if(CODE128_STOP & (0x1000 >> j))
        {
            Bar_tab.TAB_stru[pt] |= (0x01 << pt_bit);
        }
        else
        {
            Bar_tab.TAB_stru[pt] &= ((0x01 << pt_bit)^0xff);
        }
        ptx++;
    }
}

/*
--------------------------------------------------------------------------------
* 功能：判断条码的长度是否超出打印范围
* 		返回0，表示没有超出范围
* 		返回BAR_OVERFLOW，表示超出打印范围
--------------------------------------------------------------------------------
*/
unsigned char Code128_Messages(unsigned char *p, unsigned short codeABC)
{
    unsigned char char_len,bar_msg = 0;

    char_len = strlen((char const *)p);//计算条码的长度
    switch(codeABC)
    {
    case CODE128_STARTA:
    case CODE128_STARTB:
        if(((Bar_tab.x_mulriple == 1) && (char_len > 19)) ||
                ((Bar_tab.x_mulriple == 2) && (char_len > 14)) ||
                ((Bar_tab.x_mulriple == 3) && (char_len > 9)))
            bar_msg = BAR_OVERFLOW;
        break;
    case CODE128_STARTC:
        if(((Bar_tab.x_mulriple == 2) && (char_len > 26)) ||
                ((Bar_tab.x_mulriple == 3) && (char_len > 18)))
            bar_msg = BAR_OVERFLOW;
        break;
    case EAN128:
        if(((Bar_tab.x_mulriple == 2) && (char_len > 26)) ||
                ((Bar_tab.x_mulriple == 3) && (char_len > 16)))
            bar_msg = BAR_OVERFLOW;
        break;
    default :
        break;
    }

    if (((codeABC == CODE128_STARTC) || (codeABC == EAN128)) 	//如果code128码的C编码和EAN128码输入的字符时奇数个或者
            && ((char_len % 2) || (Check_Str(p) == BARCODECHAR_FALSE))) //有非数字字符的情况下，则输入的条码是非法的
    {
        bar_msg = 3;
    }

    if(bar_msg)
    {
        Bar_PrintStop();
    }
    return bar_msg;
}

/*
--------------------------------------------------------------------------------
函数功能 ：	获得条码的编码
参	  数 ：	*p：指针p指向条码字符存储区域的首地址
			codeABC：指定条码的编码方式，可以是参数
					CODE128_A；CODE128_B；CODE128_C
--------------------------------------------------------------------------------
*/
void Get_Code128Frame(unsigned char *p, unsigned short codeABC)
{
    unsigned char dat_pt, i,j,bar_len;

    for(i=0; i< 19; i++)
    {
        Code128Frame[i]=0;
        Code128char_p[i]=0;
        Code128char_p[i*2+1]=0;
        Code128char[i] = 0;
    }

    bar_len = strlen((char const *)p);//计算条码的长度
    for(i = 0; i < bar_len; i++)
    {

        Code128char[i] = *(p+i);

    }

    if(codeABC == CODE128_STARTC || codeABC == EAN128) bar_len /=2;

    for(i = 1; i <= bar_len; i++ )//循环次数由条码的字符长度而定
    {
        switch(codeABC)
        {
        case CODE128_STARTA: //A编码方式
            if((*p>=32) && ((*p <= 95)))
            {
                dat_pt = *p - 32;
            }

            p++;
            break;

        case CODE128_STARTB: //B编码方式
            if((*p >= 32) && (*p <= 126))
            {
                dat_pt = *p - 32;
            }

            p++;
            break;

        case EAN128:
        case CODE128_STARTC: //C编码方式
            dat_pt = *p-0x30;
            dat_pt *=10;

            p++;
            dat_pt += (*p-0x30);
            p++;
            break;
        }//end of switch(codeABC){}
        Code128char_p[i] = dat_pt;
        Code128Frame[i] = Code128Tab[dat_pt];
    }//end of for(i = 1; i <= bar_len; )


    Code128Frame[0] = codeABC;

    if(codeABC == CODE128_STARTA)Code128char_p[0] = 103;
    else if(codeABC == CODE128_STARTB)Code128char_p[0] = 104;
    else if(codeABC == CODE128_STARTC)Code128char_p[0] = 105;
    else if(codeABC == EAN128)
    {
        for(j=i; j>=1; j--)
        {
            Code128char_p[j+1] = Code128char_p[j];
            Code128Frame[j+1] = Code128Frame[j];

        }
        Code128char_p[0] = 105;
        Code128char_p[1] = 102;
        Code128Frame[0] = Code128Tab[105];
        Code128Frame[1] = Code128Tab[102];
        bar_len +=1;
        i++;
    }

    Code128char_p[i] = Get_Code128VarifyFlag(Code128char_p,bar_len);//计算校验码
    Code128Frame[i] = Code128Tab[Code128char_p[i]];
}
