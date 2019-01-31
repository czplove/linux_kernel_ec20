/*
 * Performance counters software interface.
 *
 * $ Copyright Open Broadcom Corporation $
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id: bcmperf.h 514727 2014-11-12 03:02:48Z $
 */
/* essai */
#ifndef _BCMPERF_H_
#define _BCMPERF_H_
/* get cache hits and misses */
#define BCMPERF_ENABLE_INSTRCOUNT()
#define BCMPERF_ENABLE_ICACHE_MISS()
#define BCMPERF_ENABLE_ICACHE_HIT()
#define	BCMPERF_GETICACHE_MISS(x)	((x) = 0)
#define	BCMPERF_GETICACHE_HIT(x)	((x) = 0)
#define	BCMPERF_GETINSTRCOUNT(x)	((x) = 0)
#endif /* _BCMPERF_H_ */
