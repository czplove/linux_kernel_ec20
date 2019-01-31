/**  @file
  ql_ntp.h

  @brief
  This file provides an API to use ntpd to synchronization system time.

*/

#ifndef __QL_NTP_H__
#define __QL_NTP_H__

/****************************************************************************
 * Error Code Definition
 ***************************************************************************/
enum {
	RES_NTP_OK = 0,
	RES_NTP_BAD_PARAMETER = -1,       /*Parameter is invalid*/
	RES_NTP_NO_CONF_FILE = -2,        /*"/etc/ntp.conf" Server List file non-exist*/
	RES_NTP_NO_NTPD = -3,	          /*ntpd Binary non-exist*/
	RES_NTP_CREATE_TIMER_FAILED = -4, /*ntpd timer create failed*/
	RES_NTP_START_FAILED = -5,	  /*ntpd start failed*/
	RES_NTP_STOP_FAILED = -6,	  /*ntpd stop failed*/
	RES_NTP_RUNNING = -7,	          /*ntpd already running*/
};

/*========================================================================
  FUNCTION:  ql_start_ntpd
=========================================================================*/
/**
    @brief
        Start ntp Time synchronization.

    @param[in][out]
	<sec> <timer interval; uint:minutes>

    @dependencies
       None
*/
/*=========================================================================*/
int ql_start_ntpd(int minutes);

#endif //QL_NTP_H 
