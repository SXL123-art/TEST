#include "ATRtc.h"


CRtc gs_rtc;

void ATGetLocalTime(DateTime *pDateTime)
{
	*pDateTime = gs_rtc.RtcGetTime();
}

void SetLocalTime(DateTime *pDateTime)
{
	gs_rtc.RtcSetTime(*pDateTime);
}

// 2016-04-16 12:30:50
void ATRtc_GetDateTimeHW(char* p)
{
	gs_rtc.GetTimeHW(p);
}

// 141229101234
void ATRtc_SetDateTimeHW(char* p)
{
	gs_rtc.SetTimeHW(p);
}

void ATRtc_GetDateTime(char* p)
{
	gs_rtc.GetTime(p);
}
