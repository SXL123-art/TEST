#ifndef REPORT_H
#define REPORT_H
#include "../../unit/shareinfo.h"
#include "../msgdialog/msgdialog.h"

class CReport
{
public:
    CReport(CShareInfo *pInfo);
    ~CReport(void);

    enQtShowType ReportProc();
private:

    enQtShowType ReportDeal();

    void initReport(void);

    void up(void);

    void down(void);

    void findOnePage(void);

    void print(void);

    void updateReportShow(void);


private:
    CShareInfo *m_pReportShareInfo;
    unsigned char index;
    int currentId;
    int currentTotalNum;
    int currentNum;
    int totalNum;

    bool   m_bUpdate ;

    QSqlQuery query;

};

#endif // REPORT_H
