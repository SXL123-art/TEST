#ifndef CALIBRATION_H
#define CALIBRATION_H
#include "../../unit/shareinfo.h"
#include "../../public/typeMenu.h"
#include "../msgdialog/msgdialog.h"
class Calibration
{
public:
    Calibration(CShareInfo *pInfo);
    ~Calibration(void);

    enQtShowType CalibrationProc();
private:
    enQtShowType CalibrationDeal();
    int UpdateCalibrationShow(void);
    int init(void);
    int dec(void);
    int add(void);
    int Sure(void);
    int Select(void);
    void ProcessEditStr(unsigned char ch);

    unsigned char focus_parent;
    bool m_bUpdate;
    bool UpdataBar;

    CShareInfo *m_pCalibrationShareInfo;
    struct_tb_calibration * stbCal;
    QString    menuStr;
};

#endif // CALIBRATION_H
