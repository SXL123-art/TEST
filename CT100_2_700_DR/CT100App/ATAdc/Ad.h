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
�궨������
����������
1.���췽��CAdc(),��һЩ��ʼ������
2.int getkg(int),o,�����ȡ������1���㣬2ȥƤ�������귽����ͨ�������kg���Ի�ȡ����ֵ��
3.int getInternalCode(),��ȡ����ֵ������ֵ�������룬���ڱ궨
4.load_param(),��ʼ��CAdc�Ĳ�������ͨ�����㣬�õ�������������Ŀǰ���ص������Կ�Ϊ��λ
5.save_param(),�����Ѿ����޸Ĺ��ò����ṹ�塣
���÷�����
CAdc adc;
1.�������أ�adc.getkg(0);cout<<adc.kg;
2.���㣬adc.getkg(1);
3.ȥƤ��adc.getkg(2);
4.�궨ʱ������ʱѭ������getInternalCode(),�ȶ��󱣴棬����ʱ�ٵ��ã��ȶ��󱣴档

*/
#ifndef _AD_H
#define _AD_H
#include "driver_ads1232.h"
#include <iostream>
//#include "../public/param.h"
#include "../public/typeAd.h"

//import gate
#define AD_GATE_TARA_ADD		1	//����ȥƤ
#define AD_GATE_TARA_NEGATIVE		1	//������ȥƤ
#define AD_GATE_ZERO_HAVETATA		0	//Ƥ����ֵ��������Ϊ0���������

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
