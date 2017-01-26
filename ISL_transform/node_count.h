#ifndef __COUNT_NODE__
#define __COUNT_NODE__


#include <pthread.h>
#include "transform.h"

#undef __MULTITHREADS__

typedef struct count_arg 
{
	isl_schedule_node * node;
	int * nb_band;
	pthread_mutex_t * lock;
} count_arg_t;


int count_band_nodes(__isl_take isl_schedule_node * node);
void* count_callback(void * arg);
void count_band_node_rec( __isl_keep isl_schedule_node * node, int * nb_band, pthread_mutex_t * lock);

#endif //__COUNT_NODE__
