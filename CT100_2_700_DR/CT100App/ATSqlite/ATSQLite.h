#ifndef ATSQLITE_H
#define ATSQLITE_H
//#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtDebug>
#include <QString>
#include "SQLiteDeal.h"

/*
 *根据表名或SQL语句选择将要操作的表
 *
 */
int FindDbNum(char * tablename);
//----------------------------------------------------------------------------------------------------分隔符

/*
 *根据SQL条件查询数据库，得到结果集
 *参数1：正确的SQL查询语句及其条件
 *参数2：外部定义QSqlQuery对象，传入对象指针，得到结果集
 *返回: -1执行SQL语句错误，0查询结果集为空，>0查询成功返回结果于参数2，返回值为数据列数
 *
 *select * from tablename;
 *select * from tablename where （条件）;
 */
int ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(QString SQL_Statements, QSqlQuery* SqlQuery);
//----------------------------------------------------------------------------------------------------分隔符

/*
 *根据SQL条件查询数据库，得到条件下对应dantiao数据
 *参数1：正确的SQL查询语句及其条件
 *参数2：外部定义QString对象，传入对象指针，得到数据
 *返回: -1执行SQL语句错误，0未查询到数据，=1 查询成功返回结果于参数2
 *
 *select 字段 from tablename where 条件；
 *select max(字段)from tablename ;
 *select min(字段)from tablename ;
 *select count(*) from tablename;
 *select count(*) from tablename where 条件;
 */
int ATdb_Select_ReturnQString_By_SQL_Statements(QString SQL_Statements,QString *ReturnQString);
//----------------------------------------------------------------------------------------------------分隔符

/*
 *根据表名将数据插入数据库，底层id自增，注意：表名与传入数据的对应
 *参数1：要插入数据的表名
 *参数2：待插入的数据集指针
 *参数3：待插入的数据集的数据条数
 *返回：-1执行SQL语句错误，1插入成功
 */
int ATdb_Insert_DataList_By_TableName_DataList(char *TableName,void * inData,int size);
//----------------------------------------------------------------------------------------------------分隔符

/*
 *执行无返回的SQL语句
 *参数1：正确的SQL语句
 *返回：-1执行SQL语句错误，1执行成功
 *
 *updata 语句 更新单条数据
 *delete 语句
 */
int ATdb_Excute_SQL_Statements(QString SQL_Statements);
//----------------------------------------------------------------------------------------------------分隔符


int ATdb_Excute_SQL_ClearDisk(void);
#endif // ATSQLITE_H
