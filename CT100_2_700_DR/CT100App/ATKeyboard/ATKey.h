

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
  *  Description	:   CT100GC  function interface of key driver manager
  */



#ifndef _AT_KEY_H
#define _AT_KEY_H


#include "imx28_key.h" 


unsigned char ATKey_getKey(unsigned char * key);
int ATKey_init(int KeyType);
bool ATKey_getStates(void);

#endif
//AT_KEY_H
