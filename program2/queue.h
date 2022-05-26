#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>


typedef struct Queue{
	char **strings;
	int maxSize;
	int currSize;
    int enqueueCount;
    int dequeueCount;
    double enqueueTime; //idk about the var type
    double dequeueTime; //same concern
    int head;
    int tail;
	sem_t waitIfFull;
	sem_t waitIfEmpty;
	sem_t queueReady;
}Queue;


Queue *CreateStringQueue(int size);


void EnqueueString(Queue *q, char *string);


char * DequeueString(Queue *q);


void PrintQueueStats(Queue *q);

#endif
