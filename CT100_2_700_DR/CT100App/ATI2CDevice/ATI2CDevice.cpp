
#include "ATI2CDevice.h"
#include "I2C_Device.h"


CI2CDevice CIICDevice;

int ATIICInit()
{
	CIICDevice.init();
	return 0;
}

int ATIICRelease()
{
	CIICDevice.release();
}

//PCF8563 读
int ATRtcRead(char* pDes)
{
    char src[7] = {0};
    CIICDevice.parse(src, IIC_RTC_READ);
    sprintf(pDes, "%04d-%02d-%02d %02d:%02d:%02d", src[0]+2000, src[1], src[2], src[3], src[4], src[5]);
    return 0;
}

//PCF8563 写，格式20141229101234
int ATRtcWrite(char* pDes)
{
    int len = strlen(pDes);
    if(len != 12)
    {
        printf("time format is error!\n");
    }
    else
    {
        char des[7]={0};
        StrToHex(pDes, (unsigned char *)des);
        // 设置RTC 时间des 14 12 29 10 12 23
        CIICDevice.parse(des, IIC_RTC_WRITE);
        // 设置系统时间
        char temp[40] = {0};
        sprintf(temp,"date -s \"%s%02x-%02x-%02x %02x:%02x:%02x\"", "20", des[0], des[1], des[2], des[3], des[4], des[5]);
        printf("\ntime:%s\n", temp);
        system(temp);
    }
        //CIICDevice.parse(pDes, IIC_RTC_WRITE);
        //return 1;
}
