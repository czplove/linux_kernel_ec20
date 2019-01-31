/*
 * @file: quectel_malloc.h
 *
 */

#ifndef __QUECTEL_MALLOC_H__
#define __QUECTEL_MALLOC_H__

static inline void *quectel_malloc(size_t size)
{
	void *ptr;
	
	if(size <= 0) {
		return NULL;
	}

	ptr = malloc(size);
	if(ptr) {
		memset(ptr, 0, size);
	}
	return ptr;
}

static inline void _quectel_free(void **ptr)
{
	if(NULL != ptr && NULL != (*ptr)) {
		free(*ptr);
		*ptr = NULL;
	}
}
#define quectel_free(ptr) _quectel_free((void **)(ptr))

#endif
