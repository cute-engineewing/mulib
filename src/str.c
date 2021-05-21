#include <mulib/str.h>

bool
is_string_a_number(const char *str)
{
	size_t length = strlen(str);

	for (size_t i = 0; i < length; i++)
	{
		if (!(isdigit(str[i]) || str[i] == '-'))
		{
			return false;
		}
	}
	return true;
}