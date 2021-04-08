#include <getopt.h>

#include "aoc.h"

int main(int argc, char **argv)
{
    int opt;
	char *url;
	char session_id[SESSION];
    char *html_response;

    // year and day is required, these vars keep track
    int d = 0;
    int y = 0;
    int submit = 0;
    int input = 0;
    //int puzzle = 0;

    char *year = NULL;
    char *day = NULL;
    char *answer = NULL;

     // options on left side of ':' take args, on right, no args
    while ((opt = getopt(argc, argv, "y:d:s:ip")) != -1) {
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
            answer = optarg;
            submit = 1;
            break;
        case 'i':
            // get input for puzzle
            printf("the -i flag has been used\n");
            input = 1;
            break;
        case 'p':
            // get puzzle directions
            printf("the -p flag has been used\n");
            //puzzle = 1;
        default:
            print_usage();
            return 1;
        }
    }
    // make sure a year and day has been given
    if (!d || !y) print_usage();

    // cant get input and submit
    if (submit && input) {
        print_usage();
    } else if (submit && !input) {
        char header[45] = "level=1&answer=";
        strcat(header, answer);
        url = concat_url(year, day, 's');
        strcpy(session_id, get_session_id(getenv("HOME")));
        html_response = submit_answer(url, session_id, header);
        
        //printf("%s\n", html_response);
        
    } else if ((!submit && input) || (!submit && !input)) {
        url = concat_url(year, day, 'i');
        strcpy(session_id, get_session_id(getenv("HOME")));
        get_input(url, session_id);
    }
   
    free(url);
    return 0;
}
