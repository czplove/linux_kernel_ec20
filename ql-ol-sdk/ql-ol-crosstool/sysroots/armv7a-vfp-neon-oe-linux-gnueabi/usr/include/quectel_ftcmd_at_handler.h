/**  @file
  quectel_ftcmd_at_handler.h

  @brief
  This file provides an API to meet FC test requirements.

*/

#ifndef __QUECTEL_FTCMD_AT_HANDLER__
#define __QUECTEL_FTCMD_AT_HANDLER__

#ifdef __cplusplus
extern "C"{
#endif

#include "quectel_at_cmd_defs.h"

/*========================================================================
  FUNCTION:  <quectel_parse_ftcmd_cmd_input>
=========================================================================*/
/** @brief
   <Handles FC test parameter functions>

    @param[in][out]
	<cmd> <AT Command>
        <param> <adfwd parameter>

    @return
        TRUE--There are corresponding parameter.
        FALSE--There is no corresponding parameter.
*/
/*=======================================================================*/
extern boolean quectel_parse_ftcmd_cmd_input(AtCmd *cmd, atfwd_param_s_type *param);

/*========================================================================
  FUNCTION:  <quectel_handle_ftcmd_at_cmd>
=========================================================================*/
/** @brief
   <Handles FC test command functions>

    @param[in][out]
        <param> <adfwd parameter>
	<response> <AT Command response>

    @return
        TRUE--There are corresponding handle functions.
        FALSE--There is no corresponding handle functions.
*/
/*=======================================================================*/
extern boolean quectel_handle_ftcmd_at_cmd(atfwd_param_s_type *param, void *response);

#ifdef __cplusplus
}
#endif
#endif
