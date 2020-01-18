///*****************************************************************************
/*
|*  Copyright       :   (c) 2011 CAS
|*  Filename        :   driver_ads1232.h
|*  Version         :   0.1
|*  Programmer(s)   :   QI ZHY ING (QZI)
|*  Created         :   2011/5/9
|*  Modified        :
|*  Description     :   CT100 ad driver function header
*/
//****************************************************************************/
#ifndef	_ADS1232_H_
#define	_ADS1232_H_
#include "../public/globals.h"
#define D_VALUE   	400//200

/*-------------------调用的外部函数-------------------------------------------*/
//extern void GPIO_PinDir(GPIO_TypeDef* GPIOx, uint8 gpio_pin,uint8 Dir,uint8 Type,
//					  uint8 IPInputConnected,uint16 Alternate);
//--------define  the pins------------------------------------------------------
#define ADS1232_DATA_PORT    		GPIO6
#define ADS1232_CLK_PORT 	   		GPIO7
#define ADS1232_PDOWN_PORT    		GPIO7

#define ADS1232_DATA_PIN	   		GPIO_Pin_2 //链接ad的data脚
#define ADS1232_CLK_PIN	   			GPIO_Pin_5//链接ad的clk脚
#define ADS1232_PDOWN_PIN	   		GPIO_Pin_4//连接ad的pdown 脚
//#define ADS1232_DATA_FLAG			0X40

#define ADS1232_DATA	   			P6_2
#define ADS1232_CLK	   			    P7_5
#define ADS1232_PDOWN	   			P7_4


//---------------设置引脚的输入方向---------------------------------------------
#define ADS1232_DATA_IN()		GPIO_PinDir(ADS1232_DATA_PORT, ADS1232_DATA_PIN, GPIO_PinInput, 	\
									GPIO_Type_PushPull,            		\
									GPIO_IPInputConnected_Disable,		\
									GPIO_InputAlt1)

#define ADS1232_DATA_OUT()		GPIO_PinDir(ADS1232_DATA_PORT, ADS1232_DATA_PIN, GPIO_PinOutput, 	\
									GPIO_Type_PushPull,            		\
									GPIO_IPInputConnected_Disable,		\
									GPIO_OutputAlt1)

#define ADS1232_CLK_OUT()		GPIO_PinDir(ADS1232_CLK_PORT, ADS1232_CLK_PIN, GPIO_PinOutput, 	\
								GPIO_Type_PushPull,            		\
								GPIO_IPInputConnected_Disable, 		\
								GPIO_OutputAlt1)
#define ADS1232_PDOWN_OUT()	GPIO_PinDir(ADS1232_PDOWN_PORT, ADS1232_PDOWN_PIN, GPIO_PinOutput, 	\
								GPIO_Type_PushPull,            		\
								GPIO_IPInputConnected_Disable, 		\
								GPIO_OutputAlt1)

//-----------------设置引脚的输入输出电平---------------------------------------
#define ADS1232_DATA_READ()				ADS1232_DATA
#define ADS1232_DATA_H()				do{ADS1232_DATA=1;}while(0)
#define ADS1232_DATA_L()				do{ADS1232_DATA=0;}while(0)

#define ADS1232_CLK_H()					do{ADS1232_CLK=1;}while(0)
#define ADS1232_CLK_L()					do{ADS1232_CLK=0;}while(0)

#define ADS1232_PDOWN_H()		   		do{ADS1232_PDOWN=1;}while(0)
#define ADS1232_PDOWN_L()		   		do{ADS1232_PDOWN=0;}while(0)

//IOCTL DEFINE
#include <linux/ioctl.h>
/*
#define AD_IOC_MAGIC	'a'
#define AD_IO_READ 	_IOR(AD_IOC_MAGIC, 1, u32)
#define AD_IO_CAL 	_IO(AD_IOC_MAGIC, 2)
#define AD_IO_VER 	_IOR(AD_IOC_MAGIC, 3, struct driver_ver)	//driver_ver defined in globals.h
#define AD_IOC_MAXNR	3
*/

#define AD_IOC_MAGIC	's' //second lc use s
#define AD_IO_READ 		0xfe
#define AD_IO_CAL 		_IO(AD_IOC_MAGIC, 2)
#define AD_IO_VER 		_IOR(AD_IOC_MAGIC, 3, struct driver_ver)	//driver_ver defined in globals.h
#define AD_IOC_MAXNR	3

void AD_Delayms(unsigned char t);
void AD_Delayus(unsigned char t);
void adcinit(void);
void ADS1232_Offset_Calibration(void);
unsigned char Read_Byte(void);  //读取一字节数据
unsigned long Get_AD_val(void);
unsigned long AD_Ready_Get(void);
unsigned long AD_Val_Handle(void);
//void getadc(unsigned char x);
extern char* get_ver_dev_ads1232(void);
extern void close_adc(void);



#endif



