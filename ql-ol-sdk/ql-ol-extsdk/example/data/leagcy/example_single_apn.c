#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include "comdef.h"
#include "ql_qcmap_client_api.h"

#define QL_WWAN_LOG(format, ...) fprintf(stderr, "[QL_CLI][%d]:"format, __LINE__, ##__VA_ARGS__)

static volatile boolean deamon_flag = TRUE;
static boolean autoconnect = TRUE; 

void ql_user_cmd_proc(int argc, char **argv)
{
    int profile_id = -1;
    boolean flag;

    if (strcmp(argv[1], "profile") == 0) {
        if (argc >= 3) {
            profile_id = argv[3][0] - '0';
            if (profile_id >= 0 && profile_id <= 8) {
                Ql_NW_MobileAP_Init();
                Ql_NW_MoblieAP_ProfileId_Set(profile_id);
            }
            else {
                goto invalid_paras;
            }
        }
        else {
            Ql_NW_MobileAP_Init();
            Ql_NW_MoblieAP_ProfileId_Get(&profile_id);
        }
    }
    else if (strcmp(argv[1], "autoconnect") == 0) {
        if (argc == 3) {
            if (argv[2][0] == '0') {
                Ql_NW_MobileAP_Init();
                Ql_NW_MoblieAP_AutoConnect_Set(FALSE);
            }
            else if (argv[2][0] == '1') {
                Ql_NW_MobileAP_Init();
                Ql_NW_MoblieAP_AutoConnect_Set(TRUE);
            }
            else {
                goto invalid_paras;
            }
        }
        else if (argc == 2) {
            Ql_NW_MobileAP_Init();
            Ql_NW_MoblieAP_AutoConnect_Get(&flag);
        }
        else {
            goto invalid_paras;
        }
    }
    else {
        goto invalid_paras;
    }

    return;
invalid_paras:
    QL_WWAN_LOG("\n\rInvalid profile paras input\n\r"
                "Get     PofileId ./<process> profile\n\r"
                "Set     PofileId ./<process> profile <Id(0-5)>\n\r"
                "Get  Autoconnect ./<process> autoconnect\n\r"
                "Set  Autoconnect ./<process> autoconnect <enable(0-1)>\n\r"
               );
    return;
}


void ql_qcmapclient_sighandler(int signal)
{
    switch(signal)
    {
    case SIGTERM:
        Ql_NW_MobileAP_Disable();
        exit(0);
        break;

    default:
        QL_WWAN_LOG("Receiver unexpected signal %s\n",signal);
        break;
    }
}


void ql_qcmapclient_nw_status_cb(ql_wwan_nw_staus_e_type status)
{
    boolean connected;

    switch (status) {
    case QL_WWAN_NW_CONNECTING:
        QL_WWAN_LOG("APPS connecting wwan...\n\r");
        break;

    case QL_WWAN_NW_CONNECTED:
        QL_WWAN_LOG("APPS connect wwan done\n\r");
        connected = TRUE;
        break;
    
    case QL_WWAN_NW_CONNECTING_TIMEOUT:
        QL_WWAN_LOG("APPS connect wwan timeout\n\r");
    case QL_WWAN_NW_DISCONNECTED:
        QL_WWAN_LOG("APPS disconnect wwan\n\r");
        connected = FALSE;
        if (!autoconnect) {
            deamon_flag = FALSE;
        }
        break;


    default:
        QL_WWAN_LOG("APPS connect wwan unkonwn evt\n\r");
        break;
        
    }

    return;
}


int main(int argc, char **argv)
{
    int connectTimeout;
    int retryTimes = 1;
    int profile_id = 1;

    if(daemon(1, 1) < 0)   
    {  
        perror("daemon");  
        exit(1);  
    }

    if (argc >= 2) {
        ql_user_cmd_proc(argc, argv);
        return 0;
    }

    signal(SIGTERM, ql_qcmapclient_sighandler);

    Ql_NW_MobileAP_Init();    
    Ql_NW_MoblieAP_AutoConnect_Set(TRUE);
    Ql_NW_MoblieAP_AutoConnect_Get(&autoconnect);
    
    Ql_NW_MobileAP_Enable(10, ql_qcmapclient_nw_status_cb);

    while (deamon_flag) {
        sleep(1);
    }
    
    Ql_NW_MobileAP_Disable();
}
