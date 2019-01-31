/**
 * @file ql_nf3303_bt_callback.c
 * @brief Quectel Openlinux BT public function declarations.
 *
 * @note this example show NF3303 BT Module Callback functions
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

#include "ql_oe.h"
#include "ql_gpio.h"
#include "ql_nf3303_bt_stack.h"
#include "ql_nf3303_bt_callback.h"
#include "ql_nf3303_bt_debug.h"
#include "ql_nf3303_ble_gattdefs.h"
#include "ql_nf3303_ble_sleep.h"

BT_ADDR connected_dev_addr;

/*
 * define the debug level of this file,
 * please see 'debug.h' for detail info
 */
DEBUG_SET_LEVEL(DEBUG_LEVEL_ERR);

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

extern volatile int bt_enable_flag;

extern int serial_port_fd;

volatile int ble_search_service_complete = 0;

timer_t timerid_notify = 0;
struct itimerspec ts_notify;
volatile int bt_notify_timer_flag = 0;

timer_t timerid_indicate = 0;
struct itimerspec ts_indicate;
volatile int bt_indicate_timer_flag = 0;

ql_gatt_services ql_gatt_demo;
ql_scan_server_info ql_scan_server_list[MAX_SCAN_SERVER_INFO_NUM];

void ql_gatt_service_register(ql_gatt_service *ql_gatt_srv)
{
    int count = 0;
    char gatt_base_uuid[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x80,0x5f,0x9b,0x34,0xfb};

    switch(ql_gatt_srv->status)
    {
        case SRV_REG_FAIL:
            DEBUG("Will GATT Delete GATT Service %d\n", ql_gatt_srv->srv_handle);
            if(ql_gatt_srv->srv_handle > 0)
                NFBT_GATT_DeleteService(ql_gatt_srv->srv_handle);
            /* break; */  /* do not use */
        case SRV_REG_NONE:
            DEBUG("Will Clear srv_handle\n");
            ql_gatt_srv->srv_handle = 0;
            break;
        case SRV_REG:
            gatt_base_uuid[3] = ql_gatt_srv->service_uuid & 0xff;
            gatt_base_uuid[2] = (ql_gatt_srv->service_uuid>>8) & 0xff;
            DEBUG("Will add GATT Service\n");
            NFBT_GATT_ServiceAdd(gatt_base_uuid);
            break;
        case SRV_UNREG:
            DEBUG("Will Delete GATT Service %d\n", ql_gatt_srv->srv_handle);
            ql_gatt_demo.registered_service_num--;
            NFBT_GATT_DeleteService(ql_gatt_srv->srv_handle);
            break;
        case SRV_STOP:
            DEBUG("Will Stop GATT Service %d\n", ql_gatt_srv->srv_handle);
            NFBT_GATT_StartService(0, ql_gatt_srv->srv_handle);
            break;
        case SRV_ATTR_REG:
            if(ql_gatt_srv->registered_attr_num == ql_gatt_srv->total_attr_num)
            {
                ql_gatt_srv->status = SRV_REG_NONE;
                /* no attribute */
                DEBUG("Will Start GATT Service %d\n", ql_gatt_srv->srv_handle);
                /* register service */
                NFBT_GATT_StartService(1, ql_gatt_srv->srv_handle);
                break;
            }
            DEBUG("Will Add GATT Attribute\n");
            DEBUG("registered_attr_num=%d, total_attr_num=%d\n", ql_gatt_srv->registered_attr_num, ql_gatt_srv->total_attr_num);

            count = ql_gatt_srv->registered_attr_num;
            gatt_base_uuid[3] = ql_gatt_srv->attributes[count].uuid & 0xff;
            gatt_base_uuid[2] = (ql_gatt_srv->attributes[count].uuid>>8) & 0xff;

            if(ql_gatt_srv->attributes[count].type == ATTRTYPE_CHARACTERISTIC) {
                DEBUG("Will Add GATT Service %d Attribute %d\n", ql_gatt_srv->srv_handle, count);
                NFBT_GATT_CharacteristicAdd(ql_gatt_srv->srv_handle, gatt_base_uuid, ql_gatt_srv->attributes[count].property, ql_gatt_srv->attributes[count].permission);
            }
            else
            {
                DEBUG("Will Add GATT Service %d Descriptor %d\n", ql_gatt_srv->srv_handle, count);
                NFBT_GATT_DescriptorAdd(ql_gatt_srv->srv_handle, gatt_base_uuid, ql_gatt_srv->attributes[count].permission);
            }
            break;
    }
}

void ble_indicate_timer_thread(union sigval v)
{
    time_t timep;
    struct tm *tm_p;
    unsigned char resp[3];
    int resp_size = 0;
    int *attr_handle = v.sival_ptr;
    int retry_times = 3;
    int index = 0;
    int char_handle = 0;

    if(NULL == attr_handle) {
        ERR("Error Happen, Do not operate a null pointer\n");
    }
    char_handle = *attr_handle - 1; /**attr_handle is Characteristic handle, char_handle is characteristic handle*/

    time(&timep);
    tm_p = localtime(&timep);

    resp[0] = tm_p->tm_hour;
    resp[1] = tm_p->tm_min;
    resp[2] = tm_p->tm_sec;
    resp_size = 3;

    for(index=0; index<3; index++)
        DEBUG("attr_handle = %d, resp[%d]:%d\n", char_handle, index, resp[index]);

    while(retry_times--) {
        if(NFBT_STATUS_SUCCESS == NFBT_GATT_SendIndication(char_handle, (char *)&resp, resp_size, GATT_INDICATION_DEF))
            break;
        else
            usleep(100);
    }
}

int ble_indicate_timer_init(int sec, int usec, int *attr_handle)
{
    struct sigevent sev;

    memset(&sev, 0, sizeof(struct sigevent));
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = ble_indicate_timer_thread;
    sev.sigev_value.sival_ptr = attr_handle;

    if(-1 == timer_create(CLOCK_REALTIME, &sev, &timerid_indicate)) {
        ERR("Create Indicate Timer Failed\n");
        return -1;
    }

    ts_indicate.it_interval.tv_sec = sec;
    ts_indicate.it_interval.tv_nsec = usec;
    ts_indicate.it_value.tv_sec = sec;
    ts_indicate.it_value.tv_nsec = usec;

    if (timer_settime(timerid_indicate, 0, &ts_indicate, NULL) == -1) {
        ERR("Indicate timer_settime Failed");
        return -1;
    }
    bt_indicate_timer_flag = 1;

    return 0;
}

int ble_indicate_timer_stop(void)
{
    ts_indicate.it_interval.tv_sec = 0;
    ts_indicate.it_interval.tv_nsec = 0;
    ts_indicate.it_value.tv_sec = 0;
    ts_indicate.it_value.tv_nsec = 0;

    if (-1 == timer_settime(timerid_indicate, 0, &ts_indicate, NULL)) {
        ERR("Indicate timer_settime Failed");
        return -1;
    }

    if(-1 == timer_delete(timerid_indicate))
        ERR("Delete Indicate Timer Failed");
    bt_indicate_timer_flag = 0;
    INFO("Stop Indicate Timer Success\n");
    return 0;
}

void ble_notify_timer_thread(union sigval v)
{
    time_t timep;
    struct tm *tm_p;
    unsigned char resp[10];
    int resp_size = 0;
    int *attr_handle = v.sival_ptr;
    int retry_times = 3;
    int index = 0;
    int char_handle = 0;

    if(NULL == attr_handle) {
        ERR("Error Happen, Do not operate a null pointer\n");
    }
    char_handle = *attr_handle - 1; /**attr_handle is Characteristic handle, char_handle is characteristic handle*/

    time(&timep);
    tm_p = localtime(&timep);
    tm_p->tm_year += 1900;
    resp[0] = tm_p->tm_year&0xFF;
    resp[1] = ((tm_p->tm_year)>>8)&0xFF;
    resp[2] = tm_p->tm_mon+1;
    resp[3] = tm_p->tm_mday;
    resp[4] = tm_p->tm_hour;
    resp[5] = tm_p->tm_min;
    resp[6] = tm_p->tm_sec;
    resp[7] = tm_p->tm_wday;
    resp[8] = 0x00;
    resp[9] = 0x20;
    resp_size = 10;

    for(index=0; index<10; index++)
        DEBUG("attr_handle = %d, resp[%d]:%d\n", char_handle, index, resp[index]);

    while(retry_times--) {
        if(NFBT_STATUS_SUCCESS == NFBT_GATT_SendIndication(char_handle, (char *)&resp, resp_size, GATT_NOTIFICATION_DEF))
            break;
        else
            usleep(100);
    }
}

int ble_notify_timer_init(int sec, int usec, int *attr_handle)
{
    struct sigevent sev;

    memset(&sev, 0, sizeof(struct sigevent));
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = ble_notify_timer_thread;
    sev.sigev_value.sival_ptr = attr_handle;

    if(-1 == timer_create(CLOCK_REALTIME, &sev, &timerid_notify)) {
        ERR("Create Notify Timer Failed\n");
        return -1;
    }

    ts_notify.it_interval.tv_sec = sec;
    ts_notify.it_interval.tv_nsec = usec;
    ts_notify.it_value.tv_sec = sec;
    ts_notify.it_value.tv_nsec = usec;

    if (timer_settime(timerid_notify, 0, &ts_notify, NULL) == -1) {
        ERR("Notify timer_settime Failed");
        return -1;
    }
    bt_notify_timer_flag = 1;

    return 0;
}

int ble_notify_timer_stop(void)
{
    ts_notify.it_interval.tv_sec = 0;
    ts_notify.it_interval.tv_nsec = 0;
    ts_notify.it_value.tv_sec = 0;
    ts_notify.it_value.tv_nsec = 0;

    if (-1 == timer_settime(timerid_notify, 0, &ts_notify, NULL)) {
        ERR("Notify timer_settime Failed");
        return -1;
    }

    if(-1 == timer_delete(timerid_notify))
        ERR("Delete Notify Timer Failed");
    bt_notify_timer_flag = 0;
    INFO("Stop Notify Timer Success\n");
    return 0;
}

/*  BT basic functional callback*/
void FuncCallback_local_device_info(LOCAL_DEVICE_INFO  *info)
{
    INFO("Local device infos:\n");
    INFO("    Name:   [%s]\n", info->name);
    INFO("    Addr:   [%02X:%02X:%02X:%02X:%02X:%02X]\n" ,
            info->addr.address[0],info->addr.address[1],info->addr.address[2],
            info->addr.address[3],info->addr.address[4],info->addr.address[5]);
    switch(info->scan_mode)
    {
        case NFBT_SCAN_MODE_NONE:
            INFO("    Device Sleep\n\n");
            break;
        case NFBT_SCAN_MODE_CONNECTABLE:
            INFO("    Device Connectable\n\n");
            break;
        case NFBT_SCAN_MODE_CONNECTABLE_DISCOVERABLE:
            INFO("    Device Discoverable\n\n");
            break;
        case NFBT_SCAN_MODE_SHUTDOWN:
            ERR("    Device Shutdown");
            break;
        default:
            ERR("    Device Unknow mode\n\n");
            break;
    };

    INFO("\n");
}

void FuncCallback_paired_device_info(nfUINT8 total, PAIRED_DEVICE_INFO  *info)
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

void FuncCallback_inquiry_states(INQ_STATES inq_state, INQUIRY_DEVICE_INFO *info)
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

        printf("    Rssi  = %d\n\n" ,info->rssi);
    }
    else if(inq_state == NFBT_INQ_START){
        printf("inquiry start\n\n");
    }
    else if(inq_state == NFBT_INQ_END){
        printf("inquiry end\n\n");
    }
}

void FuncCallback_pair_states(BT_ADDR* addr, PAIR_STATES states)
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

void FuncCallback_diagnostic_message(DIAG_ID code, void* data)
{
    char dev_name[PATH_MAX_LEN]={0};

    //if (code < NFBT_DBG_BT_BASE)
    //    printf("recive exception!!!!\n");

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
    	INFO("BT Adapter must reset hardware.\n");
	/*TODO add BT_EN reset function*/
        ql_bt_module_enable();
        break;
	case NFBT_WARNING_SYS_REC_SAVE_FAILED:
		printf("Update BT config file failed (%d).\n", code);
		break;
	case NFBT_WARNING_SYS_RINGTONE_FILE_DAMAGE:
		printf("Can not find local ringtone file (%d).\n", code);
		break;
	case NFBT_WARNING_BT_CB_BLOCKING_LONGTIME:
		printf("User Callback block a log time(%d).\n", code);
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
    case NFBT_WARNING_BT_ALLOW_SLEEP:
        INFO("Allow BT Controller sleep\n");
        ql_bt_wake_low(); /*Pull Down BT_DEV_WAKE*/
        break;
    case NFBT_WARNING_BT_WAKE:
        INFO("Wake BT Controller\n");
        ql_bt_wake_high();
        break;
    case NFBT_WARNING_BT_USERIAL_FD:
    	INFO("Get BT Userial fd: %d\n",*(int*)data);
        serial_port_fd = *(int*)data;
    	break;
    default:
    	printf("Unknown message(%x)\n", code);
    	break;
    }
}

void FuncCallback_pair_request(BT_ADDR* addr, nfUINT8 name_size, nfUINT8* name, PAIR_PIN_TYPE type)
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

void FuncCallback_ssp_request(BT_ADDR* addr, nfUINT8 name_size, nfUINT8* name, nfUINT32 passkey, nfBOOLEAN display_only)
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

void FuncCallback_disconnect_reason(BT_ADDR *addr, NFBT_DISCONNECT_REASON reason)
{
    INFO("Remote device [%02X:%02X:%02X:%02X:%02X:%02X] ",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);

    switch(reason)
    {
        case NFBT_DISCONNECT_REASON_NOT_FIND:
            INFO("not find.\n\n");
            break;
        case NFBT_DISCONNECT_REASON_LINK_LOSS:
            INFO("link loss.\n\n");
            break;
        case NFBT_DISCONNECT_REASON_REMOTE_USER:
            INFO("remote user disconnect.\n\n");
            break;
        case NFBT_DISCONNECT_REASON_REMOTE_POWER_OFF:
            INFO("remote bt off.\n\n");
            break;
        default:
            ERR("unknown.\n\n");
            break;
    }

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

    /*Unlock wakelocl, Allow Module to goto sleep*/
    //ql_bt_unlock_wakelock();
}

void FuncCallback_bt_enable_state(ENABLE_STATES state)
{
    INFO("bt state:");
    switch(state)
    {
        case NFBT_BT_ENABLE:
            bt_enable_flag = 1;
            INFO(" enable \n");
            break;
        case NFBT_BT_DISABLE:
            bt_enable_flag = 0;
            INFO(" disable \n");
            break;
        case NFBT_SYS_WARNING:
            bt_enable_flag = 0;
            INFO(" system fail\n");
            break;
        default:
            bt_enable_flag = 0;
            ERR("unknown\n");
            break;
    }

    INFO("\n");
}

/*  HFP funcational*/
void FuncCallback_hfp_connect_states(BT_ADDR* addr , HFP_STATES hfp_state)
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

void FuncCallback_hfp_call_status(BT_ADDR* addr, HFP_CALL_STATUS call_status, nfUINT8* number, nfUINT8 idx)
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
void FuncCallback_hfp_call_mpty_status(BT_ADDR* addr, HFP_CALL_STATUS mpty_status, HFP_MPTY_INFO *pMptyInfo, nfUINT8 mpty_line_count)
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

void FuncCallback_hfp_audio_states(BT_ADDR* addr, HFP_AUDIO_CONNECTION_STATES audio_state)
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

void FuncCallback_hfp_device_notification(BT_ADDR* addr, HFP_DEVICE_NOTIFY_PROP notify_prop, nfUINT8 notify_prop_state)
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
        default:
            printf("unknown device notification \n\n");
            break;
    }
}

void FuncCallback_hfp_network_operator(BT_ADDR* addr, nfUINT8* operator_name)
{
    printf("Network operator of %02X:%02X:%02X:%02X:%02X:%02X\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
    printf("    name: %s\n\n", operator_name);
}

void FuncCallback_hfp_voice_recognition_states(BT_ADDR* addr, HFP_VOICE_RECOGNITION_ACTION vr_action)
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
void hfp_at_cmd_printf(HFP_COMMAND_ID cmd)
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

void FuncCallback_hfp_command_ok(BT_ADDR *addr, HFP_COMMAND_ID cmd)
{
    printf("HFP command: ");
    hfp_at_cmd_printf(cmd);
    printf("send to device %02X:%02X:%02X:%02X:%02X:%02X ok.\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
}

void FuncCallback_hfp_command_error(BT_ADDR *addr, HFP_COMMAND_ID cmd)
{
    printf("HFP command: ");
    hfp_at_cmd_printf(cmd);
    printf("send to device %02X:%02X:%02X:%02X:%02X:%02X error.\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
}

void FuncCallback_hfp_volume_change(BT_ADDR *addr, HFP_VOLUME_TYPE type, int volume)
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

void FuncCallback_hfp_subscriber_info(BT_ADDR *addr, nfUINT8 *number, HFP_SUBSCRIBER_SERVICE_TYPE service_type)
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

void FuncCallback_hfp_manufacturer_id(BT_ADDR* addr, nfUINT8* manufacturer_id)
{
    printf("Manufacturer identification of %02X:%02X:%02X:%02X:%02X:%02X: %s\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5],
            manufacturer_id);
}

void FuncCallback_hfp_manufacturer_model_id(BT_ADDR* addr, nfUINT8* manufacturer_model_id)
{
    printf("Manufacturer model identification of %02X:%02X:%02X:%02X:%02X:%02X: %s\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5],
            manufacturer_model_id);
}

/*  A2DP functional*/
void FuncCallback_a2dp_connection_states(BT_ADDR* addr,A2DP_CONN_STATES a2dp_conn_state)
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

void FuncCallback_a2dp_audio_states(BT_ADDR* addr,A2DP_AUDIO_STATES a2dp_audio_state)
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
void FuncCallback_avrcp_states(BT_ADDR* addr, AVRCP_CONN_STATES avrcp_state)
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
void FuncCallback_avrcp_remote_feature(BT_ADDR* addr, AVRCP_FEATURE feature)
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

void FuncCallback_avrcp_play_states(BT_ADDR* addr, AVRCP_PLAY_STATE play_states,nfUINT32 song_pos, nfUINT32 song_total_len)
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
void FuncCallback_avrcp_element_attribute(BT_ADDR* addr, AVRCP_ELEMENT_ATTR_STRUCT *element_attrs)
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

void FuncCallback_avrcp_capability(BT_ADDR* addr, AVRCP_CAPABILITY *events)
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

void FuncCallback_avrcp_notification(BT_ADDR* addr, AVRCP_EVENT_ID event_id,AVRCP_NOTIFICATION_TYPE type, AVRCP_NOTIFICATION *notification_info)
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
                    notification_info->track);
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

void FuncCallback_avrcp_list_player_app_attr(BT_ADDR* addr, int num_attr, AVRCP_PLAYER_ATTR *p_attrs)
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

void FuncCallback_avrcp_list_player_app_value(BT_ADDR* addr, AVRCP_PLAYER_ATTR attr_id, int num_val, nfUINT8 *p_vals)
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

void FuncCallback_avrcp_get_player_app_value(BT_ADDR* addr, AVRCP_PLAYER_SETTING *player_settings)
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

void FuncCallback_avrcp_set_player_app_value(BT_ADDR* addr)
{
    printf("The device %02X:%02X:%02X:%02X:%02X:%02X Set Player APP Value success\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
}

void FuncCallback_avrcp_command_error(BT_ADDR *bd_addr, AVRCP_COMMAND_ID pdu,
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

void FuncCallback_avrcp_passthrough_rsp(BT_ADDR *bd_addr, AVRCP_OP_ID key_code,
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

void FuncCallback_avrcp_setaddressedplayer(BT_ADDR* addr)
{
    printf("The device %02X:%02X:%02X:%02X:%02X:%02X Set Addressed Player success\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
}

void FuncCallback_avrcp_setbrowsedplayer(BT_ADDR *addr, AVRCP_BROWSED_FOLDER *folder)
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

void FuncCallback_avrcp_changepath(BT_ADDR* addr)
{
    printf("The device %02X:%02X:%02X:%02X:%02X:%02X Change Path success\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
}

void FuncCallback_avrcp_playeritem(BT_ADDR *addr, AVRCP_PLAYERITEM *item)
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

void FuncCallback_avrcp_folderitem(BT_ADDR *addr, AVRCP_FOLDERITEM *item)
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
            item->uid_counter, item->uid, item->type,
            item->playable, item->charset_id, item->name );
}

void FuncCallback_avrcp_mediaitem(BT_ADDR *addr, AVRCP_MEDIAITEM *item)
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
            item->uid_counter, item->uid,
            item->type, item->charset_id, item->name );

    FuncCallback_avrcp_element_attribute(addr, item->attr);
}

void FuncCallback_avrcp_getitem_success(BT_ADDR *addr, int count)
{
    printf( "The AVRCP browse command get folder items from %02X:%02X:%02X:%02X:%02X:%02X"
            " is successful, total item count %hu.\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5], count );
}

void FuncCallback_avrcp_getitemattr(BT_ADDR *addr, AVRCP_UID uid, AVRCP_ELEMENT_ATTR_STRUCT *element_attrs)
{
    printf( "The AVRCP browse command get item attribute from %02X:%02X:%02X:%02X:%02X:%02X"
            " is successful. The media item attribute is:\n"
            "\tUID:\t\t%llu\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5],
            uid);

    FuncCallback_avrcp_element_attribute(addr, element_attrs);
}

void FuncCallback_avrcp_playitem(BT_ADDR* addr)
{
    printf("The device %02X:%02X:%02X:%02X:%02X:%02X Play Item success\n\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5]);
}

/*  PBAP functional*/
void FuncCallback_pb_contacts_info(BT_ADDR *addr, PB_STRUCT *pbstruct)
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

void FuncCallback_pb_download_states(BT_ADDR *addr,PB_DOWNLOAD_STATES states)
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

void FuncCallback_pb_total_contact(PB_ACCESS access, nfUINT16 total_contact, BT_ADDR *addr)
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
void FuncCallback_map_download_state(BT_ADDR *addr , MAP_DOWNLOAD_STATES state)
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

void FuncCallback_map_notification_state(BT_ADDR *addr , MAP_NOTIFICATION_STATES state)
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

void FuncCallback_map_total_message(BT_ADDR *addr, MAP_MSG_TYPE type, MAP_FOLDER_STRUCT folder, nfUINT8 total)
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

void FuncCallback_map_receive_message(BT_ADDR *addr, MAP_SMS_GSM *message,
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

void FuncCallback_map_send_message(BT_ADDR *addr, nfUINT8 *tg_num,
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

void FuncCallback_map_delete_message(BT_ADDR *addr, nfUINT8 *msg_handle,
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

void FuncCallback_map_change_read_status(BT_ADDR *addr, nfUINT8 *msg_handle,
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

void FuncCallback_map_notification_event(BT_ADDR *addr, MAP_EVENT_REPORT event, nfUINT8 *msg_handle, MAP_MSG_TYPE type, MAP_FOLDER_STRUCT folder)
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
void *thread_rfcomm_read(void *data)
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

void FuncCallback_rfcomm_connect_states(BT_ADDR* addr, PROFILE_ID id, RFCOMM_STATES rfcomm_state)
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
void FuncCallback_hid_connection_states(BT_ADDR* addr,HID_CONN_STATES hid_conn_state)
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
void FuncCallback_opp_download_states(BT_ADDR *addr, OPP_STATES state)
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

void FuncCallback_opp_access_request(BT_ADDR *addr, nfUINT8 *dev_name, OPP_FILE_INFO *file_info)
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

void FuncCallback_le_test_packet_num_recv(nfUINT16 number_of_packet)
{
    printf("LE test end: received %d number of packet.\n", number_of_packet);
}

void FuncCallback_gatt_scan_result(BT_ADDR *addr, int rssi, nfUINT8 *adv_data)
{
    int i = 0;
    int array_index = 0;
    int index = 0;
    int debug_index = 0;
    static unsigned int server_index = 0;

    if (0xFFFFFFFF == server_index++) {
        server_index = 0;
    }
    array_index = server_index%MAX_SCAN_SERVER_INFO_NUM;

    DEBUG("advertising data=\n");
    for (i = 0; i < adv_data[0]; i++)
    {
        if(!(i%8))
            DEBUG("\n");
        DEBUG(",0x%02X", adv_data[i+1]);
    }
    DEBUG("\n");

    if( NULL == memset(&ql_scan_server_list[array_index], 0x00, sizeof(ql_scan_server_list[array_index])))
        ERR("Memset Failed\n");

    memcpy(&ql_scan_server_list[array_index].addr, addr, BD_ADDR_LEN);
    ql_scan_server_list[array_index].rssi = rssi;

    for (index=2; index<adv_data[0]; index++) {

        switch (adv_data[index]) {
            case GATT_ADV_CONNECT_CAP:
                INFO("GATT_ADV_CONNECT_CAP info\n");
                break;
            case GATT_ADV_SERVICE_UUID:
                memcpy(&ql_scan_server_list[array_index].srv_uuid, &adv_data[index+1], adv_data[index - 1]-1);
                ql_scan_server_list[array_index].srv_uuid_num = (adv_data[index - 1] - 1)/2;
                break;
            case GATT_ADV_COMPLETE_LOCAL_NAME:
                memcpy(&ql_scan_server_list[array_index].server_name, &adv_data[index+1], adv_data[index - 1]-1);
                ql_scan_server_list[array_index].server_name[adv_data[index - 1]] = '\0';
                break;
            case GATT_ADV_FEATURES:
                INFO("GATT_ADV_FEATURES info\n");
                break;
            default:
                ERR("Not Support advertising type 0x%x\n", adv_data[index]);
                break;
        }

        index+=adv_data[index-1];
    }
    INFO("BLE GATT Server INFO: Local Device Name[%s] | RSSI[%d]\n", ql_scan_server_list[array_index].server_name, ql_scan_server_list[array_index].rssi);
    INFO("\t\tAddress : [");
    for (index=0; index<BD_ADDR_LEN; index++) {
        INFO("%02X:", ql_scan_server_list[array_index].addr.address[index]);
    }
    INFO("]\n");

    for (index = 0; index < (ql_scan_server_list[array_index].srv_uuid_num); index++) {
        INFO("\t\tUUID%d[0x%x%x]", index/2, ql_scan_server_list[array_index].srv_uuid[index], ql_scan_server_list[array_index].srv_uuid[index+1]); 
    }
    INFO("\n\n\n");
}

void FuncCallback_gatt_search_complete_result(GATT_STATUS status)
{
    if (NFBT_GATT_SUCCESS == status)
        ble_search_service_complete = 1;
    else
        ERR("Gatt search complete operation Failed : status=%d\n", status);
}

void FuncCallback_gatt_search_result(nfUINT8 *srvc_uuid, nfUINT8 is_primary)
{
    int i;
    int srv_index = 0;

    DEBUG("\n");
    DEBUG("\tGatt search result : is_primary=%d, service UUID=0x", is_primary);
    for(i=2;i<4;i++)
        DEBUG("%02X", srvc_uuid[i]);
    DEBUG("\n");

    for (srv_index=0; srv_index<BT_MAX_SERVICE_NUM; srv_index++) {
        if (0 == ql_gatt_demo.ql_gatt_service_s[srv_index].is_srv_valid) {
            ql_gatt_demo.ql_gatt_service_s[srv_index].is_srv_valid = 1;
            ql_gatt_demo.ql_gatt_service_s[srv_index].service_uuid = (srvc_uuid[3] | (srvc_uuid[2]<<8));
            break;
        }
    }
}

void FuncCallback_gatt_get_include_service(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *inc_srvc_uuid)
{
    int i;

    printf("Gatt get include service status=%d\n", status);

    if(srvc_uuid)
    {
        printf("                   : service UUID=");
        for(i=2;i<4;i++)
            printf("%02X", srvc_uuid[i]);
        printf("\n");
    }

    if(inc_srvc_uuid)
    {
        printf("                   : include service UUID=");	
        for(i=2;i<4;i++)
            printf("%02X", inc_srvc_uuid[i]);
        printf("\n");
    }
}

void FuncCallback_gatt_get_characteristic(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, int char_prop)
{
    int i;
    int srv_index = 0;
    int char_index = 0;
    int srv_uuid = 0;


    printf("Gatt get characteristic status=%d, property=0x%04x\n", status, char_prop);
#if 1

    if(srvc_uuid)
    {
        printf("                   : service UUID=");
        for(i=2;i<4;i++)
            printf("%02X", srvc_uuid[i]);
        printf("\n");
    }
    if(char_uuid)
    {
        printf("                   : characteristic UUID=");
        for(i=2;i<4;i++)
            printf("%02X", char_uuid[i]);
        printf("\n");
    }
#endif

    if(NULL == srvc_uuid)
        INFO("srvc_uuid is NULL Pointer\n");
    if(NULL == char_uuid)
        INFO("char_uuid is NULL Pointer\n");

    if((srvc_uuid)&&(char_uuid))
    {
        srv_uuid = (int)(srvc_uuid[3] | (srvc_uuid[2]<<8));
        for(srv_index=0; srv_index<BT_MAX_SERVICE_NUM; srv_index++) {
            if((ql_gatt_demo.ql_gatt_service_s[srv_index].is_srv_valid)&&(srv_uuid == ql_gatt_demo.ql_gatt_service_s[srv_index].service_uuid)) {
                for(char_index=0; char_index<BT_MAX_ATTR_NUM; char_index++) {
                    if(0 == ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[char_index].is_attr_valid) {
                        ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[char_index].is_attr_valid = 1;
                        ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[char_index].uuid = (int)(char_uuid[3] | (char_uuid[2]<<8));
                        ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[char_index].property = char_prop;
                        ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[char_index].type = ATTRTYPE_CHARACTERISTIC;
                        break;
                    }
                }
            }
        }
    }
}

void FuncCallback_gatt_get_descriptor(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, nfUINT8 *descr_uuid)
{
    int i;
    int srv_index = 0;
    int char_index = 0;
    unsigned short srv_uuid = 0;

    srv_uuid = (srvc_uuid[3] | (srvc_uuid[2]<<8));

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

#if 0
    for (srv_index=0; srv_index<BT_MAX_SERVICE_NUM; srv_index++) {
        if ((ql_gatt_demo.ql_gatt_service_s[srv_index].is_srv_valid) && (srv_uuid == ql_gatt_demo.ql_gatt_service_s[srv_index].service_uuid)) {
            for (char_index=0; char_index<BT_MAX_ATTR_NUM; char_index++) {
                if (0 == ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[char_index].is_attr_valid) {
                    ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[char_index].uuid = (char_uuid[3] | (char_uuid[2]<<8));
                    ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[char_index].type = ATTRTYPE_DESCRIPTOR;
                    break;
                }
            }
        }
    }
#endif
}

void FuncCallback_gatt_register_notify(GATT_STATUS status, int registered)
{
    printf("RegisterNotify(%d)=%d\n", registered, status);
}

void FuncCallback_gatt_notify(BT_ADDR *addr, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, 
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

void FuncCallback_gatt_read_characteristic(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid,
        int data_type, int data_len, nfUINT8 *data)
{
    int i;
    unsigned short read_srvc_uuid = (srvc_uuid[3] | (srvc_uuid[2]<<8));
    unsigned short read_char_uuid = (char_uuid[3] | (char_uuid[2]<<8));

    if (status != NFBT_GATT_SUCCESS) {
        ERR("Gatt read characteristic or descriptor Failed, status = %d\n", status);
    }

    if(srvc_uuid)
    {
        INFO("\tService UUID->0x");
        for(i=2;i<4;i++)
            INFO("%02X", srvc_uuid[i]);
        INFO("\n");
    }
    else
        ERR("Service UUID Illegal\n");

    if(char_uuid)
    {
        INFO("\tCharacteristic UUID->0x");
        for(i=2;i<4;i++)
            INFO("%02X", char_uuid[i]);
        INFO("\n");
    }	
    else
        ERR("tCharacteristic UUID Illegal\n");

#if 0
    switch (data_type) {
        case 0x19:
            INFO("INFO->%s\n", data);
            break;

        default:
            INFO("Unknown Data Type 0x%x\n", data_type);
            break;
    }
#endif

    if(data) 
    {
        switch (read_srvc_uuid) {
            case UUID_SERVCLASS_DEVICE_INFO:
                INFO("read characteristic service uuid->0x%x\n", read_srvc_uuid);				
                switch (read_char_uuid) {
                    case GATT_UUID_MANU_NAME:
                        INFO("read characteristic char uuid->0x%x\n", read_char_uuid);
                        INFO("\t%s\n", data);
                        break;

                    case GATT_UUID_HW_VERSION_STR:
                        INFO("read characteristic char uuid->0x%x\n", read_char_uuid);
                        INFO("\t%s\n", data);
                        break;

                    case GATT_UUID_SW_VERSION_STR:
                        INFO("read characteristic char uuid->0x%x\n", read_char_uuid);
                        INFO("\t%s\n", data);
                        break;

                    default:
                        ERR("No Support characteristic UUID\n");
                        break;
                }
                break;

            case 0x180B:
                INFO("read characteristic service uuid->0x%x\n", read_srvc_uuid);				
                switch (read_char_uuid) {
                    case GATT_UUID_FW_VERSION_STR:
                        INFO("read characteristic char uuid->0x%x\n", read_char_uuid);
                        INFO("\t%s\n", data);
                        break;

                    case GATT_UUID_SERIAL_NUMBER_STR:
                        INFO("read characteristic char uuid->0x%x\n", read_char_uuid);
                        INFO("\t%s\n", data);
                        break;

                    case GATT_UUID_MODEL_NUMBER_STR:
                        INFO("read characteristic char uuid->0x%x\n", read_char_uuid);
                        INFO("\t%s\n", data);
                        break;

                    default:
                        ERR("No Support characteristic UUID\n");
                        break;
                }
                break;

            case UUID_SERVCLASS_CURRENT_TIME:
                INFO("read characteristic service uuid->0x%x\n", read_srvc_uuid);				
                switch (read_char_uuid) {
                    case GATT_UUID_CURRENT_TIME:
                        INFO("read characteristic char uuid->0x%x\n", read_char_uuid);
                        INFO("\tTO DO\n");
                        break;

                    default:
                        ERR("No Support characteristic UUID\n");
                        break;
                }
                break;

            default:
                INFO("Unknown Data Type 0x%x\n", data_type);
                break;
        }
    }

    printf(": data_type=%d, len=%d, data=", data_type, data_len);
    for(i=0;i<data_len;i++)
        printf("%02x", data[i]);
    printf("\n");
}

void FuncCallback_gatt_write_characteristic(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid)
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

void FuncCallback_gatt_read_descriptor(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, nfUINT8 *descr_uuid,
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

void FuncCallback_gatt_write_descriptor(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, nfUINT8 *descr_uuid)
{
    int i;
    if(status != NFBT_GATT_SUCCESS && status != NFBT_GATT_BUSY)
    {
        printf("Write descriptor fail status=%d\n", status);
        //return;
    }

    printf("Gatt write descriptor status=%d\n", status);

    if(NULL == srvc_uuid)
        printf("srvc_handle NULL\n");
    if(NULL == char_uuid)
        printf("char_handle NULL\n");
    if(NULL == descr_uuid)
        printf("descr_handle NULL\n");
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

void FuncCallback_gatt_read_rssi(GATT_STATUS status, BT_ADDR *addr, int rssi)
{
    if (NFBT_GATT_SUCCESS == status) {
        INFO("Gatt read rssi :  BT ADDR[%02X:%02X:%02X:%02X:%02X:%02X], rssi=%d\n",
                addr->address[0],addr->address[1],addr->address[2],
                addr->address[3],addr->address[4],addr->address[5], rssi);
    }
    else
        ERR("Gatt read rssi Failed\n");

}

void FuncCallback_gatt_set_adv_data_result(GATT_STATUS status)
{
    if(status == NFBT_GATT_SUCCESS)
        INFO("Gatt advertising set data : status=%d\n", status);
    else
        ERR("Gatt advertising set data Fail\n");
}

void FuncCallback_gatt_client_register(GATT_STATUS status)
{
    if(status == NFBT_GATT_SUCCESS) {
        ql_gatt_demo.gatt_client_register_status = 1;
	INFO("BLE Gatt Client Register Success\n");
    }
    else {
        ql_gatt_demo.gatt_client_register_status = 0;
	ERR("BLE Gatt Client Register Fail\n");
    }
}

void FuncCallback_gatt_service_added(GATT_STATUS status, nfUINT8 *srvc_uuid, int srvc_handle)
{
    int i;
    int srv_count = ql_gatt_demo.registered_service_num;

    INFO("Gatt service added : status=%d srvc_handle=%d\n", status, srvc_handle);
    INFO("                   : service UUID=");

    for(i=0;i<16;i++)
        INFO("%02X", srvc_uuid[i]);
    INFO("\n");

    if(status == NFBT_GATT_SUCCESS) {
        ql_gatt_demo.ql_gatt_service_s[srv_count].is_srv_registered = 1;
        ql_gatt_demo.ql_gatt_service_s[srv_count].srv_handle = srvc_handle;
        ql_gatt_demo.registered_service_num++;
        DEBUG("service[%d] register success, srv_handle=%d\n", srv_count, srvc_handle);
    }
    else 
        ERR("Add Service Failed\n");

    DEBUG("ql_demo_service_reg.status=%d\n", ql_gatt_demo.ql_gatt_service_s[srv_count].status);

    if(ql_gatt_demo.ql_gatt_service_s[srv_count].status != SRV_REG_NONE)
    {
        DEBUG("FuncCallback_gatt_service_added 0\n");
        if(status == NFBT_GATT_SUCCESS)
        {
            DEBUG("[Quinn]FuncCallback_gatt_service_added 1\n");
            ql_gatt_demo.ql_gatt_service_s[srv_count].srv_handle = srvc_handle;
            ql_gatt_demo.ql_gatt_service_s[srv_count].status = SRV_ATTR_REG;
            DEBUG("Status will change to SRV_ATTR_REG\n");
        }
        else
        {
            DEBUG("[Quinn]FuncCallback_gatt_service_added 2\n");
            ql_gatt_demo.ql_gatt_service_s[srv_count].status = SRV_REG_FAIL;
            DEBUG("Status will change to SRV_REG_FAIL\n");
        }
        ql_gatt_service_register(&ql_gatt_demo.ql_gatt_service_s[srv_count]);
    }
}

void FuncCallback_gatt_char_added(GATT_STATUS status, nfUINT8 *char_uuid, int srvc_handle, int char_handle)
{
    int i;
    int tmp_uuid = 0;
    int srv_count = ql_gatt_demo.registered_service_num - 1;
    int attr_count = ql_gatt_demo.ql_gatt_service_s[srv_count].registered_attr_num;

    tmp_uuid |= char_uuid[3];
    tmp_uuid |= char_uuid[2]<<8;

    if(char_uuid)
    {
        INFO("                   : characteristic UUID=");
        for(i=0;i<16;i++)
            INFO("%02X", char_uuid[i]);
        INFO("\n");
    }

    if(status == NFBT_GATT_SUCCESS) {
        if(ql_gatt_demo.ql_gatt_service_s[srv_count].srv_handle == srvc_handle) {
            INFO("Character add Success\n");
            ql_gatt_demo.ql_gatt_service_s[srv_count].attributes[attr_count].is_attr_registered = 1;
            ql_gatt_demo.ql_gatt_service_s[srv_count].attributes[attr_count].attr_handle = char_handle;
            ql_gatt_demo.ql_gatt_service_s[srv_count].attributes[attr_count].attr_index = ql_gatt_demo.ql_gatt_service_s[srv_count].registered_attr_num++; 
        }
        else
            ERR("Because Service Handle Wrong, Character add Failed, srv_count=%d, srvc_hamdle=%d\n", srv_count, srvc_handle);
    }
    else
        ERR("Character add Failed\n");

    INFO("Gatt characteristic added status : status=%d, srvc_handle=%d, UUID = 0x%x, char. handle=%d, attr_index=%d\n", status, srvc_handle, tmp_uuid, char_handle, ql_gatt_demo.ql_gatt_service_s[srv_count].attributes[attr_count].attr_index);

    if(ql_gatt_demo.ql_gatt_service_s[srv_count].status != SRV_REG_NONE)
    {
        if(status == NFBT_GATT_SUCCESS)
        {
            ql_gatt_demo.ql_gatt_service_s[srv_count].srv_handle = srvc_handle;
            ql_gatt_demo.ql_gatt_service_s[srv_count].status = SRV_ATTR_REG;
            DEBUG("Status will change to SRV_ATTR_REG\n");
        }
        else
        {
            ql_gatt_demo.ql_gatt_service_s[srv_count].status = SRV_REG_FAIL;
            DEBUG("Status will change to SRV_REG_FAIL\n");
        }
        ql_gatt_service_register(&ql_gatt_demo.ql_gatt_service_s[srv_count]);
    }
}

void FuncCallback_gatt_des_added(GATT_STATUS status, nfUINT8 *descr_uuid, int srvc_handle, int descr_handle)
{
    int i;
    int tmp_uuid = 0;
    int srv_count = ql_gatt_demo.registered_service_num - 1;
    int attr_count = ql_gatt_demo.ql_gatt_service_s[srv_count].registered_attr_num;

    tmp_uuid |= descr_uuid[3];
    tmp_uuid |= descr_uuid[2]<<8;

    if(descr_uuid)
    {
        INFO("                   : descriptor UUID=");
        for(i=0;i<16;i++)
            INFO("%02X", descr_uuid[i]);
        INFO("\n");
    }

    if(status == NFBT_GATT_SUCCESS) {
        if(ql_gatt_demo.ql_gatt_service_s[srv_count].srv_handle == srvc_handle) {
            INFO("Descriptor add Success\n");
            ql_gatt_demo.ql_gatt_service_s[srv_count].attributes[attr_count].is_attr_registered = 1;
            ql_gatt_demo.ql_gatt_service_s[srv_count].attributes[attr_count].attr_handle = descr_handle;
            ql_gatt_demo.ql_gatt_service_s[srv_count].attributes[attr_count].attr_index = ql_gatt_demo.ql_gatt_service_s[srv_count].registered_attr_num++; 
        }
        else
            ERR("Because Service Handle Wrong, Descriptor add Failed, srv_count=%d, srvc_hamdle=%d\n", srv_count, srvc_handle);
    }
    else
        ERR("Descriptor add Failed\n");

    DEBUG("Gatt descriptor added : status=%d, srvc_handle=%d, descr_handle=%d, attr_index=%d\n", status, srvc_handle, descr_handle, ql_gatt_demo.ql_gatt_service_s[srv_count].attributes[attr_count].attr_index);

    if(ql_gatt_demo.ql_gatt_service_s[srv_count].status != SRV_REG_NONE)
    {
        if(status == NFBT_GATT_SUCCESS)
        {
            ql_gatt_demo.ql_gatt_service_s[srv_count].srv_handle = srvc_handle;
            ql_gatt_demo.ql_gatt_service_s[srv_count].status = SRV_ATTR_REG;
            DEBUG("Status will change to SRV_ATTR_REG\n");
        }
        else
        {
            ql_gatt_demo.ql_gatt_service_s[srv_count].status = SRV_REG_FAIL;
            DEBUG("Status will change to SRV_REG_FAIL\n");
        }
        ql_gatt_service_register(&ql_gatt_demo.ql_gatt_service_s[srv_count]);
    }
}

void FuncCallback_gatt_service_start(GATT_STATUS status, int srvc_handle)
{
    INFO("Gatt service start : status=%d, srvc_handle=%d\n", status, srvc_handle);
}

void FuncCallback_gatt_service_stop(GATT_STATUS status, int srvc_handle)
{
    int srv_count = ql_gatt_demo.registered_service_num - 1;
    INFO("Gatt service stop : status=%d, srvc_handle=%d\n", status, srvc_handle);

    if(ql_gatt_demo.ql_gatt_service_s[srv_count].status != SRV_REG_NONE)
    {
        if(status == NFBT_GATT_SUCCESS)
        {
            ql_gatt_demo.ql_gatt_service_s[srv_count].srv_handle = srvc_handle;
            ql_gatt_demo.ql_gatt_service_s[srv_count].status = SRV_UNREG;
        }
        else
        {
            ql_gatt_demo.ql_gatt_service_s[srv_count].status = SRV_REG_FAIL;
        }
        ql_gatt_service_register(&ql_gatt_demo.ql_gatt_service_s[srv_count]);
    }
}

void FuncCallback_gatt_service_deleted(GATT_STATUS status, int srvc_handle)
{
    int srv_count = ql_gatt_demo.registered_service_num - 1;
    INFO("Gatt service deleted : status=%d, srvc_handle=%d\n", status, srvc_handle);

    if(status == NFBT_GATT_SUCCESS)
        INFO("Gatt service deleted Succeed\n");

    if(ql_gatt_demo.ql_gatt_service_s[srv_count].status != SRV_REG_NONE)
    {
        if(status == NFBT_GATT_SUCCESS)
        {
            ql_gatt_demo.ql_gatt_service_s[srv_count].status = SRV_REG_NONE;
        }
        else
        {
            ql_gatt_demo.ql_gatt_service_s[srv_count].status = SRV_REG_FAIL;
        }
        ql_gatt_service_register(&ql_gatt_demo.ql_gatt_service_s[srv_count]);
    }
}

void FuncCallback_gatt_request_read_event(BT_ADDR *addr, int trans_id, int attr_handle, int offset, int is_long)
{
    char *resp = NULL;
    int resp_size = 0;
    int attr_handle_index = 0; 
    int srv_index = 0;
    int attr_index = 0;
    int end_loop_flag = 0;
    int string_flag = 0;    /*Whether Response is string*/
    int index = 0;
    time_t timep;
    struct tm *tm_p;

    time(&timep);
    tm_p = localtime(&timep);

    DEBUG("Gatt Request read event :  [%02X:%02X:%02X:%02X:%02X:%02X], trans_id=%d, attr_handle=%d, offset=%d, is_long=%d\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5], trans_id, attr_handle, offset, is_long);

    /*According to the unique attr_handle to find out the corresponding Characteristic/Descriptor*/
    for(srv_index=0; srv_index<ql_gatt_demo.registered_service_num; srv_index++) {
        for(attr_index=0; attr_index<ql_gatt_demo.ql_gatt_service_s[srv_index].registered_attr_num; attr_handle_index++, attr_index++) {
            DEBUG("[Quinn]srv_index=%d, attr_index=%d, attr_handle_index=%d, attr_handle=%d\n", srv_index, attr_index, attr_handle_index, ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[attr_index].attr_handle);
            if(attr_handle == ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[attr_index].attr_handle) {
                end_loop_flag = 1;
                break;
            }
        }
        if(1 == end_loop_flag) {
            end_loop_flag = 0;
            break;
        }
    }

    DEBUG("attr_handle_index=%d\n", attr_handle_index);

    /*Get data and data length from Characteristic/Descriptor private data space*/
    resp = (char *)ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[attr_index].data;
    resp_size = ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[attr_index].data_bytes;

    switch (attr_handle_index)
    {
        case SRV0_MANUFACTURER_NAME_CHAR_ATTR_HANDLE:
            string_flag = 1;
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            break;
        case SRV0_MANUFACTURER_NAME_DESC_ATTR_HANDLE:
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            break;
        case SRV0_SOFTWARE_REV_CHAR_ATTR_HANDLE:
            string_flag = 1;
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            break;
        case SRV0_SOFTWARE_REV_DESC_ATTR_HANDLE:
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            break;
        case SRV1_FIRMWARE_REV_CHAR_ATTR_HANDLE:
            string_flag = 1;
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            break;
        case SRV1_FIRMWARE_REV_DESC_ATTR_HANDLE:
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            break;
        case SRV1_MODEL_NUMBER_CHAR_ATTR_HANDLE:
            string_flag = 1;
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            break;
        case SRV1_MODEL_NUMBER_DESC_ATTR_HANDLE:
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            break;
        case SRV2_CURRENT_TIME_CHAR_ATTR_HANDLE:
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            tm_p->tm_year += 1900;
            resp[0] = tm_p->tm_year&0xFF;
            resp[1] = ((tm_p->tm_year)>>8)&0xFF;
            resp[2] = tm_p->tm_mon+1;
            resp[3] = tm_p->tm_mday;
            resp[4] = tm_p->tm_hour;
            resp[5] = tm_p->tm_min;
            resp[6] = tm_p->tm_sec;
            resp[7] = tm_p->tm_wday;
            resp[8] = 0x00;
            resp[9] = 0x20;
            break;
        case SRV2_CURRENT_TIME_DESC_ATTR_HANDLE:
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            break;
        case SRV2_INDICATE_TIME_CHAR_ATTR_HANDLE:
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            resp[0] = tm_p->tm_hour;
            resp[1] = tm_p->tm_min;
            resp[2] = tm_p->tm_sec;
            break;
        case SRV2_INDICATE_TIME_DESC_ATTR_HANDLE:
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            break;
        default:
            ERR("Unknown Characteristic Handle:%d\n", attr_handle);
    }

    if (string_flag) {
        resp_size = strlen(resp);
    }
    DEBUG("[Quinn]resp len: %d\n", resp_size);
    for(index=0; index<resp_size; index++)
        DEBUG("data[%d]=0x%x\n", index, resp[index]);

    NFBT_GATT_SendResponse(trans_id, NFBT_GATT_SUCCESS, attr_handle, attr_handle, (char*)resp, resp_size);
}

void FuncCallback_gatt_request_write_event(BT_ADDR *addr, int trans_id, int attr_handle, int offset, int is_prep, nfUINT8 *data, int length, int need_rsp)
{
    int i;
    int attr_handle_index = 0; 
    int srv_index = 0;
    int attr_index = 0;
    int end_loop_flag = 0;
    int index = 0;
    char *attr_data_p = NULL;
    int attr_data_len = 0;

    if(addr)
    {
        DEBUG("Gatt Request write event :  [%02X:%02X:%02X:%02X:%02X:%02X], trans_id=%d, attr_handle=%d, offset=%d, is_prep=%d, length=%d, need_rsp=%d\n",
                addr->address[0],addr->address[1],addr->address[2],
                addr->address[3],addr->address[4],addr->address[5], trans_id, attr_handle, offset, is_prep, length, need_rsp);
    }

    if(data)
    {
        DEBUG(": data=\n");
        for(i=0;i<length;i++)
        {
            DEBUG("index=%d,value=0x%x\n", (i+offset), data[i+offset]);
        }
        DEBUG("\n");
    }

    /*According to the unique attr_handle to find out the corresponding Characteristic/Descriptor*/
    for(srv_index=0; srv_index<ql_gatt_demo.registered_service_num; srv_index++) {
        for(attr_index=0; attr_index<ql_gatt_demo.ql_gatt_service_s[srv_index].registered_attr_num; attr_handle_index++, attr_index++) {
            DEBUG("srv_index=%d, attr_index=%d, attr_handle_index=%d, attr_handle=%d\n", srv_index, attr_index, attr_handle_index, ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[attr_index].attr_handle);
            if(attr_handle == ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[attr_index].attr_handle) {
                end_loop_flag = 1;
                break;
            }
        }
        if(1 == end_loop_flag) {
            end_loop_flag = 0;
            break;
        }
    }

    /*Write data into Characteristic/Descriptor private data space*/
    attr_data_len = ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[attr_index].data_bytes;
    attr_data_p = (char *)ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[attr_index].data;
    if(NULL == memset(attr_data_p, 0x00, attr_data_len)) {
        ERR("Memset Failed\n");
    }

    if(length > attr_data_len) {
        length = attr_data_len; /*Only write attr_data_len bytes*/
        ERR("Write too long data\n");
    }
    for(index=0; index<length; index++) {
        attr_data_p[index] = data[index];
    }

    DEBUG("attr_handle_index=%d\n", attr_handle_index);

    switch (attr_handle_index)
    {
        case SRV0_MANUFACTURER_NAME_CHAR_ATTR_HANDLE:
            /*Please add your own specific features here*/
            break;
        case SRV0_MANUFACTURER_NAME_DESC_ATTR_HANDLE:
            /*Please add your own specific features here*/
            break;
        case SRV0_SOFTWARE_REV_CHAR_ATTR_HANDLE:
            /*Please add your own specific features here*/
            break;
        case SRV0_SOFTWARE_REV_DESC_ATTR_HANDLE:
            /*Please add your own specific features here*/
            break;
        case SRV1_FIRMWARE_REV_CHAR_ATTR_HANDLE:
            /*Please add your own specific features here*/
            break;
        case SRV1_FIRMWARE_REV_DESC_ATTR_HANDLE:
            /*Please add your own specific features here*/
            break;
        case SRV1_MODEL_NUMBER_CHAR_ATTR_HANDLE:
            /*Please add your own specific features here*/
            break;
        case SRV1_MODEL_NUMBER_DESC_ATTR_HANDLE:
            /*Please add your own specific features here*/
            break;
        case SRV2_CURRENT_TIME_CHAR_ATTR_HANDLE:
            /*Please add your own specific features here*/
            break;
        case SRV2_CURRENT_TIME_DESC_ATTR_HANDLE:
            if(1 == data[0]) {
                ble_notify_timer_init(10, 0, &ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[attr_index].attr_handle);
            } 
            else 
                ble_notify_timer_stop();
            break;
        case SRV2_INDICATE_TIME_CHAR_ATTR_HANDLE:
            /*Please add your own specific features here*/
            break;
        case SRV2_INDICATE_TIME_DESC_ATTR_HANDLE:
            if(2 == data[0])  {
                ble_indicate_timer_init(5, 0, &ql_gatt_demo.ql_gatt_service_s[srv_index].attributes[attr_index].attr_handle);
            } 
            else
                ble_indicate_timer_stop();
            break;
        default:
            ERR("Unknown Characteristic Handle:%d\n", attr_handle);
            break;
    }

    if(need_rsp)
    {
        const char resp[2] = {0x00, 0x00};
        NFBT_GATT_SendResponse(trans_id, NFBT_GATT_SUCCESS, attr_handle, attr_handle, (char*)resp, sizeof(resp));
    }

    //ql_bt_unlock_wakelock(); /*Send over, go into sleep mode*/
}

void FuncCallback_gatt_request_exec_write_event(BT_ADDR *addr, int trans_id, int exec_write)
{
    printf("Gatt Request execute write event :  [%02X:%02X:%02X:%02X:%02X:%02X], trans_id=%d, exec_write=%d\n",
            addr->address[0],addr->address[1],addr->address[2],
            addr->address[3],addr->address[4],addr->address[5], trans_id, exec_write);
}

void FuncCallback_gatt_response_confirm(GATT_STATUS status)
{
    DEBUG("Gatt response confirm : status=%d\n", status);
}

void FuncCallback_gatt_indicator_sent(GATT_STATUS status)
{
    DEBUG("Gatt indicator : status=%d\n", status);

    /* write next packet or operate complete */
}
void FuncCallback_gatt_congestion(int congested)
{
    DEBUG("Gatt congestion : status=%d\n", congested);

    if(congested == 0)
    {
        DEBUG("write next packet or operate complete \n");
    }
    else if(congested == 1)
    {
        ERR("do not write next packet until congested = 0"); 
    }
}

void FuncCallback_gatt_connect_status(GATT_CONN_STATUS status, BT_ADDR *addr, GATT_ROLE role)
{
    switch(status)
    {
        case GATT_CONN_DISCONNECT:
            INFO( "GATT is now disconnected to ");
            break;
        case GATT_CONN_START_LISTENING:
            INFO( "GATT is now prepare listening to ");
            break;
        case GATT_CONN_LISTENING:
            INFO( "GATT is now listening to ");
            break;
        case GATT_CONN_CONNECTING:
            INFO( "GATT is now connecting to ");
            break;
        case GATT_CONN_CONNECTED:
            INFO( "GATT is now connected to ");
            if(NULL == memcpy(&connected_dev_addr, addr, sizeof(BT_ADDR)))
	       ERR("Fatal Error, NULL Pointer\n");
            break;
        default:
            INFO( "unknown HFP connect state to ");
            break;
    }

    if(addr)
    {
        INFO("addr: [%02X:%02X:%02X:%02X:%02X:%02X], role=%s\n",
                addr->address[0],addr->address[1],addr->address[2],
                addr->address[3],addr->address[4],addr->address[5], role?"Central":"Peripheral");
    }
    else
        INFO("role=%s\n", role?"Central":"Peripheral");

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
}

void FuncCallback_gatt_set_role_result(GATT_STATUS status, GATT_ROLE role)
{
    INFO("Gatt set role : status=%d, role=%d\n", status, role);
}

void FuncCallback_gatt_attribute_list(int handle, nfUINT8 *uuid, int property, int type)
{
    int i;

    INFO("Character list : hdl=%d, property=0x%04x, type=%d ", handle, property, type);
    if(uuid)
    {
        INFO("uuid=");
        for(i=0;i<16;i++)
            INFO("%02X", uuid[i]);
    }
    INFO("\n");
}

void FuncCallback_gatt_service_list(int handle, nfUINT8 *uuid)
{
    int i;

    INFO("Service list hdl=%d ", handle);
    if(uuid)
    {
        INFO("uuid=");
        for(i=0;i<16;i++)
            INFO("%02X", uuid[i]);
    }
    INFO("\n");
}

void FuncCallback_gatt_list_complete(GATT_STATUS status)
{
    INFO("List complete\n");
}

void FuncCallback_gatt_service_register(GATT_STATUS status)
{
    if(status == NFBT_GATT_SUCCESS) {
        ql_gatt_demo.gatt_server_register_status = 1;
	INFO("BLE Gatt Server Register Success\n");
    }
    else {
        ql_gatt_demo.gatt_server_register_status = 0;
	ERR("BLE Gatt Server Register Fail\n");
    }
}

void FuncCallback_gatt_service_unregister(GATT_STATUS status)
{
    INFO("Service unRegister status=%d\n", status);
}

void FuncCallback_gatt_battery_level_read_event(int trans_id, int attr_handle)
{
    printf("Battery level read request\n");

    const char resp[1] = {80}; /* read battery level, range is 0~100 */
    NFBT_GATT_SendResponse(trans_id, NFBT_GATT_SUCCESS, attr_handle, attr_handle, (char*)resp, sizeof(resp));
}

void FuncCallback_gatt_blood_pressure_feature_read_event(int trans_id, int attr_handle)
{
    printf("Blood pressure feature read request\n");

    const char resp[2] = {BPM_FEATURE_SUPPORT&0xff, (BPM_FEATURE_SUPPORT>>8)&0xff};
    NFBT_GATT_SendResponse(trans_id, NFBT_GATT_SUCCESS, attr_handle, attr_handle, (char*)resp, sizeof(resp));
}

void FuncCallback_gatt_spp_rx_receive_event(int offset, nfUINT8 *value, int length)
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

void FuncCallback_gatt_char_notification_req(int srvc_idx, int attr_id, int enable)
{
    printf("Characteristic 0x%04x notification %s\n", attr_id, enable? "Enable": "Disable");
}
