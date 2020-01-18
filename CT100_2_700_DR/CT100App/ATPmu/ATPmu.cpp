#include "ATPmu.h"


CPmu pmu;

int ATPmu_Init()
{
	return pmu.init();
}

int ATPmu_GetBatteryVol()
{
	return pmu.GetBatteryVol();
}

int ATPmu_GetAcDc()
{
	return pmu.GetAcDc();
}

int ATPmu_GetPoweKey()
{
	return pmu.GetPoweKey();
}

//Ǯ��򿪺�������̹رգ��������һֱ����
int ATPmu_CtrlCashBox(void)
{
	pmu.OpenCashBox();	

	usleep(20*1000);

	pmu.CloseCashBox();
	
	return 0;
}

void ATSystemReboot()
{
	return pmu.SystemReboot();
}

void ATSystemHalt()
{
	return pmu.SystemHalt();
}


