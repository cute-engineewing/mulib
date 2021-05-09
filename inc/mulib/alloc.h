#ifndef _MULIB_ALLOC_H
#define _MULIB_ALLOC_H

#include <mulib/common.h>

struct alloc_page
{
	struct alloc_page *next;

	size_t size;
	size_t used;
};

struct alloc
{
	struct alloc_page *first;
	struct alloc_page *last;
	struct alloc_page *best;
};

void alloc_init(struct alloc *alloc);

void alloc_deinit(struct alloc *alloc);

void *_alloc_alloc(struct alloc *alloc, size_t size, size_t align);

#define alloc_alloc(__alloc, __size) _alloc_alloc(__alloc, __size, 16)

#define alloc_make(__alloc, __type)                                            \
	(__type *)(_alloc_alloc(__alloc, sizeof(__type), alignof(__type)))

#endif /* !_MULIB_ALLOC_H */
