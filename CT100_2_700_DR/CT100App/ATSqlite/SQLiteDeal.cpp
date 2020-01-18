#include "SQLiteDeal.h"

const char* table_product[][2] =
{
    {TABLE_PRODUCT_ID				,TABLE_PRODUCT_ID_TYPE},
    {TABLE_PRODUCT_GOODSCODE                    ,TABLE_PRODUCT_GOODSCODE_TYPE},
    {TABLE_PRODUCT_GOODSNAME                    ,TABLE_PRODUCT_GOODSNAME_TYPE},
    {TABLE_PRODUCT_GOODSORIGIN			,TABLE_PRODUCT_GOODSORIGIN_TYPE},
    {TABLE_PRODUCT_PLUCODE			,TABLE_PRODUCT_PLUCODE_TYPE},
    {TABLE_PRODUCT_GOODSKEY			,TABLE_PRODUCT_GOODSKEY_TYPE},
    {TABLE_PRODUCT_GOODSWEIHT			,TABLE_PRODUCT_GOODSWEIHT_TYPE},
    {TABLE_PRODUCT_GROUPNO                      ,TABLE_PRODUCT_GROUPNO_TYPE},
    {TABLE_PRODUCT_VERSION                      ,TABLE_PRODUCT_VERSION_TYPE},
    {TABLE_PRODUCT_BATCHCODE			,TABLE_PRODUCT_BATCHCODE_TYPE},
    {TABLE_PRODUCT_TRACECODE			,TABLE_PRODUCT_TRACECODE_TYPE},
    {TABLE_PRODUCT_MAXPRICE                     ,TABLE_PRODUCT_MAXPRICE_TYPE},
    {TABLE_PRODUCT_MINPRICE                     ,TABLE_PRODUCT_MINPRICE_TYPE},
    {TABLE_PRODUCT_LASTPRICE                    ,TABLE_PRODUCT_LASTPRICE_TYPE},
    {TABLE_PRODUCT_ISPCS			,TABLE_PRODUCT_ISPCS_TYPE},
    {TABLE_PRODUCT_ISLIMIT			,TABLE_PRODUCT_ISLIMIT_TYPE},
    {TABLE_PRODUCT_PRODUCTX			,TABLE_PRODUCT_PRODUCTX_TYPE},
    {TABLE_PRODUCT_PRODUCTY                     ,TABLE_PRODUCT_PRODUCTY_TYPE},
    {TABLE_PRODUCT_PRODUCTZ                     ,TABLE_PRODUCT_PRODUCTZ_TYPE}
};
const char* table_convert[][2] =
{
        {TABLE_CONVERT_ID			,TABLE_CONVERT_ID_TYPE},
        {TABLE_CONVERT_PRODUCTID		,TABLE_CONVERT_PRODUCTID_TYPE},
	{TABLE_CONVERT_HOTKEY			,TABLE_CONVERT_HOTKEY_TYPE},
        {TABLE_CONVERT_GOODSCODE		,TABLE_CONVERT_GOODSCODE_TYPE},
        {TABLE_CONVERT_BSAVE			,TABLE_CONVERT_BSAVE_TYPE},
        {TABLE_CONVERT_CONVERTZ			,TABLE_CONVERT_CONVERTZ_TYPE}
};

const char* table_batch[][2] =
{
        {TABLE_BATCH_ID				,TABLE_BATCH_ID_TYPE},
        {TABLE_BATCH_GOODSCODE                  ,TABLE_BATCH_GOODSCODE_TYPE},
	{TABLE_BATCH_GOODSNAME			,TABLE_BATCH_GOODSNAME_TYPE},
	{TABLE_BATCH_ORIGINCODE			,TABLE_BATCH_ORIGINCODE_TYPE},
	{TABLE_BATCH_ORIGINNAME			,TABLE_BATCH_ORIGINNAME_TYPE},
        {TABLE_BATCH_BATCHCODE			,TABLE_BATCH_BATCHCODE_TYPE},
	{TABLE_BATCH_TRACECODE			,TABLE_BATCH_TRACECODE_TYPE},
        {TABLE_BATCH_PROPRIETORCODE		,TABLE_BATCH_PROPRIETORCODE_TYPE},
	{TABLE_BATCH_TIME			,TABLE_BATCH_TIME_TYPE},
        {TABLE_BATCH_GROUPNO			,TABLE_BATCH_GROUPNO_TYPE},
        {TABLE_BATCH_WEIGHT			,TABLE_BATCH_WEIGHT_TYPE},
        {TABLE_BATCH_ZONE			,TABLE_BATCH_ZONE_TYPE},
	{TABLE_BATCH_ISPASS			,TABLE_BATCH_ISPASS_TYPE},
        {TABLE_BATCH_BSAVE			,TABLE_BATCH_BSAVE_TYPE},
        {TABLE_BATCH_BATCHY			,TABLE_BATCH_BATCHY_TYPE},
        {TABLE_BATCH_BATCHZ			,TABLE_BATCH_BATCHZ_TYPE}
};

const char* table_identify[][2] =
{
        {TABLE_IDENTIFY_ID			,TABLE_IDENTIFY_ID_TYPE},
        {TABLE_IDENTIFY_IDENTIFYCODE		,TABLE_IDENTIFY_IDENTIFYCODE_TYPE},
        {TABLE_IDENTIFY_GOODSCODE		,TABLE_IDENTIFY_GOODSCODE_TYPE},
        {TABLE_IDENTIFY_GOODSNAME		,TABLE_IDENTIFY_GOODSNAME_TYPE},
        {TABLE_IDENTIFY_IDENTIFYZ		,TABLE_IDENTIFY_IDENTIFYZ_TYPE}
};

const char* table_attach[][2] =
{
        {TABLE_ATTACH_ID			,TABLE_ATTACH_ID_TYPE},
	{TABLE_ATTACH_BUSINESSCODE		,TABLE_ATTACH_BUSINESSCODE_TYPE},
	{TABLE_ATTACH_SALENUM			,TABLE_ATTACH_SALENUM_TYPE},
        {TABLE_ATTACH_PAYTYPE                   ,TABLE_ATTACH_PAYTYPE_TYPE},
        {TABLE_ATTACH_PAYORDER                  ,TABLE_ATTACH_PAYORDER_TYPE},
	{TABLE_ATTACH_CARDTYPE			,TABLE_ATTACH_CARDTYPE_TYPE},
	{TABLE_ATTACH_CARDNUM			,TABLE_ATTACH_CARDNUM_TYPE},
        {TABLE_ATTACH_DEVICETYPE		,TABLE_ATTACH_DEVICETYPE_TYPE},
	{TABLE_ATTACH_DEVICENUM			,TABLE_ATTACH_DEVICENUM_TYPE},
	{TABLE_ATTACH_SERIAL			,TABLE_ATTACH_SERIAL_TYPE},
	{TABLE_ATTACH_SUBTOTAL			,TABLE_ATTACH_SUBTOTAL_TYPE},
        {TABLE_ATTACH_PAYMONEY			,TABLE_ATTACH_PAYMONEY_TYPE},
        {TABLE_ATTACH_SELLERINFO                ,TABLE_ATTACH_SELLERINFO_TYPE},
        {TABLE_ATTACH_BUYERINFO			,TABLE_ATTACH_BUYERINFO_TYPE},
	{TABLE_ATTACH_SCALETIME			,TABLE_ATTACH_SCALETIME_TYPE},
	{TABLE_ATTACH_CONTEXT			,TABLE_ATTACH_CONTEXT_TYPE},
        {TABLE_ATTACH_DEALSTATE			,TABLE_ATTACH_DEALSTATE_TYPE},
        {TABLE_ATTACH_ISPRINT			,TABLE_ATTACH_ISPRINT_TYPE},
        {TABLE_ATTACH_WEBUP			,TABLE_ATTACH_WEBUP_TYPE},
        {TABLE_ATTACH_WINDUP			,TABLE_ATTACH_WINDUP_TYPE},
        {TABLE_ATTACH_ATTACHX			,TABLE_ATTACH_ATTACHX_TYPE},
        {TABLE_ATTACH_ATTACHY			,TABLE_ATTACH_ATTACHY_TYPE},
        {TABLE_ATTACH_ATTACHZ			,TABLE_ATTACH_ATTACHZ_TYPE}
};

const char* table_business[][2] =
{
        {TABLE_BUSINESS_ID			,TABLE_BUSINESS_ID_TYPE},
	{TABLE_BUSINESS_SERIALNUM		,TABLE_BUSINESS_SERIALNUM_TYPE},
        {TABLE_BUSINESS_BUSINESSCODE            ,TABLE_BUSINESS_BUSINESSCODE_TYPE},
        {TABLE_BUSINESS_PAYTYPE                 ,TABLE_BUSINESS_PAYTYPE_TYPE},
	{TABLE_BUSINESS_GOODSCODE		,TABLE_BUSINESS_GOODSCODE_TYPE},
	{TABLE_BUSINESS_GOODSNAME		,TABLE_BUSINESS_GOODSNAME_TYPE},
        {TABLE_BUSINESS_GOODSORIGIN		,TABLE_BUSINESS_GOODSORIGIN_TYPE},
	{TABLE_BUSINESS_BATCHCODE		,TABLE_BUSINESS_BATCHCODE_TYPE},
	{TABLE_BUSINESS_TRACECODE		,TABLE_BUSINESS_TRACECODE_TYPE},
	{TABLE_BUSINESS_WEIGHT			,TABLE_BUSINESS_WEIGHT_TYPE},
	{TABLE_BUSINESS_PRICE			,TABLE_BUSINESS_PRICE_TYPE},
	{TABLE_BUSINESS_PIECES			,TABLE_BUSINESS_PIECES_TYPE},
	{TABLE_BUSINESS_TOTAL			,TABLE_BUSINESS_TOTAL_TYPE},
	{TABLE_BUSINESS_TIME			,TABLE_BUSINESS_TIME_TYPE},
        {TABLE_BUSINESS_SALEMODE                ,TABLE_BUSINESS_SALEMODE_TYPE},
	{TABLE_BUSINESS_CANCEL			,TABLE_BUSINESS_CANCEL_TYPE},
        {TABLE_BUSINESS_PICTURE                 ,TABLE_BUSINESS_PICTURE_TYPE},
	{TABLE_BUSINESS_ALLWEIGHT		,TABLE_BUSINESS_ALLWEIGHT_TYPE},
        {TABLE_BUSINESS_ISOVERSALE		,TABLE_BUSINESS_ISOVERSALE_TYPE},
        {TABLE_BUSINESS_WEBUP                   ,TABLE_BUSINESS_WEBUP_TYPE},
        {TABLE_BUSINESS_WINDUP			,TABLE_BUSINESS_WINDUP_TYPE},
        {TABLE_BUSINESS_BUSINESSY		,TABLE_BUSINESS_BUSINESSY_TYPE},
        {TABLE_BUSINESS_BUSINESSZ		,TABLE_BUSINESS_BUSINESSZ_TYPE}
};

const char* table_calibration[][2] =
{
	{TABLE_CALIBRATION_ID			,TABLE_CALIBRATION_ID_TYPE},
        {TABLE_CALIBRATION_FULLARRANGE          ,TABLE_CALIBRATION_FULLARRANGE_TYPE},
        {TABLE_CALIBRATION_CALARRANGE           ,TABLE_CALIBRATION_CALARRANGE_TYPE},
	{TABLE_CALIBRATION_E			,TABLE_CALIBRATION_E_TYPE},
	{TABLE_CALIBRATION_E1			,TABLE_CALIBRATION_E1_TYPE},
        {TABLE_CALIBRATION_LOCALRAVITY          ,TABLE_CALIBRATION_LOCALRAVITY_TYPE},
        {TABLE_CALIBRATION_DEGRAVITY            ,TABLE_CALIBRATION_DEGRAVITY_TYPE},
	{TABLE_CALIBRATION_UNLOAD		,TABLE_CALIBRATION_UNLOAD_TYPE},
	{TABLE_CALIBRATION_LOAD			,TABLE_CALIBRATION_LOAD_TYPE},
        {TABLE_CALIBRATION_ISDUAL 		,TABLE_CALIBRATION_ISDUAL_TYPE}
};

const char* table_scaleSetting[][2] =
{
	{TABLE_SCALESETTING_ID			,TABLE_SCALESETTING_ID_TYPE},
	{TABLE_SCALESETTING_NAME		,TABLE_SCALESETTING_NAME_TYPE},
	{TABLE_SCALESETTING_VALUE		,TABLE_SCALESETTING_VALUE_TYPE},
	{TABLE_SCALESETTING_UPPER		,TABLE_SCALESETTING_UPPER_TYPE}
};
const char * data_table_scaleSetting[][4] =
{
    {"1",   "RemoteServer",  "http://120.24.95.208/app",      "1"},
    {"2",   "InternalServer",  "192.168.1.2:9090",  		"1"},
    {"3",   "Mac",              "",        "1"},
    {"4",   "Ip",               "192.168.101.38",            "1"},
    {"5",   "Mask",             "255.255.255.0",            "1"},
    {"6",   "Gateway",          "192.168.101.1",              "1"},
    {"7",   "NetMode",         "1",                        "1"},
    {"8",   "SSID",             "BZ",                     "1"},
    {"9",   "PassWord",         "bz88888888",                 "1"},
    {"10",  "Encryption",       "wpa2",                      "1"},
    {"11",  "DHCP",             "0",                        "1"},
    {"12",  "DNS",             "8.8.8.8",                        "1"},
    {"13",   "timeOutScreen",  "30",      "2"},
    {"14",   "weightPoint",  "3",  		"2"},
    {"15",   "totalRound",              "0",        "2"},
    {"16",   "screenBackLight",               "8",            "2"},
    {"17",   "userSwitch",             "0000010",            "3"},
    {"18",   "systemSwitch",          "010000000010000",              "4"},
    {"19",   "scaleSet1",         "",                        "5"},
    {"20",   "scaleSet2",             "",                     "5"},
    {"21",   "scaleSet3",         "",                 "5"}
};
const char* table_userSetting[][2] =
{
	{TABLE_USERSETTING_ID			,TABLE_USERSETTING_ID_TYPE},
	{TABLE_USERSETTING_NAME			,TABLE_USERSETTING_NAME_TYPE},
	{TABLE_USERSETTING_VALUE		,TABLE_USERSETTING_VALUE_TYPE},
	{TABLE_USERSETTING_UPPER		,TABLE_USERSETTING_UPPER_TYPE}
};
const char * data_table_userSetting[][4] =
{
    {"1",   "alipayId",  "",      "1"},
    {"2",   "alipayNum",  "",  		"1"},
    {"3",   "weChatId",              "",        "1"},
    {"4",   "weChatNum",               "",            "1"},
    {"5",   "weChatKey",             "",            "1"},
    {"6",   "otherId",          "",              "1"},
    {"7",   "otherNum",         "",                        "1"},
    {"8",   "bluetoothName",             "",                     "1"},
    {"9",   "bluetoothPsd",         "",                 "1"},
    {"10",  "zigbeeNetId",       "",                      "1"},
    {"11",  "zigbeeChannel",             "",                        "1"},
    {"12",  "deviceId",             "191280968",                        "2"},
    {"13",   "deviceType",  "",      "2"},
    {"14",   "boothId",  "",  		"2"},
    {"15",   "boothName",              "",        "2"},
    {"16",   "boothCode",               "",            "2"},
    {"17",   "scaleNum",             "1",            "2"},
    {"18",   "marketCode",          "",              "2"},
    {"19",   "marketName",         "",                        "2"},
    {"20",   "HeadTail1",             "",                     "2"},
    {"21",   "HeadTail2",         "",                 "2"},
    {"22",   "HeadTail3",          "",              "2"},
    {"23",   "HeadTail4",         "",                        "2"},
    {"24",   "HeadTail5",             "",                     "2"},
    {"25",   "HeadTail6",         "",                 "2"}

};
const char* table_sellerInfo[][2] =
{
	{TABLE_SELLERSINFO_ID				,TABLE_SELLERSINFO_ID_TYPE},
        {TABLE_SELLERSINFO_SELLERCARD                   ,TABLE_SELLERSINFO_SELLERCARD_TYPE},
        {TABLE_SELLERSINFO_PROPRIETORCODE               ,TABLE_SELLERSINFO_PROPRIETORCODE_TYPE},
	{TABLE_SELLERSINFO_NAME				,TABLE_SELLERSINFO_NAME_TYPE},
	{TABLE_SELLERSINFO_PASS				,TABLE_SELLERSINFO_PASS_TYPE}
};

const char* table_scaleLog[][2] =
{
        {TABLE_SCALELOG_ID		,TABLE_SCALELOG_ID_TYPE},
	{TABLE_SCALELOG_NAME		,TABLE_SCALELOG_NAME_TYPE},
	{TABLE_SCALELOG_VALUE1		,TABLE_SCALELOG_VALUE1_TYPE},
	{TABLE_SCALELOG_VALUE2		,TABLE_SCALELOG_VALUE2_TYPE},
	{TABLE_SCALELOG_UPPER		,TABLE_SCALELOG_UPPER_TYPE}
};


SQLiteDeal::SQLiteDeal(void)
{
    qDebug() << "--- [创建数据库对象] ---";
    QString dbName;
    for(int indexof = 1; indexof<=5; indexof++)
    {
        dbName.clear();
        switch(indexof)
        {
            case SaleDB:
                dbName = DBNAME_SALE;
                break;
            case SettingDB:
                dbName = DBNAME_SETTING;
                break;
            case CalDB:
                dbName = DBNAME_CAL;
                break;
            case PluDB:
                dbName = DBNAME_PLU;
                break;
            case LogDB:
                dbName = DBNAME_LOG;
                break;
            default:
                return;
        }
        QFile DBfile(dbName);
        //判断对应路径下的数据库是否存在,true存在则建立数据库链接，false不存在则创建数据库并建立链接
        if(DBfile.exists())
        {
            this->OpenDB(indexof);//测试建立链接
            this->CloseDB();
        }
        else
        {
            this->CreatDBAndTable(indexof);
            this->CloseDB();
        }
    }
    pthread_mutex_init(&mutex,	NULL);
}
SQLiteDeal::~SQLiteDeal()
{
    this->CloseDB();
}
int SQLiteDeal::OpenDB(int indexof)
{
    this->CloseDB();
    //创建连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        dbFile = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        dbFile = QSqlDatabase::addDatabase( "QSQLITE" );//第二个参数可以设置连接名字，这里为default
    }
    switch(indexof)
    {
        case SaleDB:
            dbFile.setDatabaseName(DBNAME_SALE);// 设置数据库名与路径, 此时是放在上一个目录

            break;
        case SettingDB:
            dbFile.setDatabaseName(DBNAME_SETTING);// 设置数据库名与路径, 此时是放在上一个目录

            break;
        case CalDB:
            dbFile.setDatabaseName(DBNAME_CAL);// 设置数据库名与路径, 此时是放在上一个目录
            break;
        case PluDB:
             dbFile.setDatabaseName(DBNAME_PLU);// 设置数据库名与路径, 此时是放在上一个目录

             break;
        case LogDB:
            dbFile.setDatabaseName(DBNAME_LOG);// 设置数据库名与路径, 此时是放在上一个目录

            break;
        default:
            {
               // pthread_mutex_unlock (&db_mutex);
                return 0;
            }
    }
    if(!dbFile.open())
    {
        qDebug() << dbFile.lastError();
        //pthread_mutex_unlock (&db_mutex);
        return 0;
    }
    else
    {
       qDebug("Connect Database success");
    }
    //pthread_mutex_unlock (&db_mutex);
    return 1;
}
int SQLiteDeal::CloseDB(void)
{
    this->dbFile.close();
}

int SQLiteDeal::insert_Defalt_data(char *TableName,QSqlQuery *SqlQuery)
{
    int table_size = 0;
    QString InsertTable;

    if(0 == strcmp(TableName,TABLE_CALIBRATION))
    {
        InsertTable.clear();
        InsertTable.append("insert into TABLE_CALIBRATION VALUES ('1','15','5','2','5','95536','95536','6780','35179','1');\n");
        qDebug() << InsertTable;
        if(!SqlQuery->exec(InsertTable))
           qDebug() << SqlQuery->lastError();

        InsertTable.clear();
        InsertTable.append("insert into TABLE_CALIBRATION VALUES ('2','150','50','20','50','95536','95536','5600','12083','1');");
        qDebug() << InsertTable;

        if(!SqlQuery->exec(InsertTable))
           qDebug() << SqlQuery->lastError();

    }
    else if(0 == strcmp(TableName,TABLE_USERSETTING))
    {
        table_size = sizeof(data_table_userSetting) / sizeof(const char *) / 4;
        for(int i = 0; i < table_size; i++)
        {
            InsertTable.clear();
            InsertTable.append("INSERT INTO TABLE_USERSETTING VALUES ('");
            InsertTable.append(data_table_userSetting[i][0]);
            InsertTable.append("','");
            InsertTable.append(data_table_userSetting[i][1]);
            InsertTable.append( "','");
            InsertTable.append(data_table_userSetting[i][2]);
            InsertTable.append( "','");
            InsertTable.append(data_table_userSetting[i][3]);
            InsertTable.append("');\n");
            qDebug() << InsertTable;
            if(!SqlQuery->exec(InsertTable))
               qDebug() << SqlQuery->lastError();

        }

    }
    else if(0 == strcmp(TableName,TABLE_SCALESETTING))
    {
        table_size = sizeof(data_table_scaleSetting) / sizeof(const char *) / 4;
        for(int i = 0; i < table_size; i++)
        {
            InsertTable.clear();
            InsertTable.append("INSERT INTO TABLE_SCALESETTING VALUES ('");
            InsertTable.append(data_table_scaleSetting[i][0]);
            InsertTable.append("','");
            InsertTable.append(data_table_scaleSetting[i][1]);
            InsertTable.append( "','");
            InsertTable.append(data_table_scaleSetting[i][2]);
            InsertTable.append( "','");
            InsertTable.append(data_table_scaleSetting[i][3]);
            InsertTable.append("');\n");
            qDebug() << InsertTable;
            if(!SqlQuery->exec(InsertTable))
               qDebug() << SqlQuery->lastError();

        }
    }
    return 0;
}

int SQLiteDeal::CreatDBAndTable(int indexof)
{
    this->OpenDB(indexof);
    QString CreatTable;         //SQL语句
    int table_size = 0;         //表中字段个数
    QSqlQuery SqlQuery(dbFile); //执行SQL语句类

    switch(indexof)
    {
        case SaleDB:
            {
                 //----------------------------TABLE_ATTACH------------------------//
                CreatTable.clear();
                table_size = sizeof(table_attach)/sizeof(const char *) /2;
                CreatTable.append("CREATE TABLE ");
                CreatTable.append(TABLE_ATTACH);
                CreatTable.append(" (");
                for(int i = 0; i<table_size;i++)
                {
                    if(i>0 && i<table_size -1)
                         CreatTable.append("\t");

                    CreatTable.append(table_attach[i][0]);
                    CreatTable.append("\t\t");
                    CreatTable.append(table_attach[i][1]);
                    if(i<table_size -1)
                        CreatTable.append(",\n");
                    else
                       CreatTable.append(" );\n");
                }
                qDebug() << CreatTable;
                SqlQuery.clear();
                if(!SqlQuery.exec(CreatTable))
                {
                   qDebug() << SqlQuery.lastError();
                   this->CloseDB();
                   return 0;
                }
                else
                {
                   qDebug() << "--- TABLE_ATTACH creat success ---\n";
                }
                //----------------------------TABLE_BUSINESS------------------------//
                CreatTable.clear();
                table_size = sizeof(table_business)/sizeof(const char *) /2;
                CreatTable.append("CREATE TABLE ");
                CreatTable.append(TABLE_BUSINESS);
                CreatTable.append(" (");
                for(int i = 0; i<table_size;i++)
                {
                    if(i>0 && i<table_size -1)
                         CreatTable.append("\t");

                    CreatTable.append(table_business[i][0]);
                    CreatTable.append("\t\t");
                    CreatTable.append(table_business[i][1]);
                    if(i<table_size -1)
                        CreatTable.append(",\n");
                    else
                       CreatTable.append(" );\n");
                }
                qDebug() << CreatTable;
                SqlQuery.clear();
                if(!SqlQuery.exec(CreatTable))
                {
                   qDebug() << SqlQuery.lastError();
                   this->CloseDB();
                   return 0;
                }
                else
                {
                   qDebug() << "--- TABLE_BUSINESS creat success ---\n";
                }
            }
            break;
        case PluDB:
            {
                //----------------------------TABLE_PRODUCT------------------------//
                CreatTable.clear();
                table_size = sizeof(table_product)/sizeof(const char *) /2;
                CreatTable.append("CREATE TABLE ");
                CreatTable.append(TABLE_PRODUCT);
                CreatTable.append(" (");
                for(int i = 0; i<table_size;i++)
                {
                    if(i>0 && i<table_size -1)
                         CreatTable.append("\t");

                    CreatTable.append(table_product[i][0]);
                    CreatTable.append("\t\t");
                    CreatTable.append(table_product[i][1]);
                    if(i<table_size -1)
                        CreatTable.append(",\n");
                    else
                       CreatTable.append(" );\n");
                }
                qDebug() << CreatTable;
                SqlQuery.clear();
                if(!SqlQuery.exec(CreatTable))
                {
                   qDebug() << SqlQuery.lastError();
                   this->CloseDB();
                   return 0;
                }
                else
                {
                   qDebug() << "--- TABLE_PRODUCT creat success ---\n";
                }
                //--------------------------TABLE_CONVERT--------------------------//
                CreatTable.clear();
                table_size = sizeof(table_convert)/sizeof(const char *) /2;
                CreatTable.append("CREATE TABLE ");
                CreatTable.append(TABLE_CONVERT);
                CreatTable.append(" (");
                for(int i = 0; i<table_size;i++)
                {
                    if(i>0 && i<table_size -1)
                         CreatTable.append("\t");

                    CreatTable.append(table_convert[i][0]);
                    CreatTable.append("\t\t");
                    CreatTable.append(table_convert[i][1]);
                    if(i<table_size -1)
                        CreatTable.append(",\n");
                    else
                       CreatTable.append(" );\n");
                }
                qDebug() << CreatTable;
                SqlQuery.clear();
                if(!SqlQuery.exec(CreatTable))
                {
                   qDebug() << SqlQuery.lastError();
                   this->CloseDB();
                   return 0;
                }
                else
                {
                   qDebug() << "--- TABLE_CONVERT creat success ---\n";
                }
                //--------------------------TABLE_BATCH--------------------------//
                CreatTable.clear();
                table_size = sizeof(table_batch)/sizeof(const char *) /2;
                CreatTable.append("CREATE TABLE ");
                CreatTable.append(TABLE_BATCH);
                CreatTable.append(" (");
                for(int i = 0; i<table_size;i++)
                {
                    if(i>0 && i<table_size -1)
                         CreatTable.append("\t");

                    CreatTable.append(table_batch[i][0]);
                    CreatTable.append("\t\t");
                    CreatTable.append(table_batch[i][1]);
                    if(i<table_size -1)
                        CreatTable.append(",\n");
                    else
                       CreatTable.append(" );\n");
                }
                qDebug() << CreatTable;
                SqlQuery.clear();
                if(!SqlQuery.exec(CreatTable))
                {
                   qDebug() << SqlQuery.lastError();
                   this->CloseDB();
                   return 0;
                }
                else
                {
                   qDebug() << "--- TABLE_BATCH creat success ---\n";
                }
                //--------------------------TABLE_IDENTIFY--------------------------//
                CreatTable.clear();
                table_size = sizeof(table_identify)/sizeof(const char *) /2;
                CreatTable.append("CREATE TABLE ");
                CreatTable.append(TABLE_IDENTIFY);
                CreatTable.append(" (");
                for(int i = 0; i<table_size;i++)
                {
                    if(i>0 && i<table_size -1)
                         CreatTable.append("\t");

                    CreatTable.append(table_identify[i][0]);
                    CreatTable.append("\t\t");
                    CreatTable.append(table_identify[i][1]);
                    if(i<table_size -1)
                        CreatTable.append(",\n");
                    else
                       CreatTable.append(" );\n");
                }
                qDebug() << CreatTable;
                SqlQuery.clear();
                if(!SqlQuery.exec(CreatTable))
                {
                   qDebug() << SqlQuery.lastError();
                   this->CloseDB();
                   return 0;
                }
                else
                {
                   qDebug() << "--- TABLE_IDENTIFY creat success ---\n";
                }
            }
            break;
        case CalDB:
            {
                //----------------------------TABLE_CALIBRATION------------------------//
                CreatTable.clear();
                table_size = sizeof(table_calibration)/sizeof(const char *) /2;
                CreatTable.append("CREATE TABLE ");
                CreatTable.append(TABLE_CALIBRATION);
                CreatTable.append(" (");
                for(int i = 0; i<table_size;i++)
                {
                    CreatTable.append(table_calibration[i][0]);
                    CreatTable.append("\t\t");
                    CreatTable.append(table_calibration[i][1]);
                    if(i==(table_size -1))
                        CreatTable.append(");");
                    else
                        CreatTable.append(",\n");
                }
                qDebug() << CreatTable;
                SqlQuery.clear();
                if(!SqlQuery.exec(CreatTable))
                {
                   qDebug() << SqlQuery.lastError();
                   this->CloseDB();;
                   return 0;
                }
                else
                {
                   qDebug() << "--- TABLE_CALIBRATION creat success ---\n";
                }
                insert_Defalt_data(TABLE_CALIBRATION,&SqlQuery);
            }
            break;
        case SettingDB:
            {
                //----------------------------TABLE_SCALESETTING------------------------//
                CreatTable.clear();
                table_size = sizeof(table_scaleSetting)/sizeof(const char *) /2;
                CreatTable.append("CREATE TABLE ");
                CreatTable.append(TABLE_SCALESETTING);
                CreatTable.append(" (");
                for(int i = 0; i<table_size;i++)
                {
                    if(i>0 && i<table_size -1)
                         CreatTable.append("\t");

                    CreatTable.append(table_scaleSetting[i][0]);
                    CreatTable.append("\t\t");
                    CreatTable.append(table_scaleSetting[i][1]);
                    if(i<table_size -1)
                        CreatTable.append(",\n");
                    else
                       CreatTable.append(" );\n");
                }
                qDebug() << CreatTable;
                SqlQuery.clear();
                if(!SqlQuery.exec(CreatTable))
                {
                   qDebug() << SqlQuery.lastError();
                   this->CloseDB();
                   return 0;
                }
                else
                {
                   qDebug() << "--- TABLE_SCALESETTING creat success ---\n";
                   insert_Defalt_data(TABLE_SCALESETTING,&SqlQuery);
                }
                 //----------------------------TABLE_USERSETTING------------------------//
                CreatTable.clear();
                table_size = sizeof(table_userSetting)/sizeof(const char *) /2;
                CreatTable.append("CREATE TABLE ");
                CreatTable.append(TABLE_USERSETTING);
                CreatTable.append(" (");
                for(int i = 0; i<table_size;i++)
                {
                    if(i>0 && i<table_size -1)
                         CreatTable.append("\t");

                    CreatTable.append(table_userSetting[i][0]);
                    CreatTable.append("\t\t");
                    CreatTable.append(table_userSetting[i][1]);
                    if(i<table_size -1)
                        CreatTable.append(",\n");
                    else
                       CreatTable.append(" );\n");
                }
                qDebug() << CreatTable;
                SqlQuery.clear();
                if(!SqlQuery.exec(CreatTable))
                {
                   qDebug() << SqlQuery.lastError();
                   this->CloseDB();
                   return 0;
                }
                else
                {
                   qDebug() << "--- TABLE_USERSETTING creat success ---\n";
                   insert_Defalt_data(TABLE_USERSETTING,&SqlQuery);
                }
                //----------------------------TABLE_SELLERSINFO------------------------//
                CreatTable.clear();
                table_size = sizeof(table_sellerInfo)/sizeof(const char *) /2;
                CreatTable.append("CREATE TABLE ");
                CreatTable.append(TABLE_SELLERSINFO);
                CreatTable.append(" (");
                for(int i = 0; i<table_size;i++)
                {
                    if(i>0 && i<table_size -1)
                         CreatTable.append("\t");

                    CreatTable.append(table_sellerInfo[i][0]);
                    CreatTable.append("\t\t");
                    CreatTable.append(table_sellerInfo[i][1]);
                    if(i<table_size -1)
                        CreatTable.append(",\n");
                    else
                       CreatTable.append(" );\n");
                }
                qDebug() << CreatTable;
                SqlQuery.clear();
                if(!SqlQuery.exec(CreatTable))
                {
                   qDebug() << SqlQuery.lastError();
                }
                 else
                {
                   qDebug() << "--- TABLE_SELLERSINFO creat success ---\n";
                }
            }
            break;
        case LogDB:
            {
                 //----------------------------TABLE_SCALELOG------------------------//
                CreatTable.clear();
                table_size = sizeof(table_scaleLog)/sizeof(const char *) /2;
                CreatTable.append("CREATE TABLE ");
                CreatTable.append(TABLE_SCALELOG);
                CreatTable.append(" (");
                for(int i = 0; i<table_size;i++)
                {
                    CreatTable.append(table_scaleLog[i][0]);
                    CreatTable.append("\t\t");
                    CreatTable.append(table_scaleLog[i][1]);
                    if(i==(table_size -1))
                        CreatTable.append(");");
                    else
                        CreatTable.append(",\n");
                }
                qDebug() << CreatTable;
                SqlQuery.clear();
                if(!SqlQuery.exec(CreatTable))
                {
                   qDebug() << SqlQuery.lastError();
                   this->CloseDB();
                   return 0;
                }
                else
                {
                   qDebug() << "--- TABLE_SCALELOG creat success ---\n";
                }
            }
            break;
        default:
            {
                this->CloseDB();
                return 0;
            }
    }
    this->CloseDB();
    return 1;
}
int SQLiteDeal::Select_ReturnList_QSqlQuery_By_SQL_Statements(int db,QString SQL_Statements, QSqlQuery *SqlQuery)
{
    pthread_mutex_lock (&mutex);
    //pthread_mutex_unlock (&mutex);
    OpenDB(db);
    int numRows = 0;
    qDebug() << SQL_Statements;
    QSqlQuery Query(dbFile);
    Query.prepare(SQL_Statements);
    if(!Query.exec())
    {
       qDebug() << Query.lastError();
       this->CloseDB();
       Query.clear();
       //pthread_mutex_lock (&mutex);
       pthread_mutex_unlock (&mutex);
       return -1;
    }
    if (dbFile.driver()->hasFeature(QSqlDriver::QuerySize))
    {
        numRows = Query.size();  //如果支持结果影响的行数，那么直接记录下来
    }
    else
    {
        Query.last(); //否则定位到结果最后，qt 文档说，这个方法非常慢
        numRows = Query.at() + 1;
        Query.seek(-1);
    }
    *SqlQuery = Query;
    this->CloseDB();
    Query.clear();
    //pthread_mutex_lock (&mutex);
    pthread_mutex_unlock (&mutex);
    return numRows;
}
int SQLiteDeal::Select_ReturnQString_By_SQL_Statements(int db,QString SQL_Statements,QString *ReturnQString)
{
    pthread_mutex_lock (&mutex);
    //pthread_mutex_unlock (&mutex);
    OpenDB(db);
    qDebug() << SQL_Statements;
    QSqlQuery Query(dbFile);
    if(!Query.exec(SQL_Statements))
    {
       qDebug() << Query.lastError();
       this->CloseDB();
       Query.clear();
       //pthread_mutex_lock (&mutex);
       pthread_mutex_unlock (&mutex);
       return -1;
    }
    while(Query.next())
    {
        QString tempQString = Query.value(0).toString();
        *ReturnQString = tempQString ;
        this->CloseDB();
        Query.clear();
        //pthread_mutex_lock (&mutex);
        pthread_mutex_unlock (&mutex);
        return 1;
    }
    this->CloseDB();
    Query.clear();
    //pthread_mutex_lock (&mutex);
    pthread_mutex_unlock (&mutex);
    return 0;
}
int SQLiteDeal::Insert_DataList_By_TableName_DataList(int db,char *TableName,void * inData,int size)
{
    if(size<=0)
        return 0;
    pthread_mutex_lock (&mutex);
    //事务是用户定义的一个数据库操作序列，这些操作要么全做要么全不做，是一个不可分割的工作单位。
    if(dbFile.driver()->hasFeature(QSqlDriver::Transactions))       //先判断该数据库是否支持事务操作
    {
           this->OpenDB(db);
           dbFile.transaction();//开始启动事务
           QSqlQuery Query(dbFile);
           QString Insert_SQL;
           QString maxid("select max(id) from ");
           int id=0;
           if(0 == strcmp(TableName,TABLE_PRODUCT))
           {
               maxid.append(TableName);
               if(!Query.exec(maxid))
               {
                  qDebug() << Query.lastError();
                  this->CloseDB();
                  Query.clear();
                  pthread_mutex_unlock(&mutex);
                  return -1;
               }
               Query.next();
               id = Query.value(0).toString().toInt();

               struct_tb_product *inDataTemp = (struct_tb_product*)inData;
               Insert_SQL.append("insert into ");
               Insert_SQL.append(TableName);
               Insert_SQL.append(" values ");
               Insert_SQL.append("(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
               Query.prepare(Insert_SQL);

               for(int i=0;i<size;i++)
               {
                   Query.bindValue(0,++id);
                   Query.bindValue(1,inDataTemp[i].goodsCode);
                   Query.bindValue(2,inDataTemp[i].goodsName);
                   Query.bindValue(3,inDataTemp[i].goodsOrigin);
                   Query.bindValue(4,inDataTemp[i].pluCode);
                   Query.bindValue(5,inDataTemp[i].goodsKey);
                   Query.bindValue(6,inDataTemp[i].goodsWeight);
                   Query.bindValue(7,inDataTemp[i].groupNo);
                   Query.bindValue(8,inDataTemp[i].version);
                   Query.bindValue(9,inDataTemp[i].batchCode);
                   Query.bindValue(10,inDataTemp[i].traceCode);
                   Query.bindValue(11,inDataTemp[i].maxPrice);
                   Query.bindValue(12,inDataTemp[i].minPrice);
                   Query.bindValue(13,inDataTemp[i].lastPrice);
                   Query.bindValue(14,inDataTemp[i].isPcs);
                   Query.bindValue(15,inDataTemp[i].isLimit);
                   Query.bindValue(16,inDataTemp[i].productX);
                   Query.bindValue(17,inDataTemp[i].productY);
                   Query.bindValue(18,inDataTemp[i].productZ);

                   if(!Query.exec())
                   {
                       qDebug() << Query.lastError();
                       this->CloseDB();
                       Query.clear();
                       pthread_mutex_unlock(&mutex);
                       return -1;
                   }
               }
           }
           else if(0 == strcmp(TableName,TABLE_CONVERT))
           {
               maxid.append(TableName);
               if(!Query.exec(maxid))
               {
                  qDebug() << Query.lastError();
                  this->CloseDB();
                  Query.clear();
                  pthread_mutex_unlock(&mutex);
                  return -1;
               }
               Query.next();
               id = Query.value(0).toString().toInt();

               struct_tb_convert *inDataTemp = (struct_tb_convert*)inData;
               Insert_SQL.append("insert into ");
               Insert_SQL.append(TableName);
               Insert_SQL.append(" values ");
               Insert_SQL.append("(?,?,?,?,?,?);");

               Query.prepare(Insert_SQL);

               for(int i=0;i<size;i++)
               {
                   Query.bindValue(0,++id);
                   Query.bindValue(1,inDataTemp[i].productId);
                   Query.bindValue(2,inDataTemp[i].hotKey);
                   Query.bindValue(3,inDataTemp[i].goodsCode);
                   Query.bindValue(4,inDataTemp[i].bSave);
                   Query.bindValue(5,inDataTemp[i].convertZ);

                   if(!Query.exec())
                   {
                       qDebug() << Query.lastError();
                       this->CloseDB();
                       Query.clear();
                       pthread_mutex_unlock(&mutex);
                       return -1;
                   }
               }
           }
           else if(0 == strcmp(TableName,TABLE_BATCH))
           {
               maxid.append(TableName);
               if(!Query.exec(maxid))
               {
                  qDebug() << Query.lastError();
                  this->CloseDB();
                  Query.clear();
                  pthread_mutex_unlock(&mutex);
                  return -1;
               }
               Query.next();
               id = Query.value(0).toString().toInt();

               struct_tb_batch *inDataTemp = (struct_tb_batch*)inData;
               Insert_SQL.append("insert into ");
               Insert_SQL.append(TableName);
               Insert_SQL.append(" values ");
               Insert_SQL.append("(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);");

               Query.prepare(Insert_SQL);

               for(int i=0;i<size;i++)
               {
                   Query.bindValue(0,++id);
                   Query.bindValue(1,inDataTemp[i].goodsCode);
                   Query.bindValue(2,inDataTemp[i].goodsName);
                   Query.bindValue(3,inDataTemp[i].originCode);
                   Query.bindValue(4,inDataTemp[i].originName);
                   Query.bindValue(5,inDataTemp[i].batchCode);
                   Query.bindValue(6,inDataTemp[i].traceCode);
                   Query.bindValue(7,inDataTemp[i].proprietorCode);
                   Query.bindValue(8,inDataTemp[i].time);
                   Query.bindValue(9,inDataTemp[i].groupNo);
                   Query.bindValue(10,inDataTemp[i].weight);
                   Query.bindValue(11,inDataTemp[i].zone);
                   Query.bindValue(12,inDataTemp[i].isPass);
                   Query.bindValue(13,inDataTemp[i].bSave);
                   Query.bindValue(14,inDataTemp[i].batchY);
                   Query.bindValue(15,inDataTemp[i].batchZ);

                   if(!Query.exec())
                   {
                       qDebug() << Query.lastError();
                       this->CloseDB();
                       Query.clear();
                       pthread_mutex_unlock(&mutex);
                       return -1;
                   }
               }
           }
           else if(0 == strcmp(TableName,TABLE_IDENTIFY))
           {
               maxid.append(TableName);
               if(!Query.exec(maxid))
               {
                  qDebug() << Query.lastError();
                  this->CloseDB();
                  Query.clear();
                  pthread_mutex_unlock(&mutex);
                  return -1;
               }
               Query.next();
               id = Query.value(0).toString().toInt();

               struct_tb_identify *inDataTemp = (struct_tb_identify*)inData;
               Insert_SQL.append("insert into ");
               Insert_SQL.append(TableName);
               Insert_SQL.append(" values ");
               Insert_SQL.append("(?,?,?,?,?);");

               Query.prepare(Insert_SQL);

               for(int i=0;i<size;i++)
               {
                   Query.bindValue(0,++id);
                   Query.bindValue(1,inDataTemp[i].identifyCode);
                   Query.bindValue(2,inDataTemp[i].goodsCode);
                   Query.bindValue(3,inDataTemp[i].goodsName);
                   Query.bindValue(4,inDataTemp[i].identifyZ);
                   if(!Query.exec())
                   {
                       qDebug() << Query.lastError();
                       this->CloseDB();
                       Query.clear();
                       return -1;
                   }
               }
           }
           else if(0 == strcmp(TableName,TABLE_ATTACH))
           {
               maxid.append(TableName);
               if(!Query.exec(maxid))
               {
                  qDebug() << Query.lastError();
                  this->CloseDB();
                  Query.clear();
                  pthread_mutex_unlock(&mutex);
                  return -1;
               }
               Query.next();
               id = Query.value(0).toString().toInt();

               struct_tb_attach *inDataTemp = (struct_tb_attach*)inData;
               Insert_SQL.append("insert into ");
               Insert_SQL.append(TableName);
               Insert_SQL.append(" values ");
               Insert_SQL.append("(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);");

               Query.prepare(Insert_SQL);

               for(int i=0;i<size;i++)
               {
                   Query.bindValue(0,++id);
                   Query.bindValue(1,inDataTemp[i].businessCode);
                   Query.bindValue(2,inDataTemp[i].saleNum);
                   Query.bindValue(3,inDataTemp[i].payType);
                   Query.bindValue(4,inDataTemp[i].payOrder);
                   Query.bindValue(5,inDataTemp[i].cardType);
                   Query.bindValue(6,inDataTemp[i].cardNum);
                   Query.bindValue(7,inDataTemp[i].deviceType);
                   Query.bindValue(8,inDataTemp[i].deviceNum);
                   Query.bindValue(9,inDataTemp[i].serial);
                   Query.bindValue(10,inDataTemp[i].subTotal);
                   Query.bindValue(11,inDataTemp[i].payMoney);
                   Query.bindValue(12,inDataTemp[i].sellerInfo);
                   Query.bindValue(13,inDataTemp[i].buyerInfo);
                   Query.bindValue(14,inDataTemp[i].scaleTime);
                   Query.bindValue(15,inDataTemp[i].context);
                   Query.bindValue(16,inDataTemp[i].dealState);
                   Query.bindValue(17,inDataTemp[i].isPrint);
                   Query.bindValue(18,inDataTemp[i].webUp);
                   Query.bindValue(19,inDataTemp[i].windUp);
                   Query.bindValue(20,inDataTemp[i].attachX);
                   Query.bindValue(21,inDataTemp[i].attachY);
                   Query.bindValue(22,inDataTemp[i].attachZ);

                   if(!Query.exec())
                   {
                       qDebug() << Query.lastError();
                       this->CloseDB();
                       Query.clear();
                       pthread_mutex_unlock(&mutex);
                       return -1;
                   }
               }
           }
           else if(0 == strcmp(TableName,TABLE_BUSINESS))
           {
               maxid.append(TableName);
               if(!Query.exec(maxid))
               {
                  qDebug() << Query.lastError();
                  this->CloseDB();
                  Query.clear();
                  pthread_mutex_unlock(&mutex);
                  return -1;
               }
               Query.next();
               id = Query.value(0).toString().toInt();

               struct_tb_business *inDataTemp = (struct_tb_business*)inData;
               Insert_SQL.append("insert into ");
               Insert_SQL.append(TableName);
               Insert_SQL.append(" values ");
               Insert_SQL.append("(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);");

               Query.prepare(Insert_SQL);

               for(int i=0;i<size;i++)
               {
                   Query.bindValue(0,++id);
                   Query.bindValue(1,inDataTemp[i].serialNum);
                   Query.bindValue(2,inDataTemp[i].businessCode);
                   Query.bindValue(3,inDataTemp[i].payType);
                   Query.bindValue(4,inDataTemp[i].goodsCode);
                   Query.bindValue(5,inDataTemp[i].goodsName);
                   Query.bindValue(6,inDataTemp[i].goodsOrigin);
                   Query.bindValue(7,inDataTemp[i].batchCode);
                   Query.bindValue(8,inDataTemp[i].traceCode);
                   Query.bindValue(9,inDataTemp[i].weight);
                   Query.bindValue(10,inDataTemp[i].price);
                   Query.bindValue(11,inDataTemp[i].pieces);
                   Query.bindValue(12,inDataTemp[i].total);
                   Query.bindValue(13,inDataTemp[i].time);
                   Query.bindValue(14,inDataTemp[i].mode);
                   Query.bindValue(15,inDataTemp[i].cancel);
                   Query.bindValue(16,inDataTemp[i].picture);
                   Query.bindValue(17,inDataTemp[i].allWeight);
                   Query.bindValue(18,inDataTemp[i].isOverSale);
                   Query.bindValue(19,inDataTemp[i].webUp);
                   Query.bindValue(20,inDataTemp[i].windUp);
                   Query.bindValue(21,inDataTemp[i].businessY);
                   Query.bindValue(22,inDataTemp[i].businessZ);

                   if(!Query.exec())
                   {
                       qDebug() << Query.lastError();
                       this->CloseDB();
                       Query.clear();
                       pthread_mutex_unlock(&mutex);
                       return -1;
                   }
               }
           }
           else if(0 == strcmp(TableName,TABLE_CALIBRATION))
           {
               maxid.append(TableName);
               if(!Query.exec(maxid))
               {
                  qDebug() << Query.lastError();
                  this->CloseDB();
                  Query.clear();
                  pthread_mutex_unlock(&mutex);
                  return -1;
               }
               Query.next();
               id = Query.value(0).toString().toInt();

               struct_tb_calibration *inDataTemp = (struct_tb_calibration*)inData;
               Insert_SQL.append("insert into ");
               Insert_SQL.append(TableName);
               Insert_SQL.append(" values ");
               Insert_SQL.append("(?,?,?,?,?,?,?,?,?,?);");

               Query.prepare(Insert_SQL);

               for(int i=0;i<size;i++)
               {
                   Query.bindValue(0,++id);
                   Query.bindValue(1,inDataTemp[i].fullArrange);
                   Query.bindValue(2,inDataTemp[i].calArrange);
                   Query.bindValue(3,inDataTemp[i].e);
                   Query.bindValue(4,inDataTemp[i].e1);
                   Query.bindValue(5,inDataTemp[i].localGravity);
                   Query.bindValue(6,inDataTemp[i].destGravity);
                   Query.bindValue(7,inDataTemp[i].unLoad);
                   Query.bindValue(8,inDataTemp[i].load);
                   Query.bindValue(9,inDataTemp[i].isDual);

                   if(!Query.exec())
                   {
                       qDebug() << Query.lastError();
                       this->CloseDB();
                       Query.clear();
                       pthread_mutex_unlock(&mutex);
                       return -1;
                   }
               }
           }
           else if(0 == strcmp(TableName,TABLE_SCALESETTING))
           {
               maxid.append(TableName);
               if(!Query.exec(maxid))
               {
                  qDebug() << Query.lastError();
                  this->CloseDB();
                  Query.clear();
                  pthread_mutex_unlock(&mutex);
                  return -1;
               }
               Query.next();
               id = Query.value(0).toString().toInt();

               struct_tb_scaleSetting *inDataTemp = (struct_tb_scaleSetting*)inData;
               Insert_SQL.append("insert into ");
               Insert_SQL.append(TableName);
               Insert_SQL.append(" values ");
               Insert_SQL.append("(?,?,?,?);");

               Query.prepare(Insert_SQL);

               for(int i=0;i<size;i++)
               {
                   Query.bindValue(0,++id);
                   Query.bindValue(1,inDataTemp[i].name);
                   Query.bindValue(2,inDataTemp[i].value);
                   Query.bindValue(3,inDataTemp[i].upper);

                   if(!Query.exec())
                   {
                       qDebug() << Query.lastError();
                       this->CloseDB();
                       Query.clear();
                       pthread_mutex_unlock(&mutex);
                       return -1;
                   }
               }
           }
           else if(0 == strcmp(TableName,TABLE_USERSETTING))
           {
               maxid.append(TableName);
               if(!Query.exec(maxid))
               {
                  qDebug() << Query.lastError();
                  this->CloseDB();
                  Query.clear();
                  pthread_mutex_unlock(&mutex);
                  return -1;
               }
               Query.next();
               id = Query.value(0).toString().toInt();

               struct_tb_userSetting *inDataTemp = (struct_tb_userSetting*)inData;
               Insert_SQL.append("insert into ");
               Insert_SQL.append(TableName);
               Insert_SQL.append(" values ");
               Insert_SQL.append("(?,?,?,?);");

               Query.prepare(Insert_SQL);

               for(int i=0;i<size;i++)
               {
                   Query.bindValue(0,++id);
                   Query.bindValue(1,inDataTemp[i].name);
                   Query.bindValue(2,inDataTemp[i].value);
                   Query.bindValue(3,inDataTemp[i].upper);

                   if(!Query.exec())
                   {
                       qDebug() << Query.lastError();
                       this->CloseDB();
                       Query.clear();
                       pthread_mutex_unlock(&mutex);
                       return -1;
                   }
               }
           }
           else if(0 == strcmp(TableName,TABLE_SELLERSINFO))
           {
               maxid.append(TableName);
               if(!Query.exec(maxid))
               {
                  qDebug() << Query.lastError();
                  this->CloseDB();
                  Query.clear();
                  pthread_mutex_unlock(&mutex);
                  return -1;
               }
               Query.next();
               id = Query.value(0).toString().toInt();

               struct_tb_seller_info *inDataTemp = (struct_tb_seller_info*)inData;
               Insert_SQL.append("insert into ");
               Insert_SQL.append(TableName);
               Insert_SQL.append(" values ");
               Insert_SQL.append("(?,?,?,?,?);");

               Query.prepare(Insert_SQL);

               for(int i=0;i<size;i++)
               {
                   Query.bindValue(0,++id);
                   Query.bindValue(1,inDataTemp[i].sellerCard);
                   Query.bindValue(2,inDataTemp[i].proprietorCode);
                   Query.bindValue(3,inDataTemp[i].name);
                   Query.bindValue(4,inDataTemp[i].pass);

                   if(!Query.exec())
                   {
                       qDebug() << Query.lastError();
                       this->CloseDB();
                       Query.clear();
                       pthread_mutex_unlock(&mutex);
                       return -1;
                   }
               }
           }
           else if(0 == strcmp(TableName,TABLE_SCALELOG))
           {
               maxid.append(TableName);
               if(!Query.exec(maxid))
               {
                  qDebug() << Query.lastError();
                  this->CloseDB();
                  Query.clear();
                  pthread_mutex_unlock(&mutex);
                  return -1;
               }
               Query.next();
               id = Query.value(0).toString().toInt();

               struct_tb_log *inDataTemp = (struct_tb_log*)inData;
               Insert_SQL.append("insert into ");
               Insert_SQL.append(TableName);
               Insert_SQL.append(" values ");
               Insert_SQL.append("(?,?,?,?,?);");

               Query.prepare(Insert_SQL);

               for(int i=0;i<size;i++)
               {
                   Query.bindValue(0,++id);
                   Query.bindValue(1,inDataTemp[i].name);
                   Query.bindValue(2,inDataTemp[i].value1);
                   Query.bindValue(3,inDataTemp[i].value2);
                   Query.bindValue(4,inDataTemp[i].upper);

                   if(!Query.exec())
                   {
                       qDebug() << Query.lastError();
                       this->CloseDB();
                       Query.clear();
                       pthread_mutex_unlock(&mutex);
                       return -1;
                   }
               }
           }
           Query.clear();
    }
    else
    {
        qDebug()<< "---[SQL]:数据库不支持事务操作\n";
       this->CloseDB();
       return 0;
    }
    if(!dbFile.commit())
    {
        qDebug() << "commit:" << dbFile.lastError();
        if(!QSqlDatabase::database().rollback())
        {
            qDebug() << QSqlDatabase::database().lastError(); //回滚
        }
    }
    this->CloseDB();
    pthread_mutex_unlock(&mutex);
    return 1;
}
int SQLiteDeal::Excute_SQL_Statements(int db,QString SQL_Statements)
{
    qDebug() << "perform:" << SQL_Statements;
    pthread_mutex_lock(&mutex);
    if(SQL_Statements.length()<=0)
    {
        return 0;
    }
    OpenDB(db);
    QSqlQuery Query(dbFile);
    if(!Query.exec(SQL_Statements))
    {
       qDebug() << Query.lastError();
       this->CloseDB();
       Query.clear();
       pthread_mutex_unlock(&mutex);
       return -1;
    }
    this->CloseDB();
    Query.clear();
    pthread_mutex_unlock(&mutex);
    return 1;
}
