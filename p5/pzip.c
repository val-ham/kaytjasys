#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

/* https://pages.cs.wisc.edu/~remzi/OSTEP/threads-cv.pdf PAGE 3*/
int done = 1;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;


typedef struct fileNode {
    char *name;
    int order;
} FileNode;

typedef struct rleNode {
    char c;
    int count;
    struct rleNode *next;
} RleNode;



void *mythread(void *arg) {
    FileNode *args = (FileNode *) arg; /* https://pages.cs.wisc.edu/~remzi/OSTEP/threads-api.pdf PAGE 3*/
    
    FILE *input;
    char ca;
    char prev;
    unsigned int count;
    RleNode *rle;
    RleNode *start = NULL;
    RleNode *end = NULL;

    input = fopen(args->name,"r");
    if (input == NULL) {
        printf("my-zip: cannot open file\n");
        exit(1);
    }
    count = 0;

    //Create a linked listed from the elements of the RLE encoding
    while ((ca = fgetc(input)) != EOF) {
        if (ca == prev || count == 0) {
            count++;
        } else {
            if (((rle) = malloc(sizeof(RleNode)))==NULL) {
                fprintf(stderr,"malloc failed");
                exit(1);
		    }
            rle->count = count;
            rle->c = prev;
            rle->next = NULL;
            if (start == NULL) {
                start = rle;
                end = rle;
            } else {
                end->next = rle;
                end = rle;
            }
            count = 1;
        }
        prev = ca;
    }
    if (((rle) = malloc(sizeof(RleNode)))==NULL) {
        fprintf(stderr,"malloc failed");
        exit(1);
    }
    rle->count = count;
    rle->c = prev;
    rle->next = NULL;
    if (start == NULL) {
        start = rle;
        end = rle;
    } else {
        end->next = rle;
        end = rle;
    }
    

    //Check that the files will be printed in the right order (the thread created first will be printed first)
    pthread_mutex_lock(&m);
    while (done < args->order) {
        pthread_cond_wait(&c, &m);
    }
    pthread_mutex_unlock(&m);

    //print to stdout
    rle = start;
    while (rle != NULL) {
        count = rle->count;
        fwrite(&count,4,1,stdout);
        printf("%c",rle->c);
        rle = rle->next;
        free(start);
        start = rle;
    }

    //Increment the global value so that the thread sleeping will continue execution
    pthread_mutex_lock(&m);
    done++;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);


    return NULL;
}

int main (int argc, char **argv) {
    pthread_t threads[argc-1]; /*https://stackoverflow.com/questions/35403892/creating-threads-in-a-loop*/
    void *retvals[argc-1]; /*https://stackoverflow.com/questions/35403892/creating-threads-in-a-loop*/
    FileNode *args;
    
    int i = 1;
    int j;
    while (i < argc) {
        j = i;
        //Create 2 threads at once
        for (int counter = 0;counter < 2; i++, counter++) {
            if (i>=argc) break;
            if (((args) = malloc(sizeof(FileNode)))==NULL) {
                fprintf(stderr,"malloc failed");
                exit(1);
		    }
            args->name = argv[i];
            args->order = i;
            pthread_create(&threads[i-1], NULL, mythread, args); /*https://stackoverflow.com/questions/35403892/creating-threads-in-a-loop*/
        }
        //Check that the threads have completed execution
        for (int counter = 0;counter < 2; j++, counter++) {
            if (j >= argc) break;
            pthread_join(threads[j-1], &retvals[j-1]); /*https://stackoverflow.com/questions/35403892/creating-threads-in-a-loop*/
        }
    }

    return 0;
}