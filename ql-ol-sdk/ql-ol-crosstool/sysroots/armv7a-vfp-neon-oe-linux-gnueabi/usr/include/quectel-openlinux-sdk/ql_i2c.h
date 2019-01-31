/**  
  @file
  ql_i2c.h

  @brief
  This file provides the definitions for i2c, and declares the 
  API functions.

*/
/*============================================================================
  Copyright (c) 2017 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


WHEN        WHO            WHAT, WHERE, WHY
----------  ------------   ----------------------------------------------------
19/10/2018  Stanley.yong   Added an argument for Ql_I2C_Init(),
                           Added Ql_I2C_Deinit().
20/09/2016	Stanley.yong   Create
=============================================================================*/

#ifndef __QL_I2C_H__
#define __QL_I2C_H__


/*========================================================================
  FUNCTION:  Ql_I2C_Init
=========================================================================*/
/** @brief 
    Initialize the module I2C device, and return the I2C device FD.

    @param[in] dev_name, the i2c device name. developer may refer to the 
               development guidance document for the available device name.

    @return
    on success, a non-negative integer for I2C device FD is returned.
    on failure, the return value is -1;

    @dependencies
    none.
*/
/*=======================================================================*/
int Ql_I2C_Init(char *dev_name);

//------------------------------------------------------------------------------
/*
* Function:     Ql_I2C_Read
* 
* Description:
*               Read the specified length of data from the target address of 
*               the specified I2C slave device. 
*
* Parameters:
*               slaveAddr:
*                   slave address.
*
*               ofstAddr:
*                   The register address.
*
*               ptrBuff:
*                   A pointer to the buffer to store data. 
*
*               length:
*                   data lenght to read. 
* Return:        
*               RES_OK, this function succeeds.
*               or, failed to execute the function. 
*/
//------------------------------------------------------------------------------
int Ql_I2C_Read(int fd, unsigned short slaveAddr, unsigned char ofstAddr,  unsigned char* ptrBuff, unsigned short length);

//------------------------------------------------------------------------------
/*
* Function:     Ql_I2C_Write
* 
* Description:
*               Write the specified length of data to the target address of 
*               the specified I2C slave device. 
*
* Parameters:
*               slaveAddr:
*                   slave address.
*
*               ofstAddr:
*                   The register address.
*
*               ptrData:
*                   A pointer to the data buffer to write. 
*
*               length:
*                   data lenght to read. 
* Return:        
*               RES_OK, this function succeeds.
*               or, failed to execute the function. 
*/
//------------------------------------------------------------------------------
int Ql_I2C_Write(int fd, unsigned short slaveAddr, unsigned char ofstAddr,  unsigned char* ptrData, unsigned short length);

/* Perform the I/O control operation specified by REQUEST on FD.
   One argument may follow; its presence and type depend on REQUEST.
   Return value depends on REQUEST.  Usually -1 indicates error.  */
//extern int ioctl (int __fd, unsigned long int __request, ...) __THROW;

int Ql_I2C_Deinit(int fd);

#endif  //__QL_I2C_H__
