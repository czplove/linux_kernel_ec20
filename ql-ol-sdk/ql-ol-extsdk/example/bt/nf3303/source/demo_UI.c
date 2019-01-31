#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "nFore_BlueGate.h"
#include "ql_nf3303_ble_common.h"
#include "ql_nf3303_ble_sleep.h"



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

const char *cancel = "-1";
const char *ignore = "-2";
const char *dontcare = "0";
const char *yes = "y";
const char *no = "n";

static pthread_t thread_id_rfcomm_read = 0;
static PROFILE_ID rfcomm_connected = 0;


/*  Bluetooth pair mechanisms */
typedef enum
{
    NFBT_PAIR_NONE = 0,
    NFBT_PAIR_PIN,
    NFBT_PAIR_SSP,
    NFBT_PAIR_SSP_PASSKEY,
    NFBT_PAIR_SSP_DISPLAY_ONLY,
}NFBT_PAIR_MECHANISMS;

static BT_ADDR remote_pair_addr;
NFBT_PAIR_MECHANISMS pair_request = NFBT_PAIR_NONE;

static BT_ADDR opp_remote_access_addr;
static int opp_access_request = 0;

#define BLE_INCLUDED TRUE
#if defined BLE_INCLUDED && BLE_INCLUDED == TRUE
    #define GATT_SPP_DEMO
#else
    #undef GATT_SPP_DEMO
#endif
#ifdef GATT_SPP_DEMO
typedef struct 
{
	int mask;
	int status;
	int srv_handle;
} demo_reg;
static demo_reg demo_service_reg;

typedef struct
{
	int type;
	int uuid;
	int permission;
	int property;
} attrribute;

typedef struct 
{
	int service_uuid;
	int attr_num;
	attrribute attributes[];
} N4_SERVICE_S;

#define SRV_REG_NONE	0
#define SRV_REG	1
#define SRV_ATTR_REG	2
#define SRV_REG_FAIL	3
#define SRV_STOP 4
#define SRV_UNREG 5

#define SRV_MASK 0x0F
#define NF_UUID_SERVCLASS_SPP 0x18FF
static const N4_SERVICE_S srv_spp = 
{
	NF_UUID_SERVCLASS_SPP, 4,
	{
		{	ATTRTYPE_CHARACTERISTIC, 0x2aff, /* SPP_DATA_SEND */
			GATT_PERMIT_READ, GATT_PROP_NOTIFY
		},
		{	ATTRTYPE_DESCRIPTOR, 0x2902, /* CLIENT_CONFIG */
			GATT_PERMIT_READ|GATT_PERMIT_WRITE, GATT_PROP_READ|GATT_PROP_WRITE
		},
		{	ATTRTYPE_CHARACTERISTIC, 0x2afe, /* SPP_DATA_RECEIVE */
			GATT_PERMIT_WRITE, GATT_PROP_WRITE
		},
		{	ATTRTYPE_DESCRIPTOR, 0x2902, /* CLIENT_CONFIG */
			GATT_PERMIT_READ, GATT_PROP_READ
		},
	}
};

#define SRV_SIZE 1
#define ATTR_SIZE 4
typedef struct
{
	char uuid[16];
	int type;
	int handle;
	int property;
	int permit;
} attr_conf_t;

typedef struct
{
	int srvc_hdl;
	int srvc_uuid[16];
	attr_conf_t attribute[ATTR_SIZE];
} gatt_service_t;

static int tmp_property = 0;
static int tmp_permit = 0;
static gatt_service_t gatt_spp[SRV_SIZE];
static gatt_service_t *gatt_get_service(int srv_handle)
{
	int i;
	
	for(i=0;i<SRV_SIZE;i++)
	{
		if(gatt_spp[i].srvc_hdl == srv_handle)
		{
			return &gatt_spp[i];
		}
	}

	return NULL;
}

static attr_conf_t *gatt_get_attr_conf(attr_conf_t *attribute, int attr_handle)
{
	int i;
	
	for(i=0;i<ATTR_SIZE;i++)
	{
		if(attribute[i].handle == attr_handle)
		{
			return &attribute[i];
		}
	}

	return NULL;
}

static int gatt_delete_service_info(int srv_handle)
{
	int i;
	gatt_service_t *p_srv = gatt_get_service(srv_handle);

	if(p_srv == NULL)
		return 1;

	memset(p_srv, 0 , sizeof(gatt_service_t));

	return 0;
}

static int gatt_store_service_info(int srv_handle, char *uuid)
{
	gatt_service_t *p_srv = gatt_get_service(0);

	if(p_srv == NULL)
		return false;
	
	//if(p_srv->srvc_hdl == 0)
	{
		p_srv->srvc_hdl = srv_handle;
		if(uuid)
			memcpy(&p_srv->srvc_uuid, uuid, 16);
		memset(&p_srv->attribute, 0 , sizeof(attr_conf_t));

		return true;
	}
}

static int gatt_store_attribute_info(int srv_handle, int attr_handle, int type, char *uuid)
{
	gatt_service_t *p_srv = gatt_get_service(srv_handle);
	attr_conf_t *p_attr;

	if(p_srv == NULL)
		return false;

	p_attr = gatt_get_attr_conf(p_srv->attribute, 0);

	if(p_attr == NULL)
		return false;

	p_attr->handle = attr_handle;
	p_attr->property = tmp_property;
	p_attr->permit = tmp_permit;
	p_attr->type = type;
	if(uuid)
	{
		memcpy(p_attr->uuid, uuid, 16);
	}

	return true;
}

static void gatt_service_register(void)
{
	int count = 0;
	int mask = 0;
	char gatt_base_uuid[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x80,0x5f,0x9b,0x34,0xfb};
	const N4_SERVICE_S *p_srv = &srv_spp;
	
	switch(demo_service_reg.status)
	{
		case SRV_REG_FAIL:
			if(demo_service_reg.srv_handle > 0)
				NFBT_GATT_DeleteService(demo_service_reg.srv_handle);
			/* break; */  /* do not use */
		case SRV_REG_NONE:
			demo_service_reg.mask = 0;
			demo_service_reg.srv_handle = 0;
			break;
		case SRV_REG:
			gatt_base_uuid[3] = p_srv->service_uuid & 0xff;
			gatt_base_uuid[2] = (p_srv->service_uuid>>8) & 0xff;
			NFBT_GATT_ServiceAdd(gatt_base_uuid);
			break;
		case SRV_UNREG:
			NFBT_GATT_DeleteService(demo_service_reg.srv_handle);
			break;
		case SRV_STOP:
			NFBT_GATT_StartService(0, demo_service_reg.srv_handle);
			break;
		case SRV_ATTR_REG:
			if(demo_service_reg.mask == 0)
			{
				demo_service_reg.status = SRV_REG_NONE;
				/* no attribute */
				/* register service */
				NFBT_GATT_StartService(1, demo_service_reg.srv_handle);
				break;
			}

			mask = demo_service_reg.mask;
			while(!(mask & 0x01))
			{
				count++;
				mask >>= 1;
			}

			demo_service_reg.mask &= ~(1<<count);
			gatt_base_uuid[3] = p_srv->attributes[count].uuid & 0xff;
			gatt_base_uuid[2] = (p_srv->attributes[count].uuid>>8) & 0xff;

			tmp_permit = p_srv->attributes[count].permission;
			tmp_property = p_srv->attributes[count].property;

			if(p_srv->attributes[count].type == ATTRTYPE_CHARACTERISTIC)
			{
				NFBT_GATT_CharacteristicAdd(demo_service_reg.srv_handle, gatt_base_uuid, p_srv->attributes[count].property, p_srv->attributes[count].permission);
			}
			else
			{
				NFBT_GATT_DescriptorAdd(demo_service_reg.srv_handle, gatt_base_uuid, p_srv->attributes[count].permission);
			}
			break;
	}
}
#endif /* GATT_SPP_DEMO */

/*******************************************************************************
 Bt Callback handler functions
*******************************************************************************/
/*  Basic functional*/
static void FuncCallback_local_device_info(LOCAL_DEVICE_INFO  *info);
static void FuncCallback_paired_device_info(nfUINT8 total, PAIRED_DEVICE_INFO  *info);
static void FuncCallback_inquiry_states(INQ_STATES inq_state,INQUIRY_DEVICE_INFO *info);
static void FuncCallback_pair_states(BT_ADDR* addr,PAIR_STATES states);
static void FuncCallback_diagnostic_message(DIAG_ID code, void* data);
static void FuncCallback_pair_request(BT_ADDR* addr, nfUINT8 name_size, nfUINT8* name, PAIR_PIN_TYPE type);
static void FuncCallback_ssp_request(BT_ADDR* addr, nfUINT8 name_size, nfUINT8* name, nfUINT32 passkey, nfBOOLEAN display_only);
static void FuncCallback_disconnect_reason(BT_ADDR *addr, NFBT_DISCONNECT_REASON reason);
static void FuncCallback_bt_enable_state(ENABLE_STATES state);

/*  HFP funcational*/
static void FuncCallback_hfp_connect_states(BT_ADDR* addr , HFP_STATES hfp_state);
static void FuncCallback_hfp_call_status(BT_ADDR* addr, HFP_CALL_STATUS call_status, nfUINT8* number, nfUINT8 idx);
static void FuncCallback_hfp_call_mpty_status(BT_ADDR* addr, HFP_CALL_STATUS mpty_status, HFP_MPTY_INFO *pMptyInfo, nfUINT8 mpty_line_count);
static void FuncCallback_hfp_audio_states(BT_ADDR* addr , HFP_AUDIO_CONNECTION_STATES audio_state);
static void FuncCallback_hfp_device_notification(BT_ADDR* addr, HFP_DEVICE_NOTIFY_PROP notify_prop, nfUINT8 notify_prop_state, nfUINT8* prop_data);
static void FuncCallback_hfp_network_operator(BT_ADDR* addr, nfUINT8* operator_name);
static void FuncCallback_hfp_voice_recognition_states(BT_ADDR* addr, HFP_VOICE_RECOGNITION_ACTION vr_action);
static void FuncCallback_hfp_command_ok(BT_ADDR *addr, HFP_COMMAND_ID cmd);
static void FuncCallback_hfp_command_error(BT_ADDR *addr, HFP_COMMAND_ID cmd);
static void FuncCallback_hfp_volume_change(BT_ADDR *addr, HFP_VOLUME_TYPE type, int volume);
static void FuncCallback_hfp_subscriber_info(BT_ADDR *addr, nfUINT8 *number, HFP_SUBSCRIBER_SERVICE_TYPE service_type);
static void FuncCallback_hfp_manufacturer_id(BT_ADDR* addr, nfUINT8* manufacturer_id);
static void FuncCallback_hfp_manufacturer_model_id(BT_ADDR* addr, nfUINT8* manufacturer_model_id);

/*  A2DP functional*/
static void FuncCallback_a2dp_connection_states(BT_ADDR* addr,A2DP_CONN_STATES a2dp_state);
static void FuncCallback_a2dp_audio_states(BT_ADDR* addr,A2DP_AUDIO_STATES a2dp_audio_state);
/*  AVRCP functional*/
static void FuncCallback_avrcp_states(BT_ADDR* addr, AVRCP_CONN_STATES avrcp_state);
/*  AVRCP 1.3  */
static void FuncCallback_avrcp_remote_feature(BT_ADDR* addr, AVRCP_FEATURE feature);
static void FuncCallback_avrcp_play_states(BT_ADDR* addr, AVRCP_PLAY_STATE play_states,nfUINT32 song_pos, nfUINT32 song_total_len);
static void FuncCallback_avrcp_element_attribute(BT_ADDR* addr, AVRCP_ELEMENT_ATTR_STRUCT *element_attrs);
static void FuncCallback_avrcp_capability(BT_ADDR* addr, AVRCP_CAPABILITY *events);
static void FuncCallback_avrcp_notification(BT_ADDR* addr, AVRCP_EVENT_ID event_id,AVRCP_NOTIFICATION_TYPE type, AVRCP_NOTIFICATION *notification_info);
static void FuncCallback_avrcp_list_player_app_attr(BT_ADDR* addr, int num_attr, AVRCP_PLAYER_ATTR *p_attrs);
static void FuncCallback_avrcp_list_player_app_value(BT_ADDR* addr, AVRCP_PLAYER_ATTR attr_id, int num_val, nfUINT8 *p_vals);
static void FuncCallback_avrcp_get_player_app_value(BT_ADDR* addr, AVRCP_PLAYER_SETTING *player_settings);
static void FuncCallback_avrcp_set_player_app_value(BT_ADDR* addr);
static void FuncCallback_avrcp_command_error(BT_ADDR *bd_addr, AVRCP_COMMAND_ID pdu, AVRCP_RSP_STATUS reason, AVRCP_RSP_CODE rsp_code, AVRCP_EVENT_ID event_id);
static void FuncCallback_avrcp_passthrough_rsp(BT_ADDR *bd_addr, AVRCP_OP_ID key_code, AVRCP_RSP_CODE rsp_code);
/*  AVRCP 1.4  */
static void FuncCallback_avrcp_setaddressedplayer(BT_ADDR *addr);
static void FuncCallback_avrcp_setbrowsedplayer(BT_ADDR *addr, AVRCP_BROWSED_FOLDER *folder);
static void FuncCallback_avrcp_changepath(BT_ADDR *addr);
static void FuncCallback_avrcp_playeritem(BT_ADDR *addr, AVRCP_PLAYERITEM *player_item);
static void FuncCallback_avrcp_folderitem(BT_ADDR *addr, AVRCP_FOLDERITEM *folder_item);
static void FuncCallback_avrcp_mediaitem(BT_ADDR *addr, AVRCP_MEDIAITEM *media_item);
static void FuncCallback_avrcp_getitem_success(BT_ADDR *addr, int count);
static void FuncCallback_avrcp_getitemattr(BT_ADDR *addr, AVRCP_UID uid, AVRCP_ELEMENT_ATTR_STRUCT *element_attrs);
static void FuncCallback_avrcp_playitem(BT_ADDR *addr);

/*  PBAP functional*/
static void FuncCallback_pb_contacts_info(BT_ADDR *addr, PB_STRUCT *pbstruct);
static void FuncCallback_pb_download_states(BT_ADDR *addr, PB_DOWNLOAD_STATES states);
static void FuncCallback_pb_total_contact(PB_ACCESS access, nfUINT16 total_contact, BT_ADDR *addr);

/*  MAP functional*/
static void FuncCallback_map_download_state(BT_ADDR *addr , MAP_DOWNLOAD_STATES state);
static void FuncCallback_map_notification_state(BT_ADDR *addr , MAP_NOTIFICATION_STATES state);
static void FuncCallback_map_total_message(BT_ADDR *addr, MAP_MSG_TYPE type, MAP_FOLDER_STRUCT folder, nfUINT8 total);
static void FuncCallback_map_receive_message(BT_ADDR *addr, MAP_SMS_GSM *message, nfBOOLEAN is_new);
static void FuncCallback_map_send_message(BT_ADDR *addr, nfUINT8 *tg_num, MAP_COMMAND_STATUS send_status);
static void FuncCallback_map_delete_message(BT_ADDR *addr, nfUINT8 *msg_handle, MAP_COMMAND_STATUS reason);
static void FuncCallback_map_change_read_status(BT_ADDR *addr, nfUINT8 *msg_handle, MAP_COMMAND_STATUS reason);
static void FuncCallback_map_notification_event(BT_ADDR *addr, MAP_EVENT_REPORT event, nfUINT8 *msg_handle, MAP_MSG_TYPE type, MAP_FOLDER_STRUCT folder);

/*  RFCOMM functional*/
static void FuncCallback_rfcomm_connect_states(BT_ADDR* addr, PROFILE_ID id, RFCOMM_STATES rfcomm_state);

/*  HID functional*/
static void FuncCallback_hid_connection_states(BT_ADDR* addr,HID_CONN_STATES hid_conn_state);

/*  OPP functional*/
static void FuncCallback_opp_download_states(BT_ADDR *addr, OPP_STATES state);
static void FuncCallback_opp_access_request(BT_ADDR *addr, nfUINT8 *dev_name, OPP_FILE_INFO *file_info);

/* GATT function */
static void FuncCallback_gatt_scan_result(BT_ADDR *addr, int rssi, nfUINT8 *adv_data);
static void FuncCallback_gatt_search_complete_result(GATT_STATUS status);
static void FuncCallback_gatt_search_result(nfUINT8 *uuid, nfUINT8 is_primary);
static void FuncCallback_gatt_get_include_service(GATT_STATUS status, nfUINT8 * srvc_uuid, nfUINT8 * inc_srvc_uuid);
static void FuncCallback_gatt_get_characteristic(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, int char_prop);
static void FuncCallback_gatt_get_descriptor(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, nfUINT8 *descr_uuid);
static void FuncCallback_gatt_register_notify(GATT_STATUS status, int registered);
static void FuncCallback_gatt_notify(BT_ADDR *addr, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, int data_len, nfUINT8 *data);
static void FuncCallback_gatt_read_characteristic(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, int data_type, int data_len, nfUINT8 *data);
static void FuncCallback_gatt_write_characteristic(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid);
static void FuncCallback_gatt_read_descriptor(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, nfUINT8 *descr_uuid, int data_type, int data_len, nfUINT8 *data);
static void FuncCallback_gatt_write_descriptor(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, nfUINT8 *descr_uuid);
static void FuncCallback_gatt_read_rssi(GATT_STATUS status, BT_ADDR *addr, int rssi);
static void FuncCallback_gatt_set_adv_data_result(GATT_STATUS status);
static void FuncCallback_gatt_client_register(GATT_STATUS status);

static void FuncCallback_gatt_service_added(GATT_STATUS status, nfUINT8 *srvc_uuid, int srvc_handle);
static void FuncCallback_gatt_char_added(GATT_STATUS status, nfUINT8 *char_uuid, int srvc_handle, int char_handle);
static void FuncCallback_gatt_des_added(GATT_STATUS status, nfUINT8 *descr_uuid, int srvc_handle, int descr_handle);
static void FuncCallback_gatt_service_start(GATT_STATUS status, int srvc_handle);
static void FuncCallback_gatt_service_stop(GATT_STATUS status, int srvc_handle);
static void FuncCallback_gatt_service_deleted(GATT_STATUS status, int srvc_handle);
static void FuncCallback_gatt_request_read_event(BT_ADDR *addr, int trans_id, int attr_handle, int offset, int is_long);
static void FuncCallback_gatt_request_write_event(BT_ADDR *addr, int trans_id, int attr_handle, int offset, int is_prep, nfUINT8 *data, int length, int need_rsp);
static void FuncCallback_gatt_request_exec_write_event(BT_ADDR *addr, int trans_id, int exec_write);
static void FuncCallback_gatt_response_confirm(GATT_STATUS status);
static void FuncCallback_gatt_indicator_sent(GATT_STATUS status);
static void FuncCallback_gatt_congestion(int congestion);
static void FuncCallback_gatt_connect_status(GATT_CONN_STATUS status, BT_ADDR *addr, GATT_ROLE role);
static void FuncCallback_gatt_set_role_result(GATT_STATUS status, GATT_ROLE role);
static void FuncCallback_gatt_attribute_list(int handle, nfUINT8 *uuid, int property, int type);
static void FuncCallback_gatt_service_list(int handle, nfUINT8 *uuid);
static void FuncCallback_gatt_list_complete(GATT_STATUS status);
static void FuncCallback_gatt_service_register(GATT_STATUS status);
static void FuncCallback_gatt_service_unregister(GATT_STATUS status);

/* service included in server */
static void FuncCallback_gatt_battery_level_read_event(int trans_id, int attr_handle);
static void FuncCallback_gatt_blood_pressure_feature_read_event(int trans_id, int attr_handle);
static void FuncCallback_gatt_spp_rx_receive_event(int offset, nfUINT8 *value, int length);
static void FuncCallback_gatt_char_notification_req(int srvc_idx, int attr_id, int enable);

/* BLE RF test number of packet received function */
static void FuncCallback_le_test_packet_num_recv(nfUINT16 number_of_packet);

static NFBT_CBK_STRUCT nfmw_callbacks =
{
    sizeof(NFBT_CBK_STRUCT),
    /*Basic functional cbk*/
    FuncCallback_local_device_info,
    FuncCallback_paired_device_info,
    FuncCallback_inquiry_states,
    FuncCallback_pair_states,
    FuncCallback_diagnostic_message,
    FuncCallback_pair_request,
    FuncCallback_ssp_request,
    FuncCallback_disconnect_reason,
    FuncCallback_bt_enable_state,

    /*HFP functional cbk*/
    FuncCallback_hfp_connect_states,
    FuncCallback_hfp_call_status,
    FuncCallback_hfp_call_mpty_status,
    FuncCallback_hfp_audio_states,
    FuncCallback_hfp_device_notification,
    FuncCallback_hfp_network_operator,
    FuncCallback_hfp_voice_recognition_states,
    FuncCallback_hfp_command_ok,
    FuncCallback_hfp_command_error,
    FuncCallback_hfp_volume_change,
    FuncCallback_hfp_subscriber_info,
    FuncCallback_hfp_manufacturer_id,
    FuncCallback_hfp_manufacturer_model_id,

    /*A2DP functional cbk*/
    FuncCallback_a2dp_connection_states,
    FuncCallback_a2dp_audio_states,

    /*AVRCP functional cbk*/
    FuncCallback_avrcp_states,
    FuncCallback_avrcp_remote_feature,
    FuncCallback_avrcp_play_states,
    FuncCallback_avrcp_element_attribute,
    FuncCallback_avrcp_capability,
    FuncCallback_avrcp_notification,
    FuncCallback_avrcp_list_player_app_attr,
    FuncCallback_avrcp_list_player_app_value,
    FuncCallback_avrcp_get_player_app_value,
    FuncCallback_avrcp_set_player_app_value,
    FuncCallback_avrcp_command_error,
    FuncCallback_avrcp_passthrough_rsp,
    FuncCallback_avrcp_setaddressedplayer,
    FuncCallback_avrcp_setbrowsedplayer,
    FuncCallback_avrcp_changepath,
    FuncCallback_avrcp_playeritem,
    FuncCallback_avrcp_folderitem,
    FuncCallback_avrcp_mediaitem,
    FuncCallback_avrcp_getitem_success,
    FuncCallback_avrcp_getitemattr,
    FuncCallback_avrcp_playitem,

    /*PBAP functional cbk*/
    FuncCallback_pb_contacts_info,
    FuncCallback_pb_download_states,
    FuncCallback_pb_total_contact,

    /*MAP functional cbk*/
    FuncCallback_map_download_state,
    FuncCallback_map_notification_state,
    FuncCallback_map_total_message,
    FuncCallback_map_receive_message,
    FuncCallback_map_send_message,
    FuncCallback_map_delete_message,
    FuncCallback_map_change_read_status,
    FuncCallback_map_notification_event,

    /*RFCOMM functional cbk*/
    FuncCallback_rfcomm_connect_states,

    /*HID functional cbk*/
    FuncCallback_hid_connection_states,

    /*OPP functional cbk*/
    FuncCallback_opp_download_states,
    FuncCallback_opp_access_request,

    /*GATT functional cbk*/
    /* client */
    FuncCallback_gatt_scan_result,
    FuncCallback_gatt_search_complete_result,
    FuncCallback_gatt_search_result,
    FuncCallback_gatt_get_include_service,
    FuncCallback_gatt_get_characteristic,
    FuncCallback_gatt_get_descriptor,
    FuncCallback_gatt_register_notify,
    FuncCallback_gatt_notify,
    FuncCallback_gatt_read_characteristic,
    FuncCallback_gatt_write_characteristic,
    FuncCallback_gatt_read_descriptor,
    FuncCallback_gatt_write_descriptor,
    FuncCallback_gatt_read_rssi,
    FuncCallback_gatt_set_adv_data_result,
    FuncCallback_gatt_client_register,

    /* server */
    FuncCallback_gatt_service_added,
    FuncCallback_gatt_char_added,
    FuncCallback_gatt_des_added,
    FuncCallback_gatt_service_start,
    FuncCallback_gatt_service_stop,
    FuncCallback_gatt_service_deleted,
    FuncCallback_gatt_request_read_event,
    FuncCallback_gatt_request_write_event,
    FuncCallback_gatt_request_exec_write_event,
    FuncCallback_gatt_response_confirm,
    FuncCallback_gatt_indicator_sent,
    FuncCallback_gatt_congestion,
    FuncCallback_gatt_connect_status,
    FuncCallback_gatt_set_role_result,
    FuncCallback_gatt_attribute_list,
    FuncCallback_gatt_service_list,
    FuncCallback_gatt_list_complete,
    FuncCallback_gatt_service_register,
    FuncCallback_gatt_service_unregister,

    /* service include in server */
    FuncCallback_gatt_battery_level_read_event,
    FuncCallback_gatt_blood_pressure_feature_read_event,
    FuncCallback_gatt_spp_rx_receive_event,    
    FuncCallback_gatt_char_notification_req,

    /* BLE RF test number of packet received */
    FuncCallback_le_test_packet_num_recv

};

/*  BT basic functional callback*/
static void FuncCallback_local_device_info(LOCAL_DEVICE_INFO  *info)
{
    printf("Local device infos:\n");
    printf("    Name:   [%s]\n", info->name);
    printf("    Addr:   [%02X:%02X:%02X:%02X:%02X:%02X]\n" ,
            info->addr.address[0],info->addr.address[1],info->addr.address[2],
            info->addr.address[3],info->addr.address[4],info->addr.address[5]);
    switch(info->scan_mode)
    {
        case NFBT_SCAN_MODE_NONE:
            printf("    Device Sleep\n\n");
            break;
        case NFBT_SCAN_MODE_CONNECTABLE:
            printf("    Device Connectable\n\n");
            break;
        case NFBT_SCAN_MODE_CONNECTABLE_DISCOVERABLE:
            printf("    Device Discoverable\n\n");
            break;
        case NFBT_SCAN_MODE_SHUTDOWN:
            printf("    Device Shutdown");
            break;
        default:
            printf("    Device Unknow mode\n\n");
            break;
    };

    printf("\n");
}

static void FuncCallback_paired_device_info(nfUINT8 total, PAIRED_DEVICE_INFO  *info)
{
    nfUINT8 index = 0;

    printf("pairlist:\n");
    for(index = 0; index < total; index++){
        printf("    Device %d\n", index+1);
        printf("    Device Name = [%s]\n", (char*)(info+index)->name);
        printf("    Device addr = [%02X:%02X:%02X:%02X:%02X:%02X]\n\n" ,
                (info+index)->addr.address[0],(info+index)->addr.address[1],(info+index)->addr.address[2],
                (info+index)->addr.address[3],(info+index)->addr.address[4],(info+index)->addr.address[5]);
    }

    printf("\n");
}

static void FuncCallback_inquiry_states(INQ_STATES inq_state, INQUIRY_DEVICE_INFO *info)
{
    if(inq_state==NFBT_INQ_INQUIRING){
        printf("    Addr  = %02X:%02X:%02X:%02X:%02X:%02X\n" ,
            *(info->addr.address),*(info->addr.address+1),*(info->addr.address+2),
            *(info->addr.address+3),*(info->addr.address+4),*(info->addr.address+5));
        printf("    Name  = ( %s )\n" ,info->name);
        printf("    CoD   = %06x " ,info->cod);
		switch(info->cod & 0x00FF00){
			case 0x000100:
				 printf("( Computer )\n");
				 break;
			case 0x000200:
				 printf("( Phone )\n");
				 break;
			case 0x000300:
				 printf("( Network Access Point )\n");
				 break;
			case 0x000400:
				 printf("( Audio/Video )\n");
				 break;			
			case 0x000500:
				 printf("( Peripheral )\n");
				 break;		
			case 0x000600:
				 printf("( Imaging )\n");
				 break;		
			case 0x000700:
				 printf("( Wearable )\n");
				 break;	
			case 0x000800:
				 printf("( Toy )\n");
				 break;	
			case 0x000900:
				 printf("( Health )\n");
				 break;	
			default:
				 printf("\n");
				 break;	
		}

        printf("    Inquiry Rssi  = %d\n\n" ,info->rssi);
    }
    else if(inq_state == NFBT_INQ_START){
        printf("inquiry start\n\n");
    }
    else if(inq_state == NFBT_INQ_END){
        printf("inquiry end\n\n");
    }
    else if(inq_state == NFBT_LINK_RSSI){
        printf("    Addr  = %02X:%02X:%02X:%02X:%02X:%02X\n" ,
            *(info->addr.address),*(info->addr.address+1),*(info->addr.address+2),
            *(info->addr.address+3),*(info->addr.address+4),*(info->addr.address+5));
        printf("    Name  = ( %s )\n" ,info->name);
        printf("    Link Rssi  = ( %d )\n\n" ,info->rssi);
    }
}

static void FuncCallback_pair_states(BT_ADDR* addr, PAIR_STATES states)
{
    printf("pair state:\n");
    printf("addr:[%02X:%02X:%02X:%02X:%02X:%02X]\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
    switch(states)
    {
        case NFBT_PAIR_STATE_NONE:
            printf("pair state none!!! \n");
            pair_request = NFBT_PAIR_NONE;
            break;
        case NFBT_PAIR_STATE_PAIRING:
            printf("pair state in pairing!!! \n");
            break;
        case NFBT_PAIR_STATE_PAIRED:
            printf("pair state in paired!!! \n");
            pair_request = NFBT_PAIR_NONE;
            break;
        default:
            printf("pair state unknow states!!! \n");
            break;
    };

    printf("\n");
}

static void FuncCallback_diagnostic_message(DIAG_ID code, void* data)
{
    char dev_name[PATH_MAX_LEN]={0};

    if (code < NFBT_DBG_BT_BASE)
	//printf("recive exception!!!!\n");
        ;

    switch(code)
    {
    case NFBT_NOERR:
        printf("NFBT_NOERR\n");
        break;

    case NFBT_CRITICAL_BT_PCM_IN_ERROR:
    case NFBT_CRITICAL_SYS_A2DP_AUDIO_ERROR:
    case NFBT_CRITICAL_SYS_RING_AUDIO_ERROR:
    case NFBT_CRITICAL_SYS_SCO_AUDIO_ERROR:
    case NFBT_CRITICAL_BT_PCM_OUT_ERROR:

        printf("System Audio can not open(%d).\n", code);
        break;
    case NFBT_CRITICAL_SYS_SCO_MIC_ERROR:
        printf("System Microphone can not open(%d).\n", code);
        break;

    case NFBT_ERR_BT_HW_ERROR:
    case NFBT_ERR_BT_HW_SEND_TIMEOUT:
    case NFBT_ERR_BT_HW_RECV_INVALID:
        printf("Bluetooth adapter exception(%d). Restart adapter\n", code);
        NFBT_Enable();
        break;
    case NFBT_ERR_BT_ADDR_INVALID:
        printf("Invalid BT Address(%d).\n", code);
        break;
    case NFBT_ERR_MEM_CORRUPTION:
    case NFBT_ERR_MEM_NOT_ENOUGH:
    	printf("Memory manager exception(%d).\n", code);
    	exit(-1);
        break;
    case NFBT_ERR_TACK_CREATE_ERROR:
        printf("Can not create task(%d).\n", code);
        break;

    case NFBT_WARNING_SYS_REC_DAMAGE:
    	printf("Can not find Bluetooth record file.\n");
        break;
    case NFBT_WARNING_BT_MUST_HW_RESET:
    	printf("BT Adapter must reset hardware.\n");
		ql_bt_module_enable();
        break;
	case NFBT_WARNING_SYS_REC_SAVE_FAILED:
		printf("Update BT config file failed (%d).\n", code);
		break;
	case NFBT_WARNING_SYS_RINGTONE_FILE_DAMAGE:
		printf("Can not find local ringtone file (%d).\n", code);
		break;
	case NFBT_WARNING_BT_CB_BLOCKING:
		printf("User Callback block a log time(%d).\n", code);
		break;
	case NFBT_WARNING_BT_ALLOW_SLEEP:
    	printf("Allow BT controller into sleep\n");
		ql_bt_wake_low();
        break;
    case NFBT_WARNING_BT_WAKE:
    	printf("Wake BT controller right now!\n");
		ql_bt_wake_high();
        break;
    case NFBT_WARNING_BT_USERIAL_FD:
		if(data!=NULL){
			printf("Get BT Userial fd: %d\n",*(int*)data);
		}
    	break;

    case NFBT_DBG_BT_TESTMODE:
    	printf("Bluetooth Chip enter test mode\n");
        break;
    case NFBT_DBG_SYS_A2DP_AUDIO_REQUEST:
		NFBT_GetA2DPDevice(dev_name, PATH_MAX_LEN);
    	printf("System Mdeia sound \"%s\" request\n", dev_name);
        break;
    case NFBT_DBG_SYS_A2DP_AUDIO_ACTIVE:
		NFBT_GetA2DPDevice(dev_name, PATH_MAX_LEN);
    	printf("System Mdeia sound \"%s\" active\n", dev_name);
        break;
    case NFBT_DBG_SYS_A2DP_AUDIO_RELEASE:
		NFBT_GetA2DPDevice(dev_name, PATH_MAX_LEN);
    	printf("System Mdeia sound \"%s\" release\n", dev_name);
        break;
    case NFBT_DBG_SYS_RING_AUDIO_REQUEST:
		NFBT_GetRingDevice(dev_name, PATH_MAX_LEN);
    	printf("System Ring sound \"%s\" request\n", dev_name);
        break;
    case NFBT_DBG_SYS_RING_AUDIO_ACTIVE:
		NFBT_GetRingDevice(dev_name, PATH_MAX_LEN);
    	printf("System Ring sound \"%s\" active\n", dev_name);
        break;
    case NFBT_DBG_SYS_RING_AUDIO_RELEASE:
		NFBT_GetRingDevice(dev_name, PATH_MAX_LEN);
    	printf("System Ring sound \"%s\" release\n", dev_name);
        break;
    case NFBT_DBG_SYS_SCO_AUDIO_REQUEST:
    	printf("System Phone sound resource request\n");
        break;
    case NFBT_DBG_SYS_SCO_AUDIO_ACTIVE:
    	printf("System Phone sound resource active\n");
        break;
    case NFBT_DBG_SYS_SCO_AUDIO_RELEASE:
    	printf("System Phone sound resource release\n");
        break;
    case NFBT_DBG_SYS_SCO_MIC_REQUEST:
    	printf("System Phone Microphone resource request\n");
        break;
    case NFBT_DBG_SYS_SCO_MIC_ACTIVE:
    	printf("System Phone Microphone resource active\n");
        break;
    case NFBT_DBG_SYS_SCO_MIC_RELEASE:
    	printf("System Phone Microphone resource release\n");
        break;
    case NFBT_DBG_BT_PCM_IN_REQUEST:
    	printf("System BT in resource request\n");
        break;
    case NFBT_DBG_BT_PCM_IN_ACTIVE:
    	printf("System BT in resource active\n");
        break;
    case NFBT_DBG_BT_PCM_IN_RELEASE:
    	printf("System BT in resource release\n");
        break;
    case NFBT_DBG_BT_PCM_OUT_REQUEST:
    	printf("System BT out resource request\n");
        break;
    case NFBT_DBG_BT_PCM_OUT_ACTIVE:
    	printf("System BT out resource active\n");
        break;
    case NFBT_DBG_BT_PCM_OUT_RELEASE:
    	printf("System BT out resource release\n");
        break;
		
    case NFBT_DBG_A310_CLEAR_PAIREDLIST:
    	printf("A310 Bluetooth Tester request to delete all paired device\n");
        break;
    case NFBT_DBG_A310_ENABLE_ECNR:
    	printf("A310 Bluetooth Tester request ECNR enable\n");
        break;
    case NFBT_DBG_A310_DISABLE_ECNR:
    	printf("A310 Bluetooth Tester request ECNR disable\n");
        break;
    case NFBT_DBG_A310_LMIC_LSPK:
    	printf("A310 Bluetooth Tester request Left MIC and Left Speaker enable\n");
        break;
    case NFBT_DBG_A310_LMIC_RSPK:
    	printf("A310 Bluetooth Tester request Left MIC and Right Speaker enable\n");
        break;
    case NFBT_DBG_A310_RMIC_LSPK:
    	printf("A310 Bluetooth Tester request Right MIC and Left Speaker enable\n");
        break;
    case NFBT_DBG_A310_RMIC_RSPK:
    	printf("A310 Bluetooth Tester request Right MIC and Right Speaker enable\n");
        break;

    default:
    	printf("Unknown message(%x)\n", code);
    	break;
    }

	printf("\n");
}

static void FuncCallback_pair_request(BT_ADDR* addr, nfUINT8 name_size, nfUINT8* name, PAIR_PIN_TYPE type)
{
    int i = 0;
    for(i = 0; i < BT_ADDR_LEN; i++ )
        remote_pair_addr.address[i] = addr->address[i];

    printf("pair request:\n");

    if(name_size>0)
        printf("Remote device %s: ", name);
    else
        printf("Remote device: ");

    printf("%02X:%02X:%02X:%02X:%02X:%02X send a pair request.\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);

    if(type == NFBT_SSP_PASSKEY_REQ)
    {
        pair_request = NFBT_PAIR_SSP_PASSKEY;
        printf( "Press 'y' key for ready to input pin-code\n");
    }
    else /* type == NFBT_LEGACY_PIN_REQ*/
    {
        pair_request = NFBT_PAIR_PIN;
        printf( "Enter y or n to response the remote device pairing request.\n");
    }
}

static void FuncCallback_ssp_request(BT_ADDR* addr, nfUINT8 name_size, nfUINT8* name, nfUINT32 passkey, nfBOOLEAN display_only)
{
    int i = 0;
    for(i = 0; i < BT_ADDR_LEN; i++ )
        remote_pair_addr.address[i] = addr->address[i];

    printf("ssp request:\n");

    if(name_size>0)
        printf("Remote device %s: ", name);
    else
        printf("Remote device: ");

    printf("%02X:%02X:%02X:%02X:%02X:%02X send a pair request.\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);

    if(display_only)
    {
        pair_request = NFBT_PAIR_SSP_DISPLAY_ONLY; /* This is for BLE authentication */
        printf( "To confirm passkey %06u is same as displayed on remote device.\n", passkey );
        printf( "Enter y or n to response the remote device pairing request.\n");
    }
    else if(display_only == 0 && passkey == 0)
    {
        pair_request = NFBT_PAIR_SSP_PASSKEY;
        printf( "Enter y or n to response the remote device pairing request.\n");
    }
    else
    {
        pair_request = NFBT_PAIR_SSP;
        printf( "To confirm request passkey %06u is same as displayed on remote device.\n", passkey );
        printf( "Enter y or n to response the remote device pairing request.\n");
    }
}

static void FuncCallback_disconnect_reason(BT_ADDR *addr, NFBT_DISCONNECT_REASON reason)
{
    printf("Remote device [%02X:%02X:%02X:%02X:%02X:%02X] ",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);

    switch(reason)
    {
		case NFBT_DISCONNECT_REASON_NOT_FIND:
			printf("not find.\n\n");
			break;
    	case NFBT_DISCONNECT_REASON_LINK_LOSS:
    		printf("link loss.\n\n");
    		break;
    	case NFBT_DISCONNECT_REASON_REMOTE_USER:
    		printf("remote user disconnect.\n\n");
    		break;
    	case NFBT_DISCONNECT_REASON_REMOTE_POWER_OFF:
    		printf("remote bt off.\n\n");
    		break;
    	default:
    		printf("unknown.\n\n");
    		break;
    }
}

static void FuncCallback_bt_enable_state(ENABLE_STATES state)
{
    printf("bt state:");
    switch(state)
    {
    case NFBT_BT_ENABLE:
        printf(" enable \n");
        break;
    case NFBT_BT_DISABLE:
        printf(" disable \n");
        break;
    case NFBT_SYS_WARNING:
        printf(" system fail\n");
        break;
    default:
        printf("unknown\n");
        break;
    }

    printf("\n");
}

/*  HFP funcational*/
static void FuncCallback_hfp_connect_states(BT_ADDR* addr , HFP_STATES hfp_state)
{
    switch(hfp_state)
    {
        case NFBT_HFP_DISCONNECTED:
            printf( "HFP is now disconnected to ");
            break;
        case NFBT_HFP_CONNECTING:
            printf( "HFP is now connecting to ");
            break;
        case NFBT_HFP_CONNECTED:
            printf( "HFP is now connected to ");
#if 0
            NFBT_PB_Download(addr, PB_ACCESS_COMBINE_PB_CALLLOG, PB_PHOTO_ON);
			NFBT_MAP_EnableNotification(addr);
            NFBT_MAP_DownloadAllMessage(addr, MAP_MSG_TYPE_SMS_GSM, MAP_FOLDER_STRUCTURE_INBOX, 1);
#endif
            break;
        case NFBT_HFP_DISCONNECTING:
            printf( "HFP is now disconnecting to ");
            break;
        default:
            printf( "unknown HFP connect state to ");
            break;
    }
    printf("addr:[%02X:%02X:%02X:%02X:%02X:%02X]\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
}

static void FuncCallback_hfp_call_status(BT_ADDR* addr, HFP_CALL_STATUS call_status, nfUINT8* number, nfUINT8 idx)
{
    switch(call_status){
    case HFP_CALL_STATUS_INCOMING:
        printf( "There is a call on %02X:%02X:%02X:%02X:%02X:%02X with number %s at line %d\n\n",
                addr->address[0], addr->address[1], addr->address[2],
                addr->address[3], addr->address[4], addr->address[5],
                number, idx );
        break;
    case HFP_CALL_STATUS_OUTGOING:
        printf( "The outgoing call to %s on %02X:%02X:%02X:%02X:%02X:%02X is waiting for response at line %d\n\n", number,
                addr->address[0], addr->address[1], addr->address[2],
                addr->address[3], addr->address[4], addr->address[5],
                idx );
        break;
    case HFP_CALL_STATUS_ACTIVE:
        printf( "The phone call of %s on %02X:%02X:%02X:%02X:%02X:%02X has been activated at line %d\n\n", number,
                addr->address[0], addr->address[1], addr->address[2],
                addr->address[3], addr->address[4], addr->address[5],
                idx );
        break;
    case HFP_CALL_STATUS_TERMINATE:
        printf( "The phone call of %s on %02X:%02X:%02X:%02X:%02X:%02X has been terminated at line %d\n\n", number,
                addr->address[0], addr->address[1], addr->address[2],
                addr->address[3], addr->address[4], addr->address[5],
                idx );
        break;
    case HFP_CALL_STATUS_HOLD:
        printf( "The phone call of %s on %02X:%02X:%02X:%02X:%02X:%02X has been hold at line %d\n\n", number,
                addr->address[0], addr->address[1], addr->address[2],
                addr->address[3], addr->address[4], addr->address[5],
                idx );
        break;
    default:
        printf("unknown call status \n");
        break;
    }
}
static void FuncCallback_hfp_call_mpty_status(BT_ADDR* addr, HFP_CALL_STATUS mpty_status, HFP_MPTY_INFO *pMptyInfo, nfUINT8 mpty_line_count)
{
    unsigned char i = 0;
    if(mpty_line_count>0)
        printf( "%d phone calls in multi-party on %02X:%02X:%02X:%02X:%02X:%02X\n", mpty_line_count,
            addr->address[0], addr->address[1], addr->address[2],
            addr->address[3], addr->address[4], addr->address[5]);

    switch(mpty_status){
    case HFP_CALL_STATUS_MPTYCALLHOLD:
        for( i = 0; i < mpty_line_count; i++ )
            printf( "The phone call of %s has been hold at line %d in multi-party.\n", pMptyInfo[i].number, pMptyInfo[i].idx );
        printf( "\n" );
        break;
    case HFP_CALL_STATUS_MPTYCALLACTIVE:
        for( i = 0; i < mpty_line_count; i++ )
            printf( "The phone call of %s has been active at line %d in multi-party.\n", pMptyInfo[i].number, pMptyInfo[i].idx );
        printf( "\n" );
        break;
    case HFP_CALL_STATUS_MPTYCALLTERMINATE:
    	printf( "The multi-party have been terminated.\n\n");
    	break;
    default:
        printf("unknown call mpty status \n\n");
        break;
    }
}

static void FuncCallback_hfp_audio_states(BT_ADDR* addr, HFP_AUDIO_CONNECTION_STATES audio_state)
{
    switch(audio_state)
    {
        case HFP_AUDIO_DISCONNECTED:
            printf("HFP audio is now disconnect to ");
            break;
        case HFP_AUDIO_CONNECTING:
            printf("HFP audio is now connecting to ");
            break;
        case HFP_AUDIO_CONNECTED:
            printf("HFP audio is now connected to ");
            break;
        case HFP_AUDIO_DISCONNECTING:
            printf("HFP audio is now disconnecting to ");
            break;
        default:
            printf("Unknow HFP audio state to ");
            break;
    }
    printf("addr:[%02X:%02X:%02X:%02X:%02X:%02X]\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
}

static void FuncCallback_hfp_device_notification(BT_ADDR* addr, HFP_DEVICE_NOTIFY_PROP notify_prop, nfUINT8 notify_prop_state, nfUINT8 *prop_data)
{
    switch(notify_prop)
    {
        case HFP_ROAMING_STATE:
            if(notify_prop_state){
                printf( "The Roam of %02X:%02X:%02X:%02X:%02X:%02X is roaming state\n\n",
                        addr->address[0], addr->address[1], addr->address[2],
                        addr->address[3], addr->address[4], addr->address[5]);
            }
            else{
                printf( "The Roam of %02X:%02X:%02X:%02X:%02X:%02X is home state\n\n",
                        addr->address[0], addr->address[1], addr->address[2],
                        addr->address[3], addr->address[4], addr->address[5]);
            }
            break;
        case HFP_SIGNAL_STRENGTH:
            printf( "The signal strength of %02X:%02X:%02X:%02X:%02X:%02X is now level %d\n\n",
                    addr->address[0], addr->address[1], addr->address[2],
                    addr->address[3], addr->address[4], addr->address[5],
                    notify_prop_state);
            break;
        case HFP_BATTCHG_STRENGTH:
            printf( "The battery status of %02X:%02X:%02X:%02X:%02X:%02X is now level %d\n\n",
                    addr->address[0], addr->address[1], addr->address[2],
                    addr->address[3], addr->address[4], addr->address[5],
                    notify_prop_state);
            break;
        case HFP_SERVICE_STATE:
            if(notify_prop_state){
                printf( "The network of %02X:%02X:%02X:%02X:%02X:%02X is available\n\n",
                        addr->address[0], addr->address[1], addr->address[2],
                        addr->address[3], addr->address[4], addr->address[5]);
            }
            else{
                printf( "The network of %02X:%02X:%02X:%02X:%02X:%02X is not available\n\n",
                        addr->address[0], addr->address[1], addr->address[2],
                        addr->address[3], addr->address[4], addr->address[5]);
            }
            break;
        case HFP_DEVICE_IMEI_NUMBER:
            if(prop_data){
                printf( "The device IMEI of %02X:%02X:%02X:%02X:%02X:%02X is [%s]\n\n",
                      addr->address[0], addr->address[1], addr->address[2],
                      addr->address[3], addr->address[4], addr->address[5], prop_data);
            }
            break;
        default:
            printf("unknown device notification \n\n");
            break;
    }
}

static void FuncCallback_hfp_network_operator(BT_ADDR* addr, nfUINT8* operator_name)
{
    printf("Network operator of %02X:%02X:%02X:%02X:%02X:%02X\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
    printf("    name: %s\n\n", operator_name);
}

static void FuncCallback_hfp_voice_recognition_states(BT_ADDR* addr, HFP_VOICE_RECOGNITION_ACTION vr_action)
{
    printf("Voice recognition state of %02X:%02X:%02X:%02X:%02X:%02X\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
    switch(vr_action)
    {
        case HFP_VR_STOP:
            printf("    stop. \n\n");
            break;
        case HFP_VR_START:
            printf("    start. \n\n");
            break;
        default:
            printf("    unknown state \n\n");
            break;
    }
}
static void hfp_at_cmd_printf(HFP_COMMAND_ID cmd)
{
    switch(cmd)
    {
    case HFP_CMD_ANSWER:
        printf("\"answer\" ");
        break;
    case HFP_CMD_REJECT_OR_TERMINATE:
        printf("\"reject or terminate\" ");
        break;
    case HFP_CMD_DIAL:
        printf("\"dial\" ");
        break;
    case HFP_CMD_REDIAL:
        printf("\"re-dial\" ");
        break;
    case HFP_CMD_VOICE_RECONGNITION:
        printf("\"trigger voice recongnition\" ");
        break;
    case HFP_CMD_SEND_DTMF:
        printf("\"send dtmf\" ");
        break;
    case HFP_CMD_CALL_HOLD:
        printf("\"call hold\" ");
        break;
    case HFP_CMD_SPK_VOLUME:
        printf("\"set spk volume\" ");
        break;
    case HFP_CMD_MIC_VOLUME:
        printf("\"set mic volume\" ");
        break;
    case HFP_CMD_SUBSCRIBER_INFO:
        printf("\"get subscriber name info\" ");
        break;

    case HFP_CMD_SET_XAPL:
    	printf("\"Apple-specific\" ");
        break;

    case HFP_CMD_SET_APLEFM:
    	printf("\"Eyes Free mode\" ");
    	break;

    case HFP_CMD_MANUFACTURER_IDENTIFICATION:
    	printf("\"manufacturer identification\" ");
    	break;

    case HFP_CMD_MANUFACTURER_MODEL_IDENTIFICATION:
    	printf("\"manufacturer model identification\" ");
    	break;

    default:
        printf("\"Unknown HFP command\" ");
        break;
    }
}

static void FuncCallback_hfp_command_ok(BT_ADDR *addr, HFP_COMMAND_ID cmd)
{
    printf("HFP command: ");
    hfp_at_cmd_printf(cmd);
    printf("send to device %02X:%02X:%02X:%02X:%02X:%02X ok.\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
}

static void FuncCallback_hfp_command_error(BT_ADDR *addr, HFP_COMMAND_ID cmd)
{
    printf("HFP command: ");
    hfp_at_cmd_printf(cmd);
    printf("send to device %02X:%02X:%02X:%02X:%02X:%02X error.\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
}

static void FuncCallback_hfp_volume_change(BT_ADDR *addr, HFP_VOLUME_TYPE type, int volume)
{
    switch(type)
    {
        case HFP_VOLUME_TYPE_SPK:
            printf("HFP speaker volume of ");
            break;
        case HFP_VOLUME_TYPE_MIC:
            printf("HFP mic volume of ");
            break;
        default:
            printf("Unknown HFP volume type of ");
            break;
    }
    printf("%02X:%02X:%02X:%02X:%02X:%02X changes to %d\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5],
            volume);
}

static void FuncCallback_hfp_subscriber_info(BT_ADDR *addr, nfUINT8 *number, HFP_SUBSCRIBER_SERVICE_TYPE service_type)
{
    printf("Service state of %02X:%02X:%02X:%02X:%02X:%02X\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);

    switch(service_type)
    {
        case HFP_SERVICE_VOICE:
            printf("    Voice service. \n");
            break;
        case HFP_SERVICE_FAX:
            printf("    Fax service. \n");
            break;
        case HFP_SERVICE_UNKNOWN:
        default:
            printf("    Unknown service \n");
            break;
    }
    printf("    My phone number:%s\n\n",number);
}

static void FuncCallback_hfp_manufacturer_id(BT_ADDR* addr, nfUINT8* manufacturer_id)
{
	printf("Manufacturer identification of %02X:%02X:%02X:%02X:%02X:%02X: %s\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5],
            manufacturer_id);
}

static void FuncCallback_hfp_manufacturer_model_id(BT_ADDR* addr, nfUINT8* manufacturer_model_id)
{
	printf("Manufacturer model identification of %02X:%02X:%02X:%02X:%02X:%02X: %s\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5],
            manufacturer_model_id);
}

/*  A2DP functional*/
static void FuncCallback_a2dp_connection_states(BT_ADDR* addr,A2DP_CONN_STATES a2dp_conn_state)
{
    switch(a2dp_conn_state)
    {
        case NFBT_A2DP_CONN_STATE_DISCONNECTED:
            printf( "A2DP is now disconnected to ");
            break;
        case NFBT_A2DP_CONN_STATE_CONNECTING:
            printf( "A2DP is now connecting to ");
            break;
        case NFBT_A2DP_CONN_STATE_CONNECTED:
            printf( "A2DP is now connected to ");
            break;
        case NFBT_A2DP_CONN_STATE_DISCONNECTING:
            printf( "A2DP is now disconnecting to ");
            break;
        default:
            printf( "unknown A2DP connect state %d to ", a2dp_conn_state);
            break;
    }
    printf("addr:[%02X:%02X:%02X:%02X:%02X:%02X]\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);

}

static void FuncCallback_a2dp_audio_states(BT_ADDR* addr,A2DP_AUDIO_STATES a2dp_audio_state)
{
    switch(a2dp_audio_state)
    {
        case NFBT_A2DP_AUDIO_STATE_STREAMING:
            printf( "A2DP is now streaming to ");
            break;
        case NFBT_A2DP_AUDIO_STATE_STOPPED:
            printf( "A2DP is now stopped to ");
            break;
        default:
            printf( "unknown A2DP audio state %d to ", a2dp_audio_state);
            break;
    }
    printf("addr:[%02X:%02X:%02X:%02X:%02X:%02X]\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);

}

/*  AVRCP functional*/
static void FuncCallback_avrcp_states(BT_ADDR* addr, AVRCP_CONN_STATES avrcp_state)
{
    switch(avrcp_state)
    {
        case NFBT_AVRCP_STATE_DISCONNECTED:
            printf( "AVRCP is now disconnected to ");
            break;
        case NFBT_AVRCP_STATE_CONNECTED:
            printf( "AVRCP is now connected to ");
            NFBT_AVRCP_EnableNotification(addr);
#if 0			
            NFBT_AVRCP_ControlCmd(addr, AVRCP_OPID_PLAY, AVRCP_BUTTON_PRESS_AND_RELEASE);
#endif
            break;
        default:
            printf( "unknown AVRCP connect state to ");
            break;
    }
    printf("addr:[%02X:%02X:%02X:%02X:%02X:%02X]\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
}
/*  AVRCP 1.3 functional*/
static void FuncCallback_avrcp_remote_feature(BT_ADDR* addr, AVRCP_FEATURE feature)
{
    printf("The AVRCP feature of addr %02X:%02X:%02X:%02X:%02X:%02X: \n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);

    if(feature & AVRCP_FEATURE_PASSTHROUGH)
        printf("    Pass Through (Avrcp 1.0)\n");
    if(feature & AVRCP_FEATURE_META)
        printf("    Metadata Attributes (Avrcp 1.3)\n");
    if(feature & AVRCP_FEATURE_BROWSE)
        printf("    Browsing (Avrcp 1.4)\n");

    printf("\n");
}

static void FuncCallback_avrcp_play_states(BT_ADDR* addr, AVRCP_PLAY_STATE play_states,nfUINT32 song_pos, nfUINT32 song_total_len)
{
    int pos;

    printf( "The AVRCP play status of addr %02X:%02X:%02X:%02X:%02X:%02X is now ",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5] );

    switch( play_states ){
    case AVRCP_PLAYSTATE_STOPPED:
        printf( "stopped.\n" );
        break;
    case AVRCP_PLAYSTATE_PLAYING:
        printf( "playing.\n" );
        break;
    case AVRCP_PLAYSTATE_PAUSED:
        printf( "paused.\n" );
        break;
    case AVRCP_PLAYSTATE_FWD_SEEK:
        printf( "forwarded.\n" );
        break;
    case AVRCP_PLAYSTATE_REV_SEEK:
        printf( "rewound.\n" );
        break;
    default:
        printf( "in error.\n" );
        break;
    }


	if(song_total_len != AVRCP_UNKNOWN_TIME){
    	pos = song_total_len / 1000;
    	printf( "\tTrack length: %02d:%02d:%02d\n", pos/3600, (pos%3600)/60, pos%60 );
	}

	if(song_pos != AVRCP_UNKNOWN_TIME){
   		pos = song_pos / 1000;
    	printf( "\tNow position: %02d:%02d:%02d\n", pos/3600, (pos%3600)/60, pos%60 );
	}
    printf("\n");
}
static void FuncCallback_avrcp_element_attribute(BT_ADDR* addr, AVRCP_ELEMENT_ATTR_STRUCT *element_attrs)
{
    unsigned char i, j;

    typedef struct{
        const unsigned char attr_id;
        const char          *attr_name;
    }AVRCP_ARRR_TABLE;

    AVRCP_ARRR_TABLE attr_table_t[] =
    {
        {AVRCP_ELEMENT_ATTR_TITLE,          "Title"},
        {AVRCP_ELEMENT_ATTR_ARTIST,         "Artist"},
        {AVRCP_ELEMENT_ATTR_ALBUM,          "Album"},
        {AVRCP_ELEMENT_ATTR_TRACK_NUMBER,   "Track Number"},
        {AVRCP_ELEMENT_ATTR_TRACKS,         "Number of Tracks"},
        {AVRCP_ELEMENT_ATTR_GENRE,          "Genre"},
        {AVRCP_ELEMENT_ATTR_PLAYING_TIME,   "Play Time"},
    };

    printf( "The AVRCP of addr %02X:%02X:%02X:%02X:%02X:%02X has %X attributes:\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5],
            element_attrs->count );

    for( i = 0; i < element_attrs->count; ++i ){
        for( j= 0; j< sizeof(attr_table_t)/sizeof(AVRCP_ARRR_TABLE); j++){
            if(element_attrs->attr_id[i] == attr_table_t[j].attr_id) {
                if (attr_table_t[j].attr_id == AVRCP_ELEMENT_ATTR_PLAYING_TIME) {
                    int pos = atoi((char *)element_attrs->text[i])/1000;
                    printf( "\t%-18s:  ", attr_table_t[j].attr_name);
                    printf( "%02d:%02d:%02d\n", pos/3600, (pos%3600)/60, pos%60);
                } else {
                    printf( "\t%-18s:  %s\n", attr_table_t[j].attr_name, element_attrs->text[i] );
                }
            }
        }
    }
    printf( "\n" );

}

static void FuncCallback_avrcp_capability(BT_ADDR* addr, AVRCP_CAPABILITY *events)
{
    typedef struct{
        const unsigned char event_id;
        const char          *event_name;
    }AVRCP_EVENT_TABLE;

    const AVRCP_EVENT_TABLE event_table_t[] =
    {
        {AVRCP_EVT_PLAY_STATUS_CHANGED,     "Change in playback status of the current track (0x01)"},
        {AVRCP_EVT_TRACK_CHANGE,            "Change of current track (0x02)"},
        {AVRCP_EVT_TRACK_REACHED_END,       "Reached end of a track (0x03)"},
        {AVRCP_EVT_TRACK_REACHED_START,     "Reached start of a track (0x04)"},
        {AVRCP_EVT_PLAY_POS_CHANGED,        "Change in playback position (0x05)"},
        {AVRCP_EVT_BATT_STATUS_CHANGED,     "Change in battery status (0x06)"},
        {AVRCP_EVT_SYSTEM_STATUS_CHANGED,   "Change in system status (0x07)"},
        {AVRCP_EVT_APP_SETTINGS_CHANGED,    "Change in player application setting (0x08)"},
        {AVRCP_EVT_NOWPLAYING_CONTENT_CHANGED,  "The content of the Now Playing list has changed (0x09)"},
        {AVRCP_EVT_AVAILABLE_PLAYERS_CHANGED,   "The available players have changed (0x0A)"},
        {AVRCP_EVT_ADDRESSED_PLAYER_CHANGED,    "The Addressed Player has been changed (0x0B)"},
        {AVRCP_EVT_UIDS_CHANGED,                "The UIDs have changed (0x0C)"},
        {AVRCP_EVT_VOLUME_CHANGED,              "The volume has been changed locally on the target (0x0D)"},
    };

    printf( "The AVRCP capability of %02X:%02X:%02X:%02X:%02X:%02X has %X:\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5],
            events->count );

    unsigned char i,j;

    for( i=0 ; i<events->count ; i++ ) {
        for( j=0 ; j<sizeof(event_table_t)/sizeof(AVRCP_EVENT_TABLE) ; j++)
            if(event_table_t[j].event_id == events->events[i])
                printf( "\t%s\n", event_table_t[j].event_name );
    }
    printf( "\n" );
}

static void FuncCallback_avrcp_notification(BT_ADDR* addr, AVRCP_EVENT_ID event_id,AVRCP_NOTIFICATION_TYPE type, AVRCP_NOTIFICATION *notification_info)
{
    int pos;

    if (type == AVRCP_NOTIFICATION_TYPE_INTERIM)
        return;

    printf( "The device %02X:%02X:%02X:%02X:%02X:%02X ",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);


    switch(event_id)
    {
    case AVRCP_EVT_PLAY_STATUS_CHANGED:
        printf("AVRCP play status is now ");
        switch(notification_info->play_status){
        case AVRCP_PLAYSTATE_STOPPED:
            printf( "stopped.\n" );
            break;
        case AVRCP_PLAYSTATE_PLAYING:
            printf( "playing.\n" );
            break;
        case AVRCP_PLAYSTATE_PAUSED:
            printf( "paused.\n" );
            break;
        case AVRCP_PLAYSTATE_FWD_SEEK:
            printf( "forwarded.\n" );
            break;
        case AVRCP_PLAYSTATE_REV_SEEK:
            printf( "rewound.\n" );
            break;
        default:
            printf( "in error.\n" );
            break;
        }
        break;
    case AVRCP_EVT_TRACK_CHANGE:
        printf("AVRCP track changed, track UID is %llu\n",
                (long long)notification_info->track);
        break;
    case AVRCP_EVT_PLAY_POS_CHANGED:
		if(notification_info->song_pos != AVRCP_UNKNOWN_TIME){
			pos = notification_info->song_pos / 1000;
        	printf("AVRCP play position is %02d:%02d:%02d\n", pos/3600, (pos%3600)/60, pos%60);
		}
		else{
			printf("AVRCP play position UNKNOWN\n");
		}
        break;
    case AVRCP_EVT_BATT_STATUS_CHANGED:
        printf("AVRCP battery status: ");
        switch(notification_info->battery_status)
        {
        case AVRCP_BATTERY_STATUS_NORMAL:
            printf("Normal\n");
            break;
        case AVRCP_BATTERY_STATUS_WARNING:
            printf("Warning\n");
            break;
        case AVRCP_BATTERY_STATUS_CRITICAL:
            printf("Critical\n");
            break;
        case AVRCP_BATTERY_STATUS_EXTERNAL:
            printf("External\n");
            break;
        case AVRCP_BATTERY_STATUS_FULL_CHARGE:
            printf("Full charge\n");
            break;
        default:
            break;
        }
        break;
    case AVRCP_EVT_APP_SETTINGS_CHANGED:
        printf("AVRCP player setting:\n");
        FuncCallback_avrcp_get_player_app_value(addr, &notification_info->player_setting);
		break;
    case AVRCP_EVT_NOWPLAYING_CONTENT_CHANGED:
        printf("AVRCP now playing content changed.\n");
        break;
    case AVRCP_EVT_AVAILABLE_PLAYERS_CHANGED:
        printf("AVRCP available players changed.\n");
        break;
    case AVRCP_EVT_ADDRESSED_PLAYER_CHANGED:
        printf("AVRCP adressed player changed, player id is: %d, uid counter is: %d\n",
               notification_info->addr_player.player_id, notification_info->addr_player.uid_counter);
        break;
    case AVRCP_EVT_UIDS_CHANGED:
        printf("AVRCP UIDs changed, UID counter is %d\n",
               notification_info->uid_counter);
        break;  
    case AVRCP_EVT_PLAY_STATUS_PREFER:
        printf("AVRCP prefer play status is now ");
        switch(notification_info->play_status){
        case AVRCP_PLAYSTATE_STOPPED:
            printf( "stopped.\n" );
            break;
        case AVRCP_PLAYSTATE_PLAYING:
            printf( "playing.\n" );
            break;
        case AVRCP_PLAYSTATE_PAUSED:
            printf( "paused.\n" );
            break;
        case AVRCP_PLAYSTATE_FWD_SEEK:
            printf( "forwarded.\n" );
            break;
        case AVRCP_PLAYSTATE_REV_SEEK:
            printf( "rewound.\n" );
            break;
        default:
            printf( "in error.\n" );
            break;
        }
        break;

     case AVRCP_EVT_VOLUME_NOTIFICATION:
          printf( "AVRCP volume is %d%%.\n",(notification_info->AbsoluteVolume*100)/127 );
          break;
    default:
        break;
    }

    printf( "\n" );
}

static void FuncCallback_avrcp_list_player_app_attr(BT_ADDR* addr, int num_attr, AVRCP_PLAYER_ATTR *p_attrs)
{
    int i;

    printf( "The AVRCP player setting attribute of %02X:%02X:%02X:%02X:%02X:%02X has:%d\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5],
            num_attr );

    for (i = 0; i < num_attr; i++) {
        switch(p_attrs[i]) {
        case AVRCP_PLAYER_ATTR_EQUALIZER:
            printf( "\t\"Equalizer\"\n" );
            break;
        case AVRCP_PLAYER_ATTR_REPEAT:
            printf( "\t\"Repeat\"\n" );
            break;
        case AVRCP_PLAYER_ATTR_SHUFFLE:
            printf( "\t\"Shuffle\"\n" );
            break;
        case AVRCP_PLAYER_ATTR_SCAN:
            printf( "\t\"Scan\"\n" );
            break;
        default:
            printf( "\t\"Unknown Attribute ID %d\"\n", p_attrs[i] );
            break;
        }
    }

    printf("\n");
}

static void FuncCallback_avrcp_list_player_app_value(BT_ADDR* addr, AVRCP_PLAYER_ATTR attr_id, int num_val, nfUINT8 *p_vals)
{
    unsigned char i, j;
    unsigned char val_id;

    typedef struct{
        const unsigned char attr_id;
        const unsigned char value_id;
        const char          *attr_name;
        const char          *value_name;
    }AVRCP_APP_ATTR_VALUE_TABLE;

    const AVRCP_APP_ATTR_VALUE_TABLE attribute_table_t[] =
    {
        {AVRCP_PLAYER_ATTR_EQUALIZER,   AVRCP_EQUALIZER_OFF,    "Equalizer",    "OFF"},
        {AVRCP_PLAYER_ATTR_EQUALIZER,   AVRCP_EQUALIZER_ON,     "Equalizer",    "ON"},

        {AVRCP_PLAYER_ATTR_REPEAT,      AVRCP_REPEAT_OFF,       "Repeat",       "OFF"},
        {AVRCP_PLAYER_ATTR_REPEAT,      AVRCP_REPEAT_SINGLE,    "Repeat",       "Single"},
        {AVRCP_PLAYER_ATTR_REPEAT,      AVRCP_REPEAT_ALL,       "Repeat",       "All"},
        {AVRCP_PLAYER_ATTR_REPEAT,      AVRCP_REPEAT_GROUP,     "Repeat",       "Group"},

        {AVRCP_PLAYER_ATTR_SHUFFLE,     AVRCP_SHUFFLE_OFF,      "Shuffle",      "OFF"},
        {AVRCP_PLAYER_ATTR_SHUFFLE,     AVRCP_SHUFFLE_ALL,      "Shuffle",      "All"},
        {AVRCP_PLAYER_ATTR_SHUFFLE,     AVRCP_SHUFFLE_GROUP,    "Shuffle",      "Group"},

        {AVRCP_PLAYER_ATTR_SCAN,        AVRCP_SCAN_OFF,         "Scan",         "OFF"},
        {AVRCP_PLAYER_ATTR_SCAN,        AVRCP_SCAN_ALL_TRACK,   "Scan",         "All"},
        {AVRCP_PLAYER_ATTR_SCAN,        AVRCP_SCAN_GROUP,       "Scan",         "Group"},
    };

    printf( "The AVRCP player setting " );

    switch(attr_id) {
    case AVRCP_PLAYER_ATTR_EQUALIZER:
        printf( "\"Equalizer\"" );
        break;
    case AVRCP_PLAYER_ATTR_REPEAT:
        printf( "\"Repeat\"" );
        break;
    case AVRCP_PLAYER_ATTR_SHUFFLE:
        printf( "\"Shuffle\"" );
        break;
    case AVRCP_PLAYER_ATTR_SCAN:
        printf( "\"Scan\"" );
        break;
    default:
        printf( "\"Unknown Attribute ID %d\"", attr_id );
        break;
    }

    printf( " values of %02X:%02X:%02X:%02X:%02X:%02X has:%d\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5],
            num_val );

    for( i = 0; i < num_val; i++ ){
        val_id = p_vals[i];

        for( j = 0; j < sizeof(attribute_table_t) / sizeof(AVRCP_APP_ATTR_VALUE_TABLE); j++ ){
            if (attr_id == attribute_table_t[j].attr_id &&
                val_id == attribute_table_t[j].value_id){
                printf("%10s:%s\n", attribute_table_t[j].attr_name, attribute_table_t[j].value_name);
            }
        }
    }
    printf( "\n" );

}

static void FuncCallback_avrcp_get_player_app_value(BT_ADDR* addr, AVRCP_PLAYER_SETTING *player_settings)
{
    unsigned char i, j;
    unsigned char attr_id;
    unsigned char val_id;

    typedef struct{
        const unsigned char attr_id;
        const unsigned char value_id;
        const char          *attr_name;
        const char          *value_name;
    }AVRCP_APP_ATTR_VALUE_TABLE;

    const AVRCP_APP_ATTR_VALUE_TABLE attribute_table_t[] =
    {
        {AVRCP_PLAYER_ATTR_EQUALIZER,   AVRCP_EQUALIZER_OFF,    "Equalizer",    "OFF"},
        {AVRCP_PLAYER_ATTR_EQUALIZER,   AVRCP_EQUALIZER_ON,     "Equalizer",    "ON"},

        {AVRCP_PLAYER_ATTR_REPEAT,      AVRCP_REPEAT_OFF,       "Repeat",       "OFF"},
        {AVRCP_PLAYER_ATTR_REPEAT,      AVRCP_REPEAT_SINGLE,    "Repeat",       "Single"},
        {AVRCP_PLAYER_ATTR_REPEAT,      AVRCP_REPEAT_ALL,       "Repeat",       "All"},
        {AVRCP_PLAYER_ATTR_REPEAT,      AVRCP_REPEAT_GROUP,     "Repeat",       "Group"},

        {AVRCP_PLAYER_ATTR_SHUFFLE,     AVRCP_SHUFFLE_OFF,      "Shuffle",      "OFF"},
        {AVRCP_PLAYER_ATTR_SHUFFLE,     AVRCP_SHUFFLE_ALL,      "Shuffle",      "All"},
        {AVRCP_PLAYER_ATTR_SHUFFLE,     AVRCP_SHUFFLE_GROUP,    "Shuffle",      "Group"},

        {AVRCP_PLAYER_ATTR_SCAN,        AVRCP_SCAN_OFF,         "Scan",         "OFF"},
        {AVRCP_PLAYER_ATTR_SCAN,        AVRCP_SCAN_ALL_TRACK,   "Scan",         "All"},
        {AVRCP_PLAYER_ATTR_SCAN,        AVRCP_SCAN_GROUP,       "Scan",         "Group"},
    };


    printf( "The AVRCP player setting of %02X:%02X:%02X:%02X:%02X:%02X is:\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5] );

    for( i = 0; i < player_settings->num_attr; i++ ){
        attr_id = player_settings->attr_ids[i];
        val_id = player_settings->attr_values[i];

        for( j = 0; j < sizeof(attribute_table_t) / sizeof(AVRCP_APP_ATTR_VALUE_TABLE); j++ ){
            if (attr_id == attribute_table_t[j].attr_id &&
                val_id == attribute_table_t[j].value_id){
                printf("%10s:%s\n", attribute_table_t[j].attr_name, attribute_table_t[j].value_name);
            }
        }
    }
    printf( "\n" );

}

static void FuncCallback_avrcp_set_player_app_value(BT_ADDR* addr)
{
    printf("The device %02X:%02X:%02X:%02X:%02X:%02X Set Player APP Value success\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
}

static void FuncCallback_avrcp_command_error(BT_ADDR *bd_addr, AVRCP_COMMAND_ID pdu,
		AVRCP_RSP_STATUS reason, AVRCP_RSP_CODE rsp_code, AVRCP_EVENT_ID event_id)
{
    printf("AVRCP command ");
    switch(pdu) {
    case AVRCP_CMD_GET_CAPABILITIES:
        printf("\"Get Capbilities\"");
        break;
    case AVRCP_CMD_LIST_PLAYER_APP_ATTR:
        printf("\"List Player App Attributes\"");
        break;
    case AVRCP_CMD_LIST_PLAYER_APP_VALUES:
        printf("\"List Player App Value\"");
        break;
    case AVRCP_CMD_GET_CUR_PLAYER_APP_VALUE:
        printf("\"Get Player APP Value\"" );
        break;
    case AVRCP_CMD_SET_PLAYER_APP_VALUE:
        printf("\"Set Player APP Value\"" );
        break;
    case AVRCP_CMD_GET_ELEMENT_ATTR:
        printf("\"Get Element Attributes\"" );
        break;
    case AVRCP_CMD_GET_PLAY_STATUS:
        printf("\"Get Play Status\"" );
        break;
    case AVRCP_CMD_REGISTER_NOTIFICATION:
        printf("\"Enable Notification\"" );
        break;
    case AVRCP_CMD_SET_ADDRESSED_PLAYER:
        printf("\"Set Address Player\"" );
        break;
    case AVRCP_CMD_SET_BROWSED_PLAYER:
        printf("\"Set Browsed Player\"" );
        break;
    case AVRCP_CMD_GET_FOLDER_ITEMS:
        printf("\"Get Folder Items\"" );
        break;
    case AVRCP_CMD_CHANGE_PATH:
        printf("\"Change Path\"" );
        break;
    case AVRCP_CMD_GET_ITEM_ATTRIBUTES:
        printf("\"Get Item Attributes\"" );
        break;
    case AVRCP_CMD_PLAY_ITEM:
        printf("\"Play Item\"" );
        break;
    default:
        printf("\"Unknown AVRCP Command 0x%02X\"", pdu);
    }
    printf(" send to device %02X:%02X:%02X:%02X:%02X:%02X fail.\n"
            "Remote Response is ",
            bd_addr->address[0],bd_addr->address[1],bd_addr->address[2],
            bd_addr->address[3],bd_addr->address[4],bd_addr->address[5]);

    switch(rsp_code) {
    case AVRCP_RSP_NO_RESPONSE:
        printf("\"No Response\"\n");
        break;
    case AVRCP_RSP_NOT_IMPLEMENTED:
        printf("\"Not Implemented\"\n");
        break;
    case AVRCP_RSP_ACCEPTED:
        printf("\"Accepted\"\n");
        break;
    case AVRCP_RSP_REJECTED:
        printf("\"Reject\"\n");
        break;
    case AVRCP_RSP_IN_TRANSACTION:
        printf("\"In Transaction\"\n");
        break;
    case AVRCP_RSP_STABLE:
        printf("\"Stable\"\n");
        break;
    case AVRCP_RSP_CHANGED:
        printf("\"Changed\"\n");
        break;
    case AVRCP_RSP_INTERIM:
        printf("\"Interim\"\n");
        break;
    default:
        printf("Unknown Response Code %d\n", rsp_code);
        break;
    }

    printf("Reason is ");
    switch(reason) {
    case AVRCP_BAD_CMD:
        printf("Invalid Command\n");
        break;
    case AVRCP_BAD_PARAM:
        printf("Invalid Parameter\n");
        break;
    case AVRCP_NOT_FOUND:
        printf("Specified Parameter is Wrong or Not Found\n");
        break;
    case AVRCP_INTERNAL_ERR:
        printf("Internal Error\n");
        break;
    case AVRCP_NO_ERROR:
        printf("Operation Success\n");
        break;
    case AVRCP_UID_CHANGED:
        printf("UID Changed\n");
        break;
    case AVRCP_BAD_DIR:
        printf("Invalid Direction\n");
        break;
    case AVRCP_NOT_DIR:
        printf("Not a Directory\n");
        break;
    case AVRCP_NOT_EXIST:
        printf("Does Not Exist\n");
        break;
    case AVRCP_BAD_SCOPE:
        printf("Invalid Scope \n");
        break;
    case AVRCP_BAD_RANGE:
        printf("Range Out of Bounds\n");
        break;
    case AVRCP_UID_IS_DIR:
        printf("UID is a Directory\n");
        break;
    case AVRCP_IN_USE:
        printf("Media in Use\n");
        break;
    case AVRCP_NOW_LIST_FULL:
        printf("Now Playing List Full\n");
        break;
    case AVRCP_SEARCH_NOT_SUP:
        printf("Search Not Supported\n");
        break;
    case AVRCP_SEARCH_BUSY:
        printf("Search in Progress\n");
        break;
    case AVRCP_BAD_PLAYER_ID:
        printf("Invalid Player Id\n");
        break;
    case AVRCP_PLAYER_N_BR:
        printf("Player Not Browsable\n");
        break;
    case AVRCP_PLAYER_N_ADDR:
        printf("Player Not Addressed\n");
        break;
    case AVRCP_BAD_SEARCH_RES:
        printf("No valid Search Results\n");
        break;
    case AVRCP_NO_AVAL_PLAYER:
        printf("No available players ALL\n");
        break;
    case AVRCP_ADDR_PLAYER_CHG:
        printf("Addressed Player Changed\n");
        break;
    default:
        printf("Unknown Reason Code %d\n", reason);
        break;
    }

    printf("\n");
}

static void FuncCallback_avrcp_passthrough_rsp(BT_ADDR *bd_addr, AVRCP_OP_ID key_code,
                            AVRCP_RSP_CODE rsp_code)
{
    printf("Press Key ");
    switch(key_code) {
    case AVRCP_OPID_VOL_UP:
        printf("\"Volume up\"");
        break;
    case AVRCP_OPID_VOL_DOWN:
        printf("\"Volume down\"");
        break;
    case AVRCP_OPID_MUTE:
        printf("\"Mute\"");
        break;
    case AVRCP_OPID_PLAY:
        printf("\"Play\"");
        break;
    case AVRCP_OPID_STOP:
        printf("\"Stop\"");
        break;
    case AVRCP_OPID_PAUSE:
        printf("\"Pause\"");
        break;
    case AVRCP_OPID_REWIND:
        printf("\"Rewind\"");
        break;
    case AVRCP_OPID_FAST_FORWARD:
        printf("\"Fast Forward\"");
        break;
    case AVRCP_OPID_FORWARD:
        printf("\"Forward\"");
        break;
    case AVRCP_OPID_BACKWARD:
        printf("\"Backward\"");
        break;
    default:
        printf("Unknown Key Code");
        break;
    }
    printf(" to device %02X:%02X:%02X:%02X:%02X:%02X\n",
            bd_addr->address[0],bd_addr->address[1],bd_addr->address[2],
            bd_addr->address[3],bd_addr->address[4],bd_addr->address[5]);

    printf("Remote Response is ");
    switch(rsp_code) {
    case AVRCP_RSP_NO_RESPONSE:
        printf("\"No Response\"\n");
        break;
    case AVRCP_RSP_NOT_IMPLEMENTED:
        printf("\"Not Implemented\"\n");
        break;
    case AVRCP_RSP_ACCEPTED:
        printf("\"Accepted\"\n");
        break;
    case AVRCP_RSP_REJECTED:
        printf("\"Reject\"\n");
        break;
    case AVRCP_RSP_IN_TRANSACTION:
        printf("\"In Transaction\"\n");
        break;
    case AVRCP_RSP_STABLE:
        printf("\"Stable\"\n");
        break;
    case AVRCP_RSP_CHANGED:
        printf("\"Changed\"\n");
        break;
    case AVRCP_RSP_INTERIM:
        printf("\"Interim\"\n");
        break;
    default:
        printf("Unknown Response Code %d\n", rsp_code);
    }

    printf("\n");
}

static void FuncCallback_avrcp_setaddressedplayer(BT_ADDR* addr)
{
    printf("The device %02X:%02X:%02X:%02X:%02X:%02X Set Addressed Player success\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
}

static void FuncCallback_avrcp_setbrowsedplayer(BT_ADDR *addr, AVRCP_BROWSED_FOLDER *folder)
{
    printf( "The AVRCP browse command set browsed player on player "
            "send to %02X:%02X:%02X:%02X:%02X:%02X is successful.\n"
            "The browsed folder attribute is:\n"
            "\tUID counter:\t\t%hu\n"
            "\tCharset id:\t\t%hu\n"
            "\tNumber of items:\t%hu\n"
            "\tFolder depth:\t\t%d\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5],
            folder->uid_counter, folder->charset_id, folder->num_items,
            folder->folder_depth );

    if (folder->folder_depth > 0) {
        printf("\tFolder name:\t\t%s\n", folder->name);
    }

    printf("\n");
}

static void FuncCallback_avrcp_changepath(BT_ADDR* addr)
{
    printf("The device %02X:%02X:%02X:%02X:%02X:%02X Change Path success\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
}

static void FuncCallback_avrcp_playeritem(BT_ADDR *addr, AVRCP_PLAYERITEM *item)
{
    printf( "The AVRCP browse command get player item from %02X:%02X:%02X:%02X:%02X:%02X"
            " is successful. The player item attribute is:\n"
            "\tUID counter:\t%hu\n"
            "\tPlayer id:\t%hu\n"
            "\tType:\t\t%d\n"
            "\tSubtype:\t%d\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5],
            item->uid_counter, item->player_id,
            item->type, item->subtype );

    printf( "\tCharset id:\t%hu\n"
            "\tPlayer name:\t%s\n"
            "\tPlay status:\t",
            item->charset_id, item->name );

    switch( item->play_status_id ){
    case AVRCP_PLAYSTATE_STOPPED:
        printf( "stopped.\n\n" );
        break;
    case AVRCP_PLAYSTATE_PLAYING:
        printf( "playing.\n\n" );
        break;
    case AVRCP_PLAYSTATE_PAUSED:
        printf( "paused.\n\n" );
        break;
    case AVRCP_PLAYSTATE_FWD_SEEK:
        printf( "forwarded.\n\n" );
        break;
    case AVRCP_PLAYSTATE_REV_SEEK:
        printf( "rewound.\n\n" );
        break;
    default:
        printf( "in error.\n\n" );
        break;
    }
}

static void FuncCallback_avrcp_folderitem(BT_ADDR *addr, AVRCP_FOLDERITEM *item)
{
    printf( "The AVRCP browse command get folder item from %02X:%02X:%02X:%02X:%02X:%02X"
            " is successful. The folder item attribute is:\n"
            "\tUID counter:\t%hu\n"
            "\tUID:\t\t%llu\n"
            "\tFolder type:\t%d\n"
            "\tPlayable:\t%d\n"
            "\tCharset id:\t%hu\n"
            "\tFolder name:\t%s\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5],
            item->uid_counter, (long long)item->uid, item->type,
            item->playable, item->charset_id, item->name );
}

static void FuncCallback_avrcp_mediaitem(BT_ADDR *addr, AVRCP_MEDIAITEM *item)
{
    printf( "The AVRCP browse command get media item from %02X:%02X:%02X:%02X:%02X:%02X"
            " is successful. The media item attribute is:\n"
            "\tUID counter:\t%hu\n"
            "\tUID:\t\t%llu\n"
            "\tMedia type:\t%d\n"
            "\tCharset id:\t%hu\n"
            "\tMedia name:\t%s\n"
            "\tMedia attribute:\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5],
            item->uid_counter, (long long)item->uid,
            item->type, item->charset_id, item->name );

    FuncCallback_avrcp_element_attribute(addr, item->attr);
}

static void FuncCallback_avrcp_getitem_success(BT_ADDR *addr, int count)
{
    printf( "The AVRCP browse command get folder items from %02X:%02X:%02X:%02X:%02X:%02X"
            " is successful, total item count %hu.\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5], count );
}

static void FuncCallback_avrcp_getitemattr(BT_ADDR *addr, AVRCP_UID uid, AVRCP_ELEMENT_ATTR_STRUCT *element_attrs)
{
    printf( "The AVRCP browse command get item attribute from %02X:%02X:%02X:%02X:%02X:%02X"
            " is successful. The media item attribute is:\n"
            "\tUID:\t\t%llu\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5],
            (long long)uid);

    FuncCallback_avrcp_element_attribute(addr, element_attrs);
}

static void FuncCallback_avrcp_playitem(BT_ADDR* addr)
{
    printf("The device %02X:%02X:%02X:%02X:%02X:%02X Play Item success\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
}

/*  PBAP functional*/
static void FuncCallback_pb_contacts_info(BT_ADDR *addr, PB_STRUCT *pbstruct)
{
    int i;
    printf("The downloaded phone book of %02X:%02X:%02X:%02X:%02X:%02X is:\n",
                addr->address[0], addr->address[1], addr->address[2],
                addr->address[3], addr->address[4], addr->address[5]);
    printf("  Contact ID:\t\t%d\n", pbstruct->contacts_id);
    printf("  Full Name:\t\t%s\n", pbstruct->full_name);
    printf("  First Name:\t\t%s\n", pbstruct->first_name);
    printf("  Middle Name:\t\t%s\n", pbstruct->middle_name);
    printf("  Last Name:\t\t%s\n", pbstruct->last_name);
    printf("  Prefix Name:\t\t%s\n", pbstruct->prefix_name);
    printf("  Suffix Name:\t\t%s\n", pbstruct->suffix_name);
    printf("  First Name Phonetic:\t\t%s\n", pbstruct->first_name_phonetic);
    printf("  Last Name Phonetic:\t\t%s\n", pbstruct->last_name_phonetic);
    printf("  Phone Count:\t\t%d\n", pbstruct->number_count);
    for( i = 0; i < pbstruct->number_count; i++ ){
        printf( "  Phone:\t\t" );
        switch(pbstruct->phone_number[i].type) {
        case PB_PREF:
            break;
        case PB_WORK:
            printf( "WORK:");
            break;
        case PB_HOME:
            printf( "HOME:");
            break;
        case PB_VOICE:
            printf( "VOICE:");
            break;
        case PB_FAX:
            printf( "FAX:");
            break;
        case PB_MSG:
            printf( "MSG:");
            break;
        case PB_CELL:
            printf( "CELL:");
            break;
        case PB_PAGER:
            printf( "PAGER:");
            break;
        default:
            break;
        }
        printf("%s\n", pbstruct->phone_number[i].number);
    }
    printf("  Time Stemp:\t\t%s\n", pbstruct->timestemp);
    switch(pbstruct->contact_type) {
    case PB_CONTACT_SIM:
        printf("  Contact Type:\t\tSIM\n");
        break;
    case PB_CONTACT_PHONE:
        printf("  Contact Type:\t\tPhone\n");
        break;
    case PB_CALLLOG_MISSED:
        printf("  Contact Type:\t\tMissed Call\n");
        break;
    case PB_CALLLOG_RECEIVED:
        printf("  Contact Type:\t\tReceived Call\n");
        break;
    case PB_CALLLOG_DIALED:
        printf("  Contact Type:\t\tDialed Call\n");
        break;
    default:
        printf("  Contact Type:\t\tUnknown Calllog Type\n");
        break;
    }
    printf("  Photo length:\t\t%d\n", pbstruct->photo.len);

    if (pbstruct->photo.len > 0)
        printf("  Photo file saved in\t\"%s\"\n\n", pbstruct->photo.path);
}

static void FuncCallback_pb_download_states(BT_ADDR *addr,PB_DOWNLOAD_STATES states)
{
    printf("Device [%02X:%02X:%02X:%02X:%02X:%02X] ",
            addr->address[0], addr->address[1], addr->address[2],
            addr->address[3], addr->address[4], addr->address[5]);
    switch(states)
    {
    case NFBT_PBDL_START:
        printf("Phone book download START\n");
        break;
    case NFBT_PBDL_DLING:
        printf("Phone book downloading...\n");
        break;
    case NFBT_PBDL_END:
        printf("Phone book download END\n");
        break;
    case NFBT_PBDL_FAIL:
        printf("Phone book download Fail \n");
        break;
    case NFBT_PBDL_COMPLETE:
        printf("Phone book download Complete\n");
		break;
    case NFBT_PBDL_CANCEL:
        printf("Phone book download Cancel\n");
		break;
    case NFBT_PBDL_NOTFIND:
    	printf("Phone book download Remote not find\n");
        break;
    default:
        printf("Phone book download Unknown State\n");
        break;
    }

    printf("\n");
}

static void FuncCallback_pb_total_contact(PB_ACCESS access, nfUINT16 total_contact, BT_ADDR *addr)
{
    switch(access)
    {
    case PB_ACCESS_SIM:
        printf("SIM card phone book has %d contacts\n", total_contact);
        break;
    case PB_ACCESS_PHONE:
        printf("Cell phone phone book has %d contacts\n", total_contact);
        break;
    case PB_ACCESS_MISSED:
        printf("Missed call log has %d contacts\n", total_contact);
        break;
    case PB_ACCESS_RECEIVED:
        printf("Received call log has %d contacts\n", total_contact);
        break;
    case PB_ACCESS_DIALED:
        printf("Dialed call log has %d contacts\n", total_contact);
        break;
    case PB_ACCESS_COMBINE_CALLLOG:
        printf("All call log has %d contacts\n", total_contact);
        break;
    default:
        printf("Unknown phone book access type %d\n", access);
        break;
    }

    printf("\n");
}

/*  MAP functional*/
static void FuncCallback_map_download_state(BT_ADDR *addr , MAP_DOWNLOAD_STATES state)
{
    printf("Device [%02X:%02X:%02X:%02X:%02X:%02X] ",
            addr->address[0], addr->address[1], addr->address[2],
            addr->address[3], addr->address[4], addr->address[5]);

    switch(state)
    {
    case MAP_DOWNLOAD_START:
        printf("Message download START\n");
        break;
    case MAP_DOWNLOAD_DLING:
        printf("Message downloading...\n");
        break;
    case MAP_DOWNLOAD_END:
        printf("Message download END\n");
        break;
    case MAP_DOWNLOAD_FAIL:
        printf("Message download Fail \n");
        break;
    case MAP_DOWNLOAD_COMPLETE:
        printf("Message download Complete \n");
        break;
    default:
        printf("Message download Unknown State\n");
        break;
    }

    printf("\n");
}

static void FuncCallback_map_notification_state(BT_ADDR *addr , MAP_NOTIFICATION_STATES state)
{
    printf("Remote Device [%02X:%02X:%02X:%02X:%02X:%02X] ",
            addr->address[0], addr->address[1], addr->address[2],
            addr->address[3], addr->address[4], addr->address[5]);

    switch(state)
    {
    case MAP_NOTIFICATION_OFF:
        printf("Unregistered Message Notification\n");
        break;
    case MAP_NOTIFICATION_ON:
        printf("Registered Message Notification\n");
        break;
    default:
        printf("Unknown Message Notification State\n");
        break;
    }

    printf("\n");
}

static void FuncCallback_map_total_message(BT_ADDR *addr, MAP_MSG_TYPE type, MAP_FOLDER_STRUCT folder, nfUINT8 total)
{
    printf("Device %02X:%02X:%02X:%02X:%02X:%02X ",
                addr->address[0], addr->address[1], addr->address[2],
                addr->address[3], addr->address[4], addr->address[5]);

    switch (type) {
    case MAP_MSG_TYPE_SMS_GSM:
        printf("message type SMS_GSM ");
        break;
    case MAP_MSG_TYPE_SMS_CDMA:
        printf("message type SMS_CDMA ");
        break;
    case MAP_MSG_TYPE_MMS:
        printf("message type MMS ");
        break;
    case MAP_MSG_TYPE_EMAIL:
        printf("message type EMAIL ");
        break;
    default:
        printf("message type Unknown Type ");
        break;
    }

    switch (folder) {
    case MAP_FOLDER_STRUCTURE_INBOX:
        printf("in Input Box ");
        break;
    case MAP_FOLDER_STRUCTURE_SENT:
        printf("in Sent Box ");
        break;
    case MAP_FOLDER_STRUCTURE_DELETED:
        printf("in Deleted Box ");
        break;
    case MAP_FOLDER_STRUCTURE_OUTBOX:
        printf("in Output Box ");
        break;
    case MAP_FOLDER_STRUCTURE_DRAFT:
        printf("in Draft Box ");
        break;
    default:
        printf("in Unknown Box ");
        break;
    }

    printf("has %d message.\n\n", total);
}

static void FuncCallback_map_receive_message(BT_ADDR *addr, MAP_SMS_GSM *message,
                nfBOOLEAN is_new)
{
    printf("The downloaded message of %02X:%02X:%02X:%02X:%02X:%02X is:\n",
                addr->address[0], addr->address[1], addr->address[2],
                addr->address[3], addr->address[4], addr->address[5]);

    if (is_new)
        printf("  This message is NEW message.\n");

    printf("  Current Count:\t%d\n", message->curr_count);
    printf("  Message read:\t\t%s\n", message->read_status ? "True" : "False");
    printf("  Message Handle:\t%s\n", message->msg_handle);
    printf("  Sender Name:\t\t%s\n", message->sender_name);
    printf("  Sender Address:\t%s\n", message->sender_addr);
    printf("  Recipient Address:\t%s\n", message->recipient_addr);
    printf("  Date:\t\t\t%s\n", message->date);

    printf("  Message Type:\t\t");
    switch(message->msg_type) {
    case MAP_MSG_TYPE_SMS_GSM:
        printf("\"GSM Short Message Service\"\n");
        break;
    case MAP_MSG_TYPE_SMS_CDMA:
        printf("\"CDMA Short Message Service\"\n");
        break;
    case MAP_MSG_TYPE_MMS:
        printf("\"Multimedia Messaging Service\"\n");
        break;
    case MAP_MSG_TYPE_EMAIL:
        printf("\"E-mail\"\n");
        break;
    default:
        printf("\"Unknown Type\"\n");
        break;
    }

    printf("  Folder Structure:\t");
    switch(message->folder) {
    case MAP_FOLDER_STRUCTURE_INBOX:
        printf("\"Received Box\"\n");
        break;
    case MAP_FOLDER_STRUCTURE_SENT:
        printf("\"Senting Box\"\n");
        break;
    case MAP_FOLDER_STRUCTURE_DELETED:
        printf("\"Deleted Box\"\n");
        break;
    case MAP_FOLDER_STRUCTURE_OUTBOX:
        printf("\"Output Box\"\n");
        break;
    case MAP_FOLDER_STRUCTURE_DRAFT:
        printf("\"Draft Box\"\n");
        break;
    default:
        printf("\"Unknown Type\"\n");
        break;
    }

    printf("  Subject:\t\t%s\n", message->subject);
    printf("  Message Detail:\n%s\n", message->msg);

    printf("\n");
}

static void FuncCallback_map_send_message(BT_ADDR *addr, nfUINT8 *tg_num,
                MAP_COMMAND_STATUS send_status)
{
    printf("Device %02X:%02X:%02X:%02X:%02X:%02X send message to %s ",
                addr->address[0], addr->address[1], addr->address[2],
                addr->address[3], addr->address[4], addr->address[5],
                tg_num);

    switch(send_status) {
    case MAP_STATUS_SUCCESS:
        printf("Success!\n");
        break;
    case MAP_STATUS_BAD_PARAMS:
        printf("Fail!\n\tReason : Bad Parameter.\n");
        break;
    case MAP_STATUS_TIME_OUT:
        printf("Fail!\n\tReason : Time Out.\n");
        break;
    case MAP_STATUS_FAIL:
        printf("Fail!\n\tReason : Unsupported\n");
        break;
    default:
        printf("Fail!\n\tReason : Unknown Handle Event.\n");
        break;
    }

    printf("\n");
}

static void FuncCallback_map_delete_message(BT_ADDR *addr, nfUINT8 *msg_handle,
                MAP_COMMAND_STATUS reason)
{
    printf("Device %02X:%02X:%02X:%02X:%02X:%02X delete handle %s message ",
                addr->address[0], addr->address[1], addr->address[2],
                addr->address[3], addr->address[4], addr->address[5],
                msg_handle);

    switch(reason) {
    case MAP_STATUS_SUCCESS:
        printf("Success!\n");
        break;
    case MAP_STATUS_BAD_PARAMS:
        printf("Fail!\n\tReason : \"Bad handle\"\n");
        break;
    case MAP_STATUS_TIME_OUT:
        printf("Fail!\n\tReason : \"Time out\"\n");
        break;
    case MAP_STATUS_FAIL:
        printf("Fail!\n\tReason : \"Unknown reason\"\n");
        break;
    default:
        break;
    }

    printf("\n");
}

static void FuncCallback_map_change_read_status(BT_ADDR *addr, nfUINT8 *msg_handle,
                MAP_COMMAND_STATUS reason)
{
    printf("Device %02X:%02X:%02X:%02X:%02X:%02X change handle %s message read status ",
                addr->address[0], addr->address[1], addr->address[2],
                addr->address[3], addr->address[4], addr->address[5],
                msg_handle);

    switch(reason) {
    case MAP_STATUS_SUCCESS:
        printf("Success!\n");
        break;
    case MAP_STATUS_BAD_PARAMS:
        printf("Fail!\n\tReason : \"Bad handle\"\n");
        break;
    case MAP_STATUS_TIME_OUT:
        printf("Fail!\n\tReason : \"Time out\"\n");
        break;
    case MAP_STATUS_FAIL:
        printf("Fail!\n\tReason : \"Unknown reason\"\n");
        break;
    default:
        break;
    }

    printf("\n");
}

static void FuncCallback_map_notification_event(BT_ADDR *addr, MAP_EVENT_REPORT event, nfUINT8 *msg_handle, MAP_MSG_TYPE type, MAP_FOLDER_STRUCT folder)
{
    printf("Device %02X:%02X:%02X:%02X:%02X:%02X change handle %s message event: ",
                addr->address[0], addr->address[1], addr->address[2],
                addr->address[3], addr->address[4], addr->address[5],
                msg_handle);

    switch(event) {
    case MAP_EVENT_NEW_MESSAGE:
        printf("New Message!\n");
        break;
    case MAP_EVENT_DELIVERY_SUCCESS:
        printf("Delivery Success!\n");
        break;
    case MAP_EVENT_SENDING_SUCCESS:
        printf("Sending Success!\n");
        break;
    case MAP_EVENT_DELIVERY_FAILURE:
        printf("Delivery Failure!\n");
        break;
    case MAP_EVENT_SENDING_FAILURE:
        printf("Sending Failure!\n");
        break;
    case MAP_EVENT_MEMORY_FULL:
        printf("Memory Full!\n");
        break;
    case MAP_EVENT_MEMORY_AVAILABLE:
        printf("Memory Available!\n");
        break;
    case MAP_EVENT_MESSAGE_DELETED:
        printf("Message Delete!\n");
        break;
    case MAP_EVENT_MESSAGE_SHIFT:
        printf("Message Shift!\n");
        break;
    default:
        break;
    }
}

/*RFCOMM funcational*/
static void *thread_rfcomm_read(void *data)
{
    int read_len = RFCOMM_DATA_LEN;
    int ret_len = -1, j = 0;
    int timeout = 100;  //100ms
    unsigned char rfcomm_buf[RFCOMM_DATA_LEN]={0};
    PROFILE_ID mask = NFBT_PID_SPP;

    (void)(data);

	//pthread_setname_np(pthread_self(), (const char *)"rfcomm_read");

    while(rfcomm_connected > 0){
        mask = NFBT_PID_SPP;
        while(mask < NFBT_PID_END && mask <= rfcomm_connected) {
            if (mask & rfcomm_connected){
                memset(rfcomm_buf, 0, RFCOMM_DATA_LEN);
                ret_len = NFBT_RFCOMM_Read(mask, (void*)rfcomm_buf, read_len, timeout);
                if (ret_len > 0 ){

                    switch(mask)
                    {
                        case NFBT_PID_SPP:
                            printf( "Read data from SPP service\n" );
                            break;
                        case NFBT_PID_IAP:
                            printf( "Read data from iAP service\n" );
                            break;
                        case NFBT_PID_RFCOMM_1:
                            printf( "Read data from RFCOMM 1 service\n" );
                            break;
                        case NFBT_PID_RFCOMM_2:
                            printf( "Read data from RFCOMM 2 service\n" );
                            break;
                        case NFBT_PID_RFCOMM_3:
                            printf( "Read data from RFCOMM 3 service\n" );
                            break;
                        case NFBT_PID_RFCOMM_4:
                            printf( "Read data from RFCOMM 4 service\n" );
                            break;
                        case NFBT_PID_RFCOMM_5:
                            printf( "Read data from RFCOMM 5 service\n" );
                            break;
                        case NFBT_PID_RFCOMM_6:
                            printf( "Read data from RFCOMM 6 service\n" );
                            break;
                        case NFBT_PID_RFCOMM_7:
                            printf( "Read data from RFCOMM 7 service\n" );
                            break;
                        case NFBT_PID_RFCOMM_8:
                            printf( "Read data from RFCOMM 8 service\n" );
                            break;
                        case NFBT_PID_RFCOMM_9:
                            printf( "Read data from RFCOMM 9 service\n" );
                            break;
                        default:
                            printf( "Unknown RFCOMM service" );
                            break;
                    }

                    printf("Data in hex: ");
                    for(j=0; j<ret_len; j++)
                        printf("%02X, ",rfcomm_buf[j]);
                    printf("\n");

                    printf("Data in ASCII: ");
                    for(j=0; j<ret_len; j++)
                        printf("%c",rfcomm_buf[j]);
                    printf("\n");
                }
            }
            mask = mask << 1;
        }
        usleep(2000);
    }

    thread_id_rfcomm_read = 0;
    pthread_exit(NULL);

    return NULL;
}

static void FuncCallback_rfcomm_connect_states(BT_ADDR* addr, PROFILE_ID id, RFCOMM_STATES rfcomm_state)
{
    pthread_attr_t attr;
    int result;
    char rfcomm_str[25] = {0};
    switch(id)
    {
        case NFBT_PID_SPP:
            strcpy( rfcomm_str, "SPP" );
            break;
        case NFBT_PID_IAP:
            strcpy( rfcomm_str, "iAP" );
            break;
        case NFBT_PID_RFCOMM_1:
            strcpy( rfcomm_str, "RFCOMM_1" );
            break;
        case NFBT_PID_RFCOMM_2:
            strcpy( rfcomm_str, "RFCOMM_2" );
            break;
        case NFBT_PID_RFCOMM_3:
            strcpy( rfcomm_str, "RFCOMM_3" );
            break;
        case NFBT_PID_RFCOMM_4:
            strcpy( rfcomm_str, "RFCOMM_4" );
            break;
        case NFBT_PID_RFCOMM_5:
            strcpy( rfcomm_str, "RFCOMM_5" );
            break;
        case NFBT_PID_RFCOMM_6:
            strcpy( rfcomm_str, "RFCOMM_6" );
            break;
        case NFBT_PID_RFCOMM_7:
            strcpy( rfcomm_str, "RFCOMM_7" );
            break;
        case NFBT_PID_RFCOMM_8:
            strcpy( rfcomm_str, "RFCOMM_8" );
            break;
        case NFBT_PID_RFCOMM_9:
            strcpy( rfcomm_str, "RFCOMM_9" );
            break;
        default:
            strcpy( rfcomm_str, "Unknown RFCOMM service" );
            break;
    }
    switch(rfcomm_state)
    {
        case NFBT_RFCOMM_DISCONNECTED:
            printf( "%s is now disconnected to ", rfcomm_str);
            rfcomm_connected &= ~id;
            break;
        case NFBT_RFCOMM_CONNECTED:
            printf( "%s is now connected to ", rfcomm_str);
            rfcomm_connected |= id;
            break;
        default:
            printf( "unknown %s connect state to ", rfcomm_str);
            break;
    }
    printf("addr:[%02X:%02X:%02X:%02X:%02X:%02X]\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);

    if( thread_id_rfcomm_read == 0 &&
        rfcomm_connected > 0){
        if( pthread_attr_init( &attr ) != 0 )
            printf("pthread_attr_init failed.\n" );
        if( pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE ) != 0 )
            printf("pthread_attr_setdetachstate failed.\n" );

        result = pthread_create(&(thread_id_rfcomm_read), &attr,
                thread_rfcomm_read, NULL);

        pthread_attr_destroy( &attr );

        if (result != 0) {
            printf("system lacked the necessary resources to create RFCOMM read thread.\n" );
        }
    }
    if( thread_id_rfcomm_read != 0 &&
        rfcomm_connected == 0){
        pthread_join(thread_id_rfcomm_read, NULL);
    }
}

/*  HID functional*/
static void FuncCallback_hid_connection_states(BT_ADDR* addr,HID_CONN_STATES hid_conn_state)
{
    switch(hid_conn_state)
    {
        case NFBT_HID_STATE_DISCONNECTED:
            printf( "HID is now disconnected to ");
            break;
        case NFBT_HID_STATE_CONNECTING:
            printf( "HID is now connecting to ");
            break;
        case NFBT_HID_STATE_CONNECTED:
            printf( "HID is now connected to ");
            break;
        case NFBT_HID_STATE_DISCONNECTING:
            printf( "HID is now disconnecting to ");
            break;
        default:
            printf( "unknown HID connect state %d to ", hid_conn_state);
            break;
    }
    printf("addr:[%02X:%02X:%02X:%02X:%02X:%02X]\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);

}

/*  OPP functional*/
static void FuncCallback_opp_download_states(BT_ADDR *addr, OPP_STATES state)
{
    printf("Device [%02X:%02X:%02X:%02X:%02X:%02X] ",
            addr->address[0], addr->address[1], addr->address[2],
            addr->address[3], addr->address[4], addr->address[5]);

    switch(state)
    {
    case NFBT_OPP_START:
        printf("Object transfer START\n");
        break;
    case NFBT_OPP_DLING:
        printf("Object downloading...\n");
        break;
    case NFBT_OPP_ULING:
        printf("Object uploading...\n");
        break;
    case NFBT_OPP_END:
        printf("Object transfer END\n");
        opp_access_request = 0;
        break;
    case NFBT_OPP_COMPLETE:
        printf("Object transfer Complete\n");
        break;
    case NFBT_OPP_FAIL:
        printf("Object transfer Fail \n");
        break;
    default:
        printf("Object transfer Unknown State\n");
        break;
    }

    printf("\n");
}

static void FuncCallback_opp_access_request(BT_ADDR *addr, nfUINT8 *dev_name, OPP_FILE_INFO *file_info)
{
	int i = 0;

    for(i = 0; i < BT_ADDR_LEN; i++ ){
    	opp_remote_access_addr.address[i] = addr->address[i];
	}

	opp_access_request = 1;

    printf("Access file request: Remote device %s\n", dev_name);

    printf("Device [%02X:%02X:%02X:%02X:%02X:%02X] send a access %s(%s) request.\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5],
            file_info->file_name, file_info->file_type);

    printf( "Enter y or n to response the remote device file access request.\n");

}

static void FuncCallback_le_test_packet_num_recv(nfUINT16 number_of_packet)
{
    printf("LE test end: received %d number of packet.\n", number_of_packet);
}

static void FuncCallback_gatt_scan_result(BT_ADDR *addr, int rssi, nfUINT8 *adv_data)
{
	int i = 0;
	
	printf("Gatt scan result : [%02X:%02X:%02X:%02X:%02X:%02X],rssi=%d, data length=%d advertising data=",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5], rssi, adv_data[0]);
	for(i=0;i<adv_data[0];i++)
	{
		if(!(i%8))
			printf("\n");
		printf(",0x%02X", adv_data[i+1]);
	}
	printf("\n");
}

static void FuncCallback_gatt_search_complete_result(GATT_STATUS status)
{
	printf("Gatt search complete : status=%d\n", status);
}

static void FuncCallback_gatt_search_result(nfUINT8 *srvc_uuid, nfUINT8 is_primary)
{
	int i;
	printf("Gatt search result : is_primary=%d, service UUID=", is_primary);
	for(i=0;i<16;i++)
		printf("%02X", srvc_uuid[i]);
	printf("\n");
}

static void FuncCallback_gatt_get_include_service(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *inc_srvc_uuid)
{
	int i;

	printf("Gatt get include service status=%d\n", status);

	if(srvc_uuid)
	{
		printf("                   : service UUID=");
		for(i=0;i<16;i++)
			printf("%02X", srvc_uuid[i]);
		printf("\n");
	}

	if(inc_srvc_uuid)
	{
		printf("                   : include service UUID=");	
		for(i=0;i<16;i++)
			printf("%02X", inc_srvc_uuid[i]);
		printf("\n");
	}
}

static void FuncCallback_gatt_get_characteristic(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, int char_prop)
{
	int i;

	printf("Gatt get characteristic status=%d, property=0x%04x\n", status, char_prop);

	if(srvc_uuid)
	{
		printf("                   : service UUID=");
		for(i=0;i<16;i++)
			printf("%02X", srvc_uuid[i]);
		printf("\n");
	}

	if(char_uuid)
	{
		printf("                   : characteristic UUID=");
		for(i=0;i<16;i++)
			printf("%02X", char_uuid[i]);
		printf("\n");
	}
}

static void FuncCallback_gatt_get_descriptor(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, nfUINT8 *descr_uuid)
{
	int i;
	
	printf("Gatt get descriptor status=%d\n", status);

	if(srvc_uuid)
	{
		printf("                   : service UUID=");
		for(i=0;i<16;i++)
			printf("%02X", srvc_uuid[i]);
		printf("\n");
	}

	if(char_uuid)
	{
		printf("                   : characteristic UUID=");
		for(i=0;i<16;i++)
			printf("%02X", char_uuid[i]);
		printf("\n");
	}
	
	if(descr_uuid)
	{
		printf("                   : descriptor UUID=");
		for(i=0;i<16;i++)
			printf("%02X", descr_uuid[i]);
		printf("\n");
	}
}
	
static void FuncCallback_gatt_register_notify(GATT_STATUS status, int registered)
{
	printf("RegisterNotify(%d)=%d\n", registered, status);
}

static void FuncCallback_gatt_notify(BT_ADDR *addr, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, 
										int data_len, nfUINT8 *data)
{
	int i;
	printf("Gatt notify service\n");
	
	if(srvc_uuid)
	{
		printf("                   : service UUID=");
		for(i=0;i<16;i++)
			printf("%02X", srvc_uuid[i]);
		printf("\n");
	}

	if(char_uuid)
	{
		printf("                   : characteristic UUID=");	
		for(i=0;i<16;i++)
			printf("%02X", char_uuid[i]);
		printf("\n");
	}
	
	if(data)
	{
		printf("                   : data len=%d, data=", data_len);
		for(i=0;i<data_len;i++)
			printf("%02x", data[i]);
		printf("\n");
	}
}

static void FuncCallback_gatt_read_characteristic(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid,
										int data_type, int data_len, nfUINT8 *data)
{
	int i;
	printf("Gatt read characteristic status=%d\n", status);
	
	if(srvc_uuid)
	{
		printf("                   : service UUID=");
		for(i=0;i<16;i++)
			printf("%02X", srvc_uuid[i]);
		printf("\n");
	}
	
	if(char_uuid)
	{
		printf("                   : characteristic UUID=");
		for(i=0;i<16;i++)
			printf("%02X", char_uuid[i]);
		printf("\n");
	}	
	if(data)
	{
		printf("                   : data_type=%d, len=%d, data=", data_type, data_len);
		for(i=0;i<data_len;i++)
			printf("%02x", data[i]);
		printf("\n");
	}	
}

static void FuncCallback_gatt_write_characteristic(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid)
{
        int i;
	if(status != NFBT_GATT_SUCCESS && status != NFBT_GATT_BUSY)
	{
		printf("Write characteristic fail status=%d\n", status);
		return;
	}

	if(srvc_uuid)
	{
		printf("Gatt write characteristic status=%d\n", status);
		for(i=0;i<16;i++)
			printf("%02X", srvc_uuid[i]);
		printf("\n");
	}
	
	if(char_uuid)
	{
		printf("                   : characteristic UUID=\n");
		for(i=0;i<16;i++)
			printf("%02X", char_uuid[i]);
		printf("\n");
	}
}

static void FuncCallback_gatt_read_descriptor(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, nfUINT8 *descr_uuid,
										int data_type, int data_len, nfUINT8 *data)
{
	int i;
	printf("Gatt read descriptor status=%d\n", status);
	
	if(srvc_uuid)
	{
		printf("                   : service UUID=");
		for(i=0;i<16;i++)
			printf("%02X", srvc_uuid[i]);
		printf("\n");
	}
	
	if(char_uuid)
	{
		printf("                   : characteristic UUID=");
		for(i=0;i<16;i++)
			printf("%02X", char_uuid[i]);
		printf("\n");
	}

	if(descr_uuid)
	{
		printf("                   : descriptor UUID=");
		for(i=0;i<16;i++)
			printf("%02X", descr_uuid[i]);
		printf("\n");
	}	
	
	if(data)
	{
		printf("                   : datatype=%d, len=%d, data=", data_type, data_len);
		for(i=0;i<data_len;i++)
			printf("%02x", data[i]);
		printf("\n");
	}	
}

static void FuncCallback_gatt_write_descriptor(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, nfUINT8 *descr_uuid)
{
	int i;
	if(status != NFBT_GATT_SUCCESS && status != NFBT_GATT_BUSY)
	{
		printf("Write descriptor fail status=%d\n", status);
		return;
	}

	printf("Gatt write descriptor status=%d\n", status);
	if(srvc_uuid)
	{
		printf("                   : service UUID=");
		for(i=0;i<16;i++)
			printf("%02X", srvc_uuid[i]);
		printf("\n");
	}
	
	if(char_uuid)
	{
		printf("                   : characteristic UUID=");
		for(i=0;i<16;i++)
			printf("%02X", char_uuid[i]);
		printf("\n");
	}
	
	if(descr_uuid)
	{
		printf("                   : descriptor UUID=");
		for(i=0;i<16;i++)
			printf("%02X", descr_uuid[i]);
		printf("\n");
	}
	
}

static void FuncCallback_gatt_read_rssi(GATT_STATUS status, BT_ADDR *addr, int rssi)
{
	printf("Gatt read rssi :  [%02X:%02X:%02X:%02X:%02X:%02X], status=%d, rssi=%d\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5], status, rssi);
}

static void FuncCallback_gatt_set_adv_data_result(GATT_STATUS status)
{
	printf("Gatt advertising set data : status=%d\n", status);
}

static void FuncCallback_gatt_client_register(GATT_STATUS status)
{
	printf("Client Register status=%d\n", status);
}
static void FuncCallback_gatt_service_added(GATT_STATUS status, nfUINT8 *srvc_uuid, int srvc_handle)
{
	int i;
	printf("Gatt service added : status=%d srvc_handle=%d\n", status, srvc_handle);
	printf("                   : service UUID=");
	for(i=0;i<16;i++)
		printf("%02X", srvc_uuid[i]);
	printf("\n");

	#ifdef GATT_SPP_DEMO
	if(status == NFBT_GATT_SUCCESS)
		gatt_store_service_info(srvc_handle, (char*)srvc_uuid);

	if(demo_service_reg.status != SRV_REG_NONE)
	{
		if(status == NFBT_GATT_SUCCESS)
		{
			demo_service_reg.srv_handle = srvc_handle;
			demo_service_reg.status = SRV_ATTR_REG;
		}
		else
		{
			demo_service_reg.status = SRV_REG_FAIL;
		}
		gatt_service_register();
	}
	#endif /* GATT_SPP_DEMO */
}

static void FuncCallback_gatt_char_added(GATT_STATUS status, nfUINT8 *char_uuid, int srvc_handle, int char_handle)
{
	int i;
	printf("Gatt characteristic added status : status=%d, srvc_handle=%d, char. handle=%d\n", status, srvc_handle, char_handle);

	if(char_uuid)
	{
		printf("                   : characteristic UUID=");
		for(i=0;i<16;i++)
			printf("%02X", char_uuid[i]);
		printf("\n");
	}

	#ifdef GATT_SPP_DEMO
	if(status == NFBT_GATT_SUCCESS)
		gatt_store_attribute_info(srvc_handle, char_handle, ATTRTYPE_CHARACTERISTIC, (char*)char_uuid);

	if(demo_service_reg.status != SRV_REG_NONE)
	{
		if(status == NFBT_GATT_SUCCESS)
		{
			demo_service_reg.srv_handle = srvc_handle;
			demo_service_reg.status = SRV_ATTR_REG;
		}
		else
		{
			demo_service_reg.status = SRV_REG_FAIL;
		}
		gatt_service_register();
	}
	#endif
}

static void FuncCallback_gatt_des_added(GATT_STATUS status, nfUINT8 *descr_uuid, int srvc_handle, int descr_handle)
{
	int i;
	printf("Gatt descriptor added : status=%d, srvc_handle=%d, descr_handle=%d\n", status, srvc_handle, descr_handle);

	if(descr_uuid)
	{
		printf("                   : descriptor UUID=");
		for(i=0;i<16;i++)
			printf("%02X", descr_uuid[i]);
		printf("\n");
	}

	#ifdef GATT_SPP_DEMO
	if(status == NFBT_GATT_SUCCESS)
		gatt_store_attribute_info(srvc_handle, descr_handle, ATTRTYPE_DESCRIPTOR, (char*)descr_uuid);

	if(demo_service_reg.status != SRV_REG_NONE)
	{
		if(status == NFBT_GATT_SUCCESS)
		{
			demo_service_reg.srv_handle = srvc_handle;
			demo_service_reg.status = SRV_ATTR_REG;
		}
		else
		{
			demo_service_reg.status = SRV_REG_FAIL;
		}
		gatt_service_register();
	}
	#endif /* GATT_SPP_DEMO */
}

static void FuncCallback_gatt_service_start(GATT_STATUS status, int srvc_handle)
{
	printf("Gatt service start : status=%d, srvc_handle=%d\n", status, srvc_handle);
}

static void FuncCallback_gatt_service_stop(GATT_STATUS status, int srvc_handle)
{
	printf("Gatt service stop : status=%d, srvc_handle=%d\n", status, srvc_handle);

	#ifdef GATT_SPP_DEMO
	if(demo_service_reg.status != SRV_REG_NONE)
	{
		if(status == NFBT_GATT_SUCCESS)
		{
			demo_service_reg.srv_handle = srvc_handle;
			demo_service_reg.status = SRV_UNREG;
		}
		else
		{
			demo_service_reg.status = SRV_REG_FAIL;
		}
		gatt_service_register();
	}
	#endif /* GATT_SPP_DEMO */
}

static void FuncCallback_gatt_service_deleted(GATT_STATUS status, int srvc_handle)
{
	printf("Gatt service deleted : status=%d, srvc_handle=%d\n", status, srvc_handle);

	#ifdef GATT_SPP_DEMO
	if(status == NFBT_GATT_SUCCESS)
	{
		status = gatt_delete_service_info(srvc_handle);
	}

	if(demo_service_reg.status != SRV_REG_NONE)
	{
		if(status == NFBT_GATT_SUCCESS)
		{
			demo_service_reg.status = SRV_REG_NONE;
		}
		else
		{
			demo_service_reg.status = SRV_REG_FAIL;
		}
		gatt_service_register();
	}
	#endif /* GATT_SPP_DEMO */
}

static void FuncCallback_gatt_request_read_event(BT_ADDR *addr, int trans_id, int attr_handle, int offset, int is_long)
{
	const char resp[2] = {0x01, 0x00};
	printf("Gatt Request read event :  [%02X:%02X:%02X:%02X:%02X:%02X], trans_id=%d, attr_handle=%d, offset=%d, is_long=%d\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5], trans_id, attr_handle, offset, is_long);

	NFBT_GATT_SendResponse(trans_id, NFBT_GATT_SUCCESS, attr_handle, attr_handle, (char*)resp, sizeof(resp));
}

static void FuncCallback_gatt_request_write_event(BT_ADDR *addr, int trans_id, int attr_handle, int offset, int is_prep, nfUINT8 *data, int length, int need_rsp)
{
	int i;
	if(addr)
	{
		printf("Gatt Request write event :  [%02X:%02X:%02X:%02X:%02X:%02X], trans_id=%d, attr_handle=%d, offset=%d, is_prep=%d, length=%d, need_rsp=%d\n",
       		addr->address[0],addr->address[1],addr->address[2],
            		addr->address[3],addr->address[4],addr->address[5], trans_id, attr_handle, offset, is_prep, length, need_rsp);
	}
	if(data)
	{
		printf("                   : data=");
		for(i=0;i<length;i++)
		{
			printf("%02X", data[i+offset]);
		}
		printf("\n");
	}

	if(need_rsp)
	{
		const char resp[2] = {0x01, 0x00};
		NFBT_GATT_SendResponse(trans_id, NFBT_GATT_SUCCESS, attr_handle, attr_handle, (char*)resp, sizeof(resp));
	}
}

static void FuncCallback_gatt_request_exec_write_event(BT_ADDR *addr, int trans_id, int exec_write)
{
	printf("Gatt Request execute write event :  [%02X:%02X:%02X:%02X:%02X:%02X], trans_id=%d, exec_write=%d\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5], trans_id, exec_write);
}

static void FuncCallback_gatt_response_confirm(GATT_STATUS status)
{
	/*printf("Gatt response confirm : status=%d\n", status);*/
}

static void FuncCallback_gatt_indicator_sent(GATT_STATUS status)
{
	printf("Gatt indicator : status=%d\n", status);

	/* write next packet or operate complete */
}
static void FuncCallback_gatt_congestion(int congested)
{
	printf("Gatt congestion : status=%d\n", congested);

	if(congested == 0)
	{/* write next packet or operate complete */
	}
	else if(congested == 1)
	{/* do not write next packet until congested = 0 */
	}
}

static void FuncCallback_gatt_connect_status(GATT_CONN_STATUS status, BT_ADDR *addr, GATT_ROLE role)
{
	switch(status)
	{
	    case GATT_CONN_DISCONNECT:
	        printf( "GATT is now disconnected to ");
	        break;
	    case GATT_CONN_START_LISTENING:
	        printf( "GATT is now prepare listening to ");
	        break;
	    case GATT_CONN_LISTENING:
	        printf( "GATT is now listening to ");
	        break;
	    case GATT_CONN_CONNECTING:
	        printf( "GATT is now connecting to ");
	        break;
	    case GATT_CONN_CONNECTED:
	        printf( "GATT is now connected to ");
		break;
	    default:
	        printf( "unknown HFP connect state to ");
	        break;
	}

	if(addr)
	{
		printf("addr: [%02X:%02X:%02X:%02X:%02X:%02X], role=%s\n",
	            addr->address[0],addr->address[1],addr->address[2],
	            addr->address[3],addr->address[4],addr->address[5], role?"Central":"Peripheral");
	}
	else
		printf("role=%s\n", role?"Central":"Peripheral");
		
}

static void FuncCallback_gatt_set_role_result(GATT_STATUS status, GATT_ROLE role)
{
	printf("Gatt set role : status=%d, role=%d\n", status, role);
}

static void FuncCallback_gatt_attribute_list(int handle, nfUINT8 *uuid, int property, int type)
{
	int i;
	
	printf("Character list : hdl=%d, property=0x%04x, type=%d ", handle, property, type);
	if(uuid)
	{
		printf("uuid=");
		for(i=0;i<16;i++)
			printf("%02X", uuid[i]);
	}
	printf("\n");
}

static void FuncCallback_gatt_service_list(int handle, nfUINT8 *uuid)
{
	int i;
	
	printf("Service list hdl=%d ", handle);
	if(uuid)
	{
		printf("uuid=");
		for(i=0;i<16;i++)
			printf("%02X", uuid[i]);
	}
	printf("\n");
}

static void FuncCallback_gatt_list_complete(GATT_STATUS status)
{
	printf("List complete\n");
}

static void FuncCallback_gatt_service_register(GATT_STATUS status)
{
	printf("Service Register status=%d\n", status);
}

static void FuncCallback_gatt_service_unregister(GATT_STATUS status)
{
	printf("Service unRegister status=%d\n", status);
}

static void FuncCallback_gatt_battery_level_read_event(int trans_id, int attr_handle)
{
	printf("Battery level read request\n");
	
	const char resp[1] = {80}; /* read battery level, range is 0~100 */
	NFBT_GATT_SendResponse(trans_id, NFBT_GATT_SUCCESS, attr_handle, attr_handle, (char*)resp, sizeof(resp));
}

static void FuncCallback_gatt_blood_pressure_feature_read_event(int trans_id, int attr_handle)
{
	printf("Blood pressure feature read request\n");
	
	const char resp[2] = {BPM_FEATURE_SUPPORT&0xff, (BPM_FEATURE_SUPPORT>>8)&0xff};
	NFBT_GATT_SendResponse(trans_id, NFBT_GATT_SUCCESS, attr_handle, attr_handle, (char*)resp, sizeof(resp));
}

static void FuncCallback_gatt_spp_rx_receive_event(int offset, nfUINT8 *value, int length)
{
	int i;
	printf("BLE Spp rx received: offset=%d, length=%d\n", 
		offset, length);

	if(value)
	{
		printf("                   : value=");
		for(i=0;i<length;i++)
		{
			printf("%02X", value[i+offset]);
		}
		printf("\n");
	}
}

static void FuncCallback_gatt_char_notification_req(int srvc_idx, int attr_id, int enable)
{
	printf("Characteristic 0x%04x notification %s\n", attr_id, enable? "Enable": "Disable");
}

bool check_pincode(char *ppin)
{
    if( ppin == NULL )
        return false;

    int len = strlen( ppin );
    if( len > PINCODE_MAX_LEN || len == 0 )
        return false;

    int i;
    for( i = 0; i < len; i++ ){
        if( ppin[i] < 48 || ppin[i] > 57 )
            return false;
    }

    return true;
}

void char_to_hex(char *dest, char *src, int src_len)
{
	int i, j;
	char byteInChar[2];

	if((src_len>2) && !(src_len%2))
		src_len--;

	for(i=0, j=0; i<src_len; i+=2, j++)
	{
	        byteInChar[0] = toupper( src[i] );
	        byteInChar[1] = toupper( src[i+1] );
		
	        // first 4 bit
	        if( isalpha( byteInChar[0] )  )
	            dest[j] = (byteInChar[0] - 55) * 16;
	        else if( isdigit( byteInChar[0] )  )
	            dest[j] = (byteInChar[0] - 48) * 16;

	        // last 4 bit
	        if( isalpha( byteInChar[1] ) )
	            dest[j] += (byteInChar[1] - 55);
	        else if( isdigit( byteInChar[1] )  )
	            dest[j] += (byteInChar[1] - 48);
	}
}

void str_to_char(const char *addr, unsigned char b[BT_ADDR_LEN])
{
    int i;
    char byteInChar[2];

    for( i = 0;i < BT_ADDR_LEN; i++ ){

        byteInChar[0] = toupper( addr[0] );
        byteInChar[1] = toupper( addr[1] );

        // first 4 bit
        if( isalpha( byteInChar[0] )  )
            b[i] = (byteInChar[0] - 55) * 16;
        else if( isdigit( byteInChar[0] )  )
            b[i] = (byteInChar[0] - 48) * 16;

        // last 4 bit
        if( isalpha( byteInChar[1] ) )
            b[i] += (byteInChar[1] - 55);
        else if( isdigit( byteInChar[1] )  )
            b[i] += (byteInChar[1] - 48);

        if( addr[2] != '\0' )
            addr = &addr[3];
    }
}

bool isLenConfrom(char* str, int len, bool restricted)
{
    if( strlen( str ) > (size_t)len ){
        printf( "Invalid input.\n" );
        return false;
    }
    if( restricted && strlen( str ) != (size_t)len ){
        printf( "Invalid input.\n" );
        return false;
    }
    return true;
}

static void print_cmd_error(bt_status cmd_ret)
{
    switch(cmd_ret) {
        CASE_PRINT(NFBT_STATUS_FAIL)
            break;
        CASE_PRINT(NFBT_STATUS_NOT_READY)
            break;
        CASE_PRINT(NFBT_STATUS_NOMEM)
            break;
        CASE_PRINT(NFBT_STATUS_BUSY)
            break;
        CASE_PRINT(NFBT_STATUS_DONE)
            break;
        CASE_PRINT(NFBT_STATUS_UNSUPPORTED)
            break;
        CASE_PRINT(NFBT_STATUS_PARM_INVALID)
            break;
        CASE_PRINT(NFBT_STATUS_UNHANDLED)
            break;
        CASE_PRINT(NFBT_STATUS_AUTH_FAILURE)
            break;
        CASE_PRINT(NFBT_STATUS_RMT_DEV_DOWN)
            break;
        CASE_PRINT(NFBT_STATUS_AUTH_REJECTED)
            break;
        default:
            break;
    };
}

static int enter_address(char *bt_addr)
{
    while(1){
        printf( "\n"
                "Enter the address in format 11:22:33:AA:BB:CC,"
                " or 0 to don't care, or -1 to cancel.\n" );
        fflush( stdin );
        scanf( "%18s", bt_addr );
        fflush( stdin );

        if( strcasecmp( bt_addr, cancel ) == 0 ){
            printf( "canceled\n" );
            return -1;
        }
        if( strcasecmp( bt_addr, dontcare ) == 0 ){
            strcpy(bt_addr, "FF:FF:FF:FF:FF:FF");   //auto select address
            return 0;
        }
        if( isLenConfrom( bt_addr, BT_MAC_LEN, true ) == false )
            continue;

        break;
    }

    return 1;
}

static int enter_confirm( char* show, char *input)
{
    while(1){
        printf( "\n"
                "%s\n"
                "Enter y or n, or -1 to cancel.\n", show);
        fflush( stdin );
        scanf( "%2s", input );
        fflush( stdin );

        if( strcasecmp( input, cancel ) == 0 ){
            printf( "canceled\n" );
            return -1;
        }

        if( isLenConfrom( input, 1, false ) == false )
            continue;

        if( strcasecmp( input, yes ) == 0 )
            return 1;
        else if( strcasecmp( input, no ) == 0 )
            return 0;
        else{
            printf( "Invalid input.\n" );
        }

    }

    return -1;
}

static int enter_index( char* show, char *input, int max_len, unsigned long long *output)
{
    char *end;

    while(1){
        printf( "\n"
                "%s\n", show );
        fflush( stdin );
        scanf( "%20s", input );
        fflush( stdin );

        if( strcasecmp( input, cancel ) == 0 ){
            printf( "canceled\n");
            return -1;
        }
        if( isLenConfrom( input, max_len, false ) == false )
            continue;

        *output = strtoull(input, &end, 10);
        if (end == input){
            printf( "Invalid input.\n" );
            continue;
        }

        return 0;
    }

    return -1;
}

static int enter_pincode( char* show, char *input, int max_len)
{
    while(1){
        printf( "\n"
                "%s\n", show );
        fflush( stdin );
        scanf( "%20s", input );
        fflush( stdin );

        if( strcasecmp( input, cancel ) == 0 ){
            printf( "canceled\n");
            return -1;
        }
        if( isLenConfrom( input, max_len, false ) == false )
            continue;

        return 0;
    }

    return -1;
}

static int enter_uuid_x(char* show, char *uuid_str, int check_len)
{
    char uuid[36];
	
    while(1){
        printf( "\n"
                "%s\n", show );
        fflush( stdin );
        scanf( "%32s", uuid );
        fflush( stdin );

        if( strcasecmp( uuid, cancel ) == 0 ){
            printf( "canceled\n" );
            return -1;
        }
        if( strcasecmp( uuid, ignore ) == 0 ){
            printf( "ignore\n" );
            return -2;
        }
		
        if( isLenConfrom( uuid, check_len, true ) == false )
            continue;

        break;
    }

    char_to_hex(uuid_str, uuid, check_len);
    return 1;
}

static int enter_uuid(char *uuid_str)
{
    while(1){
        printf( "\n"
                "Enter the uuid in format 00001101-0000-1000-8000-00805f9b34fb,"
                " or -1 to cancel.\n" );
        fflush( stdin );
        scanf( "%36s", uuid_str );
        fflush( stdin );

        if( strcasecmp( uuid_str, cancel ) == 0 ){
            printf( "canceled\n" );
            return -1;
        }
        if( isLenConfrom( uuid_str, UUID_LEN, true ) == false )
            continue;

        break;
    }

    return 1;
}

static int enter_data(char *buf)
{
    while(1){
        printf("\n"
                "Enter the data in Hex for maximum length %d,"
                " or -1 to cancel.\n",
                RFCOMM_DATA_LEN / 2);
        fflush(stdin);
        scanf( "%1024s", buf );
        fflush(stdin);

        if( strcasecmp( buf, cancel ) == 0 ){
            printf( "canceled\n" );
            return -1;
        }
        if( isLenConfrom( buf, RFCOMM_DATA_LEN, false ) == false )
            continue;

        break;
    }

    return 1;
}
int main(int argc, char** argv)
{
//START:
	BT_VERSION ver;
    BT_ADDR addr;
    BT_ADDR devaddr1;
    BT_ADDR devaddr2;  
    char input[MAX_CMD_LEN + 2];
    char bt_addr[BT_MAC_LEN + 2];
    char uuid_str[UUID_LEN + 2];
    char char_uuid_str[UUID_LEN + 2];
    bt_status cmd_ret;
    char data_buf[32]={0};
    int data_len = 0;
    int ret = 0;
    int i;

    (void)argc;
    (void)argv;

    ql_bt_sleepwake_pin_init();
    ql_bt_wake_high();
    ql_bt_module_enable();
    //ql_ble_sleep_init();

    //init
    cmd_ret = NFBT_Init(&nfmw_callbacks);
    if( cmd_ret == NFBT_STATUS_SUCCESS )
        printf("Register CBK success\n");
    else{
        print_cmd_error( cmd_ret );
        printf("Register CBK failed\n");
    }

	ver = NFBT_GetBtVersion();

    printf( "\n\n"
             "*************************************************************\n"
             "****** nFore BlueGate Bluetooth Middleware (v%s) *****\n"
             "*************************************************************\n\n", ver.version );

    NFBT_SetPairedDeviceNum((int) MAX_PAIR );    

    cmd_ret = NFBT_Enable();
    if( cmd_ret == NFBT_STATUS_SUCCESS ){
        printf("Enable BT stack success\n");
    }

    do{
        memset(input, 0, MAX_CMD_LEN + 2);
        memset(bt_addr, 0, BT_MAC_LEN + 2);

        printf( "\n"
                "Enter the command to execute:\n" );

        fflush( stdin );
        scanf( "%21s", input );
        fflush( stdin );

        if (isLenConfrom( input, MAX_CMD_LEN, false ) == false )
            continue;

        if (pair_request != NFBT_PAIR_NONE){

            if (pair_request <= NFBT_PAIR_SSP_DISPLAY_ONLY|| pair_request >= NFBT_PAIR_PIN)
            {
                if( strcasecmp( input, cancel ) == 0 ){
                    printf( "Cancel pairing.\n");
                }
                else if(isLenConfrom( input, 1, false ) == false){
                    printf("Enter y or n to response the remote device pairing request.\n");
                    continue;
                }
                else if(strcasecmp( input, no ) != 0 &&
                        strcasecmp( input, yes ) != 0){
                    printf( "\n"
                        "Invalid input.\n"
                        "Enter y or n to response the remote device pairing request.\n");
                    continue;
                }
            }

            if (pair_request == NFBT_PAIR_SSP || pair_request == NFBT_PAIR_SSP_DISPLAY_ONLY){
                if( strcasecmp( input, no ) == 0 ||
                    strcasecmp( input, cancel ) == 0)
                    cmd_ret = NFBT_SspReply(&remote_pair_addr, NFBT_REJECT_PAIR);
                else if( strcasecmp( input, yes ) == 0 )
                    cmd_ret = NFBT_SspReply(&remote_pair_addr, NFBT_ACCEPT_PAIR);

                if( cmd_ret == NFBT_STATUS_SUCCESS ){
                    printf( "SSP reply successful\n" );
                }
                else{
                    print_cmd_error( cmd_ret );
                    printf( "SSP reply failed\n" );
                }
            }
            else if (pair_request == NFBT_PAIR_PIN){
                //Todo user input pin code....
                char *pin = "0000";

                if( strcasecmp( input, no ) == 0 )
                    cmd_ret = NFBT_PairReply(&remote_pair_addr, NFBT_REJECT_PAIR, strlen(pin), (nfUINT8 *)pin);
                else if( strcasecmp( input, yes ) == 0 )
                    cmd_ret = NFBT_PairReply(&remote_pair_addr, NFBT_ACCEPT_PAIR, strlen(pin), (nfUINT8 *)pin);

                if( cmd_ret == NFBT_STATUS_SUCCESS ){
                    printf( "PIN reply successful\n" );
                }
                else{
                    print_cmd_error( cmd_ret );
                    printf( "PIN reply failed\n" );
                }
            }
            else if (pair_request == NFBT_PAIR_SSP_PASSKEY){
				
                if( strcasecmp( input, no ) == 0 )
                    cmd_ret = NFBT_PairReply(&remote_pair_addr, NFBT_REJECT_PAIR, PASSKEY_MAX_LEN, (nfUINT8 *)input);
                else
                {
                    enter_pincode("\n"
                        "Enter 6-digit Passkey for reply, or -1 to cancel.\n" ,
                        input,
                        PASSKEY_MAX_LEN);
                
                    cmd_ret = NFBT_PairReply(&remote_pair_addr, NFBT_ACCEPT_PAIR, PASSKEY_MAX_LEN, (nfUINT8 *)input);
                }

                if( cmd_ret == NFBT_STATUS_SUCCESS ){
                    printf( "PIN reply successful\n" );
                }
                else{
                    print_cmd_error( cmd_ret );
                    printf( "PIN reply failed\n" );
                }
            }

            pair_request = NFBT_PAIR_NONE;
            continue;
        }

        if (opp_access_request == 1){
            if(isLenConfrom( input, 1, false ) == false){
            	printf( "Enter y or n to response the remote device file access request.\n");
                continue;
            }
            else if(strcasecmp( input, no ) != 0 &&
                    strcasecmp( input, yes ) != 0){
                printf( "\n"
                        "Invalid input.\n"
                		"Enter y or n to response the remote device file access request.\n");
                continue;
            }

            if (strcasecmp( input, no ) == 0)
                cmd_ret = NFBT_OPP_AccessReply(&opp_remote_access_addr, NFBT_OPP_DOWNLOAD_REJECT);
            else if (strcasecmp( input, yes ) == 0 )
                cmd_ret = NFBT_OPP_AccessReply(&opp_remote_access_addr, NFBT_OPP_DOWNLOAD_ALLOW);

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "file access reply successful\n" );
            }
            else{
                print_cmd_error( cmd_ret );
                printf( "file access reply failed\n" );
            }

        	opp_access_request = 0;
        	continue;
        }


        if( strcasecmp( input, EXIT ) == 0 || strcasecmp( input, "c0" ) == 0 )
            break;

        if( strcasecmp( input, "bt_enable" ) == 0 || strcasecmp( input, "c1" ) == 0 ){
            cmd_ret = NFBT_Enable();

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf("Enable BT stack success\n");
            else
                print_cmd_error( cmd_ret );
        }
        else if( strcasecmp( input, "bt_disable" ) == 0 || strcasecmp( input, "c2" ) == 0 ){
            cmd_ret = NFBT_Disable();

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf("Disable BT stack success\n");
            else
                print_cmd_error( cmd_ret );
        }
        else if( strcasecmp( input, "localinfos" ) == 0 || strcasecmp( input, "c3" ) == 0 ){
            cmd_ret = NFBT_GetLocalDeviceInfo();

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf("Get local device info  success\n");
            else
                print_cmd_error( cmd_ret );
        }
        else if( strcasecmp( input, "setname" ) == 0 || strcasecmp( input, "c4" ) == 0 ){
            char name[BT_LOCAL_NAME_MAX] = {0};

            while(1){
                printf( "\n"
                        "Enter the name with maximum length 248, or -1 to cancel.\n" );
                fflush( stdin );
                scanf( "%249s", name );
                fflush( stdin );

                if( strcasecmp( name, cancel ) == 0 ){
                    printf( "Set name canceled\n" );
                    break;
                }

                if( isLenConfrom( name, BT_LOCAL_NAME_MAX, false ) == false )
                    continue;

                cmd_ret = NFBT_SetLocalDeviceName( (unsigned char)strlen(name),(unsigned char *)name);

                if( cmd_ret == NFBT_STATUS_SUCCESS )
                    printf( "Set name succeed\n" );
                else{
                    print_cmd_error( cmd_ret );
                    printf( "Set name failed\n" );
                }
                break;
            }
        }
        else if( strcasecmp( input, "setscan" ) == 0 || strcasecmp( input, "c5" ) == 0 ){
            int ret;
            unsigned long long mode;

            ret = enter_index("\n"
							  "Enter the scan mode, or -1 to cancel.\n" 
							  "0 : Scan Mode NONE \n" 
							  "1 : Scan Mode Connectable\n" 
							  "2 : Scan Mode Connectable & Discoverable\n"
							  "3 : Scan Mode NONE HOLDING\n" ,
							  input,
							  1, &mode);

            if (ret < 0)
                continue;
			if (mode == 0)
			{
				cmd_ret = NFBT_SetScanMode(NFBT_SCAN_MODE_NONE);
			}
			else if (mode == 1)
			{
				cmd_ret = NFBT_SetScanMode(NFBT_SCAN_MODE_CONNECTABLE);
			}
			else if (mode == 2)
			{
				cmd_ret = NFBT_SetScanMode(NFBT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
			}
			else if (mode == 3)
			{
				cmd_ret = NFBT_SetScanMode(NFBT_SCAN_MODE_NONE_HOLDING);
			}			

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf( "Set Scan Mode succeed\n" );
            else{
                print_cmd_error( cmd_ret );
                printf( "Set Scan Mode failed\n" );
            }
        }
        else if( strcasecmp( input, "scan" ) == 0 || strcasecmp( input, "c6" ) == 0 ||
                 strcasecmp( input, "stop" ) == 0 || strcasecmp( input, "c7" ) == 0 ){

            char inquiry_str[14];
            if (strcasecmp( input, "scan" ) == 0 ||
                strcasecmp( input, "c6" ) == 0 ){
                strcpy( inquiry_str, "Start inquiry" );
                cmd_ret = NFBT_InquiryDevice(NFBT_INQUIRY_STARTED);
            }
            else{
                strcpy( inquiry_str, "Stop inquiry" );
                cmd_ret = NFBT_InquiryDevice(NFBT_INQUIRY_STOPPED);
            }

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf("%s device success\n",inquiry_str);
            else{
                print_cmd_error( cmd_ret );
                printf("%s device failed\n",inquiry_str);
            }
        }
        else if( strcasecmp( input, "pairlist" ) == 0 || strcasecmp( input, "c8" ) == 0 ){
            cmd_ret = NFBT_GetPairedDeviceInfo();

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf("Get paired device info success\n");
            else
                print_cmd_error( cmd_ret );
        }
        else if( strcasecmp( input, "pair" ) == 0 || strcasecmp( input, "c9" ) == 0 ){
            int ret;

            ret = enter_address(bt_addr);

            if(ret == 1){
                str_to_char( bt_addr, addr.address );
                cmd_ret = NFBT_PairRemoteDevice(&addr);

                if( cmd_ret == NFBT_STATUS_SUCCESS ){
                    printf( "Pair remote device successful\n" );
                }
                else{
                    print_cmd_error( cmd_ret );
                    printf( "Pair remote device failed\n" );
                }
            }
        }
        else if( strcasecmp( input, "delete" ) == 0 || strcasecmp( input, "c10" ) == 0 ){
            int ret;

            ret = enter_address(bt_addr);

            if(ret >= 0){
                str_to_char( bt_addr, addr.address );
                cmd_ret = NFBT_DeletePairedDevice(&addr);

                if( cmd_ret == NFBT_STATUS_SUCCESS ){
                    printf( "Delete paired device successful\n" );
                }
                else{
                    print_cmd_error( cmd_ret );
                    printf( "Delete paired device failed\n" );
                }
            }
        }
        else if( strcasecmp( input, "connect" ) == 0 || strcasecmp( input, "c13" ) == 0 ||
                 strcasecmp( input, "disconnect" ) == 0 || strcasecmp( input, "c14" ) == 0 ||
                 strcasecmp( input, "switch" ) == 0 || strcasecmp( input, "c15" ) == 0 ){
            char conn_str[12];
            int ret;
            unsigned long long index;
            PROFILE_ID profile;
            char service[PROFILE_NAME_MAX + 2];

            ret = enter_address(bt_addr);

            if (ret >= 0) {
                str_to_char( bt_addr, addr.address );

                ret = enter_index("\n"
                        "0.  all         -- All Profile.\n"
                        "1.  audio       -- HPF, A2DP, AVRCP.\n"
                        "2.  phone audio -- HFP.\n"
                        "3.  media audio -- A2DP, AVRCP.\n"
                        "4.  serial port -- SPP\n"
                        "5.  iAP         -- Wireless iAP\n"
                        "6.  RFCOMM 1    -- Custom rfcomm 1\n"
                        "7.  RFCOMM 2    -- Custom rfcomm 2\n"
                        "8.  RFCOMM 3    -- Custom rfcomm 3\n"
                        "9.  RFCOMM 4    -- Custom rfcomm 4\n"
                        "10. RFCOMM 5    -- Custom rfcomm 5\n"
                        "11. RFCOMM 6    -- Custom rfcomm 6\n"
                        "12. RFCOMM 7    -- Custom rfcomm 7\n"
                        "13. RFCOMM 8    -- Custom rfcomm 8\n"
                        "14. RFCOMM 9    -- Custom rfcomm 9\n"
						"15. HID         -- HID.\n"
                        "16. GATT        -- client\n"
                        "Enter the service name, or -1 to cancel.\n\n" ,
                        service,
                        PROFILE_NAME_MAX,
                        &index);

                if (ret < 0)
                    continue;

                if( index == 0 )
                    profile = NFBT_PID_ALL_PROFILE;
                else if( index == 1 )
                    profile = (NFBT_PID_HFP | NFBT_PID_A2DP_SINK | NFBT_PID_AVRCP_CT);
                else if( index == 2 )
                    profile = NFBT_PID_HFP;
                else if( index == 3 )
                    profile = NFBT_PID_A2DP_SINK | NFBT_PID_AVRCP_CT;
                else if( index == 4 )
                    profile = NFBT_PID_SPP;
                else if( index == 5 )
                    profile = NFBT_PID_IAP;
                else if( index == 6 )
                    profile = NFBT_PID_RFCOMM_1;
                else if( index == 7 )
                    profile = NFBT_PID_RFCOMM_2;
                else if( index == 8 )
                    profile = NFBT_PID_RFCOMM_3;
                else if( index == 9 )
                    profile = NFBT_PID_RFCOMM_4;
                else if( index == 10 )
                    profile = NFBT_PID_RFCOMM_5;
                else if( index == 11 )
                    profile = NFBT_PID_RFCOMM_6;
                else if( index == 12 )
                    profile = NFBT_PID_RFCOMM_7;
                else if( index == 13 )
                    profile = NFBT_PID_RFCOMM_8;
                else if( index == 14 )
                    profile = NFBT_PID_RFCOMM_9;
				else if( index == 15 )
					profile = NFBT_PID_HIDD;
                else if( index == 16 )
                    profile = NFBT_PID_GATT;
                else {
                    printf( "Incorrect service name.\n" );
                    continue;
                }
            }
            else {
                continue;
            }

            cmd_ret = NFBT_STATUS_FAIL;

            if (strcasecmp(input, "connect") == 0 ||
                strcasecmp(input, "c13") == 0) {
                cmd_ret = NFBT_ConnectProfile(&addr , profile);
                strcpy( conn_str, "Connect" );
            }
            else if(strcasecmp(input, "disconnect") == 0 ||
                    strcasecmp(input, "c14") == 0) {
                cmd_ret = NFBT_DisconnectProfile(&addr , profile);
                strcpy( conn_str, "Disconnect" );
            }
            else if(strcasecmp(input, "switch") == 0 ||
                    strcasecmp(input, "c15") == 0) {
                cmd_ret = NFBT_ForceConnectProfile(&addr , profile);
                strcpy( conn_str, "Switch" );
            }

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf( "%s successful\n", conn_str );
            else{
                print_cmd_error( cmd_ret );
                printf( "%s failed\n", conn_str );
            }

        }
        else if( strcasecmp( input, "setprofile" ) == 0 || strcasecmp( input, "c16" ) == 0 ){
            int ret;
            unsigned long long index;
            PROFILE_ID profile;
            char service[PROFILE_NAME_MAX + 2];

            ret = enter_address(bt_addr);

            if (ret >= 0) {
                str_to_char( bt_addr, addr.address );

                ret = enter_index("\n"
                        "0.  all         -- All Profile.\n"
                        "1.  audio       -- HPF, A2DP, AVRCP.\n"
                        "2.  phone audio -- HFP.\n"
                        "3.  media audio -- A2DP, AVRCP.\n"
                        "4.  serial port -- SPP\n"
                        "5.  iAP         -- Wireless iAP\n"
                        "6.  RFCOMM 1    -- Custom rfcomm 1\n"
                        "7.  RFCOMM 2    -- Custom rfcomm 2\n"
                        "8.  RFCOMM 3    -- Custom rfcomm 3\n"
                        "9.  RFCOMM 4    -- Custom rfcomm 4\n"
                        "10. RFCOMM 5    -- Custom rfcomm 5\n"
                        "11. RFCOMM 6    -- Custom rfcomm 6\n"
                        "12. RFCOMM 7    -- Custom rfcomm 7\n"
                        "13. RFCOMM 8    -- Custom rfcomm 8\n"
                        "14. RFCOMM 9    -- Custom rfcomm 9\n"
                		"15. object push -- OPP\n"
                		"16. phonebook access	 -- PBAP\n"
                		"17. message access 	 -- MAP client\n"
                		"18. human interface	 -- HID\n"
                        "Enter the service name, or -1 to cancel.\n\n" ,
                        service,
                        PROFILE_NAME_MAX,
                        &index);

                if (ret < 0)
                    continue;

                if( index == 0 )
                    profile = NFBT_PID_ALL_PROFILE;
                else if( index == 1 )
                    profile = (NFBT_PID_HFP | NFBT_PID_A2DP_SINK | NFBT_PID_AVRCP_CT);
                else if( index == 2 )
                    profile = NFBT_PID_HFP;
                else if( index == 3 )
                    profile = NFBT_PID_A2DP_SINK | NFBT_PID_AVRCP_CT;
                else if( index == 4 )
                    profile = NFBT_PID_SPP;
                else if( index == 5 )
                    profile = NFBT_PID_IAP;
                else if( index == 6 )
                    profile = NFBT_PID_RFCOMM_1;
                else if( index == 7 )
                    profile = NFBT_PID_RFCOMM_2;
                else if( index == 8 )
                    profile = NFBT_PID_RFCOMM_3;
                else if( index == 9 )
                    profile = NFBT_PID_RFCOMM_4;
                else if( index == 10 )
                    profile = NFBT_PID_RFCOMM_5;
                else if( index == 11 )
                    profile = NFBT_PID_RFCOMM_6;
                else if( index == 12 )
                    profile = NFBT_PID_RFCOMM_7;
                else if( index == 13 )
                    profile = NFBT_PID_RFCOMM_8;
                else if( index == 14 )
                    profile = NFBT_PID_RFCOMM_9;
                else if( index == 15 )
                    profile = NFBT_PID_OPP;
                else if( index == 16 )
                    profile = NFBT_PID_PBAP_CLIENT;
                else if( index == 17 )
                    profile = NFBT_PID_MAP_CLIENT;
                else if( index == 18 )
                    profile = NFBT_PID_HIDD;
                else {
                    printf( "Incorrect service name.\n" );
                    continue;
                }
            }
            else {
                continue;
            }

            cmd_ret = NFBT_STATUS_FAIL;
            cmd_ret = NFBT_SetRemoteDeviceProfile(&addr , profile);

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf( "Set Profile successful\n");
            else{
                print_cmd_error( cmd_ret );
                printf( "Set Profile failed\n" );
            }
        }
        else if( strcasecmp( input, "getprofile" ) == 0 || strcasecmp( input, "c17" ) == 0 ) {
//				strcasecmp( input, "servicelist" ) == 0 || strcasecmp( input, "c18" ) == 0 ) {

        	char conn_str[12];
        	int ret;
        	PROFILE_ID profile;

        	ret = enter_address(bt_addr);

        	if (ret >= 0) {
        		str_to_char( bt_addr, addr.address );

        		if(strcasecmp(input, "getprofile") == 0 ||
        				strcasecmp(input, "c17") == 0) {
        			cmd_ret = NFBT_GetRemoteDeviceProfile(&addr, &profile);
        			strcpy( conn_str, "Enable" );
        		}
//	            else if( strcasecmp(input, "servicelist") == 0 ||
//	            		strcasecmp(input, "c18") == 0) {
//	            	cmd_ret = NFBT_SupportProfilelist( &addr, &profile );
//	            	strcpy( conn_str, "Support" );
//	            }

        		if( cmd_ret == NFBT_STATUS_SUCCESS ){
        			if(profile & NFBT_PID_HFP){
        				printf( "%s handsfree profile\n", conn_str);
        			}
        			if(profile & NFBT_PID_A2DP_SINK){
        				printf( "%s a2dp and avrcp\n", conn_str);
        			}
        			if(profile & NFBT_PID_SPP){
        				printf( "%s spp\n", conn_str);
        			}
        			if(profile & NFBT_PID_IAP){
        				printf( "%s iap\n", conn_str);
        			}
        			if(profile & NFBT_PID_RFCOMM_1){
        				printf( "%s rfcomm_1 profile\n", conn_str);
        			}
        			if(profile & NFBT_PID_RFCOMM_2){
        				printf( "%s rfcomm_2 profile\n", conn_str);
        			}
        			if(profile & NFBT_PID_RFCOMM_3){
        				printf( "%s rfcomm_3 profile\n", conn_str);
        			}
        			if(profile & NFBT_PID_RFCOMM_4){
        				printf( "%s rfcomm_4 profile\n", conn_str);
        			}
        			if(profile & NFBT_PID_RFCOMM_5){
        				printf( "%s rfcomm_5 profile\n", conn_str);
        			}
        			if(profile & NFBT_PID_RFCOMM_6){
        				printf( "%s rfcomm_6 profile\n", conn_str);
        			}
        			if(profile & NFBT_PID_RFCOMM_7){
        				printf( "%s rfcomm_7 profile\n", conn_str);
        			}
        			if(profile & NFBT_PID_RFCOMM_8){
        				printf( "%s rfcomm_8 profile\n", conn_str);
        			}
        			if(profile & NFBT_PID_RFCOMM_9){
        				printf( "%s rfcomm_9 profile\n", conn_str);
        			}
        			if(profile & NFBT_PID_OPP){
        				printf( "%s OPP\n", conn_str);
        			}
        			if(profile & NFBT_PID_PBAP_CLIENT){
        				printf( "%s PBAP\n", conn_str);
        			}
        			if(profile & NFBT_PID_MAP_CLIENT){
        				printf( "%s MAP\n", conn_str);
        			}
        			if(profile & NFBT_PID_HIDD){
        				printf( "%s HID\n", conn_str);
        			}
        		}
        		else{
        			print_cmd_error( cmd_ret );
        			printf( "get device %s list failed\n", conn_str );
        		}
        	}
        }
        else if( strcasecmp(input,"address") == 0 || strcasecmp(input,"c18") == 0 ){

        	cmd_ret = NFBT_GetAddr( &addr );

        	if( cmd_ret == NFBT_STATUS_SUCCESS )
        		printf(	 "Local Address:\t\t%02X:%02X:%02X:%02X:%02X:%02X\n\n",
        			addr.address[0], addr.address[1], addr.address[2], addr.address[3], addr.address[4], addr.address[5] );
        		else
        			print_cmd_error( cmd_ret );

        }

        else if( strcasecmp( input, "updaterssi" ) == 0 || strcasecmp( input, "c20" ) == 0 )
        {
            int ret;
            ret = enter_confirm("", input);

            if( ret == 1 ){
                cmd_ret = NFBT_UpdateRssi(1, 10);
            }else if( ret == 0 ){
                cmd_ret = NFBT_UpdateRssi(0, 10);
            }

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf( "NFBT_UpdateRssi successful\n\r" );
            else{
                print_cmd_error( cmd_ret );
                printf( "NFBT_UpdateRssi failed\n\r" );
            }
        }
        /* multi-hfp function start */
        else if(strcasecmp( input, "hangans") == 0  || strcasecmp( input, "c491") == 0 ||
        		strcasecmp( input, "holdans") == 0 	|| strcasecmp( input, "c492") == 0 ||
				strcasecmp( input, "changsco")== 0	|| strcasecmp( input, "c493") == 0 ||
				strcasecmp( input, "devaddr") == 0	|| strcasecmp( input, "c494") == 0)
        {
        	if( strcasecmp( input, "hangans") == 0 || strcasecmp( input, "c491") == 0){
        		int ret1;
        		unsigned long long idx;

        		ret1 = enter_index("enter the index you want to hang off and will answer another one\n",
        					input, 1, &idx);

        		if( ret1 >= 0){
        			if( idx == 1){
        				cmd_ret = NFBT_HFP_Terminate( &devaddr1);
        				printf("terminate ret %d", cmd_ret);
        				usleep(2000000);
        				cmd_ret = NFBT_HFP_Answer( &devaddr2);
        				printf("answer ret %d", cmd_ret);
        			}
        			else{
        				cmd_ret = NFBT_HFP_Terminate( &devaddr2);
        				printf("terminate ret %d", cmd_ret);
        				usleep(2000000);
        				cmd_ret = NFBT_HFP_Answer( &devaddr1);
        				printf("answer ret %d", cmd_ret);
        			}
        		}
			}
        	else if( strcasecmp( input, "holdans") == 0 || strcasecmp( input, "c492") == 0){
        		int ret1;
        		unsigned long long idx;

        		ret1 = enter_index("enter index you want to hold on and answer another one\n",
        						input, 1, &idx);
        		if( ret1 >= 0){
        			if( idx == 1){
        				cmd_ret = NFBT_HFP_AudioTransfer( &devaddr1, HFP_AUDIO_HF_TO_AG);
        				printf("disconnect hfp audio ret %d", cmd_ret);
        				usleep(2000000);
        				cmd_ret = NFBT_HFP_Answer( &devaddr2 );
        				printf("answer ret %d", cmd_ret);
        			}
        			else{
        				cmd_ret = NFBT_HFP_AudioTransfer( &devaddr2, HFP_AUDIO_HF_TO_AG);
        				printf("disconnect hfp audio ret %d", cmd_ret);
        				usleep(2000000);
        				cmd_ret = NFBT_HFP_Answer( &devaddr1 );
        				printf("answer ret %d", cmd_ret);
        			}
        		}
        	}
  			else if( strcasecmp( input, "changsco") == 0 || strcasecmp( input, "c493") == 0){
  				int ret1;
  				unsigned long long idx;

  				ret1 = enter_index("enter the index you want hold on and connect another one\n",
  								input, 1, &idx);
  				if( ret1 >= 0){
  					if( idx == 1){
  						cmd_ret = NFBT_HFP_AudioTransfer( &devaddr1, HFP_AUDIO_HF_TO_AG);
  						printf("disconnect hfp audio ret %d", cmd_ret);
  						usleep(2000000);
  						cmd_ret = NFBT_HFP_AudioTransfer( &devaddr2, HFP_AUDIO_AG_TO_HF );
  						printf("connect hfp audio ret %d", cmd_ret);
  					}
  					else{
  						cmd_ret = NFBT_HFP_AudioTransfer( &devaddr2, HFP_AUDIO_HF_TO_AG);
  						printf("disconnect hfp audio ret %d", cmd_ret);
  						usleep(2000000);
  						cmd_ret = NFBT_HFP_AudioTransfer( &devaddr1, HFP_AUDIO_AG_TO_HF );
						printf("connect hfp audio ret %d", cmd_ret);
					}
  				}
  			}
			else if( strcasecmp( input, "devaddr") == 0 || strcasecmp( input, "c494") == 0){
				int ret1;
				printf("Enter the 1st device address\n");
				ret1 = enter_address(bt_addr);
				if( ret1 >= 0)
					str_to_char( bt_addr, devaddr1.address);

				printf("Enter the 2nd device address\n");
				ret1 = enter_address(bt_addr);
				if( ret1 >=0)
					str_to_char( bt_addr, devaddr2.address);
			}
        }
        /* multi-hfp function end */
        else if(  strcasecmp( input, "transfer" ) == 0      || strcasecmp( input, "c30" ) == 0 ||
                  strcasecmp( input, "answer" ) == 0        || strcasecmp( input, "c32" ) == 0 ||
                  strcasecmp( input, "reject" ) == 0        || strcasecmp( input, "c33" ) == 0 ||
                  strcasecmp( input, "dial") == 0           || strcasecmp( input, "c34" ) == 0 ||
                  strcasecmp( input, "last") == 0           || strcasecmp( input, "c35" ) == 0 ||
                  strcasecmp( input, "dtmf") == 0           || strcasecmp( input, "c36" ) == 0 ||
                  strcasecmp( input, "terminate" ) == 0     || strcasecmp( input, "c37" ) == 0 ||
                  strcasecmp( input, "hold" ) == 0          || strcasecmp( input, "c38" ) == 0 ||
                  strcasecmp( input, "spk" ) == 0           || strcasecmp( input, "c39" ) == 0 ||
                  strcasecmp( input, "mic" ) == 0           || strcasecmp( input, "c40" ) == 0 ||
                  strcasecmp( input, "asr" ) == 0           || strcasecmp( input, "c41" ) == 0 ||
                  strcasecmp( input, "mynum" ) == 0         || strcasecmp( input, "c42" ) == 0 ||
                  strcasecmp( input, "setauto" ) == 0       || strcasecmp( input, "c43" ) == 0 ||
                  strcasecmp( input, "getauto" ) == 0       || strcasecmp( input, "c44" ) == 0 ||
                  strcasecmp( input, "setrt" ) == 0         || strcasecmp( input, "c45" ) == 0 ||
				  strcasecmp( input, "getrt" ) == 0         || strcasecmp( input, "c46" ) == 0 ||
				  strcasecmp( input, "setspkrender" ) == 0  || strcasecmp( input, "c47" ) == 0 ||
                  strcasecmp( input, "getspkrender" ) == 0  || strcasecmp( input, "c48" ) == 0 ){
            int ret;

            ret = enter_address(bt_addr);

            if(ret >= 0){
                str_to_char( bt_addr, addr.address );

                char str[100];
                if( strcasecmp( input, "transfer" ) == 0 || strcasecmp( input, "c30" ) == 0 ){
                    strcpy( str, "Set audio transfer" );
                    int ret;
                    ret = enter_confirm( str, input);

                    if( ret == 1 )
                        cmd_ret = NFBT_HFP_AudioTransfer( &addr, HFP_AUDIO_HF_TO_AG );
                    else if( ret == 0 )
                        cmd_ret = NFBT_HFP_AudioTransfer( &addr, HFP_AUDIO_AG_TO_HF );
                    else
                        continue;

                }
                else if( strcasecmp( input, "answer" ) == 0 || strcasecmp( input, "c32" ) == 0 ){
                    cmd_ret = NFBT_HFP_Answer( &addr );
                    strcpy( str, "Answer" );
                }
                else if( strcasecmp( input, "reject" ) == 0 || strcasecmp( input, "c33" ) == 0 ){
                    cmd_ret = NFBT_HFP_Reject( &addr );
                    strcpy( str, "Reject" );
                }
                else if( strcasecmp( input, "terminate" ) == 0 || strcasecmp( input, "c37" ) == 0 ){
                    cmd_ret = NFBT_HFP_Terminate( &addr );
                    strcpy( str, "Terminate" );
                }
                else if( strcasecmp( input, "hold" ) == 0 || strcasecmp( input, "c38" ) == 0 ){
                    strcpy( str, "Hold call" );
                    HFP_CALL_ACTION_CHLD hold_option;
                    int specified_call_number = 0;
                    unsigned long long index;

                    ret = enter_index("\n"
                            "0. Releases all held calls.\n"
                            "1. Releases all active calls and accepts the other call which is waiting response.\n"
                            "2. Places all active calls on hold and accepts the other non-active call.\n"
                            "3. Adds a held call to the conversation.\n"
                            "4. Releases specified active call only (<idx>).\n"
                            "5. Request private consultation mode with specified call (<idx>).\n"
                            "Enter the hold option, or -1 to cancel.\n\n" ,
                            input,
                            1, &index);

                    if (ret < 0)
                        continue;

                    if( index == 0 )
                        hold_option = HFP_CALL_ACTION_CHLD_0;
                    else if( index == 1 )
                        hold_option = HFP_CALL_ACTION_CHLD_1;
                    else if( index == 2 )
                        hold_option = HFP_CALL_ACTION_CHLD_2;
                    else if( index == 3 )
                        hold_option = HFP_CALL_ACTION_CHLD_3;
                    else if( index == 4 )
                        hold_option = HFP_CALL_ACTION_CHLD_1x;
                    else if( index == 5 )
                        hold_option = HFP_CALL_ACTION_CHLD_2x;
                    else {
                        printf( "Incorrect option name.\n" );
                        continue;
                    }

                    if( (hold_option == HFP_CALL_ACTION_CHLD_1x) ||
                        (hold_option == HFP_CALL_ACTION_CHLD_2x) ){

                        ret = enter_index("\n"
                                "Enter the specified call number (line number ex:1, 2, 3, ..., 8), or -1 to cancel.\n\n" ,
                                input,
                                1, &index);

                        if (ret < 0)
                            continue;

                        specified_call_number = index;

                        if( specified_call_number <= 0 ||
                            specified_call_number >= 8 ){
                               printf( "Invalid input.\n" );
                               continue;
                        }

                    }

                    cmd_ret = NFBT_HFP_CallHold( &addr, (HFP_CALL_ACTION_CHLD) hold_option, (nfUINT8) specified_call_number );
                }
                else if( strcasecmp( input, "last" ) == 0 || strcasecmp( input, "c35" ) == 0 ){
                    cmd_ret = NFBT_HFP_Redial( &addr );
                    strcpy( str, "Re-dial" );
                }
                else if( strcasecmp( input, "spk" ) == 0 || strcasecmp( input, "c39" ) == 0 ){
                    strcpy( str, "Set speaker volume" );
                    int ret;
                    unsigned long long spk_volume = 0;
                    ret = enter_index("\n"
                            "Enter a number between 0 to 15 to set speaker volume, or -1 to cancel.\n\n" ,
                            input,
                            2, &spk_volume);

                    if( ret < 0 ||
                        spk_volume >15 ){
                        printf( "Invalid input.\n" );
                        continue;
                    }

                    cmd_ret = NFBT_HFP_SpkVolume( &addr, spk_volume );
                }
                else if( strcasecmp( input, "mic" ) == 0 || strcasecmp( input, "c40" ) == 0 ){
                    strcpy( str, "Set microphone volume" );
                    int ret;
                    unsigned long long  mic_volume = 0;
                    ret = enter_index("\n"
                            "Enter a number between 0 to 15 to set microphone volume, or -1 to cancel.\n\n" ,
                            input,
                            2, &mic_volume);

                    if( ret < 0 ||
                        mic_volume >15 ){
                        printf( "Invalid input.\n" );
                        continue;
                    }

                    cmd_ret = NFBT_HFP_MicVolume( &addr, mic_volume );
                }
                else if( strcasecmp( input, "asr" ) == 0 || strcasecmp( input, "c41" ) == 0 ){
                    strcpy( str, "Set Voice Recognition" );
                    int ret;
                    ret = enter_confirm("", input);

                    if( ret == 1 )
                        cmd_ret = NFBT_HFP_VoiceRecognition( &addr, HFP_VR_START );
                    else if( ret == 0 )
                        cmd_ret = NFBT_HFP_VoiceRecognition( &addr, HFP_VR_STOP );
                    else
                        continue;
                }
                else if( strcasecmp( input, "mynum" ) == 0 || strcasecmp( input, "c42" ) == 0 ){
                    cmd_ret = NFBT_HFP_SubscriberInfo( &addr );
                    strcpy( str, "Get subscriber info" );
                }
                else if( strcasecmp( input, "dial") == 0 || strcasecmp( input, "c34" ) == 0 ||
                         strcasecmp( input, "dtmf") == 0 || strcasecmp( input, "c36" ) == 0 ){
                    char phone_num[PHONE_NUM_MAX_LEN + 2];

                    do{
                        printf( "\n"
                                "Enter the phone number, or -1 to cancel.\n" );
                        fflush( stdin );
                        scanf( "%21s", phone_num );
                        fflush( stdin );

                        if( strcasecmp( phone_num, cancel ) == 0 ){
                        	strcpy( str, "Dial/Send DTMF canceled" );
                            ret = -1;
                            cmd_ret = NFBT_STATUS_SUCCESS;
                            break;
                        }
                        if( isLenConfrom( phone_num, PHONE_NUM_MAX_LEN, false ) == false )
                            continue;

                        if( strcasecmp( input, "dial" ) == 0 || strcasecmp( input, "c34" ) == 0 ){
                            cmd_ret = NFBT_HFP_Dial( &addr, strlen(phone_num), (nfUINT8 *)phone_num );
                            strcpy( str, "Dial" );
                        }
                        else if( strcasecmp( input, "dtmf") == 0 || strcasecmp( input, "c36" ) == 0 ){
                            char dtmf[2] = { '\0', '\0' };

                            size_t i;
                            for( i = 0; i < strlen( phone_num ); i++ ){
                                dtmf[0] = phone_num[i];
                                if( ( cmd_ret = NFBT_HFP_SendDTMF( &addr, dtmf[0] ) ) != NFBT_STATUS_SUCCESS )
                                    break;
                            }
                            strcpy( str, "Send DTMF" );
                        }
                        break;
                    }while(1);
                }
				else if( strcasecmp( input, "setauto" ) == 0 || strcasecmp( input, "c43" ) == 0 ){
					HFP_AUTO_REACTION auto_reaction;
                    unsigned long long index;

                    ret = enter_index("\n"
                            "0. no action.\n"
                            "1. auto answer.\n"
                            "2. auto reject.\n"
                            "Enter the hold option, or -1 to cancel.\n\n" ,
                            input,
                            1, &index);

                    if (ret < 0)
                        continue;

                    if( index == 0 ){
                    	auto_reaction = HF_AUTO_NO_ACTION;
                    	strcpy( str, "Set no reaction" );
                    }
                    else if( index == 1 ){
                    	auto_reaction = HF_AUTO_ANSWER;
                    	strcpy( str, "Set auto answer" );
                    }
                    else if( index == 2 ){
                    	auto_reaction = HF_AUTO_REJECT;
                    	strcpy( str, "Set auto reject" );
                    }
                    else {
                        printf( "Incorrect input.\n" );
                        continue;
                    }

                    cmd_ret = NFBT_HFP_SetCallAutoReaction( &addr, auto_reaction );

                    //AutoReactionTime
                    if( auto_reaction ==  HF_AUTO_ANSWER ||
						auto_reaction ==  HF_AUTO_REJECT ){

                    	ret = enter_index("\n"
                    			"Enter reaction time 1~60 Secs, or -1 to cancel.\n\n" ,
                    			input,
                    			2, &index);

                    	if (ret < 0)
                    		continue;

                    	if( index > 60 ){
                    		printf( "Invalid input.\n" );
                    		continue;
                    	}

                    	cmd_ret = NFBT_HFP_SetCallAutoReactionTime( &addr, (int)index );
                    }
				}
				else if( strcasecmp( input, "getauto" ) == 0 || strcasecmp( input, "c44" ) == 0 ){
					HFP_AUTO_REACTION auto_reaction;
					int sec = 0;

					cmd_ret = NFBT_HFP_GetCallAutoReaction( &addr, &auto_reaction );

					if( cmd_ret == NFBT_STATUS_SUCCESS ){
						if(auto_reaction == HF_AUTO_NO_ACTION){
							strcpy( str, "Get HFP reaction: no action" );
						}
						else if(auto_reaction == HF_AUTO_ANSWER){
							cmd_ret = NFBT_HFP_GetCallAutoReactionTime( &addr, &sec );
							snprintf( str, sizeof( str ), "Get HFP reaction: answer, reaction time = %d secs", sec );
						}
						else if(auto_reaction == HF_AUTO_REJECT){
							cmd_ret = NFBT_HFP_GetCallAutoReactionTime( &addr, &sec );
							snprintf( str, sizeof( str ), "Get HFP reaction: reject, reaction time = %d secs", sec );
						}
						else{
							strcpy( str, "Get HFP reaction: unknown" );
						}
					}
					else{
						strcpy( str, "Get HFP reaction:" );
					}
				}
				else if( strcasecmp( input, "setrt" ) == 0 || strcasecmp( input, "c45" ) == 0 ){
					int ret;
					strcpy( str, "Set ring tone" );
		            ret = enter_confirm(str, input);
		            if(ret == 1)
		                cmd_ret = NFBT_HFP_SetPlayRing( true );
		            else if(ret == 0)
		                cmd_ret = NFBT_HFP_SetPlayRing( false );
		            else
		                continue;
				}
				else if( strcasecmp( input, "getrt" ) == 0 || strcasecmp( input, "c46" ) == 0 ){
					nfBOOLEAN enable = 0;
					cmd_ret = NFBT_HFP_GetPlayRing( &enable );
					if( enable )
						strcpy( str, "play ringtone" );
					else
						strcpy( str, "no play ringtone" );
				}
				else if( strcasecmp( input, "setspkrender" ) == 0 || strcasecmp( input, "c47" ) == 0 ){
					int ret;
					strcpy( str, "Set speaker render" );
		            ret = enter_confirm(str, input);
		            if(ret == 1)
		                cmd_ret = NFBT_HFP_SetSpkRender( true );
		            else if(ret == 0)
		                cmd_ret = NFBT_HFP_SetSpkRender( false );
		            else
		                continue;
				}
				else if( strcasecmp( input, "getspkrender" ) == 0 || strcasecmp( input, "c48" ) == 0 ){
					nfBOOLEAN enable = 0;
					cmd_ret = NFBT_HFP_GetSpkRender( &enable );
					if( enable )
						strcpy( str, "speaker render ON" );
					else
						strcpy( str, "speaker render OFF" );
				}
				
                if( cmd_ret == NFBT_STATUS_SUCCESS )
                    printf( "%s successful\n", str );
                else{
                    print_cmd_error( cmd_ret );
                    printf( "%s failed\n", str );
                }
            }
        }
        else if(strcasecmp(input, "play"        ) == 0 || strcasecmp( input, "c50" ) == 0 ||
                strcasecmp(input, "pause"       ) == 0 || strcasecmp( input, "c51" ) == 0 ||
                strcasecmp(input, "stopp"       ) == 0 || strcasecmp( input, "c52" ) == 0 ||
                strcasecmp(input, "forward"     ) == 0 || strcasecmp( input, "c53" ) == 0 ||
                strcasecmp(input, "backward"    ) == 0 || strcasecmp( input, "c54" ) == 0 ||
                strcasecmp(input, "fastforward" ) == 0 || strcasecmp( input, "c55" ) == 0 ||
                strcasecmp(input, "stopff"      ) == 0 || strcasecmp( input, "c56" ) == 0 ||
                strcasecmp(input, "rewind"      ) == 0 || strcasecmp( input, "c57" ) == 0 ||
                strcasecmp(input, "stopr"       ) == 0 || strcasecmp( input, "c58" ) == 0 ||
                strcasecmp(input, "volup"       ) == 0 || strcasecmp( input, "c59" ) == 0 ||
                strcasecmp(input, "voldown"     ) == 0 || strcasecmp( input, "c60" ) == 0){
            int ret;
            ret = enter_address(bt_addr);

            if (ret < 0)
                continue;

            AVRCP_OP_ID         avrcp_op_id;
            AVRCP_BUTTON_STATUS avrcp_button_status = AVRCP_BUTTON_PRESS_AND_RELEASE;

            if (strcasecmp(input, "play") == 0 || strcasecmp( input, "c50" ) == 0 ) {
                avrcp_op_id = AVRCP_OPID_PLAY;
            }
            else if (strcasecmp(input, "pause") == 0 || strcasecmp( input, "c51" ) == 0) {
                avrcp_op_id = AVRCP_OPID_PAUSE;
            }
            else if (strcasecmp(input, "stopp") == 0 || strcasecmp( input, "c52" ) == 0) {
                avrcp_op_id = AVRCP_OPID_STOP;
            }
            else if (strcasecmp(input, "forward") == 0 || strcasecmp( input, "c53" ) == 0) {
                avrcp_op_id = AVRCP_OPID_FORWARD;
            }
            else if (strcasecmp(input, "backward") == 0 || strcasecmp( input, "c54" ) == 0) {
                avrcp_op_id = AVRCP_OPID_BACKWARD;
            }
            else if (strcasecmp(input, "fastforward") == 0 || strcasecmp( input, "c55" ) == 0) {
                avrcp_op_id = AVRCP_OPID_FAST_FORWARD;
                avrcp_button_status = AVRCP_BUTTON_PRESS;
            }
            else if (strcasecmp(input, "stopff") == 0 || strcasecmp( input, "c56" ) == 0) {
                avrcp_op_id = AVRCP_OPID_FAST_FORWARD;
                avrcp_button_status = AVRCP_BUTTON_RELEASE;
            }
            else if (strcasecmp(input, "rewind") == 0 || strcasecmp( input, "c57" ) == 0) {
                avrcp_op_id = AVRCP_OPID_REWIND;
                avrcp_button_status = AVRCP_BUTTON_PRESS;
            }
            else if (strcasecmp(input, "stopr") == 0 || strcasecmp( input, "c58" ) == 0) {
                avrcp_op_id = AVRCP_OPID_REWIND;
                avrcp_button_status = AVRCP_BUTTON_RELEASE;
            }
            else if (strcasecmp(input, "volup") == 0 || strcasecmp( input, "c59" ) == 0) {
                avrcp_op_id = AVRCP_OPID_VOL_UP;
            }
            else if (strcasecmp(input, "voldown") == 0 || strcasecmp( input, "c60" ) == 0) {
                avrcp_op_id = AVRCP_OPID_VOL_DOWN;
            } else
                continue;

            str_to_char( bt_addr, addr.address );
            cmd_ret = NFBT_AVRCP_ControlCmd(&addr, avrcp_op_id, avrcp_button_status);

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "successful\n" );
            }
            else {
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }
        }
        else if( strcasecmp( input, "listattr" ) == 0 || strcasecmp( input, "c61" ) == 0 ){
            int ret;
            ret = enter_address(bt_addr);

            if (ret < 0)
                continue;

            str_to_char( bt_addr, addr.address );
            cmd_ret = NFBT_AVRCP_ListPlayerAppAttr(&addr);

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "successful\n" );
            } else {
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }
        }
        else if( strcasecmp( input, "listvalue" ) == 0 || strcasecmp( input, "c62" ) == 0 ){
            int ret;
            unsigned long long index;
            ret = enter_address(bt_addr);

            if (ret < 0)
                continue;

            ret = enter_index("\n"
                        "1. Equalizer ON/OFF values.\n"
                        "2. Repeat Mode values.\n"
                        "3. Shuffle ON/OFF values.\n"
                        "4. Scan ON/OFF values.\n"
                        "Enter Player Application Settings or -1 to cancel.\n\n" ,
                        input,
                        1, &index);

            if (ret < 0)
                continue;

            if (index == 1) {
                cmd_ret = NFBT_AVRCP_ListPlayerAppValue(&addr,
                    AVRCP_PLAYER_ATTR_EQUALIZER);
            }
            else if (index == 2) {
                cmd_ret = NFBT_AVRCP_ListPlayerAppValue(&addr,
                    AVRCP_PLAYER_ATTR_REPEAT);
            }
            else if (index == 3) {
                cmd_ret = NFBT_AVRCP_ListPlayerAppValue(&addr,
                    AVRCP_PLAYER_ATTR_SHUFFLE);
            }
            else if (index == 4) {
                cmd_ret = NFBT_AVRCP_ListPlayerAppValue(&addr,
                    AVRCP_PLAYER_ATTR_SCAN);
            }
            else {
                printf( "Incorrect input.\n" );
                continue;
            }

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "successful\n" );
            } else {
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }
        }
        else if( strcasecmp( input, "nowplaying" ) == 0 || strcasecmp( input, "c63" ) == 0 ){
            int ret;
            ret = enter_address(bt_addr);

            if (ret < 0)
                continue;

            str_to_char( bt_addr, addr.address );
            cmd_ret = NFBT_AVRCP_GetElementAttributes(&addr);

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "successful\n" );
            } else {
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }
        }
        else if( strcasecmp( input, "pstatus" ) == 0 || strcasecmp( input, "c64" ) == 0 ){
            int ret;
            ret = enter_address(bt_addr);

            if (ret < 0)
                continue;

            str_to_char( bt_addr, addr.address );
            cmd_ret = NFBT_AVRCP_GetPlayStatus(&addr);

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "successful\n" );
            } else {
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }
        }
        else if( strcasecmp( input, "value" ) == 0 || strcasecmp( input, "c65" ) == 0 ){
            int ret;
            ret = enter_address(bt_addr);

            if (ret < 0)
                continue;

            str_to_char( bt_addr, addr.address );
            cmd_ret = NFBT_AVRCP_GetPlayerAPPValue(&addr);

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "successful\n" );
            }
            else{
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }
        }
        else if( strcasecmp( input, "change" ) == 0 || strcasecmp( input, "c66" ) == 0 ){
            int ret;
            unsigned long long index;
            cmd_ret = NFBT_STATUS_PARM_INVALID;

            ret = enter_address(bt_addr);

            if (ret < 0)
                continue;

            str_to_char( bt_addr, addr.address );

            ret = enter_index("\n"
                        "1. Equalizer ON/OFF status.\n"
                        "2. Repeat Mode status.\n"
                        "3. Shuffle ON/OFF status.\n"
                        "4. Scan ON/OFF status.\n"
                        "Enter Player Application Settings or -1 to cancel.\n\n" ,
                        input,
                        1, &index);

            if (ret < 0)
                continue;

            if(index == 1){
                ret = enter_index("\n"
                            "1. OFF\n"
                            "2. ON.\n"
                            "Enter Player Application Equalizer Value or -1 to cancel.\n\n" ,
                            input,
                            1, &index);

                if (ret < 0)
                    continue;

                if(index == 1){
                    cmd_ret = NFBT_AVRCP_SetPlayerAPPValue(&addr, AVRCP_PLAYER_ATTR_EQUALIZER, AVRCP_EQUALIZER_OFF);
                }
                else if(index == 2){
                    cmd_ret = NFBT_AVRCP_SetPlayerAPPValue(&addr, AVRCP_PLAYER_ATTR_EQUALIZER, AVRCP_EQUALIZER_ON);
                }
                else {
                    printf( "Incorrect input.\n" );
                    continue;
                }

            }
            else if(index == 2){
                ret = enter_index("\n"
                            "1. OFF\n"
                            "2. Single track repeat.\n"
                            "3. All track repeat.\n"
                            "4. Group repeat.\n"
                            "Enter Player Application Repeat Value or -1 to cancel.\n\n" ,
                            input,
                            1, &index);

                if (ret < 0)
                    continue;

                if(index == 1){
                    cmd_ret = NFBT_AVRCP_SetPlayerAPPValue(&addr, AVRCP_PLAYER_ATTR_REPEAT, AVRCP_REPEAT_OFF);
                }
                else if(index == 2){
                    cmd_ret = NFBT_AVRCP_SetPlayerAPPValue(&addr, AVRCP_PLAYER_ATTR_REPEAT, AVRCP_REPEAT_SINGLE);
                }
                else if(index == 3){
                    cmd_ret = NFBT_AVRCP_SetPlayerAPPValue(&addr, AVRCP_PLAYER_ATTR_REPEAT, AVRCP_REPEAT_ALL);
                }
                else if(index == 4){
                    cmd_ret = NFBT_AVRCP_SetPlayerAPPValue(&addr, AVRCP_PLAYER_ATTR_REPEAT, AVRCP_REPEAT_GROUP);
                }
                else {
                    printf( "Incorrect input.\n" );
                    continue;
                }

            }
            else if(index == 3){
                ret = enter_index("\n"
                            "1. OFF\n"
                            "2. All tracks shuffle.\n"
                            "3. Group shuffle.\n"
                            "Enter Player Application Shuffle Value or -1 to cancel.\n\n" ,
                            input,
                            1, &index);

                if (ret < 0)
                    continue;

                if(index == 1){
                    cmd_ret = NFBT_AVRCP_SetPlayerAPPValue(&addr, AVRCP_PLAYER_ATTR_SHUFFLE, AVRCP_SHUFFLE_OFF);
                }
                else if(index == 2){
                    cmd_ret = NFBT_AVRCP_SetPlayerAPPValue(&addr, AVRCP_PLAYER_ATTR_SHUFFLE, AVRCP_SHUFFLE_ALL);
                }
                else if(index == 3){
                    cmd_ret = NFBT_AVRCP_SetPlayerAPPValue(&addr, AVRCP_PLAYER_ATTR_SHUFFLE, AVRCP_SHUFFLE_GROUP);
                }
                else {
                    printf( "Incorrect input.\n" );
                    continue;
                }

            }
            else if(index == 4){
                ret = enter_index("\n"
                            "1. OFF\n"
                            "2. All tracks scan.\n"
                            "3. Group scan.\n"
                            "Enter Player Application Scan Value or -1 to cancel.\n\n" ,
                            input,
                            1, &index);

                if (ret < 0)
                    continue;

                if(index == 1){
                    cmd_ret = NFBT_AVRCP_SetPlayerAPPValue(&addr, AVRCP_PLAYER_ATTR_SCAN, AVRCP_SCAN_OFF);
                }
                else if(index == 2){
                    cmd_ret = NFBT_AVRCP_SetPlayerAPPValue(&addr, AVRCP_PLAYER_ATTR_SCAN, AVRCP_SCAN_ALL_TRACK);
                }
                else if(index == 3){
                    cmd_ret = NFBT_AVRCP_SetPlayerAPPValue(&addr, AVRCP_PLAYER_ATTR_SCAN, AVRCP_SCAN_GROUP);
                }
                else {
                    printf( "Incorrect input.\n" );
                    continue;
                }
            }
            else {
                printf( "Incorrect input.\n" );
                continue;
            }

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "successful\n" );
            }
            else{
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }
        }
        else if( strcasecmp( input, "notifyon") == 0 || strcasecmp( input, "c67" ) == 0) {
            int ret;
            ret = enter_address(bt_addr);

            if (ret < 0)
                continue;

            str_to_char( bt_addr, addr.address );
            cmd_ret = NFBT_AVRCP_EnableNotification(&addr);

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "successful\n" );
            }
            else{
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }

        }
        else if( strcasecmp( input, "notifyoff") == 0 || strcasecmp( input, "c68" ) == 0) {
            int ret;
            ret = enter_address(bt_addr);

            if (ret < 0)
                continue;

            str_to_char( bt_addr, addr.address );
            cmd_ret = NFBT_AVRCP_DisableNotification(&addr);

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "successful\n" );
            }
            else{
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }

        }
        else if( strcasecmp( input, "listing") == 0 || strcasecmp( input, "c70" ) == 0) {
            int ret;
            unsigned long long mode;

            ret = enter_address(bt_addr);

            if (ret < 0)
                continue;

            str_to_char( bt_addr, addr.address );

            ret = enter_index("\n"
							  "Enter the listing scope, or -1 to cancel.\n" 
							  "0 : Player \n" 
							  "1 : File System\n" 
							  "2 : Search\n"
							  "3 : Now Playing\n" ,
							  input,
							  1, &mode);

            if (ret < 0)
                continue;
			if (mode == 0){
				cmd_ret = NFBT_AVRCP_GetFolderItem(&addr, AVRCP_SCOPE_PLAYER_LIST);
			}
			else if (mode == 1){
				cmd_ret = NFBT_AVRCP_GetFolderItem(&addr, AVRCP_SCOPE_FILE_SYSTEM);
			}
			else if (mode == 2){
				cmd_ret = NFBT_AVRCP_GetFolderItem(&addr, AVRCP_SCOPE_SEARCH );
			}
			else if (mode == 3){
				cmd_ret = NFBT_AVRCP_GetFolderItem(&addr, AVRCP_SCOPE_NOW_PLAYING);
			}			

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "successful\n" );
            }
            else{
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }

        }
        else if( strcasecmp( input, "folderdown") == 0 || strcasecmp( input, "c71" ) == 0) {
            int ret;
            unsigned long long id;
            nfUINT16 uid_cnt;
            nfUINT64 uid;

            ret = enter_address(bt_addr);

            if (ret < 0)
                continue;

            str_to_char( bt_addr, addr.address );

            ret = enter_index("\n"
                        "Enter the UID counter of player, or -1 to cancel.\n\n" ,
                        input,
                        5, &id);

            if (ret < 0)
                continue;

            uid_cnt = id;

            ret = enter_index("\n"
                        "Enter the UID of sub folder, or -1 to cancel.\n\n" ,
                        input,
                        24, &id);

            if (ret < 0)
                continue;

            uid = id;

            cmd_ret = NFBT_AVRCP_ChangePath(&addr, AVRCP_PATH_DOWN, uid_cnt, uid);

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "successful\n" );
            }
            else{
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }

        }
        else if( strcasecmp( input, "folderup") == 0 || strcasecmp( input, "c72" ) == 0) {
            int ret;
            unsigned long long id;
            nfUINT16 uid_cnt;

            ret = enter_address(bt_addr);

            if (ret < 0)
                continue;

            str_to_char( bt_addr, addr.address );

            ret = enter_index("\n"
                        "Enter the UID counter of player, or -1 to cancel.\n\n" ,
                        input,
                        5, &id);

            if (ret < 0)
                continue;

            uid_cnt = id;

            cmd_ret = NFBT_AVRCP_ChangePath(&addr, AVRCP_PATH_UP, uid_cnt, 0);

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "successful\n" );
            }
            else{
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }

        }
        else if( strcasecmp( input, "getitemattr") == 0 || strcasecmp( input, "c73" ) == 0) {
            int ret;
            unsigned long long id;
            nfUINT16 uid_cnt;
            nfUINT64 uid;

            ret = enter_address(bt_addr);

            if (ret < 0)
                continue;

            str_to_char( bt_addr, addr.address );

            ret = enter_index("\n"
                        "Enter the UID counter of player, or -1 to cancel.\n\n" ,
                        input,
                        5, &id);

            if (ret < 0)
                continue;

            uid_cnt = id;

            ret = enter_index("\n"
                        "Enter the UID of sub folder, or -1 to cancel.\n\n" ,
                        input,
                        24, &id);

            if (ret < 0)
                continue;

            uid = id;

            cmd_ret = NFBT_AVRCP_GetItemAttributes(&addr, uid_cnt, uid);

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "successful\n" );
            }
            else{
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }

        }
        else if( strcasecmp( input, "playitem") == 0 || strcasecmp( input, "c74" ) == 0) {
            int ret;
            unsigned long long id;
            nfUINT16 uid_cnt;
            nfUINT64 uid;

            ret = enter_address(bt_addr);

            if (ret < 0)
                continue;

            str_to_char( bt_addr, addr.address );

            ret = enter_index("\n"
                        "Enter the UID counter of player, or -1 to cancel.\n\n" ,
                        input,
                        5, &id);

            if (ret < 0)
                continue;

            uid_cnt = id;


            ret = enter_index("\n"
                        "Enter the UID of sub folder, or -1 to cancel.\n\n" ,
                        input,
                        24, &id);

            if (ret < 0)
                continue;

            uid = id;

            cmd_ret = NFBT_AVRCP_PlayItem(&addr, AVRCP_SCOPE_FILE_SYSTEM, uid_cnt, uid);

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "successful\n" );
            }
            else{
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }

        }
        else if( strcasecmp( input, "setplayer") == 0 || strcasecmp( input, "c75" ) == 0) {
            int ret;
            nfUINT16 player_id;
            unsigned long long id, mode;
            ret = enter_address(bt_addr);

            if (ret < 0)
                continue;

            str_to_char( bt_addr, addr.address );

            ret = enter_index("\n"
							  "Enter the Funtion, or -1 to cancel.\n" 
							  "0 : Control \n" 
							  "1 : Browse \n",
							  input,
							  1, &mode);
            if (ret < 0)
                continue;

            ret = enter_index("\n"
                        "Enter the player ID, or -1 to cancel.\n\n" ,
                        input,
                        5, &id);

            if (ret < 0)
                continue;

            player_id = id;

			if (mode == 0){
				cmd_ret = NFBT_AVRCP_SetAddressedPlayer(&addr, player_id);
			}
			else if (mode == 1){
                cmd_ret = NFBT_AVRCP_SetBrowsedPlayer(&addr, player_id);
            }

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "successful\n" );
            }
            else{
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }

        }else if(strcasecmp( input, "setvol" ) == 0 || strcasecmp( input, "c78" ) == 0){

            char str[50];
            strcpy( str, "Set TG volume" );


        	     int ret;
        	     ret = enter_address(bt_addr);

                 if (ret < 0)
                     continue;

                 str_to_char( bt_addr, addr.address );

                 unsigned long long avrcp_volume = 0;
                 ret = enter_index("\n"
                         "Enter a number between 0% to 100% to set avrcp volume, or -1 to cancel.\n\n" ,
                         input,
                         3, &avrcp_volume);

                 if( ret < 0 || avrcp_volume > 100 ){
                     printf( "Invalid input.\n" );
                     continue;
                 }

                 cmd_ret = NFBT_AVRCP_SetVolumeChange( &addr, (avrcp_volume*127)/100 );
        }
        // a2dp
        else if (strcasecmp( input, "seta2dpmute" ) == 0 || strcasecmp( input, "c80" ) == 0){
			int ret;

            ret = enter_confirm("Set Music Mute ", input);
            if(ret == 1)
                cmd_ret = NFBT_A2DP_SetMute( true );
            else if(ret == 0)
                cmd_ret = NFBT_A2DP_SetMute( false );

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "successful\n" );
            }
            else{
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }
        }
        else if (strcasecmp( input, "geta2dpmute" ) == 0 || strcasecmp( input, "c81" ) == 0){
			nfBOOLEAN enable = 0;
			cmd_ret = NFBT_A2DP_GetMute( &enable );


            if( cmd_ret == NFBT_STATUS_SUCCESS ){
    			if( enable )
    				printf( "Music Mute successful\n" );
    			else
    				printf( "Music Unmute successful\n" );
            }
            else{
                print_cmd_error( cmd_ret );
                printf( "failed\n" );
            }
        }
        // pbap
        else if( strcasecmp( input, "contact" ) == 0 || strcasecmp( input, "c90" ) == 0 ) {
            int ret;
            unsigned long long index;

            ret = enter_address(bt_addr);

            if(ret == 1){
                str_to_char( bt_addr, addr.address );

                PB_ACCESS       access = PB_ACCESS_NULL;
                PB_PHOTO_ONOFF  photo  = PB_PHOTO_OFF;
                ret = enter_index( "\n"
                        "0. SIM Card\n"
                        "1. Phone\n"
                        "2. Missed Call\n"
                        "3. Received Call\n"
                        "4. Dialed Call\n"
                        "5. Recoreds Call\n"
                        "Enter the place to get phone book, or -1 to cancel.\n\n",
                        input,
                        1, &index);

                if (ret < 0)
                    continue;

                if (index == 0)
                    access = PB_ACCESS_SIM;
                else if(index == 1)
                    access = PB_ACCESS_PHONE;
                else if(index == 2)
                    access = PB_ACCESS_MISSED;
                else if(index == 3)
                    access = PB_ACCESS_RECEIVED;
                else if(index == 4)
                    access = PB_ACCESS_DIALED;
                else if(index == 5)
                    access = PB_ACCESS_COMBINE_CALLLOG;
                else if(index == 6)
                    access = PB_ACCESS_COMBINE_PHONEBOOK;
                else if(index == 7)
                    access = PB_ACCESS_COMBINE_PB_CALLLOG;
                else {
                    printf( "Incorrect input.\n" );
                    continue;
                }

                if (access <= PB_ACCESS_PHONE) {
                    ret = enter_confirm("", input);
                    if(ret == 1)
                        photo = PB_PHOTO_ON;
                    else if(ret == 0)
                        photo = PB_PHOTO_OFF;
                    else
                        continue;
                }

                cmd_ret = NFBT_PB_Download(&addr, access, photo);

                if( cmd_ret == NFBT_STATUS_SUCCESS )
                    printf( "Start download successful\n" );
                else{
                    print_cmd_error( cmd_ret );
                    printf( "Start download failed\n" );
                }
            }
        }
        else if( strcasecmp( input, "stopdl" ) == 0 || strcasecmp( input, "c91" ) == 0 ){
            int ret;
            ret = enter_address(bt_addr);

            if(ret == 1){
                str_to_char( bt_addr, addr.address );

                cmd_ret = NFBT_PB_DownloadStop(&addr);

                if( cmd_ret == NFBT_STATUS_SUCCESS )
                    printf( "Stop download successful\n" );
                else{
                    print_cmd_error( cmd_ret );
                    printf( "Stop download failed\n" );
                }
            }
        }
        else if( strcasecmp( input, "setmaxcount" ) == 0 || strcasecmp( input, "c92" ) == 0 ) {
        	int ret;
        	unsigned int max_count;
        	unsigned long long pb_count;

        	ret = enter_index( "\n"
        			"Enter a number between 1 to 65535 to set max count, or -1 to cancel.\n\n",
        			input,
        			5, &pb_count);

        	if (ret < 0)
        		continue;

        	max_count = (unsigned int)pb_count;
        	if( max_count == ((unsigned int) -1) || max_count > 65535 ){
        		printf( "Incorrect input.\n" );
        		continue;
        	}
        	cmd_ret = NFBT_PB_SetMaxCount(max_count);

            if( cmd_ret == NFBT_STATUS_SUCCESS ){
                printf( "Set download size of phonebook/calls history successful\n" );
            }
            else{
                print_cmd_error( cmd_ret );
                printf( "Set download size of phonebook/calls history failed\n" );
            }
        }
        else if( strcasecmp( input, "getmaxcount" ) == 0 || strcasecmp( input, "c93" ) == 0 ) {
        	unsigned int max_count = 0;

        	cmd_ret = NFBT_PB_GetMaxCount(&max_count);
        	if( cmd_ret == NFBT_STATUS_SUCCESS ){
       			printf( "Get download size of phonebook/calls history %u successful\n", max_count );
        	}
        	else{
        	    print_cmd_error( cmd_ret );
        	    printf( "Get download size of phonebook/calls history failed\n" );
        	}
        }
        //map
        else if( strcasecmp( input, "msgall" ) == 0 || strcasecmp( input, "c100" ) == 0 ){
            int ret;
            unsigned long long index;

            ret = enter_address(bt_addr);

            if(ret == 1){
                MAP_MSG_TYPE msg_type;
                MAP_FOLDER_STRUCT folder;
                int is_detail;

                str_to_char( bt_addr, addr.address );

                ret = enter_index( "\n"
                        "0. SMS GSM\n"
                        "1. SMS CDMA\n"
                        "2. MMS\n"
                        "3. E-mail\n"
                        "Enter the message type to get message, or -1 to cancel.\n\n",
                        input,
                        1, &index);

                if (ret < 0)
                    continue;

                if (index == 0)
                    msg_type = MAP_MSG_TYPE_SMS_GSM;
                else if (index == 1)
                    msg_type = MAP_MSG_TYPE_SMS_CDMA;
                else if (index == 2)
                    msg_type = MAP_MSG_TYPE_MMS;
                else if (index == 3)
                    msg_type = MAP_MSG_TYPE_EMAIL;
                else {
                    printf( "Incorrect input.\n" );
                    continue;
                }

                ret = enter_index( "\n"
                        "0. In Box\n"
                        "1. Sent\n"
                        "2. Deleted\n"
                        "3. Out Box\n"
                        "4. Draft\n"
                        "Enter the message box to get message, or -1 to cancel.\n\n",
                        input,
                        1, &index);

                if (ret < 0)
                    continue;

                if (index == 0)
                    folder = MAP_FOLDER_STRUCTURE_INBOX;
                else if (index == 1)
                    folder = MAP_FOLDER_STRUCTURE_SENT;
                else if (index == 2)
                    folder = MAP_FOLDER_STRUCTURE_DELETED;
                else if (index == 3)
                    folder = MAP_FOLDER_STRUCTURE_OUTBOX;
                else if (index == 4)
                    folder = MAP_FOLDER_STRUCTURE_DRAFT;
                else {
                    printf( "Incorrect input.\n" );
                    continue;
                }

                ret = enter_confirm("If download message detail?", input);
                if(ret == 1)
                    is_detail = 1;
                else if(ret == 0)
                    is_detail = 0;
                else continue;

                cmd_ret = NFBT_MAP_DownloadAllMessage(&addr, msg_type, folder, is_detail);

                if( cmd_ret == NFBT_STATUS_SUCCESS )
                    printf( "Download message successful\n" );
                else{
                    print_cmd_error( cmd_ret );
                    printf( "Download message failed\n" );
                }
            }
        }
        else if( strcasecmp( input, "msg" ) == 0 || strcasecmp( input, "c101" ) == 0 ){
            int ret;
            unsigned long long index;

            ret = enter_address(bt_addr);

            if(ret == 1){
                char handle[MAX_MAP_MSG_HANDLE_LEN];
                MAP_MSG_TYPE msg_type;
                MAP_FOLDER_STRUCT folder;

                str_to_char( bt_addr, addr.address );

                printf( "\n"
                        "Enter the message handle with maximum length 20, "
                        "or -1 to cancel.\n" );
                fflush( stdin );
                scanf( "%20s", handle );
                fflush( stdin );

                if( strcasecmp( handle, cancel ) == 0 ){
                    printf( "Download message canceled\n" );
                    continue;
                }

                if( isLenConfrom( handle, MAX_MAP_MSG_HANDLE_LEN, false ) == false ) {
                    printf("Invalid input\n");
                    continue;
                }

                ret = enter_index( "\n"
                        "0. SMS GSM\n"
                        "1. SMS CDMA\n"
                        "2. MMS\n"
                        "3. E-mail\n"
                        "Enter the message type to get message, or -1 to cancel.\n\n",
                        input,
                        1, &index);

                if (ret < 0)
                    continue;

                if (index == 0)
                    msg_type = MAP_MSG_TYPE_SMS_GSM;
                else if (index == 1)
                    msg_type = MAP_MSG_TYPE_SMS_CDMA;
                else if (index == 2)
                    msg_type = MAP_MSG_TYPE_MMS;
                else if (index == 3)
                    msg_type = MAP_MSG_TYPE_EMAIL;
                else {
                    printf( "Incorrect input.\n" );
                    continue;
                }

                ret = enter_index( "\n"
                        "0. In Box\n"
                        "1. Sent\n"
                        "2. Deleted\n"
                        "3. Out Box\n"
                        "4. Draft\n"
                        "Enter the message box to get message, or -1 to cancel.\n\n",
                        input,
                        1, &index);

                if (ret < 0)
                    continue;

                if (index == 0)
                    folder = MAP_FOLDER_STRUCTURE_INBOX;
                else if (index == 1)
                    folder = MAP_FOLDER_STRUCTURE_SENT;
                else if (index == 2)
                    folder = MAP_FOLDER_STRUCTURE_DELETED;
                else if (index == 3)
                    folder = MAP_FOLDER_STRUCTURE_OUTBOX;
                else if (index == 4)
                    folder = MAP_FOLDER_STRUCTURE_DRAFT;
                else {
                    printf( "Incorrect input.\n" );
                    continue;
                }

                cmd_ret = NFBT_MAP_DownloadMessage(&addr, (nfUINT8 *)handle,
                    msg_type, folder);

                if( cmd_ret == NFBT_STATUS_SUCCESS )
                    printf( "Download message successful\n" );
                else{
                    print_cmd_error( cmd_ret );
                    printf( "Download message failed\n" );
                }
            }
        }
        else if( strcasecmp( input, "stopm" ) == 0 || strcasecmp( input, "c102" ) == 0 ){
            int ret;
            ret = enter_address(bt_addr);

            if(ret == 1){
                str_to_char( bt_addr, addr.address );

                cmd_ret = NFBT_MAP_DownloadStop(&addr);

                if( cmd_ret == NFBT_STATUS_SUCCESS )
                    printf( "Stop download successful\n" );
                else{
                    print_cmd_error( cmd_ret );
                    printf( "Stop download failed\n" );
                }
            }
        }
        else if( strcasecmp( input, "notify" ) == 0 || strcasecmp( input, "c103" ) == 0 ){
            int ret;
            ret = enter_address(bt_addr);

            if(ret == 1){
                str_to_char( bt_addr, addr.address );

                ret = enter_confirm("", input);

                if( ret == 1 )
                    cmd_ret = NFBT_MAP_EnableNotification(&addr);
                else if( ret == 0 )
                    cmd_ret = NFBT_MAP_DisableNotification(&addr);
                else
                    continue;


                if( cmd_ret == NFBT_STATUS_SUCCESS )
                    printf( "Message notification successful\n" );
                else{
                    print_cmd_error( cmd_ret );
                    printf( "Message notification failed\n" );
                }
            }
        }
        else if( strcasecmp( input, "sendmsg" ) == 0 || strcasecmp( input, "c104" ) == 0 ){
            int ret;
            char phone_num[PHONE_NUM_MAX_LEN + 1];
            char message[MESSAGE_MAX_LEN + 1];

            ret = enter_address(bt_addr);

            if(ret == 1){
                str_to_char( bt_addr, addr.address );

                printf( "\n"
                        "Enter the phone number, or -1 to cancel.\n" );
                fflush( stdin );
                scanf( "%20s", phone_num );
                fflush( stdin );

                if( strcasecmp( phone_num, cancel ) == 0 ){
                    printf( "Send message canceled\n" );
                    ret = -1;
                    continue;
                }

                if( isLenConfrom( phone_num, PHONE_NUM_MAX_LEN, false ) == false ) {
                    printf("Invalid input\n");
                    continue;
                }

                printf( "\n"
                        "Enter the message, or -1 to cancel.\n" );
                fflush( stdin );
                scanf( " %512[^\n]s", message );
                fflush( stdin );

                if( strcasecmp( (char *)message, cancel ) == 0 ){
                    printf( "Send message canceled\n" );
                    ret = -1;
                    continue;
                }

                if( isLenConfrom( (char *)message, MESSAGE_MAX_LEN, false ) == false ) {
                    printf("Too much message detail\n");
                    continue;
                }

                cmd_ret = NFBT_MAP_SendMessage(&addr, (nfUINT8 *)phone_num, (nfUINT8 *)message);

                if( cmd_ret == NFBT_STATUS_SUCCESS )
                    printf( "Send message successful\n" );
                else{
                    print_cmd_error( cmd_ret );
                    printf( "Send message failed\n" );
                }
            }
        }
        else if( strcasecmp( input, "delmsg" ) == 0 || strcasecmp( input, "c105" ) == 0 ){
            int ret;
            unsigned long long index;

            ret = enter_address(bt_addr);

            if(ret == 1){
                char handle[MAX_MAP_MSG_HANDLE_LEN];
                MAP_FOLDER_STRUCT folder;

                str_to_char( bt_addr, addr.address );

                printf( "\n"
                        "Enter the message handle with maximum length 20, "
                        "or -1 to cancel.\n" );
                fflush( stdin );
                scanf( "%20s", handle );
                fflush( stdin );

                if( strcasecmp( handle, cancel ) == 0 ){
                    printf( "Delete message canceled\n" );
                    continue;
                }

                if( isLenConfrom( handle, MAX_MAP_MSG_HANDLE_LEN, false ) == false ) {
                    printf("Invalid input\n");
                    continue;
                }

                ret = enter_index( "\n"
                        "0. In Box\n"
                        "1. Sent\n"
                        "2. Deleted\n"
                        "3. Out Box\n"
                        "4. Draft\n"
                        "Enter the message box to get message, or -1 to cancel.\n\n",
                        input,
                        1, &index);

                if (ret < 0)
                    continue;

                if (index == 0)
                    folder = MAP_FOLDER_STRUCTURE_INBOX;
                else if (index == 1)
                    folder = MAP_FOLDER_STRUCTURE_SENT;
                else if (index == 2)
                    folder = MAP_FOLDER_STRUCTURE_DELETED;
                else if (index == 3)
                    folder = MAP_FOLDER_STRUCTURE_OUTBOX;
                else if (index == 4)
                    folder = MAP_FOLDER_STRUCTURE_DRAFT;
                else {
                    printf( "Incorrect input.\n" );
                    continue;
                }

                cmd_ret = NFBT_MAP_DeleteMessage(&addr, folder, (nfUINT8 *)handle);

                if( cmd_ret == NFBT_STATUS_SUCCESS )
                    printf( "Delete message successful\n" );
                else{
                    print_cmd_error( cmd_ret );
                    printf( "Delete message failed\n" );
                }
            }
        }
        else if( strcasecmp( input, "chgmsg" ) == 0 || strcasecmp( input, "c106" ) == 0 ){
            int ret;
            unsigned long long index;

            ret = enter_address(bt_addr);

            if(ret == 1){
                char handle[MAX_MAP_MSG_HANDLE_LEN];
                MAP_FOLDER_STRUCT folder;
                nfBOOLEAN is_read;

                str_to_char( bt_addr, addr.address );

                printf( "\n"
                        "Enter the message handle with maximum length 20, "
                        "or -1 to cancel.\n" );
                fflush( stdin );
                scanf( "%20s", handle );
                fflush( stdin );

                if( strcasecmp( handle, cancel ) == 0 ){
                    printf( "Delete message canceled\n" );
                    continue;
                }

                if( isLenConfrom( handle, MAX_MAP_MSG_HANDLE_LEN, false ) == false ) {
                    printf("Invalid input\n");
                    continue;
                }

                ret = enter_index( "\n"
                        "0. In Box\n"
                        "1. Sent\n"
                        "2. Deleted\n"
                        "3. Out Box\n"
                        "4. Draft\n"
                        "Enter the message box to get message, or -1 to cancel.\n\n",
                        input,
                        1, &index);

                if (ret < 0)
                    continue;

                if (index == 0)
                    folder = MAP_FOLDER_STRUCTURE_INBOX;
                else if (index == 1)
                    folder = MAP_FOLDER_STRUCTURE_SENT;
                else if (index == 2)
                    folder = MAP_FOLDER_STRUCTURE_DELETED;
                else if (index == 3)
                    folder = MAP_FOLDER_STRUCTURE_OUTBOX;
                else if (index == 4)
                    folder = MAP_FOLDER_STRUCTURE_DRAFT;
                else {
                    printf( "Incorrect input.\n" );
                    continue;
                }

                ret = enter_confirm("Set message is read?", input);

                if (ret == 1)
                    is_read = true;
                else if (ret == 0)
                    is_read = false;
                else continue;

                cmd_ret = NFBT_MAP_ChangeReadStatus(&addr, folder, (nfUINT8 *)handle, is_read);

                if( cmd_ret == NFBT_STATUS_SUCCESS )
                    printf( "Send message successful\n" );
                else{
                    print_cmd_error( cmd_ret );
                    printf( "Send message failed\n" );
                }
            }
        }
        else if( strcasecmp( input, "setmsgmaxcount" ) == 0 || strcasecmp( input, "c107" ) == 0 ||
        		 strcasecmp( input, "getmsgmaxcount" ) == 0 || strcasecmp( input, "c108" ) == 0 ){
        	unsigned short map_max_count;
        	if(strcasecmp( input, "setmsgmaxcount" ) == 0 || strcasecmp( input, "c107" ) == 0)
        	{
        		char *end;
        		while(1){
        			printf( "\n"
        					"Enter a number between 1 to 2048 to set max map count, or -1 to cancel.\n\n" );
        			fflush( stdin );
        			scanf( "%5s", input );
        			fflush( stdin );

        			if( strcasecmp( input, cancel ) == 0 ){
        				printf( "Download canceled\n" );
        				break;
        			}

        			if( isLenConfrom( input, 5, false ) == false )
        				continue;

        			map_max_count = (unsigned short)strtoul(input, &end, 10);
        			if( map_max_count == ((unsigned short) -1) || map_max_count > 2048 ){
        				printf( "Invalid input.\n" );
        				continue;
        			}

        			NFBT_MAP_SetMaxCount(map_max_count);
        			break;
        		}
        	}
        	else{
        		NFBT_MAP_GetMaxCount(&map_max_count);
        		printf("The max count of msg download is %d\n", map_max_count);
        	}
        }
		
        else if( strcasecmp( input, "eiruuidreg" ) == 0 || strcasecmp( input, "c111" ) == 0 )
        {
            ret = enter_uuid(uuid_str);
            if(ret == -1)
                continue;

            cmd_ret = NFBT_EIR_UUID_Register(uuid_str);

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf( "Register EIR_UUID successful\n\r" );
            else{
                print_cmd_error( cmd_ret );
                printf( "Register EIR_UUID failed\n\r" );
            }
        }
        else if( strcasecmp( input, "eiruuidunreg" ) == 0 || strcasecmp( input, "c112" ) == 0 )
        {
            ret = enter_uuid(uuid_str);
            if(ret == -1)
                continue;

            cmd_ret = NFBT_EIR_UUID_Unregister(uuid_str);

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf( "UnRegister EIR_UUID successful\n\r" );
            else{
                print_cmd_error( cmd_ret );
                printf( "UnRegister EIR_UUID failed\n\r" );
            }
        }		
		
        else if( strcasecmp( input, "rfreg" ) == 0 || strcasecmp( input, "c130" ) == 0 ||
                 strcasecmp( input, "rfunreg" ) == 0 || strcasecmp( input, "c131" ) == 0 ||
                 strcasecmp( input, "rfwrite" ) == 0 || strcasecmp( input, "c132" ) == 0 ){
            int ret;
            unsigned long long index;

            PROFILE_ID profile;
            char service[PROFILE_NAME_MAX + 2];

            ret = enter_index("\n"
                    "0. serial port -- SPP\n"
                    "1. iAP         -- Wireless iAP\n"
                    "2. RFCOMM 1    -- Custom rfcomm 1\n"
                    "3. RFCOMM 2    -- Custom rfcomm 2\n"
                    "4. RFCOMM 3    -- Custom rfcomm 3\n"
                    "5. RFCOMM 4    -- Custom rfcomm 4\n"
                    "6. RFCOMM 5    -- Custom rfcomm 5\n"
                    "7. RFCOMM 6    -- Custom rfcomm 6\n"
                    "8. RFCOMM 7    -- Custom rfcomm 7\n"
                    "9. RFCOMM 8    -- Custom rfcomm 8\n"
                    "10. RFCOMM 9    -- Custom rfcomm 9\n"
                    "Enter the service name, or -1 to cancel.\n\n" ,
                    service,
                    PROFILE_NAME_MAX, &index);

            if (ret < 0)
                continue;

            if( index == 0 )
                profile = NFBT_PID_SPP;
            else if( index == 1 )
                profile = NFBT_PID_IAP;
            else if( index == 2 )
                profile = NFBT_PID_RFCOMM_1;
            else if( index == 3 )
                profile = NFBT_PID_RFCOMM_2;
            else if( index == 4 )
                profile = NFBT_PID_RFCOMM_3;
            else if( index == 5 )
                profile = NFBT_PID_RFCOMM_4;
            else if( index == 6 )
                profile = NFBT_PID_RFCOMM_5;
            else if( index == 7 )
                profile = NFBT_PID_RFCOMM_6;
            else if( index == 8 )
                profile = NFBT_PID_RFCOMM_7;
            else if( index == 9 )
                profile = NFBT_PID_RFCOMM_8;
            else if( index == 10 )
                profile = NFBT_PID_RFCOMM_9;
            else {
                printf( "Incorrect service name.\n" );
                continue;
            }

            if( strcasecmp( input, "rfreg" ) == 0 || strcasecmp( input, "c130" ) == 0 ){
                ret = enter_uuid(uuid_str);
                if(ret == -1)
                    continue;

                cmd_ret = NFBT_RFCOMM_Register(profile, uuid_str);

                if( cmd_ret == NFBT_STATUS_SUCCESS )
                    printf( "Register RFCOMM service successful\n" );
                else{
                    print_cmd_error( cmd_ret );
                    printf( "Register RFCOMM service failed\n" );
                }
            }
            else if( strcasecmp( input, "rfunreg" ) == 0 || strcasecmp( input, "c131" ) == 0 ){
                cmd_ret = NFBT_RFCOMM_Unregister(profile);

                if( cmd_ret == NFBT_STATUS_SUCCESS )
                    printf( "Unregister RFCOMM service successful\n" );
                else{
                    print_cmd_error( cmd_ret );
                    printf( "Unregister RFCOMM service failed\n" );
                }
            }
            else if( strcasecmp( input, "rfwrite" ) == 0 || strcasecmp( input, "c132" ) == 0 ){
                char rfcomm_data_type[MAX_CMD_LEN + 2];
                char rfcomm_buf[RFCOMM_DATA_LEN]={0};
                int ret_len = -1;
                int rfcomm_data_len = 0;
                int spp_invalid_input = 0;
                int packet_index;
                int i;

                ret = enter_index("\n"
                            "0. Packet.\n"
                            "1. ASCII.\n"
                            "Enter the data type for writing to RFCOMM, or -1 to cancel.\n\n" ,
                            rfcomm_data_type,
                            MAX_CMD_LEN, &index);

                if (ret < 0)
                    continue;

                if(index == 0){
                    do{
                        ret = enter_data(rfcomm_buf);

                        if (ret == 1){
                            rfcomm_data_len = strlen(rfcomm_buf);

                            if ((rfcomm_data_len % 2)==1) {
                                printf("Invalid input.\n");
                                continue;
                            }
                            for (i = 0; i < rfcomm_data_len; i++) {
                                if (rfcomm_buf[i] >= '0' &&
                                    rfcomm_buf[i] <= '9') {
                                    rfcomm_buf[i] = rfcomm_buf[i] - '0';
                                }else if(rfcomm_buf[i] >= 'A' &&
                                         rfcomm_buf[i] <= 'F') {
                                    rfcomm_buf[i] = rfcomm_buf[i] - 'A' + 10;
                                }else if(rfcomm_buf[i] >= 'a' &&
                                         rfcomm_buf[i] <= 'f') {
                                    rfcomm_buf[i] = rfcomm_buf[i] - 'a' + 10;
                                } else {
                                    printf("Invalid input.\n");
                                    spp_invalid_input = 1;
                                    break;
                                }
                                if ((i % 2) != 0) {
                                    rfcomm_buf[i / 2] = 16 * (rfcomm_buf[i - 1]) + rfcomm_buf[i];
                                }
                            }
                            if (spp_invalid_input) {
                                spp_invalid_input = 0;
                                continue;
                            }

                            printf("\n");
                            printf("HEX: ");
                            for (packet_index = 0; packet_index < (rfcomm_data_len / 2); packet_index++) {
                                printf("%02X, ", (unsigned char) rfcomm_buf[packet_index]);
                            }
                            printf("\n");
                            rfcomm_data_len = (rfcomm_data_len / 2);

                            ret_len = NFBT_RFCOMM_Write(profile, (void*)rfcomm_buf, rfcomm_data_len);
                            if( ret_len >= 0 &&
                                ret_len == rfcomm_data_len)
                                printf( "Write data to RFCOMM service successful\n" );
                            else
                                printf( "Write data to RFCOMM service failed\n" );
                            break;
                        }
                        else
                            break;
                    }while(1);
                }
                else if (index == 1) {
                    printf("\n"
                            "Enter the data in string for maximum length %d.\n",
                            RFCOMM_DATA_LEN);
                    fflush(stdin);
                    scanf( "%1024s", rfcomm_buf );
                    fflush(stdin);

                    ret_len = NFBT_RFCOMM_Write(profile, (void*)rfcomm_buf, strlen(rfcomm_buf));
                    if (ret_len >= 0 &&
                        (size_t)ret_len == strlen(rfcomm_buf))
                        printf( "Write data to RFCOMM service successful\n" );
                    else
                        printf( "Write data to RFCOMM service failed\n" );
                }
            }
        }
        else if(strcasecmp( input, "filepush" ) == 0 || strcasecmp( input, "c140" ) == 0 ){
            int ret;
            char path_buf[PATH_MAX_LEN]={0};

            ret = enter_address(bt_addr);
            if(ret == 1){
            	str_to_char( bt_addr, addr.address );
                printf("\n"
                        "Enter the file path of upload files \n");
                fflush(stdin);
                scanf( "%1024s", path_buf );
                fflush(stdin);

                cmd_ret = NFBT_OPP_Push(&addr, (nfUINT8 *)path_buf);

                if( cmd_ret == NFBT_STATUS_SUCCESS )
                    printf( "file push succeed\n" );
                else{
                    print_cmd_error( cmd_ret );
                    printf( "file push failed\n" );
                }
            }
        }
        else if(strcasecmp( input, "stopt" ) == 0 || strcasecmp( input, "c141" ) == 0 ){
            int ret;
            char path_buf[PATH_MAX_LEN]={0};

            ret = enter_address(bt_addr);
            if(ret == 1){
            	str_to_char( bt_addr, addr.address );

                cmd_ret = NFBT_OPP_Stop(&addr);

                if( cmd_ret == NFBT_STATUS_SUCCESS )
                    printf( "file transfer stop succeed\n" );
                else{
                    print_cmd_error( cmd_ret );
                    printf( "file transfer stop failed\n" );
                }
            }
        }
        else if(strcasecmp( input, "folderpath" ) == 0 || strcasecmp( input, "c142" ) == 0 ){
            int ret;
            char path_buf[PATH_MAX_LEN]={0};
            printf("\n"
                   "Enter the folder path of download files \n");
            fflush(stdin);
            scanf( "%1024s", path_buf );
            fflush(stdin);

            cmd_ret = NFBT_OPP_SetDownloadPath((nfUINT8 *)path_buf);

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf( "set folder path succeed\n" );
            else{
                print_cmd_error( cmd_ret );
                printf( "set folder path failed\n" );
            }
        }
		else if(strcasecmp( input, "hidkey" ) == 0 || strcasecmp( input, "c150" ) == 0 ) {
			char key_buf[MAX_CMD_LEN + 2];
			unsigned long long index = 0;
			int ret;


			ret = enter_address(bt_addr);

			if (ret >= 0) {
				str_to_char( bt_addr, addr.address );

				ret = enter_index("\n"
						"0.  Menu\n"
						"1.  Home\n"
						"2.  Back\n"
						"Choose a key, or -1 to cancel.\n\n",
						key_buf,
						MAX_CMD_LEN,
						&index);

				if (ret < 0)
					continue;
			}
			else {
				continue;
			}

			cmd_ret = NFBT_HID_SendKey(&addr, index);

			if( cmd_ret == NFBT_STATUS_SUCCESS ) {
				printf( "HID send key succeed\n" );
			}
			else {
				print_cmd_error( cmd_ret );
				printf( "HID send key failed\n" );
			}
		}
		else if(strcasecmp( input, "hidmouserel" ) == 0 || strcasecmp( input, "c151" ) == 0 ) {
			char mouse_buf[MAX_CMD_LEN + 2];
			unsigned long long index = 0;
			nfINT16 axis_x, axis_y;
			int ret;


			ret = enter_address(bt_addr);

			if (ret >= 0) {
				str_to_char( bt_addr, addr.address );

				ret = enter_index("\n"
						"Enter the delta x value\n\n",
						mouse_buf,
						MAX_CMD_LEN,
						&index);

				if (ret < 0)
					continue;

				axis_x = index;

				ret = enter_index("\n"
						"Enter the delta y value\n\n",
						mouse_buf,
						MAX_CMD_LEN,
						&index);

				if (ret < 0)
					continue;

				axis_y = index;

				ret = enter_confirm("Press button?", input);
			}
			else {
				continue;
			}

			cmd_ret = NFBT_HID_SendMouseRel(&addr, axis_x, axis_y, ret);

			if( cmd_ret == NFBT_STATUS_SUCCESS ) {
				printf( "HID send mouse succeed\n" );
			}
			else {
				print_cmd_error( cmd_ret );
				printf( "HID send mouse failed\n" );
			}
		}
		else if(strcasecmp( input, "hidmouseabs" ) == 0 || strcasecmp( input, "c152" ) == 0 ) {
			char mouse_buf[MAX_CMD_LEN + 2];
			unsigned long long index = 0;
			nfINT16 axis_x, axis_y;
			int ret;


			ret = enter_address(bt_addr);

			if (ret >= 0) {
				str_to_char( bt_addr, addr.address );

				ret = enter_index("\n"
						"Enter the delta x value\n\n",
						mouse_buf,
						MAX_CMD_LEN,
						&index);

				if (ret < 0)
					continue;

				axis_x = index;

				ret = enter_index("\n"
						"Enter the delta y value\n\n",
						mouse_buf,
						MAX_CMD_LEN,
						&index);

				if (ret < 0)
					continue;

				axis_y = index;

				ret = enter_confirm("Press button?", input);
			}
			else {
				continue;
			}

			cmd_ret = NFBT_HID_SendMouseAbs(&addr, axis_x, axis_y, ret);

			if( cmd_ret == NFBT_STATUS_SUCCESS ) {
				printf( "HID send mouse succeed\n" );
			}
			else {
				print_cmd_error( cmd_ret );
				printf( "HID send mouse failed\n" );
			}
		}
        else if( strcasecmp( input, "help" ) == 0 || strcasecmp( input, "c200" ) == 0 ){
            printf("\n"
                    "******************************************** Help ********************************************\n"
                    "bt_enable(c1)\t\t: Enable BT stack.\n"
                    "bt_disable(c2)\t\t: Disable BT stack.\n"
                    "localinfos(c3)\t\t: Get local device infos.\n"
                    "setname(c4)\t\t: Set local device name.\n"
                    "setscan(c5)\t\t: Set scan mode.\n"
                    "scan(c6)\t\t: Scan nearby bluetooth devices.\n"
                    "stop(c7)\t\t: Stop scan nearby bluetooth devices.\n"
                    "pairlist(c8)\t\t: List paired remote device.\n"
                    "pair(c9)\t\t: Pair with remote device.\n"
                    "delete(c10)\t\t: Delete paired remote device.\n"
                    "connect(c13)\t\t: Connect to paired remote device.\n"
                    "disconnect(c14)\t\t: Disconnect to paired remote device.\n"
                    "switch(c15)\t\t: Switching current remote device. \n"
					"setprofile(c16)\t\t: set enable profile\n"
					"getprofile(c17)\t\t: get enable profile\n"
            		"address(c18)\t\t: get bluetooth address of local device\n"
                    "******************************************** HFP *********************************************\n"
                    "transfer(c30)\t\t: Transfer audio to audio gate or handfree unit.\n"
                    "answer(c32)\t\t: Answer the current incoming call.\n"
                    "reject(c33)\t\t: Reject the current incoming call.\n"
                    "dial(c34)\t\t: Make a phone call.\n"
                    "last(c35)\t\t: Re-dial the last phone call.\n"
                    "dtmf(c36)\t\t: Send DTMF command when there is a current call.\n"
                    "terminate(c37)\t\t: Terminate the current call.\n"
                    "hold(c38)\t\t: Hold call.\n"
                    "spk(c39)\t\t: Set speaker volume.\n"
                    "mic(c40)\t\t: Set microphone volume.\n"
                    "asr(c41)\t\t: Trigger Voice Recognition. \n"
                    "mynum(c42)\t\t: Get the subscriber number information.\n"
                    "setauto(c43)\t\t: set automatic reaction for incoming call.\n"
                    "getauto(c44)\t\t: get automatic reaction for incoming call.\n"
                    "setrt(c45)\t\t: set play ringtone. \n"
                    "getrt(c46)\t\t: get play ringtone. \n"
					"setspkrender(c47)\t\t: set speaker render. \n"
                    "getspkrender(c48)\t\t: get speaker render. \n"
					"hangans(c491)\t\t: hang off current call and answer incoming call. \n"
            		"holdans(c492)\t\t: hold current call(disconnect sco) and answer incoming call. \n"
            		"changsco(c493)\t\t: change connected sco. \n"
            		"devaddr(c494)\t\t: input two devices' bluetooth address for multi-hfp test only. \n"
                    "******************************************** AVRCP *******************************************\n"
                    "play(c50)\t\t: Play music which is stored in remote device.\n"
                    "pause(c51)\t\t: Pause the current playing music.\n"
                    "stopp(c52)\t\t: Stop playing music.\n"
                    "forward(c53)\t\t: Change playing music to next one.\n"
                    "backward(c54)\t\t: Back to start position of current playing music.\n"
                    "fastforward(c55)\t: Fast forward position of current playing music.\n"
                    "stopff(c56)\t\t: Stop fast forward.\n"
                    "rewind(c57)\t\t: Rewind position of current playing music.\n"
                    "stopr(c58)\t\t: Stop rewind.\n"
                    "volup(c59)\t\t: Volume up.\n"
                    "voldown(c60)\t\t: Volume down.\n"
                    "listattr(c61)\t\t: List player available setting attributes.\n"
                    "listvalue(c62)\t\t: List player available setting value of attributes.\n"
                    "nowplaying(c63)\t\t: Get the attributes of current playing music.\n"
                    "pstatus(c64)\t\t: Get the playing status.\n"
                    "value(c65)\t\t: Get the current setting of remote target player.\n"
                    "change(c66)\t\t: Change the player setting of given attributes.\n"
                    "notifyon(c67)\t\t: Enable status notification which is device supported.\n"
                    "notifyoff(c68)\t\t: Disable status notification.\n"
                    "listing(c70)\t\t: List scope and show information.\n"
                    "folderdown(c71)\t\t: Change now folder path to selected child folder path.\n"
                    "folderup(c72)\t\t: Change now folder path to mother folder path.\n"
                    "getitemattr(c73)\t: Get the attributes of selected music.\n"
                    "playitem(c74)\t\t: Play selected music.\n"
                    "setplayer(c75)\t\t: Set player or browsed player.\n"
                    "setvol(c78)\t\t:  Set remote device arvcrp volume.\n"
                    "******************************************** A2DP *********************************************\n"
                    "seta2dpmute(c80)\t: mute or unmute a2dp stream.\n"
                    "geta2dpmute(c81)\t: get a2dp mute status.\n"
                    "******************************************** PBAP ********************************************\n"
                    "contact(c90)\t\t: Get the contact information saved in phone or SIM card, or call history.\n"
                    "stopdl(c91)\t\t: Stop download the contact information.\n"
                    "setmaxcount(c92)\t: set download size of phonebook/calls history. \n"
                    "getmaxcount(c93)\t: get download size of phonebook/calls history. \n"
                    "******************************************** MAP *********************************************\n"
                    "msgall(c100)\t\t: Get all message saved in phone\n"
                    "msg(c101)\t\t: Get selected message saved in phone\n"
                    "stopm(c102)\t\t: Stop download the message.\n"
                    "notify(c103)\t\t: Enable/Disable notification to receive message event on remote device.\n"
                    "sendmsg(c104)\t\t: Send message file to remote device.\n"
                    "delmsg(c105)\t\t: Delete message on remote device.\n"
                    "chgmsg(c106)\t\t: Change message read status on remote device.\n"
                    "setmsgmaxcount(c107)\t:set the message max listing count.\n"
                    "getmsgmaxcount(c108)\t:get the message max listing count.\n"
                    "******************************************** EIR *********************************************\n"
                    "eiruuidreg(c111)\t\t: Register EIR UUID.\n"
                    "eiruuidunreg(C112)\t\t: Unregister EIR UUID.\n"
                    "******************************************** RFCOMM *********************************************\n"
                    "rfreg(c130)\t\t: Register RFCOMM service by UUID.\n"
                    "rfunreg(c131)\t\t: Unregister existing RFCOMM service.\n"
                    "rfwrite(c132)\t\t: Write data to specific RFCOMM service.\n"
                    "******************************************** OPP *********************************************\n"
                    "filepush(c140)\t\t: Upload file to target device.\n"
                    "stopt(c141)\t\t: Stop file transfer.\n"
                    "folderpath(c142)\t: Change the folder path of download files.\n"
                    "******************************************** HID *********************************************\n"
                    "hidkey(c150)\t\t: Send HID key report.\n"
                    "hidmouserel(c151)\t: Send HID mouse report(relative coordinate).\n"
                    "hidmouseabs(c152)\t: Send HID mouse report(absolute coordinate).\n"
                    #ifdef BLE_ENABLE
                    "******************************************** GATT *********************************************\n"
            		"gattscanstart(c303)\t\t: start scan peripheral device.\n"
            		"gattscanstop(c304)\t\t: stop scan peripheral device.\n"
            		"gattlisten(c311)\t\t: start/stop advertising to nearby device.\n"
            		"gattsearchservice(c312)\t\t: search all service uuid.\n"
            		"gattreadrssi(c313)\t\t: read rssi value.\n"
            		"gattgetcharacteristic(c314)\t\t: get characteristic uuid by service uuid.\n"
            		"gattgetdescriptor(c315)\t\t: get descriptor uuid by service and character uuid.\n"
            		"gattnotifyreg(c316)\t\t: register characteristic is notify/indicator.\n"
            		"gattreadcharacteristic(c317)\t\t: read characteristic value by uuid.\n"
            		"gattwritecharacteristic(c318)\t\t: write characteristic value by uuid.\n"
            		"gattgetincludeservice(c320)\t\t: get include service uuid by service uuid.\n"
            		"gattsetadvdata(c323)\t\t: set advertising data.\n"
            		"gattreaddescriptor(c324)\t\t: read descriptor databy uuid.\n"
            		"gattwritedescriptor(c325)\t\t: write data to descriptor by uuid.\n"
            		"gattconnupdate(c326)\t\t: update connection parameter.\n"
            		#ifdef GATT_SPP_DEMO
                    "demosppregister(c328)\t\t: GATT SPP service register.\n"
                    "demosppwrite(c329)\t\t: GATT send SPP data.\n"
            		"demosppunregister(c330)\t\t: GATT SPP service un-register.\n"
                    #endif /* GATT_SPP_DEMO */
            		"gattserviceregister(c331)\t\t: register service .\n"
            		"gattserviceunregister(c332)\t\t: unregister service.\n"
            		"gattaddservice(c334)\t\t: add service UUID.\n"
            		"gattaddcharacteristic(c336)\t\t: add characteristic UUID.\n"
            		"gattadddescriptor(c337)\t\t: add characteristic descriptor UUID.\n"
            		"gattstartservice(c338)\t\t: start already register service .\n"
            		"gattdelservice(c339)\t\t: delete service UUID.\n"
            		"gattsendindicator(c340)\t\t: send indicator/notify data by attribute handle.\n"
            		"gattsetrole(c345)\t\t: set central or peripheral role.\n"
            		"gattservicelist(c346)\t\t: list all service uuid and handle.\n"
            		"gattcharacterlist(c347)\t\t: list characteric information.\n"
            	    "gattaccesswhitelist(c352)\t\t: Access while list.\n"
                    #endif
                    "********************************************Others************************************************\n"
                    "exit(c0)\t\t: Exit this program.\n"
                    "help(c200)\t\t: Help information of this program.\n"
                    "log(c201)\t\t: Enable/disable log.\n"
                    "ssp(c202)\t\t: Enable/disable ssp mode.\n"
                    "autoreconn(c203)\t: Enable/disable auto reconnect.\n"
                    "setaudio(c204)\t\t: Set A2dp stream output device and Ring output device\n"
                    "test(c205)\t\t: Enter/Exit Bluetooth Test Mode.\n"
                    "bletest(c206)\t\t: Ble RF test.\n"
                    "rftest(c207)\t\t: Bluetooth RF test.\n"
                    "setmaxpaired(c208)\t\t: Set max paired device number.\n"
                    "getmaxpaired(c209)\t\t: Get max paired device number.\n"
                    "seta2dp(c212)\t\t: Set A2dp stream output device only.\n"
                    "setring(c213)\t\t: Set Ring output device only.\n"
                    "geta2dp(c214)\t: Get A2DP device name.\n"
                    "getring(c215)\t\t\t: Get ringtone file path.\n"
                    "setringpth(c216)\t\t: Get ringtone file path.\n"
                    "****************************************** Help End ******************************************\n\n");
        }
        else if( strcasecmp( input, "log" ) == 0 || strcasecmp( input, "c201" ) == 0 ){
            int ret;
            ret = enter_confirm("", input);

            if( ret == 1 ){
                cmd_ret = NFBT_SetShowLog(true);
            	NFBT_SetDumpHci(true);
            }
            else if( ret == 0 )
                cmd_ret = NFBT_SetShowLog(false);
            else
                continue;

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf( "Set log succeed\n" );
            else {
                print_cmd_error( cmd_ret );
                printf( "Set log failed\n" );
            }
        }
        else if( strcasecmp( input, "ssp" ) == 0 || strcasecmp( input, "c202" ) == 0 ){
            int ret;
            ret = enter_confirm("", input);

            if( ret == 1 )
                cmd_ret = NFBT_SetSSPMode(true);
            else if( ret == 0 )
                cmd_ret = NFBT_SetSSPMode(false);
            else
                continue;

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf( "Set SSP mode succeed\n" );
            else{
                print_cmd_error( cmd_ret );
                printf( "Set SSP mode failed\n" );
            }
        }
        else if( strcasecmp( input, "autoreconn" ) == 0 || strcasecmp( input, "c203" ) == 0 ){
            int ret;
            ret = enter_confirm("", input);

            if( ret == 1 )
                cmd_ret = NFBT_SetAutoReconnection(3);
            else if( ret == 0 )
                cmd_ret = NFBT_SetAutoReconnection(0);
            else
                continue;

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf( "Set auto reconnect succeed\n" );
            else{
                print_cmd_error( cmd_ret );
                printf( "Set auto reconnect failed\n" );
            }
        }
        else if(strcasecmp( input, "setaudio" ) == 0 || strcasecmp( input, "c204" ) == 0 ||
        		strcasecmp( input, "seta2dp" ) == 0 || strcasecmp( input, "c212" ) == 0 ||
        		strcasecmp( input, "setringpth" ) == 0 || strcasecmp( input, "c216" ) == 0 ||
        		strcasecmp( input, "setring" ) == 0 || strcasecmp( input, "c213" ) == 0){
            char path_buf[PATH_MAX_LEN]={0};

            printf("\n"
                   "Enter the audio dev of media play \n");
            fflush(stdin);
            scanf( "%1024s", path_buf );
            fflush(stdin);

            if(strcasecmp( input, "setaudio" ) == 0 || strcasecmp( input, "c204" ) == 0)
            	NFBT_SetAudioDevice(path_buf);
            else if(strcasecmp( input, "seta2dp" ) == 0 || strcasecmp( input, "c212" ) == 0)
            	NFBT_SetA2DPDevice(path_buf);
			else if(strcasecmp( input, "setringpth" ) == 0 || strcasecmp( input, "c216" ) == 0 )
            	NFBT_SetRingPath(path_buf);
            else
            	NFBT_SetRingDevice(path_buf);
        }
		else if(strcasecmp( input, "geta2dp" ) == 0 || strcasecmp( input, "c214" ) == 0 ||
        		strcasecmp( input, "getring" ) == 0 || strcasecmp( input, "c215" ) == 0){
            char name_buf[PATH_MAX_LEN]={0};

            if(strcasecmp( input, "geta2dp" ) == 0 || strcasecmp( input, "c214" ) == 0)
			{
            	NFBT_GetA2DPDevice(name_buf, PATH_MAX_LEN);
				printf("\nthe A2DP dev name is [%s] \n", name_buf);
			}
            else
			{
            	NFBT_GetRingDevice(name_buf, PATH_MAX_LEN);
				printf("\nthe RING dev name is [%s] \n", name_buf);
            	NFBT_GetRingPath(name_buf, PATH_MAX_LEN);
				printf("\nthe RING path name is [%s] \n", name_buf);
			}
        }
        else if( strcasecmp( input, "test" ) == 0 || strcasecmp( input, "c205" ) == 0 ){
            int ret;
            ret = enter_confirm("", input);

            if( ret == 1 ){
                cmd_ret = NFBT_SetTestMode(true);
            	NFBT_SetDumpHci(true);
            }
            else if( ret == 0 )
                cmd_ret = NFBT_SetTestMode(false);
            else
                continue;

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf( "Set Test Mode succeed\n" );
            else {
                print_cmd_error( cmd_ret );
                printf( "Set Test Mode failed\n" );
            }
        }
        else if( strcasecmp( input, "bletest" ) == 0 || strcasecmp( input, "c206" ) == 0 ){
            int ret;
            unsigned long long freq, method;//, packet_type;

            ret = enter_index("\n"
                        "Enter the test method, or -1 to cancel.\n" 
                        "0 : end test process \n" 
                        "1 : RX mode\n" 
                        "2 : TX mode\n" ,
                        input,
                        1, &method);

            if (ret < 0)
                continue;

	     if(method > 0) {
                ret = enter_index("\n"
                        "Enter the frequence , or -1 to cancel.\n" 
                        "freq. range from 2402 to 2480",
                        input,
                        4, &freq);

                if (ret < 0)
                    continue;
	     }

            if( ret == 0 )
                cmd_ret = NFBT_SetBleTestMode((nfUINT8)method, (nfUINT16)freq);
            else
                continue;

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf( "Set BLE Test Mode succeed\n" );
            else {
                print_cmd_error( cmd_ret );
                printf( "Set BLE Test Mode failed\n" );
            }
        }
        else if( strcasecmp( input, "rftest" ) == 0 || strcasecmp( input, "c207" ) == 0 ){
            int ret;
            unsigned long long freq = 0, method = 0, power = 0, data_type = 0, modulation = 0xff;
            unsigned long long hopping = 0;

            ret = enter_index("\n"
                        "Enter the test method, or -1 to cancel.\n" 
                        "0 : end test process \n" 
                        "1 : RX mode\n" 
                        "2 : TX mode\n"
                        "3 : TX modulation mode\n",
                        input,
                        1, &method);

            if (ret < 0)
                continue;

            if(method > 0) {
                ret = enter_index("\n"
                        "Enter the frequence , or -1 to cancel.\n" 
                        "freq. range from 2402 to 2480",
                        input,
                        4, &freq);

                if (ret < 0)
                    continue;

                if(method == DUT_TX_MODE)
                {
                    ret = enter_index("\n"
                        "Whether hopping mode, or -1 to cancel.\n" 
                        "0 : single freq.\n" 
                        "1 : hopping\n",
                        input,
                        1, &hopping);

                    if(ret < 0)
                        continue;
	     	}

                if(hopping == 1)
                    freq |= 0x8000;
	     }

	     if(method == DUT_TX_MODE || method == DUT_TX_MOD_MODE) {
                ret = enter_index("\n"
                        "Enter the TX power level , or -1 to cancel.\n" 
                        "power level range from 0 to 7",
                        input,
                        1, &power);

                if (ret < 0)
                    continue;

                if(method == DUT_TX_MOD_MODE)
                {
	             ret = enter_index("\n"
	                        "Enter the modulation , or -1 to cancel.\n" 
	                        "1 : GFSK\n" 
	                        "2 : 4-DQPSK\n"
	                        "3 : 8-DPSK\n"
                                "4 : LE\n",
	                        input,
	                        1, &modulation);

	             if(ret < 0)
	                 continue;
                }
                else
                {
                    ret = enter_index("\n"
                        "Enter the data type , or -1 to cancel.\n" 
                        "0 : DM1\n" "1 : DH1\n" 
                        "2 : DM3\n" "3 : DH3\n" 
                        "4 : DM5\n" "5 : DH5\n" 
                        "6 : 2-DH1\n" "7 : 2-DH3\n" 
                        "8 : 2-DH5\n" "9 : 3-DH1\n" 
                        "10 : 3-DH3\n" "11 : 3-DH5\n",
                        input,
                        2, &data_type);

                    if(ret < 0)
                        continue;
                }
	     }

            if( ret == 0 )
                cmd_ret = NFBT_SetBrEdrTestMode((nfUINT8)method, (nfUINT16)freq, (nfUINT8)power, (nfUINT8)data_type, (nfUINT8)modulation);
            else
                continue;

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf( "Set RF Test Mode succeed\n" );
            else {
                print_cmd_error( cmd_ret );
                printf( "Set RF Test Mode failed\n" );
            }
        }
        else if( strcasecmp(input,"setmaxpaired") == 0 || strcasecmp(input , "c208") ==0 ){

        	     int max_paired;
        		 char *end;
        		 do{

        			    printf( "\n"
        			                "Enter the max-paired device number or -1 to cancel.\n");

        			        fflush( stdin );
        			        scanf( "%20s", input );

        			        if( strcasecmp( input, cancel ) == 0 ){
        			            printf( "canceled\n");
        			            break;
        			        }

        			        if( isLenConfrom( input, 3 , false ) == false )
        			            continue;


        			        max_paired = strtoull(input, &end , 10);
        			        if (max_paired >=1000){
        			            printf( "Invalid input.\n" );
        			            continue;
        			        }

        				cmd_ret = NFBT_SetPairedDeviceNum( max_paired );
        				if( cmd_ret == NFBT_STATUS_SUCCESS ){

        					printf("setting max-paired device success\n");
        				}
        				else {
        					print_cmd_error( cmd_ret );
        				}
        				break;

        			}while(1);

        }else if( strcasecmp(input,"getmaxpaired") == 0 || strcasecmp(input , "c209") ==0 ){

        		  int max_paired;

        		  do{
        				fflush( stdin );

        				cmd_ret = NFBT_GetPairedDeviceNum( &max_paired );
        				if( cmd_ret == NFBT_STATUS_SUCCESS ){
        					printf("The max-paired device is %d.\n",max_paired );
        				}
        				else {
        					print_cmd_error( cmd_ret );
        				}
        				break;

        		  }while(1);

        }else if( strcasecmp( input, "setseqpairlist" ) == 0 || strcasecmp( input, "c210" ) == 0 ){
            cmd_ret = NFBT_SetPairedDeviceSoft(1);

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf("Set Sequence paired device success\n");
            else
                print_cmd_error( cmd_ret );

        }else if( strcasecmp( input, "setautoreconnmode" ) == 0 || strcasecmp( input, "c211" ) == 0 ){
            cmd_ret = NFBT_SetAutoReconnectionMode(1);

            if( cmd_ret == NFBT_STATUS_SUCCESS )
                printf("Set Auto Reconnection Mode success\n");
            else
                print_cmd_error( cmd_ret );
        }
		else if(strcasecmp( input, "gattscanstart" ) == 0 || strcasecmp( input, "c303" ) == 0){
			cmd_ret = NFBT_GATT_Scan(1);

			if( cmd_ret  == NFBT_STATUS_SUCCESS )
				printf("LE Scan enable succeed\n");
			else {
				print_cmd_error( cmd_ret );
				printf("LE Scan enable failed\n");
			}
		}	 
		else if(strcasecmp( input, "gattscanstop" ) == 0 || strcasecmp( input, "c304" ) == 0){
			cmd_ret = NFBT_GATT_Scan(0);

			if( cmd_ret  == NFBT_STATUS_SUCCESS )
				printf("LE Scan disable succeed\n");
			else {
				print_cmd_error( cmd_ret );
				printf("LE Scan disable failed\n");
			}
		}
		else if(strcasecmp(input, "gattlisten" ) == 0 || strcasecmp( input, "c311" ) == 0){
			unsigned long long start;
			unsigned long long type;
			if(-1 == enter_index("\n"
                    "Enter a number between 0 to 1 to stop/start listen incoming connect, or -1 to cancel.\n" ,
                    input,
                    1, &start))
                    continue;

			if(start == 1)
			if(-1 == enter_index("\n"
                    "Enter a number between 0 to 1 to non-connect/connect advertising type, or -1 to cancel.\n" ,
                    input,
                    1, &type))
                    continue;

			if(NFBT_GATT_Listen(start, type) == NFBT_STATUS_SUCCESS )
				printf("Listen operation succeed\n");
			else{
				print_cmd_error( cmd_ret );
				printf("Listen operation failed\n");
			}
		}
		else if(strcasecmp(input, "gattsearchservice" ) == 0 || strcasecmp( input, "c312" ) == 0){
			if(NFBT_GATT_SearchService() != NFBT_STATUS_SUCCESS ){
				print_cmd_error( cmd_ret );
				printf( "Search service failed\n" );
			}
		}
		else if(strcasecmp(input, "gattreadrssi" ) == 0 || strcasecmp( input, "c313" ) == 0){
			if(NFBT_GATT_GetRssi() != NFBT_STATUS_SUCCESS ){
				print_cmd_error( cmd_ret );
				printf("Read remote RSSI failed\n");
			}		
		}
		else if(strcasecmp(input, "gattgetcharacteristic" ) == 0 || strcasecmp( input, "c314" ) == 0){
			if(-1 == enter_uuid_x("\n"
	                    "Enter the service uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , uuid_str, 32))
	                    continue;

			if(NFBT_GATT_GetCharacteristic(uuid_str) != NFBT_STATUS_SUCCESS ){
				print_cmd_error( cmd_ret );
				printf("Get characteristic failed\n");
			}
		}
		else if(strcasecmp(input, "gattgetdescriptor" ) == 0 || strcasecmp( input, "c315" ) == 0){
			if(-1 == enter_uuid_x("\n"
	                    "Enter the service uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , uuid_str, 32))
	                    continue;

			if(-1 == enter_uuid_x("\n"
	                    "Enter the characteristic uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , char_uuid_str, 32))
	                    continue;

			if(NFBT_GATT_GetDescriptor(uuid_str, char_uuid_str) != NFBT_STATUS_SUCCESS ){
				print_cmd_error( cmd_ret );
				printf("Get descriptor failed\n");
			}
		}
		else if(strcasecmp(input, "gattnotifyreg" ) == 0 || strcasecmp( input, "c316" ) == 0){
			unsigned long long registy;

			if(-1 == enter_uuid_x("\n"
	                    "Enter the service uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , uuid_str, 32))
	                    continue;

			if(-1 == enter_uuid_x("\n"
	                    "Enter the characteristic uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , char_uuid_str, 32))
	                    continue;

		
			if(-1 == enter_index("\n"
                    "Enter a number unregister(0) or register(1) register specific UUID for listen notification, or -1 to cancel.\n" ,
                    input,
                    1, &registy))
                    continue;

			if(NFBT_GATT_NotificationRegister(uuid_str, char_uuid_str, registy) != NFBT_STATUS_SUCCESS ){
				print_cmd_error( cmd_ret );
				printf("Notifiy Register failed\n");
			}
		}
		else if(strcasecmp(input, "gattreadcharacteristic" ) == 0 || strcasecmp( input, "c317" ) == 0){
			if(-1 == enter_uuid_x("\n"
	                    "Enter the service uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , uuid_str, 32))
	                    continue;

			if(-1 == enter_uuid_x("\n"
	                    "Enter the characteristic uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , char_uuid_str, 32))
	                    continue;

			if(NFBT_GATT_ReadCharacteristic(uuid_str, char_uuid_str) != NFBT_STATUS_SUCCESS ){
				print_cmd_error( cmd_ret );
				printf("Read characteristic failed\n");
			}
		}
		else if(strcasecmp(input, "gattwritecharacteristic" ) == 0 || strcasecmp( input, "c318" ) == 0){
			if(-1 == enter_uuid_x("\n"
	                    "Enter the service uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , uuid_str, 32))
	                    continue;
		
			if(-1 == enter_uuid_x("\n"
	                    "Enter the characteristic uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , char_uuid_str, 32))
	                    continue;
		
			ret = enter_data(data_buf);

			if (ret == 1){
				data_len = strlen(data_buf);

				if ((data_len % 2)==1) {
					printf("Invalid input.\n");
					continue;
				}
			
				for (i = 0; i < data_len; i++)
				{
					if (data_buf[i] >= '0' && data_buf[i] <= '9') {
						data_buf[i] = data_buf[i] - '0';
					}
					else if(data_buf[i] >= 'A' && data_buf[i] <= 'F') {
						data_buf[i] = data_buf[i] - 'A' + 10;
					}else if(data_buf[i] >= 'a' && data_buf[i] <= 'f') {
						data_buf[i] = data_buf[i] - 'a' + 10;
					} 
					else {
						break;
					}

					if ((i % 2) != 0)
					{
						data_buf[i / 2] = ((data_buf[i - 1])<<4) | data_buf[i];
					}
				}
				
				if (i != data_len)
				{
					printf("Invalid input.\n");
					continue;
				}

				printf("HEX: ");
				for (i = 0; i < (data_len / 2); i++)
				{
					printf("%02X, ", data_buf[i]);
				}
				printf("\n");
				data_len /= 2;
			}
		
			if(NFBT_GATT_WriteCharacteristic(uuid_str, char_uuid_str, (char*)&data_buf, data_len) != NFBT_STATUS_SUCCESS )
			{
				print_cmd_error( cmd_ret );
				printf( "Write characteristic failed\n" );
			}
		 }
		 else if(strcasecmp( input, "gattgetincludeservice" ) == 0 || strcasecmp( input, "c320" ) == 0)
		 {
			if(-1 == enter_uuid_x("\n"
	                    "Enter the service uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , uuid_str, 32))
	                    continue;

			if(NFBT_GATT_GetIncludeService(uuid_str) != NFBT_STATUS_SUCCESS )
			{
				print_cmd_error( cmd_ret );
				printf( "Get include service failed\n" );
			}
		 }	 
		 else if(strcasecmp( input, "gattsetadvdata" ) == 0 || strcasecmp( input, "c323" ) == 0)
		 {
			char data_buf_2[32*8]={0};
			unsigned long long uuid_num = 0;
			unsigned long long speed = 1; /* default is 1, speed normal */

			if(-1 == enter_index("\n"
	                    "Enter a number between 1 to 8 to set how many UUID count in list, or -1 to cancel.\n" ,
	                    input,
	                    1, &uuid_num))
	                    continue;

			if(uuid_num > 8)
				uuid_num = 8;

			if(uuid_num)
			{
				for(i=0; i<(int)uuid_num; i++)
				{
					printf("uuid number %d\n", i);
					ret = enter_uuid_x("\n"
			                "Enter the uuid as format 0000xxxx00001000800000805f9b34fb for advertising,\n"
			                 "or -1 to cancel.\n"
			                , uuid_str, 32);
					if(ret == -1)
						break;
					memcpy((data_buf_2+i*16), uuid_str, 16);
				}
			}

			if(ret == -1)
				continue;

			if(-1 == enter_index("\n"
	                    "Enter a number between 0 to 2 to set advertising speed, or -1 to cancel.\n" ,
	                    input,
	                    1, &speed))
	                    continue;
		
			if(NFBT_GATT_SetAdvertisingData(1, 0, data_buf_2, uuid_num, speed) != NFBT_STATUS_SUCCESS )
			{
				print_cmd_error( cmd_ret );
				printf( "Advertising data set failed\n" );
			}
		 }
		 else if(strcasecmp(input, "gattreaddescriptor" ) == 0 || strcasecmp( input, "c324" ) == 0)
		 {
			char descr_uuid_str[UUID_LEN];
		
			if(-1 == enter_uuid_x("\n"
	                    "Enter the service uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , uuid_str, 32))
	                    continue;

			if(-1 == enter_uuid_x("\n"
	                    "Enter the characteristic uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , char_uuid_str, 32))
	                    continue;

			if(-1 == enter_uuid_x("\n"
	                    "Enter the descriptor uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , descr_uuid_str, 32))
	                    continue;

			if(NFBT_GATT_ReadDescriptor(uuid_str, char_uuid_str, descr_uuid_str) != NFBT_STATUS_SUCCESS )
			{
				print_cmd_error( cmd_ret );
				printf("Read descriptor failed\n");
			}
		 }
		 else if(strcasecmp(input, "gattwritedescriptor" ) == 0 || strcasecmp( input, "c325" ) == 0)
		 {
			char descr_uuid_str[UUID_LEN];

			if(-1 == enter_uuid_x("\n"
	                    "Enter the service uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , uuid_str, 32))
	                    continue;
		
			if(-1 == enter_uuid_x("\n"
	                    "Enter the characteristic uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , char_uuid_str, 32))
	                    continue;

			if(-1 == enter_uuid_x("\n"
	                    "Enter the descriptor uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , descr_uuid_str, 32))
	                    continue;
		
			ret = enter_data(data_buf);

			if (ret == 1){
				data_len = strlen(data_buf);

				if ((data_len % 2)==1) {
					printf("Invalid input.\n");
					continue;
				}
			
				for (i = 0; i < data_len; i++)
				{
					if (data_buf[i] >= '0' && data_buf[i] <= '9') {
						data_buf[i] = data_buf[i] - '0';
					}
					else if(data_buf[i] >= 'A' && data_buf[i] <= 'F') {
						data_buf[i] = data_buf[i] - 'A' + 10;
					}else if(data_buf[i] >= 'a' && data_buf[i] <= 'f') {
						data_buf[i] = data_buf[i] - 'a' + 10;
					} 
					else {
						break;
					}

					if ((i % 2) != 0)
					{
						data_buf[i / 2] = ((data_buf[i - 1])<<4) | data_buf[i];
					}
				}
				
				if (i != data_len)
				{
					printf("Invalid input.\n");
					continue;
				}

				printf("HEX: ");
				for (i = 0; i < (data_len / 2); i++)
				{
					printf("%02X, ", data_buf[i]);
				}
				printf("\n");
				data_len /= 2;
			}
		
			if(NFBT_GATT_WriteDescriptor(uuid_str, char_uuid_str, descr_uuid_str, (char*)&data_buf, data_len) != NFBT_STATUS_SUCCESS )
			{
				print_cmd_error( cmd_ret );
				printf( "Write descriptor failed\n" );
			}
		 }
		 else if(strcasecmp(input, "gattconnupdate" ) == 0 || strcasecmp( input, "c326" ) == 0)
		 {

	            ret = enter_address(bt_addr);
                int speed_level;
	            if (ret >= 0) {
	                str_to_char( bt_addr, addr.address );

	                unsigned long long speed_level;

	    			if(-1 == enter_index("\n"
	                        "Enter a speed level 0(Slow) ~ 3(Top) to set connected interval, or -1 to cancel.\n" ,
	                        input, 1, &speed_level))
	    				continue;

	    			if((cmd_ret = NFBT_GATT_ConnParameterUpdt(&addr, speed_level)) == NFBT_STATUS_SUCCESS )
	    				printf("set connected interval succeed\n");
	    			else{
	    				print_cmd_error( cmd_ret );
	    				printf("set connected interval failed\n");
	    			}
	            }
	                if (ret < 0)
	                    continue;

		 }
	 	 #ifdef GATT_SPP_DEMO
	 	 else if(strcasecmp(input, "demosppregister" ) == 0 || strcasecmp( input, "c328") == 0)
	 	 {
	 	 	demo_service_reg.mask = SRV_MASK;
	 		demo_service_reg.status = SRV_REG;
	 		gatt_service_register();
	 	 }
	 	 else if(strcasecmp(input, "demosppwrite" ) == 0 || strcasecmp( input, "c329" ) == 0)
	 	 {
	 		unsigned long long handle = 0;
	 		unsigned long long notification = 1;
	 		int j;

	 		for(i=0;i<ATTR_SIZE;i++)
	 		{
	 			if(gatt_spp[0].attribute[i].handle > 0)
	 			{
	 				if(gatt_spp[0].attribute[i].type == ATTRTYPE_CHARACTERISTIC &&
	 					(gatt_spp[0].attribute[i].uuid[2]&0xff) == 0x2a &&
	 					(gatt_spp[0].attribute[i].uuid[3]&0xff) == 0xff)
	 				{
	 					if(gatt_spp[0].attribute[i].property & GATT_PROP_NOTIFY)
	 						notification = 1;
	 					else if(gatt_spp[0].attribute[i].property & GATT_PROP_INDICATE)
	 						notification = 0;
	 					handle = gatt_spp[0].attribute[i].handle;
	 				}
	 			}
	 		}
		
	 		if(handle == 0){
	 			printf("No handle\n");
	 			continue;
	 		}

	 		ret = enter_data(data_buf);

	 		if (ret == 1){
	 			data_len = strlen(data_buf);

	 			if ((data_len % 2)==1) {
	 				printf("Invalid input.\n");
	 				continue;
	 			}
			
	 			for (i = 0; i < data_len; i++)
	 			{
	 				if (data_buf[i] >= '0' && data_buf[i] <= '9') {
	 					data_buf[i] = data_buf[i] - '0';
	 				}
	 				else if(data_buf[i] >= 'A' && data_buf[i] <= 'F') {
	 					data_buf[i] = data_buf[i] - 'A' + 10;
	 				}else if(data_buf[i] >= 'a' && data_buf[i] <= 'f') {
	 					data_buf[i] = data_buf[i] - 'a' + 10;
	 				} 
	 				else {
	 					break;
	 				}

	 				if ((i % 2) != 0)
	 				{
	 					data_buf[i / 2] = ((data_buf[i - 1])<<4) | data_buf[i];
	 				}
	 			}
				
	 			if (i != data_len)
	 			{
	 				printf("Invalid input.\n");
	 				continue;
	 			}

	 			printf("HEX: ");
	 			for (i = 0; i < (data_len / 2); i++)
	 			{
	 				printf("%02X, ", data_buf[i]);
	 			}
	 			printf("\n");
	 			data_len /= 2;
	 		}

	 		if(NFBT_GATT_SendIndication(handle, (char*)&data_buf, data_len, notification) != NFBT_STATUS_SUCCESS )
	 		{
	 			print_cmd_error( cmd_ret );
	 			printf( "Send indication fail\n" );
	 		}		
	 	 }
	 	 else if(strcasecmp(input, "demosppunregister" ) == 0 || strcasecmp( input, "c330" ) == 0)
	 	 {
	 	 	demo_service_reg.srv_handle = gatt_spp[0].srvc_hdl;
	 		demo_service_reg.status = SRV_STOP; /* gatt status must at disconnect */
	 		gatt_service_register();
	 	 }
	 	 #endif /* GATT_SPP_DEMO */
		 else if(strcasecmp(input, "gattserviceregister" ) == 0 || strcasecmp( input, "c331" ) == 0){
	 		unsigned long long handle;

	 		if(-1 == enter_index("\n"
	                    "Enter service id for enable BLE specific service, or -1 to cancel.\n" ,
	                    input,
	                    2, &handle))
	                    continue;

	 		if(NFBT_GATT_PeripheralServiceRegister(handle, 0xffff) != NFBT_STATUS_SUCCESS )
	 		{
				print_cmd_error( cmd_ret );
				printf( "Service register failed\n" );
	 		}
		 }
		 else if(strcasecmp(input, "gattserviceunregister" ) == 0 || strcasecmp( input, "c332" ) == 0){
	 		unsigned long long handle;
		
	 		if(-1 == enter_index("\n"
	                    "Enter service id for disable BLE specific service, or -1 to cancel.\n" ,
	                    input,
	                    2, &handle))
	                    continue;

	 		if(NFBT_GATT_PeripheralServiceUnRegister(handle) != NFBT_STATUS_SUCCESS )
	 		{
				print_cmd_error( cmd_ret );
				printf( "Service unRegister failed\n" );
	 		}
		 }
		 else if(strcasecmp(input, "gattaddservice" ) == 0 || strcasecmp( input, "c334" ) == 0)
		 {
			if(-1 == enter_uuid_x("\n"
	                    "Enter the service uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , uuid_str, 32))
	                    continue;

			if(NFBT_GATT_ServiceAdd(uuid_str) != NFBT_STATUS_SUCCESS )
			{
				print_cmd_error( cmd_ret );
				printf( "Service create failed\n" );
			}
		 }
		 else if(strcasecmp(input, "gattaddcharacteristic" ) == 0 || strcasecmp( input, "c336" ) == 0)
		 {
		 	unsigned long long handle;
			unsigned long long char_property;
			unsigned long long char_permit;

			if(-1 == enter_index("\n"
	                    "Enter service handle number, or -1 to cancel.\n" ,
	                    input,
	                    4, &handle))
	                    continue;

			if(-1 == enter_uuid_x("\n"
	                    "Enter the characteristic uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , char_uuid_str, 32))
	                    continue;

			if(-1 == enter_index("\n"
	                    "Enter characteristic properties, or -1 to cancel.\n" ,
	                    input,
	                    4, &char_property))
	                    continue;

			if(-1 == enter_index("\n"
	                    "Enter characteristic permit, or -1 to cancel.\n" ,
	                    input,
	                    4, &char_permit))
	                    continue;

			#ifdef GATT_SPP_DEMO
			tmp_property = char_property;
			tmp_permit = char_permit;
			#endif /* GATT_SPP_DEMO */

			if(NFBT_GATT_CharacteristicAdd(handle, char_uuid_str, char_property, char_permit) != NFBT_STATUS_SUCCESS )
			{
				print_cmd_error( cmd_ret );
				printf( "Characteristic add failed\n" );
			}
		 }
		 else if(strcasecmp(input, "gattadddescriptor" ) == 0 || strcasecmp( input, "c337" ) == 0)
		 {
		 	unsigned long long handle;
			unsigned long long char_permit;

			if(-1 == enter_index("\n"
	                    "Enter a number a service handle, or -1 to cancel.\n" ,
	                    input,
	                    4, &handle))
	                    continue;

			if(-1 == enter_uuid_x("\n"
	                    "Enter the descriptor uuid as format 0000110100001000800000805f9b34fb or -1 to cancel.\n"
	                    , uuid_str, 32))
	                    continue;

			if(-1 == enter_index("\n"
	                    "Enter characteristic permit, or -1 to cancel.\n" ,
	                    input,
	                    4, &char_permit))
	                    continue;

			if(NFBT_GATT_DescriptorAdd(handle, uuid_str, char_permit) != NFBT_STATUS_SUCCESS )
			{
				print_cmd_error( cmd_ret );
				printf( "Descriptor add failed\n" );
			}
		 }	 
		 else if(strcasecmp(input, "gattstartservice" ) == 0 || strcasecmp( input, "c338" ) == 0)
		 {
			unsigned long long start;
			unsigned long long handle;
		
			if(-1 == enter_index("\n"
	                    "Enter a number to stop(0) or start(1) service, or -1 to cancel.\n" ,
	                    input,
	                    1, &start))
	                    continue;

			if(-1 == enter_index("\n"
	                    "Enter a number a service handle, or -1 to cancel.\n" ,
	                    input,
	                    4, &handle))
	                    continue;
		
			if(NFBT_GATT_StartService(start, handle) != NFBT_STATUS_SUCCESS )
			{
				print_cmd_error( cmd_ret );
				printf( "Start service fail\n" );
			}
		}
		else if(strcasecmp(input, "gattdelservice" ) == 0 || strcasecmp( input, "c339" ) == 0){
			unsigned long long handle;
	 	
			if(-1 == enter_index("\n"
	                    "Enter a number a service handle, or -1 to cancel.\n" ,
	                    input,
	                    4, &handle))
	                    continue;
		
			if(NFBT_GATT_DeleteService(handle) != NFBT_STATUS_SUCCESS )
			{
				print_cmd_error( cmd_ret );
				printf( "Delete service fail\n" );
			}		
		}
		else if(strcasecmp(input, "gattsendindicator" ) == 0 || strcasecmp( input, "c340" ) == 0){
			unsigned long long handle;
			unsigned long long notification;
	 	
			if(-1 == enter_index("\n"
	                    "Enter a number a service handle, or -1 to cancel.\n" ,
	                    input,
	                    4, &handle))
	                    continue;

			if(-1 == enter_index("\n"
	                    "If need confirmed by device Yes(1) or No(0) , or -1 to cancel.\n" ,
	                    input,
	                    1, &notification))
	                    continue;

			ret = enter_data(data_buf);

			if (ret == 1){
				data_len = strlen(data_buf);

				if ((data_len % 2)==1) {
					printf("Invalid input.\n");
					continue;
				}
			
				for (i = 0; i < data_len; i++)
				{
					if (data_buf[i] >= '0' && data_buf[i] <= '9') {
						data_buf[i] = data_buf[i] - '0';
					}
					else if(data_buf[i] >= 'A' && data_buf[i] <= 'F') {
						data_buf[i] = data_buf[i] - 'A' + 10;
					}else if(data_buf[i] >= 'a' && data_buf[i] <= 'f') {
						data_buf[i] = data_buf[i] - 'a' + 10;
					} 
					else {
						break;
					}

					if ((i % 2) != 0)
					{
						data_buf[i / 2] = ((data_buf[i - 1])<<4) | data_buf[i];
					}
				}
				
				if (i != data_len){
					printf("Invalid input.\n");
					continue;
				}

				printf("HEX: ");
				for (i = 0; i < (data_len / 2); i++)
				{
					printf("%02X, ", data_buf[i]);
				}
				printf("\n");
				data_len /= 2;
			}

			if(NFBT_GATT_SendIndication(handle, (char*)&data_buf, data_len, notification) != NFBT_STATUS_SUCCESS )
			{
				print_cmd_error( cmd_ret );
				printf( "Send indication fail\n" );
			}		
		}
		else if(strcasecmp(input, "gattsetrole" ) == 0 || strcasecmp( input, "c345" ) == 0){
			unsigned long long role;
		
			if(-1 == enter_index("\n"
	                    "Set GATT role to Peripheral(0), Central(1), or -1 to cancel.\n" ,
	                    input,
	                    1, &role))
	                    continue;
		
			if(NFBT_GATT_SetRole(role) != NFBT_STATUS_SUCCESS )
			{
				print_cmd_error( cmd_ret );
				printf( "Set GATT role fail\n" );
			}		
		}
		else if(strcasecmp(input, "gattservicelist" ) == 0 || strcasecmp( input, "c346" ) == 0){		
			if(NFBT_GATT_PeripheralListService() != NFBT_STATUS_SUCCESS )
			{
				print_cmd_error( cmd_ret );
				printf( "List service fail\n" );
			}		
		}
		else if(strcasecmp(input, "gattcharacterlist" ) == 0 || strcasecmp( input, "c347" ) == 0){		
			unsigned long long srv_hdl;
		
			if(-1 == enter_index("\n"
	                    "enter a service handle, or -1 to cancel.\n" ,
	                    input,
	                    3, &srv_hdl))
	                    continue;
		
			if(NFBT_GATT_PeripheralListAttribute(srv_hdl) != NFBT_STATUS_SUCCESS )
			{
				print_cmd_error( cmd_ret );
				printf( "List characteristic fail\n" );
			}
		}
 	 	else if(strcasecmp(input, "gattsnotify" ) == 0 || 
	 		strcasecmp( input, "c351" ) == 0 || 
	 		strcasecmp( input, "c350" ) == 0 || 
	 		strcasecmp( input, "c349" ) == 0 || 
	 		strcasecmp( input, "c348" ) == 0 ){

			ret = enter_data(data_buf);

			if (ret == 1){
				data_len = strlen(data_buf);

				if ((data_len % 2)==1) {
					printf("Invalid input.\n");
					continue;
				}
			
				for (i = 0; i < data_len; i++)
				{
					if (data_buf[i] >= '0' && data_buf[i] <= '9') {
						data_buf[i] = data_buf[i] - '0';
					}
					else if(data_buf[i] >= 'A' && data_buf[i] <= 'F') {
						data_buf[i] = data_buf[i] - 'A' + 10;
					}else if(data_buf[i] >= 'a' && data_buf[i] <= 'f') {
						data_buf[i] = data_buf[i] - 'a' + 10;
					} 
					else {
						break;
					}

					if ((i % 2) != 0)
					{
						data_buf[i / 2] = ((data_buf[i - 1])<<4) | data_buf[i];
					}
				}
				
				if (i != data_len)
				{
					printf("Invalid input.\n");
					continue;
				}

				printf("HEX: ");
				for (i = 0; i < (data_len / 2); i++)
				{
					printf("%02X, ", data_buf[i]);
				}
				printf("\n");
				data_len /= 2;
			}
		
			if(strcasecmp( input, "c348" ) == 0 && NFBT_GATT_BatteryLevelNotify(data_buf, data_len) != NFBT_STATUS_SUCCESS ){
				print_cmd_error( cmd_ret );
				printf( "Battery level notify fail\n" );
			}
			if(strcasecmp( input, "c349" ) == 0 && NFBT_GATT_BloodPressureMeasurementIndicate(data_buf, data_len) != NFBT_STATUS_SUCCESS ){
				print_cmd_error( cmd_ret );
				printf( "Blood pressure measurement indicate fail\n" );
			}
			if(strcasecmp( input, "c350" ) == 0 && NFBT_GATT_InterMediateCuffPressureNotify(data_buf, data_len) != NFBT_STATUS_SUCCESS ){
				print_cmd_error( cmd_ret );
				printf( "Inter Mediate cuff pressure notify fail\n" );
			}
			if(strcasecmp( input, "c351" ) == 0 && NFBT_GATT_SppSendData(data_buf, data_len) != NFBT_STATUS_SUCCESS ){
				print_cmd_error( cmd_ret );
				printf( "Spp send data fail\n" );
			}

        }else if(strcasecmp(input, "gattaccesswhilelist" ) == 0 || strcasecmp( input, "c352" ) == 0){

			int ret;
            long long unsigned int add;

			if(-1 == enter_index("\n"
	                    "Access While list add(1), remove(0), or -1 to cancel.\n" ,
	                    input,
	                    1, &add))
	                    continue;

			ret = enter_address(bt_addr);

			if (ret < 0)
			    continue;

			str_to_char( bt_addr, addr.address );

			if(NFBT_GATT_AccessWhileList(add, &addr) != NFBT_STATUS_SUCCESS )
			{
				printf( "Access to While List fail\n" );
			}
        }
		else{
			printf( "Incorrect command.\n"
				"Enter help for more information about current support features.\n" );
		}
    }while(1);

    //unload
    //system(CLEAR);
    cmd_ret = NFBT_Deinit();

	sleep(1);

    return 0;
}
