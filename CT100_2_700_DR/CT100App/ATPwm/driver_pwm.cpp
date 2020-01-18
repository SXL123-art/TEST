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

#include "driver_pwm.h"


CPwm::CPwm()
{

}

CPwm::~CPwm()
{

}

int CPwm::init()
{
    fd = open("/dev/ct100gc_pwm", O_RDWR);
    if (fd < 0) {
        perror("can not open device ct100gc_pwm!\n");
        exit(1);
    }
    printf("---------------------------------------pwm\n");
    return 0;
}

int CPwm::SetBacklight(int value)
{
    //value 0-9,
    value = 25*value;
    //printf("back light value:%d\n", value);
    write(fd, (char*)&value, sizeof(unsigned int));
    return 0;
}

