#include "./../../public/globals.h"
#ifndef USE_CT100G

#include "91x_adc.h"
#include "printer\PRT_Timer.h"
#include "define.h"

/*
Speed computing formula  :   V=250*1000/(16*T)	(when 1MHz)
*/

unsigned int OMTOR_TIM = SPEED75;                 //Timer1 value   //马达每步时间为OMTOR_TIM*2
void TIMER1_INT_HANDLER(void)
{
    TIM1->CNTR=0; //清空计数器寄存器
    extern void PrtControl(void);
    PrtControl();

    /*write any value to VIC0 VAR*/
}


void TIMER2_INT_HANDLER(void)
{
    TIM_ClearFlag(TIM2, TIM_FLAG_OC1);        // flag clear
    TIM2->CNTR=0; //清空计数器寄存器

    extern void strobe_process(void);
    strobe_process();
}

/*******************************************************************************
* Function Name  :
* Description    : Get the THP_AD value ,this function will wait until the AD conversion stopped,And then return the TPH_AD
* Input          : None
* Output         : None
* Return         : TPH_AD
*******************************************************************************/
unsigned short TPH_AD=0;
unsigned char TPH_AD_start=0;
unsigned short GetTphAD(void)
{
    TPH_AD_start = 1;
    ADC_ConversionCmd(ADC_Conversion_Start);
    while(TPH_AD_start);//wait until THP_AD_start==0  in ADC2_INTHandle()
    return TPH_AD;
}

#endif//#ifndef USE_CT100G
