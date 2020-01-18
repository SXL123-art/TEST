/*
*===============================================================================
*���ܣ�CODE128���루A��B��C���ֱ��뷽ʽ����EAN128�����ͼ�δ���
*˵�����ڷŴ���Ϊ2��������£�
				CODE128�����A,B���ֱ��뷽ʽ�����Ա���14���ַ�
*				C���뷽ʽ���Ա���28���ַ�
*				EAN128���������Ա���26���ַ�
* 		�ڷŴ���Ϊ3��������£�
*				CODE128�����A,B���ֱ��뷽ʽ���Ա���9���ַ�
*				CODE128�����C���뷽ʽ�����Ա���18���ַ�
*				EAN128���������Ա���16���ַ����Ŵ���2x��
*
*������볤�ȷ�Χ����������������Χ�����ش�����Ϣ��BAR_OVERFLOW��,������ӡ����
*===============================================================================
*/

#include <string.h>
#include <stdio.h>
#include "define.h"
#include "ext_var.h"
#include "ext_fun.h"
#include "bar_print.h"

unsigned short 	Code128Frame[19];       //��ž��a����ļ�
unsigned char 	Code128char_p[38];      //��ŗl�a�ַ��ھ��a���е�λ��
unsigned char 	Code128char[38];        //��ŗl�a����
unsigned short Code128Type = CODE128_STARTC;//code128��ı��뷽ʽ��־�������ǲ���
//CODE128_STARTA,CODE128_STARTB,CODE128_STARTC

/*
--------------------------------------------------------------------------------
*���ܣ������code128��У����
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
*���ܣ��������ͼ��
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
* ���ܣ��ж�����ĳ����Ƿ񳬳���ӡ��Χ
* 		����0����ʾû�г�����Χ
* 		����BAR_OVERFLOW����ʾ������ӡ��Χ
--------------------------------------------------------------------------------
*/
unsigned char Code128_Messages(unsigned char *p, unsigned short codeABC)
{
    unsigned char char_len,bar_msg = 0;

    char_len = strlen((char const *)p);//��������ĳ���
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

    if (((codeABC == CODE128_STARTC) || (codeABC == EAN128)) 	//���code128���C�����EAN128��������ַ�ʱ����������
            && ((char_len % 2) || (Check_Str(p) == BARCODECHAR_FALSE))) //�з������ַ�������£�������������ǷǷ���
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
�������� ��	�������ı���
��	  �� ��	*p��ָ��pָ�������ַ��洢������׵�ַ
			codeABC��ָ������ı��뷽ʽ�������ǲ���
					CODE128_A��CODE128_B��CODE128_C
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

    bar_len = strlen((char const *)p);//��������ĳ���
    for(i = 0; i < bar_len; i++)
    {

        Code128char[i] = *(p+i);

    }

    if(codeABC == CODE128_STARTC || codeABC == EAN128) bar_len /=2;

    for(i = 1; i <= bar_len; i++ )//ѭ��������������ַ����ȶ���
    {
        switch(codeABC)
        {
        case CODE128_STARTA: //A���뷽ʽ
            if((*p>=32) && ((*p <= 95)))
            {
                dat_pt = *p - 32;
            }

            p++;
            break;

        case CODE128_STARTB: //B���뷽ʽ
            if((*p >= 32) && (*p <= 126))
            {
                dat_pt = *p - 32;
            }

            p++;
            break;

        case EAN128:
        case CODE128_STARTC: //C���뷽ʽ
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

    Code128char_p[i] = Get_Code128VarifyFlag(Code128char_p,bar_len);//����У����
    Code128Frame[i] = Code128Tab[Code128char_p[i]];
}
