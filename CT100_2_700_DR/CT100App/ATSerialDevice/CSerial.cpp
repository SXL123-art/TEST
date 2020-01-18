#include "CSerial.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

CSerial::CSerial(void)
{
	b_SerialOpen = false;
	b_StopRecv = false;
	m_serial = new CMySerial;
}

CSerial::~CSerial(void)
{
	delete m_serial;
	m_serial = NULL;
}

int CSerial::Init(int nPort, int nSpeed, int nDataBits, char Parity, int nStopBits, FlowControl fc)
{
	// 打开串口
	if(!m_serial)
	{
		m_serial = NULL;
		b_SerialOpen = false;
		printf("-------m_serial is null----------\n");
		return -1;
	}
        if(true == b_SerialOpen)
        {
            return 0;
        }
	if( 0 < m_serial->open_port(nPort))
	{
		//打开串口成功	
		b_SerialOpen = true;
                //printf("Serial...............................[Y]\n");
		m_serial->setup_port(nSpeed, 8, 'N', 1, NoFlowControl);
	}
	else
	{
		b_SerialOpen = false;
		//printf("Serial...............................[X]\n");
		return -2;
	}
	return 0;
}
int CSerial::SendAndRecvData(char *sendData, int nsendLen, char *recvData, int recvTimeout)
{
	if (!b_SerialOpen)
	{
		printf("Serial SendAndRecvData is not open!\n");
		return -1;
	}
	// 发送数据
	int nWriteLen = m_serial->write_port(sendData, nsendLen);
       /* printf("nWriteLen:%d data:", nWriteLen);
	for(int i = 0; i<nWriteLen;i++)
		printf("%02x ",sendData[i]);
        printf("\n");*/

	int nTimeout = 0;
	int nRecvLen = 0;
        recvTimeout *= 10;
	b_StopRecv = false;
	bool nbTimeoutFlags = false;
	while(1)
	{
		// read_port一次超时时间是1S
                usleep(10*1000);
		nRecvLen = m_serial->read_port(recvData, 1024);
		if (nRecvLen > 0)
		{
			break;
		}
		if (++nTimeout >= recvTimeout)
		{
			nTimeout = 0;
			nbTimeoutFlags = true;
			break;
		}
                /*
		if (b_StopRecv)
		{
			nbTimeoutFlags = true;
			break;
                }*/
	}
	// 关闭串口
        //b_SerialOpen = false;
        //m_serial->close_port();
        /*
	if (nbTimeoutFlags)	
	{		
		nRecvLen = -2;	
        }*/
	return nRecvLen;
}

int CSerial::RecvData(char *recvData, int recvTimeout)
{
	if (!b_SerialOpen)
	{
		printf("Serial SendAndRecvData is not open!\n");
		return -1;
	}

	int nTimeout = 0;
	int nRecvLen = 0;
	recvTimeout *= 5;
	while(1)
	{
		// read_port一次超时时间是1S
		nRecvLen = m_serial->read_port(recvData, 1024);
		if (nRecvLen > 0)
		{
			break;
		}
		if (++nTimeout >= recvTimeout)
		{
			nTimeout = 0;
			b_SerialOpen = false;
			m_serial->close_port();
			return -2;
		}
	}
	/*
	printf("recvData:%\n", nRecvLen);
	for (int i=0; i<nRecvLen; i++)
	{
		printf("%02x ", recvData[i]);
	}
	printf("\n");
	*/
	// 关闭串口
	m_serial->close_port();
	
	return nRecvLen;
}

void CSerial::setStopRecv(bool bStop)
{
	b_StopRecv = bStop;
}
