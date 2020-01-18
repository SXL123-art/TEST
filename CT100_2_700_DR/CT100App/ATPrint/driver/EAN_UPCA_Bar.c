
/*
*===============================================================================
* 功能：生成EAN8，EAN13，UPC-A条码图形
*
* 注：如果条码长度范围超过打印机的打印范围或者是条码长度不够（EAV13码12位，EAN8
* 码7位，UPCA码11位，注：以上三种条码均未加上校验码），则不打印条码。
*
*===============================================================================
*/
#include <string.h>
#include <stdio.h>
#include "define.h"
#include "ext_var.h"
#include "ext_fun.h"

#include "bar_print.h"

unsigned char left_len,right_len;//定义EAN8,EAN13或UPCA左右条码的长度
unsigned char barcode_len = 0; //定义EAN8,EAN13或UPCA左右条码的长度

/*
--------------------------------------------------------------------------------
*功能：计算出EAN13校验码
--------------------------------------------------------------------------------
*/
unsigned char  EAN13_CountVarify(unsigned char *pt)
{
    unsigned short osum=0,esum=0;
    unsigned char i,verify_val;
    for(i=0; i<barcode_len-1; i+=2)
    {
        osum=osum+*(pt+i+1);	//第奇数个字符相加
        esum=esum+*(pt+i);		//第偶数个字符相加
    }
    if(Bar_tab.BAR_Encode == BARTYP_EAN13)
    {
        verify_val=(osum*3+esum)%10;
        verify_val=10-verify_val;
    }
    //else if(Bar_tab.BAR_Encode == BARTYP_UPCA){
    else
    {
        osum=osum-*(pt+barcode_len - 1);
        verify_val=(esum*3+osum)%10;
        verify_val=10-verify_val;
    }

    if(verify_val == 10) verify_val = 0;
    return 	verify_val;
}

void Dat_BitHandle(unsigned char dat,unsigned char j,unsigned char p_x,unsigned char bit_x)
{
    if(dat&(0x40>>j))
    {
        Bar_tab.TAB_stru[p_x] |= (0x01<<bit_x);
    }
    else
    {
        Bar_tab.TAB_stru[p_x] &= ((0x01<<bit_x)^0xff);
    }
}

/*
--------------------------------------------------------------------------------
* 功能 ：将编码后的条码数据转换成条码图形（EAN8，EAN13，UPC-A）
* 参数 ：Numpos: =BAR时，填充条码图案
*                =NUM时，填充条码字符
--------------------------------------------------------------------------------
*/
void Get_EAN13BarPicture(unsigned char Numpos, unsigned char warn_line)
{
    unsigned short i,j;
    unsigned char p_x,bit_x,start=0x50,mid=0x28,stop=0x50,offset=0;
    //				起始码	 中间码		结束码

    unsigned char *pt; //指向条码一点行图形的指针
    unsigned char X;//条码的起始位置

    /*判斷是那種編碼方式，并?置條碼左右手?的條碼?度*/
    if(Bar_tab.BAR_Encode == BARTYP_EAN13) offset = 1;
    else if(Bar_tab.BAR_Encode == BARTYP_EAN8)  offset = 0;
    else if(Bar_tab.BAR_Encode == BARTYP_UPCA)  offset = 0;

    X=Bar_tab.start_position;//条码的起始位置
    pt = bar_picture;

    if((Numpos==PRTBAR)&&(Bar_tab.BAR_Encode != BARTYP_EAN8))
    {
        for(i=0; i<7; i++)
        {
            p_x=X/8;
            bit_x=7-X%8;
            Dat_BitHandle(0X00,i,p_x,bit_x);
            X++;
        }

        if(Bar_tab.BAR_Encode == BARTYP_UPCA)
        {
            left_len = UPC_LEFTHAND_LEN;
            right_len = UPC_RIGHTHAND_LEN;
        }
    }

    if((Bar_tab.BAR_Encode == BARTYP_EAN13) || //第一个字符的打印
            (Bar_tab.BAR_Encode == BARTYP_UPCA))
    {
        if(Numpos==PRTNUM)
        {
            for(i=0; i<7; i++)
            {
                p_x=X/8;
                bit_x=7-X%8;
                Dat_BitHandle(*pt,i,p_x,bit_x);
                X++;
            }
        }
    }
    //==============================================================================//
    for(i=0; i<3; i++) //起始码101
    {
        p_x=X/8;
        bit_x=7-X%8;
        if(warn_line == WARN_LINE_EN)Dat_BitHandle(start,i,p_x,bit_x);
        else if(warn_line == WARN_LINE_STOP) Dat_BitHandle(0X00,i,p_x,bit_x);
        X++;
    }

    if((Numpos==PRTNUM)&&(Bar_tab.BAR_Encode == BARTYP_UPCA))
    {
        if(warn_line == WARN_LINE_EN)X += 7;
        else if(warn_line == WARN_LINE_STOP)
        {
            for(i=0; i<7; i++)
            {
                p_x=X/8;
                bit_x=7-X%8;
                Dat_BitHandle(0X00,i,p_x,bit_x);
                X++;
            }
        }
        offset = 1;
        left_len = 5;
        right_len = 5;
    }

    for(i=0; i<left_len; i++)//左手边条码
    {
        for(j=0; j<7; j++)
        {
            p_x=X/8;
            bit_x=7-X%8;
            Dat_BitHandle(*(pt+i+offset),j,p_x,bit_x);
            X++;
        }
    }
    //=============================================================================//
    for(i=0; i<5; i++) //中间码01010
    {
        p_x=X/8;
        bit_x=7-X%8;
        if(warn_line == WARN_LINE_EN)Dat_BitHandle(mid,i,p_x,bit_x);
        else if(warn_line == WARN_LINE_STOP) Dat_BitHandle(0X00,i,p_x,bit_x);
        X++;
    }

    for(i=0; i<right_len; i++) //右手边条码
    {
        for(j=0; j<7; j++)
        {
            p_x=X/8;
            bit_x=7-X%8;
            Dat_BitHandle(*(pt+i+left_len+offset),j,p_x,bit_x);
            X++;
        }
    }

    if((Numpos==PRTNUM)&&(Bar_tab.BAR_Encode == BARTYP_UPCA))
    {
        if(warn_line == WARN_LINE_EN)X += 7;
        else if(warn_line == WARN_LINE_STOP)
        {
            for(i=0; i<7; i++)
            {
                p_x=X/8;
                bit_x=7-X%8;
                Dat_BitHandle(0X00,i,p_x,bit_x);
                X++;
            }
        }
    }
    //=============================================================================//
    for(i=0; i<3; i++) //结束码101
    {
        p_x=X/8;
        bit_x=7-X%8;
        if(warn_line == WARN_LINE_EN) Dat_BitHandle(stop,i,p_x,bit_x);
        else if(warn_line == WARN_LINE_STOP) Dat_BitHandle(0X00,i,p_x,bit_x);
        X++;
    }

    if((Numpos==PRTNUM) && (Bar_tab.BAR_Encode == BARTYP_UPCA))//UPCA碼的校?碼在結束碼之後
    {
        for(j=0; j<7; j++)
        {
            p_x=X/8;
            bit_x=7-X%8;
            Dat_BitHandle(*(pt+11),j,p_x,bit_x);
            X++;
        }
    }
    EAN_UPCA_Init();
}

/*
*------------------------------------------------------------------------------*
* 功   能 ：获得条码的结构框架，条码数据,条码结构存放在BarPicture[13]中，
			条码数字存放在BarNums[13]中
* 参   数 ：*pt指向存放条码的存储区域的首地址
* 输   入 ：pt
* 输   出 ：NONE
* 返回 值 ：NONE
* 描   述 ：获得条码编码后的数据以及条码字符数据，分别存储在BarFram[13],BarNums[13]中
*------------------------------------------------------------------------------*
*/
void Get_EanUpcaFrame(unsigned char *pt)
{
    unsigned char dat,verify_val,i;

    //将条码转换成数值型
    for(i = 0; i < barcode_len-1 ; i++)
    {
        BarNums[i] = *(pt+i)-0x30;//将条码字符串转换成数值
        BarFrame[i]=0x00;//清空条码编码缓存区
    }

    //计算校验码
    verify_val = EAN13_CountVarify(BarNums);
    BarNums[ barcode_len-1 ]=verify_val;

    /*--------------初始化函数，生成EAN-13规则的条形码数据------------------------*/
    if(Bar_tab.BAR_Encode == BARTYP_EAN13)
    {
        dat = BarNums[0]; //第一个字符决定左边字符的编码奇偶性
        for(i = 1; i < barcode_len ; i++)
        {
            if(i < left_len+1)
            {
                if(EAN_parityTable[dat][i-1]=='O')
                {
                    BarFrame[i] = EAN_EncodingTable[0][BarNums[i]];
                }
                else if(EAN_parityTable[dat][i-1]=='E')
                {
                    BarFrame[i] = EAN_EncodingTable[1][BarNums[i]];
                }
            }
            else
            {
                BarFrame[i] = EAN_EncodingTable[2][BarNums[i]];
            }
        }
    }
    else if((Bar_tab.BAR_Encode == BARTYP_UPCA) ||
            (Bar_tab.BAR_Encode == BARTYP_EAN8))
    {
        for(i = 0; i < barcode_len ; i++)
        {
            if(i < left_len)
            {
                BarFrame[i] = EAN_EncodingTable[0][BarNums[i]];
            }
            else
            {
                BarFrame[i] = EAN_EncodingTable[2][BarNums[i]];
            }
        }
    }
}

void EAN_UPCA_Init()
{
    switch(Bar_tab.BAR_Encode)
    {
    case BARTYP_EAN8:
        barcode_len = 8;
        left_len = EAN8_LEFTHAND_LEN;
        right_len = EAN8_RIGHTHAND_LEN;
        break;
    case BARTYP_EAN13:
        barcode_len = 13;
        left_len = EAN13_LEFTHAND_LEN;
        right_len = EAN13_RIGHTHAND_LEN;
        break;
    case BARTYP_UPCA:
        barcode_len = 12;
        left_len = UPC_LEFTHAND_LEN;
        right_len = UPC_RIGHTHAND_LEN;
        break;
    default :
        break;
    }
}
