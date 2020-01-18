#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rtc.h"
#include "../ATLib/SysApi.h"
#include "../ATI2CDevice/ATI2CDevice.h"


DateTime CRtc::RtcGetTime()
{
	char src[8] = {0};
	ATRtcRead(src);

	DateTime time;

	time.second = src[5];
	time.minute = src[4];
	time.hour = src[3];
	time.day = src[2];
	time.month = src[1];
	time.year = src[0];

	return time;
}

int CRtc::RtcSetTime(DateTime pDateTime)
{
	char strTime[16] = {0};
	sprintf(strTime, "%02d%02d%02d%02d%02d%02d", pDateTime.year, pDateTime.month, pDateTime.day,
		pDateTime.hour, pDateTime.minute, pDateTime.second);

	SetTimeHW(strTime);
		
	return 0;
}

unsigned int CRtc::GetTime(char* p)
{
	ATRtcRead(p);

	return 0;
}

unsigned int CRtc::GetTimeHW(char* p)
{
	char src[7] = {0};
	ATRtcRead(src);

	sprintf(p, "%04d-%02d-%02d %02d:%02d:%02d", src[0]+2000, src[1], src[2], src[3], src[4], src[5]);

	return 0;
}

unsigned int CRtc::SetTimeHW(char* p)
{
	// p 141229101234
	int len = strlen(p);
	if(len != 12)
	{
		printf("time format is error!\n");
	}
	else
	{
		char des[7]={0};
		StrToHex(p, (unsigned char *)des);
		// 设置RTC 时间des 14 12 29 10 12 23
		ATRtcWrite(des);

		// 设置系统时间
		char temp[32] = {0};
		sprintf(temp,"date -s \"%s%02x-%02x-%02x %02x:%02x:%02x\"", "20", des[0], des[1], des[2], des[3], des[4], des[5]);
		printf("\ntime:%s\n", temp);
		system(temp);
	}
	
	return 0;
}

