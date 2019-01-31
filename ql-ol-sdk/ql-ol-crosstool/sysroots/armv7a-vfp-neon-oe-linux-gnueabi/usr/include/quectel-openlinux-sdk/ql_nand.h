#ifndef __QL_NAND_H__
#define __QL_NAND_H__

#include "ql_type.h"


//------------------------------------------------------------------------------
/*
* Function:     Ql_Set_Restore
* 
* Description:
*               set system firmware restore to the backup version.
*				if this function execute success(return 1), you can reboot the system,
*				and the system will restore to backup version after reboot.reboot will need 50 seconds.
*
* Parameters:
*               NULL
* Return:        
*               1, this function succeeds.
*               0, failed to execute the function. 
*/
//------------------------------------------------------------------------------
int Ql_Set_FirmwareRestore(void);


//------------------------------------------------------------------------------
/*
* Function:     Ql_Get_RestoreCnt
* 
* Description:
*               get the linux system and modem restored counts
*				
*
* Parameters:
*               linuxSystemCnt: linux system restored counts
*				modemCnt: modem restored counts
* Return:        
*               1, this function succeeds.
*               0, failed to execute the function. 
*/
//------------------------------------------------------------------------------


int Ql_Get_RestoreCnt(u32 *linuxSystemCnt, u32 *modemCnt);


#endif /* __MTD_ABI_H__ */

