/*****************************************************************************
**
**  Name:           app_ble_test_client.h
**
**  Description:    Bluetooth BLE Test client include file
**
**  Copyright (c) 2016, Cypress Semiconductor., All Rights Reserved.
**  Proprietary and confidential.
**
*****************************************************************************/
#ifndef APP_BLE_TEST_CLIENT_H
#define APP_BLE_TEST_CLIENT_H

#include "bsa_api.h"
#include "app_ble.h"
#include "app_ble_client_db.h"

#define APP_BLE_TC_APP_UUID                 0x9991
#define APP_BLE_TC_NUM_OF_CLIENT            1


/*******************************************************************************
 **
 ** Function        app_ble_test_client_start
 **
 ** Description     start ble test client and connect to test server
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_client_start(void);

/*******************************************************************************
 **
 ** Function        app_ble_test_client_register_notification
 **
 ** Description     Register notification to receive noti/indication from test server
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_client_register_notification(int char_uuid);

/*******************************************************************************
 **
 ** Function        app_ble_test_client_write_enable
 **
 ** Description     write data to enable server notification/indication.
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_client_write_enable(int char_uuid);

/*******************************************************************************
 **
 ** Function        app_ble_test_client_write_disable
 **
 ** Description     write data to disable server notification/indication.
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_client_write_disable(int char_uuid);

/*******************************************************************************
 **
 ** Function        app_ble_test_client_stop
 **
 ** Description     stop test client
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_client_stop(void);


/*******************************************************************************
 **
 ** Function        app_ble_test_client_write_information
 **
 ** Description     Write information to ble test server
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_client_write_information(int char_uuid, UINT8 value);


/*******************************************************************************
 **
 ** Function        app_ble_test_client_write
 **
 ** Description     Write information to ble test server
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_client_write(int char_uuid);


/*******************************************************************************
 **
 ** Function        app_ble_test_client_write_prepare
 **
 ** Description     Prepare Write information to ble test server
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_client_write_prepare(int char_uuid);


/*******************************************************************************
 **
 ** Function        app_ble_test_client_write_execute
 **
 ** Description     Execute Write to ble test server
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_client_write_execute(int char_uuid);

/*******************************************************************************
 **
 ** Function        app_ble_test_client_close
 **
 ** Description     ble close connection
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_client_close(void);

/*******************************************************************************
 **
 ** Function        app_ble_test_client_deregister
 **
 ** Description     This is the ble deregister app
 **
 ** Parameters      None
 **
 ** Returns         status: 0 if success / -1 otherwise
 **
 *******************************************************************************/
int app_ble_test_client_deregister(void);


/*******************************************************************************
 **
 ** Function         app_ble_test_client_disc_cback
 **
 ** Description      Discovery callback
 **
 ** Returns          void
 **
 *******************************************************************************/
void app_ble_test_client_disc_cback(tBSA_DISC_EVT event, tBSA_DISC_MSG *p_data);

#endif
