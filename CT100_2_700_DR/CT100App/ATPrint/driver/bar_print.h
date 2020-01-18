#ifndef _BAR_PRINT_H_
#define _BAR_PRINT_H_


/*-------�������������־��Ϣ---------------------*/
#define BARCODECHAR_FALSE		0   //����������ַ����Ϸ�
#define BARCODECHAR_TRUE		1   //����������ַ��Ϸ�
#define BAR_OVERFLOW			2   //��������볤�ȳ�����ӡ�����ܴ�ӡ����󳤶�


/*-------�������������--------------------------*/
//#define BAR_TYPE_EAN13		1
//#define BAR_TYPE_CODE128		2
//#define BAR_TYPE_EAN128		3
typedef enum {BARTYP_EAN13, BARTYP_EAN128, BARTYP_CODE128,BARTYP_EAN8,BARTYP_UPCA} BARTYP;

/*--------------------���������λ�ã����󣬾��У����ң�----------------------------------*/
#define Left				1
#define Middle				2
#define Right				3

/*--------���������ַ���λ�ã��ϡ��� ���߲���ӡ�����ַ���-------------*/
#define NONUM				1	//ֻ��ӡ����ͼ��
#define NUM_UP				2	//13λ����������ͼ�ε�����
#define NUM_DOWN			3	//13λ����������ͼ�ε�����


#define EAN13_LEN				13	//���볤��13���ַ�
#define EAN13_LEFTHAND_LEN		6	//�������ֱ�6���ַ�
#define EAN13_RIGHTHAND_LEN		6	//�������ֱ�6���ַ�
#define PRT_LINE_LEN			48	//��ӡ��һ���е��ֽ���

#define PRTBAR  				1
#define PRTNUM					2
#define WARN_LINE_EN			1//���뾯���߿���
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
/*---CODE128�����ʼ������ֹ�붨��-------------------------*/
#define CODE128_STARTA          0X684 	//code128A����ʼ��
#define CODE128_STARTB          0X690 	//code128B����ʼ��
#define CODE128_STARTC          0X69C 	//code128C����ʼ��
#define EAN128                  0x7ae   //FNC1(EAN128��)
#define CODE128_STOP            0X18EB  //���������־λ

/*--------��������Ľṹ-----------*/
typedef struct
{
    unsigned char BAR_Encode;		//�������ͣ�EAN13��EAN128��CODE128 ...��
    unsigned char TAB_stru[48];		//һ���е��������ݻ���

    unsigned short bar_heigh;		//����ĸ߶�
    unsigned char x_mulriple;		//�����ȵķŴ����������ǲ���1,2,3��
    unsigned short start_position;	        //�������ʼλ��
    unsigned short Numstart_position;	//�������ʼλ��
    unsigned char Num_Pos;			//�����ַ���λ�ã�NUM_UP �� NUM_DOWN �� NONUM��
    unsigned char BarPosition;              //�����λ�ã���࣬�Ҳ࣬���У�

//	unsigned char y_mulriple;
//	unsigned short coord_x;			//�����ӡ������λ��
//	unsigned short coord_y;			//������

    unsigned char space_flag;		//�ո��ӡ��ɱ�־
    unsigned char barpic_flag;		//����ͼ�δ�ӡ��ɱ�־
    unsigned char barnum_flag;		//�����ַ���ӡ��ɱ�־
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















