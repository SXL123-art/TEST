

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
  *  Description	:   CT100GC  function interface of free publice lib manager
  */



#ifndef _ATLIB__H
#define _ATLIB_H
#include <unistd.h>

void ATLib_Init();


int ATLib_g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen);
int ATLib_u2g(char *inbuf,size_t inlen,char *outbuf,size_t outlen);

void ATLib_GenerateMD5(unsigned char* buffer,size_t bufferlen);
char* ATLib_ToString();



#endif

