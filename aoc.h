#ifndef __AOC_H__
#define __AOC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <curl/curl.h>

#define BUF 45          // general purpose string buffer
#define MAXBUF 1024     // larger gen purpose string buffer
#define SESSION 100     // max buffer for session id, seems to always be 97 bytes

// struct to hold the http response in a string
struct string {
    char *ptr;
    size_t len;
};

// print usage and quit
void print_usage(void);

// build url for http request. task is submit answer or get puzzle input
char *concat_url(char *year, char *day, char task);

// get session id from ~/.santa file or create it if it doesn't exist
char *get_session_id(char *file_path);

// get puzzle input from AOC servers based on user's session id
void get_input(char *url, char *session_id);

// allocate space to store data in string struct
void init_string(struct string *s);

// store http results in the string struct
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);

// parse the html results that are returned
char *parse_results(char *s);

// submit puzzle answer to AOC servers
void submit_answer(char *url, char *session_id, char *body);

#endif