/* Copyright (c) 2013-2014 Qualcomm Technologies, Inc.  All Rights Reserved.
 * Qualcomm Technologies Proprietary and Confidential.
 */

#ifndef LOCATION_CALLBACKS_MCM_LOC_H
#define LOCATION_CALLBACKS_MCM_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mcm_client.h"

#ifdef __cplusplus
}
#endif

void test_mcm_loc_location_cb(mcm_client_handle_type hndl, uint32_t msg_id, void *ind_c_struct, uint32_t ind_len);
void test_mcm_loc_status_cb(mcm_client_handle_type hndl, uint32_t msg_id, void *ind_c_struct, uint32_t ind_len);
void test_mcm_loc_sv_cb(mcm_client_handle_type hndl, uint32_t msg_id, void *ind_c_struct, uint32_t ind_len);
void test_mcm_loc_nmea_cb(mcm_client_handle_type hndl, uint32_t msg_id, void *ind_c_struct, uint32_t ind_len);
void test_mcm_loc_cap_cb(mcm_client_handle_type hndl, uint32_t msg_id, void *ind_c_struct, uint32_t ind_len);
void test_mcm_loc_utc_time_cb(mcm_client_handle_type hndl, uint32_t msg_id, void *ind_c_struct, uint32_t ind_len);
void test_mcm_loc_xtra_data_cb(mcm_client_handle_type hndl, uint32_t msg_id, void *ind_c_struct, uint32_t ind_len);
void test_mcm_loc_xtra_report_server_cb(mcm_client_handle_type hndl, uint32_t msg_id, void *ind_c_struct, uint32_t ind_len);
void test_mcm_loc_agps_status_cb(mcm_client_handle_type hndl, uint32_t msg_id, void *ind_c_struct, uint32_t ind_len);
void test_mcm_loc_ni_notify_cb(mcm_client_handle_type hndl, uint32_t msg_id, void *ind_c_struct, uint32_t ind_len);

#endif //LOCATION_CALLBACKS_MCM_LOC_H
