/**
 * @file ql_nf3303_ble_server.c
 * @brief Quectel Openlinux BT public function declarations.
 * 
 * @note this example show how to use NF3303 BT Module as BLE Server 
 *
 * @copyright Copyright (c) 2009-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

#include <ql_oe.h>
#include "ql_nf3303_bt_stack.h"
#include "ql_nf3303_bt_callback.h"
#include "ql_nf3303_bt_debug.h"
#include "ql_nf3303_ble_gattdefs.h"
#include "ql_nf3303_ble_common.h"


/*
 * define the debug level of this file,
 * please see 'debug.h' for detail info
 */
DEBUG_SET_LEVEL(DEBUG_LEVEL_ERR);

#define BLE_INCLUDED TRUE

/*Common Definition*/
#define BT_MODULE_ROLE       GATT_ROLE_PERIPHERAL
#define BT_START_LISTEN      1
#define LOCAL_NAME           "Quectel_NF3303"

extern ql_gatt_services ql_gatt_demo;
volatile int bt_enable_flag = 0;
extern volatile int bt_notify_timer_flag;
extern volatile int bt_indicate_timer_flag;

/*Services Definition*/
#define BLE_TOTAL_SERVICE_NUM     0x3

/*Service 0 Definition*/
#define QUECTEL_SERVICE0_UUID     UUID_SERVCLASS_DEVICE_INFO
#define BLE_SERVICE0_ATTR_NUM     0x4

/*Service 1 Definition*/
#define QUECTEL_SERVICE1_UUID     UUID_SERVCLASS_DEVICE_INFO
#define BLE_SERVICE1_ATTR_NUM     0x4

/*Service 2 Definition*/
#define QUECTEL_SERVICE2_UUID     UUID_SERVCLASS_CURRENT_TIME
#define BLE_SERVICE2_ATTR_NUM     0x4
/*Characteristic 1 Definition*/
#define BLE_SRV2_ATTR1_DATA_BYTES 0x2

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

/*When the exception occurs, please add the corresponding handler here*/
void ql_ble_server_exit()
{
    /*Delete Notification Timer*/
    if(1 == bt_notify_timer_flag) {
        if(ble_notify_timer_stop() < 0) {
            ERR("Stop Timer Failed\n");
        }
    }

    if(1 == bt_indicate_timer_flag) {
        if(ble_indicate_timer_stop() < 0) {
            ERR("Stop Timer Failed\n");
        }
    }

    Ql_Autosleep_Enable(0);
    ql_bt_sleepwake_pin_uninit();
}

void ql_ble_sig_handler(int signo)
{
    bt_status cmd_ret;
    char cmd[64] = {};
    int timeout = 1; //1 s
    int retry_times = 10; //retry 3 times

    ERR("\n\nNF3303 BT Demo crashed by signal %d\n", signo);

    if(NULL == memset(&cmd, 0, 64))
        ERR("Memset Failed\n");

    if (signo == SIGSEGV || signo == SIGBUS ||
            signo == SIGTRAP || signo == SIGABRT) {
        sprintf(cmd, "cat /proc/%d/maps", getpid());
        ERR("Process maps:\n");
        system(cmd);
    }

    cmd_ret = NFBT_Disable();

    /*wait for Fcnction NFBT_Enable callback return NFBT_BT_DISABLE*/
    retry_times = 10;
    timeout = 1; //1 s
    while(retry_times--) {
        if(cmd_ret == NFBT_STATUS_SUCCESS) {
            INFO("Disable BT stack success\n");
            break;
        }
        else
            sleep(timeout);
        if(0 == retry_times) {
            ERR( "Disable BT stack fail\n" );
        }
    }

    NFBT_Deinit();

    ql_ble_server_exit();

    INFO("Stop NF3303 BT Module\n");

    exit(-1);
}

int main(int argc, char** argv)
{
    BT_VERSION ver;
    const char *local_name = LOCAL_NAME;
    bt_status cmd_ret;
    bt_enable_flag = 0; /*BT Enable result flag*/
    bt_notify_timer_flag = 0;
    bt_indicate_timer_flag = 0;
    int timeout = 1; //1 s
    int retry_times = 3; //retry 3 times
    int index = 0;
    int should_register_attr_num = 0;
    int real_register_attr_num = 0;
    char gatt_base_uuid[32] = {0x00,0x00,0x2A,0x27,0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x80,0x5f,0x9b,0x34,0xfb,
                               0x00,0x00,0x2A,0x28,0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x80,0x5f,0x9b,0x34,0xfb};
    int tmp_len = 0;
    time_t timep;
    struct tm *tm_p;

#if 0
    /*run in the background*/
    if (daemon(0, 1)) {
        ERR("\nNF3303 BT Start Daemon Failed\n");
        exit(-1);
    }
    else
        INFO("\nNF3303 BT Start Daemon Success\n");
#endif

    INFO("BLE Server Demo Build Time:%s %s\n", __DATE__, __TIME__);

    ql_bt_sleepwake_pin_init();
    ql_bt_wake_high();
    ql_bt_module_enable();

    if(NULL == memset(&ql_gatt_demo, 0, (sizeof(ql_gatt_demo)))) {
        ERR("Memset Failed\n");
        exit(-1);
    }

    ql_gatt_demo.total_service_num = BLE_TOTAL_SERVICE_NUM;
    if(ql_gatt_demo.total_service_num > BT_MAX_SERVICE_NUM) {
        ERR("Overflow max service num\n");
        ql_gatt_demo.total_service_num = BT_MAX_SERVICE_NUM;
    }

    /*Service 0 Definition*/
    ql_gatt_demo.ql_gatt_service_s[0].service_uuid = QUECTEL_SERVICE0_UUID; 
    ql_gatt_demo.ql_gatt_service_s[0].total_attr_num= BLE_SERVICE0_ATTR_NUM;
    ql_gatt_demo.ql_gatt_service_s[0].is_srv_valid = 0x1;
    ql_gatt_demo.ql_gatt_service_s[0].status = SRV_REG;

    /*Service 0 characteristic 0 Definition*/
    ql_gatt_demo.ql_gatt_service_s[0].attributes[0].type = ATTRTYPE_CHARACTERISTIC;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[0].uuid = GATT_UUID_MANU_NAME;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[0].permission = GATT_PERMIT_READ;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[0].property = GATT_PROP_READ;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[0].is_attr_valid = 0x1;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[0].data_bytes = 100;
    tmp_len = ql_gatt_demo.ql_gatt_service_s[0].attributes[0].data_bytes;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[0].data = (char *)calloc(tmp_len, sizeof(char));
    if(NULL == ql_gatt_demo.ql_gatt_service_s[0].attributes[0].data)
        ERR("Calloc Failed:\n");
    tmp_len = sprintf(ql_gatt_demo.ql_gatt_service_s[0].attributes[0].data, "%s", "Quectel NF3303 BT Module"); 
    if (tmp_len < 0) {
        ERR("Sprintf Failed:\n");
    }

    /*Service 0 characteristic 1 Definition*/
    ql_gatt_demo.ql_gatt_service_s[0].attributes[1].type = ATTRTYPE_DESCRIPTOR;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[1].uuid = GATT_UUID_CHAR_CLIENT_CONFIG;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[1].permission = GATT_PERMIT_READ|GATT_PERMIT_WRITE;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[1].property = GATT_PROP_READ|GATT_PROP_WRITE;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[1].is_attr_valid = 0x1;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[1].data_bytes = 2;
    tmp_len = ql_gatt_demo.ql_gatt_service_s[0].attributes[1].data_bytes;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[1].data = (char *)calloc(tmp_len, sizeof(char));
    if(NULL == ql_gatt_demo.ql_gatt_service_s[0].attributes[1].data)
        ERR("Calloc Failed:\n");
    ql_gatt_demo.ql_gatt_service_s[0].attributes[1].data[0] = 0x00;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[1].data[1] = 0x00;

    ql_gatt_demo.ql_gatt_service_s[0].attributes[2].type = ATTRTYPE_CHARACTERISTIC;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[2].uuid = GATT_UUID_SW_VERSION_STR;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[2].permission = GATT_PERMIT_READ;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[2].property = GATT_PROP_READ;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[2].is_attr_valid = 0x1;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[2].data_bytes = 100;
    tmp_len = ql_gatt_demo.ql_gatt_service_s[0].attributes[2].data_bytes;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[2].data = (char *)calloc(tmp_len, sizeof(char));
    if(NULL == ql_gatt_demo.ql_gatt_service_s[0].attributes[2].data)
        ERR("Calloc Failed:\n");
    tmp_len = sprintf(ql_gatt_demo.ql_gatt_service_s[0].attributes[2].data, "%s", "SoftWare Version v1.0"); 
    if (tmp_len < 0) {
        ERR("Sprintf Failed:\n");
    }

    ql_gatt_demo.ql_gatt_service_s[0].attributes[3].type = ATTRTYPE_DESCRIPTOR;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[3].uuid = GATT_UUID_CHAR_CLIENT_CONFIG;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[3].permission = GATT_PERMIT_READ|GATT_PERMIT_WRITE;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[3].property = GATT_PROP_READ|GATT_PROP_WRITE;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[3].is_attr_valid = 0x1;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[3].data_bytes = 2;
    tmp_len = ql_gatt_demo.ql_gatt_service_s[0].attributes[3].data_bytes;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[3].data = (char *)calloc(tmp_len, sizeof(char));
    if(NULL == ql_gatt_demo.ql_gatt_service_s[0].attributes[3].data)
        ERR("Calloc Failed:\n");
    ql_gatt_demo.ql_gatt_service_s[0].attributes[3].data[0] = 0x00;
    ql_gatt_demo.ql_gatt_service_s[0].attributes[3].data[1] = 0x00;

    ql_gatt_demo.ql_gatt_service_s[1].service_uuid = 0x180B;
    ql_gatt_demo.ql_gatt_service_s[1].total_attr_num= BLE_SERVICE1_ATTR_NUM;
    ql_gatt_demo.ql_gatt_service_s[1].is_srv_valid = 0x1;
    ql_gatt_demo.ql_gatt_service_s[1].status = SRV_REG;

    ql_gatt_demo.ql_gatt_service_s[1].attributes[0].type = ATTRTYPE_CHARACTERISTIC;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[0].uuid = GATT_UUID_FW_VERSION_STR;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[0].permission = GATT_PERMIT_READ|GATT_PERMIT_WRITE;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[0].property = GATT_PROP_READ|GATT_PROP_WRITE;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[0].is_attr_valid = 0x1;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[0].data_bytes = 100;
    tmp_len = ql_gatt_demo.ql_gatt_service_s[1].attributes[0].data_bytes;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[0].data = (char *)calloc(tmp_len, sizeof(char));
    if(NULL == ql_gatt_demo.ql_gatt_service_s[1].attributes[0].data)
        ERR("Calloc Failed:\n");
    tmp_len = sprintf(ql_gatt_demo.ql_gatt_service_s[1].attributes[0].data, "%s", "BlueGate v0.3.1.03"); 
    if (tmp_len < 0) {
        ERR("Sprintf Failed:\n");
    }

    ql_gatt_demo.ql_gatt_service_s[1].attributes[1].type = ATTRTYPE_DESCRIPTOR;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[1].uuid = GATT_UUID_CHAR_CLIENT_CONFIG;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[1].permission = GATT_PERMIT_READ|GATT_PERMIT_WRITE;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[1].property = GATT_PROP_READ|GATT_PROP_WRITE;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[1].is_attr_valid = 0x1;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[1].data_bytes = 2;
    tmp_len = ql_gatt_demo.ql_gatt_service_s[1].attributes[1].data_bytes;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[1].data = (char *)calloc(tmp_len, sizeof(char));
    if(NULL == ql_gatt_demo.ql_gatt_service_s[1].attributes[1].data)
        ERR("Calloc Failed:\n");
    ql_gatt_demo.ql_gatt_service_s[1].attributes[1].data[0] = 0x00;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[1].data[1] = 0x00;

    ql_gatt_demo.ql_gatt_service_s[1].attributes[2].type = ATTRTYPE_CHARACTERISTIC;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[2].uuid = GATT_UUID_MODEL_NUMBER_STR;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[2].permission = GATT_PERMIT_READ|GATT_PERMIT_WRITE;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[2].property = GATT_PROP_READ|GATT_PROP_WRITE;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[2].is_attr_valid = 0x1;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[2].data_bytes = 100;
    tmp_len = ql_gatt_demo.ql_gatt_service_s[1].attributes[2].data_bytes;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[2].data = (char *)calloc(tmp_len, sizeof(char));
    if(NULL == ql_gatt_demo.ql_gatt_service_s[1].attributes[2].data)
        ERR("Calloc Failed:\n");
    tmp_len = sprintf(ql_gatt_demo.ql_gatt_service_s[1].attributes[2].data, "%s", "EC20CEFA");
    if (tmp_len < 0) {
        ERR("Sprintf Failed:\n");
    }

    ql_gatt_demo.ql_gatt_service_s[1].attributes[3].type = ATTRTYPE_DESCRIPTOR;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[3].uuid = GATT_UUID_CHAR_CLIENT_CONFIG;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[3].permission = GATT_PERMIT_READ|GATT_PERMIT_WRITE;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[3].property = GATT_PROP_READ|GATT_PROP_WRITE;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[3].is_attr_valid = 0x1;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[3].data_bytes = 2;
    tmp_len = ql_gatt_demo.ql_gatt_service_s[1].attributes[3].data_bytes;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[3].data = (char *)calloc(tmp_len, sizeof(char));
    if(NULL == ql_gatt_demo.ql_gatt_service_s[1].attributes[3].data)
        ERR("Calloc Failed:\n");
    ql_gatt_demo.ql_gatt_service_s[1].attributes[3].data[0] = 0x00;
    ql_gatt_demo.ql_gatt_service_s[1].attributes[3].data[1] = 0x00;

    ql_gatt_demo.ql_gatt_service_s[2].service_uuid = UUID_SERVCLASS_CURRENT_TIME; 
    ql_gatt_demo.ql_gatt_service_s[2].total_attr_num= BLE_SERVICE2_ATTR_NUM;
    ql_gatt_demo.ql_gatt_service_s[2].is_srv_valid = 0x1;
    ql_gatt_demo.ql_gatt_service_s[2].status = SRV_REG;

    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].type = ATTRTYPE_CHARACTERISTIC;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].uuid = GATT_UUID_CURRENT_TIME;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].permission = GATT_PERMIT_READ|GATT_PERMIT_WRITE;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].property = GATT_PROP_READ|GATT_PROP_WRITE|GATT_PROP_NOTIFY;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].is_attr_valid = 0x1;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].data_bytes = 10;
    tmp_len = ql_gatt_demo.ql_gatt_service_s[2].attributes[0].data_bytes;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].data = (char *)calloc(tmp_len, sizeof(char));
    if(NULL == ql_gatt_demo.ql_gatt_service_s[2].attributes[0].data)
        ERR("Calloc Failed:\n");

    time(&timep);
    tm_p = localtime(&timep);
    tm_p->tm_year += 1900;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].data[0] = tm_p->tm_year&0xFF;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].data[1] = ((tm_p->tm_year)>>8)&0xFF;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].data[2] = tm_p->tm_mon+1;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].data[3] = tm_p->tm_mday;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].data[4] = tm_p->tm_hour;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].data[5] = tm_p->tm_min;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].data[6] = tm_p->tm_sec;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].data[7] = tm_p->tm_wday;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].data[8] = 0x00;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[0].data[9] = 0x20;

    ql_gatt_demo.ql_gatt_service_s[2].attributes[1].type = ATTRTYPE_DESCRIPTOR;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[1].uuid = GATT_UUID_CHAR_CLIENT_CONFIG;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[1].permission = GATT_PERMIT_READ|GATT_PERMIT_WRITE;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[1].property = GATT_PROP_READ|GATT_PROP_WRITE;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[1].is_attr_valid = 0x1;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[1].data_bytes = BLE_SRV2_ATTR1_DATA_BYTES;
    tmp_len = ql_gatt_demo.ql_gatt_service_s[2].attributes[1].data_bytes;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[1].data = (char *)calloc(tmp_len, sizeof(char));
    if(NULL == ql_gatt_demo.ql_gatt_service_s[2].attributes[1].data)
        ERR("Calloc Failed:\n");
    ql_gatt_demo.ql_gatt_service_s[2].attributes[1].data[0] = 0x00;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[1].data[1] = 0x00;

    ql_gatt_demo.ql_gatt_service_s[2].attributes[2].type = ATTRTYPE_CHARACTERISTIC;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[2].uuid = 0x2A2C;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[2].permission = GATT_PERMIT_READ|GATT_PERMIT_WRITE;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[2].property = GATT_PROP_READ|GATT_PROP_WRITE|GATT_PROP_INDICATE;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[2].is_attr_valid = 0x1;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[2].data_bytes = 3;
    tmp_len = ql_gatt_demo.ql_gatt_service_s[2].attributes[2].data_bytes;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[2].data = (char *)calloc(tmp_len, sizeof(char));
    if(NULL == ql_gatt_demo.ql_gatt_service_s[2].attributes[2].data)
        ERR("Calloc Failed:\n");

    time(&timep);
    tm_p = localtime(&timep);
    ql_gatt_demo.ql_gatt_service_s[2].attributes[2].data[0] = tm_p->tm_hour;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[2].data[1] = tm_p->tm_min;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[2].data[2] = tm_p->tm_sec;

    ql_gatt_demo.ql_gatt_service_s[2].attributes[3].type = ATTRTYPE_DESCRIPTOR;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[3].uuid = GATT_UUID_CHAR_CLIENT_CONFIG;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[3].permission = GATT_PERMIT_READ|GATT_PERMIT_WRITE;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[3].property = GATT_PROP_READ|GATT_PROP_WRITE;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[3].is_attr_valid = 0x1;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[3].data_bytes = BLE_SRV2_ATTR1_DATA_BYTES;
    tmp_len = ql_gatt_demo.ql_gatt_service_s[2].attributes[3].data_bytes;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[3].data = (char *)calloc(tmp_len, sizeof(char));
    if(NULL == ql_gatt_demo.ql_gatt_service_s[2].attributes[3].data)
        ERR("Calloc Failed:\n");
    ql_gatt_demo.ql_gatt_service_s[2].attributes[3].data[0] = 0x00;
    ql_gatt_demo.ql_gatt_service_s[2].attributes[3].data[1] = 0x00;


    for(index=0; index<ql_gatt_demo.total_service_num; index++) {
        if(ql_gatt_demo.ql_gatt_service_s[index].total_attr_num > BT_MAX_ATTR_NUM) {
            ERR("Failed: overflow max attribute num\n");
            ql_gatt_demo.ql_gatt_service_s[index].total_attr_num = BT_MAX_ATTR_NUM;
        }
    }

    cmd_ret = NFBT_Init(&nfmw_callbacks);
    if(cmd_ret == NFBT_STATUS_SUCCESS)
        INFO("Register CBK success\n");
    else{
        print_cmd_error(cmd_ret);
        ERR("Register CBK failed\n");
        exit(-1);
    }

    ver = NFBT_GetBtVersion();
    INFO("\n\n"
            "*************************************************************\n"
            "****** Bluetooth Stack Verison%s *****\n"
            "*************************************************************\n\n", ver.version );

    cmd_ret = NFBT_Enable();
    if(cmd_ret == NFBT_STATUS_SUCCESS) {
        INFO("Enable BT stack success\n");
    }

    /*wait for Fcnction NFBT_Enable callback return NFBT_BT_ENABLE*/
    retry_times = 10;
    timeout = 1; //1 s
    while(retry_times--) {
        if(1 == bt_enable_flag) {
            bt_enable_flag = 0;
            INFO("Enable BT success\n");
            break;
        }
        else
            sleep(timeout);
        if(0 == retry_times) {
            ERR("Enable BT Failed\n");
	    /*TODO add BT_EN reset function*/
            exit(-1);
        }
    }

    /*Set Local Device Name, Will be shown in the advertising data*/
    cmd_ret = NFBT_SetLocalDeviceName(strlen(local_name), (unsigned char *)local_name);
    if(cmd_ret == NFBT_STATUS_SUCCESS) {
        INFO("Set Local Device name Success\n");
    }
    else {
        print_cmd_error(cmd_ret);
        ERR("Set Local Device name Failed\n");
    }

    cmd_ret = NFBT_SetScanMode(NFBT_SCAN_MODE_NONE_HOLDING);
    if(cmd_ret == NFBT_STATUS_SUCCESS)
        INFO("Set Scan Mode Success\n");
    else {
        print_cmd_error(cmd_ret);
        ERR("Set Scan Mode Failed\n");
        exit(-1);
    }

    /*wait until Gatt Profile Register Success*/
    retry_times = 18;
    timeout = 10;
    while(retry_times--) {
        if((ql_gatt_demo.gatt_server_register_status == 1)&&(ql_gatt_demo.gatt_client_register_status == 1)) {
            INFO("BLE Gatt Client & Server Register Success\n");
            break;
        }
        else
            sleep(timeout);
        if(0 == retry_times) {
            INFO("BLE Gatt Client & Server Register Failed\n");
            exit(-1);
        }
    }

    retry_times = 10;
    timeout = 1; //1 s
    while(retry_times--) {
        if(NFBT_GATT_SetRole(BT_MODULE_ROLE) == NFBT_STATUS_SUCCESS) {
            INFO("Set Role Success\n");
            break;
        }
        else
            sleep(timeout);
        if(0 == retry_times) {
            ERR("Set GATT Role Failed\n");
            exit(-1);
        }
    }

    /*Register all services and characteristic*/
    for(index=0; index<ql_gatt_demo.total_service_num; index++) {
        ql_gatt_service_register(&ql_gatt_demo.ql_gatt_service_s[index]);
        retry_times = 30;
        timeout = 1; //1 s
        while(retry_times--) {
            should_register_attr_num = ql_gatt_demo.ql_gatt_service_s[index].total_attr_num;
            real_register_attr_num = ql_gatt_demo.ql_gatt_service_s[index].registered_attr_num;
            if((ql_gatt_demo.ql_gatt_service_s[index].is_srv_registered != 1)  || (should_register_attr_num != real_register_attr_num)) {
                sleep(timeout);
            }
            else
                break;
        }
    }

    /*Set Advertising Data*/
    retry_times = 10;
    timeout = 1; //1 s
    while(retry_times--) {
        if(NFBT_GATT_SetAdvertisingData(BLE_ADV_SHOW_NAME, 0, gatt_base_uuid, 2, GATT_ADV_SPEED_SLOW) == NFBT_STATUS_SUCCESS) {
            INFO("Set Advertising Data Success\n" );
            break;
        }
        else
            sleep(timeout);
        if(0 == retry_times) {
            ERR("Set Advertising Data Failed\n" );
            exit(-1);
        }
    }

    /*wait for Service&Characteristic&Descriptor Register Success*/
    retry_times = 10;
    timeout = 1; //1 s
    while(retry_times--) {
        should_register_attr_num = 0;
        real_register_attr_num = 0;
        for(index=0; index<BLE_TOTAL_SERVICE_NUM; index++) {
            should_register_attr_num += ql_gatt_demo.ql_gatt_service_s[index].total_attr_num;
            real_register_attr_num += ql_gatt_demo.ql_gatt_service_s[index].registered_attr_num;
        }
        if((ql_gatt_demo.registered_service_num != BLE_TOTAL_SERVICE_NUM) || (should_register_attr_num != real_register_attr_num)) {
            sleep(timeout);
        }
        else {
            if(NFBT_GATT_Listen(BT_START_LISTEN, 1) == NFBT_STATUS_SUCCESS) {
                INFO("Listen operation Success\n");
                break;
            }
            if(0 == retry_times) {
                ERR( "List characteristic Failed\n" );
                exit(-1);
            }
        }
    }

    /*signal hander*/
    signal(SIGINT, ql_ble_sig_handler);
    signal(SIGTERM, ql_ble_sig_handler);
    signal(SIGBUS, ql_ble_sig_handler);
    signal(SIGSEGV, ql_ble_sig_handler);
    signal(SIGABRT, ql_ble_sig_handler);

    ql_ble_sleep_init();
    sleep(3);
    Ql_Autosleep_Enable(1);

    printf("Bluetooth has been successfully started!\n");
    
    while(1) {
        sleep(60);
    }

    return 0;
}
