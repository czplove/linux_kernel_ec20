#include <ql_oe.h>
#include "test_base.h"

static int    h_sim   = 0;
extern func_api_test_t t_sim_test;
st_api_test_case at_sim_testlist[] = 
{
    {0,    "QL_MCM_SIM_Client_Init"},
    {1,    "QL_MCM_SIM_Client_Deinit"},
    {2,    "QL_MCM_SIM_GetIMSI"},
    {3,    "QL_MCM_SIM_GetICCID"},
    {4,    "QL_MCM_SIM_GetPhoneNumber"},
    {5,    "QL_MCM_SIM_GetOperatorPlmnList"},
    {6,    "QL_MCM_SIM_VerifyPIN"},
    {7,    "QL_MCM_SIM_ChangePin"},
    {8,    "QL_MCM_SIM_UnblockPIN"},
    {9,    "QL_MCM_SIM_EnablePIN"},
    {10,   "QL_MCM_SIM_DisablePIN"},
    {11,   "QL_MCM_SIM_GetCardStatus"},
    {12,   "QL_MCM_SIM_Depersonalization"},
    {13,   "QL_MCM_SIM_Personalization"},
    {14,   "QL_MCM_SIM_WriteFile"},
    {15,   "QL_MCM_SIM_ReadFile"},
    {16,   "QL_MCM_SIM_GetFileSize\n"},
    
    {-1,   "Return to main menu"}
};

#define BUF_SIZE 32
static int test_sim(void)
{
    int    cmdIdx  = 0;
    int    ret     = E_QL_OK;
    char    buf[BUF_SIZE] = {0};
        
    show_group_help(&t_sim_test);

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
        
        case 0://"QL_MCM_SIM_Client_Init"
        {
            ret = QL_MCM_SIM_Client_Init(&h_sim);
            printf("QL_MCM_SIM_Client_Init ret = %d with h_sim=%d\n", ret, h_sim);
            break;
        }
        case 1://"QL_MCM_SIM_Client_Deinit"
        {
            ret = QL_MCM_SIM_Client_Deinit(h_sim);
            printf("QL_MCM_SIM_Client_Deinit ret = %d\n", ret);
            break;
        }
        case 2://"QL_MCM_SIM_GetIMSI"
        {
            QL_SIM_APP_ID_INFO_T    t_info;

            memset(buf, 0, BUF_SIZE);
            t_info.e_slot_id    = E_QL_MCM_SIM_SLOT_ID_1;
            t_info.e_app        = E_QL_MCM_SIM_APP_TYPE_3GPP;
            ret = QL_MCM_SIM_GetIMSI(h_sim, &t_info, buf, BUF_SIZE);
            printf("QL_MCM_SIM_GetIMSI ret = %d, IMSI: %s\n", ret, buf);
            break;
        }
        case 3://"QL_MCM_SIM_GetICCID"
        {
            memset(buf, 0, BUF_SIZE);
            ret = QL_MCM_SIM_GetICCID(h_sim, E_QL_MCM_SIM_SLOT_ID_1, buf, BUF_SIZE);
            printf("QL_MCM_SIM_GetICCID ret = %d, ICCID: %s\n", ret, buf);
            break;
        }
        case 4://"QL_MCM_SIM_GetPhoneNumber"
        {
            int app_type = 0;
            QL_SIM_APP_ID_INFO_T    t_info;

            printf("please input AppType(0: unknown, 1: 3GPP, 2: 3GPP2, 3: ISIM): \n");
            scanf("%d", &app_type);
            if(app_type > 3)
            {
                printf("Invalid app type !\n");
                break;
            }
            
            t_info.e_slot_id    = E_QL_MCM_SIM_SLOT_ID_1;
            t_info.e_app        = (E_QL_MCM_SIM_APP_TYPE_T)(app_type + 0xB00);
            ret = QL_MCM_SIM_GetPhoneNumber(h_sim, &t_info, buf, BUF_SIZE);
            printf("QL_MCM_SIM_GetPhoneNumber ret = %d, phoneNum: %s\n", ret, buf);
            break;
        }
        case 5://"QL_MCM_SIM_GetOperatorPlmnList"
        {
            QL_SIM_PREFERRED_OPERATOR_LIST_T    t_info = {0};

            t_info.preferred_operator_list_len = 1;
            t_info.preferred_operator_list[0].mcc[0] = '4';
            t_info.preferred_operator_list[0].mcc[1] = '6';
            t_info.preferred_operator_list[0].mcc[2] = '0';            
            t_info.preferred_operator_list[0].mnc_len= 2;
            t_info.preferred_operator_list[0].mnc[0] = '1';
            t_info.preferred_operator_list[0].mnc[1] = '1';
            t_info.preferred_operator_list[0].mnc[2] = 0xFF;
            ret = QL_MCM_SIM_GetOperatorPlmnList(h_sim, E_QL_MCM_SIM_SLOT_ID_1, &t_info);
            printf("QL_MCM_SIM_GetOperatorPlmnList ret = %d\n", ret);
            break;
        }
        case 6://"QL_MCM_SIM_VerifyPIN"
        {
            QL_SIM_VERIFY_PIN_INFO_T    t_info = {0};

            t_info.app_info.e_slot_id   = E_QL_MCM_SIM_SLOT_ID_1;
            t_info.app_info.e_app       = E_QL_MCM_SIM_APP_TYPE_3GPP;
            t_info.pin_id               = E_QL_MCM_SIM_PIN_ID_1;
            
            printf("please input pin: \n");
            scanf("%9s", t_info.pin_value);
            t_info.pin_value_len = strlen(t_info.pin_value);
            
            ret = QL_MCM_SIM_VerifyPIN(h_sim, &t_info);
            printf("QL_MCM_SIM_VerifyPIN ret = %d\n", ret);
            break;
        }
        case 7://"QL_MCM_SIM_ChangePin"
        {
            QL_SIM_CHANGE_PIN_INFO_T    t_info = {0};

            t_info.app_info.e_slot_id   = E_QL_MCM_SIM_SLOT_ID_1;
            t_info.app_info.e_app       = E_QL_MCM_SIM_APP_TYPE_3GPP;
            t_info.pin_id               = E_QL_MCM_SIM_PIN_ID_1;
            
            printf("please input OLD pin: \n");
            scanf("%9s", t_info.old_pin_value);
            t_info.old_pin_value_len = strlen(t_info.old_pin_value);
            
            printf("please input NEW pin: \n");
            scanf("%9s", t_info.new_pin_value);
            t_info.new_pin_value_len = strlen(t_info.new_pin_value);
            
            ret = QL_MCM_SIM_ChangePin(h_sim, &t_info);
            printf("QL_MCM_SIM_ChangePin ret = %d\n", ret);
            break;
        }
        case 8://"QL_MCM_SIM_UnblockPIN"
        {
            QL_SIM_UNBLOCK_PIN_INFO_T    t_info = {0};

            t_info.app_info.e_slot_id   = E_QL_MCM_SIM_SLOT_ID_1;
            t_info.app_info.e_app       = E_QL_MCM_SIM_APP_TYPE_3GPP;
            t_info.pin_id               = E_QL_MCM_SIM_PIN_ID_1;
            
            printf("please input PUK pin: \n");
            scanf("%9s", t_info.puk_value);
            t_info.puk_value_len = strlen(t_info.puk_value);
            
            printf("please input NEW pin: \n");
            scanf("%9s", t_info.new_pin_value);
            t_info.new_pin_value_len = strlen(t_info.new_pin_value);
            
            ret = QL_MCM_SIM_UnblockPIN(h_sim, &t_info);
            printf("QL_MCM_SIM_UnblockPIN ret = %d\n", ret);
            break;
        }
        case 9://"QL_MCM_SIM_EnablePIN"
        {
            QL_SIM_ENABLE_PIN_INFO_T    t_info = {0};

            t_info.app_info.e_slot_id   = E_QL_MCM_SIM_SLOT_ID_1;
            t_info.app_info.e_app       = E_QL_MCM_SIM_APP_TYPE_3GPP;
            t_info.pin_id               = E_QL_MCM_SIM_PIN_ID_1;
            
            printf("please input pin: \n");
            scanf("%9s", t_info.pin_value);
            t_info.pin_value_len = strlen(t_info.pin_value);
            
            ret = QL_MCM_SIM_EnablePIN(h_sim, &t_info);
            printf("QL_MCM_SIM_EnablePIN ret = %d\n", ret);
            break;
        }
        case 10://"QL_MCM_SIM_DisablePIN"
        {
            QL_SIM_DISABLE_PIN_INFO_T    t_info = {0};

            t_info.app_info.e_slot_id   = E_QL_MCM_SIM_SLOT_ID_1;
            t_info.app_info.e_app       = E_QL_MCM_SIM_APP_TYPE_3GPP;
            t_info.pin_id               = E_QL_MCM_SIM_PIN_ID_1;
            
            printf("please input pin: \n");
            scanf("%9s", t_info.pin_value);
            t_info.pin_value_len = strlen(t_info.pin_value);
            
            ret = QL_MCM_SIM_DisablePIN(h_sim, &t_info);
            printf("QL_MCM_SIM_DisablePIN ret = %d\n", ret);
            break;
        }
        case 11://"QL_MCM_SIM_GetCardStatus"
        {
            QL_MCM_SIM_CARD_STATUS_INFO_T   t_info = {0};

            char *card_state[] = {  "UNKNOWN", 
                                    "ABSENT", 
                                    "PRESENT", 
                                    "ERROR_UNKNOWN", 
                                    "ERROR_POWER_DOWN", 
                                    "ERROR_POLL_ERROR", 
                                    "ERROR_NO_ATR_RECEIVED", 
                                    "ERROR_VOLT_MISMATCH", 
                                    "ERROR_PARITY_ERROR", 
                                    "ERROR_SIM_TECHNICAL_PROBLEMS"};            
            char *card_type[]  = {  "UNKNOWN", "ICC", "UICC"};
            char *app_state[]  = {  "UNKNOWN", 
                                    "DETECTED",
                                    "PIN1_REQ",
                                    "PUK1_REQ",
                                    "INITALIZATING",
                                    "PERSO_CK_REQ",
                                    "PERSO_PUK_REQ",
                                    "PERSO_PERMANENTLY_BLOCKED",
                                    "PIN1_PERM_BLOCKED",
                                    "ILLEGAL",
                                    "READY"};            
            ret = QL_MCM_SIM_GetCardStatus(h_sim, E_QL_MCM_SIM_SLOT_ID_1, &t_info);
            printf("QL_MCM_SIM_GetCardStatus ret = %d, card_state=%s, card_type=%s, app_state=%s, pin1_retries=%d\n", 
                    ret, 
                    card_state[t_info.e_card_state - 0xB01],
                    card_type[t_info.e_card_type - 0xB00],
                    app_state[t_info.card_app_info.app_3gpp.app_state - 0xB00],
                    t_info.card_app_info.app_3gpp.pin1_num_retries);
            break;
        }
        case 12://"QL_MCM_SIM_Depersonalization"
        {
            QL_MCM_SIM_DEPERSONALIZE_INFO_T     t_info = {0};

            t_info.e_feature_type   = E_QL_MCM_SIM_PERSO_FEATURE_TYPE_3GPP_NETWORK;
            t_info.e_operation      = E_QL_MCM_SIM_PERSO_OPERATION_DEACTIVATE;
            memcpy(t_info.ctrl_key_value, "testonly", strlen("testonly"));
            t_info.ctrl_key_value_len = strlen(t_info.ctrl_key_value);
            
            ret = QL_MCM_SIM_Depersonalization(h_sim, &t_info);
            printf("QL_MCM_SIM_Depersonalization ret = %d\n", ret);
            break;
        }
        case 13://"QL_MCM_SIM_Personalization"
        {
            QL_MCM_SIM_PERSONALIZE_FEATURE_INFO_T   t_info = {0};

            t_info.e_feature_type   = E_QL_MCM_SIM_PERSO_FEATURE_TYPE_3GPP_NETWORK;
            t_info.feature_data_len = 1;
            t_info.t_feature_data.feature_gw_network_perso[0].mcc[0] = '4';
            t_info.t_feature_data.feature_gw_network_perso[0].mcc[1] = '6';
            t_info.t_feature_data.feature_gw_network_perso[0].mcc[2] = '0';
            t_info.t_feature_data.feature_gw_network_perso[0].mnc_len= 2;
            t_info.t_feature_data.feature_gw_network_perso[0].mnc[0] = '1';
            t_info.t_feature_data.feature_gw_network_perso[0].mnc[1] = '1';
            t_info.t_feature_data.feature_gw_network_perso[0].mnc[2] = 0xFF;
            
            memcpy(t_info.ctrl_key_value, "testonly", strlen("testonly"));
            t_info.ctrl_key_value_len = strlen(t_info.ctrl_key_value);
            
            ret = QL_MCM_SIM_Personalization(h_sim, &t_info);
            printf("QL_MCM_SIM_Personalization ret = %d\n", ret);
            break;
        }
        case 14://"QL_MCM_SIM_WriteFile"
        {
            QL_MCM_SIM_CARD_WRITE_FILE_INFO_T   t_info = {0};

            #define TEST_FILE_PATH  "testonly"
            
            t_info.app_info.e_slot_id   = E_QL_MCM_SIM_SLOT_ID_1;
            t_info.app_info.e_app       = E_QL_MCM_SIM_APP_TYPE_3GPP;
            t_info.file_access.offset   = 0;
            t_info.file_access.path_len = strlen(TEST_FILE_PATH);
            memcpy(t_info.file_access.path, 
                   TEST_FILE_PATH,
                   t_info.file_access.path_len);
            t_info.file_access.record_num = 1;//0: transparent mode, other: 
            t_info.data_len = 3;
            t_info.data[0]  = 0x11;
            t_info.data[1]  = 0x22;
            t_info.data[2]  = 0x33;
            
            ret = QL_MCM_SIM_WriteFile(h_sim, &t_info);
            printf("QL_MCM_SIM_WriteFile ret = %d\n", ret);
            break;
        }
        case 15://"QL_MCM_SIM_ReadFile"
        {
            QL_MCM_SIM_CARD_FILE_INFO_TO_READ_T     t_info  = {0};
            QL_MCM_SIM_CARD_FILE_DATA_T             t_out   = {0};

            #define TEST_FILE_PATH  "testonly"
            
            t_info.app_info.e_slot_id   = E_QL_MCM_SIM_SLOT_ID_1;
            t_info.app_info.e_app       = E_QL_MCM_SIM_APP_TYPE_3GPP;
            t_info.file_access.offset   = 0;
            t_info.file_access.path_len = strlen(TEST_FILE_PATH);
            memcpy(t_info.file_access.path, 
                   TEST_FILE_PATH,
                   t_info.file_access.path_len);
            t_info.file_access.record_num = 1;//0: transparent mode, other: 

            ret = QL_MCM_SIM_ReadFile(h_sim, &t_info, &t_out);
            printf("QL_MCM_SIM_ReadFile ret = %d\n", ret);
            break;
        }
        case 16://"QL_MCM_SIM_GetFileSize"
        {    
            QL_MCM_SIM_CARD_GET_FILE_SIZE_INPUT_INFO_T  t_info  = {0};
            QL_MCM_SIM_FILE_SIZE_INFO_T                 t_out   = {0};

            #define TEST_FILE_PATH  "testonly"
            
            t_info.app_info.e_slot_id   = E_QL_MCM_SIM_SLOT_ID_1;
            t_info.app_info.e_app       = E_QL_MCM_SIM_APP_TYPE_3GPP;
            t_info.path_len = strlen(TEST_FILE_PATH);
            memcpy(t_info.path, 
                   TEST_FILE_PATH,
                   t_info.path_len);

            ret = QL_MCM_SIM_GetFileSize(h_sim, &t_info, &t_out);
            printf("QL_MCM_SIM_GetFileSize ret = %d, filesize=%d\n", ret, t_out.file_size);
            break;
        }

        default:
            show_group_help(&t_sim_test);
        }
    }
    return 0;
}


func_api_test_t t_sim_test = {"mcm_sim", at_sim_testlist, test_sim};


