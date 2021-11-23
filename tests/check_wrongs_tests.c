#include "minunit/minunit.h"
#include "../src/eggnog.h"
#include <stdlib.h>

static char *s1;
static char *s2;
static char *s3;
static char *s4;

void test_setup(void)
{
	// should be NULL
	s1 = check_wrongs("2020", "1", "1234\n");
	// should be NULL
	s2 = check_wrongs("2020", "3", "1");
	// should be "12"
	s3 = check_wrongs("2020", "3", "12\n");
	// should be null
	s4 = check_wrongs("2020", "3", NULL);
}

void test_teardown(void)
{
}

MU_TEST(test_string_eq){
	mu_assert_string_eq(NULL, s1);
	mu_assert_string_eq(NULL, s2);
	mu_assert_string_eq("12\n", s3);
	mu_assert_string_eq(NULL, s4);
}

MU_TEST_SUITE(test_suite)
{
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_string_eq);
}

int main()
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}
