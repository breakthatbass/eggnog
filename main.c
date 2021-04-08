#include <getopt.h>

#include "aoc.h"

int main(int argc, char **argv)
{
    int opt;
	char url[BUF];
	//char session_id[SESSION];

    // year and day is required, these vars keep track
    int d = 0;
    int y = 0;

    char *year = NULL;
    char *day = NULL;

     // options on left side of ':' take args, on right, no args
    while ((opt = getopt(argc, argv, "y:d:sip")) != -1) {
        switch(opt) {
        // need args
        case 'y':
            year = optarg;
            y = 1;
            // if arg isnt a number, quit
            if (!isdigit(optarg[0])) print_usage();
            //printf("YEAR: %s\n", year);
            break;
        case 'd':
            day = optarg;
            d = 1;
            // if arg isnt a number, quit
            if (!isdigit(optarg[0])) print_usage();
            //printf("DAY: %s\n", optarg);
            break;
        // no args
        case 's':
            // submit an answer
            printf("the -s flag has been used\n");
            break;
        case 'i':
            // get input for puzzle
            printf("the -i flag has been used\n");
            break;
        case 'p':
            // get puzzle directions
            printf("the -p flag has been used\n");
        default:
            print_usage();
            return 1;
        }
    }
    // make sure a year and day has been given
    if (!d || !y) print_usage();


    strcpy(url, concat_url(year, day, 'i'));
    printf("URL: %s\n", url);
    return 0;
}
/*
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
*/
