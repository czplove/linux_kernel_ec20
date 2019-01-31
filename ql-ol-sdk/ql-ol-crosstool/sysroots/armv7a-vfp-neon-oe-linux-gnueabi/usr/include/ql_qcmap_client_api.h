/**
 *@file     ql_qcmap_client_api.h
 *@date     2017-09-04
 *@author   running.qian
 *@brief    Quecte QCMAP Client API
 */
 
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    QL_WWAN_NW_DISCONNECTED = 0,
    QL_WWAN_NW_CONNECTED,
    
    QL_WWAN_NW_CONNECTING,//include reconnecting
    QL_WWAN_NW_CONNECTING_TIMEOUT,
} ql_wwan_nw_staus_e_type;

typedef void (*NW_Status_CB)(ql_wwan_nw_staus_e_type nw_status);


int  Ql_NW_MobileAP_Init(void);

int  Ql_NW_MobileAP_Enable(int timeout, NW_Status_CB nw_status_cb);//timeout =0, means using defaule value 10s
void Ql_NW_MobileAP_Disable(void);

int  Ql_NW_MoblieAP_ProfileId_Set(int profile_id);//ret < 0, fail
int  Ql_NW_MoblieAP_ProfileId_Get(int *profile_id);//ret < 0, fail
int  Ql_NW_MoblieAP_AutoConnect_Set(boolean enable);//ret < 0, fail
int  Ql_NW_MoblieAP_AutoConnect_Get(boolean *enable);//ret < 0, fail

//Old API
int  Ql_NW_EnableMobileAP(int timeout, NW_Status_CB nw_status_cb);//ret < 0.failtimeout > 0s, nw_status_cb can be null;
void Ql_NW_DisableMobileAP(void);
#ifdef __cplusplus
}
#endif