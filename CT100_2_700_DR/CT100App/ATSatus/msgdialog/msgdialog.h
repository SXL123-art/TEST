#ifndef MSGDIALOG_H
#define MSGDIALOG_H
#include "../../unit/shareInfo.h"

int showMsgProc(CShareInfo *m_pShareInfo,enMsgType MsgType, char *pTitle,char *pMSG);
void SetInputContent(CShareInfo *m_pShareInfo,const char *pTitle,const char *pData);
int ProcessMessage(CShareInfo *m_pShareInfo,enMsgType MsgType);

int showInputMsgProc(CShareInfo *m_pShareInfo,enInputType InputType, char *pMSG);
int CheckInputEnter(CShareInfo *m_pShareInfo,enInputType MsgType);

#endif
