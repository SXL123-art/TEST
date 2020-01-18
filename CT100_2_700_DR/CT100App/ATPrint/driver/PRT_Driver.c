#include "./../../public/globals.h"
#include "./../set.h"
//#include "flash.h"
#include "./../print.h"
#include "PRT_Driver.h"
#include "LOGOCAS.h"//test
#define LOGOCAS_DEF //test
void power_printer(void)
{
    prt_flag=1;
    prt_state|=0x80;
    //darkness_speed_strobe(darkness);
    /* Start the Timer counter */
    //TIM_CounterCmd(TIM1, TIM_START);            //����T1������
}

#define MAX_ONE_LINE_BYTE     48
void ImgPrt(void)
{
    // INT32U s_addr;
    // INT32U bitmap_addr_base;
    uint16 w, h, dw;
    uint8 id;
    uint8 i;

    id = 0; //BitmapStruct.Filed_id - PRT_BITMAP_IMAGE_1; //for test

    printf("for test we have changed here%d\n",__LINE__);

    //fortest
    /*
    s_addr = FLASH_SCALE_BITMAP_ADDR + (INT32U)id * FLASH_SCALE_BITMAP_SIZE;
    w = Flash_wread(s_addr);//dflash_read_short(s_addr);      //max 384 dots (width)
    h = Flash_wread(s_addr + 2);//dflash_read_short(s_addr + 2);  //max 256 dots (height)
    */

    w = 384;
    h = 48;

    if((w*h) == 0) goto EXIT;
    if(((uint32)w*h) > (FLASH_SCALE_BITMAP_SIZE*8)) goto EXIT;

    dw = w >> 3;
    if (w%8) dw++;
    // bitmap_addr_base = s_addr + 4;
#ifdef USE_CT100G
//test
//cas logo image
    LogoPointer = 0;
    for (i = 0; i < h; i++)
    {
        memset(print_oneline, 0, MAX_ONE_LINE_BYTE);
        // Flash_sread(bitmap_addr_base + LogoPointer, print_oneline, 48);
        //memcpy(print_oneline, &LOGOCAS[LogoPointer], 48); //for test
        memcpy(&patternBuf[((8*3)*48)*prt_line + LogoPointer], &print_oneline, 48);
        LogoPointer += 48;
    }
    prt_line += (h/24);
    if (h%24) prt_line++;
EXIT:
    LogoPointer=0;
    //tph_prt_line=0;
    //prt_flag=0;
    //prt_state &=0x7f;
    //prt_mode=1;
#else
    Strobe1p5();
    if(motor_step==7 || motor_step==15)
    {
        tph_prt_line++;
        if(tph_prt_line >= h)
        {
EXIT:
            LogoPointer=0;
            tph_prt_line=0;
            prt_flag=0;
            prt_state &=0x7f;
            prt_mode=1;
        }
    }

    if(motor_step==4 || motor_step==12)   //����4��12��ʱ ������һ�ε�����
    {
        memset(print_oneline, 0, MAX_ONE_LINE_BYTE);

        //memcpy(print_oneline, &LOGOCAS[LogoPointer], dw); //for test
        Flash_sread(bitmap_addr_base + LogoPointer, print_oneline, dw);
        //dflash_read_nbytes(bitmap_addr_base + LogoPointer, dw, print_oneline);

        LogoPointer += dw;
        PRT_LATCH_H();
        spi0_put_str(print_oneline);
        PRT_LATCH_L();
    }
#endif
}

#ifndef USE_CT100G
/*

������������ӡ��ģ��ĵ�����������������շ���
*/

#define PRT_DEF
#define LOGOCAS_DEF
#include "printer/PRT_Driver.h"
#include "define.h"
#include "flash.h"
//#include "print.h"
#include "driver_flash.h"

unsigned int delaynum=10000;
unsigned char chess=0,chess_re=0;

extern unsigned int OMTOR_TIM;


//---------��������-------------
//void PrtPinClear(void);
//void BarPrt(void);
//void FontPrt(void);
//void ImgPrt(void);
//----------end-----------------

unsigned char MStep_tab[8]= {0x90,0x10,0x50,0x40,0x60,0x20,0xa0,0x80}; //�ҵı�,�ɴ�ӡͷ�����Ƽ�����

//{0xf0,0xD0,0x50,0x70,0x60,0xE0,0xA0,0xB0,0x90};//��ı�,��10��


void set_dark(unsigned char dark)   //set the darkness as : 1,2,3,4,5
{
    darkness=dark;
}

void darkness_speed_strobe(unsigned char dark)
{
    unsigned short AD;
    AD=GetTphAD();
    if(AD>200)
    {
        OMTOR_TIM=SPEED75;
        switch(dark)
        {
        case 1:
            strobe_time=(2100-AD/2)/6;
            break;
        case 2:
            strobe_time=(1800-AD/2)/6;
            break;
        case 3:
            strobe_time=(1500-AD/2)/6;
            break;
        case 4:
            strobe_time=(1200-AD/2)/6;
            break;
        case 5:
            strobe_time=(1000-AD/2)/6;
            break;
        default:
            break;
        }
    }
    else if((AD>100)&&(AD<=200))
    {
        switch(dark)
        {
        case 1:
            OMTOR_TIM=SPEED75;
            strobe_time=(2200-AD/2)/6;
            break;
        case 2:
            OMTOR_TIM=SPEED75;
            strobe_time=(1900-AD/2)/6;
            break;
        case 3:
            OMTOR_TIM=SPEED75;
            strobe_time=(1600-AD/2)/6;
            break;
        case 4:
            OMTOR_TIM=SPEED70;
            strobe_time=(1300-AD/2)/6;
            break;
        case 5:
            OMTOR_TIM=SPEED70;
            strobe_time=(1100-AD/2)/6;
            break;
        default:
            break;
        }
    }
    else if(AD<=100)
    {
        switch(dark)
        {
        case 1:
            OMTOR_TIM=SPEED75;
            strobe_time=(2300-AD/2)/6;
            break;
        case 2:
            OMTOR_TIM=SPEED75;
            strobe_time=(2000-AD/2)/6;
            break;
        case 3:
            OMTOR_TIM=SPEED75;
            strobe_time=(1700-AD/2)/6;
            break;
        case 4:
            OMTOR_TIM=SPEED50;
            strobe_time=(1530-AD/2)/6;
            break;
        case 5:
            OMTOR_TIM=SPEED50;
            strobe_time=(1320-AD/2)/6;
            break;
        default:
            break;
        }
    }
    else ;
    TIM1->OC1R=OMTOR_TIM;

}
void StartT2(void)
{
    TIM2->OC1R =strobe_time;//5;//OMTOR_TIM+5;                   //���ü���ʱ��; T2����ʱ��Ƚ�T1�ж����0.005ms
    TIM_ClearFlag(TIM2, TIM_FLAG_OC1);         // flag clear
    TIM2->CNTR=0;                              //��ռ������Ĵ���
    TIM_CounterCmd(TIM2, TIM_START);           //��T2
    VIC_ITCmd(TIM2_ITLine, ENABLE);
}

void strobe_process(void)
{
#ifndef USE_ENET
    PRT_STB1_L();
    PRT_STB2_L();
    PRT_STB3_L();
    PRT_STB4_L();
    PRT_STB5_L();
    PRT_STB6_L();
#else
#ifdef _HEAT_TWO_GROUP
    PRT_STB_GROUP1_L();
    PRT_STB_GROUP2_L();
#elif defined (_HEAT_THREE_GROUP)  //use three heat groups
    /*added the code of three group */
#endif // end of #ifdef _HEAT_TWO_GROUP
#endif //end of #ifndef USE_ENET
    TIM_CounterCmd(TIM2, TIM_STOP);
}

void power_printer(void)
{
    prt_flag=1;
    prt_state|=0x80;
    darkness_speed_strobe(darkness);
    /* Start the Timer counter */
    TIM_CounterCmd(TIM1, TIM_START);            //����T1������
}

void shut_prt(void)
{
    TIM_CounterCmd(TIM1, TIM_STOP);            //�ر�T1������
    PrtPinClear();
    print_lines=0;
    tph_prt_line=0;
    prt_state &=0x7f;
    prt_flag=0;
    prt_mode=1;
}

void PrtPinClear(void)
{
    /*��ʼ�����ų�ʼ��ƽ*/
    PRT_DAT_L();
    PRT_SCLK_L();
    PRT_LATCH_H();
    PRT_MOTOR_STEPA_L();
    PRT_MOTOR_STEPAA_L();
    PRT_MOTOR_STEPB_L();
    PRT_MOTOR_STEPBB_L();
#ifndef USE_ENET
    PRT_STB1_L();
    PRT_STB2_L();
    PRT_STB3_L();
    PRT_STB4_L();
    PRT_STB5_L();
    PRT_STB6_L();
#else
#ifdef _HEAT_TWO_GROUP //���������
    PRT_STB_GROUP1_L();
    PRT_STB_GROUP2_L();
#elif defined (_HEAT_THREE_GROUP)  //use three heat groups
    /*added the code of three group */
#endif // end of #ifdef _HEAT_TWO_GROUP
#endif //end of #ifndef USE_ENET
}

void PRT_Init(void)
{
    /*�����������������������*/
    PRT_OUT_DIR();
    PRT_SCLK_DIR();
    PRT_LATCH_DIR();
    PRT_MOTOR_STEPA_DIR();
    PRT_MOTOR_STEPAA_DIR();
    PRT_MOTOR_STEPB_DIR();
    PRT_MOTOR_STEPBB_DIR();
#ifndef USE_ENET
    PRT_STB1_DIR();
    PRT_STB2_DIR();
    PRT_STB3_DIR();
    PRT_STB4_DIR();
    PRT_STB5_DIR();
    PRT_STB6_DIR();
#else
    PRT_STB_GROUP1_DIR();
    PRT_STB_GROUP2_DIR();
#endif
    PRT_TEMP_DIR();
    PRT_TEMP_HEIGH_DIR();
    PRT_PPND_DIR();

    /*
        GPIO_PinDir(GPIO6,GPIO_Pin_2|GPIO_Pin_3 ,GPIO_PinOutput ,
                    GPIO_Type_OpenCollector,
                    GPIO_IPInputConnected_Disable,
                    GPIO_OutputAlt1);
    GPIO_PinDir(GPIO7,GPIO_Pin_5,GPIO_PinOutput ,
                    GPIO_Type_OpenCollector,
                    GPIO_IPInputConnected_Disable,
                    GPIO_OutputAlt1);
    GPIO_PinDir(GPIO8,GPIO_Pin_0,GPIO_PinOutput ,
                    GPIO_Type_OpenCollector,
                    GPIO_IPInputConnected_Disable,
                    GPIO_OutputAlt1);
    */
    PrtPinClear();
//	PRT_TEMP_L();
//	PRT_TEMP_HEIGH_L();
//	PRT_PPND_L();
    printer_init();   //prt
    set_dark(3);      //default darkness is 3
}

/*******************************************************************************
* Function Name  :
* Description    : motor step control function
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PRT_MotorStep(unsigned char stepx)
{
    PRT_MOTOR_STEPBB_OUT((MStep_tab[stepx]&0x10)&&1);     //SETP 0  --B'
    PRT_MOTOR_STEPB_OUT((MStep_tab[stepx]&0x20)&&1);      //STEP 1  --B
    PRT_MOTOR_STEPAA_OUT((MStep_tab[stepx]&0x40)&&1);     //SETP 2  --A'
    PRT_MOTOR_STEPA_OUT((MStep_tab[stepx]&0x80)&&1);      //SETP 3  --A
}

void get_online(unsigned char * str,unsigned char * dst)
{
    unsigned char i;
    for(i=0; i<48; i++)
    {
        str[i]=dst[i];
        dst[i]=0;         //clear after read
    }
}

void tph_motor(void)
{
    if(motor_step%2==0)
        PRT_MotorStep((motor_step-(motor_step/16)*16)/2);          //���������һ��

    if(prt_mode==1)
    {
#if defined(USE_CHN_GB2312_24x24) || defined(USE_ENG_12x24)
        if(++motor_step >= 16) motor_step = 0;
#elif defined(USE_CHN_GB2312_16x16) || defined(USE_ENG_8x16)
        if(++motor_step >= 48) motor_step = 0;
#endif
    }
    else
    {
        if(++motor_step >= 16) motor_step = 0;    //˵��:���Ϊ8��,��ʱ���ĳ�ֵ*2=���ÿ��ʱ��
    }
}

/*******************************************************************************
* Function Name  :
* Description    : ��ʱ�ж�����ô˺���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PrtControl(void)
{
    static unsigned char NullLine = 0;

    if(prt_flag==1)
    {
        tph_motor(); 		//�ŵ�ǰ��
        //----------------
        if(NullLine <=32)	//������һ�δ�ӡʱ�ȴ�2���п��С���Ȼԭ��һ����ӡֽ�ٴ�ӡ��һ�оͻ᲻������
        {
            NullLine++;
            return;
        }
        //----------------
        if(prt_mode==1)  //������ӡģʽ
        {
            FontPrt();
        }
        else if(prt_mode == 0)    //Chess prt
        {
            ChessPrt();
        }
        else if(prt_mode == 2)   //Barcode prt
        {
            BarPrt();
        }
        else if(prt_mode == 3)
        {
            ImgPrt();
        }
        else ;
    }
    else
    {
        shut_prt();
    }
}

void Strobe1p5(void)
{

    if(!(darkness/4))       //if darkness=1,2,3
    {
        if(motor_step%2==0)
        {
            StartT2();

#ifdef _HEAT_TWO_GROUP        ////use two heat groups
            PRT_STB_GROUP1_H();
            PRT_STB_GROUP2_H();
#elif defined (_HEAT_THREE_GROUP)  //use three heat groups
            /*added the code of three group */
#else
            PRT_STB1_H();
            PRT_STB2_H();
            PRT_STB3_H();
            PRT_STB4_H();
            PRT_STB5_H();
            PRT_STB6_H();
#endif //end of #ifdef _HEAT_TWO_GROUP

        }
    }
    else                    //darkness=4,5
    {
        StartT2();
        if(motor_step%2==0)
        {
#ifdef _HEAT_TWO_GROUP        ////use two heat groups
            PRT_STB_GROUP1_H();
#elif defined (_HEAT_THREE_GROUP)  //use three heat groups
            /*added the code of three group */
#else
            PRT_STB1_H();
            PRT_STB2_H();
            PRT_STB3_H();
#endif //end of #ifdef _HEAT_TWO_GROUP
        }
        else
        {
#ifdef _HEAT_TWO_GROUP        ////use two heat groups
            PRT_STB_GROUP2_H();
#elif defined (_HEAT_THREE_GROUP)  //use three heat groups
            /*added the code of three group */
#else
            PRT_STB4_H();
            PRT_STB5_H();
            PRT_STB6_H();
#endif //end of #ifdef _HEAT_TWO_GROUP
        }
    }
}

void FontPrt(void)
{
    unsigned char i;

    Strobe1p5();
#if defined(USE_CHN_GB2312_24x24) || defined(USE_ENG_12x24)
    if(motor_step==7 || motor_step==15)
#elif defined(USE_CHN_GB2312_16x16) || defined(USE_ENG_8x16)
    if( (motor_step==0) || (motor_step==12) || (motor_step==24) || (motor_step==36))
#endif
    {
        if(tph_prt_line < tph_prt_height)
        {
            //StartT2();
            get_online(print_oneline,(uint8*)tph_prt_addr);
            tph_prt_addr += 48;

            extern bit checkPaper(void);
            extern byte nopaper;
#ifdef USE_MAIN_4_LAYER
            checkPaper();
            if(!nopaper)        //have paper
#else
            if(1)
#endif
            {
                PRT_LATCH_H();	//ccj
                spi0_put_str(print_oneline);
            }
            else                //no paper
            {
                img_buf_init();
                draw_x_pos=0;
                motor_step =0;
                PrtPinClear();
                prt_state &=0x7f;//0x80;
                prt_state ^=0x01;
                prt_flag=0;
            }
        }
        tph_prt_line++; 	//ccj
        PRT_LATCH_L();
        //----------    //��������ߵ�3�� ���ȫ�հ�
        if(tph_prt_line==tph_prt_height+1)
        {
            for(i=0; i<48; i++)
                print_oneline[i]=0x00;
            PRT_LATCH_H();
            spi0_put_str(print_oneline);
            PRT_LATCH_L();
        }
        //---end----
        if(tph_prt_line >= (tph_prt_height + space_line) )
        {
            prt_state &=0x7f;
            prt_state ^= 0x01;
            prt_flag =0;
        }
    }
}

void ChessPrt(void)
{
    unsigned char i;
    Strobe1p5();
    if(motor_step==7 || motor_step==15)
    {
        if(tph_prt_line%8 == 0)
        {
            chess_re = ~chess_re;
            chess=chess_re;
        }
        tph_prt_line++;

        if(tph_prt_line>=800) //for test 10 cm chess
        {
            tph_prt_line=0;
            prt_flag=0;
            prt_state &=0x7f;
            prt_mode=1;
        }
    }

    if(motor_step==4 || motor_step==12)   //����4��12��ʱ ������һ�ε�����
    {
        for(i=0; i<48; i++)
        {
            print_oneline[i]=chess;
            chess = ~chess;
        }

        PRT_LATCH_H();
        spi0_put_str(print_oneline);
        PRT_LATCH_L();
    }
}

void BarPrt(void)
{
    Strobe1p5();
    if(motor_step==7 || motor_step==15)
    {
        print_lines++;
    }

    if(motor_step==4 || motor_step==12)   //����4��12��ʱ ������һ�ε�����
    {
        PRT_LATCH_H();
        spi0_put_str(Bar_PrtDat);
        PRT_LATCH_L();
    }
}

void StartImgPrt(void)
{
#ifndef USE_CT100G
    while(prt_state&0x80);	//if it's printing fonts ,wait!
#endif
    prt_flag=1;
    prt_mode=3;
    tph_prt_line=0;
    LogoPointer=0;
    power_printer();
}


/*******************************************************************************
* Function Name  :
* Description    : Get the THP temperature (��)
* Input          : None
* Output         : None
* Return         : THP temperature  (signed char)
*******************************************************************************/
signed char GetTPHTemp(void)
{
    signed char temp;
    temp=(GetTphAD() - 100)/8;
    if((temp >= 100)||(temp <= -20)) return 0; //Temperature >=100 Or <=-20 degree.
    return temp;
}

#endif//#ifndef USE_CT100G
