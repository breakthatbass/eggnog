#include "aoc.h"


void print_usage(void)
{
    char *usage = "usage: xmas [-y, -d, -s, -i, -l]\
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
	exit(EXIT_FAILURE);
}


/*
*   concat_url:
*   build url for http request.
*   year and day is the year and day for the puzzle
*
*   task: i, s. i = get puzzle input, s = submit answer
*/
char *concat_url(char *year, char *day, char task)
{
    if (task != 'i' && task != 's') return NULL;

    char *url = malloc(sizeof(char)*BUF);
    if (url == NULL) {
        fprintf(stderr, "concat_url: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(url, "https://adventofcode.com/");
	strcat(url, year);
	strcat(url, "/day/");
	strcat(url, day);

    // i = get puzzle input
    // s = submit an answer
    if (task == 'i') strcat(url, "/input");
    else strcat(url, "/answer");

    return url;
}


/*  get session id from ~/.xmas file or create it if it doesn't exist
*   
*   file_path param is for testing.
*   the actual program only deals with $HOME/.xmas
*
*   RETURN VALUE:
*   string that holds the session id
*/
char *get_session_id(char *file_path)
{
    static char session[SESSION];
    char file[BUF];
    FILE *fp;
    int len;

    // file path for file containing session id
    strcpy(file, file_path);
    strcat(file, "/.xmas/session");

    if(access(file, F_OK) == 0) {
        // session id file exists
        fp = fopen(file, "r");
        if (fp == NULL) {
            fprintf(stderr, "problem getting session id\n");
            exit(EXIT_FAILURE);
        }
        fgets(session, MAXBUF, fp);

    } else {
        // file doesn't exist so we create it
        printf("enter session id: ");
        fgets(session, MAXBUF, stdin);

        // create .santa file and add session id to it
        fp = fopen(file, "w");
        if (fp == NULL) {
            perror("fopen");
            fprintf(stderr, "problem creating session id file\n");
            printf("FILE PATH: %s\n", file);
            exit(EXIT_FAILURE);
        }

        fprintf(fp, "%s", session);
        printf("successfully created '.xmas' file in %s directory\n", file_path);
    }

    // trim \n if it exists at end of string
    len = strlen(session);
    if (session[len-1] == '\n')
        session[len-1] = '\0';

    fclose(fp);
    return session;
}


// get puzzle input from AOC servers based on user's session id
char *get_input(char *url, char *session_id)
{
	CURL *curl;
	CURLcode res;
	char cookie[SESSION] = "session=";
    struct string s;

    init_string(&s);

	// we need the session id to be predicated with 'session='
	strcat(cookie, session_id);

	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_COOKIE, cookie);

        printf("HERE 4\n");
        // save the input into a string
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

		res = curl_easy_perform(curl);
    
		if (res != CURLE_OK)
			fprintf(stderr, "request failed. error: %s\n", curl_easy_strerror(res));
		curl_easy_cleanup(curl);
	}
    return s.ptr;
}


// setup struct string *s to be used to store a string and a size
void init_string(struct string *s)
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
size_t write_func(void *ptr, size_t size, size_t nmemb, struct string *s)
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


/*
*   debug_html:
*
*   this fills a variable with an html response that is pre-stored in a file
*   to use for debugging and testing the parse_results function.
*   this is to prevent the constant calls to the AOC servers.
*   files to use for this funtion are in the tests directory.
*/
char *debug_html(char *file)
{
    #include <assert.h>
    FILE *fp;
    long length;
    char *buf;

    fp = fopen(file, "r");
    assert(fp);

    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buf = malloc(sizeof(char)*length);
    assert(buf);

    // read the entire file into buffer
    fread(buf, 1, length, fp);
    buf[length] = '\0';
    fclose(fp);

    return buf;
}


/*  
*   parse_results:
*
*   parse the html results that is returned from the request
*   a NULL check should be required for this function since that is what is returned
*   if it is unable to parse what it needs from the string
*   
*   on success, the result will have to be freed
*   on failure, nothing gets allocated
*/
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
            fprintf(stderr, "parse_results: malloc failed\n");
            exit(EXIT_FAILURE);
        }

        strncpy(buf, tmp, n);
        buf[n] = '\0';

        return buf;
    }
    return NULL;
}


char *submit_answer(char *url, char *session_id, char *header)
{
    CURL *curl;
    CURLcode res;
    struct string s;
    char cookie[MAXBUF] = "session=";

    init_string(&s);

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
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl resquest failed\nerror: %s\n", curl_easy_strerror(res));
            exit(EXIT_FAILURE);
        }
        curl_easy_cleanup(curl);
    }
    // return the string that holds the html response
    return s.ptr;
}
