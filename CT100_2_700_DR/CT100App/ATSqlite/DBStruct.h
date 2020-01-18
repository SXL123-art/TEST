#ifndef _DB_STRUCT_
#define _DB_STRUCT_

#include <QString>
//==================================================================
//PLU表 // TABLE_PRODUCT
#define TABLE_PRODUCT                       "TABLE_PRODUCT"
#define TABLE_PRODUCT_ID                    "id"				//id
#define TABLE_PRODUCT_ID_TYPE               "int" //"INTEGER PRIMARY KEY AUTOINCREMENT"
#define TABLE_PRODUCT_GOODSCODE             "goodsCode"			//商品编号（国标）
#define TABLE_PRODUCT_GOODSCODE_LEN         32
#define TABLE_PRODUCT_GOODSCODE_TYPE        "varchar(32)"
#define TABLE_PRODUCT_GOODSNAME             "goodsName"			//商品名称
#define TABLE_PRODUCT_GOODSNAME_LEN         60
#define TABLE_PRODUCT_GOODSNAME_TYPE        "varchar(60)"
#define TABLE_PRODUCT_GOODSORIGIN           "goodsOrigin"               //
#define TABLE_PRODUCT_GOODSORIGIN_LEN       40
#define TABLE_PRODUCT_GOODSORIGIN_TYPE      "varchar(40)"
#define TABLE_PRODUCT_PLUCODE               "pluCode"
#define TABLE_PRODUCT_PLUCODE_LEN           32
#define TABLE_PRODUCT_PLUCODE_TYPE          "varchar(32)"
#define TABLE_PRODUCT_GOODSKEY              "goodsKey"			//
#define TABLE_PRODUCT_GOODSKEY_LEN          16
#define TABLE_PRODUCT_GOODSKEY_TYPE         "varchar(16)"
#define TABLE_PRODUCT_GOODSWEIHT            "goodsWeight"		//
#define TABLE_PRODUCT_GOODSWEIHT_LEN        9
#define TABLE_PRODUCT_GOODSWEIHT_TYPE       "varchar(9)"
#define TABLE_PRODUCT_GROUPNO               "groupNo"			//分组号
#define TABLE_PRODUCT_GROUPNO_LEN           16
#define TABLE_PRODUCT_GROUPNO_TYPE          "varchar(16)"
#define TABLE_PRODUCT_VERSION               "version"			//plu版本号
#define TABLE_PRODUCT_VERSION_LEN           8
#define TABLE_PRODUCT_VERSION_TYPE          "varchar(8)"
#define TABLE_PRODUCT_BATCHCODE             "batchCode"			//
#define TABLE_PRODUCT_BATCHCODE_LEN         32
#define TABLE_PRODUCT_BATCHCODE_TYPE        "varchar(32)"
#define TABLE_PRODUCT_TRACECODE             "traceCode"			//
#define TABLE_PRODUCT_TRACECODE_LEN         32
#define TABLE_PRODUCT_TRACECODE_TYPE        "varchar(32)"
#define TABLE_PRODUCT_MAXPRICE              "maxPrice"			//最高价格 not use
#define TABLE_PRODUCT_MAXPRICE_LEN          9
#define TABLE_PRODUCT_MAXPRICE_TYPE         "varchar(9)"
#define TABLE_PRODUCT_MINPRICE              "minPrice"			//最低价格 not use
#define TABLE_PRODUCT_MINPRICE_LEN          9
#define TABLE_PRODUCT_MINPRICE_TYPE         "varchar(9)"
#define TABLE_PRODUCT_LASTPRICE             "lastPrice"			//上一次成交价格
#define TABLE_PRODUCT_LASTPRICE_LEN         9
#define TABLE_PRODUCT_LASTPRICE_TYPE        "varchar(9)"
#define TABLE_PRODUCT_ISPCS                 "isPcs"
#define TABLE_PRODUCT_ISPCS_LEN             2
#define TABLE_PRODUCT_ISPCS_TYPE            "varchar(2)"
#define TABLE_PRODUCT_ISLIMIT               "isLimit"			//
#define TABLE_PRODUCT_ISLIMIT_LEN           2
#define TABLE_PRODUCT_ISLIMIT_TYPE          "varchar(2)"
#define TABLE_PRODUCT_PRODUCTX              "productX"			//
#define TABLE_PRODUCT_PRODUCTX_LEN          32
#define TABLE_PRODUCT_PRODUCTX_TYPE         "varchar(32)"
#define TABLE_PRODUCT_PRODUCTY              "productY"			//
#define TABLE_PRODUCT_PRODUCTY_LEN          32
#define TABLE_PRODUCT_PRODUCTY_TYPE         "varchar(32)"
#define TABLE_PRODUCT_PRODUCTZ              "productZ"			//
#define TABLE_PRODUCT_PRODUCTZ_LEN          32
#define TABLE_PRODUCT_PRODUCTZ_TYPE         "varchar(32)"

typedef struct
{
    int id;
    QString goodsCode;
    QString goodsName;
    QString goodsOrigin;
    QString pluCode;
    QString goodsKey;
    QString goodsWeight;
    QString groupNo;
    QString version;
    QString batchCode;
    QString traceCode;
    QString maxPrice;
    QString minPrice;
    QString lastPrice;
    QString isPcs;
    QString isLimit;
    QString productX;
    QString productY;
    QString productZ;

    void ClearAllData(void){
        goodsCode.clear();
        goodsName.clear();
        goodsOrigin.clear();
        goodsKey.clear();
        goodsWeight.clear();
        groupNo.clear();
        version.clear();
        batchCode.clear();
        traceCode.clear();
        maxPrice.clear();
        minPrice.clear();
        lastPrice.clear();
        isPcs.clear();
        isLimit.clear();
        productX.clear();
        productY.clear();
        productZ.clear();
    }
} struct_tb_product;

//==================================================================
//快捷键映射表 // TABLE_CONVERT
#define TABLE_CONVERT                       "TABLE_CONVERT"
#define TABLE_CONVERT_ID                    "id"		//id
#define TABLE_CONVERT_ID_TYPE               "int"//"INTEGER PRIMARY KEY AUTOINCREMENT"
#define TABLE_CONVERT_PRODUCTID             "productId"         //plu号
#define TABLE_CONVERT_PRODUCTID_TYPE        "int"
#define TABLE_CONVERT_HOTKEY                "hotKey"		//快捷键号
#define TABLE_CONVERT_HOTKEY_LEN            16
#define TABLE_CONVERT_HOTKEY_TYPE           "varchar(16)"
#define TABLE_CONVERT_GOODSCODE             "goodsCode"		//商品国标编码
#define TABLE_CONVERT_GOODSCODE_LEN         32
#define TABLE_CONVERT_GOODSCODE_TYPE        "varchar(32)"
#define TABLE_CONVERT_BSAVE                 "bSave"		//是否保存
#define TABLE_CONVERT_BSAVE_LEN             2
#define TABLE_CONVERT_BSAVE_TYPE            "varchar(2)"
#define TABLE_CONVERT_CONVERTZ              "convertZ"
#define TABLE_CONVERT_CONVERTZ_LEN          16
#define TABLE_CONVERT_CONVERTZ_TYPE         "varchar(16)"
typedef struct
{
    int id;
    int productId; //product 表的id字段
    QString hotKey;
    QString goodsCode;
    QString bSave;
    QString convertZ;
    void ClearAllData(void)
    {
        hotKey.clear();
        goodsCode.clear();
        bSave.clear();
        convertZ.clear();
    }
} struct_tb_convert;

//==================================================================
//批次-服务器下发数据-进场信息-追溯信息表 // TABLE_BATCH
#define TABLE_BATCH                         "TABLE_BATCH"
#define TABLE_BATCH_ID                      "id"			//id
#define TABLE_BATCH_ID_TYPE                 "int"//"INTEGER PRIMARY KEY AUTOINCREMENT"
#define TABLE_BATCH_GOODSCODE               "goodsCode"			//商品国标码
#define TABLE_BATCH_GOODSCODE_LEN           32
#define TABLE_BATCH_GOODSCODE_TYPE          "varchar(32)"
#define TABLE_BATCH_GOODSNAME               "goodsName"			//商品国标码
#define TABLE_BATCH_GOODSNAME_LEN           60
#define TABLE_BATCH_GOODSNAME_TYPE          "varchar(60)"
#define TABLE_BATCH_ORIGINCODE              "originCode"		//产地编码
#define TABLE_BATCH_ORIGINCODE_LEN          32
#define TABLE_BATCH_ORIGINCODE_TYPE         "varchar(32)"
#define TABLE_BATCH_ORIGINNAME              "originName"		//
#define TABLE_BATCH_ORIGINNAME_LEN          40
#define TABLE_BATCH_ORIGINNAME_TYPE         "varchar(40)"
#define TABLE_BATCH_BATCHCODE               "batchCode"                 //批次号
#define TABLE_BATCH_BATCHCODE_LEN           32
#define TABLE_BATCH_BATCHCODE_TYPE          "varchar(32)"
#define TABLE_BATCH_TRACECODE               "traceCode"			//追溯码
#define TABLE_BATCH_TRACECODE_LEN           32
#define TABLE_BATCH_TRACECODE_TYPE          "varchar(32)"
#define TABLE_BATCH_PROPRIETORCODE          "proprietorCode"            //卖方编号
#define TABLE_BATCH_PROPRIETORCODE_LEN      32
#define TABLE_BATCH_PROPRIETORCODE_TYPE     "varchar(32)"
#define TABLE_BATCH_TIME                    "time"			//时间戳
#define TABLE_BATCH_TIME_LEN                20
#define TABLE_BATCH_TIME_TYPE               "varchar(20)"
#define TABLE_BATCH_GROUPNO                 "groupNo"
#define TABLE_BATCH_GROUPNO_LEN             8
#define TABLE_BATCH_GROUPNO_TYPE            "varchar(8)"
#define TABLE_BATCH_WEIGHT                  "weight"			//重量
#define TABLE_BATCH_WEIGHT_LEN              9
#define TABLE_BATCH_WEIGHT_TYPE             "varchar(9)"
#define TABLE_BATCH_ZONE                    "zone"
#define TABLE_BATCH_ZONE_LEN                10
#define TABLE_BATCH_ZONE_TYPE               "varchar(10)"
#define TABLE_BATCH_ISPASS                  "isPass"			//是否过期 not use
#define TABLE_BATCH_ISPASS_LEN              2
#define TABLE_BATCH_ISPASS_TYPE             "varchar(2)"
#define TABLE_BATCH_BSAVE                   "bSave"			//是否保存 not use
#define TABLE_BATCH_BSAVE_LEN               2
#define TABLE_BATCH_BSAVE_TYPE              "varchar(2)"
#define TABLE_BATCH_BATCHY                  "batchY"			//
#define TABLE_BATCH_BATCHY_LEN              16
#define TABLE_BATCH_BATCHY_TYPE             "varchar(16)"
#define TABLE_BATCH_BATCHZ                  "batchZ"			//
#define TABLE_BATCH_BATCHZ_LEN              32
#define TABLE_BATCH_BATCHZ_TYPE             "varchar(32)"
typedef struct
{
    int id;
    QString goodsCode;
    QString goodsName;
    QString originCode;
    QString originName;
    QString batchCode;
    QString traceCode;
    QString proprietorCode;
    QString time;
    QString groupNo;
    QString weight;
    QString zone;
    QString isPass;
    QString bSave;
    QString batchY;
    QString batchZ;
    void ClearAllData(void)
    {
        goodsCode.clear();
        goodsName.clear();
        originCode.clear();
        batchCode.clear();
        traceCode.clear();
        proprietorCode.clear();
        time.clear();
        groupNo.clear();
        weight.clear();
        zone.clear();
        isPass.clear();
        bSave.clear();
        batchY.clear();
        batchZ.clear();
    }
} struct_tb_batch;
//==================================================================
//
#define TABLE_IDENTIFY                               "TABLE_IDENTIFY"
#define TABLE_IDENTIFY_ID                            "id"			//id
#define TABLE_IDENTIFY_ID_TYPE                       "int"//"INTEGER PRIMARY KEY AUTOINCREMENT"
#define TABLE_IDENTIFY_IDENTIFYCODE                 "identifyCode"
#define TABLE_IDENTIFY_IDENTIFYCODE_LEN             32
#define TABLE_IDENTIFY_IDENTIFYCODE_TYPE            "varchar(32)"
#define TABLE_IDENTIFY_GOODSCODE                    "goodsCode"
#define TABLE_IDENTIFY_GOODSCODE_LEN                32
#define TABLE_IDENTIFY_GOODSCODE_TYPE               "varchar(32)"
#define TABLE_IDENTIFY_GOODSNAME                    "goodsName"
#define TABLE_IDENTIFY_GOODSNAME_LEN                60
#define TABLE_IDENTIFY_GOODSNAME_TYPE               "varchar(60)"
#define TABLE_IDENTIFY_IDENTIFYZ                    "identifyZ"
#define TABLE_IDENTIFY_IDENTIFYZ_LEN                32
#define TABLE_IDENTIFY_IDENTIFYZ_TYPE               "varchar(32)"

typedef struct
{
    int id;
    QString identifyCode;
    QString goodsCode;
    QString goodsName;
    QString identifyZ;
} struct_tb_identify;

//==================================================================
//交易表，里边包含更多的交易结算等信息
#define TABLE_ATTACH                        "TABLE_ATTACH"
#define TABLE_ATTACH_ID                     "id"			//id
#define TABLE_ATTACH_ID_TYPE                "int"
#define TABLE_ATTACH_BUSINESSCODE           "businessCode"		//foreign key
#define TABLE_ATTACH_BUSINESSCODE_TYPE      "int"
#define TABLE_ATTACH_SALENUM                "saleNum"
#define TABLE_ATTACH_SALENUM_TYPE           "int"
#define TABLE_ATTACH_PAYTYPE                "payType"
#define TABLE_ATTACH_PAYTYPE_LEN             16
#define TABLE_ATTACH_PAYTYPE_TYPE           "varchar(16)"
#define TABLE_ATTACH_PAYORDER               "payOrder"
#define TABLE_ATTACH_PAYORDER_LEN           40
#define TABLE_ATTACH_PAYORDER_TYPE          "varchar(40)"
#define TABLE_ATTACH_CARDTYPE               "cardType"
#define TABLE_ATTACH_CARDTYPE_LEN           16
#define TABLE_ATTACH_CARDTYPE_TYPE          "varchar(16)"
#define TABLE_ATTACH_CARDNUM               "cardNum"
#define TABLE_ATTACH_CARDNUM_LEN            32
#define TABLE_ATTACH_CARDNUM_TYPE          "varchar(32)"
#define TABLE_ATTACH_DEVICETYPE             "deviceType"		//
#define TABLE_ATTACH_DEVICETYPE_LEN         16
#define TABLE_ATTACH_DEVICETYPE_TYPE        "varchar(16)"
#define TABLE_ATTACH_DEVICENUM              "deviceNum"
#define TABLE_ATTACH_DEVICENUM_LEN          20
#define TABLE_ATTACH_DEVICENUM_TYPE         "varchar(20)"
#define TABLE_ATTACH_SERIAL                 "serial"
#define TABLE_ATTACH_SERIAL_LEN             32
#define TABLE_ATTACH_SERIAL_TYPE            "varchar(32)"
#define TABLE_ATTACH_SUBTOTAL               "subTotal"
#define TABLE_ATTACH_SUBTOTAL_LEN           15
#define TABLE_ATTACH_SUBTOTAL_TYPE          "varchar(15)"
#define TABLE_ATTACH_PAYMONEY                "payMoney"
#define TABLE_ATTACH_PAYMONEY_LEN            15
#define TABLE_ATTACH_PAYMONEY_TYPE           "varchar(15)"
#define TABLE_ATTACH_SELLERINFO             "sellerInfo"
#define TABLE_ATTACH_SELLERINFO_LEN         32
#define TABLE_ATTACH_SELLERINFO_TYPE        "varchar(32)"
#define TABLE_ATTACH_BUYERINFO              "buyerInfo"
#define TABLE_ATTACH_BUYERINFO_LEN           32
#define TABLE_ATTACH_BUYERINFO_TYPE         "varchar(32)"
#define TABLE_ATTACH_SCALETIME              "scaleTime"
#define TABLE_ATTACH_SCALETIME_LEN          20
#define TABLE_ATTACH_SCALETIME_TYPE         "varchar(20)"
#define TABLE_ATTACH_CONTEXT                "context"
#define TABLE_ATTACH_CONTEXT_LEN            15
#define TABLE_ATTACH_CONTEXT_TYPE           "varchar(15)"
#define TABLE_ATTACH_DEALSTATE              "dealState"
#define TABLE_ATTACH_DEALSTATE_LEN           1
#define TABLE_ATTACH_DEALSTATE_TYPE         "varchar(2)"
#define TABLE_ATTACH_ISPRINT                "isPrint"
#define TABLE_ATTACH_ISPRINT_LEN            1
#define TABLE_ATTACH_ISPRINT_TYPE           "varchar(2)"
#define TABLE_ATTACH_WEBUP                  "webUp"
#define TABLE_ATTACH_WEBUP_LEN              1
#define TABLE_ATTACH_WEBUP_TYPE             "varchar(2)"
#define TABLE_ATTACH_WINDUP                "windUp"
#define TABLE_ATTACH_WINDUP_LEN            1
#define TABLE_ATTACH_WINDUP_TYPE           "varchar(2)"
#define TABLE_ATTACH_ATTACHX                "attachX"
#define TABLE_ATTACH_ATTACHX_LEN            32
#define TABLE_ATTACH_ATTACHX_TYPE           "varchar(32)"
#define TABLE_ATTACH_ATTACHY                "attachY"
#define TABLE_ATTACH_ATTACHY_LEN            32
#define TABLE_ATTACH_ATTACHY_TYPE           "varchar(32)"
#define TABLE_ATTACH_ATTACHZ                "attachZ"
#define TABLE_ATTACH_ATTACHZ_LEN            64
#define TABLE_ATTACH_ATTACHZ_TYPE           "varchar(64)"
typedef struct
{
	int	id;
	int	businessCode;
	int	saleNum;
        QString payType;
        QString payOrder;
        QString	cardType;
	QString	cardNum;
	QString	deviceType;
	QString	deviceNum;
	QString	serial;
	QString	subTotal;
        QString	payMoney;
        QString	sellerInfo;
        QString	buyerInfo;
	QString	scaleTime;
	QString	context;
        QString	dealState;
        QString isPrint;
        QString	webUp;
        QString	windUp;
        QString attachX;
        QString attachY;
        QString attachZ;

} struct_tb_attach;

//==================================================================
//数据库表创建的结构体
//交易表 // TABLE_BUSINESS
#define TABLE_BUSINESS                          "TABLE_BUSINESS"
#define TABLE_BUSINESS_ID			"id"                    //id和serialNum配合，可以在清理表的时候，起到删除但不重复的作用。
#define TABLE_BUSINESS_ID_TYPE                  "int"
#define TABLE_BUSINESS_SERIALNUM                "serialNum"             //
#define TABLE_BUSINESS_SERIALNUM_TYPE           "int"
#define TABLE_BUSINESS_BUSINESSCODE             "businessCode"		//同一笔交易，该号码相同
#define TABLE_BUSINESS_BUSINESSCODE_TYPE        "int"
#define TABLE_BUSINESS_PAYTYPE                  "payType"		//
#define TABLE_BUSINESS_PAYTYPE_LEN              16
#define TABLE_BUSINESS_PAYTYPE_TYPE             "varchar(16)"
#define TABLE_BUSINESS_GOODSCODE                "goodsCode"		//add goodscode,商品编码
#define TABLE_BUSINESS_GOODSCODE_LEN            32
#define TABLE_BUSINESS_GOODSCODE_TYPE           "varchar(32)"
#define TABLE_BUSINESS_GOODSNAME		"goodsName"		//商品名称
#define TABLE_BUSINESS_GOODSNAME_LEN            60
#define TABLE_BUSINESS_GOODSNAME_TYPE           "varchar(60)"
#define TABLE_BUSINESS_GOODSORIGIN		"goodsOrigin"		//商品chan di
#define TABLE_BUSINESS_GOODSORIGIN_LEN           40
#define TABLE_BUSINESS_GOODSORIGIN_TYPE          "varchar(40)"
#define TABLE_BUSINESS_BATCHCODE                "batchCode"		//BatchCode,批次码
#define TABLE_BUSINESS_BATCHCODE_LEN            32
#define TABLE_BUSINESS_BATCHCODE_TYPE           "varchar(32)"
#define TABLE_BUSINESS_TRACECODE                "traceCode"		//add tracecode，追溯码
#define TABLE_BUSINESS_TRACECODE_LEN            32
#define TABLE_BUSINESS_TRACECODE_TYPE           "varchar(32)"
#define TABLE_BUSINESS_WEIGHT                   "weight"		//重量
#define TABLE_BUSINESS_WEIGHT_LEN               9
#define TABLE_BUSINESS_WEIGHT_TYPE              "varchar(9)"
#define TABLE_BUSINESS_PRICE                    "price"			//单价
#define TABLE_BUSINESS_PRICE_LEN                9
#define TABLE_BUSINESS_PRICE_TYPE               "varchar(9)"
#define TABLE_BUSINESS_PIECES                   "pieces"		//数量
#define TABLE_BUSINESS_PIECES_LEN               9
#define TABLE_BUSINESS_PIECES_TYPE              "varchar(9)"
#define TABLE_BUSINESS_TOTAL                    "total"			//总价
#define TABLE_BUSINESS_TOTAL_LEN                9
#define TABLE_BUSINESS_TOTAL_TYPE               "varchar(9)"
#define TABLE_BUSINESS_TIME			"time"			//日期
#define TABLE_BUSINESS_TIME_LEN                 20
#define TABLE_BUSINESS_TIME_TYPE                "varchar(20)"
#define TABLE_BUSINESS_SALEMODE			"saleMode"			//模式，计重，计件，件重 0,1,2
#define TABLE_BUSINESS_SALEMODE_LEN             1
#define TABLE_BUSINESS_SALEMODE_TYPE            "varchar(2)"
#define TABLE_BUSINESS_CANCEL                   "cancel"		//是否取消  这里，是在交易编辑过程中，取消的0,1
#define TABLE_BUSINESS_CANCEL_LEN               1
#define TABLE_BUSINESS_CANCEL_TYPE              "varchar(2)"
#define TABLE_BUSINESS_PICTURE                  "picture"	//
#define TABLE_BUSINESS_PICTURE_LEN              64
#define TABLE_BUSINESS_PICTURE_TYPE             "varchar(64)"
#define TABLE_BUSINESS_ALLWEIGHT                "allWeight"		//重量总计，北京批发市场版本用来计算折扣后的金额
#define TABLE_BUSINESS_ALLWEIGHT_LEN            64
#define TABLE_BUSINESS_ALLWEIGHT_TYPE           "varchar(64)"
#define TABLE_BUSINESS_ISOVERSALE                "isQverSale"		//开发自定义
#define TABLE_BUSINESS_ISOVERSALE_LEN            1
#define TABLE_BUSINESS_ISOVERSALE_TYPE           "varchar(2)"
#define TABLE_BUSINESS_WEBUP		        "webUp"
#define TABLE_BUSINESS_WEBUP_LEN                1
#define TABLE_BUSINESS_WEBUP_TYPE              "varchar(2)"
#define TABLE_BUSINESS_WINDUP                  "windUp"
#define TABLE_BUSINESS_WINDUP_LEN              1
#define TABLE_BUSINESS_WINDUP_TYPE             "varchar(2)"
#define TABLE_BUSINESS_BUSINESSY		"businessY"
#define TABLE_BUSINESS_BUSINESSY_LEN            32
#define TABLE_BUSINESS_BUSINESSY_TYPE           "varchar(32)"
#define TABLE_BUSINESS_BUSINESSZ                "businessZ"
#define TABLE_BUSINESS_BUSINESSZ_LEN            32
#define TABLE_BUSINESS_BUSINESSZ_TYPE           "varchar(32)"
typedef struct
{
	int id;
	int serialNum;
	int businessCode;
        QString payType;
	QString goodsCode;
	QString goodsName;
        QString goodsOrigin;
	QString batchCode;
	QString traceCode;
	QString weight;
	QString price;
	QString pieces;
	QString total;
	QString time;
	QString mode;
	QString cancel;
        QString picture;
	QString allWeight;
        QString isOverSale;
        QString webUp;
        QString windUp;
        QString businessY;
        QString businessZ;
        void ClearAllData(void)
        {
            payType.clear();
            goodsCode.clear();
            goodsName.clear();
            goodsOrigin.clear();
            batchCode.clear();
            traceCode.clear();
            weight.clear();
            price.clear();
            pieces.clear();
            total.clear();
            time.clear();
            mode.clear();
            cancel.clear();
            picture.clear();
            allWeight.clear();
            isOverSale.clear();
            webUp.clear();
            windUp.clear();
            businessY.clear();
            businessZ.clear();
        }
} struct_tb_business;

//==================================================================
//电子秤标定 // "TABLE_CALIBRATION"
#define TABLE_CALIBRATION			"TABLE_CALIBRATION"
#define TABLE_CALIBRATION_ID                    "id"				//id
#define TABLE_CALIBRATION_ID_TYPE               "int"
#define TABLE_CALIBRATION_FULLARRANGE		"fullArrange"
#define TABLE_CALIBRATION_FULLARRANGE_LEN       9
#define TABLE_CALIBRATION_FULLARRANGE_TYPE	"varchar(9)"
#define TABLE_CALIBRATION_CALARRANGE            "calArrange"
#define TABLE_CALIBRATION_CALARRANGE_LEN        9
#define TABLE_CALIBRATION_CALARRANGE_TYPE	"varchar(9)"
#define TABLE_CALIBRATION_E                     "e"
#define TABLE_CALIBRATION_E_LEN                 4
#define TABLE_CALIBRATION_E_TYPE                "varchar(4)"
#define TABLE_CALIBRATION_E1                    "e1"
#define TABLE_CALIBRATION_E1_LEN                4
#define TABLE_CALIBRATION_E1_TYPE               "varchar(4)"
#define TABLE_CALIBRATION_LOCALRAVITY           "localGravity"
#define TABLE_CALIBRATION_LOCALRAVITY_LEN       5
#define TABLE_CALIBRATION_LOCALRAVITY_TYPE      "varchar(5)"
#define TABLE_CALIBRATION_DEGRAVITY             "destGravity"
#define TABLE_CALIBRATION_DEGRAVITY_LEN         5
#define TABLE_CALIBRATION_DEGRAVITY_TYPE        "varchar(5)"
#define TABLE_CALIBRATION_UNLOAD                "unload"
#define TABLE_CALIBRATION_UNLOAD_LEN            6
#define TABLE_CALIBRATION_UNLOAD_TYPE           "varchar(6)"
#define TABLE_CALIBRATION_LOAD                  "load"
#define TABLE_CALIBRATION_LOAD_LEN              6
#define TABLE_CALIBRATION_LOAD_TYPE             "varchar(6)"
#define TABLE_CALIBRATION_ISDUAL                "isDual"
#define TABLE_CALIBRATION_ISDUAL_LEN            1
#define TABLE_CALIBRATION_ISDUAL_TYPE           "varchar(2)"
typedef struct
{
	int id;
	QString fullArrange;
	QString calArrange;
	QString e;
	QString e1;
	QString localGravity;
	QString destGravity;
	QString unLoad;
	QString load;
	QString isDual;
} struct_tb_calibration;

//电子秤设置 // TABLE_SCALESETTING
#define TABLE_SCALESETTING			    "TABLE_SCALESETTING"
#define TABLE_SCALESETTING_ID                       "id"		//id
#define TABLE_SCALESETTING_ID_TYPE                  "int"
#define TABLE_SCALESETTING_NAME                     "name"
#define TABLE_SCALESETTING_NAME_LEN                 20
#define TABLE_SCALESETTING_NAME_TYPE                "varchar(20)"
#define TABLE_SCALESETTING_VALUE                    "value"
#define TABLE_SCALESETTING_VALUE_LEN                128
#define TABLE_SCALESETTING_VALUE_TYPE               "varchar(128)"
#define TABLE_SCALESETTING_UPPER                    "upper"
#define TABLE_SCALESETTING_UPPER_TYPE               "int"
typedef struct
{
	int id;
	QString name;
	QString value;
	int  upper;
} struct_tb_scaleSetting;

//电子秤设置 // TABLE_USERSETTING
#define TABLE_USERSETTING			    "TABLE_USERSETTING"
#define TABLE_USERSETTING_ID                        "id"		//id
#define TABLE_USERSETTING_ID_TYPE                   "int"
#define TABLE_USERSETTING_NAME                      "name"
#define TABLE_USERSETTING_NAME_LEN                  20
#define TABLE_USERSETTING_NAME_TYPE                 "varchar(20)"
#define TABLE_USERSETTING_VALUE                     "value"
#define TABLE_USERSETTING_VALUE_LEN                 128
#define TABLE_USERSETTING_VALUE_TYPE                "varchar(128)"
#define TABLE_USERSETTING_UPPER                     "upper"
#define TABLE_USERSETTING_UPPER_TYPE                "int"
typedef struct
{
	int id;
	QString name;
	QString value;
	int  upper;
} struct_tb_userSetting;

//电子秤device // TABLE_DEVICE
//#define TABLE_DEVICE                            "TABLE_DEVICE"
//#define TABLE_DEVICE_ID                          "id"		//id
//#define TABLE_DEVICE_ID_TYPE                    "int"
//#define TABLE_DEVICE_NAME                        "name"
//#define TABLE_DEVICE_NAME_LEN                   20
//#define TABLE_DEVICE_NAME_TYPE                  "varchar(20)"
//#define TABLE_DEVICE_VALUE                      "value"
//#define TABLE_DEVICE_VALUE_LEN                  64
//#define TABLE_DEVICE_VALUE_TYPE                 "varchar(64)"
//#define TABLE_DEVICE_UPPER                      "upper"
//#define TABLE_DEVICE_UPPER_TYPE                 "int"

typedef struct
{
        int id;
        QString name;
        QString value;
        int  upper;
} struct_tb_device;

//卖家信息表--允许多个卖家，绑定到电子秤上 // TABLE_SELLERSINFO
#define TABLE_SELLERSINFO			"TABLE_SELLERSINFO"
#define TABLE_SELLERSINFO_ID                    "id"			//id
#define TABLE_SELLERSINFO_ID_TYPE               "int"
#define TABLE_SELLERSINFO_SELLERCARD		"sellerCard"		//卡号
#define TABLE_SELLERSINFO_SELLERCARD_LEN        32
#define TABLE_SELLERSINFO_SELLERCARD_TYPE       "varchar(32)"
#define TABLE_SELLERSINFO_PROPRIETORCODE	"proprietorCode"	//编号
#define TABLE_SELLERSINFO_PROPRIETORCODE_LEN    32
#define TABLE_SELLERSINFO_PROPRIETORCODE_TYPE   "varchar(32)"
#define TABLE_SELLERSINFO_NAME			"name"			//姓名
#define TABLE_SELLERSINFO_NAME_LEN              32
#define TABLE_SELLERSINFO_NAME_TYPE             "varchar(32)"
#define TABLE_SELLERSINFO_PASS			"pass"                  //姓名
#define TABLE_SELLERSINFO_PASS_LEN              16
#define TABLE_SELLERSINFO_PASS_TYPE             "varchar(16)"
typedef struct
{
	int id;
	QString sellerCard;
	QString proprietorCode;
	QString name;
	QString pass;
} struct_tb_seller_info;

// table_log
#define TABLE_SCALELOG			    	"TABLE_SCALELOG"
#define TABLE_SCALELOG_ID			"id"		//id
#define TABLE_SCALELOG_ID_TYPE      		"int"
#define TABLE_SCALELOG_NAME			"name"		//区域号
#define TABLE_SCALELOG_NAME_LEN                 64
#define TABLE_SCALELOG_NAME_TYPE		"varchar(64)"
#define TABLE_SCALELOG_VALUE1			"value1"	//是否保存 not use
#define TABLE_SCALELOG_VALUE1_LEN               64
#define TABLE_SCALELOG_VALUE1_TYPE              "varchar(64)"
#define TABLE_SCALELOG_VALUE2			"value2"	//是否保存 not use
#define TABLE_SCALELOG_VALUE2_LEN               64
#define TABLE_SCALELOG_VALUE2_TYPE              "varchar(64)"
#define TABLE_SCALELOG_UPPER        		"upper"
#define TABLE_SCALELOG_UPPER_TYPE		"int"

typedef struct
{
	int id;
	QString name;
	QString value1;
	QString value2;
	int  upper;
} struct_tb_log;

#endif /*_DB_STRUCT_ */

