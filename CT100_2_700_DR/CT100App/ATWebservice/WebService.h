#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include "../unit/shareInfo.h"

using namespace std;

typedef struct {
    string weburl;
    int pluFlags;
    int userFlags;
}WebPublic;


int HeartBeat(WebPublic webPublic, CShareInfo *m_pInfo);
int GetServerTime(WebPublic webPublic, CShareInfo *m_pInfo);
int GetUserInfo(WebPublic webPublic, CShareInfo *m_pInfo);
int GetPluInfo(WebPublic webPublic, CShareInfo *m_pInfo);
int UploadSaleInfo(WebPublic webPublic, CShareInfo *m_pInfo);

#endif // WEBSERVICE_H
