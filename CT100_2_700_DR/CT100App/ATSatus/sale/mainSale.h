#ifndef MAINSALE_H
#define MAINSALE_H
#include "../../unit/shareinfo.h"
#include "../baseclass.h"
#include "../msgdialog/msgdialog.h"
#include "salefunction.h"

#define KEY_ADD1  1
#define KEY_ADD2  2
#define KEY_ADD3  3
#define KEY_ADD4  4
#define BUSSINESS_SIZE 20
#define BATCH_SIZE 2

class CMainSale : public CBaseClass
{
public:
        CMainSale(CShareInfo *pInfo);

        ~CMainSale(void);

        enQtShowType SaleProc();
        int AutoSaveSale(void);

private:
        CSaleFunction *m_pSaleFunction;
        enQtShowType SaleDeal(); 
        int InputPriceOrPieces(unsigned char ch);
        int showMsg(enMsgType MsgType,char *pTitle,char *pMSG);
        int showInputMsg(enInputType InputType, char *pInputMSG);
        int CheckSaleStatus(void);
        int FindPluInfoByKey(void);
        int PluPage(unsigned char ch,unsigned char PluPage);
        int AddFnSale(unsigned char AddFn);
        int SaveFnSale(void);
        int SubtotalSaleFn(void);
        int SaveSaleToDBFn(void);

        int DeleteAllSale(unsigned char Fn);
        int CancelSale();
        int RefreshSaleInfo();
        int FindFunctionKey(void);
        int PrintSale(void);
        int PrintSaleForFn(stReceiptData data);

        int showAddSaleMsg(struct_tb_business *stbFn);
        int showAddSaleMsgProc(bool isWeight);

public:
        long 	m_nLastWeight;
        long 	m_nLastPrice;
        long 	m_nLastTotal;

        bool m_bAccumulative;           //累计后不强制记录
        bool m_bZeroTareAccumulative;   //按去皮置零不打印，不强制记录

        CShareInfo *m_pSaleShareInfo;

        unsigned char	m_nFn;
        struct_tb_business 	*stbF1;
        struct_tb_business 	*stbF2;
        struct_tb_business 	*stbF3;
        struct_tb_business 	*stbF4;

        long m_nSaleTotalF1;                            // 合计总额
        unsigned char m_nSaleNumF1;			// 总的交易数
        long m_nSaleTotalF2;                            // 合计总额
        unsigned char m_nSaleNumF2;			// 总的交易数
        long m_nSaleTotalF3;                            // 合计总额
        unsigned char m_nSaleNumF3;			// 总的交易数
        long m_nSaleTotalF4;                            // 合计总额
        unsigned char m_nSaleNumF4;
        long m_nSaleTotal;                              // 合计总额
        long m_nDiscountSaleTotal;                      // 合计折后总额
        unsigned char m_nSaleNum;                       // 总的交易数
        unsigned char m_nCurrentSaleNum;                //当前选择的交易项

        bool m_bSelectPluNum;               //是否是通过快捷键查找PLU
        unsigned int m_nPluHotKey;          //查找plu键值
        unsigned int m_nFunctionKey;        //查找功能键
        QDateTime CurrentTime;              //记录时间
};

#endif
