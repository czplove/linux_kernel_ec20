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
#ifndef QL_CM256SM_BLE_SREVER_H
#define QL_CM256SM_BLE_SREVER_H

#include "app_ble.h"
#include "app_thread.h"
#include "app_xml_utils.h"
#include "app_utils.h"
#include "app_mgt.h"
#include "app_disc.h"
#include "app_dm.h"
#include "app_manager.h"

/*******************************************************************************
**
** Function         ql_ble_server_profile_cback
**
** Description      ble test server profile callback.
**
** Returns          void
**
*******************************************************************************/
void ql_ble_server_profile_cback(tBSA_BLE_EVT event,  tBSA_BLE_MSG *p_data);

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
int ql_ble_server_register(UINT16 uuid, tBSA_BLE_CBACK *p_cback);

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
int ql_ble_server_create_service(tBSA_BLE_SE_CREATE *p_ble_create_param);

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
int ql_ble_server_start_service(tBSA_BLE_SE_START *p_ble_start_param);

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
int ql_ble_server_add_char(tBSA_BLE_SE_ADDCHAR *p_ble_addchar_param);

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
int ql_ble_server_find_free_server(void);

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
int ql_ble_server_add_char_desc(tBSA_BLE_SE_ADDCHAR *p_ble_addchar_param);

#endif

