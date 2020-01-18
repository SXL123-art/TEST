// loaddll.cpp : Defines the entry point for the DLL application.
//

#include "ATSerialDevice.h"
#include "MyZigbee.h"
#include "MyZM703.h"
#include "MyScanner.h"
#include "MyBluetooth.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>



// Zigbee 接口
int ATSerialDevice_Read_Zigbee_Config()
{
	BeginZigbeeConfig();

	ReadZigbeeConfig();

	ResetZigbee();

	return 0;
}
int ATSerialDevice_Save_Zigbee_Config(int nTypeNet, int nChannel)
{
	BeginZigbeeConfig();

	SaveZigbeeConfig(nTypeNet, nChannel);

	ResetZigbee();

	return 0;
}
int ATSerialDevice_Zigbee_Send_Recv(char *sendData, int nsendLen, char *recvData, int nrecvLen)
{
	return SendAndRecvZigbeeData(sendData, nsendLen, recvData, nrecvLen);
}

// 串口扫描枪接口
int ATSerialDevice_Scanner_Recv(char *recvData, int nrecvLen)
{
	return RecvScannerData(recvData, nrecvLen);
}

int ATSerialDevice_Rfid_Find(void)
{
	if (Rfid_StartCard() > 0)	
	{		
		printf("寻卡成功\n");
		return 1;
	}
	return 0;
}


// M1卡接口S70卡和S50卡endBlockNum不一样，上层调用时需要考虑endBlockNum不要超过最大块
int ATSerialDevice_Rfid_ReadCompositeBlock_M1(unsigned char sectionNum, unsigned char blockNum, char *passwd, char *readData)
{
	if (Rfid_StartCard() > 0)	
	{		
		printf("寻卡成功\n");
		int nLen = Rfid_ReadCompositeBlock_M1(sectionNum, blockNum, passwd, readData);
		if (nLen > 0)
		{
			return nLen;
		}
	}
	return 0;
}
// M1卡接口S70卡和S50卡endBlockNum不一样，上层调用时需要考虑endBlockNum不要超过最大块
int ATSerialDevice_Rfid_Read_M1(unsigned char beginBlockNum, unsigned char endBlockNum, char *passwd, char *readData)
{
	if (Rfid_StartCard() > 0)	
	{		
		printf("寻卡成功\n");
		int nLen = Rfid_ReadSection_M1(beginBlockNum, endBlockNum, passwd, readData);
		if (nLen > 0)
		{
			return nLen;
		}
	}
	return 0;
}
// M1卡接口S70卡和S50卡endBlockNum不一样，上层调用时需要考虑endBlockNum不要超过最大块，当数据量过大，可以分包写卡
int ATSerialDevice_Rfid_Write_M1(unsigned char beginBlockNum, unsigned char endBlockNum, char *passwd, char *writeData, unsigned char len)
{
	if (Rfid_StartCard() > 0)	
	{		
		printf("寻卡成功\n");
	
		if (Rfid_WriteSection_M1(beginBlockNum, endBlockNum, passwd, writeData, len) > 0)
		{
			return 1;
		}
	}	
	return 0;
}

// CPU卡接口:寻卡->转入->验证->打开->读写
int ATSerialDevice_Rfid_Check_CPU(unsigned char *passwd, unsigned char passwdLen)
{
	//if (Rfid_StartCard() > 0)	
	{		
		//printf("寻卡成功\n");
		// 转入
		if (Rfid_Enter_CPU() == 0)
			return -1;	
		
		// 验证
		if (Rfid_KeyAuthentication_CPU(passwdLen, passwd) > 0)
			return 1;
	}
	return 0;
}
int ATSerialDevice_Rfid_Open_CPU(unsigned char nFolderNum, unsigned char *openFolderPefix, unsigned char folderPefixLen)
{
	unsigned char cosCmd[256] = {0};
	for (int i=0; i<folderPefixLen; i++)
		cosCmd[i] = openFolderPefix[i];
	cosCmd[folderPefixLen] = nFolderNum;
	// 打开文件
	char recvData[256] = {0};
	if (Rfid_Cos_CPU(folderPefixLen+1, cosCmd, recvData) > 0)
	{
		return 1;
	}

	return 0;
}
int ATSerialDevice_Rfid_Read_CPU(unsigned char startadress ,unsigned char nFolderNum, char *readData,int readDataSize)
{
	unsigned char cosCmd[8] = {0};
	
	if(nFolderNum == 0x17)
	{
		cosCmd[0] = 0x00;				// CLA:00，04; 0:响应报文不含MAC
		cosCmd[1] = 0xB2;				// READ RECORD
		cosCmd[2] = startadress;
		cosCmd[3] = 0x04;
	}
	else
	{
		cosCmd[0] = 0x00;
		cosCmd[1] = 0xB0;				// READ BINARY
		cosCmd[2] = startadress/256;
		cosCmd[3] = startadress%256;
	}
	cosCmd[4] = readDataSize;
	// 读文件
	if (Rfid_Cos_CPU(5, cosCmd, readData) > 0)
	{
		return 1;
	}
	return 0;
}
int ATSerialDevice_Rfid_Write_CPU(unsigned char startadress ,unsigned char nFolderNum, char *writeData, unsigned char datasize)
{
	int nLen = 4+1+datasize;
	unsigned char *cosCmd = new unsigned char[nLen+1];
	char readData[256] = {0};

	if(nFolderNum == 0x17)
	{
		cosCmd[0] = 0x00;
		cosCmd[1] = 0xDC;						// UPDATE RECORD
		cosCmd[2] = startadress;
		cosCmd[3] = 0x04;
	}
	else
	{
		cosCmd[0] = 0x00;
		cosCmd[1] = 0xD6;						// UPDATE BINARY
		cosCmd[2] = startadress/256;
		cosCmd[3] = startadress%256;
	}
	cosCmd[4] = datasize;		
	memcpy(cosCmd+5, writeData, datasize);
	// 写文件
	if (Rfid_Cos_CPU(nLen, cosCmd, readData) > 0)
	{
		cosCmd[0] = 0x00;	
		cosCmd[1] = 0xE2;						// APPEND RECORD
		cosCmd[2] = 0x00;	
		cosCmd[3] = 0x00;	
		//cosCmd[4] = datasize;	
		//memcpy(cosCmd+5, writeData, datasize);
		// 更新文件
		if (Rfid_Cos_CPU(nLen, cosCmd, readData) > 0)
		{
			delete [] cosCmd;
			return 1;
		}
	}
	delete [] cosCmd;
	return 0;
}

int ATSerialDevice_Bluetooth_Send_Recv(char *sendData, int nsendLen, char *recvData, int nrecvLen)
{
	return Bluetooth_SendAndRecvData(sendData, nsendLen, recvData, nrecvLen);
}
