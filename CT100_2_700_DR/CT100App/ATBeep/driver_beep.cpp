///*****************************************************************************
/*
|*  Copyright       :   (c) 2011 CAS
|*  Filename        :   driver_beep.c
|*  Version         :   0.1
|*  Programmer(s)   :   Tang Qian Kun(TQK)
|*  Created         :   2011/4/6
|*  Modified        :
|*  Description     :   CT200 beep function
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

 

#define DEVICE_BUZZER	"/dev/ct100gc_buzzer"

static int fd_beep = 0;

void beep_off(void)
{
	int ret = ioctl(fd_beep, 0, 0);
	if(ret < 0)
	{
		 fprintf( stderr, "%s\n", strerror( errno ));
	}
}

void beep_on(void)
{
	int ret = ioctl(fd_beep, 1, 0);
	if(ret < 0)
	{
		 fprintf( stderr, "%s\n", strerror( errno ));
	}
}

void beep_init(void)
{
	fd_beep = open(DEVICE_BUZZER, O_RDWR);
	if(fd_beep <0)
	{
		perror("can not open buzzer");
		exit(1);
	}
	else
	{
		printf("open buzzer ok!\n");
	}
}

