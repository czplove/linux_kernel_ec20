#ifndef MCM_DM_SERVICE_01_H
#define MCM_DM_SERVICE_01_H
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



/** @addtogroup mcm_dm_consts
    @{
  */

/**  Maximum array limit. */
#define MCM_MAX_ARRAY_LIMIT_V01 252

//Laurence.yin-2018/04/20-QCM9XOL00004C015-P01, <[DM] : define imei meid max length.>
/**  Max array limit */
#define MCM_MAX_DMS_IMEI_V01 32

/**  Max array limit  */
#define MCM_MAX_DMS_MEID_V01 32

/**  Max device rev id limit  */
#define MCM_MAX_DEVICE_REV_ID_V01 256
/**
    @}
  */

/** @addtogroup mcm_dm_enums
    @{
  */
typedef enum {
  MCM_DM_RADIO_MODE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_DM_RADIO_MODE_OFFLINE_V01 = 0x0001, /**< Radio power off or unknown. */
  MCM_DM_RADIO_MODE_ONLINE_V01 = 0x0002, /**< Radio online. */
  MCM_DM_RADIO_MODE_UNAVAILABLE_V01 = 0x0003, /**< Radio unvailable. */
  MCM_DM_RADIO_MODE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_dm_radio_mode_t_v01;
/**
    @}
  */

/** @cond
*/

typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}mcm_dm_get_radio_mode_req_msg_v01;

/** @endcond */

/** @addtogroup mcm_dm_messages
    @{
  */
/** Response message; Gets the radio mode of the device. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;  /**< Result code. */

  /* Optional */
  uint8_t radio_mode_valid;  /**< Must be set to TRUE if radio_mode is being passed. */
  mcm_dm_radio_mode_t_v01 radio_mode;
  /**< Current radio mode; must
 be one of the modes in #mcm_dm_radio_mode_t_v01.*/
}mcm_dm_get_radio_mode_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_dm_messages
    @{
  */
/** Request message; Sets the device radio mode. */
typedef struct {

  /* Mandatory */
  mcm_dm_radio_mode_t_v01 radio_mode;
  /**< Radio mode to set.*/
}mcm_dm_set_radio_mode_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_dm_messages
    @{
  */
/** Response message; Sets the device radio mode. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;  /**< Result code. */

  /* Optional */
  uint8_t no_change_valid;  /**< Must be set to TRUE if no_change is being passed. */
  uint8_t no_change;  /**< No change. */
}mcm_dm_set_radio_mode_resp_msg_v01;  /* Message */
/**
    @}
  */

//Laurence.yin-2018/04/20-QCM9XOL00004C015-P01, <[DM] : define imei meid messages id struct.>
/** @addtogroup mcm_dm_qmi_messages
  @{
 */
/** Request Message; Gets the device serial numbers. */
typedef struct {
    /* This element is a placeholder to prevent the declaration of
    an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
    char __placeholder;
}mcm_dm_get_device_serial_numbers_req_msg_v01;

/* Message */
/**
  @}
 */

/** @addtogroup mcm_dm_qmi_messages
  @{
 */
/** Response Message; Gets the device serial numbers. */
typedef struct {

    /* Mandatory */
    mcm_response_t_v01 response;
    /**<   Result code.*/

    /* Optional */
    uint8_t imei_valid;  /**< Must be set to true if imei is being passed */
    char imei[MCM_MAX_DMS_IMEI_V01 + 1];
    /**<   imei.*/

    /* Optional */
    uint8_t meid_valid;  /**< Must be set to true if meid is being passed */
    char meid[MCM_MAX_DMS_MEID_V01 + 1];
    /**<   meid.*/
}mcm_dm_get_device_serial_numbers_resp_msg_v01;  /* Message */
/**
  @}
 */

/** @addtogroup mcm_dm_messages
    @{
  */
/** Request message; Registers for an indication of events. */
typedef struct {

  /* Optional */
  uint8_t register_radio_mode_changed_event_valid;  /**< Must be set to TRUE if register_radio_mode_changed_event is being passed. */
  uint8_t register_radio_mode_changed_event;
  /**< Radio mode changed event.*/
}mcm_dm_event_register_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_dm_qmi_messages
  @{
 */
/** Request Message; Gets the device firmware revision identification. */
typedef struct {
    /* This element is a placeholder to prevent the declaration of
       an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
    char __placeholder;
}mcm_dm_get_device_rev_id_req_msg_v01;

/* Message */
/**
  @}
 */

/** @addtogroup mcm_dm_qmi_messages
  @{
 */
/** Response Message; Gets the device firmware revision identification. */
typedef struct {

    /* Mandatory */
    mcm_response_t_v01 response;
    /**<   Result code.*/

  /* Mandatory */
    char device_rev_id[MCM_MAX_DEVICE_REV_ID_V01 + 1];
    /**<   device rev id.*/
}mcm_dm_get_device_rev_id_resp_msg_v01;  /* Message */
/**
  @}
 */

/** @addtogroup mcm_dm_messages
    @{
  */
/** Response message; Registers for an indication of events. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;  /**< Response. */
}mcm_dm_event_register_resp_msg_v01;  /* Result code */
/**
    @}
  */

/** @addtogroup mcm_dm_messages
    @{
  */
/** Indication message; Indication when the radio mode is changed. */
typedef struct {

  /* Optional */
  uint8_t radio_mode_valid;  /**< Must be set to TRUE if radio_mode is being passed .*/
  mcm_dm_radio_mode_t_v01 radio_mode;  /**< Radio mode. */
}mcm_dm_radio_mode_changed_event_ind_msg_v01;  /* Message */
/**
    @}
  */



/** @addtogroup mcm_dm_service_msg_ids
    @{
  */
#define MCM_DM_GET_RADIO_MODE_REQ_V01 0x0201
#define MCM_DM_GET_RADIO_MODE_RESP_V01 0x0201
#define MCM_DM_SET_RADIO_MODE_REQ_V01 0x0202
#define MCM_DM_SET_RADIO_MODE_RESP_V01 0x0202
#define MCM_DM_EVENT_REGISTER_REQ_V01 0x0203
#define MCM_DM_EVENT_REGISTER_RESP_V01 0x0203
#define MCM_DM_RADIO_MODE_CHANGED_EVENT_IND_V01 0x0204
//Laurence.yin-2018/04/20-QCM9XOL00004C015-P01, <[DM] : define imei meid message id.>
#define MCM_DM_GET_DEVICE_SERIAL_NUMBERS_REQ_V01 0x02A0
#define MCM_DM_GET_DEVICE_SERIAL_NUMBERS_RESP_V01 0x02A0
#define MCM_DM_GET_DEVICE_REV_ID_REQ_V01 0x02A1
#define MCM_DM_GET_DEVICE_REV_ID_RESP_V01 0x02A1

/**
 @}
 */


#ifdef __cplusplus
}
#endif
#endif

