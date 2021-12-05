#include "minunit/minunit.h"
#include "../src/eggnog.h"
#include <stdlib.h>

static char *s1;
static char *s2;
static char *s3;
static char *s4;

void test_setup(void)
{
    // exists
    s1 = check_cache_answers("2020", "1", "r", "1");
    // doesn't exist
    s2 = check_cache_answers("2020", "2", "r", "1");
    // exists
    s3 = check_cache_answers("2020", "3", "w", "2");
    // deosn't exist
    s4 = check_cache_answers("2020", "4", "w", "2");
}

void test_teardown(void)
{
    free(s1);
    free(s3);
}

MU_TEST(test_string_eq){
	mu_assert_string_eq("1234\n", s1);
    mu_assert_string_eq(NULL, s2);
	mu_assert_string_eq("345\n601\n56789\n12\n34\n", s3);
	mu_assert_string_eq(NULL, s4);
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
