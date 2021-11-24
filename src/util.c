#include <stdio.h>
#include <stdlib.h>
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
	} else if (flags > 1 || flags == 0) {
		return NULL;
	} else {
		// no flags, input by default
		return "i";
	}
	// won't make it here ever, but just in case...
	return NULL;
}


/**
 * build_url
 *
 * @desc: make the URL for the task we want to do
 *
 * @param: `day` - the day of the puzzle we want.
 * @param: `year` - the year of the puzzle we want.
 * @param: `type` - what we want to do -> options: i[nput], s[ubmit], p[uzzle]
 *
 * @return: a string containing either puzzle input, a submission response, or directions
 * */
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


/**
 * check_input
 *
 * @desc: if year and day are supplied, make sure they are valid.
 *
 * @param: `year` - the year argument supplied by client.
 * @param: `day` - the day argument supplied by client.
 *
 * @return: 0 if `year` & `day` are valid. `1` if day is invalid. `2` if year is invalid.
 * */
int check_input(char *year, char *day)
{
	unsigned int d = atoi(day);
	unsigned int y = atoi(year);
	unsigned int *date = get_date();

	if (d < 1 || d > 25) return 1;
	

	if (
		(date[0] < 2015) ||
		(y == date[0] && date[1] < 12) ||
		(y > date[0]))
			return 2;
	
	return 0;
}


/**
 * prep_submit
 *
 * @desc: Create the header to send containing puzzle part and answer. If no answer, read from `stdin`.
 * 
 * @param: `answer` - The answer to a puzzle.
 * @param: `lev` - The puzzle part we want to answer.
 *
 * @return: Pointer to a str -> the header.
 * */
char *prep_submit(char *answer, char *lev)
{
	// build url for submitting
	static char header[URL_BUF] = "level=";
	if (lev) strcat(header, lev);
	else strcat(header, "1");
	strcat(header, "&answer=");
	if (answer)
		strcat(header, answer);
	else {
		// we read from stdin
		char buf[URL_BUF] = {0};
		fgets(buf, URL_BUF, stdin);
		buf[strcspn(buf, "\r\n")] = 0;

		printf("%s\n", buf);
		strcat(header, buf);
	}
	return header;
}


#define RED "\033[31m"
#define GREEN "\033[32m"
#define BOLDRED "\033[1m\033[31m"
#define BOLDGREEN "\033[1m\033[32m"
#define RESET "\033[0m"

void print_error(char f)
{
	printf("%serror:%s '-%c' is not a valid argument\n\n", BOLDRED, RESET, f);

	printf("\n%sUSAGE:%s\n", BOLDRED, RESET);
	printf("    nog [--day=<day> --year=<year>] [OPTION] \n");

	printf("\nTry %s--help%s or %s-h%s for more information\n", BOLDGREEN, RESET, BOLDGREEN, RESET);
}

/**
 * print_usage
 *
 * @desc: print usage and exit
 * */
void print_usage(void)
{
	printf("%seggnog%s %s\n", BOLDGREEN, RESET, VERSION);
	printf("\n🎅🏻 🎅🏻 🎅🏻\n\n");
	printf("Download inputs and submit answers to Advent of Code puzzles in your terminal.\n");
	
	printf("\n%sUSAGE:%s\n", BOLDRED, RESET);
	printf("    nog [--day=<day> --year=<year>] [OPTION] \n");
	
	printf("\nRequired unless Advent is happening - latest puzzle will automatically\n");
	printf("be returned.\n");

	/** YEAR **/
	printf("\n    %s-y <year>%s, %s--year=<year>%s\n", GREEN, RESET, GREEN, RESET);
	printf("\tThe year of the puzzle you want to do.\n");

	// DAY
	printf("\n    %s-d <day>%s, %s--day=<day>%s\n", GREEN, RESET, GREEN, RESET);
	printf("\tThe day of the puzzle you want to do.\n");

	printf("%sOPTIONS:%s\n", BOLDRED, RESET);

	// INPUT
	printf("If no options are supplied, %sinput%s is assumed.\n", BOLDGREEN, RESET);
	printf("\n    %s-i%s, %s--input%s\n", GREEN, RESET, GREEN, RESET);
	printf("\tDownload puzzle input.\n");

	printf("\n    %s-p%s, %s--directions%s\n", GREEN, RESET, GREEN, RESET);
	printf("\tGet puzzle directions\n");

	
	// SUBMIT
	printf("\n    %s-s%s, %s--submit=<answer>%s\n", GREEN, RESET, GREEN, RESET);
	printf("\tsubmit an answer to a puzzle. If no answer is supplied, read from\n");
	printf("\t%sstdin%s. Using '%s-s%s' automatically reads from %sstdin%s.\n", BOLDRED, RESET, RED, RESET, BOLDRED, RESET);

	// LEVEL
	printf("\n    %s-l <puzzle part>%s, %s--level=<puzzle part>%s\n", GREEN, RESET, GREEN, RESET);
	printf("\tIndicate whether submitting an answer to part 1 or part 2 of a puzzle.\n");
	printf("\tIf left out, part 1 is assumed. Required for submitting part 2 answers.\n");

	// VERSION
	printf("\n    %s-v%s, %s--version%s\n", GREEN, RESET, GREEN, RESET);
	printf("\tPrint version.\n");

	// HELP
	printf("\n    %s-h%s, %s--help%s\n", GREEN, RESET, GREEN, RESET);
	printf("\tPrint this info and quit.\n");
}

void print_version(void)
{
	printf("eggnog %s\n", VERSION);
}
