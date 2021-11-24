#include "minunit/minunit.h"
#include "../src/eggnog.h"
#include <stdlib.h>

// handle_flags tests
static char *hf1;
static char *hf2;
static char *hf3;
static char *hf4;
static char *hf5;
static char *hf6;

// build_url tests
static char url1[100];
static char url2[100];
static char url3[100];


void test_setup(void)
{
	/**
	 * handle_flags(int p, int i, int s)
	 * */
	// s flag
	hf1 = handle_flags(0, 0, 1);
	// i flag
	hf2 = handle_flags(0, 1, 0);
	// p flag
	hf3 = handle_flags(1, 0, 0);
	// no flags
	hf4 = handle_flags(0, 0, 0);
	// two flags - null
	hf5 = handle_flags(1, 0, 1);
	// all flags - null
	hf6 = handle_flags(1, 1, 1);
	// unexpected numbers - null
	hf6 = handle_flags(8, 0, 0);

	/**
	 * char *build_url(char *year, char *day, char *task)
	 * */
	strcpy(url1, build_url("2020", "1", "i"));
	strcpy(url2, build_url("2020", "2", "s"));
	strcpy(url3, build_url("hello", "foo", "bar"));
}

void test_teardown(void)
{
    // nothing...
}

MU_TEST(test_string_eq){
	// handle_flags
	mu_assert_string_eq("s", hf1);
	mu_assert_string_eq("i", hf2);
	mu_assert_string_eq("p", hf3);
	mu_assert_string_eq(NULL, hf4);
	mu_assert_string_eq(NULL, hf5);
	mu_assert_string_eq(NULL, hf6);

	// build_url
	mu_assert_string_eq("https://adventofcode.com/2020/day/1/input", url1);
	mu_assert_string_eq("https://adventofcode.com/2020/day/2/answer", url2);
	mu_assert_string_eq("https://adventofcode.com/hello/day/foo", url3);
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
