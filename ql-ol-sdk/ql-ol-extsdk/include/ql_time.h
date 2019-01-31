/**  @file
  ql_time.h

  @brief
  This file provides an API to set system time.

*/

#ifndef __QL_TIME_H__
#define __QL_TIME_H__

#ifdef __cplusplus
extern "C"{
#endif

/*========================================================================
  FUNCTION:  <ql_settimeofday>
=========================================================================*/
/** @brief
   <Set system time>
 
    @param[in][out]
	<tv> <time param>
	struct timeval {
	    time_t      tv_sec;     //seconds
            suseconds_t tv_usec;    //microseconds
        };
   
    @return
       0--Set system time Success.
       -1--Set system time Failed.
*/
/*=======================================================================*/
int ql_settimeofday(const struct timeval *tv);

#ifdef __cplusplus
}
#endif
#endif  /*__QL_TIME_H__*/
