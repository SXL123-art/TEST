#ifndef _RTC_H_
#define _RTC_H_
#include <string>

// ʱ�����ڸ�ʽ����
typedef struct
{
        unsigned long year		:6;	// 0-63		since 2000
        unsigned long month		:4;	// 1-12
        unsigned long day		:5;	// 1-31
        unsigned long hour		:5;	// 0-23
        unsigned long minute	:6;	// 0-59
        unsigned long second	:6;	// 0-59
} DateTime;

//Ӳ��ʱ��
class CRtc
{
public:
    CRtc(){};
    ~CRtc(){};

    DateTime RtcGetTime();
    int RtcSetTime(DateTime pDateTime);

    unsigned int GetTimeHW(char* p);			// ��ȡʱ�䣬�̶���ʽ:2014-12-29 14:30:45
    unsigned int SetTimeHW(char* p);

    unsigned int GetTime(char* p);				// ��ȡʱ�䣬�޸�ʽ
};

#endif
