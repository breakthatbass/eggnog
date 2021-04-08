#include "minunit/minunit.h"
#include "../aoc.h"

static char *str1;
static char *str2;
static char *str3;

void test_setup(void)
{
    str1 = concat_url("2020", "1", 'i');
    str2 = concat_url("2015", "23", 's');
    str3 = concat_url("2018", "19", 'x');
}

void test_teardown(void)
{
    free(str1);
    free(str2);
    free(str3);
}

MU_TEST(test_string_eq){
	mu_assert_string_eq("https://adventofcode.com/2020/day/1/input", str1);
    mu_assert_string_eq("https://adventofcode.com/2015/day/23/answer", str2);
    mu_assert_string_eq(NULL, str3);
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