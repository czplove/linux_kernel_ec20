/*
 * HND Trap handling.
 *
 * $ Copyright Open Broadcom Corporation $
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id: hnd_trap.h 514727 2014-11-12 03:02:48Z $
 */

#ifndef	_hnd_trap_h_
#define	_hnd_trap_h_


#if defined(__arm__) || defined(__thumb__) || defined(__thumb2__)
#include <hnd_armtrap.h>
#else
#error "unsupported CPU architecture"
#endif

#endif	/* _hnd_trap_h_ */
