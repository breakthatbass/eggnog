/**
 * cache.c
 *
 * save everything in a cache
 * */
#include <sys/stat.h> // mkdir, stat
#include <unistd.h> // access
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "eggnog.h"


#define CACHE_PATH "/.eggnog"

static char *get_cache_path(void)
{
#if DEBUG
	return "tests/eggnog/";
#else
	static char cache[URL_BUF];
	char *home_dir = getenv("HOME");
	strcpy(cache, home_dir);
	strcat(cache, CACHE_PATH);
	strcat(cache, "/");
	return cache;
#endif
}

void create_cache(void)
{
	char *cache = get_cache_path();
	struct stat st = {0};


	// if cache doesn't already exist, create it
	if (stat(cache, &st) == -1) {
		mkdir(cache, S_IRWXU | S_IRWXG | S_IRWXO);
	}
}


/**
 * add_to_cache
 *
 * file name structure: <type><year><day>.txt
 * ex: puzzle input for year 2020 day 1 --> i20201.txt
 *
 * @desc: create a file in the cache to save data for future queries.
 *
 * @param: `data` - a string containing the data to save.
 * @param: `year` - the year of the puzzle info we have.
 * @param: `day` - the day of the puzzle info we have.
 * @param: `type` - type of data ([i]nput, d[irections], s[ubmit])
 *
 * @return: `0` if success, > 0 if problem.
 * */
int add_to_cache(char *data, char *year, char *day, char *type)
{
	FILE *fp;
	char cache_file[URL_BUF];

	// build file name and path
	strcpy(cache_file, get_cache_path());
	strcat(cache_file, type);
	strcat(cache_file, year);
	strcat(cache_file, day);
	strcat(cache_file, ".txt");

	if (strcmp(type, "w") == 0) { 
		fp = fopen(cache_file, "a"); // add to wrong answer file
		size_t len = strlen(data);
		*(data+len) = '\n';
		*(data+(len+1)) = 0;
	}
	else
		fp = fopen(cache_file, "w");
	if (fp == NULL) return 2;

	fprintf(fp, "%s", data);
		
	fclose(fp);
	return 0;
}

/**
 * check_cache
 *
 * @desc: check cache for any data before querying AoC servers
 *
 * @param: `year`: the year of the puzzle info we want.
 * @param: `day`: the day of the puzzle info we want.
 *
 * @return: Pointer to puzzle data if it is in cache, else NULL. Pointer must be freed by user.
 **/
char *check_cache(char *year, char *day, char *type)
{
	char path[URL_BUF];
	FILE *fp;
	long filesize;
	char *data;
	
	strcpy(path, get_cache_path());
	strcat(path, type);
	strcat(path, year);
	strcat(path, day);
	strcat(path, ".txt");

	if (access(path, F_OK) == 0) {
		// file exist. read it into a str:ing and return it
		fp = fopen(path, "r");
		if (fp == NULL) {
			perror("fopen");
			return NULL;
		}

		// get file size for allocation
		fseek(fp, 0, SEEK_END);
		filesize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		
		data = calloc(filesize+10, sizeof(char));
		if (data == NULL) {
			perror("calloc");
			return NULL;
		}

		// read file into data buffer
		fread(data, 1, filesize, fp);
		fclose(fp);
		
		return data;
	}
	return NULL;
}

char *get_session_id(void)
{
	char path[URL_BUF];
	static char session_id[SESSION];
	FILE *fp;

#if DEBUG
	strcpy(path, "/Users/taylorgamache/.eggnog/sessionid.txt");
#else
	strcpy(path, get_cache_path());
	strcat(path, "sessionid.txt");
#endif

	if (access(path, F_OK) == 0) {
		// session id file exists. read into string
		fp = fopen(path, "r");
		if (fp == NULL) {
			perror("fopen -> read");
			return NULL;
		}

		fgets(session_id, SESSION, fp);
	} else {
		// no session id which means no cache has been set up
		create_cache();
		printf("enter session id: ");
		fgets(session_id, SESSION, stdin);
		// remove '\n\r';
		session_id[strcspn(session_id, "\r\n")] = 0;

		// now add it to the cache
		fp = fopen(path, "w");
		if (fp == NULL) {
			perror("fopen -> write");
			return NULL;
		}

		fprintf(fp, "%s", session_id);
		printf("successfully created cache with session id 🎄\n");
	}
	return session_id;
}

/**
 * check_cache_answers
 *
 * @desc: Check cache for answer file based on year & day.
 *
 * @param: `year` - The year of the puzzle info we want.
 * @param: `day` - The day of the puzzle info we want.
 * @param: `answer_type` - Determine whether we want to query a right("r") or wrong("w") answer file.
 *
 * @return: If answer is found in cache, a potiner to a string, else `NULL`.
 * */
char *check_cache_answers(char *year, char *day, char *type)
{
	// answer files for 2020 day 1 would look like:
	// --> right: r20201.txt
	// --> wrong: w20201.txt
	char *cdata = NULL;

	cdata = check_cache(year, day, type);
	return cdata; // if no file, NULL.
}

/**
 * check_wrongs
 *
 * @desc: check the cache to see if a `wrong file` exists for a given puzzle.
 *
 * @param: `year` - The year of the puzzle info we want.
 * @param: `day` - The day of the puzzle info we want.
 * @answer: `answer` - Answer we're trying to submit.
 *
 * @return: If answer is in `wrong file`, return `answer`, else `NULL`.
 * */
char *check_wrongs(char *year, char *day, char *answer)
{
	static char cpath[URL_BUF] = {0};
	char buf[URL_BUF];
	FILE *fp;

	if (answer == NULL) return NULL;
	
	// get path to file
	strcpy(cpath, get_cache_path());
	strcat(cpath, "w");
	strcat(cpath, year);
	strcat(cpath, day);
	strcat(cpath, ".txt");

	// check if file exists yet
	if (access(cpath, F_OK) == 0) {
		// file exist. read it into a str:ing and return it
		fp = fopen(cpath, "r");
		if (fp == NULL) {
			perror("fopen");
			return NULL;
		}
		
		// go through file line by line
		while (fgets(buf, URL_BUF, fp)) {
			if (strcmp(answer, buf) == 0) {
				// wrong answer has already been attempted
				fclose(fp);
				return answer;
			}
		}
		// if we're here, the answer hasn't been attempted yet
		fclose(fp);
		return NULL;
	}
	// if we're here, the answer hasn't been attempted yet
	// nor does the wrong file for that year/day exist yet
	return NULL;
}

