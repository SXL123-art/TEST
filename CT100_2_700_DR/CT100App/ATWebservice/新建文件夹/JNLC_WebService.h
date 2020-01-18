#ifndef JNLC_WEBSERVICE_H
#define JNLC_WEBSERVICE_H

#include "../unit/shareInfo.h"

using namespace std;
typedef struct {
    string WebUrl;
    string auth_id;
    string key;
}JNLC_WebPublic;

int JNLC_GetServerSystemTime(JNLC_WebPublic webPublic,CShareInfo *m_pInfo);

int JNLC_GetGoodsList(JNLC_WebPublic webPublic,CShareInfo *m_pInfo);

int JNLC_EnterDataToScales(JNLC_WebPublic webPublic,CShareInfo *m_pInfo);

int JNLC_GetTraceCode(JNLC_WebPublic webPublic,CShareInfo *m_pInfo);

int JNLC_PutTransData(JNLC_WebPublic webPublic,CShareInfo *m_pInfo);

#endif // JNLC_WEBSERVICE_H
