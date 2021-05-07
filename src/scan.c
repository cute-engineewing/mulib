#include <mulib/scan.h>

char
scan_peek(struct scan *scan, size_t offset)
{
	if (scan->head + offset >= scan->size)
	{
		return '\0';
	}

	return scan->buffer[scan->head + offset];
}

char
scan_current(struct scan *scan)
{
	return scan_peek(scan, 0);
}

void
scan_forward(struct scan *scan)
{
	if (scan_ended(scan))
	{
		return;
	}

	scan->head++;
}

void
scan_forward_n(struct scan *scan, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
	{
		scan_forward(scan);
	}
}

bool
scan_ended(struct scan *scan)
{
	return scan->head + 1 > scan->size;
}

bool
scan_continue(struct scan *scan)
{
	return !scan_ended(scan);
}

bool
scan_current_is_c(struct scan *scan, char c)
{
	return scan_current_is_b(scan, &c, 1);
}

bool
scan_current_is_w(struct scan *scan, const char *word)
{
	return scan_current_is_b(scan, word, strlen(word));
}

bool
scan_current_is_b(struct scan *scan, const char *buffer, size_t size)
{
	size_t i;

	for (i = 0; i < size; i++)
	{
		if (scan_peek(scan, i) != buffer[i])
		{
			return false;
		}
	}

	return true;
}

bool
scan_any_c(struct scan *scan, char c)
{
	return scan_current(scan) == c;
}

bool
scan_any_w(struct scan *scan, const char *word)
{
	return scan_any_b(scan, word, strlen(word));
}

bool
scan_any_b(struct scan *scan, const char *buffer, size_t size)
{
	size_t i;

	for (i = 0; i < size; i++)
	{
		if (scan_current_is_c(scan, buffer[i]))
		{
			return true;
		}
	}

	return false;
}

bool
scan_skip_c(struct scan *scan, char c)
{
	return scan_skip_b(scan, &c, 1);
}

bool
scan_skip_w(struct scan *scan, const char *word)
{
	return scan_skip_b(scan, word, strlen(word));
}

bool
scan_skip_b(struct scan *scan, const char *buffer, size_t size)
{
	if (!scan_current_is_b(scan, buffer, size))
	{
		return false;
	}

	scan_forward_n(scan, size);

	return true;
}

bool
scan_eat_c(struct scan *scan, char c)
{
	return scan_eat_b(scan, &c, 1);
}

bool
scan_eat_w(struct scan *scan, const char *word)
{
	return scan_eat_b(scan, word, strlen(word));
}

bool
scan_eat_b(struct scan *scan, const char *buffer, size_t size)
{
	bool any = false;

	while (scan_any_b(scan, buffer, size))
	{
		scan_forward(scan);
		any = true;
	}

	return any;
}
