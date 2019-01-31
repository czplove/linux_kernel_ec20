#include <ql_oe.h>
#include "test_base.h"

#define ATC_REQ_CMD_MAX_LEN     513
#define ATC_RESP_CMD_MAX_LEN    4097
static char atc_cmd_req[ATC_REQ_CMD_MAX_LEN]    = {0};
static char atc_cmd_resp[ATC_RESP_CMD_MAX_LEN]  = {0};
static atc_client_handle_type    h_atc     = 0;

extern func_api_test_t t_atc_test;
st_api_test_case at_atc_testlist[] = 
{
    {0,    "QL_ATC_Client_Init"},
    {1,    "QL_ATC_Send_Cmd"},
    {2,    "QL_ATC_Client_Deinit\n"},

    {-1,   "Return to main menu"}
};


#define BUF_SIZE 32
static int test_atc(void)
{
    int    cmdIdx  = 0;
    int    ret     = E_QL_OK;
    char    buf[BUF_SIZE] = {0};    
        
    show_group_help(&t_atc_test);

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
        case 0://"QL_ATC_Client_Init"
        {
            ret = QL_ATC_Client_Init(&h_atc);
            printf("QL_ATC_Client_Init ret=%d with h_atc=0x%x\n", ret, h_atc);
            break;
        }
        
        case 1://"QL_ATC_Send_Cmd"
        {
            memset(atc_cmd_req,  0, sizeof(atc_cmd_req));
            memset(atc_cmd_resp, 0, sizeof(atc_cmd_resp));
            
            printf("please input atc cmd: ");
            scanf("%s", atc_cmd_req);
            ret = QL_ATC_Send_Cmd(h_atc, atc_cmd_req, atc_cmd_resp, ATC_RESP_CMD_MAX_LEN);
            printf("QL_ATC_Send_Cmd \"%s\" ret=%d with resp=\n%s\n", atc_cmd_req, ret, atc_cmd_resp);
            break;
        }
        
        case 2://"QL_ATC_Client_Deinit"
        {
            ret = QL_ATC_Client_Deinit(h_atc);
            printf("QL_ATC_Client_Deinit ret=%d\n", ret);
            break;
        }
        
        default:
            show_group_help(&t_atc_test);
        }
    }
    return 0;
}


func_api_test_t t_atc_test = {"mcm_atcop", at_atc_testlist, test_atc};


