
/*********************************************************************************************
 *
 *                  QUECTEL - Build a Smarter World.
 *
 * Copyright Statement:
 * --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Quectel Co., Ltd. 2018
 *
 * Filename:
 * ---------
 *   example_sms.h
 *
 * Project:
 * --------
 *   MDM9x07 - OpenLinux
 *
 * Description:
 * ------------
 *   This example demonstrates how to use SMS function with APIs in OpenLinux.
 *
 *============================================================================
 * HISTORY 
 * ---------
 *   2017/12/27   Vicent GAO    Create this file by QCM9XOL00002C001-P01
 * 
 **********************************************************************************************/

#ifndef __EXAMPLE_SMS_H__
#define __EXAMPLE_SMS_H__

/////////////////////////////////////////////////////////////
// MACRO CONSTANT DEFINITIONS
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// ENUM TYPE DEFINITIONS
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// STRUCT TYPE DEFINITIONS
/////////////////////////////////////////////////////////////
typedef struct
{
    int  item;
    char *name;
} st_exsms_item;

/////////////////////////////////////////////////////////////
// GLOBAL DATA DECLARATIONS
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// QCERTIOPTION DECLARATIONS
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// MACRO QCERTIOPTION DEFINITIONS
/////////////////////////////////////////////////////////////
#define SMS_TRACE(f,...) printf("Enter %s(%d)," f "\r\n",__FUNCTION__,__LINE__,##__VA_ARGS__)

#endif  // #ifndef __EXAMPLE_SMS_H__

