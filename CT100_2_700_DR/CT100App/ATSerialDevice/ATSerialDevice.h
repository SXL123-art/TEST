
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
  *  E-Mail		:   libaoli1978@126.com
  *  Created		:   2014/10/13
  *  Modified		:
  *  Description	:   CT100GC  function interface of all serial device manager
  */

#include "../public/typeCard.h"


// Zigbee接口
int ATSerialDevice_Save_Zigbee_Config(int nTypeNet, int nChannel);
int ATSerialDevice_Read_Zigbee_Config();
int ATSerialDevice_Zigbee_Send(char* pSendData, int Len);
int ATSerialDevice_Zigbee_Recv(char* pRecvData, int Len);

// 串口扫描枪接口
int ATSerialDevice_Scanner_Recv(char *recvData, int nrecvLen);

// 溯源卡M1卡接口
int ATSerialDevice_Rfid_Find(void);
int ATSerialDevice_Rfid_ReadCompositeBlock_M1(unsigned char sectionNum, unsigned char blockNum, char *passwd, char *readData);
int ATSerialDevice_Rfid_Read_M1(unsigned char beginBlockNum, unsigned char endBlockNum, char *passwd, char *readData);
int ATSerialDevice_Rfid_Write_M1(unsigned char beginBlockNum, unsigned char endBlockNum, char *passwd, char *writeData, unsigned char len);

// 溯源卡CPU卡接口
int ATSerialDevice_Rfid_Check_CPU(unsigned char *passwd, unsigned char passwdLen);
int ATSerialDevice_Rfid_Open_CPU(unsigned char nFolderNum, unsigned char *openFolderPefix, unsigned char folderPefixLen);
int ATSerialDevice_Rfid_Read_CPU(unsigned char startadress ,unsigned char nFolderNum, char *readData,int readDataSize);
int ATSerialDevice_Rfid_Write_CPU(unsigned char startadress ,unsigned char nFolderNum, char *writeData, unsigned char datasize);

// 蓝牙接口
int ATSerialDevice_Bluetooth_Send_Recv(char *sendData, int nsendLen, char *recvData, int nrecvLen);

