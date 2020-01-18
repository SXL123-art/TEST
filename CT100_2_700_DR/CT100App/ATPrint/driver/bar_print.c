
#define BAR_EXTDEF
#include "./../../public/globals.h"
#include <string.h>
#include <stdio.h>
#include "define.h"
#include "ext_var.h"
#include "ext_fun.h"
#include "BarcodeFont.h"

#include "bar_print.h"



/*------private var define---------------------------------------------------*/

unsigned int contts=0;

unsigned int  print_lines=0;	//计数已经打印的行数


BARTYP BarType;//定义条码类型（枚举型数据）

BAR_STRUCT	Bar_tab;

unsigned char BarFrame[19];
unsigned char BarNums[38];

unsigned char Bar_PrtDat[48];			//存放将要打印数据的缓冲区
unsigned char bar_picture[13];			//用来存放编码后得到的
//0-1序列的数组，用于图形化输出
//unsigned char bar_dat[13];				//存放转换成数值型的条码
unsigned char nums = 0,lines_num= 0;	//lines_num:判断一行数据是否打印结束

unsigned char Bar_PrintOver_Flag=1;		//条码打印结束标志　＝１：条码打印结束
// =0 :条码打印中
unsigned char BarDoubProcess_Flag=0;	//此变量的功能：防止相应的函数重复执行，
//在条码打印结束以后，此标识变量必须复位
//bit7 6 5 4 3
//bit2 : 判断CODE128条码数据是否已经经过编码处理
//bit1 : 判断EAN13条码数据是否已经经过编码处理
//bit0 : 判断是否已进行条码图形转换，若转换则bit0置位


/*
--------------------------------------------------------------------------------
*功能 : 条码打印停止函数
--------------------------------------------------------------------------------
*/
void Bar_PrintStop(void)
{
#ifndef USE_CT100G
    unsigned char clrlen;

    shut_prt();//停止打印机

    for(clrlen = 0; clrlen<48; clrlen++) //清空条码缓存区
    {
        Bar_PrtDat[clrlen] = 0;
        Bar_tab.TAB_stru[clrlen] = 0;
    }

    Bar_tab.barpic_flag = 0;	//
    Bar_tab.barnum_flag = 0;	//
    Bar_tab.space_flag = 0;		//
    nums=0;						//
    lines_num=0;				//
    Bar_PrintOver_Flag = 1;		//条码打印结束标志
    BarDoubProcess_Flag = 0;	//
    // 电机停止
#endif
}

/*
--------------------------------------------------------------------------------
* 功能 ： 条码打印开始函数
--------------------------------------------------------------------------------
*/
void Bar_PrintStart(void)
{
    Bar_PrintOver_Flag = 0;//将打印结束标志清零
#ifndef USE_CT100G
    power_printer();
#endif
}

/*
--------------------------------------------------------------------------------
* 功能 ： 检查字符串中的字符是否全部为数字，若是则返回TRUE，否则返回FALSE
--------------------------------------------------------------------------------
*/
unsigned char Check_Str(unsigned char *pt)
{
    unsigned char len, i;
    len = strlen((char const *)pt);
    for(i = 0; i < len; i ++)
    {
        if( (*(pt+i)<0x30) || (*(pt+i)>0x39) )return BARCODECHAR_FALSE;
    }
    return BARCODECHAR_TRUE;
}


/*
--------------------------------------------------------------------------------
* 功能 ： 检查输入的条码数据是否合法
--------------------------------------------------------------------------------
*/
unsigned char Barcode_Check(unsigned char *p,unsigned char bar_typ)
{
    unsigned char rt_dat = 0;//返回值

    /*判断输入的是否是12位合法数字字符串*/
    switch(bar_typ)
    {
    case BARTYP_EAN8:
    case BARTYP_UPCA:
    case BARTYP_EAN13:
        if(strlen((const char*)p) != barcode_len-1)
        {
            //如果EAN13码的长度不是12个字符，则输入的数据不合法
            rt_dat = 1;
        }
        if(Check_Str(p) == BARCODECHAR_FALSE)
        {
            rt_dat = 1;
        }
        break;
    case BARTYP_EAN128:
    case BARTYP_CODE128:
        rt_dat = Code128_Messages(p, Code128Type);//co128、EAN128条码判断
        break;
    }

    return rt_dat;
}

void BAR_PosProcess(unsigned char barBitlen)
{
    unsigned short strprt;
    /*--------------判断条码的放大倍数-----------*/
    switch(Bar_tab.x_mulriple )
    {
    case 1:
        strprt = 383;
        break;
    case 2:
        strprt = 191;
        break;
    case 3:
        strprt = 127;
        break;
    default :
        break;
    }

    /*--------------判断条码的位置-----------*/
    switch(Bar_tab.BarPosition )
    {
    case Left:
        Bar_tab.start_position = 1;
        break;
    case Middle:
        Bar_tab.start_position = (strprt - barBitlen)/2;
        break;
    case Right:
        Bar_tab.start_position = strprt - barBitlen ;
        break;
    default :
        break;
    }
}

void BarNumPos_count(unsigned char  barBytelen, unsigned char barBitlen)
{
    unsigned char  barNumBitlen;

    barNumBitlen = barBytelen * 7;//计算条码数字的打印长度

    if(barNumBitlen > barBitlen)
    {
        barNumBitlen = (barNumBitlen - barBitlen)/2;
        if(Bar_tab.start_position > barNumBitlen)
            Bar_tab.Numstart_position = Bar_tab.start_position - barNumBitlen;
        else Bar_tab.Numstart_position = Bar_tab.start_position;
    }
    else
    {
        barNumBitlen = (barBitlen - barNumBitlen)/2;
        Bar_tab.Numstart_position = Bar_tab.start_position + barNumBitlen;
    }
}
/*

BC_type:条码的编码方式
*/
void BAR_Position( unsigned char BC_type, unsigned char *p )
{
    unsigned char barBytelen = 0;
    unsigned char barBitlen = 0;
    const unsigned char ean128=46, code128=35;

    barBytelen = strlen((char const *)p);

    switch(BC_type)
    {
        //Bar_tab.start_position
    case BARTYP_EAN8:
        BAR_PosProcess(67);
        break;
    case BARTYP_UPCA:
        BAR_PosProcess(109);
        break;
    case BARTYP_EAN13:
        BAR_PosProcess(102);
        break;
    case BARTYP_EAN128:
        barBitlen = ean128 + (barBytelen / 2 * 11);
        BAR_PosProcess(barBitlen);
        BarNumPos_count(barBytelen,barBitlen);//计算条码字符的起始位置
        break;
    case BARTYP_CODE128:
        if(Code128Type == CODE128_STARTC)
        {
            barBitlen = code128 + (barBytelen / 2 * 11);
        }
        else if ((Code128Type == CODE128_STARTA) ||
                 (Code128Type == CODE128_STARTB))
        {
            barBitlen = code128 + barBytelen*11;
        }
        BAR_PosProcess(barBitlen);
        BarNumPos_count(barBytelen,barBitlen);//计算条码字符的起始位置
        break;
    default :
        break;
    }
}

void Num_DotCvnt(unsigned char *pt)
{
    unsigned short prt = Bar_tab.Numstart_position;
    unsigned  char stlen = strlen((char const *)pt);
    unsigned char i = 0;
    unsigned char j;

    for(i=0; i < stlen; i++)
    {
        for(j = 0; j<7 ; j++)
        {
            if(*(pt+i)&(0x40 >> j))
            {
                Bar_tab.TAB_stru[prt/8] |= (0x80 >> (prt%8));
            }
            else
            {
                Bar_tab.TAB_stru[prt/8] &= (0x80 >> (prt%8)) ^ 0xff;
            }
            prt++;
        }
    }
}


void Bar_NumPrt(const char *num_buf)
{
    unsigned char buf[48];
    unsigned char i;

    while(1)
    {
        if((nums <= 8) && (print_lines - lines_num >= Bar_tab.x_mulriple))
        {
            for(i=0; i< 48; i++) Bar_tab.TAB_stru[i] = 0;

            if(nums == 0 ) //第一行打空行
            {
                for(i=0; i< 48; i++) buf[i]=0;
            }
            else if((nums >=1) && (nums <=7 ))  //第二行到第八行打印数字
            {
                for(i=0; i< strlen(num_buf); i++)
                {
                    buf[i]=EAN_numberTable[*(num_buf+i) - 0x30][nums-1];
                }
                Num_DotCvnt(buf);//数字点阵转换，去掉每个字节最高位
            }
            else if(nums == 8)  //第九行空行
            {
                for(i=0; i< 48; i++)
                {
                    Bar_tab.TAB_stru[i]=0;
                }
            }
            //if(print_lines - lines_num >= Bar_tab.x_mulriple)
            //{
            lines_num = print_lines;
            nums++;
            //}
            BAR_ZoomXX(Bar_tab.x_mulriple);
        }

        else if((nums >8) &&  (print_lines - lines_num >= Bar_tab.x_mulriple))
        {
            break;
        }

        /*if(nums<=3)Get_EAN13BarPicture(PRTNUM,WARN_LINE_EN);
        else  Get_EAN13BarPicture(PRTNUM,WARN_LINE_STOP);

        */
    }
}
/*
*------------------------------------------------------------------------------*
* 功能：条码横向倍数放大，可选倍数1,2,3
*------------------------------------------------------------------------------*
*/
void BAR_ZoomXX( unsigned char xbei)
{
    unsigned char i,p_x,bit_x,pn_x,bitn_x;
    unsigned short ptx=0;

    for(i=0; i<48; i++)
    {
        Bar_PrtDat[i]=0;
    }

    switch(xbei)
    {
    case 1:
        for(i=0; i<PRT_LINE_LEN; i++)
        {
            Bar_PrtDat[i] = Bar_tab.TAB_stru[i];
        }
        //		Bar_tab.coord_x = Bar_tab.start_position + 95;
        //		if(Bar_tab.coord_x >= 383) Bar_tab.coord_x=383;
        break;
    case 2:
        ptx=0;
        while(ptx*2 <= PRT_LINE_LEN*8)
        {
            p_x=ptx/8;
            bit_x=7-ptx%8;
            pn_x=ptx/4;
            bitn_x=7-ptx*2%8;

            if(Bar_tab.TAB_stru[p_x]&(0x01<<bit_x))
            {
                Bar_PrtDat[pn_x] |= (0x01<<bitn_x);
                if(bitn_x==0)
                {
                    bitn_x=7;
                    pn_x++;
                }
                else bitn_x--;

                Bar_PrtDat[pn_x] |= (0x01<< bitn_x );
                ptx++;
            }
            else
            {
                Bar_PrtDat[pn_x] &= ((0x01<<bitn_x)^0xff);
                if(bitn_x==0)
                {
                    bitn_x=7;
                    pn_x++;
                }
                else bitn_x--;
                Bar_PrtDat[pn_x] &= ((0x01<<bitn_x)^0xff);
                ptx++;
            }
        }
        //			Bar_tab.coord_x = Bar_tab.start_position + 190;
        //		if(Bar_tab.coord_x >= 383) Bar_tab.coord_x=383;
        break;
    case 3:
        ptx=0;
        //Bar_tab.TAB_stru[0]=0xaa;

        while(ptx*3 <= PRT_LINE_LEN*8)
        {
            p_x=ptx/8;
            bit_x=7-ptx%8;
            pn_x=ptx*3/8;
            bitn_x=7-ptx*3%8;

            if(Bar_tab.TAB_stru[p_x]&(0x01<<bit_x))
            {
                Bar_PrtDat[pn_x] |= (0x01<<bitn_x);

                if(bitn_x==0)
                {
                    bitn_x=7;
                    pn_x++;
                }
                else bitn_x--;
                Bar_PrtDat[pn_x] |= (0x01<< bitn_x );

                if(bitn_x==0)
                {
                    bitn_x=7;
                    pn_x++;
                }
                else bitn_x--;
                Bar_PrtDat[pn_x] |= (0x01<< bitn_x );
                ptx++;
            }
            else
            {
                Bar_PrtDat[pn_x] &= ((0x01<<bitn_x)^0xff);

                if(bitn_x==0)
                {
                    bitn_x=7;
                    pn_x++;
                }
                else bitn_x--;
                Bar_PrtDat[pn_x] &= ((0x01<<bitn_x)^0xff);

                if(bitn_x==0)
                {
                    bitn_x=7;
                    pn_x++;
                }
                else bitn_x--;
                Bar_PrtDat[pn_x] &= ((0x01<<bitn_x)^0xff);
                ptx++;
            }
        }
        //		  Bar_tab.coord_x = Bar_tab.start_position + 285;
        //		  if(Bar_tab.coord_x >= 383) Bar_tab.coord_x=383;
        break;
    default :
        break;
    }
}

/*
*------------------------------------------------------------------------------*
*功能：打印条码图形
*------------------------------------------------------------------------------*
*/
void Bar_PicturePrt(unsigned char *p,unsigned short heigh)
{
    unsigned char i;

    BarDoubProcess_Flag = 2; //KYK temp
    if((BarDoubProcess_Flag & 0x01) == 0)
    {
        for(i=0; i<48; i++) Bar_tab.TAB_stru[i] = 0;

        switch(Bar_tab.BAR_Encode)
        {
        case BARTYP_EAN8:
        case BARTYP_UPCA:
        case BARTYP_EAN13:
            for(i=0; i<EAN13_LEN; i++)
            {
                bar_picture[i]=BarFrame[i];
            }
            Get_EAN13BarPicture(PRTBAR,WARN_LINE_EN);//获得条码图形
            break;
        case BARTYP_CODE128:
            Get_Code128Picture(Code128Type); //获得code128条码图形
            if(Bar_tab.x_mulriple >= 2) Bar_tab.x_mulriple = 2;
            break;
        }//end of switch(Bar_tab.BAR_Encode)
        BAR_ZoomXX(Bar_tab.x_mulriple);//放大
        BarDoubProcess_Flag |= 0x01;
    }

    if(print_lines <= heigh )
    {
        if(print_lines  >= heigh - 2 )
            for(i=0; i<EAN13_LEN; i++) bar_picture[i]=0;

        if( print_lines >= heigh )
        {
            print_lines = 0;
            tph_prt_line=0; //2010-12-16
            nums=0;
            lines_num=0;
            Bar_tab.barpic_flag=1;
        }
    }
}

/*
*------------------------------------------------------------------------------*
* 功能：打印条码数字
*------------------------------------------------------------------------------*
*/
void  Bar_DatPrt(unsigned char updwn)
{
    unsigned char i;

    switch(Bar_tab.BAR_Encode)
    {
    case BARTYP_EAN8:
    case BARTYP_UPCA:
    case BARTYP_EAN13:
        if((nums<=8) && (print_lines - lines_num >=Bar_tab.x_mulriple ))//13为条码数据
        {
            if(nums==0)
            {
                for(i=0; i<barcode_len; i++) bar_picture[i]=0;
            }
            else if((nums >= 1) && nums<= 7)
            {
                for(i=0; i<barcode_len; i++)
                {
                    bar_picture[i]=EAN_numberTable[BarNums[i]][nums-1];
                }
            }
            else
            {
                for(i=0; i<barcode_len; i++) bar_picture[i]=0;
            }
            nums++;
            lines_num = print_lines;

            switch(updwn)
            {
            case NUM_UP:
                if(nums<=3)Get_EAN13BarPicture(PRTNUM,WARN_LINE_STOP);
                else Get_EAN13BarPicture(PRTNUM,WARN_LINE_EN);
                BAR_ZoomXX(Bar_tab.x_mulriple);
                break;
            case NUM_DOWN:
                if(nums<=3)Get_EAN13BarPicture(PRTNUM,WARN_LINE_EN);
                else  Get_EAN13BarPicture(PRTNUM,WARN_LINE_STOP);
                BAR_ZoomXX(Bar_tab.x_mulriple);
                break;
            default :
                break;
            }
        }

        if(nums>8 && (print_lines - lines_num >=Bar_tab.x_mulriple ))
        {
            for(i=0; i<EAN13_LEN; i++) bar_picture[i]=0;
            print_lines=0;
            nums=0;
            lines_num=0;
            Bar_tab.barnum_flag=1;	//条码字符打印完成
        }
        break;

    case BARTYP_EAN128:
    case BARTYP_CODE128:
        Bar_NumPrt((const char *)Code128char);
        tph_prt_line=0;
#ifdef CCJ_LIB
        //Code128char

        if(Code128Type == CODE128_STARTA)
        {
            prt_ctrl_typeface |= PRT_TYPE_BOLD; //粗体  Code128char
            prt_buf[prt_write++] = 0x1b; //命令标识符
            prt_buf[prt_write++] = 'U';
            prt_buf[prt_write++] = '1';
        }
        else if(Code128Type == CODE128_STARTB)
        {
            prt_ctrl_typeface |= PRT_TYPE_BOLD; //粗体  Code128char
            prt_buf[prt_write++] = 0x1b; //命令标识符
            prt_buf[prt_write++] = 'U';
            prt_buf[prt_write++] = '1';
        }
        else if(Code128Type == CODE128_STARTC)
        {
            prt_ctrl_typeface |= PRT_TYPE_BOLD; //粗体  Code128char
            //prt_buf[prt_write++] = 0x1b; //命令标识符
            //prt_buf[prt_write++] = 'U';
            //prt_buf[prt_write++] = '1';
        }
        else if(Code128Type == EAN128)
        {
            prt_ctrl_typeface |= PRT_TYPE_BOLD; //粗体  Code128char
            //prt_buf[prt_write++] = 0x1b; //命令标识符
            //prt_buf[prt_write++] = 'U';
            //prt_buf[prt_write++] = '1';
        }

        unsigned short string_len = strlen((char const *)Code128char);
        for(i = 0; i < string_len; i++)
        {
            //prt_write += i;
            prt_buf[prt_write++] = Code128char[i];
        }

        prt_buf[prt_write++] = ' ';

        //sprintf((char*)&prt_buf[prt_write++],"It's EAN13 Barcode:\n");
        prt_write=(unsigned int) strlen((char*)prt_buf);
        if(prt_write >= 1024) prt_write=0;
        start_prt();  //打印条码字符

        unsigned  int kkk = 100000;
        while(kkk > 10)
        {
            kkk--;
        }

        power_printer();
        //Bar_PrintStart();  //由于start_prt();函数执行结束以后复位了标志位prt_flag
        //但是在打印条码时
#endif
        print_lines=0;
        nums=0;
        lines_num=0;
        Bar_tab.barnum_flag=1;
        break;
    }
}

/*
*------------------------------------------------------------------------------*
* 功能：打印机空转，不打印图案
*------------------------------------------------------------------------------*
*/
void Bar_SpacePrt()
{
#ifndef USE_CT100G
    unsigned char i ;
    for(i=0; i<48; i++) Bar_PrtDat[i]=0;

    if(print_lines - lines_num)
    {
        lines_num = print_lines;
        nums++;
        if(nums>25)
        {
            nums=0;
            lines_num=0;
            Bar_tab.space_flag=1;			//条码空格打印完成
            shut_prt();
        }
    }
#endif
}



/*
*------------------------------------------------------------------------------*
*功能描述：	EAN13条码打印函数
*输    入：	1) *p:指向条码存储区的首地址
*			2) Numpos:设置条码的输出类型,可以是以下参数：NONUM，NUM_UP，NUM_DOWN
*输    出：
*调用函数：	1) Prt_Stop();
*		   	2) BAR_FillTable(unsigned short X,unsigned char xbei,unsigned char Numpos,
*							unsigned char warn_line)
*------------------------------------------------------------------------------*
*/
void Bar_Print(unsigned char *p,unsigned char Numpos)
{
    if(prt_flag==1 )
    {
        switch(Numpos)
        {
        case NONUM :
            Bar_tab.barnum_flag = 1;
            if((Bar_tab.barpic_flag == 0)&&(Bar_tab.space_flag == 0))
            {
                Bar_PicturePrt(p,Bar_tab.bar_heigh);
                return;
            }
            else if((Bar_tab.barpic_flag == 1)&&(Bar_tab.space_flag == 0))
            {
                Bar_SpacePrt();
            }
            break;
        case NUM_UP :
            if(Bar_tab.barnum_flag == 0)
            {
                //BarDateOprate(p,BARTYP_EAN13);
                Bar_DatPrt(NUM_UP);
                return;
            }
            else if((Bar_tab.barnum_flag == 1) &&(Bar_tab.barpic_flag == 0)
                    && (Bar_tab.space_flag == 0))
            {
                Bar_PicturePrt(p,Bar_tab.bar_heigh);
                return;
            }
            else if((Bar_tab.barnum_flag == 1) &&(Bar_tab.barpic_flag == 1)
                    && (Bar_tab.space_flag == 0))
            {
                Bar_SpacePrt();
            }
            break;
        case NUM_DOWN:
            if(Bar_tab.barpic_flag == 0)
            {
                Bar_PicturePrt(p,Bar_tab.bar_heigh);
                return;
            }
            else if((Bar_tab.barnum_flag == 0) &&(Bar_tab.barpic_flag == 1)
                    &&(Bar_tab.space_flag == 0))
            {
                Bar_DatPrt(NUM_DOWN);
                return;
            }
            else if((Bar_tab.barnum_flag == 1) &&(Bar_tab.barpic_flag == 1)
                    &&(Bar_tab.space_flag == 0))
            {
                Bar_SpacePrt();
            }
            break;
        default :
            break;
        }

        if(Bar_tab.barpic_flag && Bar_tab.space_flag && Bar_tab.barnum_flag)
        {
            Bar_PrintStop();
        }
    }
}

/*
*------------------------------------------------------------------------------*
*功   能 ：	条码打印函数
*参	  数 ：	*p指向存放条码的存储区域的首地址
*输   入 ：  p
*输   出 ： NONE
*返回 值 ： NONE
*------------------------------------------------------------------------------*
*/
void BarCode_Print( unsigned char *p)
{
#ifndef USE_CT100G
    while(prt_state&0x80);	//if it's printing fonts ,wait!如果正在打印文字则等待
#endif
    prt_mode=2;

    if(Barcode_Check(p,Bar_tab.BAR_Encode))
    {
        Bar_PrintStop();
        return;
    }

    Bar_PrintStart();//启动条码打印

    while(Bar_PrintOver_Flag == 0)
    {
        prt_flag = 1; //KYK test
        //power_printer();//---
        if(prt_flag)
        {
            line_prt_dat = Bar_PrtDat;
            switch(Bar_tab.BAR_Encode)
            {
            case BARTYP_EAN8:
            case BARTYP_UPCA:
            case BARTYP_EAN13:
                //if((BarDoubProcess_Flag & 0x02) == 0)
            {
                BAR_Position(Bar_tab.BAR_Encode,p);
                Get_EanUpcaFrame(p);
                BarDoubProcess_Flag |= 0x02;
                Bar_PrintOver_Flag = 0;
            }
            Bar_Print(p,Bar_tab.Num_Pos);
            Bar_PrintOver_Flag = 1; //KYK test
            break;
            case BARTYP_EAN128:
                BAR_Position(BARTYP_EAN128,p);
                Code128Type = EAN128;
                Bar_tab.BAR_Encode = BARTYP_CODE128;
                Bar_PrintOver_Flag = 1;
                break;
            case BARTYP_CODE128:
                //if((BarDoubProcess_Flag & 0x02) == 0) //KYK test
            {
                if(Code128Type != EAN128)BAR_Position(BARTYP_CODE128,p);
                //则不用重新计算条码的起始位置
                Get_Code128Frame(p, Code128Type);
                BarDoubProcess_Flag |= 0x02;
                //contts++;
            }
            if((Bar_tab.Num_Pos == NUM_UP) && Bar_tab.barnum_flag == 1)
            {
                Get_Code128Frame(p, Code128Type);
            }
            Bar_Print( p,Bar_tab.Num_Pos );
            Bar_PrintOver_Flag = 1; //KYK test
            break;
            default:
                break;
            }//end of switch(Bar_tab.BAR_Encode)
        }
    }
}

/*
*---------------------------------------------------------------------------*
*功   能 ：	配置条码的参数													*
*参	  数 ：	encode		 ：条码的编码方式									*
*			numpos       ：条码数字的位置（NUM_DOWN，NUM_UP,NONUM）			*
*			ZoomXX	     ：条码的放大倍数，可以是参数1,2,3					*
* 			bar_position ：条码的位置（靠左，居中，靠右）									*
*		 	BarHeigh	 ：条码图形的高度									*
*输   入 ：	encode, numpos, ZoomXX, Start_pzt, BarHeigh						*
*输   出 ： NONE															*
*返回 值 ： NONE															*
*---------------------------------------------------------------------------*
*/
void BARCode_Config(BARTYP encode,unsigned char numpos,unsigned char bar_position,unsigned char ZoomXX,unsigned char BarHeigh)
{
    //	if((encode > 3) || (encode == 0)) encode = 1; //
    if((numpos > 3) || (numpos == 0)) numpos = 3; //
    if((ZoomXX == 0) || (ZoomXX > 3) ) ZoomXX = 2;//放大倍数最大为3倍

    Bar_tab.BAR_Encode = encode;		//条码的编码方式
    Bar_tab.Num_Pos = numpos;			//条码数字的位置（上(NUM_UP)，下(NUM_DOWN)，或不打印(NONUM)）
    Bar_tab.BarPosition = bar_position;	//条码的位置（靠左(Left)，居中(Middle)，靠右(Left)）


    Bar_tab.x_mulriple = ZoomXX;		//条码的横向放大倍数
    Bar_tab.bar_heigh = BarHeigh;		//条码的高度

    if((encode==BARTYP_EAN8) || (encode==BARTYP_EAN13)
            || (encode==BARTYP_UPCA))
    {
        EAN_UPCA_Init();
    }
}

/*
*---------------------------------------------------------------------------*
*功   能 ：	条码参数初始化												    *
*输   入 ：
*输   出 ： NONE															*
*返回 值 ： NONE															*
*描   述 ： 默认情况下打印EAN13条码，条码字符在条码下方，放大倍数三倍
*			起始位置是第10个像素点，条码高度70个像素点
*---------------------------------------------------------------------------*
*/
void BARCode_Init(void)
{
    Bar_tab.BAR_Encode = BARTYP_EAN13;
    Bar_tab.Num_Pos = NUM_DOWN;
    Bar_tab.BarPosition = Middle;
    Bar_tab.x_mulriple = 2;
    Bar_tab.bar_heigh = 50;
    EAN_UPCA_Init();
}

