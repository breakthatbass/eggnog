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
