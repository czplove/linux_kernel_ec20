#ifndef MCM_CLIENT_SERVICE_01_H
#define MCM_CLIENT_SERVICE_01_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
 Copyright (c) 2014 Qualcomm Technologies, Inc.  All Rights Reserved.
 Qualcomm Technologies Proprietary and Confidential.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

#include <stdint.h>
#include "mcm_common_v01.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
    @}
  */

typedef uint64_t mcm_client_service_type_t_v01;
#define MCM_VOICE_V01       ((mcm_client_service_type_t_v01)0x00ull)
#define MCM_SMS_V01         ((mcm_client_service_type_t_v01)0x01ull)
#define MCM_DMS_V01         ((mcm_client_service_type_t_v01)0x02ull)
#define MCM_NW_V01          ((mcm_client_service_type_t_v01)0x04ull)
#define MCM_SIM_V01         ((mcm_client_service_type_t_v01)0x08ull)
#define MCM_DATA_V01        ((mcm_client_service_type_t_v01)0x10ull)
#define MCM_ATCOP_V01       ((mcm_client_service_type_t_v01)0x20ull)
#define MCM_LOC_V01         ((mcm_client_service_type_t_v01)0x40ull)
#define MCM_MOBILEAP_V01    ((mcm_client_service_type_t_v01)0x80ull)
/** @addtogroup mcm_client_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint16_t require_service;
  /**<   Preferred services to be loaded on the
 device, a bitmask of mcm_client_service_type*/
}mcm_client_require_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_client_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
}mcm_client_require_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_client_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  uint16_t not_require_service;
  /**<   Preferred services to be loaded on the
 device, a bitmask of mcm_client_service_type*/
}mcm_client_not_require_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_client_messages
    @{
  */
/**  Message;  */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;
}mcm_client_not_require_resp_msg_v01;  /* Message */
/**
    @}
  */

/*Service Message Definition*/
/** @addtogroup mcm_client_msg_ids
    @{
  */
#define MCM_CLIENT_REQUIRE_REQ_V01 0x0800
#define MCM_CLIENT_REQUIRE_RESP_V01 0x0800
#define MCM_CLIENT_NOT_REQUIRE_REQ_V01 0x0801
#define MCM_CLIENT_NOT_REQUIRE_RESP_V01 0x0801
/**
    @}
  */



#ifdef __cplusplus
}
#endif
#endif

