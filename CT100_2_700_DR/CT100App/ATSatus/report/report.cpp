#include "report.h"


#define DATASIZE (REPORT_SIZE-1)

CReport::CReport(CShareInfo *pInfo)
{
    m_pReportShareInfo = pInfo;
    m_bUpdate = false;
}

void CReport::initReport(void)
{
    index = 0;
    m_pReportShareInfo->shareReportUi.index = index;
    currentId = 0;
    currentNum = 0;
    currentTotalNum = 0;
    totalNum = 0;
    memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));
    //注意  QT表格为11行，前10行明细 11行小计
}

enQtShowType CReport::ReportProc()
{
    printf("-------------------------------------------CReport\n");
    m_pReportShareInfo->m_enQtUiType = enQt_ReportQt;
    initReport();
    m_bUpdate = true;
    updateReportShow();

    while(true)
    {
        usleep(100*1000);

        switch (ReportDeal())
        {

        case enQt_SaleQT:
                //
                return enQt_SaleQT;
        case enQt_ReportQt:

                break;

         default:break;
        }
    }

}

enQtShowType CReport::ReportDeal()
{
    unsigned char ch = 0;
    usleep(100*1000);
    if (SALE_KEY_VALID == ATKey_getKey(&ch))
    {
       m_bUpdate = true;
       switch(ch)
       {
           case _KEY_MENU:
                return enQt_SaleQT;
           case _KEY_UP://上一页
                up();
               break;
           case _KEY_DOWN://下一页
                down();
               break;
           case _KEY_NUM1://日报表
           case _KEY_NUM2://报表明细
           case _KEY_NUM3://已上传明细
           case _KEY_NUM4://未上传明细
               index = ch - _KEY_NUM0;
               findOnePage();
               break;
           case _KEY_PRINT:
                print();
           default:break;
       }

    }
    updateReportShow();
    return enQt_ReportQt;
}


void CReport::up(void)
{
    if(index <=1 || totalNum == 0 ||  currentTotalNum<=DATASIZE)
    {
        //未选择报表类型或者没有记录数
        return;
    }
    else if(index == 2)
    {
        bool ok;
        QDateTime CurrentTime = QDateTime::currentDateTime();
        QString sql = QString("select * from table_business where time like '%1%' and id<%2 order by id desc limit 10;").arg(CurrentTime.toString("yyyy-MM-dd")).arg(currentId+currentNum+DATASIZE);
        qDebug() << sql;
        int dbSize = 0;
        query.clear();
        if((dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sql,&query))<0)
        {
            qDebug() << "up 2 select  error";
            query.clear();
            return ;
        }
        memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));

        float total = 0.0;
        for(int i=0;i<dbSize;i++)
        {
           query.next();
           sprintf(m_pReportShareInfo->shareReportUi.table[i].plu,"%s", query.record().value(TABLE_BUSINESS_GOODSNAME).toString().toStdString().c_str());
           if(query.record().value(TABLE_BUSINESS_SALEMODE).toString() == "0")
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%s", query.record().value(TABLE_BUSINESS_WEIGHT).toString().toStdString().c_str());
           }
           else
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%sPcs", query.record().value(TABLE_BUSINESS_PIECES).toString().toStdString().c_str());
           }
           sprintf(m_pReportShareInfo->shareReportUi.table[i].total,"%s", query.record().value(TABLE_BUSINESS_TOTAL).toString().toStdString().c_str());
           sprintf(m_pReportShareInfo->shareReportUi.table[i].payMode,"%s", query.record().value(TABLE_BUSINESS_PAYTYPE).toString().toStdString().c_str());
            total += atof(m_pReportShareInfo->shareReportUi.table[i].total);
        }

         currentTotalNum -= currentNum;
         currentNum = dbSize;
         currentId = query.record().value(TABLE_BUSINESS_ID).toString().toInt(&ok,10);//记录12笔id
         qDebug() << "up currentId:"<<currentId;
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].plu,"%s", "页合计");
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].weight,"%d", dbSize);
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].total,"%0.2f", total);
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].payMode,"", "");
    }
    else if(index == 3)
    {
        bool ok;
        QDateTime CurrentTime = QDateTime::currentDateTime();
        QString sql = QString("select * from table_business where time like '%1%' and webUp='1' and id<%2 order by id desc limit 10;").arg(CurrentTime.toString("yyyy-MM-dd")).arg(currentId+currentNum+DATASIZE);
        qDebug() << sql;
        int dbSize = 0;
        query.clear();
        if((dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sql,&query))<0)
        {
            qDebug() << "up 3 select  error";
            query.clear();
            return ;
        }

        float total = 0.0;
        memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));
        for(int i=0;i<dbSize;i++)
        {
           query.next();
           sprintf(m_pReportShareInfo->shareReportUi.table[i].plu,"%s", query.record().value(TABLE_BUSINESS_GOODSNAME).toString().toStdString().c_str());
           if(query.record().value(TABLE_BUSINESS_SALEMODE).toString() == "0")
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%s", query.record().value(TABLE_BUSINESS_WEIGHT).toString().toStdString().c_str());
           }
           else
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%sPcs", query.record().value(TABLE_BUSINESS_PIECES).toString().toStdString().c_str());
           }
           sprintf(m_pReportShareInfo->shareReportUi.table[i].total,"%s", query.record().value(TABLE_BUSINESS_TOTAL).toString().toStdString().c_str());
           sprintf(m_pReportShareInfo->shareReportUi.table[i].payMode,"%s", query.record().value(TABLE_BUSINESS_PAYTYPE).toString().toStdString().c_str());
            total += atof(m_pReportShareInfo->shareReportUi.table[i].total);
        }
        currentTotalNum -= currentNum;
        currentNum = dbSize;
         currentId = query.record().value(TABLE_BUSINESS_ID).toString().toInt(&ok,10);//记录12笔id
         qDebug() << "up currentId:"<<currentId;
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].plu,"%s", "页合计");
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].weight,"%d", dbSize);
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].total,"%0.2f", total);
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].payMode,"", "");
    }
    else if(index == 4)
    {
        bool ok;
        QDateTime CurrentTime = QDateTime::currentDateTime();
        QString sql = QString("select * from table_business where time like '%1%' and webUp='0' and id<%2 order by id desc limit 10;").arg(CurrentTime.toString("yyyy-MM-dd")).arg(currentId+currentNum+DATASIZE);
        qDebug() << sql;
        int dbSize = 0;
        query.clear();
        if((dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sql,&query))<0)
        {
            qDebug() << "up 4 select  error";
            query.clear();
            return ;
        }
        memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));

        float total = 0.0;
        for(int i=0;i<dbSize;i++)
        {
           query.next();
           sprintf(m_pReportShareInfo->shareReportUi.table[i].plu,"%s", query.record().value(TABLE_BUSINESS_GOODSNAME).toString().toStdString().c_str());
           if(query.record().value(TABLE_BUSINESS_SALEMODE).toString() == "0")
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%s", query.record().value(TABLE_BUSINESS_WEIGHT).toString().toStdString().c_str());
           }
           else
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%sPcs", query.record().value(TABLE_BUSINESS_PIECES).toString().toStdString().c_str());
           }
           sprintf(m_pReportShareInfo->shareReportUi.table[i].total,"%s", query.record().value(TABLE_BUSINESS_TOTAL).toString().toStdString().c_str());
           sprintf(m_pReportShareInfo->shareReportUi.table[i].payMode,"%s", query.record().value(TABLE_BUSINESS_PAYTYPE).toString().toStdString().c_str());
            total += atof(m_pReportShareInfo->shareReportUi.table[i].total);
        }
        currentTotalNum -= currentNum;
        currentNum = dbSize;
         currentId = query.record().value(TABLE_BUSINESS_ID).toString().toInt(&ok,10);//记录12笔id
         qDebug() << "up currentId:"<<currentId;
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].plu,"%s", "页合计");
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].weight,"%d", dbSize);
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].total,"%0.2f", total);
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].payMode,"", "");
    }
    query.clear();
}

void CReport::down(void)
{
    if(index <=1 || totalNum == 0 || totalNum-currentTotalNum == 0)
    {
        //未选择报表类型或者没有下页记录数
        return;
    }
    else if(index == 2)
    {
        bool ok;
        QDateTime CurrentTime = QDateTime::currentDateTime();
        QString sql = QString("select * from table_business where time like '%1%' and id<%2 order by id desc limit 10;").arg(CurrentTime.toString("yyyy-MM-dd")).arg(currentId);
        qDebug() << sql;
        int dbSize = 0;
        query.clear();

        if((dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sql,&query))<0)
        {
            qDebug() << "up 2 select  error";
            query.clear();
            return ;
        }
        memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));

        float total = 0.0;
        for(int i=0;i<dbSize;i++)
        {
           query.next();
           sprintf(m_pReportShareInfo->shareReportUi.table[i].plu,"%s", query.record().value(TABLE_BUSINESS_GOODSNAME).toString().toStdString().c_str());
           if(query.record().value(TABLE_BUSINESS_SALEMODE).toString() == "0")
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%s", query.record().value(TABLE_BUSINESS_WEIGHT).toString().toStdString().c_str());
           }
           else
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%sPcs", query.record().value(TABLE_BUSINESS_PIECES).toString().toStdString().c_str());
           }
           sprintf(m_pReportShareInfo->shareReportUi.table[i].total,"%s", query.record().value(TABLE_BUSINESS_TOTAL).toString().toStdString().c_str());
           sprintf(m_pReportShareInfo->shareReportUi.table[i].payMode,"%s", query.record().value(TABLE_BUSINESS_PAYTYPE).toString().toStdString().c_str());
            total += atof(m_pReportShareInfo->shareReportUi.table[i].total);
        }
        currentNum = dbSize;
         currentTotalNum += dbSize;
         currentId = query.record().value(TABLE_BUSINESS_ID).toString().toInt(&ok,10);//记录12笔id
         qDebug() << "up currentId:"<<currentId;
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].plu,"%s", "页合计");
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].weight,"%d", dbSize);
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].total,"%0.2f", total);
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].payMode,"", "");
    }
    else if(index == 3)
    {
        bool ok;
        QDateTime CurrentTime = QDateTime::currentDateTime();
        QString sql = QString("select * from table_business where time like '%1%' and webUp='1' and id<%2 order by id desc limit 10;").arg(CurrentTime.toString("yyyy-MM-dd")).arg(currentId);
        qDebug() << sql;
        int dbSize = 0;
        query.clear();
        if((dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sql,&query))<0)
        {
            qDebug() << "up 3 select  error";
            query.clear();
            return ;
        }
        memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));

        float total = 0.0;
        for(int i=0;i<dbSize;i++)
        {
           query.next();
           sprintf(m_pReportShareInfo->shareReportUi.table[i].plu,"%s", query.record().value(TABLE_BUSINESS_GOODSNAME).toString().toStdString().c_str());
           if(query.record().value(TABLE_BUSINESS_SALEMODE).toString() == "0")
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%s", query.record().value(TABLE_BUSINESS_WEIGHT).toString().toStdString().c_str());
           }
           else
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%sPcs", query.record().value(TABLE_BUSINESS_PIECES).toString().toStdString().c_str());
           }
           sprintf(m_pReportShareInfo->shareReportUi.table[i].total,"%s", query.record().value(TABLE_BUSINESS_TOTAL).toString().toStdString().c_str());
           sprintf(m_pReportShareInfo->shareReportUi.table[i].payMode,"%s", query.record().value(TABLE_BUSINESS_PAYTYPE).toString().toStdString().c_str());
            total += atof(m_pReportShareInfo->shareReportUi.table[i].total);
        }
        currentNum = dbSize;
         currentTotalNum += dbSize;
         currentId = query.record().value(TABLE_BUSINESS_ID).toString().toInt(&ok,10);//记录12笔id
         qDebug() << "up currentId:"<<currentId;
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].plu,"%s", "页合计");
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].weight,"%d", dbSize);
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].total,"%0.2f", total);
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].payMode,"", "");
    }
    else if(index == 4)
    {
        bool ok;
        QDateTime CurrentTime = QDateTime::currentDateTime();
        QString sql = QString("select * from table_business where time like '%1%' and webUp='0' and id<%2 order by id desc limit 10;").arg(CurrentTime.toString("yyyy-MM-dd")).arg(currentId);
        qDebug() << sql;
        int dbSize = 0;
        query.clear();
        if((dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sql,&query))<0)
        {
            qDebug() << "up 4 select  error";
            query.clear();
            return ;
        }
        memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));

        float total = 0.0;
        for(int i=0;i<dbSize;i++)
        {
           query.next();
           sprintf(m_pReportShareInfo->shareReportUi.table[i].plu,"%s", query.record().value(TABLE_BUSINESS_GOODSNAME).toString().toStdString().c_str());
           if(query.record().value(TABLE_BUSINESS_SALEMODE).toString() == "0")
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%s", query.record().value(TABLE_BUSINESS_WEIGHT).toString().toStdString().c_str());
           }
           else
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%sPcs", query.record().value(TABLE_BUSINESS_PIECES).toString().toStdString().c_str());
           }
           sprintf(m_pReportShareInfo->shareReportUi.table[i].total,"%s", query.record().value(TABLE_BUSINESS_TOTAL).toString().toStdString().c_str());
           sprintf(m_pReportShareInfo->shareReportUi.table[i].payMode,"%s", query.record().value(TABLE_BUSINESS_PAYTYPE).toString().toStdString().c_str());
            total += atof(m_pReportShareInfo->shareReportUi.table[i].total);
        }
         currentNum = dbSize;
         currentTotalNum += dbSize;
         currentId = query.record().value(TABLE_BUSINESS_ID).toString().toInt(&ok,10);//记录12笔id
         qDebug() << "up currentId:"<<currentId;
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].plu,"%s", "页合计");
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].weight,"%d", dbSize);
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].total,"%0.2f", total);
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].payMode,"", "");
    }
    query.clear();
}

void CReport::findOnePage(void)
{
    totalNum = 0;
    currentTotalNum = 0;

    if(index <=0)
    {
        //未选择报表类型
        return;
    }
    else if(index == 1)//今日统计
    {
        QDateTime CurrentTime = QDateTime::currentDateTime();
        QString sql = QString("select * from table_attach where scaleTime like '%1%' and payType=%2;").arg(CurrentTime.toString("yyyy-MM-dd")).arg(enCash);
        qDebug() << sql;
        int dbSize = 0;
        query.clear();
        //查找今日现金支付
        if((dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sql,&query))<0)
        {
            dbSize = 0;
        }
        float total = 0.0;
        for(int i=0;i<dbSize;i++)
        {
           query.next();
           total += query.record().value(TABLE_ATTACH_SUBTOTAL).toString().toFloat();
        }
        memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));
         qDebug() << "findOnePage total:"<<total;
         sprintf(m_pReportShareInfo->shareReportUi.table[0].plu,"%s", "现金支付");
         sprintf(m_pReportShareInfo->shareReportUi.table[0].weight,"%d", dbSize);
         sprintf(m_pReportShareInfo->shareReportUi.table[0].total,"%0.2f", total);
         sprintf(m_pReportShareInfo->shareReportUi.table[0].payMode,"", "");

         sql = QString("select * from table_attach where scaleTime like '%1%' and payType=%2;").arg(CurrentTime.toString("yyyy-MM-dd").arg(enUnionPay));
         qDebug() << sql;
         query.clear();
         //查找今日POS支付
         if((dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sql,&query))<0)
         {
             dbSize = 0;
         }
         total = 0.0;
         for(int i=0;i<dbSize;i++)
         {
            query.next();
            total += query.record().value(TABLE_ATTACH_SUBTOTAL).toString().toFloat();
         }
         //memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));
         qDebug() << "findOnePage total:"<<total;
         sprintf(m_pReportShareInfo->shareReportUi.table[1].plu,"%s", "银联卡支付");
         sprintf(m_pReportShareInfo->shareReportUi.table[1].weight,"%d", dbSize);
         sprintf(m_pReportShareInfo->shareReportUi.table[1].total,"%0.2f", total);
         sprintf(m_pReportShareInfo->shareReportUi.table[1].payMode,"", "");

         sql = QString("select * from table_attach where scaleTime like '%1%' and payType=%2;").arg(CurrentTime.toString("yyyy-MM-dd")).arg(enAliPay);
         qDebug() << sql;
         query.clear();
         //查找今日支付宝支付
         if((dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sql,&query))<0)
         {
             dbSize = 0;
         }
         total = 0.0;
         for(int i=0;i<dbSize;i++)
         {
            query.next();
            total += query.record().value(TABLE_ATTACH_SUBTOTAL).toString().toFloat();
         }
         //memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));
         qDebug() << "findOnePage total:"<<total;
         sprintf(m_pReportShareInfo->shareReportUi.table[2].plu,"%s", "支付宝支付");
         sprintf(m_pReportShareInfo->shareReportUi.table[2].weight,"%d", dbSize);
         sprintf(m_pReportShareInfo->shareReportUi.table[2].total,"%0.2f", total);
         sprintf(m_pReportShareInfo->shareReportUi.table[2].payMode,"", "");

         sql = QString("select * from table_attach where scaleTime like '%1%' and payType=%2;").arg(CurrentTime.toString("yyyy-MM-dd")).arg(enWeChatPay);
         qDebug() << sql;
         query.clear();
         //查找今日微信支付
         if((dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sql,&query))<0)
         {
             dbSize = 0;
         }
         total = 0.0;
         for(int i=0;i<dbSize;i++)
         {
            query.next();
            total += query.record().value(TABLE_ATTACH_SUBTOTAL).toString().toFloat();
         }
         //memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));
         qDebug() << "findOnePage total:"<<total;
         sprintf(m_pReportShareInfo->shareReportUi.table[3].plu,"%s", "微信支付");
         sprintf(m_pReportShareInfo->shareReportUi.table[3].weight,"%d", dbSize);
         sprintf(m_pReportShareInfo->shareReportUi.table[3].total,"%0.2f", total);
         sprintf(m_pReportShareInfo->shareReportUi.table[3].payMode,"", "");

         sql = QString("select * from table_attach where scaleTime like '%1%' and payType=%2;").arg(CurrentTime.toString("yyyy-MM-dd")).arg(enVipPay);
         qDebug() << sql;
         query.clear();
         //查找今日会员卡支付
         if((dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sql,&query))<0)
         {
             dbSize = 0;
         }
         total = 0.0;
         for(int i=0;i<dbSize;i++)
         {
            query.next();
            total += query.record().value(TABLE_ATTACH_SUBTOTAL).toString().toFloat();
         }
         //memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));
         qDebug() << "findOnePage total:"<<total;
         sprintf(m_pReportShareInfo->shareReportUi.table[4].plu,"%s", "会员卡支付");
         sprintf(m_pReportShareInfo->shareReportUi.table[4].weight,"%d", dbSize);
         sprintf(m_pReportShareInfo->shareReportUi.table[4].total,"%0.2f", total);
         sprintf(m_pReportShareInfo->shareReportUi.table[4].payMode,"", "");

         sql = QString("select * from table_attach where scaleTime like '%1%' and payType=%2;").arg(CurrentTime.toString("yyyy-MM-dd")).arg(enBankPay);
         qDebug() << sql;
         query.clear();
         //查找今日会员卡支付
         if((dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sql,&query))<0)
         {
             dbSize = 0;
         }
         total = 0.0;
         for(int i=0;i<dbSize;i++)
         {
            query.next();
            total += query.record().value(TABLE_ATTACH_SUBTOTAL).toString().toFloat();
         }
         //memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));
         qDebug() << "findOnePage total:"<<total;
         sprintf(m_pReportShareInfo->shareReportUi.table[5].plu,"%s", "扫码支付");
         sprintf(m_pReportShareInfo->shareReportUi.table[5].weight,"%d", dbSize);
         sprintf(m_pReportShareInfo->shareReportUi.table[5].total,"%0.2f", total);
         sprintf(m_pReportShareInfo->shareReportUi.table[5].payMode,"", "");

         total += atof(m_pReportShareInfo->shareReportUi.table[0].total) + atof(m_pReportShareInfo->shareReportUi.table[1].total)
                 + atof(m_pReportShareInfo->shareReportUi.table[2].total) + atof(m_pReportShareInfo->shareReportUi.table[3].total)
                 + atof(m_pReportShareInfo->shareReportUi.table[4].total);
         dbSize = atoi(m_pReportShareInfo->shareReportUi.table[0].weight) + atoi(m_pReportShareInfo->shareReportUi.table[1].weight)
                 + atoi(m_pReportShareInfo->shareReportUi.table[2].weight) + atoi(m_pReportShareInfo->shareReportUi.table[3].weight)
                 + atoi(m_pReportShareInfo->shareReportUi.table[4].weight);;

         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].plu,"%s", "合计支付");
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].weight,"%d", dbSize);
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].total,"%0.2f", total);
         sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].payMode,"", "");
    }
    else if(index == 2)
    {
        bool ok;
        QDateTime CurrentTime = QDateTime::currentDateTime();
        QString strNum;//统计今日总笔数
        ATdb_Select_ReturnQString_By_SQL_Statements(QString("select count(*) from table_business where time like '%1%';").arg(CurrentTime.toString("yyyy-MM-dd")),&strNum);
        if(strNum.toInt(&ok,10)<=0)
        {
            totalNum = 0;
            currentTotalNum = 0;
            memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));
            query.clear();
            return ;
        }
        totalNum = strNum.toInt(&ok,10);

        QString sql = QString("select * from table_business where time like '%1%' order by id desc limit 10;").arg(CurrentTime.toString("yyyy-MM-dd"));
        qDebug() << sql;
        int dbSize = 0;
        query.clear();
        //查找今日前10笔
        if((dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sql,&query))<0)
        {
            qDebug() << "findOnePage 2 select  error";
            totalNum = 0;
            currentTotalNum = 0;
            memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));
            query.clear();
            return ;
        }
        memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));

        float total = 0.0;
        for(int i=0;i<dbSize;i++)
        {
           query.next();
           sprintf(m_pReportShareInfo->shareReportUi.table[i].plu,"%s", query.record().value(TABLE_BUSINESS_GOODSNAME).toString().toStdString().c_str());
           if(query.record().value(TABLE_BUSINESS_SALEMODE).toString() == "0")
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%s", query.record().value(TABLE_BUSINESS_WEIGHT).toString().toStdString().c_str());
           }
           else
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%sPcs", query.record().value(TABLE_BUSINESS_PIECES).toString().toStdString().c_str());
           }
           sprintf(m_pReportShareInfo->shareReportUi.table[i].total,"%s", query.record().value(TABLE_BUSINESS_TOTAL).toString().toStdString().c_str());
           sprintf(m_pReportShareInfo->shareReportUi.table[i].payMode,"%s", query.record().value(TABLE_BUSINESS_PAYTYPE).toString().toStdString().c_str());
           total += atof(m_pReportShareInfo->shareReportUi.table[i].total);
        }
        currentNum = dbSize;
         currentTotalNum += dbSize;
         currentId = query.record().value(TABLE_BUSINESS_ID).toString().toInt(&ok,10);//记录12笔id
          qDebug() << "up currentId:"<<currentId;
          sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].plu,"%s", "页合计");
          sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].weight,"%d", dbSize);
          sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].total,"%0.2f", total);
          sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].payMode,"", "");
    }
    else if(index == 3)
    {
        bool ok;
        QDateTime CurrentTime = QDateTime::currentDateTime();
        QString strNum;//统计今日已上传总笔数
        ATdb_Select_ReturnQString_By_SQL_Statements(QString("select count(*) from table_business where time like '%1%' and webUp='1';").arg(CurrentTime.toString("yyyy-MM-dd")),&strNum);
        if(strNum.toInt(&ok,10)<=0)
        {
            totalNum = 0;
            currentTotalNum = 0;
            memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));
            query.clear();
            return ;
        }
        totalNum = strNum.toInt(&ok,10);

        QString sql = QString("select * from table_business where time like '%1%' and webUp='1' order by id desc limit 10;").arg(CurrentTime.toString("yyyy-MM-dd"));
        qDebug() << sql;
        int dbSize = 0;
        query.clear();
        //查找今日已上传前10笔
        if((dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sql,&query))<0)
        {
            qDebug() << "findOnePage 3 select  error";
            totalNum = 0;
            currentTotalNum = 0;
            memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));
            query.clear();
            return ;
        }
        memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));

        float total = 0.0;
        for(int i=0;i<dbSize;i++)
        {
           query.next();
           sprintf(m_pReportShareInfo->shareReportUi.table[i].plu,"%s", query.record().value(TABLE_BUSINESS_GOODSNAME).toString().toStdString().c_str());
           if(query.record().value(TABLE_BUSINESS_SALEMODE).toString() == "0")
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%s", query.record().value(TABLE_BUSINESS_WEIGHT).toString().toStdString().c_str());
           }
           else
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%sPcs", query.record().value(TABLE_BUSINESS_PIECES).toString().toStdString().c_str());
           }
           sprintf(m_pReportShareInfo->shareReportUi.table[i].total,"%s", query.record().value(TABLE_BUSINESS_TOTAL).toString().toStdString().c_str());
           sprintf(m_pReportShareInfo->shareReportUi.table[i].payMode,"%s", query.record().value(TABLE_BUSINESS_PAYTYPE).toString().toStdString().c_str());
           total += atof(m_pReportShareInfo->shareReportUi.table[i].total);
        }
        currentNum = dbSize;
         currentTotalNum += dbSize;
         currentId = query.record().value(TABLE_BUSINESS_ID).toString().toInt(&ok,10);//记录12笔id
          qDebug() << "up currentId:"<<currentId;
          sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].plu,"%s", "页合计");
          sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].weight,"%d", dbSize);
          sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].total,"%0.2f", total);
          sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].payMode,"", "");
    }
    else if(index == 4)
    {
        bool ok;
        QDateTime CurrentTime = QDateTime::currentDateTime();
        QString strNum;//统计今日未上传总笔数
        ATdb_Select_ReturnQString_By_SQL_Statements(QString("select count(*) from table_business where time like '%1%' and webUp='0';").arg(CurrentTime.toString("yyyy-MM-dd")),&strNum);
        if(strNum.toInt(&ok,10)<=0)
        {
            totalNum = 0;
            currentTotalNum = 0;
            memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));
            query.clear();
            return ;
        }
        totalNum = strNum.toInt(&ok,10);

        QString sql = QString("select * from table_business where time like '%1%' and webUp='0' order by id desc limit 10;").arg(CurrentTime.toString("yyyy-MM-dd"));
        qDebug() << sql;
        int dbSize = 0;
        query.clear();
        //查找今日未上传前10笔
        if((dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sql,&query))<0)
        {
            qDebug() << "findOnePage 3 select  error";
            totalNum = 0;
            currentTotalNum = 0;
            memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));
            query.clear();
            return ;
        }
        memset(&m_pReportShareInfo->shareReportUi,0x00,sizeof(stReportQT));

        float total = 0.0;
        for(int i=0;i<dbSize;i++)
        {
           query.next();
           sprintf(m_pReportShareInfo->shareReportUi.table[i].plu,"%s", query.record().value(TABLE_BUSINESS_GOODSNAME).toString().toStdString().c_str());
           if(query.record().value(TABLE_BUSINESS_SALEMODE).toString() == "0")
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%s", query.record().value(TABLE_BUSINESS_WEIGHT).toString().toStdString().c_str());
           }
           else
           {
               sprintf(m_pReportShareInfo->shareReportUi.table[i].weight,"%sPcs", query.record().value(TABLE_BUSINESS_PIECES).toString().toStdString().c_str());
           }
           sprintf(m_pReportShareInfo->shareReportUi.table[i].total,"%s", query.record().value(TABLE_BUSINESS_TOTAL).toString().toStdString().c_str());
           sprintf(m_pReportShareInfo->shareReportUi.table[i].payMode,"%s", query.record().value(TABLE_BUSINESS_PAYTYPE).toString().toStdString().c_str());
           total += atof(m_pReportShareInfo->shareReportUi.table[i].total);
        }
        currentNum = dbSize;
         currentTotalNum += dbSize;
         currentId = query.record().value(TABLE_BUSINESS_ID).toString().toInt(&ok,10);//记录12笔id
          qDebug() << "up currentId:"<<currentId;
          sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].plu,"%s", "页合计");
          sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].weight,"%d", dbSize);
          sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].total,"%0.2f", total);
          sprintf(m_pReportShareInfo->shareReportUi.table[DATASIZE].payMode,"", "");
    }
    query.clear();
}

void CReport::print(void)
{
    if(index <=0)
    {
        //未选择报表类型
        return;
    }
    stReceiptData data;
    char str[64] = {0};
    QDateTime CurrentTime = QDateTime::currentDateTime();

    if(index == 1)
    {
        data.type = enCenterString;
        sprintf(str,"%s", "日报表统计");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        data.type = enString;
        sprintf(str,"%s%s", "统计时间:",CurrentTime.toString("yyyy-MM-dd hh:mm:ss").toStdString().c_str());
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(data.str,"%s","================================");
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(str, "%s", "支付方式    笔数    总计金额/元");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(data.str,"%s","================================");
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(str, "%s", m_pReportShareInfo->shareReportUi.table[0].plu);
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(data.str, "%12s %-09s %-12s"," ",m_pReportShareInfo->shareReportUi.table[0].weight,m_pReportShareInfo->shareReportUi.table[0].total);
        //ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(str, "%s", m_pReportShareInfo->shareReportUi.table[1].plu);
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(data.str, "%12s %-09s %-12s"," ", m_pReportShareInfo->shareReportUi.table[1].weight,m_pReportShareInfo->shareReportUi.table[1].total);
        //ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(str, "%s", m_pReportShareInfo->shareReportUi.table[2].plu);
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(data.str, "%12s %-09s %-12s"," ",m_pReportShareInfo->shareReportUi.table[2].weight,m_pReportShareInfo->shareReportUi.table[2].total);
        //ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(str, "%s", m_pReportShareInfo->shareReportUi.table[3].plu);
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(data.str, "%12s %-09s %-12s"," ", m_pReportShareInfo->shareReportUi.table[3].weight,m_pReportShareInfo->shareReportUi.table[3].total);
        //ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(str, "%s", m_pReportShareInfo->shareReportUi.table[4].plu);
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(data.str, "%12s %-09s %-12s"," ", m_pReportShareInfo->shareReportUi.table[4].weight,m_pReportShareInfo->shareReportUi.table[4].total);
        //ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(str, "%s", m_pReportShareInfo->shareReportUi.table[5].plu);
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(data.str, "%12s %-09s %-12s"," ", m_pReportShareInfo->shareReportUi.table[5].weight,m_pReportShareInfo->shareReportUi.table[5].total);
        //ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);


        data.type = enString;
        sprintf(data.str,"%s","--------------------------------");
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(str, "%s", "合计结果");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(data.str, "              %s       %s", m_pReportShareInfo->shareReportUi.table[DATASIZE].weight,m_pReportShareInfo->shareReportUi.table[DATASIZE].total);
        //ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);
    }
    else if(index >= 2)
    {
        if(currentNum<=0)
        {
            return;
        }
        data.type = enCenterString;
        sprintf(str,"%s", "页明细统计");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        data.type = enString;
        sprintf(str,"%s%s", "统计时间:",CurrentTime.toString("yyyy-MM-dd hh:mm:ss").toStdString().c_str());
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        // 每行最大长度为32
        sprintf(data.str,"%s","================================");
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(str, "%s", "商品名称   金额/元   支付方式");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(data.str,"%s","================================");
        m_pReportShareInfo->m_pPrint->add(data);
        for(int i = 0; i<currentNum;i++)
        {
            sprintf(str,"%s",m_pReportShareInfo->shareReportUi.table[i].plu);
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pReportShareInfo->m_pPrint->add(data);

            if(atoi(m_pReportShareInfo->shareReportUi.table[i].payMode) == enCash)
            {
                sprintf(str,"%10s %-09s %-12s"," ",m_pReportShareInfo->shareReportUi.table[i].total,"现金支付");
                ATLib_u2g(str,strlen(str),data.str,64);
            }
            else if(atoi(m_pReportShareInfo->shareReportUi.table[i].payMode) == enUnionPay)
            {
                sprintf(str,"%10s %-09s %-12s"," ",m_pReportShareInfo->shareReportUi.table[i].total,"银联卡支付");
                ATLib_u2g(str,strlen(str),data.str,64);
            }
            else if(atoi(m_pReportShareInfo->shareReportUi.table[i].payMode) == enAliPay)
            {
                sprintf(str,"%10s %-09s %-12s"," ",m_pReportShareInfo->shareReportUi.table[i].total,"支付宝支付");
                ATLib_u2g(str,strlen(str),data.str,64);
            }
            else if(atoi(m_pReportShareInfo->shareReportUi.table[i].payMode) == enWeChatPay)
            {
                sprintf(str,"%10s %-09s %-12s"," ",m_pReportShareInfo->shareReportUi.table[i].total,"微信支付");
                ATLib_u2g(str,strlen(str),data.str,64);
            }
            else if(atoi(m_pReportShareInfo->shareReportUi.table[i].payMode) == enVipPay)
            {
                sprintf(str,"%10s %-09s %-12s"," ",m_pReportShareInfo->shareReportUi.table[i].total,"会员卡支付");
                ATLib_u2g(str,strlen(str),data.str,64);
            }
            else if(atoi(m_pReportShareInfo->shareReportUi.table[i].payMode) == enBankPay)
            {
                sprintf(str,"%10s %-09s %-12s"," ",m_pReportShareInfo->shareReportUi.table[i].total,"扫码支付");
                ATLib_u2g(str,strlen(str),data.str,64);
            }
            else
            {
                sprintf(str,"%10s %-09s %-12s"," ",m_pReportShareInfo->shareReportUi.table[i].total,"其他支付");
                ATLib_u2g(str,strlen(str),data.str,64);
            }
            m_pReportShareInfo->m_pPrint->add(data);
        }
        data.type = enString;
        sprintf(data.str,"%s","--------------------------------");
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(str, "%s", "合计结果");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);

        sprintf(data.str, "%10s %-09s %-12s"," ", m_pReportShareInfo->shareReportUi.table[DATASIZE].weight,m_pReportShareInfo->shareReportUi.table[DATASIZE].total);
        //ATLib_u2g(str,strlen(str),data.str,64);
        m_pReportShareInfo->m_pPrint->add(data);
    }

    data.type = enStartPrint;
    m_pReportShareInfo->m_pPrint->add(data);
}

void CReport::updateReportShow(void)
{
    if(true == m_bUpdate)
    {
        m_pReportShareInfo->shareReportUi.index = index;
        m_pReportShareInfo->shareReportUi.curNum = currentTotalNum;
        m_pReportShareInfo->shareReportUi.totalNum = totalNum;
        m_pReportShareInfo->UpdateGui();
    }
    m_bUpdate = false;
}

