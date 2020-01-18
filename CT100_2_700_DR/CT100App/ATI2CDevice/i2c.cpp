#include "i2c.h"

#include <stdio.h>

#include "../ATLIB/SysApi.h"

static int fd;
#define TIMEOUT	3
#define RETRY	3

int help_info(void)
{
	printf("\nUsage: i2c  [-d PATH] ADDR OFFSET\n");
	printf("\nOr:    i2c  [-d PATH] -s ADDR OFFSET DATA \n");
	printf("\nRead or Write  the register of i2c slave\n");
	printf("\nFor example\n");
	printf("\ti2c 0x51 0x05\t\t\t\t\t\tRead the register: 0x05 of the address: 0x51\n");
	printf("\ti2c -d /dev/i2c-10 0x51 0x05\t\t\t\tRead the register: 0x05 of the address: 0x51\n");
	printf("\ti2c -d /dev/i2c-1 -s 0x51 0x05 18\t\t\tWrite 18 to the register: 0x05 of the address: 0x51\n\n");

	return 0;
}

void i2c_path(unsigned char* argv)
{
	fd = i2c_open(argv,TIMEOUT,RETRY);
	if( fd < 0 ){	
		printf("i2c_open error!\n");
		exit(-1);
	}else{
		printf("i2c_open ok!\n");
	}
}

int read_data(unsigned short addr, unsigned char offset, unsigned char *val)
{
	int ret;

	ret = i2c_read_data(addr,offset,val);
	if(ret < 0){
		printf("%s error!\n",__FUNCTION__);
		exit(-1);
	}
	//printf("read success, val  = %02x\n",*val);
	return 0;
}

int write_data(unsigned short addr, unsigned char offset, char* argv)
{
	int ret;
	//strtoul 将字符串转换成无符号长整型数
	//unsigned char val = (unsigned char)strtoul(argv,0,16);
	unsigned char val = *((unsigned char*)argv);
	//printf("\n real data:%d \n",val);

	ret = i2c_write_data(addr,offset,val);
	if(ret < 0){
		printf("%s error!\n",__FUNCTION__);
		exit(-1);
	}

	//	printf("write success , val  = %02x\n",val);
	usleep(10000);    // 延时程序
	return 0;
}


// 硬件设备地址是1010 000，下面代码写读0X10偏移地址的内容
int I2C_Process(int argc, char* _addr, char* _offset, char* pData)
{
	unsigned char val;
	unsigned short addr;
	unsigned char offset;
	unsigned char temp;

	switch(argc)
	{
	case IIC_OPEN:
		i2c_path((unsigned char*)"/dev/i2c-0");
		printf("/dev/i2c-0 open!\n");
		break;

	case IIC_CLOSE:
		close(fd);
		printf("/dev/i2c-0 close!\n");
		break;
		
	case IIC_EEPROM_WRITE:
	case IIC_BEEP_WRITE:		
		addr = HexToBcd(atoi(_addr+2));
		offset = HexToBcd(atoi(_offset+2));
		write_data(addr,offset,pData);
	//	printf("_addr:%s,_offset:%s,pData:%s\n", _addr, _offset, pData);
		break;
		
	case IIC_EEPROM_READ:
	//	printf("_addr:%s,_offset:%s,pData:%s\n", _addr, _offset, pData);
		addr = HexToBcd(atoi(_addr+2));
		offset = HexToBcd(atoi(_offset+2));
		read_data(addr, offset,(unsigned char*)pData);
		val = atoi(pData);

	case IIC_RTC_WRITE:	
		addr = HexToBcd(atoi(_addr+2));
		offset = HexToBcd(atoi(_offset+2));
		write_data(addr,offset,pData);
		//printf("_addr:%s,_offset:%s,pData:%s\n", _addr, _offset, pData);
		break;
		
	case IIC_RTC_READ:	
		//printf("_addr:%s,_offset:%s,pData:%s\n", _addr, _offset, pData);
		addr = HexToBcd(atoi(_addr+2));
		offset = HexToBcd(atoi(_offset+2));
		read_data(addr, offset,&temp);
		*pData = (temp);
		break;
		
	default:
		break;
	}

	return val;
}


