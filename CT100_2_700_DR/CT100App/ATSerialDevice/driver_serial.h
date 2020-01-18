#ifndef DRIVER_SERIALPORT_H
#define DRIVER_SERIALPORT_H

#pragma once

typedef enum tagFlowControl
{
	NoFlowControl,
	CtsRtsFlowControl,
	CtsDtrFlowControl,
	DsrRtsFlowControl,
	DsrDtrFlowControl,
	XonXoffFlowControl
}FlowControl;

class CMySerial
{
public:
	CMySerial(void);
	~CMySerial(void);

	int open_port(int port);
	int close_port();
	int setup_port(int speed, int data_bits, int parity, int stop_bits, FlowControl fc);
	int read_port(char* p, int size);
	int write_port(char* p, int size);

private:
	int fd;
};

#endif