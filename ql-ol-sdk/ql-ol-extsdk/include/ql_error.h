/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Quectel Co., Ltd. 2016
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ql_error.h 
 *
 * Project:
 * --------
 *   OpenLinux
 *
 * Description:
 * ------------
 *   error code  defines.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 *----------------------------------------------------------------------------
 * 
 ****************************************************************************/


#ifndef __QL_ERROR_H__
#define __QL_ERROR_H__

/****************************************************************************
 * Error Code Definition
 ***************************************************************************/
enum {
	RES_OK = 0,
	RES_BAD_PARAMETER  = -1,     ///< Parameter is invalid.
	RES_IO_NOT_SUPPORT = -2,
	RES_IO_ERROR = -3,
	RES_NOT_IMPLEMENTED = -4
};

#endif //QL_ERROR_H 
