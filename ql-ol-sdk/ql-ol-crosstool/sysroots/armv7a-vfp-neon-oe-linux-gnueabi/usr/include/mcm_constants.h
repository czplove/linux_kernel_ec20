//============================================================================
// FILE: mcm_constants.h
//
// SERVICES: MCM
//
// DESCRIPTION:
// IoE Mobile Connection Manager Event API (MCM)
//
// This file provides common event handling functions and data types
// that are not spcific to a particular MCM module such as Voice, Data,
// SIM, etc.
//
//  Copyright (c) 2013-2014 Qualcomm Technologies, Inc.  All Rights Reserved.
//  Qualcomm Technologies Proprietary and Confidential. 
//
//=============================================================================
#ifndef MCM_CONSTANTS_H
#define MCM_CONSTANTS_H

#include "qmi_client.h"
#include "qmi_idl_lib.h"
#include "qmi_csi.h"

#define qmi_util_log printf

#define NIL 0
#define NULL 0x0000L

#define TRUE 1
#define FALSE 0

#define MCM_MAX_CLIENTS 20

#define MCM_MAX_SERVICES 10

#define MCM_MAX_REQUESTS 1000

#define MCM_MAX_NUM_OF_ASYNC_CB 50

#define MCM_TOKEN_ID_INVALID     0

#define MCM_QMI_TIMEOUT (10000)

#define MCM_MAX_EONS_LENGTH (512)

#define MCM_MAX_NUM_OF_MSG_ID (6)

#define MCM_MAX_SRVS      (5)

#define MCM_RIL_SERVICE           0x401
#define MCM_DATA_SERVICE          0x404
#define MCM_SIM_SERVICE           0x402
#define MCM_MOBILEAP_SERVICE      0x406
#define MCM_LOC_SERVICE           0x405
#define MCM_ATCOP_SERVICE         0x403
#define MCM_SRV_MANAGER           0x407

#define MCM_RIL_DMS_SERVICE_MIN   0x0201
//Laurence.yin-2018/04/20-QCM9XOL00004C015-P01, <[DM] :
//increase MCM_RIL_DMS_SERVICE_MAX beause of MCM_DM_GET_DEVICE_SERIAL_NUMBERS_REQ_V01.>
#if defined (QL_DM_SERVICE_SURPORT)
#define MCM_RIL_DMS_SERVICE_MAX   0x02FF
#else
#define MCM_RIL_DMS_SERVICE_MAX   0x0206
#endif
#define MCM_RIL_NAS_SERVICE_MIN   0x0500
#define MCM_RIL_NAS_SERVICE_MAX   0x05FF

#define MCM_RIL_SMS_SERVCIE_MIN   0x0700
#define MCM_RIL_SMS_SERVICE_MAX   0x0709

#define MCM_RIL_VOICE_SERVICE_MIN 0x1000
#define MCM_RIL_VOICE_SERVICE_MAX 0x10FF

#define MCM_SIM_SERVICE_MIN       0x0B00
#define MCM_SIM_SERVICE_MAX       0x0B10

#define MCM_DATA_SERVICE_MIN      0x0100
#define MCM_DATA_SERVICE_MAX      0x010C

#define MCM_MOBILEAP_SERVICE_MIN  0x0400
#define MCM_MOBILEAP_SERVICE_MAX  0x0427

#define MCM_LOC_SERVICE_MIN       0x0300
#define MCM_LOC_SERVICE_MAX       0x0316

#define MCM_ATCOP_SERVICE_MIN     0x0600
#define MCM_ATCOP_SERVICE_MAX     0x0600

#define MCM_SRV_MANAGER_MIN    0x0800
#define MCM_SRV_MANAGER_MAX    0x0802


#endif // MCM_CONSTANTS_H
