#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eggnog.h"

// for testing the parser
#include <assert.h>
static char *get_html_test(void)
{
	FILE *fp;
	char *return_str;
	long size;
	char *path = "tests/directions_done.html";
	
	fp = fopen(path, "r");
	assert(fp);

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	return_str = calloc(size+5, sizeof(char));
	assert(return_str);

	fread(return_str, 1, size, fp);
	fclose(fp);
	return return_str;
}


/**
 * parse_results
 *
 * @desc: parse html results recieved after submitting an answer
 *
 * @param: `s` - the html in the form of a string.
 *
 * @ return: the string containing the repsonse, or `NULL`.
 * */
char *parse_results(char *s)
{
    char *tmp, *response_len, *buf;
    int n;

    if (strstr(s, "<main>\n<article><p>")) {
        // get pointer to this part of html
        tmp = strstr(s, "<main>\n<article><p>");
        //free(s);
        // move ahread past the <p>
        tmp+=19;

        // no matter what comes back as a response, there is always a double space ending the string.
        // this just gets a length we can use to pull just what we want
        response_len = strstr(tmp, "  ");
        n = response_len - tmp;

        buf = malloc(sizeof(char)*n+2);
        if (buf == NULL) {
            return NULL;
        }

        strncpy(buf, tmp, n);
        buf[n] = '\0';

        return buf;
    }
    return NULL;
}

// keep track if we're in an HTML tag or not
static int IN_TAG = 0;

char *parse_dir(char *html)
{
	char *tmp, *tmp2;
	const char *START = "class=\"day-desc\"><h2>";
	size_t startlen = strlen(START);

	tmp = strstr(html, START);
	tmp+=startlen;

	char *s = malloc(202020);

	//printf("%s", tmp);
	int i = 0;
	while (*tmp) {

		if (*tmp == '<') {
			IN_TAG = 1;
			tmp++;
		} else if (*tmp == '>') {
			IN_TAG = 0;
			tmp++;
		} else if (IN_TAG == 0) {
			*s = *tmp;
			s++;
			tmp++;
			i++;
		} else {
			tmp++;
			continue;
		}
	}
	s-=i;
	// now just make our way through the html keeping what we want
	printf("%s", s);
	return s;
}


/*
 // quick tests
int main()
{
	char *d = get_html_test();


	char *s = parse_dir(d);

	printf("%s", s);
	return 0;
}
*/
