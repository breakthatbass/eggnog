#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <ctype.h>

#include "eggnog.h"

#define SML_BUF 5


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

	int v = 0;
	int h = 0;
	

	while (1) {

		int option_index = 0;
		// make use of full word arguments
		static struct option long_options[] = {
			{"year", required_argument, NULL, 'y'},
			{"day", required_argument, NULL, 'd'},
			{"part", required_argument, NULL, 'l'},
			{"submit", optional_argument, NULL, 's'},
			{"input", no_argument, NULL, 'i'},
			{"directions", no_argument, NULL, 'p'},
			{"help", no_argument, NULL, 'h'},
			{"version", no_argument, NULL, 'v'},
			{NULL, 0, NULL, 0}
		};

		opt = getopt_long(argc, argv, ":y:d:l:s::iphv", long_options, &option_index);
		if (opt == -1) break;

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
			if (!optarg) {
				printf("opt: %s\n", optarg);
				submit_answer = optarg;
			} 
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

		case 'h':
			print_usage();
			h = 1;
			break;

		case 'v':
			print_version();
			v = 1;
			break;
        
		default:
            print_error(opt);
            return 1;
		}
	}

	if (h || v) exit(EXIT_SUCCESS);
	
	// these variables are for preventing having to free the year
	// day variables only somtimes
	char year_stack[SML_BUF] = {0};
	char day_stack[SML_BUF] = {0};

	// make sure only one or zero flags have been used
	char *flag = handle_flags(p, i, s);

	if (flag == NULL) {
		fprintf(stderr, "error: only one or zero of: -i, -pd, -s\n");
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

			// point to those stack variables so we dont have to allocate
			// dynamically allocate memory for them
			year = &(*year_stack);
			day = &(*day_stack);

		} else {
			// we need the flags
			print_usage();
			exit(EXIT_FAILURE);
		}

	} else {
	// this branch prevent needless server hits that just return 404
	if ((y && !d) || (!y && d)) {
			print_usage();
			exit(EXIT_FAILURE);
		}
		
		int r = check_input(year, day);
		if (r == 1) {
			fprintf(stderr, "error: not a valid day\n");
			exit(EXIT_FAILURE);
		} else if (r == 2) {
			fprintf(stderr, "error: not a valid year\n");
			exit(EXIT_FAILURE);
		}
	}


	data = check_cache(year, day, flag);
	
	if (data == NULL) {
		// not saved to cache yet
		char *url = build_url(year, day, flag);

		if (strcmp(flag, "i") == 0) {
			// puzzle input
			data = get_input(url, session_id);
		
		} else if (strcmp(flag, "p") == 0) {
			// puzzle directions
			data = get_input(url, session_id);

		} else {

			char *header = prep_submit(submit_answer, level);
			data = submit_puzzle_answer(url, session_id, header); 
		}

		add_to_cache(data, year, day, flag);
	} else printf("in cache\n");

	if (p) pretty_print(data);	
	else printf("%s", data);
	free(data);
	
	return 0;
}


