#include "baseclass.h"
#define MAXTOTAL  999999000
// 计算字符串小数位数
int dots(char*	p)
{
    int ret=0;
    int len = strlen(p);
    for(int i=0;i<len;i++)
    {
        if (*(p+i) == '.')
        {
            return len-i-1;
        }
    }

    return -1;
}
CBaseClass::CBaseClass()
{
    m_nWeight = 0;		// 重量
    m_nPieces = 0;		// 数量
    m_nPrice = 0;		// 单价
    m_nTotal = 0;		// 总价

}
CBaseClass::~CBaseClass(void)
{

}

void CBaseClass::updateQtShow(CShareInfo *m_pShareInfo)
{
    m_pShareInfo->UpdateGui();
}

void CBaseClass::CalculateAndUpdate(CShareInfo *m_pShareInfo)
{
    Cal(m_pShareInfo);
    m_pShareInfo->RefreshWeight();
    updateQtShow(m_pShareInfo);
    /*Cal(m_pShareInfo);
    if(m_bUpdate)
    {
        m_bUpdate = false;
        updateQtShow(m_pShareInfo);
    }
    else if(m_pShareInfo->RefreshWeight()>0)
    {
        updateQtShow(m_pShareInfo);
    }*/
    return;
}

void CBaseClass::Cal(CShareInfo *pInfo)
{
    static char nWeightOverCounter = 0;
    if ((pInfo->m_adInfo.kg>0) && (pInfo->m_adInfo.kg >= pInfo->m_pShm->m_nMaxWeight) && (pInfo->m_pShm->m_nMaxWeight > 0))
    {
        pInfo->m_LastAdInfo.kg -=1;//为了再次刷新重量

        if(++nWeightOverCounter>10 && (pInfo->m_enQtUiType == enQt_SaleQT || pInfo->m_enQtUiType == enQt_LoginQT))
        {
            nWeightOverCounter = 0;
            pInfo->ShowOverWeightWarningProc(enMsgAlarm,"称重警告", "系统超过最大量程");
        }
        return;
    }
    nWeightOverCounter = 0;

    long long nWeightOrPieces = atol(pInfo->shareInfoSaleUi.weight);
    m_nPrice = atol(pInfo->shareInfoSaleUi.unitprice);

    // 无重量/数量或者单价，不计算总价
    if ((nWeightOrPieces<=0) ||(m_nPrice<=0))
    {
        // 防止单价被清零后，总价不变化
        if (pInfo->GetSaleMode() == enWeight)
        {
             m_nWeight = nWeightOrPieces;
        }
        m_nTotal = 0;
        return;
    }
    long m_nTempTotal;
    if (pInfo->GetSaleMode() == enPieces)
    {
        m_nPieces = nWeightOrPieces * pInfo->m_ShareSystemInfo.nWeightCoe;
        m_nTempTotal = m_nPieces * m_nPrice;
    }
    else
    {
         m_nWeight = nWeightOrPieces;
         if(999999000/m_nWeight <= m_nPrice)
         {
             m_nTempTotal = 1000000000;
         }
         else
         {
            m_nTempTotal = m_nWeight * m_nPrice;
         }
    }
    int nDot = 0;
    if (pInfo->m_ShareSystemInfo.Round == 1)
    {
        if(pInfo->m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT == 3)
        {
            // 对分四舍五入
            nDot =  (m_nTempTotal%(pInfo->m_ShareSystemInfo.nWeightCoe*10))>=5000?10:0;
            m_nTotal = m_nTempTotal /10000*10+nDot;
        }
        else if(pInfo->m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT == 2)
        {
            // 对分四舍五入
            nDot =  (m_nTempTotal%(pInfo->m_ShareSystemInfo.nWeightCoe*10))>=500?10:0;
            m_nTotal = (m_nTempTotal /1000*10+nDot);
        }
        printf("1-------------m_nTotal:%d--%d--%d--%d\n", m_nTotal, m_nTempTotal, nDot);
    }
    else if (pInfo->m_ShareSystemInfo.Round == 2)
    {
        // 对角、分四舍五入
        if(pInfo->m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT == 3)
        {
            nDot =  (m_nTempTotal%(pInfo->m_ShareSystemInfo.nWeightCoe*100))>=50000?100:0;
            m_nTotal = m_nTempTotal /100000*100+nDot;
        }
        else if(pInfo->m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT == 2)
        {
            nDot =  (m_nTempTotal%(pInfo->m_ShareSystemInfo.nWeightCoe*100))>=5000?100:0;
            m_nTotal = m_nTempTotal /10000*100+nDot;
        }
        printf("2-------------m_nTotal:%d--%d--%d--%d\n", m_nTotal, m_nTempTotal, nDot);
    }
    else
    {
        m_nTotal = m_nTempTotal/pInfo->m_ShareSystemInfo.nWeightCoe;
        nDot =	(m_nTempTotal%(pInfo->m_ShareSystemInfo.nWeightCoe));
        printf("0-------------m_nTotal:%d--%d--%d--%d\n", m_nWeight, m_nPieces, m_nPrice, m_nTotal);
    }
    if (m_nTotal >= 999999)
    {
        // 总价超出显示时，报警提示
        pInfo->ShowMessage("报警:总价超出显示");
        ATBeep();
        sprintf(pInfo->shareInfoSaleUi.totalprice ,"%ld",m_nTotal);
    }
    else
    {
        if (pInfo->m_ShareUserSwitchInfo.TotalPriceSwitch == 1 && m_bSelectPlu == false)
        {
            m_nTotal = 0;//无商品不显示总价
            memset(pInfo->shareInfoSaleUi.totalprice,0x00,sizeof(pInfo->shareInfoSaleUi.totalprice));
            sprintf(pInfo->shareInfoSaleUi.totalprice ,"%s","");
            pInfo->ShowMessage("无商品不显示总价");
        }
        else
        {
            sprintf(pInfo->shareInfoSaleUi.totalprice ,"%ld",m_nTotal);
        }
    }
}

int CBaseClass::EditNum(unsigned char key, int nDecNum)
{

    int dot = dots(m_strInput);
    int len = strlen(m_strInput);
    //printf("EditNum------------------------------len: %d--dot:%d--m_strInput:%s\n", len, dot,m_strInput);
    if (dot >= 0)
    {
        // m_strInput中有小数点
        if ((key ==_KEY_DOT) || (dot==nDecNum))
        {
            // 多次输入小数点或者小数点位置超出nDecNum时，不处理
            ATBeepErr();
            return -1;
        }
        m_strInput[len] = key - _KEY_NUM0 + '0';
    }
    else
    {
        // m_strInput中无小数点
        if (len < (MAXLEN_PRICE-nDecNum))
        {
            if (key == _KEY_DOT)
            {
                m_strInput[len] = '.';
            }
            else if (len == (MAXLEN_PRICE-nDecNum-1))
            {
                m_strInput[len++] = '.';
                m_strInput[len] = key -_KEY_NUM0 + '0';
            }
            else
            {
                m_strInput[len] = key - _KEY_NUM0 + '0';
            }
        }
        else
        {
            ATBeepErr();
            return -2;
        }
    }

    m_strInput[len+1]= 0;
    len = strlen(m_strInput);
    dot = dots(m_strInput);
    //printf("3------------------------------len: %d--%d\n", len, dot);
    if (dot < 0)
    {
            // m_strprice没有小数点
            sprintf(m_strNum,"%.0f", (atoi(m_strInput))*(pow(10, nDecNum)));
            printf("1------------------------------m_strNum: %s\n",m_strNum);
    }
    else
    {
            // m_strprice有小数点
            char temp[MAXLEN_PRICE+1] = {0};
            memcpy(temp, m_strInput, len-dot-1);			// 先复制整数位
            //printf("1------------------------------temp: %s\n",temp);
            memcpy(temp+len-dot-1, m_strInput+len-dot, dot);	// 再复制小数位
            //printf("2------------------------------temp: %s\n",temp);
            sprintf(m_strNum, "%.0f",  (atoi(temp))*(pow(10, nDecNum-dot)));
            printf("2------------------------------m_strNum: %s\n",m_strNum);
    }
    return 0;
}

void CBaseClass::ClearEditNum(void)
{
    memset(m_strInput,0,sizeof(m_strInput));
    memset(m_strNum,0,sizeof(m_strNum));
}
void CBaseClass::ChangeSaleMode(CShareInfo *pInfo, enSaleMode mode)
{
    char pStr[64] = {0};

    pInfo->SetSaleMode(mode);

    switch (mode)
    {
        case enWeight:
                pInfo->shareInfoSaleUi.saleMode = 0;
                break;

        case enPieces:
                pInfo->shareInfoSaleUi.saleMode = 1;
                break;

        default:
                pInfo->shareInfoSaleUi.saleMode = 0;
                break;
    }
}

