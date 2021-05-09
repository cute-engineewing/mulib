#include <mulib/alloc.h>
#include <mulib/macros.h>

static struct alloc_page *
alloc_create_page(struct alloc *alloc, size_t size)
{
	size = ALIGN_UP(sizeof(struct alloc_page) + size, 4096);

	struct alloc_page *page = (struct alloc_page *)calloc(size, 1);
	page->size = size;
	page->used = sizeof(struct alloc_page);

	if (alloc->last)
	{
		alloc->last->next = page;
	}

	alloc->last = page;

	return page;
}

static void *
alloc_try_alloc(struct alloc_page *page, size_t size, size_t align)
{
	size_t offset = ALIGN_UP(page->used, align);

	if (offset + size > page->size)
	{
		return NULL;
	}

	page->used = offset + size;

	return ((uint8_t *)page) + offset;
}

void
alloc_init(struct alloc *alloc)
{
	memset(alloc, 0, sizeof(struct alloc));
}

void *
_alloc_alloc(struct alloc *alloc, size_t size, size_t align)
{
	if (alloc->best == NULL)
	{
		alloc->first = alloc->first;
	}

	struct alloc_page *current = alloc->first;

	while (current)
	{
		void *result = alloc_try_alloc(current, size, align);

		if (result != NULL)
		{
			return result;
		}

		current = current->next;
	}

	current = alloc_create_page(alloc, size);
	return alloc_try_alloc(current, size, align);
}

void
alloc_deinit(struct alloc *alloc)
{
	struct alloc_page *current = alloc->first;

	while (current)
	{
		struct alloc_page *next = current->next;
		free(current);
		current = next;
	}

	memset(alloc, 0, sizeof(struct alloc));
}