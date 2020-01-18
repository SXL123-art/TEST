#include "I2C_Device.h"


CI2CDevice::CI2CDevice(void)
{	

}

CI2CDevice::~CI2CDevice(void)
{
	I2C_Process(IIC_CLOSE,NULL,NULL,NULL);
}

void CI2CDevice::init()
{
	I2C_Process(IIC_OPEN,NULL,NULL,NULL);
}
	
void CI2CDevice::release()
{
	I2C_Process(IIC_CLOSE,NULL,NULL,NULL);
}

unsigned bcd2bin(unsigned char val)
{       
	return (val & 0x0f) + (val >> 4) * 10;  
}

bool CI2CDevice::parse(void* p, int type, int eepromsize)
{
	char temp[3];
	char value;
	char *pDate = (char*)p;
	char eepromoffset[5];
	switch(type)
	{
	//EEPROM
	case IIC_EEPROM_WRITE:	
		for(int m=0;m<eepromsize;m++){			
			sprintf(eepromoffset, "0x%2d",m);
			printf("eeprom addr:%s\n", eepromoffset);
			I2C_Process(IIC_EEPROM_WRITE, IICADDR_EEPROM, eepromoffset, ((char*)p)+m);
			usleep(1000*100);
		}
		break;
		
	case IIC_EEPROM_READ:
		for(int m=0;m<eepromsize;m++){		
			sprintf(eepromoffset, "0x%2d",m);
			printf("eeprom addr:%s\n", eepromoffset);
			I2C_Process(IIC_EEPROM_READ, IICADDR_EEPROM, eepromoffset, (char*)temp);
			*((char*)p+m) = temp[0];		
			usleep(1000*100);
		}
		break;	
		
				
	//LCD
	case IIC_BEEP_WRITE:
		I2C_Process(IIC_BEEP_WRITE, IICADDR_BEEP, IICADDROFFSET_BEEP, (char*)p);
		break;


	//RTC， 20141229101234
	case IIC_RTC_WRITE:		
		//printf("write pcf8563:%s\n",p);

		//i2c_write_byte(PCF8563_SLA,0x00,0x20);			//	PCF8563时钟芯停止运行
		//i2c_write_nbyte(PCF8563_SLA,0x02,ptr,7);		//	将设置时间写入PCF8563
		//i2c_write_byte(PCF8563_SLA,0x00,0x00);			//	PCF8563时钟芯开始运行

		//	PCF8563时钟芯停止运行
		value = 0x20;
		I2C_Process(IIC_RTC_WRITE, IICADDR_RTC, IICADDROFFSET_RTC, &value);
		usleep(1000*100);

		//秒
		value = pDate[5];
		printf("\nsecond:%d",value);
		I2C_Process(IIC_RTC_WRITE, IICADDR_RTC, Addr_second, &value);
		usleep(1000*100);

		//分
		value = pDate[4];
		printf("\nmint:%d",value);
		I2C_Process(IIC_RTC_WRITE, IICADDR_RTC, Addr_mint, &value);
		usleep(1000*100);

		//小时
		value = pDate[3];
		printf("\nhour:%d",value);
		I2C_Process(IIC_RTC_WRITE, IICADDR_RTC, Addr_hour,&value);
		usleep(1000*100);

		//日
		value = pDate[2];
		printf("\nday:%d",value);
		I2C_Process(IIC_RTC_WRITE, IICADDR_RTC, Addr_day, &value);
		usleep(1000*100);

		/*
		//星期
		I2C_Process(IIC_RTC_WRITE, IICADDR_RTC, IICADDROFFSET_RTC+Addr_xingqi, &value);
				usleep(1000*100);
		*/

		//月
		value = pDate[1];
		printf("\nmonth:%d",value);
		I2C_Process(IIC_RTC_WRITE, IICADDR_RTC, Addr_month, &value);
		usleep(1000*100);

		//年
		value = pDate[0];
		printf("\nyear:%d",value);
		I2C_Process(IIC_RTC_WRITE, IICADDR_RTC, Addr_year, &value);
		usleep(1000*100);


		//PCF8563时钟开始运行
		value = 0x00;
		I2C_Process(IIC_RTC_WRITE, IICADDR_RTC, IICADDROFFSET_RTC, &value);
		usleep(1000*100);
		break;
		
	case IIC_RTC_READ:
		I2C_Process(IIC_RTC_READ, IICADDR_RTC, Addr_year, (char*)&temp[0]);
		//年
		*((char*)p+0) = bcd2bin(temp[0]);

		//月
		I2C_Process(IIC_RTC_READ, IICADDR_RTC, Addr_month,(char*) &temp[0]);
		*((char*)p+1)  = bcd2bin(temp[0] & 0x1f);

		//日
		I2C_Process(IIC_RTC_READ, IICADDR_RTC, Addr_day, (char*)&temp[0]);
		*((char*)p+2) = bcd2bin(temp[0] & 0x3f);

		//小时
		I2C_Process(IIC_RTC_READ, IICADDR_RTC, Addr_hour, (char*)&temp[0]);
		*((char*)p+3) = bcd2bin(temp[0] & 0x3f);

		//分
		I2C_Process(IIC_RTC_READ, IICADDR_RTC, Addr_mint, (char*)&temp[0]);
		*((char*)p+4)  = bcd2bin(temp[0] & 0x7f);

		//秒
		I2C_Process(IIC_RTC_READ, IICADDR_RTC, Addr_second, (char*)&temp[0]);
		*((char*)p+5) = bcd2bin(temp[0] & 0x7f);
		
		break;	
	}

	return true;
}

