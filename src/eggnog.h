#ifndef _AOC_H_
#define _AOC_H_

#include <curl/curl.h>

#define DEBUG 0
#define DEBUG_URL "https://raw.githubusercontent.com/breakthatbass/advent_of_code2020/main/day01/input"

#define VERSION "0.0.1"

#define URL_BUF 75      // general purpose string buffer
#define MAXBUF 1024     // larger gen purpose string buffer
#define SESSION 125     // session ids seem to be about 97 bytes but we need room for 'session='

char *get_input(char *url, char *session_id);

char *submit_puzzle_answer(char *url, char *session_id, char *header);

/****************************************************************************
 *		cache.c															    *
 ***************************************************************************/

char *check_cache(char *year, char *day, char *type);
int add_to_cache(char *data, char *year, char *day, char *type);
void create_cache(void);
char *get_session_id(void);

/****************************************************************************
 *		util.c															    *
 ***************************************************************************/

unsigned int *is_it_advent(void);
// print usage and exit
void print_usage(void);
// make the url for the task we want to do
char *make_url(char *day, char *year, char type);
// make sure one or zero flags are passed
char *handle_flags(int p, int i, int s);
// build url based on task
char *build_url(char *year, char *day, char *task);
// check that year and day arguments have puzzles
int check_input(char *year, char *day); 
// print current version
void print_version(void);

/****************************************************************************
 *		parser.c														    *
 ***************************************************************************/

// parse out response string from HTML
char *parse_results(char *s);

#endif
