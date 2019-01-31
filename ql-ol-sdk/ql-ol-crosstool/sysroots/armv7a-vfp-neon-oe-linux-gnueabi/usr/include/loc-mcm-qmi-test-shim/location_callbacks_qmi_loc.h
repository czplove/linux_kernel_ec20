/* Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
 * Qualcomm Technologies Proprietary and Confidential.
 */


#ifndef LOCATION_CALLBACKS_QMI_LOC_H
#define LOCATION_CALLBACKS_QMI_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qmi_client.h"

#ifdef __cplusplus
}
#endif

void test_qmi_loc_location_cb(qmi_client_type hndl, unsigned int msg_id, void *ind_buf, unsigned int ind_len, void *ind_cb_data);
void test_qmi_loc_status_cb(qmi_client_type hndl, unsigned int msg_id, void *ind_buf, unsigned int ind_len, void *ind_cb_data);
void test_qmi_loc_sv_cb(qmi_client_type hndl, unsigned int msg_id, void *ind_buf, unsigned int ind_len, void *ind_cb_data);
void test_qmi_loc_nmea_cb(qmi_client_type hndl, unsigned int msg_id, void *ind_buf, unsigned int ind_len, void *ind_cb_data);
void test_qmi_loc_cap_cb(qmi_client_type hndl, unsigned int msg_id, void *ind_buf, unsigned int ind_len, void *ind_cb_data);
void test_qmi_loc_utc_time_cb(qmi_client_type hndl, unsigned int msg_id, void *ind_buf, unsigned int ind_len, void *ind_cb_data);
void test_qmi_loc_xtra_data_cb(qmi_client_type hndl, unsigned int msg_id, void *ind_buf, unsigned int ind_len, void *ind_cb_data);
void test_qmi_loc_agps_status_cb(qmi_client_type hndl, unsigned int msg_id, void *ind_buf, unsigned int ind_len, void *ind_cb_data);
void test_qmi_loc_ni_notify_cb(qmi_client_type hndl, unsigned int msg_id, void *ind_buf, unsigned int ind_len, void *ind_cb_data);

#endif //LOCATION_CALLBACKS_QMI_LOC_H

