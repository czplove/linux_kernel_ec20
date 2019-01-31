#ifndef __QL_POWERDOWN_H__
#define __QL_POWERDOWN_H__

/*	Ql_Powerdown
 *	mode :
 *	0 : gracefully power down
 *	1 : gracefully reboot
 *	2 : Immediately power down system, without unmounting or syncing filesystems
 *	3 : Immediately reboot the system, without unmounting or syncing filesystems
 */

void Ql_Powerdown(int mode);

#endif  //__QL_POWERDOWN_H__
