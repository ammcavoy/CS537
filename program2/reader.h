#ifndef __READER_H__
#define __READER_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "queue.h"


void *reader(void *q1);

#endif
