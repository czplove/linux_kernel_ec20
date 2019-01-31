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
#include "app_ble_client.h"
#include "app_ble_server.h"
#include "app_ble_client_db.h"
#include "app_ble_test_server.h"
#include "app_ble_test_client.h"
#include "app_manager.h"

#include "ql_cm256sm_ble_server.h"

#ifndef APP_BLE_TS_MITM
#define APP_BLE_TS_CHAR_PERM    BSA_GATT_PERM_READ|\
                                BSA_GATT_PERM_WRITE;
#define APP_BLE_TS_CHAR_PROP    BSA_GATT_CHAR_PROP_BIT_NOTIFY |\
                                BSA_GATT_CHAR_PROP_BIT_WRITE |\
                                BSA_GATT_CHAR_PROP_BIT_INDICATE;
#else
#define APP_BLE_TS_CHAR_PERM    BSA_GATT_PERM_READ_ENC_MITM|\
                                BSA_GATT_PERM_WRITE_SIGNED_MITM;
#define APP_BLE_TS_CHAR_PROP    BSA_GATT_CHAR_PROP_BIT_NOTIFY |\
                                BSA_GATT_CHAR_PROP_BIT_WRITE |\
                                BSA_GATT_CHAR_PROP_BIT_INDICATE |\
                                BSA_GATT_CHAR_PROP_BIT_AUTH;
#endif


/*
 * Global Variables
 */
tAPP_BLE_TS_CB app_ble_ts_cb;
static UINT8 app_ble_test_server_notification_value[APP_BLE_TS_NOTI_VALUE_LEN] =
        APP_BLE_TS_NOTI_VALUE;

/*
 * Local functions
 */
static void app_ble_test_server_thread(void);

extern tAPP_BLE_CB ql_app_ble_cb;

/*
 * BLE common functions
 */

/*******************************************************************************
 **
 ** Function        app_ble_test_server_start
 **
 ** Description     start test ble server
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_server_start(void)
{
     tBSA_BLE_SE_ADDCHAR ble_addchar_param;
     tBSA_BLE_SE_CREATE ble_create_param;
     tBSA_BLE_SE_START ble_start_param;
    int status = 0;
    int current_server_index = -1;
    int current_service_index = -1;

    /* clear control block */
    memset(&ql_app_ble_cb, 0, sizeof(ql_app_ble_cb));

    /*Register BLE Server*/
    ql_ble_server_register(APP_BLE_TS_APP_UUID, ql_ble_server_profile_cback);
    GKI_delay(1000);

    /*Create GATT Service*/
    current_server_index = ql_app_ble_cb.current_server;
    ble_create_param.server_if =  ql_app_ble_cb.ble_server[current_server_index].server_if;
    ble_create_param.num_handle = 40;
    ble_create_param.is_primary = TRUE;
    ble_create_param.service_uuid.len = 0x2;
    ble_create_param.service_uuid.uu.uuid16 = UUID_SERVCLASS_DEVICE_INFO; 
    ql_ble_server_create_service(&ble_create_param);
    GKI_delay(1000);

     /* Add a characteristic */
    current_server_index = ql_app_ble_cb.current_server;
    current_service_index = ql_app_ble_cb.ble_server[current_server_index].current_service;
    ble_addchar_param.service_id = ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].service_id;
    ble_addchar_param.is_descr = FALSE;
    ble_addchar_param.char_uuid.len = 2;
    ble_addchar_param.char_uuid.uu.uuid16 = GATT_UUID_FW_VERSION_STR;
    ble_addchar_param.perm = BSA_GATT_PERM_READ | BSA_GATT_PERM_WRITE;
    ble_addchar_param.property = BSA_GATT_CHAR_PROP_BIT_WRITE | BSA_GATT_CHAR_PROP_BIT_READ;
    ql_ble_server_add_char(&ble_addchar_param);
    GKI_delay(1000);

    /* Add a characteristic */
    current_server_index = ql_app_ble_cb.current_server;
    current_service_index = ql_app_ble_cb.ble_server[current_server_index].current_service;
    ble_addchar_param.service_id = ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].service_id;
    ble_addchar_param.is_descr = FALSE;
    ble_addchar_param.char_uuid.len = 2;
    ble_addchar_param.char_uuid.uu.uuid16 = GATT_UUID_MANU_NAME;
    ble_addchar_param.perm = BSA_GATT_PERM_READ | BSA_GATT_PERM_WRITE;
    ble_addchar_param.property = BSA_GATT_CHAR_PROP_BIT_WRITE | BSA_GATT_CHAR_PROP_BIT_READ;
    ql_ble_server_add_char(&ble_addchar_param);
    GKI_delay(1000);

    /*Start BLE Service*/
    current_service_index = ql_app_ble_cb.ble_server[current_server_index].current_service;
    ble_start_param.service_id = ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].service_id;
    ble_start_param.sup_transport = BTA_GATT_TRANSPORT_LE_BR_EDR;
    ql_ble_server_start_service(&ble_start_param);
    GKI_delay(1000);

    /*Create GATT Service*/
    current_server_index = ql_app_ble_cb.current_server;
    ble_create_param.server_if =  ql_app_ble_cb.ble_server[current_server_index].server_if;
    ble_create_param.num_handle = 40;
    ble_create_param.is_primary = TRUE;
    ble_create_param.service_uuid.len = 0x2;
    ble_create_param.service_uuid.uu.uuid16 = 0x6688; 
    ql_ble_server_create_service(&ble_create_param);
    GKI_delay(1000);

      /* Add a characteristic */
    current_server_index = ql_app_ble_cb.current_server;
    current_service_index = ql_app_ble_cb.ble_server[current_server_index].current_service;
    ble_addchar_param.service_id = ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].service_id;
    ble_addchar_param.is_descr = FALSE;
    ble_addchar_param.char_uuid.len = 2;
    ble_addchar_param.char_uuid.uu.uuid16 = GATT_UUID_MODEL_NUMBER_STR;
    ble_addchar_param.perm = BSA_GATT_PERM_READ | BSA_GATT_PERM_WRITE;
    ble_addchar_param.property = BSA_GATT_CHAR_PROP_BIT_WRITE | BSA_GATT_CHAR_PROP_BIT_READ;
    ql_ble_server_add_char(&ble_addchar_param);
    GKI_delay(1000);

    /* Add a characteristic */
    current_server_index = ql_app_ble_cb.current_server;
    current_service_index = ql_app_ble_cb.ble_server[current_server_index].current_service;
    ble_addchar_param.service_id = ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].service_id;
    ble_addchar_param.is_descr = FALSE;
    ble_addchar_param.char_uuid.len = 2;
    ble_addchar_param.char_uuid.uu.uuid16 = GATT_UUID_SYSTEM_ID;
    ble_addchar_param.perm = BSA_GATT_PERM_READ | BSA_GATT_PERM_WRITE;
    ble_addchar_param.property = BSA_GATT_CHAR_PROP_BIT_WRITE | BSA_GATT_CHAR_PROP_BIT_READ;
    ql_ble_server_add_char(&ble_addchar_param);
    GKI_delay(1000);

     /*Start BLE Service*/
    current_service_index = ql_app_ble_cb.ble_server[current_server_index].current_service;
    ble_start_param.service_id = ql_app_ble_cb.ble_server[current_server_index].attr[current_service_index].service_id;
    ble_start_param.sup_transport = BTA_GATT_TRANSPORT_LE_BR_EDR;
    ql_ble_server_start_service(&ble_start_param);
    GKI_delay(1000);

    /* make discoverable & connectable */
    app_dm_set_ble_visibility(TRUE, TRUE);

    /* make BR/EDR non-discoverable & non-connectable */
    app_dm_set_visibility(FALSE, FALSE);

    /* create thread to send notification */
    status = app_create_thread(app_ble_test_server_thread, 0, 0, &app_ble_ts_cb.resp_thread);
    if (status < 0)
    {
        APP_ERROR1("app_create_thread failed: %d", status);
        return -1;
    }
    return 0;
}

/*******************************************************************************
 **
 ** Function        app_ble_test_server_create_service
 **
 ** Description     create service
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_server_create_service(void)
{
    tBSA_STATUS status;
    tBSA_BLE_SE_CREATE ble_create_param;

    status = BSA_BleSeCreateServiceInit(&ble_create_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeCreateServiceInit failed status = %d", status);
        return -1;
    }

    ble_create_param.service_uuid.uu.uuid16 = APP_BLE_TS_SERV_UUID;
    ble_create_param.service_uuid.len = 2;
    ble_create_param.server_if = app_ble_cb.ble_server[APP_BLE_TS_NUM_OF_SERVER-1].server_if;
    ble_create_param.num_handle = APP_BLE_TS_NUM_OF_HANDLES;
    ble_create_param.is_primary = TRUE;

    app_ble_cb.ble_server[APP_BLE_TS_NUM_OF_SERVER-1].attr[APP_BLE_TS_ATTR_NUM-1].wait_flag = TRUE;

    status = BSA_BleSeCreateService(&ble_create_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeCreateService failed status = %d", status);
        app_ble_cb.ble_server[APP_BLE_TS_NUM_OF_SERVER-1].attr[APP_BLE_TS_ATTR_NUM-1].wait_flag = FALSE;
        return -1;
    }

    /* store information on control block */
    app_ble_cb.ble_server[APP_BLE_TS_NUM_OF_SERVER-1].attr[APP_BLE_TS_ATTR_NUM-1].attr_UUID.len = 2;
    app_ble_cb.ble_server[APP_BLE_TS_NUM_OF_SERVER-1].attr[APP_BLE_TS_ATTR_NUM-1].attr_UUID.uu.uuid16 = APP_BLE_TS_SERV_UUID;
    app_ble_cb.ble_server[APP_BLE_TS_NUM_OF_SERVER-1].attr[APP_BLE_TS_ATTR_NUM-1].is_pri = ble_create_param.is_primary;

    return 0;
}

/*******************************************************************************
 **
 ** Function           app_ble_test_server_thread
 **
 ** Description
 **
 ** Returns            nothing
 **
 *******************************************************************************/
static void app_ble_test_server_thread(void)
{

    while(1)
    {
        //APP_DEBUG0("running ");

        if (app_ble_ts_cb.notification_send == TRUE)
        {
            APP_DEBUG0("Notification ready! Send notification! ");
            /* send notification */
            app_ble_test_server_send_notification();
            /* need to co work with WiFi team here */
        }
        GKI_delay(2000); /* 2 sec delay */
        if (app_ble_ts_cb.indication_send == TRUE)
        {
            APP_DEBUG0("Indication ready! Send indication! ");
            /* send indication */
            app_ble_test_server_send_indication();
        }
        GKI_delay(2000); /* 2 sec delay */
    }
    APP_DEBUG0("Exit");
    pthread_exit(NULL);
}



/*******************************************************************************
 **
 ** Function        app_ble_test_server_stop
 **
 ** Description     stop test server
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_server_stop(void)
{
    /* make undiscoverable & unconnectable */
    app_dm_set_ble_visibility(FALSE, FALSE);

    app_ble_test_server_deregister();

    return 0;
}

/*******************************************************************************
 **
 ** Function        app_ble_test_server_send_notification
 **
 ** Description     Send notification to test client
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_server_send_notification(void)
{
    tBSA_STATUS status;
    tBSA_BLE_SE_SENDIND ble_sendind_param;
    int num;

    num = 0;
    if (app_ble_cb.ble_server[num].enabled != TRUE)
    {
        APP_ERROR1("Server was not registered! = %d", num);
        return -1;
    }

    status = BSA_BleSeSendIndInit(&ble_sendind_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeSendIndInit failed status = %d", status);
        return -1;
    }

    ble_sendind_param.conn_id = app_ble_ts_cb.conn_id;
    ble_sendind_param.attr_id = app_ble_ts_cb.attr_id;
    ble_sendind_param.data_len = APP_BLE_TS_NOTI_VALUE_LEN;
    memcpy(ble_sendind_param.value, app_ble_test_server_notification_value,
            APP_BLE_TS_NOTI_VALUE_LEN);
    ble_sendind_param.need_confirm = FALSE;

    status = BSA_BleSeSendInd(&ble_sendind_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeSendInd failed status = %d", status);
        return -1;
    }

    return 0;
}

/*******************************************************************************
 **
 ** Function        app_ble_test_server_send_indication
 **
 ** Description     Send indication to ble client
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_server_send_indication(void)
{
    tBSA_STATUS status;
    tBSA_BLE_SE_SENDIND ble_sendind_param;
    int num;

    num = 0;
    if (app_ble_cb.ble_server[num].enabled != TRUE)
    {
        APP_ERROR1("Server was not registered! = %d", num);
        return -1;
    }

    status = BSA_BleSeSendIndInit(&ble_sendind_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeSendIndInit failed status = %d", status);
        return -1;
    }

    ble_sendind_param.conn_id = app_ble_ts_cb.conn_id;
    ble_sendind_param.attr_id = app_ble_ts_cb.attr_id;
    ble_sendind_param.data_len = APP_BLE_TS_NOTI_VALUE_LEN;
    memcpy(ble_sendind_param.value, app_ble_test_server_notification_value,
            APP_BLE_TS_NOTI_VALUE_LEN);
    ble_sendind_param.need_confirm = TRUE;

    status = BSA_BleSeSendInd(&ble_sendind_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeSendInd failed status = %d", status);
        return -1;
    }

    return 0;
}

/*******************************************************************************
 **
 ** Function        app_ble_test_server_deregister
 **
 ** Description     Deregister server app
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_server_deregister(void)
{
    tBSA_STATUS status;
    tBSA_BLE_SE_DEREGISTER ble_deregister_param;

    APP_INFO0("Bluetooth BLE deregister menu:");
    if (app_ble_cb.ble_server[APP_BLE_TS_NUM_OF_SERVER-1].enabled != TRUE)
    {
        APP_ERROR1("Server was not registered! = %d", APP_BLE_TS_NUM_OF_SERVER-1);
        return -1;
    }

    status = BSA_BleSeAppDeregisterInit(&ble_deregister_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeAppDeregisterInit failed status = %d", status);
        return -1;
    }

    ble_deregister_param.server_if = app_ble_cb.ble_server[APP_BLE_TS_NUM_OF_SERVER-1].server_if;

    status = BSA_BleSeAppDeregister(&ble_deregister_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeAppDeregister failed status = %d", status);
        return -1;
    }

    return 0;
}


/*******************************************************************************
 **
 ** Function        app_ble_test_server_start_service
 **
 ** Description     Start Service
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_server_start_service(void)
{
    tBSA_STATUS status;
    tBSA_BLE_SE_START ble_start_param;

    status = BSA_BleSeStartServiceInit(&ble_start_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeStartServiceInit failed status = %d", status);
        return -1;
    }

    ble_start_param.service_id = app_ble_cb.ble_server[APP_BLE_TS_NUM_OF_SERVER-1].attr[APP_BLE_TS_ATTR_NUM-1].service_id;
    ble_start_param.sup_transport = BSA_BLE_GATT_TRANSPORT_LE_BR_EDR;

    APP_INFO1("service_id:%d, num:%d", ble_start_param.service_id, APP_BLE_TS_NUM_OF_SERVER-1);

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
 ** Function        app_ble_test_server_add_char
 **
 ** Description     Add character to service
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_server_add_char(int char_uuid)
{
    tBSA_STATUS status;
    tBSA_BLE_SE_ADDCHAR ble_addchar_param;

    status = BSA_BleSeAddCharInit(&ble_addchar_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeAddCharInit failed status = %d", status);
        return -1;
    }

    ble_addchar_param.service_id = app_ble_cb.ble_server[APP_BLE_TS_NUM_OF_SERVER-1].attr[APP_BLE_TS_ATTR_NUM-1].service_id;
    ble_addchar_param.is_descr = FALSE;
    ble_addchar_param.char_uuid.len = 2;
    ble_addchar_param.char_uuid.uu.uuid16 = char_uuid;
    ble_addchar_param.perm = APP_BLE_TS_CHAR_PERM;
    ble_addchar_param.property = APP_BLE_TS_CHAR_PROP;

    APP_INFO1("app_ble_wifi_add_char service_id:%d", ble_addchar_param.service_id);

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
 ** Function        app_ble_test_server_add_char_desc
 **
 ** Description     Add character to service
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_server_add_char_desc(int char_uuid)
{
    tBSA_STATUS status;
    tBSA_BLE_SE_ADDCHAR ble_addchar_param;

    status = BSA_BleSeAddCharInit(&ble_addchar_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeAddCharInit failed status = %d", status);
        return -1;
    }

    ble_addchar_param.service_id = app_ble_cb.ble_server[APP_BLE_TS_NUM_OF_SERVER-1].attr[APP_BLE_TS_ATTR_NUM-1].service_id;
    ble_addchar_param.is_descr = TRUE;
    ble_addchar_param.char_uuid.len = 2;
    ble_addchar_param.char_uuid.uu.uuid16 = char_uuid;
    ble_addchar_param.perm = APP_BLE_TS_CHAR_PERM;
    ble_addchar_param.property = APP_BLE_TS_CHAR_PROP;

    APP_INFO1("app_ble_wifi_add_char service_id:%d", ble_addchar_param.service_id);

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
** Function         app_ble_test_server_profile_cback
**
** Description      ble test server profile callback.
**
** Returns          void
**
*******************************************************************************/
void app_ble_test_server_profile_cback(tBSA_BLE_EVT event,  tBSA_BLE_MSG *p_data)
{
    int num, attr_index;
    tBSA_BLE_SE_SENDRSP send_server_resp;

    APP_DEBUG1("event = %d ", event);

    switch (event)
    {
    case BSA_BLE_SE_DEREGISTER_EVT:
        APP_INFO1("BSA_BLE_SE_DEREGISTER_EVT server_if:%d status:%d",
            p_data->ser_deregister.server_if, p_data->ser_deregister.status);
        num = app_ble_server_find_index_by_interface(p_data->ser_deregister.server_if);
        if (num < 0)
        {
            APP_ERROR0("no deregister pending!!");
            break;
        }

        app_ble_cb.ble_server[num].server_if = BSA_BLE_INVALID_IF;
        app_ble_cb.ble_server[num].enabled = FALSE;
        break;

    case BSA_BLE_SE_CREATE_EVT:
        APP_INFO1("BSA_BLE_SE_CREATE_EVT server_if:%d status:%d service_id:%d",
            p_data->ser_create.server_if, p_data->ser_create.status, p_data->ser_create.service_id);

        num = app_ble_server_find_index_by_interface(p_data->ser_create.server_if);

        /* search interface number */
        if(num < 0)
        {
            APP_ERROR0("no interface!!");
            break;
        }

        /* search attribute number */
        for (attr_index = 0 ; attr_index < BSA_BLE_ATTRIBUTE_MAX ; attr_index++)
        {
            if (app_ble_cb.ble_server[num].attr[attr_index].wait_flag == TRUE)
            {
                APP_INFO1("BSA_BLE_SE_CREATE_EVT if_num:%d, attr_num:%d", num, attr_index);
                if (p_data->ser_create.status == BSA_SUCCESS)
                {
                    app_ble_cb.ble_server[num].attr[attr_index].service_id = p_data->ser_create.service_id;
                    app_ble_cb.ble_server[num].attr[attr_index].wait_flag = FALSE;
                    break;
                }
                else  /* if CREATE fail */
                {
                    memset(&app_ble_cb.ble_server[num].attr[attr_index], 0, sizeof(tAPP_BLE_SERVICE));
                    break;
                }
            }
        }
        if (attr_index >= BSA_BLE_ATTRIBUTE_MAX)
        {
            APP_ERROR0("BSA_BLE_SE_CREATE_EVT no waiting!!");
            break;
        }
        break;

    case BSA_BLE_SE_ADDCHAR_EVT:
        APP_INFO1("BSA_BLE_SE_ADDCHAR_EVT status:%d", p_data->ser_addchar.status);
        if (p_data->ser_addchar.status == BSA_SUCCESS)
        {
            app_ble_ts_cb.attr_id = p_data->ser_addchar.attr_id;
            APP_INFO1("attr_id:0x%x", p_data->ser_addchar.attr_id);
            app_ble_ts_cb.chars[app_ble_ts_cb.counter].attr_id = p_data->ser_addchar.attr_id;
            app_ble_ts_cb.counter++;
        }
        break;

    case BSA_BLE_SE_START_EVT:
        APP_INFO1("BSA_BLE_SE_START_EVT status:%d", p_data->ser_start.status);
        break;

    case BSA_BLE_SE_READ_EVT:
        APP_INFO1("BSA_BLE_SE_READ_EVT status:%d", p_data->ser_read.status);
        printf("[Quinn]BSA_BLE_SE_READ_EVT status:%d", p_data->ser_read.status);
        BSA_BleSeSendRspInit(&send_server_resp);
        send_server_resp.conn_id = p_data->ser_read.conn_id;
        send_server_resp.trans_id = p_data->ser_read.trans_id;
        send_server_resp.status = p_data->ser_read.status;
        send_server_resp.handle = p_data->ser_read.handle;
        strncpy((char *)send_server_resp.value, "Quectel Test", 9);
        send_server_resp.len = 10;
        APP_INFO1("BSA_BLE_SE_READ_EVT: send_server_resp.conn_id:%d, send_server_resp.trans_id:%d", send_server_resp.conn_id, send_server_resp.trans_id, send_server_resp.status);
        APP_INFO1("BSA_BLE_SE_READ_EVT: send_server_resp.status:%d,send_server_resp.auth_req:%d", send_server_resp.status,send_server_resp.auth_req);
        APP_INFO1("BSA_BLE_SE_READ_EVT: send_server_resp.handle:%d, send_server_resp.offset:%d, send_server_resp.len:%d", send_server_resp.handle,send_server_resp.offset,send_server_resp.len );
        BSA_BleSeSendRsp(&send_server_resp);
        break;

    case BSA_BLE_SE_WRITE_EVT:
        APP_INFO1("BSA_BLE_SE_WRITE_EVT status:%d", p_data->ser_write.status);
        APP_DUMP("Write value", p_data->ser_write.value, p_data->ser_write.len);
        APP_INFO1("BSA_BLE_SE_WRITE_EVT trans_id:%d, conn_id:%d, handle:%d",
            p_data->ser_write.trans_id, p_data->ser_write.conn_id,
            p_data->ser_write.handle);

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
        APP_INFO1("BSA_BLE_SE_EXEC_WRITE_EVT status:%d", p_data->ser_exec_write.status);
        APP_INFO1("BSA_BLE_SE_EXEC_WRITE_EVT trans_id:%d, conn_id:%d",
            p_data->ser_exec_write.trans_id, p_data->ser_exec_write.conn_id);

        BSA_BleSeSendRspInit(&send_server_resp);
        send_server_resp.conn_id = p_data->ser_exec_write.conn_id;
        send_server_resp.trans_id = p_data->ser_exec_write.trans_id;
        send_server_resp.status = p_data->ser_exec_write.status;
        send_server_resp.handle = p_data->ser_exec_write.handle;
        send_server_resp.len = 0;
        APP_INFO1("conn_id:%d, trans_id:%d",
            send_server_resp.conn_id, send_server_resp.trans_id);
        APP_INFO1("status:%d, auth_req:%d",
            send_server_resp.status,send_server_resp.auth_req);
        APP_INFO1("handle:%d, offset:%d, len:%d",
            send_server_resp.handle,send_server_resp.offset,send_server_resp.len );
        BSA_BleSeSendRsp(&send_server_resp);
        break;

    case BSA_BLE_SE_OPEN_EVT:
        APP_INFO1("BSA_BLE_SE_OPEN_EVT status:%d", p_data->ser_open.reason);
        if (p_data->ser_open.reason == BSA_SUCCESS)
        {
            app_ble_ts_cb.conn_id = p_data->ser_open.conn_id;
            APP_INFO1("conn_id:0x%x", p_data->ser_open.conn_id);
        }
        break;

    case BSA_BLE_SE_CLOSE_EVT:
        APP_INFO1("BSA_BLE_SE_CLOSE_EVT reason:%d", p_data->ser_close.reason);
        app_ble_ts_cb.notification_send = FALSE;
        break;

    case BSA_BLE_SE_CONFIRM_EVT:
        APP_INFO0("BSA_BLE_SE_CONFIRM_EVT");
        break;


    default:
        break;
    }
}


/*******************************************************************************
 **
 ** Function        app_ble_add_system_id_char
 **
 ** Description     Add character to service
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_add_system_id_char(void)
{
    tBSA_STATUS status;
    tBSA_BLE_SE_ADDCHAR ble_addchar_param;

    status = BSA_BleSeAddCharInit(&ble_addchar_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeAddCharInit failed status = %d", status);
        return -1;
    }

    ble_addchar_param.service_id = app_ble_cb.ble_server[APP_BLE_TS_NUM_OF_SERVER-1].attr[APP_BLE_TS_ATTR_NUM-1].service_id;
    ble_addchar_param.is_descr = FALSE;
    ble_addchar_param.char_uuid.len = 2;
    ble_addchar_param.char_uuid.uu.uuid16 = GATT_UUID_SYSTEM_ID;
    ble_addchar_param.perm = BSA_GATT_PERM_READ | BSA_GATT_PERM_WRITE;
    ble_addchar_param.property = BSA_GATT_CHAR_PROP_BIT_WRITE | BSA_GATT_CHAR_PROP_BIT_READ;

    APP_INFO1("app_ble_add_system_id_char service_id:%d", ble_addchar_param.service_id);

    status = BSA_BleSeAddChar(&ble_addchar_param);
    if (status != BSA_SUCCESS)
    {
        APP_ERROR1("BSA_BleSeAddChar failed status = %d", status);
        return -1;
    }
    return 0;
}

