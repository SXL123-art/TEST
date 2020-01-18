#pragma once

#include "driver_i2c.h"

#define	IIC_OPEN				0
#define	IIC_CLOSE				1


#define 	IICADDR_EEPROM			"0x50"	//0x50
#define 	IICADDROFFSET_EEPROM		"0x00"	//0000 0000

#define 	IICADDR_RTC				"0x51"	//
#define 	IICADDROFFSET_RTC			"0x00"	//0000 0000

// RTC各个时间数据寄存器地址
#define  	Addr_second  				"0x02"	//秒
#define  	Addr_mint    				"0x03"	//分
#define  	Addr_hour    				"0x04"	//小时
#define  	Addr_day     					"0x05"	//日
#define  	Addr_xingqi    				"0x06"	//星期
#define  	Addr_month   				"0x07"	//月
#define  	Addr_year    				"0x08"	//年

#define 	IICADDR_BEEP				"0x21"	//0010 0001
#define 	IICADDROFFSET_BEEP		"0x00"	//0000 0000

#define	IIC_EEPROM_WRITE			2
#define	IIC_EEPROM_READ			3

#define	IIC_RTC_WRITE				4
#define	IIC_RTC_READ				5

#define	IIC_BEEP_WRITE				6
#define	IIC_EXTERNIO_READ			7

int I2C_Process(int argc, char* _addr, char* _offset, char* pData);


