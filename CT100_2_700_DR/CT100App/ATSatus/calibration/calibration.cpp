#include "calibration.h"

Calibration::Calibration(CShareInfo *pInfo)
{
    m_pCalibrationShareInfo = pInfo;
    printf("-------------------------------------------init Calibration\n");
    focus_parent = 1;
    m_bUpdate = false;
    UpdataBar = false;
}

Calibration::~Calibration(void)
{

}

enQtShowType Calibration::CalibrationProc()
{
    init();

    printf("-------------------------------------------Calibration\n");
    m_pCalibrationShareInfo->m_enQtUiType = enQt_CalQt;
    focus_parent = 1;
    m_pCalibrationShareInfo->shareInfoCalibrationUi.focus_parent = focus_parent;
    m_bUpdate = true;
    UpdataBar = true;
    Select();
    //UpdateCalibrationShow();

    while(true)
    {
        usleep(50*1000);
        if(enQt_CalQt == CalibrationDeal())
        {
                continue;
        }
        else
        {
            delete [] stbCal;
            return m_pCalibrationShareInfo->enLastShowType;
        }
    }
}

enQtShowType Calibration::CalibrationDeal()
{
    unsigned char ch = 0;
    static bool systemPowerOff = false;
    if (SALE_KEY_VALID == ATKey_getKey(&ch))
    {
        m_bUpdate = true;
        ProcessEditStr(ch);
        switch(ch)
        {
            case _KEY_MENU:
                //按了保存键，表示修改了标定数据，退出重启
                if(systemPowerOff == true)
                {
                    system("./ct100.sh");
                }
                return m_pCalibrationShareInfo->enLastShowType;
            case _KEY_UP:
                UpdataBar = true;
                dec();
                break;
            case _KEY_DOWN:
                UpdataBar = true;
                add();
                break;
            case _KEY_ENTER://确定
                Sure();
                systemPowerOff = true;
                break;
        }
    }  
    static unsigned long iOld=0;
    static unsigned char iShow=0;
    if(focus_parent == 7 || focus_parent == 8)
    {
        if(iShow++ >= 10)
        {
            unsigned long m_iInternalcode = ATAd_getInternalCode();
            if(iOld != m_iInternalcode)
            {
                m_bUpdate = true;
                printf("------>>Internal Code:%d!\n", m_iInternalcode);
                menuStr.sprintf("%d",m_iInternalcode);
                iOld = m_iInternalcode;
            }
            iShow=0;
        }
    }
    if(UpdataBar == true)
    {
        sprintf(m_pCalibrationShareInfo->shareInfoCalibrationUi.Statusbar ,"%s", "");
        UpdataBar = false;
    }
    UpdateCalibrationShow();
    return enQt_CalQt;
}

int Calibration::init(void)
{
    QSqlQuery query;
    if(ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements("select *from TABLE_CALIBRATION where id=1;",&query)  > 0)
    {
        query.next();
        bool ok;
        stbCal = new struct_tb_calibration[1];
        stbCal[0].id = query.record().value(TABLE_CALIBRATION_ID).toString().toInt(&ok,10);
        stbCal[0].fullArrange = query.record().value(TABLE_CALIBRATION_FULLARRANGE).toString();
        stbCal[0].calArrange = query.record().value(TABLE_CALIBRATION_CALARRANGE).toString();
        stbCal[0].e = query.record().value(TABLE_CALIBRATION_E).toString();
        stbCal[0].e1 = query.record().value(TABLE_CALIBRATION_E1).toString();
        stbCal[0].localGravity = query.record().value(TABLE_CALIBRATION_LOCALRAVITY).toString();
        stbCal[0].destGravity = query.record().value(TABLE_CALIBRATION_DEGRAVITY).toString();
        stbCal[0].unLoad = query.record().value(TABLE_CALIBRATION_UNLOAD).toString();
        stbCal[0].load = query.record().value(TABLE_CALIBRATION_LOAD).toString();
        stbCal[0].isDual = query.record().value(TABLE_CALIBRATION_ISDUAL).toString();

    }
    query.clear();
}

int Calibration::dec(void)
{
    if(focus_parent>1)
    {
        focus_parent--;
    }
    else
    {
        focus_parent = focus_parent+CAL_SIZE-1;
    }
    printf("dec focus_parent--------------------%d\n",focus_parent);
    m_pCalibrationShareInfo->shareInfoCalibrationUi.focus_parent = focus_parent;
    Select();
}

int Calibration::add(void)
{
    if(focus_parent<CAL_SIZE)
    {
        focus_parent++;
    }
    else
    {
        focus_parent = focus_parent-CAL_SIZE+1;
    }
    printf("add focus_parent--------------------%d\n",focus_parent);
    m_pCalibrationShareInfo->shareInfoCalibrationUi.focus_parent = focus_parent;
    Select();
}

int Calibration::Sure(void)
{
    int nRet = 0;
    QString Qstr;
    switch(focus_parent)
    {
        case 1:
            stbCal[0].fullArrange = menuStr;
            Qstr = QString("update TABLE_CALIBRATION set %1='%2' where id=1").arg(TABLE_CALIBRATION_FULLARRANGE).arg(stbCal[0].fullArrange);
            nRet = ATdb_Excute_SQL_Statements(Qstr);
            break;
        case 2:
            stbCal[0].calArrange = menuStr;
            Qstr = QString("update TABLE_CALIBRATION set %1='%2' where id=1").arg(TABLE_CALIBRATION_CALARRANGE).arg(stbCal[0].calArrange);
            nRet = ATdb_Excute_SQL_Statements(Qstr);
            break;
        case 3:
            stbCal[0].e = menuStr;
            Qstr = QString("update TABLE_CALIBRATION set %1='%2' where id=1").arg(TABLE_CALIBRATION_E).arg(stbCal[0].e);
            nRet = ATdb_Excute_SQL_Statements(Qstr);
            break;
        case 4:  
            stbCal[0].e1 = menuStr;
            Qstr = QString("update TABLE_CALIBRATION set %1='%2' where id=1").arg(TABLE_CALIBRATION_E1).arg(stbCal[0].e1);
            nRet = ATdb_Excute_SQL_Statements(Qstr);
            break;
        case 5:
            stbCal[0].localGravity = menuStr;
            Qstr = QString("update TABLE_CALIBRATION set %1='%2' where id=1").arg(TABLE_CALIBRATION_LOCALRAVITY).arg(stbCal[0].localGravity);
            nRet = ATdb_Excute_SQL_Statements(Qstr);
            break;
        case 6:
            stbCal[0].destGravity = menuStr;
            Qstr = QString("update TABLE_CALIBRATION set %1='%2' where id=1").arg(TABLE_CALIBRATION_DEGRAVITY).arg(stbCal[0].destGravity);
            nRet = ATdb_Excute_SQL_Statements(Qstr);
            break;
        case 7:
            stbCal[0].unLoad = menuStr;
            Qstr = QString("update TABLE_CALIBRATION set %1='%2' where id=1").arg(TABLE_CALIBRATION_UNLOAD).arg(stbCal[0].unLoad);
            nRet = ATdb_Excute_SQL_Statements(Qstr);
            break;
        case 8:
            stbCal[0].load = menuStr;
            Qstr = QString("update TABLE_CALIBRATION set %1='%2' where id=1").arg(TABLE_CALIBRATION_LOAD).arg(stbCal[0].load);
            nRet = ATdb_Excute_SQL_Statements(Qstr);
            break;
        case 9:
            stbCal[0].isDual = menuStr;
            Qstr = QString("update TABLE_CALIBRATION set %1='%2' where id=1").arg(TABLE_CALIBRATION_ISDUAL).arg(stbCal[0].isDual);
            nRet = ATdb_Excute_SQL_Statements(Qstr);
            break;
    }
    if(nRet<=0)
    {
        sprintf(m_pCalibrationShareInfo->shareInfoCalibrationUi.Statusbar ,"保存失败:%s", menuStr.toStdString().c_str());
    }
    else
    {
        sprintf(m_pCalibrationShareInfo->shareInfoCalibrationUi.Statusbar ,"保存成功:%s", menuStr.toStdString().c_str());
    }
}
int Calibration::Select(void)
{
    switch(focus_parent)
    {
        case 1:
            menuStr = stbCal[0].fullArrange;
            break;
        case 2:
            menuStr = stbCal[0].calArrange;
            break;
        case 3:
            menuStr = stbCal[0].e;
            break;
        case 4:
            menuStr = stbCal[0].e1;
            break;
        case 5:
            menuStr = stbCal[0].localGravity;
            break;
        case 6:
            menuStr = stbCal[0].destGravity;
            break;
        case 7:
            menuStr = stbCal[0].unLoad;
            break;
        case 8:
            menuStr = stbCal[0].load;
            break;
        case 9:
            menuStr = stbCal[0].isDual;
            break;
    }
}

int Calibration::UpdateCalibrationShow(void)
{
    if(m_bUpdate == true)
    {
        sprintf(m_pCalibrationShareInfo->shareInfoCalibrationUi.menuData,"%s",menuStr.toStdString().c_str());
        m_pCalibrationShareInfo->UpdateGui();
    }
     m_bUpdate = false;
}
void Calibration::ProcessEditStr(unsigned char ch)
{
    char cha;
    if ((ch >= _KEY_NUM0) && (ch <= _KEY_NUM9)
        || (ch == _KEY_BACK)
        || (ch == _KEY_CLR))
    {
        if(ch == _KEY_BACK)
        {
            if(menuStr.length()>0)
            {
                menuStr = menuStr.left(menuStr.length() - 1);
            }
        }
        else if(ch == _KEY_CLR)
        {
            menuStr.clear();
        }
        else
        {
            if(0 < (cha = m_pCalibrationShareInfo->KeyValueConversion(ch)))
            {
                printf("----------input :%c\n",cha);
                menuStr.append(cha);
            }
        }
    }
}
