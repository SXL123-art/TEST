// Copyright (C) 2016 CAS
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
  *  Copyright 	:   (c) 2016 CAS
  *  Filename		:   SysApi.h
  *  Version		:   1.0
  *  Programmer(s)	:   DQ
  *  Created		:   2016/01/22
  *  Modified		:
  *  Description	:   QT and Application share GUI struct information for POS/standard Market software
  */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "SysApi.h"
//#include "../ATRtc/ATRtc.h"


#define CHAR2HEX(x) (((x)>'9')?((x)-'A'+10):((x)-'0'))
#define CHAR2DEC(x) ((x)-'0')

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

/****************************************************
字节顺序转换函数
****************************************************/
unsigned short int EndianLittle2Local16(unsigned char *pFrom)
{
	unsigned short ret;

	ret = pFrom[1];
	ret <<= 8;
	ret |= pFrom[0];

	return ret;
}

void EndianLocal2Little16(unsigned short int nLocalData, unsigned char *pTo)
{
	pTo[0] = nLocalData & 0xff;
	pTo[1] = (nLocalData >> 8) & 0xff;
}

unsigned short int EndianBig2Local16(unsigned char *pFrom)
{
	unsigned short ret;

	ret = pFrom[0];
	ret <<= 8;
	ret |= pFrom[1];

	return ret;
}

unsigned int EndianBig2Local32(unsigned char *pFrom)
{
	unsigned long ret;

	ret = pFrom[0];
	ret <<= 8;
	ret |= pFrom[1];
	ret <<= 8;
	ret |= pFrom[2];
	ret <<= 8;
	ret |= pFrom[3];
	
	return ret;
}

void EndianLocal2Big16(unsigned short int nLocalData, unsigned char *pTo)
{
	pTo[0] = (nLocalData >> 8) & 0xff;
	pTo[1] = nLocalData & 0xff;
}

void EndianLocal2Big32(unsigned int nLocalData, unsigned char *pTo)
{
	pTo[0] = (nLocalData >> 24) & 0xff;
	pTo[1] = (nLocalData >> 16) & 0xff;
	pTo[2] = (nLocalData >> 8) & 0xff;
	pTo[3] = nLocalData & 0xff;
}

/****************************************************
字符串截取函数
****************************************************/
// 从字符串的左边截取n个字符
char*  SplitLeft(char *dst,char *src, int n)
{
	char *p = src;
	char *q = dst;
	int len = strlen(src);
	if(n>len) 
		n = len;
	
	while(n--) 
		*(q++) = *(p++);
	
	*(q++)='\0'; 
	
	return dst;
}

// 从字符串的中间截取n个字符，n为长度，m为位置
char*  SplitMid(char *dst,char *src, int n,int m) 
{
	char *p = src;
	char *q = dst;
	int len = strlen(src);
	if(n>len) 
		n = len-m;    
	if(m<0) 
		m=0;    
	if(m>len) 
		return NULL;
	p += m;
	while(n--) 
		*(q++) = *(p++);
	*(q++)='\0'; 
	return dst;
}

// 从字符串的右边截取n个字符
char*  SplitRight(char *dst,char *src, int n)
{
	char *p = src;
	char *q = dst;
	int len = strlen(src);
	if(n>len) 
		n = len;
	p += (len-n);   
	while(*(q++) = *(p++));
	
	return dst;
}

int SplitSize(char x,char * str)
{
	int i=0;
	int ret = 0;
	for(;i<strlen(str);i++)
	{
		if(str[i] == x)
			ret ++;
	}
	
	return ret+1;
}
int SplitSize_l(char x,char * str,int len)
{
	int i=0;
	int ret = 0;
	for(;i<len;i++)
	{
		if(str[i] == x)
			ret ++;
	}
	
	return ret+1;
}

int Split(char x , char * str, char (*result)[64])
{
	//printf("in split\n");
	int i=0, head=0, tail=0, size=0;
	for(; i<=strlen(str); i++)
	{
		//printf("%02X ",str[i]);
		if(str[i] == x || str[i]==0x00)
		{			
			head = i;
			char s[64]={0x00};
			memset(*(result+size), 0x00, 64);
			memcpy(*(result+size), str+tail, head-tail);
			//printf("\nhead:%d    tail:%d\nresult:%s\n", head, tail, *(result+size));

			size++;
			tail = head+1;
		}
	}

	printf("Split Start: \n");
	for (int i=0; i<size; i++)
	{
		printf("%d--%s\n", i, result[i]);
	}
	printf("Split end: \n");
	
	return 0;
}

int Split_l(char x , char * str, int len,char (*result)[64])
{
	//printf("in split\n");
	int i=0, head=0, tail=0, size=0;
	for(; i<=len; i++)
	{
		//printf("%02X ",str[i]);
		if(str[i] == x)
		{			
			head = i;
			char s[64]={0x00};
			memset(*(result+size), 0x00, 64);
			memcpy(*(result+size), str+tail, head-tail);
			//printf("\nhead:%d    tail:%d\nresult:%s\n", head, tail, *(result+size));

			size++;
			tail = head+1;
		}
	}

	printf("Split Start: \n");
	for (int i=0; i<size; i++)
	{
		printf("%d--%s\n", i, result[i]);
	}
	printf("Split end: \n");
	
	return 0;
}

/****************************************************
转换函数
****************************************************/
const char HEXCH[] = "0123456789ABCDEF";


unsigned char CharToHex(char chFrom)
{
	if (chFrom>='0' && chFrom<='9')
	{
		return chFrom - '0';
	}
	else if (chFrom>='A' && chFrom<='Z')
	{
		return 0x0A + chFrom - 'A';
	}
	else if (chFrom>='a' && chFrom<='z')
	{
		return 0x0A + chFrom - 'a';
	}
	else
	{
		return 0;
	}
}

unsigned short int Str2HexByOneByte(char *pFrom)
{
	unsigned short int nuint8Return = 0;
	unsigned short int k = 0;

	for (int i=7; i<0; i--)
	{
		k = CharToHex(pFrom[i]);
		k <<= i;
		nuint8Return += k;
	}

	return nuint8Return;
}

unsigned char BcdToHex(unsigned char nBcdData)
{   
	return (nBcdData&0x0f) + (nBcdData>>4)*10;  
/*
	unsigned char temp;   
	temp=(nBcdData/16*10 + nBcdData%16);   
	
	return temp;   
*/
}   

unsigned int HexToBcd(unsigned char nHexData)   
{   
	unsigned char temp;   
	temp=(nHexData/10*16 + nHexData%10);   
	return temp;
}   

void BcdToStr(char * dest,char * bcd,int bcdlength)
{
	int i;
	for(i=0;i<bcdlength;i++)
	{
		sprintf(dest+i*2,"%02X",(unsigned char)bcd[i]);
	}
}

int HexToStr(char *pbDest, char *pbSrc, int nLen)
{
	char ddl,ddh;

	for (int i=0; i<nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57) 
			ddh = ddh + 7;
		if (ddl > 57) 
			ddl = ddl + 7;
		pbDest[i*2] = ddh;
		pbDest[i*2+1] = ddl;
	}
	pbDest[nLen*2] = '\0';
	return 0;
}
// 16进制字符串转字符数组 05bb164a8be0 --> 05 bb 16 4a 8b e0	
int HexToStr2(char *s, unsigned char *out2)  
{
	int len1 = strlen(s);
	int len2 = len1/2;
	int count=0;
	for(;strlen(s)>0;s+=2)
	{
		if(*s>='0' && *s<='9') 
			out2[count]=*s-'0';
		else if(*s>='A'&& *s<='F')
			out2[count]=*s-'A'+10;
		else if(*s>='a'&& *s<='f')
			out2[count]=*s-'a'+10;
		if(strlen(s)>1)
		{
			out2[count]*=16;
			if(*(s+1)>='0' && *(s+1)<='9') 
				out2[count++]+=(*(s+1)-'0');
			if(*(s+1)>='A'&& *(s+1)<='F') 
				out2[count++]+=(*(s+1)-'A'+10);
			if(*(s+1)>='a'&& *(s+1)<='f') 
				out2[count++]+=(*(s+1)-'a'+10);
		}
	}
	
	return count;
} 
int HexToStr3(const char *sSrc,  char *sDest, int nSrcLen)  
{  
	int  i;  
	char szTmp[3] = {0};  

	for(i = 0; i < nSrcLen; i++)  
	{  
	    sprintf(szTmp, "%02X", (unsigned char) sSrc[i]);  
	    memcpy(&sDest[i*2], szTmp, 2);  
	}  
	return 0;  
} 
/*
  字符串转16进制，返回16进制字符串
  字符串中如果有字母a-f必须大写
  比如: 123456 --> 12 34 56
*/
int StrToHex(char *pFrom, unsigned char *pTo)
{
	int i;
	char *p;

	i = 0;
	p = pFrom;

	while (1) 
	{
		if (*p==0 || *(p+1)==0)
		{
			break;
		}
		
		*pTo = (CHAR2HEX(*p)<<4)|(CHAR2HEX(*(p+1)));
		
		++pTo;
		++p;
		++p;
		++i;
	}

	*pTo++ = 0;
	
	return i;
}
/*
  字符串转16进制数，返回16进制数
  字符串中如果有字母a-f必须大写
  比如: 123456 --> 0x1E240(123456)
*/
int StrToHex2(char *lpszIn, int nLen)
{
	int nRet = 0;
	for(int i = 1; i <= nLen; i++)
	{
		if( *lpszIn >= 'A' && *lpszIn < 'Z')
		{
			nRet += (*lpszIn - '0' - 7) * ( pow(16, (nLen-i)) );
		}
		else
		{
			nRet += (*lpszIn - '0') * ( pow(16, (nLen-i)) );
		}
		lpszIn++;
	}
	return nRet;
}
// 字符串转16进制字符串 6301091007456 --> 05bb164a8be0	
int StrToHex3(char *pDest, char *data, unsigned char nByteLen)
{
	long long nLocalData = atoll(data);
	unsigned char pTo[12] = {0};
	for (int i=0; i<nByteLen; i++)
	{
		pTo[i] = (nLocalData >> nByteLen*8-(i+1)*8) & 0xff;
	}

	char strHexTemp[4] = {0};
	//printf("begin:\n");
	for (int i=0; i<nByteLen; i++)
	{
		//printf("%02X ", pTo[i]);
		sprintf(strHexTemp, "%02x", pTo[i]);
		strcat(pDest, strHexTemp);
	}
	//printf("\npDest:%s\n", pDest);
	return 0;
}

int AsciiToBcd( char *pDst, char *pSrc)
{
	int nLen = (strlen(pSrc)+1)/2;
	for (int i=strlen(pSrc)-1; i>0; i-=2)
	{
		int n1 = pSrc[i] - '0';
		int n2 = pSrc[i-1] - '0';
		pDst[i/2] = (n1&0xFF)|(n2<<4);
	}
	if (strlen(pSrc)%2 != 2)
	{
		pDst[0] = pSrc[0] - '0';
	}

	return nLen;
}


static unsigned char hexchars[] = "0123456789ABCDEF";

 
// 打印前， 有必要转换
void printHash(unsigned char *md, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        printf("%02x", md[i]);
    }

    printf("\n");
}
int getRandStr(char *strRand, int nRandNum)
{
	//定义随机生成字符串表
	char *str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	char ss[2] = {0};
	int lstr = strlen(str);//计算字符串长度
	srand((unsigned int)time((time_t *)NULL));//使用系统时间来初始化随机数发生器
	for (int i = 1; i <= nRandNum; i++)
	{
		//按指定大小返回相应的字符串
		sprintf(ss,"%c",str[(rand()%lstr)]);//rand()%lstr 可随机返回0-71之间的整数, str[0-71]可随机得到其中的字符
		strcat(strRand, ss);//将随机生成的字符串连接到指定数组后面
	}
	return 0;
}
  
char *URLEncode(char *str, int strSize)  
{  
    register unsigned char c;  
    unsigned char *to, *start;  
    unsigned char const *from, *end;  
      
    from = (unsigned char *)str;  
    end  = (unsigned char *)str + strSize;  
    start = to = (unsigned char *)calloc(1, 3*strSize+1);  
  
    while (from < end)   
    {  
        c = *from++;  
  
        if (c == ' ')   
        {  
            *to++ = '+';  
        }   
        else if ((c < '0' && c != '-' && c != '.') ||  
                 (c < 'A' && c > '9') ||  
                 (c > 'Z' && c < 'a' && c != '_') ||  
                 (c > 'z'))   
        {  
            to[0] = '%';  
            to[1] = hexchars[c >> 4];  
            to[2] = hexchars[c & 15];  
            to += 3;  
        }  
        else   
        {  
            *to++ = c;  
        }  
    }  
    *to = 0; 
    int nNewLen =  to - start; 
    
    return (char *) start;  
} 
/*
void OutputLog(LogType nType, const char *strModule, const char *strFormat, ...)
{
	if (nType==0)
	{
		return;
	}
	
	va_list list;
	char strLog[512] = {0};

	char strTime[24] = {0};
        //ATRtc_GetDateTimeHW(strTime);
	sprintf(strLog, "[%s][%d][%s]:", strTime, nType, strModule);
	int len = strlen(strLog);

	va_start(list, strFormat);
	vsnprintf(strLog+len, 512-len-2, strFormat, list);
	va_end(list);

	len = strlen(strLog);
	while (strLog[len-1]=='\r' || strLog[len-1]=='\n')
	{
		strLog[len-1] = '\0';
		len--;
	}
	strLog[len++] = '\r';
	strLog[len++] = '\n';
	strLog[len] = '\0';

	fprintf(stdout, "%s\n", strLog);
	if (nType==NoLog)
	{
		FILE *fid = fopen("log.txt","w+");
		fprintf(fid, "%s\n", strLog);
		fclose(fid);
	}
}
*/
void PrintBigCenter(char *Str, char *PrintStr)
{
	int x = 0;
	int nStrLen = strlen(Str);
	if (nStrLen > 15)
	{
		nStrLen = 15;
	}
	for (; x<(15-nStrLen)/2; x++)
	{
		PrintStr[x] = ' ';	
	}
	sprintf(&(PrintStr[x]), "%s", Str);
}
