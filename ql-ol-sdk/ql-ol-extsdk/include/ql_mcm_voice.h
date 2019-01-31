/**
 *@file     ql_mcm_voice.h
 *@date     2017-11-30
 *@author   
 *@brief    
 */

#ifndef __QL_MCM_VOICE_H__
#define __QL_MCM_VOICE_H__



#define QL_MCM_MAX_VOICE_CALLS                  8   /**  GSM provides up to 8 calls; 3GPP2 provides 2. */
#define QL_MCM_MAX_PHONE_NUMBER                 82  /**  Maximum phone number length. */
#define QL_MCM_MAX_UUS_DATA                     20  /**  Maximum user-to-user data. */
#define QL_MCM_MAX_DTMF_LENGTH                  20  /**  Maximum DTMF length. */
#define QL_MCM_MAX_USSD_LENGTH                  128 /**  Maximum USSD length. */
#define QL_MCM_MAX_PASSWORD_LENGTH              4   /**  Maximum password length. */
#define QL_MCM_MAX_CALL_FORWARDING_INFO         13  /**  Maximum call forwarding information. */
#define QL_MCM_MAX_ECALL_MSD                    140 /**  Maximum size of the MSD sent to the network with an eCall */
#define QL_MCM_MAX_ECALL_URC_EVENT_LENGTH       128 /**  Maximum size of the ecall urc event */


typedef enum 
{
    E_QL_MCM_VOICE_CALL_STATE_INCOMING      = 0x0000, /**< MT incoming; CC setup. */
    E_QL_MCM_VOICE_CALL_STATE_DIALING       = 0x0001, /**< Dialing state. */
    E_QL_MCM_VOICE_CALL_STATE_ALERTING      = 0x0002, /**< MT call waiting; MO alterting. */
    E_QL_MCM_VOICE_CALL_STATE_ACTIVE        = 0x0003, /**< Call is active. */
    E_QL_MCM_VOICE_CALL_STATE_HOLDING       = 0x0004, /**< Call is on hold. */
    E_QL_MCM_VOICE_CALL_STATE_END           = 0x0005, /**< Call is disconnected. */
    E_QL_MCM_VOICE_CALL_STATE_WAITING       = 0x0006, /**< Call is waiting. */
}ql_mcm_voice_call_state_t;

typedef enum 
{
    E_QL_MCM_VOICE_TECH_NONE                = 0, /**<  None. */
    E_QL_MCM_VOICE_TECH_3GPP                = 1, /**<  3GPP. */
    E_QL_MCM_VOICE_TECH_3GPP2               = 2, /**<  3GPP2. */
}ql_mcm_voice_tech_t;

typedef enum 
{
    E_QL_MCM_VOICE_CALL_NUMBER_UNKNOWN      = 0x0000, 
    E_QL_MCM_VOICE_CALL_NUMBER_ALLOWED      = 0x0001, /**<  Number allowed. */
    E_QL_MCM_VOICE_CALL_NUMBER_RESTRICTED   = 0x0002, /**<  Number restricted. */
    E_QL_MCM_VOICE_CALL_NUMBER_PAYPHONE     = 0x0003, /**<  Payhone number. */
}ql_mcm_voice_call_number_presentation_type_t;

typedef enum 
{
    E_QL_MCM_VOICE_CALL_MOBILE_ORIGINATED   = 0x0000, /**<  Mobile-originated. */
    E_QL_MCM_VOICE_CALL_MOBILE_TERMINATED   = 0x0001, /**<  Mobile-terminated. */
}ql_mcm_voice_call_direction_type_t;

typedef enum 
{
    E_QL_MCM_VOICE_CALL_ANSWER,     //!<Answer the call.
    E_QL_MCM_VOICE_CALL_END,        //!<Deprecated. Do not use.
    E_QL_MCM_VOICE_CALL_HOLD,       //!<Hold the call.
    E_QL_MCM_VOICE_CALL_UNHOLD,     //!<Release the call from hold.
    E_QL_MCM_VOICE_CALL_CONFERENCE, //!<Conference call.
    E_QL_MCM_VOICE_CALL_GO_PRIVATE, //!<Private call.
    E_QL_MCM_VOICE_CALL_END_ALL     //!<End all calls.
} ql_mcm_voice_call_operation_t;       //!< Voice call operation type.

typedef enum 
{
    E_QL_MCM_VOICE_CALL_END_REASON_NONE                     = 0, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_OFFLINE                   = 1, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CDMA_LOCK                 = 2, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NO_SRV                    = 3, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_FADE                      = 4, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INTERCEPT                 = 5, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REORDER                   = 6, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REL_NORMAL                = 7, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REL_SO_REJ                = 8, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INCOM_CALL                = 9, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ALERT_STOP                = 10, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CLIENT_END                = 11, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACTIVATION                = 12, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_MC_ABORT                  = 13, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_MAX_ACCESS_PROBE          = 14, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_PSIST_N                   = 15, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_UIM_NOT_PRESENT           = 16, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACC_IN_PROG               = 17, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACC_FAIL                  = 18, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_RETRY_ORDER               = 19, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CCS_NOT_SUPPORTED_BY_BS   = 20, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NO_RESPONSE_FROM_BS       = 21, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REJECTED_BY_BS            = 22, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INCOMPATIBLE              = 23, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACCESS_BLOCK              = 24, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ALREADY_IN_TC             = 25, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_EMERGENCY_FLASHED         = 26, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_USER_CALL_ORIG_DURING_GPS = 27, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_USER_CALL_ORIG_DURING_SMS = 28, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_USER_CALL_ORIG_DURING_DATA= 29, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REDIR_OR_HANDOFF          = 30, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACCESS_BLOCK_ALL          = 31, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_OTASP_SPC_ERR             = 32, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_IS707B_MAX_ACC            = 33, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACC_FAIL_REJ_ORD          = 34, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACC_FAIL_RETRY_ORD        = 35, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_TIMEOUT_T42               = 36, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_TIMEOUT_T40               = 37, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SRV_INIT_FAIL             = 38, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_T50_EXP                   = 39, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_T51_EXP                   = 40, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_RL_ACK_TIMEOUT            = 41, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_BAD_FL                    = 42, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_TRM_REQ_FAIL              = 43, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_TIMEOUT_T41               = 44, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INCOM_REJ                 = 45, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SETUP_REJ                 = 46, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NETWORK_END               = 47, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NO_FUNDS                  = 48, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NO_GW_SRV                 = 49, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NO_CDMA_SRV               = 50, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NO_FULL_SRV               = 51, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_MAX_PS_CALLS              = 52, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_UNKNOWN_SUBSCRIBER        = 53, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ILLEGAL_SUBSCRIBER        = 54, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_BEARER_SERVICE_NOT_PROVISIONED= 55, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_TELE_SERVICE_NOT_PROVISIONED  = 56, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ILLEGAL_EQUIPMENT         = 57, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CALL_BARRED               = 58, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ILLEGAL_SS_OPERATION      = 59, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SS_ERROR_STATUS           = 60, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SS_NOT_AVAILABLE          = 61, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SS_SUBSCRIPTION_VIOLATION = 62, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SS_INCOMPATIBILITY        = 63, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_FACILITY_NOT_SUPPORTED    = 64, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ABSENT_SUBSCRIBER         = 65, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SHORT_TERM_DENIAL         = 66, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_LONG_TERM_DENIAL          = 67, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SYSTEM_FAILURE            = 68, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_DATA_MISSING              = 69, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_UNEXPECTED_DATA_VALUE     = 70, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_PWD_REGISTRATION_FAILURE  = 71, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NEGATIVE_PWD_CHECK        = 72, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NUM_OF_PWD_ATTEMPTS_VIOLATION     = 73, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_POSITION_METHOD_FAILURE   = 74, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_UNKNOWN_ALPHABET          = 75, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_USSD_BUSY                 = 76, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REJECTED_BY_USER          = 77, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REJECTED_BY_NETWORK       = 78, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_DEFLECTION_TO_SERVED_SUBSCRIBER   = 79, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SPECIAL_SERVICE_CODE      = 80, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INVALID_DEFLECTED_TO_NUMBER       = 81, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_MPTY_PARTICIPANTS_EXCEEDED= 82, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_RESOURCES_NOT_AVAILABLE   = 83, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_UNASSIGNED_NUMBER         = 84, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NO_ROUTE_TO_DESTINATION   = 85, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CHANNEL_UNACCEPTABLE      = 86, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_OPERATOR_DETERMINED_BARRING       = 87, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NORMAL_CALL_CLEARING      = 88, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_USER_BUSY                 = 89, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NO_USER_RESPONDING        = 90, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_USER_ALERTING_NO_ANSWER   = 91, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CALL_REJECTED             = 92, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NUMBER_CHANGED            = 93, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_PREEMPTION                = 94, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_DESTINATION_OUT_OF_ORDER  = 95, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INVALID_NUMBER_FORMAT     = 96, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_FACILITY_REJECTED         = 97, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_RESP_TO_STATUS_ENQUIRY    = 98, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NORMAL_UNSPECIFIED        = 99, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NO_CIRCUIT_OR_CHANNEL_AVAILABLE   = 100, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NETWORK_OUT_OF_ORDER      = 101, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_TEMPORARY_FAILURE         = 102, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SWITCHING_EQUIPMENT_CONGESTION    = 103, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACCESS_INFORMATION_DISCARDED      = 104, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REQUESTED_CIRCUIT_OR_CHANNEL_NOT_AVAILABLE = 105, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_RESOURCES_UNAVAILABLE_OR_UNSPECIFIED = 106, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_QOS_UNAVAILABLE                   = 107, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REQUESTED_FACILITY_NOT_SUBSCRIBED = 108, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INCOMING_CALLS_BARRED_WITHIN_CUG  = 109, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_BEARER_CAPABILITY_NOT_AUTH        = 110, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_BEARER_CAPABILITY_UNAVAILABLE     = 111, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SERVICE_OPTION_NOT_AVAILABLE      = 112, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACM_LIMIT_EXCEEDED                = 113, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_BEARER_SERVICE_NOT_IMPLEMENTED    = 114, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REQUESTED_FACILITY_NOT_IMPLEMENTED= 115, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ONLY_DIGITAL_INFORMATION_BEARER_AVAILABLE = 116, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SERVICE_OR_OPTION_NOT_IMPLEMENTED = 117, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INVALID_TRANSACTION_IDENTIFIER    = 118, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_USER_NOT_MEMBER_OF_CUG            = 119, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INCOMPATIBLE_DESTINATION          = 120, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INVALID_TRANSIT_NW_SELECTION      = 121, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE    = 122, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INVALID_MANDATORY_INFORMATION     = 123, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_MESSAGE_TYPE_NON_IMPLEMENTED      = 124, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE = 125, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INFORMATION_ELEMENT_NON_EXISTENT  = 126, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CONDITONAL_IE_ERROR               = 127, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE = 128, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_RECOVERY_ON_TIMER_EXPIRED         = 129, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_PROTOCOL_ERROR_UNSPECIFIED        = 130, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INTERWORKING_UNSPECIFIED          = 131, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_OUTGOING_CALLS_BARRED_WITHIN_CUG  = 132, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NO_CUG_SELECTION                  = 133, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_UNKNOWN_CUG_INDEX                 = 134, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CUG_INDEX_INCOMPATIBLE            = 135, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CUG_CALL_FAILURE_UNSPECIFIED      = 136, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CLIR_NOT_SUBSCRIBED               = 137, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CCBS_POSSIBLE                     = 138, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CCBS_NOT_POSSIBLE                 = 139, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_IMSI_UNKNOWN_IN_HLR               = 140, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ILLEGAL_MS                        = 141, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_IMSI_UNKNOWN_IN_VLR               = 142, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_IMEI_NOT_ACCEPTED                 = 143, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ILLEGAL_ME                        = 144, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_PLMN_NOT_ALLOWED                  = 145, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_LOCATION_AREA_NOT_ALLOWED         = 146, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ROAMING_NOT_ALLOWED_IN_THIS_LOCATION_AREA = 147, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NO_SUITABLE_CELLS_IN_LOCATION_AREA= 148, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NETWORK_FAILURE                   = 149, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_MAC_FAILURE                       = 150, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SYNCH_FAILURE                     = 151, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NETWORK_CONGESTION                = 152, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_GSM_AUTHENTICATION_UNACCEPTABLE   = 153, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SERVICE_NOT_SUBSCRIBED            = 154, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SERVICE_TEMPORARILY_OUT_OF_ORDER  = 155, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CALL_CANNOT_BE_IDENTIFIED         = 156, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INCORRECT_SEMANTICS_IN_MESSAGE    = 157, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_MANDATORY_INFORMATION_INVALID     = 158, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACCESS_STRATUM_FAILURE            = 159, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INVALID_SIM                       = 160, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_WRONG_STATE                       = 161, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACCESS_CLASS_BLOCKED              = 162, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NO_RESOURCES                      = 163, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INVALID_USER_DATA                 = 164, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_TIMER_T3230_EXPIRED               = 165, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NO_CELL_AVAILABLE                 = 166, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ABORT_MSG_RECEIVED                = 167, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_RADIO_LINK_LOST                   = 168, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_TIMER_T303_EXPIRED                = 169, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CNM_MM_REL_PENDING                = 170, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACCESS_STRATUM_REJ_RR_REL_IND     = 171, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACCESS_STRATUM_REJ_RR_RANDOM_ACCESS_FAILURE   = 172, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACCESS_STRATUM_REJ_RRC_REL_IND                = 173, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACCESS_STRATUM_REJ_RRC_CLOSE_SESSION_IND      = 174, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACCESS_STRATUM_REJ_RRC_OPEN_SESSION_FAILURE   = 175, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACCESS_STRATUM_REJ_LOW_LEVEL_FAIL             = 176, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACCESS_STRATUM_REJ_LOW_LEVEL_FAIL_REDIAL_NOT_ALLOWED = 177, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACCESS_STRATUM_REJ_LOW_LEVEL_IMMED_RETRY      = 178, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ACCESS_STRATUM_REJ_ABORT_RADIO_UNAVAILABLE    = 179, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SERVICE_OPTION_NOT_SUPPORTED                  = 180, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_UL_DATA_CNF_FAILURE_TXN           = 181, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_UL_DATA_CNF_FAILURE_HO            = 182, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_UL_DATA_CNF_FAILURE_CONN_REL      = 183, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_UL_DATA_CNF_FAILURE_RLF           = 184, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_UL_DATA_CNF_FAILURE_CTRL_NOT_CONN = 185, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_EST_SUCCESS                  = 186, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_EST_FAILURE                  = 187, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_EST_FAILURE_ABORTED          = 188, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_EST_FAILURE_ACCESS_BARRED    = 189, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_EST_FAILURE_CELL_RESEL       = 190, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_EST_FAILURE_CONFIG_FAILURE   = 191, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_EST_FAILURE_TIMER_EXPIRED    = 192, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_EST_FAILURE_LINK_FAILURE     = 193, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_EST_FAILURE_NOT_CAMPED       = 194, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_EST_FAILURE_SI_FAILURE       = 195, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_EST_FAILURE_CONN_REJECT      = 196, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_REL_NORMAL                   = 197, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_REL_RLF                      = 198, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_REL_CRE_FAILURE              = 199, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_REL_OOS_DURING_CRE           = 200, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_REL_ABORTED                  = 201, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_REL_SIB_READ_ERROR           = 202, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_CONN_REL_ABORTED_IRAT_SUCCESS     = 203, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_LRRC_RADIO_LINK_FAILURE                = 204, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_DETACH_WITH_REATTACH_LTE_NW_DETACH     = 205, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AS_REJ_DETACH_WITH_OUT_REATTACH_LTE_NW_DETACH = 206, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_BAD_REQ_WAIT_INVITE           = 207, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_BAD_REQ_WAIT_REINVITE         = 208, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INVALID_REMOTE_URI            = 209, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REMOTE_UNSUPP_MEDIA_TYPE      = 210, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_PEER_NOT_REACHABLE            = 211, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NETWORK_NO_RESP_TIME_OUT      = 212, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NETWORK_NO_RESP_HOLD_FAIL     = 213, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_DATA_CONNECTION_LOST          = 214, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_UPGRADE_DOWNGRADE_REJ         = 215, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SIP_403_FORBIDDEN             = 216, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NO_NETWORK_RESP               = 217, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_UPGRADE_DOWNGRADE_FAILED      = 218, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_UPGRADE_DOWNGRADE_CANCELLED   = 219, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SSAC_REJECT                   = 220, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_THERMAL_EMERGENCY             = 221, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_SOFT_FAILURE           = 222, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_HARD_FAILURE           = 223, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CONNECTION_EST_FAILURE        = 224, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CONNECTION_FAILURE            = 225, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_RRC_CONN_REL_NO_MT_SETUP      = 226, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ESR_FAILURE                   = 227, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_MT_CSFB_NO_RESPONSE_FROM_NW   = 228, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_BUSY_EVERYWHERE               = 229, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ANSWERED_ELSEWHERE            = 230, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_RLF_DURING_CC_DISCONNECT      = 231, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_TEMP_REDIAL_ALLOWED           = 232, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_PERM_REDIAL_NOT_NEEDED        = 233, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_MERGED_TO_CONFERENCE          = 234, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_LOW_BATTERY                   = 235, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CALL_DEFLECTED                = 236, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_RTP_RTCP_TIMEOUT              = 237, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_RINGING_RINGBACK_TIMEOUT      = 238, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REG_RESTORATION               = 239, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CODEC_ERROR                   = 240, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_UNSUPPORTED_SDP               = 241, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_RTP_FAILURE                   = 242, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_QoS_FAILURE                   = 243, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_MULTIPLE_CHOICES              = 244, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_MOVED_PERMANENTLY             = 245, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_MOVED_TEMPORARILY             = 246, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_USE_PROXY                     = 247, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ALTERNATE_SERVICE             = 248, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ALTERNATE_EMERGENCY_CALL      = 249, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_UNAUTHORIZED                  = 250, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_PAYMENT_REQUIRED              = 251, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_METHOD_NOT_ALLOWED            = 252, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NOT_ACCEPTABLE                = 253, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_PROXY_AUTHENTICATION_REQUIRED = 254, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_GONE                          = 255, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REQUEST_ENTITY_TOO_LARGE      = 256, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REQUEST_URI_TOO_LARGE         = 257, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_UNSUPPORTED_URI_SCHEME        = 258, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_BAD_EXTENSION                 = 259, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_EXTENSION_REQUIRED            = 260, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INTERVAL_TOO_BRIEF            = 261, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CALL_OR_TRANS_DOES_NOT_EXIST  = 262, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_LOOP_DETECTED                 = 263, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_TOO_MANY_HOPS                 = 264, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_ADDRESS_INCOMPLETE            = 265, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_AMBIGUOUS                     = 266, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REQUEST_TERMINATED            = 267, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NOT_ACCEPTABLE_HERE           = 268, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REQUEST_PENDING               = 269, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_UNDECIPHERABLE                = 270, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SERVER_INTERNAL_ERROR         = 271, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_NOT_IMPLEMENTED               = 272, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_BAD_GATEWAY                   = 273, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SERVER_TIME_OUT               = 274, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_VERSION_NOT_SUPPORTED         = 275, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_MESSAGE_TOO_LARGE             = 276, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_DOES_NOT_EXIST_ANYWHERE       = 277, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SESS_DESCR_NOT_ACCEPTABLE     = 278, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SRVCC_END_CALL                = 279, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_INTERNAL_ERROR                = 280, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_SERVER_UNAVAILABLE            = 281, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_PRECONDITION_FAILURE          = 282, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_DRVCC_IN_PROG                 = 283, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_DRVCC_END_CALL                = 284, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CS_HARD_FAILURE               = 285, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CS_ACQ_FAILURE                = 286, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_FALLBACK_TO_CS                = 287, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_DEAD_BATTERY                  = 288, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_HO_NOT_FEASIBLE               = 289, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_PDN_DISCONNECTED              = 290, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REJECTED_ELSEWHERE            = 291, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CALL_PULLED                   = 292, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_CALL_PULL_OUT_OF_SYNC         = 293, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_HOLD_RESUME_FAILED            = 294, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_HOLD_RESUME_CANCELED          = 295, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_REINVITE_COLLISION            = 296, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_MSG_INVAILD            = 297, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_MSG_IGNORE             = 298, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_FAIL_ACQ_FAIL          = 299, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_FAIL_CALL_REL_REL_ORDER= 300, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_FAIL_CALL_REL_REORDER  = 301, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_FAIL_CALL_REL_INTERCEPT_ORDER      = 302, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_FAIL_CALL_REL_NORMAL   = 303, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_FAIL_CALL_REL_SO_REJ   = 304, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_FAIL_CALL_REL_OTASP_SPC_ERR        = 305, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_FAILURE_SRCH_TT_FAIL   = 306, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_FAILURE_TCH_INIT_FAIL  = 307, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_FAILURE_FAILURE_USER_CALL_END      = 308, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_FAILURE_FAILURE_RETRY_EXHAUST      = 309, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_FAILURE_FAILURE_CALL_REL_REG_REJ   = 310, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_FAILURE_FAILURE_CALL_REL_NW_REL_ODR= 311, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_1XCSFB_HO_FAILURE                         = 312, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_EMM_REJ_TIMER_T3417_EXT_EXP               = 313, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_EMM_REJ_TIMER_T3417_EXP                   = 314, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_EMM_REJ_SERVICE_REQ_FAILURE_LTE_NW_REJECT = 315, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_EMM_REJ_SERVICE_REQ_FAILURE_CS_DOMAIN_NOT_AVAILABLE = 316, 
    E_QL_MCM_VOICE_CALL_END_CAUSE_EMM_REJ                                   = 317, 
}ql_mcm_voice_call_end_reason_t;

typedef enum 
{
    E_QL_MCM_VOICE_UUS_TYPE1_IMPLICIT,          //!< Type 1 implicit.
    E_QL_MCM_VOICE_UUS_TYPE1_REQUIRED,          //!< Type 1 required.
    E_QL_MCM_VOICE_UUS_TYPE1_NOT_REQUIRED,      //!< Type 1 not required.
    E_QL_MCM_VOICE_UUS_TYPE2_REQUIRED,          //!< Type 2 required.
    E_QL_MCM_VOICE_UUS_TYPE2_NOT_REQUIRED,      //!< Type 2 not required.
    E_QL_MCM_VOICE_UUS_TYPE3_REQUIRED,          //!< Type 3 required.
    E_QL_MCM_VOICE_UUS_TYPE3_NOT_REQUIRED,      //!< Type 3 not required.
    E_QL_MCM_VOICE_UUS_TYPE_DATA                //!< Data.
} ql_mcm_voice_uus_type_t;

typedef enum 
{
    E_QL_MCM_VOICE_UUS_DCS_IA5,                 //!< IA5.
    E_QL_MCM_VOICE_UUS_DCS_OHLP,                //!< OHLP.
    E_QL_MCM_VOICE_UUS_DCS_USP,                 //!< USP.
    E_QL_MCM_VOICE_UUS_DCS_X244                 //!< x244.
} ql_mcm_voice_uus_dcs_type_t;                  //!< UUS data coding scheme. 


typedef struct 
{
    ql_mcm_voice_uus_type_t     type;           /**< UUS type; range -- 0 to 6.*/
    ql_mcm_voice_uus_dcs_type_t dcs;            /**< UUS data coding scheme; range -- 0 to 4.*/
    uint32_t                    uus_data_len;   /**< Must be set to the number of elements in uus_data. */
    uint8_t                     uus_data[QL_MCM_MAX_UUS_DATA];/**< Voice call UUS data.*/
}ql_mcm_voice_uusdata_t;

typedef struct 
{
    uint32_t                                      call_id;                                /**<   Call ID associated with this call.*/
    ql_mcm_voice_call_state_t                     state;                                  /**<   Current call state (mcm_voice_call_state).*/
    ql_mcm_voice_tech_t                           tech;                                   /**<   Technology (mcm_tech).*/
    char                                          number[QL_MCM_MAX_PHONE_NUMBER + 1];    /**<   Phone number.*/
    ql_mcm_voice_call_number_presentation_type_t  number_presentation;                    /**<   Number presentation.*/
    ql_mcm_voice_call_direction_type_t            direction;                              /**<   Voice call direction.*/
    uint8_t                                       uusdata_valid;                          /**<   Indicates whether UUS data is valid.*/
    ql_mcm_voice_uusdata_t                        uusdata;                                /**<   User-to-user signaling data.*/
    ql_mcm_voice_call_end_reason_t                call_end_reason;
}ql_mcm_voice_call_record_t;

typedef struct 
{
    uint32_t                      calls_len;  /**< Must be set to # of elements in calls */
    ql_mcm_voice_call_record_t    calls[QL_MCM_MAX_VOICE_CALLS]; /**<   Calls.*/
}ql_mcm_voice_calls_state_t;


typedef enum 
{
    E_QL_MCM_VOICE_CALL_FORWARDING_DISABLED         = 0, /**<  Disabled. */
    E_QL_MCM_VOICE_CALL_FORWARDING_ENABLED          = 1, /**<  Enabled. */
}QL_MCM_VOICE_CALL_FORWARDING_STATUS_T;

typedef enum 
{
    E_QL_MCM_VOICE_CALL_FORWARDING_TYPE_VOICE       = 0, /**<  Voice. */
    E_QL_MCM_VOICE_CALL_FORWARDING_TYPE_DATA        = 1, /**<  Data. */
    E_QL_MCM_VOICE_CALL_FORWARDING_TYPE_VOICE_DATA  = 2, /**<  Voice and data. */
}QL_MCM_VOICE_CALL_FORWARDING_TYPE_T;

typedef struct 
{
    QL_MCM_VOICE_CALL_FORWARDING_TYPE_T     type;       /**<   Call forwarding type.*/
    char number[QL_MCM_MAX_PHONE_NUMBER + 1];           /**<   Call forwarding number.*/
}QL_MCM_VOICE_CALL_FORWARDING_INFO_T;  /* Type */

typedef struct 
{
    QL_MCM_VOICE_CALL_FORWARDING_STATUS_T     status;     /**<   Call forwarding status.*/
    uint32_t                                  info_len;   /**< Must be set to # of elements in info */
    QL_MCM_VOICE_CALL_FORWARDING_INFO_T       info[QL_MCM_MAX_CALL_FORWARDING_INFO];/**<   Call forwarding information.*/
}ql_mcm_voice_call_forwarding_status_list_t;  /* Message */

typedef enum 
{
    E_QL_MCM_VOICE_SERVICE_UNKOWN       =   0,
    E_QL_MCM_VOICE_SERVICE_REGISTER     =   1,  /**<  Register. */
    E_QL_MCM_VOICE_SERVICE_ERASE        =   2,  /**<  Erase. */
}E_QL_MCM_VOICE_CALL_SERVICE_T;

typedef enum 
{
    E_QL_MCM_VOICE_CALL_FORWARD_UNCONDITIONALLY     = 0, /**<  Unconditional call forwarding. */
    E_QL_MCM_VOICE_CALL_FORWARD_MOBILEBUSY          = 1, /**<  Forward when the mobile device is busy. */
    E_QL_MCM_VOICE_CALL_FORWARD_NOREPLY             = 2, /**<  Forward when there is no reply. */
    E_QL_MCM_VOICE_CALL_FORWARD_UNREACHABLE         = 3, /**<  Forward when the call is unreachable. */
    E_QL_MCM_VOICE_CALL_FORWARD_ALLFORWARDING       = 4, /**<  All forwarding(0-3). */
    E_QL_MCM_VOICE_CALL_FORWARD_ALLCONDITIONAL      = 5, /**<  All conditional forwarding(1-3). */
}E_QL_MCM_VOICE_CALL_FORWARDING_REASON_T;


typedef enum 
{
    E_QL_MCM_VOICE_CALL_WAITING_VOICE_ENABLED       = 0, /**<  Voice call waiting enabled. */
    E_QL_MCM_VOICE_CALL_WAITING_DISABLED            = 1, /**<  Voice call waiting disabled. */
}E_QL_MCM_VOICE_CALL_WAITING_SERVICE_T;

typedef struct 
{
    E_QL_MCM_VOICE_CALL_WAITING_SERVICE_T   waiting_service;
}ql_mcm_voice_call_waiting_service_t;



typedef enum 
{
    E_QL_MCM_VOICE_CLIR_INVOCATION                  = 0, /**<  Invocation. */
    E_QL_MCM_VOICE_CLIR_SUPPRESSION                 = 1, /**<  Suppression. */
}ql_mcm_voice_clir_action_t;

typedef enum 
{
    E_QL_MCM_VOICE_CLIR_NOT_PROVISIONED             = 0, /**<  Not provisioned. */
    E_QL_MCM_VOICE_CLIR_PROVISIONED_PERMANENT_MODE  = 1, /**<  Permanently provisioned. */
    E_QL_MCM_VOICE_CLIR_PRESENTATION_RESTRICTED     = 2, /**<  Restricted presentation. */
    E_QL_MCM_VOICE_CLIR_PRESENTATION_ALLOWED        = 3, /**<  Allowed presentation. */
}ql_mcm_voice_clir_presentation_t;

typedef struct 
{
    ql_mcm_voice_clir_action_t          action;
    ql_mcm_voice_clir_presentation_t    presentation;  /**<   CLIR presentation.*/
}ql_mcm_voice_clir_info_t;



typedef enum 
{
    E_QL_MCM_VOICE_USSD_MSG_TYPE_NEW_MESSAGE        = 0, /**<  Initiate a new USSD sesion with network. */
    E_QL_MCM_VOICE_USSD_MSG_TYPE_REPLY_TO_IND       = 1, /**<  Reply to a USSD indication from the network. */
}ql_mcm_voice_ussd_msg_type_t;

typedef enum 
{
    E_QL_MCM_VOICE_USSD_ENCODING_ASCII              = 0, /**<  ASCII coding scheme. */
    E_QL_MCM_VOICE_USSD_ENCODING_8BIT               = 1, /**<  8-bit coding scheme. */
    E_QL_MCM_VOICE_USSD_ENCODING_UCS2               = 2, /**<  UCS2. */
}ql_mcm_voice_ussd_encoding_type_t;

typedef struct 
{
  ql_mcm_voice_ussd_msg_type_t      type;           /**<   Whether sending a new request or a reply to indication*/
  ql_mcm_voice_ussd_encoding_type_t encoding;       /**<   USSD encoding.*/
  char ussd_string[QL_MCM_MAX_USSD_LENGTH + 1];    /**<   USSD string.*/
}ql_mcm_voice_ussd_info_t;  /* Message */


typedef enum 
{
    E_QL_MCM_SIMRIL_CHANGE_CALL_BARRING_PWD_REASON_ALLOUTGOING           = 0, /**<  All outgoing. */
    E_QL_MCM_SIMRIL_CHANGE_CALL_BARRING_PWD_REASON_OUTGOINGINT           = 1, /**<  Outgoing internal. */
    E_QL_MCM_SIMRIL_CHANGE_CALL_BARRING_PWD_REASON_OUTGOINGINTEXTOHOME   = 2, /**<  Outgoing external to home. */
    E_QL_MCM_SIMRIL_CHANGE_CALL_BARRING_PWD_REASON_ALLINCOMING           = 3, /**<  All incoming. */
    E_QL_MCM_SIMRIL_CHANGE_CALL_BARRING_PWD_REASON_INCOMINGROAMING       = 4, /**<  Roaming incoming. */
    E_QL_MCM_SIMRIL_CHANGE_CALL_BARRING_PWD_REASON_ALLBARRING            = 5, /**<  All calls are barred. */
    E_QL_MCM_SIMRIL_CHANGE_CALL_BARRING_PWD_REASON_ALLOUTGOINGBARRING    = 6, /**<  All outgoing calls are barred. */
    E_QL_MCM_SIMRIL_CHANGE_CALL_BARRING_PWD_REASON_ALLINCOMINGBARRING    = 7, /**<  All incoming calls are barred. */
}ql_mcm_change_call_barring_pwd_reason_t;

typedef struct 
{
    ql_mcm_change_call_barring_pwd_reason_t     reason;  /**<  Reason for the password change. Refer to 3GPP TS 27.007 @xhyperref{S8,[S8]}, Section 7.4.*/
    char                                        old_pwd[QL_MCM_MAX_PASSWORD_LENGTH + 1];
    char                                        new_pwd[QL_MCM_MAX_PASSWORD_LENGTH + 1];
}ql_mcm_change_call_barring_pwd_info_t;  /* Message */


typedef enum 
{
    E_QL_MCM_SIMRIL_E911_UNKNOWN   = -1,   /**<  E911 unknown. */
    E_QL_MCM_SIMRIL_E911_INACTIVE  = 0,    /**<  E911 INACTIVE. */
    E_QL_MCM_SIMRIL_E911_ACTIVE    = 1,    /**<  E911 ACTIVE. */
}ql_mcm_simril_e911_state_t;

typedef ql_mcm_voice_call_state_t E_QL_VOICE_CALL_STATE_T;

typedef enum 
{
    E_QL_MCM_ECALL_TEST             = 0x01, 
    E_QL_MCM_ECALL_EMERGENCY        = 0x02, 
    E_QL_MCM_ECALL_RECONFIG         = 0x03, 
}E_QL_MCM_ECALL_VARIANT_T;

typedef enum {
 	E_QL_MCM_VOICE_AUTO_ANSWER_ENABLE       = 0,    /**<  Enable auto-answer. */
 	E_QL_MCM_VOICE_AUTO_ANSWER_DISABLE      = 1,    /**<  Disable auto-answer. */
}E_QL_MCM_VOICE_AUTO_ANSWER_T;


//Laurence.yin-2018/04/03-QCM9XOL00004C011-P02, <[MCM-ECALL] : add ecall status ind info.>
typedef enum 
{
    E_QL_MCM_VOICE_UNKOWN_IND               = 0,
    E_QL_MCM_VOICE_CALL_IND                 ,
    E_QL_MCM_VOICE_ECALL_STATUE_IND         ,
    E_QL_MCM_VOICE_ECALL_URC_IND            ,
    E_QL_MCM_VOICE_ECALL_EVENT_IND          ,

}E_QL_MCM_VOICE_IND_T;

typedef enum 
 {
    E_QL_MCM_VOICE_ECALL_MSD_TRANSMISSION_STATUS_SUCCESS        = 0x00, /**<   Success \n       */
    E_QL_MCM_VOICE_ECALL_MSD_TRANSMISSION_STATUS_FAILURE        = 0x01, /**<   Generic failure  */
}E_QL_MCM_EALL_MSD_TRANSMISSION_STATUS_T;

typedef enum 
{
    E_QL_MCM_VOICE_ECALL_INACTIVE               = 0, 
    E_QL_MCM_VOICE_ECALL_ORIGINATING_CALL       = 1, 
    E_QL_MCM_VOICE_ECALL_IN_CALL_TRANSMITTING   = 2, /**<  ECALL APP TRANSMITTING */
    E_QL_MCM_VOICE_ECALL_WATING_FOR_AL_ACK      = 3, 
    E_QL_MCM_VOICE_ECALL_IN_CALL                = 4, 
    E_QL_MCM_VOICE_ECALL_IDLE_ALLOW_MT_ECALL    = 5, 
}E_QL_MCM_ECALL_STATE_T;

typedef enum
{
  E_QL_MCM_VOICE_ECALL_MODE_NOMAL   = 0, 
  E_QL_MCM_VOICE_ECALL_MODE_ONLY    = 1, 
  E_QL_MCM_VOICE_ECALL_MODE_DEFAULT = 2
}E_QL_MCM_ECALL_MODE_T;

typedef enum
{
  E_QL_MCM_VOICE_ECALL_CONFIG_ENABLE            = 0, 
  E_QL_MCM_VOICE_ECALL_CONFIG_VIOCECONF         , 
  E_QL_MCM_VOICE_ECALL_CONFIG_MODE              ,
  E_QL_MCM_VOICE_ECALL_CONFIG_PROCESSINFO       ,
  E_QL_MCM_VOICE_ECALL_CONFIG_START_TIMER       ,
  E_QL_MCM_VOICE_ECALL_CONFIG_HACK_TIMER        ,
  E_QL_MCM_VOICE_ECALL_CONFIG_MSD_TRANSMISSION  ,
  E_QL_MCM_VOICE_ECALL_CONFIG_MO_FAILR_REDIAL   ,
  E_QL_MCM_VOICE_ECALL_CONFIG_DROP_REDIAL
}E_QL_MCM_ECALL_CONFIG_T;

typedef struct
{
    char                        ecall_msd[QL_MCM_MAX_ECALL_MSD+1];  //max msd length 140
    int                         manual;
    E_QL_MCM_ECALL_VARIANT_T    eCallModeType; 
}ql_mcm_ecall_info;

typedef struct 
{
  uint8_t enable;           /*  Enable or disable ecall ;0 -- Disable; 1 -- Enable */
  uint8_t voiceconfig;      /*  Enable or disable to mute IVS speaker in MSD
                            0 -- Disable to mute IVS speaker automatical in MSD transmission
                            1 -- Enable to mute IVS speaker automatical in MSD transmission */
  E_QL_MCM_ECALL_MODE_T ecallmode;  /*  The Ecall mode. */
  uint8_t processinfo;      /*  Enable or disable to report ecall event info.   0 -- Disable; 1 -- Enable */
  uint8_t T5;               /*  The timer of IVS waiting for "START". */
  uint8_t T6;               /*  The timer of IVS waiting for "HACK". */
  uint8_t T7;               /*  The timer of MSD transmission. */
  uint8_t mofailredial;     /*  The dial fail times. */
  uint8_t dropredial;
}ql_mcm_ecall_config_info;


typedef struct 
{
    uint8_t call_id;                                                    /**<  call_id.*/
    uint8_t ecall_msd_tx_status_valid; 
    E_QL_MCM_EALL_MSD_TRANSMISSION_STATUS_T ecall_msd_tx_status;
}ql_mcm_voice_ecall_status_t;  /* Message */

typedef struct 
{
    uint8_t call_id;                /**<  call_id.*/
    uint32_t ecall_urc_event_len;   /**< Must be set to # of elements in ecall_urc_event */
    char ecall_urc_event[QL_MCM_MAX_ECALL_URC_EVENT_LENGTH];
}ql_mcm_voice_ecall_urc_t;

typedef enum 
{
  E_QL_MCM_VOICE_ECALL_EVENT_FAIL_START_TIMEOUT             = 1, /**<  Wait for START timeout. */
  E_QL_MCM_VOICE_ECALL_EVENT_FAIL_HACK_TIMEOUT              = 2, /**<  Wait for HACK timeout.	 */
  E_QL_MCM_VOICE_ECALL_EVENT_FAIL_MSD_TRANSMISSION_TIMEOUT  = 3, /**<  MSD transmission timeout.	 */
  E_QL_MCM_VOICE_ECALL_EVENT_FAIL_IVS_RESET_TIMEOUT         = 4  /**<  IVS reset. */
}E_QL_MCM_VOICE_ECALL_EVENT_FAIL_T;

typedef enum 
{
  E_QL_MCM_VOICE_ECALL_EVENT_PROCESS_IVS_START_RECEIVED_MSD = 1, /**<  IVS Link Layer receives START message and starts to send MSD */
  E_QL_MCM_VOICE_ECALL_EVENT_PROCESS_IVS_NACK_RECEIVED      = 2, /**<  IVS Link Layer receives NACK message. */
  E_QL_MCM_VOICE_ECALL_EVENT_PROCESS_IVS_ACK_RECEIVED       = 3, /**<  IVS Link Layer receives the first LACK message. */
  E_QL_MCM_VOICE_ECALL_EVENT_PROCESS_IVS_TX_COMPLETED       = 4, /**<  IVS MSD transmission is complete. */
  E_QL_MCM_VOICE_ECALL_EVENT_PROCESS_IVS_HLACK_RECEIVED     = 5  /**<  IVS Link Layer receives first HACK message. */
}E_QL_MCM_VOICE_ECALL_EVENT_PROCESS_T;

typedef enum 
{
  E_QL_MCM_VOICE_ECALL_EVENT_MSDUPDATE_IVS_UPDATING_MSD         = 0, /**<  Indicates MSD has been updated. */
  E_QL_MCM_VOICE_ECALL_EVENT_MSDUPDATE_PSAP_REQURE_UPDATE_MSD   = 1, /**<  Indicate timeout of updating MSD and module starts to transfer the old MSD. */
  E_QL_MCM_VOICE_ECALL_EVENT_MSDUPDATE_IVS_UPDATE_MSD_TIMEOUT   = 2  /**<  Indicate to update MSD in 5 seconds. */
}E_QL_MCM_VOICE_ECALL_EVENT_MSDUPDATE_T;

typedef enum 
{
  E_QL_MCM_VOICE_ECALL_EVENT_ECALL_ESTABLISHNG_SUCCESS  = 0, /**<  Establish eCall successfully. */
  E_QL_MCM_VOICE_ECALL_EVENT_ECALL_ESTABLISHNG_FAIL     = 1  /**<  Establish eCall fails. */
}E_QL_MCM_VOICE_ECALL_EVENT_ESTABLISH_T;

typedef struct 
{
  uint16_t hack_code;  /**<   psap hack code.*/
}ql_mcm_voice_ecall_event_hackcode_t;  /* Type */

typedef struct 
{
  uint8_t ori_remainder_times;  /**<   originate fail remainder times*/
  uint16_t time;                /**<   the minimum time duration between the previous call attempt*/
}ql_mcm_voice_ecall_event_originate_fail_and_redial_t;  /* Type */

typedef struct 
{
  uint8_t drop_remainder_times;  /**<   dorp remainder times*/
  uint16_t time;                 /**<   the minimum time duration between the previous call attempt*/
}ql_mcm_voice_ecall_event_drop_and_redial_t;  /* Type */


typedef struct 
{
  uint8_t                                               ecall_event_fails_valid;
  E_QL_MCM_VOICE_ECALL_EVENT_FAIL_T                     ecall_event_fails;          /**<   event of eCall Failed.*/
  uint8_t                                               ecall_event_process_valid;  
  E_QL_MCM_VOICE_ECALL_EVENT_PROCESS_T                  ecall_event_process;        /**<   event of eCall process.*/
  uint8_t                                               ecall_event_msdupdate_valid;  
  E_QL_MCM_VOICE_ECALL_EVENT_MSDUPDATE_T                ecall_event_msdupdate;      /**<   event of ecall msd update.*/
  uint8_t                                               ecall_event_establish_valid; 
  E_QL_MCM_VOICE_ECALL_EVENT_ESTABLISH_T                ecall_event_establish;      /**<   event of eCall establish.*/
  uint8_t                                               ecall_event_hackcode_valid;  
  ql_mcm_voice_ecall_event_hackcode_t                   ecall_event_hackcode;       /**<   event of eCall hack code.*/
  uint8_t                                               ecall_event_ori_redial_valid;  
  ql_mcm_voice_ecall_event_originate_fail_and_redial_t  ecall_event_ori_redial;     /**<   event of originate fail and redial.*/
  uint8_t                                               ecall_event_drop_redial_valid;
  ql_mcm_voice_ecall_event_drop_and_redial_t            ecall_event_drop_redial;    /**<   event of drop and redial.*/
}ql_mcm_voice_ecall_event_t;  /* Message */
  

typedef ql_mcm_voice_calls_state_t ql_mcm_voice_call_ind;
typedef ql_mcm_voice_ecall_status_t ql_mcm_voice_ecall_status_ind;
typedef ql_mcm_voice_ecall_urc_t ql_mcm_voice_ecall_urc_ind;
typedef ql_mcm_voice_ecall_event_t ql_mcm_voice_ecall_event_ind;




/* Callback function registered via QL_Voice_Call_AddStateHandler;
   This will be called if any stated changed of call_id
*/
typedef void (*QL_VoiceCall_CommonStateHandlerFunc_t)
(
    unsigned long message_id,
    void *ind_data,
    uint32_t ind_data_len
);


typedef uint32 voice_client_handle_type;

/* Callback function registered via QL_Voice_Call_AddStateHandler;
   This will be called if any stated changed of call_id
*/
typedef void (*QL_VoiceCall_StateHandlerFunc_t)
(
    int                     call_id,
    char*                   phone_num,
    E_QL_VOICE_CALL_STATE_T state,
    void                    *contextPtr
);

/* Init voice module and return h_voice, this should be called before any other APIs */
int QL_Voice_Call_Client_Init(voice_client_handle_type  *ph_voice);

/* DeInit voice module and release resources, this should be called at last */
int QL_Voice_Call_Client_Deinit(voice_client_handle_type h_voice);

/* Add callback function, if any call state changed, handlerPtr will be called to notify App */
int QL_Voice_Call_AddStateHandler(voice_client_handle_type          h_voice,
                                  QL_VoiceCall_StateHandlerFunc_t   handlerPtr,
                                  void*                             contextPtr);

/* Remove callback function, won't receive any notify anymore */
int QL_Voice_Call_RemoveStateHandler(voice_client_handle_type       h_voice);

//Laurence.yin-2018/04/03-QCM9XOL00004C011-P02, <[MCM-ECALL] : add ecall status ind info.>
/* Add callback function, if any call state changed, handlerPtr will be called to notify App */
int QL_Voice_Call_AddCommonStateHandler(voice_client_handle_type          h_voice,
                                  QL_VoiceCall_CommonStateHandlerFunc_t   handlerPtr);

/* Remove callback function, won't receive any notify anymore */
int QL_Voice_Call_RemoveCommonStateHandler(voice_client_handle_type          h_voice);


/* Start call and return call_id, this can be used in the later */
int QL_Voice_Call_Start(voice_client_handle_type    h_voice,
                        E_QL_VCALL_ID_T             simId,
                        char*                       phone_number,   ///< [IN] Destination identifier for the voice
                        int                         *call_id);       ///< [OUT] call id

/* End call of call_id, which returned by QL_Voice_Call_Start or callback func register via QL_Voice_Call_AddStateHandler */
int QL_Voice_Call_End(  voice_client_handle_type    h_voice,
                        int                         call_id);        ///< [IN] call id, return by QL_Voice_Start

/* Answer the call of call_id, which returned by callback func register via QL_Voice_Call_AddStateHandler */
int QL_Voice_Call_Answer(voice_client_handle_type    h_voice,
                         int                         call_id );

int QL_Voice_Call_Hold( voice_client_handle_type    h_voice);

int QL_Voice_Call_UnHold( voice_client_handle_type    h_voice);

int QL_Voice_Call_Conference( voice_client_handle_type   h_voice);

int QL_Voice_Call_EndConference( voice_client_handle_type   h_voice);

int QL_Voice_Call_Ecall(voice_client_handle_type    h_voice,
                        E_QL_VCALL_ID_T             simId,
                        char*                       phone_number, 
                        ql_mcm_ecall_info           ecall_info,
                        int                         *call_id);


int QL_Voice_Call_SetAutoAnswer(voice_client_handle_type                h_voice,
                                        E_QL_MCM_VOICE_AUTO_ANSWER_T    eAnswerType,
                                        uint32_t                        uAnswerTime);

int QL_Voice_Call_Ecall_HangUp(voice_client_handle_type h_voice);

int QL_Voice_Call_Ecall_UpdateMsd(voice_client_handle_type    h_voice,const char *hex_msd);

//Ecall Push caommand
int QL_Voice_Call_Ecall_MsdPush(voice_client_handle_type h_voice,
                                        E_QL_MCM_ECALL_STATE_T *ecall_state);

//Get Ecall config info
int QL_Voice_Call_Ecall_GetConfigInfo(voice_client_handle_type h_voice,
                                        ql_mcm_ecall_config_info *ecall_config);

int QL_Voice_Call_Ecall_SetConfigInfo(voice_client_handle_type h_voice,
                                        E_QL_MCM_ECALL_CONFIG_T ecall_config_type,
                                        uint8_t value);



//Cancel dial
int QL_Voice_Call_CancelDial( voice_client_handle_type   h_voice);

//VTS API
int QL_Voice_Call_Dtmf( voice_client_handle_type h_voice,uint8_t digit,int call_id);


int QL_Voice_Call_GetCallStatus
(
    int                         h_voice, 
    int                         call_id, // If call_id<0, means to get all calls state, or get specified call_id info
    ql_mcm_voice_calls_state_t  *pt_callstate
);

//Set forwarding
int QL_Voice_Call_SetForwarding
(
    int                             h_voice, 
    E_QL_MCM_VOICE_CALL_SERVICE_T   service, 
    E_QL_MCM_VOICE_CALL_FORWARDING_REASON_T  reason,
    char *number
);

//Get forwarding status
int QL_Voice_Call_GetForwardingStatus
(
    int                             h_voice, 
    E_QL_MCM_VOICE_CALL_FORWARDING_REASON_T  reason,
    ql_mcm_voice_call_forwarding_status_list_t *pt_status
);

//Set voice call waiting
int QL_Voice_Call_SetWaiting
( 
    int                                 h_voice, 
    ql_mcm_voice_call_waiting_service_t e_service
);

//Get voice call waiting status
int QL_Voice_Call_GetWaitingStatus
(
    int                                 h_voice, 
    ql_mcm_voice_call_waiting_service_t *pe_service
);


/*
Usage 1 (register callback and wait for new call in, then answer): 
1, QL_Voice_Call_Client_Init
2, QL_Voice_Call_AddStateHandler(pf_cb)
3, wait for new call arrive, pf_cb will pass the call_id to app.
4, QL_Voice_Call_Answer(call_id)
5, QL_Voice_Call_End
6, QL_Voice_Call_Client_Deinit


Usage 2 (call out): 
1, QL_Voice_Call_Client_Init
2, QL_Voice_Call_AddStateHandler(pf_cb)
3, QL_Voice_Call_Start
4, QL_Voice_Call_End
5, QL_Voice_Call_Client_Deinit


*/


#endif // __QL_MCM_VOICE_H__

