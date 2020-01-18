#ifndef SQLITEDEAL_H
#define SQLITEDEAL_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QtDebug>
#include <QString>
#include <QFile>
#include <pthread.h>
#include "DBStruct.h"

#define DBNAME_SALE 		"./database/CASCT100_SALE.db"
#define DBNAME_CAL 		"./database/CASCT100_CAL.db"
#define DBNAME_SETTING 	        "./database/CASCT100_SETTING.db"
#define DBNAME_PLU	        "./database/CASCT100_PLU.db"
#define DBNAME_LOG		"./database/CASCT100_LOG.db"

#define    SaleDB       1
#define    SettingDB    2
#define    CalDB        3
#define    PluDB        4
#define    LogDB        5


class SQLiteDeal
{
public:
    SQLiteDeal();
    virtual ~ SQLiteDeal();

    int Select_ReturnList_QSqlQuery_By_SQL_Statements(int db,QString SQL_Statements, QSqlQuery *SqlQuery);

    int Select_ReturnQString_By_SQL_Statements(int db,QString SQL_Statements,QString *ReturnQString);

    int Insert_DataList_By_TableName_DataList(int db,char *TableName,void * inData,int size);

    int Excute_SQL_Statements(int db,QString SQL_Statements);


private:
    int OpenDB(int indexof);
    int CreatDBAndTable(int indexof);
    int insert_Defalt_data(char *TableName,QSqlQuery *SqlQuery);
    int CloseDB(void);

    QSqlDatabase dbFile;
    pthread_mutex_t	mutex;
};

#endif // SQLITEDEAL_H
