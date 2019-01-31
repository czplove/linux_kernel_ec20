/**
 *@file     quectel_tts_client.c
 *@date     2017-08-24
 *@author   running.qian
 *@brief    Quecte TTS client
 */

#include "ql_oe.h"

#include "ql_tts_client_api.h"


#define QL_TTS_LOG(format, ...)     fprintf(stderr, "[QL_TTS_E][%d]:"format, __LINE__, ##__VA_ARGS__)


void user_callback(ql_tts_client_event_e_type evt, char *infostr)
{
   QL_TTS_LOG("user_callback Got evt:%d, %s\n\r", evt, infostr);
}


int main(int argc, char *argv[])
{
    int i, value;
    char ucs_demo[] = {"6B228FCE4F7F752879FB8FDC6A215757"};
    
    QL_TTS_LOG("cmd: %s %s\n\r", argv[1], argv[2]);
    
    QL_TTS_client_init(user_callback);
    
    if (strcmp(argv[1], "p1") == 0) {
        QL_TTS_client_play(QL_TTS_PLAY_MODE_NORMAL, QL_TTS_DATA_MODE_ASCII, argv[2], strlen(argv[2]));
    }
    else if (strcmp(argv[1], "p2") == 0) {
        QL_TTS_client_play(QL_TTS_PLAY_MODE_NORMAL, QL_TTS_DATA_MODE_UCS, argv[2], strlen(argv[2]));
    }
    else if (strcmp(argv[1], "st") == 0) {
        QL_TTS_client_stop();
    }
    else if (strcmp(argv[1], "sv") == 0) {
        value = atoi(argv[2]);
        QL_TTS_client_volume_set(value);
    }
    else if (strcmp(argv[1], "gv") == 0) {
        value = QL_TTS_client_volume_get();
        QL_TTS_LOG("Get Volume Vaule is %d\n",value);
    }
    else if (strcmp(argv[1], "ss") == 0) {
        value = atoi(argv[2]);
        QL_TTS_client_speed_set(value);
    }
    else if (strcmp(argv[1], "gs") == 0) {
        value = QL_TTS_client_speed_get();
        QL_TTS_LOG("Get Speed Value is %d\n\r",value);
    }
    else {
        QL_TTS_LOG("unkonwn test cmd\n\r");
        QL_TTS_client_deinit();
        return 0;
    }
    
    while (1) {
        sleep(1);
    }
    
    return 0;
}
