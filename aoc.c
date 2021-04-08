#include "aoc.h"


void print_usage(void)
{
	fprintf(stderr, "santa <year> <day>\n");
	exit(EXIT_FAILURE);
}


// build url for http request. task is submit answer or get puzzle input
char *concat_url(char *year, char *day, char task)
{
    char *url = malloc(sizeof(char)*55);

    strcpy(url, "https://adventofcode.com/");
	strcat(url, year);
	strcat(url, "/day/");
	strcat(url, day);

    // i = get puzzle input
    // s = submit an answer
    if (task == 'i') strcat(url, "/input");
    else if (task == 's') strcat(url, "/answer");
    else return NULL;

    return url;
}


// get session id from ~/.santa file or create it if doesn't exist
char *get_session_id(char *file_path)
{
    static char session[SESSION];
    char file[BUF];
    FILE *fp;
    int len;

    // file path for file containing session id
    strcpy(file, file_path);
    strcat(file, "/.santa");

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
            fprintf(stderr, "problem creating session id file\n");
            exit(EXIT_FAILURE);
        }

        fprintf(fp, "%s", session);
        printf("successfully created '.santa' file in %s directory\n", file_path);
    }

    // trim \n if it exists at end of string
    len = strlen(session);
    if (session[len-1] == '\n')
        session[len-1] = '\0';

    fclose(fp);
    return session;
}


// attempt to contact servers and return puzzle input
void get_input(char *url, char *session_id)
{
	CURL *curl;
	CURLcode res;
	char cookie[MAXBUF] = "session=";

	// we need the session id to be predicated with 'session='
	strcat(cookie, session_id);

	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_COOKIE, cookie);
		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
			fprintf(stderr, "request failed. error: %s\n", curl_easy_strerror(res));
		curl_easy_cleanup(curl);
	}
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


// results are a bunch of html
// we just want it to say whether the answer was right or not
char *parse_results(char *s, int debug)
{
    char *buf, *tmp, *t;

    if (debug) {
        #include <assert.h>
        FILE *fp;
        long length;

        fp = fopen("wronganswer.txt", "r");
        assert(fp);

        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        buf = malloc(sizeof(char)*length);
        assert(buf);

        // read the entire file into buffer
        if (buf) fread(buf, 1, length, fp);
        fclose(fp);
    }

    if (strstr(buf, "<main>\n<article><p>")) {
        // get pointer to this part of html
        tmp = strstr(buf, "<main>\n<article><p>");
        // move ahread past the <p>
        tmp+=19;
        // now we parse what we want from the remaining html
        t = get_answer(s);
        free(buf);
        free(s);
        return t;
    }
    return NULL;
}


char *get_answer(char *s)
{
    char *buf, *p;
    int n, i;

    // we want everything up until the double space
    p = strstr(s, "  ");
    n = p-s; // the distance until the "  "

    buf = malloc(sizeof(char)*n);
    if (buf == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }
    i = 0;
    while (i <= n) {
        *buf++ = *s++;
        i++;
    }
    *buf = '\0';
    buf-=i;
    s-=i;
    return buf;
}


char *submit_answer(char *url, char *session_id, char *body)
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
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
        curl_easy_setopt(curl, CURLOPT_COOKIE, cookie);

        // save the html results into a string so we can parse it
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "curl resquest failed\nerror: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    return s.ptr;
}
