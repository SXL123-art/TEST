#ifndef _PRT_DRIVER_H_
#define _PRT_DRIVER_H_

//#include "91x_lib.h"
//#include "bit_define.h"
//#include "91x_gpio.h"
//#include "driver_common.h"
#include "EngFont.h"
#include "ext_var.h"
#include "ext_fun.h"
#include "define.h"
//#include "printer/LOGOCAS.h"

#ifdef COUNTRY_CHINA//USE_CHN_GB2312_16x16
#include "ChineseFont.h"
#endif

//extern void GPIO_PinDir(GPIO_TypeDef* GPIOx, uint8 gpio_pin,uint8 Dir,uint8 Type,
//                                uint8 IPInputConnected,uint16 Alternate);
/*----------控制引脚定义-------------------------------------------------------*/
#ifdef USE_ENET
/*-------------choose the type of heating (two groups or three groups)---------------*/
#define _HEAT_TWO_GROUP
//#define _HEAT_THREE_GROUP

#ifdef _HEAT_THREE_GROUP
#define _HEAT_TWO_GROUP
#endif

/*----------------2011.3.10- added by QZY----------------------*/
/*------------------PORT DEFINE-----------------------------------------------------------------------*/
#define PRT_OUT_PORT                            GPIO2	//25
#define PRT_SCLK_PORT                           GPIO2	//24
#define PRT_LATCH_PORT                          GPIO7	//73

#define PRT_MOTOR_STEPA_PORT                    GPIO7	//75
#define PRT_MOTOR_STEPAA_PORT                   GPIO6	//62
#define PRT_MOTOR_STEPB_PORT                    GPIO8	//80
#define PRT_MOTOR_STEPBB_PORT                   GPIO6	//63

#ifdef _HEAT_TWO_GROUP //分两组加热，编译此段代码

#define PRT_STB_GROUP1_PORT                     GPIO7
#define PRT_STB_GROUP2_PORT                     GPIO2
#endif  //end of #ifdef _HEAT_TWO_GROUP
#ifdef _HEAT_THREE_GROUP
/*insrd your code of three groupe heating */
#define PRT_STB_GROUP3_PORT                     GPIO7
#endif //end of #ifdef _HEAT_THREE_GROUP
#define PRT_TEMP_PORT                           GPIO4	//41
#define PRT_TEMP_HEIGH_PORT                     GPIO2	//20
#define PRT_PPND_PORT                           GPIO7	//72
/*-----------------PINS DEFINE------------------------------------------------------------------------*/
#define PRT_OUT_PIN	                            GPIO_Pin_5		//数据输入
#define PRT_SCLK_PIN                            GPIO_Pin_4		//时钟输出
#define PRT_LATCH_PIN                           GPIO_Pin_3		//数据锁存

#define PRT_MOTOR_STEPA_PIN                     GPIO_Pin_5		//步进电机控制1
#define PRT_MOTOR_STEPAA_PIN                    GPIO_Pin_2		//
#define PRT_MOTOR_STEPB_PIN                     GPIO_Pin_0		//
#define PRT_MOTOR_STEPBB_PIN                    GPIO_Pin_3		//
#ifdef _HEAT_TWO_GROUP
/*---打印机分两组打印---*/
#define PRT_STB_GROUP1_PIN                      GPIO_Pin_1	    //-----加热分成两组------------
#define PRT_STB_GROUP2_PIN                      GPIO_Pin_1	    //
#endif  //end of #ifdef _HEAT_TWO_GROUP
#ifdef _HEAT_THREE_GROUP
/*insrd your code of three groupe heating */
#define PRT_STB_GROUP3_PIN                      GPIO_Pin_4
#endif    //end of #ifdef _HEAT_THREE_GROUP
#define PRT_TEMP_PIN                            GPIO_Pin_1		//温度AD
#define PRT_TEMP_HEIGH_PIN                      GPIO_Pin_0		//
#define PRT_PPND_PIN                            GPIO_Pin_2		//是否有纸标志
/*------------PINS  DATA READ AND WRITE---------------------------------------------------------------*/
#define PRT_OUT_PIN_WR	                        P2_5		    //数据输入
#define PRT_SCLK_PIN_WR                         P2_4    		//时钟输出
#define PRT_LATCH_PIN_WR                        P7_3    		//数据锁存

#define PRT_MOTOR_STEPA_PIN_WR                  P7_5      		//
#define PRT_MOTOR_STEPAA_PIN_WR                 P6_2  		    //
#define PRT_MOTOR_STEPB_PIN_WR                  P8_0      		//步进电机控制1
#define PRT_MOTOR_STEPBB_PIN_WR                 P6_3   		//

#ifdef _HEAT_TWO_GROUP//2 组
#define PRT_STB_GROUP1_WR                       P7_1	        //-----加热分成两组------------
#define PRT_STB_GROUP2_WR                       P2_1	        //
#endif //end of #ifdef _HEAT_TWO_GROUP//2 组
#ifdef _HEAT_THREE_GROUP
#define PRT_STB_GROUP3_WR                       P7_4
#endif //end of #ifdef _HEAT_THREE_GROUP

#define PRT_TEMP_PIN_WR                         P4_1    		//温度AD
#define PRT_TEMP_HEIGH_PIN_WR                   P2_0    		//
#define PRT_PPND_PIN_WR                         P7_2

#else
//============Old Printer==============================================================
/*--------------------PORT DEFINE----------------------------------------------*/
#define PRT_OUT_PORT                            GPIO2	//2_5
#define PRT_SCLK_PORT                           GPIO2	//2_4
#define PRT_LATCH_PORT                          GPIO2	//2_2

#define PRT_MOTOR_STEPA_PORT                    GPIO2	//2_3
#define PRT_MOTOR_STEPAA_PORT                   GPIO8	//8_4
#define PRT_MOTOR_STEPB_PORT                    GPIO8	//8_6
#define PRT_MOTOR_STEPBB_PORT                   GPIO8	//8_5

#define PRT_STB1_PORT                           GPIO8	//8_3
#define PRT_STB2_PORT                           GPIO6	//6_1
#define PRT_STB3_PORT                           GPIO8	//8_2
#define PRT_STB4_PORT                           GPIO8	//8_1
#define PRT_STB5_PORT                           GPIO5	//5_3
#define PRT_STB6_PORT                           GPIO8	//8_0
#define PRT_TEMP_PORT                           GPIO4	//4_1
#define PRT_TEMP_HEIGH_PORT                     GPIO6	//6_0
#define PRT_PPND_PORT                           GPIO6	//6_3
/*-----------------PINS DEFINE-------------------------------------------------*/
#define PRT_OUT_PIN	                            GPIO_Pin_5		//数据输入
#define PRT_SCLK_PIN                            GPIO_Pin_4		//时钟输出
#define PRT_LATCH_PIN                           GPIO_Pin_2		//数据锁存

#define PRT_MOTOR_STEPA_PIN                     GPIO_Pin_3		//步进电机控制1
#define PRT_MOTOR_STEPAA_PIN                    GPIO_Pin_4		//
#define PRT_MOTOR_STEPB_PIN                     GPIO_Pin_6		//
#define PRT_MOTOR_STEPBB_PIN                    GPIO_Pin_5		//

#define PRT_STB1_PIN                            GPIO_Pin_3		//热敏打印头加热元件分组1
#define PRT_STB2_PIN                            GPIO_Pin_1		//
#define PRT_STB3_PIN                            GPIO_Pin_2		//
#define PRT_STB4_PIN                            GPIO_Pin_1		//
#define PRT_STB5_PIN                            GPIO_Pin_3		//
#define PRT_STB6_PIN                            GPIO_Pin_0		//
#define PRT_TEMP_PIN                            GPIO_Pin_1		//温度AD
#define PRT_TEMP_HEIGH_PIN                      GPIO_Pin_0		//
#define PRT_PPND_PIN                            GPIO_Pin_3		//是否有纸标志
/*------------PINS  DATA READ AND WRITE----------------------------------------*/
#define PRT_OUT_PIN_WR	                        P2_5		    //数据输入
#define PRT_SCLK_PIN_WR                         P2_4    		//时钟输出
#define PRT_LATCH_PIN_WR                        P2_2    		//数据锁存

#define PRT_MOTOR_STEPA_PIN_WR                  P2_3    		//步进电机控制1
#define PRT_MOTOR_STEPAA_PIN_WR                 P8_4    		//
#define PRT_MOTOR_STEPB_PIN_WR                  P8_6    		//
#define PRT_MOTOR_STEPBB_PIN_WR                 P8_5    		//

#define PRT_STB1_PIN_WR                         P8_3    		//热敏打印头加热元件分组1
#define PRT_STB2_PIN_WR                         P6_1    		//
#define PRT_STB3_PIN_WR                         P8_2    		//
#define PRT_STB4_PIN_WR                         P8_1    		//
#define PRT_STB5_PIN_WR                         P5_3    		//
#define PRT_STB6_PIN_WR                         P8_0    		//
#define PRT_TEMP_PIN_WR                         P4_1    		//温度AD
#define PRT_TEMP_HEIGH_PIN_WR                   P6_0    		//
#define PRT_PPND_PIN_WR                         P6_3


#endif   //end of " #ifdef USE_ENET "   -------------------------------------



/*PINS DIRECTION DEFINE*/
#define PRT_OUT_DIR()               GPIO_PinDir(PRT_OUT_PORT, PRT_OUT_PIN, GPIO_PinOutput, \
                                                GPIO_Type_PushPull, \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_OutputAlt1)	//输出
#define PRT_SCLK_DIR()              GPIO_PinDir(PRT_SCLK_PORT, PRT_SCLK_PIN, GPIO_PinOutput, \
                                                GPIO_Type_PushPull, \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_OutputAlt1)	//输出
#define PRT_LATCH_DIR()             GPIO_PinDir(PRT_LATCH_PORT, PRT_LATCH_PIN, GPIO_PinOutput, \
                                                GPIO_Type_PushPull, \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_OutputAlt1)	//输出
/*-----------------------------------------------------------------------------------------------*/
#define PRT_MOTOR_STEPA_DIR()       GPIO_PinDir(PRT_MOTOR_STEPA_PORT, PRT_MOTOR_STEPA_PIN, \
                                                GPIO_PinOutput, \
                                                GPIO_Type_PushPull, \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_OutputAlt1)	//输出
#define PRT_MOTOR_STEPAA_DIR()      GPIO_PinDir(PRT_MOTOR_STEPAA_PORT, PRT_MOTOR_STEPAA_PIN,\
                                                GPIO_PinOutput, \
                                                GPIO_Type_PushPull, \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_OutputAlt1)	//输出
#define PRT_MOTOR_STEPB_DIR()       GPIO_PinDir(PRT_MOTOR_STEPB_PORT, PRT_MOTOR_STEPB_PIN, \
                                                GPIO_PinOutput, \
                                                GPIO_Type_PushPull, \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_OutputAlt1)	//输出
#define PRT_MOTOR_STEPBB_DIR()      GPIO_PinDir(PRT_MOTOR_STEPBB_PORT, PRT_MOTOR_STEPBB_PIN,\
                                                GPIO_PinOutput, \
                                                GPIO_Type_PushPull, \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_OutputAlt1)	//输出
/*-----------------------------------------------------------------------------------------------*/
#ifdef _HEAT_TWO_GROUP
#define PRT_STB_GROUP1_DIR()        GPIO_PinDir(PRT_STB_GROUP1_PORT, PRT_STB_GROUP1_PIN, GPIO_PinOutput, \
                                                GPIO_Type_PushPull, \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_OutputAlt1)	//输出
#define PRT_STB_GROUP2_DIR()        GPIO_PinDir(PRT_STB_GROUP2_PORT, PRT_STB_GROUP2_PIN, GPIO_PinOutput, \
                                                GPIO_Type_PushPull, \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_OutputAlt1)	//输出
#ifdef _HEAT_THREE_GROUP //if heating use three group ,and then compile the code below
/*insrd your code of three groupe heating */
#define PRT_STB_GROUP3_DIR()        GPIO_PinDir(PRT_STB_GROUP3_PORT, PRT_STB_GROUP3_PIN, GPIO_PinOutput, \
                                                GPIO_Type_PushPull, \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_OutputAlt1)	//输出
#endif  //end of #ifdef _HEAT_THREE_GROUP

#else //#ifdef _HEAT_TWO_GROUP

#define PRT_STB1_DIR()              GPIO_PinDir(PRT_STB1_PORT, PRT_STB1_PIN, GPIO_PinOutput, \
                                                GPIO_Type_PushPull, \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_OutputAlt1)	//输出
#define PRT_STB2_DIR()              GPIO_PinDir(PRT_STB2_PORT, PRT_STB2_PIN, GPIO_PinOutput, \
                                                GPIO_Type_PushPull, \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_OutputAlt1)	//输出
#define PRT_STB3_DIR()              GPIO_PinDir(PRT_STB3_PORT, PRT_STB3_PIN, GPIO_PinOutput, \
                                                GPIO_Type_PushPull, \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_OutputAlt1)	//输出
#define PRT_STB4_DIR()              GPIO_PinDir(PRT_STB4_PORT, PRT_STB4_PIN, GPIO_PinOutput, \
                                                GPIO_Type_PushPull, \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_OutputAlt1)	//输出
#define PRT_STB5_DIR()              GPIO_PinDir(PRT_STB5_PORT, PRT_STB5_PIN, GPIO_PinOutput, \
                                                GPIO_Type_PushPull, \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_OutputAlt1)	//输出
#define PRT_STB6_DIR()              GPIO_PinDir(PRT_STB6_PORT, PRT_STB6_PIN, GPIO_PinOutput, \
                                                GPIO_Type_PushPull, \
                                                GPIO_IPInputConnected_Disable, \
                                               GPIO_OutputAlt1)	//输出
#endif //end of #ifdef _HEAT_TWO_GROUP



#define PRT_TEMP_DIR()              GPIO_PinDir(PRT_TEMP_PORT, PRT_TEMP_PIN,GPIO_PinInput, \
                                                GPIO_Type_PushPull,  \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_InputAlt1)	//输入
#define PRT_TEMP_HEIGH_DIR()        GPIO_PinDir(PRT_TEMP_HEIGH_PORT, PRT_TEMP_HEIGH_PIN,GPIO_PinInput, \
                                                GPIO_Type_PushPull,  \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_InputAlt1)	//输入
#define PRT_PPND_DIR()              GPIO_PinDir(PRT_PPND_PORT, PRT_PPND_PIN,GPIO_PinInput, \
                                                GPIO_Type_PushPull,  \
                                                GPIO_IPInputConnected_Disable, \
                                                GPIO_InputAlt1)//输入


#define PRT_DAT_OUT(HL)                         do { PRT_OUT_PIN_WR = HL; } while(0)
#define PRT_SCLK_OUT(HL)                        do { PRT_SCLK_PIN_WR = HL; } while(0)
#define PRT_LATCH_OUT(HL)                       do { PRT_LATCH_PIN_WR = HL; } while(0)
#define PRT_MOTOR_STEPA_OUT(HL)                 do { PRT_MOTOR_STEPA_PIN_WR = HL; } while(0)
#define PRT_MOTOR_STEPAA_OUT(HL)                do { PRT_MOTOR_STEPAA_PIN_WR = HL; } while(0)
#define PRT_MOTOR_STEPB_OUT(HL)                 do { PRT_MOTOR_STEPB_PIN_WR = HL; } while(0)
#define PRT_MOTOR_STEPBB_OUT(HL)                do { PRT_MOTOR_STEPBB_PIN_WR = HL; } while(0)
#define PRT_STB1_OUT(HL)                        do { PRT_STB1_PIN_WR = HL; } while(0)
#define PRT_STB2_OUT(HL)                        do { PRT_STB2_PIN_WR = HL; } while(0)
#define PRT_STB3_OUT(HL)                        do { PRT_STB3_PIN_WR = HL; } while(0)
#define PRT_STB4_OUT(HL)                        do { PRT_STB4_PIN_WR = HL; } while(0)
#define PRT_STB5_OUT(HL)                        do { PRT_STB5_PIN_WR = HL; } while(0)
#define PRT_STB6_OUT(HL)                        do { PRT_STB6_PIN_WR = HL; } while(0)
#define PRT_TEMP_IN(HL)                         do { PRT_TEMP_PIN_WR = HL; } while(0) //41
#define PRT_TEMP_HEIGH_IN(HL)                   do { PRT_TEMP_HEIGH_PIN_WR = HL; } while(0)
#define PRT_PPND_IN(HL)                         do { PRT_PPND_PIN_WR = HL; } while(0)

#define PRT_LATCH_H()                           do { PRT_LATCH_PIN_WR=1; } while(0)
#define PRT_STB1_H()                            do { PRT_STB1_PIN_WR = 1; } while(0)
#define PRT_STB2_H()                            do { PRT_STB2_PIN_WR = 1; } while(0)
#define PRT_STB3_H()                            do { PRT_STB3_PIN_WR = 1; } while(0)
#define PRT_STB4_H()                            do { PRT_STB4_PIN_WR = 1; } while(0)
#define PRT_STB5_H()                            do { PRT_STB5_PIN_WR = 1; } while(0)
#define PRT_STB6_H()                            do { PRT_STB6_PIN_WR = 1; } while(0)
#define PRT_TEMP_H()                            do { PRT_TEMP_PIN_WR = 1; } while(0)   //41
#define PRT_TEMP_HEIGH_H()                      do { PRT_TEMP_HEIGH_PIN_WR = 1; } while(0)
#define PRT_PPND_H()                            do { PRT_PPND_PIN_WR = 1; } while(0)

#define PRT_DAT_L()                             do { PRT_OUT_PIN_WR = 0; } while(0)
#define PRT_SCLK_L()                            do { PRT_SCLK_PIN_WR = 0; } while(0)
#define PRT_LATCH_L()                           do { PRT_LATCH_PIN_WR = 0; } while(0)
#define PRT_MOTOR_STEPA_L()                     do { PRT_MOTOR_STEPA_PIN_WR = 0; } while(0)
#define PRT_MOTOR_STEPAA_L()                    do { PRT_MOTOR_STEPAA_PIN_WR = 0; } while(0)
#define PRT_MOTOR_STEPB_L()                     do { PRT_MOTOR_STEPB_PIN_WR = 0; } while(0)
#define PRT_MOTOR_STEPBB_L()                    do { PRT_MOTOR_STEPBB_PIN_WR = 0; } while(0)
#define PRT_STB1_L()                            do { PRT_STB1_PIN_WR = 0; } while(0)
#define PRT_STB2_L()                            do { PRT_STB2_PIN_WR = 0; } while(0)
#define PRT_STB3_L()                            do { PRT_STB3_PIN_WR = 0; } while(0)
#define PRT_STB4_L()                            do { PRT_STB4_PIN_WR = 0; } while(0)
#define PRT_STB5_L()                            do { PRT_STB5_PIN_WR = 0; } while(0)
#define PRT_STB6_L()                            do { PRT_STB6_PIN_WR = 0; } while(0)

#define PRT_TEMP_L()                            do { PRT_TEMP_PIN_WR = 0; } while(0)        //IN-------------
#define PRT_TEMP_HEIGH_L()                      do { PRT_TEMP_HEIGH_PIN_WR = 0; } while(0)	//IN-------------
#define PRT_PPND_L()                            do { PRT_PPND_PIN_WR = 0; } while(0)		//IN-------------


#ifdef _HEAT_TWO_GROUP //分两组加热，编译此段代码
#define PRT_STB_GROUP1_OUT(HL)                  do { PRT_STB_GROUP1_WR = HL; } while(0)
#define PRT_STB_GROUP2_OUT(HL)                  do { PRT_STB_GROUP2_WR = HL; } while(0)
#define PRT_STB_GROUP1_H()                      do { PRT_STB_GROUP1_WR = 1; } while(0)
#define PRT_STB_GROUP2_H()                      do { PRT_STB_GROUP2_WR = 1; } while(0)
#define PRT_STB_GROUP1_L()                      do { PRT_STB_GROUP1_WR = 0; } while(0)
#define PRT_STB_GROUP2_L()                      do { PRT_STB_GROUP2_WR = 0; } while(0)

#ifdef _HEAT_THREE_GROUP
#define PRT_STB_GROUP3_OUT(HL)                  do { PRT_STB_GROUP2_WR = HL; } while(0)
#define PRT_STB_GROUP3_H()                      do { PRT_STB_GROUP2_WR = 1; } while(0)
#define PRT_STB_GROUP3_L()                      do { PRT_STB_GROUP2_WR = 0; } while(0)
#endif  //#ifdef _HEAT_THREE_GROUP
#endif  //#ifdef _HEAT_TWO_GROUP

//void PRT_Init(void);
//void PRT_MotorCtrl(void);
//void PRT_MotorStep(unsigned char stepx);

extern void set_dark(unsigned char dark);
//extern void darkness_speed_strobe(unsigned char dark);
extern void StartT2(void);
extern void strobe_process(void);
extern void power_printer(void);
extern void shut_prt(void);
extern void PrtPinClear(void);
extern void PRT_Init(void);
extern void PRT_MotorStep(unsigned char stepx);
extern void get_online(unsigned char * str,unsigned char * dst);
extern void tph_motor(void);
extern void PrtControl(void);
extern void Strobe1p5(void);
extern void FontPrt(void);
extern void ChessPrt(void);
extern void BarPrt(void);
extern void ImgPrt(void);
extern void StartImgPrt(void);
extern signed char GetTPHTemp(void);
#endif
