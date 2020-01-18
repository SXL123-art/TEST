#-------------------------------------------------
#
# Project created by QtCreator 2018-03-07T19:36:41
#
#-------------------------------------------------

QT       += core \
        sql \
        network

QT       -= gui


TARGET = CT100App
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ATAdc/Ad.cpp \
    ATAdc/ATAdc.cpp \
    ATAdc/driver_ads1232.cpp \
    ATBeep/ATBeep.cpp \
    ATBeep/driver_beep.cpp \
    unit/shareInfo.cpp \
    unit/machine.cpp \
    ATShm/shmcom.cpp \
    ATSatus/sale/mainSale.cpp \
    ATSatus/baseclass.cpp \
    ATSatus/login/login.cpp \
    ATSatus/sale/salefunction.cpp \
    ATPrint/ReceiptThermal.cpp \
    ATPrint/print_manager.cpp \
    ATPrint/driver/prt_process.c \
    ATPrint/driver/QRCode_Bar.c \
    ATPrint/driver/bar_print.c \
    ATPrint/driver/ImgBuf.c \
    ATPrint/driver/PRT_Driver.c \
    ATPrint/driver/ChnFont_Bold_24x24.c \
    ATPrint/driver/ChnFont_Songti_24x24.c \
    ATPrint/driver/ChnFont_Std_16x16.c \
    ATPrint/driver/Code128_Bar.c \
    ATPrint/driver/EAN_UPCA_Bar.c \
    ATPrint/driver/EngFont_Arial_12x24.c \
    ATPrint/driver/EngFont_Bold_12x24.c \
    ATPrint/driver/EngFont_Songti_12x24.c \
    ATPrint/driver/EngFont_Std_8x16.c \
    ATPrint/driver/EngFont_Trace_16x16.c \
    ATPrint/driver/PRT_Timer.c \
    ATPrint/print.c \
    ATLib/ATLib.cpp \
    ATLib/code_convert.cpp \
    ATLib/des.cpp \
    ATLib/md5.cpp \
    ATLib/SysApi.cpp \
    ATPwm/ATPwm.cpp \
    ATPwm/driver_pwm.cpp \
    ATSqlite/ATSQLite.cpp \
    ATSqlite/SQLiteDeal.cpp \
    ATSatus/pay/pay.cpp \
    ATSerialDevice/ATSerialDevice.cpp \
    ATSerialDevice/CSerial.cpp \
    ATSerialDevice/driver_serial.cpp \
    ATSerialDevice/MyBluetooth.cpp \
    ATSerialDevice/MyScanner.cpp \
    ATSerialDevice/MyZigbee.cpp \
    ATSerialDevice/MyZM703.cpp \
    ATSocket/cJSON.c \
    ATSocket/tinystr.cpp \
    ATSocket/tinyxml.cpp \
    ATSocket/tinyxmlerror.cpp \
    ATSocket/tinyxmlparser.cpp \
    ATSocket/CHttpClient.cpp \
    ATSocket/QtHttpClient.cpp \
    ATSatus/menu/mainmenu.cpp \
    ATSatus/menu/menu.cpp \
    ATSatus/menu/menuCustomer.cpp \
    ATSatus/menu/menuSystem.cpp \
    ATSatus/menu/menuDataManagement.cpp \
    ATSatus/menu/menuUserSwitch.cpp \
    ATSatus/menu/menuSystemSwitch.cpp \
    ATSatus/menu/menuUser.cpp \
    ATSatus/menu/menuNet.cpp \
    ATSatus/pay/business.cpp \
    ATSatus/pay/pos/pos.cpp \
    ATSatus/pay/icbc/icbc.cpp \
    ATSatus/calibration/calibration.cpp \
    ATSatus/report/report.cpp \
    ATI2CDevice/ATI2CDevice.cpp \
    ATI2CDevice/driver_i2c.cpp \
    ATI2CDevice/i2c.cpp \
    ATI2CDevice/I2C_Device.cpp \
    ATSatus/msgdialog/msgdialog.cpp \
    ATSatus/goodskey/goodskey.cpp \
    ATScaleHttpServer/ATScaleHttpServer.cpp \
    ATScaleHttpServer/ScaleHttpService.cpp \
    ATKeyboard/ATKey.cpp \
    ATKeyboard/imx28_key.cpp \
    ATAtmel8/Atmel8.cpp \
    ATPmu/ATPmu.cpp \
    ATPmu/driver_pmu.cpp \
    ATWebservice/ATWebService.cpp \
    ATWebservice/WebService.cpp

HEADERS += \
    ../Public/typeTabUi.h \
    ATAdc/Ad.h \
    ATAdc/ATAdc.h \
    ATAdc/driver_ads1232.h \
    ATBeep/ATBeep.h \
    ATBeep/driver_beep.h \
    public/globals.h \
    public/Ring.h \
    public/typeAd.h \
    unit/shareInfo.h \
    unit/machine.h \
    ATShm/shmcom.h \
    ATSatus/sale/mainSale.h \
    ATSatus/baseclass.h \
    ATSatus/login/login.h \
    public/typeShow.h \
    ATSatus/sale/salefunction.h \
    ATPrint/ReceiptThermal.h \
    public/typePrinter.h \
    ATPrint/print_manager.h \
    ATPrint/set.h \
    ATPrint/driver/ext_fun.h \
    ATPrint/driver/ext_var.h \
    ATPrint/driver/PRT_Driver.h \
    ATPrint/driver/qrencode.h \
    ATPrint/driver/ChineseFont.h \
    ATPrint/driver/define.h \
    ATPrint/driver/EngFont.h \
    ATPrint/driver/bar_print.h \
    ATPrint/driver/LOGOCAS.h \
    ATPrint/driver/BarcodeFont.h \
    ATPrint/driver/PRT_Timer.h \
    ATPrint/print.h \
    ATLib/ATLib.h \
    ATLib/code_convert.h \
    ATLib/des.h \
    ATLib/iconv.h \
    ATLib/md5.h \
    ATLib/SysApi.h \
    ATPwm/ATPwm.h \
    ATPwm/driver_pwm.h \
    public/typeKey.h \
    ATSqlite/ATSQLite.h \
    ATSqlite/DBStruct.h \
    ATSqlite/SQLiteDeal.h \
    ATSatus/pay/pay.h \
    ATSerialDevice/ATSerialDevice.h \
    ATSerialDevice/CSerial.h \
    ATSerialDevice/driver_serial.h \
    ATSerialDevice/MyBluetooth.h \
    ATSerialDevice/MyScanner.h \
    ATSerialDevice/MyZigbee.h \
    ATSerialDevice/MyZM703.h \
    ATSerialDevice/typeSerial.h \
    public/typeCard.h \
    ATSocket/cJSON.h \
    ATSocket/tinystr.h \
    ATSocket/tinyxml.h \
    ATSocket/CHttpClient.h \
    ATSocket/QtHttpClient.h \
    ATSatus/menu/mainmenu.h \
    public/typeMenu.h \
    ATSatus/menu/menu.h \
    ATSatus/menu/menuCustomer.h \
    ATSatus/menu/menuSystem.h \
    ATSatus/menu/menuDataManagement.h \
    ATSatus/menu/menuUserSwitch.h \
    ATSatus/menu/menuSystemSwitch.h \
    ATSatus/menu/menuUser.h \
    ATSatus/menu/menuNet.h \
    ATSatus/pay/business.h \
    ATSatus/pay/pos/pos.h \
    ATSatus/pay/icbc/icbc.h \
    ATSatus/calibration/calibration.h \
    ATSatus/report/report.h \
    ATI2CDevice/ATI2CDevice.h \
    ATI2CDevice/driver_i2c.h \
    ATI2CDevice/i2c.h \
    ATWebservice/ATWebService.h \
    ATWebservice/WebService.h \
    public/typeEnum.h \
    ATSatus/msgdialog/msgdialog.h \
    ATSatus/goodskey/goodskey.h \
    ATScaleHttpServer/ATScaleHttpServer.h \
    ATScaleHttpServer/ScaleHttpService.h \
    public/TemplateDulLink.h \
    ATKeyboard/ATKey.h \
    ATKeyboard/imx28_key.h \
    ATAtmel8/Atmel8.h \
    ATPmu/ATPmu.h \
    ATPmu/driver_pmu.h \
    ATWebservice/ATWebService.h \
    ATWebservice/WebService.h

LIBS += -L /opt/freescale/usr/local/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/arm-fsl-linux-gnueabi/multi-libs/lib/  \
    -lqrencode \
    -liconv \
    -levent \
    -lcurl \
    -lssl \
    -lRecognition
