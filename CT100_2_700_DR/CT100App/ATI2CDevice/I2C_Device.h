
#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "i2c.h"


class CI2CDevice
{
public:
	CI2CDevice(void);

	~CI2CDevice(void);

	bool parse(void* p, int type, int eepromsize=1);

	void init();
	
	void release();

private:

};
