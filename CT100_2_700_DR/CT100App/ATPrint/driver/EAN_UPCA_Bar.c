
/*
*===============================================================================
* ���ܣ�����EAN8��EAN13��UPC-A����ͼ��
*
* ע��������볤�ȷ�Χ������ӡ���Ĵ�ӡ��Χ���������볤�Ȳ�����EAV13��12λ��EAN8
* ��7λ��UPCA��11λ��ע���������������δ����У���룩���򲻴�ӡ���롣
*
*===============================================================================
*/
#include <string.h>
#include <stdio.h>
#include "define.h"
#include "ext_var.h"
#include "ext_fun.h"

#include "bar_print.h"

unsigned char left_len,right_len;//����EAN8,EAN13��UPCA��������ĳ���
unsigned char barcode_len = 0; //����EAN8,EAN13��UPCA��������ĳ���

/*
--------------------------------------------------------------------------------
*���ܣ������EAN13У����
--------------------------------------------------------------------------------
*/
unsigned char  EAN13_CountVarify(unsigned char *pt)
{
    unsigned short osum=0,esum=0;
    unsigned char i,verify_val;
    for(i=0; i<barcode_len-1; i+=2)
    {
        osum=osum+*(pt+i+1);	//���������ַ����
        esum=esum+*(pt+i);		//��ż�����ַ����
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
* ���� ������������������ת��������ͼ�Σ�EAN8��EAN13��UPC-A��
* ���� ��Numpos: =BARʱ���������ͼ��
*                =NUMʱ����������ַ�
--------------------------------------------------------------------------------
*/
void Get_EAN13BarPicture(unsigned char Numpos, unsigned char warn_line)
{
    unsigned short i,j;
    unsigned char p_x,bit_x,start=0x50,mid=0x28,stop=0x50,offset=0;
    //				��ʼ��	 �м���		������

    unsigned char *pt; //ָ������һ����ͼ�ε�ָ��
    unsigned char X;//�������ʼλ��

    /*�Д����ǷN���a��ʽ����?�×l�a������?�ėl�a?��*/
    if(Bar_tab.BAR_Encode == BARTYP_EAN13) offset = 1;
    else if(Bar_tab.BAR_Encode == BARTYP_EAN8)  offset = 0;
    else if(Bar_tab.BAR_Encode == BARTYP_UPCA)  offset = 0;

    X=Bar_tab.start_position;//�������ʼλ��
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

    if((Bar_tab.BAR_Encode == BARTYP_EAN13) || //��һ���ַ��Ĵ�ӡ
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
    for(i=0; i<3; i++) //��ʼ��101
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

    for(i=0; i<left_len; i++)//���ֱ�����
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
    for(i=0; i<5; i++) //�м���01010
    {
        p_x=X/8;
        bit_x=7-X%8;
        if(warn_line == WARN_LINE_EN)Dat_BitHandle(mid,i,p_x,bit_x);
        else if(warn_line == WARN_LINE_STOP) Dat_BitHandle(0X00,i,p_x,bit_x);
        X++;
    }

    for(i=0; i<right_len; i++) //���ֱ�����
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
    for(i=0; i<3; i++) //������101
    {
        p_x=X/8;
        bit_x=7-X%8;
        if(warn_line == WARN_LINE_EN) Dat_BitHandle(stop,i,p_x,bit_x);
        else if(warn_line == WARN_LINE_STOP) Dat_BitHandle(0X00,i,p_x,bit_x);
        X++;
    }

    if((Numpos==PRTNUM) && (Bar_tab.BAR_Encode == BARTYP_UPCA))//UPCA�a��У?�a�ڽY���a֮��
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
* ��   �� ���������Ľṹ��ܣ���������,����ṹ�����BarPicture[13]�У�
			�������ִ����BarNums[13]��
* ��   �� ��*ptָ��������Ĵ洢������׵�ַ
* ��   �� ��pt
* ��   �� ��NONE
* ���� ֵ ��NONE
* ��   �� �������������������Լ������ַ����ݣ��ֱ�洢��BarFram[13],BarNums[13]��
*------------------------------------------------------------------------------*
*/
void Get_EanUpcaFrame(unsigned char *pt)
{
    unsigned char dat,verify_val,i;

    //������ת������ֵ��
    for(i = 0; i < barcode_len-1 ; i++)
    {
        BarNums[i] = *(pt+i)-0x30;//�������ַ���ת������ֵ
        BarFrame[i]=0x00;//���������뻺����
    }

    //����У����
    verify_val = EAN13_CountVarify(BarNums);
    BarNums[ barcode_len-1 ]=verify_val;

    /*--------------��ʼ������������EAN-13���������������------------------------*/
    if(Bar_tab.BAR_Encode == BARTYP_EAN13)
    {
        dat = BarNums[0]; //��һ���ַ���������ַ��ı�����ż��
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
