
/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Quectel Co., Ltd. 2017
*
*****************************************************************************/
#ifndef QL_POSIX_TIMER_H
#define QL_POSIX_TIMER_H

#if 1
#include "signal.h"
#else
/*rock.wang 2018-06-20 add feature __sigval_t_defined for LE2.2 
 with gcc version 6.4 for compile error*/
#if (!defined(__have_sigval_t)&&!defined(__sigval_t_defined))
typedef union sigval
{
	int sival_int;
	void *sival_ptr;
}sigval_t;
#endif

#endif
/****************************************************************
*
* Function: ql_posix_timer_enable
*
* Description:
*	Enable posix timer despite of the system suspend or active, 
*	and the callback will be triggered when timer is expire
*
* Params:
*	callback: the callback function at timer expiry
*	cb_params: parameters to callback
*	interval: the interval of timer (in msec)
*	peridic: peridic or not (0: just trigger once; 1:peridic timer)
*
* Result:
* 	success: timer_id
*	fail: NULL
*****************************************************************/
timer_t ql_posix_timer_enable(void *callback, sigval_t cb_params, int interval, int peridic);

/****************************************************************
*
* Function: ql_posix_timer_disable
*
* Description:
*	Disable the posix timer.
*
* Params:
*	timer_id: a timer id returned by ql_posix_timer_enable
*
* Result:
* 	success: 0
*	fail: -1
*****************************************************************/
int ql_posix_timer_disable(timer_t timer_id);

#endif /*QL_POSIX_TIMER_H */
