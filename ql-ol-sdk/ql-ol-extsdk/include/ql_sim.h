/**
 *@file     ql_sim.h
 *@date     2017-05-02
 *@author   
 *@brief    
 */   

#ifndef __QL_SIM_H__
#define __QL_SIM_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//#include "ql_type.h"

typedef unsigned char uint8_t;

//--------------------------------------------------------------------------------------------------
/**
 * SIM states.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    E_QL_SIM_STAT_NOT_INSERTED,
    E_QL_SIM_STAT_READY,
    E_QL_SIM_STAT_PIN_REQ,
    E_QL_SIM_STAT_PUK_REQ,
    E_QL_SIM_STAT_BUSY,
    E_QL_SIM_STAT_BLOCKED,
    E_QL_SIM_STAT_UNKNOWN
}E_QL_SIM_STATES_T;


//--------------------------------------------------------------------------------------------------
/**
 * SIM identifiers.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    E_QL_SIM_EXTERNAL_SLOT_1,
    E_QL_SIM_EXTERNAL_SLOT_2,
    E_QL_SIM_EMBEDDED,
    E_QL_SIM_REMOTE,
    E_QL_SIM_ID_MAX
}E_QL_SIM_ID_T;


#define QL_UIM_AID_MAX  32
#define QL_UIM_APDU_DATA_MAX 1024
#define QL_QMI_UIM_SELECT_RESPONSE_MAX 255

typedef enum
{
    E_QL_UIM_SLOT_1 = 0x01,
    E_QL_UIM_SLOT_2 = 0x02,
    E_QL_UIM_SLOT_3 = 0x03,
    E_QL_UIM_SLOT_4 = 0x04,
    E_QL_UIM_SLOT_5 = 0x05,
}E_QL_UIM_SLOT_T;


typedef struct 
{
    uint8_t   sw1;  /**<   SW1 status code received from the card.*/
    uint8_t   sw2;  /**<   SW2 status code received from the card.*/
}ST_QL_UIM_CARD_RESULT_TYPE;

typedef struct 
{
    uint8_t                        channel_id_valid;
    uint8_t                        channel_id;
    ST_QL_UIM_CARD_RESULT_TYPE     card_result;
    uint8_t                        select_response_valid;         /**< Must be set to true if select_response is being passed */
    uint32_t                       select_response_len;           /**< Must be set to # of elements in select_response */
    uint8_t                        select_response[QL_QMI_UIM_SELECT_RESPONSE_MAX];
}ST_QL_UIM_LOGCAL_CHANNEL_TYPE;

typedef struct
{
    uint32_t   aid_len;
    uint8_t    aid[QL_UIM_AID_MAX];
}ST_QL_UIM_AID_INFO_TYPE;  

typedef struct
{
    uint32_t  apdu_len;
    uint8_t   apdu[QL_UIM_APDU_DATA_MAX];
    uint8_t   channel_id;
}ST_QL_UIM_SEND_APDU_TYPE;  


typedef struct 
 {
  uint32_t  total_length;  /**<   Indicates the total length of the response APDU from the card*/
  uint32_t  token;         /**<   Token used in the subsequent indications with APDU response*/
}ST_QL_UIM_SEND_APDU_LONG_RESPONSE_TYPE;  /* Type */

typedef struct 
 {
  uint32_t  token;                       /**<   Unique token used in the previous QMI_UIM_SEND_APDU response.*/
  uint16_t  total_length;                /**<   Total length of the response APDU from the card.*/
  uint16_t  offset;                      /**<   Offset of the current chunk, starting from 0.*/
  uint32_t  apdu_len;                    /**<   Must be set to # of elements in apdu */
  uint8_t   apdu[QL_UIM_APDU_DATA_MAX];  /**<   Content of the data chunk of the APDU response from the card.*/
}ST_UIM_SEND_APDU_LONG_RESPONSE_CHUNK_TYPE;


typedef struct
 {
  uint8_t    apdu_valid;  
  uint32_t   apdu_len;  
  uint8_t    apdu[QL_UIM_APDU_DATA_MAX];
  
  uint8_t    long_response_valid;
  ST_UIM_SEND_APDU_LONG_RESPONSE_CHUNK_TYPE long_response;
}ST_UIM_SEND_APDU_RESPONSE_TYPE;


/*
    Get the sim card ICCID string.
     -simId     : sim ID
     -iccid     : input buffer
     -iccidLen  : input buffer length
*/
extern int  QL_SIM_GetICCID
(
    E_QL_SIM_ID_T   simId,      ///< [IN] The SIM identifier.
    char*           iccid,      ///< [OUT] ICCID
    size_t          iccidLen    ///< [IN]
);

/*
    Get the sim card IMSI string.
     -simId     : sim ID
     -imsi      : input buffer
     -imsiLen   : input buffer length
*/
extern int  QL_SIM_GetIMSI
(
    E_QL_SIM_ID_T   simId,      ///< [IN] The SIM identifier.
    char*           imsi,       ///< [OUT] IMSI
    size_t          imsiLen     ///< [IN]
);


//--------------------------------------------------------------------------------------------------
/**
 * This function been called to verify the PIN code.
 *
 * @return E_QL_BAD_PARAMETER The parameters are invalid.
 * @return E_QL_NOT_FOUND     The function failed to select the SIM card for this operation.
 * @return QL_UNDERFLOW     The PIN code is not long enough (min 4 digits).
 * @return E_QL_FAULT    The function failed to enter the PIN code.
 * @return E_QL_OK       The function succeeded.
 *
 * @note If PIN code is too long (max 8 digits), it is a fatal error, the
 *  function will not return.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *  function will not return.
 */
//--------------------------------------------------------------------------------------------------
int  QL_SIM_VerifyPIN
(
    E_QL_SIM_ID_T   simId,  ///< [IN] The SIM identifier.
    const char      *pin    ///< [IN] The PIN code.
);


//--------------------------------------------------------------------------------------------------
/**
 * This function been called to change the PIN code.
 *
 * @note If PIN code is too long (max 8 digits), it is a fatal error, the
 *  function will not return.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *  function will not return.
 */
//--------------------------------------------------------------------------------------------------
int  QL_SIM_ChangePIN
(
    E_QL_SIM_ID_T   simId,      ///< [IN] The SIM identifier.
    const char      *old_pin,   ///< [IN] The old PIN code.
    const char      *new_pin    ///< [IN] The new PIN code.
);


//--------------------------------------------------------------------------------------------------
/**
 * Unlock the SIM card: it will disables the request of PIN code.
 *
 * @note If PIN code is too long (max 8 digits), it is a fatal error, the
 *  function will not return.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *  function will not return.
*/
//--------------------------------------------------------------------------------------------------
int  QL_SIM_Unlock
(
    E_QL_SIM_ID_T   simId,      ///< [IN] The SIM identifier.
    const char      *pin        ///< [IN] The PIN code.
);


//--------------------------------------------------------------------------------------------------
/**
 * Lock the SIM card: it will enables the request of PIN code.
 *
 * @note If PIN code is too long (max 8 digits), it is a fatal error, the
 *  function will not return.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *  function will not return.
 */
//--------------------------------------------------------------------------------------------------
int  QL_SIM_Lock
(
    E_QL_SIM_ID_T   simId,  ///< [IN] The SIM identifier.
    const char      *pin    ///< [IN] The PIN code.
);


//--------------------------------------------------------------------------------------------------
/**
 * Unblock the SIM card: it will use puk to unblock the SIM card and set new pin.
 *
 * @note If PIN code is too long (max 8 digits), it is a fatal error, the
 *  function will not return.
 *
 * @note If the caller is passing a bad pointer into this function, it is a fatal error, the
 *  function will not return.
 */
//--------------------------------------------------------------------------------------------------
int  QL_SIM_Unblock
(
    E_QL_SIM_ID_T   simId,      ///< [IN] The SIM identifier.
    const char      *puk,       ///< [IN] The PIN code.
    const char      *newpin     ///< [IN] The PIN code.
);

/*
    Get the sim card PIN retry times.
     -simId     : sim ID
*/
extern int  QL_SIM_GetPINTriesCnt
(
     E_QL_SIM_ID_T simId    ///< [IN] The SIM identifier.
);


/*
    Get the sim card state.
     -simId     : sim ID
*/
extern E_QL_SIM_STATES_T  QL_SIM_GetState
(
    E_QL_SIM_ID_T simId    ///< [IN] The SIM identifier.
);

/*
    Open  the uim logical channel.
*/
E_QL_SIM_STATES_T QL_SIM_OpenLogicalChannel
(
    E_QL_UIM_SLOT_T                      uimId,                ///< [IN] The SIM identifier.
    ST_QL_UIM_AID_INFO_TYPE              st_aid,               ///<IN
    ST_QL_UIM_LOGCAL_CHANNEL_TYPE       * puimLogicalChanel    ///<OUT
);

/*
    CLOSED  the uim logical channel.
    //laurence 20180402, delete ST_QL_UIM_LOGCAL_CHANNEL_TYPE parameters
*/
E_QL_SIM_STATES_T QL_SIM_CloseLogicalChannel
(
    E_QL_UIM_SLOT_T                      uimId,                ///< [IN] The SIM identifier.
    uint8_t                              channel_id            ///<IN
);

/*
    SEND  the uim APDU.
*/
E_QL_SIM_STATES_T QL_SIM_SendApdu
(
    E_QL_UIM_SLOT_T                       uimId,                 ///< [IN] The SIM identifier.
    ST_QL_UIM_SEND_APDU_TYPE              st_apdu,               ///< [IN
    ST_UIM_SEND_APDU_RESPONSE_TYPE       * pSendResp             ///<OUT
);


#endif // __QL_SIM_H__

