#ifndef __QUECTEL_CUST_AT_HANDLER_H__
#define __QUECTEL_CUST_AT_HANDLER_H__

#include "quectel_common_at_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int parse_qcustat_param(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qcustat(void *param, quec_AtCmdResponse *response);

#ifdef __cplusplus
}
#endif

#endif /* __QUECTEL_CUST_AT_HANDLER_H__ */
