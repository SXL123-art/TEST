
#include "driver_serial.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


CMySerial::CMySerial(void)
{

}


CMySerial::~CMySerial(void)
{

}

int CMySerial::open_port(int port)
{
	int ret;
	char device[20] = {0};
	memset(&device, 0, 20);

	if (port < 1 || port > 11){
		printf("Sorry, the port number must be 1~4.\n");
	}

	sprintf(device, "/dev/ttySP%d", port-1);//MX28 上串口设备名称是ttySP
	
	fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1){
		printf("Unable to open the port :%s fd:%d\n", device,fd);		
		return fd;
	}

	ret = fcntl(fd, F_SETFL, 0);
	if (ret < 0){
		printf("fcntl fail...\n");
	}
	ret = isatty(STDIN_FILENO);
	if (ret == 0){
		//printf("Standard input is not a terminal device :%s\n", device);
	}
	else
		printf("Open %s the port success!\n", device);

	return fd;
}

int CMySerial::close_port()
{
	int ret = close(fd);
	if(ret < 0){
		//printf("Unable to close the port.");
	}
	else
		//printf("Close the port success!\n");
	return ret;
}

int CMySerial::setup_port(int speed, int data_bits, int parity, int stop_bits, FlowControl fc)
{
	int speed_arr[] = {B115200, B9600, B38400, B19200, B4800};
	int name_arr[] = {115200, 9600, 38400, 19200, 4800};
	struct termios opt;
	int ret=-1;
	int i=0;
	int len=0;

	ret = tcgetattr(fd, &opt);		/* get the port attr */
	if (ret < 0){
		printf("Unable to get the attribute!\n");
	}
	/* baud rate */
	len = sizeof(speed_arr) / sizeof(int);
	for (i = 0; i < len; i++){
		if (speed == name_arr[i]){
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&opt, speed_arr[i]);
			cfsetospeed(&opt, speed_arr[i]);
			if(tcsetattr(fd, TCSANOW, &opt) != 0)
            {
                printf("tcsetattr fd\n");
                return 0;
            }
            tcflush(fd, TCIOFLUSH);
		}
		if (i == len){
			printf("Unsupported baud rate.");
		}
	}

	ret = tcgetattr(fd, &opt);		/* get the port attr */
	if (ret < 0){
		printf("Unable to get the attribute!\n");
	}
	opt.c_cflag |= (CLOCAL | CREAD); /* enable the receiver, set local mode */
	opt.c_cflag &= ~CSIZE;			/* mask the character size bits*/

	/* data bits */
	switch (data_bits)
	{
	case 8:		opt.c_cflag |= CS8;		break;
	case 7:		opt.c_cflag |= CS7;		break;
	default:	printf("Unsupported data bits.");
	}

	/* parity bits */
	switch (parity)
	{
	case 'N':
	case 'n'://无校验,修改好的代码
		opt.c_cflag &= ~PARENB;
		//opt.c_cflag &= ~(OPOST);
		opt.c_iflag &= ~INPCK  ;
		break;
	case 'O'://奇数校验
	case 'o':
		opt.c_cflag|=(INPCK|ISTRIP); /*enable parity check, strip parity bits*/
		opt.c_cflag |= (PARODD | PARENB);
		break;
	case 'E'://偶数校验
	case 'e':
		opt.c_cflag|=(INPCK|ISTRIP); /*enable parity check, strip parity bits*/
		opt.c_cflag |= PARENB;
		opt.c_cflag &= ~PARODD;
		break;
	default:
		printf("Unsupported parity bits.");
	}

	/* stop bits */
	switch (stop_bits)
	{
	case 1:		opt.c_cflag &= ~CSTOPB;		break;
	case 2:		opt.c_cflag |= CSTOPB;		break;
	default:	printf("Unsupported stop bits.");
	}

	opt.c_cflag |= (CLOCAL | CREAD); /* enable the receiver, set local mode */

	/* raw input */
	opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	/* raw ouput */
	opt.c_oflag &= ~OPOST;
	opt.c_oflag &= ~(ONLCR | OCRNL);    //添加的

	opt.c_iflag &= ~(ICRNL | INLCR);
       opt.c_iflag &= ~(IXON | IXOFF | IXANY);    //添加的

	tcflush(fd, TCIFLUSH);
	opt.c_cc[VTIME] = 0; /* no time out */
	opt.c_cc[VMIN] = 0; /* minimum number of characters to read */

	ret = tcsetattr(fd, TCSANOW, &opt); /* update it now */
	if (ret < 0){
		printf("Unable to setup the port.\n");
	}
	else
	{
		//printf("Setup the port OK!\n");
	}
	
	return 0; /* everything is OK! */
}

int CMySerial::read_port(char* p, int size)
{
	int recvlen=0;
	
	int nLen = 0;
	char buf[1024];
	int fd_sel;
	fd_set fs_read;

	struct timeval tv_timeout;
	
	while (1)
	{
		FD_ZERO(&fs_read);
		FD_SET(fd,&fs_read);

		tv_timeout.tv_sec = 0;
                tv_timeout.tv_usec = 50000;	// 200ms
		fd_sel = select(fd+1, &fs_read, NULL, NULL, &tv_timeout);
		if(fd_sel > 0)
		{
			if(FD_ISSET(fd, &fs_read))
			{
				nLen = read(fd, buf, size);
				memcpy(p+recvlen, buf, nLen);
				recvlen += nLen;
				memset(buf, 0 , sizeof(buf)); 
			}
		}
		else
		{
			// 200ms无数据进来就break
			break;
		}
	}
	return recvlen;
}

int CMySerial::write_port(char* p, int size)
{	
	int sendlen=0;
	int fd_sel;
	fd_set fs_send;

	struct timeval tv_timeout;
	FD_ZERO(&fs_send);
	FD_SET(fd,&fs_send);
	tv_timeout.tv_sec = 1;
	tv_timeout.tv_usec = 0;

	fd_sel = select(fd+1,NULL,&fs_send,NULL,&tv_timeout);
	
	if(fd_sel>0)
	{
		if(FD_ISSET(fd,&fs_send))
		{
			sendlen = write(fd, p, size);
		}
	}
	else
	{
		sendlen = -1;
	}
/*
	if (sendlen > 0)
	{
		printf("sendlen:%d\n", sendlen);
		for (int i=0; i<sendlen; i++)
			printf("%02x ", p[i]);
		printf("\n");
	}
*/	
	return sendlen;
}

