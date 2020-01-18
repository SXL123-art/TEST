#ifndef _DRIVER_PMU_H
#define _DRIVER_PMU_H

#include <unistd.h>

#define FLAG_REBOOT		0
#define FLAG_HALT		1
#define FLAG_RUN		2
class CPmu
{
	
public:
	CPmu();
	~CPmu();

	int init();
	
	int GetBatteryVol();
	int GetAcDc();
	
	int GetPoweKey();

	int OpenCashBox(void);
	int CloseCashBox(void);
	
	void SystemReboot();
	void SystemHalt();

public:
	int	fd;
	unsigned char 	m_flag;
};



#endif