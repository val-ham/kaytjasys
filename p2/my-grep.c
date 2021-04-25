/*
my-grep:
If only 1 argument: check if the input from stdin has the argument in it. If so then print it to stdout.
If more than 1 argument: Go trough all of the files one after another. Check each line and print it out if it has the search term in it.
If file doesn't exist print error message and stop execution.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE *input;
    char *line = NULL;
	size_t len = 0;
	ssize_t nread;
    if (argc < 2) {
        printf("my-grep: searchterm [file ...]\n");
        exit(1);
    } else if (argc == 2) {
        getline(&line,&len,stdin);
        if (strstr(line,argv[1]) != NULL) printf("%s",line);
    } else {
        for (int i = 2; i < argc; i++) {
            input = fopen(argv[i],"r");
            if (input == NULL) {
                printf("my-grep: cannot open file\n");
                exit(1);
            }
            while((nread = getline(&line, &len, input)) != -1) {
                if (strstr(line,argv[1]) != NULL) printf("%s",line);
            }
        }
    }
    return 0;
}