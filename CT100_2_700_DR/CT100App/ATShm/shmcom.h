#pragma once
#ifndef SHMCOM_H
#define SHMCOM_H
#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "../../Public/typeTabUi.h"
#include "../public/typeShow.h"

class shmCom{
public:
    shmCom(decimal  *pDecimal);
    ~shmCom();
    void init();
    void initShm();
    void show(char *p,enQtShowType Type);
    void SetMaxWeight(unsigned int maxWeight);
private:
    void addDotSale(stTabSaleUi* p);
    void addDotLogin(stTabLoginUi* p);

public:
    decimal *m_deci;
    sem_t   *semr;			//有名信号灯 读
    sem_t   *semw;			//有名信号灯 写
    char    *shmAddress;                //共享内存地址
    bool    m_bReady;
    long    m_nMaxWeight;
    bool    m_bSetZero;                 //开机置零标记

private:

    stTabSaleUi  shmTabSaleUi;
    stTabLoginUi shmTabLoginUi;


};


#endif
