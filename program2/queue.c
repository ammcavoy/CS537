/////////////////////////////////////////////////////////
//
// conatins the implementation of a queue with exlusion
// on the enqueue and dequeue function
//
// Author:  Adam McAvoy
// NetId:   ammcavoy
// CSLogin: mcavoy
//
////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/time.h>
#include "queue.h"


// initializes the Queue struct with a specific size
//
// Param
// 	size - the size of the new queue
// Returns
// 	pointer to the new Queue struct
Queue *CreateStringQueue(int size){
	//malloc new Queue struct
	Queue *q = (Queue *)malloc(sizeof(Queue));
	if(q == NULL){
		// if not created right print error message and 
		// return 
		fprintf(stderr, "Unable to malloc Queue");
		free(q);
		return NULL;
	}

	//strings is the struct that will store what is enqueued and 
	//dequeue so it is where size param is used
	q->strings = (char**)malloc(sizeof(char*) * size);
	if(q->strings == NULL){
		//if errors print error message and return
		fprintf(stderr, "Unable to malloc queue");
		free(q);
		return NULL;
	}
	
	//init values for the queue stats
	q->maxSize = size;
	q->currSize = 0;
	q->enqueueCount = 0;
	q->dequeueCount = 0;
	q->enqueueTime = 0.0;
	q->dequeueTime = 0.0;
	q->head = 0;
	q->tail = 0;
	
	//init the semaphores for the queue
	if(sem_init(&q->waitIfEmpty, 0, 0) ==  -1 || sem_init(&q->waitIfFull, 0, size) == -1 ||
	   	   				    sem_init(&q->queueReady, 0, 1) == -1){
		//if error when creating sem print message and return
		fprintf(stderr, "Unable to create semaphores for Queue");
		free(q);
		return NULL;
	}		
	
	return q;	

}

//Enqueues a string into the queue
//first is waits if the queue is full
//and if it isn't it then waits for the
//queue to not be in use by another thread
//
//Param
//	q - pointer to queue to be enqueued into
//	string - the string being enqueued
//Returns
//	None
void EnqueueString(Queue *q, char *string){
	//semaphores that allow for exclusion
	sem_wait(&q->waitIfFull);
	sem_wait(&q->queueReady);

	//the vars for runtime
	struct timeval start, stop;
	double secs = 0;
	//gets the starts time
	gettimeofday(&start, NULL);

	//insert the string into the queue
	q->strings[q->tail] = string;
	//inc the tail index, with loop around
	q->tail = (q->tail + 1) % q->maxSize;
	//inc the stats
	q->currSize++;
	q->enqueueCount++;
	

	gettimeofday(&stop, NULL);
        secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
	q->enqueueTime += secs;

	//sem opening up queue to other threads
	sem_post(&q->queueReady);
	sem_post(&q->waitIfEmpty);
}


// Dequeues a string from the queue
// with exclusion, first is waits if
// the queue is empty, then it will wait
// if the queue is in use by another thread
//
// Param
// 	q - pointer to the thread to be dequeued from
// Returns
// 	the value dequeued from the queue
char * DequeueString(Queue *q){
	//semaphores that allow for exclusion
	sem_wait(&q->waitIfEmpty);
	sem_wait(&q->queueReady);

	//vars used to get time diff
	struct timeval start, stop;
	double secs = 0;
	
	//gets start time
	gettimeofday(&start, NULL);

	//gets the val at head of queue
	char *q_string = q->strings[q->head];

	//sets the val take to NULL in queue
	q->strings[q->head] = NULL;  
	//change head reference to next in queue
	//with wrap around
	q->head = (q->head + 1) % q->maxSize;
	//inc/dec stats for queue
	q->currSize--;
	q->dequeueCount++;

	// get the stop time
	gettimeofday(&stop, NULL);
	// calc the difference
	secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
	q->dequeueTime += secs;

	sem_post(&q->queueReady);
	sem_post(&q->waitIfFull);

	return q_string;
}

//prints the stats of the queue
void PrintQueueStats(Queue *q){
	printf("enqueueCount = %d\n", q->enqueueCount);
	printf("dequeueCount = %d\n", q->dequeueCount);
	printf("enqueueBlockCount = %f\n", q->enqueueTime);
	printf("dequeueBlockCount = %f\n", q->dequeueTime);
}


