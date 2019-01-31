#ifndef __QL_MCM_GPS_H__
#define __QL_MCM_GPS_H__


#define     LOC_IND_LOCATION_INFO_ON            (1 << 0)
#define     LOC_IND_STATUS_INFO_ON              (1 << 1)
#define     LOC_IND_SV_INFO_ON                  (1 << 2)
#define     LOC_IND_NMEA_INFO_ON                (1 << 3)
#define     LOC_IND_CAP_INFO_ON                 (1 << 4)
#define     LOC_IND_UTC_TIME_REQ_ON             (1 << 5)
#define     LOC_IND_XTRA_DATA_REQ_ON            (1 << 6)
#define     LOC_IND_AGPS_DATA_CONN_CMD_REQ_ON   (1 << 7)
#define     LOC_IND_NI_NFY_USER_RESP_REQ_ON     (1 << 8)

typedef uint32 loc_client_handle_type;

typedef enum 
{
    E_QL_LOC_POS_MODE_STANDALONE        = 0, /**<  Mode for running GPS standalone (no assistance).  */
    E_QL_LOC_POS_MODE_MS_BASED          = 1, /**<  AGPS MS-Based mode.  */
    E_QL_LOC_POS_MODE_MS_ASSISTED       = 2, /**<  AGPS MS-Assisted mode.  */
}E_QL_LOC_POS_MODE_T;

typedef enum 
{
    E_QL_LOC_POS_RECURRENCE_PERIODIC    = 0, /**<  Receive GPS fixes on a recurring basis at a specified period.  */
    E_QL_LOC_POS_RECURRENCE_SINGLE      = 1, /**<  Request a single-shot GPS fix.  */
}E_QL_LOC_POS_RECURRENCE_T;

typedef struct 
{
  E_QL_LOC_POS_MODE_T       mode;               /*  Position mode.      */
  E_QL_LOC_POS_RECURRENCE_T recurrence;         /*  Recurrence          */
  uint32_t                  min_interval;       /*  Minimum Interval, NMEA report frequency, 1000 means 1Hz, 100 means 10Hz    */
  uint32_t                  preferred_accuracy; /*  Preferred Accuracy , 30m or more, the less it takes longer timer. */
  uint32_t                  preferred_time;     /*  Preferred Time, first cold-boot may take 100s or more, hot boot may take 2s      */
}QL_LOC_POS_MODE_INFO_T; 

typedef enum 
{
    E_QL_LOC_LOCATION_LAT_LONG_VALID   = 0x0001, /**<  GPS location has valid latitude and longitude.  */
    E_QL_LOC_LOCATION_ALTITUDE_VALID   = 0x0002, /**<  GPS location has a valid altitude.  */
    E_QL_LOC_LOCATION_SPEED_VALID      = 0x0004, /**<  GPS location has a valid speed.  */
    E_QL_LOC_LOCATION_BEARING_VALID    = 0x0008, /**<  GPS location has a valid bearing.  */
    E_QL_LOC_LOCATION_ACCURACY_VALID   = 0x0010, /**<  GPS location has valid accuracy.  */
    E_QL_LOC_LOCATION_SOURCE_INFO_VALID= 0x0020, /**<  GPS location has valid source information.  */
    E_QL_LOC_LOCATION_IS_INDOOR_VALID  = 0x0040, /**<  GPS location has a valid "is indoor?" flag.  */
    E_QL_LOC_LOCATION_FLOOR_NUMBE_VALID= 0x0080, /**<  GPS location has a valid floor number.  */
    E_QL_LOC_LOCATION_MAP_URL_VALID    = 0x0100, /**<  GPS location has a valid map URL.  */
    E_QL_LOC_LOCATION_MAP_INDEX_VALID  = 0x0200, /**<  GPS location has a valid map index.  */
}E_QL_LOC_LOCATION_VALID_FLAG;

typedef enum 
{
    E_QL_LOC_ULP_LOCATION_SOURCE_HYBRID= 0x0001, /**<  Position source is ULP.  */
    E_QL_LOC_ULP_LOCATION_SOURCE_GNSS  = 0x0002, /**<  Position source is GNSS only.  */
}E_QL_LOC_ULP_LOCATION_SOURCE;

#define QL_LOC_GPS_RAW_DATA_LEN_MAX         256 /**  Raw data maximum size. */
#define QL_LOC_GPS_LOCATION_MAP_URL_SIZE    399 /**  Location map URL maximum size (used for indoor positioning). */
#define QL_LOC_GPS_LOCATION_MAP_IDX_SIZE    16  /**  Location map index maximum size (used for indoor positioning). */
typedef struct 
{
    uint32_t    size;                   /**<   Set to the size of mcm_gps_location_t. */
    E_QL_LOC_LOCATION_VALID_FLAG flags; /**<   Contains GPS location flags bits. */
    E_QL_LOC_ULP_LOCATION_SOURCE position_source;  /**<   Provider indicator for HYBRID or GPS. */
    double      latitude;               /**<   Latitude in degrees. */
    double      longitude;              /**<   Longitude in degrees. */
    double      altitude;               /**<   Altitude in meters above the WGS 84 reference ellipsoid. */
    float       speed;                  /**<   Speed in meters per second. */
    float       bearing;                /**<   Heading in degrees. */
    float       accuracy;               /**<   Expected accuracy in meters. */
    int64_t     timestamp;              /**<   Timestamp for the location fix in UTC million-second base.  */    
    int32_t     is_indoor;              /**<   Location is indoors. */
    float       floor_number;           /**<   Indicates the floor number. */
    uint32_t    raw_data_len;           /**<   Must be set to # of elements in raw_data */
    uint8_t     raw_data[QL_LOC_GPS_RAW_DATA_LEN_MAX];          /**<   Allows the HAL to pass additional information related to the location. */
    char        map_url[QL_LOC_GPS_LOCATION_MAP_URL_SIZE + 1];  /**<   Map URL. */
    uint8_t     map_index[QL_LOC_GPS_LOCATION_MAP_IDX_SIZE];    /**<   Map index. */
}QL_LOC_LOCATION_INFO_T;  /* Type */

typedef enum 
{
    E_QL_LOC_STATUS_NONE            = 0, /**<  GPS status unknown.  */
    E_QL_LOC_STATUS_SESSION_BEGIN   = 1, /**<  GPS has begun navigating.  */
    E_QL_LOC_STATUS_SESSION_END     = 2, /**<  GPS has stopped navigating.  */
    E_QL_LOC_STATUS_ENGINE_ON       = 3, /**<  GPS has powered on but is not navigating.  */
    E_QL_LOC_STATUS_ENGINE_OFF      = 4, /**<  GPS is powered off.  */
}E_QL_LOC_STATUS_VALUE_T;

typedef struct 
{
    uint32_t    size;                   /**<   Set to the size of mcm_gps_sv_info_t. */
    int         prn;                    /**<   Pseudo-random number for the SV. */
    float       snr;                    /**<   Signal-to-noise ratio. */
    float       elevation;              /**<   Elevation of the SV in degrees. */
    float       azimuth;                /**<   Azimuth of the SV in degrees. */
}QL_LOC_SV_INFO_T;                      /* Type */

#define     QL_LOC_GPS_SUPPORT_SVS_MAX   32  /**  Maximum number of satellites in view. */
typedef struct 
{
    uint32_t            size;                                   /**<   Set to the size of mcm_gps_sv_status_t. */
    int                 num_svs;                                /**<   Number of SVs currently visible. */
    QL_LOC_SV_INFO_T    sv_list[QL_LOC_GPS_SUPPORT_SVS_MAX];    /**<   Contains an array of SV information. */
    uint32_t            ephemeris_mask;                         /**<   Bitmask indicating which SVs have ephemeris data.  */
    uint32_t            almanac_mask;                           /**<   Bitmask indicating which SVs have almanac data.   */
    uint32_t            used_in_fix_mask;                       /**<   Bitmask indicating which SVs were used for computing the most recent position fix. */
}QL_LOC_SV_STATUS_T;  /* Type */


#define QL_LOC_NMEA_MAX_LENGTH  255                 /**  NMEA string maximum length. */
typedef struct 
{
    int64_t     timestamp;                          /**<   System Timestamp, marked for when got the nmea data */
    int         length;                             /**<   NMEA string length. */
    char        nmea[QL_LOC_NMEA_MAX_LENGTH + 1];   /**<   NMEA string.*/
}QL_LOC_NMEA_INFO_T;  /* Message */

typedef enum 
{
    E_QL_LOC_CAPABILITY_SCHEDULING      = 0x01, /**<  GPS HAL schedules fixes for GPS_POSITION_RECURRENCE_PERIODIC mode.
                                                         If this is not set, the framework uses \n 1000 ms for min_interval
                                                         and will call start() and stop() to schedule the GPS. */
    E_QL_LOC_CAPABILITY_MSB             = 0x02, /**<  GPS supports MS-Based AGPS mode.  */
    E_QL_LOC_CAPABILITY_MSA             = 0x04, /**<  GPS supports MS-Assisted AGPS mode.  */
    E_QL_LOC_CAPABILITY_SINGLE_SHOT     = 0x08, /**<  GPS supports single-shot fixes.  */
    E_QL_LOC_CAPABILITY_ON_DEMAND_TIME  = 0x10, /**<  GPS supports on-demand time injection.  */
}E_QL_LOC_CAPABILITIES_T;


#define QL_LOC_GPS_SSID_BUF_SIZE    32          /**  Maximum SSID (Service Set Identifier) buffer size. */
#define QL_LOC_IPV6_ADDR_LEN        16          /**  IPv6 address length. */
typedef enum 
{
    E_QL_LOC_AGPS_TYPE_INVALID          = -1,   /**<  Invalid.  */
    E_QL_LOC_AGPS_TYPE_ANY              = 0,    /**<  Any.  */
    E_QL_LOC_AGPS_TYPE_SUPL             = 1,    /**<  SUPL.  */
    E_QL_LOC_AGPS_TYPE_C2K              = 2,    /**<  C2K.  */
    E_QL_LOC_AGPS_TYPE_WWAN_ANY         = 3,    /**<  WWAN any.  */
    E_QL_LOC_AGPS_TYPE_WIFI             = 4,    /**<  Wi-Fi.  */
    E_QL_LOC_AGPS_TYPE_SUPL_ES          = 5,    /**<  SUPL_ES.  */
}E_QL_LOC_AGPS_TYPE_T;

typedef enum 
{
    E_QL_LOC_REQUEST_AGPS_DATA_CONN     = 1,    /**<  GPS requests a data connection for AGPS.  */
    E_QL_LOC_RELEASE_AGPS_DATA_CONN     = 2,    /**<  GPS releases the AGPS data connection.  */
    E_QL_LOC_AGPS_DATA_CONNECTED        = 3,    /**<  AGPS data connection is initiated  */
    E_QL_LOC_AGPS_DATA_CONN_DONE        = 4,    /**<  AGPS data connection is completed.  */
    E_QL_LOC_AGPS_DATA_CONN_FAILED      = 5,    /**<  AGPS data connection failed.  */
}E_QL_LOC_AGPS_STATUS_VALUE_T;

typedef struct 
{
    uint32_t                        size;       /**<   Set to the size of mcm_agps_status_t. */
    E_QL_LOC_AGPS_TYPE_T            type;       /**<   Type. */
    E_QL_LOC_AGPS_STATUS_VALUE_T    status;     /**<   Status. */
    int                             ipv4_addr;  /**<   IPv4 address. */
    char                            ipv6_addr[QL_LOC_IPV6_ADDR_LEN + 1];        /**<   IPv6 address. */
    char                            ssid[QL_LOC_GPS_SSID_BUF_SIZE + 1];         /**<   SSID. */
    char                            password[QL_LOC_GPS_SSID_BUF_SIZE + 1];     /**<   Password. */
}QL_LOC_AGPS_STATUS_T;


#define QL_LOC_NI_SHORT_STRING_MAXLEN   255     /**  NI short string maximum length. */
#define QL_LOC_NI_LONG_STRING_MAXLEN    2047    /**  NI long string maximum length. */
typedef enum 
{
    E_QL_LOC_NI_TYPE_VOICE          = 1,        /**<  Voice.  */
    E_QL_LOC_NI_TYPE_UMTS_SUPL      = 2,        /**<  UMTS SUPL.  */
    E_QL_LOC_NI_TYPE_UMTS_CTRL_PLANE= 3,        /**<  UMTS control plane.  */
}E_QL_LOC_NI_TYPE_T;

typedef enum 
{
    E_QL_LOC_NI_NEED_NOTIFY         = 0x0001,   /**<  NI requires notification.  */
    E_QL_LOC_NI_NEED_VERIFY         = 0x0002,   /**<  NI requires verification.  */
    E_QL_LOC_NI_PRIVACY_OVERRIDE    = 0x0004,   /**<  NI requires privacy override; no notification/minimal trace.  */
}E_QL_LOC_NI_NOTIFY_FLAGS_T;

typedef enum 
{
    E_QL_LOC_NI_RESPONSE_ACCEPT     = 1,        /**<  Accept.  */
    E_QL_LOC_NI_RESPONSE_DENY       = 2,        /**<  Deny.  */
    E_QL_LOC_NI_RESPONSE_NORESP     = 3,        /**<  No response.  */
}E_QL_LOC_NI_USER_RESPONSE_TYPE_T;

typedef enum 
{
    E_QL_LOC_NI_ENC_NONE            = 0,        /**<  None.  */
    E_QL_LOC_NI_ENC_SUPL_GSM_DEFAULT= 1,        /**<  SUPL GSM default.  */
    E_QL_LOC_NI_ENC_SUPL_UTF8       = 2,        /**<  SUPL UTF8.  */
    E_QL_LOC_NI_ENC_SUPL_UCS2       = 3,        /**<  SUPL UCS2.  */
    E_QL_LOC_NI_ENC_UNKNOWN         = -1,       /**<  Unknown.  */
}E_QL_LOC_NI_ENC_TYPE_T;

typedef struct 
{
    uint32_t                            size;               /**<   Set to the size of mcm_gps_ni_notification_t. */
    int                                 notification_id;    /**<   An ID generated by the HAL to associate NI notifications and UI responses.  */
    E_QL_LOC_NI_TYPE_T                  ni_type;            /**<   An NI type used to distinguish different categories of NI events, such as GPS_NI_TYPE_VOICE, GPS_NI_TYPE_UMTS_SUPL, etc.  */
    E_QL_LOC_NI_NOTIFY_FLAGS_T          notify_flags;       /**<   Notification/verification options; combinations of GpsNiNotifyFlags constants.  */
    int                                 timeout;            /**<   Timeout period to wait for a user response. Set to 0 for no timeout limit.  */
    E_QL_LOC_NI_USER_RESPONSE_TYPE_T    default_response;   /**<   Default response when the response times out.  */
    char                                requestor_id[QL_LOC_NI_SHORT_STRING_MAXLEN + 1];    /**<   Requestor ID.  */
    char                                text[QL_LOC_NI_LONG_STRING_MAXLEN + 1];             /**<   Notification message. It can also be used to store the client ID in some cases.  */
    E_QL_LOC_NI_ENC_TYPE_T              requestor_id_encoding;                              /**<   Client ID encoding scheme.  */
    E_QL_LOC_NI_ENC_TYPE_T              text_encoding;                                      /**<   Client name encoding scheme.  */
    char                                extras[QL_LOC_NI_LONG_STRING_MAXLEN + 1];           /**<   Pointer to extra data. Format:\n- key_1 = value_1, - key_2 = value_2 @tablebulletend */
}QL_LOC_NI_NOTIFICATION_INTO_T;

#define     QL_LOC_MAX_SEVER_ADDR_LENGTH    255 /**  Maximum generic server address length for the host name. */
typedef struct 
{/** Indication Message; Indication with the reported XTRA server URLs. */
    char server1[QL_LOC_MAX_SEVER_ADDR_LENGTH + 1];  /**<   server1.*/
    char server2[QL_LOC_MAX_SEVER_ADDR_LENGTH + 1];  /**<   server2.*/
    char server3[QL_LOC_MAX_SEVER_ADDR_LENGTH + 1];  /**<   server3.*/
}QL_LOC_XTRA_REPORT_SERVER_INTO_T;

typedef enum 
{
    E_QL_LOC_NFY_MSG_ID_STATUS_INFO = 0,        /**<  pv_data = &E_QL_LOC_STATUS_VALUE_T  */
    E_QL_LOC_NFY_MSG_ID_LOCATION_INFO,          /**<  pv_data = &QL_LOC_LOCATION_INFO_T  */
    E_QL_LOC_NFY_MSG_ID_SV_INFO,                /**<  pv_data = &QL_LOC_SV_STATUS_T  */
    E_QL_LOC_NFY_MSG_ID_NMEA_INFO,              /**<  pv_data = &QL_LOC_NMEA_INFO_T  */
    E_QL_LOC_NFY_MSG_ID_CAPABILITIES_INFO,      /**<  pv_data = &E_QL_LOC_CAPABILITIES_T  */
    E_QL_LOC_NFY_MSG_ID_AGPS_STATUS,            /**<  pv_data = &QL_LOC_AGPS_STATUS_T  */
    E_QL_LOC_NFY_MSG_ID_NI_NOTIFICATION,        /**<  pv_data = &QL_LOC_NI_NOTIFICATION_INTO_T  */
    E_QL_LOC_NFY_MSG_ID_XTRA_REPORT_SERVER,     /**<  pv_data = &QL_LOC_XTRA_REPORT_SERVER_INTO_T  */
}E_QL_LOC_NFY_MSG_ID_T;

/* callback function register to QL_LOC_AddRxIndMsgHandler 
   e_msg_id: which kind of msg can be received depend on the bit_mask set in QL_LOC_Set_Indications;
   pv_data: depend on the e_msg_id type.
*/
typedef void (*QL_LOC_RxIndMsgHandlerFunc_t)   
(    
    loc_client_handle_type  h_loc,
    E_QL_LOC_NFY_MSG_ID_T   e_msg_id,
    void                    *pv_data,
    void                    *contextPtr    
);

typedef enum 
{
    E_QL_LOC_DELETE_EPHEMERIS       = (1 <<  0),    /**<  Delete ephemeris data.  */
    E_QL_LOC_DELETE_ALMANAC         = (1 <<  1),    /**<  Delete almanac data.  */
    E_QL_LOC_DELETE_POSITION        = (1 <<  2),    /**<  Delete position data.  */
    E_QL_LOC_DELETE_TIME            = (1 <<  3),    /**<  Delete time data.  */
    E_QL_LOC_DELETE_IONO            = (1 <<  4),    /**<  Delete IONO data.  */
    E_QL_LOC_DELETE_UTC             = (1 <<  5),    /**<  Delete UTC data.  */
    E_QL_LOC_DELETE_HEALTH          = (1 <<  6),    /**<  Delete health data.  */
    E_QL_LOC_DELETE_SVDIR           = (1 <<  7),    /**<  Delete SVDIR data.  */
    E_QL_LOC_DELETE_SVSTEER         = (1 <<  8),    /**<  Delete SVSTEER data.  */
    E_QL_LOC_DELETE_SADATA          = (1 <<  9),    /**<  Delete SA data.  */
    E_QL_LOC_DELETE_RTI             = (1 << 10),    /**<  Delete RTI data.  */
    E_QL_LOC_DELETE_CELLDB_INFO     = (1 << 11),    /**<  Delete cell DB information.  */
    E_QL_LOC_DELETE_ALMANAC_CORR    = (1 << 12),    /**<  Delete almanac correction data.  */
    E_QL_LOC_DELETE_FREQ_BIAS_EST   = (1 << 13),    /**<  Delete frequency bias estimate.  */
    E_QL_LOC_DELETE_EPHEMERIS_GLO   = (1 << 14),    /**<  Delete ephemeris GLO data.  */
    E_QL_LOC_DELETE_ALMANAC_GLO     = (1 << 15),    /**<  Delete almanac GLO data.  */
    E_QL_LOC_DELETE_SVDIR_GLO       = (1 << 16),    /**<  Delete SVDIR GLO data.  */
    E_QL_LOC_DELETE_SVSTEER_GLO     = (1 << 17),    /**<  Delete SVSTEER GLO data.  */
    E_QL_LOC_DELETE_ALMANAC_CORR_GLO= (1 << 18),    /**<  Delete almanac correction GLO data.  */
    E_QL_LOC_DELETE_TIME_GPS        = (1 << 19),    /**<  Delete time GPS data.  */
    E_QL_LOC_DELETE_TIME_GLO        = (1 << 20),    /**<  Delete time GLO data.  */
    E_QL_LOC_DELETE_ALL             = 0xFFFFFFFF,   /**<  Delete all location data.  */
}E_QL_LOC_DELETE_AIDING_DATA_TYPE_T;

typedef struct 
{
    int64_t time;               /**<   Inject time.*/
    int64_t time_reference;     /**<   Time reference.*/
    int32_t uncertainty;        /**<   Uncertainty.*/
}QL_LOC_INJECT_TIME_INTO_T;  /* Message */

typedef struct 
{
    double  latitude;   /**<   Latitude.*/
    double  longitude;  /**<   Longitude.*/
    float   accuracy;   /**<   Accuracy.*/
}QL_LOC_INJECT_LOCATION_INTO_T;

typedef enum 
{
    E_QL_LOC_AGPS_APN_BEARER_INVALID= -1,   /**<  Invalid.  */
    E_QL_LOC_AGPS_APN_BEARER_IPV4   = 0,    /**<  IPv4.  */
    E_QL_LOC_AGPS_APN_BEARER_IPV6   = 1,    /**<  IPv6.  */
    E_QL_LOC_AGPS_APN_BEARER_IPV4V6 = 2,    /**<  IPv4/v6.  */
}E_QL_LOC_AGPS_APN_BEARER_TYPE_T;

#define QL_LOC_APN_NAME_LENGTH_MAX 100
typedef struct 
{
    E_QL_LOC_AGPS_TYPE_T                e_agps_type;                        /**<   AGPS type.*/
    char                                apn[QL_LOC_APN_NAME_LENGTH_MAX + 1];/**<   APN.*/
    E_QL_LOC_AGPS_APN_BEARER_TYPE_T     e_bearer_type;                      /**<   Bearer type.*/
}QL_LOC_AGPS_DATA_CONN_OPEN_INTO_T;

#define QL_LOC_SEVER_ADDR_LENGTH_MAX    255
typedef struct 
{
    E_QL_LOC_AGPS_TYPE_T    e_agps_type;                                /**<   AGPS type.*/
    char                    host_name[QL_LOC_SEVER_ADDR_LENGTH_MAX + 1];/**<   Host name.*/
    uint32_t                port;                                       /**<   Port.*/
}QL_LOC_AGPS_SERVER_INTO_T;

typedef struct 
{
    int32_t                             notify_id;  /**<   Notification ID.*/
    E_QL_LOC_NI_USER_RESPONSE_TYPE_T    user_resp;  /**<   User response.*/
}QL_LOC_NI_RESPONSE_INTO_T;



/* Add callback function if anything changed specified by the mask in QL_LOC_Set_Indications*/
int QL_LOC_AddRxIndMsgHandler(QL_LOC_RxIndMsgHandlerFunc_t handlerPtr, void* contextPtr);

/* Set what we want callbacks for, for the detail mean of bit_mask, please refer to the macro of LOC_IND_xxxxx_INFO_ON */
int QL_LOC_Set_Indications(loc_client_handle_type h_loc, int bit_mask);

/* Set GPS position mode, detail info please refer to QL_LOC_POS_MODE_INFO_T */
int QL_LOC_Set_Position_Mode(loc_client_handle_type h_loc, QL_LOC_POS_MODE_INFO_T *pt_mode);

/* Start navigation, same as AT+QGPS=1, NMEA port start outputing nmea data */
int QL_LOC_Start_Navigation(loc_client_handle_type h_loc);

/* Stop navigation, same as AT+QGPSEND,  NMEA port stop outputing nmea data */
int QL_LOC_Stop_Navigation(loc_client_handle_type h_loc);

/* Set up GPS connection and start getting coordinates , this api will call QL_LOC_Start_Navigation inside first,
   then wait for LOC_IND_LOCATION_INFO_ON event happen, if timeout, it will use last time stored loc_info. 
   If got this event, location infor will save to pt_loc_info and call QL_LOC_Stop_Navigation stop and exit. */
int QL_LOC_Get_Current_Location(loc_client_handle_type h_loc, QL_LOC_LOCATION_INFO_T *pt_loc_info, int timeout_sec);

/* Init LOC module and return the h_loc, this should be called before any other QL_LOC_xxx api. */
int QL_LOC_Client_Init(loc_client_handle_type  *ph_loc);

/* DeInit LOC module and release resource, this should be called at last. */
int QL_LOC_Client_Deinit(loc_client_handle_type h_loc);

/* Deletes location-aiding data. */
int QL_LOC_Delete_Aiding_Data(  loc_client_handle_type              h_loc, 
                                E_QL_LOC_DELETE_AIDING_DATA_TYPE_T  flags);

/* Injects time. */
int QL_LOC_InjectTime(  loc_client_handle_type      h_loc,
                        QL_LOC_INJECT_TIME_INTO_T   *pt_info);

/* Injects Location. */
int QL_LOC_InjectLocation( loc_client_handle_type           h_loc,
                           QL_LOC_INJECT_LOCATION_INTO_T    *pt_info);

/** Injects XTRA data.
    Since the IPC mechanism puts a limit on the size of the data transferable in one message at 64 KB, 
    the application using this command must break the data down into chunks of a smaller size and repeatedly 
    call this API until all the data has been injected. */
int QL_LOC_Xtra_InjectData(loc_client_handle_type h_loc,
                            char                   *data,
                            int                    length); // QL_LOC_XTRA_FILE_DATA_SIZE_MAX

#define QL_LOC_XTRA_FILE_DATA_SIZE_MAX     0xFC00
/* Injects xtra data via file name. */
int QL_LOC_Xtra_InjectFile( loc_client_handle_type  h_loc,
                            char                    *filename);

/** Indicates that the AGPS data connection is open. */
int QL_LOC_Agps_DataConnOpen( loc_client_handle_type            h_loc,
                              QL_LOC_AGPS_DATA_CONN_OPEN_INTO_T *pt_info);

/** Indicates that the AGPS data connection is closed. */
int QL_LOC_Agps_DataConnClose(loc_client_handle_type    h_loc,
                              E_QL_LOC_AGPS_TYPE_T      atype);

/** Indicates that the AGPS data connection failed to start. */
int QL_LOC_Agps_NfyDataConnFailed(loc_client_handle_type   h_loc,
                                  E_QL_LOC_AGPS_TYPE_T     atype);

/** Set AGPS server infor. */
int QL_LOC_Agps_SetServer(loc_client_handle_type        h_loc,
                          QL_LOC_AGPS_SERVER_INTO_T     *pt_info);

/** Sends a user response for NI. */
int QL_LOC_NI_SetResponse(loc_client_handle_type        h_loc,
                          QL_LOC_NI_RESPONSE_INTO_T     *pt_info);

/** Updates the network availability status. */
int QL_LOC_Agps_UpdateNWAvailability(loc_client_handle_type  h_loc,
                                     int                     available,
                                     const char             *apn);

/*
Usage 1 (Normally use): 
1, QL_LOC_Client_Init
2, QL_LOC_AddRxIndMsgHandler(pf_cb)
3, QL_LOC_Set_Indications
4, QL_LOC_Start_Navigation
5, handle the events in pf_cb
6, QL_LOC_Stop_Navigation
7, QL_LOC_Client_Deinit


Usage 2 (Just get position once): 
1, QL_LOC_Client_Init
2, QL_LOC_AddRxIndMsgHandler(pf_cb) ---- This can be omitted!
3, QL_LOC_Set_Indications, set bit_mask=LOC_IND_LOCATION_INFO_ON
4, QL_LOC_Get_Current_Location, if not timeout, it will return current position infor or use last stored one.
7, QL_LOC_Client_Deinit
*/

#endif // __QL_MCM_GPS_H__

