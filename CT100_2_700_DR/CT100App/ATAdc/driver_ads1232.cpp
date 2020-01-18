///*****************************************************************************
/*
|*  Copyright       :   (c) 2011 CAS
|*  Filename        :   driver_ads1232.c
|*  Version         :   0.1
|*  Programmer(s)   :   QI ZHY ING (QZI)
|*  Created         :   2011/5/9
|*  Modified        :
|*  Description     :   CT100 ad driver function
*/
//****************************************************************************/
//==============================================================================
//说明：
//     1) 在使用ADC驱动函数之前，在主函数中先调用初始化函数adcinit();
//     2)在主函数中采用查询的方式读取AD转换值，getadc(unsigned char x)其中参数
//  x=0读取的是未经滤波处理的原始值adc,x=1时，读取经过滤波的AD值kg
//
//==============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "driver_ads1232.h"
//#include "IO_config/IO_ADS1232.c"
#define BUF_CNTS                    8//6

static int adc_ads1232_fd;

unsigned long AD_ValBuf[BUF_CNTS]={0};
static unsigned char AD_CNTS=0;//BUF_CNTS=8;//KYK
//static unsigned char AD_DatReady=0;
struct driver_ver adc_ver;
//AD延时1ms
void AD_Delayms(unsigned char t)
{
	unsigned int i,j=30000;

	for(i=0;i<t;i++)
		for(j=0;j<30000;j++);
	//SysCtlDelay(t* (SysCtlClockGet()/ 3000));?????????????????????????????
}
//AD延时1us
void AD_Delayus(unsigned char t)
{
	unsigned int i,j;

	for(i=0;i<t;i++)
		for(j=0;j<100;j++);	//??????????????????????????????????????????????
	//SysCtlDelay(t);	/??????????????????????????????????????????????
}

#define MAF_BUF_SIZE    60
static int32 MAFBuf[MAF_BUF_SIZE]; /** @brief MAF 滚欺*/
static uint8 MAFBufIndex;           /** @brief MAF 滚欺 牢郸胶*/
static uint8 MAFWinSize;            /** @brief MAF 扩档快 荤捞令*/
static uint8 MAFWinMaxSize;         /** @brief MAF 扩档快 max 荤捞令*/
static uint8 MAFWinMinSize;         /** @brief MAF 扩档快 min 荤捞令*/
static int32 MAFWinBreakThres;     /** @brief MAF 扩档快甫 Break窍绰 版拌 荐摹*/
static int32 MAFWinIncThres;       /** @brief MAF 扩档快甫 刘啊矫虐绰 版拌 荐摹*/
static int32 MAFfilteredData;      /** @brief MAF output 蔼*/
//static INT32S MAFfilterRefData;     /** @brief MAF 盔档快 刘啊, Break 扁霖 蔼*/
static int32 MAFshockRefData;      /** @brief MAF shock 魄沥 扁霖蔼 */
static int32 MAFshockThres;        /** @brief MAF shock 魄沥 农扁 扁霖蔼 */
static int32 MAFThresIncThres;     /** @brief MAF shock 魄沥 农扁, MAF window break 扁霖蔼 刘啊狼 扁霖蔼*/
static int8 MAFshockWinSize;       /** @brief MAF shock 魄沥 扩档快 农扁 */
static int8 MAFshockCount;         /** @brief MAF shock 俺荐 函荐 */
/**
********************************************************************************
* @brief    Moving Average Filter with removing Shock Init 窃荐\n
* @param    winMinSize : window 弥家 农扁
*           winMaxSize : window 弥措 农扁 <= MAF_BUF_SIZE
*           winIncThres : window甫 刘啊 矫懦 炼扒
*           winBreakThres : window size甫 弥家拳且 炼扒
*           shockWinSize : shock 魄沥 window 农扁
*           shockThres : shock 魄沥 农扁
* @return   none
* @remark   檬扁俊 茄锅 角青
********************************************************************************
*/
void MAFSInit(uint8 winMinSize, uint8 winMaxSize, int32 winIncThres,
              int32 winBreakThres, uint8 shockWinSize, int32 shockThres, int32 thresIncThres)
{
        uint8 i;

	MAFWinMinSize = winMinSize;
	MAFWinMaxSize = winMaxSize;
	MAFWinIncThres = winIncThres;
	MAFWinBreakThres = winBreakThres;

	MAFBufIndex = 0;
	MAFWinSize = MAFWinMinSize;
	MAFfilteredData = 0;
	//MAFfilterRefData = 0;
	MAFshockRefData = 0;
	MAFshockCount = 0;
	MAFshockWinSize = shockWinSize;
	MAFshockThres = shockThres;
	MAFThresIncThres = thresIncThres;

	for (i = 0; i < MAF_BUF_SIZE; i++)
	{
		MAFBuf[i] = 0;
	}
}

int32 AbsInt32s(int32 num)
{
	if (num > 0)
	{
		return (num);
	}
	else
	{
		return (-num);
	}
}

/**
********************************************************************************
* @brief    Moving Average Filter with removing Shock 俊 单捞磐甫 涝仿窍绰 窃荐\n
* @param    inData : AD 涝仿蔼
* @return   Filering等 蔼
* @remark
********************************************************************************
*/
int32 MAFSProc(int32 inData)
{
        uint8 i;
        int32 sum;
        uint8 bufIndex, currIndex;
        int32 minFilteredData;
        int32 prevMinFilteredData;
        int32 diffData;
        int8 sign;
        static int8 oldSign;
        int32 shockThres;
        int32 winBreakThres;
        int32 winIncThres;
#ifdef FAST_BREAK_SHOCK_FILTER
	INT32S shockBreakThres;
#endif

	MAFBufIndex++;
	MAFBufIndex = MAFBufIndex % MAF_BUF_SIZE;

	diffData = inData - MAFshockRefData;

	if (inData < MAFThresIncThres)
	{
		shockThres = MAFshockThres;
		winBreakThres = MAFWinBreakThres;
		winIncThres = MAFWinIncThres;
	}
	else
	{
		shockThres = MAFshockThres + MAFshockThres;
		winBreakThres = MAFWinBreakThres + MAFWinBreakThres;
		winIncThres = MAFWinIncThres + MAFWinIncThres;
	}

#ifdef FAST_BREAK_SHOCK_FILTER
	shockBreakThres = 128 * shockThres;//about 200d
#endif

	if (AbsInt32s(diffData) > shockThres)
	{
#ifdef FAST_BREAK_SHOCK_FILTER
		if (AbsInt32s(diffData) > shockBreakThres)
		{
			MAFshockCount = MAFshockWinSize;
		}
#endif
		if (diffData > 0) sign = 1;
		else if (diffData < 0) sign = -1;
		else sign = 0;

		MAFshockCount++;

		if (MAFshockCount > MAFshockWinSize)
		{
			//SBUF = 'H';
			MAFshockCount = MAFshockWinSize;
			currIndex = MAFBufIndex;
		}
		else
		{
			if (MAFshockCount != 1 && oldSign != sign)
			{
				MAFBufIndex = (MAFBufIndex + MAF_BUF_SIZE - (MAFshockCount - 1)) % MAF_BUF_SIZE;
				MAFshockCount = 1;
				currIndex = (MAFBufIndex + MAF_BUF_SIZE - 1) % MAF_BUF_SIZE;
				//SBUF = 'C';
			}
			else
			{
				if (MAFshockCount == MAFshockWinSize)
				{
					MAFBufIndex = (MAFBufIndex + MAF_BUF_SIZE - MAFshockCount) % MAF_BUF_SIZE;
					currIndex = MAFBufIndex;
					//SBUF = 'A';
				}
				else
				{
					currIndex = (MAFBufIndex + MAF_BUF_SIZE - MAFshockCount) % MAF_BUF_SIZE;
					//SBUF = '+';
				}
			}
		}
		oldSign = sign;
	}
	else
	{
		if (MAFshockCount > 0 && MAFshockCount < MAFshockWinSize)
		{
		 	MAFBufIndex = (MAFBufIndex + MAF_BUF_SIZE - MAFshockCount) % MAF_BUF_SIZE;
		}
		//SBUF = 'S';
		MAFshockCount = 0;
		currIndex = MAFBufIndex;
	}
	MAFBuf[MAFBufIndex] = inData;

////////////////////////////////////////////
//Average Min. Size

	sum = MAFWinMinSize / 2;
	for (i = 0; i < MAFWinMinSize; i++)
	{
		bufIndex = (currIndex + MAF_BUF_SIZE - i) % MAF_BUF_SIZE;
		sum = sum + MAFBuf[bufIndex];
	}
	minFilteredData = sum / MAFWinMinSize;

	sum = MAFWinMinSize / 2;
	for (i = 0; i < MAFWinMinSize; i++)
	{
		bufIndex = (currIndex + MAF_BUF_SIZE - MAFWinMinSize - i) % MAF_BUF_SIZE;
		sum = sum + MAFBuf[bufIndex];
	}
	prevMinFilteredData = sum / MAFWinMinSize;

	diffData = AbsInt32s(minFilteredData - prevMinFilteredData);

	if (diffData < winBreakThres)
	{
		if (diffData < winIncThres)
		{
			if (MAFshockCount == 0)
			{
				MAFWinSize++;
			}
			if (MAFWinSize > MAFWinMaxSize)
			{
				MAFWinSize = MAFWinMaxSize;
			}
			MAFshockRefData = MAFfilteredData;
		}
	}
	else
	{
		MAFWinSize = MAFWinMinSize/2 + 1;
#ifdef FAST_AVERAGE_IN_LOW_AD
		if (inData < MAFThresIncThres)
		{
			if (inData < (MAFThresIncThres/2))
			{
				MAFWinSize = MAFWinSize - 2;
			}
			else
			{
				MAFWinSize = MAFWinSize - 1;
			}
		}
#endif
		MAFshockRefData = 0;
	}

	sum = MAFWinSize / 2;
	for (i = 0; i < MAFWinSize; i++)
	{
		bufIndex = (currIndex + MAF_BUF_SIZE - i) % MAF_BUF_SIZE;
		sum = sum + MAFBuf[bufIndex];
	}
	MAFfilteredData = sum / MAFWinSize;

//	MAFshockRefData = MAFfilteredData;

	return MAFfilteredData;
}
int32 adFilterProc(int32 raw)
{
        int32 lAd;

	lAd = raw;

	//Pre-Filter
	//lAd = MedianFiltProc(lAd);
	//lAd = FIRFiltProc(lAd);
	//lAd = MAFProc(lAd); //OIML 牢刘
	lAd = MAFSProc(lAd);

	return lAd;
}

int AD_Ready_Check(void);

void adcinit(void)
{
	unsigned char i;

	adc_ads1232_fd = open("/dev/ct100gc_ad", 0);
	if (adc_ads1232_fd < 0) {
		perror("can not open device ct100gc_ad!");
		exit(1);
	}

   // while(AD_Ready_Check());
   while(AD_Ready_Check());
    ADS1232_Offset_Calibration(); //Insert by YYC 2011.12.13
    AD_Ready_Get();

	for(i=0;i<BUF_CNTS;i++){
		AD_ValBuf[i]=0;
	}

    //Moving Average Filter with removing Shock. (refer to One module src)
    //testing as changing the parameter value
    MAFSInit(3,     //(adDrvSamplingRate*adMAFsetTable[adMAFsetIndex].min100msTime)/10,
             10,    //(adDrvSamplingRate*adMAFsetTable[adMAFsetIndex].max100msTime)/10,
             2,     //winIncThres,
             2,     //winBreakThres,
             3,     //(adDrvSamplingRate*adMAFshockTable[adMAFshockIndex].shock100msTime)/10,
             5,    //winShockThres,
             2);    //winThresIncThres);
}


void close_adc(void)
{
    close(adc_ads1232_fd);
}

#ifndef USE_CT100G
unsigned char Read_Byte(void)  //读取一字节数据
{
	unsigned char dat=0x00,i;

	ADS1232_CLK_H();
	for(i=0;i<8;i++)
	{
		ADS1232_CLK_H();
		if(ADS1232_DATA_READ())
			dat|=(0x80>>i);
		ADS1232_CLK_L();
	}
	return dat;
}
#endif

//-----------------------读取AD的原始值--------------------------------
unsigned long Get_AD_val(void)
{
	signed long AD_val=0;

#ifndef USE_CT100G
	AD_val = Read_Byte();
	AD_val = (AD_val<<8) + Read_Byte();
	AD_val = (AD_val<<8) + Read_Byte();

	ADS1232_CLK_H();
	AD_Delayus(1);
	ADS1232_CLK_L();
#endif

    if (read(adc_ads1232_fd, (char*)&AD_val, sizeof AD_val) != sizeof AD_val)
    {
        perror("open divece ads1232");
        exit(1);
    }

	AD_val >>= 1;
    if(AD_val & 0x400000) AD_val |= 0xFFC00000;

    AD_val += 48000;

	if(AD_val<0) AD_val = 0;
	return AD_val;
}

int AD_Ready_Check(void)
{
    int val = 0xff;

    ioctl(adc_ads1232_fd, AD_IO_READ, &val);

    return val;
}
char* get_ver_dev_ads1232(void)
{
	ioctl(adc_ads1232_fd, AD_IO_VER, &adc_ver);
	return &adc_ver.str[0];
}

//------------------------------------------------------
//function： data is ready and read the data,the data is the
//			primitive value ,not be processed
//------------------------------------------------------
unsigned long AD_Ready_Get(void)
{
	if(AD_Ready_Check()==0){
		return Get_AD_val();
	}
	return 0;
}

static uint32 final_ad = 0;
//------------------------------------------------------------
//function: processing the AD data
//------------------------------------------------------------
unsigned long AD_Val_Handle(void)
{
    unsigned char i=0,max=0,min=0;
    unsigned long adval = 0;
    static unsigned long ad_val=0,pre_ad = 0;

    if(AD_Ready_Check()==0){
        adval = Get_AD_val();
//printf("[CT500/AD] raw : %ld\n", adval); //test
    }else{
        return final_ad;
    }
//    adval=AD_Ready_Get();
//    if(adval == 0) return final_ad;

    min = max;
    if(ad_val > adval){
        if(ad_val - adval > D_VALUE) min = ~max;
    }else{
        if(adval - ad_val > D_VALUE) min = ~max;
    }
    if(min != max){
        for(i=0;i<BUF_CNTS;i++)
        {
            AD_ValBuf[i]=adval;
        }
        ad_val=adval;
    }else {
        AD_ValBuf[AD_CNTS]=adval;
        AD_CNTS++;
        if(AD_CNTS>=BUF_CNTS) AD_CNTS=0;
        max = min = 0;
        ad_val=AD_ValBuf[0];

        for(i=1;i<BUF_CNTS;i++)
        {
            if(AD_ValBuf[max]<AD_ValBuf[i]) max=i;
            if(AD_ValBuf[min]>AD_ValBuf[i]) min=i;
            ad_val+=AD_ValBuf[i];
        }

        ad_val=(ad_val-AD_ValBuf[max]-AD_ValBuf[min])/(BUF_CNTS-2);
        ad_val = (pre_ad + ad_val)/2;
    }
    pre_ad = ad_val;

    final_ad = ad_val>>3;

    return final_ad;

/* KYK, add MASF filter
    static INT32U lAd;
    INT32U rawAd;

    rawAd = 0xffffff;
    lAd = 0;
    //--------------------------------------//
    // Get Raw AD
    //--------------------------------------//
	rawAd = AD_Ready_Get();
	if (rawAd == 0) return lAd;

    //--------------------------------------//
    // Filtering
    //--------------------------------------//
    lAd = adFilterProc(rawAd);

	return lAd;
*/
}

//------------------------------------------------------------------------/
//get the ADC value														  /
//       x=0:use the current adc value									  /
//			x=1:when copare previous to current adc value,the difference  /
//				value is a little ,adc value keep the previous value .	  /
//																		  /
//------------------------------------------------------------------------/
/*
void getadc(unsigned char x)
{
	unsigned long adc=0,kg=0;//store the ADC value,adc:the primitive value   kg:the data after filtering

	switch(x)
	{
		case 0:
			adc=AD_Ready_Get();
			break;
		default:
			kg=AD_Val_Handle();
			break;
	}

}

*/
//------------------------------------------------------------
//function: ADS1232 Offset Calibration
//------------------------------------------------------------
//insert by YYC 2011.12.13
void ADS1232_Offset_Calibration(void)
{
#ifdef USE_CT100G
    ioctl(adc_ads1232_fd, AD_IO_CAL, 0);
#else
    unsigned char k;
    ADS1232_CLK_OUT();

    for(k=0; k<26; k++)
    {
        ADS1232_CLK_H();
	AD_Delayus(1);
	ADS1232_CLK_L();
        AD_Delayus(1);
    }

    ADS1232_CLK_L();
    AD_Delayus(1);
#endif
}
