///*****************************************************************************
/*
|*  Copyright       :   (c) 2011 CAS
|*  Filename        :   driver_beep.c
|*  Version         :   0.1
|*  Programmer(s)   :   Tang Qian Kun(TQK)
|*  Created         :   2011/4/6
|*  Modified        :
|*  Description     :   CT100 beep function
*/
//****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include <pthread.h>
#include "driver_pmu.h"


#define PMU_IOC_MAGIC	'z'
#define u32 unsigned int

struct kernel_ver {	
	char str[60];
}__attribute__((packed));

#define PMU_IO_READ_ACDC 	_IOR(PMU_IOC_MAGIC, 1, u32)
#define PMU_IO_READ_VOL 	_IOR(PMU_IOC_MAGIC, 2, u32)
#define PMU_IO_POWERKEY	 	_IOR(PMU_IOC_MAGIC, 3, u32)
#define PMU_IO_POWER_ON	 	_IOR(PMU_IOC_MAGIC, 4, u32)
#define PMU_IO_POWER_OFF 	_IOR(PMU_IOC_MAGIC, 5, u32)
#define PMU_IO_CASH_OFF 	_IOR(PMU_IOC_MAGIC, 6, u32)
#define PMU_IO_CASH_ON	 	_IOR(PMU_IOC_MAGIC, 7, u32)
#define PMU_IO_POWERKEYON 	_IOR(PMU_IOC_MAGIC, 8, u32)
#define PMU_IO_SYS_REBOOT 	_IOR(PMU_IOC_MAGIC, 9, u32)
#define PMU_IO_SYS_HALT 	_IOR(PMU_IOC_MAGIC, 10, u32)
#define PMU_IO_VER 			_IOR(PMU_IOC_MAGIC, 11, struct driver_ver)
#define KERNEL_VER 			_IOR(PMU_IOC_MAGIC, 12, struct kernel_ver)

#define PMU_IO_PWDEVICE_ON	_IOR(PMU_IOC_MAGIC, 13, u32)
#define PMU_IO_PWDEVICE_OFF _IOR(PMU_IOC_MAGIC, 14, u32)

#define PMU_ZIGBEE_HIGH 	_IOR(PMU_IOC_MAGIC, 15, u32)
#define PMU_ZIGBEE_LOW	 	_IOR(PMU_IOC_MAGIC, 16, u32)

/*
#define PMU_IOC_MAGIC	'z'

#define PMU_IO_READ_ACDC 	_IOR(PMU_IOC_MAGIC, 1, unsigned int)
#define PMU_IO_READ_VOL 	_IOR(PMU_IOC_MAGIC, 2, unsigned int)
#define PMU_IO_POWERKEY	_IOR(PMU_IOC_MAGIC, 3, unsigned int)
#define PMU_IO_POWER_ON	_IOR(PMU_IOC_MAGIC, 4, unsigned int)
#define PMU_IO_POWER_OFF 	_IOR(PMU_IOC_MAGIC, 5, unsigned int)

#define PMU_IO_CASH_OFF 	_IOR(PMU_IOC_MAGIC, 6, unsigned int)
#define PMU_IO_CASH_ON	 	_IOR(PMU_IOC_MAGIC, 7, unsigned int)

#define PMU_IO_SYS_REBOOT	_IOR(PMU_IOC_MAGIC, 9, unsigned int)
#define PMU_IO_SYS_HALT 	_IOR(PMU_IOC_MAGIC, 10, unsigned int)
*/


//单键响应关机和重启，稳定性不太好。该方法暂时保留
#ifdef WIN32
DWORD WINAPI theadPmuPower(void* p)
#else
void* theadPmuPower(void* p)
#endif
{		
	int times1,times2,total;
	CPmu* pPMU = (CPmu*)p;

/*
	while(1){
		 usleep(1000*300);//3//1秒大概400次，如果次数超过		
		 times1=0;
		 ioctl(pPMU->fd, PMU_IO_POWERKEY, &times1);
		 if(times1 >0){		
		 	times2=0;
			 sleep(5);//3//1秒大概400次，如果次数超过		
			 ioctl(pPMU->fd, PMU_IO_POWERKEY, &times2);
			 printf("------------[power:%d]\n",times2);
			
		 	if(times2>(4*400)){				
				printf("------------------------------------------------------- halt\n");
		 		 ioctl(pPMU->fd, PMU_IO_POWER_OFF, NULL);
				system("halt");
		 	}else{
		 		printf("------------------------------------------------------- reboot\n");
		 		 ioctl(pPMU->fd, PMU_IO_POWER_ON, NULL);
				 system("reboot");
		      }			
	       }
     }

	*/
/*
	while(1)
	{
		usleep(1000*300);//3//1秒大概400次，如果次数超过	

		switch(pPMU->m_flag)
		{
		case  FLAG_REBOOT:
			ioctl(pPMU->fd, PMU_IO_POWER_ON, NULL);
			system("reboot");
			break;

		case FLAG_HALT:
			ioctl(pPMU->fd, PMU_IO_POWER_OFF, NULL);
			system("halt");
			break;

		case FLAG_RUN:
			break;
		}
	}
*/
}

CPmu::CPmu()
{
	m_flag = FLAG_RUN;
}


CPmu::~CPmu()
{

}

int CPmu::init()
{
	fd = open("/dev/ct100gc_pmu", O_RDONLY);
	if (fd < 0) {
		perror("can not open device ct100gc_ad!");
		exit(1);
	}else{
#if 0		
	   pthread_t idPmu;
	   pthread_create(&idPmu,	NULL,  theadPmuPower,	this);
#endif	   
	}

	return 0;
}

int CPmu::GetBatteryVol()
{
	int BatteryVol=0;

	 ioctl(fd, PMU_IO_READ_VOL, &BatteryVol);

	return BatteryVol;
}

/*
  返回0:  	AC
  返回1:	DC
 */
int CPmu::GetAcDc()
{
	int isAc;

	ioctl(fd, PMU_IO_READ_ACDC, &isAc);

	return isAc;
}

/*
  返回 0: 	没有按下关机键
  返回!0:	按下关机键
 */
int CPmu::GetPoweKey()
{
	int times1=0;
	ioctl(fd, PMU_IO_POWERKEY, &times1);
	return times1;
}

int CPmu::OpenCashBox(void)
{
	ioctl(fd, PMU_IO_CASH_ON, NULL);

	return 0;
}

int CPmu::CloseCashBox(void)
{
	ioctl(fd, PMU_IO_CASH_OFF, NULL);

	return 0;
}

void CPmu::SystemReboot()
{
	ioctl(fd, PMU_IO_SYS_REBOOT, 0);
	
	//m_flag = FLAG_REBOOT;
}

void CPmu::SystemHalt()
{
	//m_flag = FLAG_HALT;
	ioctl(fd, PMU_IO_SYS_HALT, 0);
}

