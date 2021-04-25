#include <stdlib.h>
#include <stdio.h>


int main(int argc, char **argv) {
    if (argc < 2) {
        printf("my-unzip: file1 [file2 ...]\n");
        exit(1);
    }
    FILE *input;
    char c;
    unsigned int count;
    for (int i = 1; i < argc; i++) {
        input = fopen(argv[i],"r");
        if (input == NULL) {
            printf("my-unzip: cannot open file\n");
            exit(1);
        }
        while ((fread(&count,4,1,input)) != 0) {
            c = fgetc(input);
            for (int i = 0; i < count; i++) {
                printf("%c",c);
            }
        }
    }
    return 0;
}