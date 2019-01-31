/*
 * Trace log blocks sent over HBUS
 *
 * $ Copyright Open Broadcom Corporation $
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id: logtrace.h 333856 2012-05-17 23:43:07Z $
 */

#ifndef	_LOGTRACE_H
#define	_LOGTRACE_H

#include <msgtrace.h>
#include <osl_decl.h>
extern void logtrace_start(void);
extern void logtrace_stop(void);
extern int logtrace_sent(void);
extern void logtrace_trigger(void);
extern int logtrace_init(osl_t* osh, void *hdl1, void *hdl2, msgtrace_func_send_t func_send);
extern void logtrace_deinit(osl_t* osh);
extern bool logtrace_event_enabled(void);

#endif	/* _LOGTRACE_H */
