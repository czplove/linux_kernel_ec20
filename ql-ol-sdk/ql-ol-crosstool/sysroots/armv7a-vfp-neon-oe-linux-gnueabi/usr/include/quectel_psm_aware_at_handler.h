#ifndef __QUECTEL_PSM_AWARE_AT_HANDLER__
#define __QUECTEL_PSM_AWARE_AT_HANDLER__

#ifdef _cplusplus
extern "C"{
#endif

#include "quectel_at_cmd_defs.h"

#ifdef QUECTEL_PSM_AWARE_SUPPORT

#define PSM_AWARE_CTRL_CMD	0x00
#define PSM_AWARE_CFG_CMD	0x01


typedef struct _quectel_psm_aware_ctrl_param
{
	int	act;
	int	act_tm;
	int	wakeup_type;
	char	wakeup_tm[20];
}quectel_psm_aware_ctrl_param;

typedef struct _quectel_psm_aware_cfg_param
{
	char	name[50];
	union{
		int		i_val;
		char	ps_val[20];
	}data;
}quectel_psm_aware_cfg_param;

typedef struct _quectel_psm_aware_cmd
{
	int		cmd_type;//0x00,0x01
	int		i_len;
	union{
	quectel_psm_aware_ctrl_param ctrl_cmd;
	quectel_psm_aware_cfg_param  cfg_cmd;
	}cmd;
}quectel_psm_aware_cmd;

//extern void quectel_psm_aware_urc_pthread();
extern void quectel_psm_urc_pthread();

extern int quectel_parse_qpsm_params(AtCmd *cmd, atfwd_param_s_type *param);

extern void quectel_handle_qpsm(void *param, quec_AtCmdResponse *response);
extern void quectel_query_qpsm(quec_AtCmdResponse *response);
extern void quectel_qpsm_exec(quec_AtCmdResponse *response);

extern int quectel_parse_qpsmcfg_params(AtCmd *cmd, atfwd_param_s_type *param);

extern void quectel_handle_qpsmcfg(void *param, quec_AtCmdResponse *response);
extern void quectel_query_qpsmcfg(quec_AtCmdResponse *response);
extern void quectel_qpsmcfg_exec(quec_AtCmdResponse *response);

#endif/*QUECTEL_PSM_AWARE_SUPPORT*/

#ifdef _cplusplus
}
#endif

#endif

