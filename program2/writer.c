/////////////////////////////////////////////////////////
//
// writes the contents of q3 out for the user to see
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
#include "queue.h"

//writers the contents of q3 out 
void *writer(void *q_3){
	Queue *q3 = (Queue *) q_3;
	//dequeues first sting from q3
	char *string = DequeueString(q3);
	//loop until terminate flag met in q3
	while(string != NULL){
		//print and free the string once done being used
		printf("%s\n", string);
		free(string);
		//get the next string in the queue
		string = DequeueString(q3);
	}
	//exit thread
	pthread_exit(NULL);
}
