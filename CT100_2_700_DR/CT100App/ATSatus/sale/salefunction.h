#ifndef SALEFUNCTION_H
#define SALEFUNCTION_H

#include "../../unit/shareinfo.h"
class CSaleFunction
{
public:
    CSaleFunction(CShareInfo *pInfo);

    ~CSaleFunction(void);

    int ExcutCustomSqls(void);

    int ExportFile(void);

    int ExcutScript(void);

    int ExcutRestCmd(void);

    int TestNetwork(void);

    int PrintLastSale(void);

    int PrintDeviceInfo(void);

    void CameraTest(void);


public:
        CShareInfo *m_pSaleFunShareInfo;
private:
};

#endif // SALEFUNCTION_H
