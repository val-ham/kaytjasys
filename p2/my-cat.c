/*

my-cat:
Prints out the contents of files in the order they were specified as command line arguments.
Files are printed after each other with no newline characters to distinquish them from eachother.
If there is failure to open a file, the program prints out an error message and stops execution at that point.(correct files before the failure will be printed normally)

*/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc < 2) exit(0); // Exit when no files specified.

    FILE *input;
    char *line = NULL;
	size_t len = 0;
	ssize_t nread;
    for (int i = 1; i < argc; i++) {
        input = fopen(argv[i],"r");
        if (input == NULL) {
            printf("my-cat: cannot open file\n");
            exit(1);
        }
        while((nread = getline(&line, &len, input)) != -1) {
            printf("%s",line);
        }
    }
    printf("\n");
    return 0;
}