#ifndef MCM_COMMON_SERVICE_01_H
#define MCM_COMMON_SERVICE_01_H
/**
  @file mcm_common_v01.h

  @brief This is the public header file which defines the mcm_common service Data structures.

  This header file defines the types and structures that were defined in
  mcm_common. It contains the constant values defined, enums, structures,
  messages, and service message IDs (in that order) Structures that were
  defined in the IDL as messages contain mandatory elements, optional
  elements, a combination of mandatory and optional elements (mandatory
  always come before optionals in the structure), or nothing (null message)

  An optional element in a message is preceded by a uint8_t value that must be
  set to true if the element is going to be included. When decoding a received
  message, the uint8_t values will be set to true or false by the decode
  routine, and should be checked before accessing the values that they
  correspond to.

  Variable sized arrays are defined as static sized arrays with an unsigned
  integer (32 bit) preceding it that must be set to the number of elements
  in the array that are valid. For Example:

  uint32_t test_opaque_len;
  uint8_t test_opaque[16];

  If only 4 elements are added to test_opaque[] then test_opaque_len must be
  set to 4 before sending the message.  When decoding, the _len value is set
  by the decode routine and should be checked so that the correct number of
  elements in the array will be accessed.

*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013 Qualcomm Technologies, Inc.
   All rights reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.


  $Header$
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/** @defgroup mcm_common_consts Constant values defined in the IDL */
/** @defgroup mcm_common_msg_ids Constant values for message IDs */
/** @defgroup mcm_common_enums Enumerated types used in messages */
/** @defgroup mcm_common_messages Structures sent as messages */
/** @defgroup mcm_common_aggregates Aggregate types used in messages */
/** @defgroup mcm_common_accessor Accessor for service object */
/** @defgroup mcm_common_version Constant values for versioning information */

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup mcm_common_enums
    @{
  */
typedef enum {
  MCM_CMD_STATE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_CMD_STATE_NONE_V01 = 0x0000, /* Command state -- None. */
  MCM_CMD_STATE_PENDING_V01 = 0x0001, /* Command state -- Pending. */
  MCM_CMD_STATE_SUCCESS_V01 = 0x0002, /* Command state -- Success. */
  MCM_CMD_STATE_ERROR_V01 = 0x0003, /* Command state -- Error. */
  MCM_CMD_STATE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_cmd_state_t_v01;
/**
    @}
  */

/** @addtogroup mcm_common_enums
    @{
  */
typedef enum {
  MCM_TECH_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_TECH_NONE_V01 = 0x0000, /* Network technology -- None.  */
  MCM_TECH_3GPP_V01 = 0x0001, /* Network technology -- 3GPP.  */
  MCM_TECH_3GPP2_V01 = 0x0002, /* Network technology -- 3GPP2.  */
  MCM_TECH_IMS_V01 = 0x0003, /* Network technology -- IMS.  */
  MCM_TECH_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_tech_t_v01;
/**
    @}
  */

/** @addtogroup mcm_common_enums
    @{
  */
typedef enum {
  MCM_DOMAIN_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_DOMAIN_NONE_V01 = 0x0000, /* Network call domain -- None.  */
  MCM_DOMAIN_AUTO_V01 = 0x0001, /* Network call domain -- Auto.  */
  MCM_DOMAIN_BOTH_V01 = 0x0001, /* Network call domain -- Both CS and PS.  */
  MCM_DOMAIN_CS_V01 = 0x0002, /* Network call domain -- CS.  */
  MCM_DOMAIN_PS_V01 = 0x0003, /* Network call domain -- PS.  */
  MCM_DOMAIN_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_domain_t_v01;
/**
    @}
  */

/** @addtogroup mcm_common_enums
    @{
  */
typedef enum {
  MCM_RESULT_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_RESULT_SUCCESS_V01 = 0, /**<  Success.   */
  MCM_RESULT_FAILURE_V01 = 1, /**<  Failure. */
  MCM_RESULT_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_result_t_v01;
/**
    @}
  */

/** @addtogroup mcm_common_enums
    @{
  */
typedef enum {
  MCM_ERROR_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_SUCCESS_V01 = 0, /**<  Success. */
  MCM_SUCCESS_CONDITIONAL_SUCCESS_V01 = 1, /**<  Conditional success. */
  MCM_ERROR_MCM_SERVICES_NOT_AVAILABLE_V01 = 2, /**<  MCM services not available. */
  MCM_ERROR_GENERIC_V01 = 3, /**<  Generic error. */
  MCM_ERROR_BADPARM_V01 = 4, /**<  Bad parameter. */
  MCM_ERROR_MEMORY_V01 = 5, /**<  Memory error. */
  MCM_ERROR_INVALID_STATE_V01 = 6, /**<  Invalid state. */
  MCM_ERROR_MALFORMED_MSG_V01 = 7, /**<  Malformed message. */
  MCM_ERROR_NO_MEMORY_V01 = 8, /**<  No memory. */
  MCM_ERROR_INTERNAL_V01 = 9, /**<  Internal error. */
  MCM_ERROR_ABORTED_V01 = 10, /**<  Action was aborted. */
  MCM_ERROR_CLIENT_IDS_EXHAUSTED_V01 = 11, /**<  Client IDs have been exhausted. */
  MCM_ERROR_UNABORTABLE_TRANSACTION_V01 = 12, /**<  Unabortable transaction. */
  MCM_ERROR_INVALID_CLIENT_ID_V01 = 13, /**<  Invalid client ID. */
  MCM_ERROR_NO_THRESHOLDS_V01 = 14, /**<  No thresholds. */
  MCM_ERROR_INVALID_HANDLE_V01 = 15, /**<  Invalid handle. */
  MCM_ERROR_INVALID_PROFILE_V01 = 16, /**<  Invalid profile. */
  MCM_ERROR_INVALID_PINID_V01 = 17, /**<  Invalid PIN ID. */
  MCM_ERROR_INCORRECT_PIN_V01 = 18, /**<  Incorrect PIN. */
  MCM_ERROR_NO_NETWORK_FOUND_V01 = 19, /**<  No network found. */
  MCM_ERROR_CALL_FAILED_V01 = 20, /**<  Call failed. */
  MCM_ERROR_OUT_OF_CALL_V01 = 21, /**<  Out of call. */
  MCM_ERROR_NOT_PROVISIONED_V01 = 22, /**<  Not provisioned. */
  MCM_ERROR_MISSING_ARG_V01 = 23, /**<  Missing argument. */
  MCM_ERROR_ARG_TOO_LONG_V01 = 24, /**<  Argument is too long. */
  MCM_ERROR_INVALID_TX_ID_V01 = 25, /**<  Invalid Tx ID. */
  MCM_ERROR_DEVICE_IN_USE_V01 = 26, /**<  Device is in use. */
  MCM_ERROR_OP_NETWORK_UNSUPPORTED_V01 = 27, /**<  OP network is not supported. */
  MCM_ERROR_OP_DEVICE_UNSUPPORTED_V01 = 28, /**<  OP device is not supported. */
  MCM_ERROR_NO_EFFECT_V01 = 29, /**<  No effect. */
  MCM_ERROR_NO_FREE_PROFILE_V01 = 30, /**<  No free profile. */
  MCM_ERROR_INVALID_PDP_TYPE_V01 = 31, /**<  Invalid PDP type. */
  MCM_ERROR_INVALID_TECH_PREF_V01 = 32, /**<  Invalid technical preference. */
  MCM_ERROR_INVALID_PROFILE_TYPE_V01 = 33, /**<  Invalid profile type. */
  MCM_ERROR_INVALID_SERVICE_TYPE_V01 = 34, /**<  Invalid service type. */
  MCM_ERROR_INVALID_REGISTER_ACTION_V01 = 35, /**<  Invalid register action. */
  MCM_ERROR_INVALID_PS_ATTACH_ACTION_V01 = 36, /**<  Invalid PS attach action. */
  MCM_ERROR_AUTHENTICATION_FAILED_V01 = 37, /**<  Authentication failed. */
  MCM_ERROR_PIN_BLOCKED_V01 = 38, /**<  PIN is blocked. */
  MCM_ERROR_PIN_PERM_BLOCKED_V01 = 39, /**<  PIN is permanently blocked. */
  MCM_ERROR_SIM_NOT_INITIALIZED_V01 = 40, /**<  SIM is not initialized. */
  MCM_ERROR_MAX_QOS_REQUESTS_IN_USE_V01 = 41, /**<  Maximum QoS requests are in use. */
  MCM_ERROR_INCORRECT_FLOW_FILTER_V01 = 42, /**<  Incorrect flow filter. */
  MCM_ERROR_NETWORK_QOS_UNAWARE_V01 = 43, /**<  Network QoS is unaware. */
  MCM_ERROR_INVALID_ID_V01 = 44, /**<  Invalid ID. */
  MCM_ERROR_INVALID_QOS_ID_V01 = 45, /**<  Invalid QoS ID. */
  MCM_ERROR_REQUESTED_NUM_UNSUPPORTED_V01 = 46, /**<  Requested number is not supported. */
  MCM_ERROR_INTERFACE_NOT_FOUND_V01 = 47, /**<  Interface was not found. */
  MCM_ERROR_FLOW_SUSPENDED_V01 = 48, /**<  Flow is suspended. */
  MCM_ERROR_INVALID_DATA_FORMAT_V01 = 49, /**<  Invalid data format. */
  MCM_ERROR_GENERAL_V01 = 50, /**<  General error. */
  MCM_ERROR_UNKNOWN_V01 = 51, /**<  Unknown error. */
  MCM_ERROR_INVALID_ARG_V01 = 52, /**<  Invalid argument. */
  MCM_ERROR_INVALID_INDEX_V01 = 53, /**<  Invalid index. */
  MCM_ERROR_NO_ENTRY_V01 = 54, /**<  No entry. */
  MCM_ERROR_DEVICE_STORAGE_FULL_V01 = 55, /**<  Device storage is full. */
  MCM_ERROR_DEVICE_NOT_READY_V01 = 56, /**<  Device is not ready. */
  MCM_ERROR_NETWORK_NOT_READY_V01 = 57, /**<  Network is not ready. */
  MCM_ERROR_CAUSE_CODE_V01 = 58, /**<  Cause code error. */
  MCM_ERROR_MESSAGE_NOT_SENT_V01 = 59, /**<  Message was not sent. */
  MCM_ERROR_MESSAGE_DELIVERY_FAILURE_V01 = 60, /**<  Message delivery failure. */
  MCM_ERROR_INVALID_MESSAGE_ID_V01 = 61, /**<  Invalid message ID. */
  MCM_ERROR_ENCODING_V01 = 62, /**<  Encoding error. */
  MCM_ERROR_AUTHENTICATION_LOCK_V01 = 63, /**<  Authentication lock error. */
  MCM_ERROR_INVALID_TRANSITION_V01 = 64, /**<  Invalid transition. */
  MCM_ERROR_NOT_A_MCAST_IFACE_V01 = 65, /**<  Not an MCast interface. */
  MCM_ERROR_MAX_MCAST_REQUESTS_IN_USE_V01 = 66, /**<  Maximum MCast requests are in use. */
  MCM_ERROR_INVALID_MCAST_HANDLE_V01 = 67, /**<  Invalid MCast handle. */
  MCM_ERROR_INVALID_IP_FAMILY_PREF_V01 = 68, /**<  Invalid IP family preference. */
  MCM_ERROR_SESSION_INACTIVE_V01 = 69, /**<  Session is inactive. */
  MCM_ERROR_SESSION_INVALID_V01 = 70, /**<  Session is invalid. */
  MCM_ERROR_SESSION_OWNERSHIP_V01 = 71, /**<  Session ownership error. */
  MCM_ERROR_INSUFFICIENT_RESOURCES_V01 = 72, /**<  Insufficient resources. */
  MCM_ERROR_DISABLED_V01 = 73, /**<  Disabled. */
  MCM_ERROR_INVALID_OPERATION_V01 = 74, /**<  Invalid operation. */
  MCM_ERROR_INVALID_CMD_V01 = 75, /**<  Invalid command. */
  MCM_ERROR_TPDU_TYPE_V01 = 76, /**<  Transfer Protocol data unit type error. */
  MCM_ERROR_SMSC_ADDR_V01 = 77, /**<  Short message service center address error. */
  MCM_ERROR_INFO_UNAVAILABLE_V01 = 78, /**<  Information is not available. */
  MCM_ERROR_SEGMENT_TOO_LONG_V01 = 79, /**<  Segment is too long. */
  MCM_ERROR_SEGMENT_ORDER_V01 = 80, /**<  Segment order error. */
  MCM_ERROR_BUNDLING_NOT_SUPPORTED_V01 = 81, /**<  Bundling is not supported. */
  MCM_ERROR_OP_PARTIAL_FAILURE_V01 = 82, /**<  OP partial failure. */
  MCM_ERROR_POLICY_MISMATCH_V01 = 83, /**<  Policy mismatch. */
  MCM_ERROR_SIM_FILE_NOT_FOUND_V01 = 84, /**<  SIM file was not found. */
  MCM_ERROR_EXTENDED_INTERNAL_V01 = 85, /**<  Extended internal error. */
  MCM_ERROR_ACCESS_DENIED_V01 = 86, /**<  Access is denied. */
  MCM_ERROR_HARDWARE_RESTRICTED_V01 = 87, /**<  Hardware is restricted. */
  MCM_ERROR_ACK_NOT_SENT_V01 = 88, /**<  Acknowledgement was not sent. */
  MCM_ERROR_INJECT_TIMEOUT_V01 = 89, /**<  Inject timeout error. */
  MCM_ERROR_INCOMPATIBLE_STATE_V01 = 90, /**<  Incompatible state. */
  MCM_ERROR_FDN_RESTRICT_V01 = 91, /**<  Fixed dialing number restrict error. */
  MCM_ERROR_SUPS_FAILURE_CAUSE_V01 = 92, /**<  SUPS failure cause. */
  MCM_ERROR_NO_RADIO_V01 = 93, /**<  No radio. */
  MCM_ERROR_NOT_SUPPORTED_V01 = 94, /**<  Not supported. */
  MCM_ERROR_NO_SUBSCRIPTION_V01 = 95, /**<  No subscription. */
  MCM_ERROR_CARD_CALL_CONTROL_FAILED_V01 = 96, /**<  Card call control failed. */
  MCM_ERROR_NETWORK_ABORTED_V01 = 97, /**<  Network was aborted. */
  MCM_ERROR_MSG_BLOCKED_V01 = 98, /**<  Message was blocked. */
  MCM_ERROR_INVALID_SESSION_TYPE_V01 = 99, /**<  Invalid session type. */
  MCM_ERROR_INVALID_PB_TYPE_V01 = 100, /**<  Invalid phonebook type. */
  MCM_ERROR_NO_SIM_V01 = 101, /**<  No SIM was found. */
  MCM_ERROR_PB_NOT_READY_V01 = 102, /**<  Phonebook not ready. */
  MCM_ERROR_PIN_RESTRICTION_V01 = 103, /**<  PIN restriction. */
  MCM_ERROR_PIN2_RESTRICTION_V01 = 104, /**<  PIN2 restriction. */
  MCM_ERROR_PUK_RESTRICTION_V01 = 105, /**<  PIN unlocking key restriction. */
  MCM_ERROR_PUK2_RESTRICTION_V01 = 106, /**<  PIN unlocking key2 restriction. */
  MCM_ERROR_PB_ACCESS_RESTRICTED_V01 = 107, /**<  Phonebook access is restricted. */
  MCM_ERROR_PB_DELETE_IN_PROG_V01 = 108, /**<  Phonebook delete is in progress. */
  MCM_ERROR_PB_TEXT_TOO_LONG_V01 = 109, /**<  Phonebook text is too long. */
  MCM_ERROR_PB_NUMBER_TOO_LONG_V01 = 110, /**<  Phonebook number is too long. */
  MCM_ERROR_PB_HIDDEN_KEY_RESTRICTION_V01 = 111, /**<  Phonebook hidden key restriction. */
  MCM_ERROR_PB_NOT_AVAILABLE_V01 = 112, /**<  Phonebook is not available. */
  MCM_ERROR_DEVICE_MEMORY_ERROR_V01 = 113, /**<  Device memory error. */
  MCM_ERROR_SIM_PIN_BLOCKED_V01 = 114, /**<  SIM PIN is blocked. */
  MCM_ERROR_SIM_PIN_NOT_INITIALIZED_V01 = 115, /**<  SIM PIN is not initialized. */
  MCM_ERROR_SIM_INVALID_PIN_V01 = 116, /**<  SIM PIN is invalid. */
  MCM_ERROR_SIM_INVALID_PERSO_CK_V01 = 117, /**<  SIM invalid personalization CK. */
  MCM_ERROR_SIM_PERSO_BLOCKED_V01 = 118, /**<  SIM personalization blocked. */
  MCM_ERROR_SIM_PERSO_INVALID_DATA_V01 = 119, /**<  SIM personalization contains invalid data. */
  MCM_ERROR_SIM_ACCESS_DENIED_V01 = 120, /**<  SIM access is denied. */
  MCM_ERROR_SIM_INVALID_FILE_PATH_V01 = 121, /**<  SIM file path is invalid. */
  MCM_ERROR_SIM_SERVICE_NOT_SUPPORTED_V01 = 122, /**<  SIM service is not supported. */
  MCM_ERROR_SIM_AUTH_FAIL_V01 = 123, /**<  SIM authorization failure. */
  MCM_ERROR_SIM_PIN_PERM_BLOCKED_V01 = 124, /**<  SIM PIN is permanently blocked. */
  MCM_ERROR_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_error_t_v01;
/**
    @}
  */

/** @addtogroup mcm_common_aggregates
    @{
  */
typedef struct {

  mcm_result_t_v01 result;
  /**<   Result code: \n
         - MCM_RESULT_SUCCESS
         - MCM_RESULT_FAILURE @tablebulletend
                          */

  mcm_error_t_v01 error;
  /**<   Error code. Possible error code values are
                            described in the error codes section of each
                            message definition.
                          */
}mcm_response_t_v01;  /* Type */
/**
    @}
  */



#ifdef __cplusplus
}
#endif
#endif

