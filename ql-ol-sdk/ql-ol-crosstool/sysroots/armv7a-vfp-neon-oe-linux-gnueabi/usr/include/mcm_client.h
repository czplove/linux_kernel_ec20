#ifndef MCM_CLIENT_H
#define MCM_CLIENT_H
/**
  @file mcm_client.h

  @brief This header file defines the client facing APIs
        provided by Mobile Connection Manager.
*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
   Copyright (c) 2013 Qualcomm Technologies, Inc.
   All rights reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

#include "mcm_common_v01.h"
#include "comdef.h"

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// DATA TYPES
//=============================================================================

typedef uint32 mcm_client_handle_type;

typedef void (*mcm_client_async_cb)
(
  mcm_client_handle_type hndl,
  uint32                 msg_id,
  void                  *resp_c_struct,
  uint32                 resp_len,
  void                  *token_id
);

typedef void (*mcm_client_ind_cb)
(
  mcm_client_handle_type hndl,
  uint32                 msg_id,
  void                  *ind_c_struct,
  uint32                 ind_len
);

//=============================================================================
// FUNCTIONS
//=============================================================================

//=============================================================================
// FUNCTION: mcm_client_init <ind_cb> <default_resp_cb> <out: hndl>
//=============================================================================

/** @ingroup mcm_client_api

   Initializes the MCM client library.

   This function is to be called before to any other functions are called.

 @param[out] hndl Client handle associated with this instance.
 @param[in] ind_cb Indication callback.
 @param[in] default_resp_cb Default response callback for async methods.

 @return
    MCM_SUCCESS -- 0 is success. \n
    MCM_ERROR_TBD...
*/
uint32 mcm_client_init
(
  mcm_client_handle_type   *hndl,
  mcm_client_ind_cb         ind_cb,
  mcm_client_async_cb       default_resp_cb
);

//=============================================================================
// FUNCTION: mcm_client_execute_command_async
//=============================================================================

/** @ingroup mcm_client_api

  Sends a command asynchronously to the service.

 @param[in] hndl Client handle associated with this instance.
 @param[in] msg_id Message ID.
 @param[in] req_c_struct Command request structure.
 @param[in] req_c_struct_len Command request structure length.
 @param[in] resp_c_struct Command response structure.
 @param[in] resp_c_struct_len Command response structure length.
 @param[in] async_resp_cb Asynchronous response callback.
 @param[in] token_id Token ID.

 @return
   MCM_SUCCESS -- 0 is success. \n
   MCM_ERROR_TBD...
*/
uint32 mcm_client_execute_command_async
(
  mcm_client_handle_type     hndl,
  int                        msg_id,
  void                      *req_c_struct,
  int                        req_c_struct_len,
  void                      *resp_c_struct,
  int                        resp_c_struct_len,
  mcm_client_async_cb        async_resp_cb,
  void                      *token_id
);

#define MCM_CLIENT_EXECUTE_COMMAND_ASYNC(hndl, msg_id, req_ptr, resp_ptr, cb , token_id)  \
  mcm_client_execute_command_async(hndl, msg_id, req_ptr, sizeof(*(req_ptr)), resp_ptr, sizeof(*(resp_ptr)), cb, (token_id))

#define MCM_CLIENT_EXECUTE_COMMAND_NO_PAYLOAD_ASYNC(hndl, msg_id, req_ptr, resp_ptr, cb, token_id) \
  mcm_client_execute_command_async(hndl, msg_id, NULL, 0, resp_ptr, sizeof(*(resp_ptr)), cb, (token_id))

//=============================================================================
// FUNCTION: mcm_client_execute_command_sync
//=============================================================================

/** @ingroup mcm_client_api

   Sends a command synchronously to the service.

 @param[in] hndl Client handle associated with this instance.
 @param[in] msg_id Message ID.
 @param[in] req_c_struct Command request structure.
 @param[in] req_c_struct_len Command request structure length.
 @param[in] resp_c_struct Command response structure.
 @param[in] resp_c_struct_len Command response structure length.

 @return
    MCM_SUCCESS -- 0 is success. \n
    MCM_ERROR_TBD...
*/
uint32 mcm_client_execute_command_sync
(
  mcm_client_handle_type      hndl,
  int                         msg_id,
  void                       *req_c_struct,
  int                         req_c_struct_len,
  void                       *resp_c_struct,
  int                         resp_c_struct_len
);

#define MCM_CLIENT_EXECUTE_COMMAND_SYNC(hndl, msg_id, req_ptr, resp_ptr) \
  mcm_client_execute_command_sync(hndl, msg_id, req_ptr, sizeof(*(req_ptr)), resp_ptr, sizeof(*(resp_ptr)))

#define MCM_CLIENT_EXECUTE_COMMAND_NO_PAYLOAD_SYNC(hndl, msg_id, resp_ptr) \
  mcm_client_execute_command_sync(hndl, msg_id, NULL, 0, resp_ptr, sizeof(*(resp_ptr)))


//=============================================================================
// FUNCTION: mcm_client_execute_command_sync_ex
//=============================================================================

/** @ingroup mcm_client_api

   Sends a command synchronously to the service with the specificied timeout value.

 @param[in] hndl Client handle associated with this instance.
 @param[in] msg_id Message ID.
 @param[in] req_c_struct Command request structure.
 @param[in] req_c_struct_len Command request structure length.
 @param[in] resp_c_struct Command response structure.
 @param[in] resp_c_struct_len Command response structure length.
 @param[in] timeout Timeout in milliseconds.

 @return
    MCM_SUCCESS -- 0 is success. \n
    MCM_ERROR_TBD...
*/
uint32 mcm_client_execute_command_sync_ex
(
  mcm_client_handle_type      hndl,
  int                         msg_id,
  void                       *req_c_struct,
  int                         req_c_struct_len,
  void                       *resp_c_struct,
  int                         resp_c_struct_len,
  uint32                      timeout
);

#define MCM_CLIENT_EXECUTE_COMMAND_SYNC_EX(hndl, msg_id, req_ptr, resp_ptr, timeout) \
  mcm_client_execute_command_sync_ex(hndl, msg_id, req_ptr, sizeof(*(req_ptr)), resp_ptr, sizeof(*(resp_ptr)), timeout)

#define MCM_CLIENT_EXECUTE_COMMAND_NO_PAYLOAD_SYNC_EX(hndl, msg_id, resp_ptr, timeout) \
  mcm_client_execute_command_sync_ex(hndl, msg_id, NULL, 0, resp_ptr, sizeof(*(resp_ptr)), timeout)

//=============================================================================
// FUNCTION: mcm_client_release <hndl>
//=============================================================================

/** @ingroup mcm_client_api

  Releases the subscription to the client.

 @param[in] hndl Handle associated with this instance and event queue.

 @return
   MCM_SUCCESS -- 0 is success. \n
   MCM_ERROR_TBD...
*/
uint32 mcm_client_release(mcm_client_handle_type hndl);

#ifdef __cplusplus
}
#endif

#endif // MCM_CLIENT_H
