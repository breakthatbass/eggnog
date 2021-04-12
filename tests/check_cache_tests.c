#include "minunit/minunit.h"
#include "../cache.h"
#include <stdlib.h>
#include <stdio.h>

static char *str1;
static char *str2;

void test_setup(void)
{
    str1 = check_cache("./", "input_test");
    str2 = check_cache("./", "poo");
}

void test_teardown(void)
{
    free(str1);
}

MU_TEST(test_string_eq){
	mu_assert_string_eq("1632", str1);
    mu_assert_string_eq(NULL, str2);
}

MU_TEST_SUITE(test_suite)
{
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_string_eq);
}

int main(int argc, char *argv[]) {
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}