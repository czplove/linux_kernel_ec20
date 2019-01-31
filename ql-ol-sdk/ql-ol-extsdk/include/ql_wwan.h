#ifndef __QL_WWAN_H__
#define __QL_WWAN_H__

//#include "inc/dsi_netctrl.h"

typedef void (* nw_init_cb_func)(void* cb_data);


// init dsi_netctrl library, set callback
typedef void (* dsi_init_cb_func)(void* cb_data);


//--------------------------------------------------------------------------------------------------  
/**                                                                                                   
 * Handler of wwan connection state changes.                                                    
 *                                                                                                    
 *                                                                                                    
 * @param state                                                                                       
 *        Parameter ready to receive the wwan connection state.                                  
 * @param contextPtr                                                                                  
 */                                                                                                   
//--------------------------------------------------------------------------------------------------  
typedef void (*QL_WWAN_ConnectionStateHandlerFunc_t)                                                  
(                                                                                                     
    int     profileIdx, //const char* intfName,
    bool    isConnected,
    void    *contextPtr
); 

//--------------------------------------------------------------------------------------------------  
/**                                                                                                   
 * Add handler function for EVENT NetRegStateEvent
 * 
 * This event provides information on network registration state changes. 
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------  
extern int QL_WWAN_AddConnStateEventHandler                                 
(
    QL_WWAN_ConnectionStateHandlerFunc_t    handlerPtr, ///< [IN]                                                                                                      
    void                                    *contextPtr ///< [IN]                                                                                     
);   


/*   
    Init wlan session, and the result will be callback via cb_func.     
*/   
extern int QL_WWAN_Initialize(dsi_init_cb_func cb_func, void* cb_data);

/*   
    Deinit wlan session, release the resource.     
*/   
extern int QL_WWAN_Deinitialize(void);

/*
    get data service handle
*/
dsi_hndl_t QL_WWAN_GetDataSrvHandle(dsi_net_ev_cb cb_func, void * cb_data);


/*
    release data service handle
*/
void QL_WWAN_ReleaseDataSrvHandle(dsi_hndl_t hdl);

/*
    Set parameters (such as profile id, IP type, APN, username, passwd)
*/
int QL_WWAN_SetProfileId(dsi_hndl_t hdl, int nIndex);


/*
    Set IP Version
*/
int QL_WWAN_SetIPVersion(dsi_hndl_t hdl, int ip_version);


/*
    Start Data call
*/
int QL_WWAN_StartDataCall(dsi_hndl_t hdl);


/*
    Stop Data call
*/
int QL_WWAN_StopDataCall( dsi_hndl_t hdl);


/*
    Get Device name
*/
int QL_WWAN_GetDeviceName(dsi_hndl_t hdl, char * buf, int len);


/*
    Get IP Addr.
*/
int QL_WWAN_GetIPAddr(/*[in]*/dsi_hndl_t hdl, /*[out]*/dsi_addr_info_t * info_ptr, /*[out]*/int *len);


/*
    Get Data Bearer Tech.
*/
dsi_data_bearer_tech_t QL_WWAN_GetDataBearerTech(dsi_hndl_t hdl);


/*
    Get Data end reason.
*/
int QL_WWAN_GetDataEndReason(dsi_hndl_t hdl, dsi_ce_reason_t *ce_reason, dsi_ip_family_t ipf);


/*
    Get APN related information: apn name/username/pwd
*/
int QL_WWAN_GetAPN    
(     
    dsi_hndl_t        hdl,        /* [IN] dsi handle*/
    int               apn_id,       ///< [IN] id: 1~5, 1 means APN1      
    char              *apn,         ///< [IN] The Access Point Name    
    int                apnLen,   
    char              *userName,    ///< [IN] UserName used by authentication    
    int                userLen,    
    char              *password,    ///< [IN] Password used by authentication 
    int                pwdLen
);

/*
    Set APN related information: apn name/username/pwd
*/
int QL_WWAN_SetAPN
(     
    dsi_hndl_t              hdl,        /* [IN] dsi handle*/
    int                        apn_id,     /* [IN] id: 0~4, 0 means APN1    */   
    const char              *apn,       /* [IN] The Access Point Name    */
    const char              *userName,  /* [IN] UserName used by authentication     */
    const char              *password   /* [IN] Password used by authentication 	*/
);

/*
    Set authorization preference
*/
int QL_WWAN_SetAuthPref(dsi_hndl_t hdl, dsi_auth_pref_t auth);

#endif //__QL_WWAN_H__

