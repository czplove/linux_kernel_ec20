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
    {1,   "QL_Voice_Call_Ecall"},
    {2,   "QL_Voice_Call_Ecall_HangUp"},
    {3,   "QL_Voice_Call_Ecall_UpdateMsd"},
    {4,   "QL_Voice_Call_Ecall_MsdPush"},
    {5,   "QL_Voice_Call_Ecall_GetConfigInfo"},
    {6,   "QL_Voice_Call_Ecall_SetConfigInfo"},
    
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

static void ql_voice_call_ind_func(unsigned int ind_id,
                                   void* ind_data,
                                   uint32_t ind_data_len)
{
    if(NULL == ind_data)
    {
        return;
    }

    switch(ind_id)
    {
        case E_QL_MCM_VOICE_CALL_IND:
        {
            if(ind_data_len != sizeof(ql_mcm_voice_call_ind))
            {
                break;
            }
            
            ql_mcm_voice_call_ind *pVoiceCallInd = (ql_mcm_voice_call_ind*)ind_data;

            char *call_state[] = {"INCOMING", "DIALING", "ALERTING", "ACTIVE", "HOLDING", "END", "WAITING"};

            int i = 0;
            for(i = 0; i < pVoiceCallInd->calls_len; i++)
            {
                printf("######### Call id=%d, PhoneNum:%s, event=%s!  ######\n", 
                            pVoiceCallInd->calls[i].call_id, pVoiceCallInd->calls[i].number, call_state[pVoiceCallInd->calls[i].state]);
            }
            
            break;
        }

        case E_QL_MCM_VOICE_ECALL_STATUE_IND:
        {
            if(ind_data_len != sizeof(ql_mcm_voice_ecall_status_ind))
            {
                break;
            }

            ql_mcm_voice_ecall_status_ind *pEcallStatusInd 
                                           = (ql_mcm_voice_ecall_status_ind*)ind_data;

            if (pEcallStatusInd->ecall_msd_tx_status_valid)
            {
                if (pEcallStatusInd->ecall_msd_tx_status == E_QL_MCM_VOICE_ECALL_MSD_TRANSMISSION_STATUS_SUCCESS)
                {
                    printf("========== Ecall status  call_id =%d ,   ecall msd tx success.\r\n", pEcallStatusInd->call_id);
                }
                else
                {
                    printf("========== Ecall status  call_id =%d ,   ecall msd tx failure.\r\n",  pEcallStatusInd->call_id);
                }
            }
            else
            {
                printf("========== Ecall status  call_id =%d  \r\n", pEcallStatusInd->call_id);
            }

            break;
        }
	/*
	// Deprecated event beause of E_QL_MCM_VOICE_ECALL_EVENT_IND
	case E_QL_MCM_VOICE_ECALL_URC_IND:
        {
            if(ind_data_len != sizeof(ql_mcm_voice_ecall_urc_ind))
            {
                break;
            }

            ql_mcm_voice_ecall_urc_ind *pEcallUrcInd
                                           = (ql_mcm_voice_ecall_urc_ind*)ind_data;
 
	    printf("========== Ecall URC EVENT:  call_id =%d ,   urc_event:%s    \r\n", 
			pEcallUrcInd->call_id,pEcallUrcInd->ecall_urc_event);
	    break;
        }
	*/
	case E_QL_MCM_VOICE_ECALL_EVENT_IND:
	{
		if(ind_data_len != sizeof(ql_mcm_voice_ecall_event_ind))
		{
			break;
		}

		ql_mcm_voice_ecall_event_ind *pEcallEventInd = (ql_mcm_voice_ecall_event_ind*)ind_data;
		
		if(pEcallEventInd->ecall_event_fails_valid)
		{
			printf("========== Ecall IND EVENT:   ecall_event_fails:%d     ==========\r\n", pEcallEventInd->ecall_event_fails);
		}

		if(pEcallEventInd->ecall_event_process_valid)
		{
			printf("========== Ecall IND EVENT:   ecall_event_process:%d   ==========\r\n", pEcallEventInd->ecall_event_process);
		}

		if(pEcallEventInd->ecall_event_msdupdate_valid)
		{
			printf("========== Ecall IND EVENT:   ecall_event_msdupdate:%d ==========  \r\n", pEcallEventInd->ecall_event_msdupdate);
		}

		if(pEcallEventInd->ecall_event_establish_valid)
		{
			printf("========== Ecall IND EVENT:   ecall_event_establish:%d ==========  \r\n", pEcallEventInd->ecall_event_establish);
		}

		if(pEcallEventInd->ecall_event_hackcode_valid)
		{
			printf("========== Ecall IND EVENT:   ecall_event_hackcode:%d  ========== \r\n", pEcallEventInd->ecall_event_hackcode);
		}

		if(pEcallEventInd->ecall_event_ori_redial_valid)
		{
			printf("========== Ecall IND EVENT:  ori_remainder_times:%d		time:%d  ========== \r\n",
					pEcallEventInd->ecall_event_ori_redial.ori_remainder_times,pEcallEventInd->ecall_event_ori_redial.time);
		}

		if(pEcallEventInd->ecall_event_drop_redial_valid)
		{
			printf("========== Ecall IND EVENT:  drop_remainder_times:%d		time:%d  ========== \r\n",
					pEcallEventInd->ecall_event_drop_redial.drop_remainder_times,pEcallEventInd->ecall_event_drop_redial.time);
		}

		break;
	}
        case E_QL_MCM_VOICE_UNKOWN_IND:
        default:
            break;
    }
}


int main(int argc, char *argv[])
{
    int    cmdIdx  = 0;
    int    ret     = E_QL_OK;
    char   phoneNum[32] = {0};
    voice_client_handle_type    h_voice     = 0;
    int                         voice_call_id = 0;

    ret = QL_Voice_Call_Client_Init(&h_voice);
    if(ret < 0)
    {
        printf("QL_Voice_Call_Client_Init FAIL.	ret:%d\n",ret);
        return -1;
    }
    printf("QL_Voice_Call_Client_Init ret = %d, with h_voice=%d\n", ret, h_voice);

    ret = QL_Voice_Call_AddCommonStateHandler(h_voice, (QL_VoiceCall_CommonStateHandlerFunc_t)ql_voice_call_ind_func);
    if(ret < 0)
    {
        printf("QL_Voice_Call_AddCommonStateHandler FAIL.		ret:%d\n",ret);
        return -1;
    }
    printf("QL_Voice_Call_AddCommonStateHandler ret = %d\n", ret);



    print_help();

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
        case 0://"print_help"
        {
            print_help();
            break;
        }
       	case 1://"QL_Voice_Call_Ecall"
        {			
            char    PhoneNum[32]                  = {0}; 
            printf("please input dest phone number: \n");
            scanf("%s", PhoneNum);

			ql_mcm_ecall_info ecall_info = {0,};
            printf("please input msd content: \n");
            scanf("%s", ecall_info.ecall_msd);

            printf("please input ecall mode(1:test 2:emergency): \n");
            scanf("%d",&ecall_info.eCallModeType);

            ret = QL_Voice_Call_Ecall(h_voice, E_QL_VCALL_EXTERNAL_SLOT_1,PhoneNum, ecall_info, &voice_call_id); 
			printf(" voice_call_id = %d\n", voice_call_id);
            printf(" ret = %d\n", ret);
            break;
        }
        case 2://QL_Voice_Call_Ecall_HangUp
        {
            ret = QL_Voice_Call_Ecall_HangUp(h_voice);
            printf(" ret = %d\n", ret);
            break;
        }

        case 3://QL_Voice_Call_Ecall_UpdateMsd
        {
	    char    cMsd[280+1]                  = {0}; 
            printf("please input msd content(HEX STR): \n");
            scanf("%s", cMsd);

            ret = QL_Voice_Call_Ecall_UpdateMsd(h_voice,cMsd);
            printf(" ret = %d\n", ret);
            break;
        }
	
	case 4://QL_Voice_Call_Ecall_PushMsd
        {
	    E_QL_MCM_ECALL_STATE_T ecall_state;

            ret = QL_Voice_Call_Ecall_MsdPush(h_voice,&ecall_state);
            printf(" QL_Voice_Call_Ecall_PushMsd ret = %d\n", ret);
            break;
        }

	case 5://QL_Voice_Call_Ecall_GetConfigInfo
        {
	    ql_mcm_ecall_config_info ecall_config = {0,};

            ret = QL_Voice_Call_Ecall_GetConfigInfo(h_voice,&ecall_config);
            printf(" QL_Voice_Call_Ecall_GetConfigInfo ret = %d\n", ret);
	    printf("enable:%d, voiceconfig:%d, ecallmode:%d, processinfo:%d, T5:%d, T6:%d, T7:%d, mofailredial:%d, dropredial:%d\n",
					ecall_config.enable,
					ecall_config.voiceconfig,
					ecall_config.ecallmode,
					ecall_config.processinfo,
					ecall_config.T5,
					ecall_config.T6,
					ecall_config.T7,
					ecall_config.mofailredial,
					ecall_config.dropredial
					);
            break;
        }

	case 6://QL_Voice_Call_Ecall_SetConfigInfo
        {
	    uint8_t ecall_config_type;
	    uint8_t value = 0;
	    printf("ecall config type(0-E_QL_MCM_VOICE_ECALL_CONFIG_ENABLE): \n");
	    printf("ecall config type(1-E_QL_MCM_VOICE_ECALL_CONFIG_VIOCECONF): \n");
	    printf("ecall config type(2-E_QL_MCM_VOICE_ECALL_CONFIG_MODE): \n");
	    printf("ecall config type(3-E_QL_MCM_VOICE_ECALL_CONFIG_PROCESSINFO): \n");
	    printf("ecall config type(4-E_QL_MCM_VOICE_ECALL_CONFIG_START_TIMER): \n");
	    printf("ecall config type(5-E_QL_MCM_VOICE_ECALL_CONFIG_HACK_TIMER): \n");
	    printf("ecall config type(6-E_QL_MCM_VOICE_ECALL_CONFIG_MSD_TRANSMISSION): \n");
	    printf("ecall config type(7-E_QL_MCM_VOICE_ECALL_CONFIG_MO_FAILR_REDIAL): \n");
	    printf("ecall config type(8-E_QL_MCM_VOICE_ECALL_CONFIG_DROP_REDIAL): \n");
	    printf("please input ecall config type: \n");
            scanf("%d", &ecall_config_type);

	    printf("please input ecall config value: \n");
	    scanf("%d", &value);
	    
	    switch(ecall_config_type)
	    {
		    case 0:
            		ret = QL_Voice_Call_Ecall_SetConfigInfo(h_voice,E_QL_MCM_VOICE_ECALL_CONFIG_ENABLE,value);
			 break;
		    case 1:
		    	ret = QL_Voice_Call_Ecall_SetConfigInfo(h_voice,E_QL_MCM_VOICE_ECALL_CONFIG_VIOCECONF,value);
			break;
		    case 2:
		    	ret = QL_Voice_Call_Ecall_SetConfigInfo(h_voice,E_QL_MCM_VOICE_ECALL_CONFIG_MODE,value);
			break;
		    case 3:
		    	ret = QL_Voice_Call_Ecall_SetConfigInfo(h_voice,E_QL_MCM_VOICE_ECALL_CONFIG_PROCESSINFO,value);
			break;
		    case 4:
		    	ret = QL_Voice_Call_Ecall_SetConfigInfo(h_voice,E_QL_MCM_VOICE_ECALL_CONFIG_START_TIMER,value);
			break;
		    case 5:
		    	ret = QL_Voice_Call_Ecall_SetConfigInfo(h_voice,E_QL_MCM_VOICE_ECALL_CONFIG_HACK_TIMER,value);
			break;
		    case 6:
		    	ret = QL_Voice_Call_Ecall_SetConfigInfo(h_voice,E_QL_MCM_VOICE_ECALL_CONFIG_MSD_TRANSMISSION,value);
			break;
		    case 7:
		    	ret = QL_Voice_Call_Ecall_SetConfigInfo(h_voice,E_QL_MCM_VOICE_ECALL_CONFIG_MO_FAILR_REDIAL,value);
			break;
		    case 8:
		    	ret = QL_Voice_Call_Ecall_SetConfigInfo(h_voice,E_QL_MCM_VOICE_ECALL_CONFIG_DROP_REDIAL,value);
			break;
		default:
			printf(" Input parameters error\n");
			break;
	    }
            printf(" QL_Voice_Call_Ecall_SetConfigInfo ret = %d\n", ret);
            break;
        }

       default:
            print_help();
            break;   
        }
    }

    ret = QL_Voice_Call_RemoveCommonStateHandler(h_voice);
    if(ret < 0)
    {
        printf("QL_Voice_Call_RemoveCommonStateHandler.	ret:%d\n",ret);
        return -1;
    }
    printf("QL_Voice_Call_RemoveCommonStateHandler ret = %d\n", ret);

    ret = QL_Voice_Call_Client_Deinit(h_voice);
    if(ret < 0)
    {
        printf("QL_Voice_Call_Client_Deinit FAIL.	ret:%d\n",ret);
        return -1;
    }
    printf("QL_Voice_Call_Client_Deinit ret = %d, with h_voice=%d\n", ret, h_voice);

    return 0;
}
