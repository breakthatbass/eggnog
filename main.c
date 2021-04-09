#include <getopt.h>

#include "aoc.h"
#define DEBUG 1

int main(int argc, char **argv)
{
    int opt;
	char *url = NULL;
	char session_id[SESSION];
    char *html_response;

    //char *a = debug_html("tests/rightanswer.txt");
    //char *b = debug_html("tests/wronganswer.txt");

    //printf("%s\n", b);

    //free(b);

    //exit(0);

    // year and day is required, these vars keep track
    int d = 0;
    int y = 0;
    int submit = 0;
    int input = 0;
    //int part = 0;
    //char level = 0;
    //int puzzle = 0;

    char *year = NULL;
    char *day = NULL;
    char *answer = NULL;
    char level[2] = "1"; //part 1 by default

     // options on left side of ':' take args, on right, no args
    while ((opt = getopt(argc, argv, "y:d:s:l:ip")) != -1) {
        switch(opt) {
        // need args
        case 'y':   // required
            year = optarg;
            y = 1;
            // if arg isnt a number, quit
            if (!isdigit(optarg[0])) print_usage();
            //printf("YEAR: %s\n", year);
            break;
        case 'd':   // required
            day = optarg;
            d = 1;
            // if arg isnt a number, quit
            if (!isdigit(optarg[0])) print_usage();
            //printf("DAY: %s\n", optarg);
            break;
        case 's':
            // submit an answer
            answer = optarg;
            submit = 1;
            break;
        case 'l':
            // l for level, part 1 or 2 for submitting
            if (strcmp(optarg, "2") == 0) {
                level[0] = 0;
                level[1] = 0;
                strcpy(level, optarg);
            } else if (strcmp(optarg, "1")) {
                fprintf(stderr, "-l flag only accepts 1 or 2 as arguments\n");
                exit(EXIT_FAILURE);
            }
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

    if (submit && !input) {
        // heaser to send the part and answer along with the POST
        char header[BUF] = "level=";
        strcat(header, level);
        strcat(header, "&answer=");
        strcat(header, answer);

        url = concat_url(year, day, 's');
        strcpy(session_id, get_session_id(getenv("HOME")));

        // send the answer and get back the HTML
        html_response = submit_answer(url, session_id, header);

        // parse the HTML just to say whether we passed or failed
        char *results = parse_results(html_response);
        free(html_response);
        if (results) {
            printf("%s\n", results);
            free(results);
        } else {
            // there will probbaly always be a response of some sort but just in case...
            fprintf(stderr, "problem fetching response\n");
            exit(EXIT_FAILURE);
        }

    } else if ((!submit && input) || (!submit && !input)) {
        url = concat_url(year, day, 'i');
        strcpy(session_id, get_session_id(getenv("HOME")));
        get_input(url, session_id);
    } else {
        //something went wrong somewhere
        print_usage();
    }
   
    free(url);
    return 0;
}
