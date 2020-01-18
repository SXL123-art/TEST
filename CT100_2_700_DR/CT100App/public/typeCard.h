
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
  *  Copyright 		:   (c) 2016 CAS
  *  Filename		:   typeCard.h
  *  Version		:   1.0
  *  Programmer(s)	:   DQ
  *  Created		:   2016/11/19
  *  Modified		:
  *  Description	:   CT100GC  RFID Card type file
  */

#ifndef TYPE_CARD_H
#define TYPE_CARD_H


#define CARD_NUMBERLEN			13
#define CARD_TYPE				2
#define MARKET_CODE				9
#define SELLER_CODE				13
#define SELLER_TYPE				3
#define SELLER_NAME				64
#define SELLER_ID				18
#define SELLER_TEL				11
#define TRADE_DATE				6
#define TRADE_COUNT				2
#define TRADE_MODE				2
#define TRADE_CODE				20
#define BATCH_COUNT				16
#define SUPPLIER				16


typedef struct tagSTBASEINFO
{
	char cardNumber[CARD_NUMBERLEN+1];		//卡号
	char cardType[CARD_TYPE+1];				//卡类型
	char marketCode[MARKET_CODE+1];		//主体编码
	char sellerCode[SELLER_CODE+1];			//经营者编码
	char sellerType[SELLER_TYPE+1];			//经营者类型
	char sellerName[SELLER_NAME+1];			//经营者名称
	char sellerId[SELLER_ID+1];				//身份证
	char sellerTel[SELLER_TEL+1];				//电话
	char tradeDate[TRADE_DATE+1];			//交易日期，最后一笔
	char tradeCount[TRADE_COUNT+1];			//交易笔数
	char tradeMode[TRADE_MODE+1];			//交易模式
}stBaseInfo,*pstBaseInfo;


typedef struct tagSTTRADEDETAIL
{
	char goodsCode[8+1];
	char goodsName[12+1];
	char weight[6+1];
	char pieces[6+1];
	char price[2+1];
	char total[6+1];
	char payType[2+1];
	char saleCount[2+1];
	char saleSerial[4+1];
	char saleDate[6+1];
	char saleTime[6+1];
	char traceCode[TRADE_CODE+1];
	char batchCode[BATCH_COUNT+1];
	char supplier[SUPPLIER+1];		// 供应商
}stTradeDetail,*pstTradeDetail;

typedef struct tagSTTRACE
{
	stBaseInfo	baseinfo;
	stTradeDetail	detail;
}stCardTrace,*pstCardTrace;

typedef struct
{
	unsigned char cardsaleata[16*3];
}QHCardData;

#endif
