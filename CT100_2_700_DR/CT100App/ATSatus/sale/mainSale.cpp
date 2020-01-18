#include "mainSale.h"

void* theadAutoSaveSale(void* p)
{
    long	nLastWeight;	// 重量
    CMainSale* pMainSale = (CMainSale*)p;
    printf("-------------------------------------------[theadAutoSaveSale]------------------------------\n");
    while(1)
    {
        usleep(20*1000);
        if(pMainSale->m_pSaleShareInfo->m_ShareUserSwitchInfo.RecordSwitch != 1)
        {
            usleep(1000*1000);
            continue;
        }
        if (!pMainSale->m_bAutoSave)
        {
            if ((pMainSale->m_pSaleShareInfo->shareInfoSaleUi.stable == '1')
                    && (atol(pMainSale->m_pSaleShareInfo->shareInfoSaleUi.totalprice) > 0))
            {
                // 商品稳定后的重量
                nLastWeight = pMainSale->m_nWeight;
                pMainSale->m_nLastPrice = pMainSale->m_nPrice;
                pMainSale->m_nLastTotal = pMainSale->m_nTotal;
            }

            int nTemp = 0;
            if (nLastWeight > 0)
            {
                    nTemp = nLastWeight*pMainSale->m_pSaleShareInfo->m_ShareUserSwitchInfo.fThresholdVale;
            }
            //如果按有累计就不强制记录，去掉此次，修改m_bAccumulative变量逻辑即可
            //add song 解决累计强制记录问题，有累计也可单笔强制记录
            //解决有总价，去皮置0强制记录的问题
            if(true == pMainSale->m_bAccumulative || true ==  pMainSale->m_bZeroTareAccumulative)
            {
                    nLastWeight = 0;
                    continue;
            }

            if ((nTemp > pMainSale->m_nWeight)
                    && (pMainSale->m_nLastPrice > 0)
                    && (pMainSale->m_nLastTotal > 0)
                    && (nLastWeight > 0)
                    && (pMainSale->m_pSaleShareInfo->m_ShareUserSwitchInfo.RecordSwitch == 1)
                    && (!pMainSale->m_bAccumulative)
                    && (!pMainSale->m_bZeroTareAccumulative)
                    && (atol(pMainSale->m_pSaleShareInfo->shareInfoSaleUi.totalprice) > 0))
            {
                    printf("theadAutoSaveSale: %d--%d--%d--%d--%d\n", nTemp, pMainSale->m_nWeight, pMainSale->m_nLastPrice, pMainSale->m_nLastTotal, nLastWeight);
                    pMainSale->m_bAutoSave = true;
                    pMainSale->m_nLastWeight = nLastWeight;
                    nLastWeight = 0;

                    pMainSale->AutoSaveSale();
            }
        }
    }
}

CMainSale::CMainSale(CShareInfo *pInfo)
{
    m_pSaleShareInfo = pInfo;
    m_bAutoSave = false;
    m_bAccumulative = false;
    m_bSelectPlu = false;
    m_bZeroTareAccumulative = false;
    m_pSaleFunction = new CSaleFunction(m_pSaleShareInfo);

    stbF1 = new struct_tb_business[BUSSINESS_SIZE];
    stbF2 = new struct_tb_business[BUSSINESS_SIZE];
    stbF3 = new struct_tb_business[BUSSINESS_SIZE];
    stbF4 = new struct_tb_business[BUSSINESS_SIZE];

    m_nLastWeight = 0;
    m_nLastPrice = 0;
    m_nLastTotal = 0;

    m_nFn = 0;
    m_nSaleTotalF1 = 0;
    m_nSaleNumF1 = 0;
    m_nSaleTotalF2 = 0;
    m_nSaleNumF2 = 0;
    m_nSaleTotalF3 = 0;
    m_nSaleNumF3 = 0;
    m_nSaleTotalF4 = 0;
    m_nSaleNumF4 = 0;
    m_nSaleTotal = 0;
    m_nDiscountSaleTotal = 0;

    m_bSelectPluNum = false;

    pthread_t idAutoSaveSale;
    int pthread = pthread_create(&idAutoSaveSale, NULL, theadAutoSaveSale, this);
    if (pthread)
    {
        errno = pthread;
        perror("create theadAutoSaveSale thread");
    }
    pthread = pthread_detach(idAutoSaveSale);
    if (pthread) {
        errno = pthread;
        perror("detach theadAutoSaveSale thread");
    }
    sprintf(m_pSaleShareInfo->shareInfoSaleUi.plu,"%s" ,"自由商品");

    if (pInfo->m_ShareUserSwitchInfo.TotalPriceSwitch == 1 || pInfo->m_ShareUserSwitchInfo.SavePluSwitch == 1)
    {
        m_bSelectPluNum = true;
        m_nPluHotKey = 1;
        FindPluInfoByKey();
    }
    else
    {
        sprintf(m_pSaleShareInfo->shareInfoSaleUi.plu,"%s" ,"自由商品");
    }

}
CMainSale::~CMainSale()
{

}

enQtShowType CMainSale::SaleProc()
{
    printf("-------------------------------------------MainSale\n");
    m_pSaleShareInfo->m_enQtUiType = enQt_SaleQT;
    RefreshSaleInfo();
    updateQtShow(m_pSaleShareInfo);
    m_bUpdate = true;
    while(1)
    {
        usleep(20*1000);
        if(m_pSaleShareInfo->m_enQtUiType == enQt_MsgQT)
        {
            continue;
        }
        switch (SaleDeal())
        {
        case enQt_LoginQT:
                // 退回登陆界
                return enQt_LoginQT;

        case enQt_SaleQT:
                // 继续在销售主界面
                break;

        case enQt_PayQT:
                // 到付款界面
                return enQt_PayQT;

        case enQt_MenuQT:
                // 配置界面
                return enQt_MenuQT;

        case enQt_CalQt:
                // 一键标定配置界面
                return enQt_CalQt;

        case enQt_ReportQt:
                // 报表界面
                return enQt_ReportQt;

        case enQt_GoodsKeyQT:
                // 快捷键对应界面
                return enQt_GoodsKeyQT;
         default:
            break;
        }

        CalculateAndUpdate(m_pSaleShareInfo);
        if (m_nTotal >= 999999)
        {

            sprintf(m_pSaleShareInfo->shareInfoSaleUi.totalprice ,"%ld",m_nTotal);
        }
        else
        {
            if (m_pSaleShareInfo->m_ShareUserSwitchInfo.TotalPriceSwitch == 1 && m_bSelectPlu == false)
            {
                m_nTotal = 0;//无商品不显示总价
                m_nPrice = 0;
                memset(m_pSaleShareInfo->shareInfoSaleUi.totalprice,0x00,sizeof(m_pSaleShareInfo->shareInfoSaleUi.totalprice));
                sprintf(m_pSaleShareInfo->shareInfoSaleUi.totalprice ,"%s","");
                m_pSaleShareInfo->ShowMessage("无商品不显示总价打开");
            }
            else
            {
                sprintf(m_pSaleShareInfo->shareInfoSaleUi.totalprice ,"%ld",m_nTotal);
            }
        }
    }
}
int CMainSale::showMsg(enMsgType MsgType,char *pTitle,char *pMSG)
{
    int nRet = showMsgProc(m_pSaleShareInfo,MsgType,pTitle,pMSG);
    printf("-------------------------------------------MainSale\n");
    m_pSaleShareInfo->m_enQtUiType = enQt_SaleQT;
    updateQtShow(m_pSaleShareInfo);
    return nRet;
}
int CMainSale::showInputMsg(enInputType InputType, char *pInputMSG)
{
    int nRet = showInputMsgProc(m_pSaleShareInfo,InputType,pInputMSG);
    sprintf(m_pSaleShareInfo->m_stInputInfo.input,"%s","");
    printf("-------------------------------------------MainSale\n");
    m_pSaleShareInfo->m_enQtUiType = enQt_SaleQT;
    updateQtShow(m_pSaleShareInfo);
    return nRet;
}

int CMainSale::CheckSaleStatus(void)
{
    if ((m_pSaleShareInfo->m_ShareSaleInfo.SaleStatus == enPayPrintReady)
            || (m_pSaleShareInfo->m_ShareSaleInfo.SaleStatus == enPayCancel)
            || (m_pSaleShareInfo->m_ShareSaleInfo.SaleStatus == enPaySave))
    {
        if (m_pSaleShareInfo->m_ShareSaleInfo.SaleStatus == enPayPrintReady
            || (m_pSaleShareInfo->m_ShareSaleInfo.SaleStatus == enPaySave))
        {
            if (m_pSaleShareInfo->m_ShareSaleInfo.SaleStatus == enPayPrintReady)
            {
                PrintSale();
                printf("enPayPrintReady  Fn print or pay print\n");
            }
            SaveSaleToDBFn();
            m_bUpdate = true;
            RefreshSaleInfo();
        }
    }
}

enQtShowType CMainSale::SaleDeal()
{
    int nResult = 0;
    unsigned char ch = 0;

   // 计时
   static unsigned int nDealCnt = 0;
   static unsigned char nInputNum = 0;


   // 检查交易状态
   CheckSaleStatus();

   // 输入单价超过4S后，需要重新数据
   if (++nDealCnt >= 200)
   {
       nDealCnt = 0;
       if (nInputNum == 1)
       {
           nInputNum = 0;
       }
   }



   if (SALE_KEY_VALID == ATKey_getKey(&ch))
   {
      m_bUpdate = true;
      m_bZeroTareAccumulative = false;
      m_pSaleShareInfo->ShowMessage("");
      if ((ch>=_KEY_PLU1) && (ch<=_KEY_PLU60))
      {
          if(nInputNum == 0)
          {
              nInputNum = 1;
              PluPage(ch,1);
          }
          else
          {
              //翻2页是2，翻3页是3
              PluPage(ch,2);
          }
          nDealCnt = 0;
          return enQt_SaleQT;
      }
      else  if ((ch>=_KEY_NUM0) && (ch<=_KEY_NUM9)  || (ch==_KEY_DOT))
      {
          if(nInputNum == 0 &&  m_pSaleShareInfo->GetSaleMode() == enWeight)
          {
              nInputNum = 1;
              ClearEditNum();
              memset (m_pSaleShareInfo->shareInfoSaleUi.unitprice, 0, LEN10);
          }
          nDealCnt = 0;
          nInputNum = 1;
          InputPriceOrPieces(ch);
          return enQt_SaleQT;
      }

      switch(ch)
      {
      case _KEY_F1:
      case _KEY_F2:
      case _KEY_F3:
      case _KEY_F4:
          nResult = AddFnSale(ch);
          if(10 == nResult)
          {
              // 累计界面按打印键
              m_pSaleShareInfo->m_ShareSaleInfo.SaleStatus = enPayPrintReady;
              return enQt_SaleQT;
          }
          else if(0 == nResult)
          {
              // 累计界面按取消键
              m_bAccumulative = false;
              RefreshSaleInfo();
              ClearEditNum();
              if(m_pSaleShareInfo->GetSaleMode() == enPieces)
              {
                  ChangeSaleMode(m_pSaleShareInfo, enWeight);
                  memset(m_pSaleShareInfo->shareInfoSaleUi.unitprice, 0, LEN10);
              }
          }
          else if(-10 == nResult)
          {
              //累计界面按清除,清除对应F上的累计
              RefreshSaleInfo();
          }
          else if (1 == nResult)
          {
              //重量拿走归零
              RefreshSaleInfo();
          }
          break;
          case _KEY_MENU:
                m_nFunctionKey = atoi(m_pSaleShareInfo->shareInfoSaleUi.unitprice)/100;
                if(8888 == m_nFunctionKey)
                {
                    if(showInputMsg(enInputCalpasswd,"请输入标定密码")>0)
                    {
                        return enQt_CalQt;
                    }
                    return enQt_SaleQT;
                }
                else if(9999 == m_nFunctionKey)
                {
                    if(showInputMsg(enInputPasswd,"请输入按键绑定密码")>0)
                    {
                        return enQt_GoodsKeyQT;
                    }
                    return enQt_SaleQT;
                }

              if(showInputMsg(enInputPasswd,"请输入菜单密码")>0)
              {
                  return enQt_MenuQT;
              }
              return enQt_SaleQT;
          case _KEY_LOGIN:
              break;
          case _KEY_PLU:
              if(atoi(m_pSaleShareInfo->shareInfoSaleUi.unitprice)%10 >0)
                  m_nFunctionKey = atoi(m_pSaleShareInfo->shareInfoSaleUi.unitprice);
              else if(atoi(m_pSaleShareInfo->shareInfoSaleUi.unitprice)%100 >0)
                  m_nFunctionKey = atoi(m_pSaleShareInfo->shareInfoSaleUi.unitprice)/10;
              else
                  m_nFunctionKey = atoi(m_pSaleShareInfo->shareInfoSaleUi.unitprice)/100;

              ClearEditNum();
              memset (m_pSaleShareInfo->shareInfoSaleUi.unitprice, 0, LEN10);
              m_nPluHotKey = m_nFunctionKey;
              m_bSelectPluNum = true;
              FindPluInfoByKey();
              break;
          case _KEY_ADD:
              m_nFunctionKey = atoi(m_pSaleShareInfo->shareInfoSaleUi.unitprice)/100;
              if (m_nFunctionKey > 9989)
              {
                  ClearEditNum();
                  memset (m_pSaleShareInfo->shareInfoSaleUi.unitprice, 0, LEN10);
                  FindFunctionKey();
              }
              break;
          case  _KEY_WEIGHT_MODE:
              if (m_pSaleShareInfo->GetSaleMode() == enWeight)
              {
                  if(atoi(m_pSaleShareInfo->shareInfoSaleUi.unitprice) <= 0)
                  {
                      m_pSaleShareInfo->ShowMessage("切换计件模式,请先输入单价");
                      ATBeepErr();
                      break;
                  }
                  ClearEditNum();
                  ChangeSaleMode(m_pSaleShareInfo, enPieces);
                  sprintf(m_pSaleShareInfo->shareInfoSaleUi.weight, "%s", "1Pcs");
              }
              else
              {
                    ClearEditNum();
                    ChangeSaleMode(m_pSaleShareInfo, enWeight);
              }
                break;

          case _KEY_CANCEL:
                RefreshSaleInfo();
                break;

          case _KEY_CLR:
                memset(m_pSaleShareInfo->shareInfoSaleUi.unitprice, 0, sizeof(m_pSaleShareInfo->shareInfoSaleUi.unitprice));
              RefreshSaleInfo();
              break;
      case _KEY_PRINT:

          if (atol(m_pSaleShareInfo->shareInfoSaleUi.totalprice) <= 0 )
          {
              // 未显示总价，不打印
              m_pSaleShareInfo->ShowMessage("无总价不能打印");
              ATBeepErr();
              break;
          }
          m_nFn = 0;//单笔交易
          if(SaveFnSale()== -1)
              break;
          SubtotalSaleFn();
          nResult = showAddSaleMsgProc(false);
          m_pSaleShareInfo->m_enQtUiType = enQt_SaleQT;
          m_pSaleShareInfo->UpdateGui();
//          if(nResult != 5)//只要不是支付界面，就要刷新累计界面
//          {
//              m_pSaleShareInfo->m_enQtUiType = enQt_SaleQT;
//              m_pSaleShareInfo->UpdateGui();
//          }
          if(10 == nResult)
          {
              PrintSale();
              SaveSaleToDBFn();
              RefreshSaleInfo();
          }
          else{
              DeleteAllSale(m_nFn);
              RefreshSaleInfo();
          }
          break;
      case _KEY_PAY_MODE:
          if(m_pSaleShareInfo->m_ShareSysSwitchInfo.CashSwitch == 1)
          {
             ATPmu_CtrlCashBox();
          }
          break;
          if (strlen(m_pSaleShareInfo->shareInfoSaleUi.totalprice) <= 0)
          {
              // 未显示总价
              ATBeepErr();
              break;
          }
          SaveFnSale();
          SubtotalSaleFn();
          m_pSaleShareInfo->m_ShareSaleInfo.PayMoney = m_nSaleTotal;
          m_pSaleShareInfo->m_ShareSaleInfo.SaleAmount = m_nSaleNum;
          m_pSaleShareInfo->m_ShareSaleInfo.SaleClosingTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
          return enQt_PayQT;
      case _KEY_PAPER:
          m_nFunctionKey = atoi(m_pSaleShareInfo->shareInfoSaleUi.unitprice)/100;
          if(1 == m_nFunctionKey)
          {
              m_pSaleFunction->PrintLastSale();
          }
          else if(2 == m_nFunctionKey)
          {
              if(showInputMsg(enInputPasswd,"请输入报表密码")>0)
              {
                  ClearEditNum();
                  return enQt_ReportQt;
              }
          }
          else
          {
              m_pSaleShareInfo->m_pPrint->PrintPaper(2);
          }
          ClearEditNum();
          break;

      case _KEY_ZERO:
      case _KEY_TARE:
          m_bZeroTareAccumulative = true;
          if( -1 == m_pSaleShareInfo->ZeroTare(ch))
          {
              m_pSaleShareInfo->ShowMessage("重量超出置零范围");
              ATBeepErr();
          }
          break;
      default:
             break;
      }
   }
   return enQt_SaleQT;
}

int CMainSale::InputPriceOrPieces(unsigned char ch)
{
    int nCheckEditNum = 0;
    if ((m_pSaleShareInfo->GetSaleMode()==enPieces))
    {
        if(ch == _KEY_DOT)
        {
            return 0;
        }
        // 修改件数
        if(m_pSaleShareInfo->m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT == 3)
        {
            EditNum(ch, 3);
        }
        else if(m_pSaleShareInfo->m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT == 2)
        {
            EditNum(ch, 2);
        }
        nCheckEditNum = atoi(m_strNum);
        if(0 == nCheckEditNum)
        {
            ClearEditNum();
            return 0;
        }
        memset(m_pSaleShareInfo->shareInfoSaleUi.weight,0x00,sizeof(m_pSaleShareInfo->shareInfoSaleUi.weight));
        sprintf(m_pSaleShareInfo->shareInfoSaleUi.weight, "%d%s", nCheckEditNum/m_pSaleShareInfo->m_ShareSystemInfo.nWeightCoe, "Pcs");
        printf("w----m_pSaleShareInfo->shareInfoSaleUi.weight: %s\n", m_pSaleShareInfo->shareInfoSaleUi.weight);
        return 1;
    }

    EditNum(ch, 2);
    memset(m_pSaleShareInfo->shareInfoSaleUi.unitprice, 0, LEN10);
    sprintf(m_pSaleShareInfo->shareInfoSaleUi.unitprice, "%s", m_strNum);
    printf("u----pInfoMainSale->m_stSaleInfo.unitprice: %s\n", m_pSaleShareInfo->shareInfoSaleUi.unitprice);
    return 0;
}

int CMainSale::FindPluInfoByKey(void)
{
    m_bSelectPlu = false;
    ClearEditNum();
    QString sqlStr;
    if(false == m_bSelectPluNum)
    {
        sqlStr.sprintf("select goodsCode from table_convert where hotKey='%d';",m_nPluHotKey);
        QString returnStr;
        ATdb_Select_ReturnQString_By_SQL_Statements(sqlStr,&returnStr);
        qDebug() << returnStr;
        if(returnStr.length()<=0)
        {
            RefreshSaleInfo();
            sprintf(m_pSaleShareInfo->shareInfoSaleUi.message,"热键号%d,未找到商品信息",m_nPluHotKey);
            m_pSaleShareInfo->UpdateGui();
            ATBeep();

            return 0;
        }
        sqlStr.clear();
        sqlStr = QString("select * from table_product where goodsCode='%1';").arg(returnStr);
    }
    else{
        sqlStr = QString("select * from table_product where goodsCode='%1';").arg(m_nPluHotKey);
        qDebug() << sqlStr;
    }
    m_bSelectPluNum = false;
    QSqlQuery query;
    if(0 >= ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sqlStr,&query))
    {
        RefreshSaleInfo();
        ATBeep();
        sprintf(m_pSaleShareInfo->shareInfoSaleUi.message,"热键号%d,未找到商品信息",m_nPluHotKey);
        m_pSaleShareInfo->UpdateGui();
        query.clear();

        return 0;
    }

    m_bSelectPlu = true;

    query.first();
    QSqlRecord record = query.record();
    m_pSaleShareInfo->stp->goodsCode = record.value(TABLE_PRODUCT_GOODSCODE).toString();
    m_pSaleShareInfo->stp->goodsName = record.value(TABLE_PRODUCT_GOODSNAME).toString();
    m_pSaleShareInfo->stp->lastPrice = record.value(TABLE_PRODUCT_LASTPRICE).toString();
    m_pSaleShareInfo->stp->isPcs = record.value(TABLE_PRODUCT_ISPCS).toString();
    if(0 == m_pSaleShareInfo->stp->isPcs.compare("1"))
    {
        ChangeSaleMode(m_pSaleShareInfo, enPieces);
        sprintf(m_pSaleShareInfo->shareInfoSaleUi.weight, "%s", "1Pcs");
    }
    else
    {
        ChangeSaleMode(m_pSaleShareInfo, enWeight);
    }
     //显示商品名称
    memset(m_pSaleShareInfo->shareInfoSaleUi.plu, 0, LEN64);
    std::string str = m_pSaleShareInfo->stp->goodsName.toStdString();
    sprintf(m_pSaleShareInfo->shareInfoSaleUi.plu, "%s",str.c_str());
    if(1 == m_pSaleShareInfo->m_ShareUserSwitchInfo.SavePriceSwitch)
    {
        sprintf(m_pSaleShareInfo->shareInfoSaleUi.unitprice, "%s",m_pSaleShareInfo->stp->lastPrice.toStdString().c_str());
    }
    printf("plu:%s\n",m_pSaleShareInfo->shareInfoSaleUi.plu);
    query.clear();
    return 1;
}
int CMainSale::PluPage(unsigned char ch,unsigned char PluPage)
{
    if(PluPage == 0 && PluPage > 3)
    {
        printf("--------------plu return page error : %d\n",PluPage);
        return 0;
    }
    static unsigned char lastCh = 0;
    static unsigned char findCount = 0;

    if(lastCh == ch)
    {
        findCount++;
        if(findCount>PluPage)
        {
            findCount = 1;
        }
    }
    else
    {
        findCount = 1;
    }
    switch(findCount)
    {
        case 1:
            m_nPluHotKey = ch;
            FindPluInfoByKey();
            break;
        case 2:
            #if (KEY_TYPE == SMALL_KEY)
                    m_nPluHotKey = ch + 30;
            #elif (KEY_TYPE == MIDDLE_KEY)
                    m_nPluHotKey = ch + 60;
            #endif
            FindPluInfoByKey();
            break;

        case 3:
            #if (KEY_TYPE == SMALL_KEY)
                    m_nPluHotKey = ch + 60;
            #elif (KEY_TYPE == MIDDLE_KEY)
                    m_nPluHotKey = ch + 120;
            #endif
            FindPluInfoByKey();
            break;
    }
    lastCh = ch;
    return 0;
}
int CMainSale::AddFnSale(unsigned char AddFn)
{
    m_nFn = AddFn - _KEY_F1 + 1;
    printf("----------------------------------------x----m_nFn:%d\n",m_nFn);
    switch(m_nFn)
    {
        case KEY_ADD1:
             if(atoi(m_pSaleShareInfo->shareInfoSaleUi.totalprice)<=0 && m_nSaleNumF1 == 0)
             {
                 m_pSaleShareInfo->ShowMessage("累计A 无总价或累计信息");
                 ATBeepErr();
                 return -1;
             }
             if(atoi(m_pSaleShareInfo->shareInfoSaleUi.totalprice) > 0)
             {
                 if (m_nSaleNumF1 >= BUSSINESS_SIZE)
                 {
                     m_pSaleShareInfo->ShowMessage("累计A 累计已满20笔,不能继续累计");
                     ATBeepErr();
                     //RefreshSaleInfo();
                     return -1;
                 }
                 if(-1 == SaveFnSale())
                 {
                     return -1;
                 }
             }
             break;
        case KEY_ADD2:
             if(atoi(m_pSaleShareInfo->shareInfoSaleUi.totalprice) <= 0 && m_nSaleNumF2 == 0)
             {
                 m_pSaleShareInfo->ShowMessage("累计B 无总价或累计信息");
                 ATBeepErr();
                 return -1;
             }
             if(atoi(m_pSaleShareInfo->shareInfoSaleUi.totalprice) > 0)
             {
                 if (m_nSaleNumF2 >= BUSSINESS_SIZE)
                 {
                     m_pSaleShareInfo->ShowMessage("累计B 累计已满20笔,不能继续累计");
                     ATBeepErr();
                     //m_nSaleNumF2 = BUSSINESS_SIZE;
                     //RefreshSaleInfo();
                     return -1;
                 }
                 if(-1 == SaveFnSale())
                 {
                     return -1;
                 }
             }
             break;
        case KEY_ADD3:
             if(atoi(m_pSaleShareInfo->shareInfoSaleUi.totalprice) <= 0 && m_nSaleNumF3 == 0)
             {
                 m_pSaleShareInfo->ShowMessage("累计C 无总价或累计信息");
                 ATBeepErr();
                 return -1;
             }
             if(atoi(m_pSaleShareInfo->shareInfoSaleUi.totalprice) > 0)
             {
                 if (m_nSaleNumF3 >= BUSSINESS_SIZE)
                 {
                     m_pSaleShareInfo->ShowMessage("累计C 累计已满20笔,不能继续累计");
                     ATBeepErr();
                     //m_nSaleNumF3 = BUSSINESS_SIZE;
                     //RefreshSaleInfo();
                     return -1;
                 }
                 if(-1 == SaveFnSale())
                 {
                     return -1;
                 }
             }
             break;
        case KEY_ADD4:
             if(atoi(m_pSaleShareInfo->shareInfoSaleUi.totalprice) <= 0 && m_nSaleNumF4 == 0)
             {
                 m_pSaleShareInfo->ShowMessage("累计D 无总价或累计信息");
                 ATBeepErr();
                 return -1;
             }
             if(atoi(m_pSaleShareInfo->shareInfoSaleUi.totalprice) > 0)
             {
                 if (m_nSaleNumF4 >= BUSSINESS_SIZE)
                 {
                     m_pSaleShareInfo->ShowMessage("累计D 累计已满20笔,不能继续累计");
                     ATBeepErr();
                     //m_nSaleNumF3 = BUSSINESS_SIZE;
                     //RefreshSaleInfo();
                     return -1;
                 }
                 if(-1 == SaveFnSale())
                 {
                     return -1;
                 }
             }
             break;
        default:
             return -1;
    }

    int nResult = 0;
    printf("---------------------------m_nFn: F%d\n", m_nFn);
    switch (m_nFn)
    {
    case KEY_ADD1:
            SubtotalSaleFn();
            sprintf(m_pSaleShareInfo->shareInfoSaleUi.F1, "A%d",m_nSaleNumF1);
            break;
    case KEY_ADD2:
            SubtotalSaleFn();
            sprintf(m_pSaleShareInfo->shareInfoSaleUi.F2, "B%d",m_nSaleNumF2);
            break;
    case KEY_ADD3:
            SubtotalSaleFn();
            sprintf(m_pSaleShareInfo->shareInfoSaleUi.F3, "C%d",m_nSaleNumF3);
            break;
    case KEY_ADD4:
            SubtotalSaleFn();
            sprintf(m_pSaleShareInfo->shareInfoSaleUi.F4, "D%d",m_nSaleNumF4);
            break;
    default:
            return -1;

    }
    m_pSaleShareInfo->UpdateGui();
    if (atoi(m_pSaleShareInfo->shareInfoSaleUi.totalprice) <= 0)
    {
        m_pSaleShareInfo->m_ShareSaleInfo.PayMoney = m_nSaleTotal;
        m_pSaleShareInfo->m_ShareSaleInfo.SaleAmount = m_nSaleNum;
        m_pSaleShareInfo->m_ShareSaleInfo.SaleClosingTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        nResult = showAddSaleMsgProc(false);
        if(nResult != 5)//只要不是支付界面，就要刷新累计界面
        {
            m_pSaleShareInfo->m_enQtUiType = enQt_SaleQT;
            m_pSaleShareInfo->UpdateGui();
        }
        printf("1--------------------------nResult:%d--%d\n", m_nFn, nResult);
        return nResult;
    }

    m_bAccumulative = true;

    printf("AddUpSale-------m_bSelectPlu:%d-------------m_nTotal:%d--%d\n", m_bSelectPlu,m_nFn, m_nSaleTotal);
    m_pSaleShareInfo->m_ShareSaleInfo.PayMoney = m_nSaleTotal;
    m_pSaleShareInfo->m_ShareSaleInfo.SaleAmount = m_nSaleNum;
    m_pSaleShareInfo->m_ShareSaleInfo.SaleClosingTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    nResult = showAddSaleMsgProc(true);
    if(nResult != 5)//只要不是支付界面，就要刷新累计界面
    {
        m_pSaleShareInfo->m_enQtUiType = enQt_SaleQT;
        m_pSaleShareInfo->UpdateGui();
    }
    printf("2--------------------------nResult:%d--%d\n", m_nFn, nResult);
    return nResult;
}

int CMainSale::SaveFnSale(void)
{
    if ( m_nPrice<=0 || m_nTotal<=0 || m_nTotal >= 999999)
        return -1;

    if(m_pSaleShareInfo->shareInfoSaleUi.stable != 1 && !m_bAutoSave && m_pSaleShareInfo->GetSaleMode()==enWeight)
      {
          ATBeep();
          m_pSaleShareInfo->ShowMessage("报警:请稳定后按累计或打印");
          return -1;
      }
    if(m_pSaleShareInfo->GetSaleMode()==enWeight && atoi(m_pSaleShareInfo->shareInfoSaleUi.weight) < m_pSaleShareInfo->m_ShareSystemInfo.MinWeight && !m_bAutoSave)
      {
          ATBeep();
          m_pSaleShareInfo->ShowMessage("报警:称重请大于最小称量");
          return -1;
      }
    printf("---------------------------m_bSelectPlu:%d\n",m_bSelectPlu);
    if(false == m_bSelectPlu)
    {
        if(1 == m_pSaleShareInfo->m_ShareUserSwitchInfo.SavePluSwitch)
        {
            m_pSaleShareInfo->stb->goodsName = m_pSaleShareInfo->stp->goodsName;
            m_pSaleShareInfo->stb->goodsCode = m_pSaleShareInfo->stp->goodsCode;
        }
        else
        {
            m_pSaleShareInfo->stb->goodsName = QString("%1").arg(m_pSaleShareInfo->shareInfoSaleUi.plu);
            qDebug() << "---------------------------SaveFnSale:" << m_pSaleShareInfo->stb->goodsName;
            m_pSaleShareInfo->stb->goodsCode = "";
        }
    }
    else
    {
        m_pSaleShareInfo->stb->goodsName = m_pSaleShareInfo->stp->goodsName;
        m_pSaleShareInfo->stb->goodsCode = m_pSaleShareInfo->stp->goodsCode;
    }
    if((0 == m_pSaleShareInfo->m_ShareUserSwitchInfo.SavePluSwitch) )
    {
        m_bSelectPlu = false;
        m_pSaleShareInfo->stp->ClearAllData();
    }
     //取消选中的商品
    CurrentTime = QDateTime::currentDateTime();
    m_pSaleShareInfo->stb->time = CurrentTime.toString("yyyy-MM-dd hh:mm:ss");


    m_pSaleShareInfo->stb->businessY =  "0";
    m_pSaleShareInfo->stb->businessZ =  "0";
    if (m_pSaleShareInfo->GetSaleMode() == enPieces)
    {
        m_pSaleShareInfo->stb->pieces.sprintf("%d", m_nPieces/m_pSaleShareInfo->m_ShareSystemInfo.nWeightCoe);
        m_pSaleShareInfo->stb->weight = "0";
        m_pSaleShareInfo->stb->mode =  "1";
    }
    else
    {
        //称重
        m_pSaleShareInfo->stb->pieces = "0";
        if(m_pSaleShareInfo->m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT == 3)
        {
            m_pSaleShareInfo->stb->weight.sprintf("%d.%03d",m_nWeight/m_pSaleShareInfo->m_ShareSystemInfo.nWeightCoe, m_nWeight%m_pSaleShareInfo->m_ShareSystemInfo.nWeightCoe);
        }
        else if(m_pSaleShareInfo->m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT == 2)
        {
            m_pSaleShareInfo->stb->weight.sprintf("%d.%02d",(m_nWeight)/m_pSaleShareInfo->m_ShareSystemInfo.nWeightCoe, (m_nWeight)%m_pSaleShareInfo->m_ShareSystemInfo.nWeightCoe);
        }
        m_pSaleShareInfo->stb->mode = "0";
    }

    m_pSaleShareInfo->stb->price.sprintf("%d.%02d",m_nPrice/m_pSaleShareInfo->m_ShareSystemInfo.nPricesCoe, m_nPrice%m_pSaleShareInfo->m_ShareSystemInfo.nPricesCoe);
    m_pSaleShareInfo->stb->total.sprintf("%d.%02d",m_nTotal/m_pSaleShareInfo->m_ShareSystemInfo.nPricesCoe, m_nTotal%m_pSaleShareInfo->m_ShareSystemInfo.nPricesCoe);
    m_pSaleShareInfo->stb->batchCode = m_pSaleShareInfo->stp->batchCode;
    m_pSaleShareInfo->stb->traceCode = m_pSaleShareInfo->stp->traceCode;
    m_pSaleShareInfo->stb->webUp = "0";
    m_pSaleShareInfo->stb->windUp = "0";
    m_pSaleShareInfo->sta->isPrint = "0";

    switch (m_nFn)
    {
        case KEY_ADD1:
            stbF1[m_nSaleNumF1++] = m_pSaleShareInfo->stb[0];
            break;
        case KEY_ADD2:
            stbF2[m_nSaleNumF2++] = m_pSaleShareInfo->stb[0];
            break;
        case KEY_ADD3:
            stbF3[m_nSaleNumF3++] = m_pSaleShareInfo->stb[0];
            break;
        case KEY_ADD4:
            stbF4[m_nSaleNumF4++] = m_pSaleShareInfo->stb[0];
            break;
        default:
            //单笔打印保存
            break;
    }
    return 0;
}
int CMainSale::SubtotalSaleFn(void)
{
    int nTotalTemp = 0;
    long nSaleTotalTemp = 0;
    unsigned char nSaleNumTemp = 0;
    struct_tb_business *stbFn = NULL;
    switch (m_nFn)
    {
    case KEY_ADD1:
            stbFn = stbF1;
            nSaleNumTemp = m_nSaleNumF1;
            break;
    case KEY_ADD2:
            stbFn = stbF2;
            nSaleNumTemp = m_nSaleNumF2;
            break;
    case KEY_ADD3:
            stbFn = stbF3;
            nSaleNumTemp = m_nSaleNumF3;
            break;
    case KEY_ADD4:
            stbFn = stbF4;
            nSaleNumTemp = m_nSaleNumF4;
            break;
    default:
            stbFn = m_pSaleShareInfo->stb;
            nSaleNumTemp = 1;
            break;
    }
    for (int i=0; i<nSaleNumTemp; i++)
    {
        double fTotalTemp = stbFn[i].total.toDouble();
        nTotalTemp = (int)(fTotalTemp*100.0f+0.01);//防止出现精度问题
        printf("fTotalTemp:%f --- nTotalTemp:%d\n",fTotalTemp,nTotalTemp);
        nSaleTotalTemp += nTotalTemp;
    }
    m_nSaleTotal = nSaleTotalTemp;
    if(m_pSaleShareInfo->m_ShareDeviceInfo.BoothCode != "100" && m_pSaleShareInfo->m_ShareDeviceInfo.BoothCode.length() > 0)
    {
        bool ok;
        int discount = m_pSaleShareInfo->m_ShareDeviceInfo.BoothCode.toInt(&ok,10);
        m_nDiscountSaleTotal = m_nSaleTotal * discount / 100;
    }
    else
    {
        m_nDiscountSaleTotal = m_nSaleTotal;
    }

    m_nSaleNum = nSaleNumTemp;
    printf("----------------m_nSaleTotal-F[%d]:%d-%d\n", m_nFn, nSaleNumTemp, m_nSaleTotal);
    switch (m_nFn)
    {
    case KEY_ADD1:
            m_nSaleTotalF1 = nSaleTotalTemp;
            break;
    case KEY_ADD2:
            m_nSaleTotalF2 = nSaleTotalTemp;
            break;
    case KEY_ADD3:
            m_nSaleTotalF3 = nSaleTotalTemp;
            break;
    case KEY_ADD4:
            m_nSaleTotalF4 = nSaleTotalTemp;
            break;
    default:
            m_nSaleTotal = nSaleTotalTemp;
            break;
    }
    CurrentTime = QDateTime::currentDateTime();
    m_pSaleShareInfo->sta->scaleTime = CurrentTime.toString("yyyy-MM-dd hh:mm:ss");
    // 设备ID和时间戳
    m_pSaleShareInfo->m_ShareSaleInfo.SaleSerial = QString("%1%2").arg(m_pSaleShareInfo->m_ShareDeviceInfo.DeviceId).arg(CurrentTime.toString("yyMMddhhmmss"));
    m_pSaleShareInfo->sta->serial = m_pSaleShareInfo->m_ShareSaleInfo.SaleSerial;
    return 0;
}
int CMainSale::SaveSaleToDBFn(void)
{
    struct_tb_business *stbFn = NULL;
    unsigned char SaleNumFn = 0;
    unsigned char nSerialNum = 0;
    int busi_code = ++m_pSaleShareInfo->m_ShareSaleInfo.BusinessCode;
    switch (m_nFn)
    {
        case KEY_ADD1:
                stbFn = stbF1;
                SaleNumFn = m_nSaleNumF1;
                break;
        case KEY_ADD2:
                stbFn = stbF2;
                SaleNumFn = m_nSaleNumF2;
                break;
        case KEY_ADD3:
                stbFn = stbF3;
                SaleNumFn = m_nSaleNumF3;
                break;
        case KEY_ADD4:
                stbFn = stbF4;
                SaleNumFn = m_nSaleNumF4;
                break;
        default:
                //单笔交易保存
                stbFn = m_pSaleShareInfo->stb;
                SaleNumFn = 1;
                break;
    }

    for (int i=0; i<SaleNumFn; i++)
    {
        stbFn[i].payType.sprintf("%d", m_pSaleShareInfo->GetPayMode());
        stbFn[i].businessCode = busi_code;
        stbFn[i].serialNum = ++nSerialNum%255;
        //----------------------------------------减掉重量，如果重量超出，删除追溯码
        if(1 == m_pSaleShareInfo->m_ShareUserSwitchInfo.SavePriceSwitch)
        {
            double fPrice = stbFn[i].price.toDouble();
            int nTotalTemp = (int)(fPrice*100.0f +0.01);
            printf("fPrice:%f --- nTotalTemp:%d\n",fPrice,nTotalTemp);
            if(nTotalTemp >0 && stbFn[i].goodsCode.length()>0)
            {
                QString sqlStr = QString("update %1 set lastprice ='%2' where goodscode='%3';").arg(TABLE_PRODUCT).arg(nTotalTemp).arg(stbFn[i].goodsCode);
                if(ATdb_Excute_SQL_Statements(sqlStr)<=0)
                {
                    qDebug() << "update price fail";
                }
            }
        }
        qDebug() << stbFn[i].goodsCode << "---" << stbFn[i].goodsName << "---"<< stbFn[i].price <<"---" << stbFn[i].weight << "---" << stbFn[i].total;
    }
    ATdb_Insert_DataList_By_TableName_DataList(TABLE_BUSINESS,stbFn,SaleNumFn); 

    m_pSaleShareInfo->sta->saleNum = SaleNumFn;
    m_pSaleShareInfo->sta->businessCode = busi_code;
    m_pSaleShareInfo->sta->payType.sprintf("%d",m_pSaleShareInfo->GetPayMode());	
    m_pSaleShareInfo->sta->webUp = "0";
    m_pSaleShareInfo->sta->windUp = "0";
    m_pSaleShareInfo->sta->subTotal.sprintf("%d.%02d",m_nSaleTotal/m_pSaleShareInfo->m_ShareSystemInfo.nPricesCoe,
                                            m_nSaleTotal%m_pSaleShareInfo->m_ShareSystemInfo.nPricesCoe);
    m_pSaleShareInfo->sta->payMoney.sprintf("%d.%02d",m_nDiscountSaleTotal/m_pSaleShareInfo->m_ShareSystemInfo.nPricesCoe,
                                            m_nDiscountSaleTotal%m_pSaleShareInfo->m_ShareSystemInfo.nPricesCoe);

    m_pSaleShareInfo->sta->context= m_pSaleShareInfo->m_ShareDeviceInfo.BoothCode;

    ATdb_Insert_DataList_By_TableName_DataList(TABLE_ATTACH,m_pSaleShareInfo->sta,1);

    DeleteAllSale(m_nFn);
    m_pSaleShareInfo->m_WebserviceUpFlag = 9;
}

int CMainSale::AutoSaveSale(void)
{
    // 强制记录开启,并且拿走商品的重量小于等于商品稳定后的重量X强制记录阈值
    //printf("pInfoMainSale->stp->goodsName:%s\n", pInfoMainSale->stp->goodsName);
    // 根据稳定后的重量，计算总价
    m_nWeight = m_nLastWeight;
    m_nPrice = m_nLastPrice;
    m_nSaleTotal = m_nTotal = m_nLastTotal;

    m_nFn = 0;
    SaveFnSale();
    SubtotalSaleFn();

    if (m_pSaleShareInfo->m_ShareUserSwitchInfo.PrintSwitch == 1)
    {
        PrintSale();
    }
    SaveSaleToDBFn();
    m_bAutoSave = false;

    RefreshSaleInfo();

    return 0;
}
int CMainSale::DeleteAllSale(unsigned char Fn)
{
     switch (Fn)
     {
         case KEY_ADD1:
             for(int i=0; i<BUSSINESS_SIZE; i++)
             {
                 stbF1[i].ClearAllData();
             }
             m_nSaleNumF1 = 0;
             m_nSaleTotalF1 = 0;
             sprintf(m_pSaleShareInfo->shareInfoSaleUi.F1, "%s","");
             break;
         case KEY_ADD2:
             for(int i=0; i<BUSSINESS_SIZE; i++)
             {
                 stbF2[i].ClearAllData();
             }
             m_nSaleNumF2 = 0;
             m_nSaleTotalF2 = 0;
             sprintf(m_pSaleShareInfo->shareInfoSaleUi.F2, "%s","");
             break;
         case KEY_ADD3:
             for(int i=0; i<BUSSINESS_SIZE; i++)
             {
                 stbF3[i].ClearAllData();
             }
             m_nSaleNumF3 = 0;
             m_nSaleTotalF3 = 0;
             sprintf(m_pSaleShareInfo->shareInfoSaleUi.F3, "%s","");
             break;
         case KEY_ADD4:
             for(int i=0; i<BUSSINESS_SIZE; i++)
             {
                 stbF4[i].ClearAllData();
             }
             m_nSaleNumF4 = 0;
             m_nSaleTotalF4 = 0;
             sprintf(m_pSaleShareInfo->shareInfoSaleUi.F4, "%s","");
             break;
         default:
             break;
     }
     m_pSaleShareInfo->stb->ClearAllData();
     m_nSaleTotal = 0;
     m_nSaleNum = 0;
     Fn = 0;
}

int CMainSale::CancelSale()
{
    m_nTotal = 0;//无商品不显示总价
    m_nPrice = 0;

    m_nLastPrice = 0;
    m_nLastTotal = 0;

    m_nSaleTotal = 0;
    m_nSaleNum = 0;
    m_nFn = 0;
    m_bAccumulative = false;
    m_pSaleShareInfo->m_ShareSaleInfo.PayMoney = 0;
    m_pSaleShareInfo->m_ShareSaleInfo.SaleAmount = 0;
    m_pSaleShareInfo->m_ShareSaleInfo.SaleStatus = enPayComplete;
    if((0 == m_pSaleShareInfo->m_ShareUserSwitchInfo.SavePluSwitch))
    {
        memset(m_pSaleShareInfo->shareInfoSaleUi.unitprice, 0x00, sizeof(m_pSaleShareInfo->shareInfoSaleUi.unitprice));
        memset(m_pSaleShareInfo->shareInfoSaleUi.totalprice, 0x00, sizeof(m_pSaleShareInfo->shareInfoSaleUi.totalprice));
        sprintf(m_pSaleShareInfo->shareInfoSaleUi.plu,"%s", "自由商品");
    }
    if (m_pSaleShareInfo->GetSaleMode() != enWeight)
    {
            sprintf(m_pSaleShareInfo->shareInfoSaleUi.weight, "%d", m_pSaleShareInfo->m_adInfo.kg);
    }
    ChangeSaleMode(m_pSaleShareInfo, enWeight);
    m_pSaleShareInfo->SetPayMode(enCash);

    ClearEditNum();
    m_bUpdate = true;
}

int CMainSale::RefreshSaleInfo()
{
    CancelSale();
    if((0 == m_pSaleShareInfo->m_ShareUserSwitchInfo.SavePluSwitch) )
    {
        m_bSelectPlu = false;
        m_pSaleShareInfo->stp->ClearAllData();
        m_pSaleShareInfo->ShowMessage("");
    }
}
int CMainSale::PrintSale(void)
{
    printf("PrintSale Start ===================\n");
    stReceiptData data;
    int nPrintCnt = m_pSaleShareInfo->m_ShareSysSwitchInfo.PrintPaper;
    CurrentTime = QDateTime::currentDateTime();

    char str[64] = {0};
    char str1[64] = {0};
    for(int i = 0; i < nPrintCnt; i++)
    {
        //小票头
        int nMarketNameLen = m_pSaleShareInfo->m_ShareDeviceInfo.HeadTail6.length();
        if (nMarketNameLen > 16)
        {
            data.type = enString;
        }
        else if(nMarketNameLen > 8){

            data.type = enBigString;
        }
        else {
            data.type = enBigString;
        }
        sprintf(str1,"%s",m_pSaleShareInfo->m_ShareDeviceInfo.HeadTail6.toStdString().c_str());
        ATLib_u2g(str1,strlen(str1),str,64);
        PrintBigCenter(str, data.str);
        m_pSaleShareInfo->m_pPrint->add(data);

        data.type = enString;
        //sprintf(str,"%s", "吉林市东北亚\"菜篮子\"农贸市场");
        sprintf(str,"市场名称:%s",m_pSaleShareInfo->m_ShareDeviceInfo.MarketName.toStdString().c_str());
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleShareInfo->m_pPrint->add(data);

        data.type = enString;
        sprintf(data.str,"%s","--------------------------------");
        m_pSaleShareInfo->m_pPrint->add(data);

        sprintf(str,"摊位号: %s ", m_pSaleShareInfo->m_ShareDeviceInfo.BoothId.toStdString().c_str());
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleShareInfo->m_pPrint->add(data);

        sprintf(str,"商  户: %s ", m_pSaleShareInfo->m_ShareDeviceInfo.BoothName.toStdString().c_str());
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleShareInfo->m_pPrint->add(data);

        sprintf(str,"交易时间: %s ", CurrentTime.toString("yyyy-MM-dd hh:mm:ss ").toStdString().c_str());
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleShareInfo->m_pPrint->add(data);

        sprintf(str,"订单编号: %s ", m_pSaleShareInfo->sta->serial.toStdString().c_str());
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleShareInfo->m_pPrint->add(data);

        // 每行最大长度为32
        sprintf(data.str,"%s","================================");
        m_pSaleShareInfo->m_pPrint->add(data);

        sprintf(str, "%s", "名称 重量/件数  单价/元 金额/元");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleShareInfo->m_pPrint->add(data);

        sprintf(data.str,"%s","================================");
        m_pSaleShareInfo->m_pPrint->add(data);

        PrintSaleForFn(data);
        m_pSaleShareInfo->sta->isPrint = "1";

        data.type = enString;
        // 支付方式
        if(m_pSaleShareInfo->m_enPayMode == enUnionPay)
        {
            sprintf(str,"%s:  [%s]", "支付方式", "银联卡支付");
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pSaleShareInfo->m_pPrint->add(data);
        }
        else if(m_pSaleShareInfo->m_enPayMode == enAliPay)
        {
            sprintf(str,"%s:  [%s]", "支付方式", "支付宝支付");
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pSaleShareInfo->m_pPrint->add(data);
        }
        else if(m_pSaleShareInfo->m_enPayMode == enWeChatPay)
        {
            sprintf(str,"%s:  [%s]", "支付方式", "微信支付");
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pSaleShareInfo->m_pPrint->add(data);
        }
        else if(m_pSaleShareInfo->m_enPayMode == enBankPay)
        {
            sprintf(str,"%s:  [%s]", "支付方式", "扫码支付");
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pSaleShareInfo->m_pPrint->add(data);
            sprintf(str,"%s:  [%s]", "商户编号", m_pSaleShareInfo->m_ShareCustomerInfo.OtherID.toStdString().c_str());
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pSaleShareInfo->m_pPrint->add(data);
            sprintf(str,"%s:  [%s]", "支付订单", m_pSaleShareInfo->m_ShareSaleInfo.SaleTradeNo.toStdString().c_str());
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pSaleShareInfo->m_pPrint->add(data);
        }
        else if(m_pSaleShareInfo->m_enPayMode == enVipPay)
        {
            sprintf(str,"%s:  [%s]", "支付方式", "会员卡支付");
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pSaleShareInfo->m_pPrint->add(data);
        }
        else
        {
            sprintf(str,"%s:  [%s]", "支付方式", "现金支付");
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pSaleShareInfo->m_pPrint->add(data);
        }

        data.type = enLine;
        m_pSaleShareInfo->m_pPrint->add(data);

        if(m_pSaleShareInfo->m_ShareDeviceInfo.HeadTail5.length()>0)
        {
            data.type = enQR;
            sprintf(data.str,"%s%s", m_pSaleShareInfo->m_ShareDeviceInfo.HeadTail5.toStdString().c_str(),m_pSaleShareInfo->sta->serial.toStdString().c_str());
            m_pSaleShareInfo->m_pPrint->add(data);
        }
        if(m_pSaleShareInfo->m_ShareDeviceInfo.HeadTail1.length()>0)
        {
            data.type = enCenterString;
            sprintf(str,"%s", m_pSaleShareInfo->m_ShareDeviceInfo.HeadTail1.toStdString().c_str());
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pSaleShareInfo->m_pPrint->add(data);
        }
        if(m_pSaleShareInfo->m_ShareDeviceInfo.HeadTail2.length()>0)
        {
            data.type = enCenterString;
            sprintf(str,"%s", m_pSaleShareInfo->m_ShareDeviceInfo.HeadTail2.toStdString().c_str());
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pSaleShareInfo->m_pPrint->add(data);
        }
        if(m_pSaleShareInfo->m_ShareDeviceInfo.HeadTail3.length()>0)
        {
            data.type = enCenterString;
            sprintf(str,"%s", m_pSaleShareInfo->m_ShareDeviceInfo.HeadTail3.toStdString().c_str());
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pSaleShareInfo->m_pPrint->add(data);
        }
        data.type = enStartPrint;
        m_pSaleShareInfo->m_pPrint->add(data);
        if (nPrintCnt > 1)
        {
            // 打印签购单，停顿5S方便撕票
            usleep(3000*1000);
        }
    }
    printf("print complete ===================\n");
}
int CMainSale::PrintSaleForFn(stReceiptData data)
{
    char pStrOut[64] = {0};
    struct_tb_business *stbFn = NULL;
    unsigned char nSaleNumTemp = 0;
    switch(m_nFn)
    {
        case KEY_ADD1:
                nSaleNumTemp = m_nSaleNumF1;
                stbFn = stbF1;
                break;
        case KEY_ADD2:
                nSaleNumTemp = m_nSaleNumF2;
                stbFn = stbF2;
                break;
        case KEY_ADD3:
                nSaleNumTemp = m_nSaleNumF3;
                stbFn = stbF3;
                break;
        case KEY_ADD4:
                nSaleNumTemp = m_nSaleNumF4;
                stbFn = stbF4;
                break;
        default:
                nSaleNumTemp = m_nSaleNum;
                stbFn = m_pSaleShareInfo->stb;
                break;
    }
    for (int i=0; i<nSaleNumTemp; i++)
    {
        data.type = enString;
        if (stbFn[i].mode == "1")
        {
            QString goodsName = stbFn[i].goodsName;
            goodsName.append("(计件Pcs)");
            ATLib_u2g((char *)goodsName.toStdString().c_str(),strlen(goodsName.toStdString().c_str()),data.str,64);
            m_pSaleShareInfo->m_pPrint->add(data);
            sprintf(data.str, "%04s%08s %09s%08s", " ", stbFn[i].pieces.toStdString().c_str(), stbFn[i].price.toStdString().c_str(), stbFn[i].total.toStdString().c_str());
            m_pSaleShareInfo->m_pPrint->add(data);
        }
        else
        {
            QString goodsName = stbFn[i].goodsName;
            goodsName.append("(计重Kg)");
            ATLib_u2g((char *)goodsName.toStdString().c_str(),strlen(goodsName.toStdString().c_str()),data.str,64);
            m_pSaleShareInfo->m_pPrint->add(data);
            sprintf(data.str, "%04s%08s %09s%08s", " ", stbFn[i].weight.toStdString().c_str(), stbFn[i].price.toStdString().c_str(), stbFn[i].total.toStdString().c_str());
            m_pSaleShareInfo->m_pPrint->add(data);
        }
    }
    data.type = enLine;
    m_pSaleShareInfo->m_pPrint->add(data);

    data.type = enString;
    sprintf(pStrOut,"%s:  %d笔      %d.%02d元", "  合  计", nSaleNumTemp,  m_nSaleTotal/100,  m_nSaleTotal%100);
    ATLib_u2g(pStrOut,strlen(pStrOut),data.str,64);
    m_pSaleShareInfo->m_pPrint->add(data);

    if(m_pSaleShareInfo->m_ShareDeviceInfo.BoothCode != "100" && m_pSaleShareInfo->m_ShareDeviceInfo.BoothCode.length() > 0)
    {
        data.type = enString;
        sprintf(pStrOut,"%s:  %s折", "  优  惠", m_pSaleShareInfo->m_ShareDeviceInfo.BoothCode.toStdString().c_str());
        ATLib_u2g(pStrOut,strlen(pStrOut),data.str,64);
        m_pSaleShareInfo->m_pPrint->add(data);
    }
    else
    {
        data.type = enString;
        sprintf(pStrOut,"%s:  无", "  优  惠");
        ATLib_u2g(pStrOut,strlen(pStrOut),data.str,64);
        m_pSaleShareInfo->m_pPrint->add(data);
    }
    data.type = enString;
    sprintf(pStrOut,"%s:  %d.%02d元", "实收金额", m_nDiscountSaleTotal/100,  m_nDiscountSaleTotal%100);
    ATLib_u2g(pStrOut,strlen(pStrOut),data.str,64);
    m_pSaleShareInfo->m_pPrint->add(data);
}

int CMainSale::FindFunctionKey(void)
{
    printf("---------------------m_nFunctionKey:%d\n",m_nFunctionKey);
    switch(m_nFunctionKey)
    {
        case 9990:
            m_pSaleShareInfo->ShowMessage("正在打印出厂测试信息");
            m_pSaleShareInfo->UpdateGui();
            sleep(1);
            m_pSaleFunction->PrintDeviceInfo();
            m_pSaleShareInfo->ShowMessage("打印出厂信息测试完成");
            break;
        case 9991:
            m_pSaleShareInfo->ShowMessage("导出信息到U盘");
            m_pSaleShareInfo->UpdateGui();
            sleep(1);
            if (m_pSaleFunction->ExportFile() < 0)
            {
                m_pSaleShareInfo->ShowMessage("导出信息失败:U盘未识别");
                break;
            }
            m_pSaleShareInfo->ShowMessage("导出信息成功");
            break;

        case 9992:
            m_pSaleShareInfo->ShowMessage("执行U盘sql.txt文本SQL语句");
            m_pSaleShareInfo->UpdateGui();
            sleep(1);
            if (m_pSaleFunction->ExcutCustomSqls() < 0)
            {
                m_pSaleShareInfo->ShowMessage("执行SQL失败:U盘未识别");
                break;
            }
            m_pSaleShareInfo->ShowMessage("执行SQL成功");
            break;

        case 9993:
            m_pSaleShareInfo->ShowMessage("执行U盘shell脚本");
            m_pSaleShareInfo->UpdateGui();
            sleep(1);
            if (m_pSaleFunction->ExcutScript() < 0)
            {
                sleep(1);
                m_pSaleShareInfo->ShowMessage("执行shell失败:U盘未识别");
                break;
            }
            m_pSaleShareInfo->ShowMessage("执行shell成功");
            break;
        case 9994:
            m_pSaleShareInfo->ShowMessage("正在网关网络测试");
            m_pSaleShareInfo->UpdateGui();
            if (m_pSaleFunction->TestNetwork() < 0)
            {
                m_pSaleShareInfo->ShowMessage("网关网络测试:网络异常");
                break;
            }
            m_pSaleShareInfo->ShowMessage("网关网络测试:网络正常");
            break;
        case 9995:
            m_pSaleShareInfo->ShowMessage("手动获取用户信息");
            m_pSaleShareInfo->UpdateGui();
            sleep(1);
            m_pSaleShareInfo->m_WebserviceUpFlag = 1;
            break;
        case 9996:
            m_pSaleShareInfo->ShowMessage("手动获取商品信息");
            m_pSaleShareInfo->UpdateGui();
            sleep(1);
            m_pSaleShareInfo->m_WebserviceUpFlag = 2;
            break;
        case 9997:
            m_pSaleShareInfo->ShowMessage("重启一次网络");
            m_pSaleShareInfo->StartNetwork();
            sleep(1);
            break;
        case 9998:
//            m_pSaleShareInfo->ShowMessage("重启应用程序");
//            m_pSaleShareInfo->UpdateGui();
//            sleep(1);
//            system("./ct100.sh");
            break;
        case 9999:
            printf("----------------测试读卡\n");
            if(showMsg(enMsgRfidTest, "读卡器测试","正在请求读卡测试...")>0)
            {
                m_pSaleShareInfo->ShowMessage("读卡器测试:寻卡成功");
            }
            else
            {
                m_pSaleShareInfo->ShowMessage("读卡器测试:取消寻卡");
            }
            break;
        default:break;
    }
    sleep(2);
    m_pSaleShareInfo->UpdateGui();
    m_pSaleShareInfo->ShowMessage("");
}

int CMainSale::showAddSaleMsg(struct_tb_business *stbFn)
{
    switch (m_nFn)
    {
    case KEY_ADD1:
            sprintf(m_pSaleShareInfo->m_stAddSaleInfo.title,"A%d 累计共%d笔,当前笔:第%d笔",m_nSaleNumF1,m_nSaleNumF1,m_nCurrentSaleNum+1);
            break;
    case KEY_ADD2:
            sprintf(m_pSaleShareInfo->m_stAddSaleInfo.title,"B%d 累计共%d笔,当前笔:第%d笔",m_nSaleNumF2,m_nSaleNumF2,m_nCurrentSaleNum+1);
            break;
    case KEY_ADD3:
            sprintf(m_pSaleShareInfo->m_stAddSaleInfo.title,"C%d 累计共%d笔,当前笔:第%d笔",m_nSaleNumF3,m_nSaleNumF3,m_nCurrentSaleNum+1);
            break;
    case KEY_ADD4:
            sprintf(m_pSaleShareInfo->m_stAddSaleInfo.title,"D%d 累计共%d笔,当前笔:第%d笔",m_nSaleNumF4,m_nSaleNumF4,m_nCurrentSaleNum+1);
            break;
    default:
            sprintf(m_pSaleShareInfo->m_stAddSaleInfo.title,"%s","单笔交易结算/打印提示");
            break;
    }

    sprintf(m_pSaleShareInfo->m_stAddSaleInfo.name,"%s",stbFn[m_nCurrentSaleNum].goodsName.toStdString().c_str());
    sprintf(m_pSaleShareInfo->m_stAddSaleInfo.total,"%s元",stbFn[m_nCurrentSaleNum].total.toStdString().c_str());
    sprintf(m_pSaleShareInfo->m_stAddSaleInfo.price,"%s元",stbFn[m_nCurrentSaleNum].price.toStdString().c_str());
    if(stbFn[m_nCurrentSaleNum].mode == "1")
    {
        sprintf(m_pSaleShareInfo->m_stAddSaleInfo.weight,"%sPcs",stbFn[m_nCurrentSaleNum].pieces.toStdString().c_str());  
    }
    else
    {
        sprintf(m_pSaleShareInfo->m_stAddSaleInfo.weight,"%sKg",stbFn[m_nCurrentSaleNum].weight.toStdString().c_str());        
    }
    if(m_pSaleShareInfo->m_ShareDeviceInfo.BoothCode == "100" || m_pSaleShareInfo->m_ShareDeviceInfo.BoothCode.length() <= 0)
    {
        sprintf(m_pSaleShareInfo->m_stAddSaleInfo.msg,"应收:%ld.%02ld元,实收:%d.%02d元",m_nSaleTotal/100,m_nSaleTotal%100,m_nSaleTotal/100,m_nSaleTotal%100);
        sprintf(m_pSaleShareInfo->m_stAddSaleInfo.discount,"%s","优惠：无");
    }
    else
    {
        sprintf(m_pSaleShareInfo->m_stAddSaleInfo.msg,"应收:%ld.%02ld元,折扣后实收:%d.%02d元",m_nSaleTotal/100,m_nSaleTotal%100,m_nDiscountSaleTotal/100,m_nDiscountSaleTotal%100);
        sprintf(m_pSaleShareInfo->m_stAddSaleInfo.discount,"优惠：%s折",m_pSaleShareInfo->m_ShareDeviceInfo.BoothCode.toStdString().c_str());
    }
    m_pSaleShareInfo->UpdateGui();
}

int CMainSale::showAddSaleMsgProc(bool isWeight)
{
    m_pSaleShareInfo->m_enQtUiType = enQt_AddSaleQT;
    m_pSaleShareInfo->m_stAddSaleInfo.cmd = MSG_MESSAGE_REQ;//请求还是确认消息
    struct_tb_business *stbFn = NULL;
    switch (m_nFn)
    {
    case KEY_ADD1:
            stbFn = stbF1;
            m_nCurrentSaleNum = m_nSaleNumF1 - 1;
            break;
    case KEY_ADD2:
            stbFn = stbF2;
            m_nCurrentSaleNum = m_nSaleNumF2 - 1;
            break;
    case KEY_ADD3:
            stbFn = stbF3;
            m_nCurrentSaleNum = m_nSaleNumF3 - 1;
            break;
    case KEY_ADD4:
            stbFn = stbF4;
            m_nCurrentSaleNum = m_nSaleNumF4 - 1;
            break;
    default:
            stbFn = m_pSaleShareInfo->stb;
            m_nCurrentSaleNum = 0;
            m_nSaleNum = 1;
            break;
    }
    showAddSaleMsg(stbFn);

    unsigned char ch;
    while(true)
    {
        usleep(50*1000);

        if (SALE_KEY_VALID == ATKey_getKey(&ch))
        {
            switch(ch)
            {
                case _KEY_CANCEL:
                    m_pSaleShareInfo->m_stAddSaleInfo.cmd = MSG_MESSAGE_OK;
                    m_pSaleShareInfo->UpdateGui();
                    return 0;
                case _KEY_CLR:
                    DeleteAllSale(m_nFn);
                    RefreshSaleInfo();
                    m_pSaleShareInfo->m_stAddSaleInfo.cmd = MSG_MESSAGE_OK;
                    m_pSaleShareInfo->UpdateGui();
                    return -10;
//                case _KEY_PAY_MODE:
//                    //m_pSaleShareInfo->m_stAddSaleInfo.cmd = MSG_MESSAGE_OK;
//                    //m_pSaleShareInfo->UpdateGui();
//                    return 5;
//                case _KEY_ENTER:
//                    m_pSaleShareInfo->m_stAddSaleInfo.cmd = MSG_MESSAGE_OK;
//                    m_pSaleShareInfo->UpdateGui();
//                    return 6;
                case _KEY_PRINT:
                    m_pSaleShareInfo->m_stAddSaleInfo.cmd = MSG_MESSAGE_OK;
                    m_pSaleShareInfo->UpdateGui();
                    return 10;
                case _KEY_WEIGHT_MODE://删除单笔累计
                    if(1 == m_nSaleNum)
                    {
                        DeleteAllSale(m_nFn);
                        RefreshSaleInfo();
                        m_pSaleShareInfo->m_stAddSaleInfo.cmd = MSG_MESSAGE_OK;
                        m_pSaleShareInfo->UpdateGui();
                        return -10;
                    }
                    else if(m_nSaleNum>1)
                    {
                        for(int i=m_nCurrentSaleNum; i< m_nSaleNum-1; i++)
                        {
                            stbFn[i].payType = stbFn[i+1].payType;
                            stbFn[i].goodsCode = stbFn[i+1].goodsCode;
                            stbFn[i].goodsName = stbFn[i+1].goodsName;
                            stbFn[i].goodsOrigin = stbFn[i+1].goodsOrigin;
                            stbFn[i].batchCode = stbFn[i+1].batchCode;
                            stbFn[i].traceCode = stbFn[i+1].traceCode;
                            stbFn[i].weight = stbFn[i+1].weight;
                            stbFn[i].price = stbFn[i+1].price;
                            stbFn[i].pieces = stbFn[i+1].pieces;
                            stbFn[i].total = stbFn[i+1].total;
                            stbFn[i].time = stbFn[i+1].time;
                            stbFn[i].mode = stbFn[i+1].mode;
                            stbFn[i].cancel = stbFn[i+1].cancel;
                            stbFn[i].picture = stbFn[i+1].picture;
                            stbFn[i].allWeight = stbFn[i+1].allWeight;
                            stbFn[i].isOverSale = stbFn[i+1].isOverSale;
                            stbFn[i].webUp = stbFn[i+1].webUp;
                            stbFn[i].windUp = stbFn[i+1].windUp;
                            stbFn[i].businessY = stbFn[i+1].businessY;
                            stbFn[i].businessZ = stbFn[i+1].businessZ;
                        }
                        stbFn[m_nSaleNum-1].ClearAllData();
                        switch (m_nFn)
                        {
                        case KEY_ADD1:
                                m_nSaleNumF1 = m_nSaleNumF1 -1;
                                m_nSaleNum = m_nSaleNumF1;
                                break;
                        case KEY_ADD2:
                                m_nSaleNumF2 = m_nSaleNumF2 -1;
                                m_nSaleNum = m_nSaleNumF2;
                                break;
                        case KEY_ADD3:
                                m_nSaleNumF3 = m_nSaleNumF3 -1;
                                m_nSaleNum = m_nSaleNumF3;
                                break;
                        case KEY_ADD4:
                                m_nSaleNumF4 = m_nSaleNumF4 -1;
                                m_nSaleNum = m_nSaleNumF4;
                                break;
                        default:
                                break;
                        }
                        if(0 == m_nCurrentSaleNum)
                        {
                            m_nCurrentSaleNum = m_nSaleNum;
                        }
                        m_nCurrentSaleNum--;
                        SubtotalSaleFn();
                        showAddSaleMsg(stbFn);
                    }
                    break;
                case _KEY_ADD:
                    if(1 >= m_nSaleNum)break;
                    if(0 == m_nCurrentSaleNum)
                    {
                        m_nCurrentSaleNum = m_nSaleNum;
                    }
                    m_nCurrentSaleNum--;
                    showAddSaleMsg(stbFn);
                    break;
                default:
                        break;
            }
        }
        if(isWeight)
        {
            if(m_pSaleShareInfo->GetSaleMode() == enPieces)
            {
                continue;
            }
            if ((m_pSaleShareInfo->m_adInfo.kg>=0 && m_pSaleShareInfo->m_adInfo.kg<=10)
                    || (m_pSaleShareInfo->m_adInfo.kg < 0))
            {
                // 重量变化后
                printf("--------------------------->Fx zero\n");
                m_pSaleShareInfo->m_stAddSaleInfo.cmd = MSG_MESSAGE_OK;
                m_pSaleShareInfo->UpdateGui();
                return 1;
            }
        }
    }
}
