#ifndef __CACHE_H__
#define __CACHE_H__

// check if path/dir exists, if not, create it
void check_dir(char *path);

/*
* check_cache:
* check the local cache for puzzle input corresonding to the year
* (in the path) with day. If the file exists, read it into a buffer
* and return that buffer, other wise return NULL
*
* If not NULL, variable must be freed
*/
char *check_cache(char *path, char *day);

#endif