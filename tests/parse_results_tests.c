#include "minunit/minunit.h"
#include "../aoc.h"

static char *rightanswer;
static char *wronganswer;
static char *junkfile;

static char *a;
static char *b;
static char *c;

void test_setup(void)
{
    rightanswer = debug_html("rightanswer.txt");
    wronganswer = debug_html("wronganswer.txt");
    junkfile = debug_html("junk.txt");

    a = parse_results(rightanswer);
    b = parse_results(wronganswer);
    c = parse_results(junkfile);
}

void test_teardown(void)
{
    free(rightanswer);
    free(wronganswer);

    free(a);
    free(b);
    // no need to free c since a NULL return means nothing was allocated
}

MU_TEST(test_string_eq){
	mu_assert_string_eq("That's the right answer!", a);
    mu_assert_string_eq("That's not the right answer; your answer is too high.", b);
    mu_assert_string_eq(NULL, c);
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