


#ifndef __EXT_VAR_H__
#define __EXT_VAR_H__



//*************************PRT*****************************
#define PRT_DEF
#ifdef PRT_DEF
#define PRT_EXT
#else
#define PRT_EXT extern
#endif

//PRT_EXT unsigned char prt_flag;              						//��ӡ��־,׼��Ҫ��ӡ,������1  ; ��ӡ��ʼ����0
//unsigned char print_oneline[MAX_PRINT_BYTEWIDTH];  //Ҫ��ӡ1�е�����
PRT_EXT unsigned char motor_step;
PRT_EXT unsigned int tph_prt_line;                     // ��ӡ�������м���  //����ӡ�˶�������  //���ֵӦ��С��= tph_prt_height
PRT_EXT unsigned int tph_prt_addr;                    // ����ӡ��������RAM�еĵ�ַ
PRT_EXT unsigned char tph_prt_height;                   // ��ӡ�������ĸ߶�
PRT_EXT unsigned char print_oneline[48];				// ��Ҫ��ӡ��һ���е�����
PRT_EXT unsigned char prt_flag;				// 1:��ӡ  0:ֹͣ��ӡ

PRT_EXT unsigned char prt_buf[1000];                    //the fonts or CMD will be printed shoud write to this buffer
PRT_EXT unsigned int prt_write;              //prt_buf write pointer
PRT_EXT unsigned int prt_read;               //prt_buf read pointer
PRT_EXT volatile unsigned short ch_exist;               //prt_buf[]��������

PRT_EXT unsigned short draw_x_pos;                                  // The position in prt_img_buffer        //һ�еĵڼ���λ��  //һ�й�48��λ��
PRT_EXT unsigned char eng_font_width;                               // The width of the English character width//Ӣ���ַ����		//=8��16
#ifdef COUNTRY_CHINA
PRT_EXT unsigned char cn_font_width;                                // The width of the Chinese character width//�з��ַ����
#endif
PRT_EXT unsigned char prt_state;
PRT_EXT unsigned char prt_mode;                         //=1 ������ӡģʽ ; =0 ����ģʽ

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

    unsigned char* prt_addr;          //��ӡ��ַ
    unsigned char* write_addr;        //д��ַ
    unsigned short height;            //�߶�				//PRINT_CTRL.height=1ʱ��IMG_BUF�ĸ߶�Ϊ32
    unsigned short prt_line;          //��ӡ����

} IMG_BUF;                  //�ṹ��

IMG_EXT struct IMG_BUF img_buf0;
IMG_EXT struct IMG_BUF img_buf1;
IMG_EXT struct IMG_BUF* prt_img_buffer;

IMG_EXT unsigned char space_line;
IMG_EXT unsigned char prt_ctrl_typeface;  //��ӡ����
IMG_EXT unsigned char prt_ctrl_width;     //��ӡ���
IMG_EXT unsigned char prt_ctrl_height;    //��ӡ�߶�

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
extern unsigned char Bar_PrtDat[48];            //��Ž�Ҫ��ӡ���ݵĻ�����
extern unsigned char bar_picture[13];           //������ű����õ���
//0-1���е����飬����ͼ�λ����
//unsigned char bar_dat[13];                    //���ת������ֵ�͵�����
extern unsigned char nums,lines_num;            //lines_num:�ж�һ�������Ƿ��ӡ����
extern unsigned char Bar_PrintOver_Flag;        //�����ӡ������־�������������ӡ����
// =0 :�����ӡ��
extern unsigned char BarDoubProcess_Flag;	//�˱����Ĺ��ܣ���ֹ��Ӧ�ĺ����ظ�ִ�У�
//�������ӡ�����Ժ󣬴˱�ʶ�������븴λ
//bit7 6 5 4 3 2
//bit1 :�ж����������Ƿ��Ѿ��������봦��
//bit0 : �ж��Ƿ��ѽ�������ͼ��ת������ת����bit0��λ
extern unsigned short Code128Type;//code128��ı��뷽ʽ��־�������ǲ���
#endif
#undef BAR_EXTDEF
extern unsigned char Code128char[38];
extern unsigned short Code128Type;//code128��ı��뷽ʽ��־�������ǲ���
extern unsigned char left_len,rignt_len;//����EAN8,EAN13��UPCA��������ĳ���
extern unsigned char barcode_len; //����EAN8,EAN13��UPCA��������ĳ���
extern const unsigned char EAN_EncodingTable[3][10];
extern const unsigned char EAN_parityTable[10][6];
extern const unsigned char EAN_numberTable[10][7];//0 - 9 �����ֿ�
extern const unsigned short Code128Tab[107];//code128������
extern const  unsigned char EngFont[];//Ӣ���ֿ�
extern unsigned int  print_lines;//�����Ѿ���ӡ������
/*----------------------------------------------------------------------*/
#endif
