#ifndef _MULIB_MUTEST_H
#define _MULIB_MUTEST_H
#include "common.h"
#include <stdio.h>

#define MUTEST_SUCCESS 0
#define MUTEST_ERROR   1

typedef int (*mutest_func_t)();

struct mutest_test
{
	mutest_func_t func;
	const char *name;
};

int imp_mutest_run_grouped_test(struct mutest_test *test, size_t length,
								const char *name);

#define mutest_add_test(test_function)                                         \
	{                                                                          \
		test_function, #test_function                                          \
	}

#define mutest_run_grouped_test(test_list)                                     \
	imp_mutest_run_grouped_test(                                               \
		test_list, sizeof(test_list) / sizeof(struct mutest_test), #test_list)

#define mutest_excpect(condition)                                              \
	if (!(condition))                                                          \
	{                                                                          \
		printf("expected %-10s | ", #condition);                               \
		return MUTEST_ERROR;                                                   \
	}

#define mutest_string_equal(str1, str2)                                        \
	if ((strcmp(str1, str2) != 0))                                             \
	{                                                                          \
		printf("expected %s == %-10s | ", str1, str2);                         \
		return MUTEST_ERROR;                                                   \
	}

#endif /* _MULIB_MUTEST_H */
