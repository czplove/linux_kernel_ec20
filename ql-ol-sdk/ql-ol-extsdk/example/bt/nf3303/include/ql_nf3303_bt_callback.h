/**
 * @file ql_nf3303_bt_callback.h
 * @brief Quectel Openlinux BT public function declarations.
 * 
 * @note 
 *
 * @copyright Copyright (c) 2009-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#ifndef QL_NF3303_BT_CALLBACK_H
#define QL_NF3303_BT_CALLBACK_H

#define SRV_REG_NONE	0
#define SRV_REG	              1
#define SRV_ATTR_REG	 2
#define SRV_REG_FAIL	  3
#define SRV_STOP             4
#define SRV_UNREG          5

#define CASE_PRINT(x)           case x: printf("%s\n", #x);

#define CLEAR                   "clear"
#define EXIT                    "exit"
#define MAX_CMD_LEN             30
#define BT_MAC_LEN              17
#define BT_LOCAL_NAME_MAX       250
#define BT_ADDR_LEN             6
#define PROFILE_NAME_MAX        10
#define PHONE_NUM_MAX_LEN       20
#define PINCODE_MAX_LEN         16
#define PASSKEY_MAX_LEN	        6
#define LOCATION_LEN            5
#define MESSAGE_MAX_LEN         512
#define UUID_LEN                36
#define RFCOMM_DATA_LEN         1024
#define PATH_MAX_LEN            1024
#define MAX_PAIR				30

#define BT_MAX_SERVICE_NUM   8
#define BT_MAX_ATTR_NUM      8

typedef struct
{
    int type;
    int uuid;
    int permission;
    int property;
    int attr_handle;
    int is_attr_registered;
    int is_attr_valid;
    int attr_index;
    int data_bytes;
    char *data;
} attrribute;

typedef struct 
{
    int service_uuid;
    int total_attr_num;
    int registered_attr_num;
    int is_srv_registered;
    int is_srv_valid;
    int service_index;
    int status;
    int srv_handle;
    attrribute attributes[BT_MAX_ATTR_NUM];
} ql_gatt_service;

typedef struct 
{
    int total_service_num;
    int registered_service_num;
    ql_gatt_service ql_gatt_service_s[BT_MAX_SERVICE_NUM];
    int gatt_client_register_status;
    int gatt_server_register_status;
} ql_gatt_services;

#define MAX_SCAN_SERVER_INFO_NUM 6
#define MAX_SERVRE_ADV_UUID_NUM    6
typedef struct
{
	char is_connected;
	BT_ADDR addr;
    char server_name[20];
	int rssi;
	unsigned char srv_uuid[MAX_SERVRE_ADV_UUID_NUM*2];
	unsigned char srv_uuid_num;
} ql_scan_server_info;


enum
{
    SRV0_MANUFACTURER_NAME_CHAR_ATTR_HANDLE = 0,
    SRV0_MANUFACTURER_NAME_DESC_ATTR_HANDLE,
    SRV0_SOFTWARE_REV_CHAR_ATTR_HANDLE,
    SRV0_SOFTWARE_REV_DESC_ATTR_HANDLE,
    SRV1_FIRMWARE_REV_CHAR_ATTR_HANDLE,
    SRV1_FIRMWARE_REV_DESC_ATTR_HANDLE,
    SRV1_MODEL_NUMBER_CHAR_ATTR_HANDLE,
    SRV1_MODEL_NUMBER_DESC_ATTR_HANDLE,
    SRV2_CURRENT_TIME_CHAR_ATTR_HANDLE,
    SRV2_CURRENT_TIME_DESC_ATTR_HANDLE,
    SRV2_INDICATE_TIME_CHAR_ATTR_HANDLE,
    SRV2_INDICATE_TIME_DESC_ATTR_HANDLE,
    MAX_ATTR_HANDLE_NUM
};

void ql_gatt_service_register(ql_gatt_service *ql_gatt_srv);

void ble_notify_timer_thread(union sigval v);
int ble_notify_timer_init(int sec, int usec, int *attr_handle);
int ble_notify_timer_stop(void);

void ble_indicate_timer_thread(union sigval v);
int ble_indicate_timer_init(int sec, int usec, int *attr_handle);
int ble_indicate_timer_stop(void);

/*******************************************************************************
 Bt Callback handler functions
*******************************************************************************/
/*  Basic functional*/
void FuncCallback_local_device_info(LOCAL_DEVICE_INFO  *info);
void FuncCallback_paired_device_info(nfUINT8 total, PAIRED_DEVICE_INFO  *info);
void FuncCallback_inquiry_states(INQ_STATES inq_state,INQUIRY_DEVICE_INFO *info);
void FuncCallback_pair_states(BT_ADDR* addr,PAIR_STATES states);
void FuncCallback_diagnostic_message(DIAG_ID code, void* data);
void FuncCallback_pair_request(BT_ADDR* addr, nfUINT8 name_size, nfUINT8* name, PAIR_PIN_TYPE type);
void FuncCallback_ssp_request(BT_ADDR* addr, nfUINT8 name_size, nfUINT8* name, nfUINT32 passkey, nfBOOLEAN display_only);
void FuncCallback_disconnect_reason(BT_ADDR *addr, NFBT_DISCONNECT_REASON reason);
void FuncCallback_bt_enable_state(ENABLE_STATES state);

/*  HFP funcational*/
void FuncCallback_hfp_connect_states(BT_ADDR* addr , HFP_STATES hfp_state);
void FuncCallback_hfp_call_status(BT_ADDR* addr, HFP_CALL_STATUS call_status, nfUINT8* number, nfUINT8 idx);
void FuncCallback_hfp_call_mpty_status(BT_ADDR* addr, HFP_CALL_STATUS mpty_status, HFP_MPTY_INFO *pMptyInfo, nfUINT8 mpty_line_count);
void FuncCallback_hfp_audio_states(BT_ADDR* addr , HFP_AUDIO_CONNECTION_STATES audio_state);
void FuncCallback_hfp_device_notification(BT_ADDR* addr, HFP_DEVICE_NOTIFY_PROP notify_prop, nfUINT8 notify_prop_state);
void FuncCallback_hfp_network_operator(BT_ADDR* addr, nfUINT8* operator_name);
void FuncCallback_hfp_voice_recognition_states(BT_ADDR* addr, HFP_VOICE_RECOGNITION_ACTION vr_action);
void FuncCallback_hfp_command_ok(BT_ADDR *addr, HFP_COMMAND_ID cmd);
void FuncCallback_hfp_command_error(BT_ADDR *addr, HFP_COMMAND_ID cmd);
void FuncCallback_hfp_volume_change(BT_ADDR *addr, HFP_VOLUME_TYPE type, int volume);
void FuncCallback_hfp_subscriber_info(BT_ADDR *addr, nfUINT8 *number, HFP_SUBSCRIBER_SERVICE_TYPE service_type);
void FuncCallback_hfp_manufacturer_id(BT_ADDR* addr, nfUINT8* manufacturer_id);
void FuncCallback_hfp_manufacturer_model_id(BT_ADDR* addr, nfUINT8* manufacturer_model_id);

/*  A2DP functional*/
void FuncCallback_a2dp_connection_states(BT_ADDR* addr,A2DP_CONN_STATES a2dp_state);
void FuncCallback_a2dp_audio_states(BT_ADDR* addr,A2DP_AUDIO_STATES a2dp_audio_state);
/*  AVRCP functional*/
void FuncCallback_avrcp_states(BT_ADDR* addr, AVRCP_CONN_STATES avrcp_state);
/*  AVRCP 1.3  */
void FuncCallback_avrcp_remote_feature(BT_ADDR* addr, AVRCP_FEATURE feature);
void FuncCallback_avrcp_play_states(BT_ADDR* addr, AVRCP_PLAY_STATE play_states,nfUINT32 song_pos, nfUINT32 song_total_len);
void FuncCallback_avrcp_element_attribute(BT_ADDR* addr, AVRCP_ELEMENT_ATTR_STRUCT *element_attrs);
void FuncCallback_avrcp_capability(BT_ADDR* addr, AVRCP_CAPABILITY *events);
void FuncCallback_avrcp_notification(BT_ADDR* addr, AVRCP_EVENT_ID event_id,AVRCP_NOTIFICATION_TYPE type, AVRCP_NOTIFICATION *notification_info);
void FuncCallback_avrcp_list_player_app_attr(BT_ADDR* addr, int num_attr, AVRCP_PLAYER_ATTR *p_attrs);
void FuncCallback_avrcp_list_player_app_value(BT_ADDR* addr, AVRCP_PLAYER_ATTR attr_id, int num_val, nfUINT8 *p_vals);
void FuncCallback_avrcp_get_player_app_value(BT_ADDR* addr, AVRCP_PLAYER_SETTING *player_settings);
void FuncCallback_avrcp_set_player_app_value(BT_ADDR* addr);
void FuncCallback_avrcp_command_error(BT_ADDR *bd_addr, AVRCP_COMMAND_ID pdu, AVRCP_RSP_STATUS reason, AVRCP_RSP_CODE rsp_code, AVRCP_EVENT_ID event_id);
void FuncCallback_avrcp_passthrough_rsp(BT_ADDR *bd_addr, AVRCP_OP_ID key_code, AVRCP_RSP_CODE rsp_code);
/*  AVRCP 1.4  */
void FuncCallback_avrcp_setaddressedplayer(BT_ADDR *addr);
void FuncCallback_avrcp_setbrowsedplayer(BT_ADDR *addr, AVRCP_BROWSED_FOLDER *folder);
void FuncCallback_avrcp_changepath(BT_ADDR *addr);
void FuncCallback_avrcp_playeritem(BT_ADDR *addr, AVRCP_PLAYERITEM *player_item);
void FuncCallback_avrcp_folderitem(BT_ADDR *addr, AVRCP_FOLDERITEM *folder_item);
void FuncCallback_avrcp_mediaitem(BT_ADDR *addr, AVRCP_MEDIAITEM *media_item);
void FuncCallback_avrcp_getitem_success(BT_ADDR *addr, int count);
void FuncCallback_avrcp_getitemattr(BT_ADDR *addr, AVRCP_UID uid, AVRCP_ELEMENT_ATTR_STRUCT *element_attrs);
void FuncCallback_avrcp_playitem(BT_ADDR *addr);

/*  PBAP functional*/
void FuncCallback_pb_contacts_info(BT_ADDR *addr, PB_STRUCT *pbstruct);
void FuncCallback_pb_download_states(BT_ADDR *addr, PB_DOWNLOAD_STATES states);
void FuncCallback_pb_total_contact(PB_ACCESS access, nfUINT16 total_contact, BT_ADDR *addr);

/*  MAP functional*/
void FuncCallback_map_download_state(BT_ADDR *addr , MAP_DOWNLOAD_STATES state);
void FuncCallback_map_notification_state(BT_ADDR *addr , MAP_NOTIFICATION_STATES state);
void FuncCallback_map_total_message(BT_ADDR *addr, MAP_MSG_TYPE type, MAP_FOLDER_STRUCT folder, nfUINT8 total);
void FuncCallback_map_receive_message(BT_ADDR *addr, MAP_SMS_GSM *message, nfBOOLEAN is_new);
void FuncCallback_map_send_message(BT_ADDR *addr, nfUINT8 *tg_num, MAP_COMMAND_STATUS send_status);
void FuncCallback_map_delete_message(BT_ADDR *addr, nfUINT8 *msg_handle, MAP_COMMAND_STATUS reason);
void FuncCallback_map_change_read_status(BT_ADDR *addr, nfUINT8 *msg_handle, MAP_COMMAND_STATUS reason);
void FuncCallback_map_notification_event(BT_ADDR *addr, MAP_EVENT_REPORT event, nfUINT8 *msg_handle, MAP_MSG_TYPE type, MAP_FOLDER_STRUCT folder);

/*  RFCOMM functional*/
void FuncCallback_rfcomm_connect_states(BT_ADDR* addr, PROFILE_ID id, RFCOMM_STATES rfcomm_state);

/*  HID functional*/
void FuncCallback_hid_connection_states(BT_ADDR* addr,HID_CONN_STATES hid_conn_state);

/*  OPP functional*/
void FuncCallback_opp_download_states(BT_ADDR *addr, OPP_STATES state);
void FuncCallback_opp_access_request(BT_ADDR *addr, nfUINT8 *dev_name, OPP_FILE_INFO *file_info);

/* GATT function */
void FuncCallback_gatt_scan_result(BT_ADDR *addr, int rssi, nfUINT8 *adv_data);
void FuncCallback_gatt_search_complete_result(GATT_STATUS status);
void FuncCallback_gatt_search_result(nfUINT8 *uuid, nfUINT8 is_primary);
void FuncCallback_gatt_get_include_service(GATT_STATUS status, nfUINT8 * srvc_uuid, nfUINT8 * inc_srvc_uuid);
void FuncCallback_gatt_get_characteristic(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, int char_prop);
void FuncCallback_gatt_get_descriptor(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, nfUINT8 *descr_uuid);
void FuncCallback_gatt_register_notify(GATT_STATUS status, int registered);
void FuncCallback_gatt_notify(BT_ADDR *addr, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, int data_len, nfUINT8 *data);
void FuncCallback_gatt_read_characteristic(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, int data_type, int data_len, nfUINT8 *data);
void FuncCallback_gatt_write_characteristic(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid);
void FuncCallback_gatt_read_descriptor(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, nfUINT8 *descr_uuid, int data_type, int data_len, nfUINT8 *data);
void FuncCallback_gatt_write_descriptor(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, nfUINT8 *descr_uuid);
void FuncCallback_gatt_read_rssi(GATT_STATUS status, BT_ADDR *addr, int rssi);
void FuncCallback_gatt_set_adv_data_result(GATT_STATUS status);
void FuncCallback_gatt_client_register(GATT_STATUS status);

void FuncCallback_gatt_service_added(GATT_STATUS status, nfUINT8 *srvc_uuid, int srvc_handle);
void FuncCallback_gatt_char_added(GATT_STATUS status, nfUINT8 *char_uuid, int srvc_handle, int char_handle);
void FuncCallback_gatt_des_added(GATT_STATUS status, nfUINT8 *descr_uuid, int srvc_handle, int descr_handle);
void FuncCallback_gatt_service_start(GATT_STATUS status, int srvc_handle);
void FuncCallback_gatt_service_stop(GATT_STATUS status, int srvc_handle);
void FuncCallback_gatt_service_deleted(GATT_STATUS status, int srvc_handle);
void FuncCallback_gatt_request_read_event(BT_ADDR *addr, int trans_id, int attr_handle, int offset, int is_long);
void FuncCallback_gatt_request_write_event(BT_ADDR *addr, int trans_id, int attr_handle, int offset, int is_prep, nfUINT8 *data, int length, int need_rsp);
void FuncCallback_gatt_request_exec_write_event(BT_ADDR *addr, int trans_id, int exec_write);
void FuncCallback_gatt_response_confirm(GATT_STATUS status);
void FuncCallback_gatt_indicator_sent(GATT_STATUS status);
void FuncCallback_gatt_congestion(int congestion);
void FuncCallback_gatt_connect_status(GATT_CONN_STATUS status, BT_ADDR *addr, GATT_ROLE role);
void FuncCallback_gatt_set_role_result(GATT_STATUS status, GATT_ROLE role);
void FuncCallback_gatt_attribute_list(int handle, nfUINT8 *uuid, int property, int type);
void FuncCallback_gatt_service_list(int handle, nfUINT8 *uuid);
void FuncCallback_gatt_list_complete(GATT_STATUS status);
void FuncCallback_gatt_service_register(GATT_STATUS status);
void FuncCallback_gatt_service_unregister(GATT_STATUS status);

/* service included in server */
void FuncCallback_gatt_battery_level_read_event(int trans_id, int attr_handle);
void FuncCallback_gatt_blood_pressure_feature_read_event(int trans_id, int attr_handle);
void FuncCallback_gatt_spp_rx_receive_event(int offset, nfUINT8 *value, int length);
void FuncCallback_gatt_char_notification_req(int srvc_idx, int attr_id, int enable);

/* BLE RF test number of packet received function */
void FuncCallback_le_test_packet_num_recv(nfUINT16 number_of_packet);

#endif /*QUECTEL_NF3303_BT_CALLBACK_H*/
