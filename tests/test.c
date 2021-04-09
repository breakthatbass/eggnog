#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 1

char *parse_html(char *s)
{
  char *buf, *p;
  
  // we want all the chars up to the double space
  p = strstr(s, "  ");
  int n = p - s;  // some pointer arithmetic to get the distance to the "  "

  buf = malloc(sizeof(char)*n);

  int i = 0;
  while (i <= n) {
    *buf++ = *s++;
    i++;
  }
  *buf = '\0';
  buf-=i;
  s-=i;
  return buf;
}

char *parse_results()
{
  char *buffer = 0;
    if (DEBUG) {
        #include <assert.h>
        // we will debug with html saved in a file rather than a bunch of POSTS to the AOC servers
        FILE *fp;
        
        fp = fopen("wronganswer.txt", "r");
        assert(fp);

        fseek (fp, 0, SEEK_END);
        long length = ftell (fp);
        fseek (fp, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer)
            fread (buffer, 1, length, fp);
        fclose (fp);

        //printf("%s", buffer);

    }
    
    if (strstr(buffer, "<main>\n<article><p>")) {
        
        char *s = strstr(buffer, "<main>\n<article><p>");
        s+=19;
        char *t;
        
        t = parse_html(s);
        
        printf("%s\n", t);
    }
    return "yes";
}

int main()
{
  char *s = parse_results();

  return 0;
}