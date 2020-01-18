// loaddll.cpp : Defines the entry point for the DLL application.
//
#include "code_convert.h"
#include "des.h"
#include "md5.h"

#include <stdio.h>

code_convert *pCodeconvert;
DES			 *pDes;
CMD5		 *pMd5;

void ATLib_Init()
{
	pCodeconvert = new code_convert();
	pDes = new DES();
	pMd5 = new CMD5();
	
}

int ATLib_g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	pCodeconvert->g2u(inbuf,inlen,outbuf,outlen);
}

int ATLib_u2g(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	pCodeconvert->u2g(inbuf,inlen,outbuf,outlen);
}

void ATLib_GenerateMD5(unsigned char* buffer,size_t bufferlen)
{
	pMd5->GenerateMD5(buffer,bufferlen);
}

char* ATLib_ToString()
{
	return pMd5->ToString();
}



