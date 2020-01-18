#include "code_convert.h"
#include <stdio.h>
#include <string.h>
#include "../public/globals.h"

int code_convert::code_converts(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf, size_t outlen)
{
	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset,from_charset);
	if (cd==0){
		printf("iconv_open error!\n");
		return -1;
	}

	memset(outbuf,0,outlen);
	if(iconv(cd,pin,&inlen,pout,&outlen) == -1){
        	memcpy(outbuf,inbuf,inlen);
		printf("iconv error!\n");
		iconv_close(cd);
		return -1;
	}

	iconv_close(cd);
	return 0;
}
//UNICODE
int code_convert::u2g(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	return code_converts((char *)"utf-8",(char *)"gb2312",(char *)inbuf,inlen,(char *)outbuf,outlen);
}
//GB2312
int code_convert::g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	return code_converts((char *)"gb2312",(char *)"utf-8",(char *)inbuf,inlen,(char *)outbuf,outlen);
}
