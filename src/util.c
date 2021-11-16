#include <stdio.h>
#include <time.h>
#include <string.h>

#include "eggnog.h"

// to use with testing the is_it_advent function
#define DEBUG_MONTH 11

/**
 * get_date
 *
 * @desc: get current year, month, & day.
 *
 * @return: an int array with [0] year, [1] month, [2] day
 * */
static unsigned int *get_date(void)
{
	static unsigned int d[3];
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	
	d[0] = tm.tm_year + 1900;
	d[1] = tm.tm_mon + 1;
	d[2] = tm.tm_mday;

	/**
	 *  return array:
	 *  ----------------------------------
	 *  | [0] year | [1] month | [2] day |
	 *  ----------------------------------
	 * */

	return d;
}

/**
 * print_usage
 *
 * @desc: print usage and exit
 * */
void print_usage(void)
{
    char *usage = "usage: nog [-y, -d, -s, -i, -l]\
        \n\nrequired flags: -y, -d\
\
        \n-y: year\
        \n-d: day\
        \n\noptional flags: -s, -i, -l\
\
        \n-s: submit -> submit puzzle answer. requires answer as arg\
        \n-i: input -> this is the default setting if only -d, -y are used\
        \n-l: level -> indicates which puzzle part for -s. defaults to 1";
	fprintf(stderr, "%s\n", usage);
}

/**
 * make_url
 *
 * @desc: make the URL for the task we want to do
 *
 * @param: `day` - the day of the puzzle we want.
 * @param: `year` - the year of the puzzle we want.
 * @param: `type` - what we want to do -> options: i[nput], s[ubmit], p[uzzle]
 *
 * @return: a string containing either puzzle input, a submission response, or directions
 * */
char *make_url(char *day, char *year, char type)
{
	static char new_url[URL_BUF] = "https://adventofcode.com/";;
	strcat(new_url, year);
	strcat(new_url, "/day/");
	strcat(new_url, day);
	if (type == 'i') {
		strcat(new_url, "/input");
	}
	return new_url;
}


/**
 * is_it_advent?
 *
 * if Advent of Code is currently happening
 * we don't want to bother with day and year flags
 * automatically get puzzle data for current day
 *
 * @desc: Check if advent of code is currently happeneing
 *
 * @return: Pointer to `unsigned int` array continaing date/day if currently Advent. Else `NULL`.
 * */
unsigned int *is_it_advent(void)
{
	unsigned int *d = get_date();

#if DEBUG
	if (d[1] == DEBUG_MONTH) {
		return d;
	} else return NULL;
#else
	if (d[1] == 12 && d[2] <= 25) {
		// we are in advent! yay!
		return d;
	} else return NULL; // not advent...
#endif
}

/**
 * handle_flags
 *
 * @desc: check what flags where used and make sure there's only one.
 *
 * @param: `p` - puzzle directions flag.
 * @param: `i` - input flag.
 * @param: 's' - submit answer flag.
 *
 * @return: either p, i, s depending on what was passed, else NULL on error;
 * */
char *handle_flags(int p, int i, int s)
{
	int flags = 0;
	if (p == 1) flags++;
	if (i == 1) flags++;
	if (s == 1) flags++;

	if (flags == 1) {
		// this is probably not efficient
		if (p == 1) return "p";
		else if (i == 1) return "i";
		else return "s";
	} else if (flags > 1) {
		return NULL;
	} else {
		// no flags, input by default
		return "i";
	}
	// won't make it here ever, but just in case...
	return NULL;
}


// build url based on task
char *build_url(char *year, char *day, char *task)
{
	static char url[URL_BUF];
	strcpy(url, "https://adventofcode.com/");
	strcat(url, year);
	strcat(url, "/day/");
	strcat(url, day);

	if (strcmp(task, "i") == 0) {
		// puzzlie input
		strcat(url, "/input");
	} else if (strcmp(task, "s") == 0){
		// submit an answer
		strcat(url, "/answer");
	}

	return url;
}
