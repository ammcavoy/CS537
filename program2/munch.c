/////////////////////////////////////////////////////////
//
// contains the functions that will be used by the threads
// to execute the munch1 and munch2 steps of the program
//
// Author:  Adam McAvoy
// NetId:   ammcavoy
// CSLogin: mcavoy
//
////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include "queue.h"

// dequeues from q1 and puts '*' in all the spaces and enqueues 
// into q2
void *munch1(void *queues) {
	Queue **queue = (Queue **) queues;
	Queue *q1 = queue[0];
	Queue *q2 = queue[1];
	
	// get the first string in q1
	char *string = DequeueString(q1);
	//loop until the end flag (NULL) is reached
	while(string != NULL){
		// replace all spaces with '*'
		for(int i=0; string[i] != '\0'; i++){
			if(string[i] == ' '){
				string[i] = '*';
			}
		}
		//enqueue the new string into q2
		EnqueueString(q2, string);
		//get the next string in the queue
		string = DequeueString(q1);
	}
	
	//add the terminate flag and exit thread
	EnqueueString(q2, NULL);
	pthread_exit(NULL);
}

// dequeues from q2 and changes all lowercase letters
// to uppercase and the enqueues into q3
void *munch2(void *queues) {
	Queue **queue = (Queue **) queues;
	Queue *q2 = queue[1];
   	Queue *q3 = queue[2];
	
	// gets the first string from q2
    	char *string = DequeueString(q2);
	//loop until terminate flag (NULL) is reached
    	while(string != NULL){
		//change all lowercase to uppercase
        	for(int i=0; string[i] != '\0'; i++){
           	 	if(islower(string[i])){
                		string[i] = toupper(string[i]);
            		}
        	}
		
		//enqueue the new string into q3
		//and get next sting in q2
        	EnqueueString(q3, string);
        	string = DequeueString(q2);
    	}

	//add the terminate flag to the queue and exit thread
   	EnqueueString(q3, NULL);
    	pthread_exit(NULL);
	
}
