#include <ql_oe.h>

typedef struct
{
    int  cmdIdx;
    char *funcName;
} st_api_test_case;
    
//for server test
st_api_test_case at_api_testcases[] = 
{
    {0,   "print_help"},
    {1,   "QL_Voice_Call_Start"},
    {2,   "QL_Voice_Call_End"},
    {3,   "QL_Voice_Call_Answer"},
    {4,   "QL_Voice_Call_CancelDial"},
    {5,   "QL_Voice_Call_Dtmf"},
    {6,   "QL_Voice_Call_SetForwarding"},
    {7,   "QL_Voice_Call_GetForwardingStatus"},
    {8,   "QL_Voice_Call_SetWaiting"},
    {9,   "QL_Voice_Call_GetWaitingStatus"},
    {10,  "QL_Voice_Call_Hold"},
    {11,  "QL_Voice_Call_UnHold"},

    {-1,    NULL}
};

void print_help(void)
{
    int i;

    printf("Supported test cases:\n");
    for(i = 0; ; i++)
    {
        if(at_api_testcases[i].cmdIdx == -1)
        {
            break;
        }
        printf("%d:\t%s\n", at_api_testcases[i].cmdIdx, at_api_testcases[i].funcName);
    }
}

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


int main(int argc, char *argv[])
{
    int    cmdIdx  = 0;
    int    ret     = E_QL_OK;
    char   phoneNum[32] = {0};
    voice_client_handle_type    h_voice     = 0;
    int                         voice_call_id = 0;

    ret = QL_Voice_Call_Client_Init(&h_voice);
    if(ret != 0 && ret != 1)
    {
        printf("QL_Voice_Call_Client_Init FAIL.%d\n");
        return -1;
    }
    printf("QL_Voice_Call_Client_Init ret = %d, with h_voice=%d\n", ret, h_voice);

    ret = QL_Voice_Call_AddStateHandler(h_voice, ql_voice_call_cb_func, &voice_call_id);
    if(ret != 0 && ret != 1)
    {
        printf("QL_Voice_Call_AddStateHandler FAIL.%d\n");
        return -1;
    }
    printf("QL_Voice_Call_AddStateHandler ret = %d\n", ret);

    print_help();

    while(1)
    {
        printf("\nplease input cmd index(-1 exit): ");
        scanf("%d", &cmdIdx);
        if(cmdIdx == -1)
        {
            break;
        }

        switch(cmdIdx)
        {
        case 0://"print_help"
        {
            print_help();
            break;
        }
        case 1://"QL_Voice_Call_Start"
        {
            char    PhoneNum[32]                  = {0}; 

            printf("please input dest phone number: \n");
            scanf("%s", PhoneNum);
            
            ret = QL_Voice_Call_Start(h_voice, E_QL_VCALL_EXTERNAL_SLOT_1, PhoneNum, &voice_call_id);
            printf("QL_Voice_Call_Start ret = %d, with voice_call_id=%d\n", ret, voice_call_id);
            break;
        }
        case 2://"QL_Voice_Call_End"
        {
	    int call_id = -1;  
	    printf("please input end call id: \n");
	    scanf("%d", &call_id);
            ret = QL_Voice_Call_End(h_voice, call_id);
            printf(" ret = %d\n", ret);
            break;
        }
        case 3://"QL_Voice_Call_Answer"
        {
            int call_id = -1;
            printf(" please input answer call id\n");
            scanf("%d", &call_id);
            ret = QL_Voice_Call_Answer(h_voice, call_id);
            printf(" ret = %d\n", ret);
            break;
        }

        case 4://QL_Voice_Call_CancelDial
        {
            ret = QL_Voice_Call_CancelDial(h_voice);
            printf(" QL_Voice_Call_CancelDial ret = %d\n", ret);
            break;
        }

	case 5://QL_Voice_Call_Dtmf
        {
	    char uDigit;
	    printf("please input ASCII: \n");
	    getchar();//skip '\n'
	    uDigit = getchar();

            ret = QL_Voice_Call_Dtmf(h_voice,uDigit,voice_call_id);
            printf(" QL_Voice_Call_Dtmf ret = %d\n", ret);
            break;
        }

	case 6://QL_Voice_Call_SetForwarding
        {
		E_QL_MCM_VOICE_CALL_SERVICE_T eService;
		E_QL_MCM_VOICE_CALL_FORWARDING_REASON_T eReason;
		printf("please input service type:(1-REGISTER;2-ERASE) \n");
		scanf("%d", &eService);

		printf("please input service type:(0-UNCONDITIONALLY) \n");
		printf("please input service type:(1-MOBILEBUSY) \n");
		printf("please input service type:(2-NOREPLY) \n");
		printf("please input service type:(3-UNREACHABLE) \n");
		printf("please input service type:(4-ALLFORWARDING) \n");
		printf("please input service type:(5--ALLCONDITIONAL) \n");
		scanf("%d", &eReason);

		
		switch(eService)
		{
			
			case	E_QL_MCM_VOICE_SERVICE_REGISTER:
				{
					char    PhoneNum[32]                  = {0}; 
			            	printf("please input dest phone number: \n");
            				scanf("%s", PhoneNum);

					ret = QL_Voice_Call_SetForwarding(h_voice,eService,eReason,PhoneNum);
					break;
				}

			case	E_QL_MCM_VOICE_SERVICE_ERASE:
				{
					ret = QL_Voice_Call_SetForwarding(h_voice,eService,eReason,NULL);
					break;
				}
			default:
				printf("please input correct service type:(0-ACTIVATE;1-DEACTIVATE;2-REGISTER;3-ERASE) \n");
				break;
		}

		printf(" QL_Voice_Call_SetForwarding ret = %d\n", ret);
            	break;
        }

	case 7://QL_Voice_Call_GetForwardingStatus
        {
		E_QL_MCM_VOICE_CALL_FORWARDING_REASON_T eReason;
		ql_mcm_voice_call_forwarding_status_list_t status_list;
		
		memset(&status_list,0,sizeof(status_list));

		printf("please input service type:(0-UNCONDITIONALLY) \n");
		printf("please input service type:(1-MOBILEBUSY) \n");
		printf("please input service type:(2-NOREPLY) \n");
		printf("please input service type:(3-UNREACHABLE) \n");
		printf("please input service type:(4-ALLFORWARDING) \n");
		printf("please input service type:(5--ALLCONDITIONAL) \n");
		scanf("%d", &eReason);

            	ret = QL_Voice_Call_GetForwardingStatus(h_voice,eReason,&status_list);
		
            	printf(" QL_Voice_Call_GetForwardingStatus ret = %d\n", ret);
		printf(" status:%d 	(0-disable	1-enable)\n ",status_list.status);
		
		int i = 0;
		for (i = 0; i< status_list.info_len; i++)
		{
			printf(" i:%d	type:%d(0-VOICE;1-DATA;2-VOICE_DATA)	number:%s	\n", 
				i,status_list.info[i].type,status_list.info[i].number);
		}
            	break;
        }

	case 8://QL_Voice_Call_SetWaiting
	{
		ql_mcm_voice_call_waiting_service_t service;
		memset(&service,0,sizeof(service));
		
		printf("\n 0-VOICE_ENABLED;1-DISABLED \n");
		printf("\n please input service type:\n");
		scanf("%d", &service.waiting_service);
		
		ret = QL_Voice_Call_SetWaiting(h_voice,service);
		printf(" QL_Voice_Call_SetWaiting ret = %d\n", ret);
		break;
	}

	case 9://QL_Voice_Call_GetWaitingStatus
	{
		ql_mcm_voice_call_waiting_service_t service;
		memset(&service,0,sizeof(service));
		
		ret = QL_Voice_Call_GetWaitingStatus(h_voice,&service);
		printf(" QL_Voice_Call_GetWaitingStatus ret = %d\n", ret);
		printf(" QL_Voice_Call_GetWaitingStatus waiting_service:%d(0-VOICE_ENABLED;1-DISABLED)",
			service.waiting_service);
		break;
	}
	
	case 10://QL_Voice_Call_Hold
	{
		ret = QL_Voice_Call_Hold(h_voice);
		printf(" QL_Voice_Call_Hold ret = %d\n", ret);
		break;
	}

	case 11://QL_Voice_Call_UnHold
	{
		ret = QL_Voice_Call_UnHold(h_voice);
		printf(" QL_Voice_Call_UnHold ret = %d\n", ret);
		break;
	}

       default:
            print_help();
            break;   
        }
    }

    ret = QL_Voice_Call_RemoveStateHandler(h_voice);
    if(ret != 0 && ret != 1)
    {
        printf("QL_Voice_Call_RemoveStateHandler FAIL.%d\n");
        return -1;
    }
    printf("QL_Voice_Call_RemoveStateHandler ret = %d\n", ret);


    ret = QL_Voice_Call_Client_Deinit(h_voice);
    if(ret != 0 && ret != 1)
    {
        printf("QL_Voice_Call_Client_Deinit FAIL.%d\n");
        return -1;
    }
    printf("QL_Voice_Call_Client_Deinit ret = %d, with h_voice=%d\n", ret, h_voice);

    return 0;
}
