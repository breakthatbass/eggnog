#include "minunit/minunit.h"
#include "../src/eggnog.h"
#include <stdlib.h>

static char *s1;

void test_setup(void)
{
    // exists
    s1 = check_cache("2020", "1", "i");
}

void test_teardown(void)
{
    free(s1);
}

MU_TEST(test_string_eq){
	mu_assert_string_eq("123\n456\n", s1);
}

MU_TEST_SUITE(test_suite)
{
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_string_eq);
}

int main() {
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}
