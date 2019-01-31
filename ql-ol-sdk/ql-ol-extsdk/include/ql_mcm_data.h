/**
 *@file     ql_mcm_data.h
 *@date     2018-02-13
 *@author   
 *@brief    
 */   

#ifndef __QL_MCM_DATA_H__
#define __QL_MCM_DATA_H__

#include "ql_mcm.h"



typedef uint32 data_client_handle_type;

typedef enum 
{
    E_QL_MCM_DATA_IP_FAMILY_UNKNOWN = 0,    /**<  unknown  */
    E_QL_MCM_DATA_IP_FAMILY_IPV4    = 4,    /**<  IPv4  */
    E_QL_MCM_DATA_IP_FAMILY_IPV6    = 6,    /**<  IPv4  */
    E_QL_MCM_DATA_IP_FAMILY_IPV4V6  = 10    /**<  IPv4/v6  */
}E_QL_MCM_DATA_IP_FAMILY_TYPE_T;

typedef enum 
{
    E_QL_MCM_DATA_TECH_PREF_UNKNOWN = 0,    /**<  unknown  */
    E_QL_MCM_DATA_TECH_PREF_CDMA    = 1,    /**<  CDMA  */
    E_QL_MCM_DATA_TECH_PREF_UMTS    = 2,    /**<  UMTS  */
    E_QL_MCM_DATA_TECH_PREF_AUTO    = 7     /**<  AUTO  */
}E_QL_MCM_DATA_TECH_PREF_TYPE_T;


#define QL_MCM_DATA_MAX_APN_LEN         150 /**  Maximum length of the APN. */
#define QL_MCM_DATA_MAX_USERNAME_LEN    127 /**  Maximum length of the profile user name. */
#define QL_MCM_DATA_MAX_PASSWORD_LEN    127 /**  Maximum length of the password. */
typedef struct 
{
    uint8_t                         ip_family_valid;                        /**< Must be set to TRUE if ip_family is being passed. */
    E_QL_MCM_DATA_IP_FAMILY_TYPE_T  ip_family;                              /*  MCM Data IP Family */
    uint8_t                         apn_name_valid;                         /**< Must be set to TRUE if apn_name is being passed. */
    char                            apn_name[QL_MCM_DATA_MAX_APN_LEN];      /**< MCM Data APN Name. A character string that identifies a PDN, as defined by the operator.  */
    uint8_t                         user_name_valid;                        /**< Must be set to TRUE if user_name is being passed. */
    char                            user_name[QL_MCM_DATA_MAX_USERNAME_LEN];/**< Username for the APN. */
    uint8_t                         password_valid;                         /**< Must be set to TRUE if password is being passed. */
    char                            password[QL_MCM_DATA_MAX_PASSWORD_LEN]; /**< Password for the APN. */
    uint8_t                         tech_pref_valid;                        /**< Must be set to TRUE if tech_pref is being passed. */
    E_QL_MCM_DATA_TECH_PREF_TYPE_T  tech_pref;                              /*  MCM Data Tech Preference */
    uint8_t                         umts_profile_valid;                     /**< Must be set to TRUE if umts_profile is being passed. */
    int8_t                          umts_profile;                           /**< MCM Data UMTS/LTE profile ID. */
    uint8_t                         cdma_profile_valid;                     /**< Must be set to TRUE if cdma_profile is being passed. */
    int8_t                          cdma_profile;                           /**< MCM Data CDMA profile ID. */
}QL_MCM_DATA_CALL_CONFIG_INFO_T;


typedef enum 
{
    E_QL_MCM_DATA_CALL_STATE_INVALID            = 0x01, /**<  Call state is invalid.  */
    E_QL_MCM_DATA_CALL_STATE_CONNECTING         = 0x02, /**<  Call is connecting.  */
    E_QL_MCM_DATA_CALL_STATE_CONNECTED          = 0x03, /**<  Call is connected.  */
    E_QL_MCM_DATA_CALL_STATE_DISCONNECTING      = 0x04, /**<  Call is disconnecting.  */
    E_QL_MCM_DATA_CALL_STATE_DISCONNECTED       = 0x05, /**<  Call is disconnected.  */
}E_QL_MCM_DATA_CALL_STATE_TYPE_T;


typedef enum 
{
    E_QL_MCM_DATA_BEARER_TECH_TYPE_UNKNOWN      = 0x00, /**< Unknown technology.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_CDMA_1X      = 0x01, /**<  1X technology.         */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_EVDO_REV0    = 0x02, /**< CDMA Rev 0.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_EVDO_REVA    = 0x03, /**< CDMA Rev A.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_EVDO_REVB    = 0x04, /**< CDMA Rev B.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_EHRPD        = 0x05, /**< EHRPD.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_FMC          = 0x06, /**< Fixed mobile convergence.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_HRPD         = 0x07, /**< HRPD.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_3GPP2_WLAN   = 0x08, /**< IWLAN.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_WCDMA        = 0x09, /**< WCDMA.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_GPRS         = 0x0A, /**< GPRS.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_HSDPA        = 0x0B, /**< HSDPA.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_HSUPA        = 0x0C, /**< HSUPA.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_EDGE         = 0x0D, /**< EDGE.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_LTE          = 0x0E, /**< LTE.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_HSDPA_PLUS   = 0x0F, /**< HSDPA+.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_DC_HSDPA_PLUS= 0x10, /**< DC HSDPA+.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_HSPA         = 0x11, /**< HSPA.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_64_QAM       = 0x12, /**< 64 QAM.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_TDSCDMA      = 0x13, /**< TD-SCDMA.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_GSM          = 0x14, /**< GSM.  */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_3GPP_WLAN    = 0x15, /**< IWLAN.  @vertspace{-4} */
}E_QL_MCM_DATA_BEARER_TECH_TYPE_T;


#define QL_MCM_DATA_ADDR_LEN_MAX    128
typedef struct 
{
    char    valid_addr;                         /**< Indicates whether a valid address is available. */
    struct sockaddr_storage addr;               /**< Stores the IP address. */
}QL_MCM_DATA_CALL_ADDR_T;

typedef struct 
{
    QL_MCM_DATA_CALL_ADDR_T     iface_addr_s;   /**< Network interface address. */
    uint32_t                    iface_mask;     /**< Subnet mask.       */
    QL_MCM_DATA_CALL_ADDR_T     gtwy_addr_s;    /**< Gateway server address. */
    uint32_t                    gtwy_mask;      /**< Gateway mask.     */
    QL_MCM_DATA_CALL_ADDR_T     dnsp_addr_s;    /**< Primary DNS server address. */
    QL_MCM_DATA_CALL_ADDR_T     dnss_addr_s;    /**< Secondary DNS server address. */
}QL_MCM_DATA_CALL_ADDR_INFO_T;

#define QL_MCM_DATA_MAX_ADDR_COUNT  10          /** Maximum number of IP addresses. */
typedef struct 
{
    uint32_t                    addr_info_len;  /**< Must be set to the number of elements in addr_info. */
    QL_MCM_DATA_CALL_ADDR_INFO_T addr_info[QL_MCM_DATA_MAX_ADDR_COUNT];  /**< Data device address list. */
}QL_MCM_DATA_CALL_ADDR_INFO_LIST_T;


typedef struct 
{
    uint32_t    pkts_tx;                        /**< Number of packets transmitted.   */
    uint32_t    pkts_rx;                        /**< Number of packets received. */
    uint64_t    bytes_tx;                       /**< Number of bytes transmitted.   */
    uint64_t    bytes_rx;                       /**< Number of bytes received. */
    uint32_t    pkts_dropped_tx;                /**< Number of transmit packets dropped. */
    uint32_t    pkts_dropped_rx;                /**< Number of receive packets dropped. */
}QL_MCM_DATA_PKT_STATISTIC_INFO_T; 


typedef enum 
{
    E_QL_MCM_DATA_MODEM_STATE_OUT_OF_SERVICE= 0x01,     /**< Modem is out of service.  */
    E_QL_MCM_DATA_MODEM_STATE_IN_SERVICE    = 0x02,     /**< Modem is in service.  @vertspace{-4} */
}E_QL_MCM_DATA_MODEM_STATE_TYPE_T;

typedef struct 
{
    E_QL_MCM_DATA_MODEM_STATE_TYPE_T    srv_status;     /**< Identifies the service state of the modem. */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_T    tech_info;      /**< Identifies the preferred technology type. */
}QL_MCM_DATA_REG_STATUS_INFO_T;


typedef struct 
{
    uint32_t    current_tx_rate;                        /**< Current Tx data rate for the channel. */
    uint32_t    current_rx_rate;                        /**< Current Rx data rate for the channel. */
    uint32_t    max_tx_rate;                            /**< Maximum Tx data rate for the channel. */
    uint32_t    max_rx_rate;                            /**< Maximum Rx data rate for the channel. */
}QL_MCM_DATA_CHANNEL_RATE_INFO_T;

typedef struct 
{
    char    ip_addr[32];        /**< ip address. */
    char    gateway[32];        /**< gateway     */
    char    iface_name[64];     /**< iface name. */    
}QL_MCM_DATA_ROUTE_INFO_T;


typedef enum 
{
    E_QL_MCM_DATA_TYPE_INVALID                      = -1,       /**< Invalid.  */
    E_QL_MCM_DATA_TYPE_UNSPECIFIED                  = 0x00,     /**< Unspecified.  */
    E_QL_MCM_DATA_TYPE_MOBILE_IP                    = 0x01,     /**< Mobile IP.  */
    E_QL_MCM_DATA_TYPE_INTERNAL                     = 0x02,     /**< Internal.  */
    E_QL_MCM_DATA_TYPE_CALL_MANAGER_DEFINED         = 0x03,     /**< Call manager-defined.  */
    E_QL_MCM_DATA_TYPE_3GPP_SPEC_DEFINED            = 0x06,     /**< 3GPP specification-defined.  */
    E_QL_MCM_DATA_TYPE_PPP                          = 0x07,     /**< Point-to-Point protocol.  */
    E_QL_MCM_DATA_TYPE_EHRPD                        = 0x08,     /**< EHRPD.  */
    E_QL_MCM_DATA_TYPE_IPV6                         = 0x09      /**< IPV6.  @newpage */
}E_QL_MCM_DATA_TYPE_T;


typedef enum 
{
    E_QL_MCM_DATA_CE_INVALID                                = -1,   /**<  Invalid call end reason.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_REASON_UNSPECIFIED          = 0,    /**<  Mobile IP; unspecified error.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_ADMINISTRATIVELY_PROHIBITED = 1,    /**<  Mobile IP; administratively prohibited.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_INSUFFICIENT_RESOURCES      = 2,    /**<  Mobile IP; insufficient resources.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_MOBILE_NODE_AUTHENTICATION_FAILURE = 3,/**<  Mobile IP; mobile node authentication failure.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_HA_AUTHENTICATION_FAILURE   = 4,    /**<  Mobile IP; HA authentication failure.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_REQUESTED_LIFETIME_TOO_LONG = 5,    /**<  Mobile IP; requested lifetime is too long.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_MALFORMED_REQUEST           = 6,    /**<  Mobile IP; malformed request.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_MALFORMED_REPLY             = 7,    /**<  Mobile IP; malformed reply.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_ENCAPSULATION_UNAVAILABLE   = 8,    /**<  Mobile IP; encapsulation is unavailable.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_VJHC_UNAVAILABLE            = 9,    /**<  Mobile IP; VJHC is unavailable.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_REVERSE_TUNNEL_UNAVAILABLE  = 10,   /**<  Mobile IP; reverse tunnel is unavailable.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_REVERSE_TUNNEL_IS_MANDATORY_AND_T_BIT_NOT_SET = 11, /**<  Mobile IP; reverse tunnel is mandatory and the T-bit is not set.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_DELIVERY_STYLE_NOT_SUPPORTED= 12,   /**<  Mobile IP; delivery style is not supported.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_MISSING_NAI                 = 13,   /**<  Mobile IP; missing NAI.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_MISSING_HA                  = 14,   /**<  Mobile IP; missing HA.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_MISSING_HOME_ADDR           = 15,   /**<  Mobile IP; missing home address.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_UNKNOWN_CHALLENGE           = 16,   /**<  Mobile IP; unknown challenge.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_MISSING_CHALLENGE           = 17,   /**<  Mobile IP; missing challenge.  */
    E_QL_MCM_DATA_CE_MIP_FA_ERR_STALE_CHALLENGE             = 18,   /**<  Mobile IP; stale challenge.  */
    E_QL_MCM_DATA_CE_MIP_HA_ERR_REASON_UNSPECIFIED          = 19,   /**<  Mobile IP; reason is unspecified.  */
    E_QL_MCM_DATA_CE_MIP_HA_ERR_ADMINISTRATIVELY_PROHIBITED = 20,   /**<  Mobile IP; administratively prohibited.  */
    E_QL_MCM_DATA_CE_MIP_HA_ERR_INSUFFICIENT_RESOURCES      = 21,   /**<  Mobile IP; insufficient resources.  */
    E_QL_MCM_DATA_CE_MIP_HA_ERR_MOBILE_NODE_AUTHENTICATION_FAILURE = 22,    /**<  Mobile IP; mobile node authentication failure.  */
    E_QL_MCM_DATA_CE_MIP_HA_ERR_FA_AUTHENTICATION_FAILURE   = 23,   /**<  Mobile IP; FA authentication failure.  */
    E_QL_MCM_DATA_CE_MIP_HA_ERR_REGISTRATION_ID_MISMATCH    = 24,   /**<  Mobile IP; registration ID mismatch.  */
    E_QL_MCM_DATA_CE_MIP_HA_ERR_MALFORMED_REQUEST           = 25,   /**<  Mobile IP; malformed request.  */
    E_QL_MCM_DATA_CE_MIP_HA_ERR_UNKNOWN_HA_ADDR             = 26,   /**<  Mobile IP; unknown HA address.  */
    E_QL_MCM_DATA_CE_MIP_HA_ERR_REVERSE_TUNNEL_UNAVAILABLE  = 27,   /**<  Mobile IP; reverse tunnel is unavailable.  */
    E_QL_MCM_DATA_CE_MIP_HA_ERR_REVERSE_TUNNEL_IS_MANDATORY_AND_T_BIT_NOT_SET = 28,    /**<  Mobile IP; reverse tunnel is mandatory and the T-bit is not set.  */
    E_QL_MCM_DATA_CE_MIP_HA_ERR_ENCAPSULATION_UNAVAILABLE   = 29,   /**<  Mobile IP; encapsulation is unavailable.  */
    E_QL_MCM_DATA_CE_MIP_ERR_REASON_UNKNOWN                 = 30,   /**<  Mobile IP; reason is unknown.  */
    E_QL_MCM_DATA_CE_INTERNAL_ERROR                         = 31,   /**<  Internal error.  */
    E_QL_MCM_DATA_CE_CALL_ENDED                             = 32,   /**<  Internal error; call ended.  */
    E_QL_MCM_DATA_CE_INTERNAL_UNKNOWN_CAUSE_CODE            = 33,   /**<  Internal error; internal unknown cause code.  */
    E_QL_MCM_DATA_CE_UNKNOWN_CAUSE_CODE                     = 34,   /**<  Internal error; unknown cause code.  */
    E_QL_MCM_DATA_CE_CLOSE_IN_PROGRESS                      = 35,   /**<  Internal error; close in progress.  */
    E_QL_MCM_DATA_CE_NW_INITIATED_TERMINATION               = 36,   /**<  Internal error; NW-initiated termination.  */
    E_QL_MCM_DATA_CE_APP_PREEMPTED                          = 37,   /**<  Internal error; application was preempted.  */
    E_QL_MCM_DATA_CE_CDMA_LOCK                              = 38,   /**<  CDMA; CDMA lock.  */
    E_QL_MCM_DATA_CE_INTERCEPT                              = 39,   /**<  CDMA; intercept.  */
    E_QL_MCM_DATA_CE_REORDER                                = 40,   /**<  CDMA; reorder.  */
    E_QL_MCM_DATA_CE_REL_SO_REJ                             = 41,   /**<  CDMA; release SO was rejected.  */
    E_QL_MCM_DATA_CE_INCOM_CALL                             = 42,   /**<  CDMA; incoming call.  */
    E_QL_MCM_DATA_CE_ALERT_STOP                             = 43,   /**<  CDMA; alert stop.  */
    E_QL_MCM_DATA_CE_ACTIVATION                             = 44,   /**<  CDMA; activation.  */
    E_QL_MCM_DATA_CE_MAX_ACCESS_PROBE                       = 45,   /**<  CDMA; maximum access probe.  */
    E_QL_MCM_DATA_CE_CCS_NOT_SUPPORTED_BY_BS                = 46,   /**<  CDMA; CCS is not supported by the base station.  */
    E_QL_MCM_DATA_CE_NO_RESPONSE_FROM_BS                    = 47,   /**<  CDMA; no response from the base station.  */
    E_QL_MCM_DATA_CE_REJECTED_BY_BS                         = 48,   /**<  CDMA; rejected by the base station.  */
    E_QL_MCM_DATA_CE_INCOMPATIBLE                           = 49,   /**<  CDMA; incompatible.  */
    E_QL_MCM_DATA_CE_ALREADY_IN_TC                          = 50,   /**<  CDMA; already in TC.  */
    E_QL_MCM_DATA_CE_USER_CALL_ORIG_DURING_GPS              = 51,   /**<  CDMA; user call originated during GPS.  */
    E_QL_MCM_DATA_CE_USER_CALL_ORIG_DURING_SMS              = 52,   /**<  CDMA; user call originated during SMS.  */
    E_QL_MCM_DATA_CE_NO_CDMA_SRV                            = 53,   /**<  CDMA; no CDMA service.  */
    E_QL_MCM_DATA_CE_CONF_FAILED                            = 54,   /**<  CDMA; confirmation failed.  */
    E_QL_MCM_DATA_CE_INCOM_REJ                              = 55,   /**<  CDMA; incoming call was rejected.  */
    E_QL_MCM_DATA_CE_NO_GW_SRV                              = 56,   /**<  CDMA; no GW service.  */
    E_QL_MCM_DATA_CE_NO_GPRS_CONTEXT                        = 57,   /**<  CDMA; no GPRS context.  */
    E_QL_MCM_DATA_CE_ILLEGAL_MS                             = 58,   /**<  CDMA; illegal MS.  */
    E_QL_MCM_DATA_CE_ILLEGAL_ME                             = 59,   /**<  CDMA; illegal ME.  */
    E_QL_MCM_DATA_CE_GPRS_SERVICES_AND_NON_GPRS_SERVICES_NOT_ALLOWED = 60,    /**<  CDMA; GPRS services and non-GPRS services are not allowed.  */
    E_QL_MCM_DATA_CE_GPRS_SERVICES_NOT_ALLOWED              = 61,   /**<  CDMA; GPRS services are not allowed.  */
    E_QL_MCM_DATA_CE_MS_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK = 62,    /**<  CDMA; MS identity cannot be derived by the network.  */
    E_QL_MCM_DATA_CE_IMPLICITLY_DETACHED                    = 63,   /**<  CDMA; implicitly detached.  */
    E_QL_MCM_DATA_CE_PLMN_NOT_ALLOWED                       = 64,   /**<  CDMA; PLMN is not allowed.  */
    E_QL_MCM_DATA_CE_LA_NOT_ALLOWED                         = 65,   /**<  CDMA; LA is not allowed.  */
    E_QL_MCM_DATA_CE_GPRS_SERVICES_NOT_ALLOWED_IN_THIS_PLMN = 66,   /**<  CDMA; GPRS services are not allowed in this PLMN.  */
    E_QL_MCM_DATA_CE_PDP_DUPLICATE                          = 67,   /**<  CDMA; PDP duplicate.  */
    E_QL_MCM_DATA_CE_UE_RAT_CHANGE                          = 68,   /**<  CDMA; UE RAT change.  */
    E_QL_MCM_DATA_CE_CONGESTION                             = 69,   /**<  CDMA; congestion.  */
    E_QL_MCM_DATA_CE_NO_PDP_CONTEXT_ACTIVATED               = 70,   /**<  CDMA; no PDP context is activated.  */
    E_QL_MCM_DATA_CE_ACCESS_CLASS_DSAC_REJECTION            = 71,   /**<  CDMA; access class DSAC rejection.  */
    E_QL_MCM_DATA_CE_CD_GEN_OR_BUSY                         = 72,   /**<  CDMA; CD is generating or busy.  */
    E_QL_MCM_DATA_CE_CD_BILL_OR_AUTH                        = 73,   /**<  CDMA; CD bill or authorization.  */
    E_QL_MCM_DATA_CE_CHG_HDR                                = 74,   /**<  CDMA; change HDR.  */
    E_QL_MCM_DATA_CE_EXIT_HDR                               = 75,   /**<  CDMA; exit HDR.  */
    E_QL_MCM_DATA_CE_HDR_NO_SESSION                         = 76,   /**<  CDMA; HDR no session.  */
    E_QL_MCM_DATA_CE_HDR_ORIG_DURING_GPS_FIX                = 77,   /**<  CDMA; HDR originated during a GPS fix.  */
    E_QL_MCM_DATA_CE_HDR_CS_TIMEOUT                         = 78,   /**<  CDMA; HDR CS timeout.  */
    E_QL_MCM_DATA_CE_HDR_RELEASED_BY_CM                     = 79,   /**<  CDMA; HDR released by the CM.  */
    E_QL_MCM_DATA_CE_CLIENT_END                             = 80,   /**<  CDMA; client end.  */
    E_QL_MCM_DATA_CE_NO_SRV                                 = 81,   /**<  CDMA; no service.  */
    E_QL_MCM_DATA_CE_FADE                                   = 82,   /**<  CDMA; fade.  */
    E_QL_MCM_DATA_CE_REL_NORMAL                             = 83,   /**<  CDMA; release is normal.  */
    E_QL_MCM_DATA_CE_ACC_IN_PROG                            = 84,   /**<  CDMA; access is in progress.  */
    E_QL_MCM_DATA_CE_ACC_FAIL                               = 85,   /**<  CDMA; access failure.  */
    E_QL_MCM_DATA_CE_REDIR_OR_HANDOFF                       = 86,   /**<  CDMA; redirect or handoff.  */
    E_QL_MCM_DATA_CE_OPERATOR_DETERMINED_BARRING            = 87,   /**<  3GPP Spec defined; operator determined barring.  */
    E_QL_MCM_DATA_CE_LLC_SNDCP_FAILURE                      = 88,   /**<  3GPP Spec defined; LLC SNDCP failure.  */
    E_QL_MCM_DATA_CE_INSUFFICIENT_RESOURCES                 = 89,   /**<  3GPP Spec defined; insufficient resources.  */
    E_QL_MCM_DATA_CE_UNKNOWN_APN                            = 90,   /**<  3GPP Spec defined; unknown APN.  */
    E_QL_MCM_DATA_CE_UNKNOWN_PDP                            = 91,   /**<  3GPP Spec defined; unknown PDP.  */
    E_QL_MCM_DATA_CE_AUTH_FAILED                            = 92,   /**<  3GPP Spec defined; authorization failed.  */
    E_QL_MCM_DATA_CE_GGSN_REJECT                            = 93,   /**<  3GPP Spec defined; GGSN was rejected.  */
    E_QL_MCM_DATA_CE_ACTIVATION_REJECT                      = 94,   /**<  3GPP Spec defined; activation was rejected.  */
    E_QL_MCM_DATA_CE_OPTION_NOT_SUPPORTED                   = 95,   /**<  3GPP Spec defined; option is not supported.  */
    E_QL_MCM_DATA_CE_OPTION_UNSUBSCRIBED                    = 96,   /**<  3GPP Spec defined; option is unsubscribed.  */
    E_QL_MCM_DATA_CE_OPTION_TEMP_OOO                        = 97,   /**<  3GPP Spec defined; option is temporarily out of operation.  */
    E_QL_MCM_DATA_CE_NSAPI_ALREADY_USED                     = 98,   /**<  3GPP Spec defined; NSAPI was already used.  */
    E_QL_MCM_DATA_CE_REGULAR_DEACTIVATION                   = 99,   /**<  3GPP Spec defined; regular deactivation.  */
    E_QL_MCM_DATA_CE_QOS_NOT_ACCEPTED                       = 100,  /**<  3GPP Spec defined; QoS was not accepted.  */
    E_QL_MCM_DATA_CE_NETWORK_FAILURE                        = 101,  /**<  3GPP Spec defined; network failure.  */
    E_QL_MCM_DATA_CE_UMTS_REACTIVATION_REQ                  = 102,  /**<  3GPP Spec defined; UMTS reactivation is required.  */
    E_QL_MCM_DATA_CE_FEATURE_NOT_SUPPORTED                  = 103,  /**<  3GPP Spec defined; feature is not supported.  */
    E_QL_MCM_DATA_CE_TFT_SEMANTIC_ERROR                     = 104,  /**<  3GPP Spec defined; TFT semantic error.  */
    E_QL_MCM_DATA_CE_TFT_SYNTAX_ERROR                       = 105,  /**<  3GPP Spec defined; TFT syntax error.  */
    E_QL_MCM_DATA_CE_UNKNOWN_PDP_CONTEXT                    = 106,  /**<  3GPP Spec defined; unknown PDP context.  */
    E_QL_MCM_DATA_CE_FILTER_SEMANTIC_ERROR                  = 107,  /**<  3GPP Spec defined; filter semantic error.  */
    E_QL_MCM_DATA_CE_FILTER_SYNTAX_ERROR                    = 108,  /**<  3GPP Spec defined; filter syntax error.  */
    E_QL_MCM_DATA_CE_PDP_WITHOUT_ACTIVE_TFT                 = 109,  /**<  3GPP Spec defined; PDP is without an active TFT.  */
    E_QL_MCM_DATA_CE_IP_V4_ONLY_ALLOWED                     = 110,  /**<  3GPP Spec defined; only IPv4 is allowed.  */
    E_QL_MCM_DATA_CE_IP_V6_ONLY_ALLOWED                     = 111,  /**<  3GPP Spec defined; only IPv6 is allowed.  */
    E_QL_MCM_DATA_CE_SINGLE_ADDR_BEARER_ONLY                = 112,  /**<  3GPP Spec defined; single address bearer only.  */
    E_QL_MCM_DATA_CE_INVALID_TRANSACTION_ID                 = 113,  /**<  3GPP Spec defined; invalid transaction ID.  */
    E_QL_MCM_DATA_CE_MESSAGE_INCORRECT_SEMANTIC             = 114,  /**<  3GPP Spec defined; message has incorrect semantic.  */
    E_QL_MCM_DATA_CE_INVALID_MANDATORY_INFO                 = 115,  /**<  3GPP Spec defined; invalid mandatory information.  */
    E_QL_MCM_DATA_CE_MESSAGE_TYPE_UNSUPPORTED               = 116,  /**<  3GPP Spec defined; message type is unsupported.  */
    E_QL_MCM_DATA_CE_MSG_TYPE_NONCOMPATIBLE_STATE           = 117,  /**<  3GPP Spec defined; message type is in a noncompatible state.  */
    E_QL_MCM_DATA_CE_UNKNOWN_INFO_ELEMENT                   = 118,  /**<  3GPP Spec defined; unknown information element.  */
    E_QL_MCM_DATA_CE_CONDITIONAL_IE_ERROR                   = 119,  /**<  3GPP Spec defined; conditional IE error.  */
    E_QL_MCM_DATA_CE_MSG_AND_PROTOCOL_STATE_UNCOMPATIBLE    = 120,  /**<  3GPP Spec defined; message and protocol state are incompatible.  */
    E_QL_MCM_DATA_CE_PROTOCOL_ERROR                         = 121,  /**<  3GPP Spec defined; protocol error.  */
    E_QL_MCM_DATA_CE_APN_TYPE_CONFLICT                      = 122,  /**<  3GPP Spec defined; APN type conflict.  */
    E_QL_MCM_DATA_CE_PPP_TIMEOUT                            = 123,  /**<  PPP; timeout.  */
    E_QL_MCM_DATA_CE_PPP_AUTH_FAILURE                       = 124,  /**<  PPP; authorization failure.  */
    E_QL_MCM_DATA_CE_PPP_OPTION_MISMATCH                    = 125,  /**<  PPP; option mismatch.  */
    E_QL_MCM_DATA_CE_PPP_PAP_FAILURE                        = 126,  /**<  PPP; PAP failure.  */
    E_QL_MCM_DATA_CE_PPP_CHAP_FAILURE                       = 127,  /**<  PPP; CHAP failure.  */
    E_QL_MCM_DATA_CE_PPP_UNKNOWN                            = 128,  /**<  PPP; unknown.  */
    E_QL_MCM_DATA_CE_EHRPD_SUBS_LIMITED_TO_V4               = 129,  /**<  EHRPD; subscription is limited to v4.  */
    E_QL_MCM_DATA_CE_EHRPD_SUBS_LIMITED_TO_V6               = 130,  /**<  EHRPD; subscription is limited to v6.  */
    E_QL_MCM_DATA_CE_EHRPD_VSNCP_TIMEOUT                    = 131,  /**<  EHRPD VSNCP; timeout.  */
    E_QL_MCM_DATA_CE_EHRPD_VSNCP_FAILURE                    = 132,  /**<  EHRPD VSNCP; failure.  */
    E_QL_MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_GEN_ERROR           = 133,  /**<  EHRPD VSNCP 3GPP2I; generation error.  */
    E_QL_MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_UNAUTH_APN          = 134,  /**<  EHRPD VSNCP 3GPP2I; unauthorized APN.  */
    E_QL_MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_PDN_LIMIT_EXCEED    = 135,  /**<  EHRPD VSNCP 3GPP2I; PDM limit was exceeded.  */
    E_QL_MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_NO_PDN_GW           = 136,  /**<  EHRPD VSNCP 3GPP2I; no PDN GW.  */
    E_QL_MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_PDN_GW_UNREACH      = 137,  /**<  EHRPD VSNCP 3GPP2I; PDN GW is unreachable.  */
    E_QL_MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_PDN_GW_REJ          = 138,  /**<  EHRPD VSNCP 3GPP2I; PDN GW was rejected.  */
    E_QL_MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_INSUFF_PARAM        = 139,  /**<  EHRPD VSNCP 3GPP2I; insufficient parameters.  */
    E_QL_MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_RESOURCE_UNAVAIL    = 140,  /**<  EHRPD VSNCP 3GPP2I; resource is unavailable.  */
    E_QL_MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_ADMIN_PROHIBIT      = 141,  /**<  EHRPD VSNCP 3GPP2I; administratively prohibited.  */
    E_QL_MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_PDN_ID_IN_USE       = 142,  /**<  EHRPD VSNCP 3GPP2I; PDN ID is in use.  */
    E_QL_MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_SUBSCR_LIMITATION   = 143,  /**<  EHRPD VSNCP 3GPP2I; subscriber limitation.  */
    E_QL_MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_PDN_EXISTS_FOR_THIS_APN = 144,    /**<  EHRPD VSNCP 3GPP2I; PDN exists for this APN.  */
    E_QL_MCM_DATA_CE_PREFIX_UNAVAILABLE                     = 145,  /**<  IPv6; prefix is unavailable.  */
    E_QL_MCM_DATA_CE_IPV6_ERR_HRPD_IPV6_DISABLED            = 146   /**<  IPv6; HRPD IPv6 is disabled.  */
}E_QL_MCM_DATA_CALL_END_REASON_CODE_T;

typedef struct 
{
    E_QL_MCM_DATA_TYPE_T                    call_end_reason_type;   /**<  Call end reason type. */
    E_QL_MCM_DATA_CALL_END_REASON_CODE_T    call_end_reason_code;   /**<  Verbose data call end reason. */
}QL_MCM_DATA_CALL_END_REASON_INFO_T;  /* Type */

typedef enum 
{
    E_QL_MCM_DATA_DORMANCY_STATE_PHYSLINK_ACTIVE    = 0,        /**< Call is not dormant.  */
    E_QL_MCM_DATA_DORMANCY_STATE_PHYSLINK_DORMANT   = 1,        /**< Call is dormant.  @vertspace{-4} */
}E_QL_DATA_DORMANCY_STATE_T;

typedef enum 
{
    E_QL_DATA_NET_EVENT_UNKNOWN = 0x5000,       /**<  pv_data = &QL_MCM_DATA_CALL_INDICATION_INFO_T  */
    E_QL_DATA_NET_UP_EVENT,                     /**<  pv_data = &QL_MCM_DATA_CALL_INDICATION_INFO_T  */
    E_QL_DATA_NET_DOWN_EVENT,                   /**<  pv_data = &QL_MCM_DATA_CALL_INDICATION_INFO_T  */
    E_QL_DATA_NET_NEW_ADDR_EVENT,               /**<  pv_data = &QL_MCM_DATA_CALL_INDICATION_INFO_T  */
    E_QL_DATA_NET_DEL_ADDR_EVENT,               /**<  pv_data = &QL_MCM_DATA_CALL_INDICATION_INFO_T  */
    E_QL_DATA_NET_SERVICE_STATUS_EVENT,         /**<  pv_data = &QL_MCM_DATA_CALL_INDICATION_INFO_T  */
    E_QL_DATA_NET_BEARER_TECH_STATUS_EVENT,     /**<  pv_data = &QL_MCM_DATA_CALL_INDICATION_INFO_T  */
    E_QL_DATA_NET_DORMANCY_STATUS_EVENT,        /**<  pv_data = &QL_MCM_DATA_CALL_INDICATION_INFO_T  */
}E_QL_DATA_NET_EVENT_MSG_ID_T;

typedef struct 
{
    E_QL_DATA_NET_EVENT_MSG_ID_T        event_id;           /*  MCM Data Event ID */
    uint8_t                             call_id_valid;      /**< Must be set to TRUE if call_id is being passed. */
    int32_t                             call_id;            /**< Call ID that gets generated for a succesful call. */
    uint8_t                             call_status_valid;  /**< Must be set to TRUE if call_status is being passed. */
    E_QL_MCM_DATA_CALL_STATE_TYPE_T     call_status;        /**< Data call status. */
    uint8_t                             call_tech_valid;    /**< Must be set to TRUE if call_tech is being passed. */
    E_QL_MCM_DATA_BEARER_TECH_TYPE_T    call_tech;          /**< Data bearer technology corresponding to the call ID. */
    uint8_t                             reg_status_valid;   /**< Must be set to TRUE if reg_status is being passed. */
    QL_MCM_DATA_REG_STATUS_INFO_T       reg_status;         /**< Data modem registration status. */
    uint8_t                             dorm_status_valid;  /**< Must be set to TRUE if dorm_status is being passed. */
    E_QL_DATA_DORMANCY_STATE_T          dorm_status;        /**< Data dormancy status. */
    uint8_t                             addr_count_valid;   /**< Must be set to TRUE if addr_count is being passed. */
    int8_t                              addr_count;         /**< Data device address count. */
    uint8_t                             addr_info_valid;    /**< Must be set to TRUE if addr_info is being passed. */
    uint32_t                            addr_info_len;      /**< Must be set to the number of elements in addr_info. */
    QL_MCM_DATA_CALL_ADDR_INFO_T        addr_info[QL_MCM_DATA_MAX_ADDR_COUNT];  /**< Data device address. */
    uint8_t                             vce_reason_valid;   /**< Must be set to TRUE if vce_reason is being passed. */
    QL_MCM_DATA_CALL_END_REASON_INFO_T  vce_reason;         /**< Data call end reason in verbose. */
}QL_MCM_DATA_CALL_INDICATION_INFO_T;


/** Sends a request to start a data call for the connection associated with the specified call ID. */
typedef struct 
{
    E_QL_MCM_DATA_CALL_STATE_TYPE_T     call_status;        /**< MCM data call status. */
    int32_t                             call_id;            /**< Call ID that gets generated for a successful call. -1 means invalid*/
    QL_MCM_DATA_CALL_END_REASON_INFO_T  vcall_end_reason;   /**< Call end reason in verbose. */
}QL_MCM_DATA_CALL_RESULT_INFO_T;

/* Bit mask to enable specified indication event */
#define     DATA_NET_UP_EVENT_ON                    (1 << 0)    /**< Register for a network up event. */
#define     DATA_NET_DOWN_EVENT_ON                  (1 << 1)    /**< Register for a network down event. */
#define     DATA_NET_NEW_ADDR_EVENT_ON              (1 << 2)    /**< Register for a new address event. */
#define     DATA_NET_DEL_ADDR_EVENT_ON              (1 << 3)    /**< Register for a delete address event. */
#define     DATA_NET_SERVICE_STATUS_EVENT_ON        (1 << 4)    /**< Register for a service status event. */
#define     DATA_NET_BEARER_TECH_STATUS_EVENT_ON    (1 << 5)    /**< Register for a bearer technology status event. */
#define     DATA_NET_DORMANCY_STATUS_EVENT_ON       (1 << 6)    /**< Register for a dormancy status event. */

typedef void (*QL_DATA_RxIndMsgHandlerFunc_t)   
(    
    data_client_handle_type         h_data,
    E_QL_DATA_NET_EVENT_MSG_ID_T    e_msg_id,
    void                            *pv_data,
    void                            *contextPtr    
);









E_QL_ERROR_CODE_T QL_MCM_DATA_Client_Init
(
    data_client_handle_type  *ph_data
);

E_QL_ERROR_CODE_T QL_MCM_DATA_Client_Deinit
(
    data_client_handle_type h_data
);

int QL_MCM_DATA_AddRxIndMsgHandler
( 
    QL_DATA_RxIndMsgHandlerFunc_t   handlerPtr, 
    void*                           contextPtr
);

E_QL_ERROR_CODE_T QL_MCM_DATA_SetIPFamily
(
    QL_MCM_DATA_CALL_CONFIG_INFO_T *pt_cfg,
    E_QL_MCM_DATA_IP_FAMILY_TYPE_T  e_ip_family,
    int                             valid
);

E_QL_ERROR_CODE_T QL_MCM_DATA_SetTechPref
(
    QL_MCM_DATA_CALL_CONFIG_INFO_T *pt_cfg,
    E_QL_MCM_DATA_TECH_PREF_TYPE_T  e_tech_pref,
    int                             valid
);

E_QL_ERROR_CODE_T QL_MCM_DATA_SetProfile
(
    QL_MCM_DATA_CALL_CONFIG_INFO_T *pt_cfg,
    uint32_t                        profile,
    int                             valid
);

E_QL_ERROR_CODE_T QL_MCM_DATA_SetAPN
(
    QL_MCM_DATA_CALL_CONFIG_INFO_T *pt_cfg,
    char                           *apn,
    int                            valid
);

E_QL_ERROR_CODE_T QL_MCM_DATA_SetUsername
(
    QL_MCM_DATA_CALL_CONFIG_INFO_T *pt_cfg,
    char                           *username,
    int                            valid
);

E_QL_ERROR_CODE_T QL_MCM_DATA_SetPassword
(
    QL_MCM_DATA_CALL_CONFIG_INFO_T *pt_cfg,
    char                           *password,
    int                            valid
);

E_QL_ERROR_CODE_T QL_MCM_DATA_StartDataCall
(
    data_client_handle_type         h_data,
    QL_MCM_DATA_CALL_CONFIG_INFO_T  *pt_cfg,
    QL_MCM_DATA_CALL_RESULT_INFO_T  *pt_info //You need store this call_id to some array for later use.
);

E_QL_ERROR_CODE_T QL_MCM_DATA_StopDataCall
(
    data_client_handle_type         h_data,
    uint32_t                        call_id
);

E_QL_ERROR_CODE_T QL_MCM_DATA_GetDataCallStatus
(
    data_client_handle_type         h_data,
    uint32_t                        call_id,
    E_QL_MCM_DATA_CALL_STATE_TYPE_T *pe_status
);

E_QL_ERROR_CODE_T QL_MCM_DATA_GetDataCallBearerTech
(
    data_client_handle_type             h_data,
    uint32_t                            call_id,
    E_QL_MCM_DATA_BEARER_TECH_TYPE_T    *pe_tech
);

E_QL_ERROR_CODE_T QL_MCM_DATA_GetDeviceName
(
    data_client_handle_type             h_data,
    uint32_t                            call_id,
    char                                *device_name,
    uint32                              buflen //About 13 bytes
);

E_QL_ERROR_CODE_T QL_MCM_DATA_GetDeviceAddress
(
    data_client_handle_type             h_data,
    uint32_t                            call_id,
    QL_MCM_DATA_CALL_ADDR_INFO_LIST_T   *pt_addr_list
);

E_QL_ERROR_CODE_T QL_MCM_DATA_GetDeviceAddrCount
(
    data_client_handle_type             h_data,
    uint32_t                            call_id,
    uint32_t                            *p_count
);

E_QL_ERROR_CODE_T QL_MCM_DATA_GetPktStatistic
(
    data_client_handle_type             h_data,
    uint32_t                            call_id,
    QL_MCM_DATA_PKT_STATISTIC_INFO_T    *pt_info
);

E_QL_ERROR_CODE_T QL_MCM_DATA_ResetPktStatistic
(
    data_client_handle_type             h_data,
    uint32_t                            call_id
);

E_QL_ERROR_CODE_T QL_MCM_DATA_GetRegStatus
(
    data_client_handle_type             h_data,
    QL_MCM_DATA_REG_STATUS_INFO_T       *pt_info
);

E_QL_ERROR_CODE_T QL_MCM_DATA_GetDataChannelRate
(
    data_client_handle_type             h_data,
    uint32_t                            call_id,
    QL_MCM_DATA_CHANNEL_RATE_INFO_T     *pt_info
);

E_QL_ERROR_CODE_T QL_MCM_DATA_AddRoute
(
    data_client_handle_type             h_data,
    QL_MCM_DATA_ROUTE_INFO_T            *pt_info
);

E_QL_ERROR_CODE_T QL_MCM_DATA_RegisterEvent
(
    data_client_handle_type             h_data,
    uint32_t                            bitmask     //Defined by DATA_NET_xxxx_EVENT_ON
);



#endif // __QL_MCM_DATA_H__

