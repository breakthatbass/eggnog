#include "minunit/minunit.h"
#include "../aoc.h"

static char cookie1[SESSION];
static char cookie2[SESSION];

void test_setup(void)
{
    strcpy(cookie1, get_session_id("."));
    strcpy(cookie2, get_session_id("."));
}

void test_teardown(void)
{
    // nothing
}

MU_TEST(test_string_eq){
	mu_assert_string_eq("12345", cookie1);
}

MU_TEST(test_check) {
	mu_check(strcmp("poo", cookie2) != 0);
}

MU_TEST_SUITE(test_suite)
{
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_string_eq);
    MU_RUN_TEST(test_check);
}

int main(int argc, char *argv[]) {
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}