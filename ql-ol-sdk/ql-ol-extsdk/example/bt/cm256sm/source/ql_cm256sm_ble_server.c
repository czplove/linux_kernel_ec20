/*****************************************************************************
**
**  Name:           app_ble_test_server.c
**
**  Description:    Bluetooth BLE test server main application
**
**  Copyright (c) 2016, Cypress Semiconductor., All Rights Reserved.
**  Proprietary and confidential.
**
*****************************************************************************/

#include "app_ble.h"
#include "app_thread.h"
#include "app_xml_utils.h"
#include "app_utils.h"
#include "app_mgt.h"
#include "app_disc.h"
#include "app_dm.h"
#include "app_manager.h"

#include "ql_cm256sm_ble_server.h"
#include "ql_cm256sm_ble_sleep.h"

#define QL_BLE_SERVER_NUM 1
#define QL_BLE_SERVER_ATTR_NUM 1

enum
{
    SRV0_MANUFACTURER_NAME_CHAR_ATTR_HANDLE = 111,
    SRV0_SOFTWARE_REV_CHAR_ATTR_HANDLE,
    SRV1_FIRMWARE_REV_CHAR_ATTR_HANDLE = 121,
    SRV1_MODEL_NUMBER_CHAR_ATTR_HANDLE,
    SRV2_CURRENT_TIME_CHAR_ATTR_HANDLE = 131,
    SRV2_CURRENT_TIME_DESC_ATTR_HANDLE,
    SRV2_INDICATE_TIME_CHAR_ATTR_HANDLE,
    SRV2_INDICATE_TIME_DESC_ATTR_HANDLE,
    MAX_ATTR_HANDLE_NUM
};

tAPP_BLE_CB ql_app_ble_cb;

int ql_prepare_send_data(tBSA_BLE_SE_SENDRSP *p_send_server_resp)
{
    int server_index = 0;
    int service_index = 0;
    int char_index = 0;
    int index = 0;
    int string_flag = 0;    /*Whether Response is string*/
    printf("[ql_prepare_send_data] handle:%d\n", p_send_server_resp->handle);
    
    for(server_index=1; server_index<BSA_BLE_SERVER_MAX; server_index++)
    {
        for(service_index=1; service_index<BSA_BLE_ATTRIBUTE_MAX; service_index++)
        {
            for(char_index=1; char_index<BSA_BLE_CHAR_MAX; char_index++)
            {
                if(p_send_server_resp->handle == ql_app_ble_cb.ble_server[server_index].attr[service_index].characteristic[char_index].char_handle)
                {
                    printf("[ql_prepare_send_data] srv=%d, service=%d, char=%d, char handle:%d\n", server_index, service_index, char_index, ql_app_ble_cb.ble_server[server_index].attr[service_index].characteristic[char_index].char_handle);
                    index = 100*server_index+10*service_index+char_index;
                    goto get_index;
                }
            }
        }
    }

    return -1;
    
    printf("Failed [%s] server_index:%d, service_index:%d, char_index:%d\n", __func__, server_index, service_index, char_index);
get_index:
    switch (index)
    {
        case SRV0_MANUFACTURER_NAME_CHAR_ATTR_HANDLE:
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            string_flag = 1;
            snprintf((char *)p_send_server_resp->value, sizeof(p_send_server_resp->value), "%s", "Quectel CM256SM BT");
            break;
        case SRV0_SOFTWARE_REV_CHAR_ATTR_HANDLE:
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            string_flag = 1;
            snprintf((char *)p_send_server_resp->value, sizeof(p_send_server_resp->value), "%s", "SoftWare Version v1.0");
            break;
        case SRV1_FIRMWARE_REV_CHAR_ATTR_HANDLE:
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            string_flag = 1;
            snprintf((char *)p_send_server_resp->value, sizeof(p_send_server_resp->value), "%s", "bsa0107_00.28");
            break;
        case SRV1_MODEL_NUMBER_CHAR_ATTR_HANDLE:
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            string_flag = 1;
            snprintf((char *)p_send_server_resp->value, sizeof(p_send_server_resp->value), "%s", "EC20CEFA");
            break;
        case SRV2_CURRENT_TIME_CHAR_ATTR_HANDLE:
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            break;
        case SRV2_CURRENT_TIME_DESC_ATTR_HANDLE:
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            break;
        case SRV2_INDICATE_TIME_CHAR_ATTR_HANDLE:
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            break;
        case SRV2_INDICATE_TIME_DESC_ATTR_HANDLE:
            /*Please add your own specific features here, Pay attention to data overflow and data  max length*/
            break;
        default:
            printf("Unknown Characteristic Handle:%d\n", index);
    }

    printf("[%s] server_index:%d, service_index:%d, char_index:%d\n", __func__, server_index, service_index, char_index);
     if (string_flag) {
         p_send_server_resp->len = strlen((char *)p_send_server_resp->value);
    }
    printf("\t resp len = %d\n", p_send_server_resp->len);

    return 0;
}

/*******************************************************************************
**
** Function         ql_ble_server_profile_cback
**
** Description      ble test server profile callback.
**
** Returns          void
**
*******************************************************************************/
void ql_ble_server_profile_cback(tBSA_BLE_EVT event,  tBSA_BLE_MSG *p_data)
{
    int current_server_index = 0;
    int current_service_index = 0;
    int current_char_index = 0;
    tBSA_BLE_SE_SENDRSP send_server_resp;
    
    switch (event)
    {
        case BSA_BLE_SE_DEREGISTER_EVT:
            if (p_data->ser_deregister.status != BSA_SUCCESS)
            {
                APP_ERROR1("BLE Server is deregistered failed status = %d", p_data->ser_deregister.status);
                break;
            }
            printf("BLE Server is deregistered\n");
            printf("\tserver_if=%d\n\n", p_data->ser_deregister.server_if);
            break;
            
        case BSA_BLE_SE_CREATE_EVT:
            if (p_data->ser_create.status != BSA_SUCCESS)
            {
                APP_ERROR1("Service is created failed status = %d", p_data->ser_create.status);
                break;
            }

            current_server_index = ql_app_ble_cb.current_server;
            ql_app_ble_cb.ble_server[current_server_index].current_service++;
            current_service_index = ql_app_ble_cb.ble_server[current_server_index].current_service;
            ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].service_id = p_data->ser_create.service_id;
            printf("Service is created\n");
            printf("\tserver_if=%d, service_id=%d\n\n", p_data->ser_create.server_if, p_data->ser_create.service_id);
            printf("current_server_index:%d, current_service_index:%d\n", current_server_index, current_service_index);
            break;
            
        case BSA_BLE_SE_ADDCHAR_EVT:
            if (p_data->ser_addchar.status != BSA_SUCCESS)
            {
                APP_ERROR1("char data is added failed status = %d", p_data->ser_addchar.status);
                break;
            }

            current_server_index = ql_app_ble_cb.current_server;
            current_service_index = ql_app_ble_cb.ble_server[current_server_index].current_service;
            ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].current_char++;
            current_char_index = ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].current_char;
            ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].characteristic[current_char_index].char_id = 100*current_server_index+10*current_service_index+current_char_index;
            ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].characteristic[current_char_index].char_handle = p_data->ser_addchar.attr_id;
            
            printf("char data is added \n");
            printf("\tserver_if:%d, service_id:%d, attr_id:%d, is_discr:%d, ", \
            p_data->ser_addchar.server_if, p_data->ser_addchar.service_id, p_data->ser_addchar.attr_id, p_data->ser_addchar.is_discr);
            printf("char_id=%d\n", ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].characteristic[current_char_index].char_id);
            printf("\tcurrent_server_index:%d, current_service_index:%d, current_char_index:%d\n", current_server_index, current_service_index, current_char_index);
            printf("\thandle:%d\n", ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].characteristic[current_char_index].char_handle);
            break;
            
        case BSA_BLE_SE_START_EVT:
            current_server_index = ql_app_ble_cb.current_server;
            current_service_index = ql_app_ble_cb.ble_server[current_server_index].current_service;
            if (p_data->ser_start.status != BSA_SUCCESS)
            {
                APP_ERROR1("Service is started failed status = %d", p_data->ser_start.status);
                memset(&ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index], 0, sizeof(tAPP_BLE_SERVICE));
                break;
            }
            
            ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].service_id = p_data->ser_create.service_id;
            printf("Service is started\n");
            printf("\tserver_if=%d, service_id=%d\n\n", p_data->ser_start.server_if, p_data->ser_start.service_id);
            break;
            
        case BSA_BLE_SE_STOP_EVT:
            if (p_data->ser_stop.status != BSA_SUCCESS)
            {
                APP_ERROR1("Service is stopped failed status = %d", p_data->ser_stop.status);
                break;
            }
            printf("Service is stopped\n");
            printf("\tserver_if=%d, service_id=%d\n\n", p_data->ser_stop.server_if, p_data->ser_stop.service_id);
            break;
            
        case BSA_BLE_SE_WRITE_EVT:
            printf("Write request from client\n");
            APP_INFO1("BSA_BLE_SE_WRITE_EVT status:%d", p_data->ser_write.status);
            APP_INFO1("Write value", p_data->ser_write.value, p_data->ser_write.len);
            APP_INFO1("BSA_BLE_SE_WRITE_EVT trans_id:%d, conn_id:%d, handle:%d",
            p_data->ser_write.trans_id, p_data->ser_write.conn_id,
            p_data->ser_write.handle);

#if 0
            if (p_data->ser_write.handle == app_ble_ts_cb.chars[0].attr_id)
            {
                if (p_data->ser_write.value[0])
                {
                    app_ble_ts_cb.notification_send = TRUE;
                }
                else
                {
                    app_ble_ts_cb.notification_send = FALSE;
                }
            }
            else if (p_data->ser_write.handle == app_ble_ts_cb.chars[1].attr_id)
            {
                if (p_data->ser_write.value[0])
                {
                    app_ble_ts_cb.indication_send = TRUE;
                }
                else
                {
                    app_ble_ts_cb.indication_send = FALSE;
                }
            }
 #endif
            if (p_data->ser_write.need_rsp)
            {
                BSA_BleSeSendRspInit(&send_server_resp);
                send_server_resp.conn_id = p_data->ser_write.conn_id;
                send_server_resp.trans_id = p_data->ser_write.trans_id;
                send_server_resp.status = p_data->ser_write.status;
                send_server_resp.handle = p_data->ser_write.handle;
                send_server_resp.len = 0;
                APP_INFO1("BSA_BLE_SE_WRITE_EVT: send_server_resp.conn_id:%d, send_server_resp.trans_id:%d", send_server_resp.conn_id, send_server_resp.trans_id, send_server_resp.status);
                APP_INFO1("BSA_BLE_SE_WRITE_EVT: send_server_resp.status:%d,send_server_resp.auth_req:%d", send_server_resp.status,send_server_resp.auth_req);
                APP_INFO1("BSA_BLE_SE_WRITE_EVT: send_server_resp.handle:%d, send_server_resp.offset:%d, send_server_resp.len:%d", send_server_resp.handle,send_server_resp.offset,send_server_resp.len );
                BSA_BleSeSendRsp(&send_server_resp);
            }
            break;
            
        case BSA_BLE_SE_EXEC_WRITE_EVT:
            printf("Execute Write request from client \n");
            break;
            
        case BSA_BLE_SE_CONGEST_EVT:
            printf("Congestion event\n");
            break;
            
        case BSA_BLE_SE_READ_EVT:
            printf("Read request from client\n");
            BSA_BleSeSendRspInit(&send_server_resp);
            send_server_resp.conn_id = p_data->ser_read.conn_id;
            send_server_resp.trans_id = p_data->ser_read.trans_id;
            send_server_resp.status = p_data->ser_read.status;
            send_server_resp.handle = p_data->ser_read.handle;
            ql_prepare_send_data(&send_server_resp);
            APP_INFO1("BSA_BLE_SE_READ_EVT: send_server_resp.conn_id:%d, send_server_resp.trans_id:%d", send_server_resp.conn_id, send_server_resp.trans_id, send_server_resp.status);
            APP_INFO1("BSA_BLE_SE_READ_EVT: send_server_resp.status:%d,send_server_resp.auth_req:%d", send_server_resp.status,send_server_resp.auth_req);
            APP_INFO1("BSA_BLE_SE_READ_EVT: send_server_resp.handle:%d, send_server_resp.offset:%d, send_server_resp.len:%d", send_server_resp.handle,send_server_resp.offset,send_server_resp.len );
            BSA_BleSeSendRsp(&send_server_resp);
            break;
            
        case BSA_BLE_SE_OPEN_EVT:
            if (p_data->ser_open.reason != BSA_SUCCESS)
            {
                APP_ERROR1("Connect request from client failed status = %d", p_data->ser_open.reason);
                break;
            }
            printf("Connect request from client\n");
            break;
            
        case BSA_BLE_SE_CLOSE_EVT:
           printf("Disconnect request from client reason number = %d\n", p_data->ser_open.reason);
            current_server_index = ql_app_ble_cb.current_server;
            ql_app_ble_cb.ble_server[current_server_index].conn_id = p_data->ser_close.conn_id;
            ql_app_ble_cb.ble_server[current_server_index].server_if = p_data->ser_close.server_if;
            ql_bt_unlock_wakelock();
            break;
            
        case BSA_BLE_SE_CONFIRM_EVT:
            printf("Confirm event\n");
            break;
            
        case BSA_BLE_APCF_ENABLE_EVT:
            printf("APCF Enable/Disable\n");
            break;
            
        case BSA_BLE_APCF_CFG_EVT:
            printf("APCF Config\n");
            break;
            
        default:
            printf("Unsupport Event:%d\n", event);
            break;
    }
}

/*******************************************************************************
 **
 ** Function        ql_ble_server_register
 **
 ** Description     Register server app
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int ql_ble_server_register(UINT16 uuid, tBSA_BLE_CBACK *p_cback)
{
    tBSA_STATUS status;
    tBSA_BLE_SE_REGISTER ble_register_param;
    int server_num;

    server_num = ql_ble_server_find_free_server();
    if (server_num < 0)
    {
        APP_ERROR0("No more spaces!!");
        return -1;
    }

    status = BSA_BleSeAppRegisterInit(&ble_register_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeAppRegisterInit failed status = %d", status);
        return -1;
    }

    ble_register_param.uuid.len = 2;
    ble_register_param.uuid.uu.uuid16 = uuid;
    if (p_cback == NULL)
    {
        APP_ERROR0("Please Add Server Callback!!");
        ble_register_param.p_cback = NULL;
        return -1;
    }
    else
    {
        ble_register_param.p_cback = p_cback;
    }

    status = BSA_BleSeAppRegister(&ble_register_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeAppRegister failed status = %d", status);
        return -1;
    }

    ql_app_ble_cb.ble_server[server_num].enabled = TRUE;
    ql_app_ble_cb.ble_server[server_num].server_if = ble_register_param.server_if;
    ql_app_ble_cb.current_server = server_num;
    printf("enabled:%d, server_if:%d\n", ql_app_ble_cb.ble_server[server_num].enabled,
                    ql_app_ble_cb.ble_server[server_num].server_if);

    return 0;
}

/*******************************************************************************
 **
 ** Function        ql_ble_server_create_service
 **
 ** Description     create service
 **
 ** Parameters      Pointer on structure containing API parameters
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int ql_ble_server_create_service(tBSA_BLE_SE_CREATE *p_ble_create_param)
{
    tBSA_STATUS status;
    tBSA_BLE_SE_CREATE ble_create_param;
    int current_server_index = ql_app_ble_cb.current_server;
    int current_service_index = ql_app_ble_cb.ble_server[current_server_index].current_service;
    
    status = BSA_BleSeCreateServiceInit(&ble_create_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeCreateServiceInit failed status = %d", status);
        return -1;
    }

    ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].attr_UUID.len = ble_create_param.service_uuid.len;
    ble_create_param.service_uuid.len =  p_ble_create_param->service_uuid.len;
    if(LEN_UUID_16 == p_ble_create_param->service_uuid.len) {
        ble_create_param.service_uuid.uu.uuid16 = p_ble_create_param->service_uuid.uu.uuid16;
        ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].attr_UUID.uu.uuid16 = ble_create_param.service_uuid.uu.uuid16;
    }
    else if(LEN_UUID_32 == p_ble_create_param->service_uuid.len)
    {
        ble_create_param.service_uuid.uu.uuid32 = p_ble_create_param->service_uuid.uu.uuid32;
        ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].attr_UUID.uu.uuid32 = ble_create_param.service_uuid.uu.uuid32;
    } else if(LEN_UUID_128 == p_ble_create_param->service_uuid.len)
    {
        if(NULL == memcpy(&ble_create_param.service_uuid.uu.uuid128, &p_ble_create_param->service_uuid.uu.uuid128, 16))
        {
            printf("memcpy Failed\n");
            return -1;
        }

        if(NULL == memcpy(&ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].attr_UUID.uu.uuid128, &ble_create_param.service_uuid.uu.uuid128, 16))
        {
            printf("memcpy Failed\n");
            return -1;
        }
    }
    
    ble_create_param.server_if = p_ble_create_param->server_if;
    ble_create_param.num_handle = p_ble_create_param->num_handle;
    ble_create_param.is_primary = p_ble_create_param->is_primary;
    
    status = BSA_BleSeCreateService(&ble_create_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeCreateService failed status = %d", status);
        return -1;
    }
    
    ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].is_pri = ble_create_param.is_primary;

    return 0;
}

/*******************************************************************************
 **
 ** Function        ql_ble_server_start_service
 **
 ** Description     Start Service
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int ql_ble_server_start_service(tBSA_BLE_SE_START *p_ble_start_param)
{
    tBSA_STATUS status;
    tBSA_BLE_SE_START ble_start_param;

    status = BSA_BleSeStartServiceInit(&ble_start_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeStartServiceInit failed status = %d", status);
        return -1;
    }

    ble_start_param.service_id = p_ble_start_param->service_id;
    ble_start_param.sup_transport = p_ble_start_param->sup_transport;
    status = BSA_BleSeStartService(&ble_start_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeStartService failed status = %d", status);
        return -1;
    }
    return 0;
}

/*******************************************************************************
 **
 ** Function        ql_ble_server_add_char
 **
 ** Description     Add character to service
 **
 ** Parameters      Pointer on structure containing API parameters
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int ql_ble_server_add_char(tBSA_BLE_SE_ADDCHAR *p_ble_addchar_param)
{
    tBSA_STATUS status;
    tBSA_BLE_SE_ADDCHAR ble_addchar_param;

    status = BSA_BleSeAddCharInit(&ble_addchar_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeAddCharInit failed status = %d", status);
        return -1;
    }

    ble_addchar_param.service_id = p_ble_addchar_param->service_id;
    ble_addchar_param.is_descr = FALSE;
    
    ble_addchar_param.char_uuid.len = p_ble_addchar_param->char_uuid.len;
    if(LEN_UUID_16 == p_ble_addchar_param->char_uuid.len)
        ble_addchar_param.char_uuid.uu.uuid16 = p_ble_addchar_param->char_uuid.uu.uuid16;
    else if(LEN_UUID_32 == p_ble_addchar_param->char_uuid.len)
    {
        ble_addchar_param.char_uuid.uu.uuid32 = p_ble_addchar_param->char_uuid.uu.uuid32;
    } else if(LEN_UUID_128 == p_ble_addchar_param->char_uuid.len)
    {
        if(NULL == memcpy(&ble_addchar_param.char_uuid.uu.uuid128, &p_ble_addchar_param->char_uuid.uu.uuid128, 16))
        {
            printf("memcpy Failed\n");
            return -1;
        }
    }
        
    ble_addchar_param.perm = p_ble_addchar_param->perm;
    ble_addchar_param.property = p_ble_addchar_param->property;

    status = BSA_BleSeAddChar(&ble_addchar_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeAddChar failed status = %d", status);
        return -1;
    }
    return 0;
}

/*******************************************************************************
 **
 ** Function         ql_ble_server_find_free_server
 **
 ** Description      find free server for BLE server application
 **
 ** Parameters
 **
 ** Returns          positive number(include 0) if successful, error code otherwise
 **
 *******************************************************************************/
int ql_ble_server_find_free_server()
{
    int index;

    for (index = 1; index < BSA_BLE_SERVER_MAX; index++)
    {
        if (!ql_app_ble_cb.ble_server[index].enabled)
        {
            return index;
        }
    }
    return -1;
}

/*******************************************************************************
 **
 ** Function        ql_ble_server_add_char_desc
 **
 ** Description     Add character Descriptor to service
 **
 ** Parameters      Pointer on structure containing API parameters
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int ql_ble_server_add_char_desc(tBSA_BLE_SE_ADDCHAR *p_ble_addchar_param)
{
    tBSA_STATUS status;
    tBSA_BLE_SE_ADDCHAR ble_addchar_param;

    status = BSA_BleSeAddCharInit(&ble_addchar_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeAddCharInit failed status = %d", status);
        return -1;
    }

    ble_addchar_param.service_id = p_ble_addchar_param->service_id;
    ble_addchar_param.is_descr = TRUE;
    
    ble_addchar_param.char_uuid.len = p_ble_addchar_param->char_uuid.len;
    if(LEN_UUID_16 == p_ble_addchar_param->char_uuid.len)
        ble_addchar_param.char_uuid.uu.uuid16 = p_ble_addchar_param->char_uuid.uu.uuid16;
    else if(LEN_UUID_32 == p_ble_addchar_param->char_uuid.len)
    {
        ble_addchar_param.char_uuid.uu.uuid32 = p_ble_addchar_param->char_uuid.uu.uuid32;
    } else if(LEN_UUID_128 == p_ble_addchar_param->char_uuid.len)
    {
        if(NULL == memcpy(&ble_addchar_param.char_uuid.uu.uuid128, &p_ble_addchar_param->char_uuid.uu.uuid128, 16))
        {
            printf("memcpy Failed\n");
            return -1;
        }
    }
        
    ble_addchar_param.perm = p_ble_addchar_param->perm;
    ble_addchar_param.property = p_ble_addchar_param->property;

    status = BSA_BleSeAddChar(&ble_addchar_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeAddChar failed status = %d", status);
        return -1;
    }
    return 0;
}

