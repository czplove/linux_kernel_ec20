#ifndef MCM_DATA_SERVICE_01_H
#define MCM_DATA_SERVICE_01_H
/**
  @file mcm_data_v01.h

  @brief This is the public header file which defines the mcm_data service Data structures.

*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
   Copyright (c) 2013 Qualcomm Technologies, Inc.
   All rights reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
#include <stdint.h>
#include "mcm_common_v01.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup mcm_data_consts
    @{
  */

/**  Event ID indicating that a data call is connected. */
#define MCM_DATA_NET_UP_EV_V01 0x00005001

/**  Event ID indicating that a data call is disconnected. */
#define MCM_DATA_NET_DOWN_EV_V01 0x00005002

/**  Event ID indicating that a new IP address is configured for the interface. */
#define MCM_DATA_NET_NEW_ADDR_EV_V01 0x00005003

/**  Event ID indicating that one of the IP addresses is lost. */
#define MCM_DATA_NET_DEL_ADDR_EV_V01 0x00005004

/**  Event ID indicating the current service status of the modem. */
#define MCM_DATA_REG_SRVC_STATUS_EV_V01 0x00005005

/**  Event ID indicating the current bearer technology of the call. */
#define MCM_DATA_BEARER_TECH_STATUS_EV_V01 0x00005006

/**  Event ID indicating the dormancy state of the call. */
#define MCM_DATA_DORMANCY_STATUS_EV_V01 0x00005007

/**  Maximum length of the APN. */
#define MCM_DATA_MAX_APN_LEN_V01 150

/**  Maximum length of the profile user name. */
#define MCM_DATA_MAX_USERNAME_LEN_V01 127

/**  Maximum length of the password. */
#define MCM_DATA_MAX_PASSWORD_LEN_V01 127

/**  Maximum address length. */
#define MCM_DATA_MAX_ADDR_LEN_V01 128

/**  Maximum length of the device name. */
#define MCM_DATA_MAX_DEVICE_NAME_LEN_V01 13

/** Maximum number of IP addresses. */
#define MCM_DATA_MAX_ADDR_COUNT_V01 10
/**
    @}
  */

/** @addtogroup mcm_data_enums
    @{
  */
typedef enum {
  MCM_DATA_CALL_STATUS_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_DATA_CALL_STATE_INVALID_V01 = 0x01, /**<  Call state is invalid.  */
  MCM_DATA_CALL_STATE_CONNECTING_V01 = 0x02, /**<  Call is connecting.  */
  MCM_DATA_CALL_STATE_CONNECTED_V01 = 0x03, /**<  Call is connected.  */
  MCM_DATA_CALL_STATE_DISCONNECTING_V01 = 0x04, /**<  Call is disconnecting.  */
  MCM_DATA_CALL_STATE_DISCONNECTED_V01 = 0x05, /**<  Call is disconnected.  */
  MCM_DATA_CALL_STATUS_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_data_call_status_t_v01;
/**
    @}
  */

/** @addtogroup mcm_data_enums
    @{
  */
typedef enum {
  MCM_DATA_SRV_STATUS_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_DATA_MODEM_STATE_OOS_V01 = 0x01, /**< Modem is out of service.  */
  MCM_DATA_MODEM_STATE_IN_SERVICE_V01 = 0x02, /**< Modem is in service.  @vertspace{-4} */
  MCM_DATA_SRV_STATUS_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_data_srv_status_t_v01;
/**
    @}
  */

/** @addtogroup mcm_data_enums
    @{
  */
typedef enum {
  MCM_DATA_BEARER_TECH_INFO_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_DATA_BEARER_TECH_TYPE_UNKNOWN_V01 = 0x00, /**< Unknown technology.  */
  MCM_DATA_BEARER_TECH_TYPE_CDMA_1X_V01 = 0x01, /**<  1X technology.         */
  MCM_DATA_BEARER_TECH_TYPE_EVDO_REV0_V01 = 0x02, /**< CDMA Rev 0.  */
  MCM_DATA_BEARER_TECH_TYPE_EVDO_REVA_V01 = 0x03, /**< CDMA Rev A.  */
  MCM_DATA_BEARER_TECH_TYPE_EVDO_REVB_V01 = 0x04, /**< CDMA Rev B.  */
  MCM_DATA_BEARER_TECH_TYPE_EHRPD_V01 = 0x05, /**< EHRPD.  */
  MCM_DATA_BEARER_TECH_TYPE_FMC_V01 = 0x06, /**< Fixed mobile convergence.  */
  MCM_DATA_BEARER_TECH_TYPE_HRPD_V01 = 0x07, /**< HRPD.  */
  MCM_DATA_BEARER_TECH_TYPE_3GPP2_WLAN_V01 = 0x08, /**< IWLAN.  */
  MCM_DATA_BEARER_TECH_TYPE_WCDMA_V01 = 0x09, /**< WCDMA.  */
  MCM_DATA_BEARER_TECH_TYPE_GPRS_V01 = 0x0A, /**< GPRS.  */
  MCM_DATA_BEARER_TECH_TYPE_HSDPA_V01 = 0x0B, /**< HSDPA.  */
  MCM_DATA_BEARER_TECH_TYPE_HSUPA_V01 = 0x0C, /**< HSUPA.  */
  MCM_DATA_BEARER_TECH_TYPE_EDGE_V01 = 0x0D, /**< EDGE.  */
  MCM_DATA_BEARER_TECH_TYPE_LTE_V01 = 0x0E, /**< LTE.  */
  MCM_DATA_BEARER_TECH_TYPE_HSDPA_PLUS_V01 = 0x0F, /**< HSDPA+.  */
  MCM_DATA_BEARER_TECH_TYPE_DC_HSDPA_PLUS_V01 = 0x10, /**< DC HSDPA+.  */
  MCM_DATA_BEARER_TECH_TYPE_HSPA_V01 = 0x11, /**< HSPA.  */
  MCM_DATA_BEARER_TECH_TYPE_64_QAM_V01 = 0x12, /**< 64 QAM.  */
  MCM_DATA_BEARER_TECH_TYPE_TDSCDMA_V01 = 0x13, /**< TD-SCDMA.  */
  MCM_DATA_BEARER_TECH_TYPE_GSM_V01 = 0x14, /**< GSM.  */
  MCM_DATA_BEARER_TECH_TYPE_3GPP_WLAN_V01 = 0x15, /**< IWLAN.  @vertspace{-4} */
  MCM_DATA_BEARER_TECH_INFO_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_data_bearer_tech_info_t_v01;
/**
    @}
  */

/** @addtogroup mcm_data_enums
    @{
  */
typedef enum {
  MCM_DATA_DORMANCY_STATE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_DATA_DORMANCY_STATE_PHYSLINK_ACTIVE_V01 = 0, /**< Call is not dormant.  */
  MCM_DATA_DORMANCY_STATE_PHYSLINK_DORMANT_V01 = 1, /**< Call is dormant.  @vertspace{-4} */
  MCM_DATA_DORMANCY_STATE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_data_dormancy_state_t_v01;
/**
    @}
  */

/** @addtogroup mcm_data_enums
    @{
  */
typedef enum {
  MCM_DATA_CALL_END_REASON_TYPE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_DATA_TYPE_UNSPECIFIED_V01 = 0x00, /**< Unspecified.  */
  MCM_DATA_TYPE_MOBILE_IP_V01 = 0x01, /**< Mobile IP.  */
  MCM_DATA_TYPE_INTERNAL_V01 = 0x02, /**< Internal.  */
  MCM_DATA_TYPE_CALL_MANAGER_DEFINED_V01 = 0x03, /**< Call manager-defined.  */
  MCM_DATA_TYPE_3GPP_SPEC_DEFINED_V01 = 0x06, /**< 3GPP specification-defined.  */
  MCM_DATA_TYPE_PPP_V01 = 0x07, /**< Point-to-Point protocol.  */
  MCM_DATA_TYPE_EHRPD_V01 = 0x08, /**< EHRPD.  */
  MCM_DATA_TYPE_IPV6_V01 = 0x09, /**< IPV6.  @newpage */
  MCM_DATA_CALL_END_REASON_TYPE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_data_call_end_reason_type_t_v01;
/**
    @}
  */

/** @addtogroup mcm_data_enums
    @{
  */
typedef enum {
  MCM_DATA_CALL_END_REASON_CODE_T_MIN_ENUM_VAL_V01 = -2147483647,
  /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_DATA_CE_INVALID_V01 = -1,
  /**<  Invalid call end reason.  */
  MCM_DATA_CE_MIP_FA_ERR_REASON_UNSPECIFIED_V01 = 0,
  /**<  Mobile IP; unspecified error.  */
  MCM_DATA_CE_MIP_FA_ERR_ADMINISTRATIVELY_PROHIBITED_V01 = 1,
  /**<  Mobile IP; administratively prohibited.  */
  MCM_DATA_CE_MIP_FA_ERR_INSUFFICIENT_RESOURCES_V01 = 2,
  /**<  Mobile IP; insufficient resources.  */
  MCM_DATA_CE_MIP_FA_ERR_MOBILE_NODE_AUTHENTICATION_FAILURE_V01 = 3,
  /**<  Mobile IP; mobile node authentication failure.  */
  MCM_DATA_CE_MIP_FA_ERR_HA_AUTHENTICATION_FAILURE_V01 = 4,
  /**<  Mobile IP; HA authentication failure.  */
  MCM_DATA_CE_MIP_FA_ERR_REQUESTED_LIFETIME_TOO_LONG_V01 = 5,
  /**<  Mobile IP; requested lifetime is too long.  */
  MCM_DATA_CE_MIP_FA_ERR_MALFORMED_REQUEST_V01 = 6,
  /**<  Mobile IP; malformed request.  */
  MCM_DATA_CE_MIP_FA_ERR_MALFORMED_REPLY_V01 = 7,
  /**<  Mobile IP; malformed reply.  */
  MCM_DATA_CE_MIP_FA_ERR_ENCAPSULATION_UNAVAILABLE_V01 = 8,
  /**<  Mobile IP; encapsulation is unavailable.  */
  MCM_DATA_CE_MIP_FA_ERR_VJHC_UNAVAILABLE_V01 = 9,
  /**<  Mobile IP; VJHC is unavailable.  */
  MCM_DATA_CE_MIP_FA_ERR_REVERSE_TUNNEL_UNAVAILABLE_V01 = 10,
  /**<  Mobile IP; reverse tunnel is unavailable.  */
  MCM_DATA_CE_MIP_FA_ERR_REVERSE_TUNNEL_IS_MANDATORY_AND_T_BIT_NOT_SET_V01 = 11,
  /**<  Mobile IP; reverse tunnel is mandatory and the T-bit is not set.  */
  MCM_DATA_CE_MIP_FA_ERR_DELIVERY_STYLE_NOT_SUPPORTED_V01 = 12,
  /**<  Mobile IP; delivery style is not supported.  */
  MCM_DATA_CE_MIP_FA_ERR_MISSING_NAI_V01 = 13,
  /**<  Mobile IP; missing NAI.  */
  MCM_DATA_CE_MIP_FA_ERR_MISSING_HA_V01 = 14,
  /**<  Mobile IP; missing HA.  */
  MCM_DATA_CE_MIP_FA_ERR_MISSING_HOME_ADDR_V01 = 15,
  /**<  Mobile IP; missing home address.  */
  MCM_DATA_CE_MIP_FA_ERR_UNKNOWN_CHALLENGE_V01 = 16,
  /**<  Mobile IP; unknown challenge.  */
  MCM_DATA_CE_MIP_FA_ERR_MISSING_CHALLENGE_V01 = 17,
  /**<  Mobile IP; missing challenge.  */
  MCM_DATA_CE_MIP_FA_ERR_STALE_CHALLENGE_V01 = 18,
  /**<  Mobile IP; stale challenge.  */
  MCM_DATA_CE_MIP_HA_ERR_REASON_UNSPECIFIED_V01 = 19,
  /**<  Mobile IP; reason is unspecified.  */
  MCM_DATA_CE_MIP_HA_ERR_ADMINISTRATIVELY_PROHIBITED_V01 = 20,
  /**<  Mobile IP; administratively prohibited.  */
  MCM_DATA_CE_MIP_HA_ERR_INSUFFICIENT_RESOURCES_V01 = 21,
  /**<  Mobile IP; insufficient resources.  */
  MCM_DATA_CE_MIP_HA_ERR_MOBILE_NODE_AUTHENTICATION_FAILURE_V01 = 22,
  /**<  Mobile IP; mobile node authentication failure.  */
  MCM_DATA_CE_MIP_HA_ERR_FA_AUTHENTICATION_FAILURE_V01 = 23,
  /**<  Mobile IP; FA authentication failure.  */
  MCM_DATA_CE_MIP_HA_ERR_REGISTRATION_ID_MISMATCH_V01 = 24,
  /**<  Mobile IP; registration ID mismatch.  */
  MCM_DATA_CE_MIP_HA_ERR_MALFORMED_REQUEST_V01 = 25,
  /**<  Mobile IP; malformed request.  */
  MCM_DATA_CE_MIP_HA_ERR_UNKNOWN_HA_ADDR_V01 = 26,
  /**<  Mobile IP; unknown HA address.  */
  MCM_DATA_CE_MIP_HA_ERR_REVERSE_TUNNEL_UNAVAILABLE_V01 = 27,
  /**<  Mobile IP; reverse tunnel is unavailable.  */
  MCM_DATA_CE_MIP_HA_ERR_REVERSE_TUNNEL_IS_MANDATORY_AND_T_BIT_NOT_SET_V01 = 28,
  /**<  Mobile IP; reverse tunnel is mandatory and the T-bit is not set.  */
  MCM_DATA_CE_MIP_HA_ERR_ENCAPSULATION_UNAVAILABLE_V01 = 29,
  /**<  Mobile IP; encapsulation is unavailable.  */
  MCM_DATA_CE_MIP_ERR_REASON_UNKNOWN_V01 = 30,
  /**<  Mobile IP; reason is unknown.  */
  MCM_DATA_CE_INTERNAL_ERROR_V01 = 31,
  /**<  Internal error.  */
  MCM_DATA_CE_CALL_ENDED_V01 = 32,
  /**<  Internal error; call ended.  */
  MCM_DATA_CE_INTERNAL_UNKNOWN_CAUSE_CODE_V01 = 33,
  /**<  Internal error; internal unknown cause code.  */
  MCM_DATA_CE_UNKNOWN_CAUSE_CODE_V01 = 34,
  /**<  Internal error; unknown cause code.  */
  MCM_DATA_CE_CLOSE_IN_PROGRESS_V01 = 35,
  /**<  Internal error; close in progress.  */
  MCM_DATA_CE_NW_INITIATED_TERMINATION_V01 = 36,
  /**<  Internal error; NW-initiated termination.  */
  MCM_DATA_CE_APP_PREEMPTED_V01 = 37,
  /**<  Internal error; application was preempted.  */
  MCM_DATA_CE_CDMA_LOCK_V01 = 38,
  /**<  CDMA; CDMA lock.  */
  MCM_DATA_CE_INTERCEPT_V01 = 39,
  /**<  CDMA; intercept.  */
  MCM_DATA_CE_REORDER_V01 = 40,
  /**<  CDMA; reorder.  */
  MCM_DATA_CE_REL_SO_REJ_V01 = 41,
  /**<  CDMA; release SO was rejected.  */
  MCM_DATA_CE_INCOM_CALL_V01 = 42,
  /**<  CDMA; incoming call.  */
  MCM_DATA_CE_ALERT_STOP_V01 = 43,
  /**<  CDMA; alert stop.  */
  MCM_DATA_CE_ACTIVATION_V01 = 44,
  /**<  CDMA; activation.  */
  MCM_DATA_CE_MAX_ACCESS_PROBE_V01 = 45,
  /**<  CDMA; maximum access probe.  */
  MCM_DATA_CE_CCS_NOT_SUPPORTED_BY_BS_V01 = 46,
  /**<  CDMA; CCS is not supported by the base station.  */
  MCM_DATA_CE_NO_RESPONSE_FROM_BS_V01 = 47,
  /**<  CDMA; no response from the base station.  */
  MCM_DATA_CE_REJECTED_BY_BS_V01 = 48,
  /**<  CDMA; rejected by the base station.  */
  MCM_DATA_CE_INCOMPATIBLE_V01 = 49,
  /**<  CDMA; incompatible.  */
  MCM_DATA_CE_ALREADY_IN_TC_V01 = 50,
  /**<  CDMA; already in TC.  */
  MCM_DATA_CE_USER_CALL_ORIG_DURING_GPS_V01 = 51,
  /**<  CDMA; user call originated during GPS.  */
  MCM_DATA_CE_USER_CALL_ORIG_DURING_SMS_V01 = 52,
  /**<  CDMA; user call originated during SMS.  */
  MCM_DATA_CE_NO_CDMA_SRV_V01 = 53,
  /**<  CDMA; no CDMA service.  */
  MCM_DATA_CE_CONF_FAILED_V01 = 54,
  /**<  CDMA; confirmation failed.  */
  MCM_DATA_CE_INCOM_REJ_V01 = 55,
  /**<  CDMA; incoming call was rejected.  */
  MCM_DATA_CE_NO_GW_SRV_V01 = 56,
  /**<  CDMA; no GW service.  */
  MCM_DATA_CE_NO_GPRS_CONTEXT_V01 = 57,
  /**<  CDMA; no GPRS context.  */
  MCM_DATA_CE_ILLEGAL_MS_V01 = 58,
  /**<  CDMA; illegal MS.  */
  MCM_DATA_CE_ILLEGAL_ME_V01 = 59,
  /**<  CDMA; illegal ME.  */
  MCM_DATA_CE_GPRS_SERVICES_AND_NON_GPRS_SERVICES_NOT_ALLOWED_V01 = 60,
  /**<  CDMA; GPRS services and non-GPRS services are not allowed.  */
  MCM_DATA_CE_GPRS_SERVICES_NOT_ALLOWED_V01 = 61,
  /**<  CDMA; GPRS services are not allowed.  */
  MCM_DATA_CE_MS_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK_V01 = 62,
  /**<  CDMA; MS identity cannot be derived by the network.  */
  MCM_DATA_CE_IMPLICITLY_DETACHED_V01 = 63,
  /**<  CDMA; implicitly detached.  */
  MCM_DATA_CE_PLMN_NOT_ALLOWED_V01 = 64,
  /**<  CDMA; PLMN is not allowed.  */
  MCM_DATA_CE_LA_NOT_ALLOWED_V01 = 65,
  /**<  CDMA; LA is not allowed.  */
  MCM_DATA_CE_GPRS_SERVICES_NOT_ALLOWED_IN_THIS_PLMN_V01 = 66,
  /**<  CDMA; GPRS services are not allowed in this PLMN.  */
  MCM_DATA_CE_PDP_DUPLICATE_V01 = 67,
  /**<  CDMA; PDP duplicate.  */
  MCM_DATA_CE_UE_RAT_CHANGE_V01 = 68,
  /**<  CDMA; UE RAT change.  */
  MCM_DATA_CE_CONGESTION_V01 = 69,
  /**<  CDMA; congestion.  */
  MCM_DATA_CE_NO_PDP_CONTEXT_ACTIVATED_V01 = 70,
  /**<  CDMA; no PDP context is activated.  */
  MCM_DATA_CE_ACCESS_CLASS_DSAC_REJECTION_V01 = 71,
  /**<  CDMA; access class DSAC rejection.  */
  MCM_DATA_CE_CD_GEN_OR_BUSY_V01 = 72,
  /**<  CDMA; CD is generating or busy.  */
  MCM_DATA_CE_CD_BILL_OR_AUTH_V01 = 73,
  /**<  CDMA; CD bill or authorization.  */
  MCM_DATA_CE_CHG_HDR_V01 = 74,
  /**<  CDMA; change HDR.  */
  MCM_DATA_CE_EXIT_HDR_V01 = 75,
  /**<  CDMA; exit HDR.  */
  MCM_DATA_CE_HDR_NO_SESSION_V01 = 76,
  /**<  CDMA; HDR no session.  */
  MCM_DATA_CE_HDR_ORIG_DURING_GPS_FIX_V01 = 77,
  /**<  CDMA; HDR originated during a GPS fix.  */
  MCM_DATA_CE_HDR_CS_TIMEOUT_V01 = 78,
  /**<  CDMA; HDR CS timeout.  */
  MCM_DATA_CE_HDR_RELEASED_BY_CM_V01 = 79,
  /**<  CDMA; HDR released by the CM.  */
  MCM_DATA_CE_CLIENT_END_V01 = 80,
  /**<  CDMA; client end.  */
  MCM_DATA_CE_NO_SRV_V01 = 81,
  /**<  CDMA; no service.  */
  MCM_DATA_CE_FADE_V01 = 82,
  /**<  CDMA; fade.  */
  MCM_DATA_CE_REL_NORMAL_V01 = 83,
  /**<  CDMA; release is normal.  */
  MCM_DATA_CE_ACC_IN_PROG_V01 = 84,
  /**<  CDMA; access is in progress.  */
  MCM_DATA_CE_ACC_FAIL_V01 = 85,
  /**<  CDMA; access failure.  */
  MCM_DATA_CE_REDIR_OR_HANDOFF_V01 = 86,
  /**<  CDMA; redirect or handoff.  */
  MCM_DATA_CE_OPERATOR_DETERMINED_BARRING_V01 = 87,
  /**<  3GPP Spec defined; operator determined barring.  */
  MCM_DATA_CE_LLC_SNDCP_FAILURE_V01 = 88,
  /**<  3GPP Spec defined; LLC SNDCP failure.  */
  MCM_DATA_CE_INSUFFICIENT_RESOURCES_V01 = 89,
  /**<  3GPP Spec defined; insufficient resources.  */
  MCM_DATA_CE_UNKNOWN_APN_V01 = 90,
  /**<  3GPP Spec defined; unknown APN.  */
  MCM_DATA_CE_UNKNOWN_PDP_V01 = 91,
  /**<  3GPP Spec defined; unknown PDP.  */
  MCM_DATA_CE_AUTH_FAILED_V01 = 92,
  /**<  3GPP Spec defined; authorization failed.  */
  MCM_DATA_CE_GGSN_REJECT_V01 = 93,
  /**<  3GPP Spec defined; GGSN was rejected.  */
  MCM_DATA_CE_ACTIVATION_REJECT_V01 = 94,
  /**<  3GPP Spec defined; activation was rejected.  */
  MCM_DATA_CE_OPTION_NOT_SUPPORTED_V01 = 95,
  /**<  3GPP Spec defined; option is not supported.  */
  MCM_DATA_CE_OPTION_UNSUBSCRIBED_V01 = 96,
  /**<  3GPP Spec defined; option is unsubscribed.  */
  MCM_DATA_CE_OPTION_TEMP_OOO_V01 = 97,
  /**<  3GPP Spec defined; option is temporarily out of operation.  */
  MCM_DATA_CE_NSAPI_ALREADY_USED_V01 = 98,
  /**<  3GPP Spec defined; NSAPI was already used.  */
  MCM_DATA_CE_REGULAR_DEACTIVATION_V01 = 99,
  /**<  3GPP Spec defined; regular deactivation.  */
  MCM_DATA_CE_QOS_NOT_ACCEPTED_V01 = 100,
  /**<  3GPP Spec defined; QoS was not accepted.  */
  MCM_DATA_CE_NETWORK_FAILURE_V01 = 101,
  /**<  3GPP Spec defined; network failure.  */
  MCM_DATA_CE_UMTS_REACTIVATION_REQ_V01 = 102,
  /**<  3GPP Spec defined; UMTS reactivation is required.  */
  MCM_DATA_CE_FEATURE_NOT_SUPPORTED_V01 = 103,
  /**<  3GPP Spec defined; feature is not supported.  */
  MCM_DATA_CE_TFT_SEMANTIC_ERROR_V01 = 104,
  /**<  3GPP Spec defined; TFT semantic error.  */
  MCM_DATA_CE_TFT_SYNTAX_ERROR_V01 = 105,
  /**<  3GPP Spec defined; TFT syntax error.  */
  MCM_DATA_CE_UNKNOWN_PDP_CONTEXT_V01 = 106,
  /**<  3GPP Spec defined; unknown PDP context.  */
  MCM_DATA_CE_FILTER_SEMANTIC_ERROR_V01 = 107,
  /**<  3GPP Spec defined; filter semantic error.  */
  MCM_DATA_CE_FILTER_SYNTAX_ERROR_V01 = 108,
  /**<  3GPP Spec defined; filter syntax error.  */
  MCM_DATA_CE_PDP_WITHOUT_ACTIVE_TFT_V01 = 109,
  /**<  3GPP Spec defined; PDP is without an active TFT.  */
  MCM_DATA_CE_IP_V4_ONLY_ALLOWED_V01 = 110,
  /**<  3GPP Spec defined; only IPv4 is allowed.  */
  MCM_DATA_CE_IP_V6_ONLY_ALLOWED_V01 = 111,
  /**<  3GPP Spec defined; only IPv6 is allowed.  */
  MCM_DATA_CE_SINGLE_ADDR_BEARER_ONLY_V01 = 112,
  /**<  3GPP Spec defined; single address bearer only.  */
  MCM_DATA_CE_INVALID_TRANSACTION_ID_V01 = 113,
  /**<  3GPP Spec defined; invalid transaction ID.  */
  MCM_DATA_CE_MESSAGE_INCORRECT_SEMANTIC_V01 = 114,
  /**<  3GPP Spec defined; message has incorrect semantic.  */
  MCM_DATA_CE_INVALID_MANDATORY_INFO_V01 = 115,
  /**<  3GPP Spec defined; invalid mandatory information.  */
  MCM_DATA_CE_MESSAGE_TYPE_UNSUPPORTED_V01 = 116,
  /**<  3GPP Spec defined; message type is unsupported.  */
  MCM_DATA_CE_MSG_TYPE_NONCOMPATIBLE_STATE_V01 = 117,
  /**<  3GPP Spec defined; message type is in a noncompatible state.  */
  MCM_DATA_CE_UNKNOWN_INFO_ELEMENT_V01 = 118,
  /**<  3GPP Spec defined; unknown information element.  */
  MCM_DATA_CE_CONDITIONAL_IE_ERROR_V01 = 119,
  /**<  3GPP Spec defined; conditional IE error.  */
  MCM_DATA_CE_MSG_AND_PROTOCOL_STATE_UNCOMPATIBLE_V01 = 120,
  /**<  3GPP Spec defined; message and protocol state are incompatible.  */
  MCM_DATA_CE_PROTOCOL_ERROR_V01 = 121,
  /**<  3GPP Spec defined; protocol error.  */
  MCM_DATA_CE_APN_TYPE_CONFLICT_V01 = 122,
  /**<  3GPP Spec defined; APN type conflict.  */
  MCM_DATA_CE_PPP_TIMEOUT_V01 = 123,
  /**<  PPP; timeout.  */
  MCM_DATA_CE_PPP_AUTH_FAILURE_V01 = 124,
  /**<  PPP; authorization failure.  */
  MCM_DATA_CE_PPP_OPTION_MISMATCH_V01 = 125,
  /**<  PPP; option mismatch.  */
  MCM_DATA_CE_PPP_PAP_FAILURE_V01 = 126,
  /**<  PPP; PAP failure.  */
  MCM_DATA_CE_PPP_CHAP_FAILURE_V01 = 127,
  /**<  PPP; CHAP failure.  */
  MCM_DATA_CE_PPP_UNKNOWN_V01 = 128,
  /**<  PPP; unknown.  */
  MCM_DATA_CE_EHRPD_SUBS_LIMITED_TO_V4_V01 = 129,
  /**<  EHRPD; subscription is limited to v4.  */
  MCM_DATA_CE_EHRPD_SUBS_LIMITED_TO_V6_V01 = 130,
  /**<  EHRPD; subscription is limited to v6.  */
  MCM_DATA_CE_EHRPD_VSNCP_TIMEOUT_V01 = 131,
  /**<  EHRPD VSNCP; timeout.  */
  MCM_DATA_CE_EHRPD_VSNCP_FAILURE_V01 = 132,
  /**<  EHRPD VSNCP; failure.  */
  MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_GEN_ERROR_V01 = 133,
  /**<  EHRPD VSNCP 3GPP2I; generation error.  */
  MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_UNAUTH_APN_V01 = 134,
  /**<  EHRPD VSNCP 3GPP2I; unauthorized APN.  */
  MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_PDN_LIMIT_EXCEED_V01 = 135,
  /**<  EHRPD VSNCP 3GPP2I; PDM limit was exceeded.  */
  MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_NO_PDN_GW_V01 = 136,
  /**<  EHRPD VSNCP 3GPP2I; no PDN GW.  */
  MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_PDN_GW_UNREACH_V01 = 137,
  /**<  EHRPD VSNCP 3GPP2I; PDN GW is unreachable.  */
  MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_PDN_GW_REJ_V01 = 138,
  /**<  EHRPD VSNCP 3GPP2I; PDN GW was rejected.  */
  MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_INSUFF_PARAM_V01 = 139,
  /**<  EHRPD VSNCP 3GPP2I; insufficient parameters.  */
  MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_RESOURCE_UNAVAIL_V01 = 140,
  /**<  EHRPD VSNCP 3GPP2I; resource is unavailable.  */
  MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_ADMIN_PROHIBIT_V01 = 141,
  /**<  EHRPD VSNCP 3GPP2I; administratively prohibited.  */
  MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_PDN_ID_IN_USE_V01 = 142,
  /**<  EHRPD VSNCP 3GPP2I; PDN ID is in use.  */
  MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_SUBSCR_LIMITATION_V01 = 143,
  /**<  EHRPD VSNCP 3GPP2I; subscriber limitation.  */
  MCM_DATA_CE_EHRPD_VSNCP_3GPP2I_PDN_EXISTS_FOR_THIS_APN_V01 = 144,
  /**<  EHRPD VSNCP 3GPP2I; PDN exists for this APN.  */
  MCM_DATA_CE_PREFIX_UNAVAILABLE_V01 = 145
  , /**<  IPv6; prefix is unavailable.  */
  MCM_DATA_CE_IPV6_ERR_HRPD_IPV6_DISABLED_V01 = 146,
  /**<  IPv6; HRPD IPv6 is disabled.  */
  MCM_DATA_CALL_END_REASON_CODE_T_MAX_ENUM_VAL_V01 = 2147483647
  /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_data_call_end_reason_code_t_v01;
/**
    @}
  */

/** @addtogroup mcm_data_aggregates
    @{
  */
typedef struct {

  mcm_data_srv_status_t_v01 srv_status;
  /**< Identifies the service state of the modem. */

  mcm_data_bearer_tech_info_t_v01 tech_info;
  /**< Identifies the preferred technology type. */
}mcm_data_reg_status_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_data_aggregates
    @{
  */
typedef struct {

  uint32_t pkts_tx;
  /**<   Number of packets transmitted.   */

  uint32_t pkts_rx;
  /**< Number of packets received. */

  uint64_t bytes_tx;
  /**<   Number of bytes transmitted.   */

  uint64_t bytes_rx;
  /**< Number of bytes received. */

  uint32_t pkts_dropped_tx;
  /**< Number of transmit packets dropped. */

  uint32_t pkts_dropped_rx;
  /**< Number of receive packets dropped. */
}mcm_data_pkt_stats_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_data_aggregates
    @{
  */
typedef struct {

  uint32_t current_tx_rate;
  /**< Current Tx data rate for the channel. */

  uint32_t current_rx_rate;
  /**< Current Rx data rate for the channel. */

  uint32_t max_tx_rate;
  /**< Maximum Tx data rate for the channel. */

  uint32_t max_rx_rate;
  /**< Maximum Rx data rate for the channel. */
}mcm_data_channel_rate_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_data_aggregates
    @{
  */
typedef struct {

  char valid_addr;
  /**< Indicates whether a valid address is available. */

  uint8_t addr[MCM_DATA_MAX_ADDR_LEN_V01];
  /**< Stores the IP address. */
}mcm_data_addr_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_data_aggregates
    @{
  */
typedef struct {

  mcm_data_addr_t_v01 iface_addr_s;
  /**< Network interface address. */

  uint32_t iface_mask;
  /**< Subnet mask.       */

  mcm_data_addr_t_v01 gtwy_addr_s;
  /**< Gateway server address. */

  uint32_t gtwy_mask;
  /**< Gateway mask.     */

  mcm_data_addr_t_v01 dnsp_addr_s;
  /**< Primary DNS server address. */

  mcm_data_addr_t_v01 dnss_addr_s;
  /**< Secondary DNS server address. */
}mcm_data_addr_t_info_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_data_aggregates
    @{
  */
typedef struct {

  mcm_data_call_end_reason_type_t_v01 call_end_reason_type;
  /**<   Call end reason type. Values: \n
       - 0 -- Unspecified
       - 1 -- Mobile IP
       - 2 -- Internal
       - 3 -- Call manager-defined
       - 6 -- 3GPP specification-defined
       - 7 -- PPP
       - 8 -- EHRPD
       - 9 -- IPv6 @tablebulletend
  */

  mcm_data_call_end_reason_code_t_v01 call_end_reason_code;
  /**< Verbose data call end reason.
  */
}mcm_data_verbose_call_end_reason_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Request message; Sends a request to start a data call for the connection
           associated with the specified call ID. */
typedef struct {

  /* Optional */
  /*  MCM Data IP Family */
  uint8_t ip_family_valid;  /**< Must be set to TRUE if ip_family is being passed. */
  int8_t ip_family;
  /**<   IP family requested: \n
     - 4 -- IPv4
     - 6 -- IPv6
     - 10 -- IPv4/v6 @tablebulletend
  */

  /* Optional */
  /*  MCM Data APN Name */
  uint8_t apn_name_valid;  /**< Must be set to TRUE if apn_name is being passed. */
  char apn_name[MCM_DATA_MAX_APN_LEN_V01];
  /**< APN name requested.
    A character string that identifies a PDN, as defined by the operator.
  */

  /* Optional */
  /*  MCM Data Username for APN */
  uint8_t user_name_valid;  /**< Must be set to TRUE if user_name is being passed. */
  char user_name[MCM_DATA_MAX_USERNAME_LEN_V01];
  /**< Username for the APN. */

  /* Optional */
  /*  MCM Data Password for APN */
  uint8_t password_valid;  /**< Must be set to TRUE if password is being passed. */
  char password[MCM_DATA_MAX_PASSWORD_LEN_V01];
  /**< Password for the APN. */

  /* Optional */
  /*  MCM Data Tech Preference */
  uint8_t tech_pref_valid;  /**< Must be set to TRUE if tech_pref is being passed. */
  int8_t tech_pref;
  /**<   Technology preference: \n
     - 0 -- CDMA
     - 1 -- UMTS @tablebulletend
  */

  /* Optional */
  /*  MCM Data UMTS/LTE Profile */
  uint8_t umts_profile_valid;  /**< Must be set to TRUE if umts_profile is being passed. */
  int8_t umts_profile;
  /**< UMTS/LTE profile ID. */

  /* Optional */
  /*  MCM Data CDMA Profile */
  uint8_t cdma_profile_valid;  /**< Must be set to TRUE if cdma_profile is being passed. */
  int8_t cdma_profile;
  /**< CDMA profile ID. */
}mcm_data_start_data_call_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Response message; Sends a request to start a data call for the connection
           associated with the specified call ID. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 resp;  /**< Result code. */

  /* Mandatory */
  /*  MCM Data Call Status */
  mcm_data_call_status_t_v01 call_status;  /**< MCM data call status. */
  /**<   Call status.*/

  /* Optional */
  /*  MCM Data Call ID */
  uint8_t call_id_valid;  /**< Must be set to TRUE if call_id is being passed. */
  int32_t call_id;
  /**< Call ID that gets generated for a successful call. */

  /* Optional */
  /*  MCM Data Call End Reason */
  uint8_t vce_reason_valid;  /**< Must be set to TRUE if vce_reason is being passed. */
  mcm_data_verbose_call_end_reason_t_v01 vce_reason;
  /**< Call end reason in verbose. */
}mcm_data_start_data_call_rsp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Request message; Sends a request to stop a data call for the connection
           associated with the specified call ID. */
typedef struct {

  /* Mandatory */
  /*  MCM DATA Call ID */
  int32_t call_id;
  /**< Call ID of the call to be stopped. */
}mcm_data_stop_data_call_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Response message; Sends a request to stop a data call for the connection
           associated with the specified call ID. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Response. */
  /**<   Result code.*/
}mcm_data_stop_data_call_rsp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Request message; Gets packet statistics for the connection associated with the
           specified call ID. */
typedef struct {

  /* Mandatory */
  /*  MCM Data Call ID */
  int32_t call_id;
  /**< Call ID of the connection for which to get the packet statistics. @newpagetable */
}mcm_data_get_pkt_stats_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Response message; Gets packet statistics for the connection associated with the
           specified call ID. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Response. */
  /**<   Result code.*/

  /* Optional */
  /*  Packet Status */
  uint8_t pkt_stats_valid;  /**< Must be set to TRUE if pkt_stats is being passed. */
  mcm_data_pkt_stats_t_v01 pkt_stats;  /**< Packet statistics. */
}mcm_data_get_pkt_stats_rsp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Request message; Resets packet statistics for the connection associated with the
           specified call ID. */
typedef struct {

  /* Mandatory */
  /*  MCM Data Call ID */
  int32_t call_id;
  /**< Call ID of the connection for which to reset the packet statistics. */
}mcm_data_reset_pkt_stats_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Response message; Resets packet statistics for the connection associated with the
           specified call ID. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Response. */
  /**<   Result code.*/
}mcm_data_reset_pkt_stats_rsp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Request message; Gets routing information for the connection associated with the
           specified call ID. */
typedef struct {

  /* Mandatory */
  /*  MCM Data Call ID */
  int32_t call_id;
  /**< Call ID of the connection for which to get the routing information. @newpagetable */
}mcm_data_get_device_name_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Response message; Gets routing information for the connection associated with the
           specified call ID. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Response. */
  /**<   Result code.*/

  /* Optional */
  /*  MCM Data Device name */
  uint8_t device_name_valid;  /**< Must be set to TRUE if device_name is being passed. */
  uint32_t device_name_len;  /**< Must be set to # of elements in device_name */
  char device_name[MCM_DATA_MAX_DEVICE_NAME_LEN_V01];
  /**<   Device name. */
}mcm_data_get_device_name_rsp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Request message; Gets the number of IP addresses available for the connection
           associated with the specified call ID. */
typedef struct {

  /* Mandatory */
  /*  MCM Data Call ID */
  int32_t call_id;
  /**< Call ID of the connection for which to get the address count. */
}mcm_data_get_device_addr_count_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Response message; Gets the number of IP addresses available for the connection
           associated with the specified call ID. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Response. */
  /**<   Result code.*/

  /* Optional */
  /*  MCM Data Device Addr Count */
  uint8_t addr_count_valid;  /**< Must be set to TRUE if addr_count is being passed. */
  int8_t addr_count;  /**< Data device address count. */
}mcm_data_get_device_addr_count_rsp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Request message; Gets underlying technology available for the connection associated
           with the specified call ID. */
typedef struct {

  /* Mandatory */
  /*  MCM Data Call ID */
  int32_t call_id;
  /**< Call ID of the connection for which to get the call technology. */
}mcm_data_get_call_tech_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Response message; Gets the underlying technology available for the connection associated
           with the specified call ID. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Response. */
  /**<   Result code.*/

  /* Optional */
  /*  MCM Data Call Bearer Tech */
  uint8_t call_tech_valid;  /**< Must be set to TRUE if call_tech is being passed. */
  mcm_data_bearer_tech_info_t_v01 call_tech;  /**< Data call bearer technology. */
}mcm_data_get_call_tech_rsp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Request message; Gets the current status of the connection associated with the
           specified call ID. */
typedef struct {

  /* Mandatory */
  /*  MCM Data Call ID */
  int32_t call_id;
  /**< Call ID of the connection for which to get the call status. */
}mcm_data_get_call_status_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Response message; Gets the current status of the connection associated with the
           specified call ID. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Response. */
  /**<   Result code.*/

  /* Optional */
  /*  MCM Data Call Status */
  uint8_t call_status_valid;  /**< Must be set to TRUE if call_status is being passed. */
  mcm_data_call_status_t_v01 call_status;  /**< Data call status. */
}mcm_data_get_call_status_rsp_msg_v01;  /* Message */
/**
    @}
  */
/** @cond
*/

typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}mcm_data_get_reg_status_req_msg_v01;

/** @endcond */

/** @addtogroup mcm_data_messages
    @{
  */
/** Response message; Gets the current data registration status. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Response. */
  /**<   Result code.*/

  /* Optional */
  /*  MCM Data Reg Status */
  uint8_t reg_status_valid;  /**< Must be set to TRUE if reg_status is being passed. */
  mcm_data_reg_status_t_v01 reg_status;  /**< Data modem registration status. */
}mcm_data_get_reg_status_rsp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Request message; Gets a list of IP address for the connection associated with the
           specified call ID. */
typedef struct {

  /* Mandatory */
  /*  MCM Data Call ID */
  int32_t call_id;
  /**< Call ID of the connection for which to get the IP address. */
}mcm_data_get_device_addr_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Response message; Gets a list of IP address for the connection associated with the
           specified call ID. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Response. */
  /**<   Result code.*/

  /* Optional */
  /*  MCM Data Device Addr */
  uint8_t addr_info_valid;  /**< Must be set to TRUE if addr_info is being passed. */
  uint32_t addr_info_len;  /**< Must be set to the number of elements in addr_info. */
  mcm_data_addr_t_info_v01 addr_info[MCM_DATA_MAX_ADDR_COUNT_V01];  /**< Data device address. */
}mcm_data_get_device_addr_rsp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Request message; Gets the current and maximum channel rate of the connection associated
            with the specified call ID. */
typedef struct {

  /* Mandatory */
  /*  MCM Data Call ID */
  int32_t call_id;
  /**< Call ID of the connection for which to get the channel rate. */
}mcm_data_get_channel_rate_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Response message; Gets the current and maximum channel rate of connection associated
            with the specified call ID. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Response. */
  /**<   Result code.*/

  /* Optional */
  /*  MCM Data Channel Rate */
  uint8_t channel_rate_valid;  /**< Must be set to TRUE if channel_rate is being passed */
  mcm_data_channel_rate_t_v01 channel_rate;  /**< Data channel rate. */
}mcm_data_get_channel_rate_rsp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Request message; Registers for an indication of events */
typedef struct {

  /* Optional */
  uint8_t register_net_up_event_valid;  /**< Must be set to TRUE if register_net_up_event is being passed. */
  uint8_t register_net_up_event;  /**< Register for a network up event. */
  /**< Network up event. */

  /* Optional */
  uint8_t register_net_down_event_valid;  /**< Must be set to TRUE if register_net_down_event is being passed. */
  uint8_t register_net_down_event;  /**< Register for a network down event. */
  /**< Network down event. */

  /* Optional */
  uint8_t register_net_new_addr_event_valid;  /**< Must be set to TRUE if register_net_new_addr_event is being passed. */
  uint8_t register_net_new_addr_event;  /**< Register for a new address event. */
  /**< Net new address event. */

  /* Optional */
  uint8_t register_net_del_addr_event_valid;  /**< Must be set to TRUE if register_net_del_addr_event is being passed. */
  uint8_t register_net_del_addr_event;  /**< Register for a delete address event. */
  /**< Network deleted address event. */

  /* Optional */
  uint8_t register_reg_srvc_status_event_valid;  /**< Must be set to TRUE if register_reg_srvc_status_event is being passed. */
  uint8_t register_reg_srvc_status_event;  /**< Register for a service status event. */
  /**< Registration service status event. */

  /* Optional */
  uint8_t register_bearer_tech_status_event_valid;  /**< Must be set to TRUE if register_bearer_tech_status_event is being passed. */
  uint8_t register_bearer_tech_status_event;  /**< Register for a bearer technology status event. */
  /**< Bearer technology status event. */

  /* Optional */
  uint8_t register_dormancy_status_event_valid;  /**< Must be set to TRUE if register_dormancy_status_event is being passed. */
  uint8_t register_dormancy_status_event;  /**< Register for a dormancy status event. */
  /**< Dormancy status event. */
}mcm_data_event_register_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Response message; Registers for an indication of events. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;  /**< Response. */
  /**<   Result code.*/
}mcm_data_event_register_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_data_messages
    @{
  */
/** Indication message; Indication corresponding to an unsolicited event. */
typedef struct {

  /* Mandatory */
  /*  MCM Data Event ID */
  int32_t event_id;
  /**< Event ID: \n
  - 0x00005001 -- MCM_DATA_NET_UP_EV
  - 0x00005002 -- MCM_DATA_NET_DOWN_EV
  - 0x00005003 -- MCM_DATA_NET_NEW_ADDR_EV
  - 0x00005004 -- MCM_DATA_NET_DEL_ADDR_EV
  - 0x00005005 -- MCM_DATA_REG_SRVC_STATUS_EV
  - 0x00005006 -- MCM_DATA_BEARER_TECH_STATUS_EV
  - 0x00005007 -- MCM_DATA_DORMANCY_STATUS_EV @tablebulletend
  */

  /* Optional */
  /*  MCM Data Call ID */
  uint8_t call_id_valid;  /**< Must be set to TRUE if call_id is being passed. */
  int32_t call_id;  /**< Call ID that gets generated for a succesful call. */

  /* Optional */
  /*  Call ID that gets generated for a succesful call. 
 MCM Data Call status */
  uint8_t call_status_valid;  /**< Must be set to TRUE if call_status is being passed. */
  mcm_data_call_status_t_v01 call_status;  /**< Data call status. */

  /* Optional */
  /*  MCM Data Device Call Bearer State */
  uint8_t call_tech_valid;  /**< Must be set to TRUE if call_tech is being passed. */
  mcm_data_bearer_tech_info_t_v01 call_tech;  /**< Data bearer technology corresponding to the call ID. */

  /* Optional */
  /*  MCM Data Reg Status */
  uint8_t reg_status_valid;  /**< Must be set to TRUE if reg_status is being passed. */
  mcm_data_reg_status_t_v01 reg_status;  /**< Data modem registration status. */

  /* Optional */
  /*  MCM Data Dormant Status */
  uint8_t dorm_status_valid;  /**< Must be set to TRUE if dorm_status is being passed. */
  mcm_data_dormancy_state_t_v01 dorm_status;  /**< Data dormancy status. */

  /* Optional */
  /*  MCM Data Device Addr Count */
  uint8_t addr_count_valid;  /**< Must be set to TRUE if addr_count is being passed. */
  int8_t addr_count;  /**< Data device address count. */

  /* Optional */
  /*  MCM Data Device Addr */
  uint8_t addr_info_valid;  /**< Must be set to TRUE if addr_info is being passed. */
  uint32_t addr_info_len;  /**< Must be set to the number of elements in addr_info. */
  mcm_data_addr_t_info_v01 addr_info[MCM_DATA_MAX_ADDR_COUNT_V01];  /**< Data device address. */

  /* Optional */
  /*  MCM Data Call End Reason */
  uint8_t vce_reason_valid;  /**< Must be set to TRUE if vce_reason is being passed. */
  mcm_data_verbose_call_end_reason_t_v01 vce_reason;  /**< Data call end reason in verbose. */
}mcm_data_unsol_event_ind_msg_v01;  /* Message */
/**
    @}
  */

/*Service Message Definition*/
/** @addtogroup mcm_data_msg_ids
    @{
  */

/** @name
@{ */

#define MCM_DATA_START_DATA_CALL_REQ_V01 0x0100
#define MCM_DATA_START_DATA_CALL_RSP_V01 0x0100
#define MCM_DATA_STOP_DATA_CALL_REQ_V01 0x0101
#define MCM_DATA_STOP_DATA_CALL_RSP_V01 0x0101
#define MCM_DATA_GET_PKT_STATS_REQ_V01 0x0102
#define MCM_DATA_GET_PKT_STATS_RSP_V01 0x0102
#define MCM_DATA_RESET_PKT_STATS_REQ_V01 0x0103
#define MCM_DATA_RESET_PKT_STATS_RSP_V01 0x0103
#define MCM_DATA_GET_DEVICE_NAME_REQ_V01 0x0104
#define MCM_DATA_GET_DEVICE_NAME_RSP_V01 0x0104
#define MCM_DATA_GET_DEVICE_ADDR_COUNT_REQ_V01 0x0105
#define MCM_DATA_GET_DEVICE_ADDR_COUNT_RSP_V01 0x0105
#define MCM_DATA_GET_CALL_TECH_REQ_V01 0x0106
#define MCM_DATA_GET_CALL_TECH_RSP_V01 0x0106
#define MCM_DATA_GET_CALL_STATUS_REQ_V01 0x0107
#define MCM_DATA_GET_CALL_STATUS_RSP_V01 0x0107
#define MCM_DATA_GET_DEVICE_ADDR_REQ_V01 0x0108
#define MCM_DATA_GET_DEVICE_ADDR_RSP_V01 0x0108
#define MCM_DATA_GET_CHANNEL_RATE_REQ_MSG_V01 0x0109
#define MCM_DATA_GET_CHANNEL_RATE_RSP_MSG_V01 0x0109
#define MCM_DATA_EVENT_REGISTER_REQ_V01 0x010A
#define MCM_DATA_EVENT_REGISTER_RESP_V01 0x010A
#define MCM_DATA_GET_REG_STATUS_REQ_MSG_V01 0x010B
#define MCM_DATA_GET_REG_STATUS_RSP_MSG_V01 0x010B
#define MCM_DATA_UNSOL_EVENT_IND_V01 0x010C

/** @} */ /* end_namegroup */

/**
    @} */ /* end_namegroup */

/** @} */

#ifdef __cplusplus
}
#endif
#endif

