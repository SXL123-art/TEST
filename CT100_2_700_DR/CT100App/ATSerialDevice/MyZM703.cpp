#include "CSerial.h"
#include "MyZM703.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "typeSerial.h"

#include "../ATLib/SysApi.h"


#define	RECV_BUFFER_SIZE	1024
#define	SEND_BUFFER_SIZE	1024


static char g_RecvBuffer[RECV_BUFFER_SIZE];
static char g_SendBuffer[SEND_BUFFER_SIZE];

static int g_nRecvBufferLen;

#define 	SEND_DATA_ADDR	(g_SendBuffer+5)

int Rfid_SendAndRecvData(char *sendBuf , int sendLen)
{
	unsigned char nOffset = 0;
	unsigned char nCheckCode = 0;
	
	g_SendBuffer[nOffset++] = 0x55 ;
	g_SendBuffer[nOffset++] = 0xAA ;
	g_SendBuffer[nOffset++] = 0xFF ;
	
	unsigned char len[2] = {0};
	EndianLocal2Big16((unsigned int)sendLen+1,len);
	g_SendBuffer[nOffset++] = len[0] ;
	g_SendBuffer[nOffset++] = len[1] ;
	
	nOffset += sendLen;
	for(int i=0; i<nOffset; i++)
		nCheckCode ^= g_SendBuffer[i];
	g_SendBuffer[nOffset++] = nCheckCode ;
	
	CSerial slaveSerial;
	int nRet = slaveSerial.Init(SERIAL_RFID, RFID_BAUD, 8, 'N', 1, NoFlowControl);
	if (nRet < 0)
	{
		printf("Rfid_ReadCard slaveSerial open faile\n");
		return -1;
	}
	memset(g_RecvBuffer, 0x00, RECV_BUFFER_SIZE);
	int g_nRecvBufferLen = slaveSerial.SendAndRecvData(g_SendBuffer, nOffset, g_RecvBuffer, 1);
	if(g_nRecvBufferLen > 0)
	{
		printf("g_nRecvBufferLen:%d  data:",g_nRecvBufferLen);
		for(int i = 0; i<g_nRecvBufferLen;i++)
			printf("%02x ",g_RecvBuffer[i]);
		printf("\n");
		return 1;

	}
	else
		return 0;
}

// 55 AA FF 00 04 02 21 52 75
int Rfid_StartCard()
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;

	pSendData[nOffset++] = 0x02 ;
	pSendData[nOffset++] = 0x21 ;
	pSendData[nOffset++] = 0x52 ;

	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(g_RecvBuffer[5] != 0xFF || g_RecvBuffer[12] != 0xFF)//fail
			return -1;
		else
			return 1;
	}	
	
/*	
	int recvTimeout = 0;
	while (1)
	{
		usleep(100*1000);
		if (++recvTimeout >= 10*5)
		{
			recvTimeout = 0;
			usleep(200*1000);
			Rfid_StopCard();
			return -2;
		}
		
		if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
		{
			if(g_RecvBuffer[5] != 0xFF || g_RecvBuffer[12] != 0xFF)//fail
				continue;
			else
				return 1;
		}	
	}
*/
	return 0;
}

int Rfid_StopCard()
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;

	pSendData[nOffset++] = 0x02;
	pSendData[nOffset++] = 0x22;
	
	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff == g_RecvBuffer[5])
			return 1;
		else 
			return -1;
	}
	return 0;
}

int Rfid_KeyAuthentication_M1(unsigned char sectionNum, char *passwd)
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;
	
	pSendData[nOffset++] = 0x03;
	pSendData[nOffset++] = 0x01;
	pSendData[nOffset++] = 0x60;
	pSendData[nOffset++] = sectionNum;
	memcpy(pSendData+nOffset, passwd, 6);
	nOffset += 6;
	
	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff != g_RecvBuffer[5])
		{
			return -1;
		}
		else 
		{
			return 1;
		}
	}
	return 0;
}
int Rfid_ReadBlock_M1(unsigned char blockNum, char *readData)
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;
	unsigned int nLen = 0;
	
	pSendData[nOffset++] = 0x03;
	pSendData[nOffset++] = 0x02;
	pSendData[nOffset++] = blockNum;

	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff != g_RecvBuffer[5])
		{
			return -1;
		}
		else 
		{
			nLen = EndianBig2Local16((unsigned char*)g_RecvBuffer+3)-2;
			memcpy(readData, g_RecvBuffer+6, nLen);
		}	
	}
	return nLen;
}
int Rfid_WriteBlock_M1(unsigned char blockNum, char *writeData, unsigned int len)
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;
	unsigned int nLen = 0;
	
	pSendData[nOffset++] = 0x03;
	pSendData[nOffset++] = 0x03;

	pSendData[nOffset++] = blockNum;
	memcpy(pSendData+nOffset, writeData, len);
	nOffset += len;
	
	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff != g_RecvBuffer[5])
		{
			return -1;
		}
		else 
		{
			return 1;
		}	
	}
	return 0;
}
int Rfid_ReadCompositeBlock_M1(unsigned char sectionNum, unsigned char blockNum, char *passwd, char *readData)
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;
	unsigned int nLen = 0;
	
	pSendData[nOffset++] = 0x03;
	pSendData[nOffset++] = 0x20;
	pSendData[nOffset++] = 0x60;
	pSendData[nOffset++] = sectionNum;
	memcpy(pSendData+nOffset, passwd, 6);
	nOffset += 6;
	pSendData[nOffset++] = 0x00;
	pSendData[nOffset++] = blockNum;
	
	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff != g_RecvBuffer[5])
		{
			return -1;
		}
		else 
		{
			nLen = EndianBig2Local16((unsigned char*)g_RecvBuffer+3)-2;
			memcpy(readData, g_RecvBuffer+6, nLen);
		}	
	}
	return nLen;
}
int Rfid_WriteCompositeBlock_M1(unsigned char sectionNum, unsigned char blockNum, char *passwd, char *writeData, unsigned int len)
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;
	unsigned int nLen = 0;
	
	pSendData[nOffset++] = 0x03;
	pSendData[nOffset++] = 0x20;
	pSendData[nOffset++] = 0x60;
	pSendData[nOffset++] = sectionNum;
	memcpy(pSendData+nOffset, passwd, 6);
	nOffset += 6;
	pSendData[nOffset++] = 0x01;
	pSendData[nOffset++] = blockNum;
	memcpy(pSendData+nOffset, writeData, len);
	nOffset += len;
	
	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff != g_RecvBuffer[5])
		{
			return -1;
		}
		else 
		{
			return 1;
		}	
	}
	return 0;
}
int Rfid_ReadSection_M1(unsigned char beginBlockNum, unsigned char endBlockNum, char *passwd, char *readData)
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;
	unsigned int nLen = 0;
	
	pSendData[nOffset++] = 0x04;
	pSendData[nOffset++] = 0xA0;
	pSendData[nOffset++] = beginBlockNum;
	pSendData[nOffset++] = endBlockNum;
	pSendData[nOffset++] = 0x60;
	memcpy(pSendData+nOffset, passwd, 6);
	nOffset += 6;
	
	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff != g_RecvBuffer[5])
		{
			return -1;
		}
		else 
		{
			nLen = EndianBig2Local16((unsigned char*)g_RecvBuffer+3)-2;
			memcpy(readData, g_RecvBuffer+6, nLen);
		}	
	}
	return nLen;
}
int Rfid_WriteSection_M1(unsigned char beginBlockNum, unsigned char endBlockNum, char *passwd, char *writeData, unsigned int len)
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;
	unsigned int nLen = 0;

	pSendData[nOffset++] = 0x04;
	pSendData[nOffset++] = 0xA1;
	pSendData[nOffset++] = beginBlockNum;
	pSendData[nOffset++] = endBlockNum;
	pSendData[nOffset++] = 0x60;
	memcpy(pSendData+nOffset, passwd, 6);
	nOffset += 6;
	memcpy(pSendData+nOffset, writeData, len);
	nOffset += len;
	
	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff != g_RecvBuffer[5])
		{
			return -1;
		}
		else 
		{
			return 1;
		}	
	}
	return 0;
}
int Rfid_StoringCard_M1(void)
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;

	pSendData[nOffset++] = 0x03;
	pSendData[nOffset++] = 0x10;
	
	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff != g_RecvBuffer[5])
		{
			return -1;
		}
		else 
		{
			return 1;
		}	
	}
	return 0;
}
int Rfid_Enter_CPU(void)
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;

	pSendData[nOffset++] = 0x04 ;
	pSendData[nOffset++] = 0x01 ;
	
	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff == g_RecvBuffer[5])
		{
			return 1;
		}
	}
	return 0;
}
int Rfid_Cos_CPU(unsigned char CosLen, unsigned char *pCos, char *pRecv)
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;

	pSendData[nOffset++] = 0x04;
	pSendData[nOffset++] = 0xF1;
	
	memcpy(pSendData+nOffset, pCos, CosLen);
	nOffset += CosLen;
	
	int nLen = 0;
	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff == g_RecvBuffer[5])
		{
			nLen = EndianBig2Local16((unsigned char*)g_RecvBuffer+3)-2;
			memcpy(pRecv, g_RecvBuffer+6, nLen);
		}
	}
	return nLen;
}
int Rfid_Read_CPU(void)
{
	/*
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;

	pSendData[nOffset++] = 0x04;
	pSendData[nOffset++] = 0x81;
	
	// 数据长度
	pSendData[nOffset++] = 0x10;
	pSendData[nOffset++] = 0x00;
	
	pSendData[nOffset++] = 0x00;
	pSendData[nOffset++] = 0x0B;
	
	// 文件起止地址
	pSendData[nOffset++] = 0x00;
	pSendData[nOffset++] = 0x00;
	
	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff == g_RecvBuffer[5])
		{
			return 1;
		}
	}
	*/
	return 0;
}
int Rfid_Write_CPU(void)
{
	/*
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;

	pSendData[nOffset++] = 0x04;
	pSendData[nOffset++] = 0x80;
	
	// 数据长度
	pSendData[nOffset++] = 0x10;
	pSendData[nOffset++] = 0x00;
	
	pSendData[nOffset++] = 0x00;
	pSendData[nOffset++] = 0xD6;
	
	// 文件起止地址
	pSendData[nOffset++] = 0x00;
	pSendData[nOffset++] = 0x00;
	
	// 数据内容
	pSendData[nOffset++] = 0x00;
	pSendData[nOffset++] = 0x00;
	pSendData[nOffset++] = 0x00;
	pSendData[nOffset++] = 0x00;
	
	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff == g_RecvBuffer[5])
		{
			return 1;
		}
	}
	*/
	return 0;
}
int Rfid_Storing_CPU(void)
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;

	pSendData[nOffset++] = 0x04;
	pSendData[nOffset++] = 0xF0;

	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff == g_RecvBuffer[5])
		{
			return 1;
		}
	}
	return 0;
}
int Rfid_KeyAuthentication_CPU(unsigned char passwdLen, unsigned char *passwd)
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;

	pSendData[nOffset++] = 0x05;
	pSendData[nOffset++] = 0x01;
	
	pSendData[nOffset++] = passwdLen/2;
	pSendData[nOffset++] = 0x00;
	memcpy(pSendData+nOffset, passwd, passwdLen);
	nOffset += passwdLen;
	
	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff == g_RecvBuffer[5])
		{
			return 1;
		}
	}
	return 0;
}
int Rfid_Clear_CPU(void)
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;

	pSendData[nOffset++] = 0x05;
	pSendData[nOffset++] = 0x02;
	
	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff == g_RecvBuffer[5])
		{
			return 1;
		}
	}
	return 0;
}

int Rfid_Reset_PSAM(void)
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;

	pSendData[nOffset++] = 0x10;
	pSendData[nOffset++] = 0x01;
	
	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff == g_RecvBuffer[5])
		{
			return 1;
		}
	}
	return 0;
}

int Rfid_Cos_PSAM(void)
{
	memset(g_SendBuffer,0x00,SEND_BUFFER_SIZE);
	unsigned char nOffset = 0;
	char *pSendData = SEND_DATA_ADDR;

	pSendData[nOffset++] = 0x10;
	pSendData[nOffset++] = 0x02;
	
	pSendData[nOffset++] = 0x00;
	
	// 取8位随机数
	memcpy(pSendData+nOffset, "84000008", 8);
	nOffset += 8;
	
	if(Rfid_SendAndRecvData(pSendData, nOffset) > 0)
	{
		if(0xff == g_RecvBuffer[5])
		{
			return 1;
		}
	}
	return 0;
}