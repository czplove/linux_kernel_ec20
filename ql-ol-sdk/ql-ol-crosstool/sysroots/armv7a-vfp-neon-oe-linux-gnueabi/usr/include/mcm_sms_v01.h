#ifndef MCM_SMS_SERVICE_01_H
#define MCM_SMS_SERVICE_01_H
/**
  @file mcm_sms_v01.h

  @brief This is the public header file which defines the mcm_sms service Data structures.

  This header file defines the types and structures that were defined in
  mcm_sms. It contains the constant values defined, enums, structures,
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
   It was generated on: Thu Jun  7 2018 (Spin 0)
   From IDL File: mcm_sms_v01.idl */

/** @defgroup mcm_sms_qmi_consts Constant values defined in the IDL */
/** @defgroup mcm_sms_qmi_msg_ids Constant values for QMI message IDs */
/** @defgroup mcm_sms_qmi_enums Enumerated types used in QMI messages */
/** @defgroup mcm_sms_qmi_messages Structures sent as QMI messages */
/** @defgroup mcm_sms_qmi_aggregates Aggregate types used in QMI messages */
/** @defgroup mcm_sms_qmi_accessor Accessor for QMI service object */
/** @defgroup mcm_sms_qmi_version Constant values for versioning information */

#include <stdint.h>
#include "mcm_common_v01.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup mcm_sms_qmi_version
    @{
  */
/** Major Version Number of the IDL used to generate this file */
#define MCM_SMS_V01_IDL_MAJOR_VERS 0x01
/** Revision Number of the IDL used to generate this file */
#define MCM_SMS_V01_IDL_MINOR_VERS 0x01
/** Major Version Number of the qmi_idl_compiler used to generate this file */
#define MCM_SMS_V01_IDL_TOOL_VERS 0x06

/**
    @}
  */


/** @addtogroup mcm_sms_qmi_consts
    @{
  */

/**  Maximum length of an MO SMS (9*160). */
#define MCM_SMS_MAX_MO_MSG_LENGTH_V01 1440

/**  Maximum length of an SMS. */
#define MCM_SMS_MAX_MT_MSG_LENGTH_V01 160

/**  Maximum string length. */
#define MCM_SMS_MAX_ADDR_LENGTH_V01 252

/**  Maximum string length. */
#define MCM_SMS_MAX_SCA_TYPE_LENGTH_V01 3
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_enums
    @{
  */
typedef enum {
  MCM_SMS_MSG_FORMAT_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_SMS_MSG_FORMAT_GSM_7BIT_V01 = 0, /**<  Message format ASCII text. */
  MCM_SMS_MSG_FORMAT_TEXT_BINARY_DATA_V01 = 1, /**<  Message format binary data. */
  MCM_SMS_MSG_FORMAT_TEXT_UCS2_V01 = 2, /**<  Message format UCS2 text. */
  MCM_SMS_MSG_FORMAT_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_sms_msg_format_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_enums
    @{
  */
typedef enum {
  MCM_SMS_MSG_SIZE_VALIDATION_MODE_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_SMS_MSG_SIZE_VALIDATION_MODE_AUTO_BREAK_V01 = 1, /**<  Message size validation mode;
 Auto-break into 160-byte segments. */
  MCM_SMS_MSG_SIZE_VALIDATION_MODE_NO_AUTO_BREAK_V01 = 2, /**<  Message size validation mode;
 No auto-break. */
  MCM_SMS_MSG_SIZE_VALIDATION_MODE_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_sms_msg_size_validation_mode_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_enums
    @{
  */
typedef enum {
  MCM_SMS_RECEPTION_MODE_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_SMS_RECEPTION_MODE_NO_RECEPTION_V01 = 1, /**<  No reception. */
  MCM_SMS_RECEPTION_MODE_ON_AUTO_CONFIRM_TO_NW_V01 = 2, /**<  Reception on with auto confirm to the network. */
  MCM_SMS_RECEPTION_MODE_ON_WITHOUT_AUTO_CONFIRM_TO_NW_V01 = 3, /**<  Reception on without auto confirm to the network. */
  MCM_SMS_RECEPTION_MODE_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_sms_reception_mode_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_enums
    @{
  */
typedef enum {
  MCM_SMS_MESSAGE_CLASS_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_SMS_MESSAGE_CLASS_0_V01 = 0, /**<  Class 0. */
  MCM_SMS_MESSAGE_CLASS_1_V01 = 1, /**<  Class 1. */
  MCM_SMS_MESSAGE_CLASS_2_V01 = 2, /**<  Class 2. */
  MCM_SMS_MESSAGE_CLASS_3_V01 = 3, /**<  Class 3. */
  MCM_SMS_MESSAGE_CLASS_NONE_V01 = 4, /**<  None. */
  MCM_SMS_MESSAGE_CLASS_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_sms_message_class_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_enums
    @{
  */
typedef enum {
  MCM_CBS_CMAE_CATEGORY_TYPE_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_CBS_CMAE_CATEGORY_INVALID_V01 = -1, 
  MCM_CBS_CMAE_CATEGORY_GEO_V01 = 0, /**<  Geophysical, including landslide. */
  MCM_CBS_CMAE_CATEGORY_MET_V01 = 1, /**<  Meteorological, including flood. */
  MCM_CBS_CATEGORY_SAFETY_V01 = 2, /**<  Safety (general emergency and public safety). */
  MCM_CBS_CMAE_CATEGORY_SECURITY_V01 = 3, /**<  Security (law enforcement, military, homeland,
 and local/private security). */
  MCM_CBS_CMAE_CATEGORY_RESCUE_V01 = 4, /**<  Rescue (rescue and recovery). */
  MCM_CBS_CMAE_CATEGORY_FIRE_V01 = 5, /**<  Fire (fire suppression and rescue). */
  MCM_CBS_CMAE_CATEGORY_HEALTH_V01 = 6, /**<  Health (medical and public health). */
  MCM_CBS_CMAE_CATEGORY_ENV_V01 = 7, /**<  Environment (pollution and other environmental factors). */
  MCM_CBS_CMAE_CATEGORY_TRANSPORT_V01 = 8, /**<  Transport (public and private transportation). */
  MCM_CBS_CMAE_CATEGORY_INFRA_V01 = 9, /**<  Infrastructure (utility, telecommunication, and other
 nontransport infrastructure). */
  MCM_CBS_CMAE_CATEGORY_CBRNE_V01 = 10, /**<  CBRNE (chemical, biological, radiological, nuclear,
 or high-yield explosive thread or attack). */
  MCM_CBS_CMAE_CATEGORY_OTHER_V01 = 11, /**<  Other events. */
  MCM_CBS_CMAE_CATEGORY_TYPE_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_cbs_cmae_category_type_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_enums
    @{
  */
typedef enum {
  MCM_CBS_CMAE_RESPONSE_TYPE_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_CBS_CMAE_RESPONSE_TYPE_INVALID_V01 = -1, 
  MCM_CBS_CMAE_RESPONSE_TYPE_SHELTER_V01 = 0, /**<  Shelter (take shelter in place). */
  MCM_CBS_CMAE_RESPONSE_TYPE_EVACUATE_V01 = 1, /**<  Evacuate (relocate). */
  MCM_CBS_CMAE_RESPONSE_TYPE_PREPARE_V01 = 2, /**<  Prepare (make preparations). */
  MCM_CBS_CMAE_RESPONSE_TYPE_EXECUTE_V01 = 3, /**<  Execute (execute a preplanned activity). */
  MCM_CBS_CMAE_RESPONSE_TYPE_MONITOR_V01 = 4, /**<  Monitor (attend to information sources). */
  MCM_CBS_CMAE_RESPONSE_TYPE_AVOID_V01 = 5, /**<  Avoid (avoid hazards). */
  MCM_CBS_CMAE_RESPONSE_TYPE_ASSESS_V01 = 6, /**<  Assess (evaluate the information in this message). */
  MCM_CBS_CMAE_RESPONSE_TYPE_NONE_V01 = 7, /**<  None (no action recommended). */
  MCM_CBS_CMAE_RESPONSE_TYPE_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_cbs_cmae_response_type_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_enums
    @{
  */
typedef enum {
  MCM_CBS_CMAE_SEVERITY_TYPE_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_CBS_CMAE_SEVERITY_INVALID_V01 = -1, 
  MCM_CBS_CMAE_SEVERITY_EXTREME_V01 = 0, /**<  Extreme (extraodinary threat to life or property). */
  MCM_CBS_CMAE_SEVERITY_SEVERE_V01 = 1, /**<  Severe (significant threat to life or property). */
  MCM_CBS_CMAE_SEVERITY_TYPE_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_cbs_cmae_severity_type_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_enums
    @{
  */
typedef enum {
  MCM_CBS_CMAE_URGENCY_TYPE_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_CBS_CMAE_URGENCY_INVALID_V01 = -1, 
  MCM_CBS_CMAE_URGENCY_IMMEDIATE_V01 = 0, /**<  Immediate (responsive action should be taken immediately).  */
  MCM_CBS_CMAE_URGENCY_EXPECTED_V01 = 1, /**<  Expected (reponsive action should be taken soon, i.e., within the next hour). */
  MCM_CBS_CMAE_URGENCY_TYPE_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_cbs_cmae_urgency_type_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_enums
    @{
  */
typedef enum {
  MCM_CBS_CMAE_CERTAINTY_TYPE_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_CBS_CMAE_CERTAINTY_INVALID_V01 = -1, 
  MCM_CBS_CMAE_CERTAINTY_OBSERVED_V01 = 0, /**<  Observed (determined to have occurred or to be ongoing). */
  MCM_CBS_CMAE_CERTAINTY_LIKELY_V01 = 1, /**<  Likely (probabiltiy > ~50%). */
  MCM_CBS_CMAE_CERTAINTY_TYPE_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_cbs_cmae_certainty_type_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_aggregates
    @{
  */
typedef struct {

  uint8_t year;
  /**<   Year -- Range 00 to 99 (UTC).*/

  uint8_t month;
  /**<   Month -- Range 1 to 12 (UTC).*/

  uint8_t day;
  /**<   Day -- Range 1 to 31 (UTC).*/

  uint8_t hours;
  /**<   Hour -- Range 0 to 23 (UTC).*/

  uint8_t minutes;
  /**<   Minutes -- Range 0 to 59 (UTC).*/

  uint8_t seconds;
  /**<   Seconds -- Range 0 to 59 (UTC).*/
}mcm_cbs_cmae_expire_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_enums
    @{
  */
typedef enum {
  MCM_SMS_STORAGE_TYPE_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_SMS_STORAGE_TYPE_NONE_V01 = -1, /**<  Message no need to store. */
  MCM_SMS_STORAGE_TYPE_UIM_V01 = 0, /**<  Message store to UIM. */
  MCM_SMS_STORAGE_TYPE_NV_V01 = 1, /**<  Message store to NV. */
  MCM_SMS_STORAGE_TYPE_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_sms_storage_type_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_enums
    @{
  */
typedef enum {
  MCM_SMS_MODE_TYPE_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_SMS_MESSAGE_MODE_CDMA_V01 = 0, /**<  Message type CDMA */
  MCM_SMS_MESSAGE_MODE_GW_V01 = 1, /**<  Message type GW. */
  MCM_SMS_MODE_TYPE_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_sms_mode_type_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_aggregates
    @{
  */
typedef struct {

  mcm_sms_storage_type_t_v01 storage_type;
  /**<   Message storage type.*/

  uint32_t storage_index;
  /**<   Message storage index.*/
}mcm_sms_storage_info_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t message_content_len;  /**< Must be set to # of elements in message_content */
  char message_content[MCM_SMS_MAX_MT_MSG_LENGTH_V01];
  /**<   Message content.*/
}mcm_cbs_cmae_record_type_0_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_aggregates
    @{
  */
typedef struct {

  mcm_cbs_cmae_category_type_t_v01 category;
  /**<   Category of the CMAS alert.*/

  mcm_cbs_cmae_response_type_t_v01 response;
  /**<   Response indicated for the CMAS alert.*/

  mcm_cbs_cmae_severity_type_t_v01 severity;
  /**<   Severity of the CMAS alert.*/

  mcm_cbs_cmae_urgency_type_t_v01 urgency;
  /**<   Urgency of the CMAS alert.*/

  mcm_cbs_cmae_certainty_type_t_v01 certainty;
  /**<   Certainty of the CMAS alert.*/
}mcm_cbs_cmae_record_type_1_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_aggregates
    @{
  */
typedef struct {

  uint16_t id;
  /**<   Identification of the message.*/

  uint8_t alert_handling;
  /**<   Indicates whether this alert message requires special handling.*/

  mcm_cbs_cmae_expire_t_v01 expire;
  /**<   Expiration date and time of the CMAS alert.*/

  uint8_t language;
  /**<   Language used for the message content.*/
}mcm_cbs_cmae_record_type_2_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_aggregates
    @{
  */
typedef struct {

  uint8_t year;
  /**<     Year.*/

  uint8_t month;
  /**<     Month.*/

  uint8_t day;
  /**<     Day.*/

  uint8_t hours;
  /**<     Hour.*/

  uint8_t minutes;
  /**<     Minutes.*/

  uint8_t seconds;
  /**<     Seconds.*/

  uint8_t timezone;
  /**<     Timezone.*/
}mcm_sms_timestamp_type_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_aggregates
    @{
  */
typedef struct {

  uint8_t total_segments;
  /**<       The number of long  short message   */

  uint8_t seg_number;
  /**<       Current number. */

  uint8_t reference_num;
  /**<       reference  number. */
}mcm_sms_user_data_head_type_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Request Message; Sets the service center configuration type. */
typedef struct {

  /* Mandatory */
  char service_center_addr[MCM_SMS_MAX_ADDR_LENGTH_V01 + 1];
  /**<   Address of the service center.*/

  /* Optional */
  uint8_t service_center_addr_type_valid;  /**< Must be set to true if service_center_addr_type is being passed */
  char service_center_addr_type[MCM_SMS_MAX_SCA_TYPE_LENGTH_V01 + 1];
  /**<   129 if the SMSC address does not start with a "+" characte;
    145 if the SMSC address starts with a "+" character*/

  /* Optional */
  uint8_t validity_time_valid;  /**< Must be set to true if validity_time is being passed */
  int64_t validity_time;
  /**<   Validity time.*/
}mcm_sms_set_service_center_cfg_type_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Response Message; Sets the service center configuration type. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/
}mcm_sms_set_service_center_cfg_type_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Request Message; Gets the service center configuration type. */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}mcm_sms_get_service_center_cfg_type_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Response Message; Gets the service center configuration type. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/

  /* Optional */
  uint8_t service_center_addr_valid;  /**< Must be set to true if service_center_addr is being passed */
  char service_center_addr[MCM_SMS_MAX_ADDR_LENGTH_V01 + 1];
  /**<   Address of the service center.*/

  /* Optional */
  uint8_t service_center_addr_type_valid;  /**< Must be set to true if service_center_addr_type is being passed */
  char service_center_addr_type[MCM_SMS_MAX_SCA_TYPE_LENGTH_V01 + 1];
  /**<   129 if the SMSC address does not start with a "+" characte;
    145 if the SMSC address starts with a "+" character*/


  /* Optional */
  uint8_t validity_time_valid;  /**< Must be set to true if validity_time is being passed */
  int64_t validity_time;
  /**<   Validity time.*/
}mcm_sms_get_service_center_cfg_type_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Request Message; Sends an MO message. */
typedef struct {

  /* Mandatory */
  mcm_sms_msg_format_t_v01 message_format;
  /**<   Message format. */

  /* Mandatory */
  uint32_t message_content_len;  /**< Must be set to # of elements in message_content */
  char message_content[MCM_SMS_MAX_MO_MSG_LENGTH_V01];
  /**<   Message content. */

  /* Mandatory */
  char destination[MCM_SMS_MAX_ADDR_LENGTH_V01 + 1];
  /**<   Destination. */

  /* Optional */
  uint8_t size_validation_valid;  /**< Must be set to true if size_validation is being passed */
  mcm_sms_msg_size_validation_mode_t_v01 size_validation;
  /**<   Size validation. */
}mcm_sms_send_mo_msg_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Response Message; Sends an MO message. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/
}mcm_sms_send_mo_msg_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Request Message; Sets the message configutation. */
typedef struct {

  /* Optional */
  uint8_t default_size_validation_mode_valid;  /**< Must be set to true if default_size_validation_mode is being passed */
  mcm_sms_msg_size_validation_mode_t_v01 default_size_validation_mode;
  /**<   Default size validation mode. */

  /* Optional */
  uint8_t enable_cb_valid;  /**< Must be set to true if enable_cb is being passed */
  uint8_t enable_cb;
  /**<   Enable callback. */
}mcm_sms_set_msg_config_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Response Message; Sets the message configutation. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/
}mcm_sms_set_msg_config_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Request Message; Gets the message configuration. */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}mcm_sms_get_msg_config_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Response Message; Gets the message configuration. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/

  /* Optional */
  uint8_t default_size_validation_mode_valid;  /**< Must be set to true if default_size_validation_mode is being passed */
  mcm_sms_msg_size_validation_mode_t_v01 default_size_validation_mode;
  /**<   Default size validation mode. */

  /* Optional */
  uint8_t enable_cb_valid;  /**< Must be set to true if enable_cb is being passed */
  uint8_t enable_cb;
  /**<   Enable callback. */
}mcm_sms_get_msg_config_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Request Message; Sets the reception mode. */
typedef struct {

  /* Mandatory */
  mcm_sms_reception_mode_t_v01 reception_mode;
  /**<   Reception mode. */

  /* Optional */
  uint8_t last_absorbed_message_id_valid;  /**< Must be set to true if last_absorbed_message_id is being passed */
  int64_t last_absorbed_message_id;
  /**<   Last absorbed message ID. */
}mcm_sms_set_reception_mode_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Response Message; Sets the reception mode. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/
}mcm_sms_set_reception_mode_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Request Message; Registers for an indication of events. */
typedef struct {

  /* Optional */
  uint8_t register_sms_pp_event_valid;  /**< Must be set to true if register_sms_pp_event is being passed */
  uint8_t register_sms_pp_event;
  /**<   Receive a PP SMS event.*/
}mcm_sms_event_register_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Response Message; Registers for an indication of events. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
  /**<   Result code.*/
}mcm_sms_event_register_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Request Message; Point-to-point message indication. */
typedef struct {

  /* Mandatory */
  mcm_sms_msg_format_t_v01 message_format;
  /**<   Message format. */

  /* Mandatory */
  uint32_t message_content_len;  /**< Must be set to # of elements in message_content */
  char message_content[MCM_SMS_MAX_MT_MSG_LENGTH_V01];
  /**<   Message content. */

  /* Mandatory */
  char source_address[MCM_SMS_MAX_ADDR_LENGTH_V01 + 1];
  /**<   Source address. */

  /* Mandatory */
  int64_t message_id;
  /**<   Message ID. */

  /* Mandatory */
  mcm_sms_timestamp_type_t_v01 message_timestamp;
  /**<   Message time_stamp. */

  /* Optional */
  uint8_t user_data_head_valid;  /**< Must be set to true if user_data_head is being passed */
  mcm_sms_user_data_head_type_t_v01 user_data_head;
  /**<    long sms userdata head info. */

  /* Optional */
  uint8_t message_class_valid;  /**< Must be set to true if message_class is being passed */
  mcm_sms_message_class_t_v01 message_class;
  /**<   Message class. */
}mcm_sms_pp_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Request Message; Cell broadcast message indication. */
typedef struct {

  /* Mandatory */
  mcm_sms_msg_format_t_v01 message_format;
  /**<   Message format. */

  /* Mandatory */
  uint32_t message_content_len;  /**< Must be set to # of elements in message_content */
  char message_content[MCM_SMS_MAX_MT_MSG_LENGTH_V01];
  /**<   Message content. */
}mcm_sms_cb_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Request Message; Cell broadcast CMAS message indication. */
typedef struct {

  /* Optional */
  uint8_t type_0_record_valid;  /**< Must be set to true if type_0_record is being passed */
  mcm_cbs_cmae_record_type_0_t_v01 type_0_record;
  /**<   Type 0 record. */

  /* Optional */
  uint8_t type_1_record_valid;  /**< Must be set to true if type_1_record is being passed */
  mcm_cbs_cmae_record_type_1_t_v01 type_1_record;
  /**<   Type 1 record. */

  /* Optional */
  uint8_t type_2_record_valid;  /**< Must be set to true if type_2_record is being passed */
  mcm_cbs_cmae_record_type_2_t_v01 type_2_record;
  /**<   Type 2 record. */
}mcm_sms_cb_cmas_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sms_qmi_messages
    @{
  */
/** Request Message; Point-to-point message indication where to store. */
typedef struct {

  /* Mandatory */
  mcm_sms_storage_info_t_v01 mt_message;
  /**<   Message storage infor. */

  /* Mandatory */
  mcm_sms_mode_type_t_v01 message_mode;
  /**<   Message mode. */
}mcm_sms_storage_ind_msg_v01;  /* Message */
/**
    @}
  */


#define MCM_SMS_SET_SERVICE_CENTER_CFG_TYPE_REQ_V01 0x0700
#define MCM_SMS_SET_SERVICE_CENTER_CFG_TYPE_RESP_V01 0x0700
#define MCM_SMS_GET_SERVICE_CENTER_CFG_TYPE_REQ_V01 0x0701
#define MCM_SMS_GET_SERVICE_CENTER_CFG_TYPE_RESP_V01 0x0701
#define MCM_SMS_SEND_MO_MSG_REQ_V01 0x0702
#define MCM_SMS_SEND_MO_MSG_RESP_V01 0x0702
#define MCM_SMS_SET_MSG_CONFIG_REQ_V01 0x0703
#define MCM_SMS_SET_MSG_CONFIG_RESP_V01 0x0703
#define MCM_SMS_GET_MSG_CONFIG_REQ_V01 0x0704
#define MCM_SMS_GET_MSG_CONFIG_RESP_V01 0x0704
#define MCM_SMS_SET_RECEPTION_MODE_REQ_V01 0x0705
#define MCM_SMS_SET_RECEPTION_MODE_RESP_V01 0x0705
#define MCM_SMS_EVENT_REGISTER_REQ_V01 0x0706
#define MCM_SMS_EVENT_REGISTER_RESP_V01 0x0706
#define MCM_SMS_PP_IND_V01 0x0707
#define MCM_SMS_CB_IND_V01 0x0708
#define MCM_SMS_CB_CMAS_IND_V01 0x0709
#define MCM_SMS_STORAGE_IND_V01 0x070A

#ifdef __cplusplus
}
#endif
#endif

