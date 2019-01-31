/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Quectel Co., Ltd. 2016
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ql_eint.h 
 *
 * Project:
 * --------
 *   OpenLinux
 *
 * Description:
 * ------------
 *   External interrupt API definition.
 *
 * Author:
 * -------
 * Stanley YONG
 * -------
 *
 *============================================================================
 *             HISTORY
 *----------------------------------------------------------------------------
 * WHO            WHEN                WHAT
 *----------------------------------------------------------------------------
 * Stanley.YONG   26/07/2016		  Create
 ****************************************************************************/

#ifndef __QL_EINT_H__
#define __QL_EINT_H__

#include "ql_gpio.h"


//------------------------------------------------------------------------------
/**
 * The type of GPIO Edge Sensivity.
 */
//------------------------------------------------------------------------------
typedef enum {
    EINT_SENSE_NONE,       // pin is input, but no an interrupt pin.
    EINT_SENSE_RISING,
    EINT_SENSE_FALLING,
    EINT_SENSE_BOTH
}Enum_EintType;

//------------------------------------------------------------------------------
/*
* Description:
*     Definition for EINT callback function.
* 
* Parameters:
*     PinName:
*         EINT pin name, one value of Enum_PinName.
*
*     level:
*         The EINT level value, one value of Enum_PinLevel. 
*         0 or 1
*/
//------------------------------------------------------------------------------
typedef void (*Ql_EINT_Callback)(Enum_PinName eintPinName, int level);

//------------------------------------------------------------------------------
/*
* Function:     Ql_EINT_Enable 
* 
* Description:
*               Set the interrupt sense mode, and enable interrupt. 
*
* Parameters:
*               eintPinName:
*                   EINT pin name, one value of Enum_PinName that has 
*                   the interrupt function.
*
*               eintType:
*                   Interrupt type, level-triggered or edge-triggered.
*                   Now, only edge-triggered interrupt is supported.
*
*               eint_callback:
*                   call back function
*
* Return:        
*               RES_OK, this function succeeds.
*               else failed to execute the function. 
*/
//------------------------------------------------------------------------------
int Ql_EINT_Enable(Enum_PinName eintPinName, Enum_EintType eintType, Ql_EINT_Callback eint_callback);


//------------------------------------------------------------------------------
/*
* Function:     Ql_EINT_Disable 
* 
* Description:
*               Disable the interrupt sense. 
*
* Parameters:
*               eintPinName:
*                   EINT pin name, one value of Enum_PinName that has 
*                   the interrupt function.
*
* Return:        
*               RES_OK, this function succeeds.
*               else failed to execute the function. 
*/
//------------------------------------------------------------------------------
int Ql_EINT_Disable(Enum_PinName eintPinName);

#endif  // __QL_EINT_H__
