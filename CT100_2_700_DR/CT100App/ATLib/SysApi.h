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


/*
#define OutputLog		LOG

typedef enum
{
	DebugLog=1,		// ������־
	InformationLog,		// ��Ϣ��־
	WarningLog,			// �澯��־
	ErrorLog,			// ������־
	NoLog				// �������־
} LogType;*/

//void OutputLog(LogType nType, const char *strModule, const char *strFormat, ...);

unsigned short int EndianLittle2Local16(unsigned char *pFrom);
void EndianLocal2Little16(unsigned short int nLocalData, unsigned char *pTo);
unsigned short int EndianBig2Local16(unsigned char *pFrom);
unsigned int EndianBig2Local32(unsigned char *pFrom);
void EndianLocal2Big16(unsigned short int nLocalData, unsigned char *pTo);
void EndianLocal2Big32(unsigned int nLocalData, unsigned char *pTo);


// ���ַ�������߽�ȡn���ַ�
char*  SplitLeft(char *dst,char *src, int n);

// ���ַ������м��ȡn���ַ���nΪ���ȣ�mΪλ��
char*  SplitMid(char *dst,char *src, int n,int m);

// ���ַ������ұ߽�ȡn���ַ�
char*  SplitRight(char *dst,char *src, int n);

int SplitSize(char x,char * str);
int SplitSize_l(char x,char * str,int len);

int Split(char x , char * str, char (*result)[64]);
int Split_l(char x , char * str, int len,char (*result)[64]);

unsigned char CharToHex(char chFrom);
//unsigned char Str2HexByOneByte(char *pFrom);
unsigned short int Str2HexByOneByte(char *pFrom);

unsigned char BcdToHex(unsigned char nBcdData);
unsigned int HexToBcd(unsigned char nHexData);

void BcdToStr(char * dest,char * bcd,int bcdlength);

int HexToStr(char *pbDest, char *pbSrc, int nLen);
int HexToStr2(char *s, unsigned char *out2);
int HexToStr3(const char *sSrc,  char *sDest, int nSrcLen) ;

int StrToHex(char *pFrom, unsigned char *pTo);
int StrToHex2(char *lpszIn, int nLen);
int StrToHex3(char *pDest, char *data, unsigned char nByteLen);

int AsciiToBcd( char *pDst, char *pSrc);


void printHash(unsigned char *md, int len);
int getRandStr(char *strRand, int nRandNum);

char *URLEncode(char *str, int strSize) ;

void PrintBigCenter(char *Str, char *PrintStr);

