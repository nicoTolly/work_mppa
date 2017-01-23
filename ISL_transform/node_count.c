
#include "node_count.h"


int count_band_nodes(__isl_take isl_schedule_node * node)
{
	int depth;
	int nb_band = 0;
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	depth = isl_schedule_node_get_tree_depth(node);

	count_band_node_rec(node, &nb_band, &lock);
	return nb_band;
	

}

// Callback for multithreads calling
void* count_callback(void * arg)
{
	count_arg_t * args = (count_arg_t *) arg;
	count_band_node_rec(args->node, args->nb_band, args->mut);
	return NULL;
}


// recursive function that traverse node tree
// to count band nodes
void count_band_node_rec( __isl_keep isl_schedule_node * node, int * nb_band, pthread_mutex_t * lock)
{
	if (node == NULL)
		return;
	if (isl_schedule_node_get_type( node) == isl_schedule_node_band)
	{
		pthread_mutex_lock(lock);
		*nb_band+=1;
		pthread_mutex_unlock(lock);
	}
		int nb_children = isl_schedule_node_n_children(node);
		switch (nb_children)
		{
			case 0:
				return ;
			case 1:
				;
				isl_schedule_node * cnode = isl_schedule_node_get_child(node, 0);
				count_band_node_rec(cnode, nb_band, lock);
				//isl_schedule_node_free(cnode);
				return ;
			default:
				if (nb_children < 0)
				{
					printf("got %d children\n", nb_children);
					perror("got a negative number of children\n");
					exit(-1);
				}
				pthread_t * tthread = malloc(sizeof(pthread_t) * nb_children);
				count_arg_t *args = (count_arg_t *) malloc(sizeof(intptr_t) * nb_children);
				if (args == NULL)
				{
					printf("got a NULL args\n");
					exit(-1);
				}
				for (int i= 0; i < nb_children; i++)
				{
					args[i] = (count_arg_t) { node, nb_band, lock };
					//tthread[i] = pthread_create(&tthread[i], NULL, count_callback, (void*) &args[i]);	
					isl_schedule_node * cnode = isl_schedule_node_get_child(node, i);
					count_band_node_rec(cnode, nb_band, lock);
					//isl_schedule_node_free(cnode);
				}
		}
}


