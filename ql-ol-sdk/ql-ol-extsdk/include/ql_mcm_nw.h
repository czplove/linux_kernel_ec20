/**
 *@file     ql_mcm_nw.h
 *@date     2018-02-22
 *@author   
 *@brief    
 */   
#ifndef __QL_MCM_NW_H__
#define __QL_MCM_NW_H__

typedef uint32 nw_client_handle_type;


#define QL_MCM_NW_MODE_NONE     0x00    /**<  No network. */
#define QL_MCM_NW_MODE_GSM      0x01    /**<  Include GSM networks. */
#define QL_MCM_NW_MODE_WCDMA    0x02    /**<  Include WCDMA networks. */
#define QL_MCM_NW_MODE_CDMA     0x04    /**<  Include CDMA networks. */
#define QL_MCM_NW_MODE_EVDO     0x08    /**<  Include EVDO networks. */
#define QL_MCM_NW_MODE_LTE      0x10    /**<  Include LTE networks. */
#define QL_MCM_NW_MODE_TDSCDMA  0x20    /**<  Include TDSCDMA networks. */
#define QL_MCM_NW_MODE_PRL      0x10000 /**<  Give preference according to the preferred roaming list. */

typedef enum 
{
    E_QL_MCM_NW_ROAM_STATE_OFF  = 0,    /**<  None, or roaming indicator off. */
    E_QL_MCM_NW_ROAM_STATE_ON   = 1     /**<  Roaming indicator on. */
}E_QL_MCM_NW_ROAM_STATE_TYPE_T;

/** Configures the settings that define the MCM network interface. */
typedef struct 
{
    /*  Configuration parameters for MCM network registration Network registration details Technology dependent network registration details */
    uint64_t                        preferred_nw_mode;  /**<   Preferred network mode for connections; a bitmask of QL_MCM_NW_MODE_xxxx.*/
    E_QL_MCM_NW_ROAM_STATE_TYPE_T   roaming_pref;       /**<   Roaming preference.*/
}QL_MCM_NW_CONFIG_INFO_T;

#define QL_MCM_NW_NITZ_BUF_LEN 30
/** Get NITZ Time information. */
typedef struct 
{
    char        nitz_time[QL_MCM_NW_NITZ_BUF_LEN + 1];
    uint64_t    abs_time;   // 0 means invalid
    int8_t      leap_sec;   // 0 means invalid
}QL_MCM_NW_NITZ_TIME_INFO_T;


//defined for QL_MCM_NW_EventRegister
#define     NW_IND_VOICE_EVENT_ON                   (1 << 0)
#define     NW_IND_DATA_EVENT_ON                    (1 << 1)
#define     NW_IND_SIGNAL_STRENGTH_EVENT_ON         (1 << 2)
#define     NW_IND_CELL_ACCESS_STATE_CHG_EVENT_ON   (1 << 3)
#define     NW_IND_NITZ_TIME_UPDATE_EVENT_ON        (1 << 4)


typedef struct 
{
    char long_eons[512 + 1];    /**<   Long EONS.*/
    char short_eons[512 + 1];   /**<   Short EONS.*/
    char mcc[3 + 1];            /**<   Mobile country code.*/
    char mnc[3 + 1];            /**<   Mobile network code.*/
}QL_MCM_NW_OPERATOR_NAME_INFO_T;

typedef enum 
{
    E_QL_MCM_NW_NETWORK_STATUS_NONE             = 0,    /**<  Network status not available. */
    E_QL_MCM_NW_NETWORK_STATUS_CURRENT_SERVING  = 1,    /**<  Current serving network. */
    E_QL_MCM_NW_NETWORK_STATUS_PREFERRED        = 2,    /**<  Preferred network. */
    E_QL_MCM_NW_NETWORK_STATUS_NOT_PREFERRED    = 3,    /**<  Not the preferred network. */
    E_QL_MCM_NW_NETWORK_STATUS_AVAILABLE        = 4,    /**<  Service available. */
    E_QL_MCM_NW_NETWORK_STATUS_FORBIDDEN        = 5     /**<  Forbidden service. */
}E_QL_MCM_NW_NETWORK_STATUS_TYPE_T;

typedef enum 
{
    E_QL_MCM_NW_RADIO_TECH_TD_SCDMA = 1,
    E_QL_MCM_NW_RADIO_TECH_GSM      = 2,    /**<  GSM; only supports voice. */
    E_QL_MCM_NW_RADIO_TECH_HSPAP    = 3,    /**<  HSPA+. */
    E_QL_MCM_NW_RADIO_TECH_LTE      = 4,    /**<  LTE. */
    E_QL_MCM_NW_RADIO_TECH_EHRPD    = 5,    /**<  EHRPD. */
    E_QL_MCM_NW_RADIO_TECH_EVDO_B   = 6,    /**<  EVDO B. */
    E_QL_MCM_NW_RADIO_TECH_HSPA     = 7,    /**<  HSPA. */
    E_QL_MCM_NW_RADIO_TECH_HSUPA    = 8,    /**<  HSUPA. */
    E_QL_MCM_NW_RADIO_TECH_HSDPA    = 9,    /**<  HSDPA. */
    E_QL_MCM_NW_RADIO_TECH_EVDO_A   = 10,   /**<  EVDO A. */
    E_QL_MCM_NW_RADIO_TECH_EVDO_0   = 11,   /**<  EVDO 0. */
    E_QL_MCM_NW_RADIO_TECH_1xRTT    = 12,   /**<  1xRTT. */
    E_QL_MCM_NW_RADIO_TECH_IS95B    = 13,   /**<  IS95B. */
    E_QL_MCM_NW_RADIO_TECH_IS95A    = 14,   /**<  IS95A. */
    E_QL_MCM_NW_RADIO_TECH_UMTS     = 15,   /**<  UMTS. */
    E_QL_MCM_NW_RADIO_TECH_EDGE     = 16,   /**<  EDGE. */
    E_QL_MCM_NW_RADIO_TECH_GPRS     = 17,   /**<  GPRS. */
    E_QL_MCM_NW_RADIO_TECH_NONE     = 18    /**<  No technology selected. */
}E_QL_MCM_NW_RADIO_TECH_TYPE_T;

typedef struct 
{
    QL_MCM_NW_OPERATOR_NAME_INFO_T      operator_name;    /**<   Operator name.*/
    E_QL_MCM_NW_NETWORK_STATUS_TYPE_T   network_status;    /**<   Network status.*/
    E_QL_MCM_NW_RADIO_TECH_TYPE_T       rat;    /**<   Radio technology.*/
}QL_MCM_NW_SCAN_ENTRY_INFO_T;  /* Type */

#define QL_MCM_NW_SCAN_LIST_MAX 40
/** Gets the status associated with the connection of \<id\>. */
typedef struct 
{
    uint32_t                    entry_len;  /**< Must be set to the number of elements in entry. */
    QL_MCM_NW_SCAN_ENTRY_INFO_T entry[QL_MCM_NW_SCAN_LIST_MAX];    /**<   Scan entry.*/
}QL_MCM_NW_SCAN_RESULT_LIST_INFO_T;  /* Message */


typedef enum 
{
    E_QL_MCM_NW_TECH_DOMAIN_NONE    = 0,    /**<  None. */
    E_QL_MCM_NW_TECH_DOMAIN_3GPP    = 1,    /**<  3GPP. */
    E_QL_MCM_NW_TECH_DOMAIN_3GPP2   = 2,    /**<  3GPP2. */
}E_QL_MCM_NW_TECH_DOMAIN_TYPE_T;

typedef enum 
{
    E_QL_MCM_NW_IMSI_UNKNOWN_HLR_DENY_REASON                    = 1, /**<  IMSI unknown in HLR. */
    E_QL_MCM_NW_ILLEGAL_MS_DENY_REASON                          = 2, /**<  Illegal MS. */
    E_QL_MCM_NW_IMSI_UNKNOWN_VLR_DENY_REASON                    = 3, /**<  IMSI unknown in VLR. */
    E_QL_MCM_NW_IMEI_NOT_ACCEPTED_DENY_REASON                   = 4, /**<  IMEI not accepted. */
    E_QL_MCM_NW_ILLEGAL_ME_DENY_REASON                          = 5, /**<  Illegal ME. */
    E_QL_MCM_NW_PLMN_NOT_ALLOWED_DENY_REASON                    = 6, /**<  PLMN not allowed. */
    E_QL_MCM_NW_LA_NOT_ALLOWED_DENY_REASON                      = 7, /**<  Location area not allowed. */
    E_QL_MCM_NW_ROAMING_NOT_ALLOWED_LA_DENY_REASON              = 8, /**<  Roaming not allowed in this location area. */
    E_QL_MCM_NW_NO_SUITABLE_CELLS_LA_DENY_REASON                = 9, /**<  No suitable cells in location area. */
    E_QL_MCM_NW_NETWORK_FAILURE_DENY_REASON                     = 10, /**<  Network failure. */
    E_QL_MCM_NW_MAC_FAILURE_DENY_REASON                         = 11, /**<  MAC failure. */
    E_QL_MCM_NW_SYNCH_FAILURE_DENY_REASON                       = 12, /**<  Sync failure. */
    E_QL_MCM_NW_CONGESTION_DENY_REASON                          = 13, /**<  Congestion. */
    E_QL_MCM_NW_GSM_AUTHENTICATION_UNACCEPTABLE_DENY_REASON     = 14, /**<  GSM authentication unacceptable. */
    E_QL_MCM_NW_NOT_AUTHORIZED_CSG_DENY_REASON                  = 15, /**<  Not authorized in this CSG. */
    E_QL_MCM_NW_SERVICE_OPTION_NOT_SUPPORTED_DENY_REASON        = 16, /**<  Service option not supported. */
    E_QL_MCM_NW_REQ_SERVICE_OPTION_NOT_SUBSCRIBED_DENY_REASON   = 17, /**<  Requested service option not subscribed. */
    E_QL_MCM_NW_CALL_CANNOT_BE_IDENTIFIED_DENY_REASON           = 18, /**<  Call cannot be identified. */
    E_QL_MCM_NW_SEMANTICALLY_INCORRECT_MSG_DENY_REASON          = 19, /**<  Semantically incorrect message. */
    E_QL_MCM_NW_INVALID_MANDATORY_INFO_DENY_REASON              = 20, /**<  Invalid mandatory information. */
    E_QL_MCM_NW_MSG_TYPE_NON_EXISTENT_DENY_REASON               = 21, /**<  Message type non-existent or not implemented. */
    E_QL_MCM_NW_INFO_ELEMENT_NON_EXISTENT_DENY_REASON           = 22, /**<  Message type not compatible with the protocol state. */
    E_QL_MCM_NW_CONDITIONAL_IE_ERR_DENY_REASON                  = 23, /**<  Conditional IE error. */
    E_QL_MCM_NW_MSG_INCOMPATIBLE_PROTOCOL_STATE_DENY_REASON     = 24, /**<  Message not compatible with the protocol state. */
    E_QL_MCM_NW_PROTOCOL_ERROR_DENY_REASON                      = 25, /**<  Unspecified protocol error. */
}E_QL_MCM_NW_DENY_REASON_TYPE_T;


typedef enum 
{
    E_QL_MCM_NW_SERVICE_NONE     = 0x0000,  /**<  Not registered or no data. */
    E_QL_MCM_NW_SERVICE_LIMITED  = 0x0001,  /**<  Registered; emergency service only. */
    E_QL_MCM_NW_SERVICE_FULL     = 0x0002,  /**<  Registered, full service. */
}E_QL_MCM_NW_SERVICE_TYPE_T;

typedef struct 
{
    E_QL_MCM_NW_TECH_DOMAIN_TYPE_T  tech_domain;        /**<   Technology, used to determine the structure type  mcm_tech: 0 -- None, 1 -- 3GPP, 2 -- 3GPP2.*/
    E_QL_MCM_NW_RADIO_TECH_TYPE_T   radio_tech;         /**<   Radio technology; see #mcm_nw_radio_tech_t_v01.*/
    E_QL_MCM_NW_ROAM_STATE_TYPE_T   roaming;            /**<   0 -- Off, 1 -- Roaming (3GPP2 has extended values).*/
    E_QL_MCM_NW_DENY_REASON_TYPE_T  deny_reason;        /**<   Set when registration state is #mcm_nw_deny_reason_t_v01.*/
    E_QL_MCM_NW_SERVICE_TYPE_T      registration_state; /**<   Registration state.*/
}QL_MCM_NW_COMMON_REG_INFO_T;


typedef struct 
{
    E_QL_MCM_NW_TECH_DOMAIN_TYPE_T  tech_domain;        /**<   Technology, used to determine the structure type  mcm_tech: 0 -- None, 1 -- 3GPP, 2 -- 3GPP2.*/
    E_QL_MCM_NW_RADIO_TECH_TYPE_T   radio_tech;         /**<   Radio technology; see #mcm_nw_radio_tech_t_v01.*/
    char                            mcc[3+1];           /**<   Mobile country code.*/
    char                            mnc[3+1];           /**<   Mobile network code.*/
    E_QL_MCM_NW_ROAM_STATE_TYPE_T   roaming;            /**<   0 -- Off, 1 -- Roaming (3GPP2 has extended values).*/
    uint8_t                         forbidden;          /**<   Forbidden: 0 -- No, 1 -- Yes.*/
    uint32_t                        cid;                /**<   Cell ID for the registered 3GPP system.*/
    uint16_t                        lac;                /**<   Locatin area code for the registered 3GPP system.*/
    uint16_t                        psc;                /**<   Primary scrambling code (WCDMA only); 0 -- None.*/
    uint16_t                        tac;                /**<   Tracking area code information for LTE.*/
}QL_MCM_NW_3GPP_REG_INFO_T;


typedef struct 
{
    E_QL_MCM_NW_TECH_DOMAIN_TYPE_T  tech_domain;        /**<   Technology, used to determine structure type  mcm_tech: 0 -- None, 1 -- 3GPP, 2 -- 3GPP2.*/
    E_QL_MCM_NW_RADIO_TECH_TYPE_T   radio_tech;         /**<   Radio technology; see #mcm_nw_radio_tech_t_v01.*/
    char                            mcc[3+1];           /**<   Mobile country code.*/
    char                            mnc[3+1];           /**<   Mobile network code.*/
    E_QL_MCM_NW_ROAM_STATE_TYPE_T   roaming;            /**<   Roaming status; see #mcm_nw_roam_state_t_v01.*/
    uint8_t                         forbidden;          /**<   Forbidden: 0 -- No, 1 -- Yes.*/
    uint8_t                         inPRL;              /**<   0 -- Not in PRL, 1 -- In PRL.*/
    uint8_t                         css;                /**<   Concurrent services supported: 0 -- No, 1 -- Yes.*/
    uint16_t                        sid;                /**<   CDMA system ID.*/
    uint16_t                        nid;                /**<   CDMA network ID.*/
    uint16_t                        bsid;               /**<   Base station ID. @newpagetable */
}QL_MCM_NW_3GPP2_REG_INFO_T;

/** Gets the status associated with the connection of \<id\>. */
typedef struct 
{
    uint8_t                         voice_registration_valid;               /**< Must be set to TRUE if voice_registration is being passed. */
    QL_MCM_NW_COMMON_REG_INFO_T     voice_registration;                     /**<   Voice registration. */

    uint8_t                         data_registration_valid;                /**< Must be set to TRUE if data_registration is being passed. */
    QL_MCM_NW_COMMON_REG_INFO_T     data_registration;                      /**<   Data registration. */

    uint8_t                         voice_registration_details_3gpp_valid;  /**< Must be set to TRUE if voice_registration_details_3gpp is being passed. */
    QL_MCM_NW_3GPP_REG_INFO_T       voice_registration_details_3gpp;        /**<   Voice registration details for 3GPP. */

    uint8_t                         data_registration_details_3gpp_valid;   /**< Must be set to TRUE if data_registration_details_3gpp is being passed. */
    QL_MCM_NW_3GPP_REG_INFO_T       data_registration_details_3gpp;         /**<   Data registration details for 3GPP. */

    uint8_t                         voice_registration_details_3gpp2_valid; /**< Must be set to TRUE if voice_registration_details_3gpp2 is being passed. */
    QL_MCM_NW_3GPP2_REG_INFO_T      voice_registration_details_3gpp2;       /**<   Voice registration details for 3GPP2. */

    uint8_t                         data_registration_details_3gpp2_valid;  /**< Must be set to TRUE if data_registration_details_3gpp2 is being passed. */
    QL_MCM_NW_3GPP2_REG_INFO_T      data_registration_details_3gpp2;        /**<   Data registration details for 3GPP2. */
}QL_MCM_NW_REG_STATUS_INFO_T;


typedef enum 
{
    E_QL_MCM_NW_SELECTION_AUTOMATIC  = 0,                                   /**<  Automatic network selection. */
    E_QL_MCM_NW_SELECTION_MANUAL     = 1,                                   /**<  Manual network selection. */
}E_QL_MCM_NW_SELECTION_TYPE_T;

typedef struct 
{
    E_QL_MCM_NW_SELECTION_TYPE_T    nw_selection_type;                      /**< Network selection type. */
    char                            mcc[3 + 1];                             /**<   Mobile country code for a manual network selection.*/
    char                            mnc[3 + 1];                             /**<   Mobile network code for a manual network selection.*/
    E_QL_MCM_NW_RADIO_TECH_TYPE_T   rat;                                    /**<   Radio technology.*/
}QL_MCM_NW_SELECTION_INFO_T;


typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates received signal strength. A signed value; -125 or lower indicates no signal.*/
}QL_MCM_NW_GSM_SIGNAL_INFO_T;


typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates forward link pilot Ec. A signed value; -125 or lower indicates no signal.*/
    int16_t     ecio;       /**<   Ec/Io value representing negative 0.5 dB increments, e.g., 2 equals -1 dbm.*/
}QL_MCM_NW_WCDMA_SIGNAL_INFO_T;

typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates forward link pilot Ec.    a signed value; -125 or lower indicates no signal.*/
    int8_t      rscp;       /**<   RSCP in dBm.*/
    int16_t     ecio;       /**<   Ec/Io value representing negative 0.5 dB increments, e.g., 2 equals -1 dbm.*/
    int8_t      sinr;       /**<   Measured SINR in dB. @newpagetable */
}QL_MCM_NW_TDSCDMA_SIGNAL_INFO_T;

typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates forward link pilot Ec. A signed value; -125 or lower indicates no signal.*/
    int8_t      rsrq;       /**<   RSRQ value in dB (signed integer value), as measured by L1. Range: -3 to -20 (-3 equals -3 dB, -20 equals -20 dB).*/
    int8_t      rsrp;       /**<   Current RSRP in dBm, as measured by L1. Range: -44 to -140 (-44 equals -44 dBm, -140 equals -140 dBm).*/
    int8_t      snr;        /**<   SNR level as a scaled integer in units of 0.1 dB; e.g., -16 dB has a value of -160 and 24.6 dB has a value of 246.*/
}QL_MCM_NW_LTE_SIGNAL_INFO_T;


typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates forward link pilot Power (AGC) + Ec/Io. A signed value; -125 or lower indicates no signal.*/
    int16_t     ecio;       /**<   Ec/Io value representing negative 0.5 dB increments, e.g., 2 equals -1 dbm.*/
}QL_MCM_NW_CDMA_SIGNAL_INFO_T;

typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates forward link pilot Power (AGC) + Ec/Io. A signed value; -125 or lower indicates no signal.*/
    int16_t     ecio;       /**<   Ec/Io value representing negative 0.5 dB increments, e.g., 2 equals -1 dbm.*/
    int8_t      sinr;       /**<   SINR level.*/
    int32_t     io;         /**<   Received IO in dBm. */
}QL_MCM_NW_HDR_SIGNAL_INFO_T;

/** Gets signal strength information. */
typedef struct 
{
    uint8_t                         gsm_sig_info_valid;         /**< Must be set to TRUE if gsm_sig_info is being passed. */
    QL_MCM_NW_GSM_SIGNAL_INFO_T     gsm_sig_info;               /**<   GSM signal information. */
    uint8_t                         wcdma_sig_info_valid;       /**< Must be set to TRUE if wcdma_sig_info is being passed. */
    QL_MCM_NW_WCDMA_SIGNAL_INFO_T   wcdma_sig_info;             /**<   WCDMA signal information. */
    uint8_t                         tdscdma_sig_info_valid;     /**< Must be set to TRUE if tdscdma_sig_info is being passed. */
    QL_MCM_NW_TDSCDMA_SIGNAL_INFO_T tdscdma_sig_info;           /**<   TDSCDMA signal information. */
    uint8_t                         lte_sig_info_valid;         /**< Must be set to TRUE if lte_sig_info is being passed. */
    QL_MCM_NW_LTE_SIGNAL_INFO_T     lte_sig_info;               /**<   LTE signal information. */
    uint8_t                         cdma_sig_info_valid;        /**< Must be set to TRUE if cdma_sig_info is being passed. */
    QL_MCM_NW_CDMA_SIGNAL_INFO_T    cdma_sig_info;              /**<   CDMA signal information. */
    uint8_t                         hdr_sig_info_valid;         /**< Must be set to TRUE if hdr_sig_info is being passed. */
    QL_MCM_NW_HDR_SIGNAL_INFO_T     hdr_sig_info;               /**<   HDR signal information. */
}QL_MCM_NW_SIGNAL_STRENGTH_INFO_T;


typedef enum 
{
    E_QL_MCM_NW_CELL_ACCESS_NONE            = 0x00,             /**<  Unknown cell access state. */
    E_QL_MCM_NW_CELL_ACCESS_NORMAL_ONLY     = 0x01,             /**<  Cell access is allowed for normal calls only. */
    E_QL_MCM_NW_CELL_ACCESS_EMERGENCY_ONLY  = 0x02,             /**<  Cell access is allowed for emergency calls only. */
    E_QL_MCM_NW_CELL_ACCESS_NO_CALLS        = 0x03,             /**<  Cell access is not allowed for any call type. */
    E_QL_MCM_NW_CELL_ACCESS_ALL_CALLS       = 0x04,             /**<  Cell access is allowed for all call types. */
}E_QL_MCM_NW_CELL_ACCESS_STATE_TYPE_T;








E_QL_ERROR_CODE_T QL_MCM_NW_Client_Init(nw_client_handle_type  *ph_nw);

E_QL_ERROR_CODE_T QL_MCM_NW_Client_Deinit(nw_client_handle_type  h_nw);

E_QL_ERROR_CODE_T QL_MCM_NW_SetConfig
(
    nw_client_handle_type       h_nw,
    QL_MCM_NW_CONFIG_INFO_T     *pt_info 
);

E_QL_ERROR_CODE_T QL_MCM_NW_GetConfig
(
    nw_client_handle_type       h_nw,
    QL_MCM_NW_CONFIG_INFO_T     *pt_info 
);

E_QL_ERROR_CODE_T QL_MCM_NW_GetNitzTimeInfo
(
    nw_client_handle_type       h_nw,
    QL_MCM_NW_NITZ_TIME_INFO_T *pt_info 
);

E_QL_ERROR_CODE_T QL_MCM_NW_EventRegister
(
    nw_client_handle_type       h_nw,
    uint32_t                    bitmask // bit OR of NW_IND_xxxx_EVENT_ON
);

E_QL_ERROR_CODE_T QL_MCM_NW_GetOperatorName
(
    nw_client_handle_type           h_nw,
    QL_MCM_NW_OPERATOR_NAME_INFO_T  *pt_info //You should malloc this or may cause stack overflow
);

E_QL_ERROR_CODE_T QL_MCM_NW_PerformScan
(
    nw_client_handle_type               h_nw,
    QL_MCM_NW_SCAN_RESULT_LIST_INFO_T   *pt_info 
);

E_QL_ERROR_CODE_T QL_MCM_NW_GetRegStatus
(
    nw_client_handle_type               h_nw,
    QL_MCM_NW_REG_STATUS_INFO_T         *pt_info 
);

E_QL_ERROR_CODE_T QL_MCM_NW_SetLowPowerMode
(
    nw_client_handle_type       h_nw,
    uint32_t                    low_power_mode_on // 0: off, other: on
);

E_QL_ERROR_CODE_T QL_MCM_NW_SetSelection
(
    nw_client_handle_type       h_nw,
    QL_MCM_NW_SELECTION_INFO_T  *pt_info
);

E_QL_ERROR_CODE_T QL_MCM_NW_GetSignalStrength
(
    nw_client_handle_type               h_nw,
    QL_MCM_NW_SIGNAL_STRENGTH_INFO_T    *pt_info
);

E_QL_ERROR_CODE_T QL_MCM_NW_GetCellAccessState
(
    nw_client_handle_type                   h_nw,
    E_QL_MCM_NW_CELL_ACCESS_STATE_TYPE_T    *pe_state
);




#endif//__QL_MCM_NW_H__
