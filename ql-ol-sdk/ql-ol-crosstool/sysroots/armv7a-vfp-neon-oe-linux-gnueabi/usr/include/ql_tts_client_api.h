/**
 *@file     quectel_tts_client_api.h
 *@date     2017-08-24
 *@author   running.qian
 *@brief    Quecte TTS API
 */
#ifndef _QUECTEL_TTS_CLIENT_API_H_
#define _QUECTEL_TTS_CLIENT_API_H_

typedef enum {
    QL_TTS_DATA_MODE_ASCII,
    QL_TTS_DATA_MODE_UCS,
} ql_tts_data_mode_e_type;

typedef enum {
    QL_TTS_PLAY_MODE_NORMAL,	
    //rock.wang 20180418 add stream mode.	 
    QL_TTS_PLAY_MODE_STREAM,
} ql_tts_play_mode_e_type;

typedef enum {
    QL_TTS_CLIENT_STATE_NOREADY,
    QL_TTS_CLIENT_STATE_READY,
    QL_TTS_CLIENT_STATE_PLAYING,
} ql_tts_client_state_e_type;

typedef enum {
    QL_TTS_EVT_INIT,
    QL_TTS_EVT_DEINIT,
    
    QL_TTS_EVT_PLAY_START,
    QL_TTS_EVT_PLAY_INTERRUPT,
    QL_TTS_EVT_PLAY_STREAM,//rock.wang 20180418 add for get pcm stream.    
    QL_TTS_EVT_PLAY_FINISH,
    QL_TTS_EVT_PLAY_FAILED,
} ql_tts_client_event_e_type;

//rock.wang 20180418 add for get pcm stream mode
typedef void (*ql_tts_cb_type)(ql_tts_client_event_e_type evt, char *str, int len);

int   QL_TTS_client_init(ql_tts_cb_type user_cb);
void  QL_TTS_client_deinit(void);
void  QL_TTS_client_callback_set(ql_tts_cb_type user_cb);
void  QL_TTS_client_state_get(ql_tts_client_state_e_type *state);

int   QL_TTS_client_play(ql_tts_play_mode_e_type play_mode, ql_tts_data_mode_e_type data_mode, char *text, int text_len);
int   QL_TTS_client_stop(void);

void  QL_TTS_client_volume_set(int v); //The range is -32768 ~ 32767, default value: 0
int   QL_TTS_client_volume_get(void);
void  QL_TTS_client_speed_set(int v);  //The range is -32768 ~ 32767, default value: 0
int   QL_TTS_client_speed_get(void);

#endif
