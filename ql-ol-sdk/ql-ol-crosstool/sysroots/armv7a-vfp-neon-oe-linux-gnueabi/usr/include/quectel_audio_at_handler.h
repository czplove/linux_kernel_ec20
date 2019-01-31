
/*
 *  Quectel Audio at command handler
 *
 *  file name: quectel_audio_at_handler.h
 *
 *  Design by: scott.hu
 *
 *  Date: 2014-11-06 
 */

#ifndef __QUECTEL_AUDIO_AT_HANDLER__
#define __QUECTEL_AUDIO_AT_HANDLER__

#ifdef __cplusplus
extern "C" {
#endif

#include "quectel_at_cmd_defs.h"

#ifdef QUECTEL_AUDIO_AT_CMD

extern int parse_qiic_parameters(AtCmd *cmd, atfwd_param_s_type *para);

extern void handle_qiic(void *param, quec_AtCmdResponse *response);
	
extern int parse_qaudloop_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qaudloop(void *param, quec_AtCmdResponse *response);

extern int parse_qdai_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qdai(void *param, quec_AtCmdResponse *response);
extern void query_qdai(quec_AtCmdResponse *response);

extern int parse_cmut_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_cmut(void *param, quec_AtCmdResponse *response);
extern void query_cmut(quec_AtCmdResponse *response);
extern int cmut_exec(int mute);

extern int parse_clvl_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_clvl(void *param, quec_AtCmdResponse *response);
extern void query_clvl(quec_AtCmdResponse *response);   //bflag20161212

extern int parse_crsl_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_crsl(void *param, quec_AtCmdResponse *response);
extern void query_crsl(quec_AtCmdResponse *response);   //bflag20161212

extern int parse_qsidet_parameters(AtCmd *cmd, atfwd_param_s_type *para);

extern void handle_qsidet(void *param, quec_AtCmdResponse *response);
extern void query_qsidet(quec_AtCmdResponse *response);

extern int parse_qaudmod_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qaudmod(void *param, quec_AtCmdResponse *response);

extern int parse_qeec_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qeec(void *param, quec_AtCmdResponse *response);
extern void query_qeec(quec_AtCmdResponse *response);

extern int parse_qmic_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qmic(void *param, quec_AtCmdResponse *response);
extern void query_qmic(quec_AtCmdResponse *response);

extern int parse_qrxgain_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qrxgain(void *param, quec_AtCmdResponse *response);
extern void query_qrxgain(quec_AtCmdResponse *response);

extern int parse_qaudplaygain_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qaudplaygain(void *param, quec_AtCmdResponse *response);
extern void query_qaudplaygain(quec_AtCmdResponse *response);

extern int parse_qaudrdgain_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qaudrdgain(void *param, quec_AtCmdResponse *response);
extern void query_qaudrdgain(quec_AtCmdResponse *response);

extern int parse_qaudrd_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qaudrd(void *param, quec_AtCmdResponse *response);
extern void query_qaudrd(quec_AtCmdResponse *response);

extern int parse_qaudplay_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qaudplay(void *param, quec_AtCmdResponse *response);
extern void query_qaudplay(quec_AtCmdResponse *response);

#ifdef QUECTEL_QPSND_AT_CMD
extern int parse_qpsnd_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qpsnd(void *param, quec_AtCmdResponse *response);
extern void query_qpsnd(quec_AtCmdResponse *response);
#endif
extern void exec_qaudstop(quec_AtCmdResponse *response);

#ifdef QUECTEL_QTTS_AT_CMD //add by running.qian,20160625
extern int parse_qtts_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qtts(void *param, quec_AtCmdResponse *response);
extern void query_qtts(quec_AtCmdResponse *response);

extern int parse_qttsetup_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qttsetup(void *param, quec_AtCmdResponse *response);
#endif /* QUECTEL_QTTS_AT_CMD */
#ifdef QUECTEL_QWTTS_AT_CMD //add by becking,20161026
extern int parse_qwtts_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qwtts(void *param, quec_AtCmdResponse *response);
extern void query_qwtts(quec_AtCmdResponse *response);
#endif /* QUECTEL_QWTTS_AT_CMD */

#ifdef QUECTEL_QPCMV_AT_CMD  //add by becking.bai,bflag20161215
extern int parse_qpcmv_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qpcmv(void *param, quec_AtCmdResponse *response);
extern void query_qpcmv(quec_AtCmdResponse *response);
#endif /* QUECTEL_QPCMV_AT_CMD*/

#ifdef QUECTEL_QIIR_AT_CMD  //add by becking.bai,bflag20170509
extern int parse_qiir_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qtxiir(void *param, quec_AtCmdResponse *response);
extern void query_qtxiir(quec_AtCmdResponse *response);
extern void handle_qrxiir(void *param, quec_AtCmdResponse *response);
extern void query_qrxiir(quec_AtCmdResponse *response);
#endif /* QUECTEL_QIIR_AT_CMD*/

#ifdef QUECTEL_QTONE_QDTMF_AT_CMD
extern int parse_qltone_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qltone(void *param, quec_AtCmdResponse *response);
extern int parse_qldtmf_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qldtmf(void *param, quec_AtCmdResponse *response);
extern void exec_qldtmf(quec_AtCmdResponse *response);
extern int parse_qaudcfg_parameters(AtCmd *cmd, atfwd_param_s_type *para);
extern void handle_qaudcfg(void *param, quec_AtCmdResponse *response);
extern int parse_qtonedet_parameters(AtCmd *cmd, atfwd_param_s_type *para); //add by becking 20160926
extern void handle_qtonedet(void *param, quec_AtCmdResponse *response);
extern void query_qtonedet(quec_AtCmdResponse *response);
#endif /*QUECTEL_QTONE_QDTMF_AT_CMD*/
#endif

#ifdef __cplusplus
}
#endif
#endif /* __QUECTEL_AUDIO_AT_HANDLER__ */

