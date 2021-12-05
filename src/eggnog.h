#ifndef _AOC_H_
#define _AOC_H_

#define DEBUG 0
#define DEBUG_URL "https://raw.githubusercontent.com/breakthatbass/advent_of_code2020/main/day01/input"

#define VERSION "0.0.2"

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
char *check_cache_answers(char *year, char *day, char *type, char *level);
char *check_wrongs(char *year, char *day, char *answer, char *level);

/****************************************************************************
 *		util.c															    *
 ***************************************************************************/

unsigned int *is_it_advent(void);
// print usage and exit
void print_usage(void);
// make sure one or zero flags are passed
char *handle_flags(int p, int i, int s);
// build url based on task
char *build_url(char *year, char *day, char *task);
// check that year and day arguments have puzzles
int check_input(char *year, char *day); 
// print current version
void print_version(void);
// print error info if invalid arg is used
void print_error(char f);
// prep the url for submitting an answer
char *prep_submit(char *answer, char *lev);

/****************************************************************************
 *		parse.c														    *
 ***************************************************************************/

// print puzzle directions in a colorful and pretty way
void pretty_print(char *html);
// parse he submission response html
char *parse_submit(char *html);

#endif
