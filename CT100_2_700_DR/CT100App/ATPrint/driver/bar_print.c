
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

unsigned int  print_lines=0;	//�����Ѿ���ӡ������


BARTYP BarType;//�����������ͣ�ö�������ݣ�

BAR_STRUCT	Bar_tab;

unsigned char BarFrame[19];
unsigned char BarNums[38];

unsigned char Bar_PrtDat[48];			//��Ž�Ҫ��ӡ���ݵĻ�����
unsigned char bar_picture[13];			//������ű����õ���
//0-1���е����飬����ͼ�λ����
//unsigned char bar_dat[13];				//���ת������ֵ�͵�����
unsigned char nums = 0,lines_num= 0;	//lines_num:�ж�һ�������Ƿ��ӡ����

unsigned char Bar_PrintOver_Flag=1;		//�����ӡ������־�������������ӡ����
// =0 :�����ӡ��
unsigned char BarDoubProcess_Flag=0;	//�˱����Ĺ��ܣ���ֹ��Ӧ�ĺ����ظ�ִ�У�
//�������ӡ�����Ժ󣬴˱�ʶ�������븴λ
//bit7 6 5 4 3
//bit2 : �ж�CODE128���������Ƿ��Ѿ��������봦��
//bit1 : �ж�EAN13���������Ƿ��Ѿ��������봦��
//bit0 : �ж��Ƿ��ѽ�������ͼ��ת������ת����bit0��λ


/*
--------------------------------------------------------------------------------
*���� : �����ӡֹͣ����
--------------------------------------------------------------------------------
*/
void Bar_PrintStop(void)
{
#ifndef USE_CT100G
    unsigned char clrlen;

    shut_prt();//ֹͣ��ӡ��

    for(clrlen = 0; clrlen<48; clrlen++) //������뻺����
    {
        Bar_PrtDat[clrlen] = 0;
        Bar_tab.TAB_stru[clrlen] = 0;
    }

    Bar_tab.barpic_flag = 0;	//
    Bar_tab.barnum_flag = 0;	//
    Bar_tab.space_flag = 0;		//
    nums=0;						//
    lines_num=0;				//
    Bar_PrintOver_Flag = 1;		//�����ӡ������־
    BarDoubProcess_Flag = 0;	//
    // ���ֹͣ
#endif
}

/*
--------------------------------------------------------------------------------
* ���� �� �����ӡ��ʼ����
--------------------------------------------------------------------------------
*/
void Bar_PrintStart(void)
{
    Bar_PrintOver_Flag = 0;//����ӡ������־����
#ifndef USE_CT100G
    power_printer();
#endif
}

/*
--------------------------------------------------------------------------------
* ���� �� ����ַ����е��ַ��Ƿ�ȫ��Ϊ���֣������򷵻�TRUE�����򷵻�FALSE
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
* ���� �� �����������������Ƿ�Ϸ�
--------------------------------------------------------------------------------
*/
unsigned char Barcode_Check(unsigned char *p,unsigned char bar_typ)
{
    unsigned char rt_dat = 0;//����ֵ

    /*�ж�������Ƿ���12λ�Ϸ������ַ���*/
    switch(bar_typ)
    {
    case BARTYP_EAN8:
    case BARTYP_UPCA:
    case BARTYP_EAN13:
        if(strlen((const char*)p) != barcode_len-1)
        {
            //���EAN13��ĳ��Ȳ���12���ַ�������������ݲ��Ϸ�
            rt_dat = 1;
        }
        if(Check_Str(p) == BARCODECHAR_FALSE)
        {
            rt_dat = 1;
        }
        break;
    case BARTYP_EAN128:
    case BARTYP_CODE128:
        rt_dat = Code128_Messages(p, Code128Type);//co128��EAN128�����ж�
        break;
    }

    return rt_dat;
}

void BAR_PosProcess(unsigned char barBitlen)
{
    unsigned short strprt;
    /*--------------�ж�����ķŴ���-----------*/
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

    /*--------------�ж������λ��-----------*/
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

    barNumBitlen = barBytelen * 7;//�����������ֵĴ�ӡ����

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

BC_type:����ı��뷽ʽ
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
        BarNumPos_count(barBytelen,barBitlen);//���������ַ�����ʼλ��
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
        BarNumPos_count(barBytelen,barBitlen);//���������ַ�����ʼλ��
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

            if(nums == 0 ) //��һ�д����
            {
                for(i=0; i< 48; i++) buf[i]=0;
            }
            else if((nums >=1) && (nums <=7 ))  //�ڶ��е��ڰ��д�ӡ����
            {
                for(i=0; i< strlen(num_buf); i++)
                {
                    buf[i]=EAN_numberTable[*(num_buf+i) - 0x30][nums-1];
                }
                Num_DotCvnt(buf);//���ֵ���ת����ȥ��ÿ���ֽ����λ
            }
            else if(nums == 8)  //�ھ��п���
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
* ���ܣ�����������Ŵ󣬿�ѡ����1,2,3
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
*���ܣ���ӡ����ͼ��
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
            Get_EAN13BarPicture(PRTBAR,WARN_LINE_EN);//�������ͼ��
            break;
        case BARTYP_CODE128:
            Get_Code128Picture(Code128Type); //���code128����ͼ��
            if(Bar_tab.x_mulriple >= 2) Bar_tab.x_mulriple = 2;
            break;
        }//end of switch(Bar_tab.BAR_Encode)
        BAR_ZoomXX(Bar_tab.x_mulriple);//�Ŵ�
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
* ���ܣ���ӡ��������
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
        if((nums<=8) && (print_lines - lines_num >=Bar_tab.x_mulriple ))//13Ϊ��������
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
            Bar_tab.barnum_flag=1;	//�����ַ���ӡ���
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
            prt_ctrl_typeface |= PRT_TYPE_BOLD; //����  Code128char
            prt_buf[prt_write++] = 0x1b; //�����ʶ��
            prt_buf[prt_write++] = 'U';
            prt_buf[prt_write++] = '1';
        }
        else if(Code128Type == CODE128_STARTB)
        {
            prt_ctrl_typeface |= PRT_TYPE_BOLD; //����  Code128char
            prt_buf[prt_write++] = 0x1b; //�����ʶ��
            prt_buf[prt_write++] = 'U';
            prt_buf[prt_write++] = '1';
        }
        else if(Code128Type == CODE128_STARTC)
        {
            prt_ctrl_typeface |= PRT_TYPE_BOLD; //����  Code128char
            //prt_buf[prt_write++] = 0x1b; //�����ʶ��
            //prt_buf[prt_write++] = 'U';
            //prt_buf[prt_write++] = '1';
        }
        else if(Code128Type == EAN128)
        {
            prt_ctrl_typeface |= PRT_TYPE_BOLD; //����  Code128char
            //prt_buf[prt_write++] = 0x1b; //�����ʶ��
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
        start_prt();  //��ӡ�����ַ�

        unsigned  int kkk = 100000;
        while(kkk > 10)
        {
            kkk--;
        }

        power_printer();
        //Bar_PrintStart();  //����start_prt();����ִ�н����Ժ�λ�˱�־λprt_flag
        //�����ڴ�ӡ����ʱ
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
* ���ܣ���ӡ����ת������ӡͼ��
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
            Bar_tab.space_flag=1;			//����ո��ӡ���
            shut_prt();
        }
    }
#endif
}



/*
*------------------------------------------------------------------------------*
*����������	EAN13�����ӡ����
*��    �룺	1) *p:ָ������洢�����׵�ַ
*			2) Numpos:����������������,���������²�����NONUM��NUM_UP��NUM_DOWN
*��    ����
*���ú�����	1) Prt_Stop();
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
*��   �� ��	�����ӡ����
*��	  �� ��	*pָ��������Ĵ洢������׵�ַ
*��   �� ��  p
*��   �� �� NONE
*���� ֵ �� NONE
*------------------------------------------------------------------------------*
*/
void BarCode_Print( unsigned char *p)
{
#ifndef USE_CT100G
    while(prt_state&0x80);	//if it's printing fonts ,wait!������ڴ�ӡ������ȴ�
#endif
    prt_mode=2;

    if(Barcode_Check(p,Bar_tab.BAR_Encode))
    {
        Bar_PrintStop();
        return;
    }

    Bar_PrintStart();//���������ӡ

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
                //�������¼����������ʼλ��
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
*��   �� ��	��������Ĳ���													*
*��	  �� ��	encode		 ������ı��뷽ʽ									*
*			numpos       ���������ֵ�λ�ã�NUM_DOWN��NUM_UP,NONUM��			*
*			ZoomXX	     ������ķŴ����������ǲ���1,2,3					*
* 			bar_position �������λ�ã����󣬾��У����ң�									*
*		 	BarHeigh	 ������ͼ�εĸ߶�									*
*��   �� ��	encode, numpos, ZoomXX, Start_pzt, BarHeigh						*
*��   �� �� NONE															*
*���� ֵ �� NONE															*
*---------------------------------------------------------------------------*
*/
void BARCode_Config(BARTYP encode,unsigned char numpos,unsigned char bar_position,unsigned char ZoomXX,unsigned char BarHeigh)
{
    //	if((encode > 3) || (encode == 0)) encode = 1; //
    if((numpos > 3) || (numpos == 0)) numpos = 3; //
    if((ZoomXX == 0) || (ZoomXX > 3) ) ZoomXX = 2;//�Ŵ������Ϊ3��

    Bar_tab.BAR_Encode = encode;		//����ı��뷽ʽ
    Bar_tab.Num_Pos = numpos;			//�������ֵ�λ�ã���(NUM_UP)����(NUM_DOWN)���򲻴�ӡ(NONUM)��
    Bar_tab.BarPosition = bar_position;	//�����λ�ã�����(Left)������(Middle)������(Left)��


    Bar_tab.x_mulriple = ZoomXX;		//����ĺ���Ŵ���
    Bar_tab.bar_heigh = BarHeigh;		//����ĸ߶�

    if((encode==BARTYP_EAN8) || (encode==BARTYP_EAN13)
            || (encode==BARTYP_UPCA))
    {
        EAN_UPCA_Init();
    }
}

/*
*---------------------------------------------------------------------------*
*��   �� ��	���������ʼ��												    *
*��   �� ��
*��   �� �� NONE															*
*���� ֵ �� NONE															*
*��   �� �� Ĭ������´�ӡEAN13���룬�����ַ��������·����Ŵ�������
*			��ʼλ���ǵ�10�����ص㣬����߶�70�����ص�
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

