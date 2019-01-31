#ifndef _QL_POWER_CALLBACK_
#define _QL_POWER_CALLBACK_
/*
*The unit of 'shutdown_time' is in microseconds.
*for example:shutdown_time=400000 (400ms)
*When the powerkey is pressed less than shutdown_time(short press),the callback function will get status=0,
*However if the powerkey is pressed more than shutdown_time(long press),the callback function will get status=1.
*
*/

typedef void (*ql_powerkey_cb)(int status);

int ql_powerkey_register_cb(unsigned int shutdown_time, ql_powerkey_cb callfun);

#endif
