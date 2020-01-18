#ifndef _AT_RTC_H
#define _AT_RTC_H

#include "rtc.h"


void ATGetLocalTime(DateTime *pDateTime);
void ATSetLocalTime(DateTime *pDateTime);

void ATRtc_GetDateTimeHW(char* p);
void ATRtc_SetDateTimeHW(char* p);

void ATRtc_GetDateTime(char* p);


#endif

