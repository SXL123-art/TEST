

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
  *  Filename		:   ATAdc.h
  *  Version		:   1.0
  *  Programmer(s)	:   Li Baoli (LBL)
  *  Created		:   2014/10/13
  *  E-Mail			:   libaoli1978@126.com
  *  Modified		:
  *  Description	:   ad devcie control function interface for POS/standard Market software
  */

#ifndef _ATAD_H
#define _ATAD_H
#include "Ad.h"
#include "../public/typeAd.h"
#include "../public/globals.h"
#include "../ATSqlite/ATSQLite.h"
#include <unistd.h>

extern adc_param	m_param;

int ATAd_initAdc_param();
int ATAd_initAdc(adc_param  ap);
int ATAd_getKg(adc_ret * ret);
adc_param ATAd_getParam();
int ATAd_setZero();
int ATAd_setTara();
unsigned long ATAd_getInternalCode();

#endif

