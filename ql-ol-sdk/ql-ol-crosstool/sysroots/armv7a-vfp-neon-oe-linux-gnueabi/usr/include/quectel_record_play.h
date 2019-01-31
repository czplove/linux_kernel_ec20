
/*
 *  Quectel record/play at command handler
 *
 *  file name: quectel_record_play.h
 *
 *  Design by: scott.hu
 *
 *  Date: 2015-04-08 
 */

#ifndef __QUECTEL_RECORD_PLAY_H__
#define __QUECTEL_RECORD_PLAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "quectel_at_cmd_defs.h"   //bflag20161215

#define QAUDIO_FILENAME_MAX_LEN  100

extern void quec_set_call_state(int state);
extern int qaudrd_start_record(char *file_name, int format, int dlink);
extern void qaudrd_stop_record(void);
extern int qaudrd_is_recording(void);

extern int qaudplay_start_play(char *file_name, int format, int dlink);
extern void qaudplay_stop_play(void);
extern int qaudplay_is_playing(void);

#ifdef QUECTEL_QPSND_AT_CMD  //for QPSND
extern void *qpsnd_thread(void *arg);
extern int qpsnd_start_play(char *file_name, int repeat, int up, int down);
extern void qpsnd_stop_play(void);
extern int qpsnd_is_playing(void);
#endif
extern int qtonedet_start_dtmf_det(void);
extern void qtonedet_stop_dtmf_det(void);
extern int qtonedet_is_enabled(void);

#ifdef QUECTEL_QPCMV_AT_CMD  //add by becking.bai,bflag20161215
extern int quec_enable_pcmv(int port);
extern int quec_disable_pcmv(void);
extern void quec_pcmv_call_state_disable_path(void);
extern void quec_pcmv_call_state_enable_path(void);
extern int quec_pcmv_query_enabled(void);
extern int quec_pcmv_query_port(void);
#endif

#ifdef __cplusplus
}
#endif
#endif /* __QUECTEL_RECORD_PLAY_H__ */

