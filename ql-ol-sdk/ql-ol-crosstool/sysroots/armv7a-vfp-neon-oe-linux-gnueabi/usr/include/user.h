#ifndef __FOTA_USER_H
#define __FOTA_USER_H

struct MtdPartition {
    int num;
    unsigned int size;
    unsigned int erase_size;
    unsigned int pos;
    char name[64];
};

extern struct MtdPartition* mtd_open(const char *);
extern void mtd_close(struct MtdPartition *);
extern int mtd_read(const struct MtdPartition *part, unsigned int offset, unsigned int len, char *databuf, unsigned int *retpos);
extern int mtd_write_align(struct MtdPartition *part, unsigned int offset, 
                           unsigned int len, char *buf, unsigned int *retpos);
#endif
