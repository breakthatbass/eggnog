#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <ctype.h>

#include "eggnog.h"

#define SML_BUF 5

//static char *cookie = "53616c7465645f5f9fd72c151396ac153bfbe74d91a60304e33a95e41d3fa6e4b12552eac7445a0108728700a1b3bc20";
static char *u = "https://raw.githubusercontent.com/breakthatbass/advent_of_code2020/main/day01/input";


int main(int argc, char **argv)
{
	int opt;

	// year and day required
	int y = 0;
	char *year = NULL;
	int d = 0;
	char *day = NULL;

	int s = 0;	// submit
	char *submit_answer = NULL;

	// puzzle level (part 1 or part 2)
	// defaults to part 1, required for part 2
	int l = 0;
	char *level = NULL;
	
	int p = 0;	// puzzle directions
	int i = 0;	// pizzle input

	// variables to store returned data
	char *data = NULL;
	char *session_id = NULL;
	

	while ((opt = getopt(argc, argv, "y:d:l:s:pi")) != -1) {
		switch(opt) {
		
		case 'y':
			year = optarg;
			y = 1;
			// if arg is not a number, quit
			if (!isdigit(optarg[0])) print_usage();
			break;

		case 'd':
			day = optarg;
        	d = 1;
        	// if arg isnt a number, quit
        	if (!isdigit(optarg[0])) print_usage();
        	break;
		
		case 's':
			// submit and answer
			submit_answer = optarg;
			s = 1;
			break;

		case 'p':
			// puzzle directions
			p = 1;
			break;

		case 'i':
         // get input for puzzle
         i = 1;
         break;
		case 'l':
			// indicate part one or two
			level = optarg;
			l = 1;
			break;
        
		default:
            print_usage();
            return 1;
		}
	}

	// variables to hold the day and year if it's advent
	// they have to be global since there's memory issues with
	// passing in locally scoped variables in check_cache().
	// these prevent us from having to allocate memory on the heap
	// and therefore needing to free year and day only sometimes
	char year_stack[SML_BUF] = {0};
	char day_stack[SML_BUF] = {0};

	// make sure only one or zero flags have been used
	char *flag = handle_flags(p, i, s);

	if (flag == NULL) {
		fprintf(stderr, "error: only one or zero of: -i, -d, -s\n");
		exit(EXIT_FAILURE);
	}

	session_id = get_session_id();

	// check for -y and -d
	if (y == 0 && d == 0) {
		// check if advent of code is currently happening
		unsigned int *adv;
		adv = is_it_advent();

		if (adv) {
			// it's advent!
			sprintf(year_stack, "%d", adv[0]);
			sprintf(day_stack, "%d", adv[2]);

			year = &(*year_stack);
			day = &(*day_stack);

		} else {
			// we need the flags
			print_usage();
			exit(EXIT_FAILURE);
		}

	}

	// we do our thing
	data = check_cache(year, day, flag);
	
	if (data == NULL) {
		// not saved to cache yet
		char *url = build_url(year, day, flag);

		if (strcmp(flag, "i") == 0) {
#if DEBUG
			data = get_input(u, NULL);
#else
			data = get_input(url, session_id);
#endif
		
		} else if (strcmp(flag, "p") == 0) {

			printf("p - puzzle directions coming soon\n");
			return 0;

		} else {
			// build url for submitting
			char header[URL_BUF] = "level=";
			if (level) strcat(header, level);
			else strcat(header, "1");
			strcat(header, "&answer=");
			strcat(header, submit_answer);

			data = submit_puzzle_answer(url, session_id, header); 
		}

		add_to_cache(data, year, day, flag);
	} 
		
	printf("%s", data);
	free(data);
	
	return 0;
}


