#pragma once


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../public/typePrinter.h"
#include "../public/TemplateDulLink.h" //

//»»√Ù ’æ›
class CReceiptThermal
{
public:
	CReceiptThermal(void);
	~CReceiptThermal(void);
	
	void init(void);	
	void add(stReceiptData data);
	int Print(void);
        int SaveQrcode(char *str);
	
	int PrintPaper(int nCow);
        //int PrintLable(PrintLableData *pLable);
	
public:	
	 CDList<stReceiptData>* 	m_pReceiptBuf;	
         pthread_mutex_t                m_mutex ;
};

