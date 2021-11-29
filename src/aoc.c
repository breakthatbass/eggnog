/******************************************************************************
*
*   Author: Taylor Gamache
*   Email: gamache.taylor@gmail.com
*
*   @name: curl aoc
*   @description: functions to get info from AoC
*
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "eggnog.h"


struct string {
    char *ptr;
    size_t len;
};

// setup struct string *s to be used to store a string and a size
static void curl_init_string(struct string *s)
{
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

// store http results in the string struct
// s->ptr needs to be freed when done
static size_t curl_write_func(void *ptr, size_t size, size_t nmemb, struct string *s)
{
    size_t new_len = s->len + (size*nmemb);
    s->ptr = realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}

/**
 * get_input
 * 
 * @desc: get puzzle input or directions from AoC based on user's session id
 * 
 * @param: `url` - a string containing the proper url for what we want
 * @param: `session_id` - the user's session id (required for login)
 * 
 * @return: the input or puzzle directions as a string, if problem connecting, NULL
 * */
char *get_input(char *url, char *session_id)
{
	CURL *curl;
	CURLcode res;
   struct string s;
    
   curl_init_string(&s);
	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);

#if !DEBUG  // we don't want to hit up the servers while debugging/testing
        // we need the session id to be predicated with 'session='
        char cookie[SESSION] = "session=";
	    strcat(cookie, session_id);
		curl_easy_setopt(curl, CURLOPT_COOKIE, cookie);

		// silence warnings when running tests
		session_id = NULL;
#endif
        // don't need this anymore and setting to NULL silences warnings when testing
        session_id = NULL;
        // save the input into a string
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_func);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

		res = curl_easy_perform(curl);
    
		if (res != CURLE_OK) {
			fprintf(stderr, "request failed. error: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            free(s.ptr);
            return NULL;
        }
		curl_easy_cleanup(curl);
	}
    return s.ptr;
}


/**
 * submit_answer
 *
 * @desc: submit a puzzle answer and recieve response
 *
 * @param: `url` - string containing proper url for submitting answer
 * @param: `session_id` - user's session id
 * @param: `header` - part 1 or part 2 of puzzle
 *
 * @return: An html response as a string or `NULL` on error.
 * */
char *submit_puzzle_answer(char *url, char *session_id, char *header)
{
    CURL *curl;
    CURLcode res;
    struct string s;
    char cookie[MAXBUF] = "session=";

    curl_init_string(&s);

    // we need the session id to be predicated with 'session='
    strcat(cookie, session_id);

    curl = curl_easy_init();

    if (curl) {
        // POST request of our answer to the AOC servers
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, header);
        curl_easy_setopt(curl, CURLOPT_COOKIE, cookie);

        // save the html results into a string so we can parse it
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_func);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl resquest failed\nerror: %s\n", curl_easy_strerror(res));
				free(s.ptr);
				return NULL;
        }
        curl_easy_cleanup(curl);
    }
    // return the string that holds the html response
    return s.ptr;
}
