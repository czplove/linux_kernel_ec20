/******************************************************************************
#@file    psm_client.h
#@brief   Provides interface for clients to access PSM functionality
#
#  ---------------------------------------------------------------------------
#
#  Copyright (c) 2015 Qualcomm Technologies, Inc.
#  All Rights Reserved.
#  Confidential and Proprietary - Qualcomm Technologies, Inc.
#  ---------------------------------------------------------------------------
#******************************************************************************/

#ifndef PSM_CLIENT_H
#define PSM_CLIENT_H

#include <time.h>

/** @addtogroup psm_enums
    @{
  */

typedef enum {
    PSM_STATUS_NONE,
    PSM_STATUS_REJECT,            /**< PSM enter request is rejected */
    PSM_STATUS_READY,             /**< Ready to enter into PSM mode. */
    PSM_STATUS_NOT_READY,         /**< Not ready to enter PSM. */
    PSM_STATUS_COMPLETE,          /**< Entered into PSM mode, system might shutdown any time. */
    PSM_STATUS_DISCONNECTED,      /**< PSM Server is down. */
    PSM_STATUS_MODEM_LOADED,      /**< Modem is loaded as part of boot up. */
    PSM_STATUS_MODEM_NOT_LOADED,  /**< Modem is not loaded as part of boot up.*/
    PSM_STATUS_NW_OOS,            /** < Network is OOS. */
    PSM_STATUS_NW_LIMITED_SERVICE,/**< Network is Limited Service. */
    PSM_STATUS_HEALTH_CHECK,      /**< Application health check.*/
    PSM_STATUS_MAX
} psm_status_type_e;

/**
    @}
  */

/** @addtogroup psm_enums
    @{
  */
typedef enum {
    PSM_REJECT_REASON_NONE,                /**< Reject reason None.*/
    PSM_REJECT_REASON_NOT_ENABLED,         /**< PSM feature is not enabled. */
    PSM_REJECT_REASON_MODEM_NOT_READY,     /**< Modem is not ready to enter into PSM mode.*/
    PSM_REJECT_REASON_DURATION_TOO_SHORT,  /**< PSM duration is too short to enter into PSM mode.*/
    PSM_REJECT_REASON_MAX
} psm_reject_reason_type_e;

/**
    @}
  */

/** @addtogroup psm_enums
    @{
  */
typedef enum {
    PSM_ERR_NONE,                  /**< Success. */
    PSM_ERR_FAIL,                  /**< Failure.*/
    PSM_ERR_GENERIC_FAILURE,       /**< Miscellaneous failure.*/
    PSM_ERR_APP_NOT_REGISTERED,    /**< Application is not registered with PSM Server.*/
    PSM_ERR_WRONG_ARGUMENTS,       /**< Wrong input arguments.*/
    PSM_ERR_IPC_FAILURE,           /**< Failure to communicate with PSM server.*/
} psm_error_type_e;
/**
    @}
  */

/** @addtogroup psm_enums
    @{
  */

typedef enum {
    PSM_TIME_IN_TM,        /**< Specify time in broken down format.*/
    PSM_TIME_IN_SECS       /**< Specify time in seconds.*/
} psm_time_format_type_e;

/**
    @}
  */

/** @addtogroup psm_enums
    @{
  */

typedef enum {
    PSM_WAKEUP_MEASUREMENT_ONLY,        /**< Next wake up from PSM is for measurement purpose only.*/
    PSM_WAKEUP_MEASUREMENT_NW_ACCESS    /**< Next wake up from PSM is for measurement and network access.*/
} psm_wakeup_type_e;

/**
    @}
  */


/** @addtogroup psm_aggregates
    @{
  */
typedef struct {
    psm_time_format_type_e  time_format_flag;       /**< Time format. see #psm_time_format_type. */
    struct tm               wakeup_time;            /**< Time in broken down format if time_format_flag is set to PSM_TIME_IN_TM */
    int                     psm_duration_in_secs;   /**< Time in seconds if if time_format_flag is set to PSM_TIME_IN_SECS.*/
}psm_time_info_type;

/**
    @}
  */


/** @addtogroup psm_aggregates
    @{
  */

typedef struct {
    int                 active_time_in_secs;  /**< Active time is the duration PSM server has to wait before entering
    into PSM mode. Purpose of this time is to give chance for MTC server to react.*/
    psm_wakeup_type_e   psm_wakeup_type;      /**< Next wake up from PSM mode is for
    measurement purpose or measurement and network access. */
    psm_time_info_type  psm_time_info;        /**< PSM time information. see #psm_time_info_type.*/
}psm_info_type;


/**
    @}
  */

/** @addtogroup psm_aggregates
    @{
  */

typedef struct  {
    psm_status_type_e          status; /**< PSM status. see #psm_status_type_e. */
    psm_reject_reason_type_e   reason; /**< PSM reject reason. see #psm_reject_reason_type_e.*/
}psm_status_msg_type;

/**
    @}
  */

/* PSM status call back type.*/
typedef void (*psm_client_cb_type)(psm_status_msg_type *);

/* PSM timer expiry call back type.*/
typedef void (*psm_util_timer_expiry_cb_type)(void *, size_t);

/*=========================================================================
  FUNCTION:  psm_client_register

===========================================================================*/
/*!
    @brief
    Makes the app known to PSM server as a PSM aware application.
    This is the first API every PSM aware application need to call.
    Every application which need network related functionality should call this API.

    @return
    None.
*/
/*=========================================================================*/
psm_error_type_e psm_client_register(int *client_id, psm_client_cb_type cb_func);

/*=========================================================================
  FUNCTION:  psm_client_unregister

===========================================================================*/
/*!
    @brief
    Unregister a psm aware app.
    Application should not call any other PSM client API except psm_client_register after this API is called.

    @return
    None.
*/
/*=========================================================================*/
psm_error_type_e psm_client_unregister(int client_id);

/*=========================================================================
  FUNCTION:  psm_client_enter_power_save

===========================================================================*/
/*!
    @brief
    Used by app to notify its desire to enter PSM mode.
    Application need to pass active_time in secs, time in PSM mode, whether next wake up is for measurement
    purpose or access network. PSM time can will be accepted in either broken down format or
    in secs.

    @return
    None.
*/
/*=========================================================================*/
psm_error_type_e psm_client_enter_power_save
(
    int                 client_id,
    psm_info_type      *psm_info
);

/*=========================================================================
  FUNCTION:  psm_client_enter_backoff

===========================================================================*/
/*!
    @brief
    Used by app to notify its desire to enter PSM mode due to network OOS or MTC server not reachable.
    Duration for which app want to enter into PSM mode is decided by the PSM server based on
    the NV item configuration.

    @return
    None.
*/
/*=========================================================================*/
psm_error_type_e psm_client_enter_backoff( int client_id );


/*=========================================================================
  FUNCTION:  psm_cancel_power_save

===========================================================================*/
/*!
    @brief
    Used by app to notify its desire to withdraw a previous request to enter PSM mode.

    @return
    None.
*/
/*=========================================================================*/
psm_error_type_e psm_cancel_power_save( int client_id );

/*=========================================================================
  FUNCTION:  psm_client_load_modem

===========================================================================*/
/*!
    @brief
    Application should inform its intention to use modem services through this API.

    @return
    None.
*/
/*=========================================================================*/
psm_error_type_e psm_client_load_modem(int client_id);

/*=========================================================================
  FUNCTION:  psm_client_health_check_ack

===========================================================================*/
/*!
    @brief
    Application health check acknowledge API. Application need to call this API
    when it receives PSM_STATUS_HEALTH_CHECK event.
    This API will inform PSM server that application is healthy and functioning.
    Application should call this API from worker thread of the application.
    This will ensure that application malfunctioning is detected by PSM server.

    @return
    None.
*/
/*=========================================================================*/
int psm_client_health_check_ack(int client_id);

/*=========================================================================
  FUNCTION:  psm_util_create_timer

===========================================================================*/
/*!
    @brief
    Utility function provide for applications to create time with specified timeout.

    @return
    None.
*/
/*=========================================================================*/
int psm_util_create_timer(struct timeval *timeout,
                          psm_util_timer_expiry_cb_type timer_expiry_cb,
                          void *timer_expiry_cb_data,
                          size_t timer_expiry_cb_data_len);

/*=========================================================================
  FUNCTION:  psm_util_cancel_timer

===========================================================================*/
/*!
    @brief
    Application should inform its intention to use modem services through this API.

    @return
    None.
*/
/*=========================================================================*/
int psm_util_cancel_timer(int timer_id);

#endif //PSM_CLIENT_H

