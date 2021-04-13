#include "aoc.h"

#include <sys/types.h>
#include <sys/stat.h>


// check if a directory exists. if not, create it
void check_dir(char *path)
{
	struct stat s;

	if (stat(path, &s) == -1) {
		mkdir(path, 0700);
	}
}


// if input for a given day exists, read it into a string and return it, else return NULL
char *check_cache(char *path, char *day)
{
	char file_path[BUF];
	FILE *fp;
	long size;
	char *buf;


	// check if cache for the year exists
	check_dir(path);

	strcpy(file_path, path);
	strcat(file_path, "/");
	strcat(file_path, day);

	if (access(file_path, F_OK) == 0) {
		// file exists
		fp = fopen(file_path, "r");
		if (fp == NULL) {
			fprintf(stderr, "check_cache: problem opening file: %s\n", file_path);
			exit(EXIT_FAILURE);
		}

		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		buf = malloc(sizeof(char)*size+1);
		if (buf == NULL) {
			perror("check_cache: malloc:");
			exit(EXIT_FAILURE);
		}

		// read entire file into buffer
		fread(buf, 1, size, fp);
		buf[size] = '\0';
		fclose(fp);

		return buf;
	}
	// file doesn't exist
	return NULL;
}

// check if year dir exists, if not create it
// open a file called 'day'
// print the input string into it
// close file
void add_to_cache(char *path, char *year, char *day, char *input)
{
	FILE *fp;
	
	check_dir(path);
	strcat(path, day);

	fp = fopen(path, "w");
	if (fp == NULL) {
		fprintf(stderr, "add_to_cache: problem opening file: %s\n", path);
		exit(EXIT_FAILURE);
	}
	fprintf(fp, "%s", input);
	fclose(fp);
}
/*
int main()
{
	char p[BUF];
	strcpy(p, getenv("HOME"));
	strcat(p, "/.xmas");

	add_to_cache(p, "2020", "2", "make me poo");
	return 0;
}
*/