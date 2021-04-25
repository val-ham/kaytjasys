/*

This program reverses the lines in the given input.

The program works with 0-2 command-line arguments like this.
0 arguments: read from stdin and output to stdout
1 argument: read from the given input file and output to stdout
2 arguments: read from given file and output to given file.

*/
#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
	char *line;
	struct Node* next;
	struct Node* prev;
};

/*
This function reads the file with given fileName and creates a two-way linked list where each line on 
that file creates a node on the linked list.
*/
void readFile(char *fileName,struct Node **pEnd, struct Node **pStart, struct Node **ptr) {
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	FILE *input;

	//open file
	input = fopen(fileName, "r");
	if (input == NULL) {
		fprintf(stderr,"error: cannot open file '%s'\n",fileName);
		exit(1);
	}
	//process the file line by line
	while((nread = getline(&line, &len, input)) != -1) {
		char *str;
		if (((*ptr) = malloc(sizeof(struct Node)))==NULL) {
			fprintf(stderr,"malloc failed");
			exit(1);
		}
		if ((str = malloc(sizeof(char)*nread))==NULL) {
			fprintf(stderr,"malloc failed");
			exit(1);
		}
		for (int i = 0; i < nread; i++) {
			str[i]=line[i];
		}
		(*ptr)->line = str;
		(*ptr)->next = NULL;
		(*ptr)->prev = NULL;
		if (*pStart == NULL) {
			*pStart = *ptr;
			*pEnd = *ptr;
		} else {
			(*ptr)->prev = *pEnd;
			(*pEnd)->next = *ptr;
			*pEnd = *ptr;
		}
	}
	fclose(input);
}


int main(int argc, char **argv) {
	struct Node *pEnd = NULL, *pStart = NULL, *ptr = NULL;
	if(argc == 1) {
		printf("no args\n");
	} else if(argc == 2) {
		readFile(argv[1],&pEnd,&pStart,&ptr);
		//Print to stdout
		ptr = pEnd;
		while (ptr !=NULL) {
			fprintf(stdout,"%s",ptr->line);
			ptr = ptr->prev;
			if (ptr != NULL) free(ptr->next);
		}
		
	} else if(argc == 3) {
		readFile(argv[1],&pEnd,&pStart,&ptr);
		//Print to output file
		if(!strcmp(argv[1],argv[2])) {
			fprintf(stderr,"error: Input and output file must differ\n");
			exit(1);
		}
		FILE *output;
		output = fopen(argv[2], "w");
		if (output == NULL) {
			fprintf(stderr,"error: incorrect output filename '%s'\n",argv[2]);
			exit(1);
		}
		ptr = pEnd;
		while (ptr !=NULL) {
			fprintf(output,"%s",ptr->line);
			ptr = ptr->prev;
			if (ptr != NULL) free(ptr->next);
		}
	} else {
		fprintf(stderr,"usage: reverse <input> <output> \n");
		exit(1);
	}
	return 0;
}
