#include "minunit/minunit.h"
#include "../src/eggnog.h"
#include <stdio.h>
#include <string.h>

#define TBUF 100

// handle_flags tests
static char *hf1;
static char *hf2;
static char *hf3;
static char *hf4;
static char *hf5;
static char *hf6;

// build_url tests
static char url1[TBUF];
static char url2[TBUF];
static char url3[TBUF];

// check_input
static int ipt1;
static int ipt2;
static int ipt3;
static int ipt4;
static int ipt5;
static int ipt6;
static int ipt7;
static int ipt8;
static int ipt9;

// prep_submit
static char ps1[TBUF] = {0};
static char ps2[TBUF] = {0};
static char ps3[TBUF] = {0};


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

	/**
	 * int check_input(char *year, char *day)
	 * */
	// some tests that should all return 0
	ipt1 = check_input("2020", "25");
	ipt2 = check_input("2015", "15");
	ipt3 = check_input("2017", "1");
	// some tests that should all return 1
	ipt4 = check_input("2020", "26");
	ipt5 = check_input("2015", "0");
	ipt6 = check_input("2017", "-1");
	// some tests that should all return 2
	ipt7 = check_input("2014", "25");
	ipt8 = check_input("1952", "15");
	ipt9 = check_input("2098", "1");

	/**
	 * char *prep_submit(char *answer, char *lev)
	 * */
	strcpy(ps1, prep_submit("1234", "1"));
	strcpy(ps2, prep_submit("789", NULL));
	strcpy(ps3, prep_submit("456", "2"));

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
	
	// prep_submit
	mu_assert_string_eq("level=1&answer=1234", ps1);
	memset(ps1, 0, TBUF);
	mu_assert_string_eq("level=1&answer=789", ps2);
	memset(ps2, 0, TBUF);
	mu_assert_string_eq("level=2&answer=456", ps3);
	memset(ps3, 0, TBUF);
}

MU_TEST(test_check) {
	mu_check(ipt1 == 0);
	mu_check(ipt2 == 0);
	mu_check(ipt3 == 0);
	mu_check(ipt4 == 1);
	mu_check(ipt5 == 1);
	mu_check(ipt6 == 1);
	mu_check(ipt7 == 2);
	mu_check(ipt8 == 2);
	mu_check(ipt9 == 2);
}

MU_TEST_SUITE(test_suite)
{
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_string_eq);
	MU_RUN_TEST(test_check);
}

int main() {
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}
