#ifndef __QL_MCM_H__
#define __QL_MCM_H__


#define MCM_SERVICE_MAX             (17)

#define MCM_SERVICE_DATA            0x01
#define MCM_SERVICE_DM              0x02
#define MCM_SERVICE_LOC             0x03
#define MCM_SERVICE_MOBILEAP        0x04
#define MCM_SERVICE_NW              0x05
#define MCM_SERVICE_ATC             0x06
#define MCM_SERVICE_SMS             0x07
#define MCM_SERVICE_CLIENT          0x08
#define MCM_SERVICE_SIM             0x0B
#define MCM_SERVICE_VCALL           0x10


#define SERVICE_REQUIRED            1
#define SERVICE_NOT_REQUIRED        0

#define MAGIC_MSG_ID_SERVICE_UP     0x87654321
#define MAGIC_MSG_ID_SERVICE_DOWN   0x87654320

typedef enum 
{
    E_QL_SUCCESS                        = 0,    /**<  Success. */
    E_QL_SUCCESS_CONDITIONAL_SUCCESS    = 1,    /**<  Conditional success. */
    E_QL_ERROR_MCM_SERVICES_NOT_AVAILABLE = 2,  /**<  MCM services not available. */
    E_QL_ERROR_GENERIC                  = 3,    /**<  Generic error. */
    E_QL_ERROR_BADPARM                  = 4,    /**<  Bad parameter. */
    E_QL_ERROR_MEMORY                   = 5,    /**<  Memory error. */
    E_QL_ERROR_INVALID_STATE            = 6,    /**<  Invalid state. */
    E_QL_ERROR_MALFORMED_MSG            = 7,    /**<  Malformed message. */
    E_QL_ERROR_NO_MEMORY                = 8,    /**<  No memory. */
    E_QL_ERROR_INTERNAL                 = 9,    /**<  Internal error. */
    E_QL_ERROR_ABORTED                  = 10,   /**<  Action was aborted. */
    E_QL_ERROR_CLIENT_IDS_EXHAUSTED     = 11,   /**<  Client IDs have been exhausted. */
    E_QL_ERROR_UNABORTABLE_TRANSACTION  = 12,   /**<  Unabortable transaction. */
    E_QL_ERROR_INVALID_CLIENT_ID        = 13,   /**<  Invalid client ID. */
    E_QL_ERROR_NO_THRESHOLDS            = 14,   /**<  No thresholds. */
    E_QL_ERROR_INVALID_HANDLE           = 15,   /**<  Invalid handle. */
    E_QL_ERROR_INVALID_PROFILE          = 16,   /**<  Invalid profile. */
    E_QL_ERROR_INVALID_PINID            = 17,   /**<  Invalid PIN ID. */
    E_QL_ERROR_INCORRECT_PIN            = 18,   /**<  Incorrect PIN. */
    E_QL_ERROR_NO_NETWORK_FOUND         = 19,   /**<  No network found. */
    E_QL_ERROR_CALL_FAILED              = 20,   /**<  Call failed. */
    E_QL_ERROR_OUT_OF_CALL              = 21,   /**<  Out of call. */
    E_QL_ERROR_NOT_PROVISIONED          = 22,   /**<  Not provisioned. */
    E_QL_ERROR_MISSING_ARG              = 23,   /**<  Missing argument. */
    E_QL_ERROR_ARG_TOO_LONG             = 24,   /**<  Argument is too long. */
    E_QL_ERROR_INVALID_TX_ID            = 25,   /**<  Invalid Tx ID. */
    E_QL_ERROR_DEVICE_IN_USE            = 26,   /**<  Device is in use. */
    E_QL_ERROR_OP_NETWORK_UNSUPPORTED   = 27,   /**<  OP network is not supported. */
    E_QL_ERROR_OP_DEVICE_UNSUPPORTED    = 28,   /**<  OP device is not supported. */
    E_QL_ERROR_NO_EFFECT                = 29,   /**<  No effect. */
    E_QL_ERROR_NO_FREE_PROFILE          = 30,   /**<  No free profile. */
    E_QL_ERROR_INVALID_PDP_TYPE         = 31,   /**<  Invalid PDP type. */
    E_QL_ERROR_INVALID_TECH_PREF        = 32,   /**<  Invalid technical preference. */
    E_QL_ERROR_INVALID_PROFILE_TYPE     = 33,   /**<  Invalid profile type. */
    E_QL_ERROR_INVALID_SERVICE_TYPE     = 34,   /**<  Invalid service type. */
    E_QL_ERROR_INVALID_REGISTER_ACTION  = 35,   /**<  Invalid register action. */
    E_QL_ERROR_INVALID_PS_ATTACH_ACTION = 36,   /**<  Invalid PS attach action. */
    E_QL_ERROR_AUTHENTICATION_FAILED    = 37,   /**<  Authentication failed. */
    E_QL_ERROR_PIN_BLOCKED              = 38,   /**<  PIN is blocked. */
    E_QL_ERROR_PIN_PERM_BLOCKED         = 39,   /**<  PIN is permanently blocked. */
    E_QL_ERROR_SIM_NOT_INITIALIZED      = 40,   /**<  SIM is not initialized. */
    E_QL_ERROR_MAX_QOS_REQUESTS_IN_USE  = 41,   /**<  Maximum QoS requests are in use. */
    E_QL_ERROR_INCORRECT_FLOW_FILTER    = 42,   /**<  Incorrect flow filter. */
    E_QL_ERROR_NETWORK_QOS_UNAWARE      = 43,   /**<  Network QoS is unaware. */
    E_QL_ERROR_INVALID_ID               = 44,   /**<  Invalid ID. */
    E_QL_ERROR_INVALID_QOS_ID           = 45,   /**<  Invalid QoS ID. */
    E_QL_ERROR_REQUESTED_NUM_UNSUPPORTED= 46,   /**<  Requested number is not supported. */
    E_QL_ERROR_INTERFACE_NOT_FOUND      = 47,   /**<  Interface was not found. */
    E_QL_ERROR_FLOW_SUSPENDED           = 48,   /**<  Flow is suspended. */
    E_QL_ERROR_INVALID_DATA_FORMAT      = 49,   /**<  Invalid data format. */
    E_QL_ERROR_GENERAL                  = 50,   /**<  General error. */
    E_QL_ERROR_UNKNOWN                  = 51,   /**<  Unknown error. */
    E_QL_ERROR_INVALID_ARG              = 52,   /**<  Invalid argument. */
    E_QL_ERROR_INVALID_INDEX            = 53,   /**<  Invalid index. */
    E_QL_ERROR_NO_ENTRY                 = 54,   /**<  No entry. */
    E_QL_ERROR_DEVICE_STORAGE_FULL      = 55,   /**<  Device storage is full. */
    E_QL_ERROR_DEVICE_NOT_READY         = 56,   /**<  Device is not ready. */
    E_QL_ERROR_NETWORK_NOT_READY        = 57,   /**<  Network is not ready. */
    E_QL_ERROR_CAUSE_CODE               = 58,   /**<  Cause code error. */
    E_QL_ERROR_MESSAGE_NOT_SENT         = 59,   /**<  Message was not sent. */
    E_QL_ERROR_MESSAGE_DELIVERY_FAILURE = 60,   /**<  Message delivery failure. */
    E_QL_ERROR_INVALID_MESSAGE_ID       = 61,   /**<  Invalid message ID. */
    E_QL_ERROR_ENCODING                 = 62,   /**<  Encoding error. */
    E_QL_ERROR_AUTHENTICATION_LOCK      = 63,   /**<  Authentication lock error. */
    E_QL_ERROR_INVALID_TRANSITION       = 64,   /**<  Invalid transition. */
    E_QL_ERROR_NOT_A_MCAST_IFACE        = 65,   /**<  Not an MCast interface. */
    E_QL_ERROR_MAX_MCAST_REQUESTS_IN_USE= 66,   /**<  Maximum MCast requests are in use. */
    E_QL_ERROR_INVALID_MCAST_HANDLE     = 67,   /**<  Invalid MCast handle. */
    E_QL_ERROR_INVALID_IP_FAMILY_PREF   = 68,   /**<  Invalid IP family preference. */
    E_QL_ERROR_SESSION_INACTIVE         = 69,   /**<  Session is inactive. */
    E_QL_ERROR_SESSION_INVALID          = 70,   /**<  Session is invalid. */
    E_QL_ERROR_SESSION_OWNERSHIP        = 71,   /**<  Session ownership error. */
    E_QL_ERROR_INSUFFICIENT_RESOURCES   = 72,   /**<  Insufficient resources. */
    E_QL_ERROR_DISABLED                 = 73,   /**<  Disabled. */
    E_QL_ERROR_INVALID_OPERATION        = 74,   /**<  Invalid operation. */
    E_QL_ERROR_INVALID_CMD              = 75,   /**<  Invalid command. */
    E_QL_ERROR_TPDU_TYPE                = 76,   /**<  Transfer Protocol data unit type error. */
    E_QL_ERROR_SMSC_ADDR                = 77,   /**<  Short message service center address error. */
    E_QL_ERROR_INFO_UNAVAILABLE         = 78,   /**<  Information is not available. */
    E_QL_ERROR_SEGMENT_TOO_LONG         = 79,   /**<  Segment is too long. */
    E_QL_ERROR_SEGMENT_ORDER            = 80,   /**<  Segment order error. */
    E_QL_ERROR_BUNDLING_NOT_SUPPORTED   = 81,   /**<  Bundling is not supported. */
    E_QL_ERROR_OP_PARTIAL_FAILURE       = 82,   /**<  OP partial failure. */
    E_QL_ERROR_POLICY_MISMATCH          = 83,   /**<  Policy mismatch. */
    E_QL_ERROR_SIM_FILE_NOT_FOUND       = 84,   /**<  SIM file was not found. */
    E_QL_ERROR_EXTENDED_INTERNAL        = 85,   /**<  Extended internal error. */
    E_QL_ERROR_ACCESS_DENIED            = 86,   /**<  Access is denied. */
    E_QL_ERROR_HARDWARE_RESTRICTED      = 87,   /**<  Hardware is restricted. */
    E_QL_ERROR_ACK_NOT_SENT             = 88,   /**<  Acknowledgement was not sent. */
    E_QL_ERROR_INJECT_TIMEOUT           = 89,   /**<  Inject timeout error. */
    E_QL_ERROR_INCOMPATIBLE_STATE       = 90,   /**<  Incompatible state. */
    E_QL_ERROR_FDN_RESTRICT             = 91,   /**<  Fixed dialing number restrict error. */
    E_QL_ERROR_SUPS_FAILURE_CAUSE       = 92,   /**<  SUPS failure cause. */
    E_QL_ERROR_NO_RADIO                 = 93,   /**<  No radio. */
    E_QL_ERROR_NOT_SUPPORTED            = 94,   /**<  Not supported. */
    E_QL_ERROR_NO_SUBSCRIPTION          = 95,   /**<  No subscription. */
    E_QL_ERROR_CARD_CALL_CONTROL_FAILED = 96,   /**<  Card call control failed. */
    E_QL_ERROR_NETWORK_ABORTED          = 97,   /**<  Network was aborted. */
    E_QL_ERROR_MSG_BLOCKED              = 98,   /**<  Message was blocked. */
    E_QL_ERROR_INVALID_SESSION_TYPE     = 99,   /**<  Invalid session type. */
    E_QL_ERROR_INVALID_PB_TYPE          = 100,  /**<  Invalid phonebook type. */
    E_QL_ERROR_NO_SIM                   = 101,  /**<  No SIM was found. */
    E_QL_ERROR_PB_NOT_READY             = 102,  /**<  Phonebook not ready. */
    E_QL_ERROR_PIN_RESTRICTION          = 103,  /**<  PIN restriction. */
    E_QL_ERROR_PIN2_RESTRICTION         = 104,  /**<  PIN2 restriction. */
    E_QL_ERROR_PUK_RESTRICTION          = 105,  /**<  PIN unlocking key restriction. */
    E_QL_ERROR_PUK2_RESTRICTION         = 106,  /**<  PIN unlocking key2 restriction. */
    E_QL_ERROR_PB_ACCESS_RESTRICTED     = 107,  /**<  Phonebook access is restricted. */
    E_QL_ERROR_PB_DELETE_IN_PROG        = 108,  /**<  Phonebook delete is in progress. */
    E_QL_ERROR_PB_TEXT_TOO_LONG         = 109,  /**<  Phonebook text is too long. */
    E_QL_ERROR_PB_NUMBER_TOO_LONG       = 110,  /**<  Phonebook number is too long. */
    E_QL_ERROR_PB_HIDDEN_KEY_RESTRICTION= 111,  /**<  Phonebook hidden key restriction. */
    E_QL_ERROR_PB_NOT_AVAILABLE         = 112,  /**<  Phonebook is not available. */
    E_QL_ERROR_DEVICE_MEMORY_ERROR      = 113,  /**<  Device memory error. */
    E_QL_ERROR_SIM_PIN_BLOCKED          = 114,  /**<  SIM PIN is blocked. */
    E_QL_ERROR_SIM_PIN_NOT_INITIALIZED  = 115,  /**<  SIM PIN is not initialized. */
    E_QL_ERROR_SIM_INVALID_PIN          = 116,  /**<  SIM PIN is invalid. */
    E_QL_ERROR_SIM_INVALID_PERSO_CK     = 117,  /**<  SIM invalid personalization CK. */
    E_QL_ERROR_SIM_PERSO_BLOCKED        = 118,  /**<  SIM personalization blocked. */
    E_QL_ERROR_SIM_PERSO_INVALID_DATA   = 119,  /**<  SIM personalization contains invalid data. */
    E_QL_ERROR_SIM_ACCESS_DENIED        = 120,  /**<  SIM access is denied. */
    E_QL_ERROR_SIM_INVALID_FILE_PATH    = 121,  /**<  SIM file path is invalid. */
    E_QL_ERROR_SIM_SERVICE_NOT_SUPPORTED= 122,  /**<  SIM service is not supported. */
    E_QL_ERROR_SIM_AUTH_FAIL            = 123,  /**<  SIM authorization failure. */
    E_QL_ERROR_SIM_PIN_PERM_BLOCKED     = 124,  /**<  SIM PIN is permanently blocked. */
}E_QL_ERROR_CODE_T;

int QL_MCM_Client_Init( mcm_client_handle_type  *ph_mcm, 
                        int                     client_type,                       
                        mcm_client_ind_cb       client_ind_cb,
                        mcm_client_async_cb     client_async_cb);

int QL_MCM_Client_Deinit(mcm_client_handle_type h_mcm,
                         int                    client_type);

#endif // __QL_MCM_H__

