#include "schedule_visitor.h"

// given a node, print its type
void print_type(isl_schedule_node * node)
{
	enum isl_schedule_node_type ntype = isl_schedule_node_get_type( node);

	switch(ntype){
	case isl_schedule_node_band:
		printf("band type\n");
		break;
	case isl_schedule_node_context:
		printf("context type\n");
		break;
	case isl_schedule_node_domain:
		printf("domain type\n");
		break;
	case isl_schedule_node_expansion:
		printf("expansion type\n");
		break;
	case isl_schedule_node_extension:
		printf("extension type\n");
		break;
	case isl_schedule_node_filter:
		printf("filter type\n");
		break;
	case isl_schedule_node_leaf:
		printf("leaf type\n");
		break;
	case isl_schedule_node_guard:
		printf("guard type\n");
		break;
	case isl_schedule_node_mark:
		printf("mark type\n");
		break;
	case isl_schedule_node_sequence:
		printf("sequence type\n");
		break;
	case isl_schedule_node_set:
		printf("set type\n");
		break;
	default:
	break;
	}
}



//callback for map_descendant_bottom_up
// Call node_band_tile each time a band_node 
// is visited
isl_schedule_node * node_tiler(isl_schedule_node * node, void * user)
{
	enum isl_schedule_node_type ntype = isl_schedule_node_get_type( node);
	print_type(node);
	arg_t * args = (arg_t *) user;
	isl_schedule_node * new_node = NULL;
	// value to be returned
	isl_schedule_node * return_node = NULL;

	switch(ntype){
	case isl_schedule_node_band:
		
		printf("Look at me, I'm a band node!!!\n");
		if ( isl_schedule_node_has_children(node) == isl_bool_true)
		{
			isl_schedule_node * tiled_node;
			printf("Parsing and tiling a band\n");
			isl_multi_val *tile_sizes = NULL;
			tile_sizes = isl_multi_val_add_val( tile_sizes, isl_val_int_from_si(args->ctx, 32 ));
			isl_schedule_node * pnode = isl_schedule_node_copy(node);

			// This tile immediately segfaults...
			tiled_node = isl_schedule_node_band_tile(node, tile_sizes);
			printf("Printing new tiled node\n");
			wrap_isl_printer(ctx, (void * ) tiled_node, SCHEDULE_NODE);
			isl_printer *isprint = isl_printer_to_file(args->ctx, stdout);
			isprint = isl_printer_print_schedule_node(isprint,  tiled_node );
			isl_printer_free(isprint);
			new_node = pnode;
			
			printf("and I have children\n");
		}
		
	case isl_schedule_node_leaf:
			printf("printing leaf\n");
			wrap_isl_printer(args->ctx, (void * ) node, SCHEDULE_NODE);
	default:
	break;
	}
	if (new_node != NULL)
	{
		//node = new_node;
		
		if (isl_schedule_node_has_parent(new_node))
				return_node = isl_schedule_node_parent(new_node);
		else
			return_node = new_node;
		//return_node = isl_schedule_node_root(new_node);
	}
	else if(node != NULL)
	{
		
		if (isl_schedule_node_has_parent(node))
			return_node = isl_schedule_node_parent(node);
		else
			return_node = node;
		//return_node = isl_schedule_node_root(node);
	}
	return return_node;
	//return node;
}

// A function to be passed as callback to foreach_descendant
// Simply print the node passed as parameter
// user is a struct containing printer and ctx
isl_bool print_node(__isl_keep isl_schedule_node *node, void* user)
{
	arg_t * args = (arg_t *) user;
	print_type(node);
	wrap_isl_printer(args->ctx, (void * ) node, SCHEDULE_NODE);
	return isl_bool_true;
}

//A function for visiting a schedule tree, maybe modifying it
int schedule_visitor( isl_ctx * ctx,  __isl_keep isl_schedule * schedule, void * (*fn) (void *))
{
	isl_schedule_node * root = isl_schedule_get_root(schedule);
	wrap_isl_printer(ctx,  (void * ) root, SCHEDULE_NODE);
	//isl_schedule_node * new_root = schedule_tree_visitor(ctx,root);
	//if (new_root == NULL)
	//{
	//	printf("schedule tree visitor failed \n");
	//	return -1;
	//}
	//wrap_isl_printer(ctx, (void * ) new_root, SCHEDULE_NODE);
	arg_t args = {ctx};
	//This function is NOT supposed to change the state of the schedule node
	//It is better to use map_descendant_bottom_up which seems better suited
	//for that
	//isl_schedule_node_foreach_descendant_top_down(root, node_tiler,  (void * )&args );
	isl_schedule_node *new_root = isl_schedule_node_map_descendant_bottom_up(root, node_tiler,  (void * )&args );
	if (new_root == NULL)
	{
		printf("schedule tree visitor failed \n");
		return -1;
	}
	printf("\n\nNew schedule \n\n");
	wrap_isl_printer(ctx, (void * ) new_root, SCHEDULE_NODE);
	//isl_schedule_foreach_schedule_node_top_down(schedule, print_node,  (void * )&args );
	isl_schedule_node_free(new_root);
	return 0;
}


// Take a node as argument and visit
// each descendant manually (probably a bad idea,
// better use some pre-defined functions)
// This function segfaults at this point, do not use it
__isl_give isl_schedule_node* schedule_tree_visitor(isl_ctx *ctx,  __isl_take isl_schedule_node * node  )
{
	enum isl_schedule_node_type ntype = isl_schedule_node_get_type( node);
	print_type(node);
	if (node == NULL)
	{
		printf("Node is NULL\n");
		return NULL;
	}
	isl_schedule_node * return_node  = NULL;
	switch(ntype)
	{
case isl_schedule_node_filter :
	if ( isl_schedule_node_has_children(node))
	{
		printf("node has children\n");
		//isl_schedule_node * pnode = isl_schedule_node_copy(node);
		//isl_schedule_node * cnode = isl_schedule_node_first_child(node);
		isl_schedule_node * cnode = isl_schedule_node_get_child(node, 0);
		//node = pnode;
		isl_id *mark = isl_id_alloc(ctx, "A mark", NULL);
		isl_schedule_node * new_node = isl_schedule_node_insert_mark( cnode, mark);
		//wrap_isl_printer(ctx, (void * ) new_node, SCHEDULE_NODE);
		return_node =  schedule_tree_visitor(ctx, new_node);

	}
	break;
case isl_schedule_node_band :
	if ( isl_schedule_node_has_children(node) == isl_bool_true)
	{
		printf("node has children\n");
		isl_multi_val *tile_sizes = NULL;
		tile_sizes = isl_multi_val_add_val( tile_sizes, isl_val_int_from_si(ctx, 32 ));
		isl_schedule_node * new_node = isl_schedule_node_band_tile(node, tile_sizes);
		return_node =  schedule_tree_visitor(ctx,new_node);

	}
case isl_schedule_node_context :
case isl_schedule_node_domain :
	if ( isl_schedule_node_has_children(node) == isl_bool_true)
	{
		printf("node has children\n");
		//isl_schedule_node * pnode = isl_schedule_node_copy(node);
		//isl_schedule_node * cnode = isl_schedule_node_first_child(node);
		isl_schedule_node * cnode = isl_schedule_node_get_child(node, 0);
		//wrap_isl_printer(ctx, (void * ) cnode, SCHEDULE_NODE);
		//node = pnode;
		return_node = (cnode != NULL)? schedule_tree_visitor(ctx,  cnode):NULL;

	}
	break;
case isl_schedule_node_leaf :
	return_node = node;
case isl_schedule_node_sequence : 
	// apparently, you just can't declare a variable after a label
	;
	//get the number of children of this node 
	int nchild;
	nchild = isl_schedule_node_n_children(node);
	isl_schedule_node ** children = (isl_schedule_node **) malloc (sizeof( intptr_t) * nchild);
	for (int i =0; i < nchild; i++)
	{
		//copying the parent node and getting children
		isl_schedule_node * pnode = isl_schedule_node_copy(node);
		//warning : This frees the node pointer !!!!
		children[i] = isl_schedule_node_child(node, i);
		children[i] = schedule_tree_visitor(ctx,  children[i]);
		printf("print node in child\n");
		//wrap_isl_printer(ctx, (void * ) children[i], SCHEDULE_NODE);
		node = pnode;
	}
	return_node = node;
	break;
default :
	break;
	}
	if (return_node == NULL)
		return NULL;
	else
		return return_node;
}
