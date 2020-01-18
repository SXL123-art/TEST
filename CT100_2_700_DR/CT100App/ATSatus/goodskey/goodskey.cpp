#include "goodskey.h"

CGoodsKey::CGoodsKey(CShareInfo *pInfo)
{
    m_pGoodsKeyShareInfo = pInfo;
    m_bUpdate = false;
    printf("-------------------------------------------init CGoodsKey\n");
}

CGoodsKey::~CGoodsKey(void)
{

}

void CGoodsKey::initGoodsKey(void)
{
    goodsOrKey = false;

    QString returnStr;
    QString sqlStr;

    for(int i=1 ; i<=60; i++)
    {
        sqlStr.sprintf("select goodsCode from table_convert where hotKey='%d';",i);
        ATdb_Select_ReturnQString_By_SQL_Statements(sqlStr,&returnStr);
        qDebug() << "goodscode" << returnStr;

        if(returnStr.length()>0)
        {
            sqlStr = QString("select goodsName from table_product where goodsCode='%1';").arg(returnStr);
            returnStr.clear();
            ATdb_Select_ReturnQString_By_SQL_Statements(sqlStr,&returnStr);
            qDebug() << "goodsName" << returnStr;
            if(returnStr.length()>0)
            {
                m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.cmd = goodsOrKey;
                m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.index = i;
                sprintf(m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.goodsName,"%s",returnStr.toStdString().c_str());
                m_pGoodsKeyShareInfo->UpdateGui();
            }
        }
    }
    m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.index = 0;
    sprintf(m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.goodsName,"%s","");
}

enQtShowType CGoodsKey::GoodsKeyProc()
{
    printf("-------------------------------------------CGoodsKey\n");
    m_pGoodsKeyShareInfo->m_enQtUiType = enQt_GoodsKeyQT;
    goodsCode = "";
    m_pGoodsKeyShareInfo->UpdateGui();

    initGoodsKey();

    while(true)
    {
        usleep(100*1000);

        switch (GoodsKeyDeal())
        {
        case enQt_GoodsKeyQT:
                break ;

        case enQt_SaleQT:
                return enQt_SaleQT;
        }
        updateGoodsKeyShow();
    }
}

enQtShowType CGoodsKey::GoodsKeyDeal()
{
    unsigned char ch = 0;
    usleep(100*1000);
    if (SALE_KEY_VALID == ATKey_getKey(&ch))
    {
        ProcessEditStr(ch);
        switch(ch)
        {
            case _KEY_MENU:
                return enQt_SaleQT;

                //初始化按键设备
            #if (KEY_TYPE == MIDDLE_KEY)
                    case _KEY_ENTER://确定
            #else
                    case _KEY_TOTAL://确定  
            #endif
                Sure();
                m_bUpdate = true;
                break; 
            case _KEY_CANCEL:
            {
                 m_bUpdate = true;
                 goodsOrKey = false;
                 goodsCode.clear();
                 goodsKey.clear();
                 break;
            }
            case _KEY_PRINT:
            {
                print();
                break;
            }
            default:break;
        }
    }
    return enQt_GoodsKeyQT;
}


void CGoodsKey::updateGoodsKeyShow(void)
{
    if(m_bUpdate == true)
    {
        if(false == goodsOrKey)
        {
            goodsKey.clear();
            sprintf(m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.goodsCode,"%s",goodsCode.toStdString().c_str());
        }
        sprintf(m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.goodsKey,"%s",goodsKey.toStdString().c_str());
        m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.cmd = goodsOrKey;
        m_pGoodsKeyShareInfo->UpdateGui();
        sprintf(m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.message,"%s","");
    }
    m_bUpdate = false;
}

int CGoodsKey::Sure(void)
{
    if(false == goodsOrKey)
    {
        if(goodsCode.length()<=0)
        {
            return 0;
        }
        QString sqlStr = QString("select goodsName from table_product where goodsCode='%1';").arg(goodsCode);
        QString returnStr;
        ATdb_Select_ReturnQString_By_SQL_Statements(sqlStr,&returnStr);
        qDebug() << "goodsName " << returnStr;
        if(returnStr.length()>0)
        {
            m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.cmd = goodsOrKey;
            m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.index = 0;
            sprintf(m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.goodsName,"%s",returnStr.toStdString().c_str());
            sprintf(m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.message,"已找到商品:%s,请选择商品键",returnStr.toStdString().c_str());
            goodsOrKey = true;
        }
        else
        {
            m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.cmd = goodsOrKey;
            m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.index = 0;
            sprintf(m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.message,"编码%s无商品",goodsCode.toStdString().c_str()); 
            goodsCode.clear();
            sprintf(m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.goodsCode,"%s",goodsCode.toStdString().c_str());
        }
    }
    else if(true == goodsOrKey)
    {
        if(goodsCode.length()<=0 || goodsKey.length()<=0)
        {
            sprintf(m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.message,"%s","无商品编码或者无按键");
            return 0;
        }
        QString sqlStr = QString("select id from table_convert where hotKey='%1';").arg(goodsKey);
        QString returnStr;
        ATdb_Select_ReturnQString_By_SQL_Statements(sqlStr,&returnStr);
        qDebug() << "old goodsCode id " << returnStr;
        if(returnStr.length()>0)
        {
            sqlStr = QString("update table_convert set goodsCode='%1' where id='%2';").arg(goodsCode).arg(returnStr);
            m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.cmd = goodsOrKey;
            bool ok;
            m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.index = goodsKey.toInt(&ok,10);
            ATdb_Excute_SQL_Statements(sqlStr);
            sprintf(m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.message,"%s","已完成对应,请继续或者退出");
        }
        else
        {
            struct_tb_convert *tb_con = new struct_tb_convert[1];
            tb_con[0].goodsCode = goodsCode;
            tb_con[0].hotKey = goodsKey;
            m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.cmd = goodsOrKey;
            bool ok;
            m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.index = goodsKey.toInt(&ok,10);
            ATdb_Insert_DataList_By_TableName_DataList(TABLE_CONVERT,tb_con,1);
            sprintf(m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.message,"%s","已完成对应,请继续或者退出");
            delete [] tb_con;

        }
        goodsOrKey = false;
        goodsCode.clear();
        goodsKey.clear();
    }
}

int CGoodsKey::print(void)
{
    stReceiptData data;

    char str[64] = {0};

    data.type = enCenterString;
    sprintf(str,"%s", "商品-编码列表");
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pGoodsKeyShareInfo->m_pPrint->add(data);

    sprintf(data.str,"%s","================================");
    m_pGoodsKeyShareInfo->m_pPrint->add(data);

    sprintf(str, "%s", "序号  商品名称    商品代码");
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pGoodsKeyShareInfo->m_pPrint->add(data);

    sprintf(data.str,"%s","================================");
    m_pGoodsKeyShareInfo->m_pPrint->add(data);

    QSqlQuery query;
    int dbSize = 0;
    if((dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements("select goodsName,goodsCode from TABLE_PRODUCT;",&query))  > 0)
    {
        printf("-------------------------dbSize:%d\n",dbSize);
        for(int i=0 ; i<dbSize; i++)
        {
            query.next();
            sprintf(str, "%-04d%-16s%-12s",i+1,query.record().value(TABLE_PRODUCT_GOODSNAME).toString().toStdString().c_str(),
                    query.record().value(TABLE_PRODUCT_GOODSCODE).toString().toStdString().c_str());
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pGoodsKeyShareInfo->m_pPrint->add(data);
        }
    }
    else
    {
        sprintf(str, "%s", "电子秤无商品列表,请下发");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pGoodsKeyShareInfo->m_pPrint->add(data);
    }

    data.type = enStartPrint;
    m_pGoodsKeyShareInfo->m_pPrint->add(data);
}
void CGoodsKey::ProcessEditStr(unsigned char ch)
{
    char cha;
    if (((ch >= _KEY_PLU1) && (ch <= _KEY_PLU60))
        || (ch >= _KEY_NUM0) && (ch <= _KEY_NUM9)
        || (ch == _KEY_BACK)
        || (ch == _KEY_CLR))
    {
        m_bUpdate = true;
        if(ch == _KEY_BACK)
        {
            if(goodsOrKey == false)
            {
                if(goodsCode.length()>0)
                {
                    goodsCode = goodsCode.left(goodsCode.length() - 1);
                }
            }
            else
            {
                if(goodsKey.length()>0)
                {
                    goodsKey = goodsKey.left(goodsKey.length() - 1);
                }
            }
        }
        else if(ch == _KEY_CLR)
        {
            if(goodsOrKey == false)
            {
                goodsCode.clear();
            }
            else
            {
                goodsKey.clear();
            }
        }
        else
        {
             if(goodsOrKey == false)
             {
                 if(0 < (cha = m_pGoodsKeyShareInfo->KeyValueConversion(ch)))
                 {
                     printf("----------input :%c\n",cha);
                     goodsCode.append(cha);
                 }
             }
             else
             {
                /*if((ch >= _KEY_NUM0) && (ch <= _KEY_NUM9))
                {
                    if(0 < (cha = m_pGoodsKeyShareInfo->KeyValueConversion(ch)))
                    {
                        printf("----------input :%c\n",cha);
                        goodsKey.append(cha);
                    }
                }*/
                //else
                if((ch >= _KEY_PLU1) && (ch <= _KEY_PLU60))
                {
                    goodsKey.sprintf("%d",ch);
                }
                sprintf(m_pGoodsKeyShareInfo->m_stGoodsKeyInfo.message,"已选商品键%s,\"确定\"完成对应",goodsKey.toStdString().c_str());
             }
        }
    }
}

