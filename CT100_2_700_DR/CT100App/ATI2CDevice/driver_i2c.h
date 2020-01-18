#ifndef _DRIVER_I2C_H__
#define _DRIVER_I2C_H__

#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/rtc.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>


#define I2C_DEFAULT_TIMEOUT		1
#define I2C_DEFAULT_RETRY			3


static int __i2c_send(int fd, struct i2c_rdwr_ioctl_data *data);
static int __i2c_set(int fd, unsigned int timeout, unsigned int retry);


int i2c_open(unsigned char* dev, unsigned int timeout, unsigned int retry);
void i2c_close(int fd);

int i2c_read_data(unsigned short  addr, unsigned char offset, unsigned char *val);
int i2c_write_data(unsigned short  addr, unsigned char offset, unsigned char val);

#endif

