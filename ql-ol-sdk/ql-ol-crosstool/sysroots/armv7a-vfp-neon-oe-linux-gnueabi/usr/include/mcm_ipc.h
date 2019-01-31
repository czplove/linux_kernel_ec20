/***************************************************************************************************
    @file
    mcm_ipc.h

    @brief
    Supports functions for handling IPC requests.

  Copyright (c) 2014 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential. 

***************************************************************************************************/
#ifndef MCM_IPC_H
#define MCM_IPC_H

#ifdef __cplusplus
extern "C" {
#endif

#define MCM_IPC_MAX_SERVICES 5

typedef void (*mcm_ipc_exit_func_cb)();
typedef struct mcm_ipc_srv_mgr_exit_func_t
{
    int srv_id;
    mcm_ipc_exit_func_cb srv_exit_func;
} mcm_ipc_srv_mgr_exit_func;

int mcm_ipc_srv_mgr_start(mcm_ipc_srv_mgr_exit_func *srv_down_func);

/** add by tyler.kuang@20180502 start : for MCM service manager */
int mcm_ipc_get_service_is_ready(void);
void mcm_set_service_ready(int srv_id, int is_ready);
const char * mcm_get_service_flag_file(int srv_id);
int mcm_get_service_is_ready(int srv_id);
/** add by tyler.kuang@20180502 end */

#ifdef __cplusplus
}
#endif
#endif
