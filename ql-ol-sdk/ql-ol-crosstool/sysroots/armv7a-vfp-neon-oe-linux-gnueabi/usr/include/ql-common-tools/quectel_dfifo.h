/*===========================================================================

                         EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when       who      what, where, why
--------   ---      ---------------------------------------------------------
2018/06/04 zichar   Create, common data fifo operation.

===========================================================================*/
#ifndef __QUECTEL_DFIFO_H__
#define __QUECTEL_DFIFO_H__

typedef struct dfifo {
    int head;
    int tail;
    int size;
    char **data;
} dfifo_t;

int dfifo_init(dfifo_t *dfifo, int size);
void dfifo_deinit(dfifo_t *dfifo);
char* dfifo_get(dfifo_t *dfifo);
int dfifo_put(dfifo_t * dfifo, char *data);

#endif
