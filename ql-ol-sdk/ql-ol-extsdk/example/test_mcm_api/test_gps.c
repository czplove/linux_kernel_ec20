#include <ql_oe.h>
#include "test_base.h"

static int    h_loc   = 0;
extern func_api_test_t t_gps_test;
st_api_test_case at_gps_testlist[] = 
{
    {0,    "QL_LOC_Client_Init"},
    {1,    "QL_LOC_AddRxIndMsgHandler"},
    {2,    "QL_LOC_Set_Indications"},
    {3,    "QL_LOC_Set_Position_Mode"},
    {4,    "QL_LOC_Start_Navigation"},
    {5,    "QL_LOC_Stop_Navigation"},
    {6,    "QL_LOC_Get_Current_Location"},
    {7,    "QL_LOC_Client_Deinit"},
    {8,    "QL_LOC_Delete_Aiding_Data"},
    {9,    "QL_LOC_InjectTime"},
    {10,   "QL_LOC_InjectLocation"},
    {11,   "QL_LOC_Xtra_InjectData"},
    {12,   "QL_LOC_Xtra_InjectFile"},
    {13,   "QL_LOC_Agps_DataConnOpen"},
    {14,   "QL_LOC_Agps_DataConnClose"},
    {15,   "QL_LOC_Agps_NfyDataConnFailed"},
    {16,   "QL_LOC_Agps_SetServer"},
    {17,   "QL_LOC_NI_SetResponse"},
    {18,   "QL_LOC_Agps_UpdateNWAvailability"},    

    {-1,   "Return to main menu"}
};

static void ql_loc_rx_ind_msg_cb(loc_client_handle_type  h_loc,
                                 E_QL_LOC_NFY_MSG_ID_T   e_msg_id,
                                 void                    *pv_data,
                                 void                    *contextPtr)
{
    printf("e_msg_id=%d\n", e_msg_id); 
    switch(e_msg_id)
    {
        case E_QL_LOC_NFY_MSG_ID_STATUS_INFO:
            break;
        case E_QL_LOC_NFY_MSG_ID_LOCATION_INFO:
        {
            QL_LOC_LOCATION_INFO_T *pt_location = (QL_LOC_LOCATION_INFO_T *)pv_data;
            printf("**** flag=0x%X, Latitude = %f, Longitude=%f, accuracy = %f ****\n", 
                        pt_location->flags, 
                        pt_location->latitude, 
                        pt_location->longitude, 
                        pt_location->accuracy);
            break;
        }
        case E_QL_LOC_NFY_MSG_ID_SV_INFO:
            break;
        case E_QL_LOC_NFY_MSG_ID_NMEA_INFO:
        {
            QL_LOC_NMEA_INFO_T  *pt_nmea = (QL_LOC_NMEA_INFO_T  *)pv_data;

            printf("NMEA info: timestamp=%lld, length=%d, nmea=%s\n", 
                    pt_nmea->timestamp, pt_nmea->length, pt_nmea->nmea); 
            break;
        }
        case E_QL_LOC_NFY_MSG_ID_CAPABILITIES_INFO:
            break;
        case E_QL_LOC_NFY_MSG_ID_AGPS_STATUS:
            break;
        case E_QL_LOC_NFY_MSG_ID_NI_NOTIFICATION:
            break;
        case E_QL_LOC_NFY_MSG_ID_XTRA_REPORT_SERVER:
            break;
    }
}

static int test_gps(void)
{
    int    cmdIdx  = 0;
    int    ret     = E_QL_OK;
        
    show_group_help(&t_gps_test);

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
        case 0://"QL_LOC_Client_Init"
        {
            ret = QL_LOC_Client_Init(&h_loc);
            printf("QL_LOC_Client_Init ret=%d\n", ret);
            break;
        }
        case 1://"QL_LOC_AddRxIndMsgHandler"
        {
            ret = QL_LOC_AddRxIndMsgHandler(ql_loc_rx_ind_msg_cb, (void*)h_loc);
            printf("QL_LOC_AddRxIndMsgHandler ret=%d\n", ret);
            break;
        }
        case 2://"QL_LOC_Set_Indications"
        {
            int     bitmask     = 0;
            printf("Please input indication bitmask(NiNfy|AGPS|XTRA|UTC|CAP|NMEA|SV|Status|Location)(HEX Base, i.e. 1FF):\n", ret);
            scanf("%x", &bitmask);
            
            /* Set what we want callbacks for */
            ret = QL_LOC_Set_Indications(h_loc, bitmask);
            printf("QL_LOC_Set_Indications bitmask=0x%X ret %d\n", bitmask, ret);
            break;
        }
        case 3://"QL_LOC_Set_Position_Mode"
        {
            QL_LOC_POS_MODE_INFO_T  t_mode      = {0};

            //You need set it as you wish
            t_mode.mode                 = E_QL_LOC_POS_MODE_STANDALONE;
            t_mode.recurrence           = E_QL_LOC_POS_RECURRENCE_PERIODIC;
            t_mode.min_interval         = 1000;  //report nmea frequency 1Hz
            t_mode.preferred_accuracy   = 50;    // <50m
            t_mode.preferred_time       = 90;    // 90s
            ret = QL_LOC_Set_Position_Mode(h_loc, &t_mode);
            printf("QL_LOC_Set_Position_Mode ret %d\n", ret);
            break;
        }
        case 4://"QL_LOC_Start_Navigation"
        {
            ret = QL_LOC_Start_Navigation(h_loc);
            printf("QL_LOC_Start_Navigation ret=%d\n", ret);
            break;
        }
        case 5://"QL_LOC_Stop_Navigation"
        {
            ret = QL_LOC_Stop_Navigation(h_loc);
            printf("QL_LOC_Stop_Navigation ret=%d\n", ret);
            break;
        }   
        
        case 6://"QL_LOC_Get_Current_Location"
        {
            int                     timeout_sec = 60;
            QL_LOC_LOCATION_INFO_T  t_loc_info  = {0};

            ret = QL_LOC_Get_Current_Location(h_loc, &t_loc_info, timeout_sec);
            printf(" QL_LOC_Get_Current_Location ret %d\n", ret);
            if(ret < 0)
            {
                if(ret == -2)
                {// -2: timeout, may need try again
                    printf("QL_LOC_Get_Current_Location timeout, try again!\n");
                }
                else
                {
                    printf("QL_LOC_Get_Current_Location Fail, ret %d\n", ret);
                    break;
                }
            }
            else
            {
                printf("**** Latitude = %lf, Longitude=%lf, altitude=%lf, accuracy = %f ****\n", 
                        t_loc_info.latitude, t_loc_info.longitude, t_loc_info.altitude, t_loc_info.accuracy);
            }
            break;
        }
        
        case 7://"QL_LOC_Client_Deinit"
        {
            ret = QL_LOC_Client_Deinit(h_loc);
            printf("QL_LOC_Client_Deinit ret=%d\n", ret);
            break;
        }
        
        case 8://"QL_LOC_Delete_Aiding_Data"
        {
            int     bitmask     = 0;
            printf("Please input aiding data type bitmask\n(-1:All | 20-TIME_GLO | 19-TIME_GPS|18-ALMANAC_CORR_GLO|17-SVSTEER_GLO | 16-SVDIR_GLO|\n"
                    "15-ALMANAC_GLO | 14-EPHEMERIS_GLO | 13-FREQ_BIAS_EST | 12-ALMANAC_CORR | 11-CELLDB_INFO | 10-RTI |\n"
                    " 9-SADATA | 8-SVSTEER | 7-SVDIR | 6-HEALTH | 5-UTC | 4-IONO |\n"
                    " 3-TIME | 2-POSITION | 1-ALMANAC | 0-EPHEMERIS)(HEX Base):\n", ret);
            scanf("%x", &bitmask);
            
            ret = QL_LOC_Delete_Aiding_Data(h_loc, bitmask);
            printf("QL_LOC_Delete_Aiding_Data ret=%d\n", ret);
            break;
        }
        case 9://"QL_LOC_InjectTime"
        {
            QL_LOC_INJECT_TIME_INTO_T   t_info = {0};
            
            //You should fill the right time of xtra data
            t_info.time = 123456789;
            t_info.time_reference = 12345;
            t_info.uncertainty = 12345;
            
            ret = QL_LOC_InjectTime(h_loc, &t_info);
            printf("QL_LOC_InjectTime ret=%d\n", ret);
            break;
        }
        case 10://"QL_LOC_InjectLocation"
        {
            QL_LOC_INJECT_LOCATION_INTO_T    t_info = {0};

            t_info.longitude = 117.282699;//You should fill this with right value
            t_info.latitude  = 31.866942;
            t_info.accuracy  = 100.0;
            ret = QL_LOC_InjectLocation(h_loc, &t_info);
            printf("QL_LOC_InjectLocation ret=%d\n", ret);
            break;
        }

        case 11://"QL_LOC_Xtra_InjectData"
        case 12://"QL_LOC_Xtra_InjectFileData"
        {
            char    file_path_name[128] = {0};

            printf("please input xtra data file path+name: \n");
            scanf("%s", file_path_name);
            
            ret = QL_LOC_Xtra_InjectFile(h_loc, file_path_name);
            printf("QL_LOC_Xtra_InjectFile ret=%d\n", ret);
            break;
        }
        
        case 13://"QL_LOC_Agps_DataConnOpen"
        {
            QL_LOC_AGPS_DATA_CONN_OPEN_INTO_T t_info = {0};
            //You should fill t_info with right value
            ret = QL_LOC_Agps_DataConnOpen(h_loc, &t_info);
            printf("QL_LOC_Agps_DataConnOpen ret=%d\n", ret);
            break;
        }
        case 14://"QL_LOC_Agps_DataConnClose"
        {
            E_QL_LOC_AGPS_TYPE_T      atype;
            
            printf("please input AGPS type(0:ANY, 1:SUPL, 2:C2K, 3:WWAN_ANY, 4:WIFI, 5:SUPL_ES): \n");
            scanf("%d", atype);
            
            ret = QL_LOC_Agps_DataConnClose(h_loc, atype);
            printf("QL_LOC_Agps_DataConnClose ret=%d\n", ret);
            break;
        }
        case 15://"QL_LOC_Agps_NfyDataConnFailed"
        {
            E_QL_LOC_AGPS_TYPE_T      atype;
            
            printf("please input AGPS type(0:ANY, 1:SUPL, 2:C2K, 3:WWAN_ANY, 4:WIFI, 5:SUPL_ES): \n");
            scanf("%d", atype);
            
            ret = QL_LOC_Agps_NfyDataConnFailed(h_loc, atype);
            printf("QL_LOC_Agps_NfyDataConnFailed ret=%d\n", ret);
            break;
        }
        case 16://"QL_LOC_Agps_SetServer"
        {
            QL_LOC_AGPS_SERVER_INTO_T     t_info = {0};
            
            //You should fill t_info with right value
            ret = QL_LOC_Agps_SetServer(h_loc, &t_info);
            printf("QL_LOC_Agps_SetServer ret=%d\n", ret);
            break;
        }
        case 17://"QL_LOC_NI_SetResponse"
        {
            QL_LOC_NI_RESPONSE_INTO_T     t_info = {0};
            //You should fill t_info with right value
            ret = QL_LOC_NI_SetResponse(h_loc, &t_info);
            printf("QL_LOC_NI_SetResponse ret=%d\n", ret);
            break;
        }
        case 18://""
        {
            ret = QL_LOC_Agps_UpdateNWAvailability(h_loc, 1, "test_apn_name");
            printf("QL_LOC_Agps_UpdateNWAvailability ret=%d\n", ret);
            break;
        }
    
        default:
            show_group_help(&t_gps_test);
        }
    }
    return 0;
}


func_api_test_t t_gps_test = {"mcm_gps", at_gps_testlist, test_gps};

