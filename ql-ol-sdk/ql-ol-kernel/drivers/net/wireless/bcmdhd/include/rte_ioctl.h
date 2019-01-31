/*
 * HND Run Time Environment ioctl.
 *
 * $ Copyright Open Broadcom Corporation $
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id: rte_ioctl.h 514727 2014-11-12 03:02:48Z $
 */

#ifndef _rte_ioctl_h_
#define _rte_ioctl_h_


#define RTEGHWADDR		0x8901
#define RTESHWADDR		0x8902
#define RTEGMTU			0x8903
#define RTEGSTATS		0x8904
#define RTEGALLMULTI		0x8905
#define RTESALLMULTI		0x8906
#define RTEGPROMISC		0x8907
#define RTESPROMISC		0x8908
#define RTESMULTILIST	0x8909
#define RTEGUP			0x890A
#define RTEGPERMADDR		0x890B
#define RTEDEVPWRSTCHG		0x890C	
#define RTEDEVPMETOGGLE		0x890D	

#define RTE_IOCTL_QUERY			0x00
#define RTE_IOCTL_SET			0x01
#define RTE_IOCTL_OVL_IDX_MASK	0x1e
#define RTE_IOCTL_OVL_RSV		0x20
#define RTE_IOCTL_OVL			0x40
#define RTE_IOCTL_OVL_IDX_SHIFT	1

enum hnd_ioctl_cmd {
	HND_RTE_DNGL_IS_SS = 1, 

	
	BUS_GET_VAR = 2,
	BUS_SET_VAR = 3,
	BUS_FLUSH_RXREORDER_Q = 4,
	BUS_SET_LTR_STATE = 5,
	BUS_FLUSH_CHAINED_PKTS = 6,
	BUS_SET_COPY_COUNT = 7
};

#define SDPCMDEV_SET_MAXTXPKTGLOM	1

typedef struct memuse_info {
	uint16 ver;			
	uint16 len;			
	uint32 tot;			
	uint32 text_len;	
	uint32 data_len;	
	uint32 bss_len;		

	uint32 arena_size;	
	uint32 arena_free;	
	uint32 inuse_size;	
	uint32 inuse_hwm;	
	uint32 inuse_overhead;	
	uint32 inuse_total;	
} memuse_info_t;

#endif 
