#ifndef CGOODSKEY_H
#define CGOODSKEY_H
#include "../../unit/shareinfo.h"
#include "../msgdialog/msgdialog.h"
class CGoodsKey
{
public:
    CGoodsKey(CShareInfo *pInfo);

    ~CGoodsKey(void);

    enQtShowType GoodsKeyProc();

private:
    enQtShowType GoodsKeyDeal();

    void initGoodsKey(void);

    void updateGoodsKeyShow(void);

    void ProcessEditStr(unsigned char ch);

    int Sure(void);

    int print(void);

    CShareInfo *m_pGoodsKeyShareInfo;

    bool goodsOrKey;

    bool m_bUpdate;

    QString  goodsCode;
    QString  goodsKey;
};

#endif // CGOODSKEY_H
