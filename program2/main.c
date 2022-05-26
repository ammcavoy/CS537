/////////////////////////////////////////////////////////
// 
// Main method that drives program
// 
// Author:  Adam McAvoy
// NetId:   ammcavoy
// CSLogin: mcavoy
//
////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "queue.h"
#include "reader.h"
#include "munch.h"
#include "writer.h"
#include "stats.h"


const int QUEUE_SIZE = 10;

int main(){
	// create the three queues to be used by threads
	Queue *q1 = CreateStringQueue(QUEUE_SIZE);
	Queue *q2 = CreateStringQueue(QUEUE_SIZE);
	Queue *q3 = CreateStringQueue(QUEUE_SIZE);

	// create array to hold queues to be passed to threads
	void *queues[3];
	queues[0] = q1;
	queues[1] = q2;
	queues[2] = q3;

	pthread_t reader_t;
	pthread_t munch1_t;
	pthread_t munch2_t;
	pthread_t writer_t;
	
	//create the threads
	if(pthread_create(&reader_t, NULL, reader, q1) != 0 ||
		pthread_create(&munch1_t, NULL, munch1, queues) != 0 ||
		pthread_create(&munch2_t, NULL, munch2, queues) != 0 ||
		pthread_create(&writer_t, NULL, writer, q3) != 0) {
		// if errors happen when creating threads print error
		// message and return
		fprintf(stderr, "Error creating the threads.\n");
		return -1;
	}	
	
	//join the threads
	if(pthread_join(reader_t, NULL) != 0 ||
			pthread_join(munch1_t, NULL) != 0 ||
			pthread_join(munch2_t, NULL) != 0 ||
			pthread_join(writer_t, NULL) != 0) {
		// if errors happen when joining threads print
		// error message and return
		fprintf(stderr, "Error joining the threads.\n");
		return -1;
	}
	
	//print of the stats of the queues
	printStats(queues);
}
