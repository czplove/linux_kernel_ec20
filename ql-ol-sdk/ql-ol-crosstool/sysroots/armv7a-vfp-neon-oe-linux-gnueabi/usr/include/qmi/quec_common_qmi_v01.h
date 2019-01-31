#ifndef QUEC_COMMON_QMI_SERVICE_01_H
#define QUEC_COMMON_QMI_SERVICE_01_H
/**
  @file quec_common_qmi_v01.h

  @brief This is the public header file which defines the quec_common_qmi service Data structures.

  This header file defines the types and structures that were defined in
  quec_common_qmi. It contains the constant values defined, enums, structures,
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
  

  
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
 *THIS IS AN AUTO GENERATED FILE. DO NOT ALTER IN ANY WAY
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* This file was generated with Tool version 6.14.7 
   It was generated on: Wed Aug 15 2018 (Spin 0)
   From IDL File: quec_common_qmi_v01.idl */

/** @defgroup quec_common_qmi_qmi_consts Constant values defined in the IDL */
/** @defgroup quec_common_qmi_qmi_msg_ids Constant values for QMI message IDs */
/** @defgroup quec_common_qmi_qmi_enums Enumerated types used in QMI messages */
/** @defgroup quec_common_qmi_qmi_messages Structures sent as QMI messages */
/** @defgroup quec_common_qmi_qmi_aggregates Aggregate types used in QMI messages */
/** @defgroup quec_common_qmi_qmi_accessor Accessor for QMI service object */
/** @defgroup quec_common_qmi_qmi_version Constant values for versioning information */

#include <stdint.h>
#include "qmi_idl_lib.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup quec_common_qmi_qmi_version
    @{
  */
/** Major Version Number of the IDL used to generate this file */
#define QUEC_COMMON_QMI_V01_IDL_MAJOR_VERS 0x01
/** Revision Number of the IDL used to generate this file */
#define QUEC_COMMON_QMI_V01_IDL_MINOR_VERS 0x01
/** Major Version Number of the qmi_idl_compiler used to generate this file */
#define QUEC_COMMON_QMI_V01_IDL_TOOL_VERS 0x06
/** Maximum Defined Message ID */
#define QUEC_COMMON_QMI_V01_MAX_MESSAGE_ID 0x0013
/**
    @}
  */


/** @addtogroup quec_common_qmi_qmi_consts
    @{
  */

/**     */
#define QUEC_COMMON_QMI_SERVICE_V01 227
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_enums
    @{
  */
typedef enum {
  QUEC_RESULT_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  QUEC_RESULT_SUCCESS_V01 = 0, /**<  Success. */
  QUEC_RESULT_FAILURE_V01 = 1, /**<  Failure. */
  QUEC_RESULT_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}quec_result_t_v01;
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_enums
    @{
  */
typedef enum {
  QUEC_ERROR_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  QUEC_SUCCESS_V01 = 0, /**<  Success. */
  QUEC_SUCCESS_CONDITIONAL_SUCCESS_V01 = 1, /**<  Conditional success. */
  QUEC_ERROR_QUEC_SERVICES_NOT_AVAILABLE_V01 = 2, /**<  QUEC services not available. */
  QUEC_ERROR_GENERIC_V01 = 3, /**<  Generic error. */
  QUEC_ERROR_BADPARM_V01 = 4, /**<  Bad parameter. */
  QUEC_ERROR_MEMORY_V01 = 5, /**<  Memory error. */
  QUEC_ERROR_INVALID_STATE_V01 = 6, /**<  Invalid state. */
  QUEC_ERROR_MALFORMED_MSG_V01 = 7, /**<  Malformed message. */
  QUEC_ERROR_NO_MEMORY_V01 = 8, /**<  No memory. */
  QUEC_ERROR_INTERNAL_V01 = 9, /**<  Internal error. */
  QUEC_ERROR_ABORTED_V01 = 10, /**<  Action was aborted. */
  QUEC_ERROR_CLIENT_IDS_EXHAUSTED_V01 = 11, /**<  Client IDs have been exhausted. */
  QUEC_ERROR_UNABORTABLE_TRANSACTION_V01 = 12, /**<  Unabortable transaction. */
  QUEC_ERROR_INVALID_CLIENT_ID_V01 = 13, /**<  Invalid client ID. */
  QUEC_ERROR_NO_THRESHOLDS_V01 = 14, /**<  No thresholds. */
  QUEC_ERROR_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}quec_error_t_v01;
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_aggregates
    @{
  */
typedef struct {

  quec_result_t_v01 result;
  /**<   Result code:
                            - QUEC_RESULT_SUCCESS
                            - QUEC_RESULT_FAILURE
                          */

  quec_error_t_v01 error;
  /**<   Error code. Possible error code values are
                            described in the error codes section of each
                            message definition.
                          */
}quec_response_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}quec_common_qmi_get_usb_id_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t pid;

  /* Mandatory */
  uint32_t vid;
}quec_common_qmi_get_usb_id_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t pid;

  /* Mandatory */
  uint32_t vid;
}quec_common_qmi_get_usb_id_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}quec_common_qmi_get_des_gain_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t value;
}quec_common_qmi_get_des_gain_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t value;
}quec_common_qmi_get_des_gain_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}quec_common_qmi_get_5616_power_ctl_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t value;
}quec_common_qmi_get_5616_power_ctl_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t value;
}quec_common_qmi_get_5616_power_ctl_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}quec_common_qmi_get_audio_mod_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t value;
}quec_common_qmi_get_audio_mod_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t value;
}quec_common_qmi_get_audio_mod_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t gpio;

  uint32_t status;

  uint32_t on_time;

  uint32_t off_time;
}gpio_on_off_status_v01;  /* Type */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}quec_common_qmi_get_gpio_status_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  gpio_on_off_status_v01 gpio_status;
}quec_common_qmi_get_gpio_status_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  gpio_on_off_status_v01 gpio_status;
}quec_common_qmi_get_gpio_status_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}quec_common_qmi_get_usb_early_enable_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint8_t enable;
}quec_common_qmi_get_usb_early_enable_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint8_t enable;
}quec_common_qmi_get_usb_early_enable_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}quec_common_qmi_get_usb_function_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint8_t function_mask;
}quec_common_qmi_get_usb_function_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint8_t function_mask;
}quec_common_qmi_get_usb_function_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}quec_common_qmi_get_sclk_value_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t sclk_value;
}quec_common_qmi_get_sclk_value_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t sclk_value;
}quec_common_qmi_get_sclk_value_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}quec_common_qmi_get_usb_net_value_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t usbnet_value;
}quec_common_qmi_get_usb_net_value_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t usbnet_value;
}quec_common_qmi_get_usb_net_value_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t timer_id;
}quec_common_qmi_register_timer_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t result;
}quec_common_qmi_register_timer_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t timer_id;

  /* Mandatory */
  uint32_t interval;

  /* Mandatory */
  uint32_t auto_repeat;
}quec_common_qmi_start_timer_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t result;
}quec_common_qmi_start_timer_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t timer_id;
}quec_common_qmi_stop_timer_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t result;
}quec_common_qmi_stop_timer_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t timer_id;
}quec_common_qmi_timer_expire_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t forbid_ind;
}quec_common_qmi_forbid_ind_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t result;
}quec_common_qmi_forbid_ind_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}quec_common_qmi_get_pad_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t pad_flag;
}quec_common_qmi_get_pad_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t pad_flag;
}quec_common_qmi_get_pad_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}quec_common_qmi_wificfg_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint8_t mac[6];
}quec_common_qmi_wificfg_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint8_t mac[6];
}quec_common_qmi_wificfg_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint8_t call_id;
}quec_common_qmi_ecall_hangup_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  quec_response_t_v01 resp;
}quec_common_qmi_ecall_hangup_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}quec_common_qmi_set_ecall_config_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  quec_response_t_v01 resp;
}quec_common_qmi_set_ecall_config_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t thermal_limit_rates_flag;
}quec_common_qmi_get_thermal_limit_rates_flag_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t thermal_limit_rates_flag;
}quec_common_qmi_get_thermal_limit_rates_flag_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup quec_common_qmi_qmi_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint32_t thermal_limit_rates_flag;
}quec_common_qmi_get_thermal_limit_rates_flag_ind_msg_v01;  /* Message */
/**
    @}
  */

/* Conditional compilation tags for message removal */ 

/*Service Message Definition*/
/** @addtogroup quec_common_qmi_qmi_msg_ids
    @{
  */
#define QMI_COMM_QMI_GET_USB_ID_REQ_V01 0x0001
#define QMI_COMM_QMI_GET_USB_ID_RESP_V01 0x0001
#define QMI_COMM_QMI_GET_USB_ID_IND_V01 0x0001
#define QMI_COMM_QMI_GET_DES_GAIN_REQ_V01 0x0002
#define QMI_COMM_QMI_GET_DES_GAIN_RESP_V01 0x0002
#define QMI_COMM_QMI_GET_DES_GAIN_IND_V01 0x0002
#define QMI_COMM_QMI_GET_5616_POWER_CTL_REQ_V01 0x0003
#define QMI_COMM_QMI_GET_5616_POWER_CTL_RESP_V01 0x0003
#define QMI_COMM_QMI_GET_5616_POWER_CTL_IND_V01 0x0003
#define QMI_COMM_QMI_GET_AUDIO_MOD_REQ_V01 0x0004
#define QMI_COMM_QMI_GET_AUDIO_MOD_RESP_V01 0x0004
#define QMI_COMM_QMI_GET_AUDIO_MOD_IND_V01 0x0004
#define QMI_COMM_QMI_GET_GPIO_STATUS_REQ_V01 0x0005
#define QMI_COMM_QMI_GET_GPIO_STATUS_RESP_V01 0x0005
#define QMI_COMM_QMI_GET_GPIO_STATUS_IND_V01 0x0005
#define QMI_COMM_QMI_GET_USB_EARLY_ENABLE_REQ_V01 0x0006
#define QMI_COMM_QMI_GET_USB_EARLY_ENABLE_RESP_V01 0x0006
#define QMI_COMM_QMI_GET_USB_EARLY_ENABLE_IND_V01 0x0006
#define QMI_COMM_QMI_GET_USB_FUNCTION_REQ_V01 0x0007
#define QMI_COMM_QMI_GET_USB_FUNCTION_RESP_V01 0x0007
#define QMI_COMM_QMI_GET_USB_FUNCTION_IND_V01 0x0007
#define QMI_COMM_QMI_GET_SCLK_VALUE_REQ_V01 0x0008
#define QMI_COMM_QMI_GET_SCLK_VALUE_RESP_V01 0x0008
#define QMI_COMM_QMI_GET_SCLK_VALUE_IND_V01 0x0008
#define QMI_COMM_QMI_GET_USB_NET_VALUE_REQ_V01 0x0009
#define QMI_COMM_QMI_GET_USB_NET_VALUE_RESP_V01 0x0009
#define QMI_COMM_QMI_GET_USB_NET_VALUE_IND_V01 0x0009
#define QMI_COMM_QMI_REG_TIMER_REQ_V01 0x000A
#define QMI_COMM_QMI_REG_TIMER_RSP_V01 0x000A
#define QMI_COMM_QMI_START_TIMER_REQ_V01 0x000B
#define QMI_COMM_QMI_START_TIMER_RSP_V01 0x000B
#define QMI_COMM_QMI_STOP_TIMER_REQ_V01 0x000C
#define QMI_COMM_QMI_STOP_TIMER_RSP_V01 0x000C
#define QMI_COMM_QMI_TIMER_EXP_IND_V01 0x000D
#define QMI_COMM_QMI_FORBID_IND_REQ_V01 0x000E
#define QMI_COMM_QMI_FORBID_IND_RESP_V01 0x000E
#define QMI_COMM_QMI_GET_PAD_REQ_V01 0x000F
#define QMI_COMM_QMI_GET_PAD_RESP_V01 0x000F
#define QMI_COMM_QMI_GET_PAD_IND_V01 0x000F
#define QMI_COMM_QMI_WIFICFG_REQ_V01 0x0010
#define QMI_COMM_QMI_WIFICFG_RESP_V01 0x0010
#define QMI_COMM_QMI_WIFICFG_IND_V01 0x0010
#define QUEC_COMMON_QMI_ECALL_HANGUP_REQ_V01 0x0011
#define QUEC_COMMON_QMI_ECALL_HANGUP_RESP_V01 0x0011
#define QUEC_COMMON_QMI_SET_ECALL_CONFIG_REQ_V01 0x0012
#define QUEC_COMMON_QMI_SET_ECALL_CONFIG_RESP_V01 0x0012
#define QMI_COMM_QMI_GET_THERMAL_LIMIT_RATES_FLAG_REQ_V01 0x0013
#define QMI_COMM_QMI_GET_THERMAL_LIMIT_RATES_FLAG_RESP_V01 0x0013
#define QMI_COMM_QMI_GET_THERMAL_LIMIT_RATES_FLAG_IND_V01 0x0013
/**
    @}
  */

/* Service Object Accessor */
/** @addtogroup wms_qmi_accessor
    @{
  */
/** This function is used internally by the autogenerated code.  Clients should use the
   macro quec_common_qmi_get_service_object_v01( ) that takes in no arguments. */
qmi_idl_service_object_type quec_common_qmi_get_service_object_internal_v01
 ( int32_t idl_maj_version, int32_t idl_min_version, int32_t library_version );

/** This macro should be used to get the service object */
#define quec_common_qmi_get_service_object_v01( ) \
          quec_common_qmi_get_service_object_internal_v01( \
            QUEC_COMMON_QMI_V01_IDL_MAJOR_VERS, QUEC_COMMON_QMI_V01_IDL_MINOR_VERS, \
            QUEC_COMMON_QMI_V01_IDL_TOOL_VERS )
/**
    @}
  */


#ifdef __cplusplus
}
#endif
#endif

