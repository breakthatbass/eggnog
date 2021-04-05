#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <curl/curl.h>

#define BUF 45
#define MAXBUF 1024

void print_usage(void)
{
	fprintf(stderr, "santa <year> <day>\n");
	exit(EXIT_FAILURE);
}


// get session id from ~/.santa file or create it if doesn't exist
char *get_session_id(void)
{
    static char session[MAXBUF];
    char file[BUF];
    FILE *fp;
    int len;

    // file path for file containing session id
    strcat(strcpy(file, getenv("HOME")), "/.santa");

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
        printf("successfully created '.santa' file in %s directory\n", getenv("HOME"));
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


int main(int argc, char **argv)
{
	char url[45];
	char session_id[500];
	if (argc != 3) print_usage();

	if (!isdigit(argv[1][0]) || !isdigit(argv[2][0]))
		print_usage();	

	// put together the url
	strcpy(url, "https://adventofcode.com/");
	strcat(url, argv[1]);
	strcat(url, "/day/");
	strcat(url, argv[2]);
	strcat(url, "/input");

	strcpy(session_id, get_session_id());

	get_input(url, session_id);

    return 0;
}

