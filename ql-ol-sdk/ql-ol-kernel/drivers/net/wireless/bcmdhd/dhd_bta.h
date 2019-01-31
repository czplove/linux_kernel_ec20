/*
 * BT-AMP support routines
 *
 * $ Copyright Open Broadcom Corporation $
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id: dhd_bta.h 514727 2014-11-12 03:02:48Z $
 */
#ifndef __dhd_bta_h__
#define __dhd_bta_h__

struct dhd_pub;

extern int dhd_bta_docmd(struct dhd_pub *pub, void *cmd_buf, uint cmd_len);

extern void dhd_bta_doevt(struct dhd_pub *pub, void *data_buf, uint data_len);

extern int dhd_bta_tx_hcidata(struct dhd_pub *pub, void *data_buf, uint data_len);
extern void dhd_bta_tx_hcidata_complete(struct dhd_pub *dhdp, void *txp, bool success);


#endif /* __dhd_bta_h__ */
