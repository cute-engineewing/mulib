#include "mulib/mutest.h"
#include "mulib/scan.h"

int
test_positive_result()
{
	return MUTEST_SUCCESS;
}

int
test_mutest_equality_result()
{
	mutest_string_equal("hey", "hey");
	mutest_excpect(0 == 0);
	return MUTEST_SUCCESS;
}

int
test_for_mutest()
{
	struct mutest_test mutest_grouped_test_test[] = {
		mutest_add_test(test_positive_result),
		mutest_add_test(test_mutest_equality_result),
	};

	return mutest_run_grouped_test(mutest_grouped_test_test);
}
int
main(void)
{
	return test_for_mutest();
}
