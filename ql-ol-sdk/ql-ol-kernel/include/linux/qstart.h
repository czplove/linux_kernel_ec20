#ifndef __QSTART_H
#define __QSTART_H

//#include "quectel-features-config.h"
//
//#define NORMALMODE 0
//#define RECOVERYMODE 1
//
//#ifdef QUECTEL_MODEM_RESTORE_UBI
//#define SYSTEM_MTD_NUMBER 18 //I.E 5.1pcb /dev/mtd18 is systemfs
//#define RECOVERYFS_MTD_NUMBER 15 //I.E 5.1pcb /dev/mtd15 is recoveryfs
//#else
//#define SYSTEM_MTD_NUMBER 15 //I.E /dev/mtd15 is systemfs
//#define RECOVERYFS_MTD_NUMBER 13 //I.E /dev/mtd13 is recoveryfs
//#endif

bool get_bootmode(unsigned int *mode);
#endif
