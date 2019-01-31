/**
 * @file error_msgr.h
 * @brief Quectel IPC Error number public declarations.
 * 
 * @note 
 *
 * @copyright Copyright (c) 2009-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#ifndef __ERROR_MSGR_H__
#define __ERROR_MSGR_H__

typedef enum {
	MSG_ERR_NONE = 0,
	MSG_ERR_INVALID_HANDLE,                      /* invalid msg_handle_type */
	MSG_ERR_INVALID_CLIENT_ID,                   /* invalid client id */
	MSG_ERR_INVALID_COMMAND,                     /* invalid command */
	MSG_ERR_INVALID_PARAMETERS,                  /* invalid parameters */
	MSG_ERR_INVALID_MSG_ID,                      /* invalid message id */
	MSG_ERR_INVALID_REQ_RESP_IND,                /* invalid request or response indentity */
	MSG_ERR_MALLOC_FAILED,                       /* parameter malloc failed */
	MSG_ERR_SEND_BUFFER_FAILED,                  /* send buffer error */
	MSG_ERR_RECV_BUFFER_FAILED,                  /* receive buffer error */
	MSG_ERR_RECV_TIMEOUT,                        /* receive timeout */
	MSG_ERR_INVALID_RECEIVE_MSG,                 /* invalid receive message: version, client_id, length */
	MSG_ERR_QCMAP_MANAGER_FAILED,                /* get or set qcmap manager failure */
	MSG_ERR_NOT_SUPPORT,                         /* not support */
	MSG_ERR_NOT_ENOUGH_RESOURCES,                /* not enough resources */
	MSG_ERR_NOT_FOUND_RESOURCES,                 /* not found resources */
	MSG_ERR_SYSTEM_ERROR,                        /* the system error */
	MSG_ERR_QMI_ERROR,                           /* QMI error */
	MSG_ERR_ERROR = 0xffff,
} msg_error_type_e;
	
#endif /* __ERROR_MSGR_H__ */
