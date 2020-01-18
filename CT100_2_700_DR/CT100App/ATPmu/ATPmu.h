

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
  *  Filename		:   driver_ads1232.c
  *  Version		:   1.0
  *  Programmer(s)	:   Li Baoli (LBL)
  *  E-Mail			:   libaoli1978@126.com
  *  Created		:   2014/10/13
  *  Modified		:
  *  Description	:   CT100GC  function interface of power manage unit
  */



#ifndef _AT_PMU_H
#define _AT_PMU_H

#include "../ATPmu/ATPmu.h"
#include "driver_pmu.h"

int ATPmu_Init();

int ATPmu_GetBatteryVol();

int ATPmu_GetAcDc();

int ATPmu_GetPoweKey();

int ATPmu_CtrlCashBox(void);

void ATSystemReboot();

void ATSystemHalt();

#endif

