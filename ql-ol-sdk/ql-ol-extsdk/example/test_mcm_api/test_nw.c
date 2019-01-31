#include <ql_oe.h>
#include "test_base.h"

static nw_client_handle_type    h_nw     = 0;
extern func_api_test_t t_nw_test;

char *tech_domain[] = {"NONE", "3GPP", "3GPP2"};
char *radio_tech[] = {"unknown", 
    "TD_SCDMA", "GSM",      "HSPAP",    "LTE",      "EHRPD",    "EVDO_B", 
    "HSPA",     "HSUPA",    "HSDPA",    "EVDO_A",   "EVDO_0",   "1xRTT", 
    "IS95B",    "IS95A",    "UMTS",     "EDGE",     "GPRS",     "NONE"};


st_api_test_case at_nw_testlist[] = 
{
    {0,     "QL_MCM_NW_Client_Init"},
    {1,     "QL_MCM_NW_SetConfig"},
    {2,     "QL_MCM_NW_GetConfig"},
    {3,     "QL_MCM_NW_GetNitzTimeInfo"},
    {4,     "QL_MCM_NW_EventRegister"},
    {5,     "QL_MCM_NW_GetOperatorName"},
    {6,     "QL_MCM_NW_PerformScan"},
    {7,     "QL_MCM_NW_GetRegStatus"},    
    {8,     "QL_MCM_NW_SetLowPowerMode"},
    {9,     "QL_MCM_NW_SetSelection"},
    {10,    "QL_MCM_NW_GetSignalStrength"},
    {11,    "QL_MCM_NW_GetCellAccessState"},
    {12,    "QL_MCM_NW_Client_Deinit"},
    {13,    "Set Time Server"},
    {14,    "Set Time Zone"},
    {15,    "QL_MCM_NW_AddRxMsgHandler"},

    {-1,   "Return to main menu"}
};

void nw_event_ind_handler (
    nw_client_handle_type h_nw, 
    uint32_t ind_flag, 
    void                  *ind_msg_buf, 
    uint32_t              ind_msg_len, 
    void                  *contextPtr)
{
    switch(ind_flag) {
        case NW_IND_VOICE_REG_EVENT_IND_FLAG:
            {
                QL_MCM_NW_VOICE_REG_EVENT_IND_T *ind = (void*)ind_msg_buf;
                printf("Recv event indication : VOICE REG EVENT\n");

                if(ind->registration_valid)
                {
                    printf("voice_registration: \ntech_domain=%s, radio_tech=%s, roaming=%d, registration_state=%d\n", 
                            tech_domain[ind->registration.tech_domain], 
                            radio_tech[ind->registration.radio_tech],
                            ind->registration.roaming,
                            ind->registration.registration_state);
                }
                if(ind->registration_details_3gpp_valid)
                {
                    printf("voice_registration_details_3gpp: \ntech_domain=%s, radio_tech=%s, mcc=%s, mnc=%s, roaming=%d, forbidden=%d, cid=0x%X, lac=%d, psc=%d, tac=%d\n", 
                            tech_domain[ind->registration_details_3gpp.tech_domain], 
                            radio_tech[ind->registration_details_3gpp.radio_tech],
                            ind->registration_details_3gpp.mcc,
                            ind->registration_details_3gpp.mnc,
                            ind->registration_details_3gpp.roaming,
                            ind->registration_details_3gpp.forbidden,                    
                            ind->registration_details_3gpp.cid,
                            ind->registration_details_3gpp.lac,
                            ind->registration_details_3gpp.psc,
                            ind->registration_details_3gpp.tac);
                }

                if(ind->registration_details_3gpp2_valid)
                {
                    printf("voice_registration_details_3gpp2: \ntech_domain=%s, radio_tech=%s, mcc=%s, mnc=%s, roaming=%d, forbidden=%d, sid=%d, nid=%d, bsid=%d\n", 
                            tech_domain[ind->registration_details_3gpp2.tech_domain], 
                            radio_tech[ind->registration_details_3gpp2.radio_tech],
                            ind->registration_details_3gpp2.mcc,
                            ind->registration_details_3gpp2.mnc,
                            ind->registration_details_3gpp2.roaming,
                            ind->registration_details_3gpp2.forbidden,                    
                            ind->registration_details_3gpp2.sid,
                            ind->registration_details_3gpp2.nid,
                            ind->registration_details_3gpp2.bsid);
                }

                break;
            }
        case NW_IND_DATA_REG_EVENT_IND_FLAG:
            {
                QL_MCM_NW_DATA_REG_EVENT_IND_T *ind = (void*)ind_msg_buf;

                printf("Recv event indication : DATA REG EVENT\n");

                if(ind->registration_valid)
                {
                    printf("data_registration: \ntech_domain=%s, radio_tech=%s, roaming=%d, registration_state=%d\n", 
                            tech_domain[ind->registration.tech_domain], 
                            radio_tech[ind->registration.radio_tech],
                            ind->registration.roaming,
                            ind->registration.registration_state);
                }
                if(ind->registration_details_3gpp_valid)
                {
                    printf("data_registration_details_3gpp: \ntech_domain=%s, radio_tech=%s, mcc=%s, mnc=%s, roaming=%d, forbidden=%d, cid=0x%X, lac=%d, psc=%d, tac=%d\n", 
                            tech_domain[ind->registration_details_3gpp.tech_domain], 
                            radio_tech[ind->registration_details_3gpp.radio_tech],
                            ind->registration_details_3gpp.mcc,
                            ind->registration_details_3gpp.mnc,
                            ind->registration_details_3gpp.roaming,
                            ind->registration_details_3gpp.forbidden,                    
                            ind->registration_details_3gpp.cid,
                            ind->registration_details_3gpp.lac,
                            ind->registration_details_3gpp.psc,
                            ind->registration_details_3gpp.tac);
                }

                if(ind->registration_details_3gpp2_valid)
                {
                    printf("data_registration_details_3gpp2: \ntech_domain=%s, radio_tech=%s, mcc=%s, mnc=%s, roaming=%d, forbidden=%d, sid=%d, nid=%d, bsid=%d\n", 
                            tech_domain[ind->registration_details_3gpp2.tech_domain], 
                            radio_tech[ind->registration_details_3gpp2.radio_tech],
                            ind->registration_details_3gpp2.roaming,
                            ind->registration_details_3gpp2.forbidden,                    
                            ind->registration_details_3gpp2.sid,
                            ind->registration_details_3gpp2.nid,
                            ind->registration_details_3gpp2.bsid);
                }

                break;
            }
        case NW_IND_SIGNAL_STRENGTH_EVENT_IND_FLAG:
            {
                QL_MCM_NW_SINGNAL_EVENT_IND_T *ind = (void*)ind_msg_buf;

                printf("Recv event indication : SIGNAL STRENGTH EVENT\n");

                if(ind->gsm_sig_info_valid)
                {
                    printf("gsm_sig_info: rssi=%d\n", 
                            ind->gsm_sig_info.rssi);
                } 

                if(ind->wcdma_sig_info_valid)
                {
                    printf("wcdma_sig_info: rssi=%d, ecio=%d\n", 
                            ind->wcdma_sig_info.rssi, 
                            ind->wcdma_sig_info.ecio);
                } 
                if(ind->tdscdma_sig_info_valid)
                {
                    printf("tdscdma_sig_info: rssi=%d, rscp=%d, ecio=%d, sinr=%d\n", 
                            ind->tdscdma_sig_info.rssi, 
                            ind->tdscdma_sig_info.rscp,
                            ind->tdscdma_sig_info.ecio,
                            ind->tdscdma_sig_info.sinr);
                } 
                if(ind->lte_sig_info_valid)
                {
                    printf("lte_sig_info: rssi=%d, rsrq=%d, rsrp=%d, snr=%d\n", 
                            ind->lte_sig_info.rssi, 
                            ind->lte_sig_info.rsrq,
                            ind->lte_sig_info.rsrp,
                            ind->lte_sig_info.snr);
                } 
                if(ind->cdma_sig_info_valid)
                {
                    printf("cdma_sig_info: rssi=%d, ecio=%d\n", 
                            ind->cdma_sig_info.rssi, 
                            ind->cdma_sig_info.ecio);
                } 
                if(ind->hdr_sig_info_valid)
                {
                    printf("hdr_sig_info: rssi=%d, ecio=%d, sinr=%d, io=%d\n", 
                            ind->hdr_sig_info.rssi, 
                            ind->hdr_sig_info.ecio,
                            ind->hdr_sig_info.sinr,
                            ind->hdr_sig_info.io);
                }
                break;
            }
        case NW_IND_CELL_ACCESS_STATE_CHG_EVENT_IND_FLAG:
            {
                QL_MCM_NW_CELL_ACCESS_STATE_EVENT_IND_T *ind = (void*)ind_msg_buf;
                printf("Recv event indication : CELL ACCESS STATE EVENT\n");
                printf("state = %d\n", ind->state);
                break;
            }
        case NW_IND_NITZ_TIME_UPDATE_EVENT_IND_FLAG:
            {
                QL_MCM_NW_NITZ_TIME_EVENT_IND_T *ind = (void*)ind_msg_buf; 
                printf("Recv event indication : NITZ TIME EVENT\n");
                printf("nitz_time=%s, abs_time=%lld, leap_sec=%d, \n",
                        ind->info.nitz_time, ind->info.abs_time, ind->info.leap_sec);
                break;
            }
        default:
            break;
    }
}


#define BUF_SIZE 32
static int test_nw(void)
{
    int    cmdIdx  = 0;
    int    ret     = E_QL_OK;
    char    buf[BUF_SIZE] = {0};    
        
    show_group_help(&t_nw_test);

    while(1)
    {
        printf("please input cmd index(-1 exit): ");
        ret = scanf("%d", &cmdIdx);
        if(ret != 1)
        {
            char c;
            while(((c=getchar()) != '\n') && (c != EOF))
            {
                ;
            }
            continue;
        }
        if(cmdIdx == -1)
        {
            break;
        }
        switch(cmdIdx)
        {
        case 0://"QL_MCM_NW_Client_Init"
        {
            ret = QL_MCM_NW_Client_Init(&h_nw);
            printf("QL_MCM_NW_Client_Init ret = %d\n", ret);
            break;
        }
        case 1://"QL_MCM_NW_SetConfig"
        {
            QL_MCM_NW_CONFIG_INFO_T     t_info = {0};
            
            int mask = 0;
            printf("please input event mask hex(16_PRL | TDSCDMA | LTE | EVDO | CDMA | WCDMA | GSM): \n");
            scanf("%x", &mask);
            t_info.preferred_nw_mode = mask;
            
            printf("please input roaming pref(0:off 1:on): \n");
            scanf("%d", &mask);
            t_info.roaming_pref = mask;
            
            ret = QL_MCM_NW_SetConfig(h_nw, &t_info);
            printf("QL_MCM_NW_SetConfig ret = %d\n", ret);
            break;
        }
        case 2://"QL_MCM_NW_GetConfig"
        {
            QL_MCM_NW_CONFIG_INFO_T     t_info = {0};
            ret = QL_MCM_NW_GetConfig(h_nw, &t_info);
            printf("QL_MCM_NW_GetConfig ret = %d, preferred_nw_mode=0x%X, roaming=%d\n", ret, t_info.preferred_nw_mode, t_info.roaming_pref);
            break;
        }
        case 3://"QL_MCM_NW_GetNitzTimeInfo"
        {
            QL_MCM_NW_NITZ_TIME_INFO_T t_info;
            ret = QL_MCM_NW_GetNitzTimeInfo(h_nw, &t_info);
            printf("QL_MCM_NW_GetNitzTimeInfo ret = %d, nitz_time=%s, abs_time=%lld, leap_sec=%d, \n", ret,
                    t_info.nitz_time, t_info.abs_time, t_info.leap_sec);
            break;
        }
        case 4://"QL_MCM_NW_EventRegister"
        {
            int mask = 0;
            
            printf("please input event mask-hex(NITZ_UPDATE | CELL_ACC_STATE_CHG | SIG_STRENGTH | DATA | VOICE): \n");
            scanf("%x", &mask);

            ret = QL_MCM_NW_EventRegister(h_nw, mask);
            printf("QL_MCM_NW_EventRegister ret = %d\n", ret);
            break;
        }
        case 5://"QL_MCM_NW_GetOperatorName"
        {
            QL_MCM_NW_OPERATOR_NAME_INFO_T  t_info;
            ret = QL_MCM_NW_GetOperatorName(h_nw, &t_info);
            printf("QL_MCM_NW_GetOperatorName ret = %d, long_eons=%s, short_eons=%s, mcc=%s, mnc=%s\n", ret,
                    t_info.long_eons, t_info.short_eons, t_info.mcc, t_info.mnc);
            break;
        }
        case 6://"QL_MCM_NW_PerformScan"
        {
            QL_MCM_NW_SCAN_RESULT_LIST_INFO_T   t_info;
            ret = QL_MCM_NW_PerformScan(h_nw, &t_info);
            printf("QL_MCM_NW_PerformScan ret = %d, list_len=%d, detail info.....\n", ret, t_info.entry_len);
            break;
        }
        case 7://"QL_MCM_NW_GetRegStatus"
        {
            QL_MCM_NW_REG_STATUS_INFO_T         t_info;

            memset(&t_info, 0, sizeof(QL_MCM_NW_REG_STATUS_INFO_T));
            ret = QL_MCM_NW_GetRegStatus(h_nw, &t_info);
            printf("QL_MCM_NW_GetRegStatus ret = %d, detail info:\n", ret);
            if(t_info.voice_registration_valid)
            {
                printf("voice_registration: \ntech_domain=%s, radio_tech=%s, roaming=%d, registration_state=%d\n", 
                    tech_domain[t_info.voice_registration.tech_domain], 
                    radio_tech[t_info.voice_registration.radio_tech],
                    t_info.voice_registration.roaming,
                    t_info.voice_registration.registration_state);
            }
            if(t_info.data_registration_valid)
            {
                printf("data_registration: \ntech_domain=%s, radio_tech=%s, roaming=%d, registration_state=%d\n", 
                    tech_domain[t_info.data_registration.tech_domain], 
                    radio_tech[t_info.data_registration.radio_tech],
                    t_info.data_registration.roaming,
                    t_info.data_registration.registration_state);
            }
            if(t_info.voice_registration_details_3gpp_valid)
            {
                printf("voice_registration_details_3gpp: \ntech_domain=%s, radio_tech=%s, mcc=%s, mnc=%s, roaming=%d, forbidden=%d, cid=0x%X, lac=%d, psc=%d, tac=%d\n", 
                    tech_domain[t_info.voice_registration_details_3gpp.tech_domain], 
                    radio_tech[t_info.voice_registration_details_3gpp.radio_tech],
                    t_info.voice_registration_details_3gpp.mcc,
                    t_info.voice_registration_details_3gpp.mnc,
                    t_info.voice_registration_details_3gpp.roaming,
                    t_info.voice_registration_details_3gpp.forbidden,                    
                    t_info.voice_registration_details_3gpp.cid,
                    t_info.voice_registration_details_3gpp.lac,
                    t_info.voice_registration_details_3gpp.psc,
                    t_info.voice_registration_details_3gpp.tac);
            }
            if(t_info.data_registration_details_3gpp_valid)
            {
                printf("data_registration_details_3gpp: \ntech_domain=%s, radio_tech=%s, mcc=%s, mnc=%s, roaming=%d, forbidden=%d, cid=0x%X, lac=%d, psc=%d, tac=%d\n", 
                    tech_domain[t_info.data_registration_details_3gpp.tech_domain], 
                    radio_tech[t_info.data_registration_details_3gpp.radio_tech],
                    t_info.data_registration_details_3gpp.mcc,
                    t_info.data_registration_details_3gpp.mnc,
                    t_info.data_registration_details_3gpp.roaming,
                    t_info.data_registration_details_3gpp.forbidden,                    
                    t_info.data_registration_details_3gpp.cid,
                    t_info.data_registration_details_3gpp.lac,
                    t_info.data_registration_details_3gpp.psc,
                    t_info.data_registration_details_3gpp.tac);
            }
            
            if(t_info.voice_registration_details_3gpp2_valid)
            {
                printf("voice_registration_details_3gpp2: \ntech_domain=%s, radio_tech=%s, mcc=%s, mnc=%s, roaming=%d, forbidden=%d, sid=%d, nid=%d, bsid=%d\n", 
                    tech_domain[t_info.voice_registration_details_3gpp2.tech_domain], 
                    radio_tech[t_info.voice_registration_details_3gpp2.radio_tech],
                    t_info.voice_registration_details_3gpp2.mcc,
                    t_info.voice_registration_details_3gpp2.mnc,
                    t_info.voice_registration_details_3gpp2.roaming,
                    t_info.voice_registration_details_3gpp2.forbidden,                    
                    t_info.voice_registration_details_3gpp2.sid,
                    t_info.voice_registration_details_3gpp2.nid,
                    t_info.voice_registration_details_3gpp2.bsid);
            }
            
            if(t_info.data_registration_details_3gpp2_valid)
            {
                printf("data_registration_details_3gpp2: \ntech_domain=%s, radio_tech=%s, mcc=%s, mnc=%s, roaming=%d, forbidden=%d, sid=%d, nid=%d, bsid=%d\n", 
                    tech_domain[t_info.data_registration_details_3gpp2.tech_domain], 
                    radio_tech[t_info.data_registration_details_3gpp2.radio_tech],
                    t_info.data_registration_details_3gpp2.mcc,
                    t_info.data_registration_details_3gpp2.mnc,
                    t_info.data_registration_details_3gpp2.roaming,
                    t_info.data_registration_details_3gpp2.forbidden,                    
                    t_info.data_registration_details_3gpp2.sid,
                    t_info.data_registration_details_3gpp2.nid,
                    t_info.data_registration_details_3gpp2.bsid);
            }
                        
            break;
        }
        case 8://"QL_MCM_NW_SetLowPowerMode"
        {
            int mode = 0;
            
            printf("please input low power mode(0: off, other: on): \n");
            scanf("%d", &mode);
            ret = QL_MCM_NW_SetLowPowerMode(h_nw, mode);
            printf("QL_MCM_NW_SetLowPowerMode ret = %d\n", ret);
            break;
        }
        case 9://"QL_MCM_NW_SetSelection"
        {
            QL_MCM_NW_SELECTION_INFO_T  t_info;
            
            printf("please input nw selection mode(0: auto, 1: manual): \n");
            scanf("%d", &t_info.nw_selection_type);

            if(t_info.nw_selection_type != E_QL_MCM_NW_SELECTION_AUTOMATIC)
            {
                printf("please input mcc: \n");
                scanf("%3s",&t_info.mcc);
                
                printf("please input mnc: \n");
                scanf("%3s", &t_info.mnc);
                printf("please input rat(1: TDSCDMA, 2: GSM, 3: HSPAP, 4:LTE, 5:EHRPD, 6:EVDO_B, 7:HSPA\n"
                        "8:HSUPA, 9:HSDPA, 10:EVDO_A, 11:EVDO_0, 12:1xRTT, 13:IS95B, 14:IS95A, 15:UMTS, 16:EDGE, 17:GPRS, 18:NONE):");
                scanf("%d", &t_info.rat);
            }
            
            ret = QL_MCM_NW_SetSelection(h_nw, &t_info);
            printf("QL_MCM_NW_SetSelection ret = %d\n", ret);
            break;
        }
        case 10://"QL_MCM_NW_GetSignalStrength"
        {
            QL_MCM_NW_SIGNAL_STRENGTH_INFO_T    t_info;

            memset(&t_info, 0, sizeof(QL_MCM_NW_SIGNAL_STRENGTH_INFO_T));
            ret = QL_MCM_NW_GetSignalStrength(h_nw, &t_info);
            printf("QL_MCM_NW_GetSignalStrength ret = %d, detail info:\n", ret);

            if(t_info.gsm_sig_info_valid)
            {
                printf("gsm_sig_info: rssi=%d\n", 
                        t_info.gsm_sig_info.rssi);
            } 

            if(t_info.wcdma_sig_info_valid)
            {
                printf("wcdma_sig_info: rssi=%d, ecio=%d\n", 
                        t_info.wcdma_sig_info.rssi, 
                        t_info.wcdma_sig_info.ecio);
            } 
            if(t_info.tdscdma_sig_info_valid)
            {
                printf("tdscdma_sig_info: rssi=%d, rscp=%d, ecio=%d, sinr=%d\n", 
                        t_info.tdscdma_sig_info.rssi, 
                        t_info.tdscdma_sig_info.rscp,
                        t_info.tdscdma_sig_info.ecio,
                        t_info.tdscdma_sig_info.sinr);
            } 
            if(t_info.lte_sig_info_valid)
            {
                printf("lte_sig_info: rssi=%d, rsrq=%d, rsrp=%d, snr=%d\n", 
                        t_info.lte_sig_info.rssi, 
                        t_info.lte_sig_info.rsrq,
                        t_info.lte_sig_info.rsrp,
                        t_info.lte_sig_info.snr);
            } 
            if(t_info.cdma_sig_info_valid)
            {
                printf("cdma_sig_info: rssi=%d, ecio=%d\n", 
                        t_info.cdma_sig_info.rssi, 
                        t_info.cdma_sig_info.ecio);
            } 
            if(t_info.hdr_sig_info_valid)
            {
                printf("hdr_sig_info: rssi=%d, ecio=%d, sinr=%d, io=%d\n", 
                        t_info.hdr_sig_info.rssi, 
                        t_info.hdr_sig_info.ecio,
                        t_info.hdr_sig_info.sinr,
                        t_info.hdr_sig_info.io);
            } 

            break;
        }
        case 11://"QL_MCM_NW_GetCellAccessState"
        {
            E_QL_MCM_NW_CELL_ACCESS_STATE_TYPE_T    e_state;
            ret = QL_MCM_NW_GetCellAccessState(h_nw, &e_state);
            printf("QL_MCM_NW_GetCellAccessState ret = %d, e_state=%d\n", ret, e_state);
            break;
        }
        case 12://"QL_MCM_NW_Client_Deinit"
        {
            ret = QL_MCM_NW_Client_Deinit(h_nw);
            printf("QL_MCM_NW_Client_Deinit ret = %d\n", ret);
            break;
        }
        case 13://"Set Time Server"
        {
            char timeserver[64] = {'n','t','p','d',' ','-','p',' '};
            printf("please input time server: \n");
            scanf("%50s", &timeserver[8]);
            ret = system(timeserver);
            printf("sytem(%s) ret = %d\n", timeserver, ret);
            break;
        }
        case 14://"Set Time Zone"
        {
            char stimezone[7] = {0};
            time_t rawtime;
            struct tm *info;
            char buffer[80];
            printf("please input time zone(i.e UTC+8 uses 'GMT-8'): \n");
            scanf("%7s", stimezone);
            ret = setenv("TZ", stimezone, 1);
            //ret = putenv("TZ=EST5EDT");
            
            tzset();
            printf("timezone=%d\n", timezone);
            printf("tzname[0]=%s, tzname[1]=%s\n",tzname[0], tzname[1]);
            printf("setenv %s ret = %d\n", stimezone, ret);
            
            time( &rawtime );
            info = localtime( &rawtime );
            printf("Current time: %s\n", asctime(info));
            
            break;
        }
        case 15 :
        {
           ret = QL_MCM_NW_AddRxMsgHandler(h_nw, nw_event_ind_handler, NULL);  
           printf("QL_MCM_NW_AddRxMsgHandler, ret=%d\n", ret);
           break;
        }
        
        default:
            show_group_help(&t_nw_test);
        }
    }
    return 0;
}


func_api_test_t t_nw_test = {"mcm_nw", at_nw_testlist, test_nw};


