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
  *  Filename		:   Adc.h
  *  Version		:   1.0
  *  Programmer(s)	:   Li Baoli (LBL)
  *  Created		:   2014/10/13
  *  E-Mail			:   libaoli1978@126.com
  *  Modified		:
  *  Description	:   ad devcie control function interface for POS/standard Market software
  */


/*
标定称重类
方法概述：
1.构造方法CAdc(),做一些初始化工作
2.int getkg(int),o,代表获取重量，1置零，2去皮。调用完方法后，通过对象的kg属性获取重量值。
3.int getInternalCode(),获取内码值，返回值就是内码，用于标定
4.load_param(),初始化CAdc的参数，以通过计算，得道真正的重量，目前返回的重量以克为单位
5.save_param(),返回已经被修改过得参数结构体。
调用方法：
CAdc adc;
1.正常称重，adc.getkg(0);cout<<adc.kg;
2.置零，adc.getkg(1);
3.去皮，adc.getkg(2);
4.标定时，空载时循环调用getInternalCode(),稳定后保存，载重时再调用，稳定后保存。

*/
#ifndef _AD_H
#define _AD_H
#include "driver_ads1232.h"
#include <iostream>
//#include "../public/param.h"
#include "../public/typeAd.h"

//import gate
#define AD_GATE_TARA_ADD		1	//连续去皮
#define AD_GATE_TARA_NEGATIVE		1	//负重量去皮
#define AD_GATE_ZERO_HAVETATA		0	//皮重有值，重量不为0，置零操作

class CAdc
{
    public:
        CAdc();
        virtual ~CAdc();
        int getkg(byte key);
        int getInternalCode();
        adc_param adc_set_default();
        void load_param(adc_param  adcparam);
        adc_param save_param();
        long roundl(long x, long y);
        byte stabok ;
        byte zerook ;
        byte taraok ;
        byte overok ;
        int point_position;//23.555 point=3,kg=23555, db
        long kg,kg1,kgzero,kgtara;
        long MAXKG,MAX_KG1,MAX_E,MAX_E1,MAX_CAL,MAX_DIV,MAX_KG,MAX_TARA,MAX_ZERO;
        long MAX_AUTOZERO,MAX_AUTOZERODIS,MAX_ON,MINKG;
    protected:
    private:
        long adc_read_weight(long internal_weight);
        int gl; // local g value :db
        int gd; // destination g value :db
        int unload; //unload internal adc code :db
        int load;   // load internal adc code :db
        int full_arrange; //full arragne,3kg,6kg,15kg,30kg,60kg,150kg,300kg,600kg,1000kg :db
        int cal_arrange;
        int is_dual; //sigial or dual arrange :db
        int e; //db
        int e1;

        float cala, calb,x;

};

#endif // _ADC_H
