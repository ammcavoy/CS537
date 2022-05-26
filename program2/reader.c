/////////////////////////////////////////////////////////
//
// Readers from stdin and puts it all in the q1 for munch1
// to access
//
// Author:  Adam McAvoy
// NetId:   ammcavoy
// CSLogin: mcavoy
//
////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "queue.h"

const int BUFFER = 1024;


void *reader(void *q1){
  	Queue *q = (Queue *) q1;
  	char c  = '\0';
	char *input = NULL;
	bool newLine = true;
	int index = 0;
	 
	// loop until the EOF flag is met
  	while((c = getchar()) != EOF) {
		if(newLine) {
			//malloc input string 
			input = (char *)malloc(sizeof(char) * BUFFER);	
			if(input == NULL) {
				//if errors print error message and free string and cont.
				fprintf(stderr, "Unable ot malloc space for input.\n");
				free(input);
	            	}
			newLine = false;
		}

		//if at the end of line add string to queue
		if(c == '\n'){
			newLine = true;
			index = 0;
			if(input != NULL){
				//if the input string is valid 
				//enqueue into q1
				EnqueueString(q, input);
				input = NULL;
			}
		}
		// still putting string into input string
		else {
			if(input != NULL){
				if(index < BUFFER) {
					//adds the next char to input if within BUFFER size
					input[index] = c;
					index ++;
				}
				else {
					//if the string is too long print error message and free input string 
					//so it wont be added to queue
					fprintf(stderr, "Error, Input string is too long.\n");
					free(input);
					input = NULL;
				}
			}
		}
	}
	//if EOF met before \n and input is still valid enqueue into q1
	if(input != NULL){
		EnqueueString(q, input);
	}

	// at the terminate flag to queue and exit thread
	EnqueueString(q, NULL);
	pthread_exit(NULL);
}
