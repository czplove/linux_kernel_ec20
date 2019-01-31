/**
 * @file ql_mgmt_common.h
 * @brief Quectel manager common interface function declarations.
 *
 * @note
 *
 * @copyright Copyright (c) 2009-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#ifndef __QL_MGMT_COMMON_H__
#define __QL_MGMT_COMMON_H__

#define COMMAND_SET 1
#define COMMAND_GET 2

#define TIMEOUT_DEFAULT 5

typedef struct {
	msg_handle_type msg_handle_client;
	s32 ind_sockfd;
	pthread_t ind_thread;
	ql_manager_ind_cb ind_cb;
} ql_mgmt_client_s;


#ifdef FEATURE_QL_WIFI
extern ql_mgmt_client_error_type_e wifi_command_handle(ql_mgmt_client_type handle,
						       s32 command_flag,
						       u32 command,
						       ql_mgmt_client_wifi_s *wifi);
#endif /* end of FEATURE_QL_WIFI */

#ifdef FEATURE_QL_DATA_CALL
extern ql_mgmt_client_error_type_e data_call_command_handle(ql_mgmt_client_type handle,
							    s32 command_flag,
							    u32 command,
							    ql_mgmt_client_data_call_s *data_call);
extern s32 data_call_get_ind_val(u16 msg_id, void *in_data, s32 in_size, void **out_data, s32 *out_size);
#endif /* end of FEATURE_QL_DATA_CALL */

#ifdef FEATURE_QL_NETWORK
extern ql_mgmt_client_error_type_e network_command_handle(ql_mgmt_client_type handle, s32 command_flag,
							  u32 command, ql_mgmt_client_network_s *network);

extern s32 wifi_get_ind_val(u16 msg_id, void *in_data, s32 in_size, void **out_data, s32 *out_size);
#endif /* end of FEATURE_QL_NETWORK */

#endif /* end of __QL_MGMT_COMMON_H__ */
