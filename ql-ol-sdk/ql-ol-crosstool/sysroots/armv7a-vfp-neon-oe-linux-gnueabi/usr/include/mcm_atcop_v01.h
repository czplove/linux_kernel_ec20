#ifndef MCM_ATCOP_SERVICE_01_H
#define MCM_ATCOP_SERVICE_01_H
/**
  @file mcm_atcop_v01.h

  @brief This is the public header file which defines the mcm_atcop service Data structures.

*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c)2013 Qualcomm Technologies, Inc.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

#include <stdint.h>
#include "mcm_common_v01.h"


#ifdef __cplusplus
extern "C" {
#endif




/** @addtogroup mcm_atcop_consts
    @{
  */
/** Maximum request message size. */
#define MCM_ATCOP_MAX_REQ_MSG_SIZE_V01 513
/** Maximum response message size. */
#define MCM_ATCOP_MAX_RESP_MSG_SIZE_V01 4097

/*
 * Client waits inifinitely for a response from the server.
 * Hence a timeout value of 0
*/
#define MCM_ATCOP_MSG_TIMEOUT_VALUE_V01 0
/**
    @}
  */

/** @addtogroup mcm_atcop_messages
    @{
  */
/** Request message; Communicates an ATCoP message to the server. */
typedef struct {

  /* Mandatory */
  char cmd_req[MCM_ATCOP_MAX_REQ_MSG_SIZE_V01];  /**< Request message. */

  /* Mandatory */
  uint32_t cmd_len;  /**< Request command length. @newpagetable */
}mcm_atcop_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_atcop_messages
    @{
  */
/** Response message; Communicates an ATCoP message to the server. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 resp;  /**< Response code. */

  /* Optional */
  /*  Response  */
  uint8_t cmd_resp_valid;  /**< Must be set to TRUE if cmd_resp is being passed. */
  char cmd_resp[MCM_ATCOP_MAX_RESP_MSG_SIZE_V01];  /**< Command response. */

  /* Optional */
  uint8_t resp_len_valid;  /**< Must be set to TRUE if resp_len is being passed. */
  uint32_t resp_len;  /**< Response message length. */
}mcm_atcop_resp_msg_v01;  /* Message */
/**
    @}
  */

/*Service Message Definition*/
/** @addtogroup mcm_atcop_msg_ids
    @{
*/

/** @name
@{ */

#define MCM_ATCOP_REQ_V01 0x0600
#define MCM_ATCOP_RESP_V01 0x0600

/** @} */ /* end_namegroup */

/** @} */


#ifdef __cplusplus
}
#endif
#endif

