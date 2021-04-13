#ifndef __AOC_H__
#define __AOC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <curl/curl.h>

#define BUF 75          // general purpose string buffer
#define MAXBUF 1024     // larger gen purpose string buffer
#define SESSION 125     // session ids seem to be about 97 bytes but we need room for 'session='

// struct to hold the http response in a string
struct string {
    char *ptr;
    size_t len;
};

// print usage and quit
void print_usage(void);

/*
*   concat_url:
*   build url for http request.
*   year and day is the year and day for the puzzle
*
*   task: i, s. i = get puzzle input, s = submit answer
*
*   RETURN VALUE:
*   on success, a string that represents a valid url for AOC, needs to be freed when done
*   on failure, NULL
*/
char *concat_url(char *year, char *day, char task);

/*  get session id from ~/.xmas file or create it if it doesn't exist
*   
*   file_path param is for testing.
*   the actual program only deals with $HOME/.xmas
*
*   RETURN VALUE:
*   string that holds the session id
*/
char *get_session_id(char *file_path);

// get puzzle input from AOC servers based on user's session id
char *get_input(char *url, char *session_id);

/*
*   init_string and write_func are used to store the html response from the AOC servers
*   in a string rather than printing right to terminal
*/

// allocate space to store data in string struct
void init_string(struct string *s);

// store http results in the string struct
// s->ptr needs to be freed when done
size_t write_func(void *ptr, size_t size, size_t nmemb, struct string *s);

/*  
*   parse_results:
*
*   parse the html results that is returned from the request (char *s)
*   a NULL check should be required for this function since that is what is returned
*   if it is unable to parse what it needs from the string
*   
*   RETURN VALUE:
*   on success, it returns the bit of html we want to display. it will have to be freed
*   on failure, it returns NULL and nothing gets allocated
*/
char *parse_results(char *s);

// submit puzzle answer to AOC servers, return an html response in a string
char *submit_answer(char *url, char *session_id, char *body);

/*
*   debug_html:
*
*   this fills a variable with an html response that is pre-stored in a file
*   to use for debugging and testing the parse_results function.
*   this is to prevent the constant calls to the AOC servers.
*   files to use for this funtion are in the tests directory.
*
*   RETURN VALUE:
*   returns the contents of a file as a string - needs to be freed
*/
char *debug_html(char *file);

#endif