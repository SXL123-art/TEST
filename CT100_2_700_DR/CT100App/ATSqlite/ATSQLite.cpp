#include "ATSQLite.h"
#include "DBStruct.h"

SQLiteDeal sql;

int FindDbNum(char * TableName)
{
    char * isdbplu = strstr(TableName,TABLE_PRODUCT);
    char * isdbplu2 = strstr(TableName,"table_product");
    char * isdbplu3 = strstr(TableName,TABLE_CONVERT);
    char * isdbplu4 = strstr(TableName,"table_convert");
    char * isdbplu5 = strstr(TableName,TABLE_BATCH);
    char * isdbplu6 = strstr(TableName,"table_batch");
    char * isdbplu7 = strstr(TableName,TABLE_IDENTIFY);
    char * isdbplu8 = strstr(TableName,"table_identify");
    if(isdbplu != NULL || isdbplu2 != NULL || isdbplu3 != NULL || isdbplu4 != NULL
            || isdbplu5 != NULL || isdbplu6 != NULL || isdbplu7 != NULL || isdbplu8 != NULL)
    {
       return PluDB;
    }

    char * isdbcal = strstr(TableName,TABLE_CALIBRATION);
    char * isdbcal2 = strstr(TableName,"table_calibration");
    if(isdbcal  != NULL  ||  isdbcal2  != NULL)
    {
        return CalDB;
    }

    char * isdblog1 = strstr(TableName,TABLE_SCALELOG);
    char * isdblog2 = strstr(TableName,"table_scalelog");
    if(isdblog1  != NULL  ||  isdblog2  != NULL)
    {
        return LogDB;
    }

    char * isdbsale1 = strstr(TableName,TABLE_ATTACH);
    char * isdbsale2 = strstr(TableName,"table_attach");
    char * isdbsale3 = strstr(TableName,TABLE_BUSINESS);
    char * isdbsale4 = strstr(TableName,"table_business");
    if(isdbsale1  != NULL  ||  isdbsale2  != NULL || isdbsale3 != NULL || isdbsale4 != NULL)
    {
        return SaleDB;
    }

    char * isdbset1 = strstr(TableName,TABLE_SCALESETTING);
    char * isdbset2 = strstr(TableName,"table_scalesetting");
    char * isdbset3 = strstr(TableName,TABLE_USERSETTING);
    char * isdbset4 = strstr(TableName,"table_usersetting");
    char * isdbset5 = strstr(TableName,TABLE_SELLERSINFO);
    char * isdbset6 = strstr(TableName,"table_sellersinfo");
    if(isdbset1  != NULL  ||  isdbset2  != NULL || isdbset3 != NULL || isdbset4 != NULL
            || isdbset5 != NULL || isdbset6 != NULL)
    {
        return SettingDB;
    }

    return 0;
}

int ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(QString SQL_Statements, QSqlQuery *SqlQuery)
{
    int nRet = 0;
    int db = FindDbNum((char *)SQL_Statements.toStdString().c_str());
    if(db==0)
    {
        return nRet;
    }
    nRet = sql.Select_ReturnList_QSqlQuery_By_SQL_Statements(db,SQL_Statements,SqlQuery);
    qDebug() << "select list size-------------------------|:" << nRet;
    return nRet;
}

int ATdb_Select_ReturnQString_By_SQL_Statements(QString SQL_Statements,QString *ReturnQString)
{
    int nRet = 0;
    int db = FindDbNum((char *)SQL_Statements.toStdString().c_str());
    if(db==0)
    {
        return nRet;
    }
    nRet = sql.Select_ReturnQString_By_SQL_Statements(db,SQL_Statements,ReturnQString);
    qDebug() << "select QString data----------------------|:" << *ReturnQString ;
    return nRet;
}
int ATdb_Insert_DataList_By_TableName_DataList(char *TableName,void * inData,int size)
{
    int nRet = 0;
    int db= FindDbNum(TableName);
    if(db==0)
    {
        return nRet;
    }
    nRet = sql.Insert_DataList_By_TableName_DataList(db,TableName,inData,size);
    qDebug() << "Insert_DataList_By_TableName_DataList----------------------|:" << nRet ;
    return nRet;
}

int ATdb_Excute_SQL_Statements(QString SQL_Statements)
{
    int nRet = 0;
    int db = FindDbNum((char *)SQL_Statements.toStdString().c_str());
    if(db==0)
    {
        return nRet;
    }
    nRet = sql.Excute_SQL_Statements(db,SQL_Statements);
    return nRet;
}
int ATdb_Excute_SQL_ClearDisk(void)
{
    int nRet = 0;
    for(int db = 1; db<=5; db++)
    {
        nRet = sql.Excute_SQL_Statements(db,"VACUUM");
    }
    return nRet;
}

