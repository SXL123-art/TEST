#include "driver_i2c.h"

static int nI2C_fd;

static int __i2c_set(int fd, unsigned int timeout, unsigned int retry)
{
	if (fd == 0 ){
		return -1;
	}

	ioctl(fd, I2C_TIMEOUT, timeout ? timeout : I2C_DEFAULT_TIMEOUT);

	ioctl(fd, I2C_RETRIES, retry ? retry : I2C_DEFAULT_RETRY);

	return 0;
}
static int __i2c_send(int fd, struct i2c_rdwr_ioctl_data *data)
{
	if (fd < 0){
		return -1;
	}

	if (data == NULL){
		return -1;
	}

	if (data->msgs == NULL || data->nmsgs == 0){
		return -1;
	}

	return ioctl(fd, I2C_RDWR, (unsigned long)data) ;
}

int i2c_open(unsigned char* dev, unsigned int timeout, unsigned int retry)
{
	if ((nI2C_fd = open((const char*)dev, O_RDWR)) < 0){
		return nI2C_fd;
	}

	__i2c_set(nI2C_fd, timeout, retry);

	return nI2C_fd;
}
void i2c_close(int fd)
{
	if (fd < 0){
		return;
	}

	close(fd);
}
int i2c_read_data(unsigned short addr, unsigned char offset, unsigned char *val)
{
	int i,ret = 0;

	struct i2c_rdwr_ioctl_data data;

	struct i2c_msg msgs[2];
	unsigned char buf1,buf2;
	data.nmsgs = 2;
	msgs[0].buf = &buf1;
	msgs[1].buf = &buf2;
	data.msgs = msgs;

	data.msgs[0].addr = addr;
	data.msgs[0].flags = 0;
	data.msgs[0].len = 1;
	data.msgs[0].buf[0] = offset;

	data.msgs[1].addr = addr;
	data.msgs[1].flags = I2C_M_RD;
	data.msgs[1].len = 1;			//original data is 1
	data.msgs[1].buf[0] = 0;

	//printf("\naddr:%d,offset:%d\n", addr,offset);

	if ((ret = __i2c_send(nI2C_fd, &data)) < 0){		
		printf("read failure!\n");
	}
	
	//printf("data.msgs[1].len = %d", data.msgs[1].len);

	for(i = 0 ;i < data.msgs[1].len; i++){				
		val[i] = data.msgs[1].buf[i];
		//printf("\nval:%d\n", data.msgs[1].buf[i]);
	}

	return ret;
}

int i2c_write_data(unsigned short addr, unsigned char offset, unsigned char val)
{
	int ret = 0;

	struct i2c_rdwr_ioctl_data data;

	struct i2c_msg msgs[2];
	unsigned char buf1[2];
	unsigned char buf2[2];
	data.nmsgs = 1;
	msgs[0].buf = buf1;
	msgs[1].buf = buf2;
	data.msgs = msgs;

	data.msgs[0].addr = addr;
	data.msgs[0].flags = 0;
	data.msgs[0].len = 2;
	data.msgs[0].buf[0] = offset;
	data.msgs[0].buf[1] = val;

//	printf("\naddr:%d,offset:%d,value:%d\n", addr,offset,val);

	if ((ret = __i2c_send(nI2C_fd, &data)) < 0){
		printf("wriete failure!\n");		
	}
	return ret;
}

