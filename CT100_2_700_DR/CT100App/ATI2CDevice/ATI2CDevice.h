#ifndef ATI2CDEVICE_H
#define ATI2CDEVICE_H
#include "../ATLib/SysApi.h"

int ATIICInit();
int ATIICRelease();

int ATRtcRead(char* pDes);
int ATRtcWrite(char* pDes);


#endif
