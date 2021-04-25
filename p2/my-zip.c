
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char **argv) {
    if (argc < 2) {
        printf("my-zip: file1 [file2 ...]\n");
        exit(1);
    }
    FILE *input;
    char c;
    char prev;
    unsigned int count;
    for (int i = 1; i < argc; i++) {
        input = fopen(argv[i],"r");
        if (input == NULL) {
            printf("my-zip: cannot open file\n");
            exit(1);
        }
        count = 0;
        //Process the file one character at a time. Count the consequent characters and print them to stdout.
        while ((c = fgetc(input)) != EOF) {
            if (c == prev || count == 0) {
                count++;
            } else {
                fwrite(&count,4,1,stdout); //write one four byte element (unsigned int) that is at the count variable, to stdout.
                printf("%c",prev);
                count = 1;
            }
            prev = c;
        }
        //After loop print one more time
        fwrite(&count,4,1,stdout);
        printf("%c",prev);
    }
    return 0;
}