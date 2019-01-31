#ifndef MCM_NW_SERVICE_01_H
#define MCM_NW_SERVICE_01_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
   Copyright (c) 2013, 2017 Qualcomm Technologies, Inc.
   All rights reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
#include <stdint.h>
#include "mcm_common_v01.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup mcm_nw_consts
    @{
  */
/** Maximum items in the network scan list. */
#define MCM_NW_SCAN_LIST_MAX_V01 40
#define MCM_NW_NITZ_STR_BUF_MAX_V01 30

/**
    @}
  */

typedef uint64_t mcm_nw_mode_type_v01;

/** @addtogroup mcm_nw_consts
    @{
  */

#define MCM_MODE_NONE_V01 ((mcm_nw_mode_type_v01)0x00ull) /**<  No network. */
#define MCM_MODE_GSM_V01 ((mcm_nw_mode_type_v01)0x01ull) /**<  Include GSM networks. */
#define MCM_MODE_WCDMA_V01 ((mcm_nw_mode_type_v01)0x02ull) /**<  Include WCDMA networks. */
#define MCM_MODE_CDMA_V01 ((mcm_nw_mode_type_v01)0x04ull) /**<  Include CDMA networks. */
#define MCM_MODE_EVDO_V01 ((mcm_nw_mode_type_v01)0x08ull) /**<  Include EVDO networks. */
#define MCM_MODE_LTE_V01 ((mcm_nw_mode_type_v01)0x10ull) /**<  Include LTE networks. */
#define MCM_MODE_TDSCDMA_V01 ((mcm_nw_mode_type_v01)0x20ull) /**<  Include TDSCDMA networks. */
#define MCM_MODE_PRL_V01 ((mcm_nw_mode_type_v01)0x10000ull) /**<  Give preference according to the preferred
 roaming list. */
/**
    @}
  */


/** @addtogroup mcm_nw_enums
    @{
  */
typedef enum {
  MCM_NW_SERVICE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_NW_SERVICE_NONE_V01 = 0x0000, /**<  Not registered or no data. */
  MCM_NW_SERVICE_LIMITED_V01 = 0x0001, /**<  Registered; emergency service only. */
  MCM_NW_SERVICE_FULL_V01 = 0x0002, /**<  Registered, full service. */
  MCM_NW_SERVICE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_nw_service_t_v01;
/**
    @}
  */

/** @addtogroup mcm_nw_enums
    @{
  */
typedef enum {
  MCM_NW_SELECTION_TYPE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_NW_SELECTION_AUTOMATIC_V01 = 0x0000, /**<  Automatic network selection. */
  MCM_NW_SELECTION_MANUAL_V01 = 0x0001, /**<  Manual network selection. */
  MCM_NW_SELECTION_TYPE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_nw_selection_type_t_v01;
/**
    @}
  */

/** @addtogroup mcm_nw_enums
    @{
  */
typedef enum {
  MCM_NW_NETWORK_STATUS_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_NW_NETWORK_STATUS_NONE_V01 = 0x0000, /**<  Network status not available. */
  MCM_NW_NETWORK_STATUS_CURRENT_SERVING_V01 = 0x0001, /**<  Current serving network. */
  MCM_NW_NETWORK_STATUS_PREFERRED_V01 = 0x0002, /**<  Preferred network. */
  MCM_NW_NETWORK_STATUS_NOT_PREFERRED_V01 = 0x0003, /**<  Not the preferred network. */
  MCM_NW_NETWORK_STATUS_AVAILABLE_V01 = 0x0004, /**<  Service available. */
  MCM_NW_NETWORK_STATUS_FORBIDDEN_V01 = 0x0005, /**<  Forbidden service. */
  MCM_NW_NETWORK_STATUS_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_nw_network_status_t_v01;
/**
    @}
  */

/** @addtogroup mcm_nw_enums
    @{
  */
typedef enum {
  MCM_NW_RADIO_TECH_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_NW_RADIO_TECH_TD_SCDMA_V01 = 1,
  MCM_NW_RADIO_TECH_GSM_V01 = 2, /**<  GSM; only supports voice. */
  MCM_NW_RADIO_TECH_HSPAP_V01 = 3, /**<  HSPA+. */
  MCM_NW_RADIO_TECH_LTE_V01 = 4, /**<  LTE. */
  MCM_NW_RADIO_TECH_EHRPD_V01 = 5, /**<  EHRPD. */
  MCM_NW_RADIO_TECH_EVDO_B_V01 = 6, /**<  EVDO B. */
  MCM_NW_RADIO_TECH_HSPA_V01 = 7, /**<  HSPA. */
  MCM_NW_RADIO_TECH_HSUPA_V01 = 8, /**<  HSUPA. */
  MCM_NW_RADIO_TECH_HSDPA_V01 = 9, /**<  HSDPA. */
  MCM_NW_RADIO_TECH_EVDO_A_V01 = 10, /**<  EVDO A. */
  MCM_NW_RADIO_TECH_EVDO_0_V01 = 11, /**<  EVDO 0. */
  MCM_NW_RADIO_TECH_1xRTT_V01 = 12, /**<  1xRTT. */
  MCM_NW_RADIO_TECH_IS95B_V01 = 13, /**<  IS95B. */
  MCM_NW_RADIO_TECH_IS95A_V01 = 14, /**<  IS95A. */
  MCM_NW_RADIO_TECH_UMTS_V01 = 15, /**<  UMTS. */
  MCM_NW_RADIO_TECH_EDGE_V01 = 16, /**<  EDGE. */
  MCM_NW_RADIO_TECH_GPRS_V01 = 17, /**<  GPRS. */
  MCM_NW_RADIO_TECH_NONE_V01 = 18, /**<  No technology selected. */
  MCM_NW_RADIO_TECH_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_nw_radio_tech_t_v01;
/**
    @}
  */

/** @addtogroup mcm_nw_enums
    @{
  */
typedef enum {
  MCM_NW_CELL_ACCESS_STATE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_NW_CELL_ACCESS_NONE_V01 = 0x00, /**<  Unknown cell access state. */
  MCM_NW_CELL_ACCESS_NORMAL_ONLY_V01 = 0x01, /**<  Cell access is allowed for normal calls only. */
  MCM_NW_CELL_ACCESS_EMERGENCY_ONLY_V01 = 0x02, /**<  Cell access is allowed for emergency calls only. */
  MCM_NW_CELL_ACCESS_NO_CALLS_V01 = 0x03, /**<  Cell access is not allowed for any call type. */
  MCM_NW_CELL_ACCESS_ALL_CALLS_V01 = 0x04, /**<  Cell access is allowed for all call types. */
  MCM_NW_CELL_ACCESS_STATE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_nw_cell_access_state_t_v01;
/**
    @}
  */

/** @addtogroup mcm_nw_enums
    @{
  */
typedef enum {
  MCM_NW_ROAM_STATE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_NW_ROAM_STATE_OFF_V01 = 0, /**<  None, or roaming indicator off. */
  MCM_NW_ROAM_STATE_ON_V01 = 1, /**<  Roaming indicator on. */
  MCM_NW_ROAM_STATE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_nw_roam_state_t_v01;
/**
    @}
  */

/** @addtogroup mcm_nw_enums
    @{
  */
typedef enum {
  MCM_NW_TECH_DOMAIN_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_NW_TECH_DOMAIN_NONE_V01 = 0, /**<  None. */
  MCM_NW_TECH_DOMAIN_3GPP_V01 = 1, /**<  3GPP. */
  MCM_NW_TECH_DOMAIN_3GPP2_V01 = 2, /**<  3GPP2. */
  MCM_NW_TECH_DOMAIN_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_nw_tech_domain_t_v01;
/**
    @}
  */

/** @addtogroup mcm_nw_enums
    @{
  */
typedef enum {
  MCM_NW_DENY_REASON_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_NW_IMSI_UNKNOWN_HLR_DENY_REASON_V01 = 1, /**<  IMSI unknown in HLR. */
  MCM_NW_ILLEGAL_MS_DENY_REASON_V01 = 2, /**<  Illegal MS. */
  MCM_NW_IMSI_UNKNOWN_VLR_DENY_REASON_V01 = 3, /**<  IMSI unknown in VLR. */
  MCM_NW_IMEI_NOT_ACCEPTED_DENY_REASON_V01 = 4, /**<  IMEI not accepted. */
  MCM_NW_ILLEGAL_ME_DENY_REASON_V01 = 5, /**<  Illegal ME. */
  MCM_NW_PLMN_NOT_ALLOWED_DENY_REASON_V01 = 6, /**<  PLMN not allowed. */
  MCM_NW_LA_NOT_ALLOWED_DENY_REASON_V01 = 7, /**<  Location area not allowed. */
  MCM_NW_ROAMING_NOT_ALLOWED_LA_DENY_REASON_V01 = 8, /**<  Roaming not allowed in this location area. */
  MCM_NW_NO_SUITABLE_CELLS_LA_DENY_REASON_V01 = 9, /**<  No suitable cells in location area. */
  MCM_NW_NETWORK_FAILURE_DENY_REASON_V01 = 10, /**<  Network failure. */
  MCM_NW_MAC_FAILURE_DENY_REASON_V01 = 11, /**<  MAC failure. */
  MCM_NW_SYNCH_FAILURE_DENY_REASON_V01 = 12, /**<  Sync failure. */
  MCM_NW_CONGESTION_DENY_REASON_V01 = 13, /**<  Congestion. */
  MCM_NW_GSM_AUTHENTICATION_UNACCEPTABLE_DENY_REASON_V01 = 14, /**<  GSM authentication unacceptable. */
  MCM_NW_NOT_AUTHORIZED_CSG_DENY_REASON_V01 = 15, /**<  Not authorized in this CSG. */
  MCM_NW_SERVICE_OPTION_NOT_SUPPORTED_DENY_REASON_V01 = 16, /**<  Service option not supported. */
  MCM_NW_REQ_SERVICE_OPTION_NOT_SUBSCRIBED_DENY_REASON_V01 = 17, /**<  Requested service option not subscribed. */
  MCM_NW_CALL_CANNOT_BE_IDENTIFIED_DENY_REASON_V01 = 18, /**<  Call cannot be identified. */
  MCM_NW_SEMANTICALLY_INCORRECT_MSG_DENY_REASON_V01 = 19, /**<  Semantically incorrect message. */
  MCM_NW_INVALID_MANDATORY_INFO_DENY_REASON_V01 = 20, /**<  Invalid mandatory information. */
  MCM_NW_MSG_TYPE_NON_EXISTENT_DENY_REASON_V01 = 21, /**<  Message type non-existent or not implemented. */
  MCM_NW_INFO_ELEMENT_NON_EXISTENT_DENY_REASON_V01 = 22, /**<  Message type not compatible with the protocol state. */
  MCM_NW_CONDITIONAL_IE_ERR_DENY_REASON_V01 = 23, /**<  Conditional IE error. */
  MCM_NW_MSG_INCOMPATIBLE_PROTOCOL_STATE_DENY_REASON_V01 = 24, /**<  Message not compatible with the protocol state. */
  MCM_NW_PROTOCOL_ERROR_DENY_REASON_V01 = 25, /**<  Unspecified protocol error. */
  MCM_NW_DENY_REASON_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_nw_deny_reason_t_v01;
/**
    @}
  */

/** @addtogroup mcm_nw_aggregates
    @{
  */
typedef struct {

  char long_eons[512 + 1];
  /**<   Long EONS.*/

  char short_eons[512 + 1];
  /**<   Short EONS.*/

  char mcc[3 + 1];
  /**<   Mobile country code.*/

  char mnc[3 + 1];
  /**<   Mobile network code.*/
}mcm_nw_operator_name_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_nw_aggregates
    @{
  */
typedef struct {

  mcm_nw_operator_name_t_v01 operator_name;
  /**<   Operator name.*/

  mcm_nw_network_status_t_v01 network_status;
  /**<   Network status.*/

  mcm_nw_radio_tech_t_v01 rat;
  /**<   Radio technology.*/
}mcm_nw_scan_entry_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_nw_aggregates
    @{
  */
typedef struct {

  mcm_nw_tech_domain_t_v01 tech_domain;
  /**<   Technology, used to determine the structure type
 mcm_tech: 0 -- None, 1 -- 3GPP, 2 -- 3GPP2.*/

  mcm_nw_radio_tech_t_v01 radio_tech;
  /**<   Radio technology; see #mcm_nw_radio_tech_t_v01.*/

  mcm_nw_roam_state_t_v01 roaming;
  /**<   0 -- Off, 1 -- Roaming (3GPP2 has extended values).*/

  mcm_nw_deny_reason_t_v01 deny_reason;
  /**<   Set when registration state is
 #mcm_nw_deny_reason_t_v01.*/

  mcm_nw_service_t_v01 registration_state;
  /**<   Registration state.*/
}mcm_nw_common_registration_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_nw_aggregates
    @{
  */
typedef struct {

  mcm_nw_tech_domain_t_v01 tech_domain;
  /**<   Technology, used to determine the structure type
 mcm_tech: 0 -- None, 1 -- 3GPP, 2 -- 3GPP2.*/

  mcm_nw_radio_tech_t_v01 radio_tech;
  /**<   Radio technology; see #mcm_nw_radio_tech_t_v01.*/

  char mcc[3+1];
  /**<   Mobile country code.*/

  char mnc[3+1];
  /**<   Mobile network code.*/

  mcm_nw_roam_state_t_v01 roaming;
  /**<   0 -- Off, 1 -- Roaming (3GPP2 has extended values).*/

  uint8_t forbidden;
  /**<   Forbidden: 0 -- No, 1 -- Yes.*/

  uint32_t cid;
  /**<   Cell ID for the registered 3GPP system.*/

  uint16_t lac;
  /**<   Locatin area code for the registered 3GPP system.*/

  uint16_t psc;
  /**<   Primary scrambling code (WCDMA only); 0 -- None.*/

  uint16_t tac;
  /**<   Tracking area code information for LTE.*/
}mcm_nw_3gpp_registration_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_nw_aggregates
    @{
  */
typedef struct {

  mcm_nw_tech_domain_t_v01 tech_domain;
  /**<   Technology, used to determine structure type
 mcm_tech: 0 -- None, 1 -- 3GPP, 2 -- 3GPP2.*/

  mcm_nw_radio_tech_t_v01 radio_tech;
  /**<   Radio technology; see #mcm_nw_radio_tech_t_v01.*/

  char mcc[3+1];
  /**<   Mobile country code.*/

  char mnc[3+1];
  /**<   Mobile network code.*/

  mcm_nw_roam_state_t_v01 roaming;
  /**<   Roaming status; see #mcm_nw_roam_state_t_v01.*/

  uint8_t forbidden;
  /**<   Forbidden: 0 -- No, 1 -- Yes.*/

  uint8_t inPRL;
  /**<   0 -- Not in PRL, 1 -- In PRL.*/

  uint8_t css;
  /**<   Concurrent services supported: 0 -- No, 1 -- Yes.*/

  uint16_t sid;
  /**<   CDMA system ID.*/

  uint16_t nid;
  /**<   CDMA network ID.*/

  uint16_t bsid;
  /**<   Base station ID. @newpagetable */
}mcm_nw_3gpp2_registration_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_nw_aggregates
    @{
  */
typedef struct {

  mcm_nw_selection_type_t_v01 nw_selection_type;
  /**< Network selection type. */

  char mcc[3 + 1];
  /**<   Mobile country code for a manual network selection.*/

  char mnc[3 + 1];
  /**<   Mobile network code for a manual network selection.*/

  mcm_nw_radio_tech_t_v01 rat;
  /**<   Radio technology.*/
}mcm_nw_selection_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_nw_aggregates
    @{
  */
typedef struct {

  int8_t rssi;
  /**<   RSSI in dBm. Indicates received signal strength.
 A signed value; -125 or lower indicates no signal.*/
}mcm_nw_gsm_sig_info_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_nw_aggregates
    @{
  */
typedef struct {

  int8_t rssi;
  /**<   RSSI in dBm. Indicates forward link pilot Ec.
 A signed value; -125 or lower indicates no signal.*/

  int16_t ecio;
  /**<   Ec/Io value representing negative 0.5 dB increments, e.g., 2 equals -1 dbm.*/
}mcm_nw_wcdma_sig_info_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_nw_aggregates
    @{
  */
typedef struct {

  int8_t rssi;
  /**<   RSSI in dBm. Indicates forward link pilot Ec.
 a signed value; -125 or lower indicates no signal.*/

  int8_t rscp;
  /**<   RSCP in dBm.*/

  int16_t ecio;
  /**<   Ec/Io value representing negative 0.5 dB increments, e.g., 2 equals -1 dbm.*/

  int8_t sinr;
  /**<   Measured SINR in dB. @newpagetable */
}mcm_nw_tdscdma_sig_info_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_nw_aggregates
    @{
  */
typedef struct {

  int8_t rssi;
  /**<   RSSI in dBm. Indicates forward link pilot Ec.
 A signed value; -125 or lower indicates no signal.*/

  int8_t rsrq;
  /**<   RSRQ value in dB (signed integer value), as measured by L1.
 Range: -3 to -20 (-3 equals -3 dB, -20 equals -20 dB).*/

  int16_t rsrp;
  /**<   Current RSRP in dBm, as measured by L1.
 Range: -44 to -140 (-44 equals -44 dBm, -140 equals -140 dBm).*/

  int16_t snr;
  /**<   SNR level as a scaled integer in units of 0.1 dB;
 e.g., -16 dB has a value of -160 and 24.6 dB has a value of 246.*/
}mcm_nw_lte_sig_info_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_nw_aggregates
    @{
  */
typedef struct {

  int8_t rssi;
  /**<   RSSI in dBm. Indicates forward link pilot Power (AGC) + Ec/Io.
 A signed value; -125 or lower indicates no signal.*/

  int16_t ecio;
  /**<   Ec/Io value representing negative 0.5 dB increments, e.g., 2 equals -1 dbm.*/
}mcm_nw_cdma_sig_info_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_nw_aggregates
    @{
  */
typedef struct {

  int8_t rssi;
  /**<   RSSI in dBm. Indicates forward link pilot Power (AGC) + Ec/Io.
 A signed value; -125 or lower indicates no signal.*/

  int16_t ecio;
  /**<   Ec/Io value representing negative 0.5 dB increments, e.g., 2 equals -1 dbm.*/

  int8_t sinr;
  /**<   SINR level.*/

  int32_t io;
  /**<   Received IO in dBm. */
}mcm_nw_hdr_sig_info_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_nw_qmi_aggregates
    @{
  */
typedef struct {

  char nitz_time[MCM_NW_NITZ_STR_BUF_MAX_V01 + 1];
  /**<   NITZ Time.*/
}mcm_nw_nitz_time_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Request message; Configures the settings that define the MCM network interface. */
typedef struct {

  /* Optional */
  /*  Configuration parameters for MCM network registration
 Network registration details
 Technology dependent network registration details */
  uint8_t preferred_nw_mode_valid;  /**< Must be set to TRUE if preferred_nw_mode is being passed. */
  uint64_t preferred_nw_mode;
  /**<   Preferred network mode for connections;
 a bitmask of mcm_nw_mode.*/

  /* Optional */
  uint8_t roaming_pref_valid;  /**< Must be set to TRUE if roaming_pref is being passed. */
  mcm_nw_roam_state_t_v01 roaming_pref;
  /**<   Roaming preference.*/
}mcm_nw_set_config_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Response message; Configures the settings that define the MCM network interface. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code. @newpagetable */
}mcm_nw_set_config_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @cond
*/

typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}mcm_nw_get_config_req_msg_v01;

/** @endcond */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Response message; Gets the configuration status for this network interface. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/

  /* Optional */
  /*  Configuration parameters for MCM network registration
 Network registration details
 Technology dependent network registration details */
  uint8_t preferred_nw_mode_valid;  /**< Must be set to TRUE if preferred_nw_mode is being passed. */
  uint64_t preferred_nw_mode;
  /**<   Preferred network mode for connections;
 a bitmask of mcm_nw_mode.*/

  /* Optional */
  uint8_t roaming_pref_valid;  /**< Must be set to TRUE if roaming_pref is being passed. */
  mcm_nw_roam_state_t_v01 roaming_pref;
  /**<   Roaming preference.*/
}mcm_nw_get_config_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @cond
*/

typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}mcm_nw_get_registration_status_req_msg_v01;

/** @endcond */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Response message; Gets the status associated with the connection of \<id\>. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/

  /* Optional */
  uint8_t voice_registration_valid;  /**< Must be set to TRUE if voice_registration is being passed. */
  mcm_nw_common_registration_t_v01 voice_registration;
  /**<   Voice registration. */

  /* Optional */
  uint8_t data_registration_valid;  /**< Must be set to TRUE if data_registration is being passed. */
  mcm_nw_common_registration_t_v01 data_registration;
  /**<   Data registration. */

  /* Optional */
  uint8_t voice_registration_details_3gpp_valid;  /**< Must be set to TRUE if voice_registration_details_3gpp is being passed. */
  mcm_nw_3gpp_registration_t_v01 voice_registration_details_3gpp;
  /**<   Voice registration details for 3GPP. */

  /* Optional */
  uint8_t data_registration_details_3gpp_valid;  /**< Must be set to TRUE if data_registration_details_3gpp is being passed. */
  mcm_nw_3gpp_registration_t_v01 data_registration_details_3gpp;
  /**<   Data registration details for 3GPP. */

  /* Optional */
  uint8_t voice_registration_details_3gpp2_valid;  /**< Must be set to TRUE if voice_registration_details_3gpp2 is being passed. */
  mcm_nw_3gpp2_registration_t_v01 voice_registration_details_3gpp2;
  /**<   Voice registration details for 3GPP2. */

  /* Optional */
  uint8_t data_registration_details_3gpp2_valid;  /**< Must be set to TRUE if data_registration_details_3gpp2 is being passed. */
  mcm_nw_3gpp2_registration_t_v01 data_registration_details_3gpp2;
  /**<   Data registration details for 3GPP2. */
}mcm_nw_get_registration_status_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @cond
*/

typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}mcm_nw_scan_req_msg_v01;

/** @endcond */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Response message; Gets the status associated with the connection of \<id\>. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/

  /* Optional */
  uint8_t entry_valid;  /**< Must be set to TRUE if entry is being passed. */
  uint32_t entry_len;  /**< Must be set to the number of elements in entry. */
  mcm_nw_scan_entry_t_v01 entry[MCM_NW_SCAN_LIST_MAX_V01];
  /**<   Scan entry.*/
}mcm_nw_scan_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @cond
*/

typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}mcm_nw_get_operator_name_req_msg_v01;

/** @endcond */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Response message; Gets the operator name associated with the connection of \<id\>. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/

  /* Optional */
  uint8_t operator_name_valid;  /**< Must be set to TRUE if operator_name is being passed. */
  mcm_nw_operator_name_t_v01 operator_name;
  /**<   Operator name.*/
}mcm_nw_get_operator_name_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Request message; Turns the screen on/off to save the battery. */
typedef struct {

  /* Mandatory */
  uint8_t turn_off_screen;
  /**<   Turn the screen off.*/
}mcm_nw_screen_on_off_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Response message; Turns the screen on/off to save the battery. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/
}mcm_nw_screen_on_off_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Request message; Network selection (manual or automatic). */
typedef struct {

  /* Mandatory */
  mcm_nw_selection_t_v01 nw_selection_info;
  /**<   Network selection information.*/
}mcm_nw_selection_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Response message; Network selection (manual or automatic). */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/
}mcm_nw_selection_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @cond
*/

/** @addtogroup mcm_nw_messages
    @{
  */
/** Request Message; Get signal strength information. */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}mcm_nw_get_signal_strength_req_msg_v01;

/** @endcond */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Response message; Gets signal strength information. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/

  /* Optional */
  uint8_t gsm_sig_info_valid;  /**< Must be set to TRUE if gsm_sig_info is being passed. */
  mcm_nw_gsm_sig_info_t_v01 gsm_sig_info;
  /**<   GSM signal information. */

  /* Optional */
  uint8_t wcdma_sig_info_valid;  /**< Must be set to TRUE if wcdma_sig_info is being passed. */
  mcm_nw_wcdma_sig_info_t_v01 wcdma_sig_info;
  /**<   WCDMA signal information. */

  /* Optional */
  uint8_t tdscdma_sig_info_valid;  /**< Must be set to TRUE if tdscdma_sig_info is being passed. */
  mcm_nw_tdscdma_sig_info_t_v01 tdscdma_sig_info;
  /**<   TDSCDMA signal information. */

  /* Optional */
  uint8_t lte_sig_info_valid;  /**< Must be set to TRUE if lte_sig_info is being passed. */
  mcm_nw_lte_sig_info_t_v01 lte_sig_info;
  /**<   LTE signal information. */

  /* Optional */
  uint8_t cdma_sig_info_valid;  /**< Must be set to TRUE if cdma_sig_info is being passed. */
  mcm_nw_cdma_sig_info_t_v01 cdma_sig_info;
  /**<   CDMA signal information. */

  /* Optional */
  uint8_t hdr_sig_info_valid;  /**< Must be set to TRUE if hdr_sig_info is being passed. */
  mcm_nw_hdr_sig_info_t_v01 hdr_sig_info;
  /**<   HDR signal information. */
}mcm_nw_get_signal_strength_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @cond
*/

typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}mcm_nw_get_cell_access_state_req_msg_v01;

/** @endcond */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Response message; Gets the cell access state. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/

  /* Optional */
  uint8_t nw_cell_access_state_valid;  /**< Must be set to TRUE if nw_cell_access_state is being passed. */
  mcm_nw_cell_access_state_t_v01 nw_cell_access_state;
  /**<   Network cell access state. */
}mcm_nw_get_cell_access_state_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_nw_qmi_messages
    @{
  */
/** Request Message; Get NITZ Time information. */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}mcm_nw_get_nitz_time_info_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup mcm_nw_qmi_messages
    @{
  */
/** Response Message; Get NITZ Time information. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/

  /* Optional */
  uint8_t nw_nitz_time_valid;  /**< Must be set to true if nw_nitz_time is being passed */
  mcm_nw_nitz_time_t_v01 nw_nitz_time;

  /* Optional */
  uint8_t abs_time_valid;  /**< Must be set to true if abs_time is being passed */
  uint64_t abs_time;

  /* Optional */
  uint8_t leap_sec_valid;  /**< Must be set to true if leap_sec is being passed */
  int8_t leap_sec;
}mcm_nw_get_nitz_time_info_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Request message; Registers for an indication of events. */
typedef struct {

  /* Optional */
  uint8_t register_voice_registration_event_valid;  /**< Must be set to TRUE if register_voice_registration_event is being passed. */
  uint8_t register_voice_registration_event;
  /**<   Register for a voice registration event. */

  /* Optional */
  uint8_t register_data_registration_event_valid;  /**< Must be set to TRUE if register_data_registration_event is being passed. */
  uint8_t register_data_registration_event;
  /**<   Register for a data registration event. */

  /* Optional */
  uint8_t register_signal_strength_event_valid;  /**< Must be set to TRUE if register_signal_strength_event is being passed. */
  uint8_t register_signal_strength_event;
  /**<   Register for a signal strength event. */

  /* Optional */
  uint8_t register_cell_access_state_change_event_valid;  /**< Must be set to TRUE if register_cell_access_state_change_event is being passed. */
  uint8_t register_cell_access_state_change_event;
  /**<   Register for a cell access state change event. */

  /* Optional */
  uint8_t register_nitz_time_update_event_valid;  /**< Must be set to true if register_nitz_time_update_event is being passed */
  uint8_t register_nitz_time_update_event;
  /**<   Register for a nitz time update event. */
}mcm_nw_event_register_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Response message; Registers for an indication of events. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/
}mcm_nw_event_register_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Indication message; Indication for the corresponding registered event
               MCM_NW_VOICE_REGISTRATION_EV. */
typedef struct {

  /* Optional */
  uint8_t voice_registration_valid;  /**< Must be set to TRUE if voice_registration is being passed. */
  mcm_nw_common_registration_t_v01 voice_registration;
  /**<   Voice registration. */

  /* Optional */
  uint8_t voice_registration_details_3gpp_valid;  /**< Must be set to TRUE if voice_registration_details_3gpp is being passed. */
  mcm_nw_3gpp_registration_t_v01 voice_registration_details_3gpp;
  /**<   Voice registration details for 3GPP. */

  /* Optional */
  uint8_t voice_registration_details_3gpp2_valid;  /**< Must be set to TRUE if voice_registration_details_3gpp2 is being passed. */
  mcm_nw_3gpp2_registration_t_v01 voice_registration_details_3gpp2;
  /**<   Voice registration details for 3GPP2. */
}mcm_nw_voice_registration_event_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Indication message; Indication for the corresponding registered event
               MCM_NW_DATA_REGISTRATION_EV. */
typedef struct {

  /* Optional */
  uint8_t data_registration_valid;  /**< Must be set to TRUE if data_registration is being passed. */
  mcm_nw_common_registration_t_v01 data_registration;
  /**<   Data registration. */

  /* Optional */
  uint8_t data_registration_details_3gpp_valid;  /**< Must be set to TRUE if data_registration_details_3gpp is being passed. */
  mcm_nw_3gpp_registration_t_v01 data_registration_details_3gpp;
  /**<   Data registration details for 3GPP. */

  /* Optional */
  uint8_t data_registration_details_3gpp2_valid;  /**< Must be set to TRUE if data_registration_details_3gpp2 is being passed. */
  mcm_nw_3gpp2_registration_t_v01 data_registration_details_3gpp2;
  /**<   Data registration details for 3GPP2. */
}mcm_nw_data_registration_event_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Indication message; Indication for the corresponding registered event
               MCM_NW_SIGNAL_STRENGTH_EV. */
typedef struct {

  /* Optional */
  uint8_t gsm_sig_info_valid;  /**< Must be set to TRUE if gsm_sig_info is being passed. */
  mcm_nw_gsm_sig_info_t_v01 gsm_sig_info;
  /**<   GSM singal information. */

  /* Optional */
  uint8_t wcdma_sig_info_valid;  /**< Must be set to TRUE if wcdma_sig_info is being passed. */
  mcm_nw_wcdma_sig_info_t_v01 wcdma_sig_info;
  /**<   WCDMA singal information. */

  /* Optional */
  uint8_t tdscdma_sig_info_valid;  /**< Must be set to TRUE if tdscdma_sig_info is being passed. */
  mcm_nw_tdscdma_sig_info_t_v01 tdscdma_sig_info;
  /**<   TDSCDMA singal information. */

  /* Optional */
  uint8_t lte_sig_info_valid;  /**< Must be set to TRUE if lte_sig_info is being passed. */
  mcm_nw_lte_sig_info_t_v01 lte_sig_info;
  /**<   LTE singal information. */

  /* Optional */
  uint8_t cdma_sig_info_valid;  /**< Must be set to TRUE if cdma_sig_info is being passed. */
  mcm_nw_cdma_sig_info_t_v01 cdma_sig_info;
  /**<   CDMA singal information. */

  /* Optional */
  uint8_t hdr_sig_info_valid;  /**< Must be set to TRUE if hdr_sig_info is being passed. */
  mcm_nw_hdr_sig_info_t_v01 hdr_sig_info;
  /**<   HDR singal information. */
}mcm_nw_signal_strength_event_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_nw_messages
    @{
  */
/** Indication message; Indication for a change in the cell access state, e.g., emergency only, CS call only. */
typedef struct {

  /* Mandatory */
  mcm_nw_cell_access_state_t_v01 nw_cell_access_state;
  /**<   Network cell access state. */
}mcm_nw_cell_access_state_change_event_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_nw_qmi_messages
    @{
  */
/** Indication Message; Indication to update NITZ time. */
typedef struct {

  /* Optional */
  uint8_t nw_nitz_time_valid;  /**< Must be set to true if nw_nitz_time is being passed */
  mcm_nw_nitz_time_t_v01 nw_nitz_time;
  /**<   NITZ Time update. */

  /* Optional */
  uint8_t abs_time_valid;  /**< Must be set to true if abs_time is being passed */
  uint64_t abs_time;

  /* Optional */
  uint8_t leap_sec_valid;  /**< Must be set to true if leap_sec is being passed */
  int8_t leap_sec;
}mcm_nw_nitz_time_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_nw_msg_ids
    @{
  */
#define MCM_NW_SET_CONFIG_REQ_V01 0x0500
#define MCM_NW_SET_CONFIG_RESP_V01 0x0500
#define MCM_NW_GET_CONFIG_REQ_V01 0x0501
#define MCM_NW_GET_CONFIG_RESP_V01 0x0501
#define MCM_NW_GET_REGISTRATION_STATUS_REQ_V01 0x0502
#define MCM_NW_GET_REGISTRATION_STATUS_RESP_V01 0x0502
#define MCM_NW_SCAN_REQ_V01 0x0503
#define MCM_NW_SCAN_RESP_V01 0x0503
#define MCM_NW_GET_OPERATOR_NAME_REQ_V01 0x0504
#define MCM_NW_GET_OPERATOR_NAME_RESP_V01 0x0504
#define MCM_NW_SCREEN_ON_OFF_REQ_V01 0x0505
#define MCM_NW_SCREEN_ON_OFF_RESP_V01 0x0505
#define MCM_NW_SELECTION_REQ_V01 0x0506
#define MCM_NW_SELECTION_RESP_V01 0x0506
#define MCM_NW_GET_SIGNAL_STRENGTH_REQ_V01 0x0507
#define MCM_NW_GET_SIGNAL_STRENGTH_RESP_V01 0x0507
#define MCM_NW_GET_CELL_ACCESS_STATE_REQ_V01 0x0508
#define MCM_NW_GET_CELL_ACCESS_STATE_RESP_V01 0x0508
#define MCM_NW_GET_NITZ_TIME_INFO_REQ_V01 0x0509
#define MCM_NW_GET_NITZ_TIME_INFO_RESP_V01 0x0509
#define MCM_NW_EVENT_REGISTER_REQ_V01 0x050A
#define MCM_NW_EVENT_REGISTER_RESP_V01 0x050A
#define MCM_NW_VOICE_REGISTRATION_EVENT_IND_V01 0x050B
#define MCM_NW_DATA_REGISTRATION_EVENT_IND_V01 0x050C
#define MCM_NW_SIGNAL_STRENGTH_EVENT_IND_V01 0x050D
#define MCM_NW_CELL_ACCESS_STATE_CHANGE_EVENT_IND_V01 0x050E
#define MCM_NW_NITZ_TIME_IND_V01 0x050F




/**
    @}
  */


#ifdef __cplusplus
}
#endif
#endif

