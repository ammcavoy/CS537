#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


void printStats(void *queues){
	Queue **queue = (Queue **) queues;
	printf("\nQueue Stats:\n");
	printf("Queue1:\n");
	PrintQueueStats(queue[0]);
	printf("Queue2:\n");
        PrintQueueStats(queue[1]);
	printf("Queue3:\n");
        PrintQueueStats(queue[2]);
}
