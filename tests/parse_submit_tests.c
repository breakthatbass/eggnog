#include "minunit/minunit.h"
#include "../src/eggnog.h"
#include <stdlib.h>
#include <string.h>


static char *rightanswer;
static char *wronganswer;

static char parse_right[100];
static char parse_wrong[100];

// read file into a string for tests
char *readfile(char *file)
{
	FILE *fp;
	long filesize;
	char *file_contents;

	fp = fopen(file, "r");
	if (fp == NULL) return NULL;

	// get file size for allocation
	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	file_contents = calloc(filesize, sizeof(char));
	if (file_contents == NULL) return NULL;

	// read file into string
	fread(file_contents, 1, filesize, fp);
	fclose(fp);

	return file_contents; // user must free
}


void test_setup(void)
{
	rightanswer = readfile("tests/rightanswer.txt");
	wronganswer = readfile("tests/wronganswer.txt");

	strcpy(parse_right, parse_submit(rightanswer));
	strcpy(parse_wrong, parse_submit(wronganswer));
}

void test_teardown(void)
{
	free(rightanswer);
	free(wronganswer);
}

MU_TEST(test_string_eq){
	mu_assert_string_eq("That's the right answer!", parse_right);
	mu_assert_string_eq("That's not the right answer; your answer is too high.", parse_wrong);
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
