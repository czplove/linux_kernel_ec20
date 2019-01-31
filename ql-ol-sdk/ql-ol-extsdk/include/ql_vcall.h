/**
 *@file     ql_voicecall.h
 *@date     2017-05-02
 *@author   
 *@brief    
 */

#ifndef __QL_VCALL_H__
#define __QL_VCALL_H__

#define QL_QMI_VCALL_ECALL_MSD_MAX_LEN 140


//--------------------------------------------------------------------------------------------------
/**
 * Reference returned by Start function and used by End function
 */
//--------------------------------------------------------------------------------------------------
//typedef struct QL_VCALL_Call* ST_VCALL_CallRef;
typedef int ST_VCALL_CallRef;

//--------------------------------------------------------------------------------------------------
/**
 * SIM identifiers.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    E_QL_VCALL_EXTERNAL_SLOT_1,
    E_QL_VCALL_EXTERNAL_SLOT_2,
    E_QL_VCALL_EMBEDDED,
    E_QL_VCALL_REMOTE,
    E_QL_VCALL_ID_MAX
}E_QL_VCALL_ID_T;

//--------------------------------------------------------------------------------------------------
/**
 * Voice call establishment states.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    E_QL_VCALL_EVENT_ALERTING = 0,
   ///< Voice call establishment in progress.
   ///< Far end is now alerting its user (outgoing call).

    E_QL_VCALL_EVENT_CONNECTED = 1,
   ///< Call has been established, and is media is active.

    E_QL_VCALL_EVENT_TERMINATED = 2,
   ///< Call has terminated.

    E_QL_VCALL_EVENT_OFFLINE = 3,
   ///< NO Service available to try establish a voice call.

    E_QL_VCALL_EVENT_BUSY = 4,
   ///< Remote party (callee) is busy.

    E_QL_VCALL_EVENT_RESOURCE_BUSY = 5,
   ///< All local connection resources (lines/channels) are in use.

    E_QL_VCALL_EVENT_CALL_END_FAILED = 6,
   ///< Call ending failed.

    E_QL_VCALL_EVENT_CALL_ANSWER_FAILED = 7,
   ///< Call answering failed.

    E_QL_VCALL_EVENT_INCOMING = 8
   ///< Incoming voice call in progress.
}E_QL_VCALL_EVENT_T;


//--------------------------------------------------------------------------------------------------
/**
 * Voice call termination reason.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    E_QL_VCALL_TERM_NETWORK_FAIL = 0,
   ///< Network could not complete the call.

    E_QL_VCALL_TERM_BAD_ADDRESS = 1,
   ///< Remote address could not be resolved.

    E_QL_VCALL_TERM_BUSY = 2,
   ///< Caller is currently busy and cannot take the call.

    E_QL_VCALL_TERM_LOCAL_ENDED = 3,
   ///< Local party ended the call.

    E_QL_VCALL_TERM_REMOTE_ENDED = 4,
   ///< Remote party ended the call.

    E_QL_VCALL_TERM_UNDEFINED = 5
   ///< Undefined reason.
}E_QL_VCALL_TerminationReason_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'QL_VCALL_State'
 */
//--------------------------------------------------------------------------------------------------
//typedef struct QL_VCALL_StateHandler* QL_VCALL_StateHandlerRef_t;
typedef int QL_VCALL_StateHandlerRef_t;


typedef enum
{
    E_QL_VCALL_DIRECTION_MO = 1,
    E_QL_VCALL_DIRECTION_MT = 2
}E_QL_VCALL_DIRECTION_T;

typedef enum
{
    E_QL_VCALL_STATE_ORIGINATING = 1,   //Origination
    E_QL_VCALL_STATE_INCOMING,          //Incoming
    E_QL_VCALL_STATE_CONVESATION,       //Conversation
    E_QL_VCALL_STATE_CC_IN_PROGRESS,    //Call is originating but waiting for call control to complete
    E_QL_VCALL_STATE_ALERTING,          //Alerting
    E_QL_VCALL_STATE_HOLD,              //Hold
    E_QL_VCALL_STATE_WAITING,           //Waiting
    E_QL_VCALL_STATE_DISCONNECTING,     //Disconnecting
    E_QL_VCALL_STATE_END,               //End
    E_QL_VCALL_STATE_SETUP,             //MT call is in setup state in 3GPP
}E_QL_VCALL_STATE_T;

typedef enum
{
    E_QL_VCALL_TYPE_VOICE = 0,      //Voice
    E_QL_VCALL_TYPE_VOICE_FORCED,   //Avoid modem call classification
    E_QL_VCALL_TYPE_VOICE_IP,       //Voice over IP
    E_QL_VCALL_TYPE_VT,             //VideoTelephony call over IP
    E_QL_VCALL_TYPE_VIDEOSHARE ,    //Videoshare
    E_QL_VCALL_TYPE_TEST,           //Test call
    E_QL_VCALL_TYPE_OTAPA,          //OTAPA
    E_QL_VCALL_TYPE_STD_OTASP,      //Standard OTASP
    E_QL_VCALL_TYPE_NON_STD_OTASP,  //Nonstandard OTASP
    E_QL_VCALL_TYPE_EMERGENCY,      //Emergency
    E_QL_VCALL_TYPE_SUPS,           //Supplementary Service
    E_QL_VCALL_TYPE_EMERGENCY_IP,   //Emergency VoIP
    E_QL_VCALL_TYPE_ECALL           //eCall
}E_QL_VCALL_TYPE_T;

typedef struct
{
    uint8_t                 call_id;
    char                    PhoneNum[20];      ///Telephone number string.  
    E_QL_VCALL_DIRECTION_T  e_direct;
    E_QL_VCALL_STATE_T      e_state;
    E_QL_VCALL_TYPE_T       e_type;
} vcall_info_t;

typedef enum
{
    E_QL_VCALL_ECALL_MANUAL = 5,
    E_QL_VCALL_ECALL_AUTO = 6
}E_QL_VCALL_ECALL_CAT_T;

typedef enum 
{
    E_QL_VCALL_ECALL_TEST		   	= 0x01, 
    E_QL_VCALL_ECALL_EMERGENCY     	= 0x02, 
    E_QL_VCALL_ECALL_RECONFIG		= 0x03, 
}E_QL_VCALL_ECALL_VARIANT_T;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for voice call state changes.
 *
 * @param reference
 *   Event voice call object reference.
 * @param identifier
 *   Identifier of the remote party
 * @param event
 *   Voice call event.
 * @param contextPtr
 */
//--------------------------------------------------------------------------------------------------
typedef void (*QL_VCALL_StateHandlerFunc_t)
(
    ST_VCALL_CallRef reference,
    const char* 	identifier,
    E_QL_VCALL_STATE_T  event,
    void* contextPtr
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'QL_VCALL_State'
 *
 * This event provides information on voice call state changes
 */
//--------------------------------------------------------------------------------------------------
QL_VCALL_StateHandlerRef_t QL_VCALL_AddStateHandler
(
    QL_VCALL_StateHandlerFunc_t handlerPtr,
   ///< [IN]

    void* contextPtr
   ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'QL_VCALL_State'
 */
//--------------------------------------------------------------------------------------------------
void QL_VCALL_RemoveStateHandler
(
    QL_VCALL_StateHandlerRef_t addHandlerRef
   ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Start a voice call.
 *
 * @return
 * - Reference to the voice call (to be used later for releasing the voice call)
 * - NULL if the voice call could not be processed
 */
//--------------------------------------------------------------------------------------------------
ST_VCALL_CallRef QL_VCALL_Start
(
    E_QL_VCALL_ID_T simId,
    const char* DestinationID
   ///< [IN] Destination identifier for the voice
);

//--------------------------------------------------------------------------------------------------
/**
 * Release a voice call.
 *
 * @return
 * - E_QL_OK if the end of voice call can be processed.
 * - E_QL_NOT_FOUND if the voice call object reference is not found.
 */
//--------------------------------------------------------------------------------------------------
int QL_VCALL_End
(
    ST_VCALL_CallRef reference
   ///< [IN] Voice call object reference to hang-up.
);


//--------------------------------------------------------------------------------------------------
/**
 * Answer to incoming voice call.
 *
 * @return
 * - E_QL_OK if the incoming voice call can be answered
 * - E_QL_NOT_FOUND if the incoming voice call object reference is not found.
 */
//--------------------------------------------------------------------------------------------------
int QL_VCALL_Answer
(
    ST_VCALL_CallRef reference
   ///< [IN] Incoming voice call object reference to answer.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the termination reason of a voice call reference.
 *
 * @return
 * - E_QL_OK if the termination reason is got
 * - E_QL_NOT_FOUND if the incoming voice call object reference is not found.
 * - E_QL_FAULT if the voice call is not terminated.
 */
//--------------------------------------------------------------------------------------------------
int QL_VCALL_GetTerminationReason
(
    ST_VCALL_CallRef reference,
   ///< [IN] Voice call object reference to read from.

    E_QL_VCALL_TerminationReason_t* reasonPtr
   ///< [OUT] Termination reason of the voice call.
);


int QL_VCALL_Ecall(E_QL_VCALL_ID_T 				simId,
					char*           			phone_number, 
                    char* 						ecall_msd,
                    E_QL_VCALL_ECALL_CAT_T  	emer_cat,
                    E_QL_VCALL_ECALL_VARIANT_T	eCallModeType, 
                    ST_VCALL_CallRef   			*reference);	//call id

int QL_VCALL_UpdateMsd(	const char *msd);


#endif // __QL_VCALL_H__

