#include <ql_oe.h>
#include "test_base.h"

static voice_client_handle_type    h_voice          = 0;
static int                         voice_call_id    = 0;
extern func_api_test_t t_voice_test;
st_api_test_case at_voice_testlist[] = 
{
    {0,     "QL_Voice_Call_Client_Init"},
    {1,     "QL_Voice_Call_Client_Deinit"},
    {2,     "QL_Voice_Call_AddStateHandler"},
    {3,     "QL_Voice_Call_RemoveStateHandler"},
    {4,     "QL_Voice_Call_Start"},
    {5,     "QL_Voice_Call_End"},
    {6,     "QL_Voice_Call_Answer"},
    {7,     "QL_Voice_Call_Hold"},    
    {8,     "QL_Voice_Call_UnHold"},
    {9,     "QL_Voice_Call_Conference"},
    {10,    "QL_Voice_Call_EndConference"},
    {11,    "QL_Voice_Call_Ecall"},
    {12,    "QL_Voice_Call_UpdateMsd"},
    {13,    "QL_Voice_Call_SetAutoAnswer"},
    {14,    "QL_Voice_Call_GetCallStatus"},
    {15,    "QL_Voice_Call_CancelDial"},

    {-1,   "Return to main menu"}
};


static void ql_voice_call_cb_func(int                     call_id,
                                  char*                   phone_num,
                                  E_QL_VOICE_CALL_STATE_T state,
                                  void                    *contextPtr)
{
    voice_client_handle_type *ph_voice = (voice_client_handle_type *)contextPtr;
    
    char *call_state[] = {"INCOMING", "DIALING", "ALERTING", "ACTIVE", "HOLDING", "END", "WAITING"};

    if(*ph_voice == 0)//Call_id got here, you can save it for the later operation
    {
        *ph_voice = call_id;
    }
    
    printf("######### Call id=%d, PhoneNum:%s, event=%s!  ######\n", call_id, phone_num, call_state[state]);
}

#define BUF_SIZE 32
static int test_voice(void)
{
    int    cmdIdx  = 0;
    int    ret     = E_QL_OK;
    char    buf[BUF_SIZE] = {0};    
        
    show_group_help(&t_voice_test);

    while(1)
    {
        printf("please input cmd index(-1 exit): ");
        scanf("%d", &cmdIdx);
        if(cmdIdx == -1)
        {
            break;
        }
        switch(cmdIdx)
        {        
        case 0://"QL_Voice_Call_Client_Init"
        {
            ret = QL_Voice_Call_Client_Init(&h_voice);
            printf("QL_Voice_Call_Client_Init ret = %d, with h_voice=%d\n", ret, h_voice);
            break;
        }
        
        case 1://"QL_Voice_Call_Client_Deinit"
        {
            ret = QL_Voice_Call_Client_Deinit(h_voice);
            printf("QL_Voice_Call_Client_Deinit ret = %d\n", ret);
            break;
        }
        case 2://"QL_Voice_Call_AddStateHandler"
        {
            ret = QL_Voice_Call_AddStateHandler(h_voice, ql_voice_call_cb_func, &voice_call_id);
            printf("QL_Voice_Call_AddStateHandler ret = %d\n", ret);
            break;
        }
        case 3://"QL_Voice_Call_RemoveStateHandler"
        {
            ret = QL_Voice_Call_RemoveStateHandler(h_voice);
            printf("QL_Voice_Call_RemoveStateHandler ret = %d\n", ret);
            break;
        }
        case 4://"QL_Voice_Call_Start"
        {
            char    PhoneNum[32]                  = {0}; 

            printf("please input dest phone number: \n");
            scanf("%s", PhoneNum);
            
            ret = QL_Voice_Call_Start(h_voice, E_QL_VCALL_EXTERNAL_SLOT_1, PhoneNum, &voice_call_id);
            printf("QL_Voice_Call_Start ret = %d, with voice_call_id=%d\n", ret, voice_call_id);
            break;
        }
        case 5://"QL_Voice_Call_End"
        {
            printf("please input end call id: \n");
            scanf("%d", &voice_call_id);
            
            ret = QL_Voice_Call_End(h_voice, voice_call_id);
            printf("QL_Voice_Call_End voice_call_id=%d, ret = %d\n", voice_call_id, ret);
            break;
        }
        case 6://"QL_Voice_Call_Answer"
        {
            ret = QL_Voice_Call_Answer(h_voice, voice_call_id);
            printf("QL_Voice_Call_Answer ret = %d\n", ret);
            break;
        }
        case 7://"QL_Voice_Call_Hold"
        {
            ret = QL_Voice_Call_Hold(h_voice);
            printf("QL_Voice_Call_Hold ret = %d\n", ret);
            break;
        }
        case 8://"QL_Voice_Call_UnHold"
        {
            ret = QL_Voice_Call_UnHold(h_voice);
            printf("QL_Voice_Call_UnHold ret = %d\n", ret);
            break;
        }
        
        case 9://"QL_Voice_Call_Conference"
        {
            ret = QL_Voice_Call_Conference(h_voice);
            printf("QL_Voice_Call_Conference ret = %d\n", ret);
            break;
        }
        
        case 10://"QL_Voice_Call_EndConference"
        {
            ret = QL_Voice_Call_EndConference(h_voice);
            printf("QL_Voice_Call_EndConference ret = %d\n", ret);
            break;
        }
        case 11://"QL_Voice_Call_Ecall"
        {
            //ret = QL_Voice_Call_Ecall(h_voice);
            printf("QL_Voice_Call_Ecall ret = %d\n", ret);
            break;
        }
        
        case 12://"QL_Voice_Call_UpdateMsd"
        {
            //ret = QL_Voice_Call_UpdateMsd(h_voice);
            printf("QL_Voice_Call_UpdateMsd ret = %d\n", ret);
            break;
        }
        
        case 13://"QL_Voice_Call_SetAutoAnswer"
        {
            //ret = QL_Voice_Call_SetAutoAnswer(h_voice);
            printf("QL_Voice_Call_SetAutoAnswer ret = %d\n", ret);
            break;
        } 

        case 14://"QL_Voice_Call_GetCallStatus"
        {
            int i;
            ql_mcm_voice_calls_state_t t_info = {0};
            char *call_direct[] = { "MO",       "MT",       "??",           "??"};
            char *voice_tech[]  = { "3GPP   ",  "3GPP2  ",  "UNKNOWN",      "UNKNOWN"};
            char *voice_prsnt[] = { "UNKNOWN",  "ALLOWED",  "RESTRICTED",   "PAYPHONE  "};
            char *call_state[]  = { "INCOMING", "DIALING",  "ALERTING",     "ACTIVE", 
                                    "HOLD",     "END",      "WAITING",      "UNKNOWN"};
            
            ret = QL_Voice_Call_GetCallStatus(h_voice, -1, &t_info); // -1<0 means all, you can specified it with voice_call_id
            printf("QL_Voice_Call_GetCallStatus ret = %d\n", ret);
            for (i = 0; i < t_info.calls_len; i++)
            {
                printf("call[%d] call_ID:%d, direction:%s, number:%s, presentation:%s, tech:%s, state:%s, call_end_reason:%d\n",
                                i,
                                t_info.calls[i].call_id,
                                call_direct[t_info.calls[i].direction & 0x03],
                                t_info.calls[i].number,
                                voice_prsnt[t_info.calls[i].number_presentation & 0x03],
                                voice_tech[t_info.calls[i].tech & 0x03],
                                call_state[t_info.calls[i].state & 0x07],
                                t_info.calls[i].call_end_reason);
            }
            break;
        } 

        case 15://"QL_Voice_Call_CancelDial"
        {
            ret = QL_Voice_Call_CancelDial(h_voice);
            printf("QL_Voice_Call_CancelDial ret = %d\n", ret);
            break;
        } 


        default:
            show_group_help(&t_voice_test);
        }
    }
    return 0;
}


func_api_test_t t_voice_test = {"mcm_voice", at_voice_testlist, test_voice};


