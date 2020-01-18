
// Copyright (C) 2014 CAS
// All rights reserved.
//
//-------------------------------------------------------------------------
// This file is part of the CT100GC software.  Please visit our Web site:
//
//                       http://www.cas-china.com/
//
// This program is proprietary software; you can redistribute it and/or modify
// it under the terms of the CAS development.
// Any company and individual mustn't extract & copy the content of the sourcecode
// or all of the sourcecode without our company's written permission and mustn't
// spread in any way.

/*
  *  Copyright 		:   (c) 2014 CAS
  *  Filename		:   ad.h
  *  Version		:   1.0
  *  Programmer(s)	:   Li Baoli (LBL)
  *  E-Mail			:   libaoli1978@126.com
  *  Created		:   2014/10/13
  *  Modified		:
  *  Description	:   GUI dIsplay control function interface for POS/standard Market software
  */


#ifndef _KEY_H_
#define _KEY_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <error.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <linux/ioctl.h>

#include <pthread.h>
#define DEUBG_KEY	0
#define DEVICE_DEIVERTEST	"/dev/ct100gc_key"

#define KEY_IOC_MAGIC		'k'
#define KEY_IO_CAL_KEY  	_IOR(KEY_IOC_MAGIC, 2, unsigned char)

#include "../public/typeKey.h"

class Key{
private:
	int keyfd;
	unsigned int m_key_Value;
	void close_Key();		

protected:
	int m_nKeyType;

public:
	Key();
	virtual ~Key();
	unsigned char ParseTransmit(int a, int b);
	unsigned char ParseMidTransmit(int a, int b);

	unsigned char ParseMidTransmitBack(int a, int b);
	
	int _Key_getKey(unsigned char * key);
	int getKey(unsigned char * key);
	int _Key_init(int KeyType);
	char _Key_get_acdc(void){return 0;};
	char _Key_get_powerkey(void){return 0;};
	void _Key_switch_sal(char onoff){};
	int get_cal_key(void);
        bool getStates(void);
};

#endif
