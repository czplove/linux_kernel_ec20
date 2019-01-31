#include <ql_oe.h>
#include "test_base.h"

static dm_client_handle_type    h_dm     = 0;

extern func_api_test_t t_dm_test;
st_api_test_case at_dm_testlist[] = 
{
    {0,   "QL_MCM_DM_Client_Init"},
    {1,   "QL_MCM_DM_GetAirplaneMode"},
    {2,   "QL_MCM_DM_SetAirplaneMode"},
    {3,   "QL_MCM_DM_SetAirplaneModeChgInd"},
    {4,   "QL_MCM_DM_Client_Deinit"},
    {5,   "QL_MCM_DM_GetSerialNumbers"},
    {6,   "QL_MCM_DM_GetFirmware"},

    {-1,   "Return to main menu"}
};


#define BUF_SIZE 32
static int test_dm(void)
{
    int    cmdIdx  = 0;
    int    ret     = E_QL_OK;  
        
    show_group_help(&t_dm_test);

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
        case 0://"QL_MCM_DM_Client_Init"
        {
            ret = QL_MCM_DM_Client_Init(&h_dm);
            printf("QL_MCM_DM_Client_Init ret = %d with h_dm= %d\n", ret, h_dm);
            break;
        }
        case 1://"QL_MCM_DM_GetAirplaneMode"
        {
            E_QL_MCM_DM_AIRPLANE_MODE_TYPE_T e_airplane_mode;
            ret = QL_MCM_DM_GetAirplaneMode(h_dm, &e_airplane_mode);
            printf("QL_MCM_DM_GetAirplaneMode ret = %d, e_airplane_mode= %d\n", ret, e_airplane_mode);
            break;
        }

        case 2://"QL_MCM_DM_SetAirplaneMode"
        {
            E_QL_MCM_DM_AIRPLANE_MODE_TYPE_T e_airplane_mode;
            
            printf("please input airplane mode(1: ON, 2: OFF): \n");
            scanf("%d", &e_airplane_mode);
            
            ret = QL_MCM_DM_SetAirplaneMode(h_dm, e_airplane_mode);
            printf("QL_MCM_DM_SetAirplaneMode ret = %d\n", ret);
            break;
        }
        
        case 3://"QL_MCM_DM_SetAirplaneModeChgInd"
        {
            int onoff = 0;
            printf("please input airplane mode change indication ON/OFF(0: OFF, 1: ON): \n");
            scanf("%d", &onoff);
            
            ret = QL_MCM_DM_SetAirplaneModeChgInd(h_dm, onoff);
            printf("QL_MCM_DM_SetAirplaneModeChgInd ret = %d\n", ret);
            break;
        } 
        case 4://"QL_MCM_DM_Client_Deinit"
        {
            ret = QL_MCM_DM_Client_Deinit(h_dm);
            printf("QL_MCM_DM_Client_Deinit ret = %d\n", ret);
            break;
        }

        case 5://"QL_MCM_DM_GetSerialNumbers"
        {
            ql_dm_device_serial_numbers_t dm_device_serial_numbers;
            memset(&dm_device_serial_numbers,0,sizeof(dm_device_serial_numbers));
            ret = QL_MCM_DM_GetSerialNumbers(h_dm,&dm_device_serial_numbers);
            printf("QL_MCM_DM_GetSerialNumbers ret = %d\n", ret);
            if(ret < 0)
            {
                printf("QL_MCM_DM_GetSerialNumbers fail ret = %d\n", ret);
            }
            else
            {
                printf("SerialNumbers  imei:%s      meid:%s\n", 
                    dm_device_serial_numbers.imei,dm_device_serial_numbers.meid);
            }

            break;
        }

        case 6://"QL_MCM_DM_GetFirmware"
        {
            ql_dm_device_rev_id_t dm_device_rev;
            memset(&dm_device_rev,0,sizeof(dm_device_rev));
            ret = QL_MCM_DM_GetFirmware(h_dm,&dm_device_rev);
            printf("QL_MCM_DM_GetFirmware ret = %d\n", ret);

            if(ret < 0)
            {
                printf("QL_MCM_DM_GetFirmware fail ret = %d\n", ret);
            }
            else
            {
                printf("hardware revision identification :%s\n",
                    dm_device_rev.device_rev_id);
            }

            break;
        }
        
        default:
            show_group_help(&t_dm_test);
        }
    }
    return 0;
}


func_api_test_t t_dm_test = {"mcm_dm", at_dm_testlist, test_dm};


