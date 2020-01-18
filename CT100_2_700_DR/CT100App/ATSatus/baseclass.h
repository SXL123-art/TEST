#ifndef BASECLASS_H
#define BASECLASS_H
#include "../unit/ShareInfo.h"
#include <math.h>


class CBaseClass
{
public:

    CBaseClass();

    ~CBaseClass(void);

    void updateQtShow(CShareInfo *m_pShareInfo);
    void CalculateAndUpdate(CShareInfo *m_pShareInfo);

    int EditNum(unsigned char key, int nDecNum);

    void ClearEditNum(void);
    void ChangeSaleMode(CShareInfo *pInfo, enSaleMode mode);

public:
    bool	m_bUpdate;
    bool 	m_bAutoSave;

    long		m_nWeight;		// 重量
    long		m_nPieces;		// 数量
    long		m_nPrice;		// 单价
    long		m_nTotal;		// 总价

    char		m_strNum[MAXLEN_PRICE+1];
    char		m_strInput[MAXLEN_PRICE+1];

    unsigned int m_nPricesCoe;
    unsigned int m_nWeightCoe;
    bool    m_bSelectPlu;                  //是否找到PLU

    char	m_passwdCard[PASSWORDLEN];

private:
    void Cal(CShareInfo *pInfo);//计算总价

};

#endif //
