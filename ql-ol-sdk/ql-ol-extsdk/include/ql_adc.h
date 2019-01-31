/**  
  @file
  ql_adc.h

  @brief
  This file provides the definitions for adc, and declares the 
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
27/06/2018  Quectel        Changed the parameter of ql_adc_show() from char-type
                           to Enum_QADC.
=============================================================================*/

#ifndef _QL_ADC_H_
#define _QL_ADC_H_

typedef enum {
    QADC_NONE = 0,
    ADC0 = 1,
    ADC1 = 2,
    CBC  = 3,
    QADC_END
}Enum_QADC;

/*========================================================================
  FUNCTION:  ql_adc_show
=========================================================================*/
/** @brief 
    This function retrieves the ADC voltage value with the specified 
    ADC channel.

    @param[in] qadc, adc channel definitions, one value of Enum_QADC.

    @return
    adc value(mv) on success
    on failure, the return value is -1;
*/
/*=======================================================================*/
int ql_adc_show(Enum_QADC qadc);

#endif
