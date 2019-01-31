/*****************************************************************************
**
**  Name:           app_ble_test_server.h
**
**  Description:    Bluetooth BLE test application include file
**
**  Copyright (c) 2016, Cypress Semiconductor., All Rights Reserved.
**  Proprietary and confidential.
**
*****************************************************************************/
#ifndef APP_BLE_TEST_SERVER_H
#define APP_BLE_TEST_SERVER_H

#include "bsa_api.h"
#include "app_ble.h"
#include "app_ble_client_db.h"
#include "app_thread.h"

/* UUID Definitions */
#define APP_BLE_TS_APP_UUID           0x9990
#define APP_BLE_TS_SERV_UUID          0xA108
#define APP_BLE_TS_CHAR_NOTI_UUID     0xA155  /* Characteristic for Notification */
#define APP_BLE_TS_CHAR_INDI_UUID     0xA156  /* Characteristic for Indication */
#define APP_BLE_TS_CHAR_UUID          0xA157  /* Characteristic for Write */
#define APP_BLE_TS_NUM_OF_HANDLES     10
#define APP_BLE_TS_NUM_OF_SERVER      1
#define APP_BLE_TS_ATTR_NUM           1

#define APP_BLE_TS_ADV_VALUE_LEN        22
#define APP_BLE_TS_SCAN_RSP_VALUE_LEN   6
#define APP_BLE_TS_NOTI_VALUE_LEN       10

#define APP_BLE_TS_ADV_VALUE       {0x12 ,0x09  ,0x51 ,0x75 ,0x65 ,0x63 ,0x74 ,0x65 ,0x6C ,0x5F ,0x4E ,0x46 ,0x33 ,0x33 ,0x30 ,0x33 ,0x5F ,0x42 ,0x54 ,0x02 ,0x01 ,0x1A};
#define APP_BLE_TS_SCAN_RSP_VALUE  {0x77, 0x11, 0x22, 0x33, 0x44, 0x55};
#define APP_BLE_TS_NOTI_VALUE \
            {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00};


#define APP_BLE_TS_CHAR_MAX   3

typedef struct
{
    UINT16 char_uuid;
    UINT16 attr_id;
} tAPP_BLE_TEST_CHAR;


typedef struct
{
    tAPP_THREAD  resp_thread;
    BOOLEAN      notification_send;
    BOOLEAN      indication_send;
    UINT16       conn_id;
    UINT16       attr_id;
    UINT8        counter;
    tAPP_BLE_TEST_CHAR  chars[APP_BLE_TS_CHAR_MAX];
} tAPP_BLE_TS_CB;


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
int app_ble_test_server_start(void);

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
int app_ble_test_server_create_service(void);

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
int app_ble_test_server_stop(void);

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
int app_ble_test_server_send_notification(void);

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
int app_ble_test_server_send_indication(void);

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
int app_ble_test_server_deregister(void);


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
int app_ble_test_server_start_service(void);

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
int app_ble_test_server_add_char(int char_uuid);

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
int app_ble_test_server_add_char_desc(int char_uuid);

/*******************************************************************************
**
** Function         app_ble_test_server_profile_cback
**
** Description      ble test server profile callback.
**
** Returns          void
**
*******************************************************************************/
void app_ble_test_server_profile_cback(tBSA_BLE_EVT event,  tBSA_BLE_MSG *p_data);


#endif
