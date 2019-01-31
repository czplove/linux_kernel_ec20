/** 
 * @file quectel_nand_at_handler.h
 * @brief 
 * @author wythe,wythe.wang@quectel.com
 * @version 1.0
 * @date 2015-10-10
 */

#ifndef __QUECTEL_NAND_AT_HANDLER__
#define __QUECTEL_NAND_AT_HANDLER__

#ifdef __cplusplus
extern "C"{
#endif

#include "quectel_at_cmd_defs.h"

extern boolean quectel_parse_nand_cmd_input(AtCmd *cmd, atfwd_param_s_type *param);
extern boolean quectel_handle_nand_at_cmd(atfwd_param_s_type *param, void *response);

#ifdef __cplusplus
}
#endif
#endif
