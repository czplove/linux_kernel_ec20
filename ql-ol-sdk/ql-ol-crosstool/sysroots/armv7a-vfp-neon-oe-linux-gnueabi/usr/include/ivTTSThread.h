/**
*@file 	 ivTTSThread.h
*@author running.qian
*@date   2017-08-23
*/
#ifndef __IVTTS_THREAD__
#define __IVTTS_THREAD__

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*QL_TTS_URC_CB)(char *urcString);
typedef void (*QL_TTS_STREAM_CB)(char *urcString, int len);//rock.wang 20180418 add for playmode.


int QL_TTS_Init(QL_TTS_URC_CB sendUrcFun, QL_TTS_STREAM_CB streamFun);//rock.wang 20180418 add for playmode.
int QL_TTS_Start(int playmode,int mode, char *text);//rock.wang 20180418 add for playmode.
void QL_TTS_Stop(void);
int QL_TTS_Get_State(void);
int QL_TTS_Set_SwVol(int vol);
int QL_TTS_Get_SwVol(void);
int QL_TTS_Set_Voice_Speed(int speed);
int QL_TTS_Get_Voice_Speed(void);

#ifdef __cplusplus
}
#endif
#endif /* __IVTTS_THREAD__ */

