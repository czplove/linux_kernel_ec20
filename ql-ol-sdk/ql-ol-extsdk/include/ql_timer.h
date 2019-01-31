
/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Quectel Co., Ltd. 2017
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ql_audio.h 
 *
 * Project:
 * --------
 *   OpenLinux
 *
 * Description:
 * ------------
 *   Timer API definition.
 *
 * Author:
 * -------
 * Jun.wu
 * -------
 *
 *============================================================================
 *             HISTORY
 *----------------------------------------------------------------------------
 * WHO                  WHEN              WHAT
 *----------------------------------------------------------------------------
 * Chris.peng               27/05/2017		  Create
 ****************************************************************************/
#ifndef QL_TIMER_H
#define QL_TIMER_H

/****************************************************************************
*  Timer ID Definition
***************************************************************************/
typedef enum
{
   QL_TIMER_ID_MIN = 0,
   QL_TIMER_ID_1 = QL_TIMER_ID_MIN,
   QL_TIMER_ID_2,
   QL_TIMER_ID_3,
   QL_TIMER_ID_4,
   QL_TIMER_ID_5,
   QL_TIMER_ID_6,
   QL_TIMER_ID_7,
   QL_TIMER_ID_8,
   QL_TIMER_ID_9,
   QL_TIMER_ID_10,
   QL_TIMER_ID_MAX
}ql_timer_id;

/****************************************************************************
*  Timer expire callback function Definition
***************************************************************************/
typedef int (*ql_timer_exp_cb)(ql_timer_id timer_id, void *cb_params);

/****************************************************************************
//
// Function: QL_Timer_Register
//
// Description:
//   register timer
//
// @timer_id:
//   Timer_ID will be registered
// @exp_cb_fcn:
//   callback function to call at timer expiry
// @cb_params:
//   parameters for exp_cb_fcn
// @param result:
//   RES_OK      success
//   other       fail
***************************************************************************/

int QL_Timer_Register(ql_timer_id timer_id, ql_timer_exp_cb  exp_cb_fcn, void *cb_params);

/****************************************************************************
//
// Function: QL_Timer_Start
//
// Description:
//   start timer
//
// @timer_id:
//   Timer ID will be start
// @interval:
//   timer in ms will be expired
// @auto_repeat:
//   0:  not periodic
//   1:  periodic
// @param result:
//   RES_OK      success
//   other       fail
***************************************************************************/

int QL_Timer_Start(ql_timer_id timer_id, int interval, int auto_repeat);

/****************************************************************************
//
// Function: QL_Timer_Stop
//
// Description:
//   stop timer
//
// @timer_id:
//   Timer ID will be stop
// @param result:
//   RES_OK      success
//   other       fail
***************************************************************************/

int QL_Timer_Stop(ql_timer_id timer_id);

#endif // QL_TIMER_H
