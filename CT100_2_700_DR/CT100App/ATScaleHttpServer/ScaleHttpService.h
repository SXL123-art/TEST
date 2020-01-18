#ifndef SCALEHTTPSERVICE_H
#define SCALEHTTPSERVICE_H
#include "../unit/ShareInfo.h"
#include <iostream>
using namespace std;
//更新电子秤服务器时间
int UpdataScaleTime(CShareInfo *m_pInfo,string & sendData,string & getData);
//获取电子秤时间或检查电子秤时间
int CheckOrGetScaleTime(CShareInfo *m_pInfo,string & sendData,string & getData);
//更新电子秤网络参数
int UpdataNetParameter(CShareInfo *m_pInfo,string & sendData,string & getData);
//获取电子秤网络信息
int GetNetParameter(CShareInfo *m_pInfo,string & sendData,string & getData);
//更新电子秤系统信息
int UpdataScaleSystemInfo(CShareInfo *m_pInfo,string & sendData,string & getData);
//获取电子秤系统信息
int GetScaleSystemInfo(CShareInfo *m_pInfo,string & sendData,string & getData);
//更新电子秤用户信息
int UpdataScaleUserInfo(CShareInfo *m_pInfo,string & sendData,string & getData);
//获取电子秤用户信息
int GetScaleUserInfo(CShareInfo *m_pInfo,string & sendData,string & getData);
//更新电子秤登录用户信息
int UpdataScaleLoginUser(CShareInfo *m_pInfo,string & sendData,string & getData);
//获取电子秤用户登录信息
int GetScaleLoginUser(CShareInfo *m_pInfo,string & sendData,string & getData);
//更新电子秤用户开关
int UpdataScaleUserSwitch(CShareInfo *m_pInfo,string & sendData,string & getData);
//获取电子秤用户开关
int GetScaleUserSwitch(CShareInfo *m_pInfo,string & sendData,string & getData);
//更新电子秤系统开关
int UpdataScaleSystemSwitch(CShareInfo *m_pInfo,string & sendData,string & getData);
//获取电子秤系统开关
int GetScaleSystemSwitch(CShareInfo *m_pInfo,string & sendData,string & getData);
//更新电子秤商品列表
int UpdataPLUList(CShareInfo *m_pInfo,string & sendData,string & getData);
//获取电子秤商品列表
int GetScalePluList(CShareInfo *m_pInfo,string & sendData,string & getData);
//更新电子秤商品快捷键
int UpdataGoodsHotKey(CShareInfo *m_pInfo,string & sendData,string & getData);
//获取电子秤商品快捷键
int GetGoodsHotKey(CShareInfo *m_pInfo,string & sendData,string & getData);
//获取电子秤交易信息
int GetScaleSaleInfo(CShareInfo *m_pInfo,string & sendData,string & getData);
//电子秤软件升级
int UpdataScaleProgram(CShareInfo *m_pInfo,string & sendData,string & getData);
//电子秤执行SQL
int PerformSQL(CShareInfo *m_pInfo,string & sendData,string & getData);
#endif // SCALEHTTPSERVICE_H
