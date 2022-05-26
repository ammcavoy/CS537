#ifndef __WRITER_H__
#define __WRITER_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "queue.h"

void *writer(void *q3);
 
#endif
